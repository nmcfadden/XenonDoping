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
// CLASS IMPLEMENTATION:  MJSTCDipstickEndCap.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from UW SolidWorks file.
 * Part Origin: At the top of the End Cap, (x,y) centered.
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
#include "mjdemonstrator/MJSTCDipstickEndCap.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJSTCDipstickEndCap::MJSTCDipstickEndCap(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "STCDipstickEndCapDwg", "Copper-OFHC") //partName, serialNumber, drawingNumber, material
{;}

MJSTCDipstickEndCap::MJSTCDipstickEndCap(const MJSTCDipstickEndCap & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCDipstickEndCap::~MJSTCDipstickEndCap()
{;}

G4LogicalVolume* MJSTCDipstickEndCap::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
  
  	G4double zPlanes[] = {0.0*mm, -2.286*mm, -2.286*mm, -38.1*mm};
  	G4double rInner[] = {0.0*mm, 0.0*mm, 0.0*mm, 0.0*mm};
	G4double rOuter[] = {24.511*mm, 24.511*mm, 25.4*mm, 25.4*mm};  
    G4Polycone* endcap = new G4Polycone("endcap", 0, 2*pi, 4, zPlanes, rInner, rOuter);

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); //copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(endcap, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created STC Dipstick End Cap Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Dipstick End Cap Logical" << endlog; 
  return pVol;
}  
