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
// $Id: MJGeometryIdealCoaxWithShieldMessenger.cc,v 1.2 2005-03-31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryIdealCoaxWithShieldMessenger.cc
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

#include "mjgeometry/MJGeometryIdealCoaxWithShield.hh" 

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryIdealCoaxWithShieldMessenger.hh" 

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometryIdealCoaxWithShieldMessenger::MJGeometryIdealCoaxWithShieldMessenger(
   MJGeometryIdealCoaxWithShield *xtalAssy  ) :
  fICWS(xtalAssy)
{
  // /MG/geometry/idealCoaxWShield
  fDirectory = new G4UIdirectory("/MG/geometry/idealCoaxWShield/");

  // /MG/geometry/idealCoaxWShield/height
  fAssyHeightCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoaxWShield/height", this);
  fAssyHeightCmd->SetGuidance("Set height of enclosure");
  fAssyHeightCmd->SetDefaultUnit("cm");
  fAssyHeightCmd->SetUnitCandidates("m cm mm micrometer");

  // /MG/geometry/idealCoaxWShield/radius
  fAssyRadiusCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoaxWShield/radius", this);
  fAssyRadiusCmd->SetGuidance("Set height of enclosure");
  fAssyRadiusCmd->SetDefaultUnit("cm");
  fAssyRadiusCmd->SetUnitCandidates("m cm mm micrometer");
  
  // /MG/geometry/idealCoaxWShield/coldplateMaterial
  fColdplateMaterialCmd = new 
    G4UIcmdWithAString("/MG/geometry/idealCoaxWShield/coldplateMaterial", this);
  fColdplateMaterialCmd->SetGuidance("Material name for coldplate (holder).");  
  fColdplateMaterialCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/idealCoaxWShield/coldplateThickness
  fColdplateThicknessCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoaxWShield/coldplateThickness",
			      this);
  fColdplateThicknessCmd->SetDefaultUnit("cm");
  fColdplateThicknessCmd->SetUnitCandidates("m cm mm micrometer");
  
  // /MG/geometry/idealCoaxWShield/dump
  fDumpCmd = new G4UIcmdWithoutParameter("/MG/geometry/idealCoaxWShield/dump",
					 this);

  // /MG/geometry/idealCoaxWShield/capMaterial
  fEndcapMaterialCmd = new 
    G4UIcmdWithAString("/MG/geometry/idealCoaxWShield/capMaterial", this);
  fEndcapMaterialCmd->SetGuidance("Material name for end cap.");  
  fEndcapMaterialCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/idealCoaxWShield/capThickness
  fEndcapThicknessCmd = new 
    G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoaxWShield/capThickness",this);
  fEndcapThicknessCmd->SetDefaultUnit("cm");
  fEndcapThicknessCmd->SetUnitCandidates("m cm mm micrometer");
  
  // /MG/geometry/idealCoaxWShield/shieldOn  
  fShieldOnCmd = new G4UIcmdWithABool("/MG/geometry/idealCoaxWShield/shieldOn",
				      this);
  fShieldOnCmd->SetGuidance("Activate enclosure for crystal.");
  fShieldOnCmd->
    SetGuidance("Make sure to provide enclosure dimensions if true");
  fShieldOnCmd->AvailableForStates(G4State_PreInit);

}

//---------------------------------------------------------------------------//

MJGeometryIdealCoaxWithShieldMessenger::~MJGeometryIdealCoaxWithShieldMessenger()
{
  delete fAssyHeightCmd;
  delete fAssyRadiusCmd;
  delete fColdplateMaterialCmd;
  delete fColdplateThicknessCmd;
  delete fDumpCmd; 
  delete fEndcapMaterialCmd; 
  delete fEndcapThicknessCmd;
  delete fShieldOnCmd;
  delete fDirectory;
}

//---------------------------------------------------------------------------//

void MJGeometryIdealCoaxWithShieldMessenger::SetNewValue(G4UIcommand *cmd, 
							 G4String nV)
{
  if(cmd == fAssyHeightCmd)
    fICWS->SetAssemblyHeight(fAssyHeightCmd->GetNewDoubleValue(nV));
  else if (cmd == fAssyRadiusCmd)
    fICWS->SetAssemblyRadius(fAssyRadiusCmd->GetNewDoubleValue(nV));
  else if (cmd == fColdplateMaterialCmd)
    fICWS->SetColdplateMaterialName(nV);
  else if (cmd == fColdplateThicknessCmd)
    fICWS->SetColdplateThickness(fColdplateThicknessCmd->GetNewDoubleValue(nV));
  else if (cmd == fDumpCmd)
    fICWS->Dump();
  else if(cmd == fEndcapMaterialCmd)
    fICWS->SetEndcapMaterialName(nV);
  else if(cmd == fEndcapThicknessCmd)
    fICWS->SetEndcapThickness(fEndcapThicknessCmd->GetNewDoubleValue(nV));
  else if(cmd == fShieldOnCmd)
    fICWS->SetShieldOn(fShieldOnCmd->GetNewBoolValue(nV));
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
