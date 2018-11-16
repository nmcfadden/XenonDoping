//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//                                                          
//      
// CLASS IMPLEMENTATION:  MJGeometryVT1DetectorMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *  Modeled after MJGeometryWIPPnNewShieldMessenger.cc
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: K. Macon
 * CONTACT: mokevin@physics.unc.edu
 *  
 * REVISION: 9-25-2008 Added Calibration Source to Candidate List
 *
 * 
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "mjgeometry/MJGeometryVT1Detector.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryVT1DetectorMessenger.hh"  

//---------------------------------------------------------------------------//

MJGeometryVT1DetectorMessenger::MJGeometryVT1DetectorMessenger(MJGeometryVT1Detector* VT1):
fVT1Detector( VT1 )
{
  fVT1DetectorDir = new G4UIdirectory("/MG/geometry/VT1Detector/");
 
  // turn the use of samples on or off
  fUseSampleCmd = 
    new G4UIcmdWithABool("/MG/geometry/VT1Detector/useSample",this);
  fUseSampleCmd->SetGuidance("Command to insert a sample into geometry.");
  
  // choose which sample to insert into geometry
  fChooseSampleCmd = new G4UIcmdWithAString(
    "/MG/geometry/VT1Detector/chooseSample",this);
  fChooseSampleCmd->SetGuidance("Select sample");
  G4String candidates = "RockBucket RockBag SuperInsulation Teflon PMT Marinelli ";
  candidates += "CalibrationSource, PicoCoax ";
  fChooseSampleCmd->SetCandidates(candidates);
  fChooseSampleCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  //Distinguish Between Possibly Placements
  fPlaceSampleCmd = new G4UIcmdWithAString(
    "/MG/geometry/VT1Detector/placeSample",this);
  fPlaceSampleCmd->SetGuidance("Pick Sample Placement");
  fPlaceSampleCmd->SetCandidates("Default OnTop InBottom");
  fPlaceSampleCmd->AvailableForStates( G4State_PreInit, G4State_Idle );
}

MJGeometryVT1DetectorMessenger::~MJGeometryVT1DetectorMessenger()
{
  delete fVT1DetectorDir;
  delete fUseSampleCmd;
  delete fChooseSampleCmd;
  delete fPlaceSampleCmd;
}

void MJGeometryVT1DetectorMessenger::SetNewValue(G4UIcommand *command,
					      G4String newValues)
{
  if (command == fUseSampleCmd) 
  {
    fVT1Detector->SetUseSample(
      fUseSampleCmd->GetNewBoolValue(newValues));
    MGLog(trace) << "Using Inserted Sample." << endlog; 
  }
  else if (command == fChooseSampleCmd)
  {
    fVT1Detector->SetSample(newValues);
  }
  else if (command == fPlaceSampleCmd)
  { 
  fVT1Detector->SetPlacement(newValues);
  }
}
