/*
 * oneftpd.h
 *
 *  Created on: 11/12/2018
 *      Author: peterb
 */

#ifndef ONEFTPD_H_
#define ONEFTPD_H_
#include "FreeRTOSFATConfig.h"

// Header file for PJBES FTP Server daemon

#define CMD_CONT		1
#define CMD_QUIT		2
#define CMD_RESET		3
#define CMD_NEWFW		4
#define CTRL_OK		5
#define CTRL_NOK		6

// REFERENCE RFC959 for details of FTP processes

// Defines Here
#define FTP_HARDWARE_NAME			"PJBES FTP Server\0"
#define FTP_VERSION				"1.00R"
#define FTP_BDATE   				__DATE__
#define STR_NULL					"\0"
#define FTP_TRANST_BIN			"BINARY\0"
#define FTP_TRANST_ASC			"ASCII\0"
#define FTP_CTL_BUF_SIZ			384
#define FTP_DATA_BUF_SIZ			2097152
#define FTP_PORT					21
#define FTP_TRANS_PORT			20
#define FTP_MAX_CONNECTIONS		5
#define FTP_SESSION_TIMEOUT		pdMS_TO_TICKS( 1800000 )

// Defines for FTP Responses

#define FTP_RSTART_MARK			110
#define FTP_RDY_SOON				120
#define FTP_DCO_ST_TRANS			125
#define FTP_FOK_DCO				150
#define FTP_CMD_OK					200
#define FTP_CMD_NOT_REQ			202
#define FTP_SYS_STAT				211
#define FTP_DIR_STAT				212
#define FTP_FILE_STAT				213
#define FTP_HELP_MSG				214
#define FTP_SYS_NAME				215
#define FTP_RDY_USR				220
#define FTP_CTRL_CLOSE			221
#define FTP_DCO_NO_TRANS			225
#define FTP_SUC_DCC				226
#define FTP_TO_PASV				227
#define FTP_USR_LOGIN_OK			230
#define FTP_REQ_FACT_OK			250
#define FTP_PATH_CREAT			257
#define FTP_UOK_PASS_REQ			331
#define FTP_NEED_ACNT				332
#define FTP_REQ_FACT_PEND			350
#define FTP_NOT_AVBL				421
#define FTP_DCO_FAIL				425
#define FTP_TRANS_ABOR_DCC		426
#define FTP_NO_ACT_FBSY			450
#define FTP_ABOR_LOC_ERR			451
#define FTP_NO_SPACE				452
#define FTP_SYNTAX_ERR			500
#define FTP_SYNTAX_ERR_PARAM		501
#define FTP_CMD_NO_IMP			502
#define FTP_BAD_CMD_SEQ			503
#define FTP_NO_IMP_PARAM			504
#define FTP_NOT_LOGIN				530
#define FTP_ACT_TO_STOR			532
#define FTP_FILE_NO_FOUND			550
#define FTP_PAGE_UNKNOWN			551
#define FTP_ABOR_NO_SPACE			552
#define FTP_BAD_FNAME				553

// Command Defines

struct ftp_cmds {	// Structure to hold ftp commands.

	char	*cmd_text;
	int		cmd_id;
};

struct	ftp_session {

	uint8_t							user_found;
	uint8_t							user_ok;
	uint8_t							user_seq;
	uint8_t							quit_flag;
	char							*trans_type;
	char							user_name[128];
	Socket_t						data_sock;
	Socket_t						ctrl_sock;
	char							file_path[ffconfigMAX_FILENAME+1];
	uint8_t							active;

};

#define USER					1
#define PASS					2
#define QUIT					3
#define PORTF					4
#define MODE					5
#define RETR					6
#define STOR					7
#define ABOR					8
#define DELE					9
#define LIST					10
#define SITE					11
#define TYPEF					12
#define STRU					13
#define PASV					14
#define STOU					15
#define APPE					16
#define ALLO					17
#define REST					18
#define RNFR					19
#define RNTO					20
#define RMD					21
#define MKD					22
#define PWD					23
#define NLST					24
#define SYST					25
#define STAT					26
#define HELP					27
#define NOOP					28
#define XPWD					29
#define CWD					30
#define CDUP					31
#define XRMD					32
#define OPTS					33
#define LAST_CMD				34  // Increment this number when commands are added

// Function Prototypes
void 				ftp_server( void *p );


#endif /* __ONEFTPD_H */
