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
// CLASS IMPLEMENTATION:  MJGeometryAcrylicSource.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ryan Martin
 * CONTACT: rdmartin@lbl.gov
 * FIRST SUBMISSION: Feb 26, 2013
 * 
 * REVISION:
 * 
 * 02-26-2013, Created, R. Martin
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"                                                                                                                                               
#include "G4RotationMatrix.hh"
#include "G4UIcommand.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4Polycone.hh"
#include "G4Sphere.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryAcrylicSource.hh"


//---------------------------------------------------------------------------//
#include <vector>

using namespace CLHEP;




MJGeometryAcrylicSource::MJGeometryAcrylicSource(G4String argName)
{
  fName=argName;
  fSourceDiameter=25.4*mm;//check this
  fSourceLength=5.0*mm;//check this
  fActivityDiameter=5.0*mm;//check this
  fActivityLength=0.5*mm;//check this
  ConstructMaterials();
  
}

void MJGeometryAcrylicSource::ConstructMaterials()
{
  G4String symbol;  
  G4double a, z, density;     
  G4int ncomponents,natoms;
  
  
  G4Element* C = new G4Element("Carbon", symbol="C", z=6., a=12.01*g/mole);
  G4Element* H = new G4Element("Hydrogen",   symbol="H", z=1., a=1.007*g/mole);
  G4Element* O = new G4Element("Oxygen",   symbol="O", z=8., a=16.00*g/mole);
  
  G4Material* Acrylic = new G4Material("Acrylic", density=1.18*g/cm3, ncomponents=3);//From wikipedia...
  Acrylic->AddElement(C, natoms=5);
  Acrylic->AddElement(H, natoms=8);
  Acrylic->AddElement(O, natoms=2);
  
}



MJGeometryAcrylicSource::~MJGeometryAcrylicSource()
{
  
}


void MJGeometryAcrylicSource::Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,G4LogicalVolume *motherlogical)
{
  //Origin at the bottom and center of the source	disk
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  
  //Place the acrylic and the activity as two different volumes
  G4String name="AcrylicSourceWithHole";
  G4Tubs* AcrylicNoHoleSolid = new G4Tubs("AcrylicNoHoleSolid",0.0,0.5*fSourceDiameter,0.5*fSourceLength,0,2*pi);
  G4Tubs* AcrylicSourceActivityHole = new G4Tubs("AcrylicSourceActivityHole",0.0,0.5*fActivityDiameter+0.00001*mm,0.5*fActivityLength+0.00001*mm,0,2*pi);
  G4SubtractionSolid* AcrylicWithHoleSolid= new G4SubtractionSolid(name+"_Solid",AcrylicNoHoleSolid,AcrylicSourceActivityHole,0,G4ThreeVector(0,0,0.5*(fActivityLength-fSourceLength)));
  
  G4LogicalVolume *AcrylicWithHoleLogical = new G4LogicalVolume(AcrylicWithHoleSolid,G4Material::GetMaterial("Acrylic"),name+"_Logical");    
  AcrylicWithHoleLogical->SetVisAttributes(new G4VisAttributes(G4Colour(1,0,0))); 
  G4ThreeVector* AcrylicWithHoleLocalPosition = new G4ThreeVector(0.,0.,0.5*fSourceLength);
  G4RotationMatrix* AcrylicWithHoleLocalRotation = new G4RotationMatrix();
  G4AffineTransform* AcrylicWithHoleAffine = new G4AffineTransform(AcrylicWithHoleLocalRotation, *AcrylicWithHoleLocalPosition);
  *AcrylicWithHoleAffine *= *assemAffine;  			
  G4ThreeVector* AcrylicWithHoleGlobalPosition=new G4ThreeVector(AcrylicWithHoleAffine->NetTranslation());
  G4RotationMatrix* AcrylicWithHoleGlobalRotation= new G4RotationMatrix(AcrylicWithHoleAffine->NetRotation());				
  new G4PVPlacement(AcrylicWithHoleGlobalRotation, *AcrylicWithHoleGlobalPosition,AcrylicWithHoleLogical,name+"_Physical",motherlogical,false,0,false);
  
  
  name="AcrylicSourceActivity";
  G4Tubs* AcrylicSourceActivitySolid = new G4Tubs(name+"_Solid",0.0,0.5*fActivityDiameter,0.5*fActivityLength,0,2*pi);
  G4LogicalVolume *AcrylicSourceActivityLogical = new G4LogicalVolume(AcrylicSourceActivitySolid,G4Material::GetMaterial("Acrylic"),name+"_Logical");    
  AcrylicSourceActivityLogical->SetVisAttributes(new G4VisAttributes(G4Colour(1,1,0))); 
  G4ThreeVector* AcrylicSourceActivityLocalPosition = new G4ThreeVector(0.,0.,0.5*fActivityLength);
  G4RotationMatrix* AcrylicSourceActivityLocalRotation = new G4RotationMatrix();
  G4AffineTransform* AcrylicSourceActivityAffine = new G4AffineTransform(AcrylicSourceActivityLocalRotation, *AcrylicSourceActivityLocalPosition);
  *AcrylicSourceActivityAffine *= *assemAffine;  			
  G4ThreeVector* AcrylicSourceActivityGlobalPosition=new G4ThreeVector(AcrylicSourceActivityAffine->NetTranslation());
  G4RotationMatrix* AcrylicSourceActivityGlobalRotation= new G4RotationMatrix(AcrylicSourceActivityAffine->NetRotation());				
  new G4PVPlacement(AcrylicSourceActivityGlobalRotation, *AcrylicSourceActivityGlobalPosition,AcrylicSourceActivityLogical,name+"_Physical",motherlogical,false,0,false);
  
}




