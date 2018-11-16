dnl -*- mode: autoconf -*- 
dnl
dnl $Id: gdml.m4,v 1.3 2007-10-13 13:42:09 jliu Exp $
dnl $Author: jliu $
dnl $Date: 2007-10-13 13:42:09 $
dnl
dnl $Modified for G4GDML: alex volynets $
dnl $Date: 2010-03-08 15:03:00 $
dnl
dnl Autoconf macro to check for existence of G4GDML on the system
dnl
dnl The macro will fail if it can't guess where the libraries are. 

AC_DEFUN(MG_CHECK_G4GDML, 
[
   MG_INIT_G4GDML
   MG_WITH_G4GDML
   MG_SUBS_G4GDML
])

AC_DEFUN(MG_INIT_G4GDML, 
[
   echo 
   echo "${BOLD}Checking for G4GDML: ${OFFBOLD}"

   G4GDML_FLAG=""
])

AC_DEFUN(MG_WITH_G4GDML, 
[
   echo "1. Checking for G4GDML base directory setting ..."
   AC_ARG_ENABLE(g4gdml, AC_HELP_STRING([--disable-g4gdml],[disable G4GDML support. Enabled by default]),,enable_g4gdml=yes)
   HAVE_G4GDML_LIB="NO"
   if test "X$enable_g4gdml" != "Xyes"; then
     echo "   MaGe will be compiled WITHOUT G4GDML."
     NO_G4GDML_FLAG="-DMG_NO_G4GDML"
   else
     echo "     ...  OK"

     echo "2. Checking for G4GDML include directory setting ..."
     if test ! -f $G4INCLUDE/G4GDMLParser.hh; then
       AC_MSG_ERROR([$G4INCLUDE/G4GDMLParser.hh in your GEANT4 directory does not exist! You can disable GDML support using --disable-g4gdml])
     else
       echo "     ...  OK"
     fi

     echo "3. Checking for G4GDML lib directory setting ..."
     G4GDML_LIBDIR=$G4LIB/$G4SYSTEM
     G4GDML_LIB=""
     echo "   G4GDML libraries dir is set to $G4GDML_LIBDIR"
     if test ! x"$G4LIB_BUILD_GDML" = x; then
       HAVE_G4GDML_LIB="YES"
       G4GDML_LIBS_COUNT=$(ls $G4GDML_LIBDIR/libG4gdml* 2> /dev/null | wc -l)
       if test ! $G4GDML_LIBS_COUNT = 0; then
         G4GDML_LIB="G4gdml"
       else 
         G4GDML_LIBS_COUNT=$(ls $G4GDML_LIBDIR/libG4persistency* 2> /dev/null | wc -l)
         if test ! "$G4GDML_LIBS_COUNT" = 0; then
           G4GDML_LIB="G4persistency"
         fi
       fi
       if test x"$G4GDML_LIB" = x; then
         AC_MSG_ERROR([Couldn't set G4GDML_LIB!])
       else
dnl If either G4gdml or G4persistency exists then XERCESCROOT variable should 
dnl also exist in the case xerces is not in a standard path. Check for it 
dnl and update the setup in case it exists
    	XERCES_CFLAGS=""
	XERCES_LIBS=""
	AC_MSG_CHECKING(for non-standard location for Xerces-C in \$XERCESCROOT)
	if test ! x"$XERCESCROOT" = "x" ; then
	   XERCES_CFLAGS="-I$XERCESCROOT/include"
	   XERCES_LIBS="-L$XERCESCROOT/lib"
	   AC_MSG_RESULT(yes)
	else
	   AC_MSG_RESULT(no)
	fi
       fi
     else
       AC_MSG_ERROR([G4gdml libraries(G4gdml/G4persistency) do not exist!])
     fi
   fi
])

AC_DEFUN(MG_SUBS_G4GDML,
[
   if test X"$NO_G4GDML_FLAG" = X; then
     echo "G4GDML is properly set, configuring WITH it. If you did not mean that, set the option --disable-g4gdml "
     NO_G4GDML_FLAG="$XERCES_CFLAGS"
     G4GDML_LIB_DIR="-L$G4GDML_LIBDIR $XERCES_LIBS"
     G4GDML_LIBS="-l$G4GDML_LIB -lxerces-c"
     AC_SUBST(NO_G4GDML_FLAG)
     AC_SUBST(G4GDML_LIB_DIR)
     AC_SUBST(G4GDML_LIBS)
   else
     echo "G4GDML is not enabled, configuring WITHOUT it."
     G4GDML_LIB_DIR=""
     G4GDML_LIBS=""
     AC_SUBST(NO_G4GDML_FLAG)
     AC_SUBST(G4GDML_LIB_DIR)
     AC_SUBST(G4GDML_LIBS)
   fi
])

