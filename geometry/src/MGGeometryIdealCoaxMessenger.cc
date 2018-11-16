//---------------------------------------------------------------------------//
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
// $Id: MGGeometryIdealCoaxMessenger.cc,v 1.4 2005/03/31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeometryIdealCoaxMessenger.cc
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
 * AUTHOR: R.
 * CONTACT: Henning
 * FIRST SUBMISSION: Mon Nov 22 17:42:38 PST 2004
 * 
 * REVISION:
 *
 * 11-22-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 * 12-13-2004, Renamed to MGGeometryIdealCoaxMessenger, R. Henning
 *             Added messenger definitions.
 * 03-21-2009, Added fExtraDeadLayerConsidered messenger definitions            
 * 04-13-2011, Added fBeveledEdge and fExcessSideDeadLayer messenger definitions, M. Kidd  
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4ios.hh"
#include "G4ApplicationState.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"

#include "geometry/MGGeometryIdealCoaxCrystal.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "geometry/MGGeometryIdealCoaxMessenger.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeometryIdealCoaxMessenger::MGGeometryIdealCoaxMessenger(
MGGeometryIdealCoaxCrystal *xtal) : fIdealCoaxCrystal(xtal)
{
  // /MG/geometry/idealCoax
  fIdealCoaxDirectory = new G4UIdirectory("/MG/geometry/idealCoax/");

  fSetNameActCmd = new G4UIcmdWithABool("/MG/geometry/idealCoax/activateSetName", this);
  fSetNameActCmd->SetGuidance("Set to true if you want to concatenate to name of active volume");
  fSetNameActCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/idealCoax/setName

  fSetNameCmd = new G4UIcmdWithAString("/MG/geometry/idealCoax/setName", this);
  fSetNameCmd->SetGuidance("Set additional name of active volume in crystal");
  fSetNameCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/idealCoax/bubbleOn
  fBubbleOnCmd = new G4UIcmdWithABool("/MG/geometry/idealCoax/bubbleOn", this);
  fBubbleOnCmd->SetGuidance("Set to true if crystal has core bubble.");
  fBubbleOnCmd->SetGuidance("Set to false if crystal has core bubble.");
  fBubbleOnCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/idealCoax/coreDeadLayer
  fCoreDeadLayerThicknessCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/idealCoax/coreDeadLayer", this);
  fCoreDeadLayerThicknessCmd->SetGuidance("Set thickness of core deadlayer.");
  fCoreDeadLayerThicknessCmd->AvailableForStates(G4State_PreInit);
  fCoreDeadLayerThicknessCmd->SetDefaultUnit("cm");
  fCoreDeadLayerThicknessCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/coreDepth
  fCoreDepthCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/idealCoax/coreDepth", this);
  fCoreDepthCmd->SetGuidance("Set depth of core.");
  fCoreDepthCmd->AvailableForStates(G4State_PreInit);
  fCoreDepthCmd->SetDefaultUnit("cm");
  fCoreDepthCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/coreRadius
  fCoreRadiusCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/idealCoax/coreRadius", this);
  fCoreRadiusCmd->SetGuidance("Set radius of core.");
  fCoreRadiusCmd->AvailableForStates(G4State_PreInit);
  fCoreRadiusCmd->SetDefaultUnit("cm");
  fCoreRadiusCmd->SetUnitCandidates("mm cm m micrometer");
  
  // /MG/geometry/idealCoax/cornerRadius
  fCornerRadiusCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/idealCoax/cornerRadius", this);
  fCornerRadiusCmd->SetGuidance("Set corner radius of cyrstal edge.");
  fCornerRadiusCmd->AvailableForStates(G4State_PreInit);
  fCornerRadiusCmd->SetDefaultUnit("cm");
  fCornerRadiusCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/cutInHalfOn
  fCutInHalfCmd = new G4UIcmdWithABool("/MG/geometry/idealCoax/cutInHalfOn", 
				      this);
  fCutInHalfCmd->SetGuidance("Set to true to cut crystal in half lengthwise.");
  fCutInHalfCmd->SetGuidance("Useful for debugging.");
  fCutInHalfCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/idealCoax/deadLayerOn
  fDeadLayerOnCmd = new G4UIcmdWithABool("/MG/geometry/idealCoax/deadLayerOn",
					 this);
  fDeadLayerOnCmd->SetGuidance("Activate/inactve dead layer as separate");
  fDeadLayerOnCmd->SetGuidance("volume during tracking.");
  fDeadLayerOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/idealCoax/ExcessSideDeadLayerOn  -  M. Kidd, 04/13/2011
  fExcessSideDeadLayerConsideredCmd = new G4UIcmdWithABool(
     "/MG/geometry/idealCoax/ExcessSideDeadLayerOn", this);
  fExcessSideDeadLayerConsideredCmd->SetGuidance("Activate/inactivate extra side dead layer.");
  fExcessSideDeadLayerConsideredCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/idealCoax/SetExcessSideDeadLayer  - M. Kidd, 04/13/2011
  fExcessSideDeadLayerThicknessCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/SetExcessSideDeadLayer", this);
  fExcessSideDeadLayerThicknessCmd->SetGuidance("Set the value of the extra dead layer you want added to the side.");
  fExcessSideDeadLayerThicknessCmd->AvailableForStates(G4State_PreInit);
  fExcessSideDeadLayerThicknessCmd->SetDefaultUnit("mm");
  fExcessSideDeadLayerThicknessCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/ExtraDeadLayerOn - Vic Gehman, March 31, 2009...
  fExtraDeadLayerConsideredCmd = new G4UIcmdWithABool("/MG/geometry/idealCoax/ExtraDeadLayerOn", this);
  fExtraDeadLayerConsideredCmd->SetGuidance("Activate/inactve extra dead layer as separate");
  fExtraDeadLayerConsideredCmd->SetGuidance("volume during tracking.");
  fExtraDeadLayerConsideredCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/idealCoax/SetExtraDeadLayerBottomZ
  fExtraDeadLayerBottomZCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/SetExtraDeadLayerBottomZ", this);
  fExtraDeadLayerBottomZCmd->SetGuidance("Set z-coordinate of lowest bottom dead layer point");
  fExtraDeadLayerBottomZCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerBottomZCmd->SetDefaultUnit("cm");
  fExtraDeadLayerBottomZCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/SetExtraDeadLayerMiddleZ
  fExtraDeadLayerMiddleZCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/SetExtraDeadLayerMiddleZ", this);
  fExtraDeadLayerMiddleZCmd->SetGuidance("Set z-coordinate of middle bottom dead layer point");
  fExtraDeadLayerMiddleZCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerMiddleZCmd->SetDefaultUnit("cm");
  fExtraDeadLayerMiddleZCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/SetExtraDeadLayerTopZ
  fExtraDeadLayerTopZCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/SetExtraDeadLayerTopZ", this);
  fExtraDeadLayerTopZCmd->SetGuidance("Set z-coordinate of top bottom dead layer point");
  fExtraDeadLayerTopZCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerTopZCmd->SetDefaultUnit("cm");
  fExtraDeadLayerTopZCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/SetExtraDeadLayerBottomInnerR
  fExtraDeadLayerBottomInnerRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/SetExtraDeadLayerBottomInnerR", this);
  fExtraDeadLayerBottomInnerRCmd->SetGuidance("Set inner r-coordinate of lowest bottom dead layer point");
  fExtraDeadLayerBottomInnerRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerBottomInnerRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerBottomInnerRCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/SetExtraDeadLayerMiddleInnerR
  fExtraDeadLayerMiddleInnerRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/SetExtraDeadLayerMiddleInnerR", this);
  fExtraDeadLayerMiddleInnerRCmd->SetGuidance("Set inner r-coordinate of middle bottom dead layer point");
  fExtraDeadLayerMiddleInnerRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerMiddleInnerRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerMiddleInnerRCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/SetExtraDeadLayerTopInnerR
  fExtraDeadLayerTopInnerRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/SetExtraDeadLayerTopInnerR", this);
  fExtraDeadLayerTopInnerRCmd->SetGuidance("Set inner r-coordinate of top bottom dead layer point");
  fExtraDeadLayerTopInnerRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerTopInnerRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerTopInnerRCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/SetExtraDeadLayerBottomOuterR
  fExtraDeadLayerBottomOuterRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/SetExtraDeadLayerBottomOuterR", this);
  fExtraDeadLayerBottomOuterRCmd->SetGuidance("Set outer r-coordinate of lowest bottom dead layer point");
  fExtraDeadLayerBottomOuterRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerBottomOuterRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerBottomOuterRCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/SetExtraDeadLayerMiddleOuterR
  fExtraDeadLayerMiddleOuterRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/SetExtraDeadLayerMiddleOuterR", this);
  fExtraDeadLayerMiddleOuterRCmd->SetGuidance("Set outer r-coordinate of middle bottom dead layer point");
  fExtraDeadLayerMiddleOuterRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerMiddleOuterRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerMiddleOuterRCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/SetExtraDeadLayerTopOuterR
  fExtraDeadLayerTopOuterRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/SetExtraDeadLayerTopOuterR", this);
  fExtraDeadLayerTopOuterRCmd->SetGuidance("Set outer r-coordinate of top bottom dead layer point");
  fExtraDeadLayerTopOuterRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerTopOuterRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerTopOuterRCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/dump
  fDumpCmd = new G4UIcmdWithoutParameter("/MG/geometry/idealCoax/dump", this);
  fDumpCmd->SetGuidance("Dump current settings of crystal to screen.");

  // /MG/geometry/idealCoax/height
  fHeightCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/height",
					     this);
  fHeightCmd->SetGuidance("Set height of crystal.");
  fHeightCmd->AvailableForStates(G4State_PreInit);
  fHeightCmd->SetDefaultUnit("cm");
  fHeightCmd->SetUnitCandidates("mm cm m micrometer");
  
  // /MG/geometry/idealCoax/massAndVolume
  // (Available for all states.)
  fMassAndVolumeCmd = new G4UIcmdWithoutParameter(
			     "/MG/geometry/idealCoax/massAndVolume", this);
  fMassAndVolumeCmd->SetGuidance("Compute and display crystal mass and ");
  fMassAndVolumeCmd->SetGuidance("volume.");

  // /MG/geometry/idealCoax/material
  fMaterialCmd = new G4UIcmdWithAString("/MG/geometry/idealCoax/material",
					 this);
  fMaterialCmd->SetGuidance("Set Geant4 name of crystal material. Must be ");
  fMaterialCmd->SetGuidance("defined in database or in code.");
  fMaterialCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/idealCoax/outerDeadLayer
  fOuterDeadLayerThicknessCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/idealCoax/outerDeadLayer", this);
  fOuterDeadLayerThicknessCmd->SetGuidance("Set thickness of outer deadlayer");
  fOuterDeadLayerThicknessCmd->AvailableForStates(G4State_PreInit);
  fOuterDeadLayerThicknessCmd->SetDefaultUnit("cm");
  fOuterDeadLayerThicknessCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/radius
  fRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/idealCoax/radius",
					     this);
  fRadiusCmd->SetGuidance("Set outer radius of crystal.");
  fRadiusCmd->AvailableForStates(G4State_PreInit);
  fRadiusCmd->SetDefaultUnit("cm");
  fRadiusCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/idealCoax/setDefaults
  fSetDefaultsCmd = new G4UIcmdWithoutParameter(
     "/MG/geometry/idealCoax/setDefaults", this);
  fSetDefaultsCmd->SetGuidance("Set hard-coded default values for crystal ");
  fSetDefaultsCmd->SetGuidance("dimensions. See ");
  fSetDefaultsCmd->SetGuidance("MGGeometryIdealCoaxCrystal::SetDefaults()");
  fSetDefaultsCmd->AvailableForStates(G4State_PreInit);

}

//---------------------------------------------------------------------------//

MGGeometryIdealCoaxMessenger::~MGGeometryIdealCoaxMessenger()
{
  delete fSetNameActCmd;
  delete fSetNameCmd;
  delete fBubbleOnCmd;
  delete fCoreDeadLayerThicknessCmd;
  delete fCoreDepthCmd;
  delete fCoreRadiusCmd;
  delete fCornerRadiusCmd;
  delete fExtraDeadLayerConsideredCmd;//added by Vic Gehman, 04/01/2009
  delete fExtraDeadLayerBottomZCmd;
  delete fExtraDeadLayerMiddleZCmd;
  delete fExtraDeadLayerTopZCmd;
  delete fExtraDeadLayerBottomInnerRCmd;
  delete fExtraDeadLayerMiddleInnerRCmd;
  delete fExtraDeadLayerTopInnerRCmd;
  delete fExtraDeadLayerBottomOuterRCmd;
  delete fExtraDeadLayerMiddleOuterRCmd;
  delete fExtraDeadLayerTopOuterRCmd;
  delete fCutInHalfCmd;
  delete fDeadLayerOnCmd;
  delete fDumpCmd;
  delete fHeightCmd;
  delete fMassAndVolumeCmd;
  delete fMaterialCmd;
  delete fOuterDeadLayerThicknessCmd;
  delete fRadiusCmd;
  delete fSetDefaultsCmd;
  delete fIdealCoaxDirectory;
}

//---------------------------------------------------------------------------//

void MGGeometryIdealCoaxMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fBubbleOnCmd)
    fIdealCoaxCrystal->SetCoreBubble(fBubbleOnCmd->GetNewBoolValue(nV));
  else if(cmd == fSetNameActCmd)
    fIdealCoaxCrystal->ActivateSetName(fSetNameActCmd->GetNewBoolValue(nV));
  else if(cmd == fSetNameActCmd)
    fIdealCoaxCrystal->SetName(nV);
  else if(cmd == fCoreDeadLayerThicknessCmd)
    fIdealCoaxCrystal->SetCoreDeadLayerThickness(
      fCoreDeadLayerThicknessCmd->GetNewDoubleValue(nV));
  else if(cmd == fCoreDepthCmd)
    fIdealCoaxCrystal->SetCoreDepth(fCoreDepthCmd->GetNewDoubleValue(nV));
  else if(cmd == fCoreRadiusCmd)
    fIdealCoaxCrystal->SetCoreRadius(fCoreRadiusCmd->GetNewDoubleValue(nV));
  else if(cmd == fCornerRadiusCmd)
    fIdealCoaxCrystal->SetCornerRadius(
      fCornerRadiusCmd->GetNewDoubleValue(nV));
  else if(cmd == fCutInHalfCmd)
    fIdealCoaxCrystal->SetCutInHalf(fCutInHalfCmd->GetNewBoolValue(nV));
  else if(cmd == fDeadLayerOnCmd)
    fIdealCoaxCrystal->SetDeadLayerConsidered(
      fDeadLayerOnCmd->GetNewBoolValue(nV));
  else if(cmd == fExcessSideDeadLayerConsideredCmd)  // M. Kidd, 04/13/2011
    fIdealCoaxCrystal->SetExcessSideDeadLayerConsidered(fExcessSideDeadLayerConsideredCmd->GetNewBoolValue(nV));
  else if(cmd == fExcessSideDeadLayerThicknessCmd)   // M. Kidd, 04/13/2011
    fIdealCoaxCrystal->SetExcessSideDeadLayerThickness(fExcessSideDeadLayerThicknessCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerConsideredCmd)//Added by Vic Gehman, 04/01/2009
    fIdealCoaxCrystal->SetExtraDeadLayerConsidered(
      fExtraDeadLayerConsideredCmd->GetNewBoolValue(nV));
  else if(cmd == fExtraDeadLayerBottomZCmd) fIdealCoaxCrystal->SetExtraDeadLayerBottomZ(fExtraDeadLayerBottomZCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerMiddleZCmd) fIdealCoaxCrystal->SetExtraDeadLayerMiddleZ(fExtraDeadLayerMiddleZCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerTopZCmd) fIdealCoaxCrystal->SetExtraDeadLayerTopZ(fExtraDeadLayerTopZCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerBottomInnerRCmd) fIdealCoaxCrystal->SetExtraDeadLayerBottomInnerR(fExtraDeadLayerBottomInnerRCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerMiddleInnerRCmd) fIdealCoaxCrystal->SetExtraDeadLayerMiddleInnerR(fExtraDeadLayerMiddleInnerRCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerTopInnerRCmd) fIdealCoaxCrystal->SetExtraDeadLayerTopInnerR(fExtraDeadLayerTopInnerRCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerBottomOuterRCmd) fIdealCoaxCrystal->SetExtraDeadLayerBottomOuterR(fExtraDeadLayerBottomOuterRCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerMiddleOuterRCmd) fIdealCoaxCrystal->SetExtraDeadLayerMiddleOuterR(fExtraDeadLayerMiddleOuterRCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerTopOuterRCmd) fIdealCoaxCrystal->SetExtraDeadLayerTopOuterR(fExtraDeadLayerTopInnerRCmd->GetNewDoubleValue(nV));
  else if(cmd == fDumpCmd) //Back to the regular idealCoax...
    fIdealCoaxCrystal->Dump();
  else if(cmd == fHeightCmd)
    fIdealCoaxCrystal->SetHeight(fHeightCmd->GetNewDoubleValue(nV));
  else if(cmd == fMassAndVolumeCmd) {
    fIdealCoaxCrystal->CalculateMassAndVolume();
    G4cout << "Mass (grams): " << 
      fIdealCoaxCrystal->GetCrystalMass() / g << G4endl;
    G4cout << "Volume (cm^3): " <<
      fIdealCoaxCrystal->GetCrystalVolume() / cm3 << G4endl;
  } else if(cmd == fMaterialCmd)
    fIdealCoaxCrystal->SetG4MaterialName(nV);
  else if(cmd == fOuterDeadLayerThicknessCmd)
    fIdealCoaxCrystal->SetOuterDeadLayerThickness(
      fOuterDeadLayerThicknessCmd->GetNewDoubleValue(nV));
  else if(cmd == fRadiusCmd)
    fIdealCoaxCrystal->SetRadius(fRadiusCmd->GetNewDoubleValue(nV));
  else if(cmd == fSetDefaultsCmd)
    fIdealCoaxCrystal->SetDefaults();

}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
