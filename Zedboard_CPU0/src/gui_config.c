/*
 * gui_config.c
 *
 *  Created on: 27 Sep 2022
 *      Author: PeterB
 */


/*
 *
 * SYSTEM CONFIGURATION
 *
 */
#include <math.h>
#include <stdlib.h>

#include "support.h"
#include "lvgl.h"
#include "gui.h"

extern config_datp_t		conf_p;
extern cpu0_globals_t		*cpu0_globals;

static char *numkeys = { "+-.0123456789" };
//static char *numkeysus = { "0123456789" };
static char *ipkeys = { ".0123456789" };
static char *hexkeys = { "x0123456789abcdefABCDEF" };
//static char *timekeys = { "0123456789:" };

static lv_obj_t			*sys_lab_name;
static lv_obj_t			*sys_lab_ip;
static lv_obj_t			*sys_lab_nm;
static lv_obj_t			*sys_lab_gw;
static lv_obj_t			*sys_lab_dns;
static lv_obj_t			*sys_lab_ntp;
static lv_obj_t			*sys_lab_ntpup;
static lv_obj_t			*sys_lab_colour;
static lv_obj_t			*sys_lab_style;
static lv_obj_t			*sys_ta_name;
static lv_obj_t			*sys_ta_ip;
static lv_obj_t			*sys_ta_nm;
static lv_obj_t			*sys_ta_gw;
static lv_obj_t			*sys_ta_dns;
static lv_obj_t			*sys_ta_ntp;
static lv_obj_t			*sys_ta_ntpup;
static lv_obj_t			*sys_cp_colour;
static lv_obj_t			*sys_dd_theme;
static lv_obj_t 		*sys_btn_update;
static lv_obj_t			*sys_ta_macs0;
static lv_obj_t			*sys_ta_macs1;
static lv_obj_t			*sys_ta_macs2;
static lv_obj_t			*sys_ta_macs3;
static lv_obj_t			*sys_ta_macs4;
static lv_obj_t			*sys_ta_macs5;
static lv_obj_t			*sys_lab_macs;
static lv_obj_t			*sys_sw_bst;
static lv_obj_t			*sys_lab_bst;

void sys_scrupdate( void ) {

	char		buf[MAX_NAME_LEN];

	lv_textarea_set_text( sys_ta_name, conf_p->Sys_Name );
	FreeRTOS_inet_ntoa( conf_p->Sys_IP, buf );
	lv_textarea_set_text( sys_ta_ip, buf );
	FreeRTOS_inet_ntoa( conf_p->Sys_nm, buf );
	lv_textarea_set_text( sys_ta_nm, buf );
	FreeRTOS_inet_ntoa( conf_p->Sys_gw, buf );
	lv_textarea_set_text( sys_ta_gw, buf );
	FreeRTOS_inet_ntoa( conf_p->Sys_DNS, buf );
	lv_textarea_set_text( sys_ta_dns, buf );
	FreeRTOS_inet_ntoa( conf_p->Sys_NTP, buf );
	lv_textarea_set_text( sys_ta_ntp, buf );
	sprintf( buf,"%6.2f", conf_p->Sys_NTP_update);
	lv_textarea_set_text( sys_ta_ntpup, buf );
	if( (cpu0_globals->gui.colour_changed & CW_C_CHANGE) ) {
		cpu0_globals->gui.colour_changed &= ~CW_C_CHANGE;
		lv_colorwheel_set_rgb(sys_cp_colour, conf_p->gui_colour);
	}
	lv_dropdown_set_selected( sys_dd_theme, conf_p->gui_theme );
	if( conf_p->st_active ) lv_obj_add_state( sys_sw_bst, LV_STATE_CHECKED ); else lv_obj_clear_state( sys_sw_bst, LV_STATE_CHECKED );
	sprintf( buf, "0x%02X", conf_p->softmac_addr[0] );
	lv_textarea_set_text( sys_ta_macs0, buf );
	sprintf( buf, "0x%02X", conf_p->softmac_addr[1] );
	lv_textarea_set_text( sys_ta_macs1, buf );
	sprintf( buf, "0x%02X", conf_p->softmac_addr[2] );
	lv_textarea_set_text( sys_ta_macs2, buf );
	sprintf( buf, "0x%02X", conf_p->softmac_addr[3] );
	lv_textarea_set_text( sys_ta_macs3, buf );
	sprintf( buf, "0x%02X", conf_p->softmac_addr[4] );
	lv_textarea_set_text( sys_ta_macs4, buf );
	sprintf( buf, "0x%02X", conf_p->softmac_addr[5] );
	lv_textarea_set_text( sys_ta_macs5, buf );
}

static void restart_action( lv_event_t *event ) {

    lv_obj_t			*btn = lv_event_get_target(event);
    lv_obj_t			*mbox = lv_obj_get_parent(btn);
	lv_event_code_t 	code = lv_event_get_code(event);
	sysmsg_q_t			msg = { SYS_REBOOT, pdFALSE, " GUI" };

    if( code == LV_EVENT_VALUE_CHANGED ) {
    	if( !strcmp( lv_msgbox_get_active_btn_text(mbox), "Restart" ) ) {
			q_sys_action( &msg );
		}
    }
	lv_msgbox_close(mbox);
}

static uint8_t update_sys_cfg( void ) {

	uint8_t					update = pdFALSE, mac_chg = pdFALSE, chk_macb;
	uint32_t				addr;
	uint16_t				temp;
	lv_color_t				tempc;
	double					tfl;
	sysmsg_q_t			msg = { 0, pdFALSE, NULL };
	lv_obj_t 				*mb_restart;
	static const char		*btns[] ={"Restart", "Cancel", ""}; /*Button description. '\221' lv_btnm like control char*/

	if( strcmp( conf_p->Sys_Name, lv_textarea_get_text( sys_ta_name ) ) ) {
		strcpy( conf_p->Sys_Name, lv_textarea_get_text( sys_ta_name ) );
		update = pdTRUE;
	}
	if( (addr = FreeRTOS_inet_addr(lv_textarea_get_text(sys_ta_ip))) != 0) {
		if( conf_p->Sys_IP != addr ) {
			conf_p->Sys_IP = addr;
			update = pdTRUE;
			FreeRTOS_SetEndPointConfiguration( &addr, NULL, NULL, NULL, &cpu0_globals->tcpip.EndPoints[0] );
		}
	} else {
		msg.id = ERR_INVALID_IP;
		q_sysmsg( &msg );
	}
	if( (addr = FreeRTOS_inet_addr(lv_textarea_get_text(sys_ta_nm))) != 0) {
		if( conf_p->Sys_nm != addr ) {
			conf_p->Sys_nm = addr;
			update = pdTRUE;
			FreeRTOS_SetEndPointConfiguration( NULL, &addr, NULL, NULL, &cpu0_globals->tcpip.EndPoints[0] );
		}
	} else {
		msg.id = ERR_INVALID_NM;
		q_sysmsg( &msg );
	}
	if( (addr = FreeRTOS_inet_addr(lv_textarea_get_text(sys_ta_gw))) != 0) {
		if( conf_p->Sys_gw != addr ) {
			conf_p->Sys_gw = addr;
			update = pdTRUE;
			FreeRTOS_SetEndPointConfiguration( NULL, NULL, &addr, NULL, &cpu0_globals->tcpip.EndPoints[0] );
		}
	} else {
		msg.id = ERR_INVALID_GW;
		q_sysmsg( &msg );
	}
	if( (addr = FreeRTOS_inet_addr(lv_textarea_get_text(sys_ta_dns))) != 0) {
		if( conf_p->Sys_DNS != addr ) {
			conf_p->Sys_DNS = addr;
			update = pdTRUE;
			FreeRTOS_SetEndPointConfiguration( NULL, NULL, NULL, &addr, &cpu0_globals->tcpip.EndPoints[0] );
		}
	} else {
		msg.id = ERR_INVALID_DNS;
		q_sysmsg( &msg );
	}
	if( (addr = FreeRTOS_inet_addr(lv_textarea_get_text(sys_ta_ntp))) != 0) {
		if( conf_p->Sys_NTP != addr ) {
			conf_p->Sys_NTP = addr;
			update = pdTRUE;
		}
	} else {
		msg.id = ERR_INVALID_NTP;
		q_sysmsg( &msg );
	}
	tfl = fabs(atof(lv_textarea_get_text(sys_ta_ntpup)));
	if( tfl != conf_p->Sys_NTP_update ) {
		conf_p->Sys_NTP_update = tfl;
		update = pdTRUE;
	}
	//  The colour_changed flags are used to detect a change in colour setting as you can't compare values
	//  due to HSV/RGB conversion inconsistencies in LVGL library
	if( (cpu0_globals->gui.colour_changed & (CW_C_CHANGE | CW_UP_CLICK)) == (CW_C_CHANGE | CW_UP_CLICK) ) {
		tempc = lv_colorwheel_get_rgb(sys_cp_colour);
		memcpy ( &conf_p->gui_colour, &tempc, sizeof( lv_color_t ) );
		msg.id = UPDATE_GUI_THEME;
		q_gui_msg( &msg );
		cpu0_globals->gui.colour_changed &= ~(CW_UP_CLICK);
		update = pdTRUE;
	}
	temp = lv_dropdown_get_selected(sys_dd_theme );
	if( temp != conf_p->gui_theme ) {
		conf_p->gui_theme = temp;
		msg.id = UPDATE_GUI_THEME;
		q_gui_msg( &msg );
		update = pdTRUE;
	}
	chk_macb = lv_obj_has_state(sys_sw_bst, LV_STATE_CHECKED);
	if( chk_macb != conf_p->st_active ) {
		conf_p->st_active = chk_macb;
		if( conf_p->st_active ) conf_p->tz_idx = 38; else conf_p->tz_idx = 0;
		update = pdTRUE;
	}
	chk_macb = (uint8_t)strtol(lv_textarea_get_text( sys_ta_macs0 ), NULL, 16);
	if( chk_macb != conf_p->softmac_addr[0] ) {
		conf_p->softmac_addr[0] = chk_macb;
		mac_chg = pdTRUE;
		update = pdTRUE;
	}
	chk_macb = (uint8_t)strtol(lv_textarea_get_text( sys_ta_macs1 ), NULL, 16);
	if( chk_macb != conf_p->softmac_addr[1] ) {
		conf_p->softmac_addr[1] = chk_macb;
		update = pdTRUE;
		mac_chg = pdTRUE;
	}
	chk_macb = (uint8_t)strtol(lv_textarea_get_text( sys_ta_macs2 ), NULL, 16);
	if( chk_macb != conf_p->softmac_addr[2] ) {
		conf_p->softmac_addr[2] = chk_macb;
		update = pdTRUE;
		mac_chg = pdTRUE;
	}
	chk_macb = (uint8_t)strtol(lv_textarea_get_text( sys_ta_macs3 ), NULL, 16);
	if( chk_macb != conf_p->softmac_addr[3] ) {
		conf_p->softmac_addr[3] = chk_macb;
		update = pdTRUE;
		mac_chg = pdTRUE;
	}
	chk_macb = (uint8_t)strtol(lv_textarea_get_text( sys_ta_macs4 ), NULL, 16);
	if( chk_macb != conf_p->softmac_addr[4] ) {
		conf_p->softmac_addr[4] = chk_macb;
		update = pdTRUE;
		mac_chg = pdTRUE;
	}
	chk_macb = (uint8_t)strtol(lv_textarea_get_text( sys_ta_macs5 ), NULL, 16);
	if( chk_macb != conf_p->softmac_addr[5] ) {
		conf_p->softmac_addr[5] = chk_macb;
		update = pdTRUE;
		mac_chg = pdTRUE;
	}

	if( mac_chg ) {
		mb_restart = lv_msgbox_create(NULL, "MAC Address Changed - Restart Required!\n", "Restart Now?", btns, pdTRUE);
		lv_obj_set_width(mb_restart, 350);
		lv_obj_set_height(mb_restart, 110);
		//Changes button sizes as they don't autosize correctly with my usage of LV_DPI_DEF set to 36
		lv_obj_t *btn = lv_msgbox_get_btns(mb_restart);
		lv_theme_apply(btn);
        const lv_font_t * font = lv_obj_get_style_text_font(btn, LV_PART_ITEMS);
        lv_coord_t btn_h = lv_font_get_line_height(font) + 130 / 10;
        lv_obj_set_size(btn, 2/*2 buttons*/ * (2 * 130 / 3), btn_h);
		lv_obj_center(mb_restart);
		lv_obj_add_event_cb(mb_restart, restart_action, LV_EVENT_VALUE_CHANGED, NULL);
	}
	return update;
}

static void sys_btn_action( lv_event_t *event ) {

	lv_event_code_t 	code = lv_event_get_code(event);
	lv_obj_t			*btn = lv_event_get_target(event);
	sysmsg_q_t			msg = { 0, pdFALSE, NULL };
    uint8_t				updr;

    if( code == LV_EVENT_CLICKED ) {
		if( btn == sys_btn_update ) {
		    cpu0_globals->gui.colour_changed |= CW_UP_CLICK;
			if( (updr = update_sys_cfg()) ) {
				msg.id = SAVE_CONFIG;
				q_sys_action( &msg );
			    msg.id = UPDATE_GUI_SYS_SCR;
			    q_gui_msg( &msg );
 			}
		}
    }
}

static void cp_change_action( lv_event_t *event ) {

	cpu0_globals->gui.colour_changed |= CW_C_CHANGE;
}

static void conf_sys_create( lv_obj_t *parent ) {

	lv_obj_t			*tmp_label;

	sys_lab_name = lv_label_create(parent);
	sys_lab_ip = lv_label_create(parent);
	sys_lab_nm = lv_label_create(parent);
	sys_lab_gw = lv_label_create(parent);
	sys_lab_dns = lv_label_create(parent);
	sys_lab_ntp = lv_label_create(parent);
	sys_lab_ntpup = lv_label_create(parent);
	sys_lab_colour = lv_label_create(parent);
	sys_lab_style = lv_label_create(parent);
	sys_ta_name = lv_textarea_create(parent);
	sys_ta_ip = lv_textarea_create(parent);
	sys_ta_nm = lv_textarea_create(parent);
	sys_ta_gw = lv_textarea_create(parent);
	sys_ta_dns = lv_textarea_create(parent);
	sys_ta_ntp = lv_textarea_create(parent);
	sys_ta_ntpup = lv_textarea_create(parent);
	sys_cp_colour = lv_colorwheel_create(parent, pdTRUE);
	sys_dd_theme = lv_dropdown_create(parent);
	sys_btn_update = lv_btn_create(parent);
	sys_ta_macs0 = lv_textarea_create(parent);
	sys_ta_macs1 = lv_textarea_create(parent);
	sys_ta_macs2 = lv_textarea_create(parent);
	sys_ta_macs3 = lv_textarea_create(parent);
	sys_ta_macs4 = lv_textarea_create(parent);
	sys_ta_macs5 = lv_textarea_create(parent);
	sys_lab_macs = lv_label_create(parent);
	sys_lab_bst = lv_label_create(parent);
	sys_sw_bst = lv_switch_create(parent);

	lv_textarea_set_one_line(sys_ta_name, pdTRUE);
	lv_textarea_set_max_length(sys_ta_name, MAX_NAME_LEN-1);
	lv_obj_set_width(sys_ta_name, 270);
	lv_obj_set_pos(sys_ta_name, 200, 10);
	lv_label_set_text(sys_lab_name, "System Name:");
	lv_obj_set_pos(sys_lab_name, 10, 22);
	lv_obj_add_event_cb(sys_ta_name, kb_ta_action, LV_EVENT_CLICKED, NULL);

	lv_textarea_set_one_line(sys_ta_ip, pdTRUE);
	lv_obj_set_user_data(sys_ta_ip, (void*)UDAT_TA_NUM);
	lv_textarea_set_max_length(sys_ta_ip, MAX_IP_LEN);
	lv_textarea_set_accepted_chars(sys_ta_ip, ipkeys);
	lv_obj_set_width(sys_ta_ip, 270);
	lv_obj_set_pos(sys_ta_ip, 200, 65);
	lv_label_set_text(sys_lab_ip, "IP Address:");
	lv_obj_set_pos(sys_lab_ip, 10, 77);
	lv_obj_add_event_cb(sys_ta_ip, kb_ta_action, LV_EVENT_CLICKED, NULL);

	lv_textarea_set_one_line(sys_ta_nm, pdTRUE);
	lv_obj_set_user_data(sys_ta_nm, (void*)UDAT_TA_NUM);
	lv_textarea_set_max_length(sys_ta_nm, MAX_IP_LEN);
	lv_textarea_set_accepted_chars(sys_ta_nm, ipkeys);
	lv_obj_set_width(sys_ta_nm, 270);
	lv_obj_set_pos(sys_ta_nm, 200, 120);
	lv_label_set_text(sys_lab_nm, "Network Mask:");
	lv_obj_set_pos(sys_lab_nm, 10, 132);
	lv_obj_add_event_cb(sys_ta_nm, kb_ta_action, LV_EVENT_CLICKED, NULL);

	lv_textarea_set_one_line(sys_ta_gw, pdTRUE);
	lv_obj_set_user_data(sys_ta_gw, (void*)UDAT_TA_NUM);
	lv_textarea_set_max_length(sys_ta_gw, MAX_IP_LEN);
	lv_textarea_set_accepted_chars(sys_ta_gw, ipkeys);
	lv_obj_set_width(sys_ta_gw, 270);
	lv_obj_set_pos(sys_ta_gw, 200, 175);
	lv_label_set_text(sys_lab_gw, "Default Gateway:");
	lv_obj_set_pos(sys_lab_gw, 10, 187);
	lv_obj_add_event_cb(sys_ta_gw, kb_ta_action, LV_EVENT_CLICKED, NULL);

	lv_textarea_set_one_line(sys_ta_dns, pdTRUE);
	lv_obj_set_user_data(sys_ta_dns, (void*)UDAT_TA_NUM);
	lv_textarea_set_max_length(sys_ta_dns, MAX_IP_LEN);
	lv_textarea_set_accepted_chars(sys_ta_dns, ipkeys);
	lv_obj_set_width(sys_ta_dns, 270);
	lv_obj_set_pos(sys_ta_dns, 200, 230);
	lv_label_set_text(sys_lab_dns, "DNS Server:");
	lv_obj_set_pos(sys_lab_dns, 10, 242);
	lv_obj_add_event_cb(sys_ta_dns, kb_ta_action, LV_EVENT_CLICKED, NULL);

	lv_textarea_set_one_line(sys_ta_ntp, pdTRUE);
	lv_obj_set_user_data(sys_ta_ntp, (void*)UDAT_TA_NUM);
	lv_textarea_set_max_length(sys_ta_ntp, MAX_IP_LEN);
	lv_textarea_set_accepted_chars(sys_ta_ntp, ipkeys);
	lv_obj_set_width(sys_ta_ntp, 270);
	lv_obj_set_pos(sys_ta_ntp, 200, 285);
	lv_label_set_text(sys_lab_ntp, "NTP Server:");
	lv_obj_set_pos(sys_lab_ntp, 10, 297);
	lv_obj_add_event_cb(sys_ta_ntp, kb_ta_action, LV_EVENT_CLICKED, NULL);

	lv_textarea_set_one_line(sys_ta_ntpup, pdTRUE);
	lv_obj_set_user_data(sys_ta_ntpup, (void*)UDAT_TA_NUM);
	lv_textarea_set_max_length(sys_ta_ntpup, MAX_IP_LEN);
	lv_obj_set_width(sys_ta_ntpup, 270);
	lv_obj_set_pos(sys_ta_ntpup, 200, 340);
	lv_textarea_set_accepted_chars(sys_ta_ntpup, numkeys);
	lv_label_set_text(sys_lab_ntpup, "NTP Update Period(Hrs):");
	lv_obj_set_pos(sys_lab_ntpup, 10, 352);
	lv_obj_add_event_cb(sys_ta_ntpup, kb_ta_action, LV_EVENT_CLICKED, NULL);

	lv_obj_set_pos(sys_sw_bst, 200, 407);
	lv_obj_set_size(sys_sw_bst, 30, 16);
	lv_label_set_text(sys_lab_bst, "Summer time:");
	lv_obj_set_pos(sys_lab_bst, 12, 407);

	lv_dropdown_set_symbol(sys_dd_theme, LV_SYMBOL_DOWN);
	lv_obj_set_width(sys_dd_theme, 270);
	lv_dropdown_set_options(sys_dd_theme, SCR_OPT_LIST );
	lv_obj_move_foreground(sys_dd_theme);
	lv_obj_set_pos(sys_dd_theme, 200, 450);
	lv_label_set_text(sys_lab_style, "GUI Theme:");
	lv_obj_set_pos(sys_lab_style, 12, 462);

	lv_obj_set_size(sys_cp_colour, 100, 100);
	lv_obj_set_pos(sys_cp_colour, 200, 510);
	lv_obj_set_style_arc_width(sys_cp_colour, 16, LV_PART_MAIN);
	lv_obj_set_style_pad_left(sys_cp_colour, 6, LV_PART_KNOB);
	lv_obj_set_style_pad_right(sys_cp_colour, 6, LV_PART_KNOB);
	lv_obj_set_style_pad_top(sys_cp_colour, 6, LV_PART_KNOB);
	lv_obj_set_style_pad_bottom(sys_cp_colour, 6, LV_PART_KNOB);

	lv_obj_add_event_cb(sys_cp_colour, cp_change_action, LV_EVENT_VALUE_CHANGED, NULL);
	cpu0_globals->gui.colour_changed |= CW_C_CHANGE;	/* Forces Colour Wheel to be set to configured value at startup */

	lv_label_set_text(sys_lab_colour, "GUI Colour:");
	lv_obj_set_pos(sys_lab_colour, 12, 545);


	lv_textarea_set_one_line(sys_ta_macs0, pdTRUE);
	lv_textarea_set_max_length(sys_ta_macs0, 4);
	lv_obj_set_width(sys_ta_macs0, 64);
	lv_obj_set_pos(sys_ta_macs0, 590, 10);
	lv_textarea_set_accepted_chars(sys_ta_macs0, hexkeys);
	lv_obj_add_event_cb(sys_ta_macs0, kb_ta_action, LV_EVENT_CLICKED, NULL);
	lv_textarea_set_one_line(sys_ta_macs1, pdTRUE);
	lv_textarea_set_max_length(sys_ta_macs1, 4);
	lv_obj_set_width(sys_ta_macs1, 64);
	lv_obj_set_pos(sys_ta_macs1, 655, 10);
	lv_textarea_set_accepted_chars(sys_ta_macs1, hexkeys);
	lv_obj_add_event_cb(sys_ta_macs1, kb_ta_action, LV_EVENT_CLICKED, NULL);
	lv_textarea_set_one_line(sys_ta_macs2, pdTRUE);
	lv_textarea_set_max_length(sys_ta_macs2, 4);
	lv_obj_set_width(sys_ta_macs2, 64);
	lv_obj_set_pos(sys_ta_macs2, 720, 10);
	lv_textarea_set_accepted_chars(sys_ta_macs2, hexkeys);
	lv_obj_add_event_cb(sys_ta_macs2, kb_ta_action, LV_EVENT_CLICKED, NULL);
	lv_textarea_set_one_line(sys_ta_macs3, pdTRUE);
	lv_textarea_set_max_length(sys_ta_macs3, 4);
	lv_obj_set_width(sys_ta_macs3, 64);
	lv_obj_set_pos(sys_ta_macs3, 785, 10);
	lv_textarea_set_accepted_chars(sys_ta_macs3, hexkeys);
	lv_obj_add_event_cb(sys_ta_macs3, kb_ta_action, LV_EVENT_CLICKED, NULL);
	lv_textarea_set_one_line(sys_ta_macs4, pdTRUE);
	lv_textarea_set_max_length(sys_ta_macs4, 4);
	lv_obj_set_width(sys_ta_macs4, 64);
	lv_obj_set_pos(sys_ta_macs4, 850, 10);
	lv_textarea_set_accepted_chars(sys_ta_macs4, hexkeys);
	lv_obj_add_event_cb(sys_ta_macs4, kb_ta_action, LV_EVENT_CLICKED, NULL);
	lv_textarea_set_one_line(sys_ta_macs5, pdTRUE);
	lv_textarea_set_max_length(sys_ta_macs5, 4);
	lv_obj_set_width(sys_ta_macs5, 64);
	lv_obj_set_pos(sys_ta_macs5, 915, 10);
	lv_textarea_set_accepted_chars(sys_ta_macs5, hexkeys);
	lv_obj_add_event_cb(sys_ta_macs5, kb_ta_action, LV_EVENT_CLICKED, NULL);
	lv_label_set_text(sys_lab_macs, "MAC Address:");
	lv_obj_set_pos(sys_lab_macs, 480, 22);

	lv_obj_add_event_cb(sys_btn_update, sys_btn_action, LV_EVENT_CLICKED, NULL );
	lv_obj_set_pos(sys_btn_update, 800, 545);
	tmp_label = lv_label_create(sys_btn_update);
	lv_label_set_text(tmp_label, "Update Settings");

	sys_scrupdate();
}
/*
 *
 *
 * END SYSTEM CONFIGURATION
 *
 *
 */
void config_create( lv_obj_t *parent ) {

	conf_sys_create( parent );
}

uint8_t update_all_cfg( void ) {

	uint8_t				update_req = pdFALSE;
	sysmsg_q_t			msg = { SAVE_CONFIG, pdFALSE, NULL };

	if( update_sys_cfg() ) update_req = pdTRUE;
	if( update_req ) q_sys_action( &msg );
	return update_req;
}
