




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
 * FIRST SUBMISSION: 28 Jun 2007
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 12-01-2008, added dead layer parameter, z shift, Dusan & Marik
 * 
 */

// ---------------------------------------------------------------------------

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "munichteststand/GEGDLStandMessenger.hh"
#include "munichteststand/GEGDLStand.hh" 
#include "io/MGLogger.hh"

// --------------------------------------------------------------------------- 

GEGDLStandMessenger::GEGDLStandMessenger(GEGDLStand *teststand) : 
  fDetector(teststand) 
{
  // directories 
  
  fGDLStandDirectory =  new G4UIdirectory("/MG/geometry/gdlstand/"); 
  fGDLStandDirectory->SetGuidance("Tune teststand parameters for GDL at LNGS"); 

fDeadThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/deadlayer", this);
fDeadThicknessCmd -> SetGuidance("Set thickness of the deadlayer");
fDeadThicknessCmd -> SetParameterName("DeadThickness", true); 
fDeadThicknessCmd -> SetDefaultValue(0.85); 
fDeadThicknessCmd -> SetDefaultUnit("mm");

fShiftDetectorCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/shiftdetector", this);
fShiftDetectorCmd -> SetGuidance("Set z position of the detector");
fShiftDetectorCmd -> SetParameterName("ShiftDetector", true); 
fShiftDetectorCmd -> SetDefaultValue(3); 
fShiftDetectorCmd -> SetDefaultUnit("mm");

fDetectorDiamCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/detectordiam", this);
fDetectorDiamCmd -> SetGuidance("Set crystal diameter");
fDetectorDiamCmd -> SetParameterName("DetectorDiam", true); 
fDetectorDiamCmd -> SetDefaultValue(74.5); 
fDetectorDiamCmd -> SetDefaultUnit("mm");

fDetectorHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/detectorheight", this);
fDetectorHeightCmd -> SetGuidance("Set crystal height");
fDetectorHeightCmd -> SetParameterName("DetectorHeight", true); 
fDetectorHeightCmd -> SetDefaultValue(69); 
fDetectorHeightCmd -> SetDefaultUnit("mm");

fHoleDiamCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/holediam", this);
fHoleDiamCmd -> SetGuidance("Set hole diameter");
fHoleDiamCmd -> SetParameterName("HoleDiam", true); 
fHoleDiamCmd -> SetDefaultValue(12); 
fHoleDiamCmd -> SetDefaultUnit("mm");

fDetHoleHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/detholeheight", this);
fDetHoleHeightCmd -> SetGuidance("Set hole height");
fDetHoleHeightCmd -> SetParameterName("DetHoleHeight", true); 
fDetHoleHeightCmd -> SetDefaultValue(60); 
fDetHoleHeightCmd -> SetDefaultUnit("mm");

fGrooveOutDiamCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/grooveoutdiam", this);
fGrooveOutDiamCmd -> SetGuidance("Set outer diameter of the groove");
fGrooveOutDiamCmd -> SetParameterName("GrooveOutDiam", true); 
fGrooveOutDiamCmd -> SetDefaultValue(30.5); 
fGrooveOutDiamCmd -> SetDefaultUnit("mm");

fGrooveThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/groovethickness", this);
fGrooveThicknessCmd -> SetGuidance("Set groove thickness");
fGrooveThicknessCmd -> SetParameterName("GrooveThickness", true); 
fGrooveThicknessCmd -> SetDefaultValue(3); 
fGrooveThicknessCmd -> SetDefaultUnit("mm");

fGrooveInDiamCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/grooveindiam", this);
fGrooveInDiamCmd -> SetGuidance("Set inner diameter of the groove");
fGrooveInDiamCmd -> SetParameterName("GrooveInDiam", true); 
fGrooveInDiamCmd -> SetDefaultValue(23); 
fGrooveInDiamCmd -> SetDefaultUnit("mm");

fCutParameterCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/cutpar", this);
fCutParameterCmd -> SetGuidance("Inner radius of the cons to cut corner");
fCutParameterCmd -> SetParameterName("CutParameter", true); 
fCutParameterCmd -> SetDefaultValue(22.54); 
fCutParameterCmd -> SetDefaultUnit("mm");

fCutPositionCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/cutpos", this);
fCutPositionCmd -> SetGuidance("Position of the cons to cut corner");
fCutPositionCmd -> SetParameterName("CutPosition", true); 
fCutPositionCmd -> SetDefaultValue(-50.63); 
fCutPositionCmd -> SetDefaultUnit("mm");

fCuttopParameterCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/cuttoppar", this);
fCuttopParameterCmd -> SetGuidance("Inner radius of the cons to cut top corner");
fCuttopParameterCmd -> SetParameterName("CuttopParameter", true); 
fCuttopParameterCmd -> SetDefaultValue(8.56); 
fCuttopParameterCmd -> SetDefaultUnit("mm");

fCuttopPositionCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/gdlstand/cuttoppos", this);
fCuttopPositionCmd -> SetGuidance("Position of the cons to cut top corner");
fCuttopPositionCmd -> SetParameterName("CuttopPosition", true); 
fCuttopPositionCmd -> SetDefaultValue(-55.84); 
fCuttopPositionCmd -> SetDefaultUnit("mm");


}

// --------------------------------------------------------------------------- 

GEGDLStandMessenger::~GEGDLStandMessenger()
{
  delete fGDLStandDirectory; 
  delete fDeadThicknessCmd;
  delete fShiftDetectorCmd;
  delete fDetectorDiamCmd;
  delete fDetectorHeightCmd;
  delete fHoleDiamCmd;
  delete fDetHoleHeightCmd;
  delete fGrooveOutDiamCmd;
  delete fGrooveThicknessCmd;
  delete fGrooveInDiamCmd;
  delete fCutParameterCmd;
  delete fCutPositionCmd;
  delete fCuttopParameterCmd;
  delete fCuttopPositionCmd;
  
}

// --------------------------------------------------------------------------- 

void GEGDLStandMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
{

  if (cmd == fDeadThicknessCmd)
    {
       fDetector->SetDeadThickness (fDeadThicknessCmd->GetNewDoubleValue(newvalue));
    }

 if (cmd == fShiftDetectorCmd)
    {
       fDetector->SetShiftDetector (fShiftDetectorCmd->GetNewDoubleValue(newvalue));
    }

if (cmd == fDetectorDiamCmd)
    {
       fDetector->SetDetectorDiam (fDetectorDiamCmd->GetNewDoubleValue(newvalue));
    }


if (cmd == fDetectorHeightCmd)
    {
       fDetector->SetDetectorHeight (fDetectorHeightCmd->GetNewDoubleValue(newvalue));
    }

if (cmd == fHoleDiamCmd)
    {
       fDetector->SetHoleDiam (fHoleDiamCmd->GetNewDoubleValue(newvalue));
    }


if (cmd == fDetHoleHeightCmd)
    {
       fDetector->SetDetHoleHeight (fDetHoleHeightCmd->GetNewDoubleValue(newvalue));
    }

if (cmd == fGrooveOutDiamCmd)
    {
       fDetector->SetGrooveOutDiam (fGrooveOutDiamCmd->GetNewDoubleValue(newvalue));
    }


if (cmd == fGrooveThicknessCmd)
    {
       fDetector->SetGrooveThickness (fGrooveThicknessCmd->GetNewDoubleValue(newvalue));
    }

if (cmd == fGrooveInDiamCmd)
    {
       fDetector->SetGrooveInDiam (fGrooveInDiamCmd->GetNewDoubleValue(newvalue));
    }

if (cmd == fCutParameterCmd)
  {
     fDetector->SetCutParameter (fCutParameterCmd->GetNewDoubleValue(newvalue));
  }

if (cmd == fCutPositionCmd)
  {
     fDetector->SetCutPosition (fCutPositionCmd->GetNewDoubleValue(newvalue));
  }

if (cmd == fCuttopParameterCmd)
  {
     fDetector->SetCuttopParameter (fCuttopParameterCmd->GetNewDoubleValue(newvalue));
  }

if (cmd == fCuttopPositionCmd)
  {
     fDetector->SetCuttopPosition (fCuttopPositionCmd->GetNewDoubleValue(newvalue));
  }


}

// --------------------------------------------------------------------------- 
