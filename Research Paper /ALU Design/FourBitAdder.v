// 4 Bit Adder
// Inputs: augend, addend, SubtractEnable, carryIn
// Outputs: carryOut, sum
// Designed on 9/4/2017

module RippleCarryAddSub #(parameter DataLength = 4) (Augend, Addend, SubtractEnable, CarryIn, Sum, CarryOut);
  // Inputs
  input      signed [DataLength-1:0] Augend;
  input      signed [DataLength-1:0] Addend;
  input                              SubtractEnable;
  input                              CarryIn;
  // Outputs
  output reg signed [DataLength-1:0] Sum;
  output                             CarryOut;
  // Wires, regs and integers
  wire [DataLength-1:0] Addend2sComp;
  wire                  CarryInSelect;
  reg  [DataLength-1:0] RippleCarry;
  integer               i;
  
  // Replicate the subtract enable to the length of the data bus and perform a bitwise xor with the Addend
  assign Addend2sComp = Addend^{DataLength{SubtractEnable}};
  // The input carry in can only be high when CarryIn=1 or SubtractEnable=1, but not both
  assign CarryInSelect = CarryIn^SubtractEnable;
 
  // One bit daisy chained full adders in a ripple carry structure to compute the result of the summation/subtraction
  always @ (Augend or Addend2sComp or CarryInSelect or RippleCarry)
  begin
    for (i = 0; i < DataLength; i = i + 1) begin
      if (i == 0)
        {RippleCarry[i], Sum[i]} =  CarryInSelect + Augend[i] + Addend2sComp[i];
      else
        {RippleCarry[i], Sum[i]} =  RippleCarry[i-1] + Augend[i] + Addend2sComp[i];
    end
  end
  
  // The MSB of the RippleCarry vector contains the CarryOut bit
  assign CarryOut = RippleCarry[DataLength-1];
 
endmodule 