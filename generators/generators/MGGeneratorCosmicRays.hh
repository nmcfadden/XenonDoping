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
// Implementation of the Cosmic Ray primary generator as MG-generator
//
// History:
// --------
// 11 Nov 2004   L.Pandola    First implementation
// 27 Apr 2005   L.Pandola    Added MUSUN generator
// 28 Dec 2005   L.Pandola    Added enum MGMuonGeneratorType
// 30 Dec 2005   L.Pandola    MuonsFromFile interfaced through this class

#ifndef _MGGENERATORCOSMICRAYS_HH
#define _MGGENERATORCOSMICRAYS_HH


//---------------------------------------------------------------------------//

#include "G4VPrimaryGenerator.hh"
#include "generators/MGVGenerator.hh"

enum MGMuonGeneratorType {standard, musun, file, meihime, showerfile};

//---------------------------------------------------------------------------//

class MGGeneratorCosmicRays : public MGVGenerator
{
public:

  //default constructor
  MGGeneratorCosmicRays(MGMuonGeneratorType);

  //copy constructor
  MGGeneratorCosmicRays(const MGGeneratorCosmicRays &);

  //destructor
  ~MGGeneratorCosmicRays();

  //public interface
  void GeneratePrimaryVertex(G4Event *anevent);

  void SetParticlePosition(G4ThreeVector vec) {fParticleGun->SetParticlePosition(vec);}

  //protected members
protected:


  //private  members
private:
  G4VPrimaryGenerator  *fParticleGun;
};
#endif
