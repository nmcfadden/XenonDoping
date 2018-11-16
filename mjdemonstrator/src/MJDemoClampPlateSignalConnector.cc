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
// CLASS IMPLEMENTATION:  MJDemoClampPlateSignalConnector.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Box Center
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Max Hays
 * CONTACT: maxhays8@gmail.com
 * FIRST SUBMISSION: June 11, 2014
 * 
 * REVISION:
 * 
 * 11-06-2010, Created, M. Hays
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh" 
#include "G4LogicalVolumeStore.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4GenericTrap.hh"
#include "G4UnionSolid.hh"
#include "G4Trd.hh"
#include "G4Cons.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoClampPlateSignalConnector.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoClampPlateSignalConnector::MJDemoClampPlateSignalConnector(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-233", "Copper-EF")
{;}

MJDemoClampPlateSignalConnector::MJDemoClampPlateSignalConnector(const MJDemoClampPlateSignalConnector & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoClampPlateSignalConnector::~MJDemoClampPlateSignalConnector()
{;}

G4LogicalVolume* MJDemoClampPlateSignalConnector::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double epsilon = 0.01*mm;

    G4Box* mainRecPrism = new G4Box("Main Rec Prism",  38.1/2*mm, 10.16/2*mm, 295.275/2*mm);
    G4Box* subPrism1 = new G4Box("Sub Prism 1",  12.7/2*mm, 5.398/2*mm+epsilon, 295.275/2*mm+epsilon);
    G4Box* subPrism2 = new G4Box("Sub Prism 2",  6.35/2*mm, 10.16/2*mm+epsilon, 5.588/2*mm+epsilon);
    G4Box* subPrism3 = new G4Box("Sub Prism 3",  12.7/2*mm, 10.16/2*mm+epsilon, 2.413/2*mm+epsilon);
    G4Box* subPrism4 = new G4Box("Sub Prism 4",  6.35/2*mm, 10.16/2*mm+epsilon, 12.7/2*mm+epsilon);
    G4Box* subPrism5 = new G4Box("Sub Prism 5",  12.7/2*mm, 10.16/2*mm+epsilon, 9.525/2*mm+epsilon);
    G4Box* subPrism6 = new G4Box("Sub Prism 6",  50/2*mm, 1.905/2*mm+epsilon, 3.175/2*mm+epsilon);
    G4Box* subPrism7 = new G4Box("Sub Prism 7",  9.525/2*mm+epsilon, 10.16/2*mm+epsilon, 6.35/2*mm);
    G4Box* addPrism1 = new G4Box("Add Prism 1", 12.7/2*mm, 7.62/2*mm, 23.8/2*mm);
    G4Box* addPrism2 = new G4Box("Add Prism 2", 12.7/2*mm, 7.62/2*mm, 54.712/2*mm);
    G4Tubs* topSubCyl = new G4Tubs("Top Sub Cyl", 0, 19.05/2*mm, 2.54*mm, 0, 2*M_PI);
    G4Tubs* sideSubCyl = new G4Tubs("Side Sub Cyl", 0, 6.35/2*mm, 10*mm, 0, 2*M_PI);
    G4Tubs* screwSubCyl1 = new G4Tubs("Side Sub Cyl", 0, 8.433/2*mm, 10*mm, 0, 2*M_PI);
    G4Tubs* screwSubCyl2 = new G4Tubs("Side Sub Cyl", 0, 3.64/2*mm, 10*mm, 0, 2*M_PI); //3.454
    G4Tubs* screwSubCyl3 = new G4Tubs("Side Sub Cyl", 0, 3.64/2*mm, 10.312/2*mm, 0, 2*M_PI); //3.454
    G4Tubs* biteCyl = new G4Tubs("Bite Cyl", 0, 7.0/2*mm, 50/2*mm, 0, 2*M_PI); //6.35
//    G4Box* bitePrism = new G4Box("Bite Prism", 15/2*mm, 3/2*mm, 8/2*mm); //6.35

    G4RotationMatrix* rotX = new G4RotationMatrix;  
    G4RotationMatrix* rotY = new G4RotationMatrix;     
    G4RotationMatrix* rotN = new G4RotationMatrix;
    rotX->rotateX(M_PI/2.*rad);
    rotY->rotateY(M_PI/2.*rad);
    G4SubtractionSolid* subSolid;
    G4UnionSolid* unionSolid;
    G4int idx;
    G4double offset;

    unionSolid = new G4UnionSolid("Union Solid", mainRecPrism, addPrism1, rotN, G4ThreeVector((38.1/2+12.7/2)*mm, -(10.16/2-7.62/2)*mm, (295.275/2-162.789-23.8/2)));
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, addPrism1, rotN, G4ThreeVector(-(38.1/2+12.7/2)*mm, -(10.16/2-7.62/2)*mm, (295.275/2-162.789-23.8/2)));
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, addPrism2, rotN, G4ThreeVector((38.1/2+12.7/2)*mm, -(10.16/2-7.62/2)*mm, (295.275/2-231.038-54.712/2)));
    unionSolid = new G4UnionSolid("Union Solid", unionSolid, addPrism2, rotN, G4ThreeVector(-(38.1/2+12.7/2)*mm, -(10.16/2-7.62/2)*mm, (295.275/2-231.038-54.712/2)));
    subSolid = new G4SubtractionSolid("Sub Solid", unionSolid, subPrism1, rotN, G4ThreeVector(0, (10.16/2-5.398/2)*mm, 0));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism2, rotN, G4ThreeVector(0, 0, (295.275/2-5.588/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism3, rotN, G4ThreeVector(0, 0, (295.275/2-2.413/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism4, rotN, G4ThreeVector(0, 0, -(295.275/2-12.7/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism5, rotN, G4ThreeVector(0, 0, -(295.275/2-9.525/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism6, rotN, G4ThreeVector(0, (10.16/2-1.905/2), -(295.275/2-3.81-3.175/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism7, rotN, G4ThreeVector((38.1/2+6.35/2+9.525/2)*mm, 0, (295.275/2-165.964+6.35/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism7, rotN, G4ThreeVector(-(38.1/2+6.35/2+9.525/2)*mm, 0, (295.275/2-165.964+6.35/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism7, rotN, G4ThreeVector((38.1/2+6.35/2+9.525/2)*mm, 0, (295.275/2-183.413-6.35/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism7, rotN, G4ThreeVector(-(38.1/2+6.35/2+9.525/2)*mm, 0, (295.275/2-183.413-6.35/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism7, rotN, G4ThreeVector((38.1/2+6.35/2+9.525/2)*mm, 0, (295.275/2-234.213+6.35/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism7, rotN, G4ThreeVector(-(38.1/2+6.35/2+9.525/2)*mm, 0, (295.275/2-234.213+6.35/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism7, rotN, G4ThreeVector((38.1/2+6.35/2+9.525/2)*mm, 0, (295.275/2-282.575-6.35/2)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, subPrism7, rotN, G4ThreeVector(-(38.1/2+6.35/2+9.525/2)*mm, 0, (295.275/2-282.575-6.35/2)*mm));

    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, topSubCyl, rotX, G4ThreeVector((38.1/2+6.35)*mm, 10.16/2*mm, (295.275/2-176.276)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, topSubCyl, rotX, G4ThreeVector(-(38.1/2+6.35)*mm, 10.16/2*mm, (295.275/2-176.276)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, topSubCyl, rotX, G4ThreeVector((38.1/2+6.35)*mm, 10.16/2*mm, (295.275/2-246.126)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, topSubCyl, rotX, G4ThreeVector(-(38.1/2+6.35)*mm, 10.16/2*mm, (295.275/2-246.126)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, topSubCyl, rotX, G4ThreeVector((38.1/2+6.35)*mm, 10.16/2*mm, (295.275/2-271.526)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, topSubCyl, rotX, G4ThreeVector(-(38.1/2+6.35)*mm, 10.16/2*mm, (295.275/2-271.526)*mm));

    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(3.175*mm, 0, (295.275/2-2.413)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-3.175*mm, 0, (295.275/2-2.413)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(3.175*mm, 0, -(295.275/2-9.525)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-3.175*mm, 0, -(295.275/2-9.525)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector((38.1/2+6.35/2)*mm, 0, (295.275/2-162.789)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(38.1/2+6.35/2)*mm, 0, (295.275/2-162.789)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector((38.1/2+6.35/2)*mm, 0, (295.275/2-186.588)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(38.1/2+6.35/2)*mm, 0, (295.275/2-186.588)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector((38.1/2+6.35/2)*mm, 0, (295.275/2-231.038)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(38.1/2+6.35/2)*mm, 0, (295.275/2-231.038)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector((38.1/2+6.35/2)*mm, 0, (295.275/2-285.75)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, sideSubCyl, rotX, G4ThreeVector(-(38.1/2+6.35/2)*mm, 0, (295.275/2-285.75)*mm));

    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(-15.875*mm, 0, (295.275/2-7.188)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(-15.875*mm, 0, (295.275/2-62.763)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(-15.875*mm, 0, (295.275/2-118.339)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(-15.875*mm, 0, (295.275/2-188.189)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(-15.875*mm, 0, (295.275/2-259.461)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(-9.522*mm, 0, (295.275/2-57.988)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(-9.522*mm, 0, (295.275/2-113.563)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(-9.522*mm, 0, (295.275/2-169.139)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(-9.522*mm, 0, (295.275/2-238.989)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(-12.87*mm, 0, (295.275/2-247.701)*mm));    
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(-12.87*mm, 0, (295.275/2-271.196)*mm));    
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(15.875*mm, 0, (295.275/2-53.213)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(15.875*mm, 0, (295.275/2-108.788)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(15.875*mm, 0, (295.275/2-164.363)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(15.875*mm, 0, (295.275/2-234.213)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(15.875*mm, 0, (295.275/2-259.461)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(9.522*mm, 0, (295.275/2-2.413)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(9.522*mm, 0, (295.275/2-57.988)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(9.522*mm, 0, (295.275/2-113.563)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(9.522*mm, 0, (295.275/2-183.413)*mm));
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(12.87*mm, 0, (295.275/2-247.701)*mm));    
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(12.87*mm, 0, (295.275/2-271.196)*mm)); 
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl2, rotX, G4ThreeVector(0, 0, (295.275/2-11.938)*mm)); 

    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl3, rotY, G4ThreeVector((38.1/2+12.7-10.312/2)*mm, 0, (295.275/2-279.4)*mm)); 
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl3, rotY, G4ThreeVector(-(38.1/2+12.7-10.312/2)*mm, 0, (295.275/2-279.4)*mm)); 

    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl1, rotX, G4ThreeVector((38.1/2+6.35)*mm, 0, (295.275/2-176.276)*mm));    
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl1, rotX, G4ThreeVector(-(38.1/2+6.35)*mm, 0, (295.275/2-176.276)*mm)); 
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl1, rotX, G4ThreeVector((38.1/2+6.35)*mm, 0, (295.275/2-246.126)*mm));    
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl1, rotX, G4ThreeVector(-(38.1/2+6.35)*mm, 0, (295.275/2-246.126)*mm)); 
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl1, rotX, G4ThreeVector((38.1/2+6.35)*mm, 0, (295.275/2-271.526)*mm));    
    subSolid = new G4SubtractionSolid("Sub Solid", subSolid, screwSubCyl1, rotX, G4ThreeVector(-(38.1/2+6.35)*mm, 0, (295.275/2-271.526)*mm)); 
    
    for(idx = 0; idx<20; idx++){
        if(idx<5){offset = 0;} if(idx<10 && idx>4){offset = 17.463-9.525;} if(idx<15 && idx>9){offset = 2*(17.463-9.525);} if(idx<20 && idx>14){offset = 2*(17.463-9.525)+31.75-9.525;}
        //subSolid = new G4SubtractionSolid("Sub Solid", subSolid, bitePrism, rotN, G4ThreeVector((38.1/2-13/2)*mm, (10.16/2-1.587+3/2)*mm, (295.275/2-8.788-offset-9.525*idx)*mm));
        //subSolid = new G4SubtractionSolid("Sub Solid", subSolid, bitePrism, rotN, G4ThreeVector((38.1/2-13/2)*mm, (10.16/2-1.587+3/2)*mm, (295.275/2-13.564-offset-9.525*idx)*mm));
        subSolid = new G4SubtractionSolid("Sub Solid", subSolid, biteCyl, rotY, G4ThreeVector(27.55*mm, (10.16/2-1.587)*mm, (295.275/2-8.788-offset-9.525*idx)*mm));
        subSolid = new G4SubtractionSolid("Sub Solid", subSolid, biteCyl, rotY, G4ThreeVector(-27.55*mm, (10.16/2-1.587)*mm, (295.275/2-13.564-offset-9.525*idx)*mm));
    }

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(subSolid, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created Clamp Plate Signal Connector Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Clamp Plate Signal Connector Logical" << endlog; 
  return pVol;
}  
