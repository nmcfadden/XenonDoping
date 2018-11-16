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
/**                                                            
 * $Id: MJheaderTemplate.hh,v 1.3 2005-02-15 23:26:55 akbarm Exp $
 *      
 * CLASS DECLARATION:  TSCylinderShieldTank.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
/**
 *
 * Big cylinder shield tank that can be filled with eg. LAr, LS or H20
 * 
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ryan Martin
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * 02-14-2011, Created, R. Martin
 * 06-15-2011, Added steel tank, R. Martin
 *
 */
//---------------------------------------------------------------------------//

#include "tonscalegeometry/TSCylinderShieldTank.hh"
#include "tonscalegeometry/TSCylinderShieldTankMessenger.hh"
#include "tonscalegeometry/TSVisAttributes.hh"

#include "io/MGLogger.hh"
#include "G4VisAttributes.hh"

using namespace std;

using namespace CLHEP;

TSCylinderShieldTank::TSCylinderShieldTank()
{ 
  ConstructTankMaterials();
  fTankRadius=5*m;
  fTankHeight=2.*fTankRadius;
  
  fTankContentMaterial = G4Material::GetMaterial("LAB");
  fTankWallMaterial = G4Material::GetMaterial("Steel");
  
  fG4Messenger = new TSCylinderShieldTankMessenger(this);
  
  fTankWallThickness=2*cm;
}
 
// -----------------------------------------------------------------------

TSCylinderShieldTank::~TSCylinderShieldTank()
{
  delete fTankContentMaterial; 
  delete fTankWallMaterial;
  delete fTankContentLogical;
  delete fTankWallLogical;
}

// -----------------------------------------------------------------------

void TSCylinderShieldTank::ConstructTankMaterials()
{
  G4String symbol;  
  G4double a, z, density;     
  G4int ncomponents;
  G4double fractionmass;
  
  G4Element* C = new G4Element("Carbon", symbol="C", z=6., a=12.01*g/mole);
  G4Element* H = new G4Element("Hydrogen",   symbol="H", z=1., a=1.007*g/mole);
  G4Element* O = new G4Element("Oxygen",   symbol="O", z=8., a=16.00*g/mole);
  G4Element* Fe = new G4Element("Iron",   symbol="Fe", z=26., a=55.85*g/mole);
  
  G4Material* LAB = new G4Material("LAB", density= 0.8628*g/cm3, ncomponents=2);
  LAB->AddElement(C, fractionmass=0.88);
  LAB->AddElement(H, fractionmass=0.12);
  
  G4Material* water = new G4Material("Water", density=1*g/cm3, ncomponents=2);
  water->AddElement(O, fractionmass=0.89);
  water->AddElement(H, fractionmass=0.11);
  
  G4Material* steel = new G4Material("Steel", density=7.85*g/cm3, ncomponents=2);
  steel->AddElement(C, fractionmass=0.01);
  steel->AddElement(Fe, fractionmass=0.99);
}

// -----------------------------------------------------------------------

void TSCylinderShieldTank::Construct()
{
  G4Tubs* TankWallTub = new G4Tubs("TSCylinderWallTankTub",0,fTankRadius+fTankWallThickness,fTankHeight/2.+fTankWallThickness,0,2*pi);
  
  fTankWallLogical=new G4LogicalVolume(TankWallTub, fTankWallMaterial, "TSCylinderTankWallLogical");
  
  fTankWallLogical->SetVisAttributes(new TSVisAttributesWireFrame("black"));
  
  G4Tubs* TankContentTub = new G4Tubs("TSCylinderShieldTankTub",0,fTankRadius,fTankHeight/2.,0,2*pi);
  
  fTankContentLogical=new G4LogicalVolume(TankContentTub, fTankContentMaterial, "TSCylinderTankContentLogical");
  
  fTankContentLogical->SetVisAttributes(new TSVisAttributesWireFrame("yellow"));
  
  new G4PVPlacement(0, G4ThreeVector(0,0,0), fTankContentLogical ,"shieldtankphysical", fTankWallLogical, false, 0, false);
  
  MGLog(routine)<<"Volume of lab tank "<<TankContentTub->GetCubicVolume()/(m*m*m)<<"m3"<<endl;
}

// -----------------------------------------------------------------------

