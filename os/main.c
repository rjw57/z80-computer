#include <stdlib.h>

#include "gfx.h"

// AVR PROTOCOL
// ------------
//
// The AVR port will return all zeros when it is being read from or written to
// by the AVR. As such we set clear bit 7 when the port contains data from the
// CPU to the AVR and the AVR sets bit 7 when it has data for us.

static __sfr __at 0x80 avr_port;
static __sfr __at 0x81 port_a;

void main(void) {
  uint8_t i;

  port_a = 0;

  gfx_clear_screen();
  i=0; do { gfx_put_ch(i++); } while(i != 0);
  gfx_flip_mask = ~gfx_flip_mask;
  i=0; do { gfx_put_ch(i++); } while(i != 0);

  while(1) {
    gfx_point(
        rand() % gfx_screen_width,
        64 + (rand() % (gfx_screen_height - 64)),
        GFX_TOGGLE
    );
  }
}
