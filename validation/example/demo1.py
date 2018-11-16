#!/usr/bin/python

import sys
import os

print("+++ DEMO1 has started... +++\n")
print("+++ executing \"G4Demo\" simulation +++")
os.system("./G4Demo")

print("+++ executing \"DemoAnalysis\" +++\n")
os.system("./DemoAnalysis")

print("+++ executing \"ReportGenerator\" +++\n")
os.system("./ReportGen")

print("+++ creating PDF file +++\n")
os.system("pdflatex report.tex>/dev/null")
os.system("pdflatex report.tex>/dev/null")

print("+++ DEMO1 has finished... +++\n")
print("open report.pdf to see results\n")
