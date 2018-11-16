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
//
// CLASS IMPLEMENTATION:  GEGeometryLArInstHybrid.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES: Implementation of Hybrid design. It is independent of "mother" designs.
 * Eventually the other designs shall be discontinued.
 *
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: N. Barros
 * CONTACT: nuno *dot* barros *at* tu-dresden *dot* de
 * FIRST SUBMISSION: 2012-08-14, N. Barros
 *
 * REVISION:
 * 14-08-2012, Nuno, Imported inputs from the mother designs.
 * 16-08-2012, Nuno, Corrected optical properties.
 * 					 Refactored code to avoid duplications and improve performance.
 * 					 Added optional inner fiber cylinder.
 * 06-09-2012, Nuno, Changed the inner fiber cylinder to have open ends.
 * 07-09-2012, Nuno, Changed the threshold for WLS of TPB.
 * 					 Removed code warnings.
 * 					 Changed names of solids for better identification of each part.
 * 					 Renamed VM2000 entries to TPB. VM2000 is just a reflector.
 * 24-09-2012, Nuno, Re-implemented Hybrid design to track optical photons in the fibers.
 * 28-09-2012, Nuno, Changed VM2000 to have specular reflections (it is diffuse by default).
 *					 Applied additional fiber QE scale due to the geometrical effects from the bonding and
 *					 the support structure, which are not included in the simulation.
 * 01-10-2012, Nuno,  Updated numbers in the LAr Instrumentation.
 * 					          Corrected the WLS attenuation values in the fibers.
 * 					          Applied the geometric scaling to the SiPM, on top of the PDE.
 * 25-10-2012, Nuno, Removed superfluous warning messages.
 * 27-10-2012, Nuno, Added construction of an external source just above the upper PMT support structure.
 * 28-07-2016, JJ added macro command to set "fiber coverage"
 * 20-11-2017, K. v. Sturm, Fixed PMT volume lists because bottom pmts were not usable, unified lists pmt_top={0..8} pmt_bottom={9..15}
 */
//---------------------------------------------------------------------------//

#include "gerdageometry/GEGeometryLArInstHybrid.hh"
#include "gerdageometry/GEGeometryCryostatParts.hh"
#include "io/MGLogger.hh"

#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4EllipticalTube.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Polycone.hh"
#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"

// -- ROOT include
#include "TGraph.h"
#include "TMath.h"

using namespace CLHEP;

const G4double GEGeometryLArInstHybrid::LambdaE = twopi *1.973269602e-16 * m * GeV;

GEGeometryLArInstHybrid::GEGeometryLArInstHybrid(GEGeometryDetectorDB* theGeometryDB)
: fTPB(0), fWLSoptSurf(0), fTefOptSurf(0), fMotherPhysical(0),fShroudPhysical_top(0),
  fShroudPhysical_bottom(0),fShroudLogical(0)
{
  fDetectorDB = theGeometryDB;
  fMotherLogical = fDetectorDB->GetDetectorMotherVolume();
  // For the rough surfaces we also need the physical volume of the mother
  fMotherPhysical = fDetectorDB->GetCryostatParts()->GetCryoInnerwall_Inside()->GetPhysicalVolume();
  MGLog(debugging) << "Logical and Physical volumes imported. Describing:" << endlog;
  MGLog(debugging) << "LOG : " << fMotherLogical->GetName() << " MAT " << fMotherLogical->GetMaterial()->GetName() << endlog;
  MGLog(debugging) << "PHYS : " << fMotherPhysical->GetName() << " LOG " << fMotherPhysical->GetLogicalVolume()->GetName() << endlog;


  successfulInitialization = false;
  // By default do not build the central fibers, around the central string.
  fEnableCentralFibers = false;
  fEnableSegmentedFiberShroud = false;
  InitializeTPBSpectra();
  InitializeFiberSpectra();

  // Get the center of the string array
  // In the hybrid design there is only one Flange
  // Get the number of locks
  G4int nFlanges = fDetectorDB->GetNFlanges();
  if (nFlanges != 1) {
    MGLog(warning) << "Expected 1 flange (found " << nFlanges << ". Check that you are using a compatible lock." << endlog;
    G4Exception("Invalid lock type for use with LAr Instrumentation hybrid design.","WrongConfig",FatalException,"Hybrid design expects only 1 flange.");
  }


  fDetCenter = fDetectorDB->GetStringDetectorCenter(0);
  // The Z position is not correct
  // The uppermost surface is given by the class
  G4double up_Pos = fDetectorDB->GetZUppermostCrystalSurface();
  // Search for how many crystals are around the place

  G4double low_Pos = fDetectorDB->GetZLowermostCrystalSurface();

  // The axis for the LAr instrumentation stay in the average Z value

  fDetCenter.setZ(0.5*(up_Pos+low_Pos) +150*mm); /// Instrumentation not centered around the crystals but +150mm off center


  fTotal_height			= 2100.0*mm; //190mm is the reserved height for the PMTs
  fShroud_TubsHeight		= 600.0*mm;
  fShroud_TubsOuterRadius	= 235.0*mm;  //formerly 250mm

  fShroud_CenterHeight		= fTotal_height - 2*fShroud_TubsHeight;
  fShroud_Offset		= fShroud_CenterHeight/2.0 + fShroud_TubsHeight/2.0;
  fWLSThickness			= 5*micrometer;
  fFiberThickness		= 1*mm; // according to Saint Gobain data on square BCF-91A fibers
  fFiberCladThickness		= 0.01*fFiberThickness; // it's 2% of total...so each side has 1%
  fFiberLightYield		= 10000.; //7000 - 8000 for scintillating fibers, for WLS fiber is less

  //test
  fShroud_Thickness		= fFiberThickness + 2*fWLSThickness;
  //G4double Shroud_TubsInnerRadius	= fShroud_TubsOuterRadius - fShroud_Thickness;

  fVM2000_Thickness		= 1*mm;

  fRotatePMTInstAngle   = 90*deg;

  fNoOfSegments   = 15;
  fSegmentSpacing = 2*mm;

  // Get whether the inner fibers should be built or not
  if (fDetectorDB->GetLArInstHybridInnerFiber()) {
    fEnableCentralFibers = true;
  }
  // Get whether segmented outer fibers should be built or not
  if (fDetectorDB->GetLArInstHybridSegmentedFiber()) {
    fEnableSegmentedFiberShroud = true;
  }
  return;
}

GEGeometryLArInstHybrid::~GEGeometryLArInstHybrid() {
  // delete all instances stored in the arrays
  for (std::vector<G4LogicalVolume*>::iterator it = fSDLogicVec.begin(); it != fSDLogicVec.end(); ++it) {
    delete *it;
  }

}

void GEGeometryLArInstHybrid::ConstructCable_FE_Stuff() {
  /// Define some colors that will be used later
  //visualization attributes
  G4Colour lgreen	(0.0,  0.4, 0.0) ;
  G4Colour lblue	(0.0,  0.0, 0.4) ;
  G4Colour llblue	(0.,  0.0, 0.04) ;
  G4Colour blue_gray	(175/255. ,157/255. ,189/255. ) ;
  G4Colour red		(1,  0.0, 0.0);
  G4Colour lred		(0.4,  0.0, 0.0);
  G4Colour light_gray(214./255.,214./255.,214./255.);
  G4Colour dark_gray(135./255.,135./255.,135./255.);


  /// -- Define the necessary dimensions for the cables and FE plate... below these declarations everything is calculated automatically
  G4double fCableLoop_Length		= 550*mm;
  G4double fCableBand_Length		= 650*mm;
  G4double fCableRG178_diameter		= 1.9*mm;
  G4double fCableRG179_diameter		= 2.54*mm;
  G4double fCableSAMI_diameter		= 1.4*mm;
  G4double fCableBand_Width		= 95*mm;
  G4double fdistance_CableBand		= 3*mm;

  G4double fFEPlate_InnerRadius		= 25*mm;
  G4double fFEPlate_OuterRadius		= 120*mm;
  G4double fFEPlate_RingThickness	= 8*mm;
  G4double fFEPlate_Thickness		= 6*mm;

  G4double CC3_SubstrateX		= 80*mm;
  G4double CC3_SubstrateZ		= 50*mm;
  G4double CC3_SubstrateThickness	= 1*mm;


  G4double fCableRG178_Loop_OuterRadius	= (fShroud_TubsOuterRadius - fShroud_Thickness - fVM2000_Thickness - fWLSThickness - (3*fCableRG179_diameter)/2. - 2*fdistance_CableBand)/2.;
  G4double fCableBandRG178_Length 	= fCableLoop_Length - fCableRG178_Loop_OuterRadius;
  G4double alpha			= atan((2*fCableRG178_Loop_OuterRadius)/fCableBandRG178_Length);

  G4RotationMatrix* y_rot_alpha = new G4RotationMatrix;
  y_rot_alpha -> rotateY(alpha);

  G4RotationMatrix* x_rot_90 = new G4RotationMatrix;
  x_rot_90 -> rotateX(M_PI/2.);

  G4RotationMatrix* z_rot_PMTInst = new G4RotationMatrix;
  z_rot_PMTInst->rotateZ(-fRotatePMTInstAngle/(360*deg)*M_PI*2);

  /// Cable Bands for detectors (2x RG179, 2x SAMI)
  G4String name = "fPTFE_Cable"; // PTFE density scaled to RG179 density
  G4Material *fPTFE_Cable = G4Material::GetMaterial(name, false);

  if (fPTFE_Cable == 0) {
    G4NistManager* nist = G4NistManager::Instance();
    G4Element* F = nist->FindOrBuildElement("F");
    G4Element* C = nist->FindOrBuildElement("C");
    fPTFE_Cable= new G4Material(name, 3.15*g/cm3, 2, kStateSolid);
    fPTFE_Cable->AddElement(F,0.76);
    fPTFE_Cable->AddElement(C,0.24);
  }

  G4Material *fMaterialCable = G4Material::GetMaterial("fPTFE_Cable");

  G4VSolid*fCableBand_Solid = new G4Box("CableBand_Solid",
					(2*fCableRG179_diameter + 2*fCableSAMI_diameter)/2.,
					fCableBand_Width/2.,
					fCableBand_Length/2.);

  G4LogicalVolume *fCableBand_Logical = new G4LogicalVolume(fCableBand_Solid,
							    fMaterialCable,
							    "CableBand_Logical");

  /*G4VPhysicalVolume* fCableBand_Physical_1 */
  new G4PVPlacement(z_rot_PMTInst/*0*/,
		    G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z() + fShroud_CenterHeight/2. + fShroud_TubsHeight - fCableBand_Length/2.),
                    fCableBand_Logical,
                    "CableBand_Physical_0",
                    fMotherLogical,
                    false,
                    0);

  /// Cable Loop for PMTs and Fibers (1x RG178)
  G4VSolid*fCableBandRG178_VerticalSolid = new G4Box("CableBandRG178_VerticalSolid",
                                                     fCableRG178_diameter/2.,
                                                     fCableBand_Width/2.,
                                                     fCableBandRG178_Length/2.);

  G4VSolid*fCableBandRG178_TiltedSolid = new G4Box("CableBandRG178_VerticalSolid",
                                                   fCableRG178_diameter/2.,
                                                   fCableBand_Width/2.,
                                                   fCableBandRG178_Length/(2.*cos(alpha)) + 2.*fCableRG178_diameter*sin(alpha));

  G4VSolid*fCableBandRG178_TubeSolid = new G4Tubs("CableBandRG178_TubeSolid",
						  fCableRG178_Loop_OuterRadius - fCableRG178_diameter,
						  fCableRG178_Loop_OuterRadius,
						  fCableBand_Width/2.,
						  0,
						  M_PI);

  G4UnionSolid* fLoop_TriangularPart_Solid = new G4UnionSolid("Loop_TriangularPart",
                                                              fCableBandRG178_VerticalSolid,
                                                              fCableBandRG178_TiltedSolid,
                                                              y_rot_alpha,
                                                              G4ThreeVector(-3.*fCableRG178_diameter/2. +fCableRG178_Loop_OuterRadius,0.,0.) );

  G4UnionSolid* fCableLoop_Solid = new G4UnionSolid("CableLoop_Solid",
						    fLoop_TriangularPart_Solid,
						    fCableBandRG178_TubeSolid,
						    x_rot_90,
						    G4ThreeVector(- fCableRG178_diameter/2. +fCableRG178_Loop_OuterRadius,0.,- fCableBandRG178_Length/2.) );

  G4LogicalVolume* fCableLoop_Logical = new G4LogicalVolume(fCableLoop_Solid,
							    fMaterialCable,
							    "CableLoop_Logical");

  /*G4VPhysicalVolume* fCableBand_Physical_2 = */
    new G4PVPlacement(z_rot_PMTInst/*0*/,
                      G4ThreeVector(fDetCenter.x() + (2.*fCableRG179_diameter + 2*fCableSAMI_diameter)/2. + fCableRG178_diameter/2.,fDetCenter.y(),fDetCenter.z() + fShroud_CenterHeight/2. + fShroud_TubsHeight - fCableBandRG178_Length/2.),
                      fCableLoop_Logical,
                      "CableBand_Physical_1",
                      fMotherLogical,
                      false,
                      0);


  /// FE electronics plate
  G4Material *fMaterialFEPlate = G4Material::GetMaterial("MetalCopper");

  G4VSolid*fFEPlate_Solid = new G4Tubs("FEPlate_Solid",
				       0.,
				       fFEPlate_OuterRadius,
				       fFEPlate_Thickness/2.,
				       0,
				       2*M_PI);

  G4VSolid*fFEPlate_CutOut_Solid = new G4Tubs("FEPlate_CutOut_Solid",
				       fFEPlate_InnerRadius,
				       fFEPlate_OuterRadius-fFEPlate_RingThickness,
				       fFEPlate_Thickness/2.,
				       0,
				       2*M_PI/3 - 2*M_PI/36);

  G4VSolid* current_FEPlate_solid = fFEPlate_Solid;
  G4RotationMatrix* z_rot_FEPlate[3];
  for (int i = 0; i < 3; i++)
  {
    G4VSolid* fFEPlate_nHoles = NULL;
    z_rot_FEPlate[i] = new G4RotationMatrix;
    z_rot_FEPlate[i] -> rotateZ(i*2*M_PI/3.);

    fFEPlate_nHoles = new G4SubtractionSolid("FEPlateSolid",
					     current_FEPlate_solid,
					     fFEPlate_CutOut_Solid,
					     z_rot_FEPlate[i],
					     G4ThreeVector(0.,0.,0.));

    current_FEPlate_solid = fFEPlate_nHoles;
  }
  G4VSolid* fFEPlateSolid = current_FEPlate_solid;

  G4LogicalVolume *fFEPlate_Logical = new G4LogicalVolume(fFEPlateSolid,
							  fMaterialFEPlate,
							  "FEPlate_Logical");

  /*G4VPhysicalVolume* fFEPlate_Physical = */
    new G4PVPlacement(0,
                      G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z() + fShroud_CenterHeight/2. + fShroud_TubsHeight - fCableBand_Length- fFEPlate_Thickness/2.),
                      fFEPlate_Logical,
                      "FEPlate_Physical",
                      fMotherLogical,
                      false,
                      0);

  /// CC3 substrates + HVConnectorPlates
  G4Material *fMaterialCC3 = G4Material::GetMaterial("Teflon");

  G4VSolid*fCC3_Solid = new G4Box("CC3_Solid",
				  CC3_SubstrateX*mm/2.,
				  CC3_SubstrateThickness*mm/2.,
				  CC3_SubstrateZ*mm/2.);

  G4LogicalVolume *fCC3_Logical = new G4LogicalVolume(fCC3_Solid,
						      fMaterialCC3,
						      "CC3_Logical");

  G4RotationMatrix* z_rot_CC3[6], *z_rot_HVConnectorPlate[6];
  char CC3_Physical[50];
  char HVConnectorPlate_Physical[50];
  for (int i = 0; i < 6; i++)
  {
    sprintf(CC3_Physical,"CC3_Physical_%d", i);
    sprintf(HVConnectorPlate_Physical,"HVConnectorPlate_Physical_%d", i);
    z_rot_CC3[i] = new G4RotationMatrix;
    z_rot_CC3[i] -> rotateZ(-i*2*M_PI/6.);
    z_rot_HVConnectorPlate[i] = new G4RotationMatrix;
    z_rot_HVConnectorPlate[i] -> rotateZ(-2*M_PI/12.*(i*2 + 1));
    new G4PVPlacement(z_rot_CC3[i],
		      G4ThreeVector(fDetCenter.x()+fFEPlate_OuterRadius*cos((G4double)i*360*deg/6.), fDetCenter.y() + fFEPlate_OuterRadius*sin((G4double)i*360*deg/6.), fDetCenter.z() + fShroud_CenterHeight/2. + fShroud_TubsHeight - fCableBand_Length- fFEPlate_Thickness - CC3_SubstrateZ/2*mm),
		      fCC3_Logical,
		      CC3_Physical,
		      fMotherLogical,
		      false,
		      0);

    new G4PVPlacement(z_rot_HVConnectorPlate[i],
		      G4ThreeVector(fDetCenter.x()+fFEPlate_OuterRadius*cos((G4double)i*360*deg/6. + 30.*deg), fDetCenter.y() + fFEPlate_OuterRadius*sin((G4double)i*360*deg/6. + 30*deg), fDetCenter.z() + fShroud_CenterHeight/2. + fShroud_TubsHeight - fCableBand_Length- fFEPlate_Thickness - CC3_SubstrateZ/2*mm),
		      fCC3_Logical,
		      HVConnectorPlate_Physical,
		      fMotherLogical,
		      false,
		      0);
  }

  // Attach visualization attributes to the the cable bands
  G4VisAttributes* fCableVisAtt = new G4VisAttributes(dark_gray);
  fCableVisAtt -> SetVisibility(true);
  fCableVisAtt -> SetForceSolid(false);
  fCableLoop_Logical-> SetVisAttributes(fCableVisAtt);
  fCableBand_Logical-> SetVisAttributes(fCableVisAtt);
  fFEPlate_Logical-> SetVisAttributes(fCableVisAtt);
  fCC3_Logical-> SetVisAttributes(fCableVisAtt);
}


// TODO: for the moment just commented out... later cables have to be constructed via the dedicated cable class
// HV cables for Phase II BEGe b2b + coax configuration together with LAr instrumentation

// void GEGeometryLArInstHybrid::ConstructHVCables() {
//   G4Colour lblue	(0.0,  0.0, 0.4) ; //TODO move to general declaration
//   G4VisAttributes*fTeflonVisAtt = new G4VisAttributes(lblue);
//   fTeflonVisAtt -> SetVisibility(true);
//   fTeflonVisAtt -> SetForceSolid(true);
//
//
//   G4double fHVCable_Height = (800)*mm; // string height of the 4 pairs of BeGe's + 400 mm to reacht the CC3's
//   G4double fHVCable_Thickness = 1*mm;
//   G4double fHVCable_Width = 7*mm;
//
//   std::vector<G4LogicalVolume*> fHVCable_BEGe_Logical;
//   std::vector<G4VPhysicalVolume*> fHVCable_BEGe_Physical;
//   G4ThreeVector position;
//   std::vector<G4ThreeVector>* fHVCable_BEGe_Position = new std::vector<G4ThreeVector>();
//
//   G4double nCableStrings = 0;
//   for( G4int ii=0; ii<19; ii++ ) {
//     GEGeometryDetectorParameters* dp(0);
//     if(fDetectorDB->GetCrystalType(ii,0)>=0)
//     	dp = fDetectorDB->GetDetectorParameters( fDetectorDB->GetCrystalType(ii,0) );
//     if(dp!=0) {
//       //how many strings are inserted
//       nCableStrings++;
//
//       G4double  zb = 0, zMin = 1e9;
//       for(G4int jj=0; jj<5; jj++ ) {
//     	zb = fDetectorDB->GetCrystalPosition(ii, jj).getZ();
//     	zb -= 0.5*dp->GetHeight();
//
//     	  if(zb<zMin){
//     	    zMin = zb;
//
//     	    position = fDetectorDB->GetCrystalPosition( ii, jj );
//     	    position.setZ( zMin + 0.5*fHVCable_Height);
//     	    MGLog(debugging) << "string ii = " << ii << " detector position jj= " << jj << endlog;
//     	    MGLog(debugging) << "lower crystal surface " << position.x() << "\t" << position.y() << "\t" << position.z()  << endlog;
//     	  }
//     	  G4double yb = fDetectorDB->GetCrystalPosition(ii, jj).getY();
//     	  position.setY( yb + 46*mm); //Radius TODO
//       }
//       MGLog(debugging) << "Cable position lower end " << position.x() << "\t" << position.y() << "\t" << position.z()  << endlog;
//       fHVCable_BEGe_Position->push_back( position );
//     }
//   }
//
//   MGLog(trace) << "# filled strings (Lock_DN500) " << nCableStrings << endlog;
//
//   G4Box* fHVCable_BEGe_b2b = new G4Box("fHVCable_BEGe_b2b",
// 				       0.5*fHVCable_Width,
// 				       0.5*fHVCable_Thickness,
// 				       0.5*fHVCable_Height);
//
//   for( G4int ii=0; ii<nCableStrings; ii++ ){
//     char index[5];
//     sprintf( index, "%i", ii );
//
//     std::string *LogicalVolumeName = new std::string( "HVCableLogical_" );
//     LogicalVolumeName->append( index );
//
//     fHVCable_BEGe_Logical.push_back( new G4LogicalVolume( fHVCable_BEGe_b2b,
// 							  G4Material::GetMaterial("Teflon"),
// 							  LogicalVolumeName->c_str() ) );
//
//     fTeflonVisAtt -> SetVisibility(true);
//     fHVCable_BEGe_Logical.at(ii)->SetVisAttributes( fTeflonVisAtt );
//
//     std::string *PhysicalVolumeName = new std::string( "HVCable_BEGe_" );
//     PhysicalVolumeName->append( index );
//
//
//     fHVCable_BEGe_Physical.push_back(new G4PVPlacement( 0,
// 							 fHVCable_BEGe_Position->at(ii),
// 							 fHVCable_BEGe_Logical.at(ii),
// 							 PhysicalVolumeName->c_str() ,
// 							 fMotherLogical,
// 							 false,
// 							 0) );
//   }
// }


void GEGeometryLArInstHybrid::ConstructDesign() {

  // Define materials that are common to the different parts and associante the corresponding
  // optical properties

  // Define the WLS: TPB
  G4String name = "TPB"; // Tetraphenyl butadiene

  fTPB = G4Material::GetMaterial(name, false);

  if (fTPB == 0) {
    G4NistManager* nist = G4NistManager::Instance();

    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");

    fTPB= new G4Material(name, 1*g/cm3, 2, kStateSolid);
    fTPB->AddElement(H, 22);
    fTPB->AddElement(C, 28);
  }
  // Now attach the optical properties to it.
  G4int ji = 0;
  G4double ee=0;
  // Build table with photon energies
  const G4int num = 500;

  G4double PPSCHighE = LambdaE /(115*nanometer);
  G4double PPSCLowE = LambdaE /(650*nanometer); //598
  G4double dee = ((PPSCHighE - PPSCLowE) / ((G4double)(num-1)));
  G4double LAr_SCPP[num];
  for (ji = 0; ji < num; ji++)
  {
    ee=PPSCLowE+ ((G4double)ji) * dee;
    LAr_SCPP[ji]=ee;
  }
  G4double WLS_absorption[num];
  G4double WLS_emission[num];
  G4double Refraction[num];
  G4MaterialPropertiesTable *tpbTable = new G4MaterialPropertiesTable();
  // The refractive index of the TPB should be roughly the same as acrylic
  // The polystirene has very similar properties
  // Use the same value as the fibers
  for (ji=0;ji < num; ji++)
  {
    Refraction[ji] = 1.6; //this is just a guess
    // Should the TPB shift the Cherenkov light?
    // This makes a tail starting at 128 until the visible.
    if (LAr_SCPP[ji] > 3.31*eV)
      // For the moment set it to always absorb photons
      WLS_absorption[ji] = 0.001*nm; //absorbs UV (always)
    else
      WLS_absorption[ji] = 10.5*m; //otherwise transparent

    WLS_emission[ji] = TPBEmissionSpectrum(LAr_SCPP[ji]);
    /*
		MGLog(debugging) << "TPB:  WL: " << (LambdaE/LAr_SCPP[ji])/nanometer<< " nm Energy :" << LAr_SCPP[ji]/eV << " eV Emission : " <<
				WLS_emission[ji] << " Absorption : " << WLS_absorption[ji]/m << " m."<< endlog;
     */
  }
  tpbTable->AddProperty("RINDEX",LAr_SCPP,Refraction,num);
  tpbTable->AddProperty("WLSABSLENGTH",LAr_SCPP,WLS_absorption,num);
  tpbTable->AddProperty("WLSCOMPONENT",LAr_SCPP,WLS_emission,num);
  // From WArP
  tpbTable->AddConstProperty("WLSTIMECONSTANT", 0.01*ns);
  G4double WLSyield = 1.2;
  tpbTable->AddConstProperty("WLSMEANNUMBERPHOTONS",WLSyield);
  tpbTable->AddConstProperty("SCINTILLATIONYIELD",fFiberLightYield / MeV); // limit scint. phot. JJ
  fTPB->SetMaterialPropertiesTable(tpbTable);


  // Define a rough optical surface to be used in the interface between WLS and LAr
  // 50% roughness in the surface
  // This surface will be attached between the WLS and the LAr in all instances
  G4double roughness = 0.5;
  fWLSoptSurf = new G4OpticalSurface("WLS_rough_surf",glisur,ground,dielectric_dielectric,roughness);

  // Create a similar surface for Teflon parts
  fTefOptSurf = new G4OpticalSurface("Teflon_rough_surface",glisur,ground,dielectric_metal,roughness);
  // Attach the properties that are currently attached to the Teflon material
  fTefOptSurf->SetMaterialPropertiesTable(G4Material::GetMaterial("Teflon")->GetMaterialPropertiesTable());


  ConstructPMTSubDesign();
  ConstructFiberSubDesign();

  ConstructCable_FE_Stuff();
//   ConstructHVCables();
}

G4LogicalVolume *GEGeometryLArInstHybrid::GetLArInstrSDLogical(G4int index) {
  return fSDLogicVec.at(index);
}
G4int GEGeometryLArInstHybrid::GetNumSDLogicVols() {
  return fSDLogicVec.size();
}
G4String GEGeometryLArInstHybrid::GetLogicalSDName(G4int index) {
  return fSDLogicNameVec.at(index);
}
G4int GEGeometryLArInstHybrid::GetNLogicalInstances(G4int index) {
  return fSDLogicInstanceVec.at(index);
}

//copied from GEGSLArGeOptical.cc
void GEGeometryLArInstHybrid::InitializeTPBSpectra()
{
  char* path = getenv("MGGENERATORDATA");
  if (!path)
  {
    MGLog(warning)<< "MGGENERATORDATA environment variable not set!" << endlog;
    path = (char*) ".";
  }
  G4String pathString(path);
  G4String pathFile = pathString + "/VM2000_em_spec.dat";
  fTPBspec = new TGraph(pathFile.data());
  if (fTPBspec->GetN() > 0 ) {
    successfulInitialization = true;
    MGLog(routine) << "TPB re-emission spectrum ( " << fTPBspec->GetN()
							      << " points) successfully loaded from file." << endlog;
    /** Uncomment for debugging purposes
		fVM2000spec->Print();
     */
  } else {
    successfulInitialization = false;
    MGLog(warning) << "TPB re-emission spectrum failed to load from [" << pathFile.data() << "]." << endlog;
    MGLog(warning) << "Cross-check that this is intended... otherwise the output won't be reliable." << endlog;
  }
}


//copied from GEGSLArGeOptical.cc
void GEGeometryLArInstHybrid::InitializeFiberSpectra()
{
  char* path = getenv("MGGENERATORDATA");
  if (!path)
  {
    MGLog(warning)<< "MGGENERATORDATA environment variable not set!" << endlog;
    path = (char*) ".";
  }
  G4String pathString(path);
  G4String pathFile = pathString + "/FibersAbsorption.dat";
  fFibersAbsorptionSpec = new TGraph(pathFile.data(),"%lg,%lg");
  if (fFibersAbsorptionSpec->GetN() > 0 ) {
    MGLog(routine) << "Fibers absorption spectrum ( " << fFibersAbsorptionSpec->GetN()
                    << " points) successfully loaded from file." << endlog;
    /** Uncomment for debugging purposes
    fFibersAborptionspec->Print();
     */
  }
  else {

    MGLog(warning) << "Fiber WLS absorption spectrum failed to load from [" << pathFile.data() << "]." << endlog;
    MGLog(warning) << "Cross-check that this is intended... otherwise the output won't be reliable." << endlog;
    G4Exception("GEGeometryLArInstHybrid::InitializeFiberSpectra",
                "MissingInputData",
                FatalException,
                "Fiber WLS absorption spectrum not found.");
  }

  pathFile = pathString + "/FibersEmission.dat";
  fFibersEmissionSpec = new TGraph(pathFile.data(),"%lg,%lg");
  if (fFibersEmissionSpec->GetN() > 0 ) {
    MGLog(routine) << "Fibers emission spectrum ( " << fFibersEmissionSpec->GetN()
                      << " points) successfully loaded from file." << endlog;
    /** Uncomment for debugging purposes
      fFibersEmissionSpec->Print();
     */

  } else {
    MGLog(warning) << "Fiber re-emission spectrum failed to load from [" << pathFile.data() << "]." << endlog;
    MGLog(warning) << "Cross-check that this is intended... otherwise the output won't be reliable." << endlog;
    G4Exception("GEGeometryLArInstHybrid::InitializeFiberSpectra",
                "MissingInputData",
                FatalException,
                "Fiber WLS re-emission spectrum not found.");
  }
  successfulInitialization = true;
}

//copied from GEGSLArGeOptical.cc
G4double GEGeometryLArInstHybrid::TPBEmissionSpectrum(G4double energy)
{
  if (successfulInitialization)
  {
    //Here the data are loaded
    G4double targetf = LambdaE/energy;

    G4double eff =fTPBspec->Eval(targetf/nm);
    if (eff < 0.2) eff = 0.2;
    //MGLog(routine) << "Eval ("<< targetf/nm<< ")yielded a value of " << eff << endlog;
    return eff;
  }
  else
  {
    return 0.2;
  }
}

void GEGeometryLArInstHybrid::ConstructPMTSubDesign() {

  /// Temporary variable to use in the WLS volumes to attach the rough surfaces
  G4VPhysicalVolume* wlsPhysical = NULL;

  /// Define some colors that will be used later
  //visualization attributes
  G4Colour lgreen	(0.0,  0.4, 0.0) ;
  G4Colour lblue	(0.0,  0.0, 0.4) ;
  G4Colour llblue	(0.,  0.0, 0.04) ;
  G4Colour blue_gray	(175/255. ,157/255. ,189/255. ) ;
  G4Colour lred		(0.4,  0.0, 0.0);
  G4Colour light_gray(214./255.,214./255.,214./255.);
  G4Colour dark_gray(135./255.,135./255.,135./255.);



  // Taken as implemented in the original PMT design.
  // Only difference being the removal of the central shroud tube.
  /// -- Define the necessary dimensions... below these declarations everything is calculated automatically
  G4double Shroud_TubsInnerRadius	= fShroud_TubsOuterRadius - fShroud_Thickness;

  G4double VM2000_TubsOuterRadius	= Shroud_TubsInnerRadius;
  G4double VM2000_TubsInnerRadius	= VM2000_TubsOuterRadius - fVM2000_Thickness;

  G4double WLS_TubsOuterRadius		= VM2000_TubsInnerRadius;
  G4double WLS_TubsInnerRadius		= WLS_TubsOuterRadius - fWLSThickness;

  G4double CopperPlate_OuterRadius	= fShroud_TubsOuterRadius; 	//235.0*mm;
  G4double CopperPlate_InnerRadius	= 0.0*mm;
  G4double CopperPlate_Height		= 4*mm;

  G4double WLS_Plate_InnerRadius	= 0.0*mm;
  G4double WLS_Plate_OuterRadius	= fShroud_TubsOuterRadius; 	//235.0*mm;

  G4double PMT_Radius			= (3*25.4)/2.*mm;
  G4double PMT_Height			= 123.*mm;
  G4double PMT_MountingRadius_top_inner	= 230.0*mm/2;
  G4double PMT_MountingRadius_top_outer	= 360.0*mm/2;
  G4double PMT_MountingRadius_bottom	= 190.0*mm/2;

  G4double PMTHousing_Thickness		= 0.6*mm;
  G4double PMTGlass_Thickness		= 3.4*mm;

  G4double calhole_Radius		= 380.0*mm/2;

  G4RotationMatrix* z_rot_PMTInst = new G4RotationMatrix;
  z_rot_PMTInst->rotateZ(-fRotatePMTInstAngle/(360*deg)*M_PI*2);

  //Copper shroud --
  /// N.F.B. : In the hybrid instrumentation, the shroud is split in three parts
  /// the top and bottom are made of copper, while the central is made of fibers
  G4VSolid*fShroudSolid = new G4Tubs("PMTInstr_ShroudSolid",
                                     Shroud_TubsInnerRadius,
                                     fShroud_TubsOuterRadius,
                                     fShroud_TubsHeight/2.,
                                     0,
                                     2*M_PI);


  G4Material *fMaterialShroud = G4Material::GetMaterial("MetalCopper");

  fShroudLogical = new G4LogicalVolume(fShroudSolid,
                                       fMaterialShroud,
                                       "LArInstr_PMTShroudLogical_top");

  fShroudPhysical_top = new G4PVPlacement(0,
                                          G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z() + fShroud_Offset),
                                          fShroudLogical,
                                          "LArInstr_PMTShroudPhysical_top",
                                          fMotherLogical,
                                          false,
                                          0);

  fShroudPhysical_bottom = new G4PVPlacement(0,
                                             G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z() - fShroud_Offset),
                                             fShroudLogical,
                                             "LArInstr_PMTShroudPhysical_bottom",
                                             fMotherLogical,
                                             false,
                                             1);

  // Attach visualization attributes to the cylinders
  G4VisAttributes* fShroudVisAtt = new G4VisAttributes(lgreen);
  fShroudVisAtt -> SetVisibility(true);
  fShroudVisAtt -> SetForceSolid(false);
  fShroudLogical-> SetVisAttributes(fShroudVisAtt);



  //VM2000 reflector foil inside the copper shroud
  // Repeat the procedure as for the copper cylinder
  G4VSolid* fVM2000CylinderSolid = new G4Tubs("LArInstr_VM2000CylinderSolid",
                                              VM2000_TubsInnerRadius,
                                              VM2000_TubsOuterRadius,
                                              fShroud_TubsHeight/2.,
                                              0,
                                              2*M_PI);
  G4Material*fMaterialVM2000 = G4Material::GetMaterial("VM2000");

  G4LogicalVolume* fVM2000CylinderLogical = new G4LogicalVolume(fVM2000CylinderSolid,
                                                                fMaterialVM2000,
                                                                "LArInstr_VM2000CylinderLogical");

  new G4PVPlacement(0,
                    G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z() + fShroud_Offset),
                    fVM2000CylinderLogical,
                    "LArInstr_VM2000CylinderPhysical_top",
                    fMotherLogical,
                    false,
                    0);

  new G4PVPlacement(0,
                    G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z() - fShroud_Offset),
                    fVM2000CylinderLogical,
                    "LArInstr_VM2000CylinderPhysical_bottom",
                    fMotherLogical,
                    false,
                    1);

  // Visualization
  G4VisAttributes* fVM2000VisAtt = new G4VisAttributes(lblue);
  fVM2000VisAtt -> SetVisibility(true);
  fVM2000VisAtt -> SetForceSolid(false);
  fVM2000CylinderLogical-> SetVisAttributes(fVM2000VisAtt);


  //WLS coating on VM2000 foil

  G4VSolid* fWLSCylinderSolid = new G4Tubs("LArInstr_WLSCylinderSolid",
                                           WLS_TubsInnerRadius,
                                           WLS_TubsOuterRadius,
                                           fShroud_TubsHeight/2.,
                                           0,
                                           2*M_PI);
  G4Material* fMaterialWLS = fTPB; //G4Material::GetMaterial("Nylon");

  G4LogicalVolume* fWLSCylinderLogical = new G4LogicalVolume(fWLSCylinderSolid,
                                                             fMaterialWLS,
                                                             "LArInstr_WLSCylinderLogical");

  wlsPhysical = new G4PVPlacement(0,
                                  G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z() + fShroud_Offset),
                                  fWLSCylinderLogical,
                                  "LArInstr_WLSCylinderPhysical_top",
                                  fMotherLogical,
                                  false,
                                  0);

  new G4LogicalBorderSurface("LArInstr_WLSCylinderPhysical_top_surf_in",fMotherPhysical,wlsPhysical,fWLSoptSurf);
  new G4LogicalBorderSurface("LArInstr_WLSCylinderPhysical_top_surf_out",wlsPhysical,fMotherPhysical,fWLSoptSurf);

  wlsPhysical = new G4PVPlacement(0,
                                  G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z() - fShroud_Offset),
                                  fWLSCylinderLogical,
                                  "LArInstr_WLSCylinderPhysical_bottom",
                                  fMotherLogical,
                                  false,
                                  0);

  // consider the surface rough from both sides
  new G4LogicalBorderSurface("LArInstr_WLSCylinderPhysical_bottom_surf_in",fMotherPhysical,wlsPhysical,fWLSoptSurf);
  new G4LogicalBorderSurface("LArInstr_WLSCylinderPhysical_bottom_surf_out",wlsPhysical,fMotherPhysical,fWLSoptSurf);


  G4VisAttributes* fWLSVisAtt = new G4VisAttributes(llblue);
  fWLSVisAtt -> SetVisibility(true);
  fWLSVisAtt -> SetForceSolid(false);
  fWLSCylinderLogical-> SetVisAttributes(fWLSVisAtt);


  ///  Copper plates with holes for the PMTs + WLS coating
  ///  -- Bottom plate --
  ///  1 hole in the center, 6 holes distributed uniformly on a circle with radius "PMT_MountingRadius_bottom"
  ///
  // Define the generic solids from which the volumes are built
  G4VSolid* fCopperPlateSolid = new G4Tubs("CopperPlateSolid",
                                           CopperPlate_InnerRadius,
                                           CopperPlate_OuterRadius,
                                           CopperPlate_Height/2.,
                                           0,
                                           2*M_PI);

  G4VSolid* fCopperPlate_hole = new G4Tubs("CopperPlate_hole",
                                           0.,
                                           PMT_Radius,
                                           CopperPlate_Height/2.,
                                           0,
                                           2*M_PI);

  //hole for calibration sources
  G4VSolid* fCopperPlate_calhole = new G4Tubs("CopperPlate_calhole",
                                           0.,
                                           70./2.*mm,
                                           CopperPlate_Height/2.,
                                           0,
                                           2*M_PI);

  //rectangular cut out for top plate
  G4VSolid* fCopperPlate_cutRectangle = new G4Box("CopperPlate_cutRectangle",
                                                   32./2.*mm,
                                                   105./2.*mm,
                                                   CopperPlate_Height/2.);

  G4VSolid* fVM2000PlateSolid = new G4Tubs("VM2000PlateSolid",
                                           CopperPlate_InnerRadius,
                                           CopperPlate_OuterRadius,
                                           fVM2000_Thickness/2.,
                                           0,
                                           2*M_PI);

  G4VSolid* fVM2000Plate_hole = new G4Tubs("VM2000Plate_hole",
                                           0.,
                                           PMT_Radius,
                                           fVM2000_Thickness/2.,
                                           0,
                                           2*M_PI);

  G4VSolid* fVM2000Plate_calhole = new G4Tubs("VM2000Plate_calhole",
                                           0.,
                                           70./2.*mm,
                                           fVM2000_Thickness/2.,
                                           0,
                                           2*M_PI);

  G4VSolid* fVM2000Plate_cutRectangle = new G4Box("VM2000Plate_cutRectangle",
                                                   32./2.*mm,
                                                   105./2.*mm,
                                                   fVM2000_Thickness/2.);

  G4VSolid* fWLS_PlateSolid = new G4Tubs("WLSPlateSolid",
                                         WLS_Plate_InnerRadius,
                                         WLS_Plate_OuterRadius,
                                         fWLSThickness/2.,
                                         0,
                                         2*M_PI);

  G4VSolid* fWLS_Plate_hole = new G4Tubs("WLSPlate_hole",
                                         0.,
                                         PMT_Radius,
                                         fWLSThickness/2.,
                                         0,
                                         2*M_PI);

  G4VSolid* fWLS_Plate_calhole = new G4Tubs("WLSPlate_calhole",
                                         0.,
                                         70./2.*mm,
                                         fWLSThickness/2.,
                                         0,
                                         2*M_PI);

//   G4VSolid* fWLS_Plate_cutRectangle = new G4Box("WLSPlate_cutRectangle",
//                                                 32./2.*mm,
//                                                 105./2.*mm,
//                                                 fWLSThickness/2.); //FIXME can't be visualized

  G4VSolid* fWLS_Plate_cutEllipse = new G4EllipticalTube("WLSPlate_cutEllipse",
                                                52./2.*mm,
                                                135./2.*mm,
                                                fWLSThickness/2.);


  // Virtual pointers to build the interim solids
  G4VSolid* current_CopperPlate1_solid = fCopperPlateSolid;
  G4VSolid* current_VM2000Plate1_solid = fVM2000PlateSolid;
  G4VSolid* current_WLSPlate1_solid = fWLS_PlateSolid;
  // Construct the bottom plate
  for (int i = 0; i < 7; i++)
  {
    G4VSolid* fCopperPlate1_nHoles = NULL;
    G4VSolid* fVM2000Plate1_nHoles = NULL;
    G4VSolid* fWLSPlate1_nHoles = NULL;

    if(i== 0){
      fCopperPlate1_nHoles = new G4SubtractionSolid("LArInstr_CopperPlateSolid_bottom",
                                                    current_CopperPlate1_solid,
                                                    fCopperPlate_hole,
                                                    0,
                                                    G4ThreeVector(0., 0., 0.));

      fVM2000Plate1_nHoles = new G4SubtractionSolid("LArInstr_VM2000PlateSolid_bottom",
                                                    current_VM2000Plate1_solid,
                                                    fVM2000Plate_hole,
                                                    0,
                                                    G4ThreeVector(0., 0., 0.));

      fWLSPlate1_nHoles = new G4SubtractionSolid("LArInstr_WLSPlateSolid_bottom",
                                                 current_WLSPlate1_solid,
                                                 fWLS_Plate_hole,
                                                 0,
                                                 G4ThreeVector(0., 0., 0.));
    }
    else {
      fCopperPlate1_nHoles = new G4SubtractionSolid("LArInstr_CopperPlateSolid_bottom",
                                                    current_CopperPlate1_solid,
                                                    fCopperPlate_hole,
                                                    0,
                                                    G4ThreeVector((PMT_MountingRadius_bottom*cos(((G4double)i -1)*360*deg/6.)), (PMT_MountingRadius_bottom*sin(((G4double)i -1)*360*deg/6.)), 0.) );

      fVM2000Plate1_nHoles = new G4SubtractionSolid("LArInstr_VM2000PlateSolid_bottom",
                                                    current_VM2000Plate1_solid,
                                                    fVM2000Plate_hole,
                                                    0,
                                                    G4ThreeVector((PMT_MountingRadius_bottom*cos(((G4double)i -1)*360*deg/6.)), (PMT_MountingRadius_bottom*sin(((G4double)i -1)*360*deg/6.)), 0.) );

      fWLSPlate1_nHoles = new G4SubtractionSolid("LArInstr_WLSPlateSolid_bottom",
                                                 current_WLSPlate1_solid,
                                                 fWLS_Plate_hole,
                                                 0,
                                                 G4ThreeVector((PMT_MountingRadius_bottom*cos(((G4double)i -1)*360*deg/6.)), (PMT_MountingRadius_bottom*sin(((G4double)i -1)*360*deg/6.)), 0.) );
    }
    current_CopperPlate1_solid = fCopperPlate1_nHoles;
    current_VM2000Plate1_solid = fVM2000Plate1_nHoles;
    current_WLSPlate1_solid = fWLSPlate1_nHoles;
  }
  // Build the composite (subtraction) solids

  G4VSolid* fCopperPlateSolid_bottom = current_CopperPlate1_solid;
  G4VSolid* fVM2000PlateSolid_bottom = current_VM2000Plate1_solid;
  G4VSolid* fWLSPlateSolid_bottom = current_WLSPlate1_solid;

  G4Material* fMaterialCopperPlate = G4Material::GetMaterial("MetalCopper");
  G4LogicalVolume *fCopperPlateLogical_bottom = new G4LogicalVolume(fCopperPlateSolid_bottom,
                                                                    fMaterialCopperPlate,
                                                                    "LArInstr_CopperPlateLogical_bottom");

  MGLog(debugging) << "Placing bottom PMTs at " << fDetCenter.x() << " " << fDetCenter.y() << " " << (fDetCenter.z()-fShroud_TubsHeight/2. -fShroud_Offset) << endlog;
  new G4PVPlacement(z_rot_PMTInst/*0*/,
                    G4ThreeVector(fDetCenter.x(),fDetCenter.y(),(fDetCenter.z()-fShroud_TubsHeight/2. -fShroud_Offset - fWLSThickness - fVM2000_Thickness - CopperPlate_Height/2.)),
                    fCopperPlateLogical_bottom,
                    "LArInstr_CopperPlatePhysical_bottom",
                    fMotherLogical,
                    false,
                    0);

  G4LogicalVolume *fVM2000PlateLogical_bottom = new G4LogicalVolume(fVM2000PlateSolid_bottom,
                                                                    fMaterialVM2000,
                                                                    "LArInstr_VM2000PlateLogical_bottom");
  new G4PVPlacement(z_rot_PMTInst/*0*/,
                   G4ThreeVector(fDetCenter.x(),fDetCenter.y(),(fDetCenter.z()-fShroud_TubsHeight/2. - fShroud_Offset - fWLSThickness - fVM2000_Thickness/2.)),
                   fVM2000PlateLogical_bottom,
                   "LArInstr_VM2000PlatePhysical_bottom",
                   fMotherLogical,
                   false,
                   0);

  G4LogicalVolume *fWLSPlateLogical_bottom = new G4LogicalVolume(fWLSPlateSolid_bottom,
                                                                 fMaterialWLS,
                                                                 "LArInstr_WLSPlateLogical_bottom");
  wlsPhysical = new G4PVPlacement(z_rot_PMTInst/*0*/,
                                  G4ThreeVector(fDetCenter.x(),fDetCenter.y(),(fDetCenter.z()-fShroud_TubsHeight/2. - fShroud_Offset - fWLSThickness/2.)),
                                  fWLSPlateLogical_bottom,
                                  "LArInstr_WLSPlatePhysical_bottom",
                                  fMotherLogical,
                                  false,
                                  0);

  // consider the surface rough from both sides
  new G4LogicalBorderSurface("LArInstr_WLSPlatePhysical_bottom_surf_in",
                             fMotherPhysical,
                             wlsPhysical,
                             fWLSoptSurf);

  new G4LogicalBorderSurface("LArInstr_WLSPlatePhysical_bottom_surf_out",
                             wlsPhysical,
                             fMotherPhysical,
                             fWLSoptSurf);


  ///
  /// -- Top plate --
  ///
  /// 3 PMTs distributed uniformly on a circle with radius "PMT_MountingRadius_top_inner" => alpha= 120°
  /// 6 PMTs distributed in pairs on a circle with radius "PMT_MountingRadius_top_outer" => p= i*alpha(=120°) +-25°  + WLS coating
  /// 1 cut out in the center for cable chain and 3 holes for calibration source

  G4VSolid* current_CopperPlateTop_solid = fCopperPlateSolid;
  G4VSolid* current_VM2000PlateTop_solid = fVM2000PlateSolid;
  G4VSolid* current_WLSPlateTop_solid = fWLS_PlateSolid;
  for (int i = 0; i < 13; i++) {
    G4VSolid* fCopperPlate2_nHoles=NULL;
    G4VSolid* fVM2000Plate2_nHoles=NULL;
    G4VSolid* fWLSPlate2_nHoles=NULL;

    if(i < 3){
    fCopperPlate2_nHoles = new G4SubtractionSolid("LArInstr_CopperPlateSolid_top",
                                                  current_CopperPlateTop_solid,
                                                  fCopperPlate_hole,
                                                  0,
                                                  G4ThreeVector(PMT_MountingRadius_top_inner*cos((G4double)i*360*deg/3.), PMT_MountingRadius_top_inner*sin((G4double)i*360*deg/3.), 0.) );

    fVM2000Plate2_nHoles = new G4SubtractionSolid("LArInstr_VM2000PlateSolid_top",
                                                  current_VM2000PlateTop_solid,
                                                  fVM2000Plate_hole,
                                                  0,
                                                  G4ThreeVector(PMT_MountingRadius_top_inner*cos((G4double)i*360*deg/3.), PMT_MountingRadius_top_inner*sin((G4double)i*360*deg/3.), 0.) );

    fWLSPlate2_nHoles = new G4SubtractionSolid("LArInstr_WLSPlate2Logical",
                                               current_WLSPlateTop_solid,
                                               fWLS_Plate_hole,
                                               0,
                                               G4ThreeVector(PMT_MountingRadius_top_inner*cos((G4double)i*360*deg/3.), PMT_MountingRadius_top_inner*sin((G4double)i*360*deg/3.), 0.) );
    }
    else if(i>2 && i<6){
    fCopperPlate2_nHoles = new G4SubtractionSolid("LArInstr_CopperPlateSolid_top",
                                                  current_CopperPlateTop_solid,
                                                  fCopperPlate_hole,
                                                  0,
                                                  G4ThreeVector(PMT_MountingRadius_top_outer*cos(((G4double)i-3)*360*deg/3.-25*deg), PMT_MountingRadius_top_outer*sin(((G4double)i-3)*360*deg/3.-25*deg), 0.) );

    fVM2000Plate2_nHoles = new G4SubtractionSolid("LArInstr_VM2000PlateSolid_top",
                                                  current_VM2000PlateTop_solid,
                                                  fVM2000Plate_hole,
                                                  0,
                                                  G4ThreeVector(PMT_MountingRadius_top_outer*cos(((G4double)i-3)*360*deg/3.-25*deg), PMT_MountingRadius_top_outer*sin(((G4double)i-3)*360*deg/3.-25*deg), 0.) );

    fWLSPlate2_nHoles = new G4SubtractionSolid("LArInstr_WLSPlate2Logical",
                                               current_WLSPlateTop_solid,
                                               fWLS_Plate_hole,
                                               0,
                                               G4ThreeVector(PMT_MountingRadius_top_outer*cos(((G4double)i-3)*360*deg/3.-25*deg), PMT_MountingRadius_top_outer*sin(((G4double)i-3)*360*deg/3.-25*deg), 0.) );
    }
    else if(i>5 && i<9){
    fCopperPlate2_nHoles = new G4SubtractionSolid("LArInstr_CopperPlateSolid_top",
                                                  current_CopperPlateTop_solid,
                                                  fCopperPlate_hole,
                                                  0,
                                                  G4ThreeVector(PMT_MountingRadius_top_outer*cos(((G4double)i-6)*360*deg/3.+25*deg), PMT_MountingRadius_top_outer*sin(((G4double)i-6)*360*deg/3.+25*deg), 0.) );

    fVM2000Plate2_nHoles = new G4SubtractionSolid("LArInstr_VM2000PlateSolid_top",
                                                  current_VM2000PlateTop_solid,
                                                  fVM2000Plate_hole,
                                                  0,
                                                  G4ThreeVector(PMT_MountingRadius_top_outer*cos(((G4double)i-6)*360*deg/3.+25*deg), PMT_MountingRadius_top_outer*sin(((G4double)i-6)*360*deg/3.+25*deg), 0.) );

    fWLSPlate2_nHoles = new G4SubtractionSolid("LArInstr_WLSPlate2Logical",
                                               current_WLSPlateTop_solid,
                                               fWLS_Plate_hole,
                                               0,
                                               G4ThreeVector(PMT_MountingRadius_top_outer*cos(((G4double)i-6)*360*deg/3.+25*deg), PMT_MountingRadius_top_outer*sin(((G4double)i-6)*360*deg/3.+25*deg), 0.) );
    }
    else if(i>8 && i<12){
    fCopperPlate2_nHoles = new G4SubtractionSolid("LArInstr_CopperPlateSolid_top",
                                                  current_CopperPlateTop_solid,
                                                  fCopperPlate_calhole,
                                                  0,
                                                  G4ThreeVector(calhole_Radius*cos(((G4double)i-9)*360*deg/3.+60*deg), calhole_Radius*sin(((G4double)i-9)*360*deg/3.+60*deg), 0.) );

    fVM2000Plate2_nHoles = new G4SubtractionSolid("LArInstr_VM2000PlateSolid_top",
                                                  current_VM2000PlateTop_solid,
                                                  fVM2000Plate_calhole,
                                                  0,
                                                  G4ThreeVector(calhole_Radius*cos(((G4double)i-9)*360*deg/3.+60*deg), calhole_Radius*sin(((G4double)i-9)*360*deg/3.+60*deg), 0.) );

    fWLSPlate2_nHoles = new G4SubtractionSolid("LArInstr_WLSPlate2Logical",
                                               current_WLSPlateTop_solid,
                                               fWLS_Plate_calhole,
                                               0,
                                               G4ThreeVector(calhole_Radius*cos(((G4double)i-9)*360*deg/3.+60*deg), calhole_Radius*sin(((G4double)i-9)*360*deg/3.+60*deg), 0.) );
    }
    else{
    fCopperPlate2_nHoles = new G4SubtractionSolid("LArInstr_CopperPlateSolid_top",
                                                  current_CopperPlateTop_solid,
                                                  fCopperPlate_cutRectangle,
                                                  0,
                                                  G4ThreeVector(0., 0., 0.) );

    fVM2000Plate2_nHoles = new G4SubtractionSolid("LArInstr_VM2000PlateSolid_top",
                                                  current_VM2000PlateTop_solid,
                                                  fVM2000Plate_cutRectangle,
                                                  0,
                                                  G4ThreeVector(0., 0., 0.) );

    fWLSPlate2_nHoles = new G4SubtractionSolid("LArInstr_WLSPlate2Logical",
					       current_WLSPlateTop_solid,
					       fWLS_Plate_cutEllipse,
					       0,
					       G4ThreeVector(0., 0., 0. ) );
    }
    current_CopperPlateTop_solid = fCopperPlate2_nHoles;
    current_VM2000PlateTop_solid = fVM2000Plate2_nHoles;
    current_WLSPlateTop_solid = fWLSPlate2_nHoles;
  }
  G4VSolid* fCopperPlateSolid_top = current_CopperPlateTop_solid;
  G4VSolid* fVM2000PlateSolid_top = current_VM2000PlateTop_solid;
  G4VSolid* fWLSPlateSolid_top = current_WLSPlateTop_solid;

  G4LogicalVolume* fCopperPlateLogical_top = new G4LogicalVolume(fCopperPlateSolid_top,
                                                                 fMaterialCopperPlate,
                                                                 "LArInstr_CopperPlateLogical_top");
  MGLog(debugging) << "Placing top PMTs at " << fDetCenter.x() << " " << fDetCenter.y() << " " << (fDetCenter.z()+fShroud_TubsHeight/2. + fShroud_Offset+ fWLSThickness + CopperPlate_Height/2.) << endlog;

  new G4PVPlacement(z_rot_PMTInst/*0*/,
                    G4ThreeVector(fDetCenter.x(),fDetCenter.y(),(fDetCenter.z()+fShroud_TubsHeight/2. + fShroud_Offset+ fWLSThickness + fVM2000_Thickness + CopperPlate_Height/2.)),
                    fCopperPlateLogical_top,
                    "LArInstr_CopperPlatePhysical_top",
                    fMotherLogical,
                    false,
                    0);


  /// External source to study backgrounds in the cable chain.
  // Consider simply a 5*mm cylinder of copper placed just above the teflon plate
  // The thickness is to ensure that any beta from the source is killed.
  if (fDetectorDB->GetLArInstConstructExtSource()) {
    G4VSolid *ext_source_outer_solid = new G4Tubs("ExtSourceSolidOuter",
                                                  0.0,
                                                  5*mm,
                                                  5*mm/2.,
                                                  0,
                                                  2*M_PI);

    G4VSolid *ext_source_inner_solid = new G4Tubs("ExtSourceSolidInner",
                                                  0.0,
                                                  1*mm,
                                                  1*mm/2.,
                                                  0,
                                                  2*M_PI);

    G4LogicalVolume* fExtSourceOuterLogical = new G4LogicalVolume(ext_source_outer_solid,
                                                                  G4Material::GetMaterial("MetalCopper"),
                                                                  "LArInstr_ExtSourceOuter_Logical");

    G4LogicalVolume* fExtSourceInnerLogical = new G4LogicalVolume(ext_source_inner_solid,
                                                                  G4Material::GetMaterial("Steel"),
                                                                  "LArInstr_ExtSourceInner_Logical");

    // Now place the source right above the copper plate
    new G4PVPlacement(0,
                      G4ThreeVector(fDetCenter.x(),fDetCenter.y(),(fDetCenter.z()+fShroud_TubsHeight/2. + fShroud_Offset+ fWLSThickness + fVM2000_Thickness + CopperPlate_Height+2.5*mm)),
                      fExtSourceOuterLogical,
                      "LArInstr_ExtSourceOuter_Physical",
                      fMotherLogical,
                      false,
                      0);

    new G4PVPlacement(0,
                      G4ThreeVector(0.0,0.0,0.0),
                      fExtSourceInnerLogical,
                      "LArInstr_ExtSourceInner_Physical",
                      fExtSourceOuterLogical,
                      false,
                      0);

    G4VisAttributes*source_visatt_outer = new G4VisAttributes(light_gray);
    source_visatt_outer -> SetVisibility(true);
    source_visatt_outer -> SetForceSolid(false);
    fExtSourceOuterLogical-> SetVisAttributes(source_visatt_outer);

    G4VisAttributes*source_visatt_inner = new G4VisAttributes(dark_gray);
    source_visatt_inner -> SetVisibility(true);
    source_visatt_inner -> SetForceSolid(false);
    fExtSourceInnerLogical-> SetVisAttributes(source_visatt_inner);

  }
  /// ----

  G4LogicalVolume* fVM2000PlateLogical_top = new G4LogicalVolume(fVM2000PlateSolid_top,
								 fMaterialVM2000,
								 "LArInstr_VM2000PlateLogical_top");
  new G4PVPlacement(z_rot_PMTInst/*0*/,
		    G4ThreeVector(fDetCenter.x(),fDetCenter.y(),(fDetCenter.z()+ fShroud_Offset + fShroud_TubsHeight/2. + fWLSThickness + fVM2000_Thickness/2.)),
		    fVM2000PlateLogical_top,
		    "LArInstr_VM2000PlatePhysical_top",
		    fMotherLogical,
		    false,
		    0);

  G4LogicalVolume* fWLSPlateLogical_top = new G4LogicalVolume(fWLSPlateSolid_top,
                                                              fMaterialWLS,
                                                              "LArInstr_WLSPlateLogical_top");
  new G4PVPlacement(z_rot_PMTInst/*0*/,
		    G4ThreeVector(fDetCenter.x(),fDetCenter.y(),(fDetCenter.z()+ fShroud_Offset + fShroud_TubsHeight/2. + fWLSThickness/2.-1*mm)),
		    fWLSPlateLogical_top,
		    "LArInstr_WLSPlatePhysical_top",
		    fMotherLogical,
		    false,
		    0);

  // consider the surface rough from both sides
  new G4LogicalBorderSurface("LArInstr_WLSPlatePhysical_top_surf_in",
                             fMotherPhysical,
                             wlsPhysical,
                             fWLSoptSurf);

  new G4LogicalBorderSurface("LArInstr_WLSPlatePhysical_top_surf_out",
                             wlsPhysical,
                             fMotherPhysical,
                             fWLSoptSurf);
  // Surface of the teflon
//   new G4LogicalSkinSurface("LArInstr_TeflonPlatePhysical_top_surf",
//                            fTeflonPlateLogical_top,fTefOptSurf);


  ///
  /// -- PMTs + WLS coating --
  ///
  G4VSolid* fPMT_OuterVolume = new G4Tubs("PMT_OuterVolume",
                                          0,
                                          PMT_Radius,
                                          PMT_Height/2.,
                                          0,
                                          2*M_PI);
  G4VSolid* fPMT_InnerVolume = new G4Tubs("PMT_InnerVolume",
                                          0.,
                                          (PMT_Radius-PMTHousing_Thickness),
                                          0.5*(PMT_Height-PMTHousing_Thickness),
                                          0,
                                          2*M_PI);
  G4VSolid* fPMT_housing = new G4SubtractionSolid("PMT_housing",
                                                  fPMT_OuterVolume,
                                                  fPMT_InnerVolume,
                                                  0,
                                                  G4ThreeVector(0., 0., -PMTHousing_Thickness/2.));

  G4Material *fMaterialPMTHousing = G4Material::GetMaterial("Kovar");
  G4LogicalVolume *fPMTHousingLogical = new G4LogicalVolume(fPMT_housing,
                                                            fMaterialPMTHousing,
                                                            "LArInstr_PMTHousingLogical");
  char PMTHousingPhysical[50];

  G4VSolid* fPMT_glass = new G4Tubs("PMT_glass",
                                    0,
                                    PMT_Radius,
                                    PMTGlass_Thickness/2.,
                                    0,
                                    2*M_PI);

  G4Material *fMaterialPMTGlass = G4Material::GetMaterial("Quartz");
  G4Material *fMaterialTopPMTGlass = G4Material::GetMaterial("Quartz"); //top PMT is different JJ
  // Define two: one for upper part and one for bottom part
  G4LogicalVolume* fPMTGlassLogical_top = new G4LogicalVolume(fPMT_glass,
                                                              fMaterialTopPMTGlass,
                                                              "LArInstr_PMTGlassLogical_top");

  G4LogicalVolume* fPMTGlassLogical_bottom = new G4LogicalVolume(fPMT_glass,
                                                                 fMaterialPMTGlass,
                                                                 "LArInstr_PMTGlassLogical_bottom");

  char PMTGlassPhysical[50];

  G4VSolid* fPMT_WLS = new G4Tubs("PMT_WLS",
                                  0,
                                  PMT_Radius,
                                  fWLSThickness/2.,
                                  0,
                                  2*M_PI);

  G4LogicalVolume* fPMTGlassWLS_Logical = new G4LogicalVolume(fPMT_WLS,
                                                              fMaterialWLS,
                                                              "LArInstr_PMTGlassWLS_Logical");
  char PMTGlassWLS_Physical[50];
  // std::string pmt_subgroup;
  for (int i = 0; i < 16; i++) {

    G4RotationMatrix* x_rot_180 = new G4RotationMatrix;
    x_rot_180 -> rotateX(M_PI);
    if ( i < 3 ){
      // pmt_subgroup = "top";
      sprintf(PMTHousingPhysical,"LArInstr_PMTHousingPhysical_%d", i);
      sprintf(PMTGlassPhysical,"LArInstr_PMTGlassPhysical_%d", i);
      sprintf(PMTGlassWLS_Physical,"LArInstr_PMTGlassWLS_Physical_%d", i);
      new G4PVPlacement(0,
                        G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_top_inner*cos((G4double) i*360*deg/3. + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_top_inner*sin((G4double) i*360*deg/3. + fRotatePMTInstAngle)) ,(fDetCenter.z() + fShroud_Offset + fShroud_TubsHeight/2. + fWLSThickness + PMTGlass_Thickness +PMT_Height/2.)),
                        fPMTHousingLogical,
                        PMTHousingPhysical,
                        fMotherLogical,
                        false,
                        i);
      new G4PVPlacement(0,
                        G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_top_inner*cos((G4double) i*360*deg/3. + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_top_inner*sin((G4double) i*360*deg/3. + fRotatePMTInstAngle)) ,(fDetCenter.z() + fShroud_Offset + fShroud_TubsHeight/2. + fWLSThickness + PMTGlass_Thickness/2.)),
                        fPMTGlassLogical_top,
                        PMTGlassPhysical,
                        fMotherLogical,
                        false,
                        i);
      wlsPhysical = new G4PVPlacement(0,
                                      G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_top_inner*cos((G4double) i*360*deg/3. + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_top_inner*sin((G4double) i*360*deg/3. + fRotatePMTInstAngle)) ,(fDetCenter.z() + fShroud_Offset + fShroud_TubsHeight/2.+ fWLSThickness/2.)),
                                      fPMTGlassWLS_Logical,
                                      PMTGlassWLS_Physical,
                                      fMotherLogical,
                                      false,
                                      i);
    }
    else if ( i > 2 && i < 6 ){
      // pmt_subgroup = "top";
      sprintf(PMTHousingPhysical,"LArInstr_PMTHousingPhysical_%d", i);
      sprintf(PMTGlassPhysical,"LArInstr_PMTGlassPhysical_%d", i);
      sprintf(PMTGlassWLS_Physical,"LArInstr_PMTGlassWLS_Physical_%d", i);
      new G4PVPlacement(0,
                        G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_top_outer*cos(((G4double) i -3)*360*deg/3. -25.*deg + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_top_outer*sin(((G4double) i -3)*360*deg/3.-25.*deg + fRotatePMTInstAngle)) ,(fDetCenter.z() + fShroud_Offset + fShroud_TubsHeight/2. + fWLSThickness + PMTGlass_Thickness +PMT_Height/2.)),
                        fPMTHousingLogical,
                        PMTHousingPhysical,
                        fMotherLogical,
                        false,
                        i);
      new G4PVPlacement(0,
                        G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_top_outer*cos(((G4double) i -3)*360*deg/3.-25.*deg + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_top_outer*sin(((G4double) i -3)*360*deg/3.-25.*deg + fRotatePMTInstAngle)) ,(fDetCenter.z() + fShroud_Offset + fShroud_TubsHeight/2. + fWLSThickness + PMTGlass_Thickness/2.)),
                        fPMTGlassLogical_top,
                        PMTGlassPhysical,
                        fMotherLogical,
                        false,
                        i);
      wlsPhysical = new G4PVPlacement(0,
                                      G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_top_outer*cos(((G4double) i -3)*360*deg/3.-25.*deg + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_top_outer*sin(((G4double) i -3)*360*deg/3.-25.*deg + fRotatePMTInstAngle)) ,(fDetCenter.z() + fShroud_Offset + fShroud_TubsHeight/2.+ fWLSThickness/2.)),
                                      fPMTGlassWLS_Logical,
                                      PMTGlassWLS_Physical,
                                      fMotherLogical,
                                      false,
                                      i);
    }
    else if ( i > 5 && i < 9 ){
      // pmt_subgroup = "top";
      sprintf(PMTHousingPhysical,"LArInstr_PMTHousingPhysical_%d", i);
      sprintf(PMTGlassPhysical,"LArInstr_PMTGlassPhysical_%d", i);
      sprintf(PMTGlassWLS_Physical,"LArInstr_PMTGlassWLS_Physical_%d", i);
      new G4PVPlacement(0,
                        G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_top_outer*cos(((G4double) i -6)*360*deg/3.+25.*deg + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_top_outer*sin(((G4double) i -6)*360*deg/3.+25.*deg + fRotatePMTInstAngle)) ,(fDetCenter.z() + fShroud_Offset + fShroud_TubsHeight/2. + fWLSThickness + PMTGlass_Thickness +PMT_Height/2.)),
                        fPMTHousingLogical,
                        PMTHousingPhysical,
                        fMotherLogical,
                        false,
                        i);
      new G4PVPlacement(0,
                        G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_top_outer*cos(((G4double) i -6)*360*deg/3.+25.*deg + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_top_outer*sin(((G4double) i -6)*360*deg/3.+25.*deg + fRotatePMTInstAngle)) ,(fDetCenter.z() + fShroud_Offset + fShroud_TubsHeight/2. + fWLSThickness + PMTGlass_Thickness/2.)),
                        fPMTGlassLogical_top,
                        PMTGlassPhysical,
                        fMotherLogical,
                        false,
                        i);
      wlsPhysical = new G4PVPlacement(0,
                                      G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_top_outer*cos(((G4double) i -6)*360*deg/3.+25.*deg + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_top_outer*sin(((G4double) i -6)*360*deg/3.+25.*deg + fRotatePMTInstAngle)) ,(fDetCenter.z() + fShroud_Offset + fShroud_TubsHeight/2.+ fWLSThickness/2.)),
                                      fPMTGlassWLS_Logical,
                                      PMTGlassWLS_Physical,
                                      fMotherLogical,
                                      false,
                                      i);
    }
    else if ( i == 9 ){
      // pmt_subgroup = "bottom";
      sprintf(PMTHousingPhysical,"LArInstr_PMTHousingPhysical_%d", i);
      sprintf(PMTGlassPhysical,"LArInstr_PMTGlassPhysical_%d", i);
      sprintf(PMTGlassWLS_Physical,"LArInstr_PMTGlassWLS_Physical_%d", i);
      new G4PVPlacement(x_rot_180,
                        G4ThreeVector( fDetCenter.x(), fDetCenter.y() ,(fDetCenter.z() - fShroud_Offset -fShroud_TubsHeight/2. - fWLSThickness - PMTGlass_Thickness - PMT_Height/2.)),
                        fPMTHousingLogical,
                        PMTHousingPhysical,
                        fMotherLogical,
                        false,
                        i-9);
      new G4PVPlacement(/*x_rot_180*/0,
                        G4ThreeVector( (fDetCenter.x()), (fDetCenter.y()) ,(fDetCenter.z()- fShroud_Offset-fShroud_TubsHeight/2. - fWLSThickness -PMTGlass_Thickness/2.)),
                        fPMTGlassLogical_bottom,
                        PMTGlassPhysical,
                        fMotherLogical,
                        false,
                        i-9);
      wlsPhysical = new G4PVPlacement(/*x_rot_180*/0,
                                      G4ThreeVector( (fDetCenter.x()), (fDetCenter.y()) ,(fDetCenter.z()- fShroud_Offset-fShroud_TubsHeight/2.- fWLSThickness/2.)),
                                      fPMTGlassWLS_Logical,
                                      PMTGlassWLS_Physical,
                                      fMotherLogical,
                                      false,
                                      i-9);

    }
    else{
      // pmt_subgroup = "bottom";
      sprintf(PMTHousingPhysical,"LArInstr_PMTHousingPhysical_%d", i);
      sprintf(PMTGlassPhysical,"LArInstr_PMTGlassPhysical_%d", i);
      sprintf(PMTGlassWLS_Physical,"LArInstr_PMTGlassWLS_Physical_%d", i);
      new G4PVPlacement(x_rot_180,
                        G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_bottom*cos(((G4double)i-10)*360*deg/6. + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_bottom*sin(((G4double)i-10)*360*deg/6. + fRotatePMTInstAngle)), (fDetCenter.z()- fShroud_Offset - fShroud_TubsHeight/2. - fWLSThickness - PMTGlass_Thickness -PMT_Height/2.) ),
                        fPMTHousingLogical,
                        PMTHousingPhysical,
                        fMotherLogical,
                        false,
                        i-9);
      new G4PVPlacement(/*x_rot_180*/0,
                        G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_bottom*cos(((G4double)i-10)*360*deg/6. + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_bottom*sin(((G4double)i-10)*360*deg/6. + fRotatePMTInstAngle)), (fDetCenter.z() - fShroud_Offset- fShroud_TubsHeight/2. - fWLSThickness - PMTGlass_Thickness/2.) ),
                        fPMTGlassLogical_bottom,
                        PMTGlassPhysical,
                        fMotherLogical,
                        false,
                        i-9);
      wlsPhysical = new G4PVPlacement(/*x_rot_180*/0,
                                      G4ThreeVector( (fDetCenter.x() + PMT_MountingRadius_bottom*cos(((G4double)i-10)*360*deg/6. + fRotatePMTInstAngle)), (fDetCenter.y() + PMT_MountingRadius_bottom*sin(((G4double)i-10)*360*deg/6. + fRotatePMTInstAngle)), (fDetCenter.z() - fShroud_Offset- fShroud_TubsHeight/2. - fWLSThickness/2.) ),
                                      fPMTGlassWLS_Logical,
                                      PMTGlassWLS_Physical,
                                      fMotherLogical,
                                      false,
                                      i-9);
    }
    // consider the surface rough from both sides
    new G4LogicalBorderSurface(PMTGlassWLS_Physical + G4String("_surf_in"),
                               fMotherPhysical,
                               wlsPhysical,
                               fWLSoptSurf);

    new G4LogicalBorderSurface(PMTGlassWLS_Physical + G4String("_surf_out"),
                               wlsPhysical,
                               fMotherPhysical,
                               fWLSoptSurf);

  }

  // Attach vis attributes to a couple of volumes
  G4VisAttributes*fTeflonVisAtt = new G4VisAttributes(lblue);
  fTeflonVisAtt -> SetVisibility(true);
  fTeflonVisAtt -> SetForceSolid(false);
  fCopperPlateLogical_bottom-> SetVisAttributes(fTeflonVisAtt);
  fVM2000PlateLogical_bottom-> SetVisAttributes(fTeflonVisAtt);
  fWLSPlateLogical_bottom-> SetVisAttributes(fTeflonVisAtt);

  fCopperPlateLogical_top-> SetVisAttributes(fTeflonVisAtt);
  fVM2000PlateLogical_top-> SetVisAttributes(fTeflonVisAtt);
  fWLSPlateLogical_top-> SetVisAttributes(fTeflonVisAtt);

  G4VisAttributes*fPMTHousingVisAtt = new G4VisAttributes(lred);
  fPMTHousingVisAtt -> SetVisibility(true);
  fPMTHousingVisAtt -> SetForceSolid(true);
  fPMTHousingLogical-> SetVisAttributes(fPMTHousingVisAtt);

  G4VisAttributes*fPMTGlassVisAtt = new G4VisAttributes(blue_gray);
  fPMTGlassVisAtt -> SetVisibility(true);
  fPMTGlassVisAtt -> SetForceSolid(false);
  fPMTGlassLogical_top-> SetVisAttributes(fPMTGlassVisAtt);
  fPMTGlassLogical_bottom-> SetVisAttributes(fPMTGlassVisAtt);


  ///
  /// Optical properties
  ///
  /// Attach a series of optical properties to the detector otherwise the photons will die at the boundary and no registered hit is done
  /// ---------------------------------------------------***************************-------------------------------------------------------------

  G4int ji = 0;
  G4double ee=0;
  // Build table with photon energies
  const G4int num = 500;

  G4double PPSCHighE = LambdaE /(115*nanometer);
  G4double PPSCLowE = LambdaE /(650*nanometer); //598
  G4double dee = ((PPSCHighE - PPSCLowE) / ((G4double)(num-1)));
  G4double LAr_SCPP[num];
  for (ji = 0; ji < num; ji++)
  {
    ee=PPSCLowE+ ((G4double)ji) * dee;
    LAr_SCPP[ji]=ee;
  }


  ///Reflector (VM2000)
  //From Dario's thesis, reflectivity of VM2000 is ~98% above 370 nm
  //and ~15% below it (almost a step-function)

  G4double Reflectivity[num];
  //G4double Efficiency[num];

  for (ji=0;ji < num; ji++)
  {
    if (LAr_SCPP[ji] < (LambdaE/(370*nanometer)))
      Reflectivity[ji] = 0.98; //visible
    else
      Reflectivity[ji] = 0.15; //UV

    //Efficiency[ji] = 0.0;
  }
  G4MaterialPropertiesTable *vmOpTable = new G4MaterialPropertiesTable();
  vmOpTable->AddProperty("REFLECTIVITY",LAr_SCPP,Reflectivity,num);

  G4OpticalSurface* reflOptSurface = new G4OpticalSurface("VM_surface");

  new G4LogicalSkinSurface("VM_surface",fVM2000CylinderLogical,reflOptSurface);
  new G4LogicalSkinSurface("VMPlate_top_surface",fVM2000PlateLogical_top,reflOptSurface); //TODO check if implementation correct
  new G4LogicalSkinSurface("VMPlate_bottom_surface",fVM2000PlateLogical_bottom,reflOptSurface);
  reflOptSurface->SetType(dielectric_dielectric);
  // The WLS is on a separate volume.
  // The reflection should be either spike or Lobe.
  reflOptSurface->SetFinish(polishedfrontpainted);
  reflOptSurface->SetMaterialPropertiesTable(vmOpTable);

  /// Optical properties of the PMT glass:
//   const G4int num_entries = 24;
//   G4double PMTGlassEnergy[num_entries] = {
//                                           LambdaE/(115*nanometer),
//                                           LambdaE /(160.8*nanometer),
//                                           LambdaE /(170*nanometer), LambdaE /(182.3*nanometer), LambdaE /(200*nanometer),
//                                           LambdaE/(230*nanometer), LambdaE /(250*nanometer), LambdaE /(270*nanometer), LambdaE /(300*nanometer), LambdaE /(320*nanometer),
//                                           LambdaE/(340*nanometer), LambdaE /(360*nanometer), LambdaE /(380*nanometer), LambdaE /(400*nanometer), LambdaE /(450*nanometer),
//                                           LambdaE/(500*nanometer), LambdaE /(550*nanometer), LambdaE /(580*nanometer), LambdaE /(600*nanometer), LambdaE /(630*nanometer),
//                                           LambdaE/(660*nanometer), LambdaE /(700*nanometer), LambdaE /(730*nanometer), LambdaE /(750*nanometer)};
//   G4double PMTGlassReflectivity[num_entries] = {0.0,0.0,0.0,0.0,0.0,
//                                                 0.0,0.0,0.0,0.0,0.0,
//                                                 0.0,0.0,0.0,0.0,0.0,
//                                                 0.0,0.0,0.0,0.0};
//   G4double PMTGlassEfficiency[num_entries] = {0.0000,0.0034,0.0322,0.0741,0.1297,
//                                               0.1450,0.1673,0.1965,0.2348,0.2473,
//                                               0.2467,0.2399,0.2368,0.2264,0.1847,
//                                               0.1309,0.0692,0.0371,0.0231,0.0104,
//                                               0.0036,0.0006,0.0001,0.0000};
  const G4int num_entries = 29;
  G4double PMTGlassEnergy[num_entries] = {LambdaE /(115*nanometer), LambdaE /(160.8*nanometer), LambdaE /(170*nanometer), LambdaE /(182.3*nanometer),
                                          LambdaE /(200*nanometer), LambdaE /(230*nanometer), LambdaE /(250*nanometer), LambdaE /(270*nanometer),
                                          LambdaE /(300*nanometer), LambdaE /(320*nanometer), LambdaE /(340*nanometer), LambdaE /(360*nanometer), LambdaE /(380*nanometer),
                                          LambdaE /(400*nanometer), LambdaE /(420*nanometer), LambdaE /(440*nanometer), LambdaE /(460*nanometer), LambdaE /(480*nanometer),
                                          LambdaE /(500*nanometer), LambdaE /(520*nanometer), LambdaE /(550*nanometer), LambdaE /(580*nanometer),
                                          LambdaE /(600*nanometer), LambdaE /(630*nanometer), LambdaE /(660*nanometer),
                                          LambdaE /(700*nanometer), LambdaE /(730*nanometer), LambdaE /(750*nanometer), LambdaE /(770*nanometer)};
  G4double PMTGlassReflectivity[num_entries] = {0.0,0.0,0.0,0.0,
                                                0.0,0.0,0.0,0.0,
                                                0.0,0.0,0.0,0.0,0.0,
                                                0.0,0.0,0.0,0.0,0.0,
                                                0.0,0.0,0.0,0.0,
                                                0.0,0.0,0.0,
                                                0.0,0.0,0.0,0.0};
  G4double PMTGlassEfficiency[num_entries] = {0.0000,0.0000,0.0000,0.0000,
                                              0.1191,0.1312,0.1536,0.1730,
                                              0.2125,0.2297,0.2332,0.2310,0.2345,
                                              0.2275,0.2147,0.1985,0.1749,0.1513,
                                              0.1352,0.1131,0.0727,0.0447,
                                              0.0330,0.0198,0.0099,
                                              0.0026,0.0007,0.0002,0.0000};

  G4MaterialPropertiesTable *PMTGlassOptTable = new G4MaterialPropertiesTable();
  PMTGlassOptTable->AddProperty("REFLECTIVITY",PMTGlassEnergy,PMTGlassReflectivity,num_entries);
  PMTGlassOptTable->AddProperty("EFFICIENCY",PMTGlassEnergy,PMTGlassEfficiency,num_entries);

  fMaterialPMTGlass->SetMaterialPropertiesTable(PMTGlassOptTable);

  G4OpticalSurface* PMTGlassOptSurface = new G4OpticalSurface("LArInstrPMTGlassSurface",glisur,polished,dielectric_metal);
  PMTGlassOptSurface->SetMaterialPropertiesTable(PMTGlassOptTable);

  new G4LogicalSkinSurface("PMTGlass_surf_bottom",fPMTGlassLogical_bottom,PMTGlassOptSurface);

  // special treatment for top PMTs JJ
  G4double LAr_TopPMT_scaling = fDetectorDB->GetLArInstTopPMTscaleFact();
  MGLog(routine) << "Top PMT efficiency scaled down by: " << LAr_TopPMT_scaling << endlog;

  for (ji=0; ji < num_entries; ji++){
    PMTGlassEfficiency[ji] = PMTGlassEfficiency[ji] * LAr_TopPMT_scaling;
  }

  G4MaterialPropertiesTable *topPMTGlassOptTable = new G4MaterialPropertiesTable();
  topPMTGlassOptTable->AddProperty("REFLECTIVITY",PMTGlassEnergy,PMTGlassReflectivity,num_entries);
  topPMTGlassOptTable->AddProperty("EFFICIENCY",PMTGlassEnergy,PMTGlassEfficiency,num_entries);

  fMaterialTopPMTGlass->SetMaterialPropertiesTable(topPMTGlassOptTable);

  G4OpticalSurface* topPMTGlassOptSurface = new G4OpticalSurface("LArInstrPMTGlassSurface",glisur,polished,dielectric_metal);
  topPMTGlassOptSurface->SetMaterialPropertiesTable(topPMTGlassOptTable);

  new G4LogicalSkinSurface("PMTGlass_surf_top",fPMTGlassLogical_top,topPMTGlassOptSurface);

  fSDLogicVec.push_back(fPMTGlassLogical_top);
  fSDLogicNameVec.push_back("PMTGlass_top");
  fSDLogicInstanceVec.push_back(9);
  fSDLogicVec.push_back(fPMTGlassLogical_bottom);
  fSDLogicNameVec.push_back("PMTGlass_bottom");
  fSDLogicInstanceVec.push_back(7);

}

void GEGeometryLArInstHybrid::ConstructFiberSubDesign() {

  // Some variables to hold out the construction of the fibers part
  G4double fiber_end_height = 1.0*mm;

  G4VPhysicalVolume* wlsPhysical = NULL;
  G4LogicalVolume*	inner_fiber_logvol = NULL;
  G4LogicalVolume*	outer_fiber_logvol = NULL;

  // Define the color and the visualisation attributes.
  G4Colour lgreen  (0.0,  0.4, 0.0) ;
  G4Colour lred		(0.4,  0.0, 0.0);
  G4VisAttributes* fiber_vis_att = new G4VisAttributes(/*lgreen*/lred);
  fiber_vis_att -> SetVisibility(true);
  fiber_vis_att -> SetForceSolid(false);

  G4Colour dgreen  (0.0,  0.6, 0.0) ;
  G4VisAttributes* cladding_vis_att = new G4VisAttributes(dgreen);
  cladding_vis_att -> SetVisibility(true);
  cladding_vis_att -> SetForceSolid(false);

  G4Colour cyan  (0.0,  0.8, 1.0) ;
  G4VisAttributes* wls_vis_att = new G4VisAttributes(cyan);
  wls_vis_att -> SetVisibility(true);
  wls_vis_att -> SetForceSolid(false);


  ///
  /// -- Material definition --
  ///

  // First define the fiber material, as it will be used by all fibers
  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  G4NistManager *nistMan = G4NistManager::Instance();

  //--------------------------------------------------
  // Fiber and WLS: Polystyrene
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(8);
  elements.push_back("H");     natoms.push_back(8);

  density = 1.050*g/cm3;

  // -- Rough approximation of BCF-10
  G4Material* fiber_material = nistMan->
      ConstructNewMaterial("PolystyreneFiber", elements, natoms, density);

  elements.clear();
  natoms.clear();

  G4int npoints_abs = fFibersAbsorptionSpec->GetN();
  G4double *FiberAbsEnergies = new G4double[npoints];
  G4double *FiberWLSAbsorption = new G4double[npoints];
  G4double FiberTimeConstant = 7.2*ns;
  // stuff that we have no variable information and therefore use a constant value
  const G4int npoints_fixed = 2;
  G4double FiberFixEnergies[npoints_fixed] = {LambdaE/(650.0*nanometer),LambdaE/(115.0*nanometer)};
  G4double FiberRIndex[npoints_fixed] = {1.6,1.6};
  G4double FiberAbsorption[npoints_fixed] = {3.8*m,3.8*m};
  //G4double FiberEfficiency[npoints_fixed] = {LAr_Fiber_coverage,LAr_Fiber_coverage};
  //G4double FiberEfficiency[npoints_fixed] = {0.5,0.5};
  G4int idx = 0;
  // The WLS absorption is a distance.
  // Jozsef measured the value to be 0.7 mm at 400 nm.
  // Use this to scale the spectrum.
  // Have to do a backtrack of the value from the estimate at 400 nm.
  G4double wls_abs_scale = 0.7*mm*fFibersAbsorptionSpec->Eval(400);

  MGLog(debugging) << " Fiber WLS absorption spectrum (distance scale : " << wls_abs_scale << " ) :" << endlog;

  for (int i = 0; i < npoints_abs; ++i) {
    idx = npoints_abs-i-1;
    FiberAbsEnergies[i] = LambdaE/(fFibersAbsorptionSpec->GetX()[idx]*nanometer);

    //assuming the minimum as 0.02*cm at 423 nm
    // value taken from http://www-zeuthen.desy.de/lcdet/Feb_05_WS/talks/rd_lcdet_sim.pdf
    // based on measurements for MINOS
    // TODO: Should be verified with Jozsef
    FiberWLSAbsorption[i] = wls_abs_scale/fFibersAbsorptionSpec->GetY()[idx];
    MGLog(debugging) << "WL : " << fFibersAbsorptionSpec->GetX()[idx]
        << " nm  En : " << FiberAbsEnergies[i]/MeV << " MeV Abs : " << FiberWLSAbsorption[i]/cm << " cm " << endlog;
  }

  G4int npoints_em = fFibersEmissionSpec->GetN();
  G4double *FiberEmEnergies = new G4double[npoints_em];
  G4double *FiberWLSEmission = new G4double[npoints_em];

  MGLog(debugging) << " Fiber WLS emission spectrum :" << endlog;
  for (int i = 0; i < npoints_em; ++i) {
    idx = npoints_em-i-1;
    FiberEmEnergies[i] = LambdaE/(fFibersEmissionSpec->GetX()[idx]*nanometer);
    FiberWLSEmission[i] = fFibersEmissionSpec->GetY()[idx];

    MGLog(debugging) << "WL : " << fFibersAbsorptionSpec->GetX()[idx]
        << " nm En : " << FiberEmEnergies[i]/MeV << " MeV Emission : " << FiberWLSEmission[i] << endlog;
  }


  G4MaterialPropertiesTable *fiberTable = new G4MaterialPropertiesTable();

  fiberTable->AddProperty("RINDEX",FiberFixEnergies,FiberRIndex,npoints_fixed);
  fiberTable->AddProperty("ABSLENGTH",FiberFixEnergies,FiberAbsorption,npoints_fixed);
  fiberTable->AddProperty("WLSABSLENGTH",FiberAbsEnergies,FiberWLSAbsorption,npoints_abs);
  fiberTable->AddProperty("WLSCOMPONENT",FiberEmEnergies,FiberWLSEmission,npoints_em);

  // From WArP
  fiberTable->AddConstProperty("WLSTIMECONSTANT", FiberTimeConstant);

  fiber_material->SetMaterialPropertiesTable(fiberTable);


  //--------------------------------------------------
  // Fiber cladding: PMMA.
  // It does not really matter the materia, just the refractive index
  //--------------------------------------------------

  elements.push_back("C");  natoms.push_back(5);
  elements.push_back("H");  natoms.push_back(8);
  elements.push_back("O");  natoms.push_back(2);
  density = 1.2*g/cm3;

  // -- Rough approximation of BCF-10
  G4Material* fiber_cladding_material = nistMan->
      ConstructNewMaterial("PMMA", elements, natoms, density);

  elements.clear();
  natoms.clear();

  // Attach optical properties to the cladding. Only the RIndex is important
  G4double claddingFixEnergies[npoints_fixed] = {LambdaE/(650.0*nanometer),LambdaE/(115.0*nanometer)};
  G4double claddingRIndex[npoints_fixed] = {1.42,1.42};
  G4MaterialPropertiesTable *claddingTable = new G4MaterialPropertiesTable();

  claddingTable->AddProperty("RINDEX",claddingFixEnergies,claddingRIndex,npoints_fixed);

  fiber_cladding_material->SetMaterialPropertiesTable(claddingTable);



  //////////////////////////////////////////
  /// -- Optical properties definition
  //////////////////////////////////////////
  // Let's start attaching optical properties to the materials
  //


  ///
  /// -- SiPM collecting surface
  ///

  // SiPM "sensitive" surface.. this will be a border surface between the fibers and the upper shroud

  // Data taken from Jozsef's presentation at TUM collb meeting
  // http://www.mpi-hd.mpg.de/gerda/internal/TUMJUN12/slides/jozsef_Veto_pres_060612.pdf
  const G4int npoints_eff = 10;
  G4double SiPMPhotonEnergy[npoints_eff];
  G4double SiPMEfficiencyWl[npoints_eff] = {100.,280.,310.,350.,400.,435.,505.,525.,595.,670.};
  G4double SiPMEfficiencyTbl[npoints_eff] = {0.0,0.19,0.30,0.32,0.33,0.32,0.27,0.19,0.12,0.07};
  G4double SiPMEfficiency[npoints_eff];
  G4double SiPMReflectivity[npoints_eff];


  G4double LAr_Fiber_coverage = fDetectorDB->GetLArInstFiberCoverage();
  MGLog(routine) << "Fiber coverage: " << LAr_Fiber_coverage << endlog;

  //fiber efficiency scaling relative to PMTs
  G4double SiPM_bonding_scale = 0.7 * LAr_Fiber_coverage;

  MGLog(debugging) << "Fiber sensitive surface : " << endlog;
  for (G4int ji=0; ji < npoints_eff; ji++)
  {
    // Zero reflectivity and 1.0 efficiency means that the
    // photons are all "absorbed and identified as hits"
    SiPMPhotonEnergy[ji] = LambdaE/(SiPMEfficiencyWl[(npoints_eff-1)-ji]*nm);
    SiPMReflectivity[ji] = 0.0; // Set the reflectivity in the fibers to zero, otherwise 99.999 % is reflected due to index of refraction
    SiPMEfficiency[ji] = SiPM_bonding_scale * SiPMEfficiencyTbl[(npoints_eff-1)-ji]; // Quantum efficiency of the Fiber. The true efficiency is applied offline
    MGLog(debugging) << "WL " << SiPMEfficiencyWl[(npoints_eff-1)-ji]
                     << " nm En " << SiPMPhotonEnergy[ji]/MeV << " MeV Refl : "
                     << SiPMReflectivity[ji] << " Eff " << SiPMEfficiency[ji] << endlog;

  }
  G4MaterialPropertiesTable *sipmOptTable = new G4MaterialPropertiesTable();
  sipmOptTable->AddProperty("EFFICIENCY",SiPMPhotonEnergy,SiPMEfficiency,npoints_eff);
  sipmOptTable->AddProperty("REFLECTIVITY",SiPMPhotonEnergy,SiPMReflectivity,npoints_eff);
  G4OpticalSurface* sipmOptSurface	= new G4OpticalSurface("SiPM surface",glisur,ground,dielectric_metal);
  sipmOptSurface->SetMaterialPropertiesTable(sipmOptTable);


  ///
  /// -- Fiber Reflective end
  ///
  // The reflective end will be a reflective surface between the fibers and the lower shroud
  const G4int num = 4;
  G4double Wavelength[num] = {100.,200.,301.,650.};

  G4double ReflFiberPhotonEnergy[num];
  G4double ReflFiberReflectivity[num];
  G4double ReflFiberEfficiency[num];

  MGLog(debugging) << "Fiber reflective surface : " << endlog;
  for (G4int ji=0; ji < num; ji++)
  {
    // Assume similar reflectivity as the VM2000
    ReflFiberPhotonEnergy[ji] = LambdaE/(Wavelength[(num-1)-ji]*nm);
    ReflFiberReflectivity[ji] = 0.98; // Set the reflectivity in the fibers to zero, otherwise 99.999 % is reflected due to index of refraction
    ReflFiberEfficiency[ji] = 0.0;
    MGLog(debugging) << "WL " << Wavelength[(num-1)-ji]
                     << " nm En " << ReflFiberPhotonEnergy[ji]/MeV << " MeV Refl : "
                     << ReflFiberReflectivity[ji] << " Eff " << ReflFiberEfficiency[ji] << endlog;
  }
  G4MaterialPropertiesTable *fiberReflOptTable = new G4MaterialPropertiesTable();
  fiberReflOptTable->AddProperty("REFLECTIVITY",ReflFiberPhotonEnergy,ReflFiberReflectivity,num);
  fiberReflOptTable->AddProperty("EFFICIENCY",ReflFiberPhotonEnergy,ReflFiberEfficiency,num);

  G4OpticalSurface* fiberReflOptSurface  = new G4OpticalSurface("Fiber reflective surface",glisur,ground,dielectric_metal);
  fiberReflOptSurface->SetMaterialPropertiesTable(fiberReflOptTable);


  // All the remaining properties are part of the materials
  ///
  ///  -- Fiber construction
  ///
  if (! fEnableSegmentedFiberShroud) {
    // Let's build first the mother WLS volume.
    MGLog(debugging) << "Placing outer fibers at radius " << fShroud_TubsOuterRadius << endlog;

    G4VSolid * outer_wls_solid = BuildFiberShroudSolid(fShroud_TubsOuterRadius,fFiberThickness+2.0*fWLSThickness,fShroud_CenterHeight-2*fiber_end_height,false);
    outer_wls_solid->SetName("LArInstr_WLSOuter_Solid");

    G4LogicalVolume *outer_wls_logvol = new G4LogicalVolume(outer_wls_solid,
                                                            fTPB,
                                                            "LArInstr_WLSOuter_Physical");
    outer_wls_logvol->SetVisAttributes(wls_vis_att);

    // place the volume
    wlsPhysical = new G4PVPlacement(0,
                                    G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z()),
                                    outer_wls_logvol,
                                    "LArInstr_WLSOuter_Physical",
                                    fMotherLogical,
                                    false,
                                    0);

    // consider the surface rough from both sides
    new G4LogicalBorderSurface("LArInstr_WLSOuter_Physical_surf_in",
                               fMotherPhysical,
                               wlsPhysical,
                               fWLSoptSurf);

    new G4LogicalBorderSurface("LArInstr_WLSOuter_Physical_surf_out",
                               wlsPhysical,
                               fMotherPhysical,
                               fWLSoptSurf);

    /// --  Now build the fibers themselves.

    // - Cladding

    // To add the fibers collection and reflection, two small volumes (0.1*mm thick ) are attached
    // to the top and bottom of the cylinder, which means that the cylinder has to be build slightly smaller

    G4VSolid* outer_fiber_cladding_solid = BuildFiberShroudSolid(fShroud_TubsOuterRadius-fWLSThickness,fFiberThickness,fShroud_CenterHeight-2*fiber_end_height,false);
    outer_fiber_cladding_solid->SetName("LArInstr_FiberOuter_Cladding_Solid");
    G4LogicalVolume* outer_fiber_cladding_logvol = new G4LogicalVolume(outer_fiber_cladding_solid,
                                                                     fiber_cladding_material,
                                                                     "LArInstr_FiberOuterCladding_Logical");
    outer_fiber_cladding_logvol->SetVisAttributes(cladding_vis_att);

    // The mother in this case is the WLS shroud
    G4VPhysicalVolume *claddingPhys = new G4PVPlacement(0,
                                                        G4ThreeVector(0.0,0.0,0.0),
                                                        outer_fiber_cladding_logvol,
                                                        "LArInstr_FiberOuterCladding_Physical",
                                                        outer_wls_logvol,
                                                        false,
                                                        0);
    // - Fiber core

    G4VSolid*outer_fiber_solid = BuildFiberShroudSolid(fShroud_TubsOuterRadius-fWLSThickness-fFiberCladThickness,fFiberThickness-2*fFiberCladThickness,fShroud_CenterHeight-2*fiber_end_height,false);
    outer_fiber_solid->SetName("LArInstr_FiberOuter_Solid");

    outer_fiber_logvol = new G4LogicalVolume(outer_fiber_solid,
                                             fiber_material,
                                             "LArInstr_FiberOuter_Logical");
    outer_fiber_logvol->SetVisAttributes(fiber_vis_att);

    // The mother in this case is the cladding
    G4VPhysicalVolume *fiberPhys = new G4PVPlacement(0,
                                                     G4ThreeVector(0.0,0.0,0.0),
                                                     outer_fiber_logvol,
                                                     "LArInstr_FiberOuter_Physical",
                                                     outer_fiber_cladding_logvol,
                                                     false,
                                                     0);

    // Now place the fiber ends
    G4VSolid*outer_fiber_end_solid = BuildFiberShroudSolid(fShroud_TubsOuterRadius-fWLSThickness,fFiberThickness,fiber_end_height,false);
    outer_fiber_end_solid->SetName("LArInstr_FiberOuterEnd_Solid");

    G4LogicalVolume *outer_fiber_end_logvol_sensitive = new G4LogicalVolume(outer_fiber_end_solid,
                                                                  G4Material::GetMaterial("MetalSilicon"),
                                                                  "LArInstr_FiberOuterEnd_Top_Logical");

    // The mother in this case is the crioLiquid  - fiber_end_height + 0.5*fiber_end_height
    G4VPhysicalVolume *fiberSensPhys = new G4PVPlacement(0,
                                                         G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z()+0.5*fShroud_CenterHeight-0.5*fiber_end_height),
                                                         outer_fiber_end_logvol_sensitive,
                                                         "LArInstr_FiberOuter_SensitiveEnd_Physical",
                                                         fMotherLogical,
                                                         false,
                                                         0);

    // Attach the sensitive surfaces
    new G4LogicalBorderSurface("LArInstr_FiberOuterCladding_Sensitive_surf",
                               claddingPhys,
                               fiberSensPhys,
                               sipmOptSurface);

    new G4LogicalBorderSurface("LArInstr_FiberOuter_Sensitive_surf",
                               fiberPhys,
                               fiberSensPhys,
                               sipmOptSurface);

    // Place the reflective end
    G4LogicalVolume *outer_fiber_end_logvol_reflective = new G4LogicalVolume(outer_fiber_end_solid,
                                                                  G4Material::GetMaterial("MetalSilicon"),
                                                                  "LArInstr_FiberOuterEnd_Bottom_Logical");
    G4VPhysicalVolume *fiberReflPhys = new G4PVPlacement(0,
                                                         G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z()-0.5*fShroud_CenterHeight+0.5*fiber_end_height),
                                                         outer_fiber_end_logvol_reflective,
                                                         "LArInstr_FiberOuter_ReflEnd_Physical",
                                                          fMotherLogical,
                                                         false,
                                                         1);

    // Attach the reflective surfaces
    new G4LogicalBorderSurface("LArInstr_FiberOuterCladding_Reflective_surf",
                               claddingPhys,
                               fiberReflPhys,
                               fiberReflOptSurface);

    new G4LogicalBorderSurface("LArInstr_FiberOuter_Reflective_surf",
                               fiberPhys,
                               fiberReflPhys,
                               fiberReflOptSurface);


    //	//Associates the OpticalSurface to the actual volumes
    //	new G4LogicalSkinSurface("fiber_outer_opsurface",outer_fiber_logvol,fiberOptSurface);
    //	/**/

    // For the fibers, the sensitive volume is now the upper shroud, instead of the fibers themselves
    //fSDLogicVec.push_back(outer_fiber_logvol);
    fSDLogicVec.push_back(outer_fiber_end_logvol_sensitive);
    fSDLogicNameVec.push_back("Fiber_outer");
    fSDLogicInstanceVec.push_back(1);
  }
  else {
    //general size to matche monolitic implementation
    G4double  radius         = fShroud_TubsOuterRadius-fFiberThickness/2.-fWLSThickness;
    G4double  totalThickness = fFiberThickness+2.0*fWLSThickness;
    G4double  height         = fShroud_CenterHeight-2*fiber_end_height;
    G4double  rotation       = fDetectorDB->GetLArInstHybridSegmentedFiberShroudRotation();
    MGLog(debugging) << "Rotation segmented fiber shroud " << rotation << endlog;

    // - wls
    G4VSolid* wlsSegment_solid = BuildFiberShroudSegmentSolid(radius,fNoOfSegments,fSegmentSpacing,totalThickness,totalThickness,height);
    wlsSegment_solid->SetName("LArInstr_WLSSegment_Solid");
    G4LogicalVolume* wlsSegment_logvol = new G4LogicalVolume(wlsSegment_solid,fTPB,"LArInstr_WLSOuterSegment_Logical");
    wlsSegment_logvol->SetVisAttributes(wls_vis_att);

    // - Cladding
    G4VSolid* claddingSegment_solid = BuildFiberShroudSegmentSolid(radius,fNoOfSegments,fSegmentSpacing,fFiberThickness,totalThickness,height);
    claddingSegment_solid->SetName("LArInstr_CladdingSegment_Solid");
    G4LogicalVolume* claddingSegment_logvol = new G4LogicalVolume(claddingSegment_solid,fiber_cladding_material,"LArInstr_CladdingSegment_Logical");
    claddingSegment_logvol->SetVisAttributes(cladding_vis_att);

    // - fiber
    G4VSolid* fiberSegment_solid = BuildFiberShroudSegmentSolid(radius,fNoOfSegments,fSegmentSpacing,fFiberThickness-2*fFiberCladThickness,totalThickness,height);
    fiberSegment_solid->SetName("LArInstr_FiberSegment_Solid");
    G4LogicalVolume* fiberSegment_logvol = new G4LogicalVolume(fiberSegment_solid,fiber_material,"LArInstr_FiberSegment_Logical");
    fiberSegment_logvol->SetVisAttributes(fiber_vis_att);

    // - fiber ends
    G4VSolid* endSegment_solid = BuildFiberShroudSegmentSolid(radius,fNoOfSegments,fSegmentSpacing,fFiberThickness,totalThickness,fiber_end_height);
    endSegment_solid->SetName("LArInstr_EndSegment_Solid");
    G4LogicalVolume* endSegment_logvol_sensitive = new G4LogicalVolume(endSegment_solid,
                                                                       G4Material::GetMaterial("MetalSilicon"),
                                                                       "LArInstr_EndSegment_Top_Logical");
    G4LogicalVolume* endSegment_logvol_reflective = new G4LogicalVolume(endSegment_solid,
                                                                        G4Material::GetMaterial("MetalSilicon"),
                                                                        "LArInstr_EndSegment_Bottom_Logical");

    char wlsSegment_PhyName[50];
    char sensitiveEndSegment_PhyName[50];
    char reflectiveEndSegment_PhyName[50];
    // place the volumes
    for(int i=0;i<fNoOfSegments;i++) {
      //rotation matrix
      G4RotationMatrix* z_rot = new G4RotationMatrix;
      z_rot -> rotateZ(rotation+2*M_PI*i/fNoOfSegments);
      //MGLog(debugging) << "Rotation of segment " << i << ": " << rotation+2*M_PI*i/fNoOfSegments << endlog;

      //place wls
      sprintf(wlsSegment_PhyName,"LArInstr_WLSSegment_Physical_%d",i);
      G4VPhysicalVolume* wlsSegment_Physical = new G4PVPlacement(z_rot,
                        G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z()),
                        wlsSegment_logvol,
                        wlsSegment_PhyName,
                        fMotherLogical,
                        false,
                        i);

      sprintf(wlsSegment_PhyName,"LArInstr_WLSOuter_Physical_surf_in_%d",i);
      // consider the surface rough from both sides
      new G4LogicalBorderSurface(wlsSegment_PhyName,
                                 fMotherPhysical,
                                 wlsSegment_Physical,
                                 fWLSoptSurf);
      sprintf(wlsSegment_PhyName,"LArInstr_WLSOuter_Physical_surf_out_%d",i);
      new G4LogicalBorderSurface(wlsSegment_PhyName,
                                 wlsSegment_Physical,
                                 fMotherPhysical,
                                 fWLSoptSurf);
    }

    //place cladding
    G4VPhysicalVolume* claddingSegment_Physical = new G4PVPlacement(0,
                      G4ThreeVector(0.0,0.0,0.0),
                      claddingSegment_logvol,
                      "LArInstr_CladdingSegment_Physical",
                      wlsSegment_logvol,
                      false,
                      0);

    //place fiber
    G4VPhysicalVolume* fiberSegment_Physical = new G4PVPlacement(0,
                      G4ThreeVector(0.0,0.0,0.0),
                      fiberSegment_logvol,
                      "LArInstr_FiberSegment_Physical",
                      claddingSegment_logvol,
                      false,
                      0);

    for(int i=0;i<fNoOfSegments;i++) {
      //rotation matrix
      G4RotationMatrix* z_rot = new G4RotationMatrix;
      z_rot -> rotateZ(rotation+2*M_PI*i/fNoOfSegments);
      // place fiber top ends (fiber_end_height + 0.5*fiber_end_height)
      sprintf(sensitiveEndSegment_PhyName,"LArInstr_FiberOuter_SensitiveEnd_Physical_%d",i);
      //sprintf(sensitiveEndSegment_PhyName,"LArInstr_SensitiveEndSegment_Physical_%d",i);
      G4VPhysicalVolume* sensSegment_Physical = new G4PVPlacement(z_rot,
                        G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z()+0.5*fShroud_CenterHeight-0.5*fiber_end_height),
                        endSegment_logvol_sensitive,
                        sensitiveEndSegment_PhyName,
                        fMotherLogical,
                        false,
                        i);

      // Attach the sensitive surfaces
      sprintf(sensitiveEndSegment_PhyName,"LArInstr_CladdingSegment_Sensitive_surf_%d",i);
      new G4LogicalBorderSurface(sensitiveEndSegment_PhyName,
                                 claddingSegment_Physical,
                                 sensSegment_Physical,
                                 sipmOptSurface);
      sprintf(sensitiveEndSegment_PhyName,"LArInstr_FiberSegment_Sensitive_surf_%d",i);
      new G4LogicalBorderSurface(sensitiveEndSegment_PhyName,
                                 fiberSegment_Physical,
                                 sensSegment_Physical,
                                 sipmOptSurface);

      // place fiber botton ends
      sprintf(reflectiveEndSegment_PhyName,"LArInstr_ReflectiveEndSegment_Physical_%d",i);
      G4VPhysicalVolume *reflSegment_Physical = new G4PVPlacement(z_rot,
                        G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z()-0.5*fShroud_CenterHeight+0.5*fiber_end_height),
                        endSegment_logvol_reflective,
                        reflectiveEndSegment_PhyName,
                        fMotherLogical,
                        false,
                       i);

      // Attach the reflective surfaces
      sprintf(reflectiveEndSegment_PhyName,"LArInstr_CladdingSegment_Reflective_surf_%d",i);
      new G4LogicalBorderSurface(reflectiveEndSegment_PhyName,
                                 claddingSegment_Physical,
                                 reflSegment_Physical,
                                 fiberReflOptSurface);
      sprintf(reflectiveEndSegment_PhyName,"LArInstr_FiberSegment_Reflective_surf_%d",i);
      new G4LogicalBorderSurface(reflectiveEndSegment_PhyName,
                                 fiberSegment_Physical,
                                 reflSegment_Physical,
                                 fiberReflOptSurface);
    }
    // For the fibers, the sensitive volume is now the upper shroud, instead of the fibers themselves
    fSDLogicVec.push_back(endSegment_logvol_sensitive);
    fSDLogicNameVec.push_back("Fiber_segments");
    //fSDLogicNameVec.push_back("Fiber_outer");
    fSDLogicInstanceVec.push_back(fNoOfSegments);
  }


  // central string, if enabled
  if (fEnableCentralFibers) {

    MGLog(debugging) << "Distance between center of detector and fibers : " << fDetectorDB->GetDN500StringDistance()/2.0/mm << " mm." << endlog;

    // First build the WLS mother
    MGLog(debugging) << "Placing inner fibers at radius " << (fDetectorDB->GetDN500StringDistance()/2.0 + fWLSThickness + 0.5*fFiberThickness) << endlog;

    G4VSolid *inner_wls_solid = BuildFiberShroudSolid(fDetectorDB->GetDN500StringDistance()/2.0 + fWLSThickness + 0.5*fFiberThickness,
                                                      fFiberThickness + 2.0*fWLSThickness,
                                                      fShroud_CenterHeight,
                                                      false);

    inner_wls_solid->SetName("LArInstr_WLSInner_Solid");

    G4LogicalVolume *inner_wls_logvol = new G4LogicalVolume(inner_wls_solid,
                                                            fTPB,
                                                            "LArInstr_WLSInner_Logical");

    inner_wls_logvol->SetVisAttributes(wls_vis_att);

    // The mother in this case is the WLS shroud
    wlsPhysical = new G4PVPlacement(0,
                                    G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z()),
                                    inner_wls_logvol,
                                    "LArInstr_WLSInner_Physical",
                                    fMotherLogical,
                                    false,
                                    0);

    // consider the surface rough from both sides
    new G4LogicalBorderSurface("LArInstr_WLSInner_Physical_surf_in",
                               fMotherPhysical,
                               wlsPhysical,
                               fWLSoptSurf);

    new G4LogicalBorderSurface("LArInstr_WLSInner_Physical_surf_out",
                               wlsPhysical,
                               fMotherPhysical,
                               fWLSoptSurf);



    /// -- Fibers


    // - Cladding

    G4VSolid *inner_fiber_cladding_solid = BuildFiberShroudSolid(fDetectorDB->GetDN500StringDistance()/2.0 + 0.5*fFiberThickness ,
                                                                 fFiberThickness,
                                                                 fShroud_CenterHeight,
                                                                 false);
    inner_fiber_cladding_solid->SetName("LArInstr_FiberInnerCladding_Solid");

    G4LogicalVolume *inner_fiber_cladding_logvol = new G4LogicalVolume(inner_fiber_cladding_solid,
                                                                       fiber_cladding_material,
                                                                       "LArInstr_FiberInnerCladding_Logical");

    inner_fiber_cladding_logvol->SetVisAttributes(cladding_vis_att);

    G4VPhysicalVolume *inner_cladding_phys = new G4PVPlacement(0,
                                                               G4ThreeVector(0.0,0.0,0.0),
                                                               inner_fiber_cladding_logvol,
                                                               "LArInstr_FiberInnerCladding_Physical",
                                                               inner_wls_logvol,
                                                               false,
                                                               0);



    G4VSolid *inner_fiber_solid = BuildFiberShroudSolid(fDetectorDB->GetDN500StringDistance()/2.0 + 0.5*fFiberThickness-0.5*fFiberCladThickness,
                                                        fFiberThickness-fFiberCladThickness,
                                                        fShroud_CenterHeight,
                                                        false);
    inner_fiber_solid->SetName("LArInstr_FiberInner_Solid");

    inner_fiber_logvol = new G4LogicalVolume(inner_fiber_solid,
                                             fiber_material,
                                             "LArInstr_FiberInner_Logical");

    inner_fiber_logvol->SetVisAttributes(fiber_vis_att);

    G4VPhysicalVolume *inner_fiber_phys = new G4PVPlacement(0,
                                                            G4ThreeVector(0.0,0.0,0.0),
                                                            inner_fiber_logvol,
                                                            "LArInstr_FiberInner_Physical",
                                                            inner_fiber_cladding_logvol,
                                                            false,
                                                            0);


    // Create auxiliary volumes to attach the reflective and collecting surfaces

    G4VSolid *inner_fiber_end_solid = BuildFiberShroudSolid(fDetectorDB->GetDN500StringDistance()/2.0 + 0.5*fFiberThickness,
                                                            fFiberThickness,
                                                            fiber_end_height,
                                                            false);
    inner_fiber_solid->SetName("LArInstr_InnerFiberEnd_Solid");

    G4LogicalVolume *inner_fiber_end_logical_sensitive = new G4LogicalVolume(inner_fiber_end_solid,
                                                                             G4Material::GetMaterial("MetalSilicon"),
                                                                             "LArInstr_FiberInnerEnd_Top_Logical");

    // Now place two volumes, one at the top and one at the bottom
    G4VPhysicalVolume *fiber_end_phys = new G4PVPlacement(0,
                                                          G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z()+0.5*fShroud_CenterHeight+0.5*fiber_end_height),
                                                          inner_fiber_end_logical_sensitive,
                                                          "LArInstr_FiberInner_SensitiveEnd_Physical",
                                                          fMotherLogical,
                                                          false,
                                                          0);

    // Apply the logical detecting surface to this end
    // Attach the reflective and sensitive surfaces to the border between the shroud and the cladding
    new G4LogicalBorderSurface("LArInstr_FiberInnerCladding_Sensitive_surf",
                               inner_cladding_phys,
                               fiber_end_phys,
                               sipmOptSurface);

    new G4LogicalBorderSurface("LArInstr_FiberInner_Sensitive_surf",
                               inner_fiber_phys,
                               fiber_end_phys,
                               sipmOptSurface);

    G4LogicalVolume *inner_fiber_end_logical_reflective = new G4LogicalVolume(inner_fiber_end_solid,
                                                                             G4Material::GetMaterial("MetalSilicon"),
                                                                             "LArInstr_Fiber_End_Bottom_Logical");
    G4VPhysicalVolume *fiber_end_refl_phys = new G4PVPlacement(0,
                                                                 G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z()-0.5*fShroud_CenterHeight-0.5*fiber_end_height),
                                                                 inner_fiber_end_logical_reflective,
                                                                 "LArInstr_Fiber_Inner_End_Bottom_Physical",
                                                                 fMotherLogical,
                                                                 false,
                                                                 1);

    // Attach the reflective surfaces to the border between the shroud and bottom end
    new G4LogicalBorderSurface("LArInstr_FiberInnerCladding_Refl_surf",
                               inner_cladding_phys,
                               fiber_end_refl_phys,
                               fiberReflOptSurface);

    new G4LogicalBorderSurface("LArInstr_FiberInner_Refl_surf",
                               inner_fiber_phys,
                               fiber_end_refl_phys,
                               fiberReflOptSurface);

    /*
		//Associates the OpticalSurface to the actual volumes
		new G4LogicalSkinSurface("fiber_inner_opsurface",inner_fiber_logvol,fiberOptSurface);
     */


    fSDLogicVec.push_back(inner_fiber_end_logical_sensitive);
    fSDLogicNameVec.push_back("Fiber_inner");
    fSDLogicInstanceVec.push_back(1);

  }

  G4VSolid*fCopperSupportRing_Solid = new G4Tubs("CopperSupportRing_Solid",
						 fShroud_TubsOuterRadius,
						 fShroud_TubsOuterRadius + 5*mm,
						 1*mm/2.,
						 0,
						 2*M_PI);

  G4LogicalVolume *fCopperSupportRing_Logical = new G4LogicalVolume(fCopperSupportRing_Solid,
								    G4Material::GetMaterial("MetalCopper"),
								    "CopperSupportRing_Logical");

  /*wlsPhysical = */new G4PVPlacement(0,
				      G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z() + 800.*mm/(2*3)),
				      fCopperSupportRing_Logical,
				      "LArInstr_CopperSupportRing_Physical_0",
				      fMotherLogical,
				      false,
				      0);

  /*wlsPhysical = */new G4PVPlacement(0,
				      G4ThreeVector(fDetCenter.x(),fDetCenter.y(),fDetCenter.z() - 800.*mm/(2*3)),
				      fCopperSupportRing_Logical,
				      "LArInstr_CopperSupportRing_Physical_1",
				      fMotherLogical,
				      false,
				      0);

}

G4VSolid* GEGeometryLArInstHybrid::BuildFiberShroudSolid(G4double outer_radius,G4double thickness, G4double height, G4bool endcap, G4double endcap_ratio) {
  // Build the solid representing the fiber shroud.
  // If endcaps are specified, build it as a G4Polycone (to avoid edges and other odd effects with union solids)
  // If endcaps are not be built then construct it as a G4Tubs

  G4double inner_radius =outer_radius - thickness;
  if (endcap) {
    // The construction of the design is obtained by combining two different solids
    // 1. a tube which makes the body
    // 2. A circular shape that makes the bottom and the top.
    // The shape is basically a cylinder with rounded edges on the top and bottom.


    //G4double design_Height = fShroud_CenterHeight; 		// height of the design
    // Make the height of the bending part 5% of the staight part.

    G4double design_edge_bend = endcap_ratio*fShroud_CenterHeight; 	// The height of the bending part

    // The design itself is then a cylinder glued to two
    // spherical caps.
    // For the upper part this is not entirely true, but it is an approximation

    // The radius of the circle defining the cap should be
    // l^2 = (l-design_edge_Bend)^2 + design_innerRadius^2
    // l = 0.5*(1./design_edge_Bend)*(design_edge_Bend^2 + design_innerRadius^2)
    G4double circ_radius_inner = 0.5*(1./design_edge_bend)*(design_edge_bend*design_edge_bend + inner_radius*inner_radius);
    G4double circ_radius_outer = circ_radius_inner + thickness;

    // Both circumferences are centered in (0,0,zcenter = design_Height+design_edge_Bend)
    G4ThreeVector circ_center(0,0,-1.0*(0.5*height+design_edge_bend-circ_radius_inner));


    const G4int poly_npts = 102;
    G4double dist_step = design_edge_bend / 50.;

    G4double rin[poly_npts];
    G4double rout[poly_npts];
    G4double zpos[poly_npts];

    G4double z_bottom_outer = -1.0 * (height/2.0) - design_edge_bend - thickness;
    G4double z_bottom_inner = z_bottom_outer + thickness;

    G4int pos = 0;
    for (int i = 0; i <poly_npts; ++ i) {
      // Build from bottom to top.
      // First 40 points is the curved shape at the bottom.

      if (i<(poly_npts/2)) {
        zpos[i] = z_bottom_outer + dist_step*(G4double)i;

        // both radii are the solution of the sphere equation
        rout[i] = sqrt(circ_radius_outer*circ_radius_outer - pow((zpos[i]-circ_center.z()),2.0));
        if (zpos[i] < z_bottom_inner) {
          rin[i] = 0.0;
        } else {
          rin[i] = sqrt(circ_radius_inner*circ_radius_inner - pow((zpos[i]-circ_center.z()),2.0));
        }
      }
      if ((i==50) || (i == 51)) {
        // these two entries mark the cylider itself.
        zpos[i] = (i==50)?(-1.0*height/2.0):(height/2.0);
        rin[i] = inner_radius;
        rout[i] = inner_radius + thickness;
      }
      // upper part. Similar to lower with a few additional touches.
      if (i > 51) {
        // The center of the circumference is displaced in Z
        //circ_center.setZ(circ_center.Z());
        zpos[i] = zpos[i-1] + dist_step;
        // it should look exactly the same as the other side.
        pos = 49-(i-52);
        rout[i] = rout[pos];
        if (rout[i] < 0.0) rout[i] = 0.0;

        if (zpos[i] > (-1.0)*z_bottom_inner) {
          rin[i] = 0.0;
        } else {
          rin[i] = rin[pos];
        }
        if (rin[i] < 0.0) rin[i] = 0.0;
      }
    }
    // DEBUG: Print the points prior to creating the volume
    /* Leave it commented for now. But keep it for future debugging
		for (G4int i = 0; i < poly_npts; ++i) {
			MGLog(debugging) << " Point " << i << " zpos " << zpos[i] << " radius " << rin[i] << " , "<< rout[i] << endlog;
		}
     */

    // Now build the polycone and return
    return (new G4Polycone("tmp_solid",0.0, 2*M_PI,
                           poly_npts,
                           zpos,rin,rout));
  }
  else {
    return (new G4Tubs("tmp_solid",inner_radius,outer_radius,height/2.0,0.0,2*M_PI));
  }


}

G4VSolid* GEGeometryLArInstHybrid::BuildFiberShroudSegmentSolid(G4double radius, G4int fNoOfSegments, G4double segment_spacing, G4double thickness, G4double total_thickness, G4double height) {
  // Buils solid representing a segment of the filer shroud

  G4double inner_radius = radius - thickness/2.;
  G4double outer_radius = radius + thickness/2.;

  G4double start_angle = (total_thickness-thickness)/2./radius;
  G4double stop_angle  = 2*M_PI/fNoOfSegments - (total_thickness-thickness)/2./radius - segment_spacing/radius;

  //G4double angle  = M_PI/fNoOfSegments - segment_spacing/2./radius - (total_thickness-thickness)/2./radius;

  MGLog(debugging) << "Placing shroud segment with angle " << start_angle << " to " << stop_angle << endlog;

  return (new G4Tubs("tmp_solid",inner_radius,outer_radius,height/2.0,start_angle,stop_angle));
}
