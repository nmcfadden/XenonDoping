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
// $Id: MJGeometry17A.cc,v 1.2 2007-02-21 08:11:52 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryOPPI2.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: M. Kidd
 * CONTACT: 
 * FIRST SUBMISSION: June 21, 2012
 * 
 * REVISION:
 *
 *
 */
//---------------------------------------------------------------------------//  

#include <sstream>

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4PVParameterised.hh"
#include "G4Color.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4RotationMatrix.hh"

// MJ headers
#include "mjgeometry/MJGeometryOPPI2.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"
#include "geometry/MGGeometryIdealCoaxCrystal.hh"

using namespace std;

using namespace CLHEP;
using namespace MaGe;


//---------------------------------------------------------------------------// 
 
MJGeometryOPPI2::MJGeometryOPPI2() 
: MGGeometryDetector("OPPI2"), fTemplateCrystal(0), active_crystal_log(0), cryostatLog(0), cryovacLog(0), AlCupLog(0), polyRingLog(0), leadCylinderLog(0), CuRingLog(0), leadLidLog(0), crystal_phys(0), cryostatPhys(0), cryovacPhys(0), AlCupPhys(0),  polyRingPhys(0), leadCylinderPhys(0), CuRingPhys(0),  leadLidPhys(0) 
{

  fTemplateCrystal = new MGGeometryIdealCoaxCrystal("Template", false);
  fTemplateCrystal->SetDefaults();
  SetDefaults();
}

void MJGeometryOPPI2::ConstructDetector()
{

//---------------------------------------------------------------------------// 
// Material Definitions
//---------------------------------------------------------------------------// 

	G4Material *CuMaterial = G4Material::GetMaterial("Copper-OFHC");
	G4Material *cryoMaterial = G4Material::GetMaterial("Aluminum");
	G4Material *air = G4Material::GetMaterial("Air");
	G4Material *vac = G4Material::GetMaterial("Vacuum");
	G4Material *poly = G4Material::GetMaterial("Polyethylene");
	G4Material *PbMaterial = G4Material::GetMaterial("Lead-Mod");

	MGLog(debugging) << "Here are the materials.\n" << endlog; 

//---------------------------------------------------------------------------// 
// Dimensions lengths and heights are 1/2 lengths and 1/2 heights!
//---------------------------------------------------------------------------//

	G4double N_length = 0.5*m; //dimensions of box

	MGLog(debugging) << "Dimensions are set.\n" << endlog; 

//---------------------------------------------------------------------------//
// The position and subtraction placement vectors
//---------------------------------------------------------------------------//
	G4RotationMatrix* crystalRot = new G4RotationMatrix;
	crystalRot->rotateX(180.0*deg);


	G4ThreeVector crystal_pos(0, 0, 13.5*mm);

	MGLog(debugging) << "Here's the placement.\n" << endlog; 

//----------------------------------------------------- 				 
// The Cavity
//----------------------------------------------------- 

	G4Box* Nbox = new G4Box("Nbox",N_length,N_length,N_length);
	theDetectorLogical = new G4LogicalVolume(Nbox, air ,"theDetectorLogical",0,0,0);

	MGLog(debugging) << "Here's the cavity.\n" << endlog; 

//----------------------------------------------------- 				 
// The Cryostat
//----------------------------------------------------- 
	
	G4double cryoThickness = 1.5*mm;
	G4double cryoOuterR = 95.25/2.0*mm;
	G4double cryoInnerR = cryoOuterR - cryoThickness;
	G4double cryoLidH = 1.5*mm;
	G4double cryoH = 6.0/2.0*in;

	G4ThreeVector cryoLidPos(0, 0, cryoH);
	G4ThreeVector cryostatPos(0, 0, -13.5*mm);


	G4Tubs *cryostat0 = new G4Tubs("cryostat0", cryoInnerR, cryoOuterR, cryoH, 0*deg, 360*deg);
	G4Tubs *cryoLid = new G4Tubs("cryoLid", 0.0*mm, cryoOuterR, cryoLidH/2.0, 0*deg, 360*deg);
	G4UnionSolid *cryostat = new G4UnionSolid("cryostat", cryostat0, cryoLid, 0, cryoLidPos);

	cryostatLog = new G4LogicalVolume(cryostat, cryoMaterial, "cryostatLog");
	cryostatPhys = new G4PVPlacement(0, cryostatPos, cryostatLog, "cryostat", theDetectorLogical, false, 0);


	G4Tubs *cryovac = new G4Tubs("cryovac", 0.0, cryoInnerR, cryoH-1.0*mm, 0*deg, 360*deg);
	cryovacLog = new G4LogicalVolume(cryovac, vac, "cryovacLog");
	cryovacPhys = new G4PVPlacement(0,cryostatPos, cryovacLog, "cryovac", theDetectorLogical, false, 0);


//----------------------------------------------------- 				 
// The Crystal
//----------------------------------------------------- 


	fTemplateCrystal->Dump();

	fTemplateCrystal->ConstructDetector();
  
	MGLog(debugging) << "OPPI2 was made.\n" << endlog;  
	crystal_phys = new G4PVPlacement(0, crystal_pos, fTemplateCrystal->GetDetectorLogical(), "Crystal0CrystalColumn1", cryovacLog, false, 0);





//-----------------------------------------------------
// The Aluminum Cup
//-----------------------------------------------------

	G4double cupThickness = 0.76*mm;
	G4double cupLidH = 3.2*mm;


	G4double cupOuterR = (68.5/2.0)*mm;
	G4double cupInnerR = cupOuterR - cupThickness;
	G4double IRshieldH = 0.76*mm;
	G4double cupH = 105.0*mm+IRshieldH;
	G4double subH = cupH-cupLidH-IRshieldH;

	G4ThreeVector subPos(0,0,cupLidH-(cupH-subH)/2.0);
	G4ThreeVector cupPos(0,0,-25.7*mm+13.5*mm+IRshieldH);

	G4double AlRingH = 6.4*mm;
	G4double AlRingThickness = 0.74*mm;
	G4ThreeVector AlRingPos(0, 0, 25.7*mm);

	G4double polyRingH = AlRingH;
	G4double polyRingThickness = 0.5*mm;
	G4ThreeVector polyRingPos(0, 0, 26.8*mm-polyRingH/2.0+13.5*mm);

	G4Tubs *AlCup0 = new G4Tubs("AlCup0", 0, cupOuterR, cupH/2.0, 0*deg, 360*deg);
	G4Tubs *AlSub = new G4Tubs("AlSub", 0, cupInnerR, subH/2.0, 0*deg, 360*deg);
	G4SubtractionSolid *AlCup1 = new G4SubtractionSolid("AlCup1", AlCup0, AlSub, 0, subPos);

	G4Tubs *AlRing = new G4Tubs("AlRing", cupOuterR, cupOuterR + AlRingThickness, AlRingH/2.0, 0*deg, 360*deg);
	G4UnionSolid *AlCup = new G4UnionSolid("AlCup", AlCup1, AlRing, 0, AlRingPos);

	AlCupLog = new G4LogicalVolume(AlCup, cryoMaterial, "AlCupLog");
	AlCupPhys = new G4PVPlacement(0, cupPos, AlCupLog, "AlCup", cryovacLog, false, 0); 

	G4Tubs *polyRing = new G4Tubs("polyRing", cupOuterR, cupOuterR + polyRingThickness, polyRingH/2.0, 0*deg, 360*deg);
	polyRingLog = new G4LogicalVolume(polyRing, poly, "polyRingLog");
	polyRingPhys = new G4PVPlacement(0, polyRingPos, polyRingLog, "polyRing", cryovacLog, false, 0);


//-------------------------------------------------------------- 				 
// The Lead Cylinder
//     The lead cylinder and copper rings are not part of the 
//     OPPI2 standard geometry; it was used for the Pauli 
//     Exclusion principle study at LANL.  I left it here for 
//     future reference, but it may not be needed anymore.
//--------------------------------------------------------------

//these are for lead cryostat	
/*	G4double leadThickness = 1.15*mm;
	G4double leadOuterR = 95.25/2.0*mm;
	G4double leadInnerR = cryoOuterR - cryoThickness;
	G4double leadLidH = 1.5*mm;
	G4double leadHeight = 6.0/2.0*in;
G4ThreeVector leadPos(0, 0, -13.5*mm);
*/


	G4double leadInnerR = 4.43/2.0*in;
	G4double leadThickness = 1.15*mm;
	G4double leadOuterR = (4.43*in+2.0*leadThickness)/2.0; 	
	G4double leadHeight = 3.5/2.0*in;		

	G4ThreeVector leadPos(0, 0, leadHeight-1.35*in);		// usual position

	
//----------------------------------------------------- 				 
// The Copper Rings
//-----------------------------------------------------	

	G4double CuInnerR0 = 4.41/2.0*in;  //the outer part of the construction
	G4double CuOuterR0 = 2.50*in;
	G4double CuHeight0 = 0.625/2.0*in;
	
	G4double CuInnerR1 = 2.0*in;      //the inner part of the construction
	G4double CuOuterR1 = CuInnerR0;
	G4double CuHeight1 = 0.75/2.0*in;
	
	G4double subInnerR = CuInnerR0;  //the subtraction of the lip
	G4double subOuterR = 4.58/2.0*in;
	G4double subHeight = 0.125/2.0*in;
	
	G4double epsilon = 0.01*in;
	
	G4ThreeVector subRingPos(0,0, CuHeight0-subHeight-epsilon);
	G4ThreeVector unionPos(0, 0, CuHeight1-CuHeight0);
	
	G4Tubs *CuRing0 = new G4Tubs("CuRing0", CuInnerR0, CuOuterR0, CuHeight0, 0*deg, 360*deg);
	G4Tubs *CuRing1 = new G4Tubs("CuRing1", CuInnerR1, CuOuterR1, CuHeight1, 0*deg, 360*deg);
	G4Tubs *subRing = new G4Tubs("subRing", subInnerR-epsilon, subOuterR, subHeight+epsilon, 0*deg, 360*deg);
	
	G4SubtractionSolid *CuRing2 = new G4SubtractionSolid("CuRing2", CuRing0, subRing, 0, subRingPos);
	G4UnionSolid *CuRing = new G4UnionSolid("CuRing", CuRing2, CuRing1, 0, unionPos);
	
	G4ThreeVector ringPos1(0,0,-leadHeight - CuHeight0 + 2.0*subHeight+0.40*in);
	G4ThreeVector ringPos2(0,0,leadHeight + CuHeight0 - 2.0*subHeight+0.40*in);
	G4RotationMatrix *ringRot = new G4RotationMatrix;
	ringRot->rotateX(180*deg);
	
	CuRingLog = new G4LogicalVolume(CuRing, CuMaterial, "CuRingLog");
//	CuRingPhys = new G4PVPlacement(0, ringPos1, CuRingLog, "CuRing", theDetectorLogical, false, 0);

	G4ThreeVector leadLidPos(0, 0, leadHeight+0.40*in+CuHeight1+4.0*leadThickness);

	G4Tubs *leadCylinder = new G4Tubs("leadCylinder_1", leadInnerR, leadOuterR, leadHeight, 0*deg, 360*deg);
	leadCylinderLog = new G4LogicalVolume(leadCylinder, PbMaterial, "leadCylinderLog");
//	leadCylinderPhys = new G4PVPlacement(0, leadPos, leadCylinderLog, "leadCylinder_1", theDetectorLogical, false, 0);


	G4Tubs *leadLid = new G4Tubs("leadCylinder_2", 0.0, leadInnerR, leadThickness/2.0, 0*deg, 360*deg);
	leadLidLog = new G4LogicalVolume(leadLid, PbMaterial, "leadLidLog");
//	leadLidPhys = new G4PVPlacement(0, leadLidPos, leadLidLog, "leadCylinder_2", theDetectorLogical, false, 0);
	
//--------- Visualization attributes -------------------------------


	theDetectorLogical->SetVisAttributes(G4VisAttributes::GetInvisible());
	cryovacLog->SetVisAttributes(G4VisAttributes::GetInvisible());

	MGLog(debugging) << "Set Nbox clear.\n" << endlog; 
  
	G4VisAttributes* CopperVisAtt = new G4VisAttributes(G4Colour(0.72,0.45,0.2)); // copper
	CuRingLog->SetVisAttributes(CopperVisAtt);
  
	MGLog(debugging) << "Set copper color.\n" << endlog; 

	G4VisAttributes* SilverVisAtt = new G4VisAttributes(G4Colour(0.612,0.635,0.659)); // silvery

	fTemplateCrystal->GetDetectorLogical()->SetVisAttributes(SilverVisAtt);
	AlCupLog->SetVisAttributes(SilverVisAtt);
	cryostatLog->SetVisAttributes(SilverVisAtt);

	MGLog(debugging) << "Set silver color.\n" << endlog; 

	G4VisAttributes* LeadVisAtt = new G4VisAttributes(G4Colour(0.463,0.533,0.561)); // lead
	leadCylinderLog->SetVisAttributes(LeadVisAtt);
	leadLidLog->SetVisAttributes(LeadVisAtt);

	MGLog(debugging) << "Set lead color.\n" << endlog; 

}

void MJGeometryOPPI2::SetDefaults()
{
	G4double crystal_r = 66.5/2.0*mm;
	G4double crystal_h = 53.7*mm;  
	G4double bore_h = 2.0*mm;
	G4double bore_r = 1.5*mm;
	
	fTemplateCrystal->SetHeight(crystal_h);
        fTemplateCrystal->SetRadius(crystal_r);
	fTemplateCrystal->SetCornerRadius(2.0*mm);
	fTemplateCrystal->SetCoreBubble(false);
	fTemplateCrystal->SetCoreDepth(bore_h);
	fTemplateCrystal->SetCoreRadius(bore_r);
        fTemplateCrystal->SetIsActive(true);
	fTemplateCrystal->SetDeadLayerConsidered(true);
	fTemplateCrystal->SetExcessSideDeadLayerConsidered(true);
	fTemplateCrystal->SetExcessSideDeadLayerThickness((1.925-0.87)*mm);
	fTemplateCrystal->SetOuterDeadLayerThickness(0.87*mm);
	fTemplateCrystal->SetCoreDeadLayerThickness(0.2*micrometer);
        fTemplateCrystal->SetIsOneOfMany(false);
        fTemplateCrystal->SetG4MaterialName("Germanium-Nat");
	fTemplateCrystal->SetBeveledEdgeConsidered(true);
	fTemplateCrystal->SetBeveledEdgeAngle(45.0*deg);
	fTemplateCrystal->SetBeveledEdgeT(6.0*mm);
    MGLog(debugging) << "Defaults Set.\n" << endlog; 

}
