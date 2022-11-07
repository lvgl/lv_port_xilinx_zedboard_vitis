/*
 * main.c
 *
 *  Created on: 27 Sep 2022
 *      Author: PeterB
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "xil_printf.h"
#include "xparameters.h"
#include "xil_io.h"
#include "xscuwdt.h"

#include "support.h"
#include "cpuid_include.c"
#include "ZedboardOLED.h"

shared_memp_t 			shmem_p = (shared_memp_t)SHARED_DDR_MEMORY_BASE;
config_datp_t			conf_p = (config_datp_t)(SHARED_DDR_MEMORY_BASE + sizeof(shared_mem_t));
XScuGic 				xInterruptController; 	/* Interrupt controller instance */

static void sys_mngr1( void *p );

int main( void ) {

	prvSetupHardware();
	init_shared_mem( pdFALSE, pdTRUE, pdTRUE );
	xTaskCreate( (TaskFunction_t)sys_mngr1, "Supervisor Core 1", THREAD_STACKSIZE, NULL, DEFAULT_THREAD_PRIO , NULL );
	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	for( ;; ); /* This line should never execute as long as FreeRTOS is working. */
}

static void sys_mngr1( void *p ) {

	size_t				previous[2] = { 0, 0 }, current[2] = { 0, 0 };
	char				buf[16];
	TaskStatus_t		ProcessStatusOS[MAX_TASKS];

	shmem_p->cpud[1].Started_OK = pdTRUE;
	while( !(shmem_p->cpu_1_flags & CPU1_PROCEED) ) vTaskDelay(10);		// Hold CPU1 until CPU0 wants it to proceed.
	shmem_p->cpud[1].IdleHandle = xTaskGetIdleTaskHandle();

	while(1) {
		shmem_p->cpud[1].FreeHeap = current[1] = xPortGetFreeHeapSize();
		current[0] = shmem_p->cpud[0].FreeHeap;
		if( (previous[0] != current[0]) || (previous[1] != current[1]) ) {
			OLED_clear();
			OLED_print_message("Free Heap CPU0:" , 0);
			sprintf( buf, "%d", current[0] );
			OLED_print_message(buf , 1);
			previous[0] = current[0];
			vTaskDelay( pdMS_TO_TICKS(100) );
			OLED_print_message("Free Heap CPU1:" , 2);
			sprintf( buf, "%d", current[1] );
			OLED_print_message(buf , 3);
			previous[1] = current[1];
			vTaskDelay( pdMS_TO_TICKS(100) );
		}
		shmem_p->cpud[1].task_cnt = uxTaskGetNumberOfTasks();			// Load Task List into shared memory
		uxTaskGetSystemState( ProcessStatusOS, shmem_p->cpud[1].task_cnt, NULL );
        for( int i = 0; i < shmem_p->cpud[1].task_cnt; i++ ) {
        	strcpy( shmem_p->cpud[1].ProcessStatus[i].pcTaskName, ProcessStatusOS[i].pcTaskName );
        	shmem_p->cpud[1].ProcessStatus[i].CurrentState = ProcessStatusOS[i].eCurrentState;
        	shmem_p->cpud[1].ProcessStatus[i].CurrentPriority = ProcessStatusOS[i].uxCurrentPriority;
        	shmem_p->cpud[1].ProcessStatus[i].StackHighWaterMark = ProcessStatusOS[i].usStackHighWaterMark;
        }
		vTaskDelay( pdMS_TO_TICKS(100) );
	}
}

