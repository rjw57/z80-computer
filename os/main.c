#include <stdlib.h>

#include "gfx.h"

// AVR PROTOCOL
// ------------
//
// The AVR port will return all zeros when it is being read from or written to
// by the AVR. As such we set clear bit 7 when the port contains data from the
// CPU to the AVR and the AVR sets bit 7 when it has data for us.

static volatile __sfr __at 0x80 avr_port;

void put_hex_digit(uint8_t v) {
  if(v < 10) { gfx_put_ch('0' + v); } else { gfx_put_ch(('A' - 10) + v); }
}

void put_hex(uint8_t v) {
  put_hex_digit(v >> 4);
  put_hex_digit(v & 0x0f);
}

uint8_t avr_cmd(uint8_t cmd) {
  uint8_t response = 0x00;
  avr_port = cmd;
  while((response & 0xc0) != 0x80) { response = avr_port; }
  return response & 0x0f;
}

const uint8_t SCAN_CODE_MAP[] = {
// 0     1     2     3       4     5     6     7       8     9     A     B       C     D     E     F
  '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '`',  '\0',   // 0
  '\0', '\0', '\0', '\0',   '\0', 'q',  '1',  '\0',   '\0', '\0', 'z',  's',    'a',  'w',  '2',  '\0',   // 1
  '\0', 'c',  'x',  'd',    'e',  '4',  '3',  '\0',   '\0', ' ',  'v',  'f',    't',  'r',  '5',  '\0',   // 2
  '\0', 'n',  'b',  'h',    'g',  'y',  '6',  '\0',   '\0', '\0', 'm',  'j',    'u',  '7',  '8',  '\0',   // 3
  '\0', ',',  'k',  'i',    'o',  '0',  '9',  '\0',   '\0', '.',  '/',  'l',    ';',  'p',  '-',  '\0',   // 4
  '\0', '\0', '\'', '\0',   '[',  '=',  '\0', '\0',   '\0', '\0', '\0', ']',    '\0', '#',  '\0', '\0',   // 5
  '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '1',  '\0', '4',    '7',  '\0', '\0', '\0',   // 6
  '0',  '.',  '2',  '5',    '6',  '8',  '\0', '\0',   '\0', '+',  '3',  '-',    '*',  '9',  '\0', '\0',   // 7

  '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   // 8
  '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   // 9
  '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   // A
  '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   // B
  '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   // C
  '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   // D
  '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   // E
  '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   '\0', '\0', '\0', '\0',   // F
};

void main(void) {
  uint8_t i;
  uint8_t kbd_byte = 0;
  _Bool ignore_next_kbd = 0;

  gfx_clear_screen();
  i=32; do { gfx_put_ch(i++); } while(i != 128);
  gfx_flip_mask = ~gfx_flip_mask;
  i=32; do { gfx_put_ch(i++); } while(i != 128);
  gfx_flip_mask = ~gfx_flip_mask;

  while(1) {
    // Do we have keyboard data?
    if(avr_cmd(0x41) & 0x01) {
      kbd_byte = avr_cmd(0x40);
      kbd_byte |= avr_cmd(0x40) << 4;

      if(kbd_byte == 0xf0) {
        // break code, ignore next
        ignore_next_kbd = 1;
      } else if(ignore_next_kbd) {
        // ignoring break code
        ignore_next_kbd = 0;
      } else {
        char c = SCAN_CODE_MAP[kbd_byte];
        if(c != '\0') { gfx_put_ch(c); }
      }
    }

    gfx_point(
        rand() % gfx_screen_width,
        //24 + (rand() % (gfx_screen_height - 24)),
        rand() % 24,
        GFX_TOGGLE
    );
  }
}
