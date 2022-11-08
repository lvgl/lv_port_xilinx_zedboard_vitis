/*
 * main.c
 *
 *  Created on: 26 Sep 2022
 *      Author: PeterB
 */
#include "gui.h"
#include "NTP.h"
#include "tusb.h"
#include "sysmsg.h"
#include "ff_headers.h"
#include "ff_sddisk.h"
#include "support.h"

#include "cpuid_include.c"

/* Globals here */
shared_memp_t 			shmem_p = (shared_memp_t)SHARED_DDR_MEMORY_BASE;	/* Used store information available to both CPUs */
config_datp_t			conf_p = (config_datp_t)(SHARED_DDR_MEMORY_BASE + sizeof(shared_mem_t)); /* Used to store system configuration */
cpu0_globals_t			*cpu0_globals;				/* A global structure which contains all necessary global variables */
XScuGic					xInterruptController; 	/* Interrupt controller instance */
FF_Disk_t				*pxSDDisk;

static void startup_thread( void *p );	/* Thread is used to initialise RTOS etc. */

int main() {

	init_shared_mem( pdTRUE, pdTRUE, pdTRUE );
	prvSetupHardware();
	xTaskCreate( (TaskFunction_t)startup_thread, "STARTUP_THRD", THREAD_STACKSIZE, NULL, DEFAULT_THREAD_PRIO, NULL );
	vTaskStartScheduler();

	for(;;);	// This line should never execute as long as FreeRTOS is working.
}

#define CPU1_WAIT	3000  	/* This gives up to 30 Seconds for CPU core 1 to wake up (mainly for debug startup as normally would take no time at all!) */
#define NET_WAIT	1000 	/* Wait up to 10 seconds for Network interface to come up */

static void startup_thread( void *p ) {

	uint8_t				load_val;
	uint16_t			retry = 0;
	sysmsg_q_t			msg = { 0, pdFALSE, NULL };

	xil_printf( "CPU Core 0 Started...\r\n" );
    memset( shmem_p, 0, sizeof(shared_mem_t) );
	shmem_p->cpud[0].Started_OK = pdTRUE;
	cpu0_globals = pvPortMalloc(sizeof(cpu0_globals_t));
	memset( (void*)cpu0_globals, 0, sizeof(cpu0_globals_t));
	cpu0_globals->sys_action_q = xQueueCreate( SYS_ACTION_Q_SIZE, sizeof( sysmsg_q_t ) );
	configASSERT( cpu0_globals->sys_action_q );
	cpu0_globals->spawn_q = xQueueCreate( SPAWN_Q_SIZE, sizeof( sysmsg_q_t ) );
	configASSERT( cpu0_globals->spawn_q );
	cpu0_globals->sysmsg_q = xQueueCreate( SYSMSG_Q_SIZE, sizeof( sysmsg_q_t ) );
	configASSERT( cpu0_globals->sysmsg_q	);
	cpu0_globals->gui.mse_dat.state_q = xQueueCreate( INDEV_Q_SIZE, sizeof( lv_indev_data_t ) );
	configASSERT( cpu0_globals->gui.mse_dat.state_q );
	cpu0_globals->gui.kb_dat.state_q = xQueueCreate( INDEV_Q_SIZE, sizeof( lv_indev_data_t ) );
	configASSERT( cpu0_globals->gui.kb_dat.state_q );
	cpu0_globals->gui.msg_q = xQueueCreate( GUIMSG_Q_SIZE, sizeof( sysmsg_q_t ) );
	configASSERT( cpu0_globals->gui.msg_q );
    msg.id = MSG_STARTUP;
    q_sysmsg( &msg );
	shmem_p->cpud[0].FreeHeap = xPortGetFreeHeapSize();
	shmem_p->cpud[0].IdleHandle = xTaskGetIdleTaskHandle();
    xTaskCreate( (TaskFunction_t)task_spawner, "Task Spawner", THREAD_STACKSIZE, NULL, SPWN_PRIORITY, NULL );
	int Status = XScuGic_Connect(&xInterruptController, XPS_IRQ_INT_ID, (Xil_ExceptionHandler)&InterCpuIRQ, NULL);
	if (Status != XST_SUCCESS) {
		// TODO: Some error report...
	}
	XScuGic_SetPriorityTriggerType(&xInterruptController, XPS_IRQ_INT_ID, 0xD8, 0x3);
	XScuGic_Enable(&xInterruptController, XPS_IRQ_INT_ID);

	msg.id = SPAWN_GUI;
	q_spawn_req( &msg );
	msg.id = SPAWN_SMSGP;
	q_spawn_req( &msg );

	while( uxQueueMessagesWaiting( cpu0_globals->spawn_q ) ) vTaskDelay(pdMS_TO_TICKS(10));
    pxSDDisk = FF_SDDiskInit( SD_CARD_ROOT );/* Start and mount the SD Card File system */
	if( pxSDDisk == NULL ) {
		xil_printf("Error: Failed to initialise SD card!\r\n");
	    msg.id = ERR_SDINIT_FAIL;
	} else {
	    msg.id = INF_SDINIT_OK;
		xil_printf("Info: SD card initialised okay!\r\n");
	}
    q_sysmsg( &msg );

	xil_printf( "Info: Loading configuration file: \r\n");
	msg.id = INF_LDCONF;
    q_sysmsg( &msg );
	load_val = load_config( conf_p );
	if( load_val == CLOAD_OK ) {
		msg.id = INF_LDCONF_OK;
		q_sysmsg( &msg );
	} else {
		if( load_val & CLOAD_NODIR ) {
			msg.id = ERR_LDCONF_NODIR;
			q_sysmsg( &msg );
		}
		if( load_val & CLOAD_NOFILE ) {
			msg.id = ERR_LDCONF_NOFILE;
			q_sysmsg( &msg );
		}
		if( load_val & CLOAD_BAD_CSUM ) {
			msg.id = ERR_LDCONF_BADCSUM;
			q_sysmsg( &msg );
		}
		if( load_val & CLOAD_BAD_LEN ) {
			msg.id = ERR_LDCONF_BADLEN;
			q_sysmsg( &msg );
		}
	}
	/* Start Second CPU core */
    xil_printf( "Starting CPU Core 1...\r\n");
    Xil_Out32(CPU1_START_ADR, CPU1_MEM_BASE);	// Tell CPU 1 where it's code is
    dmb();	// Wait until memory write has finished.
    sev();	// Kick off the second CPU core and it will set started ok flag then wait on hold flag
    retry = CPU1_WAIT;
    while( !shmem_p->cpud[1].Started_OK && --retry ) vTaskDelay(pdMS_TO_TICKS(10));
	if( !retry  ) {
		xil_printf( "Error: CPU Core 1 not responding!\r\n" );
		msg.id = CPU1_NOK;
	} else {
		xil_printf( "CPU Core 1 started okay.\r\n" );
		msg.id = CPU1_OK;
	}
	q_sysmsg( &msg );

	/* Initialise the network interface */
	FreeRTOS_IPInit( (uint8_t*)&conf_p->Sys_IP, (uint8_t*)&conf_p->Sys_nm, (uint8_t*)&conf_p->Sys_gw, (uint8_t*)&conf_p->Sys_DNS, conf_p->softmac_addr );
	retry = NET_WAIT;	/* Wait to see if network interface comes up */
	while((!( FreeRTOS_IsNetworkUp() && cpu0_globals->net_started && xGetPhyLinkStatus() )) && --retry ) vTaskDelay(pdMS_TO_TICKS(10));

	if( retry ) {
		/* If the network is up attempt to update local time from Internet */
		xil_printf( "Info: Get time from Internet if possible.\r\n");
		msg.id = INFO_GET_NET_TIME;
		q_sysmsg( &msg );
		uint8_t is_task = pdFALSE;
		set_time_from_NTP_task( &is_task );
	} else {
		xil_printf( "Error: Network did not come up with in 10 Seconds!\r\n");
	}

	tusb_init();
	msg.id = SPAWN_USB;
	q_spawn_req( &msg );

	msg.id = INF_LOADING_MAIN;
	q_sysmsg( &msg );
	while( uxQueueMessagesWaiting( cpu0_globals->sysmsg_q ) ) vTaskDelay(pdMS_TO_TICKS(10));

	msg.id = LOAD_MAIN_GUI;
	q_gui_msg( &msg );
	while( uxQueueMessagesWaiting( cpu0_globals->gui.msg_q ) ) vTaskDelay(pdMS_TO_TICKS(10));

	msg.id = SPAWN_SUPV0;
	q_spawn_req( &msg );
	vTaskDelete(NULL);
}

#define SYSMNGR_LOOP_TIME		50
#define HEAP_REFRESH_TIME		20	/* This is multiplied by SYSMNGR_LOOP_TIME to get time in ms */

void sys_mngr0( void *p ) {

	time_t				cntr = 1;				// Counter to MOD for various timings as required
	sysmsg_q_t			msg = { 0, pdFALSE, NULL };

	while(1) {
		// This parameter is in hours
		if( FreeRTOS_IsNetworkUp() && cpu0_globals->net_started && xGetPhyLinkStatus() ) {
			if( (!(cntr % (time_t)(conf_p->Sys_NTP_update * 3.6E6 / SYSMNGR_LOOP_TIME)) && conf_p->Sys_NTP_update) ) {	// If enabled update time periodically
				msg.id = SPAWN_NTPC;
				q_spawn_req( &msg );
			}
		}
		if( !(cntr % HEAP_REFRESH_TIME) ) {
			shmem_p->cpud[0].FreeHeap = xPortGetFreeHeapSize();			/* Refresh heap value */
		}
		if( ( xQueueReceive( cpu0_globals->sys_action_q, &msg, 0 ) ) ) {	/* Do a non-blocking check of queue */
			switch(msg.id) {

				case SAVE_CONFIG:
					if( !save_config( conf_p ) ) {
						msg.id = INF_SAVE_CONFIG_OK;
					} else {
						msg.id = ERR_SAVE_CONFIG;
					}
					break;

				case SYS_REBOOT:
					msg.id = WARN_USR_REBOOT;
					q_sysmsg( &msg );
					vTaskDelay(pdMS_TO_TICKS(1000));
					msg.id = WARN_REBOOT_INPROG;
					msg.extra_data = NULL;
					q_sysmsg( &msg );
					vTaskDelay(pdMS_TO_TICKS(4000));
					system_reboot();
					break;

				case CLR_LOG_REQ:
					msg.id = LOG_CLEAR_CMD;
    				break;

				default:
					break;
			}
			q_sysmsg( &msg );
		}

		vTaskDelay(pdMS_TO_TICKS(SYSMNGR_LOOP_TIME));
		cntr += 1;
	}
}
