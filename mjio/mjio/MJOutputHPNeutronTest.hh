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
 * $Id: MJOutputHPNeutronTest.hh,v 1.4 2008-02-26 18:26:19 mgmarino Exp $
 *      
 * CLASS DECLARATION:  MJOutputHPNeutronTest.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Output class to histogram useful information to test the neutron HP
 * package. 
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
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Mar 12 15:15:32 EDT 2007
 * 
 * REVISION:
 * 
 * 03-12-2007, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTHPNEUTRONTEST_HH
#define _MJOUTPUTHPNEUTRONTEST_HH

//---------------------------------------------------------------------------//

#include <list>
#include <iostream>
#include "G4Event.hh"

#include "io/MGOutputRoot.hh"

class TH1F;
class TH2F;

class G4Step;
class G4SteppingManager;
class MGOutputG4Steps;

//---------------------------------------------------------------------------//

class MJOutputHPNeutronTest : public MGOutputRoot
{
public:

  MJOutputHPNeutronTest();
  ~MJOutputHPNeutronTest();

  void DefineSchema() { if(fSaveG4Steps) fG4Steps->DefineSchema();}
  void BeginOfEventAction(const G4Event*);
  void BeginOfRunAction();
  void DumpEndOfEvent();
  void EndOfEventAction(const G4Event* event); 
  void EndOfRunAction();
  void PreUserTrackingAction(const G4Track*); 
  void SteppingAction(const G4Step* step, 
		      G4SteppingManager* steppingManager) 
      { RootSteppingAction(step, steppingManager); }

  void RootSteppingAction(const G4Step* /*step*/) {} 
  void RootSteppingAction(const G4Step* step, 
			  G4SteppingManager *steppingManager); 


private:

  // Histograms
  TH1F   *fHInitNSpectrum; // Initial energy spectrum
  TH1F   *fHCaptureXSec;   // (n,0),  crossection
  TH1F   *fHN_NXSec;       // (n,n), elastic crossection
  TH1F   *fHN_2NXSec;      // (n,2n) crossection
  TH1F   *fHN_MNXSec;      // (n,m x n) crossection
  TH1F   *fHN_NGXSec;      // (n,nG) crossection
  TH1F   *fHN_GXSec;       // (n,G) crossection
  TH1F   *fHN_PXSec;       // (n,p) crossection
  TH1F   *fHN_OtherXSec;   // (n,other) crossection
  TH1F   *fHGammaSpect;    // Final state gamma spectrum
  TH1F   *fHNSecondaries;  // Total number of secondaries
  TH1F   *fHNNeutrons;     // Total number of secondary neutrons, excl. primary.
  TH1F   *fHNProtons;      // Total number of secondary protons, excl. primary.
  TH1F   *fHProcess;       // Histogram of processes that contributed to each step. 
  TH2F   *fHNTheta;     // cos(theta) of scattering angle of neutrons.
  TH1F   *fHELossInBlock;  // Energy loss in block per event. 
  TH1F   *fHEfDivEi;       // Final energy divided by initial energy. Check consistency
  TH1F   *fHRecoilPEnergy; // Recoil energy of proton in (n,n) interaction.
  TH1F   *fHNuclearRecoilE;// Recoil energy for nucleus immediately after interaction.
  TH1F   *fHNNuclei;       // Number of nuclei produced in simulation. 

  // Counters and event data
  G4double fEnergyMeV; // Initial energy of primary neutron.  
  G4int    fStepN;     // Total number of steps 
  G4int    fNGammas;   // Number of secondary gammas. 
  G4int    fNNuclei;   // Number of secondary nuclei.    
  G4int    fNProtons;  // Number of secondary protons
  G4int    fNNeutrons; // Number of secondary neutrons
  G4int    fNOther;    // Number of other processes
  G4int    fNSecondaries; // Total number of secondary particles. 
  G4double fELossInBlock; // Energy deposit in Block.
  G4bool   fNoNInteraction; // True if neutron did not interact in block.
  G4double fNNTheta;        // Neutron scatter angle for (n,n)
  G4double fRecoilPEnergy;  // Energy of recoil proton (only in H-1)
  G4double fNNFinalEnergy;  // Final energy of elastically scattered neutron.
  
  std::list<Int_t>           fNuclearTrackID;
  std::list<Double_t>        fNuclearRecoilEnergy;
  MGOutputG4Steps       *fG4Steps; // To save G4 step info if required. 
  G4bool                fSaveG4Steps; 
};
#endif
