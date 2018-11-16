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
#include "G4RotationMatrix.hh"

// MJ headers
#include "mjgeometry/MJGeometryVeto.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"
#include "geometry/MGGeometryIdealCoaxCrystal.hh"

using namespace std;

using namespace CLHEP;
using namespace MaGe;


//---------------------------------------------------------------------------// 
 
MJGeometryVeto::MJGeometryVeto() 
: MGGeometryDetector("Veto"), vetoPanelFB_log(0), vetoPanelFL_log(0), vetoPanelLSB_log(0), vetoPanelLSL_log(0), vetoPanelFT_log(0), vetoPanelFR_log(0), vetoPanelLST_log(0), vetoPanelLSR_log(0), solidPanelBB_log(0), solidPanelBT_log(0), solidPanelBL_log(0), solidPanelBR_log(0), solidPanelRSB_log(0), solidPanelRST_log(0), solidPanelRSL_log(0), solidPanelRSR_log(0), solidPanelTIL_log(0), solidPanelTIR_log(0), solidPanelTOF_log(0), solidPanelTOB_log(0), overfloorTop_log(0), overfloorBottom_log(0), vetoPanelFB_phys(0), vetoPanelFT_phys(0), vetoPanelFL_phys(0), vetoPanelFR_phys(0), vetoPanelLSB_phys(0), vetoPanelLST_phys(0), vetoPanelLSL_phys(0), vetoPanelLSR_phys(0), solidPanelBB_phys(0), solidPanelBT_phys(0), solidPanelBL_phys(0), solidPanelBR_phys(0), solidPanelRSB_phys(0), solidPanelRST_phys(0), solidPanelRSL_phys(0), solidPanelRSR_phys(0), 
solidPanelTIL_phys(0), solidPanelTIR_phys(0), solidPanelTOF_phys(0), solidPanelTOB_phys(0), overfloorTop_phys(0), overfloorBottom_phys(0)
{}

void MJGeometryVeto::ConstructDetector()
{

//---------------------------------------------------------------------------// 
// Material Definitions
//---------------------------------------------------------------------------// 

//  G4Material *air = G4Material::GetMaterial("Air");
  G4Material *vac = G4Material::GetMaterial("Vacuum");
  G4Material *vetoMaterial = G4Material::GetMaterial("Scintillator-Pla");

//---------------------------------------------------------------------------// 
// Dimensions lengths and heights are 1/2 lengths and 1/2 heights!
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Veto Panel Dimensions
//---------------------------------------------------------------------------//

//  G4double total_h = 72.250*in;

//  G4double diff;

  G4double op_fr_h[2], op_fr_l[2], op_fr_w[2], ip_fr_h[2], ip_fr_l[2], ip_fr_w[2];
  G4double op_ls_h[2], op_ls_l[2], op_ls_w[2], ip_ls_h[2], ip_ls_l[2], ip_ls_w[2];

  G4double op_tp_h[2], op_tp_l[2], op_tp_w[2], ip_tp_h[2], ip_tp_l[2], ip_tp_w[2];
  G4double overfloor_l, overfloor_h, overfloor_w, overfloor_eps;

  op_fr_h[0] = 14.975*in;  //op = outer panel, fr = front, 0 = top, 1 = bottom
  op_fr_l[0] = 35.5*in;
  op_fr_w[0] = 0.5*in;  //panel is 1.0 inch thick

  op_fr_h[1] = 18.65*in;
  op_fr_l[1] = 35.5*in;
  op_fr_w[1] = 0.5*in;

  ip_fr_h[0] = 17.75*in;   //ip = inner panel, 0 = left, 1 = right
  ip_fr_l[0] = 35.125*in;
  ip_fr_w[0] = 0.5*in;

  ip_fr_h[1] = 17.75*in;
  ip_fr_l[1] = 35.125*in;
  ip_fr_w[1] = 0.5*in;

  op_ls_h[0] = op_fr_h[0];
  op_ls_l[0] = 40.5*in;
  op_ls_w[0] = 0.5*in;

  op_ls_h[1] = op_fr_h[1];
  op_ls_l[1] = 40.5*in;
  op_ls_w[1] = 0.5*in;

  ip_ls_h[0] = 16.75*in;   //ip = inner panel, 0 = left, 1 = right
  ip_ls_l[0] = 35.125*in;
  ip_ls_w[0] = 0.5*in;

  ip_ls_h[1] = 23.75*in;
  ip_ls_l[1] = 35.125*in;
  ip_ls_w[1] = 0.5*in;

//top panels

  op_tp_h[0] = 20.5*in;
  op_tp_l[0] = op_fr_l[0];
  op_tp_w[0] = 0.5*in;

  op_tp_h[1] = op_tp_h[0];
  op_tp_l[1] = op_tp_l[0];
  op_tp_w[1] = op_tp_w[0];

  ip_tp_h[0] = 17.75*in;   //ip = inner panel, 0 = left, 1 = right
  ip_tp_l[0] = 42.5*in;
  ip_tp_w[0] = 0.5*in;

  ip_tp_h[1] = ip_tp_h[0];
  ip_tp_l[1] = ip_tp_l[0];
  ip_tp_w[1] = ip_tp_w[0];

//overfloor panels

  overfloor_l = 48.5*in;
  overfloor_h = 6.85*in;
  overfloor_w = 0.5*in;
  overfloor_eps = 0.3*in;

  G4double panel_hole_r = 2.00*in;

//---------------------------------------------------------------------------//
// Position Vectors
//---------------------------------------------------------------------------//
  G4ThreeVector FBOutPos, FTOutPos, FLOutPos, FROutPos, LSTOutPos, LSBOutPos, LSLOutPos, LSROutPos;
  G4ThreeVector BBOutPos, BTOutPos, BLOutPos, BROutPos, RSTOutPos, RSBOutPos, RSLOutPos, RSROutPos;

  G4ThreeVector TLOutPos, TROutPos, TFOutPos, TBOutPos;
  G4ThreeVector overfloorTop_pos[6], overfloorBottom_pos[6];
  int copyno;

  FBOutPos = G4ThreeVector(0, -op_fr_h[1], op_ls_l[1] + 3.0*op_fr_w[1]);
  FTOutPos = G4ThreeVector(0,  op_fr_h[0], op_ls_l[0] + 3.0*op_fr_w[0]);

  FLOutPos = G4ThreeVector(-ip_fr_h[0], -4*op_fr_w[1], op_ls_l[1] + 1.0*op_fr_w[1]);
  FROutPos = G4ThreeVector(ip_fr_h[0],  -4*op_fr_w[1], op_ls_l[0] + 1.0*op_fr_w[0]);

  LSBOutPos = G4ThreeVector(-op_fr_l[1]+op_fr_w[1], -op_ls_h[1], 0);
  LSTOutPos = G4ThreeVector(-op_fr_l[0]+op_fr_w[0],  op_ls_h[0], 0);

  LSLOutPos = G4ThreeVector(-op_fr_l[1]+3.0*op_fr_w[1], -4.0*op_fr_w[1], -ip_ls_h[1]);
  LSROutPos = G4ThreeVector(-op_fr_l[0]+3.0*op_fr_w[0], -4.0*op_fr_w[1], ip_ls_h[0]);

  BBOutPos = G4ThreeVector(0, -op_fr_h[1], -(op_ls_l[1] + 3.0*op_fr_w[1]));
  BTOutPos = G4ThreeVector(0,  op_fr_h[0], -(op_ls_l[0] + 3.0*op_fr_w[0]));

  BLOutPos = G4ThreeVector(-ip_fr_h[0], -4*op_fr_w[1], -(op_ls_l[1] + 1.0*op_fr_w[1]));
  BROutPos = G4ThreeVector(ip_fr_h[0],  -4*op_fr_w[1], -(op_ls_l[0] + 1.0*op_fr_w[0]));

  RSBOutPos = G4ThreeVector(op_fr_l[1] - op_fr_w[1], -op_ls_h[1], 0);
  RSTOutPos = G4ThreeVector(op_fr_l[0] - op_fr_w[0],  op_ls_h[0], 0);

  RSLOutPos = G4ThreeVector(op_fr_l[1] - 3.0*op_fr_w[1], -4.0*op_fr_w[1], -ip_ls_h[1]);
  RSROutPos = G4ThreeVector(op_fr_l[0] - 3.0*op_fr_w[0], -4.0*op_fr_w[1], ip_ls_h[0]);

  TLOutPos = G4ThreeVector(-ip_tp_h[0], ip_fr_l[0]-3.0*op_fr_w[0], 0);
  TROutPos = G4ThreeVector(ip_tp_h[1], ip_fr_l[1]-3.0*op_fr_w[0], 0);
  TFOutPos = G4ThreeVector(0, ip_fr_l[0]-op_fr_w[0], op_tp_h[0]-1.5*in);
  TBOutPos = G4ThreeVector(0, ip_fr_l[0]-op_fr_w[0], -op_tp_h[0]-1.5*in);

  overfloorTop_pos[0] = G4ThreeVector(2.0*0*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps), -2.0*op_fr_h[1]-1.66*in, 0);
  overfloorTop_pos[1] = G4ThreeVector(2.0*1*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps), -2.0*op_fr_h[1]-1.66*in, 0);
  overfloorTop_pos[2] = G4ThreeVector(2.0*2*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps), -2.0*op_fr_h[1]-1.66*in, 0);
  overfloorTop_pos[3] = G4ThreeVector(2.0*3*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps), -2.0*op_fr_h[1]-1.66*in, 0);
  overfloorTop_pos[4] = G4ThreeVector(2.0*4*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps), -2.0*op_fr_h[1]-1.66*in, 0);
  overfloorTop_pos[5] = G4ThreeVector(2.0*5*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps), -2.0*op_fr_h[1]-1.66*in, 0);

  overfloorBottom_pos[0] = G4ThreeVector(0, -2.0*op_fr_h[1]-1.66*in-8.0*in, 2.0*0*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps));
  overfloorBottom_pos[1] = G4ThreeVector(0, -2.0*op_fr_h[1]-1.66*in-8.0*in, 2.0*1*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps));
  overfloorBottom_pos[2] = G4ThreeVector(0, -2.0*op_fr_h[1]-1.66*in-8.0*in, 2.0*2*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps));
  overfloorBottom_pos[3] = G4ThreeVector(0, -2.0*op_fr_h[1]-1.66*in-8.0*in, 2.0*3*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps));
  overfloorBottom_pos[4] = G4ThreeVector(0, -2.0*op_fr_h[1]-1.66*in-8.0*in, 2.0*4*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps));
  overfloorBottom_pos[5] = G4ThreeVector(0, -2.0*op_fr_h[1]-1.66*in-8.0*in, 2.0*5*(overfloor_h + overfloor_eps)-5.0*(overfloor_h + overfloor_eps));

//----------------------------------------------------- 				 
// The Cavity
//----------------------------------------------------- 

  G4Box* Nbox = new G4Box("Nbox", 50.0*m, 50.0*m, 50.0*m);
  theDetectorLogical = new G4LogicalVolume(Nbox, vac ,"theDetectorLogical",0,0,0);

  MGLog(debugging) << "Here's the cavity.\n" << endlog; 

//---------------------------------------------------------------------------//
//  Rotation Matrices
//---------------------------------------------------------------------------//

  G4RotationMatrix* zRot = new G4RotationMatrix;
  zRot->rotateZ(180.0*deg);

  G4RotationMatrix* yRot = new G4RotationMatrix;
  yRot->rotateY(90.0*deg);

//---------------------------------------------------------------------------//
//  The Veto Front Panels (i.e. the smaller side)
//---------------------------------------------------------------------------//

  G4Box* solidPanelFrontBottom = new G4Box("solidPanelFrontBottom", op_fr_l[1], op_fr_h[1], op_fr_w[1]);
  G4Box* solidPanelFrontTop    = new G4Box("solidPanelFrontTop",    op_fr_l[0], op_fr_h[0], op_fr_w[0]);

  G4Box* solidPanelFrontLeft = new G4Box("solidPanelFrontLeft", ip_fr_h[0], ip_fr_l[0], ip_fr_w[0]);
  G4Box* solidPanelFrontRight = new G4Box("solidPanelFrontRight", ip_fr_h[1], ip_fr_l[1], ip_fr_w[1]);
  
  G4Box* solidPanelLSBottom = new G4Box("solidPanelLSBottom", op_ls_w[1], op_ls_h[1], op_ls_l[1]);
  G4Box* solidPanelLSTop    = new G4Box("solidPanelLSTop",    op_ls_w[0], op_ls_h[0], op_ls_l[0]);

  G4Box* solidPanelLSLeft   = new G4Box("solidPanelLSLeft", ip_ls_w[0], ip_ls_l[0], ip_ls_h[0]);
  G4Box* solidPanelLSRight  = new G4Box("solidPanelLSRight",ip_ls_w[1], ip_ls_l[1], ip_ls_h[1]);

  G4Box* solidPanelInnerLeft = new G4Box("solidPanelInnerLeft", ip_tp_h[0], ip_tp_w[0], ip_tp_l[0]);
  G4Box* solidPanelInnerRight = new G4Box("solidPanelInnerRight", ip_tp_h[1], ip_tp_w[1], ip_tp_l[1]);

  G4Box* solidPanelOuterFront = new G4Box("solidPanelOuterFront", op_tp_l[0], op_tp_w[0], op_tp_h[0]);
  G4Box* solidPanelOuterBack = new G4Box("solidPanelOuterBack", op_tp_l[1], op_tp_w[1], op_tp_h[1]);

  G4Tubs* panelHole = new G4Tubs("panelHole", 0.0, panel_hole_r, 2.0*op_fr_w[0], 0*deg, 360*deg);
  G4Box* cutOutPanel = new G4Box("cutOutPanel", 16.625*in, 2.125*in, 2.0*0.5*in);

  //position of holes
  G4ThreeVector op_bottom(0, op_fr_h[1], 0);
  G4ThreeVector op_top(0, -1.0*op_fr_h[0], 0);

  G4ThreeVector ip_left(ip_fr_h[0], 4.0*op_fr_w[1], 0);
  G4ThreeVector ip_right(-ip_fr_h[1], 4.0*op_fr_w[1], 0);

  G4ThreeVector op_ls_bottom(0, op_ls_h[1],   -2.0*ip_ls_h[1] + op_ls_l[0]);
  G4ThreeVector op_ls_top(0, -1.0*op_ls_h[0], -2.0*ip_ls_h[1] + op_ls_l[0]);

  G4ThreeVector ip_ls_left(0,  4.0*op_fr_w[1],  ip_ls_h[0]);
  G4ThreeVector ip_ls_right(0, 4.0*op_fr_w[1], -ip_ls_h[1]);

  //position of cutouts
  G4RotationMatrix *cutOutRotZ = new G4RotationMatrix();
  cutOutRotZ->rotateZ(90*deg);

  G4RotationMatrix *cutOutRotY = new G4RotationMatrix();
  cutOutRotY->rotateY(90*deg);


  G4ThreeVector cutOutPosFB(0, -op_fr_h[1], 0);
  G4ThreeVector cutOutPosFL(ip_fr_h[0], -ip_fr_l[0], 0);
  G4ThreeVector cutOutPosFR(-ip_fr_h[1], -ip_fr_l[1], 0);
  G4ThreeVector cutOutPosLSB(0, -op_ls_h[1], -2.0*ip_ls_h[1] + op_ls_l[0]);
  G4ThreeVector cutOutPosLSL(0, -ip_ls_l[0],  ip_ls_h[0]);
  G4ThreeVector cutOutPosLSR(0, -ip_ls_l[1], -ip_ls_h[1]);
  //creating front panels

  //outer front
  G4SubtractionSolid* vetoPanelFrontBottom1 = new G4SubtractionSolid("vetoPanelFrontBottom1", solidPanelFrontBottom, panelHole, 0, op_bottom);
  G4SubtractionSolid* vetoPanelFrontBottom = new G4SubtractionSolid("vetoPanelFrontBottom", vetoPanelFrontBottom1, cutOutPanel, 0, cutOutPosFB);

  G4SubtractionSolid* vetoPanelFrontTop = new G4SubtractionSolid("vetoPanelFrontTop", solidPanelFrontTop, panelHole, 0, op_top);

  //inner front
  G4SubtractionSolid* vetoPanelFrontLeft1 = new G4SubtractionSolid("vetoPanelFrontLeft1", solidPanelFrontLeft, panelHole, 0, ip_left);
  G4SubtractionSolid* vetoPanelFrontLeft = new G4SubtractionSolid("vetoPanelFrontLeft", vetoPanelFrontLeft1, cutOutPanel, 0, cutOutPosFL);


  G4SubtractionSolid* vetoPanelFrontRight1 = new G4SubtractionSolid("vetoPanelFrontRight1", solidPanelFrontRight, panelHole, 0, ip_right);
  G4SubtractionSolid* vetoPanelFrontRight = new G4SubtractionSolid("vetoPanelFrontRight", vetoPanelFrontRight1, cutOutPanel, 0, cutOutPosFR);


  //creating left side panels

  //outer left side
  G4SubtractionSolid* vetoPanelLSBottom1 = new G4SubtractionSolid("vetoPanelLSBottom1", solidPanelLSBottom, panelHole, yRot, op_ls_bottom);
  G4SubtractionSolid* vetoPanelLSBottom = new G4SubtractionSolid("vetoPanelLSBottom", vetoPanelLSBottom1, cutOutPanel, cutOutRotY, cutOutPosLSB);

  G4SubtractionSolid* vetoPanelLSTop = new G4SubtractionSolid("vetoPanelLSTop", solidPanelLSTop, panelHole, yRot, op_ls_top);

  //inner left side
  G4SubtractionSolid* vetoPanelLSLeft1 = new G4SubtractionSolid("vetoPanelLSLeft1", solidPanelLSLeft, panelHole, yRot, ip_ls_left);
  G4SubtractionSolid* vetoPanelLSLeft = new G4SubtractionSolid("vetoPanelLSLeft", vetoPanelLSLeft1, cutOutPanel, cutOutRotY, cutOutPosLSL);

  G4SubtractionSolid* vetoPanelLSRight1 = new G4SubtractionSolid("vetoPanelLSRight1", solidPanelLSRight, panelHole, yRot, ip_ls_right);
  G4SubtractionSolid* vetoPanelLSRight = new G4SubtractionSolid("vetoPanelLSRight", vetoPanelLSRight1, cutOutPanel, cutOutRotY, cutOutPosLSR);


  //overfloor panels

  G4Box* overfloorTop = new G4Box("overfloorTop", overfloor_h, overfloor_w, overfloor_l);
  G4Box* overfloorBottom = new G4Box("overfloorBottom", overfloor_l, overfloor_w, overfloor_h);

  //logical volumes

  vetoPanelFB_log = new G4LogicalVolume(vetoPanelFrontBottom, vetoMaterial, "vetoPanelFB_log");
  vetoPanelFT_log = new G4LogicalVolume(vetoPanelFrontTop, vetoMaterial, "vetoPanelFT_log");

  vetoPanelFL_log = new G4LogicalVolume(vetoPanelFrontLeft, vetoMaterial, "vetoPanelFL_log");
  vetoPanelFR_log = new G4LogicalVolume(vetoPanelFrontRight, vetoMaterial, "vetoPanelFR_log");

  vetoPanelLST_log = new G4LogicalVolume(vetoPanelLSTop, vetoMaterial, "vetoPanelLST_log");
  vetoPanelLSB_log = new G4LogicalVolume(vetoPanelLSBottom, vetoMaterial, "vetoPanelLSB_log");

  vetoPanelLSL_log = new G4LogicalVolume(vetoPanelLSLeft, vetoMaterial, "vetoPanelLSL_log");
  vetoPanelLSR_log = new G4LogicalVolume(vetoPanelLSRight, vetoMaterial, "vetoPanelLSR_log");

  solidPanelBT_log = new G4LogicalVolume(solidPanelFrontTop, vetoMaterial, "solidPanelBT_log");
  solidPanelBB_log = new G4LogicalVolume(solidPanelFrontBottom, vetoMaterial, "solidPanelBB_log");

  solidPanelBL_log = new G4LogicalVolume(solidPanelFrontLeft, vetoMaterial, "solidPanelBL_log");
  solidPanelBR_log = new G4LogicalVolume(solidPanelFrontRight, vetoMaterial, "solidPanelBR_log");

  solidPanelRST_log = new G4LogicalVolume(solidPanelLSTop, vetoMaterial, "solidPanelRST_log");
  solidPanelRSB_log = new G4LogicalVolume(solidPanelLSBottom, vetoMaterial, "solidPanelRSB_log");

  solidPanelRSL_log = new G4LogicalVolume(solidPanelLSLeft, vetoMaterial, "solidPanelRSL_log");
  solidPanelRSR_log = new G4LogicalVolume(solidPanelLSRight, vetoMaterial, "solidPanelRSR_log");

  solidPanelTIL_log = new G4LogicalVolume(solidPanelInnerLeft, vetoMaterial, "solidPanelTIL_log");
  solidPanelTIR_log = new G4LogicalVolume(solidPanelInnerRight, vetoMaterial, "solidPanelTIR_log");

  solidPanelTOF_log = new G4LogicalVolume(solidPanelOuterFront, vetoMaterial, "solidPanelTOF_log");
  solidPanelTOB_log = new G4LogicalVolume(solidPanelOuterBack, vetoMaterial, "solidPanelTOB_log");

  overfloorTop_log = new G4LogicalVolume(overfloorTop, vetoMaterial, "overfloorTop_log");
  overfloorBottom_log = new G4LogicalVolume(overfloorBottom, vetoMaterial, "overfloorBottom_log");

  //placements

  //Placing the Front Panels

  vetoPanelFB_phys = new G4PVPlacement(0, FBOutPos, vetoPanelFB_log, "FB_Out", theDetectorLogical, false, 0);
  vetoPanelFT_phys = new G4PVPlacement(0, FTOutPos, vetoPanelFT_log, "FT_Out", theDetectorLogical, false, 0);

  vetoPanelFL_phys = new G4PVPlacement(0, FLOutPos, vetoPanelFL_log, "FL_Out", theDetectorLogical, false, 0);
  vetoPanelFR_phys = new G4PVPlacement(0, FROutPos, vetoPanelFR_log, "FR_Out", theDetectorLogical, false, 0);

  vetoPanelLSB_phys = new G4PVPlacement(0, LSBOutPos, vetoPanelLSB_log, "LSB_Out", theDetectorLogical, false, 0);
  vetoPanelLST_phys = new G4PVPlacement(0, LSTOutPos, vetoPanelLST_log, "LST_Out", theDetectorLogical, false, 0);

  vetoPanelLSL_phys = new G4PVPlacement(0, LSLOutPos, vetoPanelLSL_log, "LSL_Out", theDetectorLogical, false, 0);
  vetoPanelLSR_phys = new G4PVPlacement(0, LSROutPos, vetoPanelLSR_log, "LSR_Out", theDetectorLogical, false, 0);

  solidPanelBB_phys = new G4PVPlacement(0, BBOutPos, solidPanelBB_log, "BB_Out", theDetectorLogical, false, 0);
  solidPanelBT_phys = new G4PVPlacement(0, BTOutPos, solidPanelBT_log, "BT_Out", theDetectorLogical, false, 0);

  solidPanelBL_phys = new G4PVPlacement(0, BLOutPos, solidPanelBL_log, "BL_Out", theDetectorLogical, false, 0);
  solidPanelBR_phys = new G4PVPlacement(0, BROutPos, solidPanelBR_log, "BR_Out", theDetectorLogical, false, 0);

  solidPanelRSB_phys = new G4PVPlacement(0, RSBOutPos, solidPanelRSB_log, "RSB_Out", theDetectorLogical, false, 0);
  solidPanelRST_phys = new G4PVPlacement(0, RSTOutPos, solidPanelRST_log, "RST_Out", theDetectorLogical, false, 0);

  solidPanelRSL_phys = new G4PVPlacement(0, RSLOutPos, solidPanelRSL_log, "RSL_Out", theDetectorLogical, false, 0);
  solidPanelRSR_phys = new G4PVPlacement(0, RSROutPos, solidPanelRSR_log, "RSR_Out", theDetectorLogical, false, 0);

  solidPanelTIL_phys = new G4PVPlacement(0, TLOutPos, solidPanelTIL_log, "TIL_Out", theDetectorLogical, false, 0);
  solidPanelTIR_phys = new G4PVPlacement(0, TROutPos, solidPanelTIR_log, "TIR_Out", theDetectorLogical, false, 0);

  solidPanelTOF_phys = new G4PVPlacement(0, TFOutPos, solidPanelTOF_log, "TOF_Out", theDetectorLogical, false, 0);
  solidPanelTOB_phys = new G4PVPlacement(0, TBOutPos, solidPanelTOB_log, "TOB_Out", theDetectorLogical, false, 0);


  for(copyno=0; copyno<=5; copyno++)
{
  overfloorTop_phys = new G4PVPlacement(0, overfloorTop_pos[copyno], overfloorTop_log, "overfloorTop_phys", theDetectorLogical, false, copyno);
}

  for(copyno=0; copyno<=5; copyno++)
{
  overfloorBottom_phys = new G4PVPlacement(0, overfloorBottom_pos[copyno], overfloorBottom_log, "overfloorBottom_phys", theDetectorLogical, false, copyno);
}


//--------- Visualization attributes -------------------------------

  theDetectorLogical->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VisAttributes* YellowVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0)); // pink
//G4VisAttributes* YellowVisAtt = new G4VisAttributes(G4Colour(0.0,1,0.0)); // green  
//  G4VisAttributes* YellowVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0)); // yellow
  vetoPanelFB_log->SetVisAttributes(YellowVisAtt);
  vetoPanelFT_log->SetVisAttributes(YellowVisAtt);
  solidPanelBB_log->SetVisAttributes(YellowVisAtt);
  solidPanelBT_log->SetVisAttributes(YellowVisAtt);
  vetoPanelLSB_log->SetVisAttributes(YellowVisAtt);
  vetoPanelLST_log->SetVisAttributes(YellowVisAtt);
  solidPanelRSB_log->SetVisAttributes(YellowVisAtt);
  solidPanelRST_log->SetVisAttributes(YellowVisAtt);
  solidPanelTOB_log->SetVisAttributes(YellowVisAtt);
  solidPanelTOF_log->SetVisAttributes(YellowVisAtt);

  MGLog(debugging) << "Set yellow color.\n" << endlog; 

  G4VisAttributes* GreyVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // grey
//  GreyVisAtt->SetForceWireframe( true );
  vetoPanelFL_log->SetVisAttributes(GreyVisAtt);
  vetoPanelFR_log->SetVisAttributes(GreyVisAtt);
  solidPanelBL_log->SetVisAttributes(GreyVisAtt);
  solidPanelBR_log->SetVisAttributes(GreyVisAtt);
  vetoPanelLSL_log->SetVisAttributes(GreyVisAtt);
  vetoPanelLSR_log->SetVisAttributes(GreyVisAtt);
  solidPanelRSL_log->SetVisAttributes(GreyVisAtt);
  solidPanelRSR_log->SetVisAttributes(GreyVisAtt);
  solidPanelTIL_log->SetVisAttributes(GreyVisAtt);
  solidPanelTIR_log->SetVisAttributes(GreyVisAtt);
  overfloorBottom_log->SetVisAttributes(GreyVisAtt);

  MGLog(debugging) << "Set grey color.\n" << endlog; 
/*
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

*/



} //end file
