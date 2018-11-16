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
 * $Id: MGGeometryICOAXDetectorMessenger.hh,v 1.0 2016/12/13 
 *      
 * CLASS DECLARATION:  MGGeometryICOAXDetectorMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * 
 * Messenger class for ideal ICOAX Well crystal. 
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
 * AUTHOR: Y. Kermaidic
 * CONTACT: yoann.kermaidic@mpi-hd.mpg.de
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 * 
 * 12-13-2016, Created, Y. Kermaidic
 */
// --------------------------------------------------------------------------//

#ifndef _MGGEOMETRYSAGEDETECTORMESSENGER_HH
#define _MGGEOMETRYSAGEDETECTORMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

class MGGeometryICOAXDetector;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;


//---------------------------------------------------------------------------//

class MGGeometryICOAXDetectorMessenger : public G4UImessenger
{
public:

  //default constructor
  MGGeometryICOAXDetectorMessenger(MGGeometryICOAXDetector*);

  //destructor
  ~MGGeometryICOAXDetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

  //protected members
protected:


private:
  G4UIdirectory               *fICOAXDetectorDirectory;
  MGGeometryICOAXDetector      *fICOAXDetector; 
  G4UIcmdWithABool            *fSetNameActCmd;
  G4UIcmdWithAString          *fSetNameCmd;
  G4UIcmdWithABool            *fBubbleOnCmd;
  G4UIcmdWithADoubleAndUnit   *fCoreDeadLayerThicknessCmd;
  G4UIcmdWithADoubleAndUnit   *fCoreDepthCmd;
  G4UIcmdWithADoubleAndUnit   *fCoreRadiusCmd;
  G4UIcmdWithABool            *fCoreRoundingCmd;
  G4UIcmdWithADoubleAndUnit   *fCornerRadiusCmd;
  G4UIcmdWithABool            *fExcessSideDeadLayerConsideredCmd; //M. Kidd, 4/13/2011
  G4UIcmdWithADoubleAndUnit   *fExcessSideDeadLayerThicknessCmd;  //M. Kidd, 4/13/2011
  G4UIcmdWithABool            *fCutInHalfCmd;
  G4UIcmdWithABool            *fDeadLayerOnCmd; 
  G4UIcmdWithoutParameter     *fDumpCmd;
  G4UIcmdWithADoubleAndUnit   *fHeightCmd;
  G4UIcmdWithoutParameter     *fMassAndVolumeCmd;
  G4UIcmdWithAString          *fMaterialCmd;
  G4UIcmdWithADoubleAndUnit   *fOuterDeadLayerThicknessCmd;
  G4UIcmdWithADoubleAndUnit   *fRadiusCmd;
  G4UIcmdWithoutParameter     *fSetDefaultsCmd;

  // Added by Y. Kermaidic 13/12/2016
  G4UIcmdWithABool            *fBeveledEdgeConsideredCmd;
  G4UIcmdWithADoubleAndUnit   *fBeveledEdgeAngleCmd;
  G4UIcmdWithADoubleAndUnit   *fBeveledEdgeTCmd;
  G4UIcmdWithADoubleAndUnit   *fDitchInnerRadiusCmd;
  G4UIcmdWithADoubleAndUnit   *fDitchOuterRadiusCmd;
  G4UIcmdWithADoubleAndUnit   *fDitchDepthCmd;

  /*
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerBottomZCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerMiddleZCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerTopZCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerBottomInnerRCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerMiddleInnerRCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerTopInnerRCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerBottomOuterRCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerMiddleOuterRCmd;
  G4UIcmdWithADoubleAndUnit   *fExtraDeadLayerTopOuterRCmd;
  G4UIcmdWithABool            *fExtraDeadLayerConsideredCmd;
  */
};
#endif
