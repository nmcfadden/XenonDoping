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
* $Id: MGGeneratorRDMRadioactiveDecayGun.cc,v 1.1 2004-12-07 08:56:06 pandola Exp $
*
* CLASS DECLARATION:  MGGeneratorRDMRadioactiveDecayGun.cc
*
*---------------------------------------------------------------------------//
*
* DESCRIPTION:
*
*/
// Begin description of class here
/**
*
*/
// End class description
//
/**
* SPECIAL NOTES:
*
* This is part of the G4 rdm demo package. Original contact is P. Truscott (DERA)
*/
//
// --------------------------------------------------------------------------//
/**
* AUTHOR: 
* CONTACT: ydc@lbl.gov
* FIRST SUBMISSION: Wed Mar 10 14:42:07 PST 2004
*
* REVISION:
*
* 06-02-2004, Initial port to MG
* mm-dd-yyyy, What is changed, Whoami
*/
// --------------------------------------------------------------------------//
#include "generators/MGGeneratorRDMNucleus.hh"
#include "generators/MGGeneratorRDMRadioactiveDecayGun.hh"
#include "generators/MGGeneratorRDMRadioactiveDecayGunMessenger.hh"
#include "G4DecayTable.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "io/MGLogger.hh"

#include "G4UImanager.hh"
#include "G4IonTable.hh"
#include "G4Ions.hh"
////////////////////////////////////////////////////////////////////////////////
//
MGGeneratorRDMRadioactiveDecayGun::MGGeneratorRDMRadioactiveDecayGun ()
{

  theMGGeneratorRDMRadioactiveDecayGunMessenger = new MGGeneratorRDMRadioactiveDecayGunMessenger(this);

}
////////////////////////////////////////////////////////////////////////////////
MGGeneratorRDMRadioactiveDecayGun::~MGGeneratorRDMRadioactiveDecayGun()
{
   delete theMGGeneratorRDMRadioactiveDecayGunMessenger;
}

void MGGeneratorRDMRadioactiveDecayGun::SetNucleus (MGGeneratorRDMNucleus theIon1)
{
  theIon = theIon1;

  G4IonTable *theIonTable =
    (G4IonTable*)(G4ParticleTable::GetParticleTable()->GetIonTable());
   
  //theIonTable->DumpTable();

  G4ParticleDefinition *aIon = NULL;

  G4int A = theIon.GetA();
  G4int Z = theIon.GetZ();
  G4double E = theIon.GetE();
  aIon = theIonTable->GetIon (Z, A, E);
  SetParticleDefinition(aIon);

//create Decay Table
/*
    G4DecayTable* table = new G4DecayTable();

    // create decay channels
    G4VDecayChannel** mode = new G4VDecayChannel*[1];
    // 
    mode[0] = 
      new G4PhaseSpaceDecayChannel("ldmphoton",0.999,2,"ldmhi","ldmhibar");
    // 
    mode[1] = new G4PhaseSpaceDecayChannel("ldmphoton",0.0005,2,"mu+","mu-");
    // 
    mode[2] = new G4PhaseSpaceDecayChannel("ldmphoton",0.0005,3,"pi+","pi-");

    for (G4int index=0; index <3; index++ ) table->Insert(mode[index]);

    delete [] mode;



  aIon->SetDecayTable(table);
  */
  G4DecayTable *dtable = aIon->GetDecayTable(); 
  if(!dtable) MGLog(routine) << " NO DECAY TABLE " << endlog;
  else dtable->DumpInfo();


  G4ParticleDefinition* pdef = theIonTable->GetParticle( theIonTable->GetNucleusEncoding(Z,A,0) );
  if(pdef) pdef->DumpTable();
  else MGLog(routine) << " no G4ParticleDefinion " << endlog;

  MGLog(routine) << " SetNucleus A " << A << " Z " << Z << " E " << E<< " "  << theIonTable->GetIonName(Z,A) << endlog;

  G4UImanager* UI = G4UImanager::GetUIpointer();
  UI->ApplyCommand("/gun/energy 0.000000 MeV");
  UI->ApplyCommand("/gun/position 0.0 0.0 8.0 cm");
  UI->ApplyCommand("/gun/direction 0.0 0.0 1.0 ");

}

