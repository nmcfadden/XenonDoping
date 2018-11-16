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
// CLASS IMPLEMENTATION:  MJSTCDewarLN.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from DWG No X3314 from Kadel Engineering.
 * Part Origin: At the top of the LN, (x,y) centered. (Assumes dewar is full)
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
 * 03-25-2014, Modified spacing between "touching" surfaces, J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCDewarLN.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCDewarLN::MJSTCDewarLN(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJSTCDewarLNDwg", "Nitrogen-Liq") 
{;}

MJSTCDewarLN::MJSTCDewarLN(const MJSTCDewarLN & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCDewarLN::~MJSTCDewarLN()
{;}

G4LogicalVolume* MJSTCDewarLN::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double eps = 0.001 * mm;
    
    //Assume the dewar is full
    G4double zPlanes[] = {0*mm, -(152.4+7.62+0.03)*mm, -(38.1+152.4+7.62+0.03)*mm, -(19.558+38.1+152.4+7.62+0.03)*mm,
    					-(520.7-38.1-19.558)*mm, -(520.7-38.1)*mm, -520.7*mm+eps};
    G4double rInner[] = {0*mm, 0*mm ,0*mm, 0*mm,
    					0*mm, 0*mm, 0*mm};
    G4double rOuter[] = {44.45*mm-eps, 44.45*mm-eps, 156.972*mm-eps, 174.625*mm-eps, 
    					174.625*mm-eps, 156.972*mm-eps, 0*mm};
	G4Polycone* LNfull = new G4Polycone("LNfull", 0, 2*pi, 7, zPlanes, rInner, rOuter);
	
	G4Tubs* Dipstick = new G4Tubs("Dipstick", 0*mm, 25.4*mm+eps, ((122.631+121.717-2.286+38.1)/2.0)*mm+2.*eps, 0, 2*pi);
	G4SubtractionSolid* LN = new G4SubtractionSolid("LN", LNfull, Dipstick,
											0, G4ThreeVector(0, 0, (11.43-0.03+22.098+20.0-(122.631+121.717-2.286+38.1)/2.0)*mm));
	
    G4VisAttributes* LNVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0)); // cyan
    LNVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(LN, material, logicalName);
    pVol->SetVisAttributes(LNVisAtt); 
    MGLog(debugging) << "Created STC Dewar Liquid Nitrogen Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Dewar Liquid Nitrogen Logical" << endlog; 
  return pVol;
}  
