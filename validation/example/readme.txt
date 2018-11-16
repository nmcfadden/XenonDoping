# example written by Florian Fraenkle ( fraenkle@email.unc.edu )

This example shows how an automatized validation chain can look like. 

1	Requirements
2	Installation
3	G4Demo
4	DemoAnalysis
5	ReportGenerator

1 Requirements
In order to run the example the following software packages need to be installed on your computer:
+ MaGe
+ ROOT
+ LaTeX
+ Python

2 Installation
To install the example just run the script “build.sh”. To run the validation chain, execute the Python script “demo1.py”.

3 G4Demo
G4Demo is an example Geant4 program. It shoots 500 keV electrons on a 200 μm thick silicon detector. The energy deposit of each electron is calculated and written to the file “G4Demo.txt”. The source code of G4Demo is available in “G4Demo.cc” and in the folder “/src”.

4 DemoAnalysis
This program uses the ROOT framework to analyze the data in the file “G4Demo.txt” (see 3). It creates a histogram of the energy deposit and automatically fits a Landau function. The plot is saved in a PNG file (“energydeposit.png”). The fit parameters (and other relevant information) are saved in the file “parameterlist.txt”. The source code of DemoAnalysis is available in “DemoAnalysis.cxx”.

5 ReportGenerator
The ReportGenerator uses a LaTeX template (“template.tex”)  to create a report “report.tex”. Within the template tokens of format XX:name of token:XX are defined. The ReportGenerator replaces the tokens with the values provided in “parameterlist.txt” (see 4). After ReportGenerator has finished, “demo1.py” executes “pdf2latex” in order to create a PDF file (“report.pdf”). The source code of ReportGenerator is available in the folder “/reportgenerator”.
