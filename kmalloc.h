/*
** SCCS ID:	@(#)kmalloc.h	1.1	3/29/18
**
** File:	kmalloc.h
**
** Author:	Kenneth Reek and the 4003-506 class of 20013
**
** Contributor:	Warren R. Carithers
**
** Description:	Structures and functions to support dynamic memory
**		allocation within the OS.
**
**		The list of free blocks is ordered by address to facilitate
**		combining freed blocks with adjacent blocks that are already
**		free.
**
**		All requests for memory are satisfied with blocks that are
**		an integral number of 4-byte words.  More memory may be
**		provided than was requested if the fragment left over after
**		the allocation would not be large enough to be useable.
*/

#ifndef	_KALLOC_H_
#define	_KALLOC_H_

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
** Prototypes
*/

/*
** Functions that manipulate free memory blocks.
*/

/*
** Name:	_dump_freelist
**
** Description:	Dump the current contents of the freelist to the console
*/
void _dump_freelist( void );

/*
** Name:	_kmalloc
**
** Description:	Dynamically allocates a block of memory from the system's
**		available memory.
** Arguments:	Number of bytes desired
** Returns:	A pointer to a block of memory at least that large (or
**		NULL if no block was found)
*/
void *_kmalloc( uint32_t desired_length );

/*
** Name:	_kfree
**
** Description:	Frees a previously allocated block of dynamic memory.
** Arguments:	A pointer to the block to be freed
*/
void _kfree( void *block );		/* return a block to the free list */

/*
** Name: _km_init
**
** Description:	Builds a free list of blocks from the information
**		provided by the BIOS to the bootstrap.
*/
void _km_init( void );

#endif

#endif
