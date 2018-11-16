# GNUmakefile 
# Created for MaGe modules by Jason Detwiler, June 2005

include ../buildTools/config.mk

name := $(notdir $(shell pwd))
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib mgbin

MGTARGETS = TestCrystalHits TestWaveforms WaveformBrowser TestM3D2S TestSiggenUtils TestM3dcrPPCCrystalFields TestSiggenDriftVelocity
MGTARGETS += TestOccupancyMaGe TestOccupancyCustom TestSiggen
MGTARGETS += TestDriftVelocity TestGenWFforEvents TestSOREField TestWFGenFromDrift TestWFResampler
MGTARGETDEPS = waveform io




include /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/config/binmake.gmk
include ../buildTools/mgbin.mk
include ../buildTools/install.mk

LIBFILES := 
HEADERS := 
BINARIES := $(G4BINDIR)/WaveformBrowser
