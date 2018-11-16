setenv G4WORKDIR /mnt/mjdDisk1/Majorana/users/nmcfadden/gitBACoN/XenonDoping
# the following line is for G4 version >= 9.6
source /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/geant4make.csh /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make 
# uncomment the following two lines for G4 version < 9.5
# source /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/env.csh 
# uncomment the following two lines for G4 version = 9.5
# source /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/../../../bin/geant4.csh 
# source /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/geant4make.csh 
# add G4WORKDIR/bin/G4SYSTEM to the front of the path, because stupid G4 adds it
# to the end, but we want to override the old setting if another G4WORKDIR was
# being used before MaGe/setup.(c)sh was called
setenv PATH $G4WORKDIR/bin/${G4SYSTEM}:$PATH
# G4 doesn't add CLHEP/bin to path; do it for access to clhep-config
setenv PATH $CLHEP_BASE_DIR/bin:$PATH

if (-e /mnt/mjdDisk1/Majorana/soft/root-6.12.06-build/bin/thisroot.csh) then
  source /mnt/mjdDisk1/Majorana/soft/root-6.12.06-build/bin/thisroot.csh /mnt/mjdDisk1/Majorana/soft/root-6.12.06-build/bin
else 
  setenv ROOTSYS /mnt/mjdDisk1/Majorana/soft/root-6.12.06-build
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


setenv MGDODIR /mnt/mjdDisk1/Majorana/soft/MGDO
setenv PATH $MGDODIR/bin:$PATH
if ($G4SYSTEM == "Darwin-g++") then
  setenv DYLD_LIBRARY_PATH $MGDODIR/lib:$DYLD_LIBRARY_PATH
else if ($G4SYSTEM == "Darwin-clang") then
  setenv DYLD_LIBRARY_PATH $MGDODIR/lib:$DYLD_LIBRARY_PATH
else 
  setenv LD_LIBRARY_PATH $MGDODIR/lib:$LD_LIBRARY_PATH
endif

setenv MAGEDIR /mnt/mjdDisk1/Majorana/users/nmcfadden/gitBACoN/XenonDoping
setenv MGGENERATORDATA $MAGEDIR/generators/data
