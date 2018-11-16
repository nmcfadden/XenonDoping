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
// Implementation of the neutron primary generator as a MG-generator
//
// History:
// --------
// 13 Dec 2004   L.Pandola    First implementation
// 28 Dec 2005   L.Pandola    First attempt to simplify the generator schemes; 
//                            added enum MGNeutronGeneratorType
//                            Added Sources4A generator

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Material.hh"
#include "Randomize.hh"

#include "io/MGLogger.hh"

class G4Event;

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorNeutrons.hh"
#include "generators/MGGeneratorSources4AInterface.hh"

//---------------------------------------------------------------------------//

MGGeneratorNeutrons::MGGeneratorNeutrons(MGNeutronGeneratorType choice)
{
  if (choice == gransasso) 
    {
      fParticleGun = new MGGeneratorNeutronsAtGS(); 
      MGLog(trace) << "MGGeneratorNeutrons Created." << endlog;
    }
  else if (choice == sources)
    {
      fParticleGun = new MGGeneratorSources4AInterface();
      MGLog(trace) << "MGGeneratorSources4AInterface created." << endlog;
    }
}

//---------------------------------------------------------------------------//

MGGeneratorNeutrons::MGGeneratorNeutrons(const MGGeneratorNeutrons & other) : MGVGenerator(other)
{
}

//---------------------------------------------------------------------------//

MGGeneratorNeutrons::~MGGeneratorNeutrons()
{
  delete fParticleGun;
}

//---------------------------------------------------------------------------//

void MGGeneratorNeutrons::GeneratePrimaryVertex(G4Event *anevent)
{
  fParticleGun->GeneratePrimaryVertex(anevent);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
