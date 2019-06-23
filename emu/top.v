module top(
  reset,        // reset
  clk,          // system clock
);

// inputs
input reset;
input clk;

// cpu reset counter
reg [3:0] cpu_reset_ctr;
wire cpu_reset = ~cpu_reset_ctr[3];
always @(posedge clk) begin
  if(reset)
    cpu_reset_ctr <= 0;
  else
    if(cpu_reset) cpu_reset_ctr <= cpu_reset_ctr + 1;
end

// horizontal counter
wire [4:0] h_low;
wire cpu_clk = h_low[0];
wire h_end;
hlinectr hlinectr(
  .reset(reset), .clk(clk), .low_bits(h_low), .term_cnt(h_end)
);

// horizontal interrupr
reg h_int_n;
always @(posedge clk) begin
  if(reset) begin
    h_int_n <= 1'b1;
  end else begin
    if(h_end) begin
      h_int_n <= 1'b0;
    end else if(~h_int_n && cpu_int_ack) begin
      h_int_n <= 1'b1;
    end else begin
      h_int_n <= h_int_n;
    end
  end
end

// cpu
wire cpu_read_n;
wire cpu_write_n;
wire cpu_refresh_n;
wire cpu_halt_n;
wire cpu_m1_n;
wire cpu_iorq_n;
wire cpu_int_n = h_int_n;
wire cpu_int_ack = ~cpu_m1_n && ~cpu_iorq_n;
wire [7:0] cpu_d_in = sram_d_out;
wire [7:0] cpu_d_out = ~cpu_write_n ? cpu_data : 8'hZZ;
wire [7:0] cpu_data = ~cpu_read_n ? cpu_d_in : 8'hZZ;
wire [15:0] cpu_addr;
z80_top_direct_n cpu(
  .nRESET(~cpu_reset), .CLK(cpu_clk),
  .A(cpu_addr), .D(cpu_data),
  .nBUSRQ(1'b1), .nWAIT(1'b1), .nNMI(1'b1), .nINT(cpu_int_n),
  .nRFSH(cpu_refresh_n), .nRD(cpu_read_n), .nWR(cpu_write_n),
  .nHALT(cpu_halt_n), .nM1(cpu_m1_n), .nIORQ(cpu_iorq_n)
);

// RAM
wire [7:0] sram_d_out;
sram ram(
  .clk(clk), .a(cpu_addr[14:0]), .d_in(cpu_data), .d_out(sram_d_out),
  .ce_n(1'b0), .oe_n(cpu_read_n), .we_n(cpu_write_n)
);

endmodule
