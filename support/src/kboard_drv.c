/*
 * kboard_drv.c
 *
 *  Created on: 21 Oct 2021
 *      Author: PeterB
 */

#include "support.h"

extern cpu0_globals_t		*cpu0_globals;

static void keyboard_read(lv_indev_drv_t *drv, lv_indev_data_t *data){

	lv_indev_data_t		event_q;

	if( xQueueReceive( cpu0_globals->gui.kb_dat.state_q, &event_q, 0 ) ) {
		data->key = event_q.key;            /*Get the last pressed or released key*/
		data->state = event_q.state;
	} else {
		data->key = 0;
	}
	data->continue_reading = uxQueueMessagesWaiting( cpu0_globals->gui.kb_dat.state_q ) ? pdTRUE : pdFALSE;
	return;
}


lv_indev_t* keyboard_drv_init( void ) {

    lv_indev_drv_init(&cpu0_globals->gui.kb_dat.indev_drv);          /*Basic initialisation*/
    cpu0_globals->gui.kb_dat.indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    cpu0_globals->gui.kb_dat.indev_drv.read_cb = &keyboard_read;         /*This function will be called periodically (by the library) to get the keys and state*/
    cpu0_globals->gui.kb_dat.indev_drv.user_data = NULL;
    return( lv_indev_drv_register(&cpu0_globals->gui.kb_dat.indev_drv) );

}

