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
// CLASS IMPLEMENTATION:  MJSTCThermalCuFoil.cc
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
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Thursday November 14 2013
 * 
 * REVISION:
 * 11-14-2013, Created, J. MacMullin
 * 03-25-2014, Modified spacing between "touching" surfaces, J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCThermalCuFoil.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCThermalCuFoil::MJSTCThermalCuFoil(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJSTCThermalCuFoilDwg", "Copper-OFHC") //partName, serialNumber, drawingNumber, material
{;}

MJSTCThermalCuFoil::MJSTCThermalCuFoil(const MJSTCThermalCuFoil & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCThermalCuFoil::~MJSTCThermalCuFoil()
{;}

G4LogicalVolume* MJSTCThermalCuFoil::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double eps = 0.001 * mm;     
     
	G4Tubs* foil = new G4Tubs("foil", 61.595*mm+eps, (61.595+0.25)*mm+eps, ((295.275-15.875)/2.)*mm, 0, 2*pi);
	G4Tubs* overlap = new G4Tubs("overlap", (61.595+0.25-0.1)*mm+eps, (61.595+0.25*2.)*mm+eps, 
											((295.275-15.875)/2.)*mm, 0, 25.4/(61.595+0.25*2.));
	G4UnionSolid* CuFoil = new G4UnionSolid("CuFoil", overlap, foil, 
											0, G4ThreeVector(0, 0, 0));	
                                                       
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(1.0,0.373,0.169,0.5)); //copper color "more red"
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(CuFoil, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created STC Thermal Cu Foil Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Thermal Cu Foil Logical" << endlog; 
  return pVol;
}  
