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
 * $Id: MGGeneratorRDM.hh,v 1.1 2004-12-07 08:45:48 pandola Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorRDM.hh
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
 * FIRST SUBMISSION: Wed Jul  7 18:12:55 PDT 2004
 * 
 * REVISION:
 * 
 * 04-19-2004, Created, ydchan
 * 07-28-2004, Changed name from MGGeneratorRDMPrimary R. Henning
 * 11-08-2004, Implemented virtual method SetParticlePosition of MGVGenerator, Luciano
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORRDM_HH
#define _MGGENERATORRDM_HH


//---------------------------------------------------------------------------//

#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Navigator.hh"

#include "generators/MGGeneratorRDMRadioactiveDecayGun.hh"
#include "generators/MGVGenerator.hh"

class G4Event;

//---------------------------------------------------------------------------//

class MGGeneratorRDM : public MGVGenerator
{
public:

  //default constructor
  MGGeneratorRDM();

  //copy constructor
  MGGeneratorRDM(const MGGeneratorRDM &);

  //destructor
  ~MGGeneratorRDM();

  //public interface
  void GeneratePrimaryVertex(G4Event *anevent);
  void SetParticlePosition(G4ThreeVector vec) {fParticleGun->SetParticlePosition(vec);}
  //protected members
protected:


  //private  members
private:
  G4ParticleGun  *fParticleGun;
};
#endif
