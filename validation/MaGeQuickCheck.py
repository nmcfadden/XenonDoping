#!/usr/bin/python

import sys
import os

print("+++ MaGe QuickCheck has started... +++\n")
print("* executing MaGe and gathering version information...")

os.system("MaGe version.mac > QuickCheck/MaGeInfo.txt")
os.system("env > QuickCheck/MaGeEnv.txt")

os.chdir("QuickCheck/")
os.system("touch quickcheckparameter.txt")
os.system("./MaGeTest.py")
gdml = os.system("./MaGeEnv.py")

print("* simulating radioactive decays...")
os.system("./decaySimulation.py")

if gdml == 0:
    print("* simulating gamma interactions...")
    os.system("./gammaSimulation.py")

    print("* simulating electron interactions...")
    os.system("./electronSimulation.py")

os.chdir("../")

# generating report
print("* generating report...")
os.system("./RGen QuickCheck/quickcheckparameter.txt QuickCheck/template.tex QuickCheckReport.tex 1 > /dev/null")
result =  os.system("pdflatex QuickCheckReport.tex>/dev/null")
os.system("pdflatex QuickCheckReport.tex>/dev/null")

# cleaning up
os.system("rm -f QuickCheckReport.aux")
os.system("rm -f QuickCheckReport.log")
os.system("rm -f QuickCheckReport.out")

# only remove tex and txt files if pdflatex worked:
if result == 0:
    os.system("rm -f QuickCheckReport.tex")
    os.system("rm -f QuickCheck/MaGeInfo.txt")
    os.system("rm -f QuickCheck/MaGeEnv.txt")
    os.system("rm -f QuickCheck/quickcheckparameter.txt")

print("\n+++ MaGe QuickCheck has finished. See QuickCheckReport.pdf for results. +++\n")
