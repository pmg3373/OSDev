/*
** SCCS ID:	%W%	%G%
**
** File:	kwindow.h
**
** Author:	CSCI-452 class of 20175
**
** Contributor: Jonathan Schenk
**
** Description:	Windowing system
*/

#ifndef _KWINDOW_H_
#define _KWINDOW_H_

#include "common.h"

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP_ASM__

/*
** Start of C-only definitions
*/

/*
** Types
*/

/*
** Window structure
*/
typedef struct window_struct {
  uint8_t id;            // Id for this window
  char *title;           // Title written in top left corner
  uint32_t ul_x;         // upper left x
  uint32_t ul_y;         // upper left y
  uint32_t height;       // height
  uint32_t width;        // width
  uint32_t cursor_x;     // window cursor x (unused)
  uint32_t cursor_y;     // window cursor y (unused)
  uint8_t is_selected;   // flag for if this window is selected
  uint32_t z;            // z of this window (unused)
  uint16_t bg_color;     // background color
  uint16_t fg_color;     // foreground color
  char *buff;            // text buffer
} window_t;

/*
** Mouse cursor structure
*/
typedef struct mouse_struct {
  uint32_t x;         // Mouse x
  uint32_t y;         // Mouse y
  uint16_t color;     // mouse color
} mouse_t;

/*
** Globals
*/
#define WINDOW_MAX 64  // max number of windows
//extern window_t *wbuff[WINDOW_MAX]; // Buffer of windows

/*
** Prototypes
*/

/*
** Initializes the windowing system
*/
void _window_init(void);

/*
** Draws all the windows to the screen
** Draws them last to first, so that window 0 is furthest forward
*/
void _paint_all_windows(void);

/*
** Draws a single window
**
** Argument: pointer to the window you want to draw
*/
void _paint_window(window_t *w);

/*
** Adds a new window. Sticks it in the front of the window buffer
** pushes every other window 1 back.
**
** Arguments: top left x,y height, width, background, foreground colors
**            text and title buffers
** Returns: id of new window on success
**          -1 on failure
*/
int _add_window(uint32_t x,
                uint32_t y,
                uint32_t h,
                uint32_t w,
                uint16_t fgc,
                uint16_t bgc,
                char *buff,
                char *title);

/*
** Kills window with given id
**
** Argument: id of window to kill
*/
int _kill_window(uint8_t window_id);

/*
** Pulls window with given id to spot 0 in window buffer
**
** Argument: id of window being pulled forward
*/
void _pull_forward(uint8_t window_id);

/*
** Returns the id of the window that the mouse cursor is on
*/
uint8_t _click_event(void);

/*
** Updates mouse position
**
** Arguments: new x,y of mouse
*/
void _update_mouse(uint32_t x, uint32_t y);

/*
** Draws mouse cursor
*/
void _paint_mouse(void);

#endif

#endif
