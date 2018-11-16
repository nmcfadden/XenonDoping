#! /bin/bash

export G4ANALYSIS_USE_ROOT=1

# make sure that all python scripts are executable
chmod +x *.py

# compile radioactive decay simulation and analysis modules
cd RadioactiveDecays
make -f GNUmakefile
make -f Makefile
chmod +x *.py
cd ..

# compile electron interaction simulation and analysis modules
cd ElectronInteractions
make -f Makefile
chmod +x *.py
cd macros
chmod +x *.py
cd ..
cd ..

# compile alpha interaction simulation and analysis modules
cd AlphaInteractions
make -f Makefile
chmod +x *.py
cd macros
chmod +x *.py
cd ..
cd ..

# compile gamma interaction simulation and analysis modules
cd GammaInteractions
make -f Makefile
chmod +x *.py
cd macros
chmod +x *.py
cd ..
cd ..

# compile neutron interaction simulation and analysis modules
cd NeutronInteractions
make -f Makefile
chmod +x *.py
cd macros
chmod +x *.py
cd ..
cd ..

# compile CERN neutron analysis module
cd CERN_NA55
make -f Makefile
chmod +x *.py
cd ..

# prepare MaGe QuickCheck
cd QuickCheck
chmod +x *.py
cd ..

# compile report generator
cd reportgenerator
make -f makefile
mv -v RGen ../
cd ..

echo type ./MaGeValidation.py to execute validation
