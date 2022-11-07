onbreak {quit -force}
onerror {quit -force}

asim +access +r +m+charLib -L xpm -L unisims_ver -L unimacro_ver -L secureip -O5 xil_defaultlib.charLib xil_defaultlib.glbl

set NumericStdNoWarnings 1
set StdArithNoWarnings 1

do {wave.do}

view wave
view structure

do {charLib.udo}

run -all

endsim

quit -force
