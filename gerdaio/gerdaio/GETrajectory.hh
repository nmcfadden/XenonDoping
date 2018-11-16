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
 * CLASS DECLARATION:  GETrajectory.hh
 *
 * DESCRIPTION:
 *
 *   A class to represent the trajectory of a particle tracked.
 *   It includes information of 
 *     1) List of trajectory points which compose the trajectory,
 *     2) static information of particle which generated the 
 *        trajectory,
 *     3) trackID and parent particle ID of the trajectory,
 *
 * Contact:
 *   Questions and comments to this code should be sent to
 *     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
 *     Makoto  Asai   (e-mail: asai@kekvax.kek.jp)
 *     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
 *
 * REVISION: MM-DD-YYYY
 *
 *   02-07-2006, added trajectory mass, Xiang
 */


#ifndef GETrajectory_h
#define GETrajectory_h 1

#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include <stdlib.h>                 // Include from 'system'
#include "G4ios.hh"               // Include from 'system'
#include <vector>            // G4RWTValOrderedVector
#include "globals.hh"               // Include from 'global'
#include "G4ParticleDefinition.hh"  // Include from 'particle+matter'
#include "G4Track.hh"
#include "G4Step.hh"

#include "gerdaio/GETrajectoryPoint.hh"     // Include from 'tracking'


class G4Polyline;                   // Forward declaration.

typedef std::vector<GETrajectoryPoint*> GETrajectoryPointContainer;

class GETrajectory : public G4VTrajectory
{

public:

// Constructor/Destrcutor
   GETrajectory();

   GETrajectory(const G4Track* aTrack);
   GETrajectory(GETrajectory &);
   ~GETrajectory();

// Operators
   inline void* operator new(size_t);
   inline void  operator delete(void*);
   inline int operator == (const GETrajectory& right) const
   {return (this==&right);} 

// Get/Set functions 
   inline G4int GetTrackID() const
     { return fTrackID; }
   inline G4int GetParentID() const
     { return fParentID; }
   inline G4String GetParticleName() const
     { return ParticleName; }
   inline G4double GetCharge() const
     { return PDGCharge; }
   inline G4int GetPDGEncoding() const
     { return PDGEncoding; }
   inline G4ThreeVector GetInitialMomentum() const
     { return initialMomentum; }
   inline G4double GetMass() const
     { return ParticleMass; }
   inline G4int GetLeptonNumber() const
     { return ParticleLeptonNumber; }
   inline G4int GetBaryonNumber() const
     { return ParticleBaryonNumber; }


// Other member functions
   void ShowTrajectory(std::ostream& os=G4cout) const;
   //void DrawTrajectory(G4int i_mode=0) const;
   void AppendStep(const G4Step* aStep);
   int GetPointEntries() const { return positionRecord->size(); }
   GETrajectoryPoint* GetPoint(G4int i) const 
     { return (*positionRecord)[i]; }
   void MergeTrajectory(G4VTrajectory* secondTrajectory);

   G4ParticleDefinition* GetParticleDefinition();

   const std::map<G4String,G4AttDef>* GetAttDefs() const;
   std::vector<G4AttValue>* CreateAttValues() const;

//---------
   private:
//---------

  GETrajectoryPointContainer* positionRecord;
//  G4std::vector<GETrajectoryPoint*>* positionRecord;

  G4int                     fTrackID;
  G4int                     fParentID;
  G4int                     PDGEncoding;
  G4double                  PDGCharge;
  G4String                  ParticleName;
  G4ThreeVector             initialMomentum;
  G4double                  ParticleMass;
  G4int                     ParticleLeptonNumber;
  G4int                     ParticleBaryonNumber;

};

extern G4Allocator<GETrajectory> GETrajectoryAllocator;

inline void* GETrajectory::operator new(size_t)
{
  void* aTrajectory;
  aTrajectory = (void*)GETrajectoryAllocator.MallocSingle();
  return aTrajectory;
}

inline void GETrajectory::operator delete(void* aTrajectory)
{
  GETrajectoryAllocator.FreeSingle((GETrajectory*)aTrajectory);
}

#endif

