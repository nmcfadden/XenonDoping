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
 * CLASS DECLARATION:  MJOutputCERN_NA55.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Root output class for spallation neutron studies in solid block.
 *
 * AUTHOR: M. Marino
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 * 02-2006: Created, M. Marino
 *
 */

#ifndef _CERN_NA55OUTPUTROOT_HH
#define _CERN_NA55OUTPUTROOT_HH
#include <map>
#include <list>
#include <string>
#include "io/MGOutputRoot.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "TParameter.h"

using namespace std;
class MJOutputCERN_NA55: public MGOutputRoot
{
  public:
    MJOutputCERN_NA55(G4bool isMother);
    ~MJOutputCERN_NA55();

    //inherited virtual functions.
    void BeginOfRunAction();
    void DefineSchema();
    void BeginOfEventAction(const G4Event *event);
    void RootSteppingAction(const G4Step *step);
    void EndOfEventAction(const G4Event *event);
    void EndOfRunAction();
    void WriteFile();
    void CloseFile();
     
    enum EMJOutputCERNConsts{kMaxCharacterLength = 100,kMaxMultiplicity = 600 };
  protected:
    void Clear();
    void GetData(const G4Step*);
    void KillTrack(G4Track*);

  protected:
    void GetPhysicalVolumes();
    G4double fCosTheta[kMaxMultiplicity];
    G4double fPhi[kMaxMultiplicity];
    G4double fRadius[kMaxMultiplicity];
    G4double fE_MeV[kMaxMultiplicity];
    G4double fXMomentumDir[kMaxMultiplicity];
    G4double fYMomentumDir[kMaxMultiplicity];
    G4double fZMomentumDir[kMaxMultiplicity];
    G4double fTime[kMaxMultiplicity];
    G4int fEventNumber;   
    G4int fTrackID[kMaxMultiplicity];
    G4int fParentID[kMaxMultiplicity];
    G4int fNMultiplicity;
    G4bool fFirstTime;
    char fTheProcessName[kMaxMultiplicity*(kMaxCharacterLength+1)];
    G4int fTheProcessNamePos;


    G4VPhysicalVolume* fTheSphere;
    G4VPhysicalVolume* fTheDetector;
    TParameter<long>* fParEventSuccess;
    TParameter<double>* fParBeamDumpLength_cm;
    TParameter<double>* fParBeamDumpDensity;
    TParameter<long>* fParNumCrashParticles;
    G4double fTheBDHalfZLength;
    G4int fNumCrashParticles;
    G4ParticleDefinition* fTheCrashingParticle; // temp hack for 194Tl


};
  
#endif
