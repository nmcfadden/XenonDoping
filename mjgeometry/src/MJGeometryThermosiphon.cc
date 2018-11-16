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
// CLASS IMPLEMENTATION:  MJGeometryThermosiphon.cc
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
 * FIRST SUBMISSION: August 11, 2010
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
#include "mjgeometry/MJGeometryThermosiphon.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"
#include "geometry/MGGeometryIdealCoaxCrystal.hh"

using namespace std;

using namespace CLHEP;
using namespace MaGe;


//---------------------------------------------------------------------------// 
 
MJGeometryThermosiphon::MJGeometryThermosiphon() 
: MGGeometryDetector("Thermosiphon"), fTemplateCrystal(0), outer_tube_log(0), inner_tube_log(0), elbow_log(0), PEEK_log(0), vetoPanel_log(0), vetoPanelcp_log(0), cryostat_log(0), inner_can_log(0), active_crystal_log(0), LN_tube_log(0), PbShield_log(0), spacer_log(0), spacer_cp_log(0), flange_log(0), spacerCube_log(0), dewar_log(0), cylinder_log(0), plate_log(0), rn_enc_log(0), CuShield_log(0), efCuShield_log(0), 
                                     outer_tube_phys(0), inner_tube_phys(0), elbow_phys(0), PEEK_phys(0), vetoPanel_phys(0), vetoPanelcp_phys(0), cryostat_phys(0), inner_can_phys(0), crystal_phys(0), LN_tube_phys(0), PbShield_phys(0), spacer_phys(0), spacer_cp_phys(0), flange_phys(0), spacerCube_phys(0), dewar_phys(0), cylinder_phys(0), plate_phys(0), rn_enc_phys(0), CuShield_phys(0), efCuShield_phys(0)
{
//  crystal = new MGGeometryIdealCoaxCrystal("Template", false);
  fTemplateCrystal = new MGGeometryIdealCoaxCrystal("Template", false);
//  fTemplateCrystal->SetDefaults();
  SetDefaults();
}

void MJGeometryThermosiphon::ConstructDetector()
{

//---------------------------------------------------------------------------// 
// Material Definitions
//---------------------------------------------------------------------------// 

  G4Material *armMaterial = G4Material::GetMaterial("Copper-EF");
  G4Material *CuMaterial = G4Material::GetMaterial("Copper-OFHC");
//  G4Material *crystalMaterial = G4Material::GetMaterial("Germanium-Enr");
  G4Material *rnMaterial = G4Material::GetMaterial("Aluminum");
//  G4Material *air = G4Material::GetMaterial("Air");
  G4Material *vac = G4Material::GetMaterial("Vacuum");
//  G4Material *nitrogen = G4Material::GetMaterial("Nitrogen-Gas");
  G4Material *lNitrogen = G4Material::GetMaterial("Nitrogen-Liq");
  G4Material *vetoMaterial = G4Material::GetMaterial("Scintillator-Pla");
  G4Material *PbMaterial = G4Material::GetMaterial("Lead-Mod");

  G4Element* elH = new G4Element("Hydrogen", "H", 1., 1.01*g/mole);
  G4Element* elO = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
  G4Element* elC = new G4Element("Carbon", "C", 6., 12.01*g/mole);

  G4double PEEK_rho = 1.32*g/cm3;

  G4Material* PEEK = new G4Material("PEEK", PEEK_rho, 3);
  PEEK->AddElement(elH, 18);
  PEEK->AddElement(elO, 3);
  PEEK->AddElement(elC, 19);

  MGLog(debugging) << "Here are the materials.\n" << endlog; 

//---------------------------------------------------------------------------// 
// Dimensions lengths and heights are 1/2 lengths and 1/2 heights!
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Thermosiphon tube dimensions
//---------------------------------------------------------------------------//

  G4double oa_length = 12.72*in;         //outer arm length
  G4double ia_length = 15.095*in;        //inner arm length  

  G4double plate_h = 0.25*in;
  G4double plate_l = 2.375*in;
  G4double plate_w = 1.5*in;

  G4double ia_inner_radius = 0.55*in;   //inner arm IR
  G4double ia_outer_radius = 0.71*in;   //inner arm OR
  G4double oa_inner_radius = 1.625*in;   //outer arm IR
  G4double oa_outer_radius = 1.75*in;   //outer arm OR

  G4double PEEK_l = 0.425*in;
  G4double PEEK_ir = ia_outer_radius;
  G4double PEEK_or = oa_inner_radius;

//---------------------------------------------------------------------------//
// Spacer Dimensions
//---------------------------------------------------------------------------//

  G4double spacer_ir = 1.87*in;  
  G4double spacer_or = 2.00*in;  
  G4double spacer_h  = 2.0285*in;   
  G4double cube_side = 4.00*in;

//---------------------------------------------------------------------------//
// Dewar Dimensions
//---------------------------------------------------------------------------//

  G4double dewar_or = 13.0*in;
  G4double dewar_ir = 12.75*in;
  G4double dewar_h = 9.92*in;

//---------------------------------------------------------------------------//
// Mysterious Cylinder Dimensions
//---------------------------------------------------------------------------//

  G4double cyl_or = 4.25*in;
  G4double cyl_ir = 4.00*in;
  G4double cyl_h = 9.92*in;

//---------------------------------------------------------------------------//
// Veto Panel Dimensions
//---------------------------------------------------------------------------//

  G4double outer_pan_h = 18.0625*in;
  G4double outer_pan_l = 41.00*in;
  G4double outer_pan_w = 1.0*in;  //panel is actually just 1.0 inch thick, but doubled thickness for two veto panels

  G4double panel_hole_r = 2.00*in;

  G4double veto_panel_z = 14.02*in - outer_pan_w;  //the distance from the center of crystat to outside of panel is 
                                                   //33.5 inches.  This setup is centered at middle of cross arm.

//---------------------------------------------------------------------------//
// Radon Enclosure Dimensions
//---------------------------------------------------------------------------//

  G4double rn_enc_h = 36.125*in;
  G4double rn_enc_l = 41.00*in;
  G4double rn_enc_w = 0.125*in;


//---------------------------------------------------------------------------//
// Regular Copper Dimensions
//---------------------------------------------------------------------------//

  G4double Cu_h = 12.00*in;
  G4double Cu_l = 12.00*in;
  G4double Cu_w = 2.5*cm;

//---------------------------------------------------------------------------//
// EF Copper Box Dimensions
//---------------------------------------------------------------------------//

  G4double efCu_h = 12.00*in;
  G4double efCu_l = 12.00*in;
  G4double efCu_w = 2.5*cm;

//---------------------------------------------------------------------------//
// Lead Shield Dimensions
//---------------------------------------------------------------------------//

  G4double Pb_h = 36.125*in;  
  G4double Pb_l = 41.00*in;  
  G4double Pb_w = 9.0*in;  

  G4double Pb_hole_r = oa_outer_radius;  

//---------------------------------------------------------------------------//
// Cryostat Dimensions
//---------------------------------------------------------------------------//

  G4double cryostat_or = 6.76*in;
  G4double cryostat_ir = 6.5*in;  
  G4double cryostat_h = 8.41255*in;  
  G4double cryostat_end_w = 0.13*in;  

//---------------------------------------------------------------------------//
// Inner Can Dimensions
//---------------------------------------------------------------------------//

  G4double inner_can_or = 6.14*in;
  G4double inner_can_ir = 6.1*in;  
  G4double inner_can_end_w = 0.04*in;  
  G4double cold_plate_w = 0.1575*in;
  G4double inner_can_h = 5.68*in;


  G4double N_length = 2.0*m; //dimensions of box

  G4double elbow_h = cube_side+1.0*ia_outer_radius;

  MGLog(debugging) << "Dimensions are set.\n" << endlog; 

//---------------------------------------------------------------------------//
// The position and subtraction placement vectors
//---------------------------------------------------------------------------//

  G4ThreeVector outer_tube_pos(0, 0, 0);
  G4ThreeVector inner_tube_pos(0, 0, -(ia_length - oa_length));
  G4ThreeVector plate_pos(0, -(ia_outer_radius + plate_h), -1.0*ia_length);
  G4ThreeVector spacer_pos(0, 0, oa_length + spacer_h);
  G4ThreeVector spacerCube_pos(0, 0, oa_length + 2.0*spacer_h + cube_side);
  G4ThreeVector dewar_pos(0, cube_side + 2.0*spacer_h + dewar_h, oa_length + dewar_or + spacer_h);
  G4ThreeVector cylinder_pos(0, -1.0*cube_side-cyl_h, oa_length + 2.0*spacer_h + cube_side);

  G4ThreeVector vetoPanel_pos(0, -1.0*outer_pan_h, veto_panel_z);
  G4ThreeVector rn_enc_pos(0,0, veto_panel_z - rn_enc_w - outer_pan_w);
  G4ThreeVector PbShield_pos(0, 0, veto_panel_z - 2.0*rn_enc_w - outer_pan_w - Pb_w);
  G4ThreeVector CuShield_pos(0, -4.0*in, veto_panel_z - 2.0*rn_enc_w - 2.0*Pb_w - 2.0*Cu_w);
  G4ThreeVector efCuShield_pos(0, -4.0*in, veto_panel_z - 2.0*rn_enc_w - 2.0*Pb_w - 2.0*Cu_w - 2.0*efCu_w);

  G4ThreeVector cryostat_pos(0, -0.6*cryostat_h, (-1.0*oa_length - cryostat_or));
  G4ThreeVector PEEK_pos(0, 0, (-1.0*oa_length-cryostat_or)+350*mm);  //position of PEEK was given to me from center of cryostat axis

  G4ThreeVector inner_can_pos(0, -1.0*(ia_outer_radius + 2.0*plate_h + inner_can_h + 2.0*cold_plate_w), (-1.0*oa_length - cryostat_or));

  G4ThreeVector crystal_pos(0, -1.0*(ia_outer_radius + inner_can_h + 2.0*cold_plate_w), (-1.0*oa_length - cryostat_or));

  G4ThreeVector elbow_pos(0, 0, oa_length+elbow_h);

  MGLog(debugging) << "Here's the placement.\n" << endlog; 

//----------------------------------------------------- 				 
// The Cavity
//----------------------------------------------------- 

  G4Box* Nbox = new G4Box("Nbox",N_length,N_length,N_length);
  theDetectorLogical = new G4LogicalVolume(Nbox, vac ,"theDetectorLogical",0,0,0);
//  theDetectorLogical = new G4LogicalVolume(Nbox, air ,"Nbox_log",0,0,0);
//  Nbox_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0), Nbox_log,"Nbox",0,false,0);

  MGLog(debugging) << "Here's the cavity.\n" << endlog; 


//---------------------------------------------------------------------------//
//  The Veto Panel
//---------------------------------------------------------------------------//

  G4Box* solidPanel = new G4Box("solidPanel", outer_pan_l, outer_pan_h, outer_pan_w);
  
  G4Tubs* panelHole = new G4Tubs("panelHole", 0.0, panel_hole_r, 2.0*outer_pan_w, 0*deg, 180*deg);

  G4RotationMatrix* zRot = new G4RotationMatrix;
  zRot->rotateZ(180.0*deg);
  G4ThreeVector yTrans(0, outer_pan_h, 0);

  G4SubtractionSolid* vetoPanel = new G4SubtractionSolid("vetoPanel", solidPanel, panelHole, zRot, yTrans);
  
  vetoPanel_log = new G4LogicalVolume(vetoPanel, vetoMaterial, "vetoPanel_log");
  vetoPanel_phys = new G4PVPlacement(0, vetoPanel_pos, vetoPanel_log, "vetoPanel", theDetectorLogical, false, 0);

  vetoPanelcp_log = new G4LogicalVolume(vetoPanel, vetoMaterial, "vetoPanelcp_log");
  vetoPanelcp_phys = new G4PVPlacement(zRot, 2.0*yTrans, vetoPanelcp_log, "vetoPanelcp_phys", vetoPanel_log, false, 1, false);

//---------------------------------------------------------------------------//
//  The Radon Enclosure
//---------------------------------------------------------------------------//

  G4Box* rn_enc_slab = new G4Box("rn_enc_slab", rn_enc_l, rn_enc_h, rn_enc_w);
  G4Tubs* rn_hole = new G4Tubs("rn_hole", 0.0, panel_hole_r, 2.0*rn_enc_w, 0*deg, 360*deg);

  G4SubtractionSolid* rn_enc = new G4SubtractionSolid("rn_enc", rn_enc_slab, rn_hole);

  rn_enc_log = new G4LogicalVolume(rn_enc, rnMaterial, "rn_enc_log");
  rn_enc_phys = new G4PVPlacement(0, rn_enc_pos, rn_enc_log, "rn_enc", theDetectorLogical, false, 0);

//---------------------------------------------------------------------------//
//  The Lead Shield
//---------------------------------------------------------------------------//

  G4Box* PbSlab = new G4Box("PbSlab", Pb_l, Pb_h, Pb_w);
  G4Tubs* PbHole = new G4Tubs("PbHole", 0.0, Pb_hole_r, 2.0*Pb_w, 0*deg, 360*deg);

  G4SubtractionSolid* PbShield = new G4SubtractionSolid("PbShield", PbSlab, PbHole);
  
  PbShield_log = new G4LogicalVolume(PbShield, PbMaterial, "PbShield_log");
  PbShield_phys = new G4PVPlacement(0, PbShield_pos, PbShield_log, "PbShield", theDetectorLogical, false, 0);

//---------------------------------------------------------------------------//
//  The Copper Shield
//---------------------------------------------------------------------------//

  G4Box* CuSlab = new G4Box("CuSlab", Cu_l, Cu_h, Cu_w);
  G4Tubs* CuHole = new G4Tubs("CuHole", 0.0, panel_hole_r, 2.0*Cu_w, 0*deg, 360*deg);

  G4ThreeVector yTrans0(0, 4.0*in, 0);

  G4SubtractionSolid* CuShield = new G4SubtractionSolid("CuShield", CuSlab, CuHole, 0, yTrans0);
  
  CuShield_log = new G4LogicalVolume(CuShield, CuMaterial, "CuShield_log");
  CuShield_phys = new G4PVPlacement(0, CuShield_pos, CuShield_log, "CuShield", theDetectorLogical, false, 0);

  G4Box* efCuSlab = new G4Box("efCuSlab", efCu_l, efCu_h, efCu_w);
  G4Tubs* efCuHole = new G4Tubs("efCuHole", 0.0, panel_hole_r, 2.0*efCu_w, 0*deg, 360*deg);

  G4SubtractionSolid* efCuShield = new G4SubtractionSolid("efCuShield", efCuSlab, efCuHole, 0, yTrans0);
  
  efCuShield_log = new G4LogicalVolume(efCuShield, armMaterial, "efCuShield_log");
  efCuShield_phys = new G4PVPlacement(0, efCuShield_pos, efCuShield_log, "efCuShield", theDetectorLogical, false, 0);

//---------------------------------------------------------------------------//
//  The Cryostat
//---------------------------------------------------------------------------//

  G4Tubs* cryostatEnd = new G4Tubs("cryostatTop", 0.0, cryostat_or, cryostat_end_w, 0*deg, 360*deg);
  G4Tubs* cryostatEnd2 = new G4Tubs("cryostatTop2", 0.0, cryostat_or, cryostat_end_w, 0*deg, 360*deg);
  G4Tubs* cryostatSide0 = new G4Tubs("cryostatSide0", cryostat_ir, cryostat_or, cryostat_h, 0*deg, 360*deg);

  G4Tubs* cryoHole = new G4Tubs("cryoHole", 0, oa_inner_radius, 0.5*in, 0*deg, 360*deg);

  G4ThreeVector zTrans2(0, 0, cryostat_h + cryostat_end_w);
  G4ThreeVector transHole(0, -cryostat_or, (-ia_outer_radius + inner_can_h + 0.5*cold_plate_w));

  G4RotationMatrix* cryoRot = new G4RotationMatrix;
  cryoRot->rotateX(90.0*deg);

  G4SubtractionSolid* cryostatSide = new G4SubtractionSolid("cryostatSide", cryostatSide0, cryoHole, cryoRot, transHole);

  G4UnionSolid* cryostat1 = new G4UnionSolid("cryostat1", cryostatSide, cryostatEnd, 0, 1.0*zTrans2);
  G4UnionSolid* cryostat = new G4UnionSolid("cryostat", cryostat1, cryostatEnd2, 0, -1.0*zTrans2);

  cryostat_log = new G4LogicalVolume(cryostat, armMaterial, "cryostat_log");
  cryostat_phys = new G4PVPlacement(cryoRot, cryostat_pos, cryostat_log, "cryostat", theDetectorLogical, false, 0);

//  G4Tubs* cryoFill = new G4Tubs("cryoFill", 0.0, cryostat_ir, cryostat_h, 0*deg, 360*deg);
//  cryoFill_log = new G4LogicalVolume(cryoFill, vac, "cryoFill_log");
//  cryoFill_phys = new G4PVPlacement(cryoRot, cryostat_pos, cryoFill_log, "cryoFill", theDetectorLogical, false, 0);


//---------------------------------------------------------------------------//
//  The Cryostat Inner Can
//---------------------------------------------------------------------------//

  G4Tubs* innerCanTop = new G4Tubs("innerCanTop", 0.0, inner_can_or, cold_plate_w, 0*deg, 360*deg);
  G4Tubs* innerCanEnd = new G4Tubs("innerCanTop2", 0.0, inner_can_or, inner_can_end_w, 0*deg, 360*deg);
  G4Tubs* innerCanSide = new G4Tubs("innerCanSide", inner_can_ir, inner_can_or, inner_can_h, 0*deg, 360*deg);

  G4ThreeVector zTrans3(0, 0, inner_can_h + cold_plate_w);
  G4ThreeVector zTrans4(0, 0, inner_can_h + inner_can_end_w);
  G4RotationMatrix* innerCanRot = new G4RotationMatrix;
  innerCanRot->rotateX(90.0*deg);

  G4UnionSolid* inner_can1 = new G4UnionSolid("inner_can1", innerCanSide, innerCanTop, 0, 1.0*zTrans3);
  G4UnionSolid* inner_can = new G4UnionSolid("inner_can", inner_can1, innerCanEnd, 0, -1.0*zTrans4);

  inner_can_log = new G4LogicalVolume(inner_can, armMaterial, "inner_can_log");
  inner_can_phys = new G4PVPlacement(innerCanRot, inner_can_pos, inner_can_log, "inner_can", theDetectorLogical, false, 0);

//----------------------------------------------------- 				 
// The Crystal
//----------------------------------------------------- 

  G4RotationMatrix* crystalRot = new G4RotationMatrix;
  crystalRot->rotateX(90.0*deg);
  fTemplateCrystal->Dump();
  fTemplateCrystal->ConstructDetector();
  
  MGLog(debugging) << "The Crystal was made.\n" << endlog;  

//  active_crystal_log = ;  

  crystal_phys = new G4PVPlacement(crystalRot, crystal_pos, fTemplateCrystal->GetDetectorLogical(), "crystal_phys", theDetectorLogical, false, 0);

//------------------------------------
//  Arm tubes
//------------------------------------

  G4Tubs* outer_tube = new G4Tubs("outer_tube", oa_inner_radius, oa_outer_radius, oa_length, 0*deg, 360*deg);
  outer_tube_log = new G4LogicalVolume(outer_tube, armMaterial, "outer_tube_log", 0, 0, 0);
  outer_tube_phys = new G4PVPlacement(0, outer_tube_pos, outer_tube_log, "outer_tube", theDetectorLogical, false, 0);
  MGLog(debugging) << "Outer tube.\n" << endlog; 

  G4Tubs* inner_tube = new G4Tubs("inner_tube", ia_inner_radius, ia_outer_radius, ia_length, 0*deg, 360*deg);
  inner_tube_log = new G4LogicalVolume(inner_tube, armMaterial, "inner_tube_log", 0, 0, 0);
  inner_tube_phys = new G4PVPlacement(0, inner_tube_pos, inner_tube_log, "inner_tube", theDetectorLogical, false, 0);

  G4Tubs* LN_tube = new G4Tubs("LN_tube", 0, ia_inner_radius, ia_length, 0*deg, 360*deg);
  LN_tube_log = new G4LogicalVolume(LN_tube, lNitrogen, "LN_tube_log", 0, 0, 0);
  LN_tube_phys = new G4PVPlacement(0, inner_tube_pos, LN_tube_log, "LN_tube", theDetectorLogical, false, 0);

  G4RotationMatrix* peekRot = new G4RotationMatrix;
  peekRot->rotateZ(97.5*deg);

  G4Tubs* PEEK_tube = new G4Tubs("PEEK_tube", PEEK_ir, PEEK_or, PEEK_l, 0*deg, 15*deg);
  PEEK_log = new G4LogicalVolume(PEEK_tube, PEEK,  "PEEK_log", 0, 0, 0);
  PEEK_phys = new G4PVPlacement(peekRot, PEEK_pos, PEEK_log, "PEEK_tube", theDetectorLogical, false, 0);

  MGLog(debugging) << "Inner tube.\n" << endlog; 

//------------------------------------
//  Elbow
//------------------------------------

  G4Tubs* elbow_1 = new G4Tubs("elbow_1", ia_inner_radius, ia_outer_radius, elbow_h, 0*deg, 360*deg);
  G4Tubs* elbow_2 = new G4Tubs("elbow_2", ia_inner_radius, ia_outer_radius, cube_side+1.8*ia_outer_radius, 0*deg, 360*deg);

  G4Box* slicer = new G4Box("slicer", 2.5*ia_outer_radius, 2.5*ia_outer_radius, 2.5*ia_outer_radius);

  G4RotationMatrix* slicerRot1 = new G4RotationMatrix;
  G4RotationMatrix* slicerRot2 = new G4RotationMatrix;
  slicerRot1->rotateX(45*deg);
  slicerRot2->rotateX(135*deg);
  G4ThreeVector transElbow1(0, ia_outer_radius, cube_side+1.5*ia_outer_radius);
  G4ThreeVector transElbow2(0, ia_outer_radius, -cube_side-0.8*ia_outer_radius);

  G4SubtractionSolid* elbow_3 = new G4SubtractionSolid("elbow_3", elbow_1, slicer, slicerRot1, transElbow1);
  G4SubtractionSolid* elbow_4 = new G4SubtractionSolid("elbow_4", elbow_2, slicer, slicerRot2, transElbow2);

  G4RotationMatrix* elbowRot = new G4RotationMatrix;
  elbowRot->rotateX(90*deg);
  G4ThreeVector transElbow3(0, cube_side-1.75*ia_outer_radius, cube_side-ia_outer_radius);

  G4UnionSolid* elbow = new G4UnionSolid("elbow", elbow_3, elbow_4, elbowRot, transElbow3);

  elbow_log = new G4LogicalVolume(elbow, armMaterial, "elbow_log");
  elbow_phys = new G4PVPlacement(0, elbow_pos, elbow_log, "elbow", theDetectorLogical, false, 0);


//------------------------------------
//  Plate between inner arm and coldplate
//------------------------------------

  G4Box* plate = new G4Box("plate", plate_w, plate_h, plate_l);
  plate_log = new G4LogicalVolume(plate, armMaterial, "plate_log", 0, 0, 0);
  plate_phys = new G4PVPlacement(0, plate_pos, plate_log, "plate", theDetectorLogical, false, 0);

  MGLog(debugging) << "Plate.\n" << endlog; 

//------------------------------------
//  Spacers
//------------------------------------

  G4Tubs* spacer = new G4Tubs("spacer", spacer_ir, spacer_or, spacer_h, 0*deg, 360*deg);
  spacer_log = new G4LogicalVolume(spacer, armMaterial, "spacer_log", 0, 0, 0);
  spacer_phys = new G4PVPlacement(0, spacer_pos, spacer_log, "spacer_phys", theDetectorLogical, false, 0);

  G4ThreeVector yTrans2(0, cube_side + spacer_h, spacer_h + cube_side);

  spacer_cp_log = new G4LogicalVolume(spacer, armMaterial, "spacer_cp_log");
  spacer_cp_phys = new G4PVPlacement(cryoRot, yTrans2, spacer_cp_log, "spacer_cp_phys", spacer_log, false, 1, false);

  MGLog(debugging) << "Spacer.\n" << endlog; 

//  G4Tubs* flange = new G4Tubs("flange", spacer_ir, spacer_or, spacer_h, 0*deg, 360*deg);
//  flange_log = new G4LogicalVolume(flange, armMaterial, "flange_log", 0, 0, 0);
//  flange_phys = new G4PVPlacement(0, spacer_pos, flange_log, "flange_phys", theDetectorLogical, false, 0);

//------------------------------------
//  Breakout Box
//------------------------------------

  G4Box* spacerCube1 = new G4Box("spacerCube1", cube_side, cube_side, cube_side);
  G4Box* spacerCube2 = new G4Box("spacerCube2", cube_side-0.25*in, cube_side-0.25*in, cube_side-0.25*in);

  G4Tubs* breakoutHole1 = new G4Tubs("breakoutHole1", 0, oa_inner_radius, 0.5*in, 0*deg, 360*deg);
  G4Tubs* breakoutHole2 = new G4Tubs("breakoutHole2", 0, oa_inner_radius, 0.5*in, 0*deg, 360*deg);

  G4ThreeVector zTrans5(0, 0, -1.0*cube_side);
  G4ThreeVector zTrans6(0, 1.0*cube_side, 0);

  G4RotationMatrix* holeRot = new G4RotationMatrix;
  holeRot->rotateX(90*deg);

  G4SubtractionSolid* spacerCube3 = new G4SubtractionSolid("spacerCube3", spacerCube1, spacerCube2, 0, G4ThreeVector());
  G4SubtractionSolid* spacerCube4 = new G4SubtractionSolid("spacerCube4", spacerCube3, breakoutHole1, 0, zTrans5);
  G4SubtractionSolid* spacerCube = new G4SubtractionSolid("spacerCube", spacerCube4, breakoutHole2, holeRot, zTrans6);

  spacerCube_log = new G4LogicalVolume(spacerCube, armMaterial, "spacerCube_log", 0, 0, 0);
  spacerCube_phys = new G4PVPlacement(0, spacerCube_pos, spacerCube_log, "spacerCube", theDetectorLogical, false, 0);

  MGLog(debugging) << "Spacer cube.\n" << endlog; 

//------------------------------------
//  Dewar
//------------------------------------
 
  G4Tubs* dewar = new G4Tubs("dewar", dewar_ir, dewar_or, dewar_h, 0*deg, 360*deg);
  dewar_log = new G4LogicalVolume(dewar, armMaterial, "dewar_log", 0, 0, 0);
  dewar_phys = new G4PVPlacement(cryoRot, dewar_pos, dewar_log, "dewar", theDetectorLogical, false, 0);

  MGLog(debugging) << "Dewar.\n" << endlog; 

//------------------------------------
//  Cylinder
//------------------------------------
 
  G4Tubs* cylinder = new G4Tubs("cylinder", cyl_ir, cyl_or, cyl_h, 0*deg, 360*deg);
  cylinder_log = new G4LogicalVolume(cylinder, armMaterial, "cylinder_log", 0, 0, 0);
  cylinder_phys = new G4PVPlacement(cryoRot, cylinder_pos, cylinder_log, "cylinder", theDetectorLogical, false, 0);

  MGLog(debugging) << "Cylinder.\n" << endlog; 

//--------- Visualization attributes -------------------------------

//  G4VisAttributes* ClearVisAtt= new G4VisAttributes(SetVisibility(false)); // clear?
  theDetectorLogical->SetVisAttributes(G4VisAttributes::GetInvisible());

  MGLog(debugging) << "Set Nbox clear.\n" << endlog; 
  
  G4VisAttributes* YellowVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0)); // yellow
  outer_tube_log->SetVisAttributes(YellowVisAtt);

  MGLog(debugging) << "Set yellow color.\n" << endlog; 

  G4VisAttributes* GreyVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // grey
  GreyVisAtt->SetForceWireframe( true );
  cryostat_log->SetVisAttributes(GreyVisAtt);
  PbShield_log->SetVisAttributes(GreyVisAtt);

  MGLog(debugging) << "Set grey color.\n" << endlog; 

  G4VisAttributes* GreenVisAtt = new G4VisAttributes(G4Colour(0.0,1,0.0)); // green
//  GreenVisAtt->SetForceWireframe( true );
  inner_tube_log->SetVisAttributes(GreenVisAtt);
  spacer_log->SetVisAttributes(GreenVisAtt);
  spacer_cp_log->SetVisAttributes(GreenVisAtt);
  rn_enc_log->SetVisAttributes(GreenVisAtt);

  MGLog(debugging) << "Set green color.\n" << endlog; 

  G4VisAttributes* BlueVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0)); // blue
  dewar_log->SetVisAttributes(BlueVisAtt);
  theDetectorLogical->SetVisAttributes(BlueVisAtt);  
  LN_tube_log->SetVisAttributes(BlueVisAtt);

  MGLog(debugging) << "Set blue color.\n" << endlog; 

}

void MJGeometryThermosiphon::SetDefaults()
{
  G4double crystal_r = 4.72*in;
  G4double crystal_h = 9.5224*in;  

  fTemplateCrystal->SetHeight(crystal_h);
  fTemplateCrystal->SetRadius(crystal_r);
  fTemplateCrystal->SetIsActive(true);
  fTemplateCrystal->SetIsOneOfMany(false);
  fTemplateCrystal->SetG4MaterialName("Germanium-Enr");

  MGLog(debugging) << "Defaults Set.\n" << endlog; 

}
