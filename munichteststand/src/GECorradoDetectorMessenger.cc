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
// $Id: GECorradoDetectorMessenger.cc,v 1.4 2009-05-18 16:08:53 dusan Exp $ 
//      
// CLASS IMPLEMENTATION:  GECorradoDetectorMessenger.cc
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
 * 06-01-2007, Implemented parameters from Hardy, Dusan
 * 14-05-2009, Added geometry options, modified command directories, added hole-size parameters, Dusan
 *
 */
// ---------------------------------------------------------------------------

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh" 
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "munichteststand/GECorradoDetectorMessenger.hh"
#include "munichteststand/GECorradoDetector.hh" 

// --------------------------------------------------------------------------- 

GECorradoDetectorMessenger::GECorradoDetectorMessenger(GECorradoDetector *dete) :
  fDetectorPointer(dete) 
{
  // directories 
  fDetectorDir =  new G4UIdirectory("/MG/geometry/corrado/"); 
  fDetectorDir -> SetGuidance("Tune sample parameter for Corrado detector"); 
  
fSampleTypeCmd =  new G4UIcmdWithAString("/MG/geometry/sample", this);
fSampleTypeCmd -> SetGuidance("Select the shape of the sample");
fSampleTypeCmd -> SetParameterName("SampleType", true); 
fSampleTypeCmd -> SetDefaultValue("none"); 
fSampleTypeCmd -> SetCandidates("none box tube sbox liquid collimator marinelli roundmarinelli NPL-GL custom"); 
 
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
 
fFrontDeadThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/corrado/frontdeadlayer", this);
fFrontDeadThicknessCmd -> SetGuidance("Set thickness of front deadlayer");
fFrontDeadThicknessCmd -> SetParameterName("FrontDeadThickness", true); 
fFrontDeadThicknessCmd -> SetDefaultValue(0.07); 
fFrontDeadThicknessCmd -> SetDefaultUnit("cm");

fSideDeadThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/corrado/sidedeadlayer", this);
fSideDeadThicknessCmd -> SetGuidance("Set thickness of side deadlayer");
fSideDeadThicknessCmd -> SetParameterName("SideDeadThickness", true); 
fSideDeadThicknessCmd -> SetDefaultValue(0.07); 
fSideDeadThicknessCmd -> SetDefaultUnit("cm");

fCollimatorxposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/corrado/collimatorxpos", this);
fCollimatorxposCmd -> SetGuidance("Set x position of collimator");
fCollimatorxposCmd -> SetParameterName("CollimatorXPosition", true); 
fCollimatorxposCmd -> SetDefaultValue(0); 
fCollimatorxposCmd -> SetDefaultUnit("cm");

fCollimatoryposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/corrado/collimatorypos", this);
fCollimatoryposCmd -> SetGuidance("Set y position of collimator");
fCollimatoryposCmd -> SetParameterName("CollimatorYPosition", true); 
fCollimatoryposCmd -> SetDefaultValue(0); 
fCollimatoryposCmd -> SetDefaultUnit("cm");

fCollimatorzposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/corrado/collimatorzpos", this);
fCollimatorzposCmd -> SetGuidance("Set z position of collimator");
fCollimatorzposCmd -> SetParameterName("CollimatorZPosition", true); 
fCollimatorzposCmd -> SetDefaultValue(0); 
fCollimatorzposCmd -> SetDefaultUnit("cm");

fHoleRadiusCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/corrado/holeradius", this);
fHoleRadiusCmd -> SetGuidance("Set crystal hole radius");
fHoleRadiusCmd -> SetParameterName("HoleRadius", true); 
fHoleRadiusCmd -> SetDefaultValue(0.55); 
fHoleRadiusCmd -> SetDefaultUnit("cm");

fHoleLengthCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/corrado/holelength", this);
fHoleLengthCmd -> SetGuidance("Setcrystal hole length");
fHoleLengthCmd -> SetParameterName("HoleLength", true); 
fHoleLengthCmd -> SetDefaultValue(4.3); 
fHoleLengthCmd -> SetDefaultUnit("cm");

}

// --------------------------------------------------------------------------- 

GECorradoDetectorMessenger::~GECorradoDetectorMessenger()
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
  
  delete fSideDeadThicknessCmd;
  delete fFrontDeadThicknessCmd;

  delete fCollimatorxposCmd;
  delete fCollimatoryposCmd;
  delete fCollimatorzposCmd;
  delete fHoleRadiusCmd;
  delete fHoleLengthCmd;  

}

// --------------------------------------------------------------------------- 

void GECorradoDetectorMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
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
  else if (cmd == fFrontDeadThicknessCmd)
    {
       fDetectorPointer->SetFrontDeadThickness (fFrontDeadThicknessCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fSideDeadThicknessCmd)
    {
       fDetectorPointer->SetSideDeadThickness (fSideDeadThicknessCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fCollimatorxposCmd)
    {
       fDetectorPointer->SetCollimatorxpos (fCollimatorxposCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fCollimatoryposCmd)
    {
       fDetectorPointer->SetCollimatorypos (fCollimatoryposCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fCollimatorzposCmd)
    {
       fDetectorPointer->SetCollimatorzpos (fCollimatorzposCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fHoleRadiusCmd)
    {
       fDetectorPointer->SetHoleRadius(fHoleRadiusCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fHoleLengthCmd)
    {
       fDetectorPointer->SetHoleLength(fHoleLengthCmd->GetNewDoubleValue(newvalue));
    }
  

  
}

// --------------------------------------------------------------------------- 
