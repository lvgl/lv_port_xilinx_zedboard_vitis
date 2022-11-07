/*
 * network.c
 *
 *  Created on: 3 Dec 2018
 *      Author: peterb
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "network.h"
#include "support.h"

extern cpu0_globals_t		*cpu0_globals;

void sock_graceful_shutdown( Socket_t *sock ) {

	char			buf = 0;
	TickType_t 		TimeOnShutdown;

	FreeRTOS_shutdown( *sock, FREERTOS_SHUT_RDWR );
	/* Wait for the shutdown to take effect, indicated by FreeRTOS_recv() returning an error. */
	TimeOnShutdown = xTaskGetTickCount();
	do {
		if( FreeRTOS_recv( *sock, &buf, sizeof(buf), 0 ) < 0 ) {
			break;
		}
		vTaskDelay(pdMS_TO_TICKS(20)); // Don't hog the CPU!
	} while( ( xTaskGetTickCount() - TimeOnShutdown ) < 5000 );

	/* Finished with the socket, buffer, the task. */
	if(*sock != FREERTOS_INVALID_SOCKET ) FreeRTOS_closesocket( *sock );
	*sock = FREERTOS_INVALID_SOCKET;
}

void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent ) {		// This function is called after the IP stack is up and running

	sysmsg_q_t	msg = { 0, pdFALSE, NULL };
	static BaseType_t	xTasksAlreadyCreated = pdFALSE;

    /* Both eNetworkUp and eNetworkDown events can be processed here. */
    if( eNetworkEvent == eNetworkUp ) {
        /* Create the tasks that use the TCP/IP stack if they have not already
        been created. */
        if( xTasksAlreadyCreated == pdFALSE ) {
        	uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
        	char cBuffer[ 16 ];
            /*
             * For convenience, tasks that use FreeRTOS+TCP can be created here
             * to ensure they are not created before the network is usable.
             */

            xTasksAlreadyCreated = pdTRUE;
            FreeRTOS_GetAddressConfiguration( &ulIPAddress, &ulNetMask, &ulGatewayAddress, &ulDNSServerAddress );
			FreeRTOS_inet_ntoa( ulIPAddress, cBuffer );
			printf( "IP Address: %s\r\n", cBuffer );
			FreeRTOS_inet_ntoa( ulNetMask, cBuffer );
			printf( "Subnet Mask: %s\r\n", cBuffer );
			FreeRTOS_inet_ntoa( ulGatewayAddress, cBuffer );
			printf( "Gateway IP Address: %s\r\n", cBuffer );
			FreeRTOS_inet_ntoa( ulDNSServerAddress, cBuffer );
			printf( "DNS server IP Address: %s\r\n", cBuffer );
			cpu0_globals->net_started = pdTRUE;
			msg.id = SPAWN_FTP;
			q_spawn_req( &msg );

        }
    }
}

void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier ) {


}

inline uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress, uint16_t usSourcePort, uint32_t ulDestinationAddress, uint16_t usDestinationPort ) {
	(void) ulSourceAddress;
	(void) usSourcePort;
	(void) ulDestinationAddress;
	(void) usDestinationPort;
	int i = 0;
	time_t s;

	time(&s);
	i =  rand_r((unsigned int*)&s);
	return i;
}

inline BaseType_t xApplicationGetRandomNumber( uint32_t *pulNumber ) {

	time_t s;

	time(&s);
	*pulNumber = rand_r((unsigned int*)&s);
	return *pulNumber;
}

void my_debug( void *ctx, int level, const char *file, int line, const char *str ) {
    const char *p, *basename;

    /* Extract basename from file */
    for( p = basename = file; *p != '\0'; p++ )
        if( *p == '/' || *p == '\\' )
            basename = p + 1;

    printf( "%s:%04d: |%d| %s", basename, line, level, str );
}

void tcp_set_low_high_water( Socket_t xSocket, size_t uxLittleSpace, size_t uxEnoughSpace ) {

	LowHighWater_t xLowHighWater;
	memset( &( xLowHighWater ), 0, sizeof( xLowHighWater ) );
	xLowHighWater.uxLittleSpace = uxLittleSpace;
	xLowHighWater.uxEnoughSpace = uxEnoughSpace;
	FreeRTOS_setsockopt( xSocket,	0, FREERTOS_SO_SET_LOW_HIGH_WATER, ( void * ) &( xLowHighWater ), sizeof( xLowHighWater ) );
}
//	tcp_set_low_high_water( listen_sock.fd, 10 * ipconfigTCP_MSS, 100 * ipconfigTCP_MSS );

