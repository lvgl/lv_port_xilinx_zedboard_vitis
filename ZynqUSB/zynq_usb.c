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
 * This file is part of the TinyUSB stack.
 */

#include <ULPI.h>
#include "host/hcd.h"
#include "support.h"

#if CFG_TUSB_MCU == OPT_MCU_ZYNQ7000

#include <stdlib.h>
#include <zynq_usb.h>

extern XScuGic 			xInterruptController; 	/* Interrupt controller instance */
extern cpu0_globals_t	*cpu0_globals;

#ifdef CFG_TUSB_DEBUG
	int xwrap_printf(const char *format, ...) {
		xil_printf( (const char8*)format );
		return 0;
	}
#endif

bool zynq_usb_init(void) {

	bool	ret =  true;
	uint8_t	temp;

	if( (cpu0_globals->usb.xraw = XUsbPs_LookupConfig(XPAR_PS7_USB_0_DEVICE_ID)) == NULL ) {
		ret = false;
	} else {
		reset_usb_phy();
		vTaskDelay(pdMS_TO_TICKS(2));
		usb_CTRL_reset( 0 );
		vTaskDelay(pdMS_TO_TICKS(2));
		cpu0_globals->usb.regs = (struct zynq_usb_regs*)cpu0_globals->usb.xraw->BaseAddress;
		cpu0_globals->usb.regs->Cmd |= USB_CMD_RST; // Soft reset the controller
		while( cpu0_globals->usb.regs->Cmd & USB_CMD_RST ) vTaskDelay(pdMS_TO_TICKS(1)); // Wait for reset to release
		cpu0_globals->usb.regs->Mode = USB_MODE_HOST;
		vTaskDelay(pdMS_TO_TICKS(1000));				// Wait then power up port
		cpu0_globals->usb.regs->Mode |=  USB_MODE_VBON | USB_MODE_SDIS;
		cpu0_globals->usb.regs->Port_SCR1 |= USB_PORTSCR_PFSC;
		/* Update periodic list base address register with reset value */
		cpu0_globals->usb.regs->ListBase = 0;
		/* Update async/endpoint list base address register with reset value */
		cpu0_globals->usb.regs->AsyncListAddr = 0;
		vTaskDelay(pdMS_TO_TICKS(2));
		cpu0_globals->usb.regs->ULPI_View = USB_ULPIVIEW_RUN | USB_ULPIVIEW_ADDR(ULPI_OTG_CTRL); // Read OTG_CTRL
		temp = USB_ULPIVIEW_DATRD(cpu0_globals->usb.regs->ULPI_View);
		cpu0_globals->usb.regs->ULPI_View = (USB_ULPIVIEW_RUN | USB_ULPIVIEW_RW | USB_ULPIVIEW_ADDR(ULPI_OTG_CTRL) |
				(temp | ULPI_OTG_DRVVBUS_EXT	| ULPI_OTG_ID_PULLUP | ULPI_OTG_DP_PULLDOWN	| ULPI_OTG_DM_PULLDOWN));
		cpu0_globals->usb.regs->ULPI_View = (USB_ULPIVIEW_RUN | USB_ULPIVIEW_RW | USB_ULPIVIEW_ADDR(ULPI_OTG_CTRL) |
				(temp | ULPI_OTG_DRVVBUS_EXT	| ULPI_OTG_ID_PULLUP | ULPI_OTG_DP_PULLDOWN	| ULPI_OTG_DM_PULLDOWN |
						ULPI_OTG_DRVVBUS));
	}
	XScuGic_Disable(&xInterruptController, XPS_USB0_INT_ID);
	XScuGic_Connect(&xInterruptController, XPS_USB0_INT_ID, (Xil_ExceptionHandler)&hcd_int_handler, NULL);

	return ret;

}

#endif
