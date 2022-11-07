onbreak {quit -f}
onerror {quit -f}

vsim -lib xil_defaultlib charLib_opt

set NumericStdNoWarnings 1
set StdArithNoWarnings 1

do {wave.do}

view wave
view structure
view signals

do {charLib.udo}

run -all

quit -force
