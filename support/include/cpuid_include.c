/*
 * c_include.c
 *
 *  Created on: 26 Sep 2022
 *      Author: PeterB
 *
 *
 *      !!!NOTE THIS FILE MUST ONLY BE INCLUDED WHEN REQUIRED!!!
 *
 *      it should be disabled in the general build and included in the required
 *      source files in the project so the correct CPU ID is automatically selected
 *
 *
 */
#include "vga.h"
#include "xtime_l.h"
#include "xil_mmu.h"
#include "support.h"
#include "xparameters.h"

/* Externals here */
extern shared_memp_t 		shmem_p;					/* Memory shared between CPUs    */
extern XScuGic 				xInterruptController; 	/* Interrupt controller instance */

void init_shared_mem( uint8_t init_video, uint8_t init_ocm, uint8_t init_ddr ) {

	uint32_t	addr;

	if( init_video ) {		// CPU Core 0 sets up Video buffer else CPU Core 1 sets Beacon Rx Buffer
		/* Set Write Through Caching for Video Frame Memory Buffer Area Has to be done 1MByte at a time */
		/* This type of caching appears to work well with DMA and gives a 4 fold increase in throughput over non-cached!*/
		for( addr = VGA_DDR_DMA_BASE; addr < (VGA_DDR_DMA_BASE + VGA_DDR_DMA_SIZE); addr += 0x100000 ) {
			Xil_SetTlbAttributes(addr, NORM_WT_CACHE);
		}
	}

	if( init_ocm ) Xil_SetTlbAttributes(SHARED_OCM_MEMORY_BASE, NORM_NONCACHE);	// Turn off cache for on chip shared memory
	// Set no cache for shared DDR memory
	if( init_ddr ) {
		for( addr = SHARED_DDR_MEMORY_BASE; addr < (SHARED_DDR_MEMORY_BASE + 0x1000000); addr += 0x100000 ) {
			Xil_SetTlbAttributes(addr, NORM_NONCACHE);
		}
	}
	return;
}

void vApplicationTickHook (void) {

  static int tick = 0;

  if(tick++ == CALCULATION_PERIOD) {
    tick = 0;
    if( shmem_p->cpud[XPAR_CPU_ID].TotalIdleTime > CALCULATION_PERIOD)
    	shmem_p->cpud[XPAR_CPU_ID].TotalIdleTime = CALCULATION_PERIOD;
    shmem_p->cpud[XPAR_CPU_ID].Usage = (100 - (shmem_p->cpud[XPAR_CPU_ID].TotalIdleTime * 100) / CALCULATION_PERIOD);
    shmem_p->cpud[XPAR_CPU_ID].TotalIdleTime = 0;
  }
#if XPAR_CPU_ID == 0
  lv_tick_inc( 1000/configTICK_RATE_HZ );		// We are piggy backing off OS timer for GUI saves having a separate task etc...
#endif
}

void StartIdleMon (void) {
  if( xTaskGetCurrentTaskHandle() == shmem_p->cpud[XPAR_CPU_ID].IdleHandle ) {
	  shmem_p->cpud[XPAR_CPU_ID].IdleStartTime = xTaskGetTickCountFromISR();
  }
}

void EndIdleMon (void) {
  if( xTaskGetCurrentTaskHandle() == shmem_p->cpud[XPAR_CPU_ID].IdleHandle ) {
	shmem_p->cpud[XPAR_CPU_ID].IdleSpentTime = xTaskGetTickCountFromISR() - shmem_p->cpud[XPAR_CPU_ID].IdleStartTime;
	shmem_p->cpud[XPAR_CPU_ID].TotalIdleTime += shmem_p->cpud[XPAR_CPU_ID].IdleSpentTime;
  }
}

int _gettimeofday( struct timeval *tv, void *tzvp ) {

    XTime t;

    XTime_GetTime(&t);								//__your_system_time_function_here__() this gets up time in nanoseconds
    tv->tv_sec = (t*3) / 1000000000;  				// convert to seconds
    tv->tv_usec = ( (t*3) % 1000000000 ) / 1000;	// get remaining microseconds
    return 0;  // return non-zero for error
}

int _settimeofday( struct timeval *tv, void *tzvp ) {

	XTime t;

    t = ((XTime)tv->tv_sec * 1000000000) / 3;		// convert to nsec and / 3 for tick conversion
    t += ((XTime)tv->tv_usec * 1000) /3;			// convert microseconds to nsec and / 3 for tick conversion
	XTime_SetTime(t);
	return 0;

}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ) {
	/* Attempt to prevent the handle and name of the task that overflowed its stack
	from being optimised away because they are not used. */
	volatile TaskHandle_t xOverflowingTaskHandle = xTask;
	volatile char *pcOverflowingTaskName = pcTaskName;

	( void ) xOverflowingTaskHandle;
	( void ) pcOverflowingTaskName;

	xil_printf( "HALT: Task %s overflowed its stack.\r\n", pcOverflowingTaskName );
	portDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationMallocFailedHook( void ) {
	xil_printf( "vApplicationMallocFailedHook() called\r\n" );
}

void vAssertCalled( const char *pcFileName, uint32_t ulLine ) {
	volatile uint32_t ul = 0;
	volatile const char *pcLocalFileName = pcFileName; /* To prevent pcFileName being optimized away. */
	volatile uint32_t ulLocalLine = ulLine; /* To prevent ulLine being optimized away. */

	/* Prevent compile warnings about the following two variables being set but
	not referenced.  They are intended for viewing in the debugger. */
	( void ) pcLocalFileName;
	( void ) ulLocalLine;

	xil_printf( "Assert failed in file %s, line %lu\r\n", pcLocalFileName, ulLocalLine );

	/* If this function is entered then a call to configASSERT() failed in the
	FreeRTOS code because of a fatal error.  The pcFileName and ulLine
	parameters hold the file name and line number in that file of the assert
	that failed.  Additionally, if using the debugger, the function call stack
	can be viewed to find which line failed its configASSERT() test.  Finally,
	the debugger can be used to set ul to a non-zero value, then step out of
	this function to find where the assert function was entered. */
	taskENTER_CRITICAL();
	{
		while( ul == 0 )
		{
			__asm volatile( "NOP" );
		}
	}
	taskEXIT_CRITICAL();
}

void prvSetupHardware( void ) {

	BaseType_t xStatus;
	XScuGic_Config *pxGICConfig;

	/* Ensure no interrupts execute while the scheduler is in a inconsistent
	state.  Interrupts are automatically enabled when the scheduler is
	started. */
	portDISABLE_INTERRUPTS();

	/* Obtain the configuration of the GIC. */
	pxGICConfig = XScuGic_LookupConfig( XPAR_SCUGIC_SINGLE_DEVICE_ID );

	/* Sanity check the FreeRTOSConfig.h settings are correct for the
	hardware. */
	configASSERT( pxGICConfig );
	configASSERT( pxGICConfig->CpuBaseAddress == ( configINTERRUPT_CONTROLLER_BASE_ADDRESS + configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET ) );
	configASSERT( pxGICConfig->DistBaseAddress == configINTERRUPT_CONTROLLER_BASE_ADDRESS );

	/* Install a default handler for each GIC interrupt. */
	xStatus = XScuGic_CfgInitialize( &xInterruptController, pxGICConfig, pxGICConfig->CpuBaseAddress );
	configASSERT( xStatus == XST_SUCCESS );
	( void ) xStatus; /* Remove compiler warning if configASSERT() is not defined. */

	/* The Xilinx projects use a BSP that does not allow the start up code to be
	altered easily.  Therefore the vector table used by FreeRTOS is defined in
	FreeRTOS_asm_vectors.S, which is part of this project.  Switch to use the
	FreeRTOS vector table. */
	vPortInstallFreeRTOSVectorTable();
}

