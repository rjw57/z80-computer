module sram(
  d_in,
  d_out,
  a,
  ce_n,
  we_n,
  oe_n,
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
  input ce_n, we_n, oe_n, clk;

  // internal variables
  reg [DAT-1:0] SRAM [DPTH-1:0];
  always @ (posedge clk)
  begin
    if (~ce_n) begin
      if (~we_n && oe_n) begin
        SRAM [a] = d_in;
      end else if (~oe_n && we_n) begin
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
