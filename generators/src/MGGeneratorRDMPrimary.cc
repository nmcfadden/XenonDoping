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
//                                                          
// $Id: MGGeneratorRDMPrimary.cc,v 1.2 2007-02-21 09:31:33 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorPrimary.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR:  y. Chan
 * CONTACT: ydc@lbl.gov
 * FIRST SUBMISSION: Mon Apr 19 14:55:56 PDT 2004
 * 
 * REVISION:
 *
 * 06-11-2004, for interfacing to MGGeneratorRDM  
 *
 */
//---------------------------------------------------------------------------//
//

//---------------------------------------------------------------------------//

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Material.hh"
#include "Randomize.hh"

#include "generators/MGGeneratorRDMPrimary.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;


class G4Event;

//---------------------------------------------------------------------------//

MGGeneratorRDMPrimary::MGGeneratorRDMPrimary()
{
  //Inititialize particle gun with RDM
  fParticleGun = new MGGeneratorRDMRadioactiveDecayGun(); 
  MGLog(trace) << "MGGeneratorRDM Created. " << endlog;
}

//---------------------------------------------------------------------------//

MGGeneratorRDMPrimary::MGGeneratorRDMPrimary(const MGGeneratorRDMPrimary & other) : G4VUserPrimaryGeneratorAction(other)
{
}

//---------------------------------------------------------------------------//

MGGeneratorRDMPrimary::~MGGeneratorRDMPrimary()
{
  delete fParticleGun;
}

//---------------------------------------------------------------------------//

void MGGeneratorRDMPrimary::GeneratePrimaries(G4Event *anevent)
{
  //Set the nucleus at default position (0.,0.,0.)
  fParticleGun->SetParticlePosition(G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm));
  fParticleGun->GeneratePrimaryVertex(anevent);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
