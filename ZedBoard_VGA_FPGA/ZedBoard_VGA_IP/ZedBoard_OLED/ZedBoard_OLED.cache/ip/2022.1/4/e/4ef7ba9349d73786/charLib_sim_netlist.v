// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2022.1 (win64) Build 3526262 Mon Apr 18 15:48:16 MDT 2022
// Date        : Thu Sep 22 12:47:51 2022
// Host        : PETE-LT running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ charLib_sim_netlist.v
// Design      : charLib
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7z020clg484-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "charLib,blk_mem_gen_v8_4_5,{}" *) (* downgradeipidentifiedwarnings = "yes" *) (* x_core_info = "blk_mem_gen_v8_4_5,Vivado 2022.1" *) 
(* NotValidForBitStream *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix
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
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_blk_mem_gen_v8_4_5 U0
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
`pragma protect encoding = (enctype = "BASE64", line_length = 76, bytes = 17744)
`pragma protect data_block
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
f0mpl9jaWqo9h4W4iCLOhlo=
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
