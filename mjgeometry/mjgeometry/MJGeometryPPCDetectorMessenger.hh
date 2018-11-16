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
 * $Id: MJGeometryPPCDetectorMessenger.hh,v 1.1 2008-09-18 17:40:01 mitzi Exp $
 *      
 * CLASS DECLARATION:  MJGeometryPPCDetectorMessenger.hh
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Feb 14 15:33:13 PST 2005
 * 
 * REVISION:
 * 
 * 02-14-2005, Created, R. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYPPCDETECTORMESSENGER_HH
#define _MJGEOMETRYPPCDETECTORMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class MJGeometryPPCDetector;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcommand;

class MJGeometryPPCDetectorMessenger : public G4UImessenger
{
public:

  MJGeometryPPCDetectorMessenger(MJGeometryPPCDetector *assy);

  ~MJGeometryPPCDetectorMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String nV);

private:
  G4UIdirectory                     *fDirectory;
  MJGeometryPPCDetector     *fICWS;

  // Commands.
  G4UIcmdWithADoubleAndUnit         *fAssyHeightCmd;
  G4UIcmdWithADoubleAndUnit         *fAssyRadiusCmd;

};
#endif
