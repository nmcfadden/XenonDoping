dnl
dnl Autoconf macro to check for existence of GEANT4 on the system
dnl Synopsis:
dnl
dnl  GEANT4_INSTALL([ACTION-IF-FOUND, [ACTION-IF-NOT-FOUND])
dnl
dnl Some examples: 
dnl 
dnl    GEANT4_INSTALL(, AC_MSG_ERROR(GEANT4 cannot be found))
dnl    GEANT4_INSTALL(AC_DEFUN(HAVE_GEANT4))
dnl 
dnl The macro defines the following substitution variables
dnl
dnl    G4INSTALL         Where the GEANT4 install is 
dnl    G4WORKDIR         Where the GEANT4 workdir is 
dnl    G4SYSTEM          The system on which GEANT4 was compiled.
dnl
dnl The macro will fail if it can't find the relevant variables. 
dnl
dnl MGMarino Feb 2007 
dnl
AC_DEFUN([GEANT4_INSTALL],
[

  AC_PATH_PROG(GEANT4_CONFIG, geant4-config, no)

  if test x"$GEANT4_CONFIG" != "xno" ; then
    GEANT4_VERSION=`$GEANT4_CONFIG --version`
    if test x"$GEANT4_VERSION" != "x" ; then
        echo "Found Geant4 version $GEANT4_VERSION"
    else
        AC_MSG_ERROR([Can't determine Geant4 version!])
    fi

    GEANT4_PREFIX=`$GEANT4_CONFIG --prefix`
    echo "Geant4 install prefix: \"$GEANT4_PREFIX\""

dnl There is a problem here in which geant4 defines a default workdir
dnl which should (probably) not be used. It would be better to simply ignore it and 
dnl define the current directory as the current build directory. After version 9.5 the 
dnl default is now $HOME/geant4_workdir so check for that one as well
    if test x"$G4INSTALL" = "x" ; then
      . "$GEANT4_PREFIX/share/Geant4-$GEANT4_VERSION/geant4make/geant4make.sh"
      G4LIB_BUILD_GDML="$G4LIB_USE_GDML"
    fi
  fi

  echo "Checking for relevant Geant4 variables: "
  echo "Checking for G4INSTALL..."
  if test ! x"$G4INSTALL" = x; then 
    echo "  Found: $G4INSTALL"
  else
    echo "  Not Found! Please set \$G4INSTALL"
    no_geant4="yes"
  fi
  echo "Checking for G4SYSTEM..."
  if test ! x"$G4SYSTEM" = x; then 
    echo "  Found: $G4SYSTEM"
  else
    echo "  Not Found! Please set \$G4SYSTEM"
    no_geant4="yes"
  fi
  echo "Checking for G4WORKDIR..."
  if test x"$G4WORKDIR" != x -a x"$G4WORKDIR" != x"$HOME/geant4" -a x"$G4WORKDIR" != x"$HOME/geant4_workdir" ; then 
    echo "  Found: $G4WORKDIR"
  else
    G4WORKDIR=`pwd`
    echo "G4WORKDIR not set, setting it to current build directory: \"$G4WORKDIR\""
  fi

  if test x"$no_geant4" = x ; then 
    AC_CHECK_FILE([$G4INSTALL/config/binmake.gmk],,no_geant4=yes)
dnl    AC_CHECK_FILE([$G4INSTALL/env.csh],,no_geant4=yes)
  fi
  
  echo "CLHEP_BASE_DIR=\"$CLHEP_BASE_DIR\""
  echo "CLHEP_INCLUDE_DIR=\"$CLHEP_INCLUDE_DIR\""
  echo "CLHEP_LIB_DIR=\"$CLHEP_LIB_DIR\""
  echo "G4INSTALL=\"$G4INSTALL\""
  echo "G4INCLUDE=\"$G4INCLUDE\""
  echo "G4LIB=\"$G4LIB\""
  echo "G4SYSTEM=\"$G4SYSTEM\""
  echo "G4WORKDIR=\"$G4WORKDIR\""
  echo "G4LIB_BUILD_GDML=\"$G4LIB_BUILD_GDML\""

  AC_SUBST(CLHEP_BASE_DIR)
  AC_SUBST(CLHEP_INCLUDE_DIR)
  AC_SUBST(CLHEP_LIB_DIR)
  AC_SUBST(G4INSTALL)
  AC_SUBST(G4INCLUDE)
  AC_SUBST(G4LIB)
  AC_SUBST(G4SYSTEM)
  AC_SUBST(G4WORKDIR)
  AC_SUBST(G4VIS_USE_OPENGLX)
  AC_SUBST(G4VIS_USE_OPENGLQT)
  AC_SUBST(G4VIS_USE_RAYTRACERX)
  AC_SUBST(G4VIS_USE_OPENGLXM)


  if test x"$no_geant4" = x ; then 
    ifelse([$1], , :, [$1])     
  else 
    ifelse([$2], , :, [$2])     
  fi
])
