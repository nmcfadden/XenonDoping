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
 * $Id: MJGeometryRDCryostatMessenger.hh,v 1.2 2009-04-23 18:32:24 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometryRDCryostatMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger class for RD Cryostat. 
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
 * FIRST SUBMISSION: Wed Nov 16 16:25:29 PST 2005
 * 
 * REVISION:
 * 
 * 11-17-2005, Created, R. Henning
 * 04-21-2009, Added DEMO On command, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYRDCRYOSTATMESSENGER_HH
#define _MJGEOMETRYRDCRYOSTATMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class MJGeometryRDCryostat;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcommand;

class MJGeometryRDCryostatMessenger: public G4UImessenger 
{
public:

  MJGeometryRDCryostatMessenger(MJGeometryRDCryostat *parent);
  MJGeometryRDCryostatMessenger(const MJGeometryRDCryostatMessenger &);
  ~MJGeometryRDCryostatMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String nV);

private:
  
  G4UIdirectory            *fDirectory;
  MJGeometryRDCryostat     *fRDCryostat;

  // Commands
  G4UIcmdWithABool         *fDEMOOnCmd;

};
#endif
