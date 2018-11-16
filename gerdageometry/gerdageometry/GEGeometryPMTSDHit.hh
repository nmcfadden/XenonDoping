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
 * CLASS DECLARATION:  GEGeometryPMTSDHit.hh
 *
 * DESCRIPTION:
 *
 * AUTHOR:
 *
 * REVISION: MM-DD-YYYY
 *
 *   10-19-2011  Improved readability, Alex
 *   05-11-2011	 Using this class as Hit register for the LAr instrumentation.
 *   			 Added an additional method to get the incident particle direction.
 *
 */


#ifndef GEGeometryPMTSDHit_h
#define GEGeometryPMTSDHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class GEGeometryPMTSDHit : public G4VHit
{
public:

  /// Default constructor
  GEGeometryPMTSDHit();

  /// Constructor
  GEGeometryPMTSDHit(G4LogicalVolume* logVol);

  /// Destructor
  ~GEGeometryPMTSDHit();

  /// Copy constructor
  GEGeometryPMTSDHit(const GEGeometryPMTSDHit &right);

  const GEGeometryPMTSDHit& operator=(const GEGeometryPMTSDHit &right);
  int operator==(const GEGeometryPMTSDHit &right) const;


  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
  void *operator new(size_t,void*p){return p;}

#ifndef G4NOT_ISO_DELETES
  void operator delete(void *,void*){}
#endif

  void Draw();
  void Print();


  inline void SetEdep(G4double de)            { edep = de; }
  inline G4double GetEdep()                   { return edep; }
  inline void AddEdep(G4double de)            { edep += de; }

  inline void SetTime (G4double t2)           { time=t2; }
  inline void SetTimeInit(G4double ti)        { TimeInit = ti; }
  inline G4double GetTimeInit()               { return TimeInit; }
  inline void AddTime(G4double dt)            { time += dt; }

  inline void SetNdet( G4int i)               { ndet = i; }
  inline G4int GetNdet( )                     { return ndet; }
  inline void Set0Ndet()                      { Ndet = 1; }
  inline void NumDet()                        { Ndet ++; }
  inline G4int GetNumDet( )                   { return Ndet; }

  inline void SetHittime(G4double t)          { hittime = t; }
  inline G4double GetHittime()                { return hittime; }

  inline void SetPos(G4ThreeVector xyz)       { pos = xyz; }
  inline G4ThreeVector GetPos()               { return pos; }

  inline void SetRot(G4RotationMatrix rmat)   { rot = rmat; }
  inline G4RotationMatrix GetRot()            { return rot; }

  inline const G4LogicalVolume * GetLogV()    { return pLogV; };

  inline void SetTotEnergy(G4double te)       { etotal = te; }
  inline G4double GetTotEnergy()              { return etotal; }

  void SetParticle (G4String name)     { particleName = name; }
  void SetParticleEnergy (G4double e1) { particleEnergy = e1; }

  G4String GetParticle()                      { return particleName; }

  G4double GetParticleEnergy()                { return particleEnergy; }

  G4double GetTime()                          { return time; }

  inline void SetParticleDirection(G4ThreeVector xyz)       { particleDirection = xyz; }
  inline G4ThreeVector GetParticleDirection()               { return particleDirection; }


private:

  G4int ndet,Ndet;
  G4double edep;
  G4ThreeVector pos;
  G4RotationMatrix rot;
  const G4LogicalVolume* pLogV;
  G4double      time, TimeInit;
  G4String      particleName;
  G4double      particleEnergy;
  G4double      etotal;
  G4double hittime;     // time in ns unit
  G4ThreeVector particleDirection;

};

// ---------------------------------------------------------------------------

typedef G4THitsCollection<GEGeometryPMTSDHit> GEGeometryPMTSDHitsCollection;

// ---------------------------------------------------------------------------

extern G4Allocator<GEGeometryPMTSDHit> GEGeometryPMTSDHitAllocator;

// ---------------------------------------------------------------------------

inline void* GEGeometryPMTSDHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) GEGeometryPMTSDHitAllocator.MallocSingle();
  return aHit;
}

// ---------------------------------------------------------------------------

inline void GEGeometryPMTSDHit::operator delete(void *aHit)
{
  GEGeometryPMTSDHitAllocator.FreeSingle((GEGeometryPMTSDHit*) aHit);
}

#endif

