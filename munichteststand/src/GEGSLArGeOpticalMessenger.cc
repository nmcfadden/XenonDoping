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
#include "G4UIcmdWithAString.hh"

#include "munichteststand/GEGSLArGeOpticalMessenger.hh"
#include "munichteststand/GEGSLArGeOptical.hh"
#include "io/MGLogger.hh"

// --------------------------------------------------------------------------- 

GEGSLArGeOpticalMessenger::GEGSLArGeOpticalMessenger(GEGSLArGeOptical *teststand) :
  fDetector(teststand) 
{
  // directories 
  fGSLArGeDirectory =  new G4UIdirectory("/MG/geometry/gslargeoptical/");
  fGSLArGeDirectory->SetGuidance("Tune teststand parameters for GS LArGe test stand"); 

  fNumCol = new G4UIcmdWithAnInteger("/MG/geometry/gslargeoptical/numcol",this);
  fNumCol->SetGuidance("Set the number of crystal columns (default = 3)");
  fNumCol->AvailableForStates(G4State_PreInit);

  fCryPerCol = new G4UIcmdWithAnInteger("/MG/geometry/gslargeoptical/crypercol",this);
  fCryPerCol->SetGuidance("Set the number of crystal in a column (default = 1)");
  fCryPerCol->AvailableForStates(G4State_PreInit);
  
  fColDistance = new G4UIcmdWithADoubleAndUnit("/MG/geometry/gslargeoptical/coldistance",this);
  fColDistance->SetGuidance("Set the distance between columns (default: 1 cm)");
  fColDistance->AvailableForStates(G4State_PreInit);
  
  fColGap =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gslargeoptical/colgap",this);
  fColGap->SetGuidance("Set distance between crystals in a column (default: 5 cm)");
  fColGap->AvailableForStates(G4State_PreInit);

  fTrueCoax = new G4UIcmdWithABool("/MG/geometry/gslargeoptical/truecoax",this);
  fTrueCoax->SetGuidance("Set true/not true coaxial crystals (default: false)");
  fTrueCoax->AvailableForStates(G4State_PreInit);
  // INIZIO PROVA
  fEncapsulation = new G4UIcmdWithABool("/MG/geometry/gslargeoptical/encaps",this);
  fEncapsulation->SetGuidance("Set/unset crystal encapsulation (default: false)");
  fEncapsulation->AvailableForStates(G4State_PreInit);
  // FINE PROVA
  fNumPhiSegments = new G4UIcmdWithAnInteger("/MG/geometry/gslargeoptical/phisegments",this);
  fNumPhiSegments->SetGuidance("Set the number of segments in phi (default: 6)");
  fNumPhiSegments->SetGuidance("To be used after truecoax is set to true");
  fNumPhiSegments->AvailableForStates(G4State_PreInit);

  fNumZSegments = new G4UIcmdWithAnInteger("/MG/geometry/gslargeoptical/zsegments",this);
  fNumZSegments->SetGuidance("Set the number of segments in z (default: 3)");
  fNumZSegments->SetGuidance("To be used after truecoax is set to true");
  fNumZSegments->AvailableForStates(G4State_PreInit);

  fBuildOptics = new G4UIcmdWithABool("/MG/geometry/gslargeoptical/buildoptics",this);
  fBuildOptics->SetGuidance("Activates the optical properties of the setup (default: false).");
  fBuildOptics->AvailableForStates(G4State_PreInit);

  fBuildSource = new G4UIcmdWithABool("/MG/geometry/gslargeoptical/buildsource",this);
  fBuildSource->SetGuidance("Builds a source in the geometry.(default: false).");
  fBuildSource->AvailableForStates(G4State_PreInit);

  fSourceGeometry = new G4UIcmdWithAString("/MG/geometry/gslargeoptical/sourcedesign",this);
  fSourceGeometry->SetGuidance("Chooses the design of the source. Valid options:");
  fSourceGeometry->SetGuidance("\"ThWire\" : MPIK Th wire source.");
  fSourceGeometry->SetGuidance("\"CoWire\" : Testing Co wire source (like Th but with thinner shielding.");
  fSourceGeometry->SetGuidance("");
  fSourceGeometry->SetGuidance("To be used with \"/MG/geometry/gslargeoptical/buildsource\".");
  fSourceGeometry->AvailableForStates(G4State_PreInit);

  fSourceType = new G4UIcmdWithAString("/MG/geometry/gslargeoptical/sourceposition",this);
  fSourceType->SetGuidance("Defines the position of the source (\"internal\" or \"external\").");
  fSourceType->SetGuidance("\"/MG/geometry/gslargeoptical/buildsource\" needs to be switched on.");


}


// --------------------------------------------------------------------------- 

GEGSLArGeOpticalMessenger::~GEGSLArGeOpticalMessenger()
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
  delete fBuildOptics;
  delete fBuildSource;
  delete fSourceGeometry;
  delete fSourceType;
}

// --------------------------------------------------------------------------- 

void GEGSLArGeOpticalMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
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
	  MGLog(warning) << "Please set /MG/geometry/gslargeoptical/truecoax to true first" << endlog;
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
	  MGLog(warning) << "Please set /MG/geometry/gslargeoptical/truecoax to true first" << endlog;
	  MGLog(warning) << "Nothing happens" << endlog;
	}
    }
  else if (cmd == fBuildOptics){
	  fDetector->SetFlagBuildOptics(fBuildOptics->GetNewBoolValue(newvalue));
  }
  else if (cmd == fBuildSource) {
	  fDetector->SetFlagBuildSource(fBuildSource->GetNewBoolValue(newvalue));
  }
  else if (cmd == fSourceGeometry) {
	  fDetector->SetSourceGeometry(newvalue);
  }
  else if (cmd == fSourceType) {
	  fDetector->SetSourceType(newvalue);
  }
}

// --------------------------------------------------------------------------- 
