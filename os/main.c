#include "gfx.h"

static __sfr __at 0x80 arduino_port;
static __sfr __at 0x81 port_a;

void main(void) {
  unsigned char i;
  unsigned int j;

  port_a = 0;

#if 0
  i = 0;
  while(1) {
    port_a = ++i;
    j=0; do { ++j; } while(j!=0x5000);
  }
#endif

  __asm__("im 1"); // mode 1 interrupts, jump to 0x0038
  __asm__("ei");

#if 0
  unsigned char* base = gfx_screen;
  for(i=0; i<360>>2; ++i, base+=128) {
    for(k=0; k<64; ++k) {
      base[k] = 0x55;
      base[k+64] = 0xaa;
    }
  }

  gfx_draw_ch('R', 40, -1);
  gfx_draw_ch('A', 0, 0);
  gfx_draw_ch('G', -2, 48);
  gfx_draw_ch('G', -1, 40);
  gfx_draw_ch('G', 1, 32);
  gfx_draw_ch('B', 32, 2);
  gfx_draw_ch('C', gfx_screen_width-8, 2);
  gfx_draw_ch('D', gfx_screen_width-4, 20);
  gfx_draw_ch('X', 10, 10);
  gfx_draw_ch('P', 52, gfx_screen_height-20);
  gfx_draw_ch('n', 56, gfx_screen_height-8);
  gfx_draw_ch('Z', 64, gfx_screen_height-3);
#endif

  i=0; j=0;
  gfx_clear_screen();
  while(1) {
    //gfx_draw_ch(i, 0, 0, 0x00);
    //gfx_put_ch(i);
    gfx_put_str(" Clem smells! "); gfx_flip_mask = ~gfx_flip_mask;
    arduino_port = i;
    port_a = i;
    ++i;
    //port_a = arduino_port;
    //port_a = i;
    //if(i == 0) { gfx_flip_mask = ~gfx_flip_mask; }
    //j=0; do { ++j; } while(j!=0x1000);
    //j=0; do { ++j; } while(j!=1);
  }

#if 0
  i = 0x01;
  //i = 0x55;
  while(1) {
    unsigned char n=1<<(i&0x7);
    i<<=1;
    if(i == 0) { i = 0x01; }
    //i = ~i;
    j=0; do { ++j; } while(j!=0x1000);
  }

  for(;;++i) {
    //arduino_port = port_a;
    arduino_port = i;
    //port_a = arduino_port;
    //port_a = i;
    j=0; do { ++j; } while(j!=0x5000);
  }
#endif
}
