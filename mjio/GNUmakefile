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

include /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/config/binmake.gmk
include ../buildTools/rootsomake.mk
include ../buildTools/install.mk
