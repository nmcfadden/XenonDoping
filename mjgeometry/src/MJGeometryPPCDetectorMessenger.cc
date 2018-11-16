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
// $Id: MJGeometryPPCDetectorMessenger.cc,v 1.1 2008-09-18 17:39:34 mitzi Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryPPCDetectorMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Feb 14 15:33:13 PST 2005
 * 
 * REVISION:
 *
 * 02-14-2005, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4ApplicationState.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"

#include "mjgeometry/MJGeometryPPCDetector.hh" 

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryPPCDetectorMessenger.hh" 

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryPPCDetectorMessenger::MJGeometryPPCDetectorMessenger(
   MJGeometryPPCDetector *xtalAssy  ) :
  fICWS(xtalAssy)
{
  // /MG/geometry/PPCDetector
  fDirectory = new G4UIdirectory("/MG/geometry/PPCdetector/");

  // /MG/geometry/idealCoaxWShield/height
  fAssyHeightCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/geometry/PPCDetector/height", this);
  fAssyHeightCmd->SetGuidance("Set height of enclosure");
  fAssyHeightCmd->SetDefaultUnit("cm");
  fAssyHeightCmd->SetUnitCandidates("m cm mm micrometer");

  // /MG/geometry/PPCDetector/radius
  fAssyRadiusCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/geometry/PPCDetector/radius", this);
  fAssyRadiusCmd->SetGuidance("Set height of enclosure");
  fAssyRadiusCmd->SetDefaultUnit("cm");
  fAssyRadiusCmd->SetUnitCandidates("m cm mm micrometer");
  
  

}

//---------------------------------------------------------------------------//

MJGeometryPPCDetectorMessenger::~MJGeometryPPCDetectorMessenger()
{
  delete fAssyHeightCmd;
  delete fAssyRadiusCmd;
  delete fDirectory;
}

//---------------------------------------------------------------------------//

void MJGeometryPPCDetectorMessenger::SetNewValue(G4UIcommand *cmd, 
							 G4String nV)
{
  if(cmd == fAssyHeightCmd)
    fICWS->SetCrystalHeight(fAssyHeightCmd->GetNewDoubleValue(nV));
  else if (cmd == fAssyRadiusCmd)
    fICWS->SetCrystalRadius(fAssyRadiusCmd->GetNewDoubleValue(nV));
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
