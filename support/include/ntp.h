/*
 * ntp.h
 *
 *  Created on: 30 Nov 2018
 *      Author: peterb
 */

#ifndef SRC_NTP_NTP_H_
#define SRC_NTP_NTP_H_

#include <stdint.h>

/* See RFC1305 for details
 *
 * 	Leap Indicator, bits 0 & 1 of first byte of packet ( Don't forget Network uses Big Endian!)
 * 	00 no warning
 *	01 last minute has 61 seconds
 *	10 last minute has 59 seconds)
 *	11 alarm condition (clock not synchronised)
 *
 * 	Protocol Version Number, bits 2-4 of first byte of packet
 * 	( We are using Version 3 for simplicity for now )
 *
 * Mode, bits 4-7 of first byte of packet
 *	0 reserved
 *	1 symmetric active
 *	2 symmetric passive
 *	3 client
 *	4 server
 *	5 broadcast
 *	6 reserved for NTP control message
 *	7 reserved for private use
 *
 *	Stratum: This is a eight-bit integer indicating the stratum level of the local clock, with values defined
 *	as follows:
 *	0 unspecified
 *	1 primary reference (e.g., radio clock)
 *	2-255 secondary reference (via NTP)
 *	The values that can appear in this field range from zero to NTP.INFIN inclusive.
 *
 *	Poll Interval: This is an eight-bit signed integer indicating the maximum interval between successive
 *	messages, in seconds to the nearest power of two. The values that can appear in this field range
 *	from NTP.MINPOLL to NTP.MAXPOLL inclusive.
 *
 *	Precision: This is an eight-bit signed integer indicating the precision of the local clock, in seconds
 *	to the nearest power of two.
 *
 *	Root Delay: This is a 32-bit signed fixed-point number indicating the total roundtrip delay to the
 *	primary reference source, in seconds with fraction point between bits 15 and 16. Note that this
 *	variable can take on both positive and negative values, depending on clock precision and skew.
 *
 *	Root Dispersion: This is a 32-bit signed fixed-point number indicating the maximum error relative
 *	to the primary reference source, in seconds with fraction point between bits 15 and 16. Only
 *	positive values greater than zero are possible.
 *
 *	Reference Clock Identifier: This is a 32-bit code identifying the particular reference clock. In the
 *	case of stratum 0 (unspecified) or stratum 1 (primary reference), this is a four-octet, left-justified,
 *	zero-padded ASCII string. While not enumerated as part of the NTP specification, the following
 *	are suggested ASCII identifiers:
 *	Stratum Code Meaning
 *	0 DCN DCN routing protocol
 *	0 NIST NIST public modem
 *	0 TSP TSP time protocol
 *	0 DTS Digital Time Service
 *	1 ATOM Atomic clock (calibrated)
 *	1 VLF VLF radio (OMEGA, etc.)
 *	1 call-sign Generic radio
 *	1 LORC LORAN-C radio navigation
 *	1 GOES GOES UHF environment satellite
 *	1 GPS GPS UHF satellite positioning
 *	In the case of stratum 2 and greater (secondary reference) this is the four-octet Internet address
 *	of the primary reference host.
 *
 *	Reference Time-stamp: This is the local time at which the local clock was last set or corrected, in
 *	64-bit time-stamp format.
 *
 *	Originate Time-stamp: This is the local time at which the request departed the client host for the
 *	service host, in 64-bit time-stamp format.
 *
 *	Receive Time-stamp: This is the local time at which the request arrived at the service host, in 64-bit
 *	time-stamp format.
 *
 *	Transmit Time-stamp: This is the local time at which the reply departed the service host for the client
 *	host, in 64-bit time-stamp format.
 *
 *	Authenticator (optional): When the NTP authentication mechanism is implemented, this contains
 *	the authenticator information defined in Appendix C.
 *
 *	Note: 64-bit time-stamp format is as follows:
 *	first 32-bits are time in seconds since 1900
 *	second 32-bits are fractions of a second i.e. multiples of 1/(2^32) seconds which is about 232.8 picosecond increments
 *
 */

/* Defines for NTP Leap version and mode byte */
#define LEAPI_NO_WARN	(uint8_t)(0x00 << 6)
#define LEAPI_LM_61	(uint8_t)(0x01 << 6)
#define LEAPI_LM_59	(uint8_t)(0x02 << 6)
#define LEAPI_ALM		(uint8_t)(0x03 << 6)

#define PROT_VER	(uint8_t)(0x03 << 3)

#define MODE_RES	(uint8_t)(0x00 << 0)
#define MODE_SYMA	(uint8_t)(0x01 << 0)
#define MODE_SYMP	(uint8_t)(0x02 << 0)
#define MODE_CLI	(uint8_t)(0x03 << 0)
#define MODE_SERV	(uint8_t)(0x04 << 0)
#define MODE_BRDC	(uint8_t)(0x05 << 0)
#define MODE_NTP	(uint8_t)(0x06 << 0)
#define MODE_PRIV	(uint8_t)(0x07 << 0)

#define NTP_PORT	123

// NTP packet.
struct ntp_packet {

  uint8_t	leap_ver_mode;
  uint8_t	stratum;
  uint8_t	poll_interval;
  int8_t	precision;
  uint32_t	root_delay;
  uint32_t	root_dispersion;
  uint32_t	ref_id;
  uint32_t	ref_ts_secs;
  uint32_t	ref_ts_frac;
  uint32_t	origin_ts_secs;
  uint32_t	origin_ts_frac;
  uint32_t	rx_ts_secs;
  uint32_t	rx_ts_frac;
  uint32_t	tx_ts_secs;
  uint32_t	tx_ts_frac;

};

#define NTP_TO_UNIX_EPOCH_DELTA		2208988800ul
#define NTP_RETRIES					5

// Prototypes
void ntp_server( void *a );
void set_time_from_NTP_task( void *terminate );

#endif /* SRC_NTP_NTP_H_ */
