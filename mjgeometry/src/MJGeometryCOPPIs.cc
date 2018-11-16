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
// CLASS IMPLEMENTATION:  MJGeometryCOPPIs.cc
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
 * FIRST SUBMISSION: June 22, 2012
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
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
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
#include "mjgeometry/MJGeometryCOPPIs.hh"
#include "mjgeometry/MJGeometryCOPPIsMessenger.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"
#include "geometry/MGGeometryIdealCoaxCrystal.hh"

using namespace std;

using namespace CLHEP;
using namespace MaGe;


//---------------------------------------------------------------------------// 
 
MJGeometryCOPPIs::MJGeometryCOPPIs() 
: MGGeometryDetector("COPPIs"), 
	fTemplateCrystal(0), 
	fTemplateCrystal2(0), 
	cryostatLog(0), 
	cryostatLog2(0),
	cryovacLog(0), 
	cryovacLog2(0), 
	AlCupLog(0), 	
	leadCylinderLog(0), 
	ZnFoilLog(0),
	ZrFoilLog(0),
	NbFoilLog(0),
	CdFoilLog(0),
	leadCylinderPhys(0), 
	cryostatPhys(0), 
	cryovacPhys(0), 
	AlCupPhys(0), 
	crystal_phys(0),  
	cryostatPhys2(0), 
	cryovacPhys2(0),
	AlCupPhys2(0),  
	crystal_phys2(0),
	ZnFoilPhys(0),
	ZrFoilPhys(0),
	NbFoilPhys(0),
	CdFoilPhys(0)
{

  fTemplateCrystal = new MGGeometryIdealCoaxCrystal("OPPI3temp", false, true);
  fTemplateCrystal2 = new MGGeometryIdealCoaxCrystal("OPPI4temp", false, true);
  fTemplateCrystal->SetDefaults();
  fTemplateCrystal2->SetDefaults();
  SetDefaults();

  fMessenger = new MJGeometryCOPPIsMessenger(this);

}

MJGeometryCOPPIs::~MJGeometryCOPPIs()
{
  delete fMessenger;
  delete fTemplateCrystal;
  delete fTemplateCrystal2;
}

void MJGeometryCOPPIs::ConstructDetector()
{

//---------------------------------------------------------------------------// 
// Material Definitions
//---------------------------------------------------------------------------// 

//	G4Material *CFiber = G4Material::GetMaterial("Carbon");
	G4Material *CuMaterial = G4Material::GetMaterial("Copper-OFHC");
//	G4Material *crystalMaterial = G4Material::GetMaterial("Germanium-Nat");
//	G4Material *cryoMaterial = G4Material::GetMaterial("Aluminum");
	G4Material *air = G4Material::GetMaterial("Air");
	G4Material *vac = G4Material::GetMaterial("Vacuum");
//	G4Material *poly = G4Material::GetMaterial("Polyethylene");

	G4double density = 1.6*g/cm3;  //from email from Kyle Sherrod  4/30/2012
	G4double a = 12.0107*g/mole;
	G4Material *CFiber = new G4Material("CFiber", 6., a, density);

	G4double densityZn = 7.14*g/cm3;
	G4double aZn = 65.38*g/mole;
	G4Material *Zn = new G4Material("Zn", 30., aZn, densityZn);

	G4double densityZr = 6.52*g/cm3;
	G4double aZr = 91.224*g/mole;
	G4Material *Zr = new G4Material("Zr", 40., aZr, densityZr);

	G4double densityNb = 8.57*g/cm3;
	G4double aNb = 92.90638*g/mole;
	G4Material *Nb = new G4Material("Nb", 41., aNb, densityNb);

	G4double densityCd = 8.65*g/cm3;
	G4double aCd = 112.411*g/mole;
	G4Material *Cd = new G4Material("Cd", 48., aCd, densityCd);


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
	
	G4double cryoThickness = 1.6*mm;
	G4double cryoOuterR = 82.804/2.0*mm;  //(diameter of 3.26 inches)
	G4double cryoInnerR = cryoOuterR - cryoThickness;
	G4double cryoLidH = 0.5*mm;  //  given is 0.9*mm
	G4double cryoH = 5.25/2.0*in;

	G4ThreeVector cryoLidPos(0, 0, cryoH);
	G4ThreeVector cryostatPos(0, 0, -cryoH-cryoLidH/2.0-fSetDetSep/2.0);
	
	G4ThreeVector cryostatPos2(0, 0, cryoH+cryoLidH/2.0+fSetDetSep/2.0);

	G4ThreeVector cryovacPos(0, 0, -0.0001*mm);
	G4ThreeVector cryovacPos2(0, 0,-0.0001*mm);

	G4Tubs *cryostat0 = new G4Tubs("cryostat0", cryoInnerR, cryoOuterR, cryoH, 0*deg, 360*deg);
	G4Tubs *cryoLid = new G4Tubs("cryoLid", 0.0*mm, cryoOuterR, cryoLidH/2.0, 0*deg, 360*deg);
	G4UnionSolid *cryostat = new G4UnionSolid("cryostat", cryostat0, cryoLid, 0, cryoLidPos);

	cryostatLog = new G4LogicalVolume(cryostat, CFiber, "cryostatLog");


	cryostatLog2 = new G4LogicalVolume(cryostat, CFiber, "cryostatLog2");


	G4Tubs *cryovac = new G4Tubs("cryovac", 0.0, cryoInnerR-0.001*mm, cryoH-0.5*cryoLidH, 0*deg, 360*deg);
	cryovacLog = new G4LogicalVolume(cryovac, vac, "cryovacLog");


	cryovacLog2 = new G4LogicalVolume(cryovac, vac, "cryovacLog2");


//-----------------------------------------------------
// The Copper Cup
//-----------------------------------------------------
	G4double IRshieldH = 0.03*mm;
	G4double cupThickness = 0.76*mm;
	G4double cupLidH = 3.0*mm+21.0*mm;


	G4double cupOuterR = (68.5/2.0)*mm;
	G4double cupInnerR = cupOuterR - cupThickness;

	G4double cupH = 105.0*mm+IRshieldH+21.0*mm;
	G4double subH = cupH-cupLidH-IRshieldH;

	G4ThreeVector subPos(0,0,cupLidH-(cupH-subH)/2.0);
	G4ThreeVector cupPos(0,0,cryoH-0.5*cryoLidH-cupH/2.0-4.0*mm);
	G4ThreeVector cupPos2(0,0,cryoH-0.5*cryoLidH-cupH/2.0-4.0*mm);

	G4Tubs *AlCup0 = new G4Tubs("AlCup0", 0, cupOuterR, cupH/2.0, 0*deg, 360*deg);
	G4Tubs *AlSub = new G4Tubs("AlSub", 0, cupInnerR, subH/2.0, 0*deg, 360*deg);
	G4SubtractionSolid *AlCup1 = new G4SubtractionSolid("AlCup1", AlCup0, AlSub, 0, subPos);

	AlCupLog = new G4LogicalVolume(AlCup1, CuMaterial, "AlCupLog");


//----------------------------------------------------- 				 
// The Crystals
//----------------------------------------------------- 

	G4ThreeVector crystal_pos(0, 0,(cryoH-0.5*cryoLidH-53.1/2.0*mm-4.0*mm-IRshieldH));

	G4ThreeVector crystal_pos2(0, 0,cryoH-0.5*cryoLidH-53.4/2.0*mm-4.0*mm-IRshieldH);
	fTemplateCrystal->SetOuterDeadLayerThickness(fSetOPPI3TDL);
	fTemplateCrystal2->SetOuterDeadLayerThickness(fSetOPPI4TDL);

	if(fSetOPPI3SDL > 0.0*mm) 
	{
		fTemplateCrystal->SetExcessSideDeadLayerConsidered(true);
		fTemplateCrystal->SetExcessSideDeadLayerThickness(fSetOPPI3SDL);
	}

	if(fSetOPPI4SDL > 0.0*mm) 
	{
		fTemplateCrystal2->SetExcessSideDeadLayerConsidered(true);
		fTemplateCrystal2->SetExcessSideDeadLayerThickness(fSetOPPI4SDL);
	}

	fTemplateCrystal->Dump();
	fTemplateCrystal2->Dump();

	fTemplateCrystal->ConstructDetector();
  
	MGLog(debugging) << "OPPI3 was made.\n" << endlog;  


	fTemplateCrystal2->ConstructDetector();
  
	MGLog(debugging) << "OPPI4 was made.\n" << endlog;

	MGLog(routine)<<"Use OPPI3 set to "<<fUseOPPI3<<"\n";
	MGLog(routine)<<"Use OPPI4 set to "<<fUseOPPI4<<"\n";
	MGLog(routine)<<"Use Pb Cyl set to "<<fUsePbCyl<<"\n";
	MGLog(routine)<<"Use Zn set to "<<fUseZn<<"\n";
  	MGLog(routine)<<"Use Zr set to "<<fUseZr<<"\n";
	MGLog(routine)<<"Use Nb set to "<<fUseNb<<"\n";
	MGLog(routine)<<"Use Cd set to "<<fUseCd<<"\n";


//----------------------------------------------------- 				 
// The Lead Cylinder
//-----------------------------------------------------	
	if(fUsePbCyl == true){

	MakePbCyl(cryoOuterR);

	}

//--------- Placements ---------------------------------------------

	if(fUseOPPI3 == true)
	{
	cryostatPhys = new G4PVPlacement(0, cryostatPos, cryostatLog, "cryostat", theDetectorLogical, false, 0);
	cryovacPhys = new G4PVPlacement(0,cryostatPos, cryovacLog, "cryovac", theDetectorLogical, false, 0);
	AlCupPhys = new G4PVPlacement(0, cupPos, AlCupLog, "AlCup", cryovacLog, false, 0); 
	crystal_phys = new G4PVPlacement(0, crystal_pos, fTemplateCrystal->GetDetectorLogical(), "OPPI3", cryovacLog, false, 0);
	}

	if(fUseOPPI4 == true)
	{
	cryostatPhys2 = new G4PVPlacement(crystalRot, cryostatPos2, cryostatLog2, "cryostat2", theDetectorLogical, false, 0);
	cryovacPhys2 = new G4PVPlacement(crystalRot, cryostatPos2, cryovacLog2, "cryovac2", theDetectorLogical, false, 0);
	AlCupPhys2 = new G4PVPlacement(0, cupPos2, AlCupLog, "AlCup2", cryovacLog2, false, 1); 
	crystal_phys2 = new G4PVPlacement(0, crystal_pos2, fTemplateCrystal2->GetDetectorLogical(), "OPPI4", cryovacLog2, false, 0);
	}


//-------------------------
//  Foils
//-------------------------


	G4double foilL = 5.0*cm/2.0;
	G4double foilT = 2.0*mm/2.0;
	G4double CdFoilT = 1.0*mm/2.0;

	G4double eps = 0.01*mm;

	G4ThreeVector Zn_pos(0, 0, fSetDetSep/2.0-foilT-eps);
	G4ThreeVector Zr_pos(0, 0, fSetDetSep/2.0-3.0*foilT-eps);
	G4ThreeVector Nb_pos(0, 0, fSetDetSep/2.0-5.0*foilT-eps);
	G4ThreeVector Cd_pos(0, 0, fSetDetSep/2.0-6.5*foilT-eps);  // 6.5 b/c the Cd foil was thinner.

	G4Box* ZnFoil = new G4Box("ZnFoil",foilL,foilL,foilT);
	ZnFoilLog = new G4LogicalVolume(ZnFoil, Zn ,"theDetectorLogical",0,0,0);

	G4Box* ZrFoil = new G4Box("ZrFoil",foilL,foilL,foilT);
	ZrFoilLog = new G4LogicalVolume(ZrFoil, Zr ,"theDetectorLogical",0,0,0);

	G4Box* NbFoil = new G4Box("NbFoil",foilL,foilL,foilT);
	NbFoilLog = new G4LogicalVolume(NbFoil, Nb ,"theDetectorLogical",0,0,0);

	G4Box* CdFoil = new G4Box("CdFoil",foilL,foilL,CdFoilT);
	CdFoilLog = new G4LogicalVolume(CdFoil, Cd ,"theDetectorLogical",0,0,0);

	if(fUseZn == true)	ZnFoilPhys = new G4PVPlacement(0, Zn_pos, ZnFoilLog, "ZnFoil", theDetectorLogical, false, 0);

	if(fUseZn == false && fUseZr == true && fUseNb == false && fUseCd == false)	ZrFoilPhys = new G4PVPlacement(0, Zn_pos, ZrFoilLog, "ZrFoil", theDetectorLogical, false, 0);
	else if(fUseZr == true)	ZrFoilPhys = new G4PVPlacement(0, Zr_pos, ZrFoilLog, "ZrFoil", theDetectorLogical, false, 0);

	if(fUseZr == false && fUseNb == true && fUseCd == false && fUseZn == false)	NbFoilPhys = new G4PVPlacement(0, Zn_pos, NbFoilLog, "NbFoil", theDetectorLogical, false, 0);
	else if(fUseNb == true)	NbFoilPhys = new G4PVPlacement(0, Nb_pos, NbFoilLog, "NbFoil", theDetectorLogical, false, 0);

	if(fUseNb == false && fUseCd == true && fUseZn == false && fUseZr == false)	CdFoilPhys = new G4PVPlacement(0, Zn_pos, CdFoilLog, "CdFoil", theDetectorLogical, false, 0);
	else if(fUseCd == true)	CdFoilPhys = new G4PVPlacement(0, Cd_pos, CdFoilLog, "CdFoil", theDetectorLogical, false, 0);

//--------- Visualization attributes -------------------------------

  
	G4VisAttributes* YellowVisAtt = new G4VisAttributes(G4Colour(0.72,0.45,0.2, 0.3)); // copper
	AlCupLog->SetVisAttributes(YellowVisAtt);
  
	MGLog(debugging) << "Set yellow color.\n" << endlog; 

	G4VisAttributes* GreenVisAtt = new G4VisAttributes(G4Colour(0.612,0.635,0.659, 0.4)); // silvery

	G4VisAttributes* GreyVisAtt = new G4VisAttributes(G4Colour(74./256.,74./256.,74./256., 0.4)); // dark grey

	fTemplateCrystal->GetDetectorLogical()->SetVisAttributes(GreenVisAtt);
	cryostatLog->SetVisAttributes(GreyVisAtt);
	cryostatLog2->SetVisAttributes(GreyVisAtt);
}

void MJGeometryCOPPIs::MakePbCyl(G4double cryoOuterR)
{
	G4double leadInnerR = cryoOuterR+0.1*cm;
	G4double leadThickness = 1.15*mm;
	G4double leadOuterR = (2.0*leadInnerR+2.0*leadThickness)/2.0; 	
	G4double leadHeight = fSetPbL/2.0;		
	G4Material *PbMaterial = G4Material::GetMaterial("Lead-Mod");

	G4ThreeVector leadPos(0, 0, 0);		// usual position


	G4ThreeVector leadLidPos(0, 0, leadHeight+0.40*in+0.75/2.0*in+4.0*leadThickness);

	G4Tubs *leadCylinder = new G4Tubs("leadCylinder_1", leadInnerR, leadOuterR, leadHeight, 0*deg, 360*deg);
	leadCylinderLog = new G4LogicalVolume(leadCylinder, PbMaterial, "leadCylinderLog");

	leadCylinderPhys = new G4PVPlacement(0, leadPos, leadCylinderLog, "leadCylinder_1", theDetectorLogical, false, 0);

}

void MJGeometryCOPPIs::SetDefaults()
{
	G4double crystal_r = 66.7/2.0*mm;
	G4double crystal_h = 53.1*mm;  
	G4double bore_h = 2.1*mm;
	G4double bore_r = 2.05*mm;

	G4double crystal_h2 = 53.4*mm;  
	G4double bore_h2 = 2.0*mm;
	G4double bore_r2 = 3.6/2.0*mm;

	fSetDetSep = 2.54/2.0*cm;
	fUseOPPI3 = true;
	fUseOPPI4 = true;
	fUsePbCyl = false;
	fUseZn = false;
	fUseZr = false;
	fUseNb = false;
	fUseCd = false;

	fSetPbL = 50.0*cm;
	fSetOPPI3TDL = 0.7*mm;
	fSetOPPI4TDL = 0.7*mm;
	fSetOPPI3SDL = 0.0*mm;
	fSetOPPI4SDL = 0.0*mm;

	fTemplateCrystal->SetHeight(crystal_h);
        fTemplateCrystal->SetRadius(crystal_r);
	fTemplateCrystal->SetCornerRadius(8.0*mm);
	fTemplateCrystal->SetCoreBubble(false);
	fTemplateCrystal->ActivateSetName(true);
	fTemplateCrystal->SetName("OPPI3");
	fTemplateCrystal->SetCoreDepth(bore_h);
	fTemplateCrystal->SetCoreRadius(bore_r);
        fTemplateCrystal->SetIsActive(true);
	fTemplateCrystal->SetDeadLayerConsidered(true);
	fTemplateCrystal->SetExcessSideDeadLayerConsidered(false);
	fTemplateCrystal->SetOuterDeadLayerThickness(fSetOPPI3TDL);
	fTemplateCrystal->SetCoreDeadLayerThickness(0.3*micrometer);
        fTemplateCrystal->SetIsOneOfMany(false);
        fTemplateCrystal->SetG4MaterialName("Germanium-Nat");
	fTemplateCrystal->SetBeveledEdgeConsidered(true);
	fTemplateCrystal->SetBeveledEdgeAngle(45.0*deg);
	fTemplateCrystal->SetBeveledEdgeT(6.0*mm);


	fTemplateCrystal2->SetHeight(crystal_h2);
        fTemplateCrystal2->SetRadius(crystal_r);
	fTemplateCrystal2->SetCornerRadius(8.0*mm);
	fTemplateCrystal2->SetCoreBubble(false);
	fTemplateCrystal2->ActivateSetName(true);
	fTemplateCrystal2->SetName("OPPI4");
	fTemplateCrystal2->SetCoreDepth(bore_h2);
	fTemplateCrystal2->SetCoreRadius(bore_r2);
        fTemplateCrystal2->SetIsActive(true);
	fTemplateCrystal2->SetDeadLayerConsidered(true);
	fTemplateCrystal2->SetExcessSideDeadLayerConsidered(false);
	fTemplateCrystal2->SetOuterDeadLayerThickness(fSetOPPI4TDL);
	fTemplateCrystal2->SetCoreDeadLayerThickness(0.3*micrometer);
        fTemplateCrystal2->SetIsOneOfMany(false);
        fTemplateCrystal2->SetG4MaterialName("Germanium-Nat");
	fTemplateCrystal2->SetBeveledEdgeConsidered(true);
	fTemplateCrystal2->SetBeveledEdgeAngle(45.0*deg);
	fTemplateCrystal2->SetBeveledEdgeT(6.0*mm);




    MGLog(debugging) << "Defaults Set.\n" << endlog; 

}

int MJGeometryCOPPIs::GetSensitiveVolumeID(const string& volName)
{
  int sensVolID = 0;
	MGLog(debugging)<<"volume name is:  "<<volName<<endlog;
  size_t found = volName.find("ctive");
  if(found == string::npos) return 0;

  G4PhysicalVolumeStore *storePtr = G4PhysicalVolumeStore::GetInstance();
  G4VPhysicalVolume* pVol = storePtr->GetVolume(volName, false);
  G4String material = pVol->GetLogicalVolume()->GetMaterial()->GetName();

    sensVolID = 100;
    MGLog(debugging) << "Germanium Crystal:" << endlog;
    MGLog(debugging)<<"found is:  "<<found<<endlog;
    if (found != string::npos){
    MGLog(debugging) << "Crystal No: " << volName.substr(4, 1) << endlog;
    sensVolID += atoi(volName.substr(4, 1).c_str()) * 1;
  }

  return sensVolID;
}

