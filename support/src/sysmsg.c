/*
 * sysmsg.c
 *
 *  Created on: 8 Feb 2019
 *      Author: peterb
 */
#include "gui.h"
#include "sysmsg.h"
#include "pjbes_time.h"
#include "ff_headers.h"
#include "ff_stdio.h"
#include "support.h"

extern config_datp_t	conf_p;
extern cpu0_globals_t	*cpu0_globals;

static char *log_msg_types[] = {

	"",
	"Info  ",
	"Warn  ",
	"Error ",
};

static char *log_cat_names[] = {

	"",         //None
	"",         //Startup
	"CPU1 ",
	"Main ",
	"NTP  ",
	"Sys  ",
	"Log  ",
	"GUI  ",
	"Glob ",
	"FTP  ",
	"USB  ",
};

sysmsg_t sys_msgs[] =  {

	{ MSG_STARTUP, log_msg_none, log_cat_startup, pdFALSE, pdTRUE, pdFALSE, "Zedboard Dual Core ARM Cortex A9 Hardware Platform\nBooting..." },
	{ INF_SDINIT_OK, log_msg_info, log_cat_startup, pdFALSE, pdTRUE, pdFALSE, "SD Card Initialised Okay" },
	{ ERR_SDINIT_FAIL, log_msg_error, log_cat_startup, pdTRUE, pdTRUE, pdTRUE, "Unable to initialise SD Card!" },
	{ INF_LDCONF, log_msg_info, log_cat_startup, pdFALSE, pdTRUE, pdFALSE, "Loading Configuration File..." },
	{ INF_LDCONF_OK, log_msg_info, log_cat_startup, pdTRUE, pdTRUE, pdTRUE, "Configuration Loaded Okay" },
	{ ERR_LDCONF_NODIR, log_msg_error, log_cat_startup, pdTRUE, pdTRUE, pdTRUE, "No Configuration directory - new directory created!" },
	{ ERR_LDCONF_NOFILE, log_msg_error, log_cat_startup, pdTRUE, pdTRUE, pdTRUE, "No Configuration file - new file created!" },
	{ ERR_LDCONF_BADCSUM, log_msg_error, log_cat_startup, pdTRUE, pdTRUE, pdTRUE, "Configuration file bad checksum - new file created!" },
	{ ERR_LDCONF_BADLEN, log_msg_error, log_cat_startup, pdTRUE, pdTRUE, pdTRUE, "Configuration file bad length - new file created!" },
	{ INF_LOADING_MAIN, log_msg_none, log_cat_startup, pdFALSE, pdTRUE, pdFALSE, "Loading Main Graphical Interface..." },
	{ CPU1_OK, log_msg_info, log_cat_cpu1, pdTRUE, pdTRUE, pdTRUE, "Started Okay" },
	{ CPU1_NOK, log_msg_error, log_cat_cpu1, pdTRUE, pdTRUE, pdTRUE, "Not responding!" },
	{ ERR_INVALID_IP, log_msg_error, log_cat_glob, pdTRUE, pdTRUE, pdTRUE, "Invalid IP Address - Not Saved!" },
	{ ERR_INVALID_NM, log_msg_error, log_cat_glob, pdTRUE, pdTRUE, pdTRUE, "Invalid Network Mask - Not Saved!" },
	{ ERR_INVALID_GW, log_msg_error, log_cat_glob, pdTRUE, pdTRUE, pdTRUE, "Invalid Gateway Address - Not Saved!" },
	{ ERR_INVALID_DNS, log_msg_error, log_cat_glob, pdTRUE, pdTRUE, pdTRUE, "Invalid DNS Address - Not Saved!" },
	{ ERR_INVALID_NTP, log_msg_error, log_cat_glob, pdTRUE, pdTRUE, pdTRUE, "Invalid NTP Server Address - Not Saved!" },
	{ ERR_SAVE_CONFIG, log_msg_error, log_cat_sys, pdTRUE, pdTRUE, pdTRUE, "Configuration File Save Failed!" },
	{ INF_SAVE_CONFIG_OK, log_msg_info, log_cat_sys, pdTRUE, pdTRUE, pdTRUE, "Configuration File Updated Successfully." },
	{ ERR_NTPC_OSOCK, log_msg_error, log_cat_ntp, pdTRUE, pdTRUE, pdTRUE, "Opening client socket!" },
	{ ERR_NTPC_SOCKTO, log_msg_error, log_cat_ntp, pdTRUE, pdTRUE, pdTRUE, "Setting client socket timeout!" },
	{ ERR_NTPC_SOCKWR, log_msg_error, log_cat_ntp, pdTRUE, pdTRUE, pdTRUE, "Writing to client socket!" },
	{ ERR_NTPC_SOCKRDTO, log_msg_error, log_cat_ntp, pdTRUE, pdTRUE, pdTRUE, "Client timed out, getting time, will retry!" },
	{ INF_NTPC_UPD_INT, log_msg_info, log_cat_ntp, pdFALSE, pdTRUE, pdTRUE, "Client updated real time clock from Internet" },
	{ INF_NTPC_SYNCOK, log_msg_info, log_cat_ntp, pdFALSE, pdTRUE, pdTRUE, "Client reports clock is already synchronised..." },
	{ ERR_NTPC_MULTITO_FAIL, log_msg_error, log_cat_ntp, pdTRUE, pdTRUE, pdTRUE, "Error: NTP:  Maximum client timeouts reached - time not set from NTP server!" },
	{ ERR_GUILOG_ALLOC_FAIL, log_msg_error, log_cat_gui, pdTRUE, pdTRUE, pdTRUE, "Failed to allocate GUI Log memory!" },
	{ ERR_NTPS_OSOCK, log_msg_error, log_cat_ntp, pdTRUE, pdTRUE, pdTRUE, "Error opening server socket!" },
	{ ERR_NTPS_SOCKTO, log_msg_error, log_cat_ntp, pdTRUE, pdTRUE, pdTRUE, "Error setting server socket timeout!" },
	{ ERR_NTPS_BIND_FAIL, log_msg_error, log_cat_ntp, pdTRUE, pdTRUE, pdTRUE, "Error binding server socket!" },
	{ ERR_NTPS_RECV_FAIL, log_msg_error, log_cat_ntp, pdTRUE, pdTRUE, pdTRUE, "Server receive error!" },
	{ ERR_NTPS_SEND_FAIL, log_msg_error, log_cat_ntp, pdTRUE, pdTRUE, pdTRUE, "Server send error!" },
	{ INF_LOG_CLR, log_msg_info, log_cat_log, pdTRUE, pdTRUE, pdTRUE, "Cleared by User" },
	{ WARN_USR_REBOOT, log_msg_warn, log_cat_sys, pdTRUE, pdTRUE, pdTRUE, "Reboot Requested! - Source: " },
	{ WARN_REBOOT_INPROG, log_msg_warn, log_cat_sys, pdTRUE, pdTRUE, pdTRUE, "Reboot in progress!" },
	{ INF_FTP_LOGIN_OK, log_msg_info, log_cat_ftp, pdTRUE, pdTRUE, pdTRUE, "Login okay for user: " },
	{ INF_FTP_LOGIN_FAIL, log_msg_info, log_cat_ftp, pdTRUE, pdTRUE, pdTRUE, "Login failed for user: " },
	{ INF_FTP_NEW_FILE_RXD, log_msg_info, log_cat_ftp, pdTRUE, pdTRUE, pdTRUE, "New file received: " },
	{ INF_FTP_FILE_TXD, log_msg_info, log_cat_ftp, pdTRUE, pdTRUE, pdTRUE, "File Sent: " },
	{ INF_FTP_NEW_FW_RXD, log_msg_warn, log_cat_ftp, pdTRUE, pdTRUE, pdTRUE, "New firmware received!" },
	{ INF_FTP_NEW_CONF_RXD, log_msg_warn, log_cat_ftp, pdTRUE, pdTRUE, pdTRUE, "New configuration file received!" },
	{ INF_FTP_CON_CLOSE, log_msg_info, log_cat_ftp, pdTRUE, pdTRUE, pdTRUE, "Session ended." },
	{ INF_FTP_FILE_DELETED, log_msg_info, log_cat_ftp, pdTRUE, pdTRUE, pdTRUE, "File Deleted: " },
	{ INF_USB_EVENT, log_msg_info, log_cat_usb, pdTRUE, pdTRUE, pdTRUE, "USB Event: " },
	{ INFO_GET_NET_TIME, log_msg_info, log_cat_startup, pdFALSE, pdTRUE, pdFALSE, "Attempting to get time from Internet" },
	{ LOG_DB_END, log_msg_none, log_cat_none, pdFALSE, pdFALSE, pdFALSE, ""  } // MUST STAY LAST ENTRY!!!  ORDER of rest not crucial.
};

sysmsg_t* get_log_msg_by_id( uint16_t id ) {

	sysmsg_t *msg;

	msg = sys_msgs;

	while( msg->id !=  LOG_DB_END ) {
		if( id == msg->id ) return msg;
		msg++;
	}

	return NULL;
}

static void clear_log( void ) {

    sysmsg_q_t      msg = { 0, pdFALSE, NULL };

    ff_remove( LOG_FILE_PATH );  // Delete file
    msg.id = INF_LOG_CLR;         // Add an entry to say when it was cleared
    q_sysmsg( &msg );
}

static void process_sysmsg( sysmsg_q_t *msg ) {

	FF_FILE				*file;
	unsigned int		wr_len, written, len = 0;
	int					res;
	char				*buf = NULL, tbuf[33];
	sysmsg_t			*db_entry;
	size_t				buf_len;
	sysmsg_q_t			gmsg = { 0, pdFALSE, NULL };

    if( msg->id == LOG_CLEAR_CMD ) {
        clear_log();
        gmsg.id = UPDATE_GUI_LOG;
        gmsg.extra_data = NULL;
        q_gui_msg( &gmsg );
        return;
    }
    if( (db_entry = get_log_msg_by_id( msg->id )) == NULL ) return;
    if( db_entry->wr_log_file || db_entry->notify_gui ) {
        get_zone_time( conf_p->tz_idx, tbuf );
    	buf_len = strlen( tbuf ) + strlen(log_msg_types[db_entry->type]) + strlen(log_cat_names[db_entry->category]) +
    	  strlen( db_entry->msg ) + (msg->extra_data ? strlen( msg->extra_data ) : 0) + 4;
    	buf = pvPortMalloc( buf_len );
    	if( buf == NULL) return;
    	len = snprintf( buf, buf_len, "%s - %s%s%s%s", tbuf, log_msg_types[db_entry->type], log_cat_names[db_entry->category],
    	  db_entry->msg, msg->extra_data ? (char*)msg->extra_data : "" );
    	if( len < 1 ) return; // bail if something is wrong!
    }
    if( db_entry->wr_log_file && len) {
    	if( (res = ff_chdir( LOG_PATHDIR )) != XST_SUCCESS ) {       // Check our directory exists
    		res = ff_mkdir( LOG_PATHDIR );								// If not create it!
    	}
    	if( ((file = ff_fopen( LOG_FILE_PATH, "a" )) != NULL)  ) { 	// Normal log file open...
    		written = 0;
    		while ( written < len ) {
    			wr_len = ff_fwrite(  buf + written, 1, len - written, file );
    			written += wr_len;
    		}
    		ff_fwrite(  "\n", 1, 1, file ); // Add newline
    		res = ff_fclose( file );
    	}

        gmsg.id = UPDATE_GUI_LOG;
    	gmsg.extra_data = NULL;
    	q_gui_msg( &gmsg );
    }
	if( db_entry->notify_gui ) {
        gmsg.id = msg->id;
        gmsg.extra_data = pvPortMalloc( strlen(buf) + 1 );
        strcpy( gmsg.extra_data, buf );
        gmsg.free_ext = pdTRUE;
		q_gui_msg( &gmsg );    // Display the log message in the GUI
	}
	if( buf ) vPortFree(buf);
}

uint8_t q_sysmsg( sysmsg_q_t *msg ) {

	return xQueueSend( cpu0_globals->sysmsg_q, msg, 0 );
}

void sysmsg_task( void *p ) {

	static sysmsg_q_t		msg;

	while( 1 ) {
		xQueueReceive( cpu0_globals->sysmsg_q, &msg, portMAX_DELAY );
		process_sysmsg( &msg );
	}
}




