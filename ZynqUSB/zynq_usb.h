/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Pete J. Bone - PJBES
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 */

#ifndef _PJBES_ZYNQ_USB_H_
#define _PJBES_ZYNQ_USB_H_

// Xilinx
#define OPT_MCU_ZYNQ7000         1900 ///< Zynq 7000

#include <stdbool.h>
#include <stdint.h>
#include "xusbps.h"
#include "xscugic.h"

// Structure for memory mapped USB IO
struct zynq_usb_regs {

	volatile uint32_t	ID;
	volatile uint32_t	HwGeneral;
	volatile uint32_t	HwHost;
	volatile uint32_t	HwDevice;
	volatile uint32_t	HwTxBuf;
	volatile uint32_t	HwRxBuf;
	volatile uint32_t	SkipA[26];
	volatile uint32_t	GpTimer0Ld;
	volatile uint32_t	GpTimer0Ctrl;
	volatile uint32_t	GpTimer1ld;
	volatile uint32_t	GpTimer1Ctrl;
	volatile uint32_t	SbusCfg;
	volatile uint32_t	SkipB[27];
	volatile uint32_t	CapLen_HciVersion;
	volatile uint32_t	HcsPrams;
	volatile uint32_t	HccPrams;
	volatile uint32_t	SkipC[5];
	volatile uint32_t	DciVersion;
	volatile uint32_t	DccPrams;
	volatile uint32_t	SkipD[6];
	volatile uint32_t	Cmd;
	volatile uint32_t	ISR;
	volatile uint32_t	IER;
	volatile uint32_t	Frame;
	volatile uint32_t	SkipE;
	volatile uint32_t	ListBase;
	volatile uint32_t	AsyncListAddr;
	volatile uint32_t	TTCtrl;
	volatile uint32_t	BurstSize;
	volatile uint32_t	TxFill;
	volatile uint32_t	TxtFillTuning;
	volatile uint32_t	IC_USB;
	volatile uint32_t	ULPI_View;
	volatile uint32_t	SkipF;
	volatile uint32_t	Dev_EpNakISR;
	volatile uint32_t	Dev_EpNakIER;
	volatile uint32_t	ConfigFlag;
	volatile uint32_t	Port_SCR1;
	volatile uint32_t	SkipG[7];
	volatile uint32_t	OTG_SCR;
	volatile uint32_t	Mode;
	volatile uint32_t	DevEpStat;
	volatile uint32_t	DevEpPrime;
	volatile uint32_t	DevEpFlush;
	volatile uint32_t	DevEpReady;
	volatile uint32_t	DevEpTxComp;
	volatile uint32_t	DevEpCtrl0;
	volatile uint32_t	DevEpCtrl1;
	volatile uint32_t	DevEpCtrl2;
	volatile uint32_t	DevEpCtrl3;
	volatile uint32_t	DevEpCtrl4;
	volatile uint32_t	DevEpCtrl5;
	volatile uint32_t	DevEpCtrl6;
	volatile uint32_t	DevEpCtrl7;
	volatile uint32_t	DevEpCtrl8;
	volatile uint32_t	DevEpCtrl9;
	volatile uint32_t	DevEpCtrl10;
	volatile uint32_t	DevEpCtrl11;
};

// Register Bit Definitions

// Cmd Register
#define USB_CMD_RUN_ST			0x00000001	/* Run/Stop */
#define USB_CMD_RST				0x00000002	/* Controller RESET */
#define USB_CMD_FS01				0x0000000C	/* Frame List Size bit 0,1 */
#define USB_CMD_PSE				0x00000010	/* Periodic Sched Enable */
#define USB_CMD_ASE				0x00000020	/* Async Sched Enable */
#define USB_CMD_IAA				0x00000040	/* IRQ Async Advance Doorbell */
#define USB_CMD_ASP				0x00000300	/* Async Sched Park Mode Cnt */
#define USB_CMD_ASPE				0x00000800	/* Async Sched Park Mode Enbl */
#define USB_CMD_SUTW				0x00002000	/* Setup TripWire */
#define USB_CMD_ATDTW				0x00004000	/* Add dTD TripWire */
#define USB_CMD_FS2				0x00008000	/* Frame List Size bit 2 */
#define USB_CMD_ITC				0x00FF0000	/* IRQ Threshold Control */
#define USB_CMD_ITH_0				0x00000000	/* Immediate interrupt. */
#define USB_CMD_ITH_1				0x00010000	/* 1 micro-frame threshold*/
#define USB_CMD_ITH_2				0x00020000	/* 2 micro-frames threshold*/
#define USB_CMD_ITH_4				0x00040000	/* 4 micro-frames threshold*/
#define USB_CMD_ITH_8				0x00080000	/* 8 micro-frames threshold*/
#define USB_CMD_ITH_16			0x00100000	/* 16 micro-frames threshold*/
#define USB_CMD_ITH_32			0x00200000	/* 32 micro-frames threshold*/
#define USB_CMD_ITH_64			0x00400000	/* 64 micro-frames threshold*/
#define USB_CMD_ITH_DEFAULT		USB_CMD_ITH_8

// Mode Register
#define USB_MODE_DEVICE			0x00000002	/* Device Mode Select */
#define USB_MODE_HOST				0x00000003	/* Host Mode Select */
#define USB_MODE_ES				0x00000004	/* Endian Select */
#define USB_MODE_SLOM				0x00000008	/* Setup Lockout Mode Disable */
#define USB_MODE_SDIS				0x00000010	/* Stream Disable Mode */
#define USB_MODE_VBON				0x00000020	/* Turn on bus power for host mode */

// Port Status/Control Register
#define USB_PORTSCR_CCS			0x00000001 /* Current Connect Status */
#define USB_PORTSCR_CSC			0x00000002 /* Connect Status Change */
#define USB_PORTSCR_PE			0x00000004 /* Port Enable/Disable */
#define USB_PORTSCR_PEC			0x00000008 /* Port Enable/Disable Change */
#define USB_PORTSCR_OCA			0x00000010 /* Over-current Active */
#define USB_PORTSCR_OCC			0x00000020 /* Over-current Change */
#define USB_PORTSCR_FPR			0x00000040 /* Force Port Resume */
#define USB_PORTSCR_SUSP			0x00000080 /* Suspend */
#define USB_PORTSCR_PR			0x00000100 /* Port Reset */
#define USB_PORTSCR_HSP			0x00000200 /* High Speed Port */
#define USB_PORTSCR_LS			0x00000C00 /* Line Status */
#define USB_PORTSCR_PP			0x00001000 /* Port Power */
#define USB_PORTSCR_PO			0x00002000 /* Port Owner */
#define USB_PORTSCR_PIC			0x0000C000 /* Port Indicator Control */
#define USB_PORTSCR_PTC			0x000F0000 /* Port Test Control */
#define USB_PORTSCR_WKCN			0x00100000 /* Wake on Connect Enable */
#define USB_PORTSCR_WKDS			0x00200000 /* Wake on Disconnect Enable */
#define USB_PORTSCR_WKOC			0x00400000 /* Wake on Over-current Enable */
#define USB_PORTSCR_PHCD			0x00800000 /* PHY Low Power Suspend - Clock Disable */
#define USB_PORTSCR_PFSC			0x01000000 /* Port Force Full Speed Connect */
#define USB_PORTSCR_PSPD			0x0C000000 /* Port Speed */

// ULPI Viewport
#define USB_ULPIVIEW_DATRD_MASK 	0x0000FF00 /**< ULPI Data Read */
#define USB_ULPIVIEW_DATRD(x)		((x & USB_ULPIVIEW_DATRD_MASK) >> 8)
#define USB_ULPIVIEW_ADDR(x)		(x << 16)  /* Macro to set PHY register address */
#define USB_ULPIVIEW_SS			0x08000000 /* ULPI Synchronous State */
#define USB_ULPIVIEW_RW			0x20000000 /* ULPI Read/Write Control */
#define USB_ULPIVIEW_RUN			0x40000000 /* ULPI Run */
#define USB_ULPIVIEW_WU			0x80000000 /* ULPI Wakeup */

/*
// Hardware information per endpoint
struct hw_endpoint {

    // Is this a valid struct
    bool configured;

    // Transfer direction (i.e. IN is rx for host but tx for device)
    // allows us to common up transfer functions
    bool rx;

    uint8_t ep_addr;
    uint8_t next_pid;

    // Endpoint control register
    volatile uint32_t *endpoint_control;

    // Buffer control register
    volatile uint32_t *buffer_control;

    // Buffer pointer in usb dpram
    uint8_t *hw_data_buf;

    // Current transfer information
    bool active;
    uint16_t remaining_len;
    uint16_t xferred_len;

    // User buffer in main memory
    uint8_t *user_buf;

    // Data needed from EP descriptor
    uint16_t wMaxPacketSize;

    // Interrupt, bulk, etc
    uint8_t transfer_type;

#if TUSB_OPT_HOST_ENABLED
    // Only needed for host
    uint8_t dev_addr;

    // If interrupt endpoint
    uint8_t interrupt_num;
#endif
};
*/
struct usb_data {

	XUsbPs_Config			*xraw;
	struct zynq_usb_regs	*regs;

};


bool zynq_usb_init(void);

/*
void hw_endpoint_xfer_start(struct hw_endpoint *ep, uint8_t *buffer, uint16_t total_len);
bool hw_endpoint_xfer_continue(struct hw_endpoint *ep);
void hw_endpoint_reset_transfer(struct hw_endpoint *ep);

void _hw_endpoint_buffer_control_update32(struct hw_endpoint *ep, uint32_t and_mask, uint32_t or_mask);
static inline uint32_t _hw_endpoint_buffer_control_get_value32(struct hw_endpoint *ep) {
    return *ep->buffer_control;
}
static inline void _hw_endpoint_buffer_control_set_value32(struct hw_endpoint *ep, uint32_t value) {
    return _hw_endpoint_buffer_control_update32(ep, 0, value);
}
static inline void _hw_endpoint_buffer_control_set_mask32(struct hw_endpoint *ep, uint32_t value) {
    return _hw_endpoint_buffer_control_update32(ep, ~value, value);
}
static inline void _hw_endpoint_buffer_control_clear_mask32(struct hw_endpoint *ep, uint32_t value) {
    return _hw_endpoint_buffer_control_update32(ep, ~value, 0);
}

static inline uintptr_t hw_data_offset(uint8_t *buf)
{
    // Remove usb base from buffer pointer
    return (uintptr_t)buf;
}

extern const char *ep_dir_string[];
*/
#endif
