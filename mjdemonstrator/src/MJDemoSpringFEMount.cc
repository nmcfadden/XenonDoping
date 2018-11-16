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
// CLASS IMPLEMENTATION:  MJDemoSpringFEMount.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Midpoint of segment conincident with bottom mating face, 
 * end of part with two bolt holes.
 * Part requires 180 deg rotation about Z axis.
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
#include "G4ExtrudedSolid.hh"
#include <vector>
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoSpringFEMount.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoSpringFEMount::MJDemoSpringFEMount(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-010", "Copper-EF")
{;}

MJDemoSpringFEMount::MJDemoSpringFEMount(const MJDemoSpringFEMount & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoSpringFEMount::~MJDemoSpringFEMount()
{;}

G4LogicalVolume* MJDemoSpringFEMount::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4Box* body = new G4Box("body", 0.825*25.4*mm, 0.431*25.4*mm, 0.0165*25.4*mm);
    G4DisplacedSolid* body2 = new G4DisplacedSolid("body2", body, 0, 
    									G4ThreeVector(0.825*25.4*mm, 0, 0.0165*25.4*mm));

    G4Box* box1 = new G4Box("box1", 0.0375*25.4*mm, 0.431*25.4*mm, 0.02*25.4*mm);
    G4UnionSolid* body3 = new G4UnionSolid("body3", body2, box1, 0,
    									G4ThreeVector(1.1165*25.4*mm, 0, 0));

    G4Box* box2 = new G4Box("box2", 0.025*25.4*mm, 0.431*25.4, 0.02*25.4*mm);
    G4UnionSolid* body4 = new G4UnionSolid("body4", body3, box2, 0, 
    									G4ThreeVector(1.329*25.4*mm, 0, 0));
    									
    G4Box* box3 = new G4Box("box3", 0.05*25.4*mm, 0.431*25.4*mm, 0.0125*25.4*mm);
    G4UnionSolid* body5 = new G4UnionSolid("body5", body4, box3, 0,
    									G4ThreeVector(1.304*25.4*mm, 0, -0.0275*25.4*mm));

    G4Box* box4 = new G4Box("box4", 0.055*25.4*mm, 0.218*25.4*mm, 0.1*25.4*mm);
    G4SubtractionSolid* body6 = new G4SubtractionSolid("body6", body5, box4, 0,
    									G4ThreeVector(0.315*25.4*mm, 0, 0));
    
    G4Box* box5 = new G4Box("box5", 0.15*25.4*mm, 0.1*25.4*mm, 0.1*25.4*mm);
    G4SubtractionSolid* body7 = new G4SubtractionSolid("body7", body6, box5, 0,
    									G4ThreeVector(1.154*25.4*mm, 0, 0));
    									
    G4Box* box6 = new G4Box("box6", 0.075*25.4*mm, 0.137*25.4*mm, 0.1*25.4*mm);
    G4SubtractionSolid* body8 = new G4SubtractionSolid("body8", body7, box6, 0,
    									G4ThreeVector(1.079*25.4*mm, 0, 0));    

    G4Tubs* thruHole = new G4Tubs("thruHole", 0, 0.06*25.4*mm, 0.1*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* body9 = new G4SubtractionSolid("body9", body8, thruHole, 0, 
    										G4ThreeVector(0.13*25.4*mm, 0.150*25.4*mm, 0));
    G4SubtractionSolid* body10 = new G4SubtractionSolid("body10", body9, thruHole, 0, 
    										G4ThreeVector(0.13*25.4*mm, -0.150*25.4*mm, 0));  
    G4SubtractionSolid* body11 = new G4SubtractionSolid("body11", body10, thruHole, 0,
    										G4ThreeVector(1.53*25.4*mm, 0, 0)); 	

    std::vector<G4TwoVector> polygon;
    polygon.push_back(G4TwoVector(-0.010*25.4*mm,   0.260*25.4*mm));
    polygon.push_back(G4TwoVector(-0.010*25.4*mm,   0.440*25.4*mm));
    polygon.push_back(G4TwoVector( 1.512*25.4*mm,   0.431*25.4*mm));
    polygon.push_back(G4TwoVector( 0.370*25.4*mm,   0.260*25.4*mm));
    G4ExtrudedSolid* bodyCut = new G4ExtrudedSolid("bodyCut", polygon, 0.1*25.4*mm, G4TwoVector(0,0),
    										1.0, G4TwoVector(0,0), 1.0);
    G4SubtractionSolid* body12 = new G4SubtractionSolid("body12", body11, bodyCut);
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateX(pi);
    G4SubtractionSolid* body13 = new G4SubtractionSolid("body13", body12, bodyCut, rotation,
    									G4ThreeVector(0, 0, 0));

    G4Tubs* cornerRound = new G4Tubs("cornerRound", 0.12*25.4*mm, 0.3*25.4*mm, 0.1*25.4*mm, 0, 98.443*deg);
    G4SubtractionSolid* body14 = new G4SubtractionSolid("body14", body13, cornerRound, 0,
    										G4ThreeVector(1.53*25.4*mm, 0.311*25.4*mm, -0.0165*25.4*mm));
    G4SubtractionSolid* body15 = new G4SubtractionSolid("body15", body14, cornerRound, rotation,
    										G4ThreeVector(1.53*25.4*mm, -0.311*25.4*mm, -0.0165*25.4*mm));									

    G4Tubs* thruHole2 = new G4Tubs("thruHole2", 0, 0.031*25.4*mm, 0.1*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* body16 = new G4SubtractionSolid("body16", body15, thruHole2, 0, 
    										G4ThreeVector(1.386*25.4*mm, 0.217*25.4*mm, 0));
    
    std::vector<G4TwoVector> polygon2;
    polygon2.push_back(G4TwoVector( 0.575*25.4*mm, 	0.173*25.4*mm));
    polygon2.push_back(G4TwoVector( 0.575*25.4*mm, 	0.183*25.4*mm));
    polygon2.push_back(G4TwoVector( 1.361*25.4*mm, 	0.225*25.4*mm));
    polygon2.push_back(G4TwoVector( 1.361*25.4*mm, -0.225*25.4*mm));
    polygon2.push_back(G4TwoVector( 0.575*25.4*mm, -0.183*25.4*mm));
    polygon2.push_back(G4TwoVector( 0.575*25.4*mm, -0.173*25.4*mm));
    polygon2.push_back(G4TwoVector( 1.351*25.4*mm, -0.215*25.4*mm));
    polygon2.push_back(G4TwoVector( 1.351*25.4*mm, 	0.215*25.4*mm));

    G4ExtrudedSolid* slot = new G4ExtrudedSolid("slot", polygon2, 0.1*25.4*mm, G4TwoVector(0,0),
    										1.0, G4TwoVector(0,0), 1.0);    
    G4SubtractionSolid* body17 = new G4SubtractionSolid("body17", body16, slot);        
    	
    std::vector<G4TwoVector> polygon4;
    polygon4.push_back(G4TwoVector(0.3699*25.4*mm,  0.0758*25.4*mm));
    polygon4.push_back(G4TwoVector(1.079*25.4*mm, 0.1683*25.4*mm));
    polygon4.push_back(G4TwoVector(1.079*25.4*mm, -0.1683*25.4*mm));
    polygon4.push_back(G4TwoVector(0.3699*25.4*mm,  -0.0758*25.4*mm));
        
    G4ExtrudedSolid* pocket = new G4ExtrudedSolid("pocket", polygon4, 0.1*25.4*mm, G4TwoVector(0,0),
    										1.0, G4TwoVector(0,0), 1.0);    

    G4SubtractionSolid* FEMount = new G4SubtractionSolid("FEMount", body17, pocket);

    // G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8,0.0,0.0)); // red
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    // redVisAtt->SetForceWireframe( false );
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(FEMount, material, logicalName);
    // pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Spring FE Mount Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Spring FE Mount Logical" << endlog; 
  return pVol;
}  
