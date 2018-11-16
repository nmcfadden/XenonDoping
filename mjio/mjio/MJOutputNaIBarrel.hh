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
 * $Id: MJOutputNaIBarrel.hh,v 1.1 2004-12-08 02:02:21 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJOutputNaIBarrel.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Handles ROOT output for NaI barrel used at TUNL for the FEL run.
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
 * FIRST SUBMISSION: Mon Aug  9 08:43:28 PDT 2004
 * 
 * REVISION:
 * 
 * 08-09-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTNAIBARREL_HH
#define _MJOUTPUTNAIBARREL_HH

//---------------------------------------------------------------------------//

#include "io/MGOutputRoot.hh"

//---------------------------------------------------------------------------//

class G4Event;
class G4Step;
class MJOutputNaIBarrelData;

class MJOutputNaIBarrel : public MGOutputRoot
{
public:

  //default constructor
  MJOutputNaIBarrel(G4String name, G4bool isMother);

  //destructor
  ~MJOutputNaIBarrel();

  //public interface
  void BeginOfEventAction(const G4Event *event);
  void BeginOfRunAction();
  void DefineSchema();
  void EndOfEventAction(const G4Event *event);
  void EndOfRunAction();
  void RootSteppingAction(const G4Step *step);

  //protected members
protected:


  //private  members
private:
  MJOutputNaIBarrelData     *fData;
  G4double                   fTotalEnergyDeposit;
  G4double                   fSegEdep[16];
  size_t                     fSensitiveMaterialIndex;
};
#endif
