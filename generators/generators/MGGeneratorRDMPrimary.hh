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
 * $Id: MGGeneratorRDMPrimary.hh,v 1.1 2004-12-07 08:45:48 pandola Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorRDMPrimary.hh
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
 * Class theat generated primary particles as requested by user.
 * Called by SetUserInitialization() in main()
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
 * FIRST SUBMISSION: Mon Apr 19 14:55:56 PDT 2004
 * 
 * REVISION:
 * 
 * 04-19-2004
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORRDMPRIMARY_HH
#define _MGGENERATORRDMPRIMARY_HH

//---------------------------------------------------------------------------//

#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Navigator.hh"

#include "generators/MGGeneratorRDMRadioactiveDecayGun.hh"

class G4Event;

//---------------------------------------------------------------------------//

class MGGeneratorRDMPrimary : public G4VUserPrimaryGeneratorAction
{
public:

  //default constructor
  MGGeneratorRDMPrimary();

  //copy constructor
  MGGeneratorRDMPrimary(const MGGeneratorRDMPrimary &);

  //destructor
  ~MGGeneratorRDMPrimary();

  //public interface
  void GeneratePrimaries(G4Event *anevent);

  //protected members
protected:


  //private  members
private:
  G4ParticleGun  *fParticleGun;
};
#endif
