# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct E:\ZeboardTemplate\Zed_WS\Zedboard_Platform\platform.tcl
# 
# OR launch xsct and run below command.
# source E:\ZeboardTemplate\Zed_WS\Zedboard_Platform\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {Zedboard_Platform}\
-hw {E:\ZeboardTemplate\Zed_Git\ZedBoard_VGA_FPGA\design_1_wrapper.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -out {E:/ZeboardTemplate/Zed_WS}

platform write
platform generate -domains 
platform active {Zedboard_Platform}
domain create -name {standalone_on_ps7_cortexa9_1} -os {standalone} -proc {ps7_cortexa9_1} -arch {32-bit} -display-name {standalone on ps7_cortexa9_1} -desc {} -runtime {cpp}
platform generate -domains 
platform write
domain -report -json
domain active {zynq_fsbl}
bsp reload
bsp config sleep_timer "ps7_globaltimer_0"
bsp write
bsp reload
catch {bsp regenerate}
domain active {standalone_domain}
bsp reload
bsp config sleep_timer "ps7_globaltimer_0"
bsp write
bsp reload
catch {bsp regenerate}
domain active {standalone_on_ps7_cortexa9_1}
bsp reload
bsp config sleep_timer "ps7_globaltimer_0"
bsp write
bsp reload
catch {bsp regenerate}
bsp config extra_compiler_flags "-mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -nostartfiles -g -Wall -Wextra -fno-tree-loop-distribute-patterns -DUSE_AMP=1"
bsp write
bsp reload
catch {bsp regenerate}
platform generate
