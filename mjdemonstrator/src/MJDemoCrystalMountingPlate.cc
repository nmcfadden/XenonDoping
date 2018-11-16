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
// CLASS IMPLEMENTATION:  MJDemoCrystalMountingPlate.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Part Origin:  COM of part envelope
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 * 
 * 07-22-2010, Created, M. Green
 * 01-13-2012, Changed color attribute from red to copper, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4ExtrudedSolid.hh"
#include "G4Polyhedra.hh"
#include "G4Polycone.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCrystalMountingPlate.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoCrystalMountingPlate::MJDemoCrystalMountingPlate(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-008", "Copper-EF")
{;}

MJDemoCrystalMountingPlate::MJDemoCrystalMountingPlate(const MJDemoCrystalMountingPlate & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCrystalMountingPlate::~MJDemoCrystalMountingPlate()
{;}

G4LogicalVolume* MJDemoCrystalMountingPlate::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* body = new G4Tubs("body", 0, 0.779*25.4*mm, 0.125*25.4*mm, 0, 2*pi);

    G4Tubs* cylinderCut = new G4Tubs("cylinderCut", 0, 1.0*25.4*mm, 0.15*25.4*mm, 0, 2*pi);
    
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, cylinderCut, 0, 
                                 G4ThreeVector(-1.5588*25.4*mm, 0, 0));
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, cylinderCut, 0, 
                                 G4ThreeVector(0.7794*25.4*mm, 1.35*25.4*mm, 0));
    G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, cylinderCut, 0, 
                                 G4ThreeVector(0.7794*25.4*mm, -1.35*25.4*mm, 0));  

    G4Box* tabs = new G4Box("tabs", 1*25.4*mm, 0.35*25.4*mm, 0.125*25.4*mm);
    G4UnionSolid* body5 = new G4UnionSolid("body5", body4, tabs, 0,
                                     G4ThreeVector(1.02*25.4*mm, 0, 0));
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateZ(120*deg);
    G4UnionSolid* body6 = new G4UnionSolid("body6", body5, tabs, rotation,
                  G4ThreeVector(1.02*25.4*mm*cos(120*deg), -1.02*25.4*mm*sin(120*deg), 0));
    rotation->rotateZ(120*deg);
    G4UnionSolid* body7 = new G4UnionSolid("body7", body6, tabs, rotation,
                  G4ThreeVector(1.02*25.4*mm*cos(120*deg), 1.02*25.4*mm*sin(120*deg), 0));

  
    G4double hexRadius = 1.84*25.4*mm;
    G4double zPlanes[] = {-0.15*25.4*mm, 0.15*25.4*mm};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {0.126*25.4*mm, 0.126*25.4*mm};//0.125
    G4Polyhedra* hex = new G4Polyhedra("hex", pi/2, 2*pi, 6, 2, zPlanes, rInner, rOuter);
    G4SubtractionSolid* body8 = new G4SubtractionSolid("body8", body7, hex, 0,
                                                 G4ThreeVector(hexRadius, 0*25.4*mm, 0));
    G4SubtractionSolid* body9 = new G4SubtractionSolid("body9", body8, hex, 0,
                  G4ThreeVector(hexRadius*cos(120*deg), hexRadius*sin(120*deg), 0));                                                
    G4SubtractionSolid* body10 = new G4SubtractionSolid("body10", body9, hex, 0,
                  G4ThreeVector(hexRadius*cos(240*deg), hexRadius*sin(240*deg), 0));    
                  
                  
    G4Tubs* centerPocket = new G4Tubs("centerPocket", 1.06*25.4*mm, 1.64*25.4*mm, 0.2*25.4*mm, 
    								-30*deg, 60*deg);
    G4RotationMatrix* rotation2 = new G4RotationMatrix();
    G4SubtractionSolid* body11 = new G4SubtractionSolid("body11", body10, centerPocket, 0, 
                                       G4ThreeVector(-1.559*25.4*mm, 0, 0.125*25.4*mm));
    rotation2->rotateZ(120*deg);
    G4SubtractionSolid* body12 = new G4SubtractionSolid("body12", body11, centerPocket, rotation2, 
                                       G4ThreeVector(0.779*25.4*mm, 1.35*25.4*mm, 0.125*25.4*mm));
    rotation2->rotateZ(120*deg);
    G4SubtractionSolid* body13 = new G4SubtractionSolid("body13", body12, centerPocket, rotation2, 
                                       G4ThreeVector(0.779*25.4*mm, -1.35*25.4*mm, 0.125*25.4*mm));
    
    G4double cylinderRad = 1.394*25.4*mm;
    G4Tubs* cylinderCut2 = new G4Tubs("cylinderCut2", 0, 0.29*25.4*mm, 0.2*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* body14 = new G4SubtractionSolid("body14", body13, cylinderCut2, 0, 
                        G4ThreeVector(cylinderRad, 0, 0.245*25.4*mm));
    G4SubtractionSolid* body15 = new G4SubtractionSolid("body15", body14, cylinderCut2, 0, 
          G4ThreeVector(cylinderRad*cos(120*deg), cylinderRad*sin(120*deg), 0.125*25.4*mm));
    G4SubtractionSolid* body16 = new G4SubtractionSolid("body16", body15, cylinderCut2, 0, 
          G4ThreeVector(cylinderRad*cos(240*deg), cylinderRad*sin(240*deg), 0.125*25.4*mm));
   
    G4Box* boxCut = new G4Box("boxCut", 0.5*25.4*mm, 0.29*25.4*mm, 0.2*25.4*mm);
    G4double boxCenters = cylinderRad-0.5*25.4*mm;
    G4SubtractionSolid* body17 = new G4SubtractionSolid("body17", body16, boxCut, 0, 
                        G4ThreeVector(boxCenters, 0, 0.245*25.4*mm));
    G4SubtractionSolid* body17a = new G4SubtractionSolid("body17a", body17, boxCut, 0, 
                        G4ThreeVector((0.95-0.5)*25.4*mm, 0, 0.125*25.4*mm));
    G4RotationMatrix* rotation3 = new G4RotationMatrix();
    rotation3->rotateZ(240*deg);
    G4SubtractionSolid* body18 = new G4SubtractionSolid("body18", body17a, boxCut, rotation3, 
          G4ThreeVector(boxCenters*cos(120*deg), boxCenters*sin(120*deg), 0.125*25.4*mm));
    rotation3->rotateZ(-120*deg);
    G4SubtractionSolid* body19 = new G4SubtractionSolid("body19", body18, boxCut, rotation3, 
          G4ThreeVector(boxCenters*cos(240*deg), boxCenters*sin(240*deg), 0.125*25.4*mm));
     
    G4double holeRadius = hexRadius - 0.35*25.4*mm;
    G4double zPlanes2[] = {0.2*25.4*mm, -0.095*25.4*mm, -0.095*25.4*mm,-0.20*25.4*mm};
    G4double rInner2[] = {0, 0, 0, 0};
    G4double rOuter2[] = {0.140*25.4*mm, 0.140*25.4*mm, 0.110*25.4*mm, 0.110*25.4*mm};
    G4Polycone* thruHole = new G4Polycone("thruHole", pi/2, 2*pi, 4, zPlanes2, rInner2, rOuter2);
    G4SubtractionSolid* body20 = new G4SubtractionSolid("body20", body19, thruHole, 0,
                  G4ThreeVector(holeRadius, 0*25.4*mm, 0));
    G4SubtractionSolid* body21 = new G4SubtractionSolid("body21", body20, thruHole, 0,
                  G4ThreeVector(holeRadius*cos(120*deg), holeRadius*sin(120*deg), 0));                                                
    G4SubtractionSolid* body22 = new G4SubtractionSolid("body22", body21, thruHole, 0,
                  G4ThreeVector(holeRadius*cos(240*deg), holeRadius*sin(240*deg), 0));
    G4SubtractionSolid* body23 = new G4SubtractionSolid("body23", body22, thruHole, 0,
                  G4ThreeVector(0, 0, 0));

    G4Tubs* threadedStud = new G4Tubs("threadedStud", 0, 0.0483*25.4*mm, 0.185*25.4*mm, 0, 2*pi);
    G4UnionSolid* body24a = new G4UnionSolid("body24a", body23, threadedStud, 0, 
    				G4ThreeVector(-0.3466*25.4*mm, 0, 0.06*25.4*mm));
    G4UnionSolid* body24b = new G4UnionSolid("body24b", body24a, threadedStud, 0, 
    				G4ThreeVector(1.0534*25.4*mm, 0.15*25.4*mm, 0.06*25.4*mm));
    G4UnionSolid* body24 = new G4UnionSolid("body24", body24b, threadedStud, 0, 
    				G4ThreeVector(1.0534*25.4*mm, -0.15*25.4*mm, 0.06*25.4*mm));
    
    G4Tubs* cornerTrim = new G4Tubs("cornerTrim", 0.25*25.4*mm, 1.5*25.4*mm, 0.2*25.4*mm, 
    									197*deg, 326*deg);
    G4SubtractionSolid* body25 = new G4SubtractionSolid("body25", body24, cornerTrim, 0,
                  	G4ThreeVector(2.6349*25.4*mm, 0, 0));
    G4RotationMatrix* rotation4 = new G4RotationMatrix();
    rotation4->rotateZ(240*deg);
    G4SubtractionSolid* body26 = new G4SubtractionSolid("body26", body25, cornerTrim, rotation4,
                  	G4ThreeVector(2.6349*25.4*mm*cos(120*deg), 2.6349*25.4*mm*sin(120*deg), 0));
    rotation4->rotateZ(-120*deg);
    G4SubtractionSolid* body27 = new G4SubtractionSolid("body27", body26, cornerTrim, rotation4,
                 	G4ThreeVector(2.6349*25.4*mm*cos(240*deg), 2.6349*25.4*mm*sin(240*deg), 0));
 

    				
    G4Box* shimAccess = new G4Box("shim access", 0.1375*25.4*mm, 0.5*25.4*mm, 0.2*25.4*mm);
    G4SubtractionSolid* mountingPlate = new G4SubtractionSolid("mounting plate", body27, shimAccess, 0,
    				G4ThreeVector(0.4275*25.4*mm, 0, 0.155*25.4*mm));
    				
    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8,0.0,0.0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(mountingPlate, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Crystal Mounting Plate Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Crystal Mounting Plate Logical" << endlog; 
  return pVol;
}  
