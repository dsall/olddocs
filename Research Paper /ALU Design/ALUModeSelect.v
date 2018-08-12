// ALUModeSelect: Cycles through the ALU control signals based on the button pushes.
// Inputs: button, clk
// Outputs: modeSelect
// Designed on 9/4/2017

module ALUModeSelect
(
	// inputs
	input button,
	input clk,
	
	//outputs
	output reg [3:0] modeSelect
);

// used to count button presses
reg [3:0] counter = 0;

//used to sync button press to clock
reg button_f;
reg button_sync;
reg button_sync_f;
wire button_edge;

// align button push with clock
always @ (posedge clk) begin
	button_f <= button;
	button_sync <= button_f;
end

// delay button push by one clock cycle
always @ (posedge clk) begin
	button_sync_f <= button_sync;
end

// increment counter based on a button push. restart counter at 10
always @ (posedge clk) begin
	if (button_edge == 1'b1) begin
		counter = counter + 4'b0001;
		if (counter == 4'b1010) begin
			counter = 4'b0000;
		end
	end
end

// output ALU mode based on counter number
always @ (posedge clk) begin
	case (counter)
		4'b0000:
			modeSelect = 4'b0100;
		4'b0001:
			modeSelect = 4'b0101;
		4'b0010:
			modeSelect = 4'b1000;
		4'b0011:
			modeSelect = 4'b1001;
		4'b0100:
			modeSelect = 4'b1010;
		4'b0101:
			modeSelect = 4'b1011;
		4'b0110: 
			modeSelect = 4'b1100;
		4'b0111:
			modeSelect = 4'b1101;
		4'b1000:
			modeSelect = 4'b1110;
		4'b1001:
			modeSelect = 4'b1111;
		default:
			modeSelect = 4'b1111;
	endcase
end

// creates signal that only goes high on the button push edge
assign button_edge = button_sync & ~button_sync_f;

endmodule 