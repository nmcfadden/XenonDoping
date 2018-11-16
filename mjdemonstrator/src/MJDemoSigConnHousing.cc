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
// $Id: MGcodeTemplate.cc,v 1.1 2004-12-09 08:58:35 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MJDemoSigConnHousing.cc
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
 * AUTHOR: Ian Guinn
 * CONTACT: iguinn@uw.edu
 * FIRST SUBMISSION: Jun 4, 2014
 * 
 * REVISION:
 * 
 * 06-04-2014, Created, I. Guinn
 * 08-20-2014, Added flat and pin holes, I. Guinn
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Transform3D.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4GeometryTolerance.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoSigConnHousing.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;
using namespace MaGe;


const G4double MJDemoSigConnHousing::kRInner[MJDemoSigConnHousing::kNPlanes] = {0.03*in,0.03*in,0.03*in,0.03*in};
const G4double MJDemoSigConnHousing::kROuter[MJDemoSigConnHousing::kNPlanes] = {0.125*in, 0.125*in, 0.0625*in, 0.0625*in};
const G4double MJDemoSigConnHousing::kZPlanes[MJDemoSigConnHousing::kNPlanes] = {0.*in, 0.508*in, 0.544*in, 0.744*in};

MJDemoSigConnHousing::MJDemoSigConnHousing(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "SigConnHousing", "Vespel")
{;}

MJDemoSigConnHousing::MJDemoSigConnHousing(const MJDemoSigConnHousing & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoSigConnHousing::~MJDemoSigConnHousing()
{;}

G4LogicalVolume* MJDemoSigConnHousing::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4VSolid* housing = new G4Polycone("sigHousingPCone", 0, 2.*pi, GetNPlanes(), GetZPlanes(), GetRInner(), GetROuter());
    G4Tubs* pinhole = new G4Tubs("sigPinHole", 0., GetPinHoleDiam()/2., GetPinHoleLength()/2., 0., 2.*pi);

    for(int i=0; i<8; i++) housing = new G4SubtractionSolid(string("sigHousing")+(char)('1'+i)+"Hole", housing, pinhole, G4RotateZ3D((22.5+45*i)*deg)*G4Translate3D(GetPinHoleRho(), 0., GetPinHoleLength()/2));

    G4Box* housingFlat = new G4Box("sigHousingFlat", GetFlatWidth()/2., GetFlatDepth()/2., GetFlatLength()/2.);
    housing = new G4SubtractionSolid("sigHousing", housing, housingFlat, G4Translate3D(0, GetROuter()[0]-GetFlatDepth()/2., GetFlatLength()/2.));
    
    G4VisAttributes* brownVisAtt = new G4VisAttributes(G4Colour(0.63, 0.32, 0.18)); // brown
    brownVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(housing, material, logicalName);
    pVol->SetVisAttributes(brownVisAtt);
    MGLog(debugging) << "Created SigConnHousing Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing SigConnHousing Logical" << endlog; 
  return pVol;
}  
