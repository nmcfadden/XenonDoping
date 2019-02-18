# GNUmakefile 
# Created for MaGe modules by Jason Detwiler, June 2005

include ../buildTools/config.mk

name := $(notdir $(shell pwd))
G4TARGET := $(name)
G4EXLIB := true

#ROOTSOCLASSES = MJOutputSegXtal MJOutputDetectorEventData
ROOTSOCLASSES = $(basename $(notdir $(shell grep -r -l ClassDef $(name)/* | xargs)))

.PHONY: all
all: rootso lib

include /usr/local/share/Geant4-10.4.0/geant4make/config/binmake.gmk
include ../buildTools/rootsomake.mk
include ../buildTools/install.mk
