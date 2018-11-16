######################################
#
# g4system.U
#
#+
G4SYSTEM="Linux-g++"
export G4SYSTEM
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4SYSTEM=$G4SYSTEM"
fi

#
# g4dirs.U
#
#+
if [ X/u/akbar/mj/simu/geant4.7.1 != X/u/akbar/mj/simu/geant4.7.1 ] ; then
G4INSTALL="/u/akbar/mj/simu/geant4.7.1/src/geant4"
else
G4INSTALL="/u/akbar/mj/simu/geant4.7.1"
fi
export G4INSTALL
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4INSTALL=$G4INSTALL"
fi

#+
if [ Xy != Xn ] ; then 
if [ X/u/akbar/mj/simu/geant4.7.1 != X/u/akbar/mj/simu/geant4.7.1 ] ; then
G4INCLUDE="/u/akbar/mj/simu/geant4.7.1/include/geant4"
else
G4INCLUDE="/u/akbar/mj/simu/geant4.7.1/include/"
fi
export G4INCLUDE
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4INCLUDE=$G4INCLUDE"
fi
#else
#G4BASE="/u/akbar/mj/simu/geant4.7.1/include/geant4/source"
#export G4BASE
#if [ X$g4non_display = X ] ; then
##echo "On this machine the G4BASE=$G4BASE"
#fi
fi

#+
if [ X/u/akbar/mj/simu/geant4.7.1/lib != X ] ; then 
if [ X/u/akbar/mj/simu/geant4.7.1 != X/u/akbar/mj/simu/geant4.7.1 ] ; then
G4LIB="/u/akbar/mj/simu/geant4.7.1/lib/geant4"
else
G4LIB="/u/akbar/mj/simu/geant4.7.1/lib"
fi
export G4LIB
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4LIB=$G4LIB"
fi
fi

#+
if [ X/u/akbar/mj/simu/geant4.7.1/data/PhotonEvaporation != X ] ; then 
G4LEVELGAMMADATA="/u/akbar/mj/simu/geant4.7.1/data/PhotonEvaporation"
export G4LEVELGAMMADATA
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4LEVELGAMMADATA=$G4LEVELGAMMADATA"
fi
fi

#+
if [ X/u/akbar/mj/simu/geant4.7.1/data/RadiativeDecay != X ] ; then 
G4RADIOACTIVEDATA="/u/akbar/mj/simu/geant4.7.1/data/RadiativeDecay"
export G4RADIOACTIVEDATA
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4RADIOACTIVEDATA=$G4RADIOACTIVEDATA"
fi
fi

#+
if [ X/u/akbar/mj/simu/geant4.7.1/data/G4EMLOW3.0 != X ] ; then 
G4LEDATA="/u/akbar/mj/simu/geant4.7.1/data/G4EMLOW3.0"
export G4LEDATA
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4LEDATA=$G4LEDATA"
fi
fi

#+
if [ X/u/akbar/mj/simu/geant4.7.1/data/G4NDL3.7 != X ] ; then 
NeutronHPCrossSections="/u/akbar/mj/simu/geant4.7.1/data/G4NDL3.7"
export NeutronHPCrossSections
if [ X$g4non_display = X ] ; then
#echo "On this machine the NeutronHPCrossSections=$NeutronHPCrossSections"
fi
fi

#+
if [ X/u/akbar/mj/simu/geant4.7.1/data/G4ELASTIC1.1 != X ] ; then 
G4ELASTICDATA="/u/akbar/mj/simu/geant4.7.1/data/G4ELASTIC1.1"
export G4ELASTICDATA
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4ELASTICDATA=$G4ELASTICDATA"
fi
fi

#
# g4clhep.U
#
if [ X/auto/common/majorana/simu/CLHEP/1.9.2.1/ != X ] ; then 
CLHEP_BASE_DIR="/auto/common/majorana/simu/CLHEP/1.9.2.1"
export CLHEP_BASE_DIR
if [ X$g4non_display = X ] ; then
#echo "On this machine the CLHEP_BASE_DIR=$CLHEP_BASE_DIR"
fi
fi

#+
if [ X/auto/common/majorana/simu/CLHEP/1.9.2.1//include != X ] ; then 
CLHEP_INCLUDE_DIR="/auto/common/majorana/simu/CLHEP/1.9.2.1/include"
export CLHEP_INCLUDE_DIR
if [ X$g4non_display = X ] ; then
#echo "On this machine the CLHEP_INCLUDE_DIR=$CLHEP_INCLUDE_DIR"
fi
fi

#+
if [ X/auto/common/majorana/simu/CLHEP/1.9.2.1//lib != X ] ; then 
CLHEP_LIB_DIR="/auto/common/majorana/simu/CLHEP/1.9.2.1/lib"
export CLHEP_LIB_DIR
if [ X$g4non_display = X ] ; then
#echo "On this machine the CLHEP_LIB_DIR=$CLHEP_LIB_DIR"
fi
fi

#+
if [ XCLHEP != X ] ; then 
CLHEP_LIB="CLHEP"
export CLHEP_LIB
if [ X$g4non_display = X ] ; then
#echo "On this machine the CLHEP_LIB=$CLHEP_LIB"
fi
fi

#+
#
# g4debug
#
if [ X != X ] ; then 
G4DEBUG=""
export G4DEBUG
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4DEBUG=$G4DEBUG"
fi
fi

#
# g4analysis
#
#+
if [ Xy = Xy ] ; then
G4ANALYSIS_USE=1
export G4ANALYSIS_USE
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4ANALYSIS_USE=$G4ANALYSIS_USE"
fi
fi 

#
# g4ui
#
#+
if [ Xn = Xy ] ; then
G4UI_BUILD_XAW_SESSION=1
export G4UI_BUILD_XAW_SESSION
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4UI_BUILD_XAW_SESSION=$G4UI_BUILD_XAW_SESSION"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4UI_USE_XAW=1
export G4UI_USE_XAW
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4UI_USE_XAW=$G4UI_USE_XAW"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4UI_BUILD_XM_SESSION=1
export G4UI_BUILD_XM_SESSION
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4UI_BUILD_XM_SESSION=$G4UI_BUILD_XM_SESSION"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4UI_USE_XM=1
export G4UI_USE_XM
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4UI_USE_XM=$G4UI_USE_XM"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4UI_BUILD_WIN32_SESSION=1
export G4UI_BUILD_WIN32_SESSION
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4UI_BUILD_WIN32_SESSION=$G4UI_BUILD_WIN32_SESSION"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4UI_USE_WIN32=1
export G4UI_USE_WIN32
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4UI_USE_WIN32=$G4UI_USE_WIN32"
fi
fi 

#
# g4vis
#
#+
if [ Xy = Xy ] ; then
G4VIS_BUILD_DAWN_DRIVER=1
export G4VIS_BUILD_DAWN_DRIVER
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_BUILD_DAWN_DRIVER=$G4VIS_BUILD_DAWN_DRIVER"
fi
fi 

#+
if [ Xy = Xy ] ; then
G4VIS_BUILD_OPENGLX_DRIVER=1
export G4VIS_BUILD_OPENGLX_DRIVER
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_BUILD_OPENGLX_DRIVER=$G4VIS_BUILD_OPENGLX_DRIVER"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4VIS_BUILD_OPENGLXM_DRIVER=1
export G4VIS_BUILD_OPENGLXM_DRIVER
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_BUILD_OPENGLXM_DRIVER=$G4VIS_BUILD_OPENGLXM_DRIVER"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4VIS_BUILD_OPENGLWIN32_DRIVER=1
export G4VIS_BUILD_OPENGLWIN32_DRIVER
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_BUILD_OPENGLWIN32_DRIVER=$G4VIS_BUILD_OPENGLWIN32_DRIVER"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4VIS_BUILD_OIX_DRIVER=1
export G4VIS_BUILD_OIX_DRIVER
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_BUILD_OIX_DRIVER=$G4VIS_BUILD_OIX_DRIVER"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4VIS_BUILD_OIWIN32_DRIVER=1
export G4VIS_BUILD_OIWIN32_DRIVER
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_BUILD_OIWIN32_DRIVER=$G4VIS_BUILD_OIWIN32_DRIVER"
fi
fi 

#+
if [ Xy = Xy ] ; then
G4VIS_BUILD_VRML_DRIVER=1
export G4VIS_BUILD_VRML_DRIVER
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_BUILD_VRML_DRIVER=$G4VIS_BUILD_VRML_DRIVER"
fi
fi 

#+
if [ Xy = Xy ] ; then
G4VIS_USE_DAWN=1
export G4VIS_USE_DAWN
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_USE_DAWN=$G4VIS_USE_DAWN"
fi
fi 

#+
if [ Xy = Xy ] ; then
G4VIS_USE_OPENGLX=1
export G4VIS_USE_OPENGLX
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_USE_OPENGLX=$G4VIS_USE_OPENGLX"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4VIS_USE_OPENGLXM=1
export G4VIS_USE_OPENGLXM
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_USE_OPENGLXM=$G4VIS_USE_OPENGLXM"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4VIS_USE_OPENGLWIN32=1
export G4VIS_USE_OPENGLWIN32
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_USE_OPENGLWIN32=$G4VIS_USE_OPENGLWIN32"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4VIS_USE_OIX=1
export G4VIS_USE_OIX
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_USE_OIX=$G4VIS_USE_OIX"
fi
fi 

#+
if [ Xn = Xy ] ; then
G4VIS_USE_OIWIN32=1
export G4VIS_USE_OIWIN32
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_USE_OIWIN32=$G4VIS_USE_OIWIN32"
fi
fi 

#+
if [ Xy = Xy ] ; then
G4VIS_USE_VRML=1
export G4VIS_USE_VRML
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4VIS_USE_VRML=$G4VIS_USE_VRML"
fi
fi 

#+
if [ X != X ] ; then
OGLHOME=""
export OGLHOME
if [ X$g4non_display = X ] ; then
#echo "On this machine the OGLHOME=$OGLHOME"
fi
fi 

#
# Use G3TOG4 module
#
#+
if [ Xy = Xy ] ; then
G4LIB_BUILD_G3TOG4=1
export G4LIB_BUILD_G3TOG4
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4LIB_BUILD_G3TOG4=$G4LIB_BUILD_G3TOG4"
fi
fi 

if [ Xy = Xy ] ; then
G4LIB_USE_G3TOG4=1
export G4LIB_USE_G3TOG4
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4LIB_USE_G3TOG4=$G4LIB_USE_G3TOG4"
fi
fi 

#
# Use ZLIB module
#
#+
if [ Xn = Xy ] ; then
G4LIB_BUILD_ZLIB=1
export G4LIB_BUILD_ZLIB
#echo "On this machine the G4LIB_BUILD_ZLIB=$G4LIB_BUILD_ZLIB"
fi 

if [ Xn = Xy ] ; then
G4LIB_USE_ZLIB=1
export G4LIB_USE_ZLIB
#echo "On this machine the G4LIB_USE_ZLIB=$G4LIB_USE_ZLIB"
fi 


#+
#
# g4shared
#
if [ Xn = Xy ] ; then
G4LIB_BUILD_SHARED=1
export G4LIB_BUILD_SHARED
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4LIB_BUILD_SHARED=$G4LIB_BUILD_SHARED"
fi
fi 

if [ Xy = Xy ] ; then
G4LIB_BUILD_STATIC=1
export G4LIB_BUILD_STATIC
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4LIB_BUILD_STATIC=$G4LIB_BUILD_STATIC"
fi
fi 

#+
#
# g4granular
#
if [ Xy = Xy ] ; then
G4LIB_USE_GRANULAR=1
export G4LIB_USE_GRANULAR
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4LIB_USE_GRANULAR=$G4LIB_USE_GRANULAR"
fi
fi 

# Check for Windows!
if test "XLinux-g++" != "XWIN32-VC" -a "XLinux-g++" != "XWIN32-VC7"; then
#+
#
# UI_USE_TCSH for Unix
#
G4UI_USE_TCSH=1
export G4UI_USE_TCSH
if [ X$g4non_display = X ] ; then
#echo "On this machine the G4UI_USE_TCSH=$G4UI_USE_TCSH"
fi 
fi

#####################################################################


#+
#
# G4WORKDIR
#
if [ $G4WORKDIR ] ; then

if [ X$g4non_display = X ] ; then
#echo "In your environment you have the G4WORKDIR=$G4WORKDIR"
fi # if non_display

else

if [ X$g4non_display = X ] ; then

# Check for Windows!
if test "X$G4SYSTEM" = "XWIN32-VC" -o "X$G4SYSTEM" = "XWIN32-VC7"; then

#echo "G4WORKDIR will be set to c:/geant4 (in "native" Windows syntax)"
G4WORKDIR="c:/geant4"
export G4WORKDIR

else # if Unix

#echo "G4WORKDIR will be set to HOME/geant4=$HOME/geant4"
export G4WORKDIR=$HOME/geant4

fi # if platforms

fi

fi

