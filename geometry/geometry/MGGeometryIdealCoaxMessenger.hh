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
/**                                                            
 * $Id: MGGeometryIdealCoaxMessenger.hh,v 1.2 2005/01/06 01:09:02 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGGeometryIdealCoaxMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * 
 * Messenger class for ideal coaxial crystal. 
 * Define dimensions using G4 macros.
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
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
 * 04-13-2011, Added fBeveledEdge and fExcessSideDeadLayer messenger definitions  
 */
// --------------------------------------------------------------------------//

#ifndef _MGGEOMETRYIDEALCOAXMESSENGER_HH
#define _MGGEOMETRYIDEALCOAXMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

class MGGeometryIdealCoaxCrystal;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;


//---------------------------------------------------------------------------//

class MGGeometryIdealCoaxMessenger : public G4UImessenger
{
public:

  //default constructor
  MGGeometryIdealCoaxMessenger(MGGeometryIdealCoaxCrystal*);

  //destructor
  ~MGGeometryIdealCoaxMessenger();

  void SetNewValue(G4UIcommand*, G4String);

  //protected members
protected:


private:
  G4UIdirectory               *fIdealCoaxDirectory;
  MGGeometryIdealCoaxCrystal  *fIdealCoaxCrystal; 
  G4UIcmdWithABool            *fSetNameActCmd;
  G4UIcmdWithAString          *fSetNameCmd;
  G4UIcmdWithABool            *fBubbleOnCmd;
  G4UIcmdWithADoubleAndUnit   *fCoreDeadLayerThicknessCmd;
  G4UIcmdWithADoubleAndUnit   *fCoreDepthCmd;
  G4UIcmdWithADoubleAndUnit   *fCoreRadiusCmd;
  G4UIcmdWithADoubleAndUnit   *fCornerRadiusCmd;
  G4UIcmdWithABool            *fExcessSideDeadLayerConsideredCmd; //M. Kidd, 4/13/2011
  G4UIcmdWithADoubleAndUnit   *fExcessSideDeadLayerThicknessCmd;  //M. Kidd, 4/13/2011
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerBottomZCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerMiddleZCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerTopZCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerBottomInnerRCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerMiddleInnerRCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerTopInnerRCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerBottomOuterRCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerMiddleOuterRCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerTopOuterRCmd;
  G4UIcmdWithABool            *fCutInHalfCmd;
  G4UIcmdWithABool            *fDeadLayerOnCmd;
  G4UIcmdWithABool            *fExtraDeadLayerConsideredCmd;
  G4UIcmdWithoutParameter     *fDumpCmd;
  G4UIcmdWithADoubleAndUnit   *fHeightCmd;
  G4UIcmdWithoutParameter     *fMassAndVolumeCmd;
  G4UIcmdWithAString          *fMaterialCmd;
  G4UIcmdWithADoubleAndUnit   *fOuterDeadLayerThicknessCmd;
  G4UIcmdWithADoubleAndUnit   *fRadiusCmd;
  G4UIcmdWithoutParameter     *fSetDefaultsCmd;
};
#endif
