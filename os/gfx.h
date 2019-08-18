#ifndef GFX__H
#define GFX__H

#include <stdint.h>

extern uint8_t* gfx_screen;
extern const int16_t gfx_screen_width;
extern const int16_t gfx_screen_height;
extern uint8_t gfx_flip_mask;
extern void gfx_put_ch(uint8_t c);
extern void gfx_draw_ch(uint8_t c, int16_t x, int16_t y, uint8_t flip_mask);
extern void gfx_clear_screen(void);
extern void gfx_scroll_screen(int16_t rows);

#endif /* GFX__H */
