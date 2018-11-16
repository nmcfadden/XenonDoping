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
// CLASS IMPLEMENTATION:  MJGeometrySLACBDMessenger.cc
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
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

#include "mjgeometry/MJGeometrySLACBD.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;



//---------------------------------------------------------------------------//

//Present MJ Class Headers: 
#include "mjgeometry/MJGeometrySLACBDMessenger.hh"  

//---------------------------------------------------------------------------//

MJGeometrySLACBDMessenger::MJGeometrySLACBDMessenger(MJGeometrySLACBD *theDet) : fSLACBD(theDet)
{
  // /MG/geometry/cloverInShield
  fSLACBDDirectory = new G4UIdirectory("/MG/geometry/SLACBD/");

  // /MG/geometry/SLACBD/setShieldThickness
  fShieldThicknessCmd = new G4UIcmdWithAString(
     "/MG/geometry/SLACBD/setShieldThickness", this);
  fShieldThicknessCmd->SetGuidance("Set thickness of concrete shield.");
  fShieldThicknessCmd->AvailableForStates(G4State_PreInit);
  fShieldThicknessCmd->SetCandidates("None VeryThin Thin Medium Thick");
}

//---------------------------------------------------------------------------//

MJGeometrySLACBDMessenger::~MJGeometrySLACBDMessenger()
{
  delete fShieldThicknessCmd;
  delete fSLACBDDirectory;
}

//---------------------------------------------------------------------------//

void MJGeometrySLACBDMessenger::SetNewValue(G4UIcommand* cmd, G4String nV)
{
  if(cmd == fShieldThicknessCmd) {
    if(nV=="None") fSLACBD->SetConcShieldThickness(kNoShield);
    else if(nV=="VeryThin") fSLACBD->SetConcShieldThickness(kVeryThinShield);
    else if(nV=="Thin") fSLACBD->SetConcShieldThickness(kThinShield);
    else if(nV=="Medium") fSLACBD->SetConcShieldThickness(kMedShield);
    else if(nV=="Thick") fSLACBD->SetConcShieldThickness(kThickShield);
  }
}
