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
// $Id: GEBrunoDetectorMessenger.cc,v 1.6 2009-05-18 16:08:53 dusan Exp $ 
//      
// CLASS IMPLEMENTATION:  GEBrunoDetectorMessenger.cc
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
 * 03-06-2007, Changed to avoid problem with materials defined on-the-fly, Luciano
 * 03-07-2007, Added 'custom' option for geometry and changed default material to NitrogenGas, Dusan
 * 06-01-2007, Sample type "none" instead of "sphere", Dusan
 * 14-05-2009, modified command directories, Dusan
 *
 */
//---------------------------------------------------------------------------//
//
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh" 
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "munichteststand/GEBrunoDetectorMessenger.hh"
#include "munichteststand/GEBrunoDetector.hh" 

// --------------------------------------------------------------------------- 

GEBrunoDetectorMessenger::GEBrunoDetectorMessenger(GEBrunoDetector *dete) :
  fDetectorPointer(dete) 
{
  // directories 
  fDetectorDir =  new G4UIdirectory("/MG/geometry/bruno/"); 
  fDetectorDir -> SetGuidance("Tune sample parameter for Bruno detector"); 
  
fSampleTypeCmd =  new G4UIcmdWithAString("/MG/geometry/sample", this);
fSampleTypeCmd -> SetGuidance("Select the shape of the sample");
fSampleTypeCmd -> SetParameterName("SampleType", true); 
fSampleTypeCmd -> SetDefaultValue("none"); 
fSampleTypeCmd -> SetCandidates("none box tube sbox liquid twobox custom"); 
 
fSampleMaterialCmd =  new G4UIcmdWithAString("/MG/geometry/samplematerial", this);
fSampleMaterialCmd -> SetGuidance("Select the material of the sample");
fSampleMaterialCmd -> SetParameterName("SampleMaterial", true); 
fSampleMaterialCmd -> SetDefaultValue("NitrogenGas"); 

fBoxHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/boxheight", this);
fBoxHeightCmd->SetGuidance("Set the height of the sample box");
fBoxHeightCmd->SetParameterName("BoxHeight", true);
fBoxHeightCmd->SetDefaultValue(2.0); 
fBoxHeightCmd-> SetDefaultUnit("cm");
  
fBoxWidthCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/boxwidth", this);
fBoxWidthCmd -> SetGuidance("Set the width of the sample box");
fBoxWidthCmd -> SetParameterName("BoxWidth", true); 
fBoxWidthCmd -> SetDefaultValue(2.0); 
fBoxWidthCmd -> SetDefaultUnit("cm"); 

fBoxThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/boxthickness", this);
fBoxThicknessCmd -> SetGuidance("Set the thickness of the sample box");
fBoxThicknessCmd -> SetParameterName("BoxThickness", true); 
fBoxThicknessCmd -> SetDefaultValue(2.0); 
fBoxThicknessCmd -> SetDefaultUnit("cm"); 

fTubeInnerRadiusCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/tubeinnerradius", this);
fTubeInnerRadiusCmd -> SetGuidance("Set the inner radius of the sample tube");
fTubeInnerRadiusCmd -> SetParameterName("TubeInnerRadius", true); 
fTubeInnerRadiusCmd -> SetDefaultValue(0.); 
fTubeInnerRadiusCmd -> SetDefaultUnit("cm"); 

fTubeOuterRadiusCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/tubeouterradius", this);
fTubeOuterRadiusCmd -> SetGuidance("Set the outer radius of the sample tube");
fTubeOuterRadiusCmd -> SetParameterName("TubeOuterRadius", true); 
fTubeOuterRadiusCmd -> SetDefaultValue(2.0); 
fTubeOuterRadiusCmd -> SetDefaultUnit("cm");

fTubeLengthCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/tubelength", this);
fTubeLengthCmd -> SetGuidance("Set the length of the sample tube");
fTubeLengthCmd -> SetParameterName("TubeLength", true); 
fTubeLengthCmd -> SetDefaultValue(2.0); 
fTubeLengthCmd -> SetDefaultUnit("cm"); 

fSamplexposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/samplexpos", this);
fSamplexposCmd -> SetGuidance("Set the x coordinate of the center of the sample");
fSamplexposCmd -> SetParameterName("Samplexpos", true); 
fSamplexposCmd -> SetDefaultValue(0.); 
fSamplexposCmd -> SetDefaultUnit("cm"); 

fSampleyposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/sampleypos", this);
fSampleyposCmd -> SetGuidance("Set the y coordinate of the center of the sample");
fSampleyposCmd -> SetParameterName("Sampleypos", true); 
fSampleyposCmd -> SetDefaultValue(0.); 
fSampleyposCmd -> SetDefaultUnit("cm"); 

fSamplezposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/samplezpos", this);
fSamplezposCmd -> SetGuidance("Set the z coordinate of the center of the sample");
fSamplezposCmd -> SetParameterName("Samplezpos", true); 
fSamplezposCmd -> SetDefaultValue(14.); 
fSamplezposCmd -> SetDefaultUnit("cm"); 

}

// --------------------------------------------------------------------------- 

GEBrunoDetectorMessenger::~GEBrunoDetectorMessenger()
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
  
}

// --------------------------------------------------------------------------- 

void GEBrunoDetectorMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
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
  
}

// --------------------------------------------------------------------------- 
