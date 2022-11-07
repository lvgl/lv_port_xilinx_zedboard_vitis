// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2022.1 (win64) Build 3526262 Mon Apr 18 15:48:16 MDT 2022
// Date        : Thu Sep 22 12:47:51 2022
// Host        : PETE-LT running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode funcsim -rename_top charLib -prefix
//               charLib_ charLib_sim_netlist.v
// Design      : charLib
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7z020clg484-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "charLib,blk_mem_gen_v8_4_5,{}" *) (* downgradeipidentifiedwarnings = "yes" *) (* x_core_info = "blk_mem_gen_v8_4_5,Vivado 2022.1" *) 
(* NotValidForBitStream *)
module charLib
   (clka,
    addra,
    douta);
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTA CLK" *) (* x_interface_parameter = "XIL_INTERFACENAME BRAM_PORTA, MEM_SIZE 8192, MEM_WIDTH 32, MEM_ECC NONE, MASTER_TYPE OTHER, READ_LATENCY 1" *) input clka;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTA ADDR" *) input [10:0]addra;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTA DOUT" *) output [7:0]douta;

  wire [10:0]addra;
  wire clka;
  wire [7:0]douta;
  wire NLW_U0_dbiterr_UNCONNECTED;
  wire NLW_U0_rsta_busy_UNCONNECTED;
  wire NLW_U0_rstb_busy_UNCONNECTED;
  wire NLW_U0_s_axi_arready_UNCONNECTED;
  wire NLW_U0_s_axi_awready_UNCONNECTED;
  wire NLW_U0_s_axi_bvalid_UNCONNECTED;
  wire NLW_U0_s_axi_dbiterr_UNCONNECTED;
  wire NLW_U0_s_axi_rlast_UNCONNECTED;
  wire NLW_U0_s_axi_rvalid_UNCONNECTED;
  wire NLW_U0_s_axi_sbiterr_UNCONNECTED;
  wire NLW_U0_s_axi_wready_UNCONNECTED;
  wire NLW_U0_sbiterr_UNCONNECTED;
  wire [7:0]NLW_U0_doutb_UNCONNECTED;
  wire [10:0]NLW_U0_rdaddrecc_UNCONNECTED;
  wire [3:0]NLW_U0_s_axi_bid_UNCONNECTED;
  wire [1:0]NLW_U0_s_axi_bresp_UNCONNECTED;
  wire [10:0]NLW_U0_s_axi_rdaddrecc_UNCONNECTED;
  wire [7:0]NLW_U0_s_axi_rdata_UNCONNECTED;
  wire [3:0]NLW_U0_s_axi_rid_UNCONNECTED;
  wire [1:0]NLW_U0_s_axi_rresp_UNCONNECTED;

  (* C_ADDRA_WIDTH = "11" *) 
  (* C_ADDRB_WIDTH = "11" *) 
  (* C_ALGORITHM = "1" *) 
  (* C_AXI_ID_WIDTH = "4" *) 
  (* C_AXI_SLAVE_TYPE = "0" *) 
  (* C_AXI_TYPE = "1" *) 
  (* C_BYTE_SIZE = "9" *) 
  (* C_COMMON_CLK = "0" *) 
  (* C_COUNT_18K_BRAM = "1" *) 
  (* C_COUNT_36K_BRAM = "0" *) 
  (* C_CTRL_ECC_ALGO = "NONE" *) 
  (* C_DEFAULT_DATA = "0" *) 
  (* C_DISABLE_WARN_BHV_COLL = "0" *) 
  (* C_DISABLE_WARN_BHV_RANGE = "0" *) 
  (* C_ELABORATION_DIR = "./" *) 
  (* C_ENABLE_32BIT_ADDRESS = "0" *) 
  (* C_EN_DEEPSLEEP_PIN = "0" *) 
  (* C_EN_ECC_PIPE = "0" *) 
  (* C_EN_RDADDRA_CHG = "0" *) 
  (* C_EN_RDADDRB_CHG = "0" *) 
  (* C_EN_SAFETY_CKT = "0" *) 
  (* C_EN_SHUTDOWN_PIN = "0" *) 
  (* C_EN_SLEEP_PIN = "0" *) 
  (* C_EST_POWER_SUMMARY = "Estimated Power for IP     :     1.2196 mW" *) 
  (* C_FAMILY = "zynq" *) 
  (* C_HAS_AXI_ID = "0" *) 
  (* C_HAS_ENA = "0" *) 
  (* C_HAS_ENB = "0" *) 
  (* C_HAS_INJECTERR = "0" *) 
  (* C_HAS_MEM_OUTPUT_REGS_A = "0" *) 
  (* C_HAS_MEM_OUTPUT_REGS_B = "0" *) 
  (* C_HAS_MUX_OUTPUT_REGS_A = "0" *) 
  (* C_HAS_MUX_OUTPUT_REGS_B = "0" *) 
  (* C_HAS_REGCEA = "0" *) 
  (* C_HAS_REGCEB = "0" *) 
  (* C_HAS_RSTA = "0" *) 
  (* C_HAS_RSTB = "0" *) 
  (* C_HAS_SOFTECC_INPUT_REGS_A = "0" *) 
  (* C_HAS_SOFTECC_OUTPUT_REGS_B = "0" *) 
  (* C_INITA_VAL = "0" *) 
  (* C_INITB_VAL = "0" *) 
  (* C_INIT_FILE = "charLib.mem" *) 
  (* C_INIT_FILE_NAME = "charLib.mif" *) 
  (* C_INTERFACE_TYPE = "0" *) 
  (* C_LOAD_INIT_FILE = "1" *) 
  (* C_MEM_TYPE = "3" *) 
  (* C_MUX_PIPELINE_STAGES = "0" *) 
  (* C_PRIM_TYPE = "1" *) 
  (* C_READ_DEPTH_A = "2048" *) 
  (* C_READ_DEPTH_B = "2048" *) 
  (* C_READ_LATENCY_A = "1" *) 
  (* C_READ_LATENCY_B = "1" *) 
  (* C_READ_WIDTH_A = "8" *) 
  (* C_READ_WIDTH_B = "8" *) 
  (* C_RSTRAM_A = "0" *) 
  (* C_RSTRAM_B = "0" *) 
  (* C_RST_PRIORITY_A = "CE" *) 
  (* C_RST_PRIORITY_B = "CE" *) 
  (* C_SIM_COLLISION_CHECK = "ALL" *) 
  (* C_USE_BRAM_BLOCK = "0" *) 
  (* C_USE_BYTE_WEA = "0" *) 
  (* C_USE_BYTE_WEB = "0" *) 
  (* C_USE_DEFAULT_DATA = "0" *) 
  (* C_USE_ECC = "0" *) 
  (* C_USE_SOFTECC = "0" *) 
  (* C_USE_URAM = "0" *) 
  (* C_WEA_WIDTH = "1" *) 
  (* C_WEB_WIDTH = "1" *) 
  (* C_WRITE_DEPTH_A = "2048" *) 
  (* C_WRITE_DEPTH_B = "2048" *) 
  (* C_WRITE_MODE_A = "WRITE_FIRST" *) 
  (* C_WRITE_MODE_B = "WRITE_FIRST" *) 
  (* C_WRITE_WIDTH_A = "8" *) 
  (* C_WRITE_WIDTH_B = "8" *) 
  (* C_XDEVICEFAMILY = "zynq" *) 
  (* downgradeipidentifiedwarnings = "yes" *) 
  (* is_du_within_envelope = "true" *) 
  charLib_blk_mem_gen_v8_4_5 U0
       (.addra(addra),
        .addrb({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .clka(clka),
        .clkb(1'b0),
        .dbiterr(NLW_U0_dbiterr_UNCONNECTED),
        .deepsleep(1'b0),
        .dina({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .dinb({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .douta(douta),
        .doutb(NLW_U0_doutb_UNCONNECTED[7:0]),
        .eccpipece(1'b0),
        .ena(1'b0),
        .enb(1'b0),
        .injectdbiterr(1'b0),
        .injectsbiterr(1'b0),
        .rdaddrecc(NLW_U0_rdaddrecc_UNCONNECTED[10:0]),
        .regcea(1'b0),
        .regceb(1'b0),
        .rsta(1'b0),
        .rsta_busy(NLW_U0_rsta_busy_UNCONNECTED),
        .rstb(1'b0),
        .rstb_busy(NLW_U0_rstb_busy_UNCONNECTED),
        .s_aclk(1'b0),
        .s_aresetn(1'b0),
        .s_axi_araddr({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arburst({1'b0,1'b0}),
        .s_axi_arid({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arready(NLW_U0_s_axi_arready_UNCONNECTED),
        .s_axi_arsize({1'b0,1'b0,1'b0}),
        .s_axi_arvalid(1'b0),
        .s_axi_awaddr({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awburst({1'b0,1'b0}),
        .s_axi_awid({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awready(NLW_U0_s_axi_awready_UNCONNECTED),
        .s_axi_awsize({1'b0,1'b0,1'b0}),
        .s_axi_awvalid(1'b0),
        .s_axi_bid(NLW_U0_s_axi_bid_UNCONNECTED[3:0]),
        .s_axi_bready(1'b0),
        .s_axi_bresp(NLW_U0_s_axi_bresp_UNCONNECTED[1:0]),
        .s_axi_bvalid(NLW_U0_s_axi_bvalid_UNCONNECTED),
        .s_axi_dbiterr(NLW_U0_s_axi_dbiterr_UNCONNECTED),
        .s_axi_injectdbiterr(1'b0),
        .s_axi_injectsbiterr(1'b0),
        .s_axi_rdaddrecc(NLW_U0_s_axi_rdaddrecc_UNCONNECTED[10:0]),
        .s_axi_rdata(NLW_U0_s_axi_rdata_UNCONNECTED[7:0]),
        .s_axi_rid(NLW_U0_s_axi_rid_UNCONNECTED[3:0]),
        .s_axi_rlast(NLW_U0_s_axi_rlast_UNCONNECTED),
        .s_axi_rready(1'b0),
        .s_axi_rresp(NLW_U0_s_axi_rresp_UNCONNECTED[1:0]),
        .s_axi_rvalid(NLW_U0_s_axi_rvalid_UNCONNECTED),
        .s_axi_sbiterr(NLW_U0_s_axi_sbiterr_UNCONNECTED),
        .s_axi_wdata({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_wlast(1'b0),
        .s_axi_wready(NLW_U0_s_axi_wready_UNCONNECTED),
        .s_axi_wstrb(1'b0),
        .s_axi_wvalid(1'b0),
        .sbiterr(NLW_U0_sbiterr_UNCONNECTED),
        .shutdown(1'b0),
        .sleep(1'b0),
        .wea(1'b0),
        .web(1'b0));
endmodule
`pragma protect begin_protected
`pragma protect version = 1
`pragma protect encrypt_agent = "XILINX"
`pragma protect encrypt_agent_info = "Xilinx Encryption Tool 2022.1"
`pragma protect key_keyowner="Synopsys", key_keyname="SNPS-VCS-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
RgPKnWr9n0dGgttm3akiFhAlfB96usOQYxnEmPhGyTGg1AbizYAjGPWLXBWl50n/d0IA71ci4aJB
wt6mtfyNADm3ZReK7D3mKu037BOgxryoEwwf1kiC6q/PllxsdAgEMfQrfHJ3E2AzSpdYjoxVYito
y0JW6CUDcWvWa4WV0EA=

`pragma protect key_keyowner="Aldec", key_keyname="ALDEC15_001", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
M0l6KpOGH3jL8eRt3NCD7e2USYnkg5H9GAnE1PKmnjiouFN3Y8kjWA2PZDAQLm9UW+TsC1HeVlzO
WjNCHkjR/6ubCsIcWfpPZWdIuAenlsyq8Y9l6b8vMj8JSbDEOiFF/GHSbKsn22MJdDJKEhHFK6GV
s8gR2vywRFwG69gIRE4qGhVB+WIg8GJrDpDMYH6lCjMkTrjXuKDUcNlJN3NPLuhJ7tsditwf1pr5
moJRmGpJnip/rGm0g4o4A6ev4CtePjoao8C1wFtzHkERX9oenhh7cGjDMejU5IrLv8NxFnLj1FpB
9MuF1beTU20NI5oAn6zLiLiOtXjf0ghU3AN4DA==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VELOCE-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
hAsrUfp6Qgjm8yBjNYTEtQmVQmMxzL8TE/3oiQSxSI3+yEkXAbQCXkT9mo+LCdv+fGECOB0istHd
eLtbsiYbxjxNxYkXiUrRE5O+aSxynIray+uF9DJigTEUZu8JJXUbzxK4DDUu1Lm9tpGps4+Prz1m
0gkj13RT/Y/418s2VTw=

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VERIF-SIM-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
BP/54Wm/GJmb1jy0bxWJJX4aiKyiWPVh4X3VL30BQrmX4PlEsNKzBJH3Qu8IIYERfnFP0ifAgboa
vypMQ5Ed0BrMePGkWIgT6I8hxJCMFpHdkSK7m1giSKyZzFfTOrVqoNFXE+qdzLfY1J5hBWCvouYo
jllavK4N3gF9FLScH2AUWYVMcVth2QPaTAU2NLnAUNH8kgtBjBfc8/KbPPTznD1QNVqvFstzcbTA
hGQ1ETVPvINQ0KqxxAG5PRhtQD4+pC+hr/Tvk+RSvGyBOfy9zE86OXkJiYs9dSFhNiMFmCPL9DBO
se4OxNNC0/7aBtb1mkSEA9YFDYEb9jS7Jasy2A==

`pragma protect key_keyowner="Real Intent", key_keyname="RI-RSA-KEY-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
SnzT9DVH3xlEN8nrr2YrlvTO5qj7L22d7WaBcuKyTaiHoIwwFHrC4HQbfs0TAwkdWcOgmJoATPSF
F6qm0KiddbrlERF3MfKUldeGBJtqLdX+zGw7+3JD7S+HB9dIMOFOHy+IiCZp1/Pz8epKpi238cel
rcVoJQKz406wmXDvOo8KsT+XhRLs9BVCrBErPGGXKYDk6NXAp0duOgQE9DbslzMU83M/kUC7uERV
tQW02240peKQFp2elEZC7Tetvgp0TaFTtJiKN45REi8GQUCKGa85JjNIk1qb/+k95TIIP1xrHirc
6iX7qbwnPetv8TVu2NjkZ0WDEK5RXdOXcxBwHA==

`pragma protect key_keyowner="Xilinx", key_keyname="xilinxt_2021_07", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
erkR82V0jX8ytva+9MzEs0c75Z7j7TsgxWRLNAUfbbU28i+U9YzuZlSfYU75M0f9jx1gvxtBrfKv
cNHVdkR+i5zfHDZsDwfMEEBhs8wzDCKqe+eex6BBEvlIOesCPXrr2RozQgaQ1PBh/os2Arfu+873
BjsVxFJkbhpzIqlddOo/XZV9Yi+eih7A7pXXEBR6IL7Poo4Ka49MiVQU0xJrDTm+ddOuMPDRRD7g
dsxS/uzdcBcO2myV6g/7YH/C2Ce9s6+UywJN/0JeXSqwA7bsBqqnfFicVAT0lckLopMLiuzK7dsN
EwhFeqoetciFrDIj9+o0xDMWBZhgNP1u68vURA==

`pragma protect key_keyowner="Metrics Technologies Inc.", key_keyname="DSim", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
R+BI65BKLT0I9hEtsxGnDyM5XY9gzULeTPOSDXvd3KWOzZJAx6C0xlbyZcFZhAEG/QIK2yd0wAi6
IUWxyF/sx3HsqKjhVi5KxnpuXDBOZVoj811O7JukedFVmDW7OHGtBkuiJ5X5irw7mfsEKRQmF/1i
V6lj9HYHZEjxtDeZjACsLY4y1QxWalSKT4HIMOHznBLL8dLbGMlS+ZmFuFn0gcwZavVl7gTkTtkf
W0gn01A9ru7NKsf+iLX0kj4dgItPu9N2g02M1vWQ9UUQEVvfV7lUc7GY1suibrD8aEkhH9S7lZ7n
bFsT4qxyvzg8ML6v4g4v7N6VuyhEtgFgNd725Q==

`pragma protect key_keyowner="Atrenta", key_keyname="ATR-SG-RSA-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=384)
`pragma protect key_block
jNUVKiwH68vHsU54idgvKwaVJcoxTUuxfgrQpbpmM/IpesWA3wHsGzYClwAxkKzw3KRnFyQqTWcZ
yj1EQ2CMBxlJ0kyNbZW8OK8pXzeigToZ0U6Aq3Gy+j7wBbhe83wE1Ygn82sK8dHTEulvaRLn/c5r
ispy1s7jMKIvYNzoUuZrgyBQyfaYmdqUia8XlQjFd+VwzhTXKwzvmaqHWyaHjfBKeCooO7+oUxMG
OJg83W54EVe9ronFQ8Wr9EOL8ia7qelCAgyQe/bC0HHCoMAm8apI7sX23iMR/wMiPP5V2bQzycy+
rBX/+SWkqSeIE1FLm+muFPvrE7iLwJaW8d1fzdFFjAZ5aIXArbWNfwbK8S0TczXc9lEzmpb69rwA
UJIrs4alo81qGQ32UFhjuMQjX75O9Od1HWHDj5PFaT/Ja5Ly+bK8Cc3gfO6dCE81m7d+B2JBZ/Hl
tBA19QuOAYwT2EIPOdpaVtCULb33cWODWu3qQFhZMmDzKTb3kwpcr0LL

`pragma protect key_keyowner="Cadence Design Systems.", key_keyname="CDS_RSA_KEY_VER_1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
MnzcA2swaxH1LRacKDou0ZmiiMRETbWIdHgeBgyQz7ILronsXLoB/C20WuFNGEVSiL2/51EZ6MXZ
vMHI8fFcMQCJcuTBDBibUMKv6bXI9s8fTbtrBZppbF/R1icG5JYhqmX4aRnv4W/dxJRjI2L35nLN
Y48E9OfgfkD1sr+IRwx8WEKFmUhuk8dLe0VOK7ywe3XEcneYvrz+HhPj16bGmNfMwNnDgZ3gKKZD
hRnys+jzvAX3HyISrErWXhMKrhWMxXeTNFJCqNQ0LWAVHQYwyKnF5xVpyXSuGNSIrva+QXqOrZBG
3VNLirNVtMRiKLfwZeMaqvswkqBDAa53utlAAA==

`pragma protect key_keyowner="Synplicity", key_keyname="SYNP15_1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
d0xXkKtsKM8GvXhDZr16p1+oE3uBtD04BJ76eGGIEj/CFECfHVy9qsJ43oSKjas0+AJr4GFFnVb6
X7gJV6MmX/OboC9ier5joUCGz0mxVzkRZK9a+LPEDcg0K6+cLE36kr+FfxW9Uk2816EHBCMCf5mK
A4eAhSmAb5Nq74F/q0quiG416npbny7faiQ+xmPDfYYiM3UuMKaD4iE8ODlz1w5xThPllWESf3LZ
NTkw6fozyTqZ47vvE21O3dgIGAY1v+C6BwlCK24VwPJa1xs9csY+qTk31j5jjAc1ExlB6QF7t9UH
lk70qdNPWxT87OH7kFT8UvPO1D6BTC3/WkDZfA==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-PREC-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
LYhdzKTvo93uJrAaiP5OfCDuOnu2BSvPnxlv6I7h2n1+xHtj34LNNKzWEgOg9dUV9cYDaHYUjjEt
DKdWcz6mZ61d5qyxAhpv67fc90v7JVgtOAcT94/Yb+AuLxXFcGA6Gic7uoJtUgz6JmTnb22Dxdjc
KuIewDj4IOTfP8XGXKTaF+cNp0CFrQgTAcVSQFyLFxr0I/9h3S+GZLecA7ntEeHEOfCJzPvy0ddi
7MCdQWECLb+fXC0IAn8V95TumcpINiRAX1BHi9IGJ4QoMrb3jOCrPkFhDMTJj2aiImUWdi/l/0QE
d7wcXlgIEYVeoKYUOJ4mqy+zZPUbLNeOPADUDQ==

`pragma protect data_method = "AES128-CBC"
`pragma protect encoding = (enctype = "BASE64", line_length = 76, bytes = 17376)
`pragma protect data_block
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
`pragma protect end_protected
`ifndef GLBL
`define GLBL
`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;
    parameter GRES_WIDTH = 10000;
    parameter GRES_START = 10000;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    wire GRESTORE;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;
    wire FCSBO_GLBL;
    wire [3:0] DO_GLBL;
    wire [3:0] DI_GLBL;
   
    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;
    reg GRESTORE_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (strong1, weak0) GSR = GSR_int;
    assign (strong1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;
    assign (strong1, weak0) GRESTORE = GRESTORE_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

    initial begin 
	GRESTORE_int = 1'b0;
	#(GRES_START);
	GRESTORE_int = 1'b1;
	#(GRES_WIDTH);
	GRESTORE_int = 1'b0;
    end

endmodule
`endif
