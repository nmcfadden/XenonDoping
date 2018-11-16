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
// CLASS IMPLEMENTATION:  MJDemoSilicaStack.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Solid Work Drawing: silica stack of 5, 10 wire.sldasm  
 * Wenqin: WARNING: this solid work drawing is not current, not the final. Adjustment is needed!
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR:  Wenqin Xu 
 * CONTACT: wxu@lanl.gov
 * FIRST SUBMISSION: Jan 09, 2013 
 * 
 * REVISION:
 * 
 * 01-09-2013, Created, W. Xu
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4Trd.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoSilicaStack.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoSilicaStack::MJDemoSilicaStack(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "SilicaStack Solid Work", "Copper-EF")
{;}

MJDemoSilicaStack::MJDemoSilicaStack(const MJDemoSilicaStack & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoSilicaStack::~MJDemoSilicaStack()
{;}

G4LogicalVolume* MJDemoSilicaStack::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){    
    G4Box * silica_stack_tap = new G4Box("silica_stack_tap",0.2685*25.4*mm,0.25*25.4*mm,0.042*25.4*mm); 
    //Wenqin: The tap is the center, so that the top for the bolt is at the center

    G4Box * silica_stack_base = new G4Box("silica_stack_base", 0.2685*25.4*mm, 0.653*25.4*mm, 0.042*25.4*mm); //x/2, y/2, z/2, X is the narrow side

    G4UnionSolid* silica_stack1= new G4UnionSolid("silica_stack1",silica_stack_tap,silica_stack_base,0,G4ThreeVector(0,(0.6530-0.25)*25.4*mm,0));
    //This is the lowest level

    G4Box * silica_stack_mid = new G4Box("silica_stack_mid", 0.2685*25.4*mm, 0.3405*25.4*mm, 0.186*25.4*mm);//starting from bottom
    G4UnionSolid* silica_stack2= new G4UnionSolid("silica_stack2",silica_stack1,silica_stack_mid,0,G4ThreeVector(0,(0.3405+0.375)*25.4*mm,(0.186-0.042)*25.4*mm)); 
    //Wenqin: From edge to hole center =0.375", 1/2 thickness in y = 0.3405". 
    //Wenqin: This Center to bottom = 0.186", hole center to bottom = 0.042", so center to hole center =  0.186" - 0.042" 
    //This is the middle level

    G4Box * silica_stack_top = new G4Box("silica_stack_top", 0.2685*25.4*mm, 0.4*25.4*mm, 0.1295*25.4*mm);//not starting from bottom; 
    //Wenqin: Bottom to silica_stack_tap top = 0.078", so height = 0.078"+0.1295+0.042=0.2495"  
    //Wenqin: From edge to hole center =0.341", 1/2 thickness in y = 0.4". 
    //This is the top level
    G4Tubs * tophole = new G4Tubs ("tophole", 0, 0.1335*25.4*mm, 0.13*25.4*mm, 0, 2*pi);    
    G4SubtractionSolid* silica_stack_top_a= new G4SubtractionSolid("silica_stack_top_a",silica_stack_top,tophole,0,G4ThreeVector(0.01*25.4*mm,0,0));
    G4UnionSolid* silica_stack3= new G4UnionSolid("silica_stack3",silica_stack2,silica_stack_top_a,0,G4ThreeVector(0,(0.4+0.341)*25.4*mm,0.2495*25.4*mm)); 

    G4Box * silica_stack_back = new G4Box("silica_stack_back", 0.5785*25.4*mm, 0.0595*25.4*mm, 0.186*25.4*mm);//starting from bottom
    G4UnionSolid* silica_stack4= new G4UnionSolid("silica_stack4",silica_stack3,silica_stack_back,0,G4ThreeVector(0,(0.0595+0.937)*25.4*mm,(0.186-0.042)*25.4*mm));
    //Wenqin: From edge to hole center = 0.937", 1/2 thickness in y = 0.0595"
    //Wenqin: Omission, The groove on the back. The two groove on the front of the back plate

    G4Tubs * tappedhole = new G4Tubs ("tappedhole", 0, 0.083*25.4*mm, 1*25.4*mm, 0, 2*pi);	 
    //Wenqin: Hole  diameter = 0.166"
    G4SubtractionSolid* silica_stack5= new G4SubtractionSolid("silica_stack5",silica_stack4,tappedhole,0,G4ThreeVector(0,0,0)); 
    //Wenqin: The hole is the center 

    G4Tubs * edgecut = new G4Tubs ("edgecut",0.125*25.4*mm,0.2*25.4*mm,1*25.4*mm,0,0.5*pi); 
    G4RotationMatrix* cutRotation1 = new G4RotationMatrix();
    cutRotation1->rotateZ(90*deg);
    G4SubtractionSolid* silica_stack6= new G4SubtractionSolid("silica_stack6",silica_stack5,edgecut,cutRotation1,G4ThreeVector(0.144*25.4*mm,-0.125*24.4*mm,0));  

    cutRotation1->rotateZ(90*deg);
    G4SubtractionSolid* silica_stack7= new G4SubtractionSolid("silica_stack7",silica_stack6,edgecut,cutRotation1,G4ThreeVector(-0.144*25.4*mm,-0.125*24.4*mm,0));  

    G4Box * silica_stack_hollow = new G4Box("silica_stack_hollow",0.2685*25.4*mm,0.1965*25.4*mm,0.1235*25.4*mm);
    G4SubtractionSolid* silica_stack8= new G4SubtractionSolid("silica_stack8",silica_stack7,silica_stack_hollow,0,G4ThreeVector(0,(0.514+0.1965)*24.4*mm,(0.1235+0.041+0.042)*24.4*mm));  

   
    //Wenqin: All dimensions Checked. Note, the bottom is at -0.042"z. (Facing the narrow side , with the tap pointing to me, with the hole center at 0, 0, 0)

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(silica_stack8, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created HV Twist Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing HV Twist Logical" << endlog; 
  return pVol;
}  
