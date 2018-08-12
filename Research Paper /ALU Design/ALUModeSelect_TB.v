
// test bench for ALUModeSelect.v 

`timescale 1ns/1ps

module ALUModeSelect_TB();

	reg clk;
	reg button;
	
	wire [3:0] control;
	
	ALUModeSelect uut
	(
		.button(button),
		.clk(clk),
		.modeSelect(control)
	);
	
	initial begin
		clk = 0;
		button = 1;
	end
	
	always #20 clk = ~clk;
	
	initial begin
		#125
		button = 0;
		#153
		button = 1;
		#163
		button = 0;
		#205
		button = 1;
		#111
		button = 0;
		#289
		button = 1;
		#301
		button = 0;
	end

endmodule 