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
//                                                          
// $Id: GEDarioDetectorMessenger.cc,v 1.3 2007-03-13 16:22:39 dusan Exp $ 
//      
// CLASS IMPLEMENTATION:  GEDarioDetectorMessenger.cc
//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Luciano Pandola
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * 03-06-2007, Added in MaGe, Luciano
 * 03-07-2007, Changed to avoid problem with materials defined on-the-fly, Luciano
 *
 */
// ---------------------------------------------------------------------------

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh" 
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "munichteststand/GEDarioDetectorMessenger.hh"
#include "munichteststand/GEDarioDetector.hh" 

// --------------------------------------------------------------------------- 

GEDarioDetectorMessenger::GEDarioDetectorMessenger(GEDarioDetector *dete) :
  fDetectorPointer(dete) 
{
  // directories 
  fDetectorDir =  new G4UIdirectory("/MG/geometry/dario/"); 
  fDetectorDir -> SetGuidance("Tune sample parameter for Dario detector"); 
  
fSampleTypeCmd =  new G4UIcmdWithAString("/MG/geometry/dario/sample", this);
fSampleTypeCmd -> SetGuidance("Select the shape of the sample");
fSampleTypeCmd -> SetParameterName("SampleType", true); 
fSampleTypeCmd -> SetDefaultValue("sphere"); 
fSampleTypeCmd -> SetCandidates("sphere box tube sbox liquid twobox marinelli custom"); 
 
fSampleMaterialCmd =  new G4UIcmdWithAString("/MG/geometry/dario/samplematerial", this);
fSampleMaterialCmd -> SetGuidance("Select the material of the sample");
fSampleMaterialCmd -> SetParameterName("SampleMaterial", true); 
fSampleMaterialCmd -> SetDefaultValue("NitrogenGas"); 


fBoxHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/boxheight", this);
fBoxHeightCmd->SetGuidance("Set the height of the sample box");
fBoxHeightCmd->SetParameterName("BoxHeight", true);
fBoxHeightCmd->SetDefaultValue(2.0); 
fBoxHeightCmd-> SetDefaultUnit("cm");
  
fBoxWidthCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/boxwidth", this);
fBoxWidthCmd -> SetGuidance("Set the width of the sample box");
fBoxWidthCmd -> SetParameterName("BoxWidth", true); 
fBoxWidthCmd -> SetDefaultValue(2.0); 
fBoxWidthCmd -> SetDefaultUnit("cm"); 

fBoxThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/boxthickness", this);
fBoxThicknessCmd -> SetGuidance("Set the thickness of the sample box");
fBoxThicknessCmd -> SetParameterName("BoxThickness", true); 
fBoxThicknessCmd -> SetDefaultValue(2.0); 
fBoxThicknessCmd -> SetDefaultUnit("cm"); 

fTubeInnerRadiusCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/tubeinnerradius", this);
fTubeInnerRadiusCmd -> SetGuidance("Set the inner radius of the sample tube");
fTubeInnerRadiusCmd -> SetParameterName("TubeInnerRadius", true); 
fTubeInnerRadiusCmd -> SetDefaultValue(0.); 
fTubeInnerRadiusCmd -> SetDefaultUnit("cm"); 

fTubeOuterRadiusCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/tubeouterradius", this);
fTubeOuterRadiusCmd -> SetGuidance("Set the outer radius of the sample tube");
fTubeOuterRadiusCmd -> SetParameterName("TubeOuterRadius", true); 
fTubeOuterRadiusCmd -> SetDefaultValue(2.0); 
fTubeOuterRadiusCmd -> SetDefaultUnit("cm");

fTubeLengthCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/tubelength", this);
fTubeLengthCmd -> SetGuidance("Set the length of the sample tube");
fTubeLengthCmd -> SetParameterName("TubeLength", true); 
fTubeLengthCmd -> SetDefaultValue(2.0); 
fTubeLengthCmd -> SetDefaultUnit("cm"); 

fSamplexposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/samplexpos", this);
fSamplexposCmd -> SetGuidance("Set the x coordinate of the center of the sample");
fSamplexposCmd -> SetParameterName("Samplexpos", true); 
fSamplexposCmd -> SetDefaultValue(0.); 
fSamplexposCmd -> SetDefaultUnit("cm"); 

fSampleyposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/sampleypos", this);
fSampleyposCmd -> SetGuidance("Set the y coordinate of the center of the sample");
fSampleyposCmd -> SetParameterName("Sampleypos", true); 
fSampleyposCmd -> SetDefaultValue(0.); 
fSampleyposCmd -> SetDefaultUnit("cm"); 

fSamplezposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/samplezpos", this);
fSamplezposCmd -> SetGuidance("Set the z coordinate of the center of the sample");
fSamplezposCmd -> SetParameterName("Samplezpos", true); 
fSamplezposCmd -> SetDefaultValue(14.); 
fSamplezposCmd -> SetDefaultUnit("cm"); 

fDetzposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/detzpos", this);
fDetzposCmd -> SetGuidance("Set z distance from detector window to crystal surface");
fDetzposCmd -> SetParameterName("Detzpos", true); 
fDetzposCmd -> SetDefaultValue(0.7); 
fDetzposCmd -> SetDefaultUnit("cm"); 

fDeadThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/deadthickness", this);
fDeadThicknessCmd -> SetGuidance("Set dead layer thickness");
fDeadThicknessCmd -> SetParameterName("DeadThickness", true); 
fDeadThicknessCmd -> SetDefaultValue(0.2); 
fDeadThicknessCmd -> SetDefaultUnit("cm"); 

fDetDiameterCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/detdiameter", this);
fDetDiameterCmd -> SetGuidance("Set crystal outer diameter (including dead layer)");
fDetDiameterCmd -> SetParameterName("DetDiameter", true); 
fDetDiameterCmd -> SetDefaultValue(5.95);
fDetDiameterCmd -> SetDefaultUnit("cm"); 

fDetLenghtCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/dario/detlenght", this);
fDetLenghtCmd -> SetGuidance("Set crystal lenght (including dead layer)");
fDetLenghtCmd -> SetParameterName("DetLenght", true); 
fDetLenghtCmd -> SetDefaultValue(6.1);
fDetLenghtCmd -> SetDefaultUnit("cm"); 
/*
fDetSizeFactorCmd =  new G4UIcmdWithADouble("/MG/geometry/dario/detsizefactor", this);
fDetSizeFactorCmd -> SetGuidance("Set multiplication factor of crystal size (not working)");
fDetSizeFactorCmd -> SetParameterName("DetSizeFactor", true); 
fDetSizeFactorCmd -> SetDefaultValue(1);
*/
}

// --------------------------------------------------------------------------- 

GEDarioDetectorMessenger::~GEDarioDetectorMessenger()
{

  delete fDetectorDir;
  delete fSampleTypeCmd;
  delete fSampleMaterialCmd;
  delete fBoxHeightCmd;
  delete fBoxWidthCmd;
  delete fBoxThicknessCmd;
  delete fTubeInnerRadiusCmd;
  delete fTubeOuterRadiusCmd;
  delete fTubeLengthCmd;
  
  delete fSamplexposCmd;
  delete fSampleyposCmd;
  delete fSamplezposCmd;

  delete fDetzposCmd;
  delete fDeadThicknessCmd;
  delete fDetDiameterCmd;
  delete fDetLenghtCmd;
  //delete fDetSizeFactorCmd;
  
}

// --------------------------------------------------------------------------- 

void GEDarioDetectorMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
{
  if (cmd == fSampleTypeCmd)
    {
      fDetectorPointer->SetSampleGeometry(newvalue);
    }
  else if (cmd == fSampleMaterialCmd)
    {
      fDetectorPointer->SetSampleMaterial(newvalue);
    }
  else if (cmd == fBoxHeightCmd)
    {
      fDetectorPointer->SetBoxHeight(fBoxHeightCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fBoxWidthCmd)
    {
      fDetectorPointer->SetBoxWidth(fBoxWidthCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fBoxThicknessCmd)
    {
       fDetectorPointer->SetBoxThickness(fBoxThicknessCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fTubeInnerRadiusCmd)
    {
       fDetectorPointer->SetTubeInnerRadius(fTubeInnerRadiusCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fTubeOuterRadiusCmd)
    {
       fDetectorPointer->SetTubeOuterRadius(fTubeOuterRadiusCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fTubeLengthCmd)
    {
       fDetectorPointer->SetTubeLength(fTubeLengthCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fSamplexposCmd)
    {
       fDetectorPointer->SetSamplexpos(fSamplexposCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fSampleyposCmd)
    {
       fDetectorPointer->SetSampleypos(fSampleyposCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fSamplezposCmd)
    {
       fDetectorPointer->SetSamplezpos(fSamplezposCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fDetzposCmd)
    {
       fDetectorPointer->SetDetzpos(fDetzposCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fDeadThicknessCmd)
    {
       fDetectorPointer->SetDeadThickness(fDeadThicknessCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fDetDiameterCmd)
    {
       fDetectorPointer->SetDetDiameter(fDetDiameterCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fDetLenghtCmd)
    {
       fDetectorPointer->SetDetLenght(fDetLenghtCmd->GetNewDoubleValue(newvalue));
    }
  
}

// --------------------------------------------------------------------------- 
