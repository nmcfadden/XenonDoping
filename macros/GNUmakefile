# $Id: GNUmakefile,v 1.2 2004-11-09 13:42:39 xliu Exp $
# --------------------------------------------------------------
# GNUmakefile for this module.      Akbar Mokhtarani, March-4-2004 
# --------------------------------------------------------------
SUBDIRS = 

.PHONY : all 

PACKAGENAME=$(notdir $(shell /bin/pwd))

SRCDIRS += $(SUBDIRS) 

#list aditional .cc files (not in src) here
CCFILES += 

#executable, name is package name + App
#BINTARGET :=$(PACKAGENAME)App
BINTARGET :=

#BINCCFILE := test$(PACKAGENAME).cc   #this is the name of test routine

BINCCFILE :=

VPATH += $(SRCDIRS:' '=':')
VPATH +=$(BASE_DIR)

setup:

