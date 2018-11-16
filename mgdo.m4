dnl -*- mode: autoconf -*- 
dnl
dnl $Id: mgdo.m4,v 1.6 2009-03-18 13:36:01 lenz Exp $
dnl $Author: lenz $
dnl $Date: 2009-03-18 13:36:01 $
dnl
dnl Autoconf macro to check for existence of MGDO on the system
dnl Synopsis:
dnl
dnl  MGDO_PATH([ACTION-IF-FOUND, [ACTION-IF-NOT-FOUND])
dnl
dnl Some examples: 
dnl 
dnl    MGDO_PATH(, AC_MSG_ERROR(MGDO cannot be found))
dnl    MGDO_PATH(AC_DEFUN(HAVE_MGDO))
dnl 
dnl The macro defines the following substitution variables
dnl
dnl    MGDO_BASE_LIB_DIR         Where the MGDO Base library is 
dnl    MGDO_ROOT_LIB_DIR         Where the MGDO Root library is
dnl    MGDO_TRANSFORMS_LIB_DIR   Where the MGDO Transforms library is
dnl    MGDO_GERDA_LIB_DIR        Where the MGDO Gerda library is
dnl    MGDO_BASE_LIB             Name of MGDO Base lib
dnl    MGDO_ROOT_LIB             Name of MGDO ROOT lib
dnl    MGDO_TRANSFORMS_LIB       Name of MGDO Transforms lib
dnl    MGDO_GERDA_LIB            Name of MGDO Gerda lib
dnl    MGDO_ROOT_INC_DIR         Name of MGDO ROOT include directory
dnl    MGDO_BASE_INC_DIR         Name of MGDO Base include directory
dnl    MGDO_TRANSFORMS_INC_DIR   Name of MGDO Transforms include directory
dnl    MGDO_GERDA_INC_DIR        Name of MGDO Gerda include directory
dnl
dnl The macro will fail if it can't guess where the libraries are. 
dnl
dnl MGMarino based upon root.m4 
dnl
AC_DEFUN([MGDO_PATH],
[
  AC_ARG_WITH(mgdo,
    AC_HELP_STRING([--with-mgdo],[full path to MGDO base directory]),
    user_mgdo=$withval,
    user_mgdo="none")

  no_mgdo="yes"
  mgdoconf_path=$PATH

  if test ! x"$user_mgdo" = xnone; then
    echo "Using user-defined mgdo in path $user_mgdo"
    MGDODIR="$user_mgdo"
    mgdoconf_path="$user_mgdo/bin"
  elif test ! x"$MGDODIR" = x; then
    MGDODIR="$MGDODIR"
  elif test -d "../MGDO" ; then
    MGDODIR=`cd ../; pwd`
    MGDODIR="$MGDODIR/MGDO"
  fi

  AC_PATH_PROG(MGDOCONF, mgdo-config , no, $mgdoconf_path)

  if test ! x"$MGDOCONF" = "xno" ; then

    MGDO_CFLAGS=`$MGDOCONF --cflags --root --transforms --gerda`
    MGDO_LIBS=`$MGDOCONF --libs --root --transforms --gerda`
    no_mgdo=""

  elif test ! x"$MGDODIR" = "x" ; then

    echo "Using mgdo in $MGDODIR"
    no_mgdo=""
    MGDO_BASE_LIB_DIR="$MGDODIR/lib"
    MGDO_ROOT_LIB_DIR="$MGDODIR/lib"
    MGDO_TRANSFORMS_LIB_DIR="$MGDODIR/lib"
    MGDO_GERDA_LIB_DIR="$MGDODIR/lib"

    echo "Using MGDO_BASE_LIB_DIR: $MGDO_BASE_LIB_DIR"
    echo "Using MGDO_ROOT_LIB_DIR: $MGDO_ROOT_LIB_DIR"
    MGDO_BASE_LIB="MGDOBase" 
    MGDO_ROOT_LIB="MGDORoot" 
    MGDO_TRANSFORMS_LIB="MGDOTransforms" 
    MGDO_GERDA_LIB="MGDOGerda" 
    if test -f "$MGDO_BASE_LIB_DIR/lib$MGDO_BASE_LIB.so"; then
      # .a file exists
      echo "MGDO Base lib found."
    elif test -f "$MGDO_BASE_LIB_DIR/lib$MGDO_BASE_LIB.dylib"; then
      # so file exists. 
      echo "MGDO Base lib found."
    elif test -f "$MGDO_BASE_LIB_DIR/lib$MGDO_BASE_LIB.a"; then
      echo "MGDO Base lib found."
    else
      # no lib files!
      echo "No MGDO lib Base file found!"
      no_mgdo="yes"
    fi
    if test -f "$MGDO_ROOT_LIB_DIR/lib$MGDO_ROOT_LIB.so"; then
      # .a file exists
      echo "MGDO Root lib found."
    elif test -f "$MGDO_ROOT_LIB_DIR/lib$MGDO_ROOT_LIB.dylib"; then
      # so file exists. 
      echo "MGDO Root lib found."
    elif test -f "$MGDO_ROOT_LIB_DIR/lib$MGDO_ROOT_LIB.a"; then
      echo "MGDO Root lib found."
    else
      # no lib files!
      echo "No MGDO Root lib file found!"
      no_mgdo="yes"
    fi

    if test -f "$MGDO_TRANSFORMS_LIB_DIR/lib$MGDO_TRANSFORMS_LIB.so"; then
      # .a file exists
      echo "MGDO Transforms lib found."
    elif test -f "$MGDO_TRANSFORMS_LIB_DIR/lib$MGDO_TRANSFORMS_LIB.dylib"; then
      # so file exists. 
      echo "MGDO Transforms lib found."
    elif test -f "$MGDO_TRANSFORMS_LIB_DIR/lib$MGDO_TRANSFORMS_LIB.a"; then
      echo "MGDO Transforms lib found."
    else
      # no lib files!
      echo "No MGDO Transforms lib file found!"
      no_mgdo="yes"
    fi

    if test -f "$MGDO_GERDA_LIB_DIR/lib$MGDO_GERDA_LIB.so"; then
      # .a file exists
      echo "MGDO Gerda lib found."
    elif test -f "$MGDO_GERDA_LIB_DIR/lib$MGDO_GERDA_LIB.dylib"; then
      # so file exists. 
      echo "MGDO Gerda lib found."
    elif test -f "$MGDO_GERDA_LIB_DIR/lib$MGDO_GERDA_LIB.a"; then
      echo "MGDO Gerda lib found."
    else
      # no lib files!
      echo "No MGDO Gerda lib file found!"
      no_mgdo="yes"
    fi

    MGDO_BASE_INC_DIR="$MGDODIR/Base"
    MGDO_ROOT_INC_DIR="$MGDODIR/Root"
    MGDO_TRANSFORMS_INC_DIR="$MGDODIR/Transforms"
    MGDO_GERDA_INC_DIR="$MGDODIR/Gerda"
    echo "Using MGDO_BASE_INC_DIR: $MGDO_BASE_INC_DIR"
    echo "Using MGDO_ROOT_INC_DIR: $MGDO_ROOT_INC_DIR"
    echo "Using MGDO_TRANSFORMS_INC_DIR: $MGDO_TRANSFORMS_INC_DIR"
    echo "Using MGDO_GERDA_INC_DIR: $MGDO_GERDA_INC_DIR"
    AC_CHECK_FILE($MGDO_BASE_INC_DIR/MGDOUtils.hh,,no_mgdo=yes) 
    AC_CHECK_FILE($MGDO_ROOT_INC_DIR/MGTDataObject.hh,,no_mgdo=yes) 
    AC_CHECK_FILE($MGDO_TRANSFORMS_INC_DIR/MGWFAddNoise.hh,,no_mgdo=yes) 
    AC_CHECK_FILE($MGDO_GERDA_INC_DIR/GETSiegfriedIICrystalData.hh,,no_mgdo=yes) 

    MGDO_CFLAGS=["-I${MGDO_BASE_INC_DIR} -I${MGDO_ROOT_INC_DIR} -I${MGDO_TRANSFORMS_INC_DIR}  -I${MGDO_GERDA_INC_DIR}"]
    MGDO_LIBS=["-L${MGDO_BASE_LIB_DIR} -l${MGDO_BASE_LIB} -l${MGDO_ROOT_LIB} -l${MGDO_TRANSFORMS_LIB} -l${MGDO_GERDA_LIB}"]

  fi
  
  AC_SUBST(MGDODIR)
  AC_SUBST(MGDO_CFLAGS)
  AC_SUBST(MGDO_LIBS)

  if test x"$no_mgdo" = x ; then 
    ifelse([$1], , :, [$1])
  else 
    ifelse([$2], , :, [$2])
  fi
])


#
# EOF
#
