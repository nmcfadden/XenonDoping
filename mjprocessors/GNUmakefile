# $Id: GNUmakefile.in,v 1.1 2007-04-04 18:36:55 mgmarino Exp $
# --------------------------------------------------------------
#
# 04 Apr 2007, Created, M. Marino 
#


include ../buildTools/config.mk

name := $(notdir $(shell pwd))
G4TARGET := $(name)
G4EXLIB := true

#ROOTSOCLASSES = MGProcessor MGProcessorAnalysis MGProcessorMCRadialDiscriminator MGProcessorMCSegmentation
ROOTSOCLASSES = $(basename $(notdir $(shell grep -r -l ClassDef $(name)/* | xargs)))
MGROOTSODEPS = io

MGTARGETS = MJAnalysis
MGTARGETDEPS = waveform mjio io 
.PHONY : all 
all: rootso lib mgbin

include /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/config/binmake.gmk
include ../buildTools/rootsomake.mk
include ../buildTools/mgbin.mk
include ../buildTools/noinstall.mk
