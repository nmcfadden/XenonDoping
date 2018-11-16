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
// $Id: MJcodeTemplate.cc,v 1.3 2005-02-15 23:26:55 akbarm Exp $ 
//      
// CLASS IMPLEMENTATION:  TSCrystalAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Origin at top of the crystal assembly HV ring (highest point), centered
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ryan Martin
 * CONTACT: 
 * REVISION:
 *
 * 06-13-2011, Created, R. Martin
 * 07-30-2012, Changed color of various component parts, K. Nguyen
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"                                                                                                                                               
#include "G4RotationMatrix.hh"
#include "G4AffineTransform.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"

#include "tonscalegeometry/TSCrystalAssembly.hh"
#include "tonscalegeometry/TSVisAttributes.hh"

#include "mjdemonstrator/MJVDemoPart.hh"
#include "mjdemonstrator/MJDemo7StringAssembly.hh"
#include "mjdemonstrator/MJDemoBottomThermalShieldAssembly.hh"
#include "tonscalegeometry/TSCryostatAssembly.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

TSCrystalAssembly::TSCrystalAssembly(G4String arg)
{
  fName=arg;
  fCrystalRadius=77./2.*mm;
  fCrystalHeight=33*mm;
  
  fRodOuterRadius=3*mm;
  fRodInnerRadius=0.;
  fRodToCrystalRadialClearance=5*mm;
	
  fEpsilon=0.001*mm;
	
  fHVRingInnerRadius= fCrystalRadius-5*mm;
  fHVRingOuterRadius= fCrystalRadius+ 2*fRodOuterRadius+fRodToCrystalRadialClearance;
  fHVRingThickness= 5*mm;
	
  fPTFEBlockX=15*mm;
  fPTFEBlockY=10*mm;	
  fPTFEBlockZ=7*mm;
	
  fMercedesThickness=8*mm;
  fMercedesRadius=fHVRingOuterRadius;
  fMercedesCutoutRadius=fMercedesRadius-1*cm;

  fRodHeight=fCrystalHeight+fPTFEBlockZ;
}

// -----------------------------------------------------------------------

void TSCrystalAssembly::Initialize()
{
  fHVRingInnerRadius= fCrystalRadius-5*mm;
  fHVRingOuterRadius= fCrystalRadius+ 2*fRodOuterRadius+fRodToCrystalRadialClearance;

  fMercedesRadius=fHVRingOuterRadius;
  fMercedesCutoutRadius=fMercedesRadius-1*cm;

  fRodHeight=fCrystalHeight+fPTFEBlockZ;
}

//---------------------------------------------------------------------------//

void TSCrystalAssembly::Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation, G4LogicalVolume *motherlogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4double CrystalTopZ = -fHVRingThickness-fEpsilon;
	
  G4Tubs *CrystalTub = new G4Tubs(fName+"_CrystalTub",0.,fCrystalRadius,0.5*fCrystalHeight, 0, 2*pi);
  G4LogicalVolume *CrystalLogical = new G4LogicalVolume(CrystalTub,G4Material::GetMaterial("Germanium-Enr"),fName+"_CrystalLogical");
  //CrystalLogical->SetVisAttributes(new TSVisAttributesSolid("blue"));
  CrystalLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.878,0.878,0.867))); // FIXME: Changed color attribute from 
                                                                                      //        blue to germanium, K. Nguyen
  
  G4ThreeVector* CrystalLocalPosition = new G4ThreeVector(0.,0.,CrystalTopZ-0.5*fCrystalHeight);
  G4RotationMatrix* CrystalLocalRotation = new G4RotationMatrix();
  G4AffineTransform* CrystalAssemblyAffine = new G4AffineTransform(CrystalLocalRotation, *CrystalLocalPosition);
  *CrystalAssemblyAffine *= *assemAffine;  			
  G4ThreeVector* CrystalGlobalPosition=new G4ThreeVector(CrystalAssemblyAffine->NetTranslation());
  G4RotationMatrix* CrystalGlobalRotation= new G4RotationMatrix(CrystalAssemblyAffine->NetRotation());
				
  new G4PVPlacement(CrystalGlobalRotation, *CrystalGlobalPosition,CrystalLogical,fName+"_ActiveCrystal_001",motherlogical,false,0,false);

  //rods for the mount (3 placements of the same logical volume)
  G4Tubs *RodTub = new G4Tubs(fName+"_RodTub",fRodInnerRadius,fRodOuterRadius,0.5*fRodHeight, 0, 2*pi);
  G4LogicalVolume *RodLogical = new G4LogicalVolume(RodTub,G4Material::GetMaterial("Copper-EF"),fName+"_RodLogical");
  //RodLogical->SetVisAttributes(new TSVisAttributesSolid("red"));
  RodLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0))); // FIXME: Changed color attribute from
                                                                                      //        red to copper, K. Nguyen
	
  G4double RodRadialOrigin=fRodOuterRadius+fRodToCrystalRadialClearance+fCrystalRadius;
	
  ////rod 1
  G4ThreeVector* Rod1LocalPosition = new G4ThreeVector(RodRadialOrigin,0.,CrystalTopZ-0.5*fRodHeight);
  G4RotationMatrix* Rod1LocalRotation = new G4RotationMatrix();
  G4AffineTransform* Rod1AssemblyAffine = new G4AffineTransform(Rod1LocalRotation,	*Rod1LocalPosition);
  *Rod1AssemblyAffine *= *assemAffine;  
  G4ThreeVector* Rod1GlobalPosition=new G4ThreeVector(Rod1AssemblyAffine->NetTranslation());
  G4RotationMatrix* Rod1GlobalRotation= new G4RotationMatrix(Rod1AssemblyAffine->NetRotation());
  new G4PVPlacement(Rod1GlobalRotation, *Rod1GlobalPosition,RodLogical,fName+"_Rod_001",motherlogical,false,0,false);

  ////rod 2
  G4ThreeVector* Rod2LocalPosition = new G4ThreeVector(RodRadialOrigin*cos(120*deg),RodRadialOrigin*sin(120*deg),CrystalTopZ-0.5*fRodHeight);
  G4RotationMatrix* Rod2LocalRotation = new G4RotationMatrix();
  G4AffineTransform* Rod2AssemblyAffine = new G4AffineTransform(Rod2LocalRotation,	*Rod2LocalPosition);
  *Rod2AssemblyAffine *= *assemAffine;  
  G4ThreeVector* Rod2GlobalPosition=new G4ThreeVector(Rod2AssemblyAffine->NetTranslation());
  G4RotationMatrix* Rod2GlobalRotation= new G4RotationMatrix(Rod2AssemblyAffine->NetRotation());	
  new G4PVPlacement(Rod2GlobalRotation, *Rod2GlobalPosition,RodLogical,fName+"_Rod_002",motherlogical,false,0,false);

  ////rod 3
  G4ThreeVector* Rod3LocalPosition = new G4ThreeVector(RodRadialOrigin*cos(240*deg),RodRadialOrigin*sin(240*deg),CrystalTopZ-0.5*fRodHeight);
  G4RotationMatrix* Rod3LocalRotation = new G4RotationMatrix();
  G4AffineTransform* Rod3AssemblyAffine = new G4AffineTransform(Rod3LocalRotation,	*Rod3LocalPosition);
  *Rod3AssemblyAffine *= *assemAffine;  
  G4ThreeVector* Rod3GlobalPosition=new G4ThreeVector(Rod3AssemblyAffine->NetTranslation());
  G4RotationMatrix* Rod3GlobalRotation= new G4RotationMatrix(Rod3AssemblyAffine->NetRotation());	
  new G4PVPlacement(Rod3GlobalRotation, *Rod3GlobalPosition,RodLogical,fName+"_Rod_003",motherlogical,false,0,false);

  //HV ring
  G4Tubs *HVRingTub = new G4Tubs(fName+"_HVRingTub",fHVRingInnerRadius,fHVRingOuterRadius,0.5*fHVRingThickness, 0, 2*pi);
  G4LogicalVolume *HVRingLogical = new G4LogicalVolume(HVRingTub,G4Material::GetMaterial("Copper-EF"),fName+"_HVRingLogical");
  //HVRingLogical->SetVisAttributes(new TSVisAttributesSolid("red"));
  HVRingLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0))); // FIXME: Changed color attribute from
                                                                                         //        red to copper, K. Nguyen
	
  G4ThreeVector* HVRingLocalPosition = new G4ThreeVector(0.,0.,-0.5*fHVRingThickness);
  G4RotationMatrix* HVRingLocalRotation = new G4RotationMatrix();
  G4AffineTransform* HVRingAssemblyAffine = new G4AffineTransform(HVRingLocalRotation,	*HVRingLocalPosition);
  *HVRingAssemblyAffine *= *assemAffine;  
  G4ThreeVector* HVRingGlobalPosition=new G4ThreeVector(HVRingAssemblyAffine->NetTranslation());
  G4RotationMatrix* HVRingGlobalRotation= new G4RotationMatrix(HVRingAssemblyAffine->NetRotation());	
  new G4PVPlacement(HVRingGlobalRotation, *HVRingGlobalPosition,HVRingLogical,fName+"_HVRing",motherlogical,false,0,false);

  //PTFE blocks
  G4Box *PTFEBlockBox = new G4Box(fName+"_PTFEBlockBox",0.5*fPTFEBlockX,0.5*fPTFEBlockY,0.5*fPTFEBlockZ);
  G4LogicalVolume *PTFEBlockLogical = new G4LogicalVolume(PTFEBlockBox,G4Material::GetMaterial("Teflon"),fName+"_PTFEBlockLogical");
  PTFEBlockLogical->SetVisAttributes(new TSVisAttributesSolid("white"));

  G4double PTFEBlockRadius=fCrystalRadius-0.5*fPTFEBlockX;
	
  ////PTFE Block 1
  G4ThreeVector* PTFEBlock1LocalPosition = new G4ThreeVector(PTFEBlockRadius,0.,CrystalTopZ-fCrystalHeight-0.5*fPTFEBlockZ-fEpsilon);
  G4RotationMatrix* PTFEBlock1LocalRotation = new G4RotationMatrix();
  PTFEBlock1LocalRotation->rotateZ(0*deg); 
  G4AffineTransform* PTFEBlock1AssemblyAffine = new G4AffineTransform(PTFEBlock1LocalRotation,	*PTFEBlock1LocalPosition);
  *PTFEBlock1AssemblyAffine *= *assemAffine;  
  G4ThreeVector* PTFEBlock1GlobalPosition=new G4ThreeVector(PTFEBlock1AssemblyAffine->NetTranslation());
  G4RotationMatrix* PTFEBlock1GlobalRotation= new G4RotationMatrix(PTFEBlock1AssemblyAffine->NetRotation());	
  new G4PVPlacement(PTFEBlock1GlobalRotation, *PTFEBlock1GlobalPosition,PTFEBlockLogical,fName+"_PTFEBlock1",motherlogical,false,0,false);

  ////PTFE Block 2
  G4ThreeVector* PTFEBlock2LocalPosition = new G4ThreeVector(PTFEBlockRadius*cos(120*deg), PTFEBlockRadius*sin(120*deg), 
                                                             CrystalTopZ-fCrystalHeight-0.5*fPTFEBlockZ-fEpsilon);
  G4RotationMatrix* PTFEBlock2LocalRotation = new G4RotationMatrix();
  PTFEBlock2LocalRotation->rotateZ((-120)*deg); 
  G4AffineTransform* PTFEBlock2AssemblyAffine = new G4AffineTransform(PTFEBlock2LocalRotation,	*PTFEBlock2LocalPosition);
  *PTFEBlock2AssemblyAffine *= *assemAffine;  
  G4ThreeVector* PTFEBlock2GlobalPosition=new G4ThreeVector(PTFEBlock2AssemblyAffine->NetTranslation());
  G4RotationMatrix* PTFEBlock2GlobalRotation= new G4RotationMatrix(PTFEBlock2AssemblyAffine->NetRotation());	
  new G4PVPlacement(PTFEBlock2GlobalRotation, *PTFEBlock2GlobalPosition,PTFEBlockLogical,fName+"_PTFEBlock2",motherlogical,false,0,false);

  ////PTFE Block 3
  G4ThreeVector* PTFEBlock3LocalPosition = new G4ThreeVector(PTFEBlockRadius*cos(240*deg), PTFEBlockRadius*sin(240*deg), 
                                                             CrystalTopZ-fCrystalHeight-0.5*fPTFEBlockZ-fEpsilon);
  G4RotationMatrix* PTFEBlock3LocalRotation = new G4RotationMatrix();
  PTFEBlock3LocalRotation->rotateZ((-240)*deg); 
  G4AffineTransform* PTFEBlock3AssemblyAffine = new G4AffineTransform(PTFEBlock3LocalRotation,	*PTFEBlock3LocalPosition);
  *PTFEBlock3AssemblyAffine *= *assemAffine;  
  G4ThreeVector* PTFEBlock3GlobalPosition=new G4ThreeVector(PTFEBlock3AssemblyAffine->NetTranslation());
  G4RotationMatrix* PTFEBlock3GlobalRotation= new G4RotationMatrix(PTFEBlock3AssemblyAffine->NetRotation());	
  new G4PVPlacement(PTFEBlock3GlobalRotation, *PTFEBlock3GlobalPosition,PTFEBlockLogical,fName+"_PTFEBlock3",motherlogical,false,0,false);

  //"Mercedes", simple disk with 3 disks cutout
  G4Tubs *MercedesFullTub = new G4Tubs(fName+"_MercedesFullTub",0,fMercedesRadius,0.5*fMercedesThickness, 0, 2*pi);
  //G4Tubs *MercedesCutoutTub = new G4Tubs(fName+"_MercedesCutoutTub",0,fMercedesCutoutRadius,0.5*fMercedesThickness, 0, 2*pi);
  G4Tubs *MercedesCutoutTub = new G4Tubs(fName+"_MercedesCutoutTub",0,fMercedesCutoutRadius,0.51*fMercedesThickness, 0, 2*pi);
  G4SubtractionSolid* Mercedes1 = new G4SubtractionSolid("Mercedes1", MercedesFullTub, MercedesCutoutTub, 0,
                                                         G4ThreeVector(fMercedesRadius*cos(60*deg), fMercedesRadius*sin(60*deg), 0));
  G4SubtractionSolid* Mercedes2 = new G4SubtractionSolid("Mercedes2", Mercedes1, MercedesCutoutTub, 0,
                                                         G4ThreeVector(fMercedesRadius*cos(180*deg), fMercedesRadius*sin(180*deg), 0));
  G4SubtractionSolid* MercedesVol = new G4SubtractionSolid("MercedesVol", Mercedes2, MercedesCutoutTub, 0,
                                                           G4ThreeVector(fMercedesRadius*cos(300*deg), fMercedesRadius*sin(300*deg), 0));
  G4LogicalVolume *MercedesLogical = new G4LogicalVolume(MercedesVol,G4Material::GetMaterial("Copper-EF"),fName+"_MercedesLogical");
  //MercedesLogical->SetVisAttributes(new TSVisAttributesSolid("red"));
  MercedesLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0))); // FIXME: Changed color attribute from
                                                                                           //        red to copper, K. Nguyen
	
  G4ThreeVector* MercedesLocalPosition = new G4ThreeVector(0.,0.,CrystalTopZ-fCrystalHeight-fEpsilon-fPTFEBlockZ-fEpsilon-0.5*fMercedesThickness);
  G4RotationMatrix* MercedesLocalRotation = new G4RotationMatrix();
  G4AffineTransform* MercedesAssemblyAffine = new G4AffineTransform(MercedesLocalRotation, *MercedesLocalPosition);
  *MercedesAssemblyAffine *= *assemAffine;  			
  G4ThreeVector* MercedesGlobalPosition=new G4ThreeVector(MercedesAssemblyAffine->NetTranslation());
  G4RotationMatrix* MercedesGlobalRotation= new G4RotationMatrix(MercedesAssemblyAffine->NetRotation());
	
  new G4PVPlacement(MercedesGlobalRotation, *MercedesGlobalPosition, MercedesLogical,fName+"_Mercedes",motherlogical,false,0,false);
	
  //uncommenting one of these messes things up...
	/*delete CrystalLocalPosition;
  delete CrystalLocalRotation; 
  delete CrystalAssemblyAffine;
  delete CrystalGlobalPosition;
  delete CrystalGlobalRotation;*/	
  /*delete Rod1LocalPosition;
  delete Rod1LocalRotation; 
  delete Rod1AssemblyAffine;
  delete Rod1GlobalPosition;
  delete Rod1GlobalRotation;*/	
  /*delete Rod2LocalPosition;
  delete Rod2LocalRotation; 
  delete Rod2AssemblyAffine;
  delete Rod2GlobalPosition;
  delete Rod2GlobalRotation;*/
  /*delete Rod3LocalPosition;
  delete Rod3LocalRotation; 
  delete Rod3AssemblyAffine;
  delete Rod3GlobalPosition;
  delete Rod3GlobalRotation;*/	
	/*delete HVRingLocalPosition;
  delete HVRingLocalRotation; 
  delete HVRingAssemblyAffine;
  delete HVRingGlobalPosition;
  delete HVRingGlobalRotation;*/
	/*delete PTFEBlock1LocalPosition;
  delete PTFEBlock1LocalRotation; 
  delete PTFEBlock1AssemblyAffine;
  delete PTFEBlock1GlobalPosition;
  delete PTFEBlock1GlobalRotation;*/
	/*delete PTFEBlock2LocalPosition;
  delete PTFEBlock2LocalRotation; 
  delete PTFEBlock2AssemblyAffine;
  delete PTFEBlock2GlobalPosition;
  delete PTFEBlock2GlobalRotation;*/
	/*delete PTFEBlock3LocalPosition;
  delete PTFEBlock3LocalRotation; 
  delete PTFEBlock3AssemblyAffine;
  delete PTFEBlock3GlobalPosition;
  delete PTFEBlock3GlobalRotation;*/
	/*delete MercedesLocalPosition;
  delete MercedesLocalRotation; 
  delete MercedesAssemblyAffine;
  delete MercedesGlobalPosition;
  delete MercedesGlobalRotation;*/

}

// -----------------------------------------------------------------------

