/*
** SCCS ID:	%W%	%G%
**
** File:	display.h
**
** Author:	CSCI-452 class of 20175
**
** Contributor: Jonathan Schenk
**
** Description: Display Driver for 1024x768 16 bit color graphics
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

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
** Globals
*/

#define PUT_PIX_SUCCESS 0
#define PUT_PIX_X_OOB 1
#define PUT_PIX_Y_OOB 2
#define DRAW_RECT_SUCCESS 3
#define DRAW_RECT_X_OOB 4
#define DRAW_RECT_Y_OOB 5
#define DRAW_LINE_SUCCESS 6
#define DRAW_LINE_X_OOB 7
#define DRAW_LINE_Y_OOB 8

extern uint16_t screen_width;  // Screen width
extern uint16_t screen_height; // Screen height

/*
** Prototypes
*/

/*
** Initializes display (extracts useful info from VESA mode info
** and stores globally)
*/
void _display_init(void);

/*
** Places a pixel of the given color at the given coordinate
**
** Arguments: pixel location, color
*/
void _put_pixel(uint32_t x, uint32_t y, uint16_t color);

/*
** Draws a rectangle
**
** Arguments: Top left corner coordinates, height, width, color
**
*/
void _draw_rect(uint32_t top_left_x, uint32_t top_left_y,
              uint32_t w, uint32_t h, uint16_t color);

/*
** Draws a line between two points.
**
** Arguments: Coordinates of first point, coordinates of second point, color
*/
void _draw_line (uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);

/*
** Draws a character on the screen
**
** Arguments: top left coordinates of character, the character to draw,
**            foreground color, background color
*/
void _draw_char(uint32_t top_left_x,
                    uint32_t top_left_y,
                    uint32_t ch,
                    uint16_t fore,
                    uint16_t back);

/*
** This function redraws the screen
*/
void _screen_refresh(void);
#endif

#endif
