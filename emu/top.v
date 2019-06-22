module top(
  reset,        // reset
  clk,          // system clock
);

// inputs
input reset;
input clk;

// cpu clock
reg cpu_clk;
always @(posedge clk) cpu_clk <= reset ? 0 : ~cpu_clk;

// cpu reset counter
reg [3:0] cpu_reset_ctr;
wire cpu_reset = ~cpu_reset_ctr[3];
always @(posedge clk) begin
  if(reset)
    cpu_reset_ctr <= 0;
  else
    if(cpu_reset) cpu_reset_ctr <= cpu_reset_ctr + 1;
end

// cpu
wire cpu_read_n;
wire cpu_write_n;
wire cpu_refresh_n;
wire cpu_halt_n;

wire [7:0] cpu_d_in = sram_d_out;
wire [7:0] cpu_d_out = ~cpu_write_n ? cpu_data : 8'hZZ;
wire [7:0] cpu_data = ~cpu_read_n ? cpu_d_in : 8'hZZ;
wire [15:0] cpu_addr;
z80_top_direct_n cpu(
  .nRESET(~cpu_reset), .CLK(cpu_clk),
  .A(cpu_addr), .D(cpu_data),
  .nBUSRQ(1'b1), .nWAIT(1'b1), .nINT(1'b1), .nNMI(1'b1),
  .nRFSH(cpu_refresh_n), .nRD(cpu_read_n), .nWR(cpu_write_n),
  .nHALT(cpu_halt_n)
);

// RAM
wire sram_cs = ~cpu_addr[15];
wire [7:0] sram_d_out;
sram ram(
  .clk(clk), .a(cpu_addr[14:0]), .d_in(cpu_data), .d_out(sram_d_out),
  .CS(sram_cs), .OE(~cpu_read_n), .WE(~cpu_write_n)
);

endmodule
