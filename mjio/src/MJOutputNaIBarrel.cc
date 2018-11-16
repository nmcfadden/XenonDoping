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
// $Id: MJOutputNaIBarrel.cc,v 1.4 2007-02-23 09:24:09 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputNaIBarrel.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
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
//---------------------------------------------------------------------------//
//

#include "TTree.h"

#include "G4Event.hh"
#include "G4Material.hh"

#include "io/MGLogger.hh"
#include "mjio/MJOutputNaIBarrelData.hh"

//---------------------------------------------------------------------------//

#include "mjio/MJOutputNaIBarrel.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

MJOutputNaIBarrel::MJOutputNaIBarrel(G4String, G4bool isMother):
  MGOutputRoot(isMother), fData(0)
{
  SetSchemaDefined(false);
}

//---------------------------------------------------------------------------//

MJOutputNaIBarrel::~MJOutputNaIBarrel()
{
  delete fData;
}

//---------------------------------------------------------------------------//

void MJOutputNaIBarrel::BeginOfEventAction(const G4Event*)
{
  fData->FlagNaI = 0;
  fTotalEnergyDeposit = 0.0;
  for(G4int i = 0; i < 16; i++) 
    fSegEdep[i] = 0.0;
}

//---------------------------------------------------------------------------//

void MJOutputNaIBarrel::BeginOfRunAction()
{
  fSensitiveMaterialIndex = G4Material::GetMaterial("NaI")->GetIndex();
}

//---------------------------------------------------------------------------//

void MJOutputNaIBarrel::DefineSchema()
{
  if(!SchemaDefined()) {
    fData = new MJOutputNaIBarrelData;
    if(!fTree) {
       fTree = new TTree("fTree", "NaI Barrel Tree");
       if(!IsMother())
	 MGLog(warning) << "No tree assigned to child output class." << endlog;
    }
    fTree->Branch("TotEdepNaI", &fData->TotEdepNaI, "TotEdepNaI/F");
    fTree->Branch("SegEdep", fData->SegEdep, "SegEdep[16]/F");
    fTree->Branch("FlagNaI", &fData->FlagNaI, "FlagNaI/I");
    SetSchemaDefined(true);
  }
}

//---------------------------------------------------------------------------//

void MJOutputNaIBarrel::EndOfEventAction(const G4Event*)
{
  fData->TotEdepNaI = (Float_t) G4ToRoot(fTotalEnergyDeposit / keV);
  for(G4int i = 0; i < 16; i++)
    fData->SegEdep[i] = (Float_t) G4ToRoot(fSegEdep[i] / keV);

  if(IsMother())
    FillTree();
}

//---------------------------------------------------------------------------//

void MJOutputNaIBarrel::EndOfRunAction()
{
  if(IsMother()) CloseFile();
}

//---------------------------------------------------------------------------//

void MJOutputNaIBarrel::RootSteppingAction(const G4Step *step)
{
  // Define these variables as static to avoid having to define them at
  // every stepping action, saving CPU cycles.
  static G4Track *track;
  static G4Material *material;
  static G4int i;

  track = step->GetTrack();
  material = track->GetMaterial();
  if(material->GetIndex() == fSensitiveMaterialIndex) {
    fTotalEnergyDeposit += step->GetTotalEnergyDeposit();
    i = G4int((track->GetPosition().phi() + pi)* 2.5464792);// 22.5 degrees / segment
    fSegEdep[i] += step->GetTotalEnergyDeposit();
  }
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

