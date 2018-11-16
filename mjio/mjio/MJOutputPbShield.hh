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
 * CLASS DECLARATION:  MJOutputPbShield.hh
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

#ifndef _PbShieldOUTPUTROOT_HH
#define _PbShieldOUTPUTROOT_HH
#include "io/MGOutputRoot.hh"
#include "G4VPhysicalVolume.hh"
#include "TParameter.h"

using namespace std;
class MJOutputPbShield: public MGOutputRoot
{
  public:
    MJOutputPbShield(G4bool isMother);
    ~MJOutputPbShield();

    //inherited virtual functions.
    void BeginOfRunAction();
    void DefineSchema();
    void BeginOfEventAction(const G4Event *event);
    void RootSteppingAction(const G4Step *step);
    void EndOfEventAction(const G4Event *event);
    void EndOfRunAction();
    void WriteFile();
    void CloseFile();
     
    enum EMJOutputCERNConsts{kMaxCharacterLength = 100,kMaxMultiplicity = 6000 };
  protected:
    void Clear();
    void GetData(const G4Step*);
    void KillTrack(G4Track*);

  protected:
    void GetPhysicalVolumes();
    G4double fX[kMaxMultiplicity];
    G4double fY[kMaxMultiplicity];
    G4double fZ[kMaxMultiplicity];
    G4double fE_MeV[kMaxMultiplicity];
    G4double fXMomentumDir[kMaxMultiplicity];
    G4double fYMomentumDir[kMaxMultiplicity];
    G4double fZMomentumDir[kMaxMultiplicity];
    G4double fTime[kMaxMultiplicity];
    G4double fWeight[kMaxMultiplicity];
    G4int fEventNumber;   
    G4int fTrackID[kMaxMultiplicity];
    G4int fParentID[kMaxMultiplicity];
    G4int fNMultiplicity;
    G4bool fFirstTime;


    G4VPhysicalVolume* fTheInsideOfTheShield;
    G4VPhysicalVolume* fTheOuterEdge;
    TParameter<long>* fParEventSuccess;
};
  
#endif
