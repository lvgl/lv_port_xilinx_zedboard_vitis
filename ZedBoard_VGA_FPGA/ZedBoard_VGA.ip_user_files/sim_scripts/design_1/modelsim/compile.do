vlib modelsim_lib/work
vlib modelsim_lib/msim

vlib modelsim_lib/msim/xilinx_vip
vlib modelsim_lib/msim/xpm
vlib modelsim_lib/msim/axi_lite_ipif_v3_0_4
vlib modelsim_lib/msim/lib_cdc_v1_0_2
vlib modelsim_lib/msim/interrupt_control_v3_1_4
vlib modelsim_lib/msim/axi_gpio_v2_0_28
vlib modelsim_lib/msim/xil_defaultlib
vlib modelsim_lib/msim/axi_infrastructure_v1_1_0
vlib modelsim_lib/msim/axi_vip_v1_1_12
vlib modelsim_lib/msim/processing_system7_vip_v1_0_14
vlib modelsim_lib/msim/generic_baseblocks_v2_1_0
vlib modelsim_lib/msim/axi_register_slice_v2_1_26
vlib modelsim_lib/msim/fifo_generator_v13_2_7
vlib modelsim_lib/msim/axi_data_fifo_v2_1_25
vlib modelsim_lib/msim/axi_crossbar_v2_1_27
vlib modelsim_lib/msim/proc_sys_reset_v5_0_13
vlib modelsim_lib/msim/xlconcat_v2_1_4
vlib modelsim_lib/msim/xlconstant_v1_1_7
vlib modelsim_lib/msim/xlslice_v1_0_2
vlib modelsim_lib/msim/xbip_utils_v3_0_10
vlib modelsim_lib/msim/c_reg_fd_v12_0_6
vlib modelsim_lib/msim/xbip_dsp48_wrapper_v3_0_4
vlib modelsim_lib/msim/xbip_pipe_v3_0_6
vlib modelsim_lib/msim/xbip_dsp48_addsub_v3_0_6
vlib modelsim_lib/msim/xbip_addsub_v3_0_6
vlib modelsim_lib/msim/c_addsub_v12_0_14
vlib modelsim_lib/msim/c_gate_bit_v12_0_6
vlib modelsim_lib/msim/xbip_counter_v3_0_6
vlib modelsim_lib/msim/c_counter_binary_v12_0_15
vlib modelsim_lib/msim/blk_mem_gen_v8_4_5
vlib modelsim_lib/msim/axi_protocol_converter_v2_1_26
vlib modelsim_lib/msim/axi_clock_converter_v2_1_25
vlib modelsim_lib/msim/axi_dwidth_converter_v2_1_26

vmap xilinx_vip modelsim_lib/msim/xilinx_vip
vmap xpm modelsim_lib/msim/xpm
vmap axi_lite_ipif_v3_0_4 modelsim_lib/msim/axi_lite_ipif_v3_0_4
vmap lib_cdc_v1_0_2 modelsim_lib/msim/lib_cdc_v1_0_2
vmap interrupt_control_v3_1_4 modelsim_lib/msim/interrupt_control_v3_1_4
vmap axi_gpio_v2_0_28 modelsim_lib/msim/axi_gpio_v2_0_28
vmap xil_defaultlib modelsim_lib/msim/xil_defaultlib
vmap axi_infrastructure_v1_1_0 modelsim_lib/msim/axi_infrastructure_v1_1_0
vmap axi_vip_v1_1_12 modelsim_lib/msim/axi_vip_v1_1_12
vmap processing_system7_vip_v1_0_14 modelsim_lib/msim/processing_system7_vip_v1_0_14
vmap generic_baseblocks_v2_1_0 modelsim_lib/msim/generic_baseblocks_v2_1_0
vmap axi_register_slice_v2_1_26 modelsim_lib/msim/axi_register_slice_v2_1_26
vmap fifo_generator_v13_2_7 modelsim_lib/msim/fifo_generator_v13_2_7
vmap axi_data_fifo_v2_1_25 modelsim_lib/msim/axi_data_fifo_v2_1_25
vmap axi_crossbar_v2_1_27 modelsim_lib/msim/axi_crossbar_v2_1_27
vmap proc_sys_reset_v5_0_13 modelsim_lib/msim/proc_sys_reset_v5_0_13
vmap xlconcat_v2_1_4 modelsim_lib/msim/xlconcat_v2_1_4
vmap xlconstant_v1_1_7 modelsim_lib/msim/xlconstant_v1_1_7
vmap xlslice_v1_0_2 modelsim_lib/msim/xlslice_v1_0_2
vmap xbip_utils_v3_0_10 modelsim_lib/msim/xbip_utils_v3_0_10
vmap c_reg_fd_v12_0_6 modelsim_lib/msim/c_reg_fd_v12_0_6
vmap xbip_dsp48_wrapper_v3_0_4 modelsim_lib/msim/xbip_dsp48_wrapper_v3_0_4
vmap xbip_pipe_v3_0_6 modelsim_lib/msim/xbip_pipe_v3_0_6
vmap xbip_dsp48_addsub_v3_0_6 modelsim_lib/msim/xbip_dsp48_addsub_v3_0_6
vmap xbip_addsub_v3_0_6 modelsim_lib/msim/xbip_addsub_v3_0_6
vmap c_addsub_v12_0_14 modelsim_lib/msim/c_addsub_v12_0_14
vmap c_gate_bit_v12_0_6 modelsim_lib/msim/c_gate_bit_v12_0_6
vmap xbip_counter_v3_0_6 modelsim_lib/msim/xbip_counter_v3_0_6
vmap c_counter_binary_v12_0_15 modelsim_lib/msim/c_counter_binary_v12_0_15
vmap blk_mem_gen_v8_4_5 modelsim_lib/msim/blk_mem_gen_v8_4_5
vmap axi_protocol_converter_v2_1_26 modelsim_lib/msim/axi_protocol_converter_v2_1_26
vmap axi_clock_converter_v2_1_25 modelsim_lib/msim/axi_clock_converter_v2_1_25
vmap axi_dwidth_converter_v2_1_26 modelsim_lib/msim/axi_dwidth_converter_v2_1_26

vlog -work xilinx_vip  -incr -mfcu -sv -L axi_vip_v1_1_12 -L processing_system7_vip_v1_0_14 -L xilinx_vip "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"E:/Xilinx/Vivado/2022.1/data/xilinx_vip/hdl/axi4stream_vip_axi4streampc.sv" \
"E:/Xilinx/Vivado/2022.1/data/xilinx_vip/hdl/axi_vip_axi4pc.sv" \
"E:/Xilinx/Vivado/2022.1/data/xilinx_vip/hdl/xil_common_vip_pkg.sv" \
"E:/Xilinx/Vivado/2022.1/data/xilinx_vip/hdl/axi4stream_vip_pkg.sv" \
"E:/Xilinx/Vivado/2022.1/data/xilinx_vip/hdl/axi_vip_pkg.sv" \
"E:/Xilinx/Vivado/2022.1/data/xilinx_vip/hdl/axi4stream_vip_if.sv" \
"E:/Xilinx/Vivado/2022.1/data/xilinx_vip/hdl/axi_vip_if.sv" \
"E:/Xilinx/Vivado/2022.1/data/xilinx_vip/hdl/clk_vip_if.sv" \
"E:/Xilinx/Vivado/2022.1/data/xilinx_vip/hdl/rst_vip_if.sv" \

vlog -work xpm  -incr -mfcu -sv -L axi_vip_v1_1_12 -L processing_system7_vip_v1_0_14 -L xilinx_vip "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"E:/Xilinx/Vivado/2022.1/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
"E:/Xilinx/Vivado/2022.1/data/ip/xpm/xpm_fifo/hdl/xpm_fifo.sv" \
"E:/Xilinx/Vivado/2022.1/data/ip/xpm/xpm_memory/hdl/xpm_memory.sv" \

vcom -work xpm  -93 \
"E:/Xilinx/Vivado/2022.1/data/ip/xpm/xpm_VCOMP.vhd" \

vcom -work axi_lite_ipif_v3_0_4  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/66ea/hdl/axi_lite_ipif_v3_0_vh_rfs.vhd" \

vcom -work lib_cdc_v1_0_2  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ef1e/hdl/lib_cdc_v1_0_rfs.vhd" \

vcom -work interrupt_control_v3_1_4  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/a040/hdl/interrupt_control_v3_1_vh_rfs.vhd" \

vcom -work axi_gpio_v2_0_28  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/3ed9/hdl/axi_gpio_v2_0_vh_rfs.vhd" \

vcom -work xil_defaultlib  -93 \
"../../../bd/design_1/ip/design_1_axi_gpio_0_0/sim/design_1_axi_gpio_0_0.vhd" \

vlog -work axi_infrastructure_v1_1_0  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl/axi_infrastructure_v1_1_vl_rfs.v" \

vlog -work axi_vip_v1_1_12  -incr -mfcu -sv -L axi_vip_v1_1_12 -L processing_system7_vip_v1_0_14 -L xilinx_vip "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/1033/hdl/axi_vip_v1_1_vl_rfs.sv" \

vlog -work processing_system7_vip_v1_0_14  -incr -mfcu -sv -L axi_vip_v1_1_12 -L processing_system7_vip_v1_0_14 -L xilinx_vip "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl/processing_system7_vip_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_processing_system7_0_0/sim/design_1_processing_system7_0_0.v" \

vlog -work generic_baseblocks_v2_1_0  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/b752/hdl/generic_baseblocks_v2_1_vl_rfs.v" \

vlog -work axi_register_slice_v2_1_26  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/0a3f/hdl/axi_register_slice_v2_1_vl_rfs.v" \

vlog -work fifo_generator_v13_2_7  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/83df/simulation/fifo_generator_vlog_beh.v" \

vcom -work fifo_generator_v13_2_7  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/83df/hdl/fifo_generator_v13_2_rfs.vhd" \

vlog -work fifo_generator_v13_2_7  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/83df/hdl/fifo_generator_v13_2_rfs.v" \

vlog -work axi_data_fifo_v2_1_25  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5390/hdl/axi_data_fifo_v2_1_vl_rfs.v" \

vlog -work axi_crossbar_v2_1_27  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/3fa0/hdl/axi_crossbar_v2_1_vl_rfs.v" \

vlog -work xil_defaultlib  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_xbar_0/sim/design_1_xbar_0.v" \

vcom -work proc_sys_reset_v5_0_13  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/8842/hdl/proc_sys_reset_v5_0_vh_rfs.vhd" \

vcom -work xil_defaultlib  -93 \
"../../../bd/design_1/ip/design_1_rst_ps7_0_100M_0/sim/design_1_rst_ps7_0_100M_0.vhd" \

vlog -work xlconcat_v2_1_4  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/4b67/hdl/xlconcat_v2_1_vl_rfs.v" \

vlog -work xil_defaultlib  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_xlconcat_0_0/sim/design_1_xlconcat_0_0.v" \

vlog -work xlconstant_v1_1_7  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/fcfc/hdl/xlconstant_v1_1_vl_rfs.v" \

vlog -work xil_defaultlib  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_xlconstant_0_0/sim/design_1_xlconstant_0_0.v" \

vcom -work xil_defaultlib  -93 \
"../../../bd/design_1/ip/design_1_rst_ps7_0_200M_0/sim/design_1_rst_ps7_0_200M_0.vhd" \

vlog -work xlslice_v1_0_2  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/11d0/hdl/xlslice_v1_0_vl_rfs.v" \

vlog -work xil_defaultlib  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_xlslice_0_0/sim/design_1_xlslice_0_0.v" \
"../../../bd/design_1/ip/design_1_RED_SLICE_0/sim/design_1_RED_SLICE_0.v" \
"../../../bd/design_1/ip/design_1_RED_SLICE_1/sim/design_1_RED_SLICE_1.v" \

vcom -work xbip_utils_v3_0_10  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/364f/hdl/xbip_utils_v3_0_vh_rfs.vhd" \

vcom -work c_reg_fd_v12_0_6  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/edec/hdl/c_reg_fd_v12_0_vh_rfs.vhd" \

vcom -work xbip_dsp48_wrapper_v3_0_4  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/cdbf/hdl/xbip_dsp48_wrapper_v3_0_vh_rfs.vhd" \

vcom -work xbip_pipe_v3_0_6  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/7468/hdl/xbip_pipe_v3_0_vh_rfs.vhd" \

vcom -work xbip_dsp48_addsub_v3_0_6  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/910d/hdl/xbip_dsp48_addsub_v3_0_vh_rfs.vhd" \

vcom -work xbip_addsub_v3_0_6  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/cfdd/hdl/xbip_addsub_v3_0_vh_rfs.vhd" \

vcom -work c_addsub_v12_0_14  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ebb8/hdl/c_addsub_v12_0_vh_rfs.vhd" \

vcom -work c_gate_bit_v12_0_6  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/7161/hdl/c_gate_bit_v12_0_vh_rfs.vhd" \

vcom -work xbip_counter_v3_0_6  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/9ac8/hdl/xbip_counter_v3_0_vh_rfs.vhd" \

vcom -work c_counter_binary_v12_0_15  -93 \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/e1e1/hdl/c_counter_binary_v12_0_vh_rfs.vhd" \

vcom -work xil_defaultlib  -93 \
"../../../bd/design_1/ip/design_1_c_counter_binary_0_0/sim/design_1_c_counter_binary_0_0.vhd" \

vlog -work xil_defaultlib  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_xlslice_0_1/sim/design_1_xlslice_0_1.v" \

vcom -work xil_defaultlib  -93 \
"../../../bd/design_1/ip/design_1_c_counter_binary_1_0/sim/design_1_c_counter_binary_1_0.vhd" \

vlog -work xil_defaultlib  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../bd/design_1/ipshared/2730/zedboard_axi_vga.v" \
"../../../bd/design_1/ipshared/2730/zedboard_axi_vga_top.v" \
"../../../bd/design_1/ip/design_1_Zedboard_AXI_VGA_0_0/sim/design_1_Zedboard_AXI_VGA_0_0.v" \
"../../../bd/design_1/ipshared/1182/Zedboard_InterCPU_IRQ_axi_ctrl.v" \
"../../../bd/design_1/ipshared/1182/ZedBoard_InterCPU_IRQ.v" \
"../../../bd/design_1/ip/design_1_ZedBoard_InterCPU_IRQ_0_0/sim/design_1_ZedBoard_InterCPU_IRQ_0_0.v" \

vlog -work blk_mem_gen_v8_4_5  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ip/design_1_Zedboard_OLED_v1_0_0_0/charLib/simulation/blk_mem_gen_v8_4.v" \

vlog -work xil_defaultlib  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_Zedboard_OLED_v1_0_0_0/charLib/sim/charLib.v" \
"../../../bd/design_1/ipshared/76ce/Delay.v" \
"../../../bd/design_1/ipshared/76ce/SpiCtrl.v" \
"../../../bd/design_1/ipshared/76ce/ZedboardOLED_v1_0_S00_AXI.v" \
"../../../bd/design_1/ipshared/76ce/ZedboardOLED_v1_0.v" \
"../../../bd/design_1/ip/design_1_Zedboard_OLED_v1_0_0_0/sim/design_1_Zedboard_OLED_v1_0_0_0.v" \

vlog -work axi_protocol_converter_v2_1_26  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/90c8/hdl/axi_protocol_converter_v2_1_vl_rfs.v" \

vlog -work xil_defaultlib  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_auto_pc_0/sim/design_1_auto_pc_0.v" \
"../../../bd/design_1/ip/design_1_auto_pc_1/sim/design_1_auto_pc_1.v" \

vlog -work axi_clock_converter_v2_1_25  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/e893/hdl/axi_clock_converter_v2_1_vl_rfs.v" \

vlog -work axi_dwidth_converter_v2_1_26  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/b3c7/hdl/axi_dwidth_converter_v2_1_vl_rfs.v" \

vlog -work xil_defaultlib  -incr -mfcu "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../ZedBoard_VGA.gen/sources_1/bd/design_1/ipshared/5765/hdl" "+incdir+E:/Xilinx/Vivado/2022.1/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_auto_us_0/sim/design_1_auto_us_0.v" \
"../../../bd/design_1/sim/design_1.v" \

vlog -work xil_defaultlib \
"glbl.v"

