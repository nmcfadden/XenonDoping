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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 02-06-2005
 *
 * REVISION: MM-DD-YYYY
 *
 * 03-03-2006,  added source position setting commands for Siegfried
 * 12-07-2006 & 01-15-2007, added collimator-commands, Manuela
 * 10-23-2009,  new command is added to set the collimator length in the macro,
 *                 CollimatorHalfLength, Neslihan.
   08-04-2010,  added a source holder stick of length 11 cm between the source
 *                 and the detector. Alex
 */

// ---------------------------------------------------------------------------

#ifndef _GEMUNICHTESTSTANDDBMESSENGER_HH
#define _GEMUNICHTESTSTANDDBMESSENGER_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4UImessenger.hh"

// --------------------------------------------------------------------------- 

class GEMunichTestStandDB; 
class G4UIdirectory; 
class G4UIcmdWithAString; 
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger; 
class G4UIcmdWithABool; 

class GEMunichTestStandDBMessenger : public G4UImessenger
{

public:

  //default constructor

  GEMunichTestStandDBMessenger(GEMunichTestStandDB* teststanddb);

  // destructor

  ~GEMunichTestStandDBMessenger();

  // methods 

   void SetNewValue(G4UIcommand *cmd, G4String newvalue);

private:

  GEMunichTestStandDB* fTestStandDB; 

  G4UIdirectory* fTestStandDir;

  G4UIcmdWithAString*        fTestStandTypeCmd; 
  G4UIcmdWithADoubleAndUnit* fTestStandCrystalHeightCmd; 
  G4UIcmdWithADoubleAndUnit* fTestStandCrystalInnerRadiusCmd; 
  G4UIcmdWithADoubleAndUnit* fTestStandCrystalOuterRadiusCmd;
  G4UIcmdWithADoubleAndUnit* fTestStandCrystalDeadLayerCmd; 
  G4UIcmdWithADoubleAndUnit* fTestStandDepthOfSourceCmd; 
  G4UIcmdWithADoubleAndUnit* fTestStandDepthOfCrystalCmd; 
  G4UIcmdWithADoubleAndUnit* fTestStandCrystalDistanceToSourceCmd; 
  G4UIcmdWithADoubleAndUnit* fTestStandSourceDistanceToBrickCmd; 
  G4UIcmdWithAnInteger*      fTestStandSourceHole; 
  G4UIcmdWithAString*        fTestStandCrystalType; 

  //position of source system:
  G4UIcmdWithADoubleAndUnit* fTestStandSiegfriedSourcezPositionCmd;
  G4UIcmdWithADoubleAndUnit* fTestStandSiegfriedSourceToCanTopCmd;
  G4UIcmdWithADoubleAndUnit* fTestStandSiegfriedSourceToCanSideCmd;
  G4UIcmdWithADoubleAndUnit* fTestStandSiegfriedSourcerPositionCmd;
  G4UIcmdWithADoubleAndUnit* fTestStandSiegfriedSourcephiPositionCmd;

  G4UIcmdWithABool         * fTestStandSiegfriedUseSourceHolderStickCmd;
  G4UIcmdWithADoubleAndUnit* fTestStandSiegfriedSourceHolderStickLengthCmd;
  

  G4UIcmdWithADoubleAndUnit* fTestStandSiegfriedREGeDistanceCmd;
  G4UIcmdWithABool*          fTestStandSiegfriedUseCollimatorCmd;
  
  //Collimator
  G4UIcmdWithAnInteger*      fTestStandCollimatorCollimatorPositionCmd;
  G4UIcmdWithADoubleAndUnit* fTestStandCollimatorCollimatorInnerRadiusCmd; 
  G4UIcmdWithADoubleAndUnit* fTestStandCollimatorCollimatorOuterRadiusCmd; 
  G4UIcmdWithADoubleAndUnit* fTestStandCollimatorCollimatorLowerHeightCmd;
  G4UIcmdWithADoubleAndUnit* fTestStandCollimatorCollimatorHalfLengthCmd;
  G4UIcmdWithADoubleAndUnit* fTestStandCollimatorCollimatorDetectorDistanceCmd;
  G4UIcmdWithADoubleAndUnit* fTestStandCollimatorSystemRotationCmd;
  G4UIcmdWithADoubleAndUnit* fTestStandCollimatorDeltaCmd;
  G4UIcmdWithAString*        fTestStandCollimatorMaterialCmd;
  G4UIcmdWithAString*        fTestStandCollimatorTypeCmd;


  //Milchkanne
  G4UIcmdWithADoubleAndUnit* fMilchkanneSourceXPositionCmd;


};

// --------------------------------------------------------------------------- 

#endif
