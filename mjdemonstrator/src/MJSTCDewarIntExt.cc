//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//      
// CLASS IMPLEMENTATION:  MJSTCDewarIntExt.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from DWG No X3314 from Kadel Engineering.
 * Part Origin: At the top of the aluminum dewar exterior, (x,y) centered.
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Thursday October 17 2013
 * 
 * REVISION: 
 * 10-17-2013, Created, J. MacMullin
 * 11-14-2013, Updated visualization properties to make compatible with Ray Tracer, J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCDewarIntExt.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCDewarIntExt::MJSTCDewarIntExt(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJSTCDewarIntExtDwg", "Aluminum") 
{;}

MJSTCDewarIntExt::MJSTCDewarIntExt(const MJSTCDewarIntExt & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCDewarIntExt::~MJSTCDewarIntExt()
{;}

G4LogicalVolume* MJSTCDewarIntExt::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

  	G4double zPlanesE[] = {0*mm, -11.43*mm, -11.43*mm, -(574.802-11.43)*mm, -(574.802-11.43)*mm, -574.802*mm};
  	G4double rInnerE[] = {44.45*mm, 44.45*mm, (215.9-3.81)*mm, (215.9-3.81)*mm, 0*mm, 0*mm}; 
	G4double rOuterE[] = {215.9*mm, 215.9*mm, 215.9*mm, 215.9*mm, 215.9*mm, 215.9*mm};
    G4Polycone* exterior = new G4Polycone("exterior", 0, 2*pi, 6, zPlanesE, rInnerE, rOuterE);
    
    G4double zPlanesI[] = {0*mm, -(7.62+0.03)*mm, -(7.62+0.03)*mm, -(152.4+7.62+0.03)*mm, -(38.1+152.4+7.62+0.03)*mm, 
    					-(19.558+38.1+152.4+7.62+0.03)*mm, -(520.7-38.1-19.558+0.03)*mm, -(520.7-38.1+0.03)*mm, -(520.7+0.03)*mm};
    G4double rInnerI[] = {44.45*mm, 44.45*mm, 44.45*mm, 44.45*mm, 156.972*mm, 
    					174.625*mm, 174.625*mm, 156.972*mm, 0*mm};
	G4double rOuterI[] = {70.612*mm, 70.612*mm, (44.45+3.81)*mm, (44.45+3.81)*mm, (156.972+3.81)*mm,
						(174.625+3.81)*mm, (174.625+3.81)*mm, (156.972+3.81)*mm, 3.81*mm};
	G4Polycone* interior = new G4Polycone("interior", 0, 2*pi, 9, zPlanesI, rInnerI, rOuterI);
	
	G4UnionSolid* dewarBody = new G4UnionSolid("dewarBody", exterior, interior,
											0, G4ThreeVector(0, 0, -(11.43-0.03)*mm));
  										    
    G4VisAttributes* AlVisAtt = new G4VisAttributes(G4Colour(0.75, 0.75, 0.75)); // light gray
    AlVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(dewarBody, material, logicalName);
    pVol->SetVisAttributes(AlVisAtt); 
    MGLog(debugging) << "Created STC Dewar Interior and Exterior Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Dewar Interior and Exterior Logical" << endlog; 
  return pVol;
}  
