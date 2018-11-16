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
 * CLASS DECLARATION: GEOutputHadesScintillator.hh
 *
 * CLASS DESCRIPTION:
 *
 * AUTHOR: Kevin Kroeninger
 *
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 *
 * FIRST SUBMISSION: 06-29-2007
 *
 * REVISION: MM-DD-YYYY
 *
 *   12-07-2007, added MC information to output scheme, Kevin 
 *
 */

#ifndef _GEOUTPUTHADESSCINTILLATOR_HH
#define _GEOUTPUTHADESSCINTILLATOR_HH

#include "TObject.h"
#include "G4ThreeVector.hh"

#include "globals.hh"
#include "io/MGOutputRoot.hh"

#include "G4ClassificationOfNewTrack.hh"

// ---------------------------------------------------------------------------

// // ROOT declarations
// class TFile;
// class TH1D;
// class TTree;

// //G4 classes
// class G4Run;
// class G4Event;
// class G4DynamicParticle;
// class G4Navigator;

// ---------------------------------------------------------------------------

class GEOutputHadesScintillator : public MGOutputRoot
{

public:

  /// Default constructor
  GEOutputHadesScintillator();

  /// Copy constructor
  GEOutputHadesScintillator(const GEOutputHadesScintillator &);

  /// Destructor
  ~GEOutputHadesScintillator();

  // public functions to replace those in MGOutputRoot

  void DefineSchema();

  void BeginOfRunAction();                        // define tree here

  void BeginOfEventAction(const G4Event *event);  // copy primary particles

  void EndOfEventAction(const G4Event *event);    // hits from sensitive D

  void EndOfRunAction();                          // close tree

  void RootSteppingAction(const G4Step *step);

  G4ClassificationOfNewTrack StackingAction(const G4Track *track);

private: 

  static const G4int MAX_NVERTICES=10; 
  static const G4int MAX_NPARTICLES=2000;

  // variables to be saved to ntuples

  Int_t    eventnumber;
  Float_t  energy_gedetector1;
  Float_t  energy_gedetector2;
  Float_t  energy_scintillator1; 
  Float_t  energy_scintillator2; 

  Int_t    vertex_nvertices; 
  Float_t  vertex_xpos[MAX_NVERTICES]; 
  Float_t  vertex_ypos[MAX_NVERTICES];
  Float_t  vertex_zpos[MAX_NVERTICES];
  Float_t  vertex_time[MAX_NVERTICES];
  Int_t    vertex_nparticles[MAX_NVERTICES]; 

  Int_t    mc_nparticles; 
  Int_t    mc_pdgcode[MAX_NPARTICLES]; 
  Int_t    mc_ivertex[MAX_NPARTICLES]; 
  Float_t  mc_px[MAX_NPARTICLES];
  Float_t  mc_py[MAX_NPARTICLES];
  Float_t  mc_pz[MAX_NPARTICLES];
  Float_t  mc_pe[MAX_NPARTICLES];
  Int_t    mc_id[MAX_NPARTICLES]; 

  // navigator

  G4Navigator* gNavigator;

};

#endif

