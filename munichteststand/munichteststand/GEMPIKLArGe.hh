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
 * AUTHOR: Luciano Pandola
 * FIRST SUBMISSION: 04-07-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 05-03-2005, Luciano, Added messenger and a few methods
 * 05-04-2005, Added flag to switch on/off the crystal
 * 
 */

// ---------------------------------------------------------------------------

#ifndef _GEMPIKLARGE_HH
#define _GEMPIKLARGE_HH

// --------------------------------------------------------------------------- 

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"
#include "munichteststand/GEMPIKLArGeMessenger.hh"

// --------------------------------------------------------------------------- 

class GEMPIKLArGe : public MGGeometryDetector
{

public:
  
  // default constructor

  GEMPIKLArGe();

  // destructor

  ~GEMPIKLArGe();

  // construction inherited from MJGeometryDetector

  void ConstructDetector();
  void SetWLSYield(G4double y){WLSyield = y;};
  void SetCrystalIn(G4bool iscin){isCrystalIn = iscin;};

private:
  void RegisterArgonOpticalProperties();
  G4double VM2000EmissionSpectrum(G4double);
  void InitializeVM2000Spectrum();
  static const G4int npoints = 250;
  G4double frequencyV[npoints];
  G4double efficiencyV[npoints];
  G4bool successfulInitialization;

  G4double WLSyield;

  G4double LArRefIndex(const G4double);
  G4double LArEpsilon(const G4double);
  G4double LArRayLength(const G4double,const G4double temp);
  G4double ArScintillationSpectrum(const G4double);
  G4double CalculateWLSmfp(G4double);

  G4bool isCrystalIn;

  GEMPIKLArGeMessenger* fTheMessenger;
}; 

// --------------------------------------------------------------------------- 

#endif
