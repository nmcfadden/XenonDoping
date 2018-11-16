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
 * CLASS DECLARATION:  GEGeometrySDHit.hh
 *
 * DESCRIPTION:
 *
 *   A class for hits from all the sensitive detectors defined
 *
 * AUTHOR:  Xiang Liu
 *
 * REVISION: MM-DD-YYYY
 *
 *   03-13-2005, Xiang, try to add track ID and PID information to hits
 *               so we know which track created each hit
 *               also added the hit time information
 *   04-02-2007, Luciano, Added PhysicalVolume information
 *   06-16-2009, Luciano, Added ParentID information
 */

#ifndef _GEGEOMETRYSDHIT_HH
#define _GEGEOMETRYSDHIT_HH

//---------------------------------------------------------------------------//

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

//---------------------------------------------------------------------------//

class GEGeometrySDHit : public G4VHit
{
public:

  /// Default constructor
  GEGeometrySDHit();

  /// Copy constructor
  GEGeometrySDHit(const GEGeometrySDHit &right);

  /// Destructor
  ~GEGeometrySDHit();

  const GEGeometrySDHit& operator=(const GEGeometrySDHit &right);
  G4int operator==(const GEGeometrySDHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  void Draw();
  void Print();

private:

  G4double edep;
  G4ThreeVector pos;
  G4ThreeVector localpos;
  G4double ekinetic;
  G4double steplength;
  G4double tracklength;
  G4String volumename;
  G4int    copynumber;
  G4double hittime;     // time in ns unit
  G4int    trackid;     // track id
  G4int    trackpdg;    // track type
  G4int    trackparentid; //track parent id
  G4VPhysicalVolume* pVolume;

public:

  inline void SetEdep(G4double de)              { edep = de; }
  inline G4double GetEdep()                     { return edep; }

  inline void SetPos(G4ThreeVector xyz)         { pos = xyz; }
  inline G4ThreeVector GetPos()                 { return pos; }

  inline void SetLocalPos(G4ThreeVector lxyz)    { localpos = lxyz; }
  inline G4ThreeVector GetLocalPos()             { return localpos; }

  inline void SetEkinetic(G4double ke)           { ekinetic = ke; }
  inline G4double GetEkinetic()                  { return ekinetic; }

  inline void SetSteplength(G4double l)          { steplength = l; }
  inline  G4double GetSteplength()               { return steplength; }

  inline void SetTracklength(G4double l)         { tracklength = l; }
  inline  G4double GetTracklength()              { return tracklength; }

  inline void SetVolumename(G4String name)       { volumename = name; }
  inline G4String  GetVolumename()               { return volumename; }

  inline void SetCopynumber(G4int n)             { copynumber = n; }
  inline G4int GetCopynumber()                   { return copynumber; }

  inline void SetHittime(G4double t)             { hittime = t; }
  inline G4double GetHittime()                   { return hittime; }

  inline void SetTrackID(G4int id)               { trackid = id; }
  inline G4int GetTrackID()                      { return trackid; }

  inline void SetParentTrackID(G4int id)         { trackparentid = id; }
  inline G4int GetParentTrackID()                { return trackparentid; }

  inline void SetTrackPDG(G4int id)              { trackpdg = id; }
  inline G4int GetTrackPDG()                     { return trackpdg; }

  inline void SetPhysicalVolume(G4VPhysicalVolume* vol) { pVolume = vol; }
  inline G4VPhysicalVolume* GetPhysicalVolume()         { return pVolume; }

};

// ---------------------------------------------------------------------------

typedef G4THitsCollection<GEGeometrySDHit> GEGeometrySDHitsCollection;

// ---------------------------------------------------------------------------

extern G4Allocator<GEGeometrySDHit> GEGeometrySDHitAllocator;

// ---------------------------------------------------------------------------

inline void* GEGeometrySDHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) GEGeometrySDHitAllocator.MallocSingle();
  return aHit;
}

// ---------------------------------------------------------------------------

inline void GEGeometrySDHit::operator delete(void *aHit)
{
  GEGeometrySDHitAllocator.FreeSingle((GEGeometrySDHit*) aHit);
}

#endif

