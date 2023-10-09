/*
 * ntp_server.c
 *
 *  Created on: 22 Feb 2021
 *      Author: PeterB
 */
#include <time.h>

#include "support.h"
#include "ntp.h"
#include "sysmsg.h"
#include "network.h"

extern config_datp_t		conf_p;
extern cpu0_globals_t		*cpu0_globals;

static int ntp_reply( Socket_t sock, struct freertos_sockaddr *cl_addrp, socklen_t cl_addrlen, struct ntp_packet *recv_pkt, struct timeval *recv_time) {

	struct ntp_packet	send_pkt;
	uint32_t			ref_id = conf_p->Sys_NTP;
	struct timeval		now;
	sysmsg_q_t			msg = { 0, pdFALSE, NULL };

	if ((recv_pkt->leap_ver_mode & 0x07) != MODE_CLI) { // Check mode is correct
		sock_graceful_shutdown( &sock );
		sock = FREERTOS_INVALID_SOCKET;
		msg.id = ERR_NTPS_SEND_FAIL;
		q_sysmsg( &msg );
		return 1;
	}

	/* Get the info from the received packet to send back in the rply
	   	LEAP INDICATOR   = 0
		VERSION NUMBER   = Version number form the client
		Mode = 4
	 */
	send_pkt.leap_ver_mode = (recv_pkt->leap_ver_mode & 0x38) + 4;

	/* Stratum = 2 (secondary reference)
	   Reference ID = primary NTP server address
	 */
	send_pkt.stratum = 0x02;
	send_pkt.ref_id = ref_id;

	/* Copy Poll */
	send_pkt.poll_interval = recv_pkt->poll_interval;

	/* Precision in Microsecond */
	send_pkt.precision = -20;  /* 2^(-20) sec */

	/* For simplicity , Root Delay = 0, Root Dispersion = 0 */
	send_pkt.root_delay = 0;
	send_pkt.root_dispersion = 0;

	send_pkt.ref_ts_secs = cpu0_globals->ntp.prev_secs; // This is updated by the client when it fetches time from its primary
	send_pkt.ref_ts_frac = cpu0_globals->ntp.prev_frac;

	/* Originate Time = Transmit Time @ Client if zero fill in local time google servers seem to do this as it shouldn't be 0!*/
	if( !recv_pkt->origin_ts_secs ) {
		_gettimeofday( &now, NULL );
		send_pkt.origin_ts_secs = FreeRTOS_htonl( now.tv_sec + NTP_TO_UNIX_EPOCH_DELTA );
		send_pkt.origin_ts_frac = FreeRTOS_htonl( (uint32_t)(((uint64_t)now.tv_usec << 32) / 1000000) );
	} else {
		send_pkt.origin_ts_secs = recv_pkt->origin_ts_secs; // These are just turned around so no Endian swap required!
		send_pkt.origin_ts_frac = recv_pkt->origin_ts_frac;
	}
	/* Receive Time @ Server */
	send_pkt.rx_ts_secs = FreeRTOS_htonl( recv_time->tv_sec + NTP_TO_UNIX_EPOCH_DELTA );
	send_pkt.rx_ts_frac = FreeRTOS_htonl( (uint32_t)(((uint64_t)recv_time->tv_usec << 32) / 1000000) );

	/* Transmit Time*/
	_gettimeofday( &now, NULL );
	send_pkt.tx_ts_secs = FreeRTOS_htonl( now.tv_sec + NTP_TO_UNIX_EPOCH_DELTA ) ;
	send_pkt.tx_ts_frac = FreeRTOS_htonl( (uint32_t)(((uint64_t)now.tv_usec << 32) / 1000000) );

	if ( FreeRTOS_sendto( sock, &send_pkt, sizeof(send_pkt), 0, cl_addrp, cl_addrlen ) < 48) {
		sock_graceful_shutdown( &sock );
		sock = FREERTOS_INVALID_SOCKET;
		msg.id = ERR_NTPS_SEND_FAIL;
		q_sysmsg( &msg );
		return 1;
	}

	return 0;
}

void ntp_server( void *a ) {

	struct ntp_packet 			recv_pkt;
	sysmsg_q_t					msg = { 0, pdFALSE, NULL };
	Socket_t					listen_sock;
	struct freertos_sockaddr 	server_addr, client_addr;
    socklen_t 					client_addrlen = sizeof(client_addr);
	TickType_t 					ReceiveTimeOut =  portMAX_DELAY;
	struct timeval				rx_time;

reset:
	listen_sock = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );
	if (listen_sock == FREERTOS_INVALID_SOCKET) {
		msg.id = ERR_NTPS_OSOCK;
		q_sysmsg( &msg );
	    vTaskDelete(NULL);
	}

	if( FreeRTOS_setsockopt( listen_sock, 0, FREERTOS_SO_RCVTIMEO, &ReceiveTimeOut, sizeof( ReceiveTimeOut )) ) {
		msg.id = ERR_NTPS_SOCKTO;
		q_sysmsg( &msg );
		sock_graceful_shutdown( &listen_sock );
	    vTaskDelete(NULL);
	}

	server_addr.sin_family = FREERTOS_AF_INET;
	server_addr.sin_address.ulIP_IPv4 = 0;
	server_addr.sin_port = FreeRTOS_htons( NTP_PORT );

	if( FreeRTOS_bind( listen_sock, &server_addr, sizeof(server_addr) ) < 0 ) {
		sock_graceful_shutdown( &listen_sock );
		listen_sock = FREERTOS_INVALID_SOCKET;
		msg.id = ERR_NTPS_BIND_FAIL;
		q_sysmsg( &msg );
		goto reset;
	}


	while( 1 ) {

		if( FreeRTOS_recvfrom( listen_sock, &recv_pkt, sizeof( recv_pkt ), 0, &client_addr, &client_addrlen ) != 48 ) {
			sock_graceful_shutdown( &listen_sock );
			listen_sock = FREERTOS_INVALID_SOCKET;
			msg.id = ERR_NTPS_RECV_FAIL;
			q_sysmsg( &msg );
			goto reset;
		} else {
			_gettimeofday( &rx_time, NULL );
			if( ntp_reply( listen_sock, &client_addr, client_addrlen, &recv_pkt, &rx_time) ) goto reset;
		}
		vTaskDelay(100); // Just to prevent any excessive CPU Usage should the system somehow get in a tight loop!

	}

}
