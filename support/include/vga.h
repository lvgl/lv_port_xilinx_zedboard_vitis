/*
 * vga.h
 *
 *  Created on: 1 Jun 2018
 *      Author: peterb
 */
#ifndef VGA_H_INCLUDED
#define VGA_H_INCLUDED

#include "xdebug.h"
#include "xil_exception.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xscugic.h"
#include "xreg_cortexa9.h"
#include "lvgl.h"

#define VGA_640X480				0
#define VGA_800x600_72HZ			1
#define VGA_1024X768_60HZ			2
#define VGA_1024X768_70HZ			3
#define VGA_1440X900_60HZ_CVTS	4
#define VGA_1440X900_60HZ_CVTR	5
#define VGA_1440X900_75HZ_CVTS	6
#define VGA_1440X900_85HZ_CVTS	7
#define VGA_1440X900_120HZ_CVTR	8

#define VGA_DDR_DMA_BASE		0x0F000000		// This is the DDR address the DMA hardware fetches VGA pixel data from (32MB is reserved)
#define VGA_DDR_DMA_SIZE		0x2000000		// Size of above buffer
#define VGA_CTRL_BASE			XPAR_ZEDBOARD_AXI_VGA_0_BASEADDR
#define SYNC_NEG				0
#define SYNC_POS				1

// Horizontal Defines
#define TOTH_PIX_M				0x00000FFF
#define SET_THP(x)				x-1
#define H_PIX_M				0x00FFF000
#define SET_HP(x)				(x << 12)
#define H_SYNC_M				0xFF000000
#define SET_HS(x)				(x << 24)
#define H_FPOR_M				0x00000FFF
#define H_BPOR_M				0x00FFF000
#define H_POL_M				0x80000000
// Vertical Defines
#define TOTV_LINES_M			0x00000FFF
#define SET_TVL(x)				x-1
#define V_LINES_M				0x00FFF000
#define SET_VL(x)				(x << 12)
#define V_SYNC_M				0xFF000000
#define SET_VS(x)				(x << 24)
#define V_FPOR_M				0x00000FFF
#define V_BPOR_M				0x00FFF000
#define V_POL_M				0x80000000

#define DMA_FRAME_READY		0x80000000	// Top bit of total pixel register to tell hardware the frame buffer is valid
#define DMA_FIFO_RST			0x40000000	// Next bit down is DMA FIFO reset signal

#define SET_HFP(x)				x-1
#define SET_VFP(x)				x
#define SET_BP(x)				(x << 12)
#define SET_POL(x)				(x << 31)

#define VGA_IRQ_EN				0x00000001

struct vga_creg_map {

	volatile uint32_t	h_ctrl1;
	volatile uint32_t	h_ctrl2;
	volatile uint32_t	v_ctrl1;
	volatile uint32_t	v_ctrl2;
	volatile uint32_t	px_nco_lsbs;
	volatile uint32_t	px_nco_msbs;
	volatile uint32_t	vga_fbuf_addr;
	volatile uint32_t	total_pixels;
	volatile uint32_t	irq_reg;

};

struct vga_prams {

	uint32_t	tot_h_px;
	uint32_t	h_px;
	uint32_t	h_sync_len;
	uint32_t	h_fporch;
	uint32_t	h_bporch;
	uint8_t		h_pol;
	uint32_t	tot_v_ln;
	uint32_t	v_ln;
	uint32_t	v_sync_len;
	uint32_t	v_fporch;
	uint32_t	v_bporch;
	uint8_t		v_pol;
	uint32_t	px_clk;
};

extern struct vga_prams	vga_table[];
extern struct vga_prams 	*vga_data;

//VGA Controller related functions
void set_vga_prams( uint8_t table_idx );
void vga_disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
void vga_disp_flush_hard(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
void vga_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
void vga_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color);
//void flush_wait( struct _lv_disp_drv_t * disp_drv );

#endif

