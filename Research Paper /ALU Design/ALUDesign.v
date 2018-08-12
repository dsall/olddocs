// ALU
// Inputs: InputA, InputB, carryIn, Select
// Outputs: OutputResultesult, carryOut
// Designed on 9/4/2017

	module ALUDesign #(parameter DataLength = 4) (InputA, InputB, Select, CarryIn, CarryOut, OutputResult);
  // Inputs
  input      signed [DataLength-1:0] InputA;
  input      signed [DataLength-1:0] InputB;
  input             [3:0]            Select;
  input                              CarryIn;
  // Outputs
  output reg                         CarryOut;
  output reg signed [DataLength-1:0] OutputResult;
  // Constants (states) for output mux
  localparam s_Add                  = 4'b0100;
  localparam s_Subtract             = 4'b0101;
  localparam s_InvertA              = 4'b1000;
  localparam s_BitwiseOr            = 4'b1001;
  localparam s_BitwiseAnd           = 4'b1010;
  localparam s_BitwiseXor           = 4'b1011;
  localparam s_ShiftLeft            = 4'b1100;
  localparam s_ShiftRight           = 4'b1101;
  localparam s_ShiftRightArithmetic = 4'b1110;
  localparam s_PassThroughA         = 4'b1111;
  // Wires
  wire signed [DataLength-1:0] AddSubResult;
  wire signed [DataLength-1:0] ShifterResult;
  wire                         SubtractEnable;
  wire		  [1:0]            ShifterControl;
  wire								 CarryAddSub;
  wire								 CarryShifter;
  
  // Subtraction will occur when the LSB of the select line is asserted
  assign SubtractEnable = Select[0];
  
  // Shifter control will be determined by the 2 LSBs of the select line.
  assign ShifterControl = Select[1:0];
  
  // FourBitAdder.v
  //
  // Ripple Carry Adder instantiation. High latency for large data lengths due to the daisy chained design.
  RippleCarryAddSub #(
    .DataLength(DataLength)
  ) 
  AddSubInst (
    .Augend(InputA),
    .Addend(InputB),
    .SubtractEnable(SubtractEnable),
    .CarryIn(CarryIn),
    .Sum(AddSubResult),
    .CarryOut(CarryAddSub)
  );
  
  // Shifter.v
  //
  // Left/Right shifter instantiation.
  Shifter #(
	 .DataLength(DataLength)
  )
  ShifterInst (
	 .carryIn(CarryIn),
    .control(ShifterControl),
	 .numToShift(InputA),
	 .shiftAmount(InputB),
	 .result(ShifterResult),
	 .carryOut(CarryShifter)
  );
  
  // Asyncronous mux. No need for "if, elsif, else" construct since priority is not an issue.
  always @ (InputA or InputB or Select or AddSubResult or ShifterResult or CarryIn or CarryAddSub or CarryShifter)
  begin
    casex (Select)
      s_Add, s_Subtract : begin
        /*
          We will output the result of the ripple carry adder when the first 3 bits of the select line are 010. 
          The ripple carry adder determins to perform addition or subtraction
        */
        OutputResult = AddSubResult;
		  CarryOut = CarryAddSub;
		end
      s_InvertA : begin       
        OutputResult = ~InputA;
		  CarryOut = CarryIn;
		end
      s_BitwiseOr : begin
        OutputResult = InputA | InputB;
		  CarryOut = CarryIn;
		end
      s_BitwiseAnd : begin
        OutputResult = InputA & InputB;
		  CarryOut = CarryIn;
		end
      s_BitwiseXor : begin
        OutputResult = InputA ^ InputB;
		  CarryOut = CarryIn;
		end
      s_ShiftLeft : begin
        OutputResult = ShifterResult;
		  CarryOut = CarryShifter;
		end
      s_ShiftRight : begin
        OutputResult = ShifterResult;
		  CarryOut = CarryShifter;
		end
      s_ShiftRightArithmetic : begin
        OutputResult = ShifterResult;
		  CarryOut = CarryShifter;
		end
      s_PassThroughA : begin
        OutputResult = InputA;
		  CarryOut = CarryIn;
		end
      default : begin
        OutputResult = { DataLength {1'b0} };
		  CarryOut = CarryIn;
		end
    endcase
  end
endmodule
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

