// SevenSegmentBinary: Used to display a 0 or 1 based on the input bit
// Inputs: bitToDisplay
// Outputs: SevnSegDisplay
// Designed on 9/6/2017

module SevenSegmentBinary
(
	// inputs
	input bitToDisplay,
	
	//outputs
	output reg [7:0] SevenSegDisplay
);

// constants (states) for mux
localparam Zero = 1'b0;
localparam One = 1'b1;

// mux to choose whether to display 1 or 0 on the sevensegment displays
always @ (bitToDisplay) begin
	casex (bitToDisplay)
		Zero:
			SevenSegDisplay = 8'b11000000;
		One:
			SevenSegDisplay = 8'b11111001;
		default:
			SevenSegDisplay = 8'b11000000;
	endcase
end

endmodule 