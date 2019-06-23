AS:=sdasz80
CC:=sdcc -mz80
LD:=sdldz80
MAKEBIN:=makebin

ASFLAGS:=
CFLAGS:=--std-c99
LDFLAGS:=

SOURCES:=$(wildcard *.c) $(wildcard *.s)
OBJECTS:=$(patsubst %.c,%.rel,$(SOURCES)) $(patsubst %.s,%.rel,$(SOURCES))

all: sram.hex
.DEFAULT: all
.PHONY: all

sram.ihx sram.map: $(OBJECTS) Makefile
	$(CC) $(LDFLAGS) -o "$@" $(filter %.asm,$^) $(filter %.rel,$^)

%.hex: %.bin
	xxd -ps -c 1 -g 1 "$<" "$@"

%.bin: %.ihx
	$(MAKEBIN) -s 32768 "$<" "$@"

%.rel: %.s Makefile
	$(AS) $(ASFLAGS) -o "$*" $<
	mv "$*.asm" "$*.rel"

%.rel: %.c Makefile
	$(CC) $(CFLAGS) -c $<
