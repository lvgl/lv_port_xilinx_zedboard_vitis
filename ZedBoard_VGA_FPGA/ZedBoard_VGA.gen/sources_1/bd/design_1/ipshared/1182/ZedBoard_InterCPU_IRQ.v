`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: PJBES
// Engineer: Pete Bone
// 
// Create Date: 06.10.2020 09:09:47
// Design Name: 
// Module Name: ZedBoard_InterCPU_IRQ
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module ZedBoard_InterCPU_IRQ(

    output  CPU0_IRQ,
    output  CPU1_IRQ,
    output  USB_RESET,

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
    input           s_axi_rready

    );

    // AXI Control Interface registers
    reg     [31:0]  axi_rd_reg;
    reg             up_wackr        = 1'b0;
    reg             up_rackr        = 1'b0;
    reg     [31:0]  ctrl_reg;

      // Internal AXI Control Interface connections
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
 
    //AXI Control - Assignments
    assign up_clk = s_axi_aclk;
    assign up_rstn = s_axi_aresetn;
    assign up_addr_w[13:0] = up_wreq ? up_waddr[13:0] : up_raddr[13:0];
    assign up_wack = up_wackr;
    assign up_rack = up_rackr;
    assign up_rdata_w = axi_rd_reg;
    
    // IRQs
    assign CPU0_IRQ = ctrl_reg[0];
    assign CPU1_IRQ = ctrl_reg[1];
    assign USB_RESET = ctrl_reg[31];
    
    // AXI control Interface
    always@( posedge up_clk ) begin
           
        // Acknowledge Signals
        up_wackr <= up_wreq;    
        up_rackr <= up_rreq;
        // Write
        if( up_addr_w[9:0] == 10'h000 & up_wreq ) ctrl_reg <= up_wdata_w;
//        if( up_addr_w[9:0] == 10'h001 & up_wreq ) status_reg <= up_wdata_w;  // Read ONLY

        // Read
        if (up_rreq == 1'b1) begin       
           case( up_addr_w[9:0] ) 
               10'h000: axi_rd_reg <= ctrl_reg;
           endcase
        end
    end

    // AXI Control Interface
    axi_ctrl_cpu_irq i_axi_ctrl (
        .up_rstn(s_axi_aresetn),
        .up_clk(s_axi_aclk),
        .up_axi_awvalid(s_axi_awvalid),
        .up_axi_awaddr(s_axi_awaddr),
        .up_axi_awready(s_axi_awready),
        .up_axi_wvalid(s_axi_wvalid),
        .up_axi_wdata(s_axi_wdata),
        .up_axi_wstrb(s_axi_wstrb),
        .up_axi_wready(s_axi_wready),
        .up_axi_bvalid(s_axi_bvalid),
        .up_axi_bresp(s_axi_bresp),
        .up_axi_bready(s_axi_bready),
        .up_axi_arvalid(s_axi_arvalid),
        .up_axi_araddr(s_axi_araddr),
        .up_axi_arready(s_axi_arready),
        .up_axi_rvalid(s_axi_rvalid),
        .up_axi_rresp(s_axi_rresp),
        .up_axi_rdata(s_axi_rdata),
        .up_axi_rready(s_axi_rready),
        .up_wreq(up_wreq),
        .up_waddr(up_waddr[13:0]),
        .up_wdata(up_wdata_w),
        .up_wack(up_wack),
        .up_rreq(up_rreq),
        .up_raddr(up_raddr[13:0]),
        .up_rdata(up_rdata_w),
        .up_rack(up_rack)
    );

endmodule
