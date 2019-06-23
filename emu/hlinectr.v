module hlinectr(
  reset,
  clk,
  low_bits,
  term_cnt,
);

input         reset;
input         clk;
output [4:0]  low_bits;
output reg    term_cnt;

parameter CTR_DEPTH = 12;
parameter CTR_MAX = (1<<CTR_DEPTH) - 1;
parameter CTR_INIT = CTR_MAX - 799;

reg [11:0] counter;
assign low_bits = counter[4:0];

always @(posedge clk) begin
  if(reset) begin
    counter <= CTR_INIT;
    term_cnt <= 0;
  end else begin
    counter <= term_cnt ? CTR_INIT : counter + 1;
    term_cnt <= counter == (CTR_MAX - 1);
  end
end

endmodule
