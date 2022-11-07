/*
 * support.c
 *
 *  Created on: 26 Sep 2022
 *      Author: PeterB
 */
#include "support.h"

#include "gui.h"
#include "ntp.h"
#include "vga.h"
#include "tusb.h"
#include "sysmsg.h"
#include "xscugic.h"
#include "ff_stdio.h"
#include "FreeRTOS.h"
#include "ff_sddisk.h"
#include "ff_headers.h"
#include "pjbes_ftpd.h"
#include "pjbes_crc32.h"
#include "xparameters.h"
#include "xil_misc_psreset_api.h"

/* Externals here */
extern shared_memp_t 		shmem_p;					/* Memory shared between CPUs    */
extern XScuGic 				xInterruptController; 	/* Interrupt controller instance */
extern cpu0_globals_t		*cpu0_globals;				/* A global structure which contains all necessary global variables */

/* Globals here */
volatile uint32_t			*Inter_CPU_IRQ_reg = (void*)XPAR_ZEDBOARD_INTERCPU_IRQ_0_BASEADDR;

uint8_t q_spawn_req( sysmsg_q_t *msg ) {

	return xQueueSend( cpu0_globals->spawn_q, msg, 0 );
}

uint8_t q_sys_action( sysmsg_q_t *msg ) {

	return xQueueSend( cpu0_globals->sys_action_q, msg, 0 );
}

void task_spawner( void *p ) {

	static uint8_t		is_task = pdTRUE;
	uint16_t 			msg;

	while( 1 ) {
		if( ( xQueueReceive( cpu0_globals->spawn_q, &msg, portMAX_DELAY ) ) ) {
			switch( msg ) {

			case SPAWN_SMSGP:
			    xTaskCreate( (TaskFunction_t)sysmsg_task, "System Messages", THREAD_STACKSIZE, NULL, SYSMSGPROC_PRIORITY, NULL );
			    break;

			case SPAWN_SUPV0:
			    xTaskCreate( (TaskFunction_t)sys_mngr0, "System Manager", THREAD_STACKSIZE, NULL, SVISOR_PRIORITY, NULL );
			    break;

			case SPAWN_NTPS:
				xTaskCreate( (TaskFunction_t)ntp_server, "NTP Server", THREAD_STACKSIZE, NULL, DEFAULT_THREAD_PRIO, NULL );
				break;

			case SPAWN_NTPC:
				xTaskCreate( (TaskFunction_t)set_time_from_NTP_task, "NTP Client", THREAD_STACKSIZE, (void*)&is_task, DEFAULT_THREAD_PRIO, NULL );
				break;

			case SPAWN_GUI:
				xTaskCreate( (TaskFunction_t)gui_thread, "GUI Scheduler", THREAD_STACKSIZE, NULL, GUI_PRIORITY, NULL );
				break;

			case SPAWN_USB:
				xTaskCreate( (TaskFunction_t)tuh_task, "USB Driver", THREAD_STACKSIZE, NULL, USB_PRIORITY, NULL );
				break;

			case SPAWN_FTP:
				xTaskCreate( (TaskFunction_t)ftp_server, "FTP Server", THREAD_STACKSIZE,  NULL, FTP_PRIORITY, NULL );
				break;

			default:
				break;
			}
		}
	}
}


/* Set default configuration */
void set_default_config( config_datp_t confp ) {

	uint8_t		def_MAC[] = { 0x00, 0x0A, 0x35, 0x00, 0x01, 0x22 };		/* This appears to be widely used for Xilinx & Zedboard? */
																		/* Personally I would sort out my own MAC... */

	memset( confp, 0, sizeof( config_dat_t ) );

	strcpy( confp->Sys_Name, "Zedboard VGA Template System");
	confp->Sys_IP = FreeRTOS_inet_addr_quick(192, 168, 1, 77);
	confp->Sys_nm = FreeRTOS_inet_addr_quick(255, 255, 255, 0);
	confp->Sys_gw = FreeRTOS_inet_addr_quick(192, 168, 1, 1);
	confp->Sys_DNS = FreeRTOS_inet_addr_quick(8, 8, 8, 8);			// DNS server host address - this is a google server...
	confp->Sys_NTP = FreeRTOS_inet_addr_quick(216, 239, 35, 4);		// NTP server host address - this is also a google server...
	hash_password( "Hello", &confp->pword[0] );
	memcpy(confp->softmac_addr, def_MAC, sizeof(confp->softmac_addr) );
	confp->gui_colour = lv_palette_main(LV_PALETTE_PURPLE);
	confp->gui_theme = GUI_LITE;
	confp->Sys_NTP_update = 12.0;
	/* Always update checksum last! */
	confp->csum = mem_chk_sum( (unsigned char*)confp, (sizeof( config_dat_t ) - sizeof(uint64_t)) );
}

/* Load a configuration file from SD card */
uint8_t	load_config( config_datp_t confp ) {

	FF_FILE			*file;
	unsigned int	rdwr, t_rdwr;
	uint64_t		csum;
	uint8_t			ret_val = CLOAD_OK;

	if( (rdwr = ff_chdir( SYS_PATHDIR )) != 0 ) {						// Check our directory exists
		rdwr = ff_mkdir( SYS_PATHDIR );									// If not create it!
		ret_val |= CLOAD_NODIR;
	}
	if( ((file = ff_fopen( CONFIG_PATH, "r" )) != NULL)  ) { 			// Normal configuration file open...
		if((t_rdwr = ff_filelength(file)) == sizeof(config_dat_t)) {
			t_rdwr = 0;
			rdwr = 1;
			while( t_rdwr < rdwr ) {
				rdwr = ff_fread( ((uint8_t*)confp) + t_rdwr, 1, (sizeof(config_dat_t) - t_rdwr), file );
				t_rdwr += rdwr;
			}
			rdwr = ff_fclose( file );
			csum = mem_chk_sum( (unsigned char*)confp, (sizeof( config_dat_t ) - sizeof(uint64_t)) );
			if( csum != confp->csum ) { 									// Check for checksum error
				rdwr = ff_fclose( file );
				ret_val |= CLOAD_BAD_CSUM;
			}
		} else { 															// File exists but is wrong size
			ff_fclose( file );
			ret_val |= CLOAD_BAD_LEN;
		}
	} else {																// No configuration file
		ret_val |= CLOAD_NOFILE;
	}
	if( ret_val ) {															// If any bad flags create a new default configuration file!
		set_default_config( confp );
		file = ff_fopen( CONFIG_PATH, "w" );
		t_rdwr = 0;
		rdwr = 1;
		while( t_rdwr < rdwr ) {
			rdwr = ff_fwrite( ((uint8_t*)confp) + t_rdwr, 1, (sizeof(config_dat_t) - t_rdwr), file );
			t_rdwr += rdwr;
		}
		rdwr = ff_fclose( file );

	}
	return ret_val;
}

/* Save a configuration file to SD Card */
int save_config( config_datp_t confp ) {

	FF_FILE			*file;
	unsigned int	rdwr, t_rdwr;

	ff_rename( CONFIG_PATH, BCONFIG_PATH, pdFALSE );
	file = ff_fopen( CONFIG_PATH, "w" );
	if( file == NULL ) return -1;
	confp->csum = mem_chk_sum( (unsigned char*)confp, (sizeof( config_dat_t ) - sizeof(uint64_t)) ); // Update the checksum before saving...
	t_rdwr = 0;
	rdwr = 1;
	while( t_rdwr < rdwr ) {
		rdwr = ff_fwrite( ((uint8_t*)confp) + t_rdwr, 1, (sizeof(config_dat_t) - t_rdwr), file );
		t_rdwr += rdwr;
	}
	rdwr = ff_fclose( file );
	ff_remove( BCONFIG_PATH );
	if( rdwr != 0 ) return -1; else return 0;
}

/* functions to make reentrancy work properly with newlib for FreeRTOS */
void __malloc_lock(struct _reent *REENT) {

    vTaskSuspendAll();
}

void __malloc_unlock(struct _reent *REENT) {

    xTaskResumeAll();
}

void reset_usb_phy( void ) {
	*Inter_CPU_IRQ_reg &= ~USB_RESET;
	vTaskDelay(pdMS_TO_TICKS(1));
	*Inter_CPU_IRQ_reg |= USB_RESET;
}


uint16_t GetCPUUsage ( uint8_t core ) {
  return (uint16_t)shmem_p->cpud[core].Usage;
}

void system_reboot() {
	uint32_t	reboot;

	/* Unlock the slcr register access lock */
	Xil_Out32(XSLCR_UNLOCK_ADDR, XSLCR_UNLOCK_CODE);
	reboot = Xil_In32(REBOOT_STATUS_REG);
	Xil_Out32(reboot & 0xF0FFFFFF, REBOOT_STATUS_REG);
	Xil_Out32(PS_RST_CTRL_REG, PS_RST_MASK);
}

void usb_CTRL_reset( uint8_t port ) {

	uint32_t	reset;

	/* Unlock the slcr register access lock */
	Xil_Out32(XSLCR_UNLOCK_ADDR, XSLCR_UNLOCK_CODE);

	if( !port ) reset = PS_USB0_RST;
	else reset = PS_USB1_RST;

	Xil_Out32(PS_USB_RST_CTRL_REG, reset);
	vTaskDelay( 1 );
	Xil_Out32(PS_USB_RST_CTRL_REG, 0);

}

void hash_password( char *password, char *hashed_password ) {		/* Very very very simple hash.
																	 * Implement what ever is appropriate for your system here.
																	 */
	int		pass_len;
	char	temp_buf[MAX_NAME_LEN];

	pass_len = strlen( password );

	strcpy( temp_buf, password );
	for( int hash_loop = 0; hash_loop < pass_len; hash_loop++ ) {
		temp_buf[hash_loop] = temp_buf[hash_loop] + 20;
	}
	strcpy( hashed_password, temp_buf );
	return;
}

void unhash_password( char *hashed_password, char *password ) {

	int		pass_len;
	char	temp_buf[MAX_NAME_LEN];

	pass_len = strlen( password );

	strcpy( temp_buf, hashed_password );
	for( int hash_loop = 0; hash_loop < pass_len; hash_loop++ ) {
		temp_buf[hash_loop] = temp_buf[hash_loop] - 20;
	}
	strcpy( password, temp_buf );
	return;
}

void InterCpuIRQ( void ) {			/* Implement this to interrupt core 0 from core 1 */

}

