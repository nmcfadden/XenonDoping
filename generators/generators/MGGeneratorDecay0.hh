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
// Implementation of the Decay0 primary generator as MaGe-generator
//
// History:
// --------
// 25 Oct 2004   L.Pandola    First implementation
// 06 Dec 2004   L.Pandola    Re-naming
// 23 Dec 2005   L.Pandola    Changed class name of the Generator

#ifndef _MGGENERATORDECAY0_HH
#define _MGGENERATORDECAY0_HH


//---------------------------------------------------------------------------//

#include "G4VPrimaryGenerator.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Navigator.hh"

#include "generators/MGGeneratorDecay0Interface.hh"
#include "generators/MGVGenerator.hh"

class G4Event;

//---------------------------------------------------------------------------//

class MGGeneratorDecay0 : public MGVGenerator
{
public:

  //default constructor
  MGGeneratorDecay0();

  //copy constructor
  MGGeneratorDecay0(const MGGeneratorDecay0 &);

  //destructor
  ~MGGeneratorDecay0();

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
