//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                            MaGe Simulation                                //
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
 * CLASS DECLARATION:  GEGeometryLArInstSDHit.hh
 *
 * DESCRIPTION:
 *
 * AUTHOR: Nuno Barros
 *
 * REVISION: MM-DD-YYYY
 *
 *   05-22-2012  N. Barros, Initial commit.
 *
 */


#ifndef GEGeometryLArInstSDHit_h
#define GEGeometryLArInstSDHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class GEGeometryLArInstSDHit : public G4VHit
{
public:

  /// Default constructor
  GEGeometryLArInstSDHit();

  /// Constructor with volume information
  GEGeometryLArInstSDHit(G4LogicalVolume* logVol);

  /// Destructor
  ~GEGeometryLArInstSDHit();

  /// Copy constructor
  GEGeometryLArInstSDHit(const GEGeometryLArInstSDHit &right);

  const GEGeometryLArInstSDHit& operator=(const GEGeometryLArInstSDHit &right);
  int operator==(const GEGeometryLArInstSDHit &right) const;


  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
  void *operator new(size_t,void*p){return p;}

#ifndef G4NOT_ISO_DELETES
  void operator delete(void *,void*){}
#endif

  void Draw();
  void Print();


  inline void SetPart( G4int i)               { ndet = i; }
  inline G4int GetPart( )                     { return ndet; }

  inline void SetHittime(G4double t)          { hittime = t; }
  inline G4double GetHittime()                { return hittime; }

  inline void SetPos(G4ThreeVector xyz)       { position = xyz; }
  inline G4ThreeVector GetPos()               { return position; }

  inline const G4LogicalVolume * GetLogV()    { return pLogV; };

  inline void SetEnergy(G4double te)       { etotal = te; }
  inline G4double GetEnergy()              { return etotal; }

  inline void SetParticleDirection(G4ThreeVector xyz)       { direction = xyz; }
  inline G4ThreeVector GetParticleDirection()               { return direction; }

  inline void SetPartName(G4String name) {partname = name;};
  inline G4String GetPartName() {return partname;};

//  inline void SetTrackID(G4int id)               { trackid = id; }
//  inline G4int GetTrackID()                      { return trackid; }
//
//  inline void SetParentTrackID(G4int id)         { trackparentid = id; }
//  inline G4int GetParentTrackID()                { return trackparentid; }

private:

  G4int ndet;
  G4ThreeVector position;
  G4ThreeVector direction;

  const G4LogicalVolume* pLogV;
  G4double      etotal;
  G4double 		hittime;     // time in ns unit
  G4String		partname;
//  G4int trackid;
//  G4int trackparentid;

};

// ---------------------------------------------------------------------------

typedef G4THitsCollection<GEGeometryLArInstSDHit> GEGeometryLArInstSDHitsCollection;

// ---------------------------------------------------------------------------

extern G4Allocator<GEGeometryLArInstSDHit> GEGeometryLArInstSDHitAllocator;

// ---------------------------------------------------------------------------

inline void* GEGeometryLArInstSDHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) GEGeometryLArInstSDHitAllocator.MallocSingle();
  return aHit;
}

// ---------------------------------------------------------------------------

inline void GEGeometryLArInstSDHit::operator delete(void *aHit)
{
  GEGeometryLArInstSDHitAllocator.FreeSingle((GEGeometryLArInstSDHit*) aHit);
}

#endif

