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
 * $Id: MGGeneratorLGND200Calibration.hh, $
 *      
 * CLASS DECLARATION:  MGGeneratorLGND200Calibration.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R. Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * June-2018, Created, Ralph
 *
 */
//---------------------------------------------------------------------------//
//

#ifndef _MGGENERATORLGND200CALIBRATION_HH
#define _MGGENERATORLGND200CALIBRATION_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4ThreeVector.hh"

#include "generators/MGVGenerator.hh"

//---------------------------------------------------------------------------//

class G4Event;
class G4Messenger;
class G4ParticleGun;
class G4Run;

class MGGeneratorLGND200Calibration : public MGVGenerator
{
public:

  //default constructor
  MGGeneratorLGND200Calibration();
	
  //copy constructor
  MGGeneratorLGND200Calibration(const MGGeneratorLGND200Calibration &);

  //destructor
  ~MGGeneratorLGND200Calibration();

  //public interface
  void BeginOfEventAction(G4Event *event);
  void BeginOfRunAction(G4Run const *run);
  void EndOfRunAction(G4Run const *run);
  void GeneratePrimaryVertex(G4Event *event);

 
  //This method is not used but it is necessary 
  //because it is purely virtual in MGVGenerator
  void SetParticlePosition(G4ThreeVector) {;}

	
	void Dump();
  void SetIonZ(G4int z) {fZ = z;}
  void SetIonA(G4int a) {fA = a;}
  G4double GetIonZ() const {return fZ;}
  G4double GetIonA() const {return fA;}

  
  //protected members
protected:

  //private  members
private:
  G4ParticleGun *fParticleGun;

  G4int fZ;
  G4int fA;
	G4ThreeVector	*fSourcePosition;
};
#endif
