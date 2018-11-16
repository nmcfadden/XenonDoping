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
// CLASS IMPLEMENTATION:  MJSTCDewarActChar.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from DWG No X3314 from Kadel Engineering.
 * Part Origin: The (x,y,z) middle of the activated charcoal, as viewed as a torus.
 * 
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
#include "G4Torus.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCDewarActChar.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCDewarActChar::MJSTCDewarActChar(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJSTCDewarActCharDwg", "ActivatedCharcoal") 
{
	G4String symbol;  
	G4double a, z, density;     
	G4int ncomponents,natoms;
  	
  	G4Element* C = new G4Element("Carbon", symbol="C", z=6., a=12.0107*g/mole);
  	
  	//Density is "apparent" density because activated charcoal mostly consists of air space. 
  	//"Skeletal" density is 2.0-2.1 g/cm^3.
  	G4Material* ActivatedCharcoal = new G4Material("ActivatedCharcoal", density=0.45*g/cm3, ncomponents=1);
  		ActivatedCharcoal->AddElement(C, natoms=1);
}

MJSTCDewarActChar::MJSTCDewarActChar(const MJSTCDewarActChar & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCDewarActChar::~MJSTCDewarActChar()
{;}

G4LogicalVolume* MJSTCDewarActChar::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double eps = 0.001 * mm;
    
  	G4Torus* ring = new G4Torus("ring", 0*mm, 49.0682*mm, 96.1736*mm, 0, 2*pi);
     
    G4double zPlanes[] = {0*mm, 0*mm, -(520.7-38.1)*mm, -520.7*mm-eps};
    G4double rInner[] = {0*mm, 0*mm, 0*mm, 0*mm};
    G4double rOuter[] = {0*mm, (156.972+3.81)*mm+eps, (156.972+3.81)*mm+eps, 3.81*mm};
	G4Polycone* cutAway = new G4Polycone("cutAway", 0, 2*pi, 4, zPlanes, rInner, rOuter);
	
	G4SubtractionSolid* charcoal = new G4SubtractionSolid("charcoal", ring, cutAway,
											0, G4ThreeVector(0, 0, (520.7-49.0682)*mm));
  										    
    G4VisAttributes* CharVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0)); // red
    CharVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(charcoal, material, logicalName);
    pVol->SetVisAttributes(CharVisAtt); 
    MGLog(debugging) << "Created STC Dewar Activated Charcoal Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Dewar Activated Charcoal Logical" << endlog; 
  return pVol;
}  
