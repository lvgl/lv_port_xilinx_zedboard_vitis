/*
 * gui.c
 *
 *  Created on: 27 Sep 2022
 *      Author: PeterB
 */
#include <time.h>
#include <stdio.h>

#include "support.h"
#include "mouse.h"
#include "pjbes_time.h"
#include "gui.h"
#include "vga.h"
#include "lvgl.h"
#include "lvgl_wdemo/lv_demos.h"

/* Pointers to configuration data and shared memory */
extern config_datp_t		conf_p;
extern shared_memp_t 		shmem_p;
extern cpu0_globals_t		*cpu0_globals;

static lv_obj_t				*main_tabview = NULL;
static lv_obj_t 			*lab_loc_current_time;
static lv_obj_t 			*lab_utc_current_time;
static lv_obj_t				*lab_gui_load;
static lv_obj_t				*ta_startup = NULL;

#define NO_MSG_SLOTS		10	/* Number of pop up message positions on screen */
#define MSG_B_SIZEX		1014												// Width of pop up message box
#define MSG_B_SIZEY		40													// Height of pop up message box
#define MSG_STARTPOS_Y	(LV_VER_RES_MAX - NO_MSG_SLOTS * MSG_B_SIZEY) / 2	// Y Position for first pop up message on screen
#define MSG_ANIM_TIME		1000												// Number of ms to move a pop up message on/off screen
#define MSG_HOLD_TIME		5000												// Number of mS to show a pop up message

static void anim_x_cb(void * var, lv_coord_t v) {

    lv_obj_set_x(var, v);
}

static void show_sys_message( char* msg ) {

	static uint8_t				msg_pos = 0, first_call = pdTRUE;
	static lv_obj_t				*mbox[NO_MSG_SLOTS];
	static lv_style_t			mbox_style;
	static lv_anim_t 			a1[NO_MSG_SLOTS];

	if( first_call ) {
		lv_style_init(&mbox_style);
		lv_style_set_radius(&mbox_style, 3);
		lv_style_set_opa(&mbox_style, LV_OPA_60);
		lv_style_set_border_width(&mbox_style, 2);
		lv_style_set_border_color(&mbox_style, conf_p->gui_colour);
		lv_style_set_text_align(&mbox_style, LV_TEXT_ALIGN_CENTER);
		for( uint8_t i = 0; i < NO_MSG_SLOTS; i++ ) {

			mbox[i] = lv_textarea_create(lv_scr_act());
			lv_textarea_set_one_line(mbox[i], pdTRUE);
			lv_obj_add_style(mbox[i], &mbox_style, LV_PART_MAIN);
			lv_obj_set_width( mbox[i], MSG_B_SIZEX);
			lv_obj_set_pos(mbox[i], LV_HOR_RES_MAX, (MSG_STARTPOS_Y + (MSG_B_SIZEY * i)) );
			lv_anim_init(&a1[i]);
			lv_anim_set_var(&a1[i], mbox[i]);
			lv_anim_set_values(&a1[i], LV_HOR_RES_MAX, (LV_HOR_RES_MAX - MSG_B_SIZEX - 10));
			lv_anim_set_time(&a1[i], 2*MSG_ANIM_TIME);
			lv_anim_set_playback_time(&a1[i], MSG_ANIM_TIME);
			lv_anim_set_playback_delay(&a1[i], MSG_HOLD_TIME);
			lv_anim_set_exec_cb(&a1[i], anim_x_cb);
			lv_anim_set_path_cb(&a1[i], lv_anim_path_ease_in_out);
		}
		first_call = pdFALSE;
	}
	lv_textarea_set_text(mbox[msg_pos], msg);
	lv_obj_move_foreground( mbox[msg_pos] );
	lv_style_set_border_color(&mbox_style, conf_p->gui_colour);	// we do this every time...
	lv_anim_start(&a1[msg_pos]);
	if( ++msg_pos == NO_MSG_SLOTS ) msg_pos = 0;

}

static void kb_drag_handler(lv_event_t * e) {

    lv_obj_t * obj = lv_event_get_target(e);

    lv_indev_t * indev = lv_indev_get_act();
    if(indev == NULL)  return;

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    lv_coord_t x = lv_obj_get_x(obj) + ((vect.x) * 5);
    lv_coord_t y = lv_obj_get_y(obj) + ((vect.y) * 5);
    lv_obj_set_pos(obj, x, y);
    lv_obj_invalidate(obj);
}

static void kb_cb_gen( lv_event_t *event ) {

	lv_event_code_t 	code = lv_event_get_code(event);
	sysmsg_q_t			msg = { 0, pdFALSE, NULL };

	switch( code ) {

			case LV_EVENT_CANCEL:
				lv_obj_add_flag( cpu0_globals->gui.kb_dat.kb_container, LV_OBJ_FLAG_HIDDEN );
				break;

			case LV_EVENT_READY:
				lv_obj_update_layout(cpu0_globals->gui.kb_dat.kb_container);
				lv_obj_set_pos(cpu0_globals->gui.kb_dat.kb_container,
					((LV_HOR_RES_MAX - 30) - lv_obj_get_width(cpu0_globals->gui.kb_dat.kb_container)),
					((LV_VER_RES_MAX - 40) - lv_obj_get_height(cpu0_globals->gui.kb_dat.kb_container)) );
				if( update_all_cfg() ) {	// If anything has changed update configuration file
				    msg.id = UPDATE_GUI_SYS_SCR;
				    q_gui_msg( &msg );
					lv_obj_add_flag( cpu0_globals->gui.kb_dat.kb_container, LV_OBJ_FLAG_HIDDEN );
				}
				break;

			default:
				lv_keyboard_def_event_cb(event);
				break;
	}

}

void setup_keyboard( void ) {

	static uint8_t			first_call = pdTRUE;

	if( first_call ) {
		cpu0_globals->gui.kb_dat.kb_container = lv_obj_create(lv_layer_top());
		lv_obj_set_size(cpu0_globals->gui.kb_dat.kb_container, 560, 260 );
	    cpu0_globals->gui.kb_dat.kb = lv_keyboard_create(cpu0_globals->gui.kb_dat.kb_container);
		lv_obj_set_size(cpu0_globals->gui.kb_dat.kb, 500, 200 );
		lv_obj_update_layout(cpu0_globals->gui.kb_dat.kb);
		lv_obj_center(cpu0_globals->gui.kb_dat.kb);
		lv_obj_set_pos(cpu0_globals->gui.kb_dat.kb_container, ((LV_HOR_RES_MAX - 30) - lv_obj_get_width(cpu0_globals->gui.kb_dat.kb_container)),
				((LV_VER_RES_MAX - 40) - lv_obj_get_height(cpu0_globals->gui.kb_dat.kb_container)) );
		lv_obj_set_style_border_width (cpu0_globals->gui.kb_dat.kb, 1, LV_PART_ITEMS );
		lv_obj_add_event_cb(cpu0_globals->gui.kb_dat.kb_container, kb_drag_handler, LV_EVENT_PRESSING, NULL);
	    lv_obj_add_event_cb(cpu0_globals->gui.kb_dat.kb, kb_cb_gen, LV_EVENT_CANCEL, NULL);
	    lv_obj_add_event_cb(cpu0_globals->gui.kb_dat.kb, kb_cb_gen, LV_EVENT_READY, NULL);
	    lv_obj_add_flag(cpu0_globals->gui.kb_dat.kb_container, LV_OBJ_FLAG_HIDDEN);
		first_call = pdFALSE;

	}
	lv_obj_set_style_border_color(cpu0_globals->gui.kb_dat.kb,	conf_p->gui_colour, LV_PART_ITEMS );
}

void kb_ta_action( lv_event_t *event ) {

	lv_event_code_t 	code = lv_event_get_code(event);
	lv_obj_t			*ta = lv_event_get_target(event);
	uint32_t			kb_type;

	if( code == LV_EVENT_CLICKED ) {
		lv_group_focus_obj(ta);
		lv_keyboard_set_textarea(cpu0_globals->gui.kb_dat.kb, ta);
		kb_type = (uint32_t)lv_obj_get_user_data(ta);
		switch( kb_type ) {

			case UDAT_TA_NUM:
				lv_keyboard_set_mode(cpu0_globals->gui.kb_dat.kb, LV_KEYBOARD_MODE_NUMBER);
				lv_obj_set_size(cpu0_globals->gui.kb_dat.kb_container, 290, 240 );
				lv_obj_set_size(cpu0_globals->gui.kb_dat.kb, 250, 200 );
				break;

			case UDAT_TA_ALPHA:
				lv_keyboard_set_mode(cpu0_globals->gui.kb_dat.kb, LV_KEYBOARD_MODE_TEXT_LOWER);
				lv_obj_set_size(cpu0_globals->gui.kb_dat.kb_container, 540, 240 );
				lv_obj_set_size(cpu0_globals->gui.kb_dat.kb, 500, 200 );
				break;

			case UDAT_TA_ALPHA_UPPER:
				lv_keyboard_set_mode(cpu0_globals->gui.kb_dat.kb, LV_KEYBOARD_MODE_TEXT_UPPER);
				lv_obj_set_size(cpu0_globals->gui.kb_dat.kb_container, 540, 240 );
				lv_obj_set_size(cpu0_globals->gui.kb_dat.kb, 500, 200 );
				break;

			default:
				lv_keyboard_set_mode(cpu0_globals->gui.kb_dat.kb, LV_KEYBOARD_MODE_TEXT_LOWER);
				lv_obj_set_size(cpu0_globals->gui.kb_dat.kb_container, 540, 240 );
				lv_obj_set_size(cpu0_globals->gui.kb_dat.kb, 500, 200 );
				break;
		}
		lv_obj_update_layout(cpu0_globals->gui.kb_dat.kb);
		lv_obj_center(cpu0_globals->gui.kb_dat.kb);
		lv_obj_update_layout(cpu0_globals->gui.kb_dat.kb_container);
		lv_obj_set_pos(cpu0_globals->gui.kb_dat.kb_container, ((LV_HOR_RES_MAX - 30) - lv_obj_get_width(cpu0_globals->gui.kb_dat.kb_container)),
				((LV_VER_RES_MAX - 40) - lv_obj_get_height(cpu0_globals->gui.kb_dat.kb_container)) );
		lv_obj_clear_flag(cpu0_globals->gui.kb_dat.kb_container, LV_OBJ_FLAG_HIDDEN);
	}

}

void update_scrl_bar_style( void ) {

//	return; // TODO: Look in to this!
	lv_style_set_bg_grad_color(&cpu0_globals->gui.style_sb, conf_p->gui_colour );
}


void setup_scrl_bar( lv_obj_t *obj, uint32_t part, uint8_t size ) {

	static uint8_t		first_call = pdTRUE;

	if( first_call ) {
		lv_style_init(&cpu0_globals->gui.style_sb);
		lv_style_set_radius(&cpu0_globals->gui.style_sb, LV_RADIUS_CIRCLE);
		lv_style_set_bg_opa(&cpu0_globals->gui.style_sb, LV_OPA_60);
		lv_style_set_bg_grad_color(&cpu0_globals->gui.style_sb, conf_p->gui_colour);
		lv_style_set_bg_grad_dir(&cpu0_globals->gui.style_sb, LV_GRAD_DIR_VER);
		lv_style_set_border_width(&cpu0_globals->gui.style_sb, 0);
		lv_style_set_pad_bottom(&cpu0_globals->gui.style_sb, 3);
		lv_style_set_pad_right(&cpu0_globals->gui.style_sb, 3);
		first_call = pdFALSE;
	}
    lv_obj_add_style(obj, &cpu0_globals->gui.style_sb, part);
    lv_obj_set_style_width(obj, size, part);
}
static void update_gui_theme( uint8_t scrl_bar ) {

	lv_theme_t * th = lv_theme_default_init(cpu0_globals->gui.disp, conf_p->gui_colour, lv_palette_main(LV_PALETTE_PURPLE),
			conf_p->gui_theme, LV_FONT_DEFAULT);
	lv_disp_set_theme(cpu0_globals->gui.disp, th);
	setup_keyboard();
	if( scrl_bar ) update_scrl_bar_style();
	lv_obj_invalidate(lv_scr_act());
}

uint8_t q_gui_msg( sysmsg_q_t *msg ) {

	return xQueueSend( cpu0_globals->gui.msg_q, msg, 0 );

}


static void process_gui_msg_q( lv_timer_t *timer ) {

	char				*pmsg = NULL;
	sysmsg_q_t			msg;
	sysmsg_t			*db_entry;

	if( ( xQueueReceive( cpu0_globals->gui.msg_q, &msg, 0 ) ) ) {
		if( msg.id < LOG_DB_END ){
			if( (db_entry = get_log_msg_by_id( msg.id ) ) == NULL ) return;
			if( msg.extra_data != NULL ) {
				pmsg = strstr( msg.extra_data," - " ); // This strips the date off the front of messages so we don't print it on screen
				if( pmsg != NULL ) {
					pmsg += 3;
				} else pmsg = msg.extra_data;
			}
			if( ta_startup != NULL && db_entry->category == log_cat_startup ) {
				if( pmsg) {
					lv_textarea_add_text( ta_startup, pmsg );	// All startup messages are defined first so they are sent to pseudo console
					lv_textarea_add_text( ta_startup, "\n" );
				}
			} else show_sys_message( pmsg );
			if( msg.free_ext ) vPortFree( msg.extra_data );
		} else {
			switch( msg.id ) {

				case LOAD_MAIN_GUI:
					if( ta_startup ) lv_obj_del( ta_startup );
					ta_startup = NULL;
					main_gui_create();
					break;

				case UPDATE_GUI_LOG:
					update_gui_log();
					break;

				case UPDATE_GUI_THEME:
					update_gui_theme( pdTRUE );
					break;

				case UPDATE_GUI_SYS_SCR:
					sys_scrupdate();
					break;

				case GUI_HIDE_MOUSE_CURSOR:
					if( cpu0_globals->gui.mse_dat.cursor_obj ) lv_obj_add_flag(cpu0_globals->gui.mse_dat.cursor_obj, LV_OBJ_FLAG_HIDDEN);
					break;

				case GUI_SHOW_MOUSE_CURSOR:
					if( cpu0_globals->gui.mse_dat.cursor_obj ) lv_obj_clear_flag(cpu0_globals->gui.mse_dat.cursor_obj, LV_OBJ_FLAG_HIDDEN);
					break;

				default:
					break;
			}
			lv_obj_invalidate(lv_scr_act());
		}
	}
}
static void update_time( lv_timer_t *timer ) {

	static char		u_buf[36] = "UTC: ", s_buf[80];  // Text Buffers
	static char		l_buf[36] = "LOC: ";  // Text Buffer
	static time_t		time_last_l = 0;
	static time_t		time_last_u = 0;
	static int			tz_l = 0;
	time_t				time_now;

	/* Update Local Time Display */
	if( tz_l != conf_p->tz_idx ) {
		tz_l = conf_p->tz_idx;
		time_last_l = 0;
		time_last_u = 0;
	}

	/* Update Local Time Display */
	time_now = get_zone_time( conf_p->tz_idx, l_buf + 5 );	// Append time to our text buffer
	if( time_now > time_last_l ) {
		lv_label_set_text(lab_loc_current_time, l_buf);
		lv_obj_align(lab_loc_current_time, LV_ALIGN_TOP_RIGHT, -320, 4);  /*Align to the top*/
	}
	/* Update UTC Time Display */
	time_now = get_zone_time( 0, u_buf + 5 );	// Append time to our text buffer
	if( time_now > time_last_u ) {
		lv_label_set_text(lab_utc_current_time, u_buf);
		lv_obj_align(lab_utc_current_time, LV_ALIGN_TOP_RIGHT, -4, 4);  /*Align to top right*/
	}
	sprintf( s_buf, "CPU0:%03d%%|CPU1:%03d%%|GUI:%03d%%", GetCPUUsage(0), GetCPUUsage(1), (100 - lv_timer_get_idle()) );
	lv_label_set_text(lab_gui_load, s_buf);
	lv_obj_align(lab_gui_load, LV_ALIGN_TOP_LEFT, 4, 4);  /*Align to top left*/
	// Hide the keyboard on tab change
	if( main_tabview && cpu0_globals->gui.kb_dat.kb_container ) {
		switch( lv_tabview_get_tab_act(main_tabview) ) {

			case 2:
				lv_obj_add_flag(cpu0_globals->gui.kb_dat.kb_container, LV_OBJ_FLAG_HIDDEN);
				break;

			default:
				break;

		}
	}

}

void main_gui_create(void) {

	lv_obj_t			*tab1, *tab2, *tab3, *tab4;

	main_tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 24);
	lv_obj_set_style_pad_top(main_tabview, 24, LV_PART_MAIN);
	lv_obj_set_size(main_tabview, lv_pct(100), lv_pct(100));
	lv_obj_align(main_tabview, LV_ALIGN_BOTTOM_LEFT, 0, 0);  /*Align to the Bottom*/
	lv_obj_set_style_border_width (main_tabview, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_border_color (main_tabview, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_border_width (main_tabview, 1, LV_PART_ITEMS);
	lv_obj_set_style_border_color (main_tabview, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_PART_ITEMS);

	lab_utc_current_time = lv_label_create(lv_scr_act());
	lv_obj_set_style_text_font (lab_utc_current_time, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
	lab_gui_load = lv_label_create(lv_scr_act());
	lv_obj_set_style_text_font (lab_gui_load, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
	lab_loc_current_time = lv_label_create(lv_scr_act());
	lv_obj_set_style_text_font (lab_loc_current_time, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

	setup_keyboard();
	mouse_init();

	tab1 = lv_tabview_add_tab(main_tabview, "Configuration");
	tab2 = lv_tabview_add_tab(main_tabview, "Demo Widgets");
	tab3 = lv_tabview_add_tab(main_tabview, "System Info");
	tab4 = lv_tabview_add_tab(main_tabview, "Log");

	config_create(tab1);
	lv_demo_widgets(tab2);
	sysinfo_create(tab3);
	log_create(tab4);
	update_gui_theme( pdTRUE );
    lv_obj_invalidate(lv_scr_act());
	cpu0_globals->gui.gui_ready = pdTRUE;
	shmem_p->cpu_1_flags |= CPU1_PROCEED;		// Allow CPU1 to start
	xil_printf( "CPU1 Flags: 0x%06X\r\n", shmem_p->cpu_1_flags );
	update_time( NULL );

   lv_timer_create((lv_timer_cb_t)update_time, 250, NULL );  // Update the screen every 0.25 seconds
}

void startup_gui_create(void) {

	lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
	ta_startup = lv_textarea_create(lv_scr_act());
	lv_obj_set_size(ta_startup, lv_obj_get_width(lv_scr_act())-20, lv_obj_get_height(lv_scr_act())-20);
	lv_textarea_set_text( ta_startup, "");
	lv_obj_align(ta_startup, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_bg_opa (ta_startup, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ta_startup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	cpu0_globals->gui.kb_dat.kb_gp_sysc = lv_group_create();
	lv_group_set_default(cpu0_globals->gui.kb_dat.kb_gp_sysc);
	cpu0_globals->gui.kb_dat.indev = keyboard_drv_init();
	lv_indev_set_group(cpu0_globals->gui.kb_dat.indev, cpu0_globals->gui.kb_dat.kb_gp_sysc);
	lv_obj_invalidate(lv_scr_act());
}

void gui_thread(void *p) {

	// Initialise VGA Hardware
	set_vga_prams( VGA_1024X768_60HZ );
	// Initialise GUI
	lv_init();
	lv_theme_default_init(cpu0_globals->gui.disp, conf_p->gui_colour, lv_palette_main(LV_PALETTE_PURPLE),
			conf_p->gui_theme, LV_FONT_DEFAULT);
	lv_disp_drv_init((lv_disp_drv_t*)&cpu0_globals->gui.disp_drv);
	lv_disp_draw_buf_init(&cpu0_globals->gui.disp_buf, (void*)LV_VDB_ADR, (void*)LV_VDB2_ADR,
			(LV_HOR_RES_MAX*LV_VER_RES_MAX));
	cpu0_globals->gui.disp_drv.flush_cb = vga_disp_flush;
	cpu0_globals->gui.disp_drv.hor_res = LV_HOR_RES_MAX;                 /*Set the horizontal resolution in pixels*/
	cpu0_globals->gui.disp_drv.ver_res = LV_VER_RES_MAX;                 /*Set the vertical resolution in pixels*/
	cpu0_globals->gui.disp_drv.draw_buf = &cpu0_globals->gui.disp_buf;
	cpu0_globals->gui.disp_drv.full_refresh = pdFALSE;
	cpu0_globals->gui.disp_drv.direct_mode = pdTRUE;
	cpu0_globals->gui.disp = lv_disp_drv_register((lv_disp_drv_t*)&cpu0_globals->gui.disp_drv);
	lv_disp_set_bg_opa(NULL, LV_OPA_TRANSP);
	startup_gui_create();
	lv_timer_create((lv_timer_cb_t)process_gui_msg_q, 20, NULL);	// Check for GUI thread messages every 20ms
	while(1) {
		lv_task_handler();
		vTaskDelay(pdMS_TO_TICKS(4));
	}

}
