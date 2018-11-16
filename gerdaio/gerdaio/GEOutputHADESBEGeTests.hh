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
 * CLASS DECLARATION:  GEOutputHADESBEGeTests.hh
 *
 * DESCRIPTION:
 *
 *   A class to dump an energy spectrum for a typical Ge detector used for gamma-ray 
 *   spectrometry
 *
 * AUTHOR:  B. Lehnert
 *
 * REVISION: MM-DD-YYYY
 *
 *      12-21-2011  Derived from GEOutputDario.hh,  B. Lehnert
 *
 *
 */

#ifndef _GEOUTPUTHADESBEGETESTS_HH
#define _GEOUTPUTHADESBEGETESTS_HH

#include "TObject.h"
#include "G4ThreeVector.hh"

#include "globals.hh"
#include "io/MGOutputRoot.hh"

#include "G4ClassificationOfNewTrack.hh"
//---------------------------------------------------------------------------//
// ROOT declarations
class TFile;
class TH1D;
class TTree;

//G4 classes
class G4Run;
class G4Event;
class G4DynamicParticle;
class G4Navigator;
class GEOutputHADESBEGeTestsMessenger;

//---------------------------------------------------------------------------//


class GEOutputHADESBEGeTests : public MGOutputRoot
{
public:
  //default constructor
  GEOutputHADESBEGeTests();

  //copy constructor
  GEOutputHADESBEGeTests(const GEOutputHADESBEGeTests &);

  //destructor
  ~GEOutputHADESBEGeTests();

  // public functions to replace those in MGOutputRoot
  void DefineSchema();
  void BeginOfRunAction();                        // define tree here
  void BeginOfEventAction(const G4Event *event);  // copy primary particles
  void EndOfEventAction(const G4Event *event);    // hits from sensitive D
  void EndOfRunAction();                          // close tree
  void RootSteppingAction(const G4Step *step);
  G4ClassificationOfNewTrack StackingAction(const G4Track *track);

  void PrepareFileForClosing(); 

  void SetWriteHitCoordinates(G4bool val){fWriteHitCoordinates = val;};
  G4int GetWriteHitCoordinates(){return fWriteHitCoordinates;};

  void SetWriteDLInformation(G4bool val){fWriteDLInformation = val;};
  G4bool GetWriteDLInformation(){return fWriteDLInformation;};

  void SetWritePPInformation(G4bool val){fWritePPInformation = val;};
  G4bool GetWritePPInformation(){return fWritePPInformation;};

  void SetWritePVPInformation(G4bool val){fWritePVPInformation = val;};
  G4bool GetWritePVPInformation(){return fWritePVPInformation;};

  void FillHitsToNtupleTrees(const G4Event *event);
  void FillNtuple();

  //private  members
private: 

  static const G4int MAX_NHITS=5000;

  // variables to be saved to ntuples
  Int_t    eventnumber;
  Float_t  hits_tote;
  Int_t    hits_totnum;

  Int_t   isPP;

  Float_t vertex_xpos;
  Float_t vertex_ypos;
  Float_t vertex_zpos;
  Int_t vertex_numparticle;

  Float_t  vertex_distToUpperSurf;
  Float_t  vertex_distToLowerSurf;
  Float_t  vertex_distToSideSurf;
  Float_t  vertex_distToCornerSurf;
  Float_t  vertex_distToDitchSurf;

  Float_t  hits_edep[MAX_NHITS];
  Float_t  hits_xpos[MAX_NHITS];
  Float_t  hits_ypos[MAX_NHITS];
  Float_t  hits_zpos[MAX_NHITS];
  Int_t    hits_iddet[MAX_NHITS];  // which Ge detector this hit is in

  Float_t  hits_distToUpperSurf[MAX_NHITS];
  Float_t  hits_distToLowerSurf[MAX_NHITS];
  Float_t  hits_distToSideSurf[MAX_NHITS];
  Float_t  hits_distToCornerSurf[MAX_NHITS];
  Float_t  hits_distToDitchSurf[MAX_NHITS];


  Int_t numberOfPrimaries;

  //energy deposit in argon
  Float_t ene_in_argon;

  G4bool rootFileOpened;

  G4Navigator* gNavigator;

  TH1D* DetEnergy;
  TH1D* DetEnergyPP;
  TH1D* DetHitsX;
  TH1D* DetHitsY;
  TH1D* DetHitsZ;

  TH1D* VertexPosX;
  TH1D* VertexPosY;
  TH1D* VertexPosZ;
  TH1D* VertexPartNum;

  TH1D* VertexDistToUpperSurface;
  TH1D* VertexDistToLowerSurface;
  TH1D* VertexDistToSideSurface;
  TH1D* VertexDistToCornerSurface;
  TH1D* VertexDistToDitchSurface;

  TH1D* DistToUpperSurface;
  TH1D* DistToLowerSurface;
  TH1D* DistToSideSurface;
  TH1D* DistToCornerSurface;
  TH1D* DistToDitchSurface;

  std::ofstream* fHitOutputFile;

  std::vector<G4String> GeDetectorNames;
  std::vector<Float_t> *energyInDetectors;

  G4bool fWriteHitCoordinates;
  G4bool fWriteDLInformation;
  G4bool fWritePPInformation;
  G4bool fWritePVPInformation;

  GEOutputHADESBEGeTestsMessenger* fMessenger;
};

#endif

