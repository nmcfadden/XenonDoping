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
// $Id: 
//      
// CLASS IMPLEMENTATION:  MJGeometryPbShieldMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
    Messenger for CERN NA55 beam dump simulation.
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: mgmarino 
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 *  */
//---------------------------------------------------------------------------//
//

//#include <systemG4Stuff.hh>             //additional system Headers
//#include "globalsStuff.hh"              //additional Global Headers
//#include "G4Stuff.hh"		              //additional G4 Class Headers
//#include "MJStuff.hh"                   //additional MJ Headers

#include "globals.hh"
#include "G4ios.hh"
#include "G4ApplicationState.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"

#include "mjgeometry/MJGeometryPbShield.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;



//---------------------------------------------------------------------------//

//Present MJ Class Headers: 
#include "mjgeometry/MJGeometryPbShieldMessenger.hh"  

//---------------------------------------------------------------------------//

MJGeometryPbShieldMessenger::MJGeometryPbShieldMessenger(MJGeometryPbShield *theDet) : fPbShield(theDet)
{
  // /MG/geometry/PbShield/
  fPbShieldDirectory = new G4UIdirectory("/MG/geometry/PbShield/");

  // /MG/geometry/PbShield/setOuterShieldDim
  fSetOuterShieldDim = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/PbShield/setOuterShieldDim", this);
  fSetOuterShieldDim->SetGuidance("Set outer shield dimensions of lead.");
  fSetOuterShieldDim->AvailableForStates(G4State_PreInit);

  // /MG/geometry/PbShield/setInnerShieldDim
  fSetInnerShieldDim = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/PbShield/setInnerShieldDim", this);
  fSetInnerShieldDim->SetGuidance("Set inner shield dimensions of lead.");
  fSetInnerShieldDim->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/PbShield/setImpSampRatio
  fSetImpSampRatio = new G4UIcmdWithADouble(
     "/MG/geometry/PbShield/setImpSampRatio", this);
  fSetImpSampRatio->SetGuidance("Set importance sampling ratio.");
  fSetImpSampRatio->AvailableForStates(G4State_PreInit);

  // /MG/geometry/PbShield/setNumberImpSampRegions
  fSetNumberImpSampRegions = new G4UIcmdWithAnInteger(
     "/MG/geometry/PbShield/setNumberImpSampRegions", this);
  fSetNumberImpSampRegions->SetGuidance("Set number of importance sampling regions.");
  fSetNumberImpSampRegions->AvailableForStates(G4State_PreInit);
}

//---------------------------------------------------------------------------//

MJGeometryPbShieldMessenger::~MJGeometryPbShieldMessenger()
{
  delete fSetNumberImpSampRegions;
  delete fSetImpSampRatio;
  delete fSetInnerShieldDim;
  delete fSetOuterShieldDim;
  delete fPbShieldDirectory;
}

//---------------------------------------------------------------------------//

void MJGeometryPbShieldMessenger::SetNewValue(G4UIcommand* cmd, G4String nV)
{
  if(cmd == fSetOuterShieldDim) {
    fPbShield->SetOuterShieldDim(fSetOuterShieldDim->GetNewDoubleValue(nV)); 
  } else if (cmd == fSetInnerShieldDim) {
    fPbShield->SetInnerShieldDim(fSetInnerShieldDim->GetNewDoubleValue(nV));
  } else if (cmd == fSetImpSampRatio) {
    fPbShield->SetImpSampRatio(fSetImpSampRatio->GetNewDoubleValue(nV));
  } else if (cmd == fSetNumberImpSampRegions) {
    fPbShield->SetNumberImpSampRegions(fSetNumberImpSampRegions->GetNewIntValue(nV));
  }

}
