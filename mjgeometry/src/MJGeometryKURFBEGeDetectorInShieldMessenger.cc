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
/**
 * AUTHOR: alexis
 * CONTACT: 
 * FIRST SUBMISSION: Fri Aug 27 07:08:08 PDT 2010
 * 
 * REVISION:
 *
 * 10-01-2012, Creation, A. Schubert
 */

#include "G4ApplicationState.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"


#include "mjgeometry/MJGeometryKURFBEGeDetectorInShield.hh"      
#include "mjgeometry/MJGeometryKURFBEGeDetectorInShieldMessenger.hh"      

using namespace CLHEP;



MJGeometryKURFBEGeDetectorInShieldMessenger::MJGeometryKURFBEGeDetectorInShieldMessenger(
  MJGeometryKURFBEGeDetectorInShield *kurfBEGeDetectorInShield 
):
  fKURFBEGeDetectorInShield(kurfBEGeDetectorInShield)
{

  // /MG/geometry/kurfBEGeDetectorInShield
  fDirectory = new G4UIdirectory("/MG/geometry/kurfBEGeDetectorInShield/");

  // /MG/geometry/kurfBEGeDetectorInShield/usePolyShield
  fIsPolyPresentCmd = new 
    G4UIcmdWithABool("/MG/geometry/kurfBEGeDetectorInShield/usePolyShield",this);
  fIsPolyPresentCmd->SetGuidance("True if you want to use the poly shield.");  
  fIsPolyPresentCmd->SetDefaultValue(true);  
  fIsPolyPresentCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/kurfBEGeDetectorInShield/isDetectorInShield
  fIsDetectorInShieldCmd = new 
    G4UIcmdWithABool("/MG/geometry/kurfBEGeDetectorInShield/isDetectorInShield",this);
  fIsDetectorInShieldCmd->SetGuidance("True if you want to use the poly shield.");  
  fIsDetectorInShieldCmd->SetDefaultValue(true);  
  fIsDetectorInShieldCmd->AvailableForStates(G4State_PreInit);
  
}

MJGeometryKURFBEGeDetectorInShieldMessenger::~MJGeometryKURFBEGeDetectorInShieldMessenger()
{
  delete fIsPolyPresentCmd;
  delete fIsDetectorInShieldCmd;
  delete fDirectory;
}

void MJGeometryKURFBEGeDetectorInShieldMessenger::SetNewValue(
  G4UIcommand *cmd, 
  G4String nV
) {
    
  if(cmd == fIsPolyPresentCmd) {
    fKURFBEGeDetectorInShield->SetIsPolyPresent( 
        fIsPolyPresentCmd->GetNewBoolValue(nV) 
    );
  }
  
  if(cmd == fIsDetectorInShieldCmd) {
    fKURFBEGeDetectorInShield->SetIsDetectorInShield( 
        fIsDetectorInShieldCmd->GetNewBoolValue(nV) 
    );
  }
    
}

