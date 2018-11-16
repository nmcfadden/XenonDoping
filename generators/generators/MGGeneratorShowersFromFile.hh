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
 *
 * CLASS DECLARATION:  MGGeneratorShowersFromFile.hh
 *
 *---------------------------------------------------------------------------//
 */
/**
 * AUTHOR: R. Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION: 2015

 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORSHOWERGUN_HH
#define _MGGENERATORSHOWERGUN_HH

//---------------------------------------------------------------------------//


#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4VPrimaryGenerator.hh"

#include "TTree.h"

class G4Event;
class MGGeneratorShowersFromFileMessenger;

//---------------------------------------------------------------------------//

class MGGeneratorShowersFromFile : public G4VPrimaryGenerator
{
public:

  //default constructor
  MGGeneratorShowersFromFile();

  //copy constructor
  MGGeneratorShowersFromFile(const MGGeneratorShowersFromFile &);

  //destructor
  ~MGGeneratorShowersFromFile();

  //public interface
  void GeneratePrimaryVertex(G4Event *event);
  G4String GetFileName() {return fileName;};
  void SetFileName(G4String stri);
  void OnlyMuons(G4bool boo);

  //protected members
protected:


  //private  members
private:
  //G4int n_particle;
  G4int ev_ID;
  //G4int muon_number[1500];
  MGGeneratorShowersFromFileMessenger* theMessenger;
  G4String fileName;
  //G4int muonnumber;
  G4int max_entries;
  G4bool MuonsOnly;

  Double_t Distribution(Double_t, Double_t);
};
#endif
