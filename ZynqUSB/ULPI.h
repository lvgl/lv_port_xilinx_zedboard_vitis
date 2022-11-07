
#ifndef _OTG_ULPI_REGS_H
#define _OTG_ULPI_REGS_H

// Standard ULPI Register Map

#define ULPI_VENDOR_ID_LOW			0x00
#define ULPI_VENDOR_ID_HIGH			0x01
#define ULPI_PRODUCT_ID_LOW			0x02
#define ULPI_PRODUCT_ID_HIGH			0x03
#define ULPI_FUNC_CTRL				0x04
#define ULPI_IFC_CTRL					0x07
#define ULPI_OTG_CTRL					0x0a
#define ULPI_USB_INT_EN_RISE			0x0d
#define ULPI_USB_INT_EN_FALL			0x10
#define ULPI_USB_INT_STS				0x13
#define ULPI_USB_INT_LATCH			0x14
#define ULPI_DEBUG						0x15
#define ULPI_SCRATCH					0x16

/*
 * Register Bits
 */

/* Function Control */
#define ULPI_FUNC_HSPEED				0x00
#define ULPI_FUNC_FSPEED				0x01
#define ULPI_FUNC_LSPEED				0x02
#define ULPI_FUNC_FS4LS				0x03
#define ULPI_FUNC_TERMSELECT			0x04
#define ULPI_FUNC_OPMODE_NORM			0x00
#define ULPI_FUNC_OPMODE_NONDRIV		0x08
#define ULPI_FUNC_OPMODE_DIS_NRZI	0x10
#define ULPI_FUNC_OPMODE_NOSYN_NOEOP	0x18
#define ULPI_FUNC_RESET				0x20
#define ULPI_FUNC_SUSPENDM			0x40

/* Interface Control */
#define ULPI_IFC_6_PIN_SERIAL_MODE	0x01
#define ULPI_IFC_3_PIN_SERIAL_MODE	0x02
#define ULPI_IFC_CARKITMODE			0x04
#define ULPI_IFC_CLOCKSUSPENDM		0x08
#define ULPI_IFC_AUTORESUME			0x10
#define ULPI_IFC_EXTERNAL_VBUS		0x20
#define ULPI_IFC_PASSTHRU				0x40
#define ULPI_IFC_PROTCT_IFC_DIS		0x80

/* OTG Control */
#define ULPI_OTG_ID_PULLUP			0x01
#define ULPI_OTG_DP_PULLDOWN			0x02
#define ULPI_OTG_DM_PULLDOWN			0x04
#define ULPI_OTG_DISCHRGVBUS			0x08
#define ULPI_OTG_CHRGVBUS				0x10
#define ULPI_OTG_DRVVBUS				0x20
#define ULPI_OTG_DRVVBUS_EXT			0x40
#define ULPI_OTG_EXTVBUSIND			0x80

/* USB Interrupt Enable Rising,
 * USB Interrupt Enable Falling,
 * USB Interrupt Status and
 * USB Interrupt Latch
 */
#define ULPI_INT_HOST_DISCONNECT		0x01
#define ULPI_INT_VBUS_VALID			0x02
#define ULPI_INT_SESS_VALID			0x04
#define ULPI_INT_SESS_END				0x08
#define ULPI_INT_IDGRD				0x10



#endif /* _OTG_ULPI_REGS_H */
