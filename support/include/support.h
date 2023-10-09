/*
 * support.h
 *
 *  Created on: 26 Sep 2022
 *      Author: PeterB
 */

#ifndef INCLUDE_SUPPORT_H_
#define INCLUDE_SUPPORT_H_
#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_DNS.h"
#include "lvgl.h"
#include "semphr.h"
#include "sysmsg.h"
#include "zynq_usb.h"

/* Our Defines */
#define NUM_CORES					2
#define MAX_TASKS					20
#define THREAD_STACKSIZE 			2048 * 4
#define THREAD_SMALLSTACK			1024
#define SYSMSGPROC_PRIORITY		5
#define DEFAULT_THREAD_PRIO		7
#define SVISOR_PRIORITY			9
#define GUI_PRIORITY				12
#define USB_PRIORITY				12
#define FTP_PRIORITY				9
#define SPWN_PRIORITY				13
#define SYS_ACTION_Q_SIZE			8
#define SPAWN_Q_SIZE				8
#define SYSMSG_Q_SIZE				128
#define INDEV_Q_SIZE				256
#define GUIMSG_Q_SIZE				128

#define MAX_NAME_LEN				80

/* Stuff for Dual Core operation */
#define CPU0_IRQ					0x00000001	// Send interrupt to CPU Core 0 private irq
#define CPU1_IRQ					0x00000002	// Send interrupt to CPU Core 1 private irq
#define USB_RESET					0x80000000	// Reset USB PHY via PL
#define sev() __asm__("sev")
#define CPU1_MEM_BASE       	 	0x10000000
#define CPU1_START_ADR      	 	0xFFFFFFF0
#define CPU1_PROCEED				0x00000001	// Tells CPU 1 it can proceed with startup.

/* Define a low latency data memory space which is visible to both CPUs */
#define SHARED_OCM_MEMORY_BASE	0xFFFF0000

/* Define a large data memory space which is visible to both CPUs.*/
#define SHARED_DDR_MEMORY_BASE	0x1F000000

/* Reset Related registers */
#define REBOOT_STATUS_REG			(XPS_SYS_CTRL_BASEADDR + 0x258)
#define PS_RST_CTRL_REG			(XPS_SYS_CTRL_BASEADDR + 0x200)
#define PS_RST_MASK				0x00000001	/**< PS software reset */
#define PS_USB_RST_CTRL_REG		(XPS_SYS_CTRL_BASEADDR + 0x210)
#define PS_USB0_RST				0x00000001
#define PS_USB1_RST				0x00000002

#define CALCULATION_PERIOD		100		/* Number of system ticks to calculate FreeRTOS stats over */

#define pdTICKS_TO_MS( xTimeInTicks ) ( ( TickType_t ) ( ( ( TickType_t ) ( xTimeInTicks ) * ( TickType_t ) 1000 ) / ( TickType_t ) configTICK_RATE_HZ ) )

#define SD_CARD_ROOT				"/"
#define SYS_PATH					"/Zedboard/"
#define SYS_PATHDIR				"/Zedboard"
#define CONFIG_FNAME				"CONFIG.DAT"
#define CONFIG_PATH 				SYS_PATH CONFIG_FNAME
#define BCONFIG_PATH				SYS_PATH"CONFIG.BAK"
#define FW_FILE					"BOOT.BIN"

#define CLOAD_OK					0x00	// During boot configuration loaded successfully from SD Card
#define CLOAD_NODIR				0x01	// During boot configuration directory was missing from SD Card and a new one was created
#define CLOAD_NOFILE				0x02	// During boot configuration file was missing from SD Card and a default new one created
#define CLOAD_BAD_CSUM			0x04	// During boot configuration file had a bad checksum and a default new one created
#define CLOAD_BAD_LEN				0x08	// During boot configuration file had an incorrect length and a default new one created

#define GUI_LITE					0
#define GUI_DARK					1

struct ntp_s {

	uint32_t				prev_secs;
	uint32_t				prev_frac;
	uint32_t				origin_secs;
	uint32_t				origin_frac;
	uint32_t				destin_secs;
	uint32_t				destin_frac;
};

struct mouse_data {

    lv_obj_t			*cursor_obj;
	lv_indev_data_t		last_state;
    lv_indev_drv_t		indev_drv;
    lv_indev_t			*indev;
	QueueHandle_t		state_q;
#ifdef MOUSE_EMU
    volatile uint8_t 	left;
	volatile uint8_t 	right;
	volatile uint8_t 	up;
	volatile uint8_t 	down;
	volatile uint8_t 	click;
	volatile uint8_t	irq_true;
#endif
};

struct kb_data {

	QueueHandle_t		state_q;
	lv_obj_t			*kb_container;
	lv_obj_t			*kb;
	lv_group_t 			*kb_gp_sysc;
    lv_indev_drv_t		indev_drv;
    lv_indev_t			*indev;
};

struct gui_globals {

	struct mouse_data		mse_dat;
	struct kb_data			kb_dat;
	QueueHandle_t			msg_q;				// This queue is used to tell the GUI to do things as it all has to run in one thread
												// There is an associated timer in the GUI to process these messages, it's all defined in gui_main.c/h
												// Any other thread can post to the queue and the GUI will handle the messages itself.
	lv_disp_drv_t			disp_drv;			/*Descriptor for display driver*/
	lv_disp_draw_buf_t		disp_buf;			/*Descriptor for display buffer*/
	lv_disp_t				*disp;				/*Descriptor for display*/
	lv_style_t		 		style_sb;			/* A style for all our scroll bars*/
	lv_obj_t				*log_ta;			/* The text area on the log tab */
	volatile uint32_t		dma_src;
	volatile uint8_t		buf_switched : 1;
	volatile uint8_t		gui_ready : 1;
	volatile uint8_t		colour_changed : 2;

};

#define		TCPIP_INTERFACES	1
#define		TCPIP_ENDPOINTS	1
#define		GetPhyLinkStatus	cpu0_globals->tcpip.Interfaces[0].pfGetPhyLinkStatus

struct ip_stack_data {

	NetworkInterface_t			Interfaces[ TCPIP_INTERFACES ];
	NetworkEndPoint_t			EndPoints[ TCPIP_ENDPOINTS ];
};

/* CPU0 global struct */
typedef struct _cpu0_globals {

	QueueHandle_t			sys_action_q;  		// This message queue receives message requests from the GUI thread
	QueueHandle_t			sysmsg_q;  			// This message queue receives message requests for system events
	QueueHandle_t			spawn_q;  			// This message queue receives message requests to spawn threads
	struct gui_globals		gui;
	struct usb_data			usb;
	struct ntp_s			ntp;
	struct ip_stack_data	tcpip;
	uint8_t					net_started;

} cpu0_globals_t;

typedef struct our_task_status {

    char		pcTaskName[configMAX_TASK_NAME_LEN];	/* A pointer to the task's name.  This value will be invalid if the task was deleted since the structure was populated! */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    eTaskState	CurrentState;							/* The state in which the task existed when the structure was populated. */
    UBaseType_t	CurrentPriority;						/* The priority at which the task was running (may be inherited) when the structure was populated. */
    uint16_t	StackHighWaterMark;						/* The minimum amount of stack space that has remained for the task since the task was created.  The closer this value is to zero the closer the task has come to overflowing its stack. */

} OurTaskStatus_t;

struct cpu_data	{

	size_t					FreeHeap;
	uint32_t  				Usage;
	uint32_t       			IdleStartTime;
	uint32_t       			IdleSpentTime;
	uint32_t       			TotalIdleTime;
	TaskHandle_t    		IdleHandle;
	OurTaskStatus_t			ProcessStatus[MAX_TASKS];
	uint32_t				task_cnt;
	uint8_t					Started_OK;
};

typedef struct	_shared_mem {

	struct cpu_data			cpud[NUM_CORES];
	uint32_t				cpu_1_flags;

} shared_mem_t, *shared_memp_t;

typedef struct _config_dat { /* NOTE: Keep csum at end! */

	char				Sys_Name[MAX_NAME_LEN];	/* System Identification name */
	char				pword[MAX_NAME_LEN];		/* Stores hashed password */
	uint32_t			Sys_IP;						/* System IP Address */
	uint32_t			Sys_nm;						/* System Network Mask */
	uint32_t			Sys_gw;						/* System Gateway Address */
	uint32_t			Sys_DNS;					/* System DNS Server Address */
	uint32_t			Sys_NTP;					/* System NTP Server Address */
	double				Sys_NTP_update;				/* Time in hours between NTP sync */
	uint8_t				softmac_addr[6];			/* User Defined MAC address */
	lv_color_t			gui_colour;					/* Allows changing GUI primary colour */
	uint8_t				gui_theme;					/* Light or Dark theme */
	uint8_t				st_active;					/* daylight saving is active */
	uint16_t			tz_idx;						/* Time Zone Index */
	uint64_t			csum;						/* Configuration Checksum - MUST ALWAYS BE LAST! */

} config_dat_t, *config_datp_t;

/* Prototypes */
uint8_t	load_config( config_datp_t conf_p );
int save_config( config_datp_t conf_p );
int _gettimeofday( struct timeval *tv, void *tzvp );
int _settimeofday( struct timeval *tv, void *tzvp );
uint16_t GetCPUUsage ( uint8_t core );
void init_shared_mem( uint8_t init_video, uint8_t init_ocm, uint8_t init_ddr );
void system_reboot();
void prvSetupHardware( void );
void task_spawner( void *p );
void reset_usb_phy( void );
void usb_CTRL_reset( uint8_t port );
void unhash_password( char *hashed_password, char *password );
void hash_password( char *password, char *hashed_password );
uint8_t q_spawn_req( sysmsg_q_t *msg );
uint8_t q_sys_action( sysmsg_q_t *msg );
lv_indev_t* keyboard_drv_init( void );
void sys_mngr0( void *p );
void InterCpuIRQ( void );

#endif /* INCLUDE_SUPPORT_H_ */
