######################################
#
# g4system.U
#
#+
setenv G4SYSTEM "Linux-g++"
#echo "On this machine the G4SYSTEM=$G4SYSTEM"

#
# g4dirs.U
#
#+
if ( X/auto/common/majorana/simu/geant4.7.1.p01 != X/auto/common/majorana/simu/geant4.7.1.p01 ) then
setenv G4INSTALL "/auto/common/majorana/simu/geant4.7.1.p01/src/geant4"
#echo "On this machine the G4INSTALL=$G4INSTALL"
else
setenv G4INSTALL "/auto/common/majorana/simu/geant4.7.1.p01"
#echo "On this machine the G4INSTALL=$G4INSTALL"
endif

#+
if ( Xy != Xn ) then
if ( X/auto/common/majorana/simu/geant4.7.1.p01 != X/auto/common/majorana/simu/geant4.7.1.p01 ) then
setenv G4INCLUDE "/auto/common/majorana/simu/geant4.7.1.p01/include/geant4"
else
setenv G4INCLUDE "/auto/common/majorana/simu/geant4.7.1.p01/include"
endif
#echo "On this machine the G4INCLUDE=$G4INCLUDE"
#else
#setenv G4BASE "/auto/common/majorana/simu/geant4.7.1.p01/include/geant4/source"
##echo "On this machine the G4BASE=$G4BASE"
endif

#+
if ( X/u/akbar/mj/simu/geant4.7.1.p01/lib != X ) then
if ( X/auto/common/majorana/simu/geant4.7.1.p01 != X/auto/common/majorana/simu/geant4.7.1.p01 ) then
setenv G4LIB "/auto/common/majorana/simu/geant4.7.1.p01/lib/geant4"
else
setenv G4LIB "/auto/common/majorana/simu/geant4.7.1.p01/lib"
endif
#echo "On this machine the G4LIB=$G4LIB"
endif

#+
if ( X/auto/common/majorana/simu/geant4.7.1.p01/data/PhotonEvaporation != X ) then
setenv G4LEVELGAMMADATA "/auto/common/majorana/simu/geant4.7.1.p01/data/PhotonEvaporation"
#echo "On this machine the G4LEVELGAMMADATA=$G4LEVELGAMMADATA"
endif

#+
if ( X/auto/common/majorana/simu/geant4.7.1.p01/data/RadiativeDecay != X ) then
setenv G4RADIOACTIVEDATA "/auto/common/majorana/simu/geant4.7.1.p01/data/RadiativeDecay"
#echo "On this machine the G4RADIOACTIVEDATA=$G4RADIOACTIVEDATA"
endif

#+
if ( X/auto/common/majorana/simu/geant4.7.1.p01/data/G4EMLOW3.0 != X ) then
setenv G4LEDATA "/auto/common/majorana/simu/geant4.7.1.p01/data/G4EMLOW3.0"
#echo "On this machine the G4LEDATA=$G4LEDATA"
endif

#+
if ( X/auto/common/majorana/simu/geant4.7.1.p01/data/G4NDL3.7 != X ) then
setenv NeutronHPCrossSections "/auto/common/majorana/simu/geant4.7.1.p01/data/G4NDL3.7"
#echo "On this machine the NeutronHPCrossSections=$NeutronHPCrossSections"
endif

#+
if ( X/auto/common/majorana/simu/geant4.7.1.p01/data/G4ELASTIC1.1 != X ) then
setenv G4ELASTICDATA "/auto/common/majorana/simu/geant4.7.1.p01/data/G4ELASTIC1.1"
#echo "On this machine the G4ELASTICDATA=$G4ELASTICDATA"
endif

#
# g4clhep.U
#
if ( X/auto/common/majorana/simu/CLHEP != X ) then
setenv CLHEP_BASE_DIR "/auto/common/majorana/simu/CLHEP"
#echo "On this machine the CLHEP_BASE_DIR=$CLHEP_BASE_DIR"
endif

#+
if ( X/auto/common/majorana/simu/CLHEP/include != X ) then
setenv CLHEP_INCLUDE_DIR "/auto/common/majorana/simu/CLHEP/include"
#echo "On this machine the CLHEP_INCLUDE_DIR=$CLHEP_INCLUDE_DIR"
endif

#+
if ( X/auto/common/majorana/simu/CLHEP/lib != X ) then
setenv CLHEP_LIB_DIR "/auto/common/majorana/simu/CLHEP/lib"
#echo "On this machine the CLHEP_LIB_DIR=$CLHEP_LIB_DIR"
endif

#+
if ( XCLHEP != X ) then
setenv CLHEP_LIB "CLHEP"
#echo "On this machine the CLHEP_LIB=$CLHEP_LIB"
endif

#+
#
# g4debug
#
if ( X != X ) then
setenv G4DEBUG ""
#echo "On this machine the G4DEBUG=$G4DEBUG"
endif

#
# g4analysis
#
#+
if ( Xy == Xy ) then
setenv G4ANALYSIS_USE 1
#echo "On this machine the G4ANALYSIS_USE=$G4ANALYSIS_USE"
endif

#
# g4ui
#
#+
if ( Xn == Xy ) then
setenv G4UI_BUILD_XAW_SESSION 1
#echo "On this machine the G4UI_BUILD_XAW_SESSION=$G4UI_BUILD_XAW_SESSION"
endif

#+
if ( Xn == Xy ) then
setenv G4UI_USE_XAW 1
#echo "On this machine the G4UI_USE_XAW=$G4UI_USE_XAW"
endif

#+
if ( Xn == Xy ) then
setenv G4UI_BUILD_XM_SESSION 1
#echo "On this machine the G4UI_BUILD_XM_SESSION=$G4UI_BUILD_XM_SESSION"
endif

#+
if ( Xn == Xy ) then
setenv G4UI_USE_XM 1
#echo "On this machine the G4UI_USE_XM=$G4UI_USE_XM"
endif

#+
if ( Xn == Xy ) then
setenv G4UI_BUILD_WIN32_SESSION 1
#echo "On this machine the G4UI_BUILD_WIN32_SESSION=$G4UI_BUILD_WIN32_SESSION"
endif

#+
if ( Xn == Xy ) then
setenv G4UI_USE_WIN32 1
#echo "On this machine the G4UI_USE_WIN32=$G4UI_USE_WIN32"
endif

#
# g4vis
#
#+
if ( Xy == Xy ) then
setenv G4VIS_BUILD_DAWN_DRIVER 1
#echo "On this machine the G4VIS_BUILD_DAWN_DRIVER=$G4VIS_BUILD_DAWN_DRIVER"
endif

#+
if ( Xy == Xy ) then
setenv G4VIS_BUILD_OPENGLX_DRIVER 1
#echo "On this machine the G4VIS_BUILD_OPENGLX_DRIVER=$G4VIS_BUILD_OPENGLX_DRIVER"
endif

#+
if ( Xn == Xy ) then
setenv G4VIS_BUILD_OPENGLXM_DRIVER 1
#echo "On this machine the G4VIS_BUILD_OPENGLXM_DRIVER=$G4VIS_BUILD_OPENGLXM_DRIVER"
endif

#+
if ( Xn == Xy ) then
setenv G4VIS_BUILD_OPENGLWIN32_DRIVER 1
#echo "On this machine the G4VIS_BUILD_OPENGLWIN32_DRIVER=$G4VIS_BUILD_OPENGLWIN32_DRIVER"
endif

#+
if ( Xn == Xy ) then
setenv G4VIS_BUILD_OIX_DRIVER 1
#echo "On this machine the G4VIS_BUILD_OIX_DRIVER=$G4VIS_BUILD_OIX_DRIVER"
endif

#+
if ( Xn == Xy ) then
setenv G4VIS_BUILD_OIWIN32_DRIVER 1
#echo "On this machine the G4VIS_BUILD_OIWIN32_DRIVER=$G4VIS_BUILD_OIWIN32_DRIVER"
endif

#+
if ( Xy == Xy ) then
setenv G4VIS_BUILD_VRML_DRIVER 1
#echo "On this machine the G4VIS_BUILD_VRML_DRIVER=$G4VIS_BUILD_VRML_DRIVER"
endif

#+
if ( Xy == Xy ) then
setenv G4VIS_USE_DAWN 1
#echo "On this machine the G4VIS_USE_DAWN=$G4VIS_USE_DAWN"
endif

#+
if ( Xy == Xy ) then
setenv G4VIS_USE_OPENGLX 1
#echo "On this machine the G4VIS_USE_OPENGLX=$G4VIS_USE_OPENGLX"
endif

#+
if ( Xn == Xy ) then
setenv G4VIS_USE_OPENGLXM 1
#echo "On this machine the G4VIS_USE_OPENGLXM=$G4VIS_USE_OPENGLXM"
endif

#+
if ( Xn == Xy ) then
setenv G4VIS_USE_OPENGLWIN32 1
#echo "On this machine the G4VIS_USE_OPENGLWIN32=$G4VIS_USE_OPENGLWIN32"
endif

#+
if ( Xn == Xy ) then
setenv G4VIS_USE_OIX 1
#echo "On this machine the G4VIS_USE_OIX=$G4VIS_USE_OIX"
endif

#+
if ( Xn == Xy ) then
setenv G4VIS_USE_OIWIN32 1
#echo "On this machine the G4VIS_USE_OIWIN32=$G4VIS_USE_OIWIN32"
endif

#+
if ( Xy == Xy ) then
setenv G4VIS_USE_VRML 1
#echo "On this machine the G4VIS_USE_VRML=$G4VIS_USE_VRML"
endif

#+
if ( X != X )  then
setenv OGLHOME ""
#echo "On this machine the OGLHOME=$OGLHOME"
endif 

#
# Use G3TOG4 module
#
#+
if ( Xy == Xy )  then
setenv G4LIB_BUILD_G3TOG4 1
#echo "On this machine the G4LIB_BUILD_G3TOG4=$G4LIB_BUILD_G3TOG4"
endif 

if ( Xy == Xy )  then
setenv G4LIB_USE_G3TOG4 1
#echo "On this machine the G4LIB_USE_G3TOG4=$G4LIB_USE_G3TOG4"
endif 


#
# Use ZLIB module
#
#+
if ( Xn == Xy )  then
setenv G4LIB_BUILD_ZLIB 1
#echo "On this machine the G4LIB_BUILD_ZLIB=$G4LIB_BUILD_ZLIB"
endif 

if ( X == Xy )  then
setenv G4LIB_USE_ZLIB 1
#echo "On this machine the G4LIB_USE_ZLIB=$G4LIB_USE_ZLIB"
endif 

#+
#
# g4shared
#
if ( Xn == Xy ) then
setenv G4LIB_BUILD_SHARED 1
#echo "On this machine the G4LIB_BUILD_SHARED=$G4LIB_BUILD_SHARED"
endif

if ( Xy == Xy ) then
setenv G4LIB_BUILD_STATIC 1
#echo "On this machine the G4LIB_BUILD_STATIC=$G4LIB_BUILD_STATIC"
endif

#+
#
# g4granular
#
if ( Xy == Xy ) then
setenv G4LIB_USE_GRANULAR 1
#echo "On this machine the G4LIB_USE_GRANULAR=$G4LIB_USE_GRANULAR"
endif

# Check for Windows!
if ( "XLinux-g++" != "XWIN32-VC" && "XLinux-g++" != "XWIN32-VC7" ) then
setenv G4UI_USE_TCSH 1
#echo "On this machine the G4UI_USE_TCSH=$G4UI_USE_TCSH"
endif

#####################################################################


#+
#
# G4WORKDIR
#
if ( ${?G4WORKDIR} ) then
#echo "In your environment you have the G4WORKDIR=$G4WORKDIR"
else
# Check for Windows!
if ( "X$G4SYSTEM" == "XWIN32-VC" || "X$G4SYSTEM" == "XWIN32-VC7" ) then

#echo "G4WORKDIR will be set to c:/geant4 (in "native" Windows syntax)."
setenv G4WORKDIR "c:/geant4"

else # if Unix

#echo "G4WORKDIR will be set to HOME/geant4=$HOME/geant4"
setenv G4WORKDIR $HOME/geant4

endif
endif

