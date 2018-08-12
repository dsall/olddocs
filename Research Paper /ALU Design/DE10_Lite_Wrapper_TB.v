
// test bench for DE10_Lite_Wrapper.v

`timescale 1ns/1ps

module DE10_Lite_Wrapper_TB;

	reg [3:0] switchA;
	reg [3:0] switchB;
	reg carryIn;
	reg key1;
	reg clk;
	
	wire [3:0] LEDs;
	wire [7:0] HEX0;
	wire [7:0] HEX1;
	wire [7:0] HEX2;
	wire [7:0] HEX3;
	wire [7:0] HEX4;
	
	DE10_Lite_Wrapper uut
	(
		.switchSetA(switchA),
		.switchSetB(switchB),
		.carryInSwitch(carryIn),
		.Key1(key1),
		.clk_50MHz(clk),
		.modeLEDs(LEDs),
		.Hex0(HEX0),
		.Hex1(HEX1),
		.Hex2(HEX2),
		.Hex3(HEX3),
		.Hex4(HEX4)
	);
	
	initial begin
		clk = 0;
		switchA = 4'b0000;
		switchB = 4'b0000;
		carryIn = 0;
		key1 = 1;
	end
	
	always #20 clk = ~clk;
	
	initial begin
		#100
		switchA = 4'b0011;
		switchB = 4'b0001;
		#100
		key1 = 0;
		#100
		key1 = 1;
		#100
		key1 = 0;
		#100
		key1 = 1;
		#100
		key1 = 0;
		#100
		key1 = 1;
		#100
		key1 = 0;
		#100
		key1 = 1;
	end

endmodule
		
		
		