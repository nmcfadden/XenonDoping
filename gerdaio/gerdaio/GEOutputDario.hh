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
 * CLASS DECLARATION:  GEOutputDario.hh
 *
 * DESCRIPTION:
 *
 *   A class to dump an energy spectrum for a typical Ge detector used for gamma-ray 
 *   spectrometry
 *
 * AUTHOR:  Luciano
 *
 * REVISION: MM-DD-YYYY
 *
 *      04-02-2007  Luciano   Cleaned unused fields
 *      07-12-2007  Luciano   Added bool for checking creation of root file
 *      07-20-2007  Luciano   Added method to allow for multiple runs within the same file
 *      09-15-2009  Luciano   Log separately energies of sensitive detectors
 *      02-03-2010  Luciano   Add boolean flag to write local coordinates, add R90
 *      15-03-2012  Krzysztof  Added total energy deposit for a primary vertex
 *
 */

#ifndef _GEOUTPUTDARIO_HH
#define _GEOUTPUTDARIO_HH

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
class GEOutputDarioMessenger;

//---------------------------------------------------------------------------//


class GEOutputDario : public MGOutputRoot
{
public:
  //default constructor
  GEOutputDario();

  //copy constructor
  GEOutputDario(const GEOutputDario &);

  //destructor
  ~GEOutputDario();

  // public functions to replace those in MGOutputRoot
  void DefineSchema();
  void BeginOfRunAction();                        // define tree here
  void BeginOfEventAction(const G4Event *event);  // copy primary particles
  void EndOfEventAction(const G4Event *event);    // hits from sensitive D
  void EndOfRunAction();                          // close tree
  void RootSteppingAction(const G4Step *step);
  G4ClassificationOfNewTrack StackingAction(const G4Track *track);

  void PrepareFileForClosing(); 
  void SetWriteDetailsFlag(G4bool val){fWriteHitDetailFlag = val;};
  G4bool SetWriteDetailsFlag(){return fWriteHitDetailFlag;};

  void SetMajority(G4int ii){fMajority = ii;};
  G4int GetMajority(){return fMajority;};

  void UseLocalCoordinates(G4bool val){fUseLocalCoordinates = val;};
  G4bool GetUseLocalCoordinates(){return fUseLocalCoordinates;};

  //private  members
private: 
  // variables to be saved to ntuples
  Int_t    eventnumber;
  Float_t  hits_tote;
  Int_t    hits_totnum;

  Int_t numberOfPrimaries;
  Float_t R90;
  Float_t tote;
  Float_t primVertex[3];

  //energy deposit in argon
  Float_t ene_in_argon;

  G4bool rootFileOpened;

  G4Navigator* gNavigator;
  TH1D* energyHisto;
  TH1D* primaryGammaHisto;
  TH1D* primaryGammaHisto2;

  G4bool fWriteHitDetailFlag;
  std::ofstream* fHitOutputFile;
  G4bool fUseLocalCoordinates;

  std::vector<G4String> GeDetectorNames;
  std::vector<Float_t> *energyInDetectors;
  G4int fMajority;

  GEOutputDarioMessenger* fMessenger;
};

#endif

