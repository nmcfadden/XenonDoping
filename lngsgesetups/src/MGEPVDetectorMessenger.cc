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
// $Id: MGEPVDetectorMessenger.cc,v 1.0 2010-02-26 18:06:00 matthiasl Exp $ 
//      
// CLASS IMPLEMENTATION:  MGEPVDetectorMessenger.cc
//	to be used for GsOr, GePV and GePaolo
//
/**
 * SPECIAL NOTES:
 */
// 
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

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh" 
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "lngsgesetups/MGEPVDetectorMessenger.hh"
#include "lngsgesetups/MGEPVDetector.hh" 

// --------------------------------------------------------------------------- 

MGEPVDetectorMessenger::MGEPVDetectorMessenger(MGEPVDetector *dete) :
  fDetectorPointer(dete) 
{
  // directories 
  fDetectorDir =  new G4UIdirectory("/MG/geometry/mgepv/"); 
  fDetectorDir -> SetGuidance("Tune sample parameter for MGEPV detector"); 
  
  fSampleTypeCmd =  new G4UIcmdWithAString("/MG/geometry/mgepv/sample", this);
  fSampleTypeCmd -> SetGuidance("Select the shape of the sample");
  fSampleTypeCmd -> SetParameterName("SampleType", true); 
  fSampleTypeCmd -> SetDefaultValue("smarinelli"); 
  fSampleTypeCmd -> SetCandidates("sphere box tube sbox liquid twobox smarinelli custom"); 
 
  fSampleMaterialCmd =  new G4UIcmdWithAString("/MG/geometry/mgepv/samplematerial", this);
  fSampleMaterialCmd -> SetGuidance("Select the material of the sample");
  fSampleMaterialCmd -> SetParameterName("SampleMaterial", true); 
  fSampleMaterialCmd -> SetDefaultValue("NitrogenGas"); 


  fSMarinelliHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/smarinelliheight", this);
  fSMarinelliHeightCmd -> SetGuidance("Set the height of the empty space in the small Marinelli");
  fSMarinelliHeightCmd -> SetParameterName("SMarinelliHeight", true); 
  fSMarinelliHeightCmd -> SetDefaultValue(0.0); 
  fSMarinelliHeightCmd -> SetDefaultUnit("cm"); 

  fSBoxHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/sboxheight", this);
  fSBoxHeightCmd->SetGuidance("Set the height of the std sample box");
  fSBoxHeightCmd->SetParameterName("SBoxHeight", true);
  fSBoxHeightCmd->SetDefaultValue(3.8); 
  fSBoxHeightCmd-> SetDefaultUnit("cm");
  
  fSBoxIDiameterCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/sboxinnerd", this);
  fSBoxIDiameterCmd -> SetGuidance("Set the inner diameter of the std box");
  fSBoxIDiameterCmd -> SetParameterName("SBoxIDiameter", true); 
  fSBoxIDiameterCmd -> SetDefaultValue(7.0); 
  fSBoxIDiameterCmd -> SetDefaultUnit("cm"); 

  fSBoxODiameterCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/sboxouterd", this);
  fSBoxODiameterCmd -> SetGuidance("Set the outer diameter of the std box");
  fSBoxODiameterCmd -> SetParameterName("SBoxODiameter", true); 
  fSBoxODiameterCmd -> SetDefaultValue(7.2); 
  fSBoxODiameterCmd -> SetDefaultUnit("cm"); 

  fSupportHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/supportheight", this);
  fSupportHeightCmd->SetGuidance("Set the height of the std sample box");
  fSupportHeightCmd->SetParameterName("SupportHeight", true);
  fSupportHeightCmd->SetDefaultValue(0.2); 
  fSupportHeightCmd-> SetDefaultUnit("cm");
  
  fSupportIDiameterCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/supportinnerd", this);
  fSupportIDiameterCmd -> SetGuidance("Set the inner diameter of the std box");
  fSupportIDiameterCmd -> SetParameterName("SupportIDiameter", true); 
  fSupportIDiameterCmd -> SetDefaultValue(5.5); 
  fSupportIDiameterCmd -> SetDefaultUnit("cm"); 

  fSupportODiameterCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/supportouterd", this);
  fSupportODiameterCmd -> SetGuidance("Set the outer diameter of the std box");
  fSupportODiameterCmd -> SetParameterName("SupportODiameter", true); 
  fSupportODiameterCmd -> SetDefaultValue(9.5); 
  fSupportODiameterCmd -> SetDefaultUnit("cm"); 

  fBoxHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/boxheight", this);
  fBoxHeightCmd->SetGuidance("Set the height of the sample box");
  fBoxHeightCmd->SetParameterName("BoxHeight", true);
  fBoxHeightCmd->SetDefaultValue(2.0); 
  fBoxHeightCmd-> SetDefaultUnit("cm");
  
  fBoxWidthCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/boxwidth", this);
  fBoxWidthCmd -> SetGuidance("Set the width of the sample box");
  fBoxWidthCmd -> SetParameterName("BoxWidth", true); 
  fBoxWidthCmd -> SetDefaultValue(2.0); 
  fBoxWidthCmd -> SetDefaultUnit("cm"); 

  fBoxThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/boxthickness", this);
  fBoxThicknessCmd -> SetGuidance("Set the thickness of the sample box");
  fBoxThicknessCmd -> SetParameterName("BoxThickness", true); 
  fBoxThicknessCmd -> SetDefaultValue(2.0); 
  fBoxThicknessCmd -> SetDefaultUnit("cm"); 

  fTubeInnerRadiusCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/tubeinnerradius", this);
  fTubeInnerRadiusCmd -> SetGuidance("Set the inner radius of the sample tube");
  fTubeInnerRadiusCmd -> SetParameterName("TubeInnerRadius", true); 
  fTubeInnerRadiusCmd -> SetDefaultValue(0.); 
  fTubeInnerRadiusCmd -> SetDefaultUnit("cm"); 

  fTubeOuterRadiusCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/tubeouterradius", this);
  fTubeOuterRadiusCmd -> SetGuidance("Set the outer radius of the sample tube");
  fTubeOuterRadiusCmd -> SetParameterName("TubeOuterRadius", true); 
  fTubeOuterRadiusCmd -> SetDefaultValue(2.0); 
  fTubeOuterRadiusCmd -> SetDefaultUnit("cm");

  fTubeLengthCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/tubelength", this);
  fTubeLengthCmd -> SetGuidance("Set the length of the sample tube");
  fTubeLengthCmd -> SetParameterName("TubeLength", true); 
  fTubeLengthCmd -> SetDefaultValue(2.0); 
  fTubeLengthCmd -> SetDefaultUnit("cm"); 

  fSamplexposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/samplexpos", this);
  fSamplexposCmd -> SetGuidance("Set the x coordinate of the center of the sample");
  fSamplexposCmd -> SetParameterName("Samplexpos", true); 
  fSamplexposCmd -> SetDefaultValue(0.); 
  fSamplexposCmd -> SetDefaultUnit("cm"); 

  fSampleyposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/sampleypos", this);
  fSampleyposCmd -> SetGuidance("Set the y coordinate of the center of the sample");
  fSampleyposCmd -> SetParameterName("Sampleypos", true); 
  fSampleyposCmd -> SetDefaultValue(0.); 
  fSampleyposCmd -> SetDefaultUnit("cm"); 

  fSamplezposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/samplezpos", this);
  fSamplezposCmd -> SetGuidance("Set the z coordinate of the center of the sample");
  fSamplezposCmd -> SetParameterName("Samplezpos", true); 
  fSamplezposCmd -> SetDefaultValue(14.); 
  fSamplezposCmd -> SetDefaultUnit("cm"); 

  fDetzposCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/detzpos", this);
  fDetzposCmd -> SetGuidance("Set z distance from detector window to crystal surface");
  fDetzposCmd -> SetParameterName("Detzpos", true); 
  fDetzposCmd -> SetDefaultValue(0.4); 
  fDetzposCmd -> SetDefaultUnit("cm"); 

  fDeadThicknessCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/deadthickness", this);
  fDeadThicknessCmd -> SetGuidance("Set dead layer thickness");
  fDeadThicknessCmd -> SetParameterName("DeadThickness", true); 
  fDeadThicknessCmd -> SetDefaultValue(0.15); 
  fDeadThicknessCmd -> SetDefaultUnit("cm"); 

  fHoleDiameterCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/holediameter", this);
  fHoleDiameterCmd -> SetGuidance("Set hole diameter (including dead layer)");
  fHoleDiameterCmd -> SetParameterName("HoleDiameter", true); 
  fHoleDiameterCmd -> SetDefaultValue(0.97);
  fHoleDiameterCmd -> SetDefaultUnit("cm"); 

  fHoleLengthCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/holelength", this);
  fHoleLengthCmd -> SetGuidance("Set hole length (including dead layer)");
  fHoleLengthCmd -> SetParameterName("HoleLength", true); 
  fHoleLengthCmd -> SetDefaultValue(6.57);
  fHoleLengthCmd -> SetDefaultUnit("cm"); 

  fDetDiameterCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/detdiameter", this);
  fDetDiameterCmd -> SetGuidance("Set crystal outer diameter (including dead layer)");
  fDetDiameterCmd -> SetParameterName("DetDiameter", true); 
  fDetDiameterCmd -> SetDefaultValue(7.72);
  fDetDiameterCmd -> SetDefaultUnit("cm"); 

  fDetLengthCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/mgepv/detlength", this);
  fDetLengthCmd -> SetGuidance("Set crystal length (including dead layer)");
  fDetLengthCmd -> SetParameterName("DetLength", true); 
  fDetLengthCmd -> SetDefaultValue(7.87);
  fDetLengthCmd -> SetDefaultUnit("cm"); 
  /*
    fDetSizeFactorCmd =  new G4UIcmdWithADouble("/MG/geometry/dario/detsizefactor", this);
    fDetSizeFactorCmd -> SetGuidance("Set multiplication factor of crystal size (not working)");
    fDetSizeFactorCmd -> SetParameterName("DetSizeFactor", true); 
    fDetSizeFactorCmd -> SetDefaultValue(1);
  */
}

// --------------------------------------------------------------------------- 

MGEPVDetectorMessenger::~MGEPVDetectorMessenger()
{

  delete fDetectorDir;
  delete fSampleTypeCmd;
  delete fSampleMaterialCmd;
  delete fSMarinelliHeightCmd;
  delete fSBoxHeightCmd;
  delete fSBoxIDiameterCmd;
  delete fSBoxODiameterCmd;
  delete fSupportHeightCmd;
  delete fSupportIDiameterCmd;
  delete fSupportODiameterCmd;
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
  delete fHoleDiameterCmd;
  delete fHoleLengthCmd;
  delete fDetDiameterCmd;
  delete fDetLengthCmd;
  //delete fDetSizeFactorCmd;
  
}

// --------------------------------------------------------------------------- 

void MGEPVDetectorMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
{
  if (cmd == fSampleTypeCmd)
    {
      fDetectorPointer->SetSampleType(newvalue);
    }
  else if (cmd == fSampleMaterialCmd)
    {
      fDetectorPointer->SetSampleMaterial(newvalue);
    }
  else if (cmd == fSMarinelliHeightCmd)
    {
      fDetectorPointer->SetSMarinelliHeight(fSMarinelliHeightCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fSBoxHeightCmd)
    {
      fDetectorPointer->SetSBoxHeight(fSBoxHeightCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fSBoxIDiameterCmd)
    {
      fDetectorPointer->SetSBoxIDiameter(fSBoxIDiameterCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fSBoxODiameterCmd)
    {
      fDetectorPointer->SetSBoxODiameter(fSBoxODiameterCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fSupportHeightCmd)
    {
      fDetectorPointer->SetSupportHeight(fSupportHeightCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fSupportIDiameterCmd)
    {
      fDetectorPointer->SetSupportIDiameter(fSupportIDiameterCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fSupportODiameterCmd)
    {
      fDetectorPointer->SetSupportODiameter(fSupportODiameterCmd->GetNewDoubleValue(newvalue));
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
  else if (cmd == fHoleDiameterCmd)
    {
      fDetectorPointer->SetHoleDiameter(fHoleDiameterCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fHoleLengthCmd)
    {
      fDetectorPointer->SetHoleLength(fHoleLengthCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fDetDiameterCmd)
    {
      fDetectorPointer->SetDetDiameter(fDetDiameterCmd->GetNewDoubleValue(newvalue));
    }
  else if (cmd == fDetLengthCmd)
    {
      fDetectorPointer->SetDetLength(fDetLengthCmd->GetNewDoubleValue(newvalue));
    }
  
}

// --------------------------------------------------------------------------- 
