#! /bin/bash

# compiling G4 example simulation
make -f GNUmakefile
mv -v $G4WORKDIR/bin/$G4SYSTEM/G4Demo ./

# compiling analysis tool
make -f Makefile

# compiling PDF report generator
cd reportgenerator
make -f makefile
mv -v ReportGen ../
cd ..

echo type ./demo1.py to execute validation demo
