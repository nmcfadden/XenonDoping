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
 * $Id: MJGeometryWIPPnDetectorMessenger.hh,v 1.1 2009-05-29 17:12:56 rojo Exp $
 *      
 * CLASS DECLARATION:  MJGeometryWIPPnDetectorMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * Messenger class for Solid Block
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Tue Aug 24 13:08:03 PDT 2004
 * 
 * REVISION:
 * 
 * 08-24-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYWIPPNDETECTORMESSENGER_HH
#define _MJGEOMETRYWIPPNDETECTORMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class MJGeometryWIPPnDetector;

class MJGeometryWIPPnDetectorMessenger : public G4UImessenger
{
public:

  //default constructor
  MJGeometryWIPPnDetectorMessenger(MJGeometryWIPPnDetector *NewWIPPnShield);

  //destructor
  ~MJGeometryWIPPnDetectorMessenger();

  //public interface
  void SetNewValue(G4UIcommand *cmd, G4String cmdstr);

  //protected members
protected:


  //private  members
private:
  G4UIdirectory             *fWIPPnDetectorDir;
  G4UIcmdWithABool          *fUseSampleCmd;
  G4UIcmdWithAString        *fChooseSampleCmd;
  G4UIcmdWithAString        *fChooseButtonMatCmd;
  G4UIcmdWithAString        *fChooseShieldCmd;
  G4UIcmdWithADoubleAndUnit *fSetCryostatThicknessCmd;
  G4UIcmdWithADoubleAndUnit *fSetCryoXtalDistanceCmd;
  MJGeometryWIPPnDetector   *fWIPPnDetector;
};
#endif
