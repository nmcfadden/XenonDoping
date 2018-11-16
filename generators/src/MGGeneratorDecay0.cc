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
// Implementation of the Decay0 primary generator as MG-generator
//
// History:
// --------
// 25 Oct 2004   L.Pandola    First implementation
// 06 Dec 2004   L.Pandola    Re-naming
// 23 Dec 2005   L.Pandola    Changed class name of the Generator

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Material.hh"
#include "Randomize.hh"

#include "io/MGLogger.hh"

class G4Event;

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorDecay0.hh"

//---------------------------------------------------------------------------//

MGGeneratorDecay0::MGGeneratorDecay0()
{
  fParticleGun = new MGGeneratorDecay0Interface(" "); 
  MGLog(trace) << "MGGeneratorDecay0 Created." << endlog;
}

//---------------------------------------------------------------------------//

MGGeneratorDecay0::MGGeneratorDecay0(const MGGeneratorDecay0 & other) : MGVGenerator(other)
{
}

//---------------------------------------------------------------------------//

MGGeneratorDecay0::~MGGeneratorDecay0()
{
  delete fParticleGun;
}

//---------------------------------------------------------------------------//

void MGGeneratorDecay0::GeneratePrimaryVertex(G4Event *anevent)
{
  fParticleGun->GeneratePrimaryVertex(anevent);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
