/*
 * gui_logs.c
 *
 *  Created on: 18 Apr 2019
 *      Author: peterb
 */

#include "support.h"
#include "gui.h"
#include "ff_headers.h"
#include "ff_stdio.h"

//extern shared_memp_t		shmem_p;
//extern config_datp_t		conf_p;
//extern cpu0_globals_t		*cpu0_globals;

static lv_obj_t			*log_ta = NULL;
static lv_obj_t			*log_btn_clr;

void update_gui_log( void ) {

    char				*buf;
	FF_FILE				*file;
	int32_t				read, t_read, len;
	sysmsg_q_t			msg = { 0, pdFALSE, NULL };

	if( log_ta == NULL ) return; // Just in case this gets called before the log ta is created!
    if( (file = ff_fopen( LOG_FILE_PATH, "r" )) != NULL )  {
		len = ff_filelength( file );
		if( (buf = pvPortMalloc(len+1)) == NULL ) {
			msg.id = ERR_GUILOG_ALLOC_FAIL;
			q_sysmsg( &msg );
			ff_fclose( file );
			return;
		}
		t_read = 0;
		while( t_read < len ) {
			read = ff_fread( (buf + t_read), 1, (len - t_read), file );
			if( read < 0 ) {
				ff_fclose( file );
				return;
			}
			t_read += read;
		}
		*(buf+len) = 0;	// Make sure buffer is NULL terminated!
	    lv_textarea_set_text(log_ta, buf);
	    vPortFree(buf);
		ff_fclose( file );
	} else {
		lv_textarea_set_text(log_ta, "No Events Logged..." );
	}
}

static void log_btn_action(lv_event_t *event) {

	lv_event_code_t 	code = lv_event_get_code(event);
	lv_obj_t			*btn = lv_event_get_target(event);
	sysmsg_q_t			msg = { 0, pdFALSE, NULL };

    if( code == LV_EVENT_CLICKED ) {
    	if( btn == log_btn_clr ) {
    		msg.id = LOG_CLEAR_CMD;
    		q_sys_action( &msg );
    	}
    }
}

void log_create( lv_obj_t *parent ) {

    lv_obj_t			*genlog_lab;
	lv_obj_t			*tmp_label;

    genlog_lab = lv_label_create(parent);
 	lv_obj_set_style_text_font (genlog_lab, &lv_font_montserrat_16, LV_PART_MAIN);
	lv_obj_set_pos(genlog_lab, 50, 15);
	lv_label_set_text( genlog_lab, "System log:");

    log_ta = lv_textarea_create(parent);
	lv_obj_set_style_text_font (log_ta, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(log_ta, lv_obj_get_width(parent)-100, lv_obj_get_height(parent) - 100);
    lv_obj_set_pos(log_ta, 50, 50);
    setup_scrl_bar( log_ta, LV_PART_SCROLLBAR, 8);
    lv_obj_set_scrollbar_mode(log_ta, LV_SCROLLBAR_MODE_AUTO);
    lv_textarea_set_cursor_click_pos(log_ta, pdFALSE);
    lv_obj_clear_flag(log_ta, LV_OBJ_FLAG_CLICK_FOCUSABLE);

    log_btn_clr = lv_btn_create(parent);
	lv_obj_add_event_cb(log_btn_clr, log_btn_action, LV_EVENT_CLICKED, NULL);
	lv_obj_set_pos(log_btn_clr, 750, 0);
	tmp_label = lv_label_create(log_btn_clr);
	lv_label_set_text(tmp_label, "Clear Log");

	sysmsg_q_t msg = { UPDATE_GUI_LOG, pdFALSE, NULL };
    q_gui_msg( &msg );

}
