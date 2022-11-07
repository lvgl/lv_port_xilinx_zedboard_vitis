/*
 * sysmsg.h
 *
 *  Created on: 8 Feb 2019
 *      Author: peterb
 */

#ifndef SRC_SYSMSG_H_
#define SRC_SYSMSG_H_
#include <stdint.h>

/* Our Defines */
#define LOG_PATH		SYS_PATH"Logs/"
#define LOG_PATHDIR	SYS_PATH"Logs"
#define LOG_FILE_PATH	LOG_PATH"log.log"

#define LOG_WR_OK		0

// Define system message ID's here then create a text message entry in the array at the top of gui_main.c
#define MSG_STARTUP				0x0000		// This first block are all messages for display on startup screen.
#define ERR_I2CINIT_FAIL			0x0001
#define ERR_TFRTC_FAIL			0x0002
#define INF_SDINIT_OK				0x0003
#define ERR_SDINIT_FAIL			0x0004
#define INF_LDCONF					0x0005
#define INF_LDCONF_OK				0x0006
#define ERR_LDCONF_NODIR			0x0007
#define ERR_LDCONF_NOFILE			0x0008
#define ERR_LDCONF_BADCSUM		0x0009
#define ERR_LDCONF_BADLEN			0x000A
#define INF_LOADING_MAIN			0x000B
#define CPU1_OK					0x000C
#define CPU1_NOK					0x000D

#define ERR_INVALID_IP			0x0120		// This block are messages displayed in associated with the configuration screens
#define ERR_INVALID_NM			0x0121
#define ERR_INVALID_GW			0x0122
#define ERR_INVALID_DNS			0x0123
#define ERR_INVALID_NTP			0x0124
#define ERR_SAVE_CONFIG			0x0126
#define INF_SAVE_CONFIG_OK		0x0127
#define ERR_NTPC_OSOCK			0x012A
#define ERR_NTPC_SOCKTO			0x012B
#define ERR_NTPC_SOCKWR			0x012C
#define ERR_NTPC_SOCKRDTO			0x012D
#define INF_NTPC_CLKUPD			0x012E
#define INF_NTPC_SYNCOK			0x012F
#define INF_NTPC_UPD_IDU			0x0158
#define ERR_GUILOG_ALLOC_FAIL		0x0130
#define INF_USR_REBOOT			0x0133
#define INF_REBOOT_INPROG			0x0134
#define ERR_NTPC_MULTITO_FAIL		0x0135
#define ERR_NTPS_OSOCK			0x0153
#define ERR_NTPS_SOCKTO			0x0154
#define ERR_NTPS_BIND_FAIL		0x0155
#define ERR_NTPS_RECV_FAIL		0x0156
#define ERR_NTPS_SEND_FAIL		0x0157
#define INF_LOG_CLR				0x0159
#define WARN_USR_REBOOT			0x015A
#define WARN_REBOOT_INPROG		0x015B
#define INF_FTP_LOGIN_OK			0x015C
#define INF_FTP_LOGIN_FAIL		0x015D
#define INF_FTP_NEW_FILE_RXD		0x015E
#define INF_FTP_FILE_TXD			0x015F
#define INF_FTP_NEW_FW_RXD		0x0160
#define INF_FTP_NEW_CONF_RXD		0x0161
#define INF_FTP_CON_CLOSE			0x0162
#define INF_FTP_FILE_DELETED		0x0163
#define INF_USB_EVENT				0x0164
#define INF_VNC_CON_OPEN			0x0165
#define INF_VNC_CON_CLOSE			0x0166

#define LOG_DB_END					0x7FFF		// End of textual type messages defined in database do not use this ID

#define LOAD_MAIN_GUI				0x8000		// This block are for messages associated with GUI thread tasks
#define UPDATE_GUI_LOG			0x8001
#define GUI_HIDE_MOUSE_CURSOR		0x8002
#define GUI_SHOW_MOUSE_CURSOR		0x8003
#define UPDATE_GUI_THEME			0x8004
#define UPDATE_GUI_SYS_SCR		0x8005

#define SAVE_CONFIG				0xC000		// This block are for messages associated with system requests
#define CLR_LOG_REQ				0xC001		// Clear the log
#define SYS_REBOOT					0xC002		// Reboot system

#define SPAWN_SUPV0				0xE000		// This block are for messages associated with spawning tasks
#define SPAWN_NTPS					0xE001
#define SPAWN_NTPC					0xE002
#define SPAWN_USB					0xE003
#define SPAWN_GUI					0xE004
#define SPAWN_FTP					0xE005
#define SPAWN_SMSGP				0xE006

#define LOG_CLEAR_CMD				0xFFFF

typedef enum {

	log_msg_none,
	log_msg_info,
	log_msg_warn,
	log_msg_error,

} sysmsg_type_t;

typedef enum {

	log_cat_none,
	log_cat_startup,
	log_cat_cpu1,
	log_cat_main,
	log_cat_ntp,
	log_cat_sys,
	log_cat_log,
	log_cat_gui,
	log_cat_glob,
	log_cat_ftp,
	log_cat_usb,

} sysmsg_category_t;

typedef struct _sysmsg_q_t {

	uint32_t            id : 16;
	uint32_t			free_ext : 1;
	void                *extra_data;

} sysmsg_q_t;

typedef struct _sysmsg_t {

	uint16_t            id;             	// Defined message id in sysmsg.h
	sysmsg_type_t		type;				// type of message Error/info etc.
	sysmsg_category_t	category;			// category of message Main/CPU1 etc.
	uint8_t             wr_log_file : 1;	// if true write message to log file
	uint8_t 			notify_gui : 1;		// if true put the message in the GUI message queue also
	uint8_t				snmp_trap : 1;		// if true send snmp_trap
	char                *msg;           	// Pointer to defined message text in log.c

} sysmsg_t;

uint8_t 	q_sysmsg( sysmsg_q_t *msg );
sysmsg_t*	get_log_msg_by_id( uint16_t id );
void 		sysmsg_task ( void *p );

#endif /* SRC_SYSMSG_H_ */
