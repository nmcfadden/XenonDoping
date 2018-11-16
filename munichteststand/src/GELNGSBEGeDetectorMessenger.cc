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
 * 05-26-2009, Added in MaGe, Luciano
 * 04-28-2010, Added command to set detector type, Luciano
 *
 */
// ---------------------------------------------------------------------------

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh" 
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "munichteststand/GELNGSBEGeDetectorMessenger.hh"
#include "munichteststand/GELNGSBEGeDetector.hh" 
#include "io/MGLogger.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GELNGSBEGeDetectorMessenger::GELNGSBEGeDetectorMessenger(GELNGSBEGeDetector *dete) :
  fDetectorPointer(dete) 
{
  // directories 
  fDetectorDir =  new G4UIdirectory("/MG/geometry/LNGSBEGe/"); 
  fDetectorDir -> SetGuidance("Tune sample parameter for LNGSBEGe detector"); 
  
  fTopDLCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/LNGSBEGe/top_lateral_DL",this);
  fTopDLCmd->SetGuidance("Set the thickness of the top and lateral dead layers");
  fTopDLCmd->SetGuidance("of the LNGS BEGe detector");
  fTopDLCmd->SetParameterName("ThicknessTop", true);
  fTopDLCmd->SetDefaultUnit("mm");
  fTopDLCmd->AvailableForStates( G4State_PreInit);

  fBottomDLCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/LNGSBEGe/bottom_DL",this);
  fBottomDLCmd->SetGuidance("Set the thickness of the bottom dead layers");
  fBottomDLCmd->SetGuidance("of the LNGS BEGe detector");
  fBottomDLCmd->SetParameterName("ThicknessBottom", true);
  fBottomDLCmd->SetDefaultUnit("mm");
  fBottomDLCmd->AvailableForStates( G4State_PreInit);

  fDetectorTypeCmd = new G4UIcmdWithAString("/MG/geometry/LNGSBEGe/detectorType",this);
  fDetectorTypeCmd->SetGuidance("Sets the detector type. Available candidates are: ");
  fDetectorTypeCmd->SetGuidance("Mi --> Milano BEGE (default)");
  fDetectorTypeCmd->SetGuidance("CC --> CC depBEGE");
  fDetectorTypeCmd->SetGuidance("DD --> DD depBEGE");
  fDetectorTypeCmd->SetCandidates("Mi CC DD ");
  fDetectorTypeCmd->AvailableForStates(G4State_PreInit);
}

// --------------------------------------------------------------------------- 

GELNGSBEGeDetectorMessenger::~GELNGSBEGeDetectorMessenger()
{

  delete fDetectorDir;
  delete fTopDLCmd;
  delete fBottomDLCmd;
  delete fDetectorTypeCmd;
}

// --------------------------------------------------------------------------- 

void GELNGSBEGeDetectorMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
{
  if (cmd == fTopDLCmd)
    {
      G4double val = fTopDLCmd->GetNewDoubleValue(newvalue);
      if (val>0 && val < 2.0*mm)
	{
	  fDetectorPointer->SetTopLateralDeadLayer(val);
	  MGLog(trace) << "Thickness of top/lateral dead layer set to " << 
	    fDetectorPointer->GetTopLateralDeadLayer()/mm << " mm" << endlog;
	}
    }
  else if (cmd == fBottomDLCmd)
    {
      G4double val = fBottomDLCmd->GetNewDoubleValue(newvalue);
      if (val>0 && val < 2.0*mm)
	{
	  fDetectorPointer->SetBottomDeadLayer(val);
	  MGLog(trace) << "Thickness of bottom dead layer set to " << 
	    fDetectorPointer->GetBottomDeadLayer()/mm << " mm" << endlog;
	}
    }
  else if (cmd == fDetectorTypeCmd)    
    {
      fDetectorPointer->SetDetectorType(newvalue);    
      MGLog(trace) << "BEGe detector type set to " << newvalue << endlog;
    }
}


// --------------------------------------------------------------------------- 
