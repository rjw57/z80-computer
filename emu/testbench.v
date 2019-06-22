`timescale 1ns/1ns

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
reg reset = 1;
initial begin
  #100 reset <= 0;
end

// Top-level module
top top(.reset(reset), .clk(clk));

// Test bench
integer ticks = 0;
initial begin
  $dumpfile("testbench.vcd");
  $dumpvars();

  $display("Clock half period: ", CLK_HALF_PERIOD, "ns");

  #100000;

  $finish;
end

endmodule
