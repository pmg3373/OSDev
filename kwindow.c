/*
** SCCS ID:	%W%	%G%
**
** File:	kwindow.c
**
** Author:	CSCI-452 class of 20175
**
** Contributor: Jonathan Schenk
**
** Description:	Code for creating, deleting, changing order for windows
*/

#define	__SP_KERNEL__

#include "common.h"
#include "kwindow.h"
#include "display.h"
#include "sio.h"

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/
#define CHAR_MAX 1024/16 // Max characters per row
int w_count = 0; // Number of windows

/*
** PUBLIC GLOBAL VARIABLES
*/
window_t *wbuff[WINDOW_MAX]; //Window buffer
mouse_t pointer; // Mouse pointer object

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

void _window_init(void) {
  pointer.x = screen_width/2;
  pointer.y = screen_height/2;
  pointer.color = 0xF00F;
}

void _paint_all_windows(void) {
  if(w_count == 0) return;
  for(int i = w_count - 1; i >= 0; --i) {
    _paint_window(wbuff[i]);
  }
}

void _paint_window(window_t *w) {
  uint32_t wx = w->ul_x; uint32_t wy = w->ul_y;
  _draw_rect(wx, wy, w->width, w->height, w->bg_color); //window
  _draw_rect(wx, wy, w->width, 16, 0xFFFF); //title bar
  _draw_line(wx, wy, wx, wy + w->height - 1, 0xFFFF); // border
  _draw_line(wx + w->width, wy, wx + w->width, wy + w->height - 1, 0xFFFF); // border
  _draw_line(wx, wy + w->height - 1, wx + w->width, wy + w->height - 1, 0xFFFF); // border
  char *title = w->title;
  uint32_t ofst = 0;
  // write title
  while(*title != '\0') {
    _draw_char(wx+ofst, wy, *title, 0x0000, 0xFFFF);
    title += 1;
    ofst +=16;
  }
  char *buff = w->buff;
  //uint32_t cx = w->cursor_x; uint32_t cy = w->cursor_y;
  uint32_t cx = wx + 2; uint32_t cy = wy + 16;
  // write buffer
  while(*buff != '\0') {
    _draw_char(cx, cy, *buff, w->fg_color, w->bg_color);
    buff++;
    if(cx + 32 > wx + w->width) {
      cx = wx + 2;
      cy += 16;
    }
    else {
      cx += 16;
    }
  }
  w->cursor_x = cx; w->cursor_y = cy;
}

int _kill_window(uint8_t window_id) {
  int index = -1;
  // find window to kill
  for(int i = 0; i < w_count; ++i) {
    if(wbuff[i]->id == window_id) {
      index = i;
      break;
    }
  }
  if(index == -1) return index;
  window_t *temp = wbuff[index];
  _kfree(temp);
  // fill in its spot
  for(int i = index; i < w_count - 1; ++i) {
    wbuff[i] = wbuff[i+1];
  }
  --w_count;
  return 0;
}

int _add_window(uint32_t x,
                uint32_t y,
                uint32_t h,
                uint32_t w,
                uint16_t fgc,
                uint16_t bgc,
                char *buff,
                char *title) {
  if(w_count == 64) return -1;
  window_t *w_new = (window_t*)_kmalloc(sizeof(window_t));
  w_new->id = w_count;
  w_new->title = title;
  w_new->ul_x = x;
  w_new->ul_y = y;
  w_new->height = h;
  w_new->width = w;
  w_new->cursor_x = 0;
  w_new->cursor_y = 16;
  w_new->is_selected = 1;
  w_new->z = 0;
  w_new->bg_color = bgc;
  w_new->fg_color = fgc;
  w_new->buff = buff;
  wbuff[0]->is_selected = 0;
  // push everything back
  for(int i = w_count; i > 0 ; --i) {
    wbuff[i] = wbuff[i-1];
  }
  // put new window at 0
  wbuff[0] = w_new;
  ++w_count;
  return w_new->id;
}

void _pull_forward(uint8_t window_id) {
  int index = -1;
  // find window to pull forward
  for(int i = 0; i < w_count; ++i) {
    if(wbuff[i]->id == window_id) index = i;
  }
  if(index == -1) return;
  window_t *temp = wbuff[index];
  temp->is_selected = 1;
  wbuff[0]->is_selected = 0;
  // fill its spot
  for(int i = index; i > 0; --i) {
    wbuff[i] = wbuff[i-1];
  }
  // put it at 0
  wbuff[0] = temp;
}

uint8_t _click_event() {
  uint32_t mx = pointer.x;
  uint32_t my = pointer.y;
  uint32_t wx; uint32_t wy;
  uint32_t wh; uint32_t ww;
  // go through windows and get id of window we are colliding with
  for(int i = 0; i < w_count; ++i) {
    wx = wbuff[i]->ul_x;
    wy = wbuff[i]->ul_y;
    wh = wbuff[i]->height;
    ww = wbuff[i]->width;
    if(mx > wx && mx < wx + ww && my > wy && wy < wy + wh)
      return wbuff[i]->id;
  }
  return WINDOW_MAX+1;
}

void _update_mouse(uint32_t x, uint32_t y) {
  pointer.x = x;
  pointer.y = y;
}

void _paint_mouse(void) {
  _draw_rect(pointer.x, pointer.y, 5, 5, pointer.color);
}
