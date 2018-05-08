/*
** SCCS ID:	%W%	%G%
**
** File:	display.c
**
** Author:	CSCI-452 class of 20175
**
** Contributor: Jonathan Schenk
**
** Description:	Display driver with drawing functionality
*/

#define	__SP_KERNEL__

#include "common.h"
#include "startup.h"
#include "display.h"
#include "sio.h"
#include "font.h"
#include "kwindow.h"

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** The VBE mode information struct as it is defined on OSdev
*/
typedef struct vbe_mode_info_struct {
  uint16_t attributes;
  uint8_t window_a;
  uint8_t window_b;
  uint16_t granularity;
  uint16_t window_size;
  uint16_t segment_a;
  uint16_t segment_b;
  uint32_t win_func_ptr;
  uint16_t pitch;
  uint16_t width;
  uint16_t height;
  uint8_t w_char;
  uint8_t y_char;
  uint8_t planes;
  uint8_t bpp;
  uint8_t banks;
  uint8_t memory_model;
  uint8_t bank_size;
  uint8_t image_pages;
  uint8_t reserved0;

  uint8_t red_mask;
  uint8_t red_position;
  uint8_t green_mask;
  uint8_t green_position;
  uint8_t blue_mask;
  uint8_t blue_position;
  uint8_t reserved_mask;
  uint8_t reserved_position;
  uint8_t direct_color_attributes;

  uint32_t frame_buffer;
  uint32_t off_screen_mem_off;
  uint16_t off_screen_mem_size;
  uint8_t reserved[206];
} __attribute__ ((packed)) VBE_info_t;

/*
** PRIVATE GLOBAL VARIABLES
*/
uint8_t *fb;     // Actual frame buffer
uint8_t *bb;     // Back buffer
uint8_t bpp;     // Bits per pixel
uint16_t screen_width;  // Screen width
uint16_t screen_height; // Screen height
uint16_t pitch;  // Pixels per line
uint32_t buffer_size;

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/

/*
** Calculates offset into the framebuffer given x,y coords
**
** Arguments: Coordinates we want offset for
*/
uint32_t offset_calc(uint32_t x, uint32_t y) {
  return (y * pitch) + (x * (bpp/8));
}

/*
** Draws a horizontal line more efficiently
*/
void draw_horizontal(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color) {
  uint8_t *bb_offset;
  uint32_t end;
  if(x0 < x1) {
    bb_offset = bb + offset_calc(x0, y0);
    end = x1 - x0;
  }
  else {
    bb_offset = bb + offset_calc(x1, y1);
    end = x0 - x1;
  }
  for(int i = 0; i < end; ++i) {
    bb_offset[i*(bpp/8)] = color & 255;
    bb_offset[(i*(bpp/8))+1] = (color >> 8) & 255;
  }
}

/*
** Draws a vertical more efficiently
*/
void draw_vertical(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color) {
  uint8_t *bb_offset;
  uint32_t end;
  if(y0 < y1) {
    bb_offset = bb + offset_calc(x0, y0);
    end = y1 - y0;
  }
  else {
    bb_offset = bb + offset_calc(x1, y1);
    end = y0 - y1;
  }
  for(int i = 0; i < end; ++i) {
    *bb_offset = (color) & 255;
    *(bb_offset+1) = (color >> 8) & 255;
    bb_offset += pitch;
  }
}

/*
** Draws desktop and windows
*/
void _draw_desktop(void) {
  _draw_rect(0, 0, screen_width, screen_height, 0x2084);
  _paint_all_windows();
}

/*
** Copies backbuffer into the framebuffer
*/
void _buffer_copy(void) {
  _kmemcpy(fb, bb, buffer_size);
}

/*
** PUBLIC FUNCTIONS
*/
void _display_init(void) {
  VBE_info_t *vbe_info;
  vbe_info = ((VBE_info_t *) (RMTEXT_ADDRESS + 512));
  bpp = vbe_info->bpp;
  screen_width = vbe_info->width;
  screen_height = vbe_info->height;
  pitch = vbe_info->pitch;
  buffer_size = (uint32_t)screen_height * (uint32_t)screen_width * (uint32_t)(bpp/8);
  fb = (uint8_t *)vbe_info->frame_buffer;
  bb = (uint8_t *)_kmalloc(buffer_size);
}

void _put_pixel(uint32_t x, uint32_t y, uint16_t color) {
  // Deal with bad args
  if( x >= screen_width || x < 0)
    return;
  if(y >= screen_height || y < 0)
    return;
  uint32_t offset = offset_calc(x, y);
  bb[offset] = (color) & 255;
  bb[offset+1] = (color >> 8) & 255;

}

void _draw_rect(uint32_t top_left_x, uint32_t top_left_y,
              uint32_t w, uint32_t h, uint16_t color) {
  // Deal with bad args
  if( top_left_x > screen_width || top_left_y > screen_height)
    return;
  if(top_left_x < 0) top_left_x = 0;
  if(top_left_y < 0) top_left_y = 0;
  if(top_left_x + w > screen_width) w = screen_width - top_left_x;
  if(top_left_y + h > screen_height) h = screen_height - top_left_y;

  uint8_t *bb_offset = bb + offset_calc(top_left_x, top_left_y);

  // Draw the rectangle one line at a time
  for(int i = 0; i < h; ++i) {
    for(int j = 0; j < w; ++j) {
      bb_offset[j*(bpp/8)] = color & 255;
      bb_offset[(j*(bpp/8))+1] = (color >> 8) & 255;
    }
    bb_offset += pitch;
  }
}

void _draw_line (uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color) {
  int dx = (x1 - x0) < 0 ? -1 * (x1 - x0) : (x1 - x0);
  int dy = (y1 - y0) < 0 ? (y1 - y0) : -1 * (y1 - y0);
  int step_x = x0 < x1 ? 1 : -1;
  int step_y = y0 < y1 ? 1 : -1;
  int e1 = dx + dy, e2;

  // Deal with bad args
  if(x0 < 0) x0 = 0;
  if(x0 > screen_width) x0 = screen_width - 1;
  if(x1 < 0) x1 = 0;
  if(x1 > screen_width) x1 = screen_width - 1;
  if(y0 < 0) y0 = 0;
  if(y0 > screen_height) y0 = screen_height - 1;
  if(y1 < 0) y1 = 0;
  if(y1 > screen_height) y1 = screen_height - 1;

  // If vertical, draw it more efficiently
  if(x0 == x1) {
    draw_vertical(x0, y0, x1, y1, color);
    return;
  }
  // If horizontal, draw it more efficiently
  if(y0 == y1) {
    draw_horizontal(x0, y0, x1, y1, color);
    return;
  }

  // Else use _put_pixel :(
  for (;;) {
    _put_pixel (x0,y0, color);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * e1;
    if (e2 >= dy) {
      e1 += dy;
      x0 += step_x;
    }
    if (e2 <= dx) {
      e1 += dx;
      y0 += step_y;
    }
  }
}

void _draw_char(uint32_t top_left_x,
                uint32_t top_left_y,
                uint32_t ch,
                uint16_t fore,
                uint16_t back) {
  uint32_t packed_fore = (fore << 16) | (fore);
  uint32_t packed_back = (back << 16) | (back);
  uint32_t mask1, mask2, mask3, mask4, mask5, mask6, mask7, mask8;
  uint16_t row_data;

  uint8_t *bb_offset = bb + offset_calc(top_left_x, top_left_y);

  // For each row
  for(int r = 0; r < 16; ++r) {
    row_data = font_bitmaps[ch][r]; // Get bitmap for that characters row r
    mask1 = font_masks[(row_data >> 14) & 3]; // Get masks to draw 2 pix at a time
    mask2 = font_masks[(row_data >> 12) & 3];
    mask3 = font_masks[(row_data >> 10) & 3];
    mask4 = font_masks[(row_data >> 8) & 3];
    mask5 = font_masks[(row_data >> 6) & 3];
    mask6 = font_masks[(row_data >> 4) & 3];
    mask7 = font_masks[(row_data >> 2) & 3];
    mask8 = font_masks[(row_data) & 3];

    // Draw the row, 2 pixels at a time
    *(uint32_t *)bb_offset = (packed_fore & mask1) | (packed_back & ~mask1);
    *(uint32_t *)(&bb_offset[4]) = (packed_fore & mask2) | (packed_back & ~mask2);
    *(uint32_t *)(&bb_offset[8]) = (packed_fore & mask3) | (packed_back & ~mask3);
    *(uint32_t *)(&bb_offset[12]) = (packed_fore & mask4) | (packed_back & ~mask4);
    *(uint32_t *)(&bb_offset[16]) = (packed_fore & mask5) | (packed_back & ~mask5);
    *(uint32_t *)(&bb_offset[20]) = (packed_fore & mask6) | (packed_back & ~mask6);
    *(uint32_t *)(&bb_offset[24]) = (packed_fore & mask7) | (packed_back & ~mask7);
    *(uint32_t *)(&bb_offset[28]) = (packed_fore & mask8) | (packed_back & ~mask8);
    // Next row
    bb_offset += pitch;
  }
}

void _screen_refresh(void) {
  _sio_writec('@');
  _draw_desktop();
  _paint_mouse();
  _buffer_copy();
}
