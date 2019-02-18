setenv G4WORKDIR /home/nmcfadden/XenonDoping
# the following line is for G4 version >= 9.6
source /usr/local/share/Geant4-10.4.0/geant4make/geant4make.csh /usr/local/share/Geant4-10.4.0/geant4make 
# uncomment the following two lines for G4 version < 9.5
# source /usr/local/share/Geant4-10.4.0/geant4make/env.csh 
# uncomment the following two lines for G4 version = 9.5
# source /usr/local/share/Geant4-10.4.0/geant4make/../../../bin/geant4.csh 
# source /usr/local/share/Geant4-10.4.0/geant4make/geant4make.csh 
# add G4WORKDIR/bin/G4SYSTEM to the front of the path, because stupid G4 adds it
# to the end, but we want to override the old setting if another G4WORKDIR was
# being used before MaGe/setup.(c)sh was called
setenv PATH $G4WORKDIR/bin/${G4SYSTEM}:$PATH
# G4 doesn't add CLHEP/bin to path; do it for access to clhep-config
setenv PATH $CLHEP_BASE_DIR/bin:$PATH

if (-e /usr/local/root/bin/thisroot.csh) then
  source /usr/local/root/bin/thisroot.csh /usr/local/root/bin
else 
  setenv ROOTSYS /usr/local/root
  setenv PATH $ROOTSYS/bin:$PATH
  if ($G4SYSTEM == "Darwin-g++") then
    if ($?DYLD_LIBRARY_PATH) then
      setenv DYLD_LIBRARY_PATH $ROOTSYS/lib$DYLD_LIBRARY_PATH
    else
      setenv DYLD_LIBRARY_PATH $ROOTSYS/lib
    endif
  else 
    if ($?LD_LIBRARY_PATH) then
      setenv LD_LIBRARY_PATH $ROOTSYS/lib:$LD_LIBRARY_PATH
    else
      setenv LD_LIBRARY_PATH $ROOTSYS/lib
    endif
  endif
  if ($?PYTHONPATH) then
    setenv PYTHONPATH $ROOTSYS/lib:$PYTHONPATH
  else
    setenv PYTHONPATH $ROOTSYS/lib
  endif
endif


setenv MGDODIR /home/nmcfadden/MGDO
setenv PATH $MGDODIR/bin:$PATH
if ($G4SYSTEM == "Darwin-g++") then
  setenv DYLD_LIBRARY_PATH $MGDODIR/lib:$DYLD_LIBRARY_PATH
else if ($G4SYSTEM == "Darwin-clang") then
  setenv DYLD_LIBRARY_PATH $MGDODIR/lib:$DYLD_LIBRARY_PATH
else 
  setenv LD_LIBRARY_PATH $MGDODIR/lib:$LD_LIBRARY_PATH
endif

setenv MAGEDIR /home/nmcfadden/XenonDoping
setenv MGGENERATORDATA $MAGEDIR/generators/data
