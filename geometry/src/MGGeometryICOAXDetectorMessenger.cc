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
// $Id: MGGeometryICOAXDetectorMessenger.cc,v 1.0 2016/13/12
//      
// CLASS IMPLEMENTATION:  MGGeometryICOAXDetectorMessenger.cc
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
 * AUTHOR: Y. Kermaidic
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 * 12-13-2016, Created, Y. Kermaidic
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

#include "geometry/MGGeometryICOAXDetector.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "geometry/MGGeometryICOAXDetectorMessenger.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

MGGeometryICOAXDetectorMessenger::MGGeometryICOAXDetectorMessenger(
MGGeometryICOAXDetector *xtal) : fICOAXDetector(xtal)
{
  // /MG/geometry/ICOAX
  fICOAXDetectorDirectory = new G4UIdirectory("/MG/geometry/ICOAX/");

  fSetNameActCmd = new G4UIcmdWithABool("/MG/geometry/ICOAX/activateSetName", this);
  fSetNameActCmd->SetGuidance("Set to true if you want to concatenate to name of active volume");
  fSetNameActCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/ICOAX/setName

  fSetNameCmd = new G4UIcmdWithAString("/MG/geometry/ICOAX/setName", this);
  fSetNameCmd->SetGuidance("Set additional name of active volume in crystal");
  fSetNameCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/ICOAX/bubbleOn
  fBubbleOnCmd = new G4UIcmdWithABool("/MG/geometry/ICOAX/bubbleOn", this);
  fBubbleOnCmd->SetGuidance("Set to true if crystal has core bubble.");
  fBubbleOnCmd->SetGuidance("Set to false if crystal has core bubble.");
  fBubbleOnCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/ICOAX/coreDeadLayer
  fCoreDeadLayerThicknessCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/ICOAX/coreDeadLayer", this);
  fCoreDeadLayerThicknessCmd->SetGuidance("Set thickness of core deadlayer.");
  fCoreDeadLayerThicknessCmd->AvailableForStates(G4State_PreInit);
  fCoreDeadLayerThicknessCmd->SetDefaultUnit("cm");
  fCoreDeadLayerThicknessCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/coreDepth
  fCoreDepthCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/ICOAX/coreDepth", this);
  fCoreDepthCmd->SetGuidance("Set depth of core.");
  fCoreDepthCmd->AvailableForStates(G4State_PreInit);
  fCoreDepthCmd->SetDefaultUnit("cm");
  fCoreDepthCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/coreRadius
  fCoreRadiusCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/ICOAX/coreRadius", this);
  fCoreRadiusCmd->SetGuidance("Set radius of core.");
  fCoreRadiusCmd->AvailableForStates(G4State_PreInit);
  fCoreRadiusCmd->SetDefaultUnit("cm");
  fCoreRadiusCmd->SetUnitCandidates("mm cm m micrometer");
  
  // /MG/geometry/ICOAX/coreRounding
  fCoreRoundingCmd = new G4UIcmdWithABool(
						 "/MG/geometry/ICOAX/coreRounding", this);
  fCoreRoundingCmd->SetGuidance("Set core rounding or not.");
  fCoreRoundingCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/ICOAX/cornerRadius
  fCornerRadiusCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/ICOAX/cornerRadius", this);
  fCornerRadiusCmd->SetGuidance("Set corner radius of cyrstal edge.");
  fCornerRadiusCmd->AvailableForStates(G4State_PreInit);
  fCornerRadiusCmd->SetDefaultUnit("cm");
  fCornerRadiusCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/cutInHalfOn
  fCutInHalfCmd = new G4UIcmdWithABool("/MG/geometry/ICOAX/cutInHalfOn", 
				      this);
  fCutInHalfCmd->SetGuidance("Set to true to cut crystal in half lengthwise.");
  fCutInHalfCmd->SetGuidance("Useful for debugging.");
  fCutInHalfCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/ICOAX/deadLayerOn
  fDeadLayerOnCmd = new G4UIcmdWithABool("/MG/geometry/ICOAX/deadLayerOn",
					 this);
  fDeadLayerOnCmd->SetGuidance("Activate/inactve dead layer as separate");
  fDeadLayerOnCmd->SetGuidance("volume during tracking.");
  fDeadLayerOnCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/ICOAX/ExcessSideDeadLayerOn  -  M. Kidd, 04/13/2011
  fExcessSideDeadLayerConsideredCmd = new G4UIcmdWithABool(
     "/MG/geometry/ICOAX/ExcessSideDeadLayerOn", this);
  fExcessSideDeadLayerConsideredCmd->SetGuidance("Activate/inactivate extra side dead layer.");
  fExcessSideDeadLayerConsideredCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/ICOAX/SetExcessSideDeadLayer  - M. Kidd, 04/13/2011
  fExcessSideDeadLayerThicknessCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/SetExcessSideDeadLayer", this);
  fExcessSideDeadLayerThicknessCmd->SetGuidance("Set the value of the extra dead layer you want added to the side.");
  fExcessSideDeadLayerThicknessCmd->AvailableForStates(G4State_PreInit);
  fExcessSideDeadLayerThicknessCmd->SetDefaultUnit("mm");
  fExcessSideDeadLayerThicknessCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/beveledEdgeOn  -  Y. Kermaidic, 13/12/2016
  fBeveledEdgeConsideredCmd = new G4UIcmdWithABool(
     "/MG/geometry/ICOAX/beveledEdgeOn", this);
  fBeveledEdgeConsideredCmd->SetGuidance("Consider beveled detector or not.");
  fBeveledEdgeConsideredCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/ICOAX/beveledEdgeAngle  -  Y. Kermaidic, 13/12/2016
  fBeveledEdgeAngleCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/ICOAX/beveledEdgeAngle", this);
  fBeveledEdgeAngleCmd->SetGuidance("Set angle of bevel.");
  fBeveledEdgeAngleCmd->AvailableForStates(G4State_PreInit);
  fBeveledEdgeAngleCmd->SetDefaultUnit("deg");
  fBeveledEdgeAngleCmd->SetUnitCandidates("deg rad");

  // /MG/geometry/ICOAX/beveledEdgeT  -  Y. Kermaidic, 13/12/2016
  fBeveledEdgeTCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/ICOAX/beveledEdgeT", this);
  fBeveledEdgeTCmd->SetGuidance("Set edge of bevel.");
  fBeveledEdgeTCmd->AvailableForStates(G4State_PreInit);
  fBeveledEdgeTCmd->SetDefaultUnit("cm");
  fBeveledEdgeTCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/ditchInnerRadius  -  Y. Kermaidic, 13/12/2016
  fDitchInnerRadiusCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/ICOAX/ditchInnerRadius", this);
  fDitchInnerRadiusCmd->SetGuidance("Set inner radius of ditch.");
  fDitchInnerRadiusCmd->AvailableForStates(G4State_PreInit);
  fDitchInnerRadiusCmd->SetDefaultUnit("cm");
  fDitchInnerRadiusCmd->SetUnitCandidates("mm cm m micrometer");
 
 // /MG/geometry/ICOAX/ditchOuterRadius  -  Y. Kermaidic, 13/12/2016
  fDitchOuterRadiusCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/ICOAX/ditchOuterRadius", this);
  fDitchOuterRadiusCmd->SetGuidance("Set outer radius of ditch.");
  fDitchOuterRadiusCmd->AvailableForStates(G4State_PreInit);
  fDitchOuterRadiusCmd->SetDefaultUnit("cm");
  fDitchOuterRadiusCmd->SetUnitCandidates("mm cm m micrometer");

 // /MG/geometry/ICOAX/ditchDepth  -  Y. Kermaidic, 13/12/2016
  fDitchDepthCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/ICOAX/ditchDepth", this);
  fDitchDepthCmd->SetGuidance("Set depth of ditch.");
  fDitchDepthCmd->AvailableForStates(G4State_PreInit);
  fDitchDepthCmd->SetDefaultUnit("cm");
  fDitchDepthCmd->SetUnitCandidates("mm cm m micrometer");

  /*
  // /MG/geometry/ICOAX/ExtraDeadLayerOn - Vic Gehman, March 31, 2009...
  fExtraDeadLayerConsideredCmd = new G4UIcmdWithABool("/MG/geometry/ICOAX/ExtraDeadLayerOn", this);
  fExtraDeadLayerConsideredCmd->SetGuidance("Activate/inactve extra dead layer as separate");
  fExtraDeadLayerConsideredCmd->SetGuidance("volume during tracking.");
  fExtraDeadLayerConsideredCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/ICOAX/SetExtraDeadLayerBottomZ
  fExtraDeadLayerBottomZCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/SetExtraDeadLayerBottomZ", this);
  fExtraDeadLayerBottomZCmd->SetGuidance("Set z-coordinate of lowest bottom dead layer point");
  fExtraDeadLayerBottomZCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerBottomZCmd->SetDefaultUnit("cm");
  fExtraDeadLayerBottomZCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/SetExtraDeadLayerMiddleZ
  fExtraDeadLayerMiddleZCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/SetExtraDeadLayerMiddleZ", this);
  fExtraDeadLayerMiddleZCmd->SetGuidance("Set z-coordinate of middle bottom dead layer point");
  fExtraDeadLayerMiddleZCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerMiddleZCmd->SetDefaultUnit("cm");
  fExtraDeadLayerMiddleZCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/SetExtraDeadLayerTopZ
  fExtraDeadLayerTopZCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/SetExtraDeadLayerTopZ", this);
  fExtraDeadLayerTopZCmd->SetGuidance("Set z-coordinate of top bottom dead layer point");
  fExtraDeadLayerTopZCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerTopZCmd->SetDefaultUnit("cm");
  fExtraDeadLayerTopZCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/SetExtraDeadLayerBottomInnerR
  fExtraDeadLayerBottomInnerRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/SetExtraDeadLayerBottomInnerR", this);
  fExtraDeadLayerBottomInnerRCmd->SetGuidance("Set inner r-coordinate of lowest bottom dead layer point");
  fExtraDeadLayerBottomInnerRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerBottomInnerRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerBottomInnerRCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/SetExtraDeadLayerMiddleInnerR
  fExtraDeadLayerMiddleInnerRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/SetExtraDeadLayerMiddleInnerR", this);
  fExtraDeadLayerMiddleInnerRCmd->SetGuidance("Set inner r-coordinate of middle bottom dead layer point");
  fExtraDeadLayerMiddleInnerRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerMiddleInnerRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerMiddleInnerRCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/SetExtraDeadLayerTopInnerR
  fExtraDeadLayerTopInnerRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/SetExtraDeadLayerTopInnerR", this);
  fExtraDeadLayerTopInnerRCmd->SetGuidance("Set inner r-coordinate of top bottom dead layer point");
  fExtraDeadLayerTopInnerRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerTopInnerRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerTopInnerRCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/SetExtraDeadLayerBottomOuterR
  fExtraDeadLayerBottomOuterRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/SetExtraDeadLayerBottomOuterR", this);
  fExtraDeadLayerBottomOuterRCmd->SetGuidance("Set outer r-coordinate of lowest bottom dead layer point");
  fExtraDeadLayerBottomOuterRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerBottomOuterRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerBottomOuterRCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/SetExtraDeadLayerMiddleOuterR
  fExtraDeadLayerMiddleOuterRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/SetExtraDeadLayerMiddleOuterR", this);
  fExtraDeadLayerMiddleOuterRCmd->SetGuidance("Set outer r-coordinate of middle bottom dead layer point");
  fExtraDeadLayerMiddleOuterRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerMiddleOuterRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerMiddleOuterRCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/SetExtraDeadLayerTopOuterR
  fExtraDeadLayerTopOuterRCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/SetExtraDeadLayerTopOuterR", this);
  fExtraDeadLayerTopOuterRCmd->SetGuidance("Set outer r-coordinate of top bottom dead layer point");
  fExtraDeadLayerTopOuterRCmd->AvailableForStates(G4State_PreInit);
  fExtraDeadLayerTopOuterRCmd->SetDefaultUnit("cm");
  fExtraDeadLayerTopOuterRCmd->SetUnitCandidates("mm cm m micrometer");
*/

  // /MG/geometry/ICOAX/dump
  fDumpCmd = new G4UIcmdWithoutParameter("/MG/geometry/ICOAX/dump", this);
  fDumpCmd->SetGuidance("Dump current settings of crystal to screen.");

  // /MG/geometry/ICOAX/height
  fHeightCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/height",
					     this);
  fHeightCmd->SetGuidance("Set height of crystal.");
  fHeightCmd->AvailableForStates(G4State_PreInit);
  fHeightCmd->SetDefaultUnit("cm");
  fHeightCmd->SetUnitCandidates("mm cm m micrometer");
  
  // /MG/geometry/ICOAX/massAndVolume
  // (Available for all states.)
  fMassAndVolumeCmd = new G4UIcmdWithoutParameter(
			     "/MG/geometry/ICOAX/massAndVolume", this);
  fMassAndVolumeCmd->SetGuidance("Compute and display crystal mass and ");
  fMassAndVolumeCmd->SetGuidance("volume.");

  // /MG/geometry/ICOAX/material
  fMaterialCmd = new G4UIcmdWithAString("/MG/geometry/ICOAX/material",
					 this);
  fMaterialCmd->SetGuidance("Set Geant4 name of crystal material. Must be ");
  fMaterialCmd->SetGuidance("defined in database or in code.");
  fMaterialCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/ICOAX/outerDeadLayer
  fOuterDeadLayerThicknessCmd = new G4UIcmdWithADoubleAndUnit(
     "/MG/geometry/ICOAX/outerDeadLayer", this);
  fOuterDeadLayerThicknessCmd->SetGuidance("Set thickness of outer deadlayer");
  fOuterDeadLayerThicknessCmd->AvailableForStates(G4State_PreInit);
  fOuterDeadLayerThicknessCmd->SetDefaultUnit("cm");
  fOuterDeadLayerThicknessCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/radius
  fRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/ICOAX/radius",
					     this);
  fRadiusCmd->SetGuidance("Set outer radius of crystal.");
  fRadiusCmd->AvailableForStates(G4State_PreInit);
  fRadiusCmd->SetDefaultUnit("cm");
  fRadiusCmd->SetUnitCandidates("mm cm m micrometer");

  // /MG/geometry/ICOAX/setDefaults
  fSetDefaultsCmd = new G4UIcmdWithoutParameter(
     "/MG/geometry/ICOAX/setDefaults", this);
  fSetDefaultsCmd->SetGuidance("Set hard-coded default values for crystal ");
  fSetDefaultsCmd->SetGuidance("dimensions. See ");
  fSetDefaultsCmd->SetGuidance("MGGeometryICOAXDetector::SetDefaults()");
  fSetDefaultsCmd->AvailableForStates(G4State_PreInit);

}

//---------------------------------------------------------------------------//

MGGeometryICOAXDetectorMessenger::~MGGeometryICOAXDetectorMessenger()
{
  delete fSetNameActCmd;
  delete fSetNameCmd;
  delete fBubbleOnCmd;
  delete fCoreDeadLayerThicknessCmd;
  delete fCoreDepthCmd;
  delete fCoreRadiusCmd;
  delete fCornerRadiusCmd;
  /*
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
  */
  delete fCutInHalfCmd;
  delete fDeadLayerOnCmd;
  delete fDumpCmd;
  delete fHeightCmd;
  delete fMassAndVolumeCmd;
  delete fMaterialCmd;
  delete fOuterDeadLayerThicknessCmd;
  delete fRadiusCmd;
  delete fSetDefaultsCmd;
  delete fICOAXDetectorDirectory;
  delete fBeveledEdgeConsideredCmd;
  delete fBeveledEdgeAngleCmd;
  delete fBeveledEdgeTCmd;
  delete fDitchInnerRadiusCmd;
  delete fDitchOuterRadiusCmd;
  delete fDitchDepthCmd;
}

//---------------------------------------------------------------------------//

void MGGeometryICOAXDetectorMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fBubbleOnCmd)
    fICOAXDetector->SetCoreBubble(fBubbleOnCmd->GetNewBoolValue(nV));
  else if(cmd == fSetNameActCmd)
    fICOAXDetector->ActivateSetName(fSetNameActCmd->GetNewBoolValue(nV));
  else if(cmd == fSetNameActCmd)
    fICOAXDetector->SetName(nV);
  else if(cmd == fCoreDeadLayerThicknessCmd)
    fICOAXDetector->SetCoreDeadLayerThickness(
      fCoreDeadLayerThicknessCmd->GetNewDoubleValue(nV));
  else if(cmd == fCoreDepthCmd)
    fICOAXDetector->SetCoreDepth(fCoreDepthCmd->GetNewDoubleValue(nV));
  else if(cmd == fCoreRadiusCmd)
    fICOAXDetector->SetCoreRadius(fCoreRadiusCmd->GetNewDoubleValue(nV));
  else if(cmd == fCoreRoundingCmd)
    fICOAXDetector->SetCoreRounding(fCoreRoundingCmd->GetNewBoolValue(nV));
  else if(cmd == fCornerRadiusCmd)
    fICOAXDetector->SetCornerRadius(
      fCornerRadiusCmd->GetNewDoubleValue(nV));
  else if(cmd == fCutInHalfCmd)
    fICOAXDetector->SetCutInHalf(fCutInHalfCmd->GetNewBoolValue(nV));
  else if(cmd == fDeadLayerOnCmd)
    fICOAXDetector->SetDeadLayerConsidered(
      fDeadLayerOnCmd->GetNewBoolValue(nV));
  else if(cmd == fExcessSideDeadLayerConsideredCmd)  // M. Kidd, 04/13/2011
    fICOAXDetector->SetExcessSideDeadLayerConsidered(fExcessSideDeadLayerConsideredCmd->GetNewBoolValue(nV));
  else if(cmd == fExcessSideDeadLayerThicknessCmd)   // M. Kidd, 04/13/2011
    fICOAXDetector->SetExcessSideDeadLayerThickness(fExcessSideDeadLayerThicknessCmd->GetNewDoubleValue(nV));
  else if(cmd == fDumpCmd) //Back to the regular ICOAX...
    fICOAXDetector->Dump();
  else if(cmd == fHeightCmd)
    fICOAXDetector->SetHeight(fHeightCmd->GetNewDoubleValue(nV));
  else if(cmd == fMassAndVolumeCmd) {
    fICOAXDetector->CalculateMassAndVolume();
    G4cout << "Mass (grams): " << 
      fICOAXDetector->GetCrystalMass() / g << G4endl;
    G4cout << "Volume (cm^3): " <<
      fICOAXDetector->GetCrystalVolume() / cm3 << G4endl;
  } else if(cmd == fMaterialCmd)
    fICOAXDetector->SetG4MaterialName(nV);
  else if(cmd == fOuterDeadLayerThicknessCmd)
    fICOAXDetector->SetOuterDeadLayerThickness(
      fOuterDeadLayerThicknessCmd->GetNewDoubleValue(nV));
  else if(cmd == fRadiusCmd)
    fICOAXDetector->SetRadius(fRadiusCmd->GetNewDoubleValue(nV));
  else if(cmd == fSetDefaultsCmd)
    fICOAXDetector->SetDefaults();
  else if(cmd == fBeveledEdgeConsideredCmd)                            //Added by Y. Kermaidic, 13/12/2016
    fICOAXDetector->SetBeveledEdgeConsidered(
      fBeveledEdgeConsideredCmd->GetNewBoolValue(nV));
 else if(cmd == fBeveledEdgeAngleCmd)                                        //Added by Y. Kermaidic, 13/12/2016
   fICOAXDetector->SetBeveledEdgeAngle(fBeveledEdgeAngleCmd->GetNewDoubleValue(nV));
 else if(cmd == fBeveledEdgeTCmd)                                         //Added by Y. Kermaidic, 13/12/2016
    fICOAXDetector->SetBeveledEdgeT(fBeveledEdgeTCmd->GetNewDoubleValue(nV));
 else if(cmd == fDitchInnerRadiusCmd)                                         //Added by Y. Kermaidic, 13/12/2016
    fICOAXDetector->SetDitchInnerRadius(fDitchInnerRadiusCmd->GetNewDoubleValue(nV));
 else if(cmd == fDitchOuterRadiusCmd)                                         //Added by Y. Kermaidic, 13/12/2016
    fICOAXDetector->SetDitchOuterRadius(fDitchOuterRadiusCmd->GetNewDoubleValue(nV));
 else if(cmd == fDitchDepthCmd)                                         //Added by Y. Kermaidic, 13/12/2016
    fICOAXDetector->SetDitchDepth(fDitchDepthCmd->GetNewDoubleValue(nV));

  /*
  else if(cmd == fExtraDeadLayerConsideredCmd)//Added by Vic Gehman, 04/01/2009
    fICOAXDetector->SetExtraDeadLayerConsidered(
      fExtraDeadLayerConsideredCmd->GetNewBoolValue(nV));
  else if(cmd == fExtraDeadLayerBottomZCmd) fICOAXDetector->SetExtraDeadLayerBottomZ(fExtraDeadLayerBottomZCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerMiddleZCmd) fICOAXDetector->SetExtraDeadLayerMiddleZ(fExtraDeadLayerMiddleZCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerTopZCmd) fICOAXDetector->SetExtraDeadLayerTopZ(fExtraDeadLayerTopZCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerBottomInnerRCmd) fICOAXDetector->SetExtraDeadLayerBottomInnerR(fExtraDeadLayerBottomInnerRCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerMiddleInnerRCmd) fICOAXDetector->SetExtraDeadLayerMiddleInnerR(fExtraDeadLayerMiddleInnerRCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerTopInnerRCmd) fICOAXDetector->SetExtraDeadLayerTopInnerR(fExtraDeadLayerTopInnerRCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerBottomOuterRCmd) fICOAXDetector->SetExtraDeadLayerBottomOuterR(fExtraDeadLayerBottomOuterRCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerMiddleOuterRCmd) fICOAXDetector->SetExtraDeadLayerMiddleOuterR(fExtraDeadLayerMiddleOuterRCmd->GetNewDoubleValue(nV));
  else if(cmd == fExtraDeadLayerTopOuterRCmd) fICOAXDetector->SetExtraDeadLayerTopOuterR(fExtraDeadLayerTopInnerRCmd->GetNewDoubleValue(nV));
  */
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
