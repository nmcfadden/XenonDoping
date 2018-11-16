
name := DecayEngine
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../../..
endif

.PHONY: all
all: lib bin

ifdef G4ANALYSIS_USE_ROOT
  CPPFLAGS += -DG4ANALYSIS_USE_ROOT 
endif

#G4ANALYSIS_USE := true

include $(G4INSTALL)/config/architecture.gmk

ifdef G4ANALYSIS_USE_ROOT
  CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
  LDFLAGS  += $(shell $(ROOTSYS)/bin/root-config --glibs)
endif

include $(G4INSTALL)/config/binmake.gmk

histclean:
	rm ${G4WORKDIR}/tmp/${G4SYSTEM}/${G4TARGET}/HistoManager.o
