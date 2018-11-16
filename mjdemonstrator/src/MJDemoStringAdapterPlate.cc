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
// CLASS IMPLEMENTATION:  MJDemoStringAdapterPlate.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *String Adapter Plate compatible with 77mm BeGe Assemblies
 *Part Origin: Coaxial with string, 0.175" below top surface.
 *
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
 *
 * Rev 2: 
 * 01-09-2013, Updated to match current prototype design, according to Solid Work Coldplate w_Connectors SubAssy.SLDAM, 
 * W. Xu, wxu@lanl.gov
 *
 * 11-14-2013, Updated hex holes according to MJ80-02-109 rev5. J. MacMullin
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
#include "mjdemonstrator/MJDemoStringAdapterPlate.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoStringAdapterPlate::MJDemoStringAdapterPlate(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-109", "Copper-EF")
{;}

MJDemoStringAdapterPlate::MJDemoStringAdapterPlate(const MJDemoStringAdapterPlate & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoStringAdapterPlate::~MJDemoStringAdapterPlate()
{;}
//Wenqin: see also MJ80-02-109 R3

G4LogicalVolume* MJDemoStringAdapterPlate::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* body = new G4Tubs("body", 0.425*25.4*mm,  1.84*25.4*mm, 0.175*25.4*mm, 0, 2*pi);//Original
    //Wenqin: Dimension Checked with MJ80-02-109 R3. inner D = 0.85", outer D=3.68", whole height =0.35"    
    //Wenqin: This doesn't include the taps, which has a straight edge at the beginning 	

    G4Tubs* cylinderCut = new G4Tubs("cylinderCut", 0, 2*25.4*mm, 0.2*25.4*mm, 0, 2*pi);    
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, cylinderCut, 0, 
                     G4ThreeVector(-2.60*25.4*mm, 0, 0));
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, cylinderCut, 0, 
                     G4ThreeVector(-2.60*25.4*mm*cos(120*deg), 2.60*25.4*mm*sin(120*deg), 0));
    G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, cylinderCut, 0, 
                     G4ThreeVector(-2.60*25.4*mm*cos(240*deg), 2.60*25.4*mm*sin(240*deg), 0));  
    //Wenqin: Dimension Checked with MJ80-02-109 R3. these cuts are in a mercedes logo origintation.

    G4Tubs* tabRound = new G4Tubs("tabRound", 0, 0.25*25.4*mm, 0.175*25.4*mm, 0, 2*pi);
    //Wenqin: Dimension Checked,D=0.5"
  
    //G4double tabRadius = 1.783*25.4*mm; //Original
    G4double tabRadius = 3.68/2.*25.4*mm; 
    //Wenqin: Dimension Checked with MJ80-02-109 R3. center to center, this takes care of the entire distance issue

    G4UnionSolid* body5 = new G4UnionSolid("body5", body4, tabRound, 0,
    				G4ThreeVector(tabRadius, 0, 0));
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateZ(120*deg);
    G4UnionSolid* body6 = new G4UnionSolid("body6", body5, tabRound, rotation,
                  	G4ThreeVector(tabRadius*cos(240*deg), tabRadius*sin(240*deg), 0));
    rotation->rotateZ(120*deg);
    G4UnionSolid* body7 = new G4UnionSolid("body7", body6, tabRound, rotation,
                  	G4ThreeVector(tabRadius*cos(120*deg), tabRadius*sin(120*deg), 0));

    G4Box* tapCut= new G4Box("tapcut",1*25.4*mm,1*25.4*mm,1*25.4*mm); 
    //Wenqin: This is going to cut at both sides of the tabs

    G4Box* tapCuthollow= new G4Box("tapcuthollow",1*25.4*mm,0.25*25.4*mm,1*25.4*mm); 
    //Wenqin: The tap itself is 0.5" wide, should not be cut out

    G4SubtractionSolid* tapCut2sides=new G4SubtractionSolid("tapCut2sides",tapCut,tapCuthollow,0, G4ThreeVector(0, 0, 0));
    //Wenqin: This cuts around the tap
  
    G4double tabCutRadius = (1+1.323)*25.4*mm; 
    //Wenqin: from the center to the tap is 1.323. Dimension Checked. The box cut itself is 1"

    G4SubtractionSolid* body7a = new G4SubtractionSolid("body7a", body7, tapCut2sides, 0,
                                G4ThreeVector(tabCutRadius, 0, 0 ));

    G4RotationMatrix* rotationcut = new G4RotationMatrix();
    rotationcut->rotateZ(120*deg);

    G4SubtractionSolid* body7b = new G4SubtractionSolid("body7b", body7a, tapCut2sides, rotationcut,
                                G4ThreeVector(tabCutRadius*cos(240*deg), tabCutRadius*sin(240*deg), 0 ));

    rotationcut->rotateZ(120*deg);
    G4SubtractionSolid* body7c = new G4SubtractionSolid("body7c", body7b, tapCut2sides, rotationcut,
                                G4ThreeVector(tabCutRadius*cos(120*deg), tabCutRadius*sin(120*deg), 0 ));
    //Wenqin: The tap edge is straight lines now


    //Wenqin: The taps and the main body are not on the same surface. The height 0.35" is the total. A hollow cylinder cut is applied to the 3 taps at the same time, the topCylinderCut, and a cylinder cut is applied to the main body, the bottomCylinderCut
    G4Tubs* topCylinderCut = new G4Tubs("topCylinderCut", 1.45*25.4*mm, 3*25.4*mm, 0.2*25.4*mm,
    									0, 2*pi);
    //Wenqin: The top cylinder radius = center to tap inner edge. Dimension Checked. D=2.9"
    G4SubtractionSolid* body8 = new G4SubtractionSolid("body8", body7c, topCylinderCut, 0,
    				G4ThreeVector(0, 0, 0.275*25.4*mm));
    //Wenqin: the cut should be 0.35"-0.25"=0.1" deep. Thus distance = side1+ side2-cut depth=0.35/2"+0.2"-0.1=0.275". Dimension Checked
    //Wenqin: Omission: The smooth, round edge of the cut is omitted

    //G4Tubs* bottomCylinderCut = new G4Tubs("bottomCylinderCut", 0, 1.625*25.4*mm, //Original 
    G4Tubs* bottomCylinderCut = new G4Tubs("bottomCylinderCut", 0, 1.67*25.4*mm, 
    									0.2*25.4*mm, 0, 2*pi);
    G4SubtractionSolid* body9 = new G4SubtractionSolid("body9", body8, bottomCylinderCut, 0,
    				G4ThreeVector(0, 0, -0.345*25.4*mm));
    //Wenqin: The smooth, round edge of the cut has two radius 3.31" and 3.37", thus 3.34" is used.
    //Wenqin: The cut is 0.35"-0.32"=0.03" deep. Thus 0.35"/2+0.2"-0.03=0.345" Dimension Checked


    G4double tapRadius = 0.75*25.4*mm;
    //Wenqin: Distance Dimension Checked. These holes are on the main body, close to the central hole, distance Dimension Checked; 
    //Wenqin: These hole are hex shape at the top, and round at the bottom (D=0.422", 0.14" deep)
 
    G4double zPlanes2[] = {0*mm, 0.22*25.4*mm};
    G4double rInner2[] = {0*mm, 0*mm};
    G4double rOuter2[] = {0.127*25.4*mm, 0.127*25.4*mm}; 
    G4Polyhedra* tappedhexHole = new G4Polyhedra("tappedhexHole", 0, 2*pi, 6, 2, zPlanes2, rInner2, rOuter2);

    G4Tubs* tappedroundHole = new G4Tubs("tappedroundHole", 0, 0.211*25.4*mm, 0.1*25.4*mm, 0, 2*pi);
	G4UnionSolid* tappedHole= new G4UnionSolid("tappedHole",tappedhexHole, tappedroundHole,0,
    				 G4ThreeVector(0, 0, (-0.1+0.02)*25.4*mm));

    G4SubtractionSolid* body10 = new G4SubtractionSolid("body10", body9, tappedHole, 0, 
    				 G4ThreeVector(tapRadius, 0, -0.025*25.4*mm));                 
    G4SubtractionSolid* body11 = new G4SubtractionSolid("body11", body10, tappedHole, 0, 
                     G4ThreeVector(tapRadius*cos(120*deg), tapRadius*sin(120*deg), -0.025*25.4*mm));
    G4SubtractionSolid* body12 = new G4SubtractionSolid("body12", body11, tappedHole, 0, 
                     G4ThreeVector(tapRadius*cos(240*deg), tapRadius*sin(240*deg), -0.025*25.4*mm));  
    //Wenqin: Omission: The 6 small holes on each hex hole are omitted. The shallow cut (groove) on each hex are omitted 
    //Wenqin: Omission: The two bigger grooves are also omitted

    G4double thruHoleRadius = 1.84*25.4*mm;
    //Wenqin: These holes are on the tap, distance Dimension Checked; 
    //Wenqin: Each hole has 4 sections, with different radius. so G4Polycone is used; Dimension Checked, roughly match
    G4double zPlanes[] = {0.2*25.4*mm, -0.035*25.4*mm, -0.035*25.4*mm, -0.115*25.4*mm,
                          -0.115*25.4*mm, -0.155*25.4*mm, -0.195*25.4*mm};
    G4double rInner[] = {0, 0, 0, 0, 0, 0, 0};
    G4double rOuter[] = {0.125*25.4*mm, 0.125*25.4*mm, 0.068*25.4*mm, 0.068*25.4*mm, 
                         0.109*25.4*mm, 0.109*25.4*mm, 0.149*25.4*mm};	       
    G4Polycone* thruHole = new G4Polycone("thruHole", pi/2, 2*pi, 7, zPlanes, rInner, rOuter);
    G4SubtractionSolid* body13 = new G4SubtractionSolid("body13", body12, thruHole, 0,
                  G4ThreeVector(thruHoleRadius, 0*25.4*mm, 0));
    G4SubtractionSolid* body14 = new G4SubtractionSolid("body14", body13, thruHole, 0,
                  G4ThreeVector(thruHoleRadius*cos(120*deg), thruHoleRadius*sin(120*deg), 0));                                                
    G4SubtractionSolid* stringAdapterPlate = new G4SubtractionSolid("String Adapter Plate", 
    				body14, thruHole, 0,
                  	G4ThreeVector(thruHoleRadius*cos(240*deg), thruHoleRadius*sin(240*deg), 0));

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(stringAdapterPlate, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created String Adapter Plate Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing String Adapter Plate Logical" << endlog; 
  return pVol;
}  
