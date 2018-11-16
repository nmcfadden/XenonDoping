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
//
// $Id: GETrajectoryPoint.cc,v 1.2 2006-07-31 12:46:54 jliu Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// ---------------------------------------------------------------
//
// GETrajectoryPoint.cc
//
// ---------------------------------------------------------------

#include "gerdaio/GETrajectoryPoint.hh"

#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UnitsTable.hh"
#include <sstream>

G4Allocator<GETrajectoryPoint> GETrajectoryPointAllocator;

GETrajectoryPoint::GETrajectoryPoint()
{
  fPosition = G4ThreeVector(0.,0.,0.);
  fELost=0.0;
  fStepLength=0.0;
  fTrackLength=0.0;
  fVolumeName="World";
  fProcessName="null";
}

GETrajectoryPoint::GETrajectoryPoint(G4ThreeVector pos, G4String volname)
{
  fPosition = pos;
  fELost=0.0;
  fStepLength=0.0;
  fTrackLength=0.0;
  fVolumeName=volname;
  fProcessName="initstep";
}

GETrajectoryPoint::GETrajectoryPoint(G4ThreeVector pos)
{
  fPosition = pos;
  fELost=0.0;
  fStepLength=0.0;
  fTrackLength=0.0;
  fVolumeName="Null";
  fProcessName="initstep";
}



GETrajectoryPoint::GETrajectoryPoint(const GETrajectoryPoint &right)
 : G4VTrajectoryPoint(),fPosition(right.fPosition),
   fELost(right.fELost), fStepLength(right.fStepLength),
   fTrackLength(right.fTrackLength),fVolumeName(right.fVolumeName),
   fProcessName(right.fProcessName)
{
}

GETrajectoryPoint::~GETrajectoryPoint()
{
}

const std::map<G4String,G4AttDef>* GETrajectoryPoint::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("GETrajectoryPoint",isNew);
  if (isNew) {
    G4String Pos("Pos");
    (*store)[Pos] = G4AttDef(Pos, "Position", "Physics","","G4ThreeVector");
  }
  return store;
}

std::vector<G4AttValue>* GETrajectoryPoint::CreateAttValues() const
{
  std::ostringstream ss;

  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  ss.seekp(std::ios::beg);
  ss << G4BestUnit(fPosition,"Length") << std::ends;
  values->push_back(G4AttValue("Pos",ss.str(),""));

  return values;
}
