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
// Implementation of the Cosmic Ray primary generator as MG-generator;
// the particle gun in this case is not the usual G4ParticleGun, but a
// derived class of G4VPrimaryGenerator
//
// History:
// --------
// 11 Nov 2004   L.Pandola    First implementation
// 27 Apr 2005   L.Pandola    Added MUSUN generator
// 28 Dec 2005   L.Pandola    First attempt to simplify the generator schemes;
//                            added enum MGMuonGeneratorType
// 30 Dec 2005   L.Pandola    MuonsFromFile interfaced through this class
// xx Aug 2015   R.Massarczyk Added ShowerfromFile

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorCosmicRays.hh"
#include "generators/MGGeneratorMUSUNInterface.hh"
#include "generators/MGGeneratorMuonsFromFile.hh"
#include "generators/MGGeneratorCosmicRayMuons.hh"
#include "generators/MGGeneratorMeiHimeMu.hh"
#include "generators/MGGeneratorShowersFromFile.hh"

//---------------------------------------------------------------------------//

MGGeneratorCosmicRays::MGGeneratorCosmicRays(MGMuonGeneratorType choice)
{
  if (choice == standard)
    fParticleGun = new MGGeneratorCosmicRayMuons();
  else if (choice == musun)
    fParticleGun = new MGGeneratorMUSUNInterface(" ");
  else if (choice == file)
    fParticleGun = new MGGeneratorMuonsFromFile();
  else if (choice == meihime)
    fParticleGun = new MGGeneratorMeiHimeMu();
  else if (choice == showerfile)
    fParticleGun = new MGGeneratorShowersFromFile();
	//fParticleGun = new MGGeneratorMuonsFromFile();

  MGLog(trace) << "MGGeneratorCosmicRays created. Choice = "
	       << choice << endlog;

}

//---------------------------------------------------------------------------//

MGGeneratorCosmicRays::MGGeneratorCosmicRays(const MGGeneratorCosmicRays & other) : MGVGenerator(other)
{
}

//---------------------------------------------------------------------------//

MGGeneratorCosmicRays::~MGGeneratorCosmicRays()
{
  delete fParticleGun;
}

//---------------------------------------------------------------------------//

void MGGeneratorCosmicRays::GeneratePrimaryVertex(G4Event *anevent)
{
  fParticleGun->GeneratePrimaryVertex(anevent);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
