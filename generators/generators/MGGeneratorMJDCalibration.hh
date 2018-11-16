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
 * $Id: MGGeneratorMJDCalibration.hh, $
 *      
 * CLASS DECLARATION:  MGGeneratorMJDCalibration.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Generate Beam of Gammas as from the TUNL FEL
 * Currently the beam is simulated with a Gaussian energy and
 * width distribution in x and y.
 * The beam is created with the major axis along the x-axis and the minor
 * axis along the y axis. It is then rotated by fRho degrees on the x-y plane
 * fRho is relative to the x-axis. Particles are then generated parallel to 
 * the z-axis.
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
 * AUTHOR: K. Vorren
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 
 * 02-24-2016, Modified generator to read calibration source parameters
 *             from messenger commands, T. Caldwell
 * 07-14-2014, Created, K. Vorren
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORMJDCAL_HH
#define _MGGENERATORMJDCAL_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4ThreeVector.hh"

#include "generators/MGVGenerator.hh"

//---------------------------------------------------------------------------//

class G4Event;
class G4Messenger;
class G4ParticleGun;
class G4Run;

class MGGeneratorMJDCalibration : public MGVGenerator
{
public:

  //default constructor
  MGGeneratorMJDCalibration();

  //copy constructor
  MGGeneratorMJDCalibration(const MGGeneratorMJDCalibration &);

  //destructor
  ~MGGeneratorMJDCalibration();

  //public interface
  void BeginOfEventAction(G4Event *event);
  void BeginOfRunAction(G4Run const *run);
  void Dump();
  void EndOfRunAction(G4Run const *run);
  void GeneratePrimaryVertex(G4Event *event);

  G4ThreeVector GetCurrentPosition() { return fCurrentPosition; }
  
  //This method is not used but it is necessary 
  //because it is purely virtual in MGVGenerator
  void SetParticlePosition(G4ThreeVector) {;}

  void SetOrigin(G4ThreeVector origin){fOrigin = origin;}
  void SetZrotation(G4double zrotation){fZrotation = zrotation;}
  void SetIonZ(G4int z) {fZ = z;}
  void SetIonA(G4int a) {fA = a;}
  G4double GetIonZ() const {return fZ;}
  G4double GetIonA() const {return fA;}
  void SetSourcePos(std::string sourcePos);
  
  //protected members
protected:

  //private  members
private:
  G4ParticleGun *fParticleGun;

  G4double fCurrentAngle;
  G4ThreeVector fCurrentPosition; // Current position of particle 
                                  // generated.
  G4ThreeVector fOrigin;          // Beam origin. Particles will be 
                                  // distributed around this point.
  G4double fZrotation;   // rotation about the z axis for the source
  G4double fHelixRadius; // radius of the calibration source
  G4double fHelixAngle;  // angle of the helical incline
  G4double fStartAngle;  // start angle of the helical generator
  G4double fTotalAngle;  // angle subtended by helix
  G4int fZ;
  G4int fA;
};
#endif
