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
// CLASS IMPLEMENTATION:  MJSTCDipstickFlange.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Part Origin: At the top of the flange (knife-edge up), (x,y) centered.
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday June 12 2013
 * 
 * REVISION:
 * 06-12-2013, Created, J. Strain
 * 11-14-2013, Updated visualization properties to make compatible with Ray Tracer, J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCDipstickFlange.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCDipstickFlange::MJSTCDipstickFlange(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "STCDipstickFlangeDwg", "SS303") //partName, serialNumber, drawingNumber, material
{;}

MJSTCDipstickFlange::MJSTCDipstickFlange(const MJSTCDipstickFlange & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCDipstickFlange::~MJSTCDipstickFlange()
{;}

G4LogicalVolume* MJSTCDipstickFlange::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; //STCCanDwg_SS303
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
  
  	G4double zPlanes[] = {0*mm, -1.27*mm, -0.249*mm, -1.27*mm, -1.27*mm, -16.104*mm, -16.104*mm, -22.098*mm};
  	G4double rInner[] = {83.82*mm, 83.82*mm, 80.01*mm, 80.01*mm, 76.2*mm, 76.2*mm, 28.575*mm, 28.575*mm};
	G4double rOuter[] = {101.219*mm, 101.219*mm, 101.219*mm, 101.219*mm, 101.219*mm, 101.219*mm, 101.219*mm, 101.219*mm};  
    G4Polycone* bulk = new G4Polycone("bulk", 0, 2*pi, 8, zPlanes, rInner, rOuter);

	G4Tubs* innerCyl = new G4Tubs("innerCyl", 25.4*mm, 28.575*mm, 10.414*mm, 0, 2*pi);

    G4UnionSolid* flange = new G4UnionSolid("flange", bulk, innerCyl,
    										0, G4ThreeVector(0, 0, -11.684*mm));                                                               

    G4VisAttributes* SSVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // gray
    SSVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(flange, material, logicalName);
    pVol->SetVisAttributes(SSVisAtt); 
    MGLog(debugging) << "Created STC Dipstick Flange Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Dipstick Flange Logical" << endlog; 
  return pVol;
}  
