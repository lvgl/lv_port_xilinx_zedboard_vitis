/*
 * gui.h
 *
 *  Created on: 27 Sep 2022
 *      Author: PeterB
 */

#ifndef INCLUDE_GUI_H_
#define INCLUDE_GUI_H_
#include "lvgl.h"
#include "sysmsg.h"

/* For colour changed handling */
#define CW_C_CHANGE				0x01
#define CW_UP_CLICK				0x02

#define MAX_IP_LEN					15		/* Maximum Number of characters for IP address */

#define SCR_OPT_LIST				"Light\nDark"

#define UDAT_TA_ALPHA				0x00000001		/* Values to select correct keyboard in event handlers*/
#define UDAT_TA_NUM				0x00000002
#define UDAT_TA_ALPHA_UPPER		0x00000003

/* PJBES Defines */
#define LV_HOR_RES_MAX			(1024)
#define LV_VER_RES_MAX			(768)
#define LV_VGA_DDR_DMA_BASE		0x0F000000		// This is the DDR address the DMA hardware fetches VGA pixel data from (32MB is reserved)
#define LV_VDB_ADR          		LV_VGA_DDR_DMA_BASE
#define LV_VDB2_ADR         		(LV_VGA_DDR_DMA_BASE + (((LV_HOR_RES_MAX * LV_VER_RES_MAX)*LV_COLOR_DEPTH)>>3))


void gui_thread(void *p);
void startup_gui_create(void);
void main_gui_create(void);
void setup_keyboard( void );
void kb_ta_action( lv_event_t *event );
void log_create( lv_obj_t * parent );
void update_gui_log( void );
uint8_t q_gui_msg( sysmsg_q_t *msg );
uint8_t update_all_cfg( void );
void sys_scrupdate( void );
void config_create( lv_obj_t *parent );
void setup_scrl_bar( lv_obj_t *obj, lv_part_t part, uint8_t size );
void sysinfo_create( lv_obj_t * parent );

#endif /* INCLUDE_GUI_H_ */
