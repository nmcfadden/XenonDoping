export G4WORKDIR=/mnt/mjdDisk1/Majorana/users/nmcfadden/gitBACoN/XenonDoping
# the following line is for G4 version >= 9.6
source /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/geant4make.sh 
# /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make 
# 
# uncomment the following two lines for G4 version < 9.5
# source /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/env.sh 
# uncomment the following two lines for G4 version = 9.5
# source /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/../../../bin/geant4.sh 
# source /mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/share/Geant4-10.4.2/geant4make/geant4make.sh 
# add G4WORKDIR/bin/G4SYSTEM to the front of the path, because stupid G4 adds it
# to the end, but we want to override the old setting if another G4WORKDIR was
# being used before MaGe/setup.(c)sh was called
export PATH=$G4WORKDIR/bin/${G4SYSTEM}:$PATH
# G4 doesn't add CLHEP/bin to path; do it for access to clhep-config
export PATH=$CLHEP_BASE_DIR/bin:$PATH

if [ -e /mnt/mjdDisk1/Majorana/soft/root-6.12.06-build/bin/thisroot.sh ] ; then
  source /mnt/mjdDisk1/Majorana/soft/root-6.12.06-build/bin/thisroot.sh 
else 
  export ROOTSYS=/mnt/mjdDisk1/Majorana/soft/root-6.12.06-build
  export PATH=$ROOTSYS/bin:$PATH
  if [ $G4SYSTEM = "Darwin-g++" ] ; then
    if [ -n "$DYLD_LIBRARY_PATH" ] ; then
      export DYLD_LIBRARY_PATH=$ROOTSYS/lib:$DYLD_LIBRARY_PATH
    else
      export DYLD_LIBRARY_PATH=$ROOTSYS/lib
    fi
  else 
    if [ -n "$LD_LIBRARY_PATH" ] ; then
      export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
    else
      export LD_LIBRARY_PATH=$ROOTSYS/lib
    fi
  fi
fi


export MGDODIR=/mnt/mjdDisk1/Majorana/soft/MGDO
export PATH=$MGDODIR/bin:$PATH
if [ $G4SYSTEM = "Darwin-g++" ] ; then
  export DYLD_LIBRARY_PATH=$MGDODIR/lib:$DYLD_LIBRARY_PATH
elif [ $G4SYSTEM = "Darwin-clang" ] ; then
  export DYLD_LIBRARY_PATH=$MGDODIR/lib:$DYLD_LIBRARY_PATH
else 
  export LD_LIBRARY_PATH=$MGDODIR/lib:$LD_LIBRARY_PATH
fi

export MAGEDIR=/mnt/mjdDisk1/Majorana/users/nmcfadden/gitBACoN/XenonDoping
export MGGENERATORDATA=$MAGEDIR/generators/data

