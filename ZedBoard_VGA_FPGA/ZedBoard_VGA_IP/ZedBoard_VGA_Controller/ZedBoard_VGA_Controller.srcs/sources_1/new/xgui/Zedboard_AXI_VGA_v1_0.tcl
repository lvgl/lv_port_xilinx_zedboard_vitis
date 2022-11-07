# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "ADDR_READY" -parent ${Page_0}
  ipgui::add_param $IPINST -name "FIFO_WAIT" -parent ${Page_0}
  ipgui::add_param $IPINST -name "IDLE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "READ" -parent ${Page_0}
  ipgui::add_param $IPINST -name "READ_REQ" -parent ${Page_0}


}

proc update_PARAM_VALUE.ADDR_READY { PARAM_VALUE.ADDR_READY } {
	# Procedure called to update ADDR_READY when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.ADDR_READY { PARAM_VALUE.ADDR_READY } {
	# Procedure called to validate ADDR_READY
	return true
}

proc update_PARAM_VALUE.FIFO_WAIT { PARAM_VALUE.FIFO_WAIT } {
	# Procedure called to update FIFO_WAIT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.FIFO_WAIT { PARAM_VALUE.FIFO_WAIT } {
	# Procedure called to validate FIFO_WAIT
	return true
}

proc update_PARAM_VALUE.IDLE { PARAM_VALUE.IDLE } {
	# Procedure called to update IDLE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.IDLE { PARAM_VALUE.IDLE } {
	# Procedure called to validate IDLE
	return true
}

proc update_PARAM_VALUE.READ { PARAM_VALUE.READ } {
	# Procedure called to update READ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.READ { PARAM_VALUE.READ } {
	# Procedure called to validate READ
	return true
}

proc update_PARAM_VALUE.READ_REQ { PARAM_VALUE.READ_REQ } {
	# Procedure called to update READ_REQ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.READ_REQ { PARAM_VALUE.READ_REQ } {
	# Procedure called to validate READ_REQ
	return true
}


proc update_MODELPARAM_VALUE.IDLE { MODELPARAM_VALUE.IDLE PARAM_VALUE.IDLE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.IDLE}] ${MODELPARAM_VALUE.IDLE}
}

proc update_MODELPARAM_VALUE.ADDR_READY { MODELPARAM_VALUE.ADDR_READY PARAM_VALUE.ADDR_READY } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.ADDR_READY}] ${MODELPARAM_VALUE.ADDR_READY}
}

proc update_MODELPARAM_VALUE.READ { MODELPARAM_VALUE.READ PARAM_VALUE.READ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.READ}] ${MODELPARAM_VALUE.READ}
}

proc update_MODELPARAM_VALUE.READ_REQ { MODELPARAM_VALUE.READ_REQ PARAM_VALUE.READ_REQ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.READ_REQ}] ${MODELPARAM_VALUE.READ_REQ}
}

proc update_MODELPARAM_VALUE.FIFO_WAIT { MODELPARAM_VALUE.FIFO_WAIT PARAM_VALUE.FIFO_WAIT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.FIFO_WAIT}] ${MODELPARAM_VALUE.FIFO_WAIT}
}

