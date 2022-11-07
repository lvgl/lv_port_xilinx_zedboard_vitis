`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// 
// Engineer:  Peter J. Bone
// 
// Create Date: 30.05.2018 14:19:18
// Design Name: 
// Module Name: Zedboard_AXI_VGA
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 24-bit VGA controller generates upto 16 Million Colour Video Output to drive various screen devices 
// 
// Dependencies: 
// 
// Revision:
// Revision 1 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////
												
module Zedboard_AXI_VGA (
    
    // VGA Interface
    input           vga_clk,
    output          h_sync,
    output          v_sync,
    output [7:0]    r,
    output [7:0]    g,
    output [7:0]    b,
    output          frm_cpt_irq,
    
    // AXI Control Interace
    input           s_axi_aclk,
    input           s_axi_aresetn,
    input           s_axi_awvalid,
    input   [31:0]  s_axi_awaddr,
    output          s_axi_awready,
    input           s_axi_wvalid,
    input   [31:0]  s_axi_wdata,
    input   [3:0]   s_axi_wstrb,
    output          s_axi_wready,
    output          s_axi_bvalid,
    output  [1:0]   s_axi_bresp,
    input           s_axi_bready,
    input           s_axi_arvalid,
    input   [31:0]  s_axi_araddr,
    output          s_axi_arready,
    output          s_axi_rvalid,
    output  [31:0]  s_axi_rdata,
    output  [1:0]   s_axi_rresp,
    input           s_axi_rready,
    
    // AXI Master DMA interface
    input           m_axi_aclk,
    input           m_axi_aresetn,
    input           m_axi_arready,
    input   [31:0]  m_axi_rdata,
    input           m_axi_rlast,
    input           m_axi_rvalid,
    output  [31:0]  m_axi_araddr,
    output  [7:0]   m_axi_arlen,
    output  [2:0]   m_axi_arsize,
    output  [1:0]   m_axi_arburst,
    output  [2:0]   m_axi_arprot,
    output          m_axi_arvalid,
    output          m_axi_rready
    );

    // AXI DMA Master parameters
    parameter    IDLE = 8'h01, ADDR_READY = 8'h02, READ = 8'h04, READ_REQ = 8'h08, FIFO_WAIT = 8'h10;
      
    // VGA registers
    reg         h_sync_r = 0;
    reg [10:0]  h_cnt = 11'd0;
    reg         v_sync_r = 0;
    reg [10:0]  v_cnt = 11'd0;
    reg [35:0]  px_clk_nco_r = 36'd0;
    reg         irq = 0;
    reg         fifo_ready; // lets VGA processing know when there is some data in the fifo
  
    // AXI Control Registers
    reg     [31:0]              axi_rd_reg;
    reg                         up_wackr   = 1'b0;
    reg                         up_rackr   = 1'b0;
    reg     [31:0]              h_cntrl1   = 32'h8840053f;
    reg     [31:0]              h_cntrl2   = 32'h000A0017;
    reg     [31:0]              v_cntrl1   = 32'h06300325;
    reg     [31:0]              v_cntrl2   = 32'h0001D003;
    reg     [31:0]              msbs_px_nco = 32'h00000005;  // Use bits 3->0 for msbs of value
    reg     [31:0]              lsbs_px_nco = 32'h33333333;  // Use bits 31->0 for lsbs of value
    reg     [31:0]              ddr_fbuf_addr = 32'h0F000000; // Reserved VGA DDR RAM Base Address (16MB)
    reg     [31:0]              total_pixels = 32'h400C0000;  // Default is for 1024x768 (786432) DMA controller uses this to know when
                                                              // to reset to beginning of buffer.  Top bit (31) Tells the DMA when CPU has first loaded frame buffer
                                                              // Bit 30 resets DMA FIFO it is held in reset after power up until the software driver clears it...
    reg     [31:0]              irq_reg = 32'h00000000;
    
    // AXI DMA Master Registers
    reg     [31:0]  ddr_address = 32'b0;
    reg             ar_valid = 1'b0;
    reg             r_ready = 1'b0;
    reg     [7:0]   state_r = IDLE;
    reg     [22:0]  dma_cnt = 23'b0;
 
    // Internal AXI Control connections
    wire    [13:0]  up_waddr;
    wire    [13:0]  up_raddr;
    wire    [13:0]  up_addr_w;
    wire    [31:0]  up_wdata_w;
    wire    [31:0]  up_rdata_w;
    wire            up_clk;
    wire            up_rstn;
    wire            up_wack;
    wire            up_rack;
    wire            up_wreq;
    wire            up_rreq;
    
    // Internal VGA related connections
    wire            px_clk;
    wire    [35:0]  px_nco_phase;
    wire    [35:0]  px_nco_cntrl;
    wire    [11:0]  thpixels;   // pixels per line
    wire    [11:0]  hpixels;    // viewable pixels
    wire    [7:0]   hpulse;     // hsync length
    wire    [11:0]  hfp;        // horizontal front porch
    wire    [11:0]  hbp;        // horizontal back porch
    wire            hspol;      // horizontal sync polarity 0 = neg, 1 = pos
    wire    [11:0]  tvlines;    // lines per frame
    wire    [11:0]  vlines;     // viewable lines
    wire    [7:0]   vpulse;     // vsync length
    wire    [11:0]  vfp;        // vertical front porch
    wire    [11:0]  vbp;        // vertical back porch
    wire            vspol;      // vertical sync polarity 0 = neg, 1 = pos
    wire    [31:0]  dma_data;   // AXI DMA data in  
    wire    [31:0]  rgb_data;   // This only needs to be 24-bits normally but we allow for different scenarios
    wire            vgamem_rd_en;
    wire            fifo_readyw; // lets VGA processing know when there is some data in the fifo
    wire            dma_ready;  // To let DMA processing know when CPU has loaded frame buffer
    wire            dma_rst;
    wire            rst_busy;
    wire            rst_busy_wr;
    wire            rst_busy_rd;
    wire            rst_ready;
    wire    [24:0]  dma_tcnt;
    wire            irq_en;
   
    // VGA Assignments
    assign h_sync = hspol ? h_sync_r : ~h_sync_r;
    assign v_sync = vspol ? v_sync_r : ~v_sync_r;
    assign r = ( (v_cnt >= vlines) || (h_cnt >= hpixels) ) ? 8'd0 : rgb_data[23:16];  // Send rgb data or blank during blanking phase
    assign g = ( (v_cnt >= vlines) || (h_cnt >= hpixels) ) ? 8'd0 : rgb_data[15:8];
    assign b = ( (v_cnt >= vlines) || (h_cnt >= hpixels) ) ? 8'd0 : rgb_data[7:0];
    assign px_nco_cntrl[35:32] = msbs_px_nco[3:0];
    assign px_nco_cntrl[31:0] = lsbs_px_nco[31:0];
    assign thpixels = h_cntrl1[11:0];
    assign hpixels = h_cntrl1[23:12];
    assign hpulse = h_cntrl1[31:24];
    assign hfp = h_cntrl2[11:0];
    assign hbp = h_cntrl2[23:12];
    assign hspol = h_cntrl2[31];
    assign tvlines = v_cntrl1[11:0];
    assign vlines = v_cntrl1[23:12];
    assign vpulse = v_cntrl1[31:24];
    assign vfp = v_cntrl2[11:0];
    assign vbp = v_cntrl2[23:12];
    assign vspol = v_cntrl2[31];
    assign vgamem_rd_en  = (( h_cnt < hpixels ) & (v_cnt < vlines)) & fifo_ready;
    assign dma_ready = total_pixels[31];
    assign dma_rst = total_pixels[30];
    assign rst_busy = rst_busy_wr | rst_busy_rd;
    assign rst_ready = ~rst_busy;
    assign dma_tcnt = (total_pixels[22:0] << 2)-128;
    assign irq_en = irq_reg[0];
    assign frm_cpt_irq = irq;
    
    //AXI Control Assignments
    assign up_clk = s_axi_aclk;
    assign up_rstn = s_axi_aresetn;
    assign up_addr_w[13:0] = up_wreq ? up_waddr[13:0] : up_raddr[13:0];
    assign up_wack = up_wackr;
    assign up_rack = up_rackr;
    assign up_rdata_w = axi_rd_reg;

    // AXI DMA Master Assignments
    assign m_axi_rready = r_ready;
    assign m_axi_arvalid = ar_valid;
    assign m_axi_arprot = 3'b0;             // Standard mode...
    assign m_axi_arsize = 3'b010;           // Fix burst size to 4 bytes (32-bits)
    assign m_axi_arlen  = 8'h1f;            // Transfer length 32
    assign m_axi_arburst = 2'b01;           // Set Address Increment mode
    assign m_axi_araddr = ddr_address; 
    assign dma_data = m_axi_rdata;       
    
    // VGA Processing
    always@( posedge px_clk ) begin
    
        if( fifo_ready & dma_ready & rst_ready ) begin
            if( h_cnt >= thpixels ) begin
                h_cnt <= 11'd0;
                if( v_cnt >= tvlines ) begin
                    v_cnt <= 11'd0;
                end else begin
                    v_cnt <= v_cnt + 1;
                end
            end else begin
                h_cnt <= h_cnt + 1;
            end
            if( (v_cnt >= (vlines + vfp )) && (v_cnt < (vlines + vfp + vpulse)) ) v_sync_r <= 1; else v_sync_r <= 0;  // Generate sync pulses...
            if( (h_cnt >= (hpixels + hfp )) && (h_cnt < (hpixels + hfp + hpulse)) ) h_sync_r <= 1; else h_sync_r <= 0;  
        end else if( fifo_readyw & rst_ready ) begin
            fifo_ready <= fifo_readyw;
        end
    end
    
    // AXI Control Interface Processsing
    always@( posedge up_clk ) begin
        
        // Acknowledge Signals
        up_wackr <= up_wreq;    
        up_rackr <= up_rreq;
        // Write
        if( up_addr_w[9:0] == 10'h000 & up_wreq ) h_cntrl1 <= up_wdata_w;
        if( up_addr_w[9:0] == 10'h001 & up_wreq ) h_cntrl2 <= up_wdata_w;
        if( up_addr_w[9:0] == 10'h002 & up_wreq ) v_cntrl1 <= up_wdata_w;
        if( up_addr_w[9:0] == 10'h003 & up_wreq ) v_cntrl2 <= up_wdata_w;
        if( up_addr_w[9:0] == 10'h004 & up_wreq ) lsbs_px_nco <= up_wdata_w; 
        if( up_addr_w[9:0] == 10'h005 & up_wreq ) msbs_px_nco <= up_wdata_w;
        if( up_addr_w[9:0] == 10'h006 & up_wreq ) ddr_fbuf_addr <= up_wdata_w;
        if( up_addr_w[9:0] == 10'h007 & up_wreq ) total_pixels <= up_wdata_w;
        if( up_addr_w[9:0] == 10'h008 & up_wreq ) irq_reg <= up_wdata_w;
                
        // Read
        if (up_rreq == 1'b1) begin       
           case( up_addr_w[9:0] ) 
               10'h000: axi_rd_reg <= h_cntrl1;
               10'h001: axi_rd_reg <= h_cntrl2;
               10'h002: axi_rd_reg <= v_cntrl1;
               10'h003: axi_rd_reg <= v_cntrl2;
               10'h004: axi_rd_reg <= lsbs_px_nco;
               10'h005: axi_rd_reg <= msbs_px_nco;
               10'h006: axi_rd_reg <= ddr_fbuf_addr;
               10'h007: axi_rd_reg <= total_pixels;
               10'h008: axi_rd_reg <= irq_reg;
               default: axi_rd_reg <= 32'hDEADDEAD;
           endcase
        end
    end
    
    // AXI Master DMA processing
    always@( posedge m_axi_aclk ) begin
    
        case ( state_r )
            
            IDLE: begin
                irq <= 1'b0;
                if( /*fifo_fill &*/ dma_ready & rst_ready ) begin
                    if( ddr_address == 32'b0 ) begin
                        ddr_address <= ddr_fbuf_addr;
                        dma_cnt <= 23'b0;
                    end
                    state_r <= READ_REQ;
                end
            end
            
            READ_REQ: begin
               // Set DMA transfer address from ddr_address register
               if( !fifo_readyw ) begin
                   ar_valid <= 1'b1;
                   state_r <= ADDR_READY;
               end         
            end           
            
            ADDR_READY: begin
                if( m_axi_arready ) begin
                    ar_valid <= 1'b0;
                    r_ready <= 1'b1;
                    state_r <= READ; 
                end
            end
            
            READ: begin
                if( m_axi_rlast) begin
                    r_ready <= 1'b0;
                    if( dma_cnt < dma_tcnt ) begin 
                        ddr_address <= ddr_address + 128; //128 magic number is number of 32-bit words transfered 4 bytes x 32 transfer length
                        dma_cnt <= dma_cnt + 128;
                    end else begin
                        irq <= irq_en;
                        ddr_address <= 32'b0;
                    end
                    state_r <= IDLE;
                end
            end
            
            default: begin
                    state_r <= IDLE;
            end            
            
        endcase
    
    end
    
    BUFG px_clk_bufg (      // Clock distribution for px clock derived from NCO
        .O(px_clk),         // 1-bit output: Clock output
        .I(px_nco_phase[35]) // 1-bit input: Clock input
    );
    
    // AXI Control Interface
    axi_vga_ctrl i_axi_vga_ctrl (
        .up_rstn (up_rstn),
        .up_clk (up_clk),
        .up_axi_awvalid (s_axi_awvalid),
        .up_axi_awaddr (s_axi_awaddr),
        .up_axi_awready (s_axi_awready),
        .up_axi_wvalid (s_axi_wvalid),
        .up_axi_wdata (s_axi_wdata),
        .up_axi_wstrb (s_axi_wstrb),
        .up_axi_wready (s_axi_wready),
        .up_axi_bvalid (s_axi_bvalid),
        .up_axi_bresp (s_axi_bresp),
        .up_axi_bready (s_axi_bready),
        .up_axi_arvalid (s_axi_arvalid),
        .up_axi_araddr (s_axi_araddr),
        .up_axi_arready (s_axi_arready),
        .up_axi_rvalid (s_axi_rvalid),
        .up_axi_rresp (s_axi_rresp),
        .up_axi_rdata (s_axi_rdata),
        .up_axi_rready (s_axi_rready),
        .up_wreq(up_wreq),
        .up_waddr(up_waddr[13:0]),
        .up_wdata(up_wdata_w),
        .up_wack(up_wack),
        .up_rreq(up_rreq),
        .up_raddr(up_raddr[13:0]),
        .up_rdata(up_rdata_w),
        .up_rack(up_rack)
    );

    xpm_fifo_async # (
        .FIFO_MEMORY_TYPE ("auto"), //string; "auto", "block", or "distributed";
        .ECC_MODE ("no_ecc"), //string; "no_ecc" or "en_ecc";
        .RELATED_CLOCKS (0), //positive integer; 0 or 1
        .FIFO_WRITE_DEPTH (4096), //positive integer
        .WRITE_DATA_WIDTH (32), //positive integer
        .WR_DATA_COUNT_WIDTH (12), //positive integer
        .PROG_FULL_THRESH (3968), //positive integer
        .FULL_RESET_VALUE (0), //positive integer; 0 or 1
        .USE_ADV_FEATURES ("0707"), //string; "0000" to "1F1F";
        .READ_MODE ("fwft"), //string; "std" or "fwft";
        .FIFO_READ_LATENCY (0), //positive integer;
        .READ_DATA_WIDTH (32), //positive integer
        .RD_DATA_COUNT_WIDTH (12), //positive integer
        .PROG_EMPTY_THRESH (256), //positive integer
        .DOUT_RESET_VALUE ("0"), //string
        .CDC_SYNC_STAGES (2), //positive integer
        .WAKEUP_TIME (0) //positive integer; 0 or 2;
    ) fifo_inst (
        .rst (dma_rst),
        .wr_clk (m_axi_aclk),
        .wr_en ((m_axi_rvalid & m_axi_rready)),
        .din (dma_data),
        .full (),
        .overflow (),
        .prog_full (fifo_readyw),
        .wr_data_count (),
        .almost_full (),
        .wr_ack (),
        .wr_rst_busy (rst_busy_wr),
        .rd_clk (px_clk),
        .rd_en (vgamem_rd_en),
        .dout (rgb_data),
        .empty (),
        .underflow (),
        .rd_rst_busy (rst_busy_rd),
        .prog_empty (/*prog_empty*/),
        .rd_data_count (),
        .almost_empty (),
        .data_valid (),
        .sleep (1'b0),
        .injectsbiterr (1'b0),
        .injectdbiterr (1'b0),
        .sbiterr (),
        .dbiterr ()
    );

    // Pixel Clock NCO
    nco36_o36 px_nco (
        .clk(vga_clk),
        .cntrl(px_nco_cntrl),
        .phase_shift(36'd0),
        .phase(px_nco_phase)
    );       
endmodule

/*  NCO36 configuraton
    n = (b / c) * 2^36
    b = (n / 2^36) * c 
    
    Where n = cntrl value.
          b = desired clock rate
          c = system clock rate 
*/

module nco36_o36(
    input           clk,
    input   [35:0]  cntrl,
    input   [35:0]  phase_shift,
    output  [35:0]  phase
    );
    
    reg     [35:0]  phase_acc = 36'h000000000;
    reg     [35:0]  phase_out = 36'h000000000;
    
    assign phase = phase_out;
    
    always @( posedge clk ) begin
        phase_acc <= phase_acc + cntrl;
        phase_out <= phase_acc + phase_shift;
    end
endmodule
