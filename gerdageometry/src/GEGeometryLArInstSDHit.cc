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
 * AUTHOR:  Nuno Barros
 * CONTACT:
 * FIRST SUBMISSION: 05-22-2012
 * DESCRIPTION: Implements the hit class (and collection) for the LAr Instrumentation.
 * REVISION: MM-DD-YYYY
 *
 * 09-07-2012, N. Barros, * Added also the name of the physical volume to the hit class.
 *
 **/
#include "gerdageometry/GEGeometryLArInstSDHit.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4ios.hh"
#include <iomanip>

G4Allocator<GEGeometryLArInstSDHit> GEGeometryLArInstSDHitAllocator;

GEGeometryLArInstSDHit::GEGeometryLArInstSDHit() : 
  G4VHit()
{ 
  pLogV=NULL; // initialize the logical volume to NULL
  ndet=-1;    // number of instrumentation parts
  etotal = 0.;
  hittime = 0.;
  position = G4ThreeVector(0., 0., 0.);
  direction = G4ThreeVector(0., 0., 0.);
  partname = "";
}

GEGeometryLArInstSDHit::GEGeometryLArInstSDHit(G4LogicalVolume* logVol)
:pLogV(logVol)
{
  ;
}

GEGeometryLArInstSDHit::~GEGeometryLArInstSDHit()
{
 ;
}

GEGeometryLArInstSDHit::GEGeometryLArInstSDHit(const GEGeometryLArInstSDHit &right) 
  : G4VHit()
{
//  edep = right.edep;
	pLogV = right.pLogV;
	ndet = right.ndet;
	etotal = right.etotal;
	hittime = right.hittime;
	position = right.position;
	direction = right.direction;
	partname = right.partname;

}

const GEGeometryLArInstSDHit& GEGeometryLArInstSDHit::operator=(const GEGeometryLArInstSDHit &right)
{
	pLogV = right.pLogV;
	ndet = right.ndet;
	etotal = right.etotal;
	hittime = right.hittime;
	position = right.position;
	direction = right.direction;
	partname = right.partname;

  return *this;
}

int GEGeometryLArInstSDHit::operator==(const GEGeometryLArInstSDHit& /*newhit*/) const
{
  return 0;
}

void GEGeometryLArInstSDHit::Draw()
{
/*
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {

	  G4Transform3D trans(rotation,position);
      G4VisAttributes attribs;
      const G4VisAttributes* pVA = pLogV->GetVisAttributes();

      if(pVA)
	{
	  attribs = *pVA;
	}

      G4Colour colour(1.,0.,0.);
      attribs.SetColour(colour);
      attribs.SetForceWireframe(false);
      attribs.SetForceSolid(true);
      pVVisManager->Draw((*pLogV),attribs,trans);
    }
    */
	;
}

void GEGeometryLArInstSDHit::Print()
{
/*
	MGLog(debugging) << "Dumping hit:"<< endlog;
*/
  ;
}
