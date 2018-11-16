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
// $Id: GEGeometrySDHit.cc,v 1.4 2009-06-16 14:42:20 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  @CLASS_NAME@.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR:  Xiang Liu
 * CONTACT: @CONTACT@
 * FIRST SUBMISSION: @START_DATE@
 * 
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 12-06-2004, xiang liu, added this line to check syncmail
 * 12-06-2004, xiang liu, added this line to check syncmail again
 * 04-02-2007, Luciano, Added PhysicalVolume information
 */
//---------------------------------------------------------------------------//
//
#include "gerdageometry/GEGeometrySDHit.hh"

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<GEGeometrySDHit> GEGeometrySDHitAllocator;

GEGeometrySDHit::GEGeometrySDHit()
{;}

GEGeometrySDHit::~GEGeometrySDHit()
{;}

GEGeometrySDHit::GEGeometrySDHit(const GEGeometrySDHit &right)
  : G4VHit()
{
  edep = right.edep;
  pos = right.pos;
  ekinetic = right.ekinetic;
  steplength = right.steplength;
  tracklength = right.tracklength;
  volumename = right.volumename;
  copynumber = right.copynumber;
  hittime    = right.hittime;
  trackid    = right.trackid;
  trackpdg   = right.trackpdg;
  trackparentid = right.trackparentid;
  pVolume = right.pVolume;
}

const GEGeometrySDHit& GEGeometrySDHit::operator=(const GEGeometrySDHit &right)
{
  edep = right.edep;
  pos = right.pos;
  ekinetic = right.ekinetic;
  steplength = right.steplength;
  tracklength = right.tracklength;
  volumename = right.volumename;
  copynumber = right.copynumber;
  hittime    = right.hittime;
  trackid    = right.trackid;
  trackpdg   = right.trackpdg;
  trackparentid = right.trackparentid;
  pVolume = right.pVolume;
  return *this;
}

G4int GEGeometrySDHit::operator==(const GEGeometrySDHit &right) const
{
  return (this==&right) ? 1 : 0;
}


void GEGeometrySDHit::Draw()
{;}


void GEGeometrySDHit::Print()
{;}

