AS:=sdasz80
CC:=sdcc -mz80
LD:=sdldz80
MAKEBIN:=makebin

ASFLAGS:=-plosgff
CFLAGS:=--std-c99 --Werror
LDFLAGS:=--no-std-crt0 -Wl-b_DATA=0x0 -Wl-b_CODE=0x100

SOURCES:=$(wildcard *.c) $(wildcard *.s)
OBJECTS:=$(patsubst %.c,%.rel,$(SOURCES)) $(patsubst %.s,%.rel,$(SOURCES))

all: sram.hex sram_contents.h
.DEFAULT: all
.PHONY: all

sram.ihx sram.map: $(OBJECTS) Makefile
	$(CC) $(LDFLAGS) -o "$@" $(filter %.asm,$^) $(filter %.rel,$^)

%_contents.h: %.bin
	xxd -i "$<" | sed -e 's/unsigned char/const PROGMEM uint8_t/' >"$@"

%.hex: %.bin
	xxd -ps -c 1 -g 1 "$<" "$@"

%.bin: %.ihx
	$(MAKEBIN) -s 8192 "$<" "$@"

%.rel: %.s Makefile
	$(AS) $(ASFLAGS) -o "$*" $<
	mv "$*.asm" "$*.rel"

%.rel: %.c Makefile
	$(CC) $(CFLAGS) -c $<
