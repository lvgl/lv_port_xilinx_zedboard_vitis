/*
 * ntp.c
 *
 *  Created on: 30 Nov 2018
 *      Author: peterb
 */
#include <time.h>
#include <stdlib.h>

#include "support.h"
#include "sysmsg.h"
#include "network.h"
#include "ntp.h"

#include "queue.h"

//extern shared_memp_t		shmem_p;
extern config_datp_t		conf_p;
extern cpu0_globals_t		*cpu0_globals;

void set_time_from_NTP_task( void *is_task ) {

	Socket_t					sockfd;
	int							stat;
	struct ntp_packet 			ntp_pkt;
	struct freertos_sockaddr 	serv_addr;						// Server address data structure.
    socklen_t 					addrlen = sizeof(serv_addr);	/* length of addresses */
    struct timeval 				now;
    struct tm					rtc;
	TickType_t 					ReceiveTimeOut =  pdMS_TO_TICKS(5000);	// 5 Seconds
	uint32_t					serv_ip = conf_p->Sys_NTP;
	uint16_t					retry, master_retry;
	sysmsg_q_t					msg = { 0, pdFALSE, NULL };
	uint8_t						got_time = pdFALSE;
	uint32_t					ref_id = conf_p->Sys_NTP;
	int64_t						sec_offset=0, frac_offset=0;

	sockfd = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP ); // Create a UDP socket.

	if( sockfd == FREERTOS_INVALID_SOCKET ) {
		msg.id = ERR_NTPC_OSOCK;
		q_sysmsg( &msg );
		if( *((uint8_t*)is_task) == pdTRUE ) vTaskDelete(NULL); else return;
	}
	if( FreeRTOS_setsockopt( sockfd, 0, FREERTOS_SO_RCVTIMEO, &ReceiveTimeOut, sizeof( ReceiveTimeOut )) ) {
		msg.id = ERR_NTPC_SOCKTO;
		q_sysmsg( &msg );
		sock_graceful_shutdown( &sockfd );
		if( *((uint8_t*)is_task) == pdTRUE ) vTaskDelete(NULL); else return;
	}

	memset( (char*)&serv_addr, 0, sizeof(serv_addr) );
	serv_addr.sin_family = FREERTOS_AF_INET;
	serv_addr.sin_addr = serv_ip;
	serv_addr.sin_port = FreeRTOS_htons( NTP_PORT );

	for( master_retry = 0; master_retry < NTP_RETRIES; master_retry++ ) {
		for( retry = 0; retry < NTP_RETRIES; retry++ ) {
			memset( &ntp_pkt, 0, sizeof( struct ntp_packet ) );
			ntp_pkt.leap_ver_mode = LEAPI_NO_WARN | PROT_VER | MODE_CLI;
			/* Stratum */
			ntp_pkt.stratum = 0x02;
			ntp_pkt.poll_interval = 6; // this is 2^6 == 64!
			/* Precision in Microsecond ( from API gettimeofday() ) */
			ntp_pkt.ref_id = ref_id;
			ntp_pkt.precision = -20;  /* 2^(-20) sec */
			ntp_pkt.ref_ts_secs = cpu0_globals->ntp.prev_secs;
			ntp_pkt.ref_ts_frac = cpu0_globals->ntp.prev_frac;
			_gettimeofday( &now, NULL );
			ntp_pkt.origin_ts_secs = cpu0_globals->ntp.origin_secs = FreeRTOS_htonl( now.tv_sec + NTP_TO_UNIX_EPOCH_DELTA );
			ntp_pkt.origin_ts_frac = cpu0_globals->ntp.origin_frac = FreeRTOS_htonl( (uint32_t)(((uint64_t)now.tv_usec << 32) / 1000000) );
			stat = FreeRTOS_sendto( sockfd, &ntp_pkt, sizeof(ntp_pkt), 0, &serv_addr, addrlen );
			if( stat < 0 ) {
				msg.id = ERR_NTPC_SOCKWR;
				q_sysmsg( &msg );
				sock_graceful_shutdown( &sockfd );
				if( *((uint8_t*)is_task) == pdTRUE ) vTaskDelete(NULL); else return;
			}
			stat = FreeRTOS_recvfrom( sockfd, &ntp_pkt, sizeof(ntp_pkt), 0, &serv_addr, &addrlen );
			if( stat > 0 ) {
				got_time = pdTRUE;
				break;
			}
		}
		if( retry != NTP_RETRIES ) break;
		msg.id = ERR_NTPC_SOCKRDTO;
		q_sysmsg( &msg );
	}
	sock_graceful_shutdown( &sockfd );
	if( got_time ) {
		// Correct Endianess of NTP server time
		_gettimeofday( &now, NULL );
		cpu0_globals->ntp.prev_secs = cpu0_globals->ntp.destin_secs = FreeRTOS_htonl( now.tv_sec + NTP_TO_UNIX_EPOCH_DELTA );
		cpu0_globals->ntp.prev_frac = cpu0_globals->ntp.destin_frac = FreeRTOS_htonl( (uint32_t)(((uint64_t)now.tv_usec << 32) / 1000000) );
		sec_offset = (((int64_t)FreeRTOS_ntohl(ntp_pkt.rx_ts_secs) - (int64_t)FreeRTOS_ntohl(cpu0_globals->ntp.origin_secs)) +
				((int64_t)FreeRTOS_ntohl(ntp_pkt.tx_ts_secs) - (int64_t)FreeRTOS_ntohl(cpu0_globals->ntp.destin_secs))) / 2;
		frac_offset = ((((int64_t)(FreeRTOS_ntohl(ntp_pkt.rx_ts_frac) - (int64_t)FreeRTOS_ntohl(cpu0_globals->ntp.origin_frac)) +
				((int64_t)FreeRTOS_ntohl(ntp_pkt.tx_ts_frac) - (int64_t)FreeRTOS_ntohl(cpu0_globals->ntp.destin_frac))) / 2) * 1000000) >> 32;
//		printf( "s: %lld  f: %lld\r\n", sec_offset, frac_offset );
		ntp_pkt.tx_ts_secs = FreeRTOS_ntohl( ntp_pkt.tx_ts_secs ); // seconds.
		ntp_pkt.tx_ts_frac = FreeRTOS_ntohl( ntp_pkt.tx_ts_frac ); // fraction of a second in units of 2^-32(about 233pS)
		// Convert to UNIX epoch 1970 by deducting 70 years of seconds...
		if( abs((int)sec_offset) > 0 || abs((int)frac_offset) > 100000) {  // Keep our times synchronised with in 100mS
			_gettimeofday( &now, NULL );
			now.tv_sec += sec_offset;
			now.tv_usec += frac_offset;
			_settimeofday( &now, NULL );
			localtime_r( (time_t*)&now.tv_sec, &rtc );
//			update_rtc_from_user( &rtc );
			msg.id = INF_NTPC_UPD_INT;
			q_sysmsg( &msg );
		}else {
			msg.id = INF_NTPC_SYNCOK;
			q_sysmsg( &msg );
		}
	} else {
		msg.id = ERR_NTPC_MULTITO_FAIL;
		q_sysmsg( &msg );
	}
    if( *((uint8_t*)is_task) == pdTRUE ) vTaskDelete(NULL);
}
