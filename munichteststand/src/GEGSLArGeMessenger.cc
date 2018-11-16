//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//

/**
 * AUTHOR: Luciano Pandola
 * FIRST SUBMISSION: 05-03-2005
 *
 * REVISION:
 *
 * 01-04-2006, Added commands, Luciano
 * 
 */

// ---------------------------------------------------------------------------

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "munichteststand/GEGSLArGeMessenger.hh"
#include "munichteststand/GEGSLArGe.hh" 
#include "io/MGLogger.hh"

// --------------------------------------------------------------------------- 

GEGSLArGeMessenger::GEGSLArGeMessenger(GEGSLArGe *teststand) : 
  fDetector(teststand) 
{
  // directories 
  fGSLArGeDirectory =  new G4UIdirectory("/MG/geometry/gslarge/"); 
  fGSLArGeDirectory->SetGuidance("Tune teststand parameters for GS LArGe test stand"); 

  fNumCol = new G4UIcmdWithAnInteger("/MG/geometry/gslarge/numcol",this);
  fNumCol->SetGuidance("Set the number of crystal columns (default = 3)");
  fNumCol->AvailableForStates(G4State_PreInit);

  fCryPerCol = new G4UIcmdWithAnInteger("/MG/geometry/gslarge/crypercol",this);
  fCryPerCol->SetGuidance("Set the number of crystal in a column (default = 1)");
  fCryPerCol->AvailableForStates(G4State_PreInit);
  
  fColDistance = new G4UIcmdWithADoubleAndUnit("/MG/geometry/gslarge/coldistance",this);
  fColDistance->SetGuidance("Set the distance between columns (default: 1 cm)");
  fColDistance->AvailableForStates(G4State_PreInit);
  
  fColGap =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gslarge/colgap",this);
  fColGap->SetGuidance("Set distance between crystals in a column (default: 5 cm)");
  fColGap->AvailableForStates(G4State_PreInit);

  fTrueCoax = new G4UIcmdWithABool("/MG/geometry/gslarge/truecoax",this);
  fTrueCoax->SetGuidance("Set true/not true coaxial crystals (default: false)");
  fTrueCoax->AvailableForStates(G4State_PreInit);
  // INIZIO PROVA
  fEncapsulation = new G4UIcmdWithABool("/MG/geometry/gslarge/encaps",this);
  fEncapsulation->SetGuidance("Set/unset crystal encapsulation (default: false)");
  fEncapsulation->AvailableForStates(G4State_PreInit);
  // FINE PROVA
  fNumPhiSegments = new G4UIcmdWithAnInteger("/MG/geometry/gslarge/phisegments",this);
  fNumPhiSegments->SetGuidance("Set the number of segments in phi (default: 6)");
  fNumPhiSegments->SetGuidance("To be used after truecoax is set to true");
  fNumPhiSegments->AvailableForStates(G4State_PreInit);

  fNumZSegments = new G4UIcmdWithAnInteger("/MG/geometry/gslarge/zsegments",this);
  fNumZSegments->SetGuidance("Set the number of segments in z (default: 3)");
  fNumZSegments->SetGuidance("To be used after truecoax is set to true");
  fNumZSegments->AvailableForStates(G4State_PreInit);
}


// --------------------------------------------------------------------------- 

GEGSLArGeMessenger::~GEGSLArGeMessenger()
{
  delete fGSLArGeDirectory; 
  delete fNumCol;
  delete fCryPerCol;
  delete fColDistance;
  delete fColGap;
  delete fTrueCoax;
  delete fEncapsulation;
  delete fNumPhiSegments;
  delete fNumZSegments;
}

// --------------------------------------------------------------------------- 

void GEGSLArGeMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
{
  if (cmd == fNumCol)
    fDetector->SetNumberOfColumns(fNumCol->GetNewIntValue(newvalue));
  else if (cmd == fCryPerCol)
    fDetector->SetCrystalsPerColumn(fCryPerCol->GetNewIntValue(newvalue));
  else if (cmd == fColDistance)
    fDetector->SetColumnDistance(fColDistance->GetNewDoubleValue(newvalue));
  else if (cmd == fColGap)
    fDetector->SetCrystalColumnGap(fColGap->GetNewDoubleValue(newvalue));
  else if (cmd == fTrueCoax)
    fDetector->SetFlagCrystalTrueCoaxial(fTrueCoax->GetNewBoolValue(newvalue));
  else if (cmd == fEncapsulation)
    fDetector->SetFlagCrystalEncapsulation(fEncapsulation->GetNewBoolValue(newvalue));
  else if (cmd == fNumPhiSegments)
    {
      if (fDetector->GetFlagCrystalTrueCoaxial())
	{
	  fDetector->SetPhiSegmentsPerCrystal(fNumPhiSegments->GetNewIntValue(newvalue));
	}
      else
	{
	  MGLog(warning) << "Please set /MG/geometry/gslarge/truecoax to true first" << endlog;
	  MGLog(warning) << "Nothing happens" << endlog;
	}
    }
  else if (cmd == fNumZSegments)
    {
      if (fDetector->GetFlagCrystalTrueCoaxial())
	{
	  fDetector->SetZSegmentsPerCrystal(fNumZSegments->GetNewIntValue(newvalue));
	}
      else
	{
	  MGLog(warning) << "Please set /MG/geometry/gslarge/truecoax to true first" << endlog;
	  MGLog(warning) << "Nothing happens" << endlog;
	}
    }

}

// --------------------------------------------------------------------------- 
