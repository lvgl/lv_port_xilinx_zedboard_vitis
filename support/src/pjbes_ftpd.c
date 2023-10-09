#include "network.h"
#include "pjbes_ftpd.h"

#include "ff_headers.h"
#include "ff_stdio.h"
#include "support.h"

static uint8_t				parse_ftp_ctl( char *in_buf, struct ftp_session *session );
static uint8_t				send_ctl_response( int resp_id, char *resp_text, struct ftp_session *session );
static uint8_t				send_data( int mode, char *buffer, int data_len, struct ftp_session *session );
static uint8_t				execute_cmd( struct ftp_cmds *cmd_ptr, char *arg_ptr, struct ftp_session *session );
static void				ftp_connection( void *s  );

/* Pointer to configuration data */
extern config_datp_t		conf_p;

struct ftp_cmds ftp_cmds_def[] = {

//  cmd_text, cmd_id
	{ "USER", 1  },			// Implemented commands start here
	{ "PASS", 2  },
	{ "QUIT", 3  },
	{ "PORT", 4  },
	{ "MODE", 5  },
	{ "RETR", 6  },
	{ "STOR", 7  },
	{ "ABOR", 8  },
	{ "DELE", 9  },
	{ "LIST", 10 },
	{ "SITE", 11 },
	{ "TYPE", 12 },
	{ "STRU", 13 },			// Implemented commands end here
	{ "PASV", 14 },			// These commands may be added in the future - for now they will return unimplemented
	{ "STOU", 15 },
	{ "APPE", 16 },
	{ "ALLO", 17 },
	{ "REST", 18 },
	{ "RNFR", 19 },
	{ "RNTO", 20 },
	{ "RMD" , 21 },
	{ "MKD" , 22 },
	{ "PWD" , 23 },
	{ "NLST", 24 },
	{ "SYST", 25 },
	{ "STAT", 26 },
	{ "HELP", 27 },
	{ "NOOP", 28 },
	{ "XPWD", 29 },
	{ "CWD" , 30 },
	{ "CDUP", 31 },
	{ "XRMD", 32 },
	{ "OPTS", 33 },
	{ STR_NULL, 0 }	// Terminating Entry

};

void ftp_server( void *p ) {

	struct freertos_sockaddr	BindAddress;
	Socket_t					ListeningSocket, SessionSocket;
	struct ftp_session			sessions[FTP_MAX_CONNECTIONS], *current_session;

	memset( sessions, 0, sizeof(struct ftp_session)*FTP_MAX_CONNECTIONS );
	ListeningSocket = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP );
	/* Set a time out so accept() will just wait for a connection. */
	memset(&BindAddress, 0, sizeof(BindAddress));
	BindAddress.sin_port = FreeRTOS_htons(FTP_PORT);
	BindAddress.sin_family = FREERTOS_AF_INET;

	FreeRTOS_bind( ListeningSocket, (struct freertos_sockaddr *)&BindAddress, sizeof(BindAddress) );
	FreeRTOS_listen(ListeningSocket, FTP_MAX_CONNECTIONS);

	while( 1 ) {
		/* Wait for a client to connect. */
		SessionSocket = FreeRTOS_accept( ListeningSocket, NULL, NULL );
		if( SessionSocket != FREERTOS_INVALID_SOCKET ) {
			current_session = sessions;
			for( uint8_t i = 0; i < FTP_MAX_CONNECTIONS; i++ ) {
				if( current_session->active == pdFALSE ) {
					current_session->data_sock = FREERTOS_INVALID_SOCKET;
					current_session->ctrl_sock = SessionSocket;
					current_session->quit_flag = pdFALSE;
					current_session->user_found = pdFALSE;
					current_session->user_ok = pdFALSE;
					current_session->trans_type = FTP_TRANST_ASC;
					strcpy( current_session->file_path, SYS_PATH );
					current_session->active = pdTRUE;
					break;
				}
				current_session++;
			}
			/* Spawn a task to handle the connection. */
			xTaskCreate( (TaskFunction_t)ftp_connection, "FTP_conn", THREAD_STACKSIZE, ( void * )current_session, FTP_PRIORITY, NULL );
		}
	}
}

static void ftp_connection( void *s  ) {

	uint8_t				ret;
	struct ftp_session	*session = (struct ftp_session*)s;
	char				ctl_in_buf[FTP_CTL_BUF_SIZ];
	TickType_t			ReceiveTimeOut = pdMS_TO_TICKS( 600000 );	// 10 Minute time-out connection
	TickType_t			SendTimeOut = pdMS_TO_TICKS( 600000 );
	SocketSet_t 		FD_Set;
	BaseType_t			sel_ret;
	sysmsg_q_t			msg = { 0, pdFALSE, NULL };

	if( send_ctl_response( FTP_RDY_USR, STR_NULL, session ) == CTRL_NOK ) vTaskDelete( NULL );

	FD_Set = FreeRTOS_CreateSocketSet();
	FreeRTOS_setsockopt( session->ctrl_sock, 0, FREERTOS_SO_RCVTIMEO, &ReceiveTimeOut, sizeof( ReceiveTimeOut ) );
	FreeRTOS_setsockopt( session->ctrl_sock, 0, FREERTOS_SO_SNDTIMEO, &SendTimeOut, sizeof( SendTimeOut ) );
	FreeRTOS_FD_SET( session->ctrl_sock, FD_Set, eSELECT_READ | eSELECT_EXCEPT );
	while( 1 ) {
		if( FreeRTOS_select( FD_Set, FTP_SESSION_TIMEOUT ) != 0 ) {
			sel_ret = FreeRTOS_FD_ISSET( session->ctrl_sock, FD_Set );
			if( sel_ret == eSELECT_READ ) {	// We are only interested in reads, if eSELECT_EXCEPT it will be a disconnection
				if( (ret = parse_ftp_ctl( ctl_in_buf, session )) != CMD_CONT ) {
					switch( ret ) {

						case CMD_QUIT:
							/* Initiate a shutdown in case it has not already been initiated. */
							sock_graceful_shutdown( &session->ctrl_sock );
							break;

						case CMD_RESET:
							msg.id = INF_FTP_NEW_CONF_RXD;
							msg.extra_data = NULL;
							q_sysmsg( &msg );
							vTaskDelay(pdMS_TO_TICKS(2000));
							send_ctl_response( FTP_CTRL_CLOSE, "New Configuration File - System Rebooting!", session );
							vTaskDelay(pdMS_TO_TICKS(2000));
							sock_graceful_shutdown( &session->ctrl_sock );
							vTaskDelay(pdMS_TO_TICKS(1000));
							msg.id = SYS_REBOOT;
							msg.extra_data = " FTP";
							q_sys_action( &msg ); // If we received a new configuration file reset the system
							break;

						case CMD_NEWFW:
							msg.id = INF_FTP_NEW_FW_RXD;
							msg.extra_data = NULL;
							q_sysmsg( &msg );
							vTaskDelay(pdMS_TO_TICKS(2000));
							send_ctl_response( FTP_CTRL_CLOSE, "New Firmware File - System Rebooting!", session );
							vTaskDelay(pdMS_TO_TICKS(1000));
							sock_graceful_shutdown( &session->ctrl_sock );
							vTaskDelay(pdMS_TO_TICKS(1000));
							msg.id = SYS_REBOOT;
							msg.extra_data = " FTP";
							q_sys_action( &msg ); // If we received a new firmware file reset the system
							break;

						case CTRL_NOK:
							send_ctl_response( FTP_NOT_AVBL, "Server connection timed out.", session );
							/* Initiate a shutdown in case it has not already been initiated. */
							vTaskDelay(pdMS_TO_TICKS(1000));
							sock_graceful_shutdown( &session->ctrl_sock );
							break;
					}
					session->active = pdFALSE;
					FreeRTOS_DeleteSocketSet( FD_Set );
					vTaskDelete( NULL );
				}
			} else {
				send_ctl_response( FTP_NOT_AVBL, "Server connection lost.", session );
				/* Initiate a shutdown in case it has not already been initiated. */
				vTaskDelay(pdMS_TO_TICKS(1000));
				sock_graceful_shutdown( &session->ctrl_sock );
				session->active = pdFALSE;
				FreeRTOS_DeleteSocketSet( FD_Set );
				vTaskDelete( NULL );
			}
		} else {
			send_ctl_response( FTP_NOT_AVBL, "Server connection timed out.", session );
			/* Initiate a shutdown in case it has not already been initiated. */
			vTaskDelay(pdMS_TO_TICKS(1000));
			sock_graceful_shutdown( &session->ctrl_sock );
			session->active = pdFALSE;
			FreeRTOS_DeleteSocketSet( FD_Set );
			vTaskDelete( NULL );
		}
	}
}

static uint8_t parse_ftp_ctl( char *in_buf, struct ftp_session *session ) {

	uint8_t				ret;
	int					bytes_read, cnt;
	char				*rd_ptr, *wr_ptr;
	char				cmd_buf[16];
	struct ftp_cmds		*temp_ptr;

	rd_ptr = in_buf;
	wr_ptr = cmd_buf;
	memset( in_buf, 0, FTP_CTL_BUF_SIZ );							// Zero buffer before read
	bytes_read = FreeRTOS_recv(session->ctrl_sock, in_buf, FTP_CTL_BUF_SIZ, 0 );	// Read data into buffer
	if( bytes_read < 1 ) return pdFALSE;
	cnt = 0;
	while( !isspace( *((unsigned char*)rd_ptr) ) && cnt < (sizeof(cmd_buf)-1) ) {		// Extract first argument
		*wr_ptr = toupper( *rd_ptr );
		wr_ptr++;
		rd_ptr++;
		cnt++;
	}
	*wr_ptr = 0; // Terminate command as a string

	temp_ptr = &ftp_cmds_def[0];  // Point to the defined command list in header file

	while( strlen(temp_ptr->cmd_text)  ) {		// If it is a known command find the cmd_id

		if( strcmp( cmd_buf, temp_ptr->cmd_text ) ) {
			temp_ptr++;
		} else {

			break;
		}  // Command found;
	}

	rd_ptr++;	// Point to the next argument should it be required for further processing by the command parser

	ret = execute_cmd( temp_ptr, rd_ptr, session );

	return ret;
}


static uint8_t send_ctl_response( int resp_id, char* resp_text, struct ftp_session *session ) {	// resp_id is defined in header file and resp_text is passed
															// to the function for extra information to the client.
	int					reply_len = 0;							// Responses that are not required just return.
	char				ctl_out_buf[FTP_CTL_BUF_SIZ];
	sysmsg_q_t			msg = { 0, pdFALSE, NULL };

	memset( ctl_out_buf, 0, FTP_CTL_BUF_SIZ );
	switch( resp_id ) {

		case FTP_RSTART_MARK:
			break; // Not Implemented

		case FTP_RDY_SOON:
			break; // Not Implemented

		case FTP_DCO_ST_TRANS:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Data connection open - transfer starting.\r\n", resp_id );
			break;

		case FTP_FOK_DCO:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Opening %s mode data connection for transfer - %s.\r\n", resp_id, session->trans_type,
			  resp_text );
			break;

		case FTP_CMD_OK:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d %s Command OK.\r\n", resp_id, resp_text );
			break;

		case FTP_CMD_NOT_REQ:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Command %s not implemented, not required for this device.\r\n", resp_id, resp_text );
			break;

		case FTP_SYS_STAT:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Transfer Mode: %s    Current User: %s.\r\n", resp_id, session->trans_type, resp_text );
			break;

		case FTP_DIR_STAT:
			break; // Not Implemented

		case FTP_FILE_STAT:
			break; // Not Implemented

		case FTP_HELP_MSG:
			break; // Not Implemented

		case FTP_SYS_NAME:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d %s - %s\r\n", resp_id, FTP_HARDWARE_NAME, conf_p->Sys_Name );
			break;

		case FTP_RDY_USR:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "220 PJBES FTP Server V%s Build Date: %s.\r\n", FTP_VERSION, FTP_BDATE );
			break;

		case FTP_CTRL_CLOSE:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Service Terminating - %s.\r\n", resp_id, resp_text );
			msg.id = INF_FTP_CON_CLOSE;
			char t_buf[33];
			msg.extra_data = itoa( resp_id, t_buf, 10 );
			q_sysmsg( &msg );
			break;

		case FTP_SYNTAX_ERR:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Syntax Error in command.\r\n", resp_id );
			break;

		case FTP_UOK_PASS_REQ:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Password required for %s.\r\n", resp_id, session->user_name );
			break;

		case FTP_NOT_LOGIN:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d User NOT logged in. %s\r\n", resp_id, resp_text );
			msg.id = INF_FTP_LOGIN_FAIL;
			msg.extra_data = resp_text;
			q_sysmsg( &msg );
			break;

		case FTP_USR_LOGIN_OK:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d User %s logged in OK.\r\n", resp_id, resp_text );
			msg.id = INF_FTP_LOGIN_OK;
			msg.extra_data = resp_text;
			q_sysmsg( &msg );
			break;

		case FTP_SUC_DCC:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Transfer successful closing data connection.\r\n", resp_id );
			break;

		case FTP_FILE_NO_FOUND:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d File %s not found.\r\n", resp_id, resp_text );
			break;

		case FTP_BAD_CMD_SEQ:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Command Sequence error - %s.\r\n", resp_id, resp_text );
			break;

		case FTP_DCO_FAIL:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Failed to open data connection - %s.\r\n", resp_id, resp_text );
			break;

		case FTP_NOT_AVBL:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Service unavailable - %s.\r\n", resp_id, resp_text );
			break;

		case FTP_DCO_NO_TRANS:
			break;

		case FTP_TO_PASV:
			break;

		case FTP_REQ_FACT_OK:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d %s Command Successful.\r\n", resp_id, resp_text );
			break;

		case FTP_PATH_CREAT:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d \"%s\" is current directory.\r\n", resp_id, resp_text );
			break;

		case FTP_NEED_ACNT:
			break;

		case FTP_REQ_FACT_PEND:
			break;

		case FTP_TRANS_ABOR_DCC:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Transfer aborted data connection closing - %s.\r\n", resp_id, resp_text );
			break;

		case FTP_NO_ACT_FBSY:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d %s.\r\n", resp_id, resp_text );
			break;

		case FTP_ABOR_LOC_ERR:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Aborted data connection closing - %s.\r\n", resp_id, resp_text );
			break;

		case FTP_NO_SPACE:
			break;

		case FTP_SYNTAX_ERR_PARAM:
			break;

		case FTP_CMD_NO_IMP:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Command not implemented - %s.\r\n", resp_id, resp_text );
			break;

		case FTP_NO_IMP_PARAM:
			break;

		case FTP_ACT_TO_STOR:
			break;

		case FTP_PAGE_UNKNOWN:
			break;

		case FTP_ABOR_NO_SPACE:
			break;

		case FTP_BAD_FNAME:
			snprintf( ctl_out_buf, sizeof(ctl_out_buf), "%d Bad file name - %s\r\n", resp_id, resp_text );
			break;

		default:
			break;

	}
	reply_len = strlen( ctl_out_buf );
	if( reply_len > 0 ) {
		if( FreeRTOS_send(session->ctrl_sock, ctl_out_buf, reply_len, 0) < 0 ) {
			return CTRL_NOK;
		}
	}
	return CTRL_OK;
}


static uint8_t send_data( int mode, char* buffer, int data_len, struct ftp_session *session ) {	//sends the contents of the buffer using the data_len parameter
															//to define the length - mode is for future enhancements.
	if( session->data_sock == FREERTOS_INVALID_SOCKET ) {
		if( send_ctl_response( FTP_TRANS_ABOR_DCC, "No data socket!", session ) == CTRL_NOK ) return CTRL_NOK;
	}
	if( FreeRTOS_send(session->data_sock, buffer, data_len, 0) < 0 ) {
		if( send_ctl_response( FTP_TRANS_ABOR_DCC, "Send Failed!", session ) == CTRL_NOK ) return CTRL_NOK;
	}
	return CTRL_OK;
}

static uint8_t execute_cmd( struct ftp_cmds *cmd_ptr, char *arg_ptr, struct ftp_session *session ) {

	int							check_result, rd_loop, wr_status = 0;
	int							bytes_read = 1, bytes_written = 0, t_written;
	char						*crunch_ptr, *rd_ptr, *wr_ptr, *data_io_buf = NULL;
	struct freertos_sockaddr	peer_address, local_address;
	char						arg_2[257], full_path[257], crunch_buf[320], *dir_reg;
	TickType_t					TRTimeOut = pdMS_TO_TICKS( 10000 );
	FF_FILE						*trans_fd;
	FF_FindData_t 				*pxFindStruct = NULL;
	FF_Stat_t 					xStat;
    WinProperties_t 			WinProps;
    time_t						tmp_time;
	sysmsg_q_t					msg = { 0, pdFALSE, NULL };

	wr_ptr = arg_2;
	switch( cmd_ptr->cmd_id ) {

		case USER:
			session->user_seq = pdTRUE;
			session->user_found = pdFALSE;
			rd_loop = 0;
			while( !isspace( *((unsigned char*)arg_ptr) ) && rd_loop < (sizeof(arg_2)-1) ) {	// Locate second argument
				*wr_ptr = *arg_ptr;
				wr_ptr++;
				arg_ptr++;
				rd_loop++;
			}
			*wr_ptr = 0;
			//Find user and get password from data base
			if( !strcmp( "Admin", arg_2 ) ) session->user_found = pdTRUE;
			strcpy( session->user_name, arg_2 );												//Save user_name in a global
			if( send_ctl_response( FTP_UOK_PASS_REQ, session->user_name, session ) == CTRL_NOK ) return CTRL_NOK;
			break;

		case PASS:
			if( session->user_found ) {
				rd_loop = 0;
				while( !isspace( *((unsigned char*)arg_ptr) )  && rd_loop < (sizeof(arg_2)-1) ) {	// Locate second argument
					*wr_ptr = *arg_ptr;
					wr_ptr++;
					arg_ptr++;
					rd_loop++;
				}
				*wr_ptr = 0;
				hash_password( arg_2, crunch_buf );
				if( ( !strcmp( crunch_buf, conf_p->pword ) ) ) { //Check password
					if( send_ctl_response( FTP_USR_LOGIN_OK, session->user_name, session ) == CTRL_NOK ) return CTRL_NOK;
					session->user_ok = pdTRUE;
					session->user_seq = pdFALSE;
					break;
				}
			}
			if( !session->user_seq ) {			// Check command sequence correct
				if( send_ctl_response( FTP_BAD_CMD_SEQ, "Please login with USER first", session ) == CTRL_NOK ) return CTRL_NOK;
			} else if( send_ctl_response( FTP_NOT_LOGIN, session->user_name, session ) == CTRL_NOK ) return CTRL_NOK;
			session->user_ok = pdFALSE;
			session->user_seq = pdFALSE;
			memset( session->user_name, 0, sizeof( session->user_name ) );
			break;

		case QUIT:
			if( send_ctl_response( FTP_CTRL_CLOSE, "Goodbye", session ) == CTRL_NOK ) return CTRL_NOK;
			return CMD_QUIT;

		case PORTF:
			rd_loop = 0;
			while( !isspace( *((unsigned char*)arg_ptr) ) && rd_loop < (sizeof(arg_2)-1) ) {			// Locate second argument
				*wr_ptr = *arg_ptr;
				wr_ptr++;
				arg_ptr++;
				rd_loop++;
			}
			*wr_ptr = 0;									// info arrives in format 10,1,1,2,23,33
			wr_ptr = (char *)&peer_address.sin_address.ulIP_IPv4;		// The 10,1,1,2 denotes the IP address
			rd_ptr = arg_2;									// The 23,33 Denotes the 2 bytes of the TCP PORT network byte order
			for( rd_loop = 0; rd_loop < 6; rd_loop++ ) {
				memset( crunch_buf, 0, sizeof( crunch_buf ) );
				crunch_ptr = crunch_buf;
				while( *rd_ptr != ',' && *rd_ptr != 0 ) {
					*crunch_ptr = *rd_ptr;
					crunch_ptr++;
					rd_ptr++;
				}
				rd_ptr++;
				if( rd_loop < 4 ) {
					*wr_ptr = atoi( crunch_buf );
					wr_ptr++;
				} else if( rd_loop == 4 ) {
					wr_ptr = (char *)&peer_address.sin_port;
					*wr_ptr = atoi( crunch_buf );
					wr_ptr++;
				} else *wr_ptr = atoi( crunch_buf );
			}

			/* Create socket */
			session->data_sock = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP );
			if( session->data_sock == FREERTOS_INVALID_SOCKET) {
				if( send_ctl_response( FTP_DCO_FAIL, "Socket Failure", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			local_address.sin_address.ulIP_IPv4 = FreeRTOS_GetIPAddress( );
			local_address.sin_port = FreeRTOS_htons( FTP_TRANS_PORT );		/* Bind to FTP transfer port number */
			local_address.sin_family = FREERTOS_AF_INET;

			FreeRTOS_bind( session->data_sock, &local_address, sizeof( local_address ) );

			/* Set the window and buffer sizes. */
			WinProps.lTxBufSize = ipconfigFTP_TX_BUFSIZE;
			WinProps.lTxWinSize = ipconfigFTP_TX_WINSIZE;
			WinProps.lRxBufSize = ipconfigFTP_RX_BUFSIZE;
			WinProps.lRxWinSize = ipconfigFTP_RX_WINSIZE;
			FreeRTOS_setsockopt( session->data_sock, 0, FREERTOS_SO_WIN_PROPERTIES, ( void * ) &WinProps,	sizeof( WinProps ) );

			FreeRTOS_setsockopt( session->data_sock, 0, FREERTOS_SO_RCVTIMEO, &TRTimeOut, sizeof( TRTimeOut ) );
			FreeRTOS_setsockopt( session->data_sock, 0, FREERTOS_SO_SNDTIMEO, &TRTimeOut, sizeof( TRTimeOut ) );
			peer_address.sin_family = FREERTOS_AF_INET;
			if( FreeRTOS_connect( session->data_sock, &peer_address, sizeof(peer_address) ) < 0 ) {
				if( send_ctl_response( FTP_DCO_FAIL, "Connect Failure", session ) == CTRL_NOK ) return CTRL_NOK;
				sock_graceful_shutdown( &session->data_sock );
				session->data_sock = FREERTOS_INVALID_SOCKET;
				break;
			}
			if( send_ctl_response( FTP_CMD_OK, cmd_ptr->cmd_text, session ) == CTRL_NOK ) return CTRL_NOK;
			break;

		case LIST:
		case NLST:
			if( !session->user_ok ) {
				if( send_ctl_response( FTP_NOT_LOGIN, "Please login with USER & PASS commands", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			if( session->data_sock == FREERTOS_INVALID_SOCKET ) {
				if( send_ctl_response( FTP_TRANS_ABOR_DCC, STR_NULL, session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			rd_loop = 0;
			while( !isspace( *((unsigned char*)arg_ptr) ) && rd_loop < (sizeof(arg_2)-1) ) {	// Locate second argument
				*wr_ptr = *arg_ptr;
				wr_ptr++;
				arg_ptr++;
				rd_loop++;
			}
			*wr_ptr = 0;
			if( send_ctl_response( FTP_FOK_DCO, "File Listing..", session ) == CTRL_NOK ) return CTRL_NOK;
		    pxFindStruct = ( FF_FindData_t * ) pvPortMalloc( sizeof( FF_FindData_t ) );
		    memset( pxFindStruct, 0, sizeof(FF_FindData_t));
		    data_io_buf = pvPortMalloc( FTP_DATA_BUF_SIZ );
		    ff_getcwd( session->file_path, sizeof(session->file_path) );
		    if( ff_findfirst( session->file_path, pxFindStruct ) == 0 ) {
				do {
					if( pxFindStruct->ucAttributes & (FF_FAT_ATTR_HIDDEN | FF_FAT_ATTR_SYSTEM) ) continue;
					if( pxFindStruct->ucAttributes & FF_FAT_ATTR_DIR ) {
						dir_reg = "Directory";
					} else dir_reg = "File";
					if( !strcmp( arg_2, "-l" ) ) {
						char time_buf[30];
						struct tm	tmp_tm;
						memset( &tmp_tm, 0, sizeof(struct tm) );
						tmp_tm.tm_hour = pxFindStruct->xDirectoryEntry.xModifiedTime.Hour;
						tmp_tm.tm_min = pxFindStruct->xDirectoryEntry.xModifiedTime.Minute;
						tmp_tm.tm_sec =  pxFindStruct->xDirectoryEntry.xModifiedTime.Second;
						tmp_tm.tm_mday = pxFindStruct->xDirectoryEntry.xModifiedTime.Day;
						tmp_tm.tm_mon = pxFindStruct->xDirectoryEntry.xModifiedTime.Month - 1;
						tmp_tm.tm_year = pxFindStruct->xDirectoryEntry.xModifiedTime.Year - 1900;
						tmp_time = mktime( &tmp_tm );
						ctime_r(&tmp_time, time_buf);
						snprintf(data_io_buf, FTP_DATA_BUF_SIZ, "%-14s%-11lu  %-11s  %s", pxFindStruct->pcFileName, pxFindStruct->ulFileSize, dir_reg, time_buf );
					} else snprintf(data_io_buf, FTP_DATA_BUF_SIZ, "%-14s  %-11s\n", pxFindStruct->pcFileName, dir_reg );
					if( send_data( 1, data_io_buf, strlen(data_io_buf), session ) == CTRL_NOK ) {
				        vPortFree( pxFindStruct );
				        vPortFree( data_io_buf );
				        return CTRL_NOK;
					}
				} while( ff_findnext( pxFindStruct ) == 0 );
		        vPortFree( pxFindStruct );
		        vPortFree( data_io_buf );
				if( send_ctl_response( FTP_SUC_DCC, STR_NULL, session ) == CTRL_NOK ) return CTRL_NOK;
		    } else {
		    	if( send_ctl_response( FTP_TRANS_ABOR_DCC, "Directory listing failed!", session ) == CTRL_NOK ) {
			        vPortFree( pxFindStruct );
			        vPortFree( data_io_buf );
		    		return CTRL_NOK;
		    	}
		    }
			sock_graceful_shutdown( &session->data_sock );
			session->data_sock = FREERTOS_INVALID_SOCKET;
			break;

		case RETR:
			if( !session->user_ok ) {
				if( send_ctl_response( FTP_NOT_LOGIN, "Please login with USER & PASS commands", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			if( session->data_sock == FREERTOS_INVALID_SOCKET ) {
				if( send_ctl_response( FTP_TRANS_ABOR_DCC, STR_NULL, session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			rd_loop = 0;
			while( !isspace( *((unsigned char*)arg_ptr) ) && rd_loop < (sizeof(arg_2)-1) ) {	// Locate second argument
				*wr_ptr = *arg_ptr;
				wr_ptr++;
				arg_ptr++;
				rd_loop++;
			}
			*wr_ptr = 0;
			memset( full_path, 0, sizeof( full_path ) );
			strcpy( full_path, session->file_path );
			bytes_read = strlen(full_path);
			full_path[bytes_read] = '/';
			full_path[bytes_read+1] = 0;	// Append slash and re-terminate buffer
			strcpy( &full_path[strlen( full_path )], arg_2 );
			if( ( check_result = ff_stat( full_path, &xStat ) ) == -1 ) {
				if( send_ctl_response( FTP_FILE_NO_FOUND, arg_2, session ) == CTRL_NOK ) return CTRL_NOK;
				sock_graceful_shutdown( &session->data_sock );
				session->data_sock = FREERTOS_INVALID_SOCKET;
				break;
			}
			if( ( check_result = xStat.st_mode ) == FF_IFDIR ) {
				if( send_ctl_response( FTP_FILE_NO_FOUND, "Can not get a directory", session ) == CTRL_NOK ) return CTRL_NOK;
				sock_graceful_shutdown( &session->data_sock );
				session->data_sock = FREERTOS_INVALID_SOCKET;
				break;
			}
			trans_fd = ff_fopen( full_path, "r" );
			if( trans_fd != NULL ) {
				if( send_ctl_response( FTP_FOK_DCO, arg_2, session ) == CTRL_NOK ) return CTRL_NOK;
			} else {
				if( send_ctl_response( FTP_ABOR_LOC_ERR, "Failed to open local file!", session ) == CTRL_NOK ) return CTRL_NOK;
				sock_graceful_shutdown( &session->data_sock );
				session->data_sock = FREERTOS_INVALID_SOCKET;
				break;
			}
			data_io_buf = pvPortMalloc( FTP_DATA_BUF_SIZ );
			while( bytes_read > 0 ) {
				bytes_read = ff_fread( data_io_buf, 1, FTP_DATA_BUF_SIZ, trans_fd );
				if( bytes_read > 0 ) {
					int txd = 0, ret;
				    while( txd < bytes_read ) {
				    	ret = FreeRTOS_send(session->data_sock, data_io_buf + txd, bytes_read - txd, 0);
				    	if( ret < 0 ) break;
				    	txd += ret;
				    }
				}
			}
			vPortFree( data_io_buf );
			if( bytes_read < 0 ) {
				ff_fclose( trans_fd );
				if( send_ctl_response( FTP_CTRL_CLOSE, "File Read Error!", session ) == CTRL_NOK ) return CTRL_NOK;
				sock_graceful_shutdown( &session->data_sock );
				session->data_sock = FREERTOS_INVALID_SOCKET;
				break;
			}
			ff_fclose( trans_fd );
			if( send_ctl_response( FTP_SUC_DCC, STR_NULL, session ) == CTRL_NOK ) return CTRL_NOK;
			msg.id = INF_FTP_FILE_TXD;
			msg.extra_data = full_path;
			q_sysmsg( &msg );
			sock_graceful_shutdown( &session->data_sock );
			session->data_sock = FREERTOS_INVALID_SOCKET;
			break;

		case TYPEF:
			if( !session->user_ok ) {
				if( send_ctl_response( FTP_NOT_LOGIN, "Please login with USER & PASS commands.", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			if( toupper(*arg_ptr) == 'I' ) {
				if( send_ctl_response( FTP_CMD_OK, "TYPE I", session ) == CTRL_NOK ) return CTRL_NOK;
				session->trans_type = FTP_TRANST_BIN;
			}
			if( toupper(*arg_ptr) == 'A' ) {
				if( send_ctl_response( FTP_CMD_OK, "TYPE A", session ) == CTRL_NOK ) return CTRL_NOK;
				session->trans_type = FTP_TRANST_ASC;
			}
			break;

		case SYST:
			if( send_ctl_response( FTP_SYS_NAME, STR_NULL, session ) == CTRL_NOK ) return CTRL_NOK;
			break;

		case STAT:
			if( !session->user_ok ) {
				if( send_ctl_response( FTP_NOT_LOGIN, "Please login with USER & PASS commands.", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			if( send_ctl_response( FTP_SYS_STAT, session->user_name, session ) == CTRL_NOK ) return CTRL_NOK;
			break;

		case CWD:
			if( !session->user_ok ) {
				if( send_ctl_response( FTP_NOT_LOGIN, "Please login with USER & PASS commands.", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			rd_loop = 0;
			while( !isspace( *((unsigned char*)arg_ptr) ) && rd_loop < (sizeof(arg_2)-1) ) {	// Locate second argument
				*wr_ptr = *arg_ptr;
				wr_ptr++;
				arg_ptr++;
				rd_loop++;
			}
			*wr_ptr = 0;

			if( !ff_chdir(arg_2) ) {
				if( send_ctl_response( FTP_REQ_FACT_OK, "CWD", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			} else {
				if( send_ctl_response( FTP_FILE_NO_FOUND, "CWD", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			break;

		case STOR:
			if( !session->user_ok ) {
				if( send_ctl_response( FTP_NOT_LOGIN, "Please login with USER & PASS commands.", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			if( session->data_sock == FREERTOS_INVALID_SOCKET ) {
				if( send_ctl_response( FTP_TRANS_ABOR_DCC, STR_NULL, session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			rd_loop = 0;
			while( !isspace( *((unsigned char*)arg_ptr) ) && rd_loop < (sizeof(arg_2)-1) ) {	// Extract second argument
				if( *arg_ptr == '/' || *arg_ptr == '\\' ) {
					if( send_ctl_response( FTP_BAD_FNAME, "May only store files in the current directory.", session ) == CTRL_NOK ) return CTRL_NOK;
					break;
				} else if( !isprint(*((unsigned char*)arg_ptr)) || *arg_ptr == '*' ) {
					if( send_ctl_response( FTP_BAD_FNAME, "Invalid character in file name.", session ) == CTRL_NOK ) return CTRL_NOK;
					break;
				}
				*wr_ptr = *arg_ptr;
				wr_ptr++;
				arg_ptr++;
				rd_loop++;
			}
			*wr_ptr = 0;
			if( strlen( arg_2 ) > 12 ) {
				if( send_ctl_response( FTP_BAD_FNAME, "File name too long (8.3 DOS format only).", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			strcpy( full_path, session->file_path );
			bytes_read = strlen(full_path);
			full_path[bytes_read] = '/';
			full_path[bytes_read+1] = 0;	// Append slash and re-terminate buffer
			strcpy( &full_path[strlen(full_path)], arg_2 );
			trans_fd = ff_fopen( full_path, "w" );
			if( trans_fd != NULL ) {
				if( send_ctl_response( FTP_FOK_DCO, arg_2, session ) == CTRL_NOK ) return CTRL_NOK;
			} else {
				if( send_ctl_response( FTP_ABOR_LOC_ERR, "Failed to open local file!", session ) == CTRL_NOK ) return CTRL_NOK;
				sock_graceful_shutdown( &session->data_sock );
				session->data_sock = FREERTOS_INVALID_SOCKET;
				break;
			}
			data_io_buf = pvPortMalloc( FTP_DATA_BUF_SIZ );
			while( (bytes_read = FreeRTOS_recv(session->data_sock, data_io_buf, FTP_DATA_BUF_SIZ, 0)) > 0 ) {
				if( bytes_read > 0 ) {
					t_written = 0;
					while( t_written < bytes_read ) {
						bytes_written = ff_fwrite( data_io_buf + t_written, 1, (bytes_read - t_written), trans_fd  );
						if( bytes_written < 0 ) break;
						t_written += bytes_written;
					}
				}
				if( bytes_written < 0 ) {
					if( send_ctl_response( FTP_TRANS_ABOR_DCC, itoa( wr_status, crunch_buf, 10 ), session ) == CTRL_NOK ) return CTRL_NOK;
					sock_graceful_shutdown( &session->data_sock );
					session->data_sock = FREERTOS_INVALID_SOCKET;
					ff_fclose( trans_fd );
					vPortFree( data_io_buf );
					return CMD_CONT;
				}
			}
			vPortFree( data_io_buf );
			ff_fclose( trans_fd );
			if( send_ctl_response( FTP_SUC_DCC, STR_NULL, session ) == CTRL_NOK ) return CTRL_NOK;
			msg.id = INF_FTP_NEW_FILE_RXD;
			msg.extra_data = full_path;
			q_sysmsg( &msg );
			sock_graceful_shutdown( &session->data_sock );
			session->data_sock = FREERTOS_INVALID_SOCKET;
			for( rd_loop = 0; rd_loop < strlen(arg_2); rd_loop++ ) arg_2[rd_loop] = toupper(arg_2[rd_loop]);
			if( !strcmp( arg_2, CONFIG_FNAME ) ) {
				return CMD_RESET;
			}
			if( !strcmp( arg_2, FW_FILE ) ) {
				return CMD_NEWFW;
			}
			break;

		case DELE:
			if( !session->user_ok ) {
				if( send_ctl_response( FTP_NOT_LOGIN, "Please login with USER & PASS commands", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			rd_loop = 0;
			while( !isspace( *((unsigned char*)arg_ptr) ) && rd_loop < (sizeof(arg_2)-1) ) {	// Locate second argument
				*wr_ptr = *arg_ptr;
				wr_ptr++;
				arg_ptr++;
				rd_loop++;
			}
			*wr_ptr = 0;
			strcpy( full_path, session->file_path );
			bytes_read = strlen(full_path);
			full_path[bytes_read] = '/';
			full_path[bytes_read+1] = 0;	// Append slash and re-terminate buffer
			strcpy( &full_path[strlen(full_path)], arg_2 );
			if( ( check_result = ff_stat( full_path, &xStat ) ) == -1 ) {
				if( send_ctl_response( FTP_FILE_NO_FOUND, arg_2, session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			if(  ( check_result = xStat.st_mode ) == 0x4000 ) {
				if( send_ctl_response( FTP_NO_ACT_FBSY, "Access Denied - Not allowed to delete directories", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			if( ff_remove( full_path ) == 0 ) {
				snprintf( crunch_buf, sizeof(crunch_buf), "File %s deleted OK", arg_2 );
				msg.id = INF_FTP_FILE_DELETED;
				msg.extra_data = full_path;
				q_sysmsg( &msg );
				if( send_ctl_response( FTP_REQ_FACT_OK, crunch_buf, session ) == CTRL_NOK ) return CTRL_NOK;
			} else if( send_ctl_response( FTP_NO_ACT_FBSY, "File delete failed", session ) == CTRL_NOK ) return CTRL_NOK;
			break;

		case PWD:
		case XPWD:
			if( !session->user_ok ) {
				if( send_ctl_response( FTP_NOT_LOGIN, "Please login with USER & PASS commands", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			ff_getcwd( session->file_path, sizeof(session->file_path) );
			if( send_ctl_response( FTP_PATH_CREAT, session->file_path, session ) == CTRL_NOK ) return CTRL_NOK;
			break;

		case OPTS:
			if( send_ctl_response( FTP_CMD_NO_IMP, "OPTS", session ) == CTRL_NOK ) return CTRL_NOK;
			break;

		default:
			if( !session->user_ok ) {
				if( send_ctl_response( FTP_NOT_LOGIN, "Please login with USER & PASS commands", session ) == CTRL_NOK ) return CTRL_NOK;
				break;
			}
			if( cmd_ptr->cmd_id > 0 && cmd_ptr->cmd_id < LAST_CMD ) {
				if( send_ctl_response( FTP_CMD_NOT_REQ, cmd_ptr->cmd_text, session ) == CTRL_NOK ) return CTRL_NOK;		/* If command not implemented tell client */
			} else if( send_ctl_response( FTP_SYNTAX_ERR, STR_NULL, session ) == CTRL_NOK ) return CTRL_NOK;				/* If command not valid tell client */
			break;
	}

	return CMD_CONT;
}

