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
/**
 *
 * CLASS DECLARATION:  GEOutputCrystal.hh
 *
 * DESCRIPTION: 
 *   It is a class not only for output but also trying to get full
 *   control of the simulation in a general way.
 *
 * Output Control:
 *   The class gets as much as possible information directly from the
 *   G4Step object, fills it into a tree and save the tree to a ROOT
 *   file which can be opened in ROOT without loading any extra libs.
 *
 * Simulation Control:
 *
 * 1. One can seperate nuclei in the same decay chain into different events
 *    by using the macro commands: useTimeWindow, setTimeWindow (see USAGE)
 * 
 * x. kill specified particles (coming later)
 *
 * USAGE:
 *
 *   /MG/eventaction/rootschema Crystal
 *
 *   /MG/eventaction/Crystal/save any combination of "source hits tracks all"
 *    source: info. of primary vertex and particles
 *    hits:   info. of simulation in sensitive volumes
 *    tracks: info. of simulation in all volumes (including "hits")
 *    all:    info. of everything
 *    if the parameter is omitted, "hits" is used by default
 *
 *   /MG/eventaction/Crystal/useTimeWindow
 *   /MG/eventaction/Crystal/setTimeWindow 20 microsecond
 *
 *   /MG/eventaction/Crystal/kill (coming later)
 *
 * The class recognizes sensitive volumes simply by physical volume
 * names. For example, if you want to change "aPhysVol" to a sensitive
 * one, just rename it as "sensitive_aPhysVol".
 *
 * The class recognizes the types of crystals also by their physical
 * volume names, and then does the segmentation according to the
 * type. So that users don't need to do the segmentation in their
 * geometry construction class. For example, if you name your volume
 * (a simple G4Tubs) as "siegfried", hits inside this volume will have
 * a segment ID according to their positions and "siegfried"
 * segmentation schema.
 *
 * The class recognizes the positions of the crystal by their
 * "CopyNo.". For example, if you have 3 detectors in LAr, place their
 * logical volumes into mother volume "LAr" from top to bottom with
 * CopyNo.: 0, 1, 2. Hits inside these volumes will have a crystal ID:
 * 1, 2, 3. Currently maximum 3 detectors are allowed.
 *
 * Detailed information about this class can be found in MaGe Gerda
 * Users' Guide.
 *
 * REVISION: MM-DD-YYYY
 *
 *   XX-11-2007: created, Jing Liu
 *   04-08-2008: added TimeWindow functions to chop event, Jing Liu
 *   05-29-2008: added Super-Siegfried index, Jing Liu
 *   05-29-2008: documented the decay chain chopping, Jing Liu
 *
 */

#ifndef _GEOutputCrystal_hh
#define _GEOutputCrystal_hh

#include "io/MGOutputRoot.hh"

using namespace std;

class GEOutputCrystal:public MGOutputRoot
{
public:
   GEOutputCrystal(G4bool isMother);
   ~GEOutputCrystal();

   inline void SetSaveSourceFlag(G4bool flag)                { saveSource = flag; }
   inline void SetSaveHitsInSensitiveVolumeFlag(G4bool flag) { saveHitsInSensitiveVolume = flag; }
   inline void SetSaveHitsInOtherVolumeFlag(G4bool flag)     { saveHitsInOtherVolume = flag; }

   inline G4bool SaveSource()                 { return saveSource; }
   inline G4bool SaveHitsInSensitiveVolume()  { return saveHitsInSensitiveVolume; }
   inline G4bool SaveHitsInOtherVolume()      { return saveHitsInOtherVolume; }

   //inherited virtual functions
   void BeginOfRunAction();
   void DefineSchema();
   void BeginOfEventAction(const G4Event*);
   void RootSteppingAction(const G4Step*);
   void EndOfEventAction(const G4Event*);
   void EndOfRunAction();
   void WritePartialEvent(const G4Event*);
   void ResetPartialEvent(const G4Event*);

private:
   G4int ProcessIndex(G4String processName); // index processes by integers

   void saveHitsInfo(const G4Step*);
   void saveCrystalInfo(const G4Step*, G4String volumeName);

   G4bool saveSource;
   G4bool saveHitsInSensitiveVolume;
   G4bool saveHitsInOtherVolume;
// -----------------------------------------------------------------------

   G4int ranSeed;                 // = random seed for this run
   G4int eventID;

// -----------------------------------------------------------------------

   G4int Nsrc;                      // NprimaryVertex * NprimaryParticle
   static const G4int maxNsrc = 10; // maximum 10 per event

   G4float srcX[maxNsrc],srcY[maxNsrc],srcZ[maxNsrc],srcT[maxNsrc];
   G4float srcPx[maxNsrc],srcPy[maxNsrc],srcPz[maxNsrc],srcEk[maxNsrc];
   G4float srcMass[maxNsrc],srcCharge[maxNsrc]; ///< PDG mass and charge
   G4int   srcPDG[maxNsrc];                     ///< PDG encoding

// -----------------------------------------------------------------------

   G4int Nhits;                        ///< total number of hits
   static const G4int maxNhits = 2000; ///< maximum 2000 per event

   G4float hitWorldX[maxNhits],hitWorldY[maxNhits],hitWorldZ[maxNhits];
   G4float hitX[maxNhits],hitY[maxNhits],hitZ[maxNhits];  ///< local coordinates
   G4float hitR[maxNhits],hitPhi[maxNhits];               ///< local coordinates

   G4float hitPx[maxNhits],hitPy[maxNhits],hitPz[maxNhits];

   G4float hitDeltaE[maxNhits]; ///< energy loss along step
   G4float hitE[maxNhits];      ///< total energy deposition per step
   G4float hitT[maxNhits];      ///< time lapse since event starts

   G4float hitCharge[maxNhits]; ///< PDG charge
   G4int   hitPDG[maxNhits];    ///< PDG encoding
   G4int   hitBaryonNumber[maxNhits];

   G4int   hitProcess[maxNhits]; ///< an index of process that
                                 ///< determined the length of step

   G4float hitStepLength[maxNhits],hitTrackLength[maxNhits];
   G4int   hitTrackParentID[maxNhits],hitTrackID[maxNhits];

// -----------------------------------------------------------------------

   G4int hitCrystalType[maxNhits]; ///< Type index:
                                   ///<-1 : not a sensitive volume at all
                                   ///< 0 : sensitive volume but not a crystal
                                   ///< 1 : RolandI
                                   ///< 2 : RolandII
                                   ///< 3 : RolandIII
                                   ///< 4 : Siegfried
                                   ///< 5 : Super-Siegfried

   G4int hitCrystalID[maxNhits]; ///< ID index:
                                 ///<-1 : not a sensitive volume at all
                                 ///< 0 : sensitive volume but not a crystal
                                 ///< 1 : the first crystal
                                 ///< 2 : the second crystal
                                 ///< ...

   G4int hitSegmentID[maxNhits]; ///< Segment index:
                                 ///<-1 : not a sensitive volume at all
                                 ///< 0 : the whole volume which is sensitive
                                 ///< 1 : the first segment
                                 ///< 2 : the second segment
                                 ///< ...

   static const G4int maxNcrystals = 4; ///< 1 for sensitive volume, 3
                                        ///< for detectors
   static const G4int maxNsegments = 20;
   G4float segEnergy[maxNcrystals][maxNsegments]; 
   G4int segNhits[maxNcrystals][maxNsegments]; 
   // segEnergy[CrystalID][SegmentID] & segNhits[CrystalID][SegmentID]
   // CrystalID = 0 is for sensitive volume other than a crystal.
   // SegmentID = 0 is for the whole volume.

// -----------------------------------------------------------------------
   G4int Nnuclei; ///< number of nuclei undergo delayed radioactive decay
   static const G4int maxNnuclei = 5;
   G4float MotherNuclearX[maxNnuclei];
   G4float MotherNuclearY[maxNnuclei];
   G4float MotherNuclearZ[maxNnuclei];
   G4float MotherNuclearMass[maxNnuclei];
   G4float MotherNuclearCharge[maxNnuclei];
   G4int MotherNuclearPDG[maxNnuclei];
};

#endif
