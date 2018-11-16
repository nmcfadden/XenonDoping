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
 * CLASS DECLARATION:  GETrajectoryPoint.hh
 *
 * DESCRIPTION:
 *
 *   This class represents the trajectory of a particle tracked.
 *   It includes information of
 *     1) List of trajectory points which compose the trajectory,
 *     2) static information of particle which generated the 
 *        trajectory,
 *     3) trackID and parent particle ID of the trajectory,
 *     4) termination condition of the trajectory.
 *
 * Contact:
 *   Questions and comments to this code should be sent to
 *     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
 *     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
 */

#ifndef GETrajectoryPoint_h
#define GETrajectoryPoint_h 1

#include "G4VTrajectoryPoint.hh"
#include "globals.hh"                // Include from 'global'
#include "G4ThreeVector.hh"          // Include from 'geometry'
#include "G4Allocator.hh"            // Include from 'particle+matter'


class GETrajectoryPoint : public G4VTrajectoryPoint
{

//--------
public:
//--------

// Constructor/Destructor
   GETrajectoryPoint();
   GETrajectoryPoint(G4ThreeVector pos, G4String volname);
   GETrajectoryPoint(G4ThreeVector pos);
   GETrajectoryPoint(const GETrajectoryPoint &right);
   virtual ~GETrajectoryPoint();

// Operators
   inline void *operator new(size_t);
   inline void operator delete(void *aTrajectoryPoint);
   inline int operator==(const GETrajectoryPoint& right) const
     { return (this==&right); }

// Get/Set functions
   inline const G4ThreeVector GetPosition() const { return fPosition; }
   inline void SetPosition(G4ThreeVector pos) { fPosition=pos; }

   inline G4double GetEnergyLost() const { return fELost; }
   inline void SetEnergyLost(G4double e){ fELost=e; }

   inline G4double GetStepLength() const { return fStepLength; }
   inline void SetStepLength(G4double l){ fStepLength=l; }

   inline G4double GetTrackLength() const { return fTrackLength; }
   inline void SetTrackLength(G4double l){ fTrackLength=l; }

   inline const G4String GetVolumeName() const { return fVolumeName; }
   inline void SetVolumeName(G4String ss) { fVolumeName = ss; }

   inline const G4String GetProcessName() const { return fProcessName; }
   inline void SetProcessName(G4String ss) { fProcessName = ss; }

// Get method for HEPRep style attributes
   const std::map<G4String,G4AttDef>* GetAttDefs() const;
   std::vector<G4AttValue>* CreateAttValues() const;

//---------
private:
//---------

// Member data
   G4ThreeVector fPosition;
   G4double      fELost;
   G4double      fStepLength;
   G4double      fTrackLength;
   G4String      fVolumeName;
   G4String      fProcessName;
};


extern G4Allocator<GETrajectoryPoint> GETrajectoryPointAllocator;

inline void* GETrajectoryPoint::operator new(size_t)
{
   void *aTrajectoryPoint;
   aTrajectoryPoint = (void *) GETrajectoryPointAllocator.MallocSingle();
   return aTrajectoryPoint;
}

inline void GETrajectoryPoint::operator delete(void *aTrajectoryPoint)
{
   GETrajectoryPointAllocator.FreeSingle((GETrajectoryPoint *) aTrajectoryPoint);
}

#endif

