#include <stdlib.h>

#include "gfx.h"

// AVR PROTOCOL
// ------------
//
// The AVR port will return all zeros when it is being read from or written to
// by the AVR. As such we set clear bit 7 when the port contains data from the
// CPU to the AVR and the AVR sets bit 7 when it has data for us.

static __sfr __at 0xa0 avr_port;

void main(void) {
  uint8_t i;

  gfx_clear_screen();
  i=32; do { gfx_put_ch(i++); } while(i != 128);
  gfx_flip_mask = ~gfx_flip_mask;
  i=32; do { gfx_put_ch(i++); } while(i != 128);

  while(1) {
    gfx_point(
        rand() % gfx_screen_width,
        24 + (rand() % (gfx_screen_height - 24)),
        GFX_TOGGLE
    );
  }
}
