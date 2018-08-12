// Shifter
// Inputs: a, b
// Outputs: carry, sum
// Designed on 9/4/2017

module Shifter
#(
	parameter DataLength = 4
)
(
	// inputs
	input carryIn,
	input [1:0] control,
	input signed [(DataLength - 1):0] numToShift,
	input signed [(DataLength - 1):0] shiftAmount,
	
	// output
	output reg signed [(DataLength - 1):0] result,
	output reg carryOut
);

// constants (states) for output mux.
localparam LeftShift = 2'b00;
localparam RightShift = 2'b01;
localparam ArithRightShift = 2'b10;
localparam ArithLeftShift = 2'b11;

// asyncronous mux to determine which shift operation to perform
always @ (numToShift or shiftAmount or control or carryIn) begin
	casex (control)
		RightShift:
			{carryOut, result} = {carryIn, numToShift} << $unsigned(shiftAmount);
		LeftShift:
			{carryOut, result} = {carryIn, numToShift} >> $unsigned(shiftAmount);
		ArithRightShift:
			{carryOut, result} = {carryIn, numToShift} >>> shiftAmount;
		ArithLeftShift:
			{carryOut, result} = {carryIn, numToShift} <<< shiftAmount;
		default:
			{carryOut, result} = {carryIn, numToShift} >> $unsigned(shiftAmount);
	endcase
end

endmodule 