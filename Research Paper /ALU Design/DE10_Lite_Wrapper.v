// DE10_Lite_Wrapper: Used to map the ALU to the I/O on the DE10 Lite Board 
// Inputs: switchSetA, switchSetB, CarryInSwitch, Key1, clk_50MHz
// Outputs: modeLEDs, Hex0, Hex1, Hex2, Hex3, Hex4
// Designed on 9/4/2017


module DE10_Lite_Wrapper 
#(
	parameter DataLength = 4
)
(
	// inputs
	input signed [3:0] switchSetA,
	input signed [3:0] switchSetB,
	input carryInSwitch,
	input Key1,
	input clk_50MHz,
	
	// outputs
	output [3:0] modeLEDs,
	output [5:0] otherLEDs,
	output [7:0] Hex0,
	output [7:0] Hex1,
	output [7:0] Hex2,
	output [7:0] Hex3,
	output [7:0] Hex4
);

// wires
wire [3:0] ALUControl;
wire signed [3:0] ALUOutput;
wire carryOut;

// ALUModeSelect.v
//
// Instantiation of the block that selects the ALU's mode
ALUModeSelect ALUModeSelect_Inst
(
	.button(Key1),
	.clk(clk_50MHz),
	.modeSelect(ALUControl)
);

// ALUDesign-top.v
//
// Instantiation of the ALU
ALUDesign
#(
	.DataLength(DataLength)
)
ALUDesign_Inst
(
	.InputA(switchSetA),
	.InputB(switchSetB),
	.Select(ALUControl),
	.CarryIn(carryInSwitch),
	.CarryOut(carryOut),
	.OutputResult(ALUOutput)
);

// SevenSegementBinary.v
//
// displays bit 0 of ALU output to HEX0
SevenSegmentBinary HEX0_Inst
(
	.bitToDisplay(ALUOutput[0]),
	.SevenSegDisplay(Hex0)
);

// SevenSegementBinary.v
//
// displays bit 1 of ALU output to HEX1
SevenSegmentBinary HEX1_Inst
(
	.bitToDisplay(ALUOutput[1]),
	.SevenSegDisplay(Hex1)
);

// SevenSegementBinary.v
//
// displays bit 2 of ALU output to HEX2
SevenSegmentBinary HEX2_Inst
(
	.bitToDisplay(ALUOutput[2]),
	.SevenSegDisplay(Hex2)
);

// SevenSegementBinary.v
//
// displays bit 3 of ALU output to HEX3
SevenSegmentBinary HEX3_Inst
(
	.bitToDisplay(ALUOutput[3]),
	.SevenSegDisplay(Hex3)
);

// SevenSegementBinary.v
//
// displays carry out from ALU output to HEX4
SevenSegmentBinary HEX4_Inst
(
	.bitToDisplay(carryOut),
	.SevenSegDisplay(Hex4)
);

// assignment ALU control to LEDs 0 - 3.
assign modeLEDs[3:0] = ALUControl[3:0];

// assign 0 value to other LEDs to ensure they are off
assign otherLEDs[0] = 0;
assign otherLEDs[1] = 0;
assign otherLEDs[2] = 0;
assign otherLEDs[3] = 0;
assign otherLEDs[4] = 0;
assign otherLEDs[5] = 0;

endmodule
