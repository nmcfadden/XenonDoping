#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.50)
AC_INIT([MaGe], [0], [], [mage])
AC_CONFIG_SRCDIR([management/MaGe.cc])

echo "Run ./configure --help for a list of options"
echo

# Check for tools
AC_PROG_INSTALL

AC_PROG_GREP
AC_PROG_SED

AC_CANONICAL_HOST

# defaults, these will work for linux
# but NOT in screen.  Perhaps a fix for this?
BOLD=`tput bold`
OFFBOLD=`tput sgr0`

# doing host os specific things
EXTRALIBS=""
EXTRACPPFLAGS=""
ROOTSOFLAGS="-shared -fPIC"
MGLIBEXTSHARED=so
case "${host_os}" in
  darwin*)
  EXTRALIBS="-bind_at_load"
  ROOTSOFLAGS="-bundle -fPIC"
  MGLIBEXTSHARED=dylib
  BOLD=`tput smso`
esac

# doing host cpu (i.e. architecture) specific things

case "${host_cpu}" in
  x86_64*)
  EXTRACPPFLAGS="-fPIC"
  echo "Compiling on an x86_64 chipset...Setting -fPIC flag."
  # only on amd64 should one compile static libs with this flag
  # Other tested architectures currently compile PIC libraries
  # and so it is no problem to link a dynamic library using the PIC
  # libs
esac
# end host


#find relevant G4 vars"
echo ""
echo "${BOLD}Checking for Geant4: ${OFFBOLD}"
m4_include(geant4.m4)
GEANT4_INSTALL(AC_DEFUN(HAVE_G4), AC_MSG_ERROR([Some Geant4 variables are not defined correctly.  Has the env.(c)sh file in the G4 directory been sourced?]))
echo "${BOLD}Geant4 seems to be successfully installed. ${OFFBOLD}"
#end geant4

#Find root
echo ""
echo "${BOLD}Checking for ROOT: ${OFFBOLD} "
m4_include(root.m4)
ROOT_PATH(,AC_DEFUN(HAVE_ROOT), AC_MSG_ERROR([ROOT cannot be found. Please ensure that it is installed and that either \$ROOTSYS is defined corectly or root-config is in the path.  One can pass --with-rootsys=/path/to/rootsys to configure as well.]))
echo "${BOLD}ROOT seems to be successfully installed. ${OFFBOLD}"
#end root

#Find mgdo
echo ""
echo "${BOLD}Checking for MGDO: ${OFFBOLD} "
m4_include(mgdo.m4)
MGDO_PATH(AC_DEFUN(HAVE_MGDO), AC_MSG_ERROR([MGDO cannot be found. Please ensure that it is installed and that \$MGDODIR is defined corectly.  One can pass --with-mgdo=/path/to/mgdo to configure as well.]))
echo "${BOLD}MGDO seems to be successfully installed. ${OFFBOLD}"
#end mgdo

#Find gdml (11-10-2007, Jing)
# G4-GDML  (08-03-2010, Alex)
m4_include(gdml.m4)
MG_CHECK_G4GDML
#end gdml

# checking to compile with everything?
AC_ARG_ENABLE(no-skip, AC_HELP_STRING([--enable-no-skip],[doesn't skip any of the packages]),
  [ if test "x$enableval"="xyes"; then
     enable_noskip=yes
    else
     enable_noskip=no
    fi
   ]
  ,[enable_noskip=no])
if test "x$enable_noskip" = "xyes" ; then
  MGSKIPLIBS=""
  echo
  echo "${BOLD}Compiling all libs. ${OFFBOLD}"
else
  MGSKIPLIBS="mjprocessors"
fi
AC_SUBST(MGSKIPLIBS)


echo ""
# Setting MaGe path to be where the config file is.
# Soon, it would be nice to compile in this directory
# as opposed to G4WORKDIR given that it would self-contain
# all the libs and bins to the one directory.
# This must be done after a complete migration away from the
# old make process.
MAGEDIR=`pwd`
AC_SUBST(MAGEDIR)


# compile with high verbosity (output everything that "make" does)?
# default is no extra output
AC_ARG_ENABLE(verbosity, AC_HELP_STRING([--enable-verbosity],[enables verbosity of compilation commands]),
  [ if test "x$enableval"="xyes"; then
     enable_verbosity=yes
    else
     enable_verbosity=no
    fi
   ]
  ,[enable_verbosity=no])
if test "x$enable_verbosity" = "xyes" ; then
  CPPVERBOSE="ON"
  echo
  echo "${BOLD}Configuring with verbosity ON. All \"make\" commands will be echoed. ${OFFBOLD}"
  echo
else
  unset CPPVERBOSE
  echo
  echo "${BOLD}Configuring without extra verbosity. The compilation will be quiet as in Geant4. ${OFFBOLD}"
  echo "Verbosity may be enabled by passing --enable-verbosity to configure."
  echo
fi
AC_SUBST(CPPVERBOSE)


#
# Now add extra CPPFLAGS if specified in configure
#

# checking to compile as debug?
AC_ARG_ENABLE(debug, AC_HELP_STRING([--enable-debug],[enables compiling with debug flags]),
  [ if test "x$enableval"="xyes"; then
     enable_debug=yes
    else
     enable_debug=no
    fi
   ]
  ,[enable_debug=no])
if test "x$enable_debug" = "xyes" ; then
  EXTRACPPFLAGS="-g $EXTRACPPFLAGS"
  echo
  echo "${BOLD}Configuring with debug flags ON. ${OFFBOLD}"
else
  EXTRACPPFLAGS="-O2 $EXTRACPPFLAGS"
  echo ""
  echo "${BOLD}Configuring with debug flags OFF. ${OFFBOLD}"
  echo "Debug flags enabled by passing --enable-debug to configure."
fi


# compile with -Wshadow warnings?
AC_ARG_ENABLE(shadow-warnings, AC_HELP_STRING([--enable-shadow-warnings],[enables -Wshadow flag]),
  [ if test "x$enableval"="xyes"; then
     enable_shadow_warnings=yes
    else
     enable_shadow_warnings=no
    fi
   ]
  ,[enable_shadow_warnings=no])
if test "x$enable_shadow_warnings" = "xyes" ; then
  EXTRACPPFLAGS="-Wshadow $EXTRACPPFLAGS"
  echo
  echo "${BOLD}Configuring with -Wshadow flag. It will check for all shadowed warnings, also in extra packages like Geant, CLHEP. ${OFFBOLD}"
  echo
fi

# checking to silence g4env output from setup.(c)sh
AC_ARG_ENABLE(silence-g4env, AC_HELP_STRING([--enable-silence-g4env],[redirect output from source g4env.(c)sh to /dev/null in setup.(c)sh]),
  [ if test "x$enableval"="xyes"; then
     enable_silence_g4env=yes
    else
     enable_silence_g4env=no
    fi
   ]
  ,[enable_silence_g4env=no])
if test "x$enable_silence_g4env" = "xyes" ; then
  SILENCEG4ENV="> /dev/null"
  echo
  echo "${BOLD}Silencing g4env${OFFBOLD}"
else
  SILENCEG4ENV=""
fi
AC_SUBST(SILENCEG4ENV)


AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

AC_SUBST(EXTRALIBS)
AC_SUBST(EXTRACPPFLAGS)
AC_SUBST(ROOTSOFLAGS)
AC_SUBST(MGLIBEXTSHARED)



AC_CONFIG_FILES([
                 setup.csh
                 setup.sh
                 mage-config
                 buildTools/config.mk
                 buildTools/rootsomake.mk
                 buildTools/mgbin.mk
                 buildTools/install.mk
                 buildTools/noinstall.mk
                 GNUmakefile
                 analysis/GNUmakefile
                 generators/GNUmakefile
                 geometry/GNUmakefile
                 gerdageometry/GNUmakefile
                 gerdaio/GNUmakefile
                 io/GNUmakefile
                 management/GNUmakefile
                 materials/GNUmakefile
                 mjgeometry/GNUmakefile
                 mjdemonstrator/GNUmakefile
                 mjio/GNUmakefile
                 mjprocessors/GNUmakefile
                 munichteststand/GNUmakefile
                 lngsgesetups/GNUmakefile
                 processes/GNUmakefile
                 sandbox/GNUmakefile
                 tonscalegeometry/GNUmakefile
                 waveform/GNUmakefile
                 legendgeometry/GNUmakefile
                 legendio/GNUmakefile
                 bacongeometry/GNUmakefile])
AC_OUTPUT

echo
echo "**********************************************************************************"
echo "**********************************************************************************"
echo
echo "You have successfully configured MaGe.  Please now compile by issuing the command:"
echo
echo ${BOLD}"     make" ${OFFBOLD}
echo
echo "from the MaGe base directory. If you want a little speed-up of the compilation procedure, run"
echo
echo ${BOLD}"     make -jN" ${OFFBOLD}
echo
echo "where N is the number of compilation jobs to be run in parallel. You may need to run this command"
echo "several times as the compilation is not yet fully optimized to run in parallel."
echo "Always run a simple \"make\" in the end."

echo
if test x"$NO_G4GDML_FLAG" = "x" ; then
  echo
  echo "You are compiling WITH the G4GDML support."
else
  echo
  echo "You are compiling WITHOUT the G4GDML support."
  echo "\"./Configure --help\" gives you information about how to enable G4GDML."
fi

echo

echo "Temporary object files will be installed in: \$G4WORKDIR/tmp/\$G4SYSTEM ($G4WORKDIR/tmp/$G4SYSTEM)"
echo
echo "Binaries will be installed in: \$G4WORKDIR/bin/\$G4SYSTEM ($G4WORKDIR/bin/$G4SYSTEM)"
echo
echo "If this is a problem, please reset the variable G4WORKDIR and rerun configure."
