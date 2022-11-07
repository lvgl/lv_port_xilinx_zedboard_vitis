/*
 * vga.c
 *
 *  Created on: 1 Jun 2018
 *      Author: peterb
 */

#include "vga.h"

#include "xscugic.h"
#include "support.h"

struct vga_prams 					*vga_data;
extern XScuGic 						xInterruptController;
extern cpu0_globals_t				*cpu0_globals;
struct vga_creg_map					*vga = (void*)VGA_CTRL_BASE;

static uint64_t get_nco_value( unsigned int px_clk );



struct vga_prams vga_table[] = {
//tot_h_px, h_px, h_syn_len, h_fpch, h_bpch, h_pol,    tot_v_ln,v_ln, v_syn_len, v_fpch, v_bpch, v_pol, 	px_clk
  {800,     640,  96,  		 16, 	 48,  	SYNC_NEG, 525, 		480, 	2,  	 10, 	 33, 	 SYNC_NEG, 25175000 },	// 640 x 480 @ 60 Hz Standard VGA
  {1056,    800,  128, 		 40, 	 88,  	SYNC_POS, 628, 		600, 	4,  	 1,  	 23, 	 SYNC_POS, 40000000 },	// 800 x 600 @ 60 Hz
  {1344,    1024, 136, 		 24, 	 160, 	SYNC_NEG, 806, 		768, 	6,  	 3,  	 29, 	 SYNC_NEG, 65000000 }, 	// 1024 x 768 @ 60 Hz
  {1328,    1024, 136, 		 24, 	 144, 	SYNC_NEG, 806, 		768, 	6,  	 3,  	 29, 	 SYNC_NEG, 75000000 }, 	// 1024 x 768 @ 70 Hz
  {1904,    1440, 152, 		 80, 	 232, 	SYNC_NEG, 934, 		900, 	6,  	 3,  	 25,	 SYNC_POS, 106500000},	// 1440 x 900 @ 60 Hz CVT Standard
  {1600,    1440, 32,  		 48, 	 80,  	SYNC_NEG, 926, 		900, 	6,  	 3,  	 17,	 SYNC_POS, 88750000 },	// *1440 x 900 @ 60 Hz CVT Reduced Blanking
  {1600,    1440, 32,  		 48, 	 80,  	SYNC_NEG, 942, 		900, 	6,  	 3,  	 33,	 SYNC_POS, 136750000},	// 1440 x 900 @ 75 Hz CVT Standard
  {1600,    1440, 32,  		 48, 	 80,  	SYNC_NEG, 948, 		900, 	6,  	 3,  	 39,	 SYNC_POS, 157000000},	// 1440 x 900 @ 85 Hz CVT Standard
  {1600,    1440, 32,		 48, 	 80,  	SYNC_NEG, 953, 		900, 	6,  	 3,  	 44,	 SYNC_POS, 182750000}	// 1440 x 900 @ 120 Hz CVT Reduced Blanking
};

void vga_irq_handler( void *p ) {

	vga->vga_fbuf_addr = cpu0_globals->gui.dma_src;
	cpu0_globals->gui.buf_switched = pdTRUE;
	XScuGic_Disable(&xInterruptController, XPAR_FABRIC_ZEDBOARD_AXI_VGA_0_FRM_CPT_IRQ_INTR);
	lv_disp_flush_ready((lv_disp_drv_t*)&cpu0_globals->gui.disp_drv);
}

void set_vga_prams( uint8_t table_idx ) {

	uint64_t	nco_val;
	int			Status;

	vga_data = &vga_table[table_idx];
	vga->h_ctrl1 = SET_THP( vga_data->tot_h_px );
	vga->h_ctrl1 |= SET_HP( vga_data->h_px );
	vga->h_ctrl1 |= SET_HS( vga_data->h_sync_len );
	vga->h_ctrl2 = SET_HFP( vga_data->h_fporch );
	vga->h_ctrl2 |= SET_BP( vga_data->h_bporch );
	vga->h_ctrl2 |= SET_POL( vga_data->h_pol );
	vga->v_ctrl1 = SET_TVL( vga_data->tot_v_ln );
	vga->v_ctrl1 |= SET_VL( vga_data->v_ln );
	vga->v_ctrl1 |= SET_VS( vga_data->v_sync_len );
	vga->v_ctrl2 = SET_VFP( vga_data->v_fporch );
	vga->v_ctrl2 |= SET_BP( vga_data->v_bporch );
	vga->v_ctrl2 |= SET_POL( vga_data->v_pol );
	nco_val = get_nco_value( vga_data->px_clk );
	vga->px_nco_lsbs = nco_val & 0xFFFFFFFF;
	vga->px_nco_msbs = ((nco_val & 0xFF00000000) >> 32);
	vga->vga_fbuf_addr = VGA_DDR_DMA_BASE;
	vga->total_pixels = (vga_data->h_px * vga_data->v_ln) | DMA_FIFO_RST;
	/*
	 * Connect the interrupt handler that will be called when an
	 * interrupt occurs for the device.
	 */
	Status = XScuGic_Connect(&xInterruptController, XPAR_FABRIC_ZEDBOARD_AXI_VGA_0_FRM_CPT_IRQ_INTR, \
			(Xil_ExceptionHandler)&vga_irq_handler, NULL);
	if (Status != XST_SUCCESS) {
		// TODO: Some error report...
	}
	XScuGic_SetPriorityTriggerType(&xInterruptController, XPAR_FABRIC_ZEDBOARD_AXI_VGA_0_FRM_CPT_IRQ_INTR, \
			0xB8, 0x3);
	return;
}

static void update_dual_buf( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *colour_p ) {

	lv_disp_t*	disp = _lv_refr_get_disp_refreshing();
	lv_coord_t	y, hres = lv_disp_get_hor_res(disp);
    uint16_t	a;
    lv_color_t	*buf_cpy;

    if( colour_p == disp_drv->draw_buf->buf1)
        buf_cpy = disp_drv->draw_buf->buf2;
    else
        buf_cpy = disp_drv->draw_buf->buf1;

    for(a = 0; a < disp->inv_p; a++) {
    	if(disp->inv_area_joined[a]) continue;
        lv_coord_t w = lv_area_get_width(&disp->inv_areas[a]);
        for(y = disp->inv_areas[a].y1; y <= disp->inv_areas[a].y2 && y < disp_drv->ver_res; y++) {
            memcpy(buf_cpy+(y * hres + disp->inv_areas[a].x1), colour_p+(y * hres + disp->inv_areas[a].x1), w * sizeof(lv_color_t));
        }
    }
}

void vga_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *colour_p) {

	static uint8_t	first_call = 1;

	if( first_call ) {
		cpu0_globals->gui.dma_src = (uint32_t)colour_p;
		first_call =  0;
		vga->total_pixels &= ~DMA_FIFO_RST; 	// Release Reset
		vga->total_pixels |= DMA_FRAME_READY;	// Start Proceedings
		vga->irq_reg = VGA_IRQ_EN;				// Enable DMA complete interrupt
	}
	if( lv_disp_flush_is_last( disp_drv ) ) {
		cpu0_globals->gui.dma_src = (uint32_t)colour_p;
		cpu0_globals->gui.buf_switched = pdFALSE;
		XScuGic_Enable(&xInterruptController, XPAR_FABRIC_ZEDBOARD_AXI_VGA_0_FRM_CPT_IRQ_INTR);
		while(!cpu0_globals->gui.buf_switched);// vTaskDelay(1);
		update_dual_buf(disp_drv, area, colour_p);
	} else lv_disp_flush_ready( disp_drv );
}

#define NCO_2POW36 68719476736LL
#define NCO_CLK 200000000L
uint64_t get_nco_value( unsigned int px_clk ) {

	uint64_t	nco;

	nco = (uint64_t)(((double)px_clk / (double)NCO_CLK) * (double)NCO_2POW36);
	return nco;
	/*  NCO36 configuraton
	    n = (b / c) * 2^36
	    b = (n / 2^36) * c

	    Where n = cntrl value.
	          b = desired clock rate
	          c = system clock rate
	*/
}



