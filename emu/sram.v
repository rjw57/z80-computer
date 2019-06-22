module sram(
  d_in,
  d_out,
  a,
  CS,
  WE,
  OE,
  clk
);
  // parameters for the width
  parameter ADR   = 15;
  parameter DAT   = 8;
  parameter DPTH  = 1<<ADR;

  // ports
  input [DAT-1:0]       d_in;
  output reg [DAT-1:0]  d_out;
  input [ADR-1:0]       a;
  input CS, WE, OE, clk;

  // internal variables
  reg [DAT-1:0] SRAM [DPTH-1:0];
  always @ (posedge clk)
  begin
    if (CS == 1'b1) begin
      if (WE == 1'b1 && OE == 1'b0) begin
        SRAM [a] = d_in;
      end else if (OE == 1'b1 && WE == 1'b0) begin
        d_out = SRAM [a];
      end
    end;
  end

  // initial contents
  initial begin
    $display("Loading RAM");
    $readmemh("sram.hex", SRAM, 0, DPTH-1);
  end
endmodule
