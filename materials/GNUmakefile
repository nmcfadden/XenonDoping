# GNUmakefile 
# Created for MaGe modules by Jason Detwiler, June 2005

include ../buildTools/config.mk

name := $(notdir $(shell pwd))
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib 

include /usr/local/share/Geant4-10.4.0/geant4make/config/binmake.gmk
include ../buildTools/install.mk
