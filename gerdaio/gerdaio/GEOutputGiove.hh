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
 * CLASS DECLARATION: GEOutputGiove.hh
 *
 *CLASS DESCRIPTION:
 *
 * REVISION:MM-DD-YYYY
 *
 *   06-24-2009  Alan Poon   First version
 */

#ifndef _GEOUTPUTGIOVE_HH
#define _GEOUTPUTGIOVE_HH

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
class GEOutputGioveMessenger;

//---------------------------------------------------------------------------//


class GEOutputGiove : public MGOutputRoot
{
public:
  /// Default constructor
  GEOutputGiove();

  /// Copy constructor
  GEOutputGiove(const GEOutputGiove &);

  /// Destructor
  ~GEOutputGiove();

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

  //private  members
private:
  // variables to be saved to ntuples
  Int_t    eventnumber;
  Float_t  hits_tote;
  Int_t    hits_totnum;


  G4bool rootFileOpened;
 
  G4Navigator* gNavigator;
  TH1D* energyHisto;
  TH1D* primaryGammaHisto;
  TH1D* neutronEnergyHisto;

  G4bool fWriteHitDetailFlag;
  std::ofstream* fHitOutputFile;

  GEOutputGioveMessenger* fMessenger;
};

#endif

