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


#include "gerdageometry/GEGeometryPMTSDHit.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4ios.hh"
#include <iomanip>

G4Allocator<GEGeometryPMTSDHit> GEGeometryPMTSDHitAllocator;

GEGeometryPMTSDHit::GEGeometryPMTSDHit() : 
  G4VHit()
{ 
  pLogV=NULL;                               // everything is initialized as zero, important: time and edep = 0
  ndet=-1; 
  Ndet=0;
  edep=0.; 
  time = 0.; 
  TimeInit=0.0;
  pos = G4ThreeVector(0., 0., 0.);
  particleEnergy = 0.;
  particleDirection = G4ThreeVector(0., 0., 0.);
}

GEGeometryPMTSDHit::GEGeometryPMTSDHit(G4LogicalVolume* logVol)
:pLogV(logVol)
{
  ;
}

GEGeometryPMTSDHit::~GEGeometryPMTSDHit()
{
 ;
}

GEGeometryPMTSDHit::GEGeometryPMTSDHit(const GEGeometryPMTSDHit &right) :
  G4VHit()
{
  edep = right.edep;
  pos = right.pos;
  rot = right.rot;
  pLogV = right.pLogV;
  ndet  = right.ndet;
  Ndet  = right.Ndet;
  time           = right.time;
  TimeInit       = right.TimeInit;
  particleName   = right.particleName;
  particleEnergy = right.particleEnergy;
  particleDirection = right.particleDirection;
}

const GEGeometryPMTSDHit& GEGeometryPMTSDHit::operator=(const GEGeometryPMTSDHit &right)
{
  edep = right.edep;
  pos = right.pos;
  rot = right.rot;
  pLogV = right.pLogV;
  ndet  = right.ndet;
  Ndet  = right.Ndet;
  time           = right.time;
  TimeInit       = right.TimeInit;
  particleName   = right.particleName;
  particleEnergy = right.particleEnergy;
  particleDirection = right.particleDirection;

  return *this;
}

int GEGeometryPMTSDHit::operator==(const GEGeometryPMTSDHit& /*right*/) const
{
  return 0;
}

void GEGeometryPMTSDHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4Transform3D trans(rot,pos);
      G4VisAttributes attribs;
      const G4VisAttributes* pVA = pLogV->GetVisAttributes();

      if(pVA)
	{
	  attribs = *pVA;
	}

      G4Colour colour(1.,1.,1.);
      attribs.SetColour(colour);
      attribs.SetForceWireframe(false);
      attribs.SetForceSolid(true);
      pVVisManager->Draw(*pLogV,attribs,trans);
    }
}

void GEGeometryPMTSDHit::Print()
{
  ;
}
