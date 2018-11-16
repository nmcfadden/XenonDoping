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
 * AUTHOR: Matthias Laubenstein
 * CONTACT:
 * FIRST SUBMISSION:
 *
 * REVISION:
 * 03-08-2010, Added in MaGe, Luciano
 */
// ---------------------------------------------------------------------------

#ifndef _MGEMIDETECTORMESSENGER_HH
#define _MGEMIDETECTORMESSENGER_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4UImessenger.hh"

// --------------------------------------------------------------------------- 

class MGEMIDetector; 
class G4UIdirectory; 
class G4UIcmdWithAString; 
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble; 


class MGEMIDetectorMessenger : public G4UImessenger
{
public:
  //default constructor
  MGEMIDetectorMessenger(MGEMIDetector*);

  // destructor
  ~MGEMIDetectorMessenger();

  // methods 
   void SetNewValue(G4UIcommand *cmd, G4String newvalue);

private:

  MGEMIDetector* fDetectorPointer; 

  G4UIdirectory* fDetectorDir;

  G4UIcmdWithAString*        fSampleTypeCmd;
  G4UIcmdWithAString*        fSampleMaterialCmd;
  G4UIcmdWithADoubleAndUnit* fSMarinelliHeightCmd;
  G4UIcmdWithADoubleAndUnit* fSBoxHeightCmd;
  G4UIcmdWithADoubleAndUnit* fSBoxIDiameterCmd;
  G4UIcmdWithADoubleAndUnit* fSBoxODiameterCmd;
  G4UIcmdWithADoubleAndUnit* fSupportHeightCmd;
  G4UIcmdWithADoubleAndUnit* fSupportIDiameterCmd;
  G4UIcmdWithADoubleAndUnit* fSupportODiameterCmd;
  G4UIcmdWithADoubleAndUnit* fBoxHeightCmd;
  G4UIcmdWithADoubleAndUnit* fBoxWidthCmd; 
  G4UIcmdWithADoubleAndUnit* fBoxThicknessCmd;  
  G4UIcmdWithADoubleAndUnit* fTubeInnerRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fTubeOuterRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fTubeLengthCmd;
  //G4UIcmdWithADoubleAndUnit* fTubeAngleCmd;
  //G4UIcmdWithADoubleAndUnit* fSphereRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fSamplexposCmd;
  G4UIcmdWithADoubleAndUnit* fSampleyposCmd;
  G4UIcmdWithADoubleAndUnit* fSamplezposCmd;
  G4UIcmdWithADoubleAndUnit* fDetzposCmd;
  G4UIcmdWithADoubleAndUnit* fDeadThicknessCmd;
  G4UIcmdWithADoubleAndUnit* fHoleDiameterCmd;
  G4UIcmdWithADoubleAndUnit* fHoleLengthCmd;
  G4UIcmdWithADoubleAndUnit* fDetDiameterCmd;
  G4UIcmdWithADoubleAndUnit* fDetLengthCmd;
  //G4UIcmdWithADouble* fDetSizeFactorCmd;
  
};

// --------------------------------------------------------------------------- 

#endif
