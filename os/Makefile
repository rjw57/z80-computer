CC:=sdcc -mz80
LD:=sdldz80
MAKEBIN:=makebin

LDFLAGS:=
CFLAGS:=--std-c11

SOURCES:=$(wildcard *.c)
OBJECTS:=$(patsubst %.c,%.rel,$(SOURCES))

all: sram.hex
.DEFAULT: all
.PHONY: all

sram.ihx sram.map: $(OBJECTS) Makefile
	$(LD) $(LDFLAGS) -m -i "$@" $(filter %.rel,$^)

%.hex: %.bin
	xxd -ps -c 1 -g 1 "$<" "$@"

%.bin: %.ihx
	$(MAKEBIN) -s 32768 "$<" "$@"

%.rel: %.c Makefile
	$(CC) $(CFLAGS) -c $<
