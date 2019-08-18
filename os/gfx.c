#include <stdint.h>
#include <string.h>

#include "gfx.h"
#include "font.h"

const int16_t gfx_screen_width = 512;
const int16_t gfx_screen_height = 176;
int16_t gfx_cursor_x = 0;
int16_t gfx_cursor_y = 0;
uint8_t gfx_flip_mask = 0;

// The screen is 64 * 176 = 11264 bytes in length or 0x2C00 in hex. If we allow
// a page for the stack, the highest start address is 0x8000 - 0x2C00 - 0x100 or
// 0x5300.
uint8_t* gfx_screen = (void*)(0x5300);

void gfx_clear_screen(void) {
  memset(gfx_screen, 0, gfx_screen_height<<6);
}

void gfx_scroll_screen(int16_t rows) {
  if(rows > 0) {
    if (rows < gfx_screen_height) {
      memcpy(gfx_screen, gfx_screen + (rows<<6), (gfx_screen_height - rows)<<6);
      memset(gfx_screen + ((gfx_screen_height - rows)<<6), 0, rows<<6);
    } else {
      gfx_clear_screen();
    }
  } // TODO: other cases
}

void gfx_put_ch(uint8_t c) {
  gfx_draw_ch(c, gfx_cursor_x, gfx_cursor_y, gfx_flip_mask);
  gfx_cursor_x += 8;
  if(gfx_cursor_x >= gfx_screen_width) {
    gfx_cursor_x = 0;
    gfx_cursor_y += 8;
    if(gfx_cursor_y >= gfx_screen_height) {
      gfx_scroll_screen(8);
      gfx_cursor_y -= 8;
    }
  }
}

void gfx_put_str(const char* s) {
  uint8_t c;
  for(; (c = *s) != '\0'; ++s) { gfx_put_ch(c); }
}

void gfx_draw_ch(uint8_t c, int16_t x, int16_t y, uint8_t flip_mask) {
  if(x <= -8) { return; }
  if(y <= -8) { return; }
  if(x >= gfx_screen_width) { return; }
  if(y >= gfx_screen_height) { return; }

  uint8_t i = (y <= gfx_screen_height - 8) ? 8 : gfx_screen_height-y;
  const uint8_t* font_data = FONT + (((unsigned int)c)<<3);

  uint8_t* cursor = gfx_screen;
  cursor += (x < 0) ? -1 : (x>>3);
  cursor += (y < 0) ? 0 : (((unsigned int)y)<<6);

  if(y < 0) { i += y; font_data -= y; }

  uint8_t l_shift = x & 0x7;
  if(l_shift) {
    // Un-aligned to char cell boundary
    uint8_t f, v;
    uint8_t r_shift = 8 - l_shift;
    uint8_t mask = 0xff >> l_shift;
    _Bool draw_left = x >= 0;
    _Bool draw_right = x < gfx_screen_width-8;

    for(; i>0; i--, cursor+=64, font_data++) {
      f = (*font_data) ^ flip_mask;
      if(draw_left) {
        v = *cursor;
        v &= ~mask; v |= f >> l_shift;
        *cursor = v;
      }
      if(draw_right) {
        v = *(cursor + 1);
        v &= mask; v |= (f << r_shift) & ~mask;
        *(cursor+1) = v;
      }
    }
  } else {
    // Aligned to char cell boundary
    for(; i>0; i--, cursor+=64, font_data++) {
      *cursor = *font_data ^ flip_mask;
    }
  }
}

void gfx_point(int16_t x, int16_t y, gfx_draw_mode_t mode) {
  if((x < 0) || (x >= gfx_screen_width) || (y < 0) || (y >= gfx_screen_height)) { return; }
  uint8_t* cursor = gfx_screen + (x>>3) + (((unsigned int)y)<<6);
  uint8_t pt_mask = 0x80 >> (x & 0x7);
  switch(mode) {
    case GFX_RESET:
      *cursor &= ~pt_mask;
      break;
    case GFX_SET:
      *cursor |= pt_mask;
      break;
    case GFX_TOGGLE:
      *cursor ^= pt_mask;
      break;
  }
}
