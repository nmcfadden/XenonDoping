#!/usr/bin/env python

import sys
import os

print("+++ MaGe Validation has started... +++\n")

os.system("rm -f globalparameter.txt")
os.system("rm -f report.tex")
os.system("rm -f RadioactiveDecays/RDcomp_new.root")

###get version information
os.system("touch globalparameter.txt")
os.system("MaGe version.mac > MaGeInfo.txt")
os.system("./MaGeInfo.py")

###execute radioactive decay diagnostics
os.chdir("RadioactiveDecays/")
os.system("./RadioactiveDecayConfiguration.py")
os.system("./RadioactiveDecayValidation.py")
os.chdir("../")

###execute gamma interaction diagnostics
os.chdir("GammaInteractions/")
os.system("./GammaInteractionConfiguration.py")
os.system("./GammaInteractionValidation.py")
os.chdir("../")

###execute electron interaction diagnostics
os.chdir("ElectronInteractions/")
os.system("./ElectronInteractionConfiguration.py")
os.system("./ElectronInteractionValidation.py")
os.chdir("../")

###execute alpha interaction diagnostics
os.chdir("AlphaInteractions/")
os.system("./AlphaInteractionConfiguration.py")
os.system("./AlphaInteractionValidation.py")
os.chdir("../")

###execute neutron interaction diagnostics
#os.chdir("NeutronInteractions/")
#os.system("./NeutronInteractionConfiguration.py")
#os.system("./NeutronInteractionValidation.py")
#os.chdir("../")

###execute CERN neutron analysis
#os.chdir("CERN_NA55/")
#os.system("./doCERNanalysis.py")
#os.chdir("../")

print("+++ executing \"ReportGenerator\" +++\n")
os.system("./RGen globalparameter.txt template.tex report.tex")

#print("+++ creating PDF file +++\n")
#os.system("pdflatex report.tex>/dev/null")
#os.system("bibtex report.aux>/dev/null")
#os.system("pdflatex report.tex>/dev/null")
#os.system("pdflatex report.tex>/dev/null")

###clean up temporary files
os.system("rm -f report.aux")
os.system("rm -f report.log")
os.system("rm -f report.out")
os.system("rm -f report.toc")
os.system("rm -f report.bbl")
os.system("rm -f report.blg")
os.system("rm -f MaGeInfo.txt")

print("+++ MaGe Validation has finished... +++\n")
#print("open report.pdf to see results\n")
