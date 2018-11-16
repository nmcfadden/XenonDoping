//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
// $Id: MGGeneratorSPS.cc,v 1.2 2006-02-28 18:25:08 rojo Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorSPS.cc
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Jun 20 17:52:06 PDT 2005
 * 
 * REVISION:
 *
 * 06-20-2005, Created, R. Henning.
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//

#include "G4Event.hh"
#include "G4ThreeVector.hh"
#include "G4GeneralParticleSource.hh"

#include "generators/MGGeneratorSPS.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

MGGeneratorSPS::MGGeneratorSPS()
{
  MGLog(routine) << "Using SPS Generator." << endlog;
  fParticleSource = new G4GeneralParticleSource(); 
}

//---------------------------------------------------------------------------//

MGGeneratorSPS::~MGGeneratorSPS()
{
  delete fParticleSource;
}

//---------------------------------------------------------------------------//

void MGGeneratorSPS::GeneratePrimaryVertex(G4Event *event)
{
  fParticleSource->GeneratePrimaryVertex(event);
}

//---------------------------------------------------------------------------//

void MGGeneratorSPS::SetParticlePosition(G4ThreeVector vec)
{
  fParticleSource->GetCurrentSource()->GetPosDist()->SetCentreCoords(vec);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
