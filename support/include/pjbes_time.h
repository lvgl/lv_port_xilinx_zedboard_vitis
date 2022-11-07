/*
 * our_time.h
 *
 *  Created on: 24 Apr 2019
 *      Author: peterb
 */

#ifndef SRC_SUPPORT_OUR_TIME_H_
#define SRC_SUPPORT_OUR_TIME_H_

#include <time.h>

#include "support.h"


struct tz_data {

	char		zname[6];		// Abbreviated name of zone e.g. UTC, BST, GMT, CET...
	char		zdesc[50];		// Textual Description of zone
	int32_t		utc_offset;		// Offset from UTC in seconds
};

extern struct	tz_data		t_zones[];

time_t get_zone_time( int z_idx, char* t_buf );

#endif /* SRC_SUPPORT_OUR_TIME_H_ */
