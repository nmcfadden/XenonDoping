# GNUmakefile 
# Created for MaGe modules by Jason Detwiler, June 2005

include ../buildTools/config.mk

name := $(notdir $(shell pwd))
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib

include /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/config/binmake.gmk
include ../buildTools/install.mk
