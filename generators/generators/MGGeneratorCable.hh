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
 * $Id: MGGeneratorCable.hh, $
 *      
 * CLASS DECLARATION:  MGGeneratorCable.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Approximate source generator for signal and HV cables
 * - Signal and HV cable bundles near the detectors are approximated as cylinders
 * - Aprroximate each string as 4 detectors in length
 * - HV cables extend to the HV ring of each detector while the signal cables extend to the cable guides of each detector
 * - Cables on top of the cold plate are estimated as a disk
 * - Cables inside the cross arm are estimated as a hollow cylinder surrounding the thermosyphon tube
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: B. Zhu
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 
 * 09-2016, Created, B. Zhu
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORCABLE_HH
#define _MGGENERATORCABLE_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4ThreeVector.hh"
//#include "G4SystemOfUnits.hh"  //doesnt exist in g4.10
#include "generators/MGVGenerator.hh"

//---------------------------------------------------------------------------//

class G4Event;
class G4Messenger;
class G4ParticleGun;
class G4Run;

class MGGeneratorCable : public MGVGenerator
{
public:

  //default constructor
  MGGeneratorCable();

  //copy constructor
  MGGeneratorCable(const MGGeneratorCable &);

  //destructor
  ~MGGeneratorCable();

  //public interface
  void BeginOfEventAction(G4Event *event);
  void BeginOfRunAction(G4Run const *run);
  void EndOfRunAction(G4Run const *run);
  void GeneratePrimaryVertex(G4Event *event);
  
  //This method is not used but it is necessary 
  //because it is purely virtual in MGVGenerator
  void SetParticlePosition(G4ThreeVector) {;}

  // Sets dimensions of cables
  void SetSourcePos(std::string sourcePos);
  void SetSourceType(std::string sourceType) {fSourceType = sourceType;}
  void SetIonZ(G4int z) {fZ = z;}
  void SetIonA(G4int a) {fA = a;}

  G4double GetIonZ() const {return fZ;}
  G4double GetIonA() const {return fA;}
  
  //protected members
protected:

  //private  members
private:
  G4ParticleGun *fParticleGun;

  // Particle types
  G4int fZ;
  G4int fA;

  std::string fSourceType;
  std::string fSourcePos;

  G4ThreeVector fPosition; // position of particle generated
  G4double fPositionX;
  G4double fPositionY;
  G4double fPositionZ; 
  G4double fZrotation;

  // Random variables
  G4double fRandRadiusSq;
  G4double fRandAngle;
  G4int fRandString; 
  G4int fRandPos;

  G4double fCableRadius; // Radius of a bundle of cables
  G4ThreeVector fCableOffset[7]; // XY location of cables wrt center of cold plate, even is signal and odd is HV
  G4ThreeVector fHVOffset[7]; // XY location of cables wrt center of cold plate, even is signal and odd is HV

  G4double fColdPlateRadius;
  G4double fColdPlateZ; 
  G4double fCrossArmLength;
  G4double fCrossArmWidth;
  G4double fCrossArmIRadius;
  G4double fCrossArmT;

  G4double fCableLength[4]; // Half length of signal cable, one for each detector 
  G4double fCableCenter[4]; // Centers of signal cables, one for each detector
  G4double fHVLength[4]; // Half length of HV cable, one for each detector
  G4double fHVCenter[4]; // Centers of HV cables, one for each detector
  G4ThreeVector fColdPlateOffset; // offset of cold plate to origin in world

};
#endif
