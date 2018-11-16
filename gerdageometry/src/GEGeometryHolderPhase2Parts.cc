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
/**
 * $Id: GEGeometryHolderPhase2Parts.cc$
 *
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/*
 *  This class should contain all
 *    - parameters
 *    - logical volumes
 *    - physical volumes
 *  of all parts of the Phase 2 holders *
 */
// End class description
//
//
// --------------------------------------------------------------------------//
/**
 * AUTHOR: Anne Wegmann
 * CONTACT:
 * FIRST SUBMISSION: @START_DATE@
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 16-04-2012, first version, Anne
 * 25-05-2012, changed solid name to avoid confusion with Phase1 holders, Anne
 * 04-10-2016, adapted to status december 2016
 * 	       individual holder bar length for individual detector position, Janina
 */
// --------------------------------------------------------------------------//
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Polycone.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4Trap.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"


#include "gerdageometry/GEGeometryHolderPhase2Parts.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;


G4double GEGeometryHolderPhase2Parts::SmallNumber = 0.00123 * mm;

//--------------------------------------------------------------------------------
GEGeometryHolderPhase2Parts::GEGeometryHolderPhase2Parts(const G4double gap) :
  fMaximalAllowedCrystalRadius( -DBL_MAX ),
  fRadialDistance_CrystalSurface_Holder( -DBL_MAX ),
  fTopDistance_CrystalSurface_Holder( -DBL_MAX ),
  fBotDistance_CrystalSurface_Holder( -DBL_MAX ),
  fCrystalRadius( -DBL_MAX ),
  fCrystalInnerRadius( -DBL_MAX ),
  fCrystalHeight( -DBL_MAX ),
  fCrystalID( -DBL_MAX ),  
  fCopperBarLength( -DBL_MAX ),

  // solides  
  fSolid_LowerPlate ( 0 ),
  fSolid_UpperPlate ( 0 ),
  fSolid_VerticalBar( 0 ),

  // vis attributes 
  fVisAtt_LowerPlate ( 0 ),
  fVisAtt_UpperPlate ( 0 ),
  fVisAtt_VerticalBar( 0 ),

  // materials
  fMaterial_LowerPlate ( 0 ),
  fMaterial_UpperPlate ( 0 ),  
  fMaterial_VerticalBar( 0 ),

  // logical volumes
  fLogicalVol_LowerPlate ( 0 ),
  fLogicalVol_UpperPlate ( 0 ),
  fLogicalVol_VerticalBar( 0 ),
  
  fCrystalPairGap(gap),
  fIsAPair(false),
  fIsType3(false),
  fIsCoax(false)
  
{
  for (size_t i=0;i<100;i++)
    fNumberOfPhysicalVolumesSets[i] = 0;


  // parameters probably beeing reset before creating solids and volumes
  // --------------------------------------------------------------------
  fMaximalAllowedCrystalRadius = 41.*mm;//37.5*mm;
  fRadialDistance_CrystalSurface_Holder = 1.00*mm;
  fTopDistance_CrystalSurface_Holder = 4.00*mm;
  fBotDistance_CrystalSurface_Holder = 4.00*mm;
  fHolderWidth=90.00*mm;
  fCrystalRadius = -100;  // must be reset
  fCrystalInnerRadius = -100;  // must be reset
  fCrystalHeight = -100;  // must be reset
  fCopperBarLength = -100; // must be reset
  
  //parameters for Holder plates
  // ---------------------------
  fSiliconPlate_h = 1.5*mm;

  // parameters for vertical bars
  // -----------------------------
  fVerticalBar_Ro = 1.5*mm;
}

//--------------------------------------------------------------------------------
GEGeometryHolderPhase2Parts::GEGeometryHolderPhase2Parts(const GEGeometryHolderPhase2Parts& holder):

  fCrystalRadius( -DBL_MAX ),
  fCrystalInnerRadius( -DBL_MAX ),
  fCrystalHeight( -DBL_MAX ),
  fCrystalID( -DBL_MAX ),
  fCopperBarLength( -DBL_MAX ),
  
  // solides 
  fSolid_LowerPlate ( 0 ),
  fSolid_UpperPlate ( 0 ),
  fSolid_VerticalBar( 0 ),

  // vis attributes
  fVisAtt_LowerPlate ( 0 ),
  fVisAtt_UpperPlate ( 0 ), 
  fVisAtt_VerticalBar( 0 ),

  // materials
  fMaterial_LowerPlate ( 0 ),
  fMaterial_UpperPlate ( 0 ),  
  fMaterial_VerticalBar( 0 ),

  // logical volumes
  fLogicalVol_LowerPlate ( 0 ),
  fLogicalVol_UpperPlate ( 0 ),  
  fLogicalVol_VerticalBar( 0 ),
  
  fCrystalPairGap( holder.fCrystalPairGap),
  fIsAPair(holder.fIsAPair)
{

  for (size_t i=0;i<100;i++)
    fNumberOfPhysicalVolumesSets[i] = 0;
  
  // parameters probably beeing reset before creating solids and volumes
  // --------------------------------------------------------------------
  fMaximalAllowedCrystalRadius            = holder.fMaximalAllowedCrystalRadius    ;
  fRadialDistance_CrystalSurface_Holder   = holder.fRadialDistance_CrystalSurface_Holder;
  fTopDistance_CrystalSurface_Holder      = holder.fTopDistance_CrystalSurface_Holder;
  fBotDistance_CrystalSurface_Holder      = holder.fBotDistance_CrystalSurface_Holder;
  fHolderWidth=holder.fHolderWidth;
  //parameters for Holder plates
  // ---------------------------
  fSiliconPlate_h = holder.fSiliconPlate_h;
  
  // parameters for vertical bars
  // -----------------------------
  fVerticalBar_Ro = holder.fVerticalBar_Ro;

  ResetDependentParameters();
}

//--------------------------------------------------------------------------------

GEGeometryHolderPhase2Parts::~GEGeometryHolderPhase2Parts()
{
  delete fLogicalVol_UpperPlate;
  delete fLogicalVol_LowerPlate;
  delete fLogicalVol_VerticalBar;
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase2Parts::ResetDependentParameters()
{
    // The order of the dependencies is given in the default constructor
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase2Parts::CreateSolids()
{
  MGLog(debugging) << "   Starting to create solids"<<  endlog;
  CreateSolids_UpperPlate();
  CreateSolids_LowerPlate();
  CreateSolids_VerticalBar();

  MGLog(debugging) << "   Ending to create solids"<<  endlog;
}


//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase2Parts::CreateSolids_VerticalBar()
{
  char solidname[100];
  sprintf(solidname,"solid_Holder_VerticalBar_%d",fCrystalID);		  
				  
	fSolid_VerticalBar = new G4Tubs(solidname,
				  0.*mm,
				  fVerticalBar_Ro,
				  0.5 *fCopperBarLength,
// 				  fCrystalHeight // is the sum of both detector heigths in case of a pair
// 				          +aGap
// 				          +34*mm),
// 				          +fTopDistance_CrystalSurface_Holder 
// 				          +fBotDistance_CrystalSurface_Holder),
				  0, 360*deg);			  
	//MGLog(debugging) << "fCrystalID " << fCrystalID << " LengthOfCopperBars "<< fCopperBarLength <<endlog;			


}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase2Parts::CreateSolids_LowerPlate()
{
  G4double TubsStartAngle =  0;
  G4double TubsSpanningAngle = 360 * deg;

  char solidname[100];
    
//   sprintf(solidname,"solid_LowerPlate_%d",fCrystalID);
//   G4Tubs* solid_LowerPlate  = new G4Tubs(solidname,
// 					 15*mm,
// 					 42*mm,
// 					 0.5*fSiliconPlate_h,
// 					 TubsStartAngle, TubsSpanningAngle);
  
  // build rectangular base plate
  G4double trg_b = (fHolderWidth/sqrt(2.))*mm;
  G4double trg_h = 45.*mm;
  G4double rect_x = 2.*(trg_h + 9.5*mm);
  G4double rect_y = fHolderWidth;
  G4double rect_bc_x = 28.*mm;
  G4double cut_x = 20.*mm;
  G4double cut_y = 31.*mm;
  
  G4RotationMatrix* rm = new G4RotationMatrix();
  rm->rotateZ(45.*deg);
  G4RotationMatrix* rm1 = new G4RotationMatrix();
  rm1->rotateZ(-120.*deg);
  G4RotationMatrix* rm2 = new G4RotationMatrix();
  rm2->rotateZ(120.*deg);
  sprintf(solidname,"solid_LowerPlate_Rect_%d",fCrystalID);
  G4VSolid* solid_LowerPlate_Rect  = new G4Box(solidname,
                                      rect_x/2,
                                      rect_y/2,
                                      0.5*fSiliconPlate_h);
  
  // build rectangular cut plates
  sprintf(solidname,"solid_LowerPlate_CutRect0_%d",fCrystalID);
  G4VSolid* solid_LowerPlate_CutRect0  = new G4Box(solidname,
                                                   (0.5*rect_x- rect_bc_x+1*mm)*mm/2,
                                                   (rect_y+1*mm)/2,
                                                   0.5*(fSiliconPlate_h+.1*mm));
  
  sprintf(solidname,"solid_LowerPlate_CutRect1_%d",fCrystalID);
  G4VSolid* solid_LowerPlate_CutRect1  = new G4Box(solidname,
                                                   trg_b/2,
                                                   trg_b/2,
                                                   0.5*(fSiliconPlate_h+.1*mm));
  
  sprintf(solidname,"solid_LowerPlate_CutRect2_%d",fCrystalID);
  G4VSolid* solid_LowerPlate_CutRect2  = new G4Box(solidname,
                                                   cut_x/2,
                                                   cut_y/2,
                                                   0.5*(fSiliconPlate_h+.1*mm));
  
  //build rectangular cut plates (to imitate rounded corners, important to fit into MS)
  sprintf(solidname,"solid_LowerPlate_CutRect3_%d",fCrystalID);
  G4VSolid* solid_LowerPlate_CutRect3  = new G4Box(solidname,
                                                   9*mm/2,
                                                   20*mm/2,
                                                   0.5*(fSiliconPlate_h+.1*mm));
  
  sprintf(solidname,"solid_LowerPlate_CutRect4_%d",fCrystalID);
  G4VSolid* solid_LowerPlate_CutRect4  = new G4Box(solidname,
                                                   6*mm/2,
                                                   20*mm/2,
                                                   0.5*(fSiliconPlate_h+.1*mm));
  
  // cut final silicon plate
  sprintf(solidname, "solid_LowerPlate_Base0_%d", fCrystalID);
  G4VSolid* solid_LowerPlate_Base0 = new G4SubtractionSolid(solidname,
                                                            solid_LowerPlate_Rect,
                                                            solid_LowerPlate_CutRect0,
                                                            0,
                                                            G4ThreeVector((-.5*rect_bc_x-.5*.5*rect_x-.5*1*mm),0.,0.));
  
  sprintf(solidname, "solid_LowerPlate_Base1_%d", fCrystalID);
  G4VSolid* solid_LowerPlate_Base1 = new G4SubtractionSolid(solidname,
                                                            solid_LowerPlate_Base0,
                                                            solid_LowerPlate_CutRect1,
                                                            rm,
                                                            G4ThreeVector(rect_x/2,trg_h,0.));
  
  sprintf(solidname, "solid_LowerPlate_Base2_%d", fCrystalID);
  G4VSolid* solid_LowerPlate_Base2 = new G4SubtractionSolid(solidname,
                                                            solid_LowerPlate_Base1,
                                                            solid_LowerPlate_CutRect1,
                                                            rm,
                                                            G4ThreeVector(rect_x/2,-trg_h,0.));
  
  sprintf(solidname, "solid_LowerPlate_Base3_%d", fCrystalID);
  G4VSolid* solid_LowerPlate_Base3 = new G4SubtractionSolid(solidname,
                                                            solid_LowerPlate_Base2,
                                                            solid_LowerPlate_CutRect2,
                                                            0,
                                                            G4ThreeVector(0.,(0.5*cut_y-3.5*mm),0.));
  
  sprintf(solidname, "solid_LowerPlate_Base4_%d", fCrystalID);
  G4VSolid* solid_LowerPlate_Base4 = new G4SubtractionSolid(solidname,
                                                            solid_LowerPlate_Base3,
                                                            solid_LowerPlate_CutRect3,
                                                            0,
                                                            G4ThreeVector(rect_x/2,0,0.));
  
  sprintf(solidname, "solid_LowerPlate_Base5_%d", fCrystalID);
  G4VSolid* solid_LowerPlate_Base5 = new G4SubtractionSolid(solidname,
                                                            solid_LowerPlate_Base4,
                                                            solid_LowerPlate_CutRect4,
                                                            rm1,
                                                            G4ThreeVector(-rect_bc_x,trg_h,0.));
  
  sprintf(solidname, "solid_LowerPlate_Base_%d", fCrystalID);
  G4VSolid* solid_LowerPlate_Base = new G4SubtractionSolid(solidname,
                                                            solid_LowerPlate_Base5,
                                                            solid_LowerPlate_CutRect4,
                                                            rm2,
                                                            G4ThreeVector(-rect_bc_x,-trg_h,0.));
  
  sprintf(solidname,"solid_LowerPlate_hole_%d",fCrystalID);
  G4Tubs* solid_LowerPlate_hole  = new G4Tubs(solidname,
					      0*mm,
					      (fVerticalBar_Ro+0.1*mm),
					      0.5*fSiliconPlate_h,
					      TubsStartAngle, TubsSpanningAngle);
  
  if(fCrystalRadius>41.*mm){fMaximalAllowedCrystalRadius=46.*mm;} 
  //radius condition identifies GTFs 
  G4double tmpR = fMaximalAllowedCrystalRadius + fRadialDistance_CrystalSurface_Holder + fVerticalBar_Ro;
  G4VSolid* current_solid_LowerPlate = solid_LowerPlate_Base;

  for (int i = 0; i < 3; i++) {
    const G4double VertBarAngle = ((G4double) i) * 120.*deg; 
    const G4ThreeVector VertBarTranslation (/* x */ tmpR * std::cos(VertBarAngle),  /* y */ tmpR * std::sin(VertBarAngle), /* z */ 0.);
  
    sprintf(solidname, "solid_LowerPlate_%dhole_%d", i + 1, fCrystalID);
    G4VSolid* solid_LowerPlate_nHoles = new G4SubtractionSolid(solidname,
							       current_solid_LowerPlate,
							       solid_LowerPlate_hole,
							       0,
							       VertBarTranslation);
    current_solid_LowerPlate = solid_LowerPlate_nHoles;
  }
  fSolid_LowerPlate = current_solid_LowerPlate;
}


//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase2Parts::CreateSolids_UpperPlate()
{
  G4double TubsStartAngle =  0.;
  G4double TubsSpanningAngle = 360. * deg;

  //char solidname_UpperPlate[100];
//   sprintf(solidname_UpperPlate,"solid_UpperPlate_%d",fCrystalID);
//   G4Tubs* solid_UpperPlate  = new G4Tubs(solidname_UpperPlate,
// 					 15*mm,
// 					 42*mm,
// 					 0.5*fSiliconPlate_h,
// 					 TubsStartAngle, TubsSpanningAngle);
  
  // build rectangular base plate
  G4double trg_b = (fHolderWidth/sqrt(2))*mm;
  G4double trg_h = 45*mm;
  G4double rect_x = 2*(trg_h + 9.5*mm);
  G4double rect_y = fHolderWidth;
  G4double rect_bc_x = 28*mm;
  G4double cut_x = 20*mm;
  G4double cut_y = 31*mm;
  
  G4RotationMatrix* rm = new G4RotationMatrix();
  rm->rotateZ(45.*deg);
  G4RotationMatrix* rm1 = new G4RotationMatrix();
  rm1->rotateZ(-120.*deg);
  G4RotationMatrix* rm2 = new G4RotationMatrix();
  rm2->rotateZ(120.*deg);

  char solidname[100];
  sprintf(solidname,"solid_UpperPlate_Rect_%d",fCrystalID);
  G4VSolid* solid_UpperPlate_Rect  = new G4Box(solidname,
                                               rect_x/2,
                                               rect_y/2,
                                               0.5*fSiliconPlate_h);
  
  // build rectangular cut plates
  sprintf(solidname,"solid_UpperPlate_CutRect0_%d",fCrystalID);
  G4VSolid* solid_UpperPlate_CutRect0  = new G4Box(solidname,
                                                   (rect_x/2-rect_bc_x+1*mm)/2,
                                                   (rect_y+1)*mm/2,
                                                   0.5*(fSiliconPlate_h+.1*mm));
  
  sprintf(solidname,"solid_UpperPlate_CutRect1_%d",fCrystalID);
  G4VSolid* solid_UpperPlate_CutRect1  = new G4Box(solidname,
                                                   trg_b/2,
                                                   trg_b/2,
                                                   0.5*(fSiliconPlate_h+.1*mm));
  
  sprintf(solidname,"solid_UpperPlate_CutRect2_%d",fCrystalID);
  G4VSolid* solid_UpperPlate_CutRect2  = new G4Box(solidname,
                                                   cut_x/2,
                                                   cut_y/2,
                                                   0.5*(fSiliconPlate_h+.1*mm));
  
  //build rectangular cut plates (to imitate rounded corners, important to fit into MS)
  sprintf(solidname,"solid_UpperPlate_CutRect3_%d",fCrystalID);
  G4VSolid* solid_UpperPlate_CutRect3  = new G4Box(solidname,
                                                   9*mm/2,
                                                   20*mm/2,
                                                   0.5*(fSiliconPlate_h+.1*mm));
  
  sprintf(solidname,"solid_UpperPlate_CutRect4_%d",fCrystalID);
  G4VSolid* solid_UpperPlate_CutRect4  = new G4Box(solidname,
                                                   6*mm/2,
                                                   20*mm/2,
                                                   0.5*(fSiliconPlate_h+.1*mm));
  
  // cut final silicon plate
  sprintf(solidname, "solid_UpperPlate_Base0_%d", fCrystalID);
  G4VSolid* solid_UpperPlate_Base0 = new G4SubtractionSolid(solidname,
                                                            solid_UpperPlate_Rect,
                                                            solid_UpperPlate_CutRect0,
                                                            0,
                                                            G4ThreeVector((-.5*rect_bc_x-.5*.5*rect_x-.5*1*mm),0.,0.));
  
  sprintf(solidname, "solid_UpperPlate_Base1_%d", fCrystalID);
  G4VSolid* solid_UpperPlate_Base1 = new G4SubtractionSolid(solidname,
                                                            solid_UpperPlate_Base0,
                                                            solid_UpperPlate_CutRect1,
                                                            rm,
                                                            G4ThreeVector(rect_x/2,trg_h,0.));
  
  sprintf(solidname, "solid_UpperPlate_Base2_%d", fCrystalID);
  G4VSolid* solid_UpperPlate_Base2 = new G4SubtractionSolid(solidname,
                                                            solid_UpperPlate_Base1,
                                                            solid_UpperPlate_CutRect1,
                                                            rm,
                                                            G4ThreeVector(rect_x/2,-trg_h,0.));
  
  sprintf(solidname, "solid_UpperPlate_Base3_%d", fCrystalID);
  G4VSolid* solid_UpperPlate_Base3 = new G4SubtractionSolid(solidname,
                                                           solid_UpperPlate_Base2,
                                                           solid_UpperPlate_CutRect2,
                                                           0,
                                                           G4ThreeVector(0.,(0.5*cut_y-3.5*mm),0.));
  
  sprintf(solidname, "solid_UpperPlate_Base4_%d", fCrystalID);
  G4VSolid* solid_UpperPlate_Base4 = new G4SubtractionSolid(solidname,
                                                            solid_UpperPlate_Base3,
                                                            solid_UpperPlate_CutRect3,
                                                            0,
                                                            G4ThreeVector(rect_x/2,0,0.));
  
  sprintf(solidname, "solid_UpperPlate_Base5_%d", fCrystalID);
  G4VSolid* solid_UpperPlate_Base5 = new G4SubtractionSolid(solidname,
                                                            solid_UpperPlate_Base4,
                                                            solid_UpperPlate_CutRect4,
                                                            rm1,
                                                            G4ThreeVector(-rect_bc_x,trg_h,0.));
  
  sprintf(solidname, "solid_UpperPlate_Base_%d", fCrystalID);
  G4VSolid* solid_UpperPlate_Base = new G4SubtractionSolid(solidname,
                                                            solid_UpperPlate_Base5,
                                                            solid_UpperPlate_CutRect4,
                                                            rm2,
                                                            G4ThreeVector(-rect_bc_x,-trg_h,0.));
         
//   char solidname[100];
  sprintf(solidname,"solid_UpperPlate_hole_%d",fCrystalID);
  G4Tubs* solid_UpperPlate_hole  = new G4Tubs(solidname,
					      0*mm,
					      (fVerticalBar_Ro+0.1*mm),
					      0.5*fSiliconPlate_h,
					      TubsStartAngle, TubsSpanningAngle);
  
  if(fCrystalRadius>41.*mm){fMaximalAllowedCrystalRadius=46.*mm;}
  //radius condition identifies GTFs 
  G4double tmpR = fMaximalAllowedCrystalRadius + fRadialDistance_CrystalSurface_Holder + fVerticalBar_Ro;
  G4VSolid* current_solid_UpperPlate = solid_UpperPlate_Base;

  for (int i = 0; i < 3; i++) {
    const G4double VertBarAngle = ((G4double) i) * 120.*deg; 
    const G4ThreeVector VertBarTranslation (/* x */ tmpR * std::cos(VertBarAngle),  /* y */ tmpR * std::sin(VertBarAngle), /* z */ 0.);
  
    sprintf(solidname, "solid_UpperPlate_%dhole_%d", i + 1, fCrystalID);
    G4VSolid* solid_UpperPlate_nHoles = new G4SubtractionSolid(solidname,
							       current_solid_UpperPlate,
							       solid_UpperPlate_hole,
							       0,
							       VertBarTranslation);
    current_solid_UpperPlate = solid_UpperPlate_nHoles;
  }
  fSolid_UpperPlate = current_solid_UpperPlate;
}

    
//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase2Parts::CreateLogicalVolumes()
{
    MGLog(debugging) << "   Starting to create logical volumes"<<  endlog;
    CreateLogicalVolumes_LowerPlate();
    CreateLogicalVolumes_UpperPlate();
    CreateLogicalVolumes_VerticalBar();

    MGLog(debugging) << "   Ending to create logical volumes"<<  endlog;
}


//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase2Parts::CreateLogicalVolumes_VerticalBar()
{
  char tmpname[100];
  sprintf(tmpname,"Holder_VerticalBar_%d_Logical",fCrystalID);
  fLogicalVol_VerticalBar = new G4LogicalVolume( fSolid_VerticalBar,
						 fMaterial_VerticalBar,
						 tmpname); 
  fLogicalVol_VerticalBar -> SetVisAttributes(fVisAtt_VerticalBar);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase2Parts::CreateLogicalVolumes_LowerPlate()
{
  char tmpname[100];
  sprintf(tmpname,"Holder_LowerPlate_%d_Logical",fCrystalID);
  fLogicalVol_LowerPlate = new G4LogicalVolume( fSolid_LowerPlate,
						fMaterial_LowerPlate,
						tmpname); 
  fLogicalVol_LowerPlate -> SetVisAttributes(fVisAtt_LowerPlate);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase2Parts::CreateLogicalVolumes_UpperPlate()
{
  char tmpname[100];
  sprintf(tmpname,"Holder_UpperPlate_%d_Logical",fCrystalID);
  fLogicalVol_UpperPlate = new G4LogicalVolume( fSolid_UpperPlate,
						fMaterial_UpperPlate,
						tmpname); 
  fLogicalVol_UpperPlate -> SetVisAttributes(fVisAtt_UpperPlate);
}

//--------------------------------------------------------------------------------
void
GEGeometryHolderPhase2Parts::CreatePhysicalVolume(GEGeometryDetectorDB *db, G4LogicalVolume* motherVol, G4int col, G4int row, G4int ffHolderGeometryNumber,G4int ffHolderGeometryNumberSlot, const G4ThreeVector& crystalPos)
{
  char volname[100];
  G4ThreeVector position;

  G4bool ffIsAPair=db->IsAPair(col,row);

  G4double    fHolderPlateRot_angle=db->GetAngleForPhase2CablePosition(col);
      
  G4RotationMatrix* HolderPlateRot = new G4RotationMatrix();
  HolderPlateRot->rotateZ(fHolderPlateRot_angle);
  
  //fCrystalHeight = 0.0;
  fCrystalHeight =  (db->GetDetectorParameters(db->GetCrystalType(col,row,0)))->GetHeight();
  
  // the upper plate 
  // ---------------
  G4double aGap = 0.0*mm;
  aGap = (ffIsAPair || fIsType3) ? fCrystalPairGap : 0.;
  if(ffIsAPair){
  ///MGLog(debugging) << "col "<<col <<" row "<< row << endlog;
  position = crystalPos;

  G4double fCrystalHeight0 =  (db->GetDetectorParameters(db->GetCrystalType(col,row,0)))->GetHeight();
  fCrystalHeight=2*fCrystalHeight0;
 
  position.setZ( position.getZ() + 0.5*fCrystalHeight + fTopDistance_CrystalSurface_Holder + 0.5 * aGap + 0.5*fSiliconPlate_h);
  sprintf(volname,"Holder_Plate_%d",(ffHolderGeometryNumber-2));
  /*G4PVPlacement* UpperPlatePhysical =*/ new G4PVPlacement( HolderPlateRot, 
  							     position,
							     fLogicalVol_UpperPlate,
							     volname, 
							     motherVol,
							     false, 
							     fNumberOfPhysicalVolumesSets[fCrystalID]);
  }
  // the lower plate
  // ---------------
  position = crystalPos;
  if(ffIsAPair){
    G4double fCrystalHeight1 =  (db->GetDetectorParameters(db->GetCrystalType(col,row,1)))->GetHeight();
    fCrystalHeight=2*fCrystalHeight1;
  }
  position.setZ( position.getZ() - 0.5*fCrystalHeight - fBotDistance_CrystalSurface_Holder - 0.5 * aGap - 0.5*fSiliconPlate_h);
  sprintf(volname,"Holder_Plate_%d",(ffHolderGeometryNumber-1));
  /*G4PVPlacement* LowerPlatePhysical =*/ new G4PVPlacement(HolderPlateRot,
							    position, 
							    fLogicalVol_LowerPlate, 
							    volname, 
							    motherVol, 
							    false, 
							    fNumberOfPhysicalVolumesSets[fCrystalID]);
   					    
    
  // the three vertical bars
  // -----------------------
  for(int ii=0;ii<3;ii++) {
    G4double VertBarAngle = ii*120.*deg+fHolderPlateRot_angle;
    G4ThreeVector VertBarTranslation;
    if(fCrystalRadius>41.*mm){fMaximalAllowedCrystalRadius=46.*mm;}
    G4double tmpR = fMaximalAllowedCrystalRadius + fRadialDistance_CrystalSurface_Holder + fVerticalBar_Ro;
    VertBarTranslation.setX(tmpR*std::cos(VertBarAngle));
    VertBarTranslation.setY(tmpR*std::sin(VertBarAngle));
    VertBarTranslation.setZ( 0.5*fCopperBarLength*mm -11.00*mm - fSiliconPlate_h - fBotDistance_CrystalSurface_Holder -0.5*fCrystalHeight - 0.5*aGap );

    position = crystalPos + VertBarTranslation;
    G4RotationMatrix VertBarRot;
    VertBarRot.rotateZ(VertBarAngle+ fHolderPlateRot_angle);
    sprintf(volname,"Holder_VerticalBar_%d",  3*(ffHolderGeometryNumberSlot-1)+ii );
    /* VerticalBarPhysical[ii]  = */
    new G4PVPlacement( G4Transform3D(VertBarRot,position),
		       fLogicalVol_VerticalBar, 
		       volname, 
		       motherVol, 
		       false, 
		       3*fNumberOfPhysicalVolumesSets[fCrystalID] + ii );
    MGLog(debugging) << "######## 3*fNumberOfPhysicalVolumesSets["<<fCrystalID<<"] + ii = "
                     << (3*fNumberOfPhysicalVolumesSets[fCrystalID] + ii) << "   volname="<< volname<<endlog;	     
  }
 
  fNumberOfPhysicalVolumesSets[fCrystalID]++;
}


//--------------------------------------------------------------------------------------------
//distance from bottom of crystal to bottom side of silicon plate 
G4double GEGeometryHolderPhase2Parts::GetZDistanceToEndOfSiliconPlate() const
{   
    return (fBotDistance_CrystalSurface_Holder + fSiliconPlate_h);
}
//distance in pair from bottom of top crystal to top side of silicon plate above this crystal
G4double GEGeometryHolderPhase2Parts::GetZDistanceToEndOfSiliconPlatePair() const
{   
    return (fCrystalHeight + fBotDistance_CrystalSurface_Holder + fSiliconPlate_h);
}
//--------------------------------------------------------------------------------------------
//holder parameter for the placement of cables

G4double GEGeometryHolderPhase2Parts::GetHolderPhase2Width() const
{   
    return fHolderWidth;
}

//--------------------------------------------------------------------------------------------
void GEGeometryHolderPhase2Parts::ResetParametersForAPair(GEGeometryDetectorDB* db,G4int col,
                                                          G4int row/*, G4int pairID=0*/)
{
  fIsAPair = true;
  fIsCoax = false;//required as flag will still be set to true arbitrarily
  fIsType3 = false;//required as flag will still be set to true arbitrarily
  GEGeometryDetectorParameters* detector1 =
    db->GetDetectorParameters(db->GetCrystalType(col,row,0));
  GEGeometryDetectorParameters* detector2 =
    db->GetDetectorParameters(db->GetCrystalType(col,row,1));

  SetCrystalRadius(std::max(detector1->GetOuterRadius(),detector2->GetOuterRadius()));
  SetCrystalInnerRadius(std::min(detector1->GetInnerRadius(),detector2->GetInnerRadius()));
  SetCrystalHeight(detector1->GetHeight()+detector2->GetHeight()); //sum!
  SetCrystalID(detector1->GetDetectorID());
  SetLengthOfCopperBars(db->MatchDetectorIDwithHolderBarsPhase2(detector1->GetDetectorID(),detector1->GetGeometry()));

  ResetDependentParameters();
  return;
}


void GEGeometryHolderPhase2Parts::ResetParametersForSingle(GEGeometryDetectorDB* db,G4int col,
                                                          G4int row)
{ fIsAPair = false;//required as flag will still be set to true arbitrarily
  fIsCoax = false;//required as flag will still be set to true arbitrarily
  fIsType3 = false;//required as flag will still be set to true arbitrarily
  
  
  GEGeometryDetectorParameters* detector =
    db->GetDetectorParameters(db->GetCrystalType(col,row));

  SetCrystalHeight(2*(detector->GetHeight())); //sum! 
  SetCrystalID(detector->GetDetectorID());
  SetLengthOfCopperBars(db->MatchDetectorIDwithHolderBarsPhase2(detector->GetDetectorID(),detector->GetGeometry()));
  ResetDependentParameters();
  return;
}


void GEGeometryHolderPhase2Parts::ResetParametersForType3Pair(GEGeometryDetectorDB* db,G4int col,
                                                          G4int row)
{
  fIsType3 = true;
  
  GEGeometryDetectorParameters* detector =
    db->GetDetectorParameters(db->GetCrystalType(col,row));

  SetCrystalHeight(2*(detector->GetHeight())); //sum!
  
  ResetDependentParameters();
  return;
}


void GEGeometryHolderPhase2Parts::ResetParametersForCoax(GEGeometryDetectorDB* /* db*/ ,
							 G4int /* col*/,
							 G4int /*row*/)
{
  fIsCoax = true;
  fIsType3 = false;//required as flag will still be set to true arbitrarily
  fIsAPair = false;//required as flag will still be set to true arbitrarily
  
  return;
}
