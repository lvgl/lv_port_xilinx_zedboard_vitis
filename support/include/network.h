/*
 * network.h
 *
 *  Created on: 3 Dec 2018
 *      Author: peterb
 */

#ifndef SRC_DRIVERS_NETWORK_H_
#define SRC_DRIVERS_NETWORK_H_

#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "NetworkInterface.h"

void sock_graceful_shutdown( Socket_t *sock );
void my_debug( void *ctx, int level, const char *file, int line, const char *str );
void tcp_set_low_high_water( Socket_t xSocket, size_t uxLittleSpace, size_t uxEnoughSpace );

#endif /* SRC_DRIVERS_NETWORK_H_ */
