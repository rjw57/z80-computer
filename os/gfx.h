#ifndef GFX__H
#define GFX__H

#include <stdint.h>

typedef enum {
  GFX_RESET, GFX_SET, GFX_TOGGLE
} gfx_draw_mode_t;

extern uint8_t* gfx_screen;
extern uint8_t* gfx_font_base;

extern const int16_t gfx_screen_width;
extern const int16_t gfx_screen_height;
extern uint8_t gfx_flip_mask;
extern void gfx_put_str(const char* s);
extern void gfx_put_ch(uint8_t c);
extern void gfx_draw_ch(uint8_t c, int16_t x, int16_t y, uint8_t flip_mask);
extern void gfx_clear_screen(void);
extern void gfx_scroll_screen(int16_t rows);
extern void gfx_point(int16_t x, int16_t y, gfx_draw_mode_t mode);

#endif /* GFX__H */
