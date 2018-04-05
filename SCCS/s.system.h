h55343
s 00090/00000/00000
d D 1.1 18/03/29 15:11:20 wrc 1 0
c date and time created 18/03/29 15:11:20 by wrc
e
u
U
f e 0
t
T
I 1
/*
** SCCS ID:	%W%	%G%
**
** File:	system.h
**
** Author:	CSCI-452 class of 20175
**
** Contributor:
**
** Description:	miscellaneous OS support functions
*/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "common.h"
#include <x86arch.h>
#include "bootstrap.h"

#include "pcbs.h"

// copied from ulib.h
void do_exit( void );

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP_ASM__

/*
** Start of C-only definitions
*/

// default contents of EFLAGS register

#define DEFAULT_EFLAGS  (EFLAGS_MB1 | EFLAGS_IF)

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
** _bite(pcb)
**
** turn a process into a zombie, or give its status to
** a waiting parent
*/
void _bite( pcb_t *pcb );

/*
** _prune() - release any "spare" allocated memory
**
** Check with all the modules which may retain allocated memory
** for later re-use, and have them release it all to increase the
** pool of available storage.
**
** Returns the number of bytes released.
*/
uint32_t _prune( void );

/*
** _init - system initialization routine
**
** Called by the startup code immediately before returning into the
** first user process.
*/
void _init( void );

#ifdef INCLUDE_SHELL
/*
** _shell - extremely simple shell for handling console input
**
** Called whenever we want to take input from the console and
** act upon it (e.g., for debugging the kernel)
*/
void _shell( void );
#endif

#endif

#endif
E 1
