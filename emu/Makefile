TESTBENCH_SOURCES:= \
	$(wildcard cpu/*.v) \
	hlinectr.v \
	sram.v \
	testbench.v \
	top.v

IVERILOG_FLAGS:=-I cpu/ -g2012

all: testbench.vcd
.DEFAULT: all
.PHONY: all

view: testbench.vcd
	gtkwave testbench.vcd testbench.gtkwave
.PHONY: view

clean:
	rm -f testbench.vcd testbench.vvp
.PHONY: clean

testbench.vvp: $(TESTBENCH_SOURCES) sram.hex
	iverilog $(IVERILOG_FLAGS) -o "$@" $(filter %.v,$^)

%.vcd: %.vvp
	vvp $(VVP_FLAGS) "$<"
