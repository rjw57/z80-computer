`timescale 1ns/10ps

module testbench;

// System clock frequency in MHz
parameter CLK_FREQ_MHZ = 25.175;
parameter CLK_HALF_PERIOD = 1000/(2*CLK_FREQ_MHZ);

// Clocks
reg clk = 0;
reg cpu_clk = 0;
always #CLK_HALF_PERIOD clk <= ~clk;
always @(posedge clk) cpu_clk <= ~cpu_clk;

// Reset
reg [3:0] reset_ctr = 0;
wire reset = ~reset_ctr[3];
always @(posedge clk) reset_ctr <= reset ? reset_ctr + 1 : reset_ctr;

// Top-level module
top top(.reset(reset), .clk(clk));

// Test bench
integer ticks = 0;
initial begin
  $dumpfile("testbench.vcd");
  $dumpvars();

  $display("Clock half period: ", CLK_HALF_PERIOD, "ns");

  #70000;

  $finish;
end

endmodule
