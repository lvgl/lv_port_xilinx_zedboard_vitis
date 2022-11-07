onbreak {quit -f}
onerror {quit -f}

vsim -voptargs="+acc" -L xpm -L unisims_ver -L unimacro_ver -L secureip -lib xil_defaultlib xil_defaultlib.charLib xil_defaultlib.glbl

set NumericStdNoWarnings 1
set StdArithNoWarnings 1

do {wave.do}

view wave
view structure
view signals

do {charLib.udo}

run -all

quit -force
