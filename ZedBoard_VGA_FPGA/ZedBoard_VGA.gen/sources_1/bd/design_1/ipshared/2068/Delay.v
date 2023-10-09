`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// 
//  
// 
//									  
// 
// Create Date:    12:12:51 08/04/2014
// Module Name:    Delay 
// Project Name: 	 ZedboardOLED
// Target Devices: Zynq
// Tool versions:  Vivado 14.2 (64-bits)
// Description:    Creates a delay of DELAY_MS ms
//
// Revision: 1.0
// Revision 0.01 - File Created
//
//////////////////////////////////////////////////////////////////////////////////
module Delay(
    CLK,
    RST,
    DELAY_MS,
    DELAY_EN,
    DELAY_FIN
    );

	// ===========================================================================
	// 										Port Declarations
	// ===========================================================================
    input CLK;
    input RST;
    input [11:0] DELAY_MS;
    input DELAY_EN;
    output DELAY_FIN;

	// ===========================================================================
	// 							  Parameters, Regsiters, and Wires
	// ===========================================================================
	wire DELAY_FIN;

	reg [31:0] current_state = "Idle";						// Signal for state machine
	reg [16:0] clk_counter = 17'b00000000000000000;		// Counts up on every rising edge of CLK
	reg [11:0] ms_counter = 12'h000;							// Counts up when clk_counter = 100,000


	// ===========================================================================
	// 										Implementation
	// ===========================================================================
	assign DELAY_FIN = (current_state == "Done" && DELAY_EN == 1'b1) ? 1'b1 : 1'b0;
	
	//  State Machine
	always @(posedge CLK) begin
			// When RST is asserted switch to idle (synchronous)
			if(RST == 1'b1) begin
					current_state <= "Idle";
			end
			else begin
					case(current_state)

							"Idle" : begin
									// Start delay on DELAY_EN
									if(DELAY_EN == 1'b1) begin
											current_state <= "Hold";
									end
							end
							
							"Hold" : begin
									// Stay until DELAY_MS has occured
									if(ms_counter == DELAY_MS) begin
											current_state <= "Done";
									end
							end
							
							"Done" : begin
									// Wait until DELAY_EN is deasserted to go to IDLE
									if(DELAY_EN == 1'b0) begin
											current_state <= "Idle";
									end
							end
							
							default : current_state <= "Idle";
							
					endcase
			end
	end
	//  End State Machine


	// Creates ms_counter that counts at 1KHz
	// CLK_DIV
	always @(posedge CLK) begin
			if(current_state == "Hold") begin
					if(clk_counter == 17'b11000011010100000) begin		// 100,000
							clk_counter <= 17'b00000000000000000;
							ms_counter <= ms_counter + 1'b1;					// increments at 1KHz
					end
					else begin
							clk_counter <= clk_counter + 1'b1;
					end
			end
			else begin																// If not in the hold state reset counters
					clk_counter <= 17'b00000000000000000;
					ms_counter <= 12'h000;
			end
	end

endmodule
