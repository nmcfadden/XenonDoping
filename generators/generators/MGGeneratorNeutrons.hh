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
// 28 Dec 2005   L.Pandola    Added enum MGNeutronGeneratorType

#ifndef _MGGENERATORNEUTRONS_HH
#define _MGGENERATORNEUTRONS_HH


//---------------------------------------------------------------------------//

#include "G4VPrimaryGenerator.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Navigator.hh"

#include "generators/MGGeneratorNeutronsAtGS.hh"
#include "generators/MGVGenerator.hh"

class G4Event;
enum MGNeutronGeneratorType {gransasso, sources };

//---------------------------------------------------------------------------//

class MGGeneratorNeutrons : public MGVGenerator
{
public:

  //default constructor
  MGGeneratorNeutrons(MGNeutronGeneratorType);

  //copy constructor
  MGGeneratorNeutrons(const MGGeneratorNeutrons &);

  //destructor
  ~MGGeneratorNeutrons();

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
