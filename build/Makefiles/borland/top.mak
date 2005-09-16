# Time-stamp:
# $Id: top.mak

#!message build\Makefiles\borland\top.mak

.AUTODEPEND

.SUFFIXES: .exe .dll .lib .obj .cpp .cc .c

!ifndef BC5ROOT
BC5ROOT          = $(MAKEDIR)\..
!endif

!ifndef STLPORT_DIR
! error STLPORT_DIR is not defined.
!endif

!ifndef SRCROOT
SRCROOT = $(STLPORT_DIR)\build
!endif

!ifndef SRCDIR
!error SRCDIR not defined.
!endif

!ifndef RULESBASE
RULESBASE     = $(SRCROOT)\Makefiles
!endif

!ifndef ALL_TAGS
ALL_TAGS      = release-shared dbg-shared stldbg-shared release-static dbg-static stldbg-static
!endif

all: build_system_message $(ALL_TAGS)
	@echo all done.

all_dynamic: build_system_message release-shared dbg-shared stldbg-shared
	@echo all_dynamic done.

all_static: build_system_message release-static dbg-static stldbg-static
	@echo all_static done.

# define what make clone we use
USE_MAKE = borland

# identify OS and build date
!include $(RULESBASE)\$(USE_MAKE)\sysid.mak

# include config file, generated by configure
!ifdef CFGSET
! include $(RULESBASE)\config.mak
!endif

# rules to make dirs for targets
!include $(RULESBASE)\$(USE_MAKE)\targetdirs.mak

# OS-specific definitions, like ar, ln, install, etc.
!include $(RULESBASE)\$(USE_MAKE)\sys.mak

# extern libraries
!include $(RULESBASE)\$(USE_MAKE)\extern.mak

# derive common targets
!include $(RULESBASE)\$(USE_MAKE)\targets.mak

# dependencies
!include $(RULESBASE)\$(USE_MAKE)\depend.mak

# general clean
!include $(RULESBASE)\$(USE_MAKE)\clean.mak

# implicit rules
!include $(RULESBASE)\$(USE_MAKE)\rules-o.mak

# if target is library, rules for library
!ifdef LIBNAME
!include $(RULESBASE)\$(USE_MAKE)\lib\top.mak
!endif

# if target is program, rules for executable
!ifdef PRGNAME
!include $(RULESBASE)\$(USE_MAKE)\app\top.mak
!endif

install: build_system_message $(INSTALL_TAGS) delete_tags
	@echo install done.
