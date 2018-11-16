#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

#include "TRandom3.h"
#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraph2DErrors.h"
#include "TMath.h"

#include "GETSiegfriedIICrystalData.hh"
#include "GETSiegfriedIData.hh"
#include "MGCrystalData.hh"

#include "io/MGLogger.hh"
#include "waveform/MGSORFieldCalculation.hh"
#include "waveform/MGSORCrystalFields.hh"

using namespace std;
using namespace CLHEP;

int main()
{
   MGLogger::SetSeverity(MGLogger::routine);
   //MGLogger::SetSeverity(MGLogger::debugging);

   //setup a detector using MGcrystalData
   MGCrystalData* fCrystalData = new MGCrystalData(0,
         MGCrystalData::kNType,
         MGCrystalData::kCoaxial,
         5*mm,
         37.5*mm,
         70*mm,
         70*mm,
         0., 0., 0.,
         1, 3);

   //set Impurity Density
   fCrystalData->SetImpurityDensity(0.35e10/cm3,0.55e10/cm3);

   //set operation voltage
   fCrystalData->SetVoltage(2000*volt);

   //or setup a specific detector
   //MGCrystalData* fSiegfriedI  = new GETSiegfriedIData();
   //MGCrystalData* fSiegfriedII = new GETSiegfriedIICrystalData();

   //create the crystalfields
   MGSORCrystalFields* fFields = new MGSORCrystalFields(*fCrystalData);
   //MGSORCrystalFields* fFields = new MGSORCrystalFields(fSiegfriedI);
   //MGSORCrystalFields* fFields = new MGSORCrystalFields(fSiegfriedII);

   // set the grid (r, phi, z) for calculation 
   fFields->SetGrid(33,61,4);

   //set a filename, the suffix .root is added automatically
   fFields->SetFileName("Efield2kV33x61x4"); 

//calculate fields
//================

   //create an instance of the calculation class
   MGSORFieldCalculation* fCalc =  new MGSORFieldCalculation(fFields);

   //when should the algortihm stop, at what level of change of the fields
   //  fCalc->SetConvergenceLimit(1E-4);
   fCalc->SetConvergenceLimit(1E-6);

   //if you set this the Convergence parameter will be written to disk, nice to
   //check if the algorithm did converge
   //fCalc->SaveConvergenceToFile("TESTConvergence");

   //set the SOR Constant. SOR algorithm only converges if SOR const 1<SorConst
   //< 2, you could calculate exact and best value, or you just try some. 1.4
   //seemd to be good  
   fCalc->SetSORConst(1.4);

   // then you need to initialize everything 
   fCalc->Initialize();

   //then do the Calculation!
   fCalc->CalculateFields();

   delete fCalc;

//draw fields
//==============

   fFields->LoadFields();
   fFields->DrawFields();

   delete fFields;
   delete fCrystalData;
}

