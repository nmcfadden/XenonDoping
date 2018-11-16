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
// CLASS IMPLEMENTATION:  MJSTC8Flange6Bore.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from UW SolidWorks file.
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
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCMainBody8Flange6Bore.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCMainBody8Flange6Bore::MJSTCMainBody8Flange6Bore(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "STCMainBody8Flange6BoreDwg", "SS303") //partName, serialNumber, drawingNumber, material
{;}

MJSTCMainBody8Flange6Bore::MJSTCMainBody8Flange6Bore(const MJSTCMainBody8Flange6Bore & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCMainBody8Flange6Bore::~MJSTCMainBody8Flange6Bore()
{;}

G4LogicalVolume* MJSTCMainBody8Flange6Bore::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
  
  	G4double zPlanes[] = {0*mm, -9.652*mm, -9.652*mm, -22.352*mm};
  	G4double rInner[] = {76.454*mm, 76.454*mm, 73.8125*mm, 73.8125*mm};
	G4double rOuter[] = {101.219*mm, 101.219*mm, 101.219*mm, 101.219*mm};  
    G4Polycone* flange = new G4Polycone("flange", 0, 2*pi, 4, zPlanes, rInner, rOuter);

    G4VisAttributes* SSVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // gray
    SSVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(flange, material, logicalName);
    pVol->SetVisAttributes(SSVisAtt); 
    MGLog(debugging) << "Created STC Main Body 8 Flange 6 Bore Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Main Body 8 Flange 6 Bore Logical" << endlog; 
  return pVol;
}  
