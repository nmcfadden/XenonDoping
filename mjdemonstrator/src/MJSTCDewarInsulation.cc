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
// CLASS IMPLEMENTATION:  MJSTCDewarInsulation.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from DWG No X3314 from Kadel Engineering.
 * Part Origin: At the top of the insulation, (x,y) centered.
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
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCDewarInsulation.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------// 

using namespace CLHEP;


MJSTCDewarInsulation::MJSTCDewarInsulation(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJSTCDewarInsulationDwg", "Mylar") 
{
	G4String symbol;  
	G4double a, z, density;     
	G4int ncomponents,natoms;
  	
  	G4Element* C = new G4Element("Carbon", symbol="C", z=6., a=12.01*g/mole);
  	G4Element* H = new G4Element("Hydrogen",   symbol="H", z=1., a=1.007*g/mole);
  	G4Element* O = new G4Element("Oxygen",   symbol="O", z=8., a=16.00*g/mole);
  	
  	//Taken from MJGeometryPopTop
  	G4Material* Mylar = new G4Material("Mylar", density=1.38*g/cm3, ncomponents=3);
  		Mylar->AddElement(C, natoms=10);
  		Mylar->AddElement(H, natoms=8);
  		Mylar->AddElement(O, natoms=4);
}

MJSTCDewarInsulation::MJSTCDewarInsulation(const MJSTCDewarInsulation & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCDewarInsulation::~MJSTCDewarInsulation()
{;}

G4LogicalVolume* MJSTCDewarInsulation::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double eps = 0.001 * mm;
    
	G4Tubs* bulk = new G4Tubs("bulk", 0.0*mm, (215.9-3.81)*mm-eps, ((574.802-2.0*11.43)/2.0)*mm-eps, 0, 2*pi);

    G4double zPlanes[] = {0.03*mm, -7.62*mm-eps, -7.62*mm-eps, -(152.4+7.62)*mm, -(38.1+152.4+7.62)*mm, 
    					-(19.558+38.1+152.4+7.62)*mm, -(520.7-38.1-19.558)*mm, -(520.7-38.1)*mm, -520.7*mm-eps};
    G4double rInner[] = {0*mm, 0*mm, 0*mm, 0*mm, 0*mm, 
    					0*mm, 0*mm, 0*mm, 0*mm};
	G4double rOuter[] = {70.612*mm+eps, 70.612*mm+eps, (44.45+3.81)*mm+eps, (44.45+3.81)*mm+eps, (156.972+3.81)*mm+eps,
						(174.625+3.81)*mm+eps, (174.625+3.81)*mm+eps, (156.972+3.81)*mm+eps, 3.81*mm};
	G4Polycone* interiorCutAway = new G4Polycone("interior", 0, 2*pi, 9, zPlanes, rInner, rOuter);	
	
	G4SubtractionSolid* bulkMinusInterior = new G4SubtractionSolid("insulation", bulk, interiorCutAway,
											0, G4ThreeVector(0, 0, (574.802/2.0-11.43)*mm));
											
	G4Torus* actCharRing = new G4Torus("ring", 0*mm, 49.0682*mm+eps, 96.1736*mm, 0, 2*pi);
    G4double zPlanesC[] = {0*mm, -(520.7-38.1-19.558)*mm, -(520.7-38.1)*mm, -520.7*mm};
    G4double rInnerC[] = {0*mm, 0*mm, 0*mm, 0*mm};
    G4double rOuterC[] = {174.625*mm, 174.625*mm, 156.972*mm, 0*mm};
	G4Polycone* actCharCutAway = new G4Polycone("actCharCutAway", 0, 2*pi, 4, zPlanesC, rInnerC, rOuterC);
	G4SubtractionSolid* actCharCut = new G4SubtractionSolid("actCharCut", actCharRing, actCharCutAway,
											0, G4ThreeVector(0, 0, (520.7-49.0682)*mm-eps));
											
	G4SubtractionSolid* insulation = new G4SubtractionSolid("insulation", bulkMinusInterior, actCharCut,
											0, G4ThreeVector(0, 0, (((574.802-2.0*11.43)/2.0)-(520.7-49.0682))*mm-eps));
  										    
    G4VisAttributes* MylarVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0, 0.3)); // magenta
    MylarVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(insulation, material, logicalName);
    pVol->SetVisAttributes(MylarVisAtt); 
    MGLog(debugging) << "Created STC Dewar Insulation Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Dewar Insulation Logical" << endlog; 
  return pVol;
}  
