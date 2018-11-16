# config.mk
#
# DO NOT EDIT THIS FILE.  IT HAS BEEN AUTOGENERATED BY CONFIGURE.
# If you have changed your settings, rerun configure.
#
# Created for MaGe by Jason Detwiler, June 2006
# Modified by MGMarino Feb 2007

# Please contact the author if any exceptions are found:
# jasondet@u.washington.edu. config.mk should be included at the TOP of the
# user's makefile

PACKAGE_NAME = MaGe
PACKAGE_TARNAME = mage
PACKAGE_VERSION = 0

DESTDIR =


srcdir = .
subdirs = @subdirs@
top_srcdir = ..

prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
includedir = ${prefix}/include
infodir = ${prefix}/share/info
mandir = ${prefix}/share/man
docdir = ${prefix}/share/doc/${PACKAGE_TARNAME}

pkgdatadir = $(datadir)/$(PACKAGE_TARNAME)
pkgincludedir = $(includedir)/$(PACKAGE_TARNAME)


SHELL = /bin/sh

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}

GREP = /usr/bin/grep
SED = /usr/bin/sed


MAGEDIR=/mnt/mjdDisk1/Majorana/users/nmcfadden/gitBACoN/XenonDoping

CLHEP_BASE_DIR = /opt/CLHEP/2.4.0.1/install
CLHEP_INCLUDE_DIR = /opt/CLHEP/2.4.0.1/install/include
CLHEP_LIB_DIR = /opt/CLHEP/2.4.0.1/install/lib

G4INSTALL = /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make
G4INCLUDE = /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/include/Geant4
G4LIB = /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/lib64/Geant4-10.4.2
G4SYSTEM = Linux-g++

#list of packages to exclude from the build
PKG_SKIP := MJTools buildTools macros doc mjprocessors 

#### You should not need to make modifications below this line ####

PACKAGES := $(dir $(wildcard $(MAGEDIR)/*/GNUmakefile.in))
PACKAGES := $(subst $(MAGEDIR)/,, $(PACKAGES))
PACKAGES := $(subst /,, $(PACKAGES))
PACKAGES := $(filter-out $(PKG_SKIP), $(PACKAGES))

G4WORKDIR=/mnt/mjdDisk1/Majorana/users/nmcfadden/gitBACoN/XenonDoping
G4SYSTEM=Linux-g++
MGTMPDIR=$(G4WORKDIR)/tmp/$(G4SYSTEM)

MGLIBS := $(subst -l, -l, $(join $(addprefix -L$(MGTMPDIR)/, $(PACKAGES)), $(addprefix -l, $(PACKAGES))))

# Basic flags

MGCPPFLAGS := $(addprefix -I$(MAGEDIR)/, $(PACKAGES))
CPPFLAGS += -O2 -fPIC $(MGCPPFLAGS)

# ROOT
CPPFLAGS += -pthread -std=c++11 -m64 -I/mnt/mjdDisk1/Majorana/soft/root-6.12.06-build/include 
EXTRALIBS += -L/mnt/mjdDisk1/Majorana/soft/root-6.12.06-build/lib -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic 

# MGDO
CPPFLAGS += -I/mnt/mjdDisk1/Majorana/soft/MGDO/install/include/mgdo
EXTRALIBS += -L/mnt/mjdDisk1/Majorana/soft/MGDO/install/lib -lMGDOBase -lMGDOTransforms -lMGDORoot -lMGDOGerda

# GDML
CPPFLAGS += -DMG_NO_G4GDML
EXTRALIBS +=  

CPPFLAGS += 
LDFLAGS += 
EXTRALIBS += 

# An option for the full output. Geant4 scripts will echo all the make commands
# if this variable is defined as non-empty
CPPVERBOSE = 

# Add MGLIBS at the end (because we are using static libs)
EXTRALIBS += $(MGLIBS) 

ifdef MJDEBUG
  G4DEBUG := 1
  G4NO_OPTIMISE := 1
#else
#  CXXFLAGS += -O2
endif

EXTRALIBS +=  
ROOTSOFLAGS = -shared -fPIC 
ROOTSOSUFFIX = so

#ifdef G4LIB_BUILD_SHARED
MGLIBEXTSHARED = so
#else
# This is temporary solution to the static/shared problem.  It forces MaGe to be built with static libraries
# which was how the makefile worked previously.  This is not a permanent solution but rather a patch so that
# MaGe works easily until I figure out the best method for fixing this.  M. Marino 
#MGLIBEXT = $(MGLIBEXTSHARED)
MGLIBEXT = a
G4LIB_BUILD_STATIC=1
#endif

