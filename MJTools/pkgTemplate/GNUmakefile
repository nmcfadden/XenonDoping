# $Id: GNUmakefile,v 1.1.1.1 2004-11-08 17:53:42 gast Exp $
# --------------------------------------------------------------
# GNUmakefile for this module.      Akbar Mokhtarani, March-4-2004 
# --------------------------------------------------------------
SUBDIRS = src

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

all: lib bin
bin : 
lib : $(OBJFILES)


include $(BASE_DIR)/buildTools/standard.mk