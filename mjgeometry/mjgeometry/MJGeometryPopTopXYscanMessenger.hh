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
 * $Id: MJGeometryPopTopXYscanMessenger.hh,v 1.1 2011-03-11  $
 *      
 * CLASS DECLARATION:  MJGeometryPopTopXYscanMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *
 *
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
 * AUTHOR: R.Martin, W. Xu
 * CONTACT: 
 * FIRST SUBMISSION: 11 Mar 2011
 * 
 * REVISION:
 * 05-08-2013 W. Xu modified for xyscan
 * 03-05-2011 R. Martin created
 */
// --------------------------------------------------------------------------//

#ifndef _MJGeometryPopTopXYscanMESSENGER_HH
#define _MJGeometryPopTopXYscanMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class MJGeometryPopTopXYscan;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class G4UIcommand;

class MJGeometryPopTopXYscanMessenger : public G4UImessenger
{
public:
  MJGeometryPopTopXYscanMessenger(MJGeometryPopTopXYscan *popTopxyscan);
  ~MJGeometryPopTopXYscanMessenger();
  
  void SetNewValue(G4UIcommand* cmd, G4String nV);
  
private:
  G4UIdirectory    *fDirectory;
  MJGeometryPopTopXYscan *fPopTopxyscan;
  
  // Commands.
  G4UIcmdWith3VectorAndUnit *fSourcePositionCmd;	
  G4UIcmdWithADoubleAndUnit *fCrystalOffsetCmd;
  G4UIcmdWithAnInteger* fCryostatTypeCmd;
  G4UIcmdWithAString* fCrystalGeomFileCmd;
  G4UIcmdWithADoubleAndUnit *fCrystalDimpleNomRadiusCmd;
  G4UIcmdWithADoubleAndUnit *fCrystalDimpleDepthCmd;
  G4UIcmdWithADoubleAndUnit *fCrystalDimpleDiameterCmd;
  G4UIcmdWithADoubleAndUnit *fCrystalDimpleRadialOffsetCmd;
  G4UIcmdWithADoubleAndUnit *fCrystalDiameterCmd;
  G4UIcmdWithADoubleAndUnit *fCrystalLengthCmd;
  G4UIcmdWithADoubleAndUnit *fCollimatorGapCmd;
  G4UIcmdWithABool *fPlaceCollimatorCmd;
  G4UIcmdWithABool *fPlaceSourceHolderCmd; 
};
#endif
