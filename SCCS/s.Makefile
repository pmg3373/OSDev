h35091
s 00118/00020/00164
d D 1.18 18/03/29 15:09:43 wrc 18 17
c updates for 20175 baseline system
e
s 00003/00003/00181
d D 1.17 16/03/10 12:58:04 wrc 17 16
c (wrc) updated prog.dis target to use objdump
e
s 00001/00001/00183
d D 1.16 15/03/21 19:02:36 wrc 16 15
c updated to use new dcopy/dfind commands for usb
e
s 00013/00011/00171
d D 1.15 15/03/17 13:21:46 wrc 15 14
c (wrc) mods to force i386 compile/assemble/link even on 64-bit archs
e
s 00001/00001/00181
d D 1.14 12/10/15 11:11:41 wrc 14 13
c added -std=c99
e
s 00007/00012/00175
d D 1.13 11/10/17 15:25:22 wrc 13 12
c Ubuntu update - /local/devices/disk for USB drives
e
s 00001/00000/00186
d D 1.12 11/03/31 13:24:51 wrc 12 11
c changed .S.o to remove .s file afterwards
e
s 00001/00000/00185
d D 1.11 11/01/31 18:20:21 wrc 11 10
c added REPORT_MYSTERY_INTS to control int 0x27 messages
e
s 00005/00002/00180
d D 1.10 10/11/05 18:02:37 wrc 10 9
c updated for new USB jack locations
e
s 00015/00006/00167
d D 1.9 09/10/18 16:29:44 wrc 9 8
c updated for usb flash drive use
e
s 00022/00007/00151
d D 1.8 06/10/14 13:58:03 wrc 8 7
c configuration option changes
e
s 00001/00001/00157
d D 1.7 06/10/14 13:44:45 wrc 7 6
c sp1/sp2 compatability tweaks
e
s 00013/00004/00145
d D 1.6 06/10/14 12:43:30 wrc 6 5
c added Offsets and .S.o xform rules for sp1/sp2 compatability
e
s 00013/00011/00136
d D 1.5 05/03/15 15:46:57 wrc 5 4
c (wrc) minor name changes
e
s 00008/00009/00139
d D 1.4 03/09/30 11:27:01 kar 4 3
c Update for installation of latest OS & utilities
e
s 00028/00028/00120
d D 1.3 02/05/16 15:14:49 kar 3 2
c Version 2: improvements based on my experience with 20013 506.
e
s 00008/00007/00140
d D 1.2 02/01/22 16:50:04 kar 2 1
c New version for interrupt stuff
e
s 00147/00000/00000
d D 1.1 02/01/15 16:18:23 kar 1 0
c date and time created 02/01/15 16:18:23 by kar
e
u
U
f e 0
t
T
I 1
#
I 5
D 18
# SCCS ID: %W%	%G%
E 18
I 18
# SCCS ID: %W%        %G%
E 18
#
E 5
D 7
# Makefile to control the compiling, assembling and linking
E 7
I 7
# Makefile to control the compiling, assembling and linking of
E 7
D 8
# standalone programs in the DSL
E 8
I 8
D 15
# standalone programs in the DSL.  Used for both 4003-406 and
# 4003-506 (with appropriate tweaking).
E 15
I 15
# standalone programs in the DSL.  Used for both individual
# interrupt handling assignments and the SP baseline OS (with
# appropriate tweaking).
E 15
E 8
#
D 5
# SCCS ID: %W%	%G%
#
E 5

#
# User supplied files
#
D 18
U_C_SRC = main.c
U_C_OBJ = main.o
U_S_SRC =
U_S_OBJ =
I 4
U_LIBS	=
E 18
I 18
SYS_C_SRC = clock.c klibc.c kmalloc.c pcbs.c queues.c scheduler.c \
	sio.c stacks.c syscalls.c system.c

SYS_C_OBJ = clock.o klibc.o kmalloc.o pcbs.o queues.o scheduler.o \
	sio.o stacks.o syscalls.o system.o

SYS_S_SRC = klibs.S

SYS_S_OBJ = klibs.o

SYS_SRCS = $(SYS_C_SRC) $(SYS_S_SRC)
SYS_OBJS = $(SYS_C_OBJ) $(SYS_S_OBJ)

USR_C_SRC = ulibc.c users.c

USR_C_OBJ = ulibc.o users.o

USR_S_SRC = ulibs.S

USR_S_OBJ = ulibs.o

USR_SRCS = $(USR_C_SRC) $(USR_S_SRC)
USR_OBJS = $(USR_C_OBJ) $(USR_S_OBJ)
E 18
E 4

#
I 8
# User compilation/assembly definable options
#
D 18
#	ISR_DEBUGGING_CODE	include context restore debugging code
E 18
I 18
# General options:
#	CHILD_FIRST		allow the child to run first after fork()
E 18
#	CLEAR_BSS_SEGMENT	include code to clear all BSS space
I 18
#	DUMP_QUEUES		clock ISR dumps queues etc. every 10 seconds
#	DUMP_QUEUE_CONTENTS	dump full contents (vs. sizes)
E 18
D 15
#	SP2_CONFIG		enable SP2-specific startup variations
E 15
I 15
#	SP_OS_CONFIG		enable SP OS-specific startup variations
E 15
I 11
D 18
#	REPORT_MYSTERY_INTS	print a message on interrupt 0x27
E 18
E 11
#
D 18
USER_OPTIONS = -DCLEAR_BSS_SEGMENT
E 18
I 18
# Debugging options:
#	DEBUG_KMALLOC		debug the kernel allocator code
#	DEBUG_KMALLOC_FREELIST	debug the freelist creation
#	DEBUG_UNEXP_INTS	dump process info in the 'unexpected' ISR
#	INCLUDE_SHELL		include a debugging shell
#	ISR_DEBUGGING_CODE	include context restore debugging code
#	REPORT_MYSTERY_INTS	print a message on interrupt 0x27
#	SANITY_CHECK		include "sanity check" tests
#	TRACE_STACK_SETUP	enable tracing of _stk_setup()
#	TRACE_EXEC		trace exec() pre/post _stk_setup()
#	TRACE_SPAWN=n		trace the activity of spawn()
#
# Values usable for TRACE_SPAWN cause output at these times:
#	1:	entry to spawn(), incoming argv, strtbl pre/post fork(),
#		outgoing argv, _sys_exec pre & post _stk_setup call
#	2 adds:	addr of strtbl, arg string list as length is calculated,
#		_sys_fork after allocs, strtbl contents
#	3 adds:	fork() return value in parent, child priority set
#
MAIN_OPTIONS = -DSP_OS_CONFIG -DCLEAR_BSS_SEGMENT -DDUMP_QUEUES
DBG_OPTIONS = -DSANITY_CHECK -DISR_DEBUGGING_CODE -DDEBUG_UNEXP_INTS
USER_OPTIONS = $(MAIN_OPTIONS) $(DBG_OPTIONS)
E 18

#
E 8
I 5
D 6
# DO NOT CHANGE ANYTHING BELOW THIS POINT!!!
E 6
I 6
# YOU SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS POINT!!!
E 6
#
E 5
# Compilation/assembly control
#

D 2
INCLUDES = -I. -I/home/fac/kar/include -I/home/fac/wrc/include
E 2
I 2
D 3
INCLUDES = -I. -I/home/fac/kar/include
E 3
I 3
#
D 4
# We only want the include files in the current directory
E 4
I 4
D 5
# We only want to include from the current directory and ~kar/include
E 5
I 5
# We only want to include from the current directory and ~wrc/include
E 5
E 4
#
D 4
INCLUDES = -I.
E 4
I 4
D 5
INCLUDES = -I. -I/home/fac/kar/include
E 5
I 5
INCLUDES = -I. -I/home/fac/wrc/include
E 5
E 4
E 3
E 2

I 8
#
# Compilation/assembly/linking commands and options
#
CPP = cpp
D 13
CPPFLAGS = $(USER_OPTIONS) -nostdinc -I- $(INCLUDES)
E 13
I 13
# CPPFLAGS = $(USER_OPTIONS) -nostdinc -I- $(INCLUDES)
CPPFLAGS = $(USER_OPTIONS) -nostdinc $(INCLUDES)
E 13

E 8
CC = gcc
D 3
CFLAGS = -fno-builtin -Wall -Wstrict-prototypes -nostdinc -I- $(INCLUDES)
E 3
I 3
D 4
CFLAGS = -fno-builtin -Wall -Wstrict-prototypes -nostdinc -I- $(INCLUDES) -ggdb
E 4
I 4
D 8
CFLAGS = -fno-builtin -Wall -Wstrict-prototypes -nostdinc -I- $(INCLUDES)
E 8
I 8
D 13
CFLAGS = -fno-builtin -Wall -Wstrict-prototypes $(CPPFLAGS)
E 13
I 13
D 14
CFLAGS = -fno-stack-protector -fno-builtin -Wall -Wstrict-prototypes $(CPPFLAGS)
E 14
I 14
D 15
CFLAGS = -std=c99 -fno-stack-protector -fno-builtin -Wall -Wstrict-prototypes $(CPPFLAGS)
E 15
I 15
CFLAGS = -m32 -std=c99 -fno-stack-protector -fno-builtin -Wall -Wstrict-prototypes $(CPPFLAGS)
E 15
E 14
E 13
E 8
E 4
E 3

D 3
AS = as
E 3
I 3
D 13
AS = gas
E 13
I 13
AS = as
E 13
E 3
D 15
ASFLAGS =
E 15
I 15
ASFLAGS = --32
E 15

D 8
CPP = cpp
D 2
CPPFLAGS = -traditional -nostdinc $(INCLUDES)
E 2
I 2
CPPFLAGS = -nostdinc $(INCLUDES)
E 8
I 8
D 13
LD = gld
E 13
I 13
LD = ld
I 15
LDFLAGS = -melf_i386
E 15
E 13
E 8
E 2

#		
# Transformation rules - these ensure that all compilation
# flags that are necessary are specified
#
D 3
# Note use of 'cc' to assemble R4600 statements - this allows
# use of CPP #include/#define statements
E 3
I 3
D 5
# Note use of 'cc' to convert .S files to temporary .s files: this allows
# use of CPP #include/#define statements. However, the line numbers of
E 5
I 5
# Note use of 'cpp' to convert .S files to temporary .s files: this allows
D 8
# use of #include/#define statements. However, the line numbers of
E 8
I 8
# use of #include/#define/#ifdef statements. However, the line numbers of
E 8
E 5
# error messages reflect the .s file rather than the original .S file. 
# (If the .s file already exists before a .S file is assembled, then
# the temporary .s file is not deleted.  This is useful for figuring
# out the line numbers of error messages, but take care not to accidentally
# start fixing things by editing the .s file.)
E 3
#
I 18
# The .c.X rule produces a .X file which contains the original C source
# code from the file being compiled mixed in with the generated
# assembly language code.  Very helpful when you need to figure out
# exactly what C statement generated which assembly statements!
#
E 18

D 18
.SUFFIXES:	.S .b
E 18
I 18
.SUFFIXES:	.S .b .X

.c.X:
	$(CC) $(CFLAGS) -g -c -Wa,-adhln $*.c > $*.X
E 18

.c.s:
	$(CC) $(CFLAGS) -S $*.c

.S.s:
	$(CPP) $(CPPFLAGS) -o $*.s $*.S

I 6
.S.o:
	$(CPP) $(CPPFLAGS) -o $*.s $*.S
D 15
	$(AS) -o $*.o $*.s -a=$*.lst
E 15
I 15
	$(AS) $(ASFLAGS) -o $*.o $*.s -a=$*.lst
E 15
I 12
	$(RM) -f $*.s
E 12

E 6
.s.b:
D 15
	$(AS) -o $*.o $*.s -a=$*.lst
	$(LD) -Ttext 0x0 -s --oformat binary -e begtext -o $*.b $*.o
E 15
I 15
	$(AS) $(ASFLAGS) -o $*.o $*.s -a=$*.lst
	$(LD) $(LDFLAGS) -Ttext 0x0 -s --oformat binary -e begtext -o $*.b $*.o
E 15

.c.o:
	$(CC) $(CFLAGS) -c $*.c

D 3
.S.o:
	$(CC) $(CFLAGS) -c $*.S

E 3
D 5
# Binary/source file for system start routines
E 5
I 5
# Binary/source file for system bootstrap code
E 5

D 5
START_OBJ = bootstrap.b
START_SRC = bootstrap.S
E 5
I 5
BOOT_OBJ = bootstrap.b
BOOT_SRC = bootstrap.S
E 5

D 8
# Object/source files from assembly files in the system
E 8
I 8
# Assembly language object/source files
E 8

D 2
S_OBJ = startup.o $(U_S_OBJ)
S_SRC =	startup.S $(U_S_SRC)
E 2
I 2
D 18
S_OBJ = startup.o isr_stubs.o $(U_S_OBJ)
S_SRC =	startup.S isr_stubs.S $(U_S_SRC)
E 18
I 18
FMK_S_OBJ = startup.o isr_stubs.o $(U_S_OBJ)
FMK_S_SRC =	startup.S isr_stubs.S $(U_S_SRC)
E 18
E 2

D 8
# Object/source files from C files in system
E 8
I 8
# C object/source files
E 8

D 2
C_OBJ =	c_io.o $(U_C_OBJ)
C_SRC =	c_io.c $(U_C_SRC)
E 2
I 2
D 3
C_OBJ =	c_io.o support.o $(U_C_OBJ)
C_SRC =	c_io.c support.c $(U_C_SRC)
E 3
I 3
D 4
C_OBJ =	c_io.o support.o gdb_stub.o $(U_C_OBJ)
C_SRC =	c_io.c support.c gdb_stub.c $(U_C_SRC)
E 4
I 4
D 18
C_OBJ =	c_io.o support.o $(U_C_OBJ)
C_SRC =	c_io.c support.c $(U_C_SRC)
E 18
I 18
FMK_C_OBJ =	c_io.o support.o $(U_C_OBJ)
FMK_C_SRC =	c_io.c support.c $(U_C_SRC)
E 18
E 4
E 3
E 2

# Collections of files

D 5
OBJECTS  = $(S_OBJ) $(C_OBJ)
E 5
I 5
D 18
OBJECTS = $(S_OBJ) $(C_OBJ)
E 18
I 18
FMK_OBJS = $(FMK_S_OBJ) $(FMK_C_OBJ)
FMK_SRCS = $(FMK_S_SRC) $(FMK_C_SRC)
E 18
E 5

D 3
HEADERS = c_io.h
E 3
I 3
D 4
HEADERS = c_io.h gdb_stub.h
E 3

E 4
D 5
SOURCES = $(START_SRC) $(S_SRC) $(C_SRC)
E 5
I 5
D 18
SOURCES = $(BOOT_SRC) $(S_SRC) $(C_SRC)
E 18
I 18
OBJECTS = $(FMK_OBJS) $(SYS_OBJS) $(USR_OBJS)
SOURCES = $(FMK_SRCS) $(SYS_SRCS) $(USR_SRCS)
E 18
E 5

#
# Targets for remaking bootable image of the program
#
D 9
# This is the default target
E 9
I 9
# Default target:  usb.image
E 9
#

D 3
prog.img: bootstrap.b prog.b nl dis modify
	cat bootstrap.b prog.b > prog.img
	./modify prog.img prog.b
E 3
I 3
D 4
disk.image: bootstrap.b prog.b prog.nl prog.dis BuildImage
E 4
I 4
D 9
disk.image: bootstrap.b prog.b prog.nl BuildImage #prog.dis 
E 4
	./BuildImage -o disk.image bootstrap.b prog.b 0x10000
E 9
I 9
D 17
usb.image: bootstrap.b prog.b prog.nl BuildImage #prog.dis 
E 17
I 17
D 18
usb.image: bootstrap.b prog.b prog.nl BuildImage prog.dis 
E 18
I 18
usb.image: bootstrap.b prog.b prog.nl BuildImage prog.dis Offsets
E 18
E 17
	./BuildImage -d usb -o usb.image -b bootstrap.b prog.b 0x10000
E 9
E 3

I 9
D 17
floppy.image: bootstrap.b prog.b prog.nl BuildImage #prog.dis 
E 17
I 17
D 18
floppy.image: bootstrap.b prog.b prog.nl BuildImage prog.dis 
E 18
I 18
floppy.image: bootstrap.b prog.b prog.nl BuildImage prog.dis Offsets
E 18
E 17
	./BuildImage -d floppy -o floppy.image -b bootstrap.b prog.b 0x10000

E 9
I 3
prog.out: $(OBJECTS)
D 15
	$(LD) -o prog.out $(OBJECTS)
E 15
I 15
	$(LD) $(LDFLAGS) -o prog.out $(OBJECTS)
E 15

E 3
prog.o:	$(OBJECTS)
D 3
	$(LD) -o prog.o -Ttext 0x10800 $(OBJECTS)
E 3
I 3
D 4
	$(LD) -o prog.o -Ttext 0x10000 $(OBJECTS)
E 4
I 4
D 15
	$(LD) -o prog.o -Ttext 0x10000 $(OBJECTS) $(U_LIBS)
E 15
I 15
	$(LD) $(LDFLAGS) -o prog.o -Ttext 0x10000 $(OBJECTS) $(U_LIBS)
E 15
E 4
E 3

prog.b:	prog.o
D 3
	$(LD) -o prog.b -s --oformat binary -Ttext 0x10800 prog.o
E 3
I 3
D 15
	$(LD) -o prog.b -s --oformat binary -Ttext 0x10000 prog.o
E 15
I 15
	$(LD) $(LDFLAGS) -o prog.b -s --oformat binary -Ttext 0x10000 prog.o
E 15
E 3

#
D 9
# Target for copying bootable image onto a floppy
E 9
I 9
# Targets for copying bootable image onto boot devices
E 9
#

D 3
floppy:	prog.img
	dd if=prog.img of=/dev/fd0
E 3
I 3
D 9
floppy:	disk.image
	dd if=disk.image of=/dev/fd0
E 9
I 9
floppy:	floppy.image
	dd if=floppy.image of=/dev/fd0
E 9
E 3

I 9
D 13
usbu:	usb.image
D 10
	dd if=usb.image of=/local/usbu
E 10
I 10
	dd if=usb.image of=/local/devices/usbu
E 10

usbl:	usb.image
D 10
	dd if=usb.image of=/local/usbl
E 10
I 10
	dd if=usb.image of=/local/devices/usbl
E 10

I 10
usbr:	usb.image
	dd if=usb.image of=/local/devices/usbr
E 13
I 13
usb:	usb.image
D 16
	dd if=usb.image of=/local/devices/disk
E 16
I 16
	/usr/local/dcs/bin/dcopy usb.image
E 16
E 13

E 10
E 9
#
D 6
# Special rule for creating the modification program (don't want
# to use the same options as for the OS binaries)
E 6
I 6
# Special rule for creating the modification and offset programs
E 6
#
I 6
# These are required because we don't want to use the same options
# as for the standalone binaries.
#
E 6

D 3
modify:	modify.c
	$(CC) -o modify modify.c
E 3
I 3
BuildImage:	BuildImage.c
	$(CC) -o BuildImage BuildImage.c
E 3

I 6
Offsets:	Offsets.c
D 18
	$(CC) $(INCLUDES) -o Offsets Offsets.c
E 18
I 18
	$(CC) -mx32 -std=c99 $(INCLUDES) -o Offsets Offsets.c
E 18

E 6
#
# Clean out this directory
#

clean:
D 3
	rm -f *.nl *.lst *.b *.o *.img *.dis modify 
E 3
I 3
D 6
	rm -f *.nl *.lst *.b *.o *.image *.dis BuildImage 
E 6
I 6
D 18
	rm -f *.nl *.lst *.b *.o *.image *.dis BuildImage Offsets
E 18
I 18
	rm -f *.X *.nl *.lst *.b *.o *.image *.dis BuildImage Offsets

realclean:	clean
E 18
E 6
E 3

#
# Create a printable namelist from the prog.o file
#

D 3
nl:	prog.nl

E 3
prog.nl: prog.o
	nm -Bng prog.o | pr -w80 -3 > prog.nl

#
# Generate a disassembly
#

D 3
dis:	prog.dis

E 3
prog.dis: prog.o
D 17
	dis prog.o > prog.dis
E 17
I 17
	objdump -d prog.o > prog.dis
E 17

#
#       makedepend is a program which creates dependency lists by
#       looking at the #include lines in the source files
#

depend:
	makedepend $(INCLUDES) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.

I 18
startup.o: bootstrap.h
isr_stubs.o: bootstrap.h
c_io.o: c_io.h startup.h support.h /home/fac/wrc/include/x86arch.h
support.o: startup.h support.h c_io.h /home/fac/wrc/include/x86arch.h
support.o: bootstrap.h
clock.o: /home/fac/wrc/include/x86arch.h startup.h common.h c_io.h kmalloc.h
clock.o: support.h system.h bootstrap.h pcbs.h stacks.h queues.h klib.h
clock.o: clock.h scheduler.h sio.h syscalls.h
klibc.o: common.h c_io.h kmalloc.h support.h system.h
klibc.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h queues.h
klibc.o: klib.h scheduler.h sio.h
kmalloc.o: common.h c_io.h kmalloc.h support.h system.h
kmalloc.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h
kmalloc.o: queues.h klib.h
pcbs.o: common.h c_io.h kmalloc.h support.h system.h
pcbs.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h queues.h
pcbs.o: klib.h
queues.o: common.h c_io.h kmalloc.h support.h system.h
queues.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h
queues.o: queues.h klib.h scheduler.h sio.h
scheduler.o: common.h c_io.h kmalloc.h support.h system.h
scheduler.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h
scheduler.o: queues.h klib.h scheduler.h
sio.o: common.h c_io.h kmalloc.h support.h system.h
sio.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h queues.h
sio.o: klib.h sio.h scheduler.h startup.h /home/fac/wrc/include/uart.h
stacks.o: common.h c_io.h kmalloc.h support.h system.h
stacks.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h
stacks.o: queues.h klib.h
syscalls.o: common.h c_io.h kmalloc.h support.h system.h
syscalls.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h
syscalls.o: queues.h klib.h /home/fac/wrc/include/uart.h startup.h syscalls.h
syscalls.o: scheduler.h clock.h sio.h
system.o: common.h c_io.h kmalloc.h support.h system.h
system.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h
system.o: queues.h klib.h clock.h syscalls.h sio.h scheduler.h users.h
ulibc.o: common.h c_io.h kmalloc.h support.h system.h
ulibc.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h queues.h
ulibc.o: klib.h
users.o: common.h c_io.h kmalloc.h support.h system.h
users.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h queues.h
users.o: klib.h users.h
ulibs.o: syscalls.h common.h c_io.h kmalloc.h support.h system.h
ulibs.o: /home/fac/wrc/include/x86arch.h bootstrap.h pcbs.h stacks.h queues.h
ulibs.o: klib.h
E 18
D 2
c_io.o: c_io.h startup.h
E 2
I 2
D 3
c_io.o: c_io.h startup.h support.h /home/fac/kar/include/x86arch.h
support.o: startup.h support.h c_io.h /home/fac/kar/include/x86arch.h
E 2
main.o: c_io.h
E 3
E 1
