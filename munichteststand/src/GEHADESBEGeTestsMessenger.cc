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
// $Id: GELNGSBEGeDetectorMessenger.cc,v 1.1 2009-05-26 08:58:20 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  GELNGSBEGeDetectorMessenger.cc
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
 * 12-21-2011, Derived from GELNGSBEGeDetectorMessenger class and implemented as new class in MaGe
 * 01-26-2012, Added Source Messengers, Bjoern
 *
 */
// ---------------------------------------------------------------------------

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh" 
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"


#include "munichteststand/GEHADESBEGeTestsMessenger.hh"
#include "munichteststand/GEHADESBEGeTests.hh" 
#include "io/MGLogger.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEHADESBEGeTestsMessenger::GEHADESBEGeTestsMessenger(GEHADESBEGeTests *dete) :
  fDetectorPointer(dete) 
{
  // directories 
  fDetectorDir =  new G4UIdirectory("/MG/geometry/HADESBEGe/"); 
  fDetectorDir -> SetGuidance("Tune sample parameter for HADESBEGe detector"); 
  

  fCryostatWindowThicknessCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/CryostatWindowThickness",this);
  fCryostatWindowThicknessCmd->SetGuidance("Sets cryostat window thickness ");
  fCryostatWindowThicknessCmd->SetDefaultUnit("mm");
  fCryostatWindowThicknessCmd->AvailableForStates( G4State_PreInit);

  fCryostatWallThicknessCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/CryostatWallThickness",this);
  fCryostatWallThicknessCmd->SetGuidance("Sets cryostat wall thickness ");
  fCryostatWallThicknessCmd->SetDefaultUnit("mm");
  fCryostatWallThicknessCmd->AvailableForStates( G4State_PreInit);

  fCryostatDiameterCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/CryostatDiameter",this);
  fCryostatDiameterCmd->SetGuidance("Sets cryostat diameter ");
  fCryostatDiameterCmd->SetDefaultUnit("mm");
  fCryostatDiameterCmd->AvailableForStates( G4State_PreInit);

  fCryostatHeightCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/CryostatHeight",this);
  fCryostatHeightCmd->SetGuidance("Sets cryostat height ");
  fCryostatHeightCmd->SetDefaultUnit("mm");
  fCryostatHeightCmd->AvailableForStates( G4State_PreInit);


  fXtalDiameterCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/XtalDiameter",this);
  fXtalDiameterCmd->SetGuidance("Sets crystal diameter (incl. DL)");
  fXtalDiameterCmd->SetDefaultUnit("mm");
  fXtalDiameterCmd->AvailableForStates( G4State_PreInit);

  fXtalHeightCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/XtalHeight",this);
  fXtalHeightCmd->SetGuidance("Sets crystal height (incl. DL)");
  fXtalHeightCmd->SetDefaultUnit("mm");
  fXtalHeightCmd->AvailableForStates( G4State_PreInit);

  fXtalDistanceToWindowCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/XtalDistanceToWindow",this);
  fXtalDistanceToWindowCmd->SetGuidance("Sets distance of crystal top to cryostat window");
  fXtalDistanceToWindowCmd->SetDefaultUnit("mm");
  fXtalDistanceToWindowCmd->AvailableForStates( G4State_PreInit);

  fXtalDitchInnerRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/XtalDitchInnerRadius",this);
  fXtalDitchInnerRadiusCmd->SetGuidance("Sets inner radius of ditch ");
  fXtalDitchInnerRadiusCmd->SetDefaultUnit("mm");
  fXtalDitchInnerRadiusCmd->AvailableForStates( G4State_PreInit);

  fXtalDitchOuterRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/XtalDitchOuterRadius",this);
  fXtalDitchOuterRadiusCmd->SetGuidance("Sets outer radius for ditch ");
  fXtalDitchOuterRadiusCmd->SetDefaultUnit("mm");
  fXtalDitchOuterRadiusCmd->AvailableForStates( G4State_PreInit);

  fXtalDitchDepthCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/XtalDitchDepth",this);
  fXtalDitchDepthCmd->SetGuidance("Sets depth of ditch ");
  fXtalDitchDepthCmd->SetDefaultUnit("mm");
  fXtalDitchDepthCmd->AvailableForStates( G4State_PreInit);

  fXtalDitchOnBottomCmd = new G4UIcmdWithABool("/MG/geometry/HADESBEGe/XtalDitchOnBottom",this);
  fXtalDitchOnBottomCmd->SetGuidance("Sets the ditch to a side of the detector (default: bottom side) ");
  fXtalDitchOnBottomCmd->AvailableForStates( G4State_PreInit);

  fXtalCornerDiameterCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/XtalCornerDiameter",this);
  fXtalCornerDiameterCmd->SetGuidance("Sets diameter of top/bottom side with edge ");
  fXtalCornerDiameterCmd->SetDefaultUnit("mm");
  fXtalCornerDiameterCmd->AvailableForStates( G4State_PreInit);

  fXtalCornerHeightCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/XtalCornerHeight",this);
  fXtalCornerHeightCmd->SetGuidance("Sets height from top/bottom side to the end of the edge ");
  fXtalCornerHeightCmd->SetDefaultUnit("mm");
  fXtalCornerHeightCmd->AvailableForStates( G4State_PreInit);

  fXtalCornerOnBottomCmd = new G4UIcmdWithABool("/MG/geometry/HADESBEGe/XtalCornerOnBottom",this);
  fXtalCornerOnBottomCmd->SetGuidance("Sets the edge to a side of the detector (default: top side) ");
  fXtalCornerOnBottomCmd->AvailableForStates( G4State_PreInit);


  fSpecialDetectorTypeCmd = new G4UIcmdWithAString("/MG/geometry/HADESBEGe/SpecialDetectorType",this);
  fSpecialDetectorTypeCmd->SetGuidance("Sets special detector type. Available candidates are: ");
  fSpecialDetectorTypeCmd->SetGuidance("noting implemented");
  fSpecialDetectorTypeCmd->SetCandidates("");
  fSpecialDetectorTypeCmd->AvailableForStates(G4State_PreInit);

  fSourceTypeCmd = new G4UIcmdWithAString("/MG/geometry/HADESBEGe/SourceType",this);
  fSourceTypeCmd->SetGuidance("Sets the source type. Available candidates are: ");
  fSourceTypeCmd->SetCandidates("Tueb LNGSWireSource HS6 PTBPointlike AlPillUR AlPillUD Cerca HS29 HS20 HS20like HS29like HS21 HS21like HS7 HS7like SSG0 SSG1 SSG2 SSG3 SSG4 SSG5 SSG6 flat");
  fSourceTypeCmd->AvailableForStates(G4State_PreInit);

  fSourceDistanceCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/SourceDistance",this);
  fSourceDistanceCmd->SetGuidance("Sets distance of source above endcap ");
  fSourceDistanceCmd->SetDefaultUnit("cm");
  fSourceDistanceCmd->AvailableForStates( G4State_PreInit);

  fActivateEnrBEGeCryostatHoldersCmd = new G4UIcmdWithABool("/MG/geometry/HADESBEGe/ActivateEnrBEGeCryostatHolders",this);
  fActivateEnrBEGeCryostatHoldersCmd->SetGuidance("Activates the holder, cup and base for an enriched BEGe");
  fActivateEnrBEGeCryostatHoldersCmd->AvailableForStates( G4State_PreInit);

  fActivateDepBEGeCryostatHoldersCmd = new G4UIcmdWithABool("/MG/geometry/HADESBEGe/ActivateDepBEGeCryostatHolders",this);
  fActivateDepBEGeCryostatHoldersCmd->SetGuidance("Activates the holder, cup and base for a depleted BEGe");
  fActivateDepBEGeCryostatHoldersCmd->AvailableForStates( G4State_PreInit);


  fSetHADESLeadCastleTypeCmd = new G4UIcmdWithAnInteger("/MG/geometry/HADESBEGe/SetLeadCastleType",this);
  fSetHADESLeadCastleTypeCmd->SetGuidance("Sets HADES lead castle type. (Default 0=none)");
  fSetHADESLeadCastleTypeCmd->AvailableForStates( G4State_PreInit);



  fSetBRADYEnvCmd = new G4UIcmdWithABool("/MG/geometry/HADESBEGe/SetBRADYEnv",this);
  fSetBRADYEnvCmd->SetGuidance("Activates the BRADY geometry (Padova scanning table) ");
  fSetBRADYEnvCmd->AvailableForStates( G4State_PreInit);

  fSetCollimatorCmd = new G4UIcmdWithAnInteger("/MG/geometry/HADESBEGe/SetCollimator",this);
  fSetCollimatorCmd->SetGuidance("Sets Collimator to 0 = false, 1 = Side, 2 = Top");
  fSetCollimatorCmd->SetGuidance("Reference is the top/mid of the endcap of the Vacuum Cryostat.");  
  fSetCollimatorCmd->AvailableForStates(G4State_PreInit);
  
  fCollimatorDistanceCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/SetCollimatorDistance",this);
  fCollimatorDistanceCmd->SetGuidance("Sets Collimator distance to endcap.");
  fCollimatorDistanceCmd->SetDefaultUnit("mm");
  fCollimatorDistanceCmd->AvailableForStates(G4State_PreInit);
  
  fCollimatorPositionCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/HADESBEGe/SetCollimatorPosition",this);
  fCollimatorPositionCmd->SetGuidance("Sets the position of the collimator and the source in mm.");
  fCollimatorPositionCmd->SetGuidance("Reference is the top of the endcap of the Vacuum Cryostat for Side Scan.");  
  fCollimatorPositionCmd->SetGuidance("And mid of endcap for the Top Scan.");  
  fCollimatorPositionCmd->SetDefaultUnit("mm");
  fCollimatorPositionCmd->AvailableForStates(G4State_PreInit);

  fCryostatFillMaterialCmd = new G4UIcmdWithAString("/MG/geometry/HADESBEGe/CryostatFillMaterial",this);
  fCryostatFillMaterialCmd->SetGuidance("Sets the cryostat fill material. Default is vacuum. Available candidates are: ");
  fCryostatFillMaterialCmd->SetCandidates("Vacuum LiquidArgon");
  fCryostatFillMaterialCmd->AvailableForStates(G4State_PreInit);


}

// --------------------------------------------------------------------------- 

GEHADESBEGeTestsMessenger::~GEHADESBEGeTestsMessenger()
{

  delete fDetectorDir;
 
  delete fCryostatWindowThicknessCmd;
  delete fCryostatWallThicknessCmd;
  delete fCryostatDiameterCmd;
  delete fCryostatHeightCmd;

  delete fXtalDiameterCmd;
  delete fXtalHeightCmd;
  delete fXtalDistanceToWindowCmd;
  delete fXtalDitchInnerRadiusCmd;
  delete fXtalDitchOuterRadiusCmd;
  delete fXtalDitchDepthCmd;
  delete fXtalDitchOnBottomCmd;
  delete fXtalCornerDiameterCmd;
  delete fXtalCornerHeightCmd;
  delete fXtalCornerOnBottomCmd;

  delete fSpecialDetectorTypeCmd;;
  delete fSourceTypeCmd;
  delete fSourceDistanceCmd;

  delete fActivateEnrBEGeCryostatHoldersCmd;
  delete fActivateDepBEGeCryostatHoldersCmd;
  
  delete fSetHADESLeadCastleTypeCmd;
  
  delete fSetBRADYEnvCmd;
  delete fSetCollimatorCmd;
  delete fCollimatorDistanceCmd;
  delete fCollimatorPositionCmd;

  delete fCryostatFillMaterialCmd;
}

// --------------------------------------------------------------------------- 

void GEHADESBEGeTestsMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
{

  if (cmd == fCryostatWindowThicknessCmd)
    {
      G4double val = fCryostatWindowThicknessCmd->GetNewDoubleValue(newvalue);
      if (val>0 && val < 5.0*mm)
	{
	  fDetectorPointer->SetCryostatWindowThickness(val);
	  MGLog(trace) << "Cryostat window thickness set to " << 
	    fDetectorPointer->GetCryostatWindowThickness()/mm << " mm" << endlog;
	}
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetCryostatWindowThickness()/mm << " mm" << endlog;}
    }
  else if (cmd == fCryostatWallThicknessCmd)
    {
      G4double val = fCryostatWallThicknessCmd->GetNewDoubleValue(newvalue);
      if (val>0 && val < 5.0*mm)
	{
	  fDetectorPointer->SetCryostatWallThickness(val);
	  MGLog(trace) << "Cryostat wall thickness set to " << 
	    fDetectorPointer->GetCryostatWallThickness()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetCryostatWallThickness()/mm << " mm" << endlog;}
    }
  else if (cmd == fCryostatDiameterCmd)
    {
      G4double val = fCryostatDiameterCmd->GetNewDoubleValue(newvalue);
      if (val>50. && val < 1500.*mm)
	{
	  fDetectorPointer->SetCryostatDiameter(val);
	  MGLog(trace) << "Cryostat diameter set to " << 
	    fDetectorPointer->GetCryostatDiameter()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetCryostatDiameter()/mm << " mm" << endlog;}
    }
  else if (cmd == fCryostatHeightCmd)
    {
      G4double val = fCryostatHeightCmd->GetNewDoubleValue(newvalue);
      if (val>50. && val < 5000.*mm)
      {
	  fDetectorPointer->SetCryostatHeight(val);
	  MGLog(trace) << "Cryostat height set to " << 
	    fDetectorPointer->GetCryostatHeight()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetCryostatHeight()/mm << " mm" << endlog;}
    } 
  else if (cmd == fXtalDiameterCmd)
    {
      G4double val = fXtalDiameterCmd->GetNewDoubleValue(newvalue);
      if (val>20. && val < 100.*mm)
	{
	  fDetectorPointer->SetXtalDiameter(val);
	  MGLog(trace) << "Crystal diameter set to " << 
	    fDetectorPointer->GetXtalDiameter()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetXtalDiameter()/mm << " mm" << endlog;}
    }
  else if (cmd == fXtalHeightCmd)
    {
      G4double val = fXtalHeightCmd->GetNewDoubleValue(newvalue);
      if (val>10. && val < 100.*mm)
	{
	  fDetectorPointer->SetXtalHeight(val);
	  MGLog(trace) << "Crystal height set to " << 
	    fDetectorPointer->GetXtalHeight()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetXtalHeight()/mm << " mm" << endlog;}
    }
  else if (cmd == fXtalDistanceToWindowCmd)
    {
      G4double val = fXtalDistanceToWindowCmd->GetNewDoubleValue(newvalue);
      if (val>0. && val < 1000.*mm)
	{
	  fDetectorPointer->SetXtalDistanceToWindow(val);
	  MGLog(trace) << "Crystal distance to window set to " << 
	    fDetectorPointer->GetXtalDistanceToWindow()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetXtalDistanceToWindow()/mm << " mm" << endlog;}
    }
  else if (cmd == fXtalDitchInnerRadiusCmd)
    {
      G4double val = fXtalDitchInnerRadiusCmd->GetNewDoubleValue(newvalue);
      if (val>0. && val < 100.*mm)
	{
	  fDetectorPointer->SetXtalDitchInnerRadius(val);
	  MGLog(trace) << "Ditch inner radius set to " << 
	    fDetectorPointer->GetXtalDitchInnerRadius()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetXtalDitchInnerRadius()/mm << " mm" << endlog;}
    }
  else if (cmd == fXtalDitchOuterRadiusCmd)
    {
      G4double val = fXtalDitchOuterRadiusCmd->GetNewDoubleValue(newvalue);
      if (val>=0. && val < 100.*mm)
	{
	  fDetectorPointer->SetXtalDitchOuterRadius(val);
	  MGLog(trace) << "Ditch outer radius " << 
	    fDetectorPointer->GetXtalDitchOuterRadius()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetXtalDitchOuterRadius()/mm << " mm" << endlog;}
    }
  else if (cmd == fXtalDitchDepthCmd)
    {
      G4double val = fXtalDitchDepthCmd->GetNewDoubleValue(newvalue);
      if (val>=0. && val < 20.*mm)
	{
	  fDetectorPointer->SetXtalDitchDepth(val);
	  MGLog(trace) << "Ditch depth set to " << 
	    fDetectorPointer->GetXtalDitchDepth()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetXtalDitchDepth()/mm << " mm" << endlog;}
    }
  else if (cmd == fXtalDitchOnBottomCmd)
    {
      G4double val = fXtalDitchOnBottomCmd->GetNewBoolValue(newvalue);
      if (val==true || val==false)
	{
	  fDetectorPointer->SetXtalDitchOnBottom(val);
	  MGLog(trace) << "Crystal ditch on bottom set to " << 
	    fDetectorPointer->GetXtalDitchOnBottom() << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetXtalDitchOnBottom() << endlog;}
    }

  else if (cmd == fXtalCornerDiameterCmd)
    {
      G4double val = fXtalCornerDiameterCmd->GetNewDoubleValue(newvalue);
      if (val>=0. && val < 100.*mm)
	{
	  fDetectorPointer->SetXtalCornerDiameter(val);
	  MGLog(trace) << "Crystal corner diameter set to " << 
	    fDetectorPointer->GetXtalCornerDiameter()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetXtalCornerDiameter()/mm << " mm" << endlog;}
    }
  else if (cmd == fXtalCornerHeightCmd)
    {
      G4double val = fXtalCornerHeightCmd->GetNewDoubleValue(newvalue);
      if (val>=0. && val < 100.*mm)
	{
	  fDetectorPointer->SetXtalCornerHeight(val);
	  MGLog(trace) << "Crystal corner height set to " << 
	    fDetectorPointer->GetXtalCornerHeight()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetXtalCornerHeight()/mm << " mm" << endlog;}
    }
  else if (cmd == fXtalCornerOnBottomCmd)
    {
      G4double val = fXtalCornerOnBottomCmd->GetNewBoolValue(newvalue);
      if (val==true || val==false)
	{
	  fDetectorPointer->SetXtalCornerOnBottom(val);
	  MGLog(trace) << "Crystal corner on bottom set to " << 
	    fDetectorPointer->GetXtalCornerOnBottom() << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	       fDetectorPointer->GetXtalCornerOnBottom() << endlog;}
    }

  else if (cmd == fSpecialDetectorTypeCmd)    
    {
      fDetectorPointer->SetSpecialDetectorType(newvalue);    
      MGLog(trace) << "BEGe detector type set to " << newvalue << endlog;
    }
  else if (cmd == fSourceTypeCmd)    
    {
      fDetectorPointer->SetSourceType(newvalue);    
      MGLog(trace) << "Source type set to " << newvalue << endlog;
    }
  else if (cmd == fSourceDistanceCmd)
    {
      G4double val = fSourceDistanceCmd->GetNewDoubleValue(newvalue);
      if (val>=0. && val < 1000.*mm)
	{
	  fDetectorPointer->SetSourceDistance(val);
	  MGLog(trace) << "Source distance set to " << 
	    fDetectorPointer->GetSourceDistance()/mm << " mm" << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	     fDetectorPointer->GetSourceDistance()/mm << " mm" << endlog;}
   } 

  else if (cmd == fActivateEnrBEGeCryostatHoldersCmd)
    {
      G4bool val = fActivateEnrBEGeCryostatHoldersCmd->GetNewBoolValue(newvalue);
      if (val==true || val==false)
	{
	  fDetectorPointer->SetActivateEnrBEGeCryostatHolders(val);
	  if (val==true)
	    {fDetectorPointer->SetActivateDepBEGeCryostatHolders(false);};
	  MGLog(trace) << "EnrBEGe Holder set to " << 
	    fDetectorPointer->GetActivateEnrBEGeCryostatHolders() << endlog;
	  MGLog(trace) << "DepBEGe Holder set to " << 
	    fDetectorPointer->GetActivateDepBEGeCryostatHolders() << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	    fDetectorPointer->GetActivateEnrBEGeCryostatHolders() << endlog;}
    }


  else if (cmd == fActivateDepBEGeCryostatHoldersCmd)
    {
      G4bool val = fActivateDepBEGeCryostatHoldersCmd->GetNewBoolValue(newvalue);
      if (val==true || val==false)
	{
	  fDetectorPointer->SetActivateDepBEGeCryostatHolders(val);
	  if (val==true)
	    {fDetectorPointer->SetActivateEnrBEGeCryostatHolders(false);};
	  MGLog(trace) << "DepBEGe Holder set to " << 
	    fDetectorPointer->GetActivateDepBEGeCryostatHolders() << endlog;
	  MGLog(trace) << "EnrBEGe Holder set to " << 
	    fDetectorPointer->GetActivateEnrBEGeCryostatHolders() << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	    fDetectorPointer->GetActivateDepBEGeCryostatHolders() << endlog;}
    }

  else if (cmd == fSetHADESLeadCastleTypeCmd)
    {
      G4int val = fSetHADESLeadCastleTypeCmd->GetNewIntValue(newvalue);
      if (val>=0. && val < 3)
	{
	  fDetectorPointer->SetHADESLeadCastleType(val);
	  MGLog(trace) << "HADES lead castle type " <<
	    fDetectorPointer->GetHADESLeadCastleType() << endlog;
	}      
      else{MGLog(trace) << "Wrong input. Value =  " << 
	     fDetectorPointer->GetHADESLeadCastleType() << endlog;}
   }

  // delete fSetHADESLeadCastleTypeCmd;


  else if (cmd == fSetBRADYEnvCmd)
    {
      G4bool val = fSetBRADYEnvCmd->GetNewBoolValue(newvalue);
      if (val==true || val==false)
	{
	  fDetectorPointer->SetBRADYEnv(val);
	  MGLog(trace) << "BRADY Geometry set to " << 
	    fDetectorPointer->GetBRADYEnv() << endlog;
	}      
      else{MGLog(trace) << "BRADY Geometry Wrong input. Value =  " << 
	    fDetectorPointer->GetBRADYEnv() << endlog;}
    }
  else if (cmd == fSetCollimatorCmd)
    {
      G4int num = fSetCollimatorCmd->GetNewIntValue(newvalue);
      if (num>=0 && num<=2)
	{
	  fDetectorPointer->SetCollimator(num);
	  MGLog(trace) << "COLLIMATOR set to " << 
	  fDetectorPointer->GetCollimator() << endlog;
	}
      else{MGLog(trace) << "COLLIMATOR Wrong input. Value =  " << 
	    fDetectorPointer->GetCollimator() << endlog;}      
    }
  else if (cmd == fCollimatorDistanceCmd)
    {
      G4double val = fCollimatorDistanceCmd->GetNewDoubleValue(newvalue);
      if(val>=0. && val <= 100.*mm)
	{
	  fDetectorPointer->SetCollimatorDistance(val);
	  MGLog(trace) << "COLLIMATOR DISTANCE set to " << 
	  fDetectorPointer->GetCollimatorDistance() << endlog;      
	}
      else{MGLog(trace) << "COLLIMATOR DISTANCE Wrong input. Value =  " << 
	    fDetectorPointer->GetCollimatorDistance() << endlog;}
    }
  else if (cmd == fCollimatorPositionCmd)
    {
      G4double val = fCollimatorPositionCmd->GetNewDoubleValue(newvalue);
      if(val<15.*cm && val>(-15.*cm))
	{
	  fDetectorPointer->SetCollimatorPosition(val);
	  MGLog(trace) << "COLLIMATOR POSITION set to " << 
	  fDetectorPointer->GetCollimatorPosition() << endlog;      
	}
      else{MGLog(trace) << "COLLIMATOR POSITION Wrong input. Value =  " << 
	    fDetectorPointer->GetCollimatorPosition() << endlog;}
    }
  else if (cmd == fCryostatFillMaterialCmd)
    {
      fDetectorPointer->SetCryostatFillMaterial(newvalue);
      MGLog(trace) << "Cryostat fill material set to " << newvalue << endlog;
    }

}


// --------------------------------------------------------------------------- 
