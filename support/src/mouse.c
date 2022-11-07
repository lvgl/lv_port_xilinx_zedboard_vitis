/*
 * mouse_emu.c
 *
 *  Created on: 14 Jan 2019
 *      Author: peterb
 */
#include "mouse.h"
#include "FreeRTOS.h"
#include "support.h"
#include "xscugic.h"
#include "xgpio.h"
#include "vga.h"

#ifdef MOUSE_EMU
void mouse_emu_Handler(void *CallbackRef);
extern XScuGic 				xInterruptController;
XGpio 						Gpio; 						/* The Instance of the GPIO Driver */
#endif

extern cpu0_globals_t		*cpu0_globals;
LV_IMG_DECLARE(mouse_cursor_icon);			/*Declare the image file.*/
//#define _DEBUG_INDEV_TOUCH_MOUSE_

static void mouse_read(struct _lv_indev_drv_t * indev_drv, lv_indev_data_t * data) {

	static lv_indev_data_t		event_d_q;

	xQueueReceive( cpu0_globals->gui.mse_dat.state_q, &event_d_q, 0 );
	memcpy( data, &event_d_q, sizeof(lv_indev_data_t) );
	data->continue_reading = uxQueueMessagesWaiting( cpu0_globals->gui.mse_dat.state_q ) ? pdTRUE : pdFALSE;
#ifdef _DEBUG_INDEV_TOUCH_MOUSE_
    xil_printf( "INDEV: %ld, %ld, %s\r\n", data->point.x, data->point.y, data->state ? "PR" : "RL" ) ;
#endif
    return;
}


void mouse_init( void ) {

    lv_indev_drv_init(&cpu0_globals->gui.mse_dat.indev_drv);	/*Basic initialisation of pointer interface for GUI is attached to USB Mouse/Touch and VNC*/
    cpu0_globals->gui.mse_dat.indev_drv.type = LV_INDEV_TYPE_POINTER;
    cpu0_globals->gui.mse_dat.indev_drv.read_cb = &mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    cpu0_globals->gui.mse_dat.indev_drv.user_data = NULL;
    cpu0_globals->gui.mse_dat.indev = lv_indev_drv_register(&cpu0_globals->gui.mse_dat.indev_drv);
	cpu0_globals->gui.mse_dat.cursor_obj =  lv_img_create( lv_scr_act() );			/*Create an image object for the cursor */
	lv_img_set_src(cpu0_globals->gui.mse_dat.cursor_obj, &mouse_cursor_icon);			/*Set the image source*/
	lv_indev_set_cursor(cpu0_globals->gui.mse_dat.indev, cpu0_globals->gui.mse_dat.cursor_obj);
	lv_obj_add_flag(cpu0_globals->gui.mse_dat.cursor_obj, LV_OBJ_FLAG_HIDDEN);// Hide the cursor, the USB HID driver will show cursor if a mouse is connected.

#ifdef MOUSE_EMU // If defined allows mouse emulation using Zedboard cursor keys
    /* Start Mouse Task */
	pjbes_sysmsg_q_t msg = { SPAWN_MEMU, NULL };
	q_spawn_req( &msg );

	int Status  = XGpio_Initialize(&Gpio, XPAR_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		/* TODO: Error logged.... */
	}
	/* Set the direction for all signals to be outputs */
	XGpio_SetDataDirection(&Gpio, 1, 0x0);
	/*
	 * Connect the interrupt handler that will be called when an
	 * interrupt occurs for the device.
	 */
	Status = XScuGic_Connect(&xInterruptController, XPAR_FABRIC_GPIO_0_VEC_ID, (Xil_ExceptionHandler)&mouse_emu_Handler, &Gpio);
	if (Status != XST_SUCCESS) {
			/* TODO: Error logged.... */
	}
	XScuGic_SetPriorityTriggerType(&xInterruptController, XPAR_FABRIC_GPIO_0_VEC_ID, 0xC0, 0x3);


	/* Enable the interrupt for the GPIO device.*/
	XScuGic_Enable(&xInterruptController, XPAR_FABRIC_GPIO_0_VEC_ID);
	XGpio_InterruptEnable(&Gpio, 0x1F);
	XGpio_InterruptGlobalEnable(&Gpio);
#endif
}

#ifdef MOUSE_EMU // If defined allows mouse emulation using Zedboard cursor keys

void mouse_Task( void *p ) {

	uint8_t		chg = pdFALSE;

	while(1) {
		if( cpu0_globals->mse_dat.down ) {
			if( cpu0_globals->mse_dat.last_state.point.y < vga_data->v_ln ) cpu0_globals->mse_dat.last_state.point.y++;	// Down
			chg = pdTRUE;
		}
		if( cpu0_globals->mse_dat.left ) {
			if( cpu0_globals->mse_dat.last_state.point.x > 0 ) cpu0_globals->mse_dat.last_state.point.x--;				// Left
			chg = pdTRUE;
		}
		if( cpu0_globals->mse_dat.right ) {
			if( cpu0_globals->mse_dat.last_state.point.x < vga_data->h_px ) cpu0_globals->mse_dat.last_state.point.x++;	// Right
			chg = pdTRUE;
		}
		if( cpu0_globals->mse_dat.up ) {
			if( cpu0_globals->mse_dat.last_state.point.y > 0 ) cpu0_globals->mse_dat.last_state.point.y--;				// Up
			chg = pdTRUE;
		}
		if( cpu0_globals->mse_dat.irq_true ) {		// This is to stop this from interfering with VNC
			cpu0_globals->mse_dat.last_state.state = cpu0_globals->mse_dat.click;
			cpu0_globals->mse_dat.irq_true = pdFALSE;
			chg = pdTRUE;
		}
		if( chg ) {
			xQueueSend( cpu0_globals->mse_dat.state_q, &cpu0_globals->mse_dat.last_state, 0 );
			chg = pdFALSE;
		}
		vTaskDelay(pdMS_TO_TICKS(4));
	}
}

void mouse_emu_Handler(void *CallbackRef) {

	uint32_t	Register, state;
	XGpio		*GpioPtr = (XGpio *)CallbackRef;

	while( 	(Register = XGpio_ReadReg(Gpio.BaseAddress, XGPIO_ISR_OFFSET)) ) {
		state = XGpio_DiscreteRead(&Gpio, 1);

		if( state & 0x01 ) cpu0_globals->mse_dat.click = LV_INDEV_STATE_PR; else cpu0_globals->mse_dat.click = LV_INDEV_STATE_REL;	// Click
		if( state & 0x02 ) cpu0_globals->mse_dat.down = pdTRUE; else cpu0_globals->mse_dat.down = pdFALSE;							// Down
		if( state & 0x04 ) cpu0_globals->mse_dat.left = pdTRUE; else cpu0_globals->mse_dat.left = pdFALSE;							// Left
		if( state & 0x08 ) cpu0_globals->mse_dat.right = pdTRUE; else cpu0_globals->mse_dat.right = pdFALSE;						// Right
		if( state & 0x10 ) cpu0_globals->mse_dat.up = pdTRUE; else cpu0_globals->mse_dat.up = pdFALSE;								// Up

		/* Clear the Interrupt */
		XGpio_InterruptClear(GpioPtr, Register);

	}
	cpu0_globals->mse_dat.irq_true = pdTRUE;

}
#endif
