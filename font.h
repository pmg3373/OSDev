/*
** SCCS ID:	%W%	%G%
**
** File:	font.h
**
** Author:	CSCI-452 class of 20175
**
** Contributor: Jonathan Schenk
**
** Description:	Text support for graphics mode
*/

#ifndef _font_H_
#define _font_H_

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

/*
** Masks for drawing 2 16 bit pixels at one time
*/
extern uint32_t font_masks[4];

/*
** From http://prettyuglycode.net/16X16_font.pdf
**
** ASCII table of row data for drawing characters
** Bitmaps created meticulously by hand based on the grids on the site linked above
*/
extern uint16_t font_bitmaps[][16];

/*
** Prototypes
*/

#endif

#endif
