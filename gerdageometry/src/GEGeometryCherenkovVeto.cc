//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                            MaGe Simulation                                //
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
// $Id: GEGeometryCherenkovVeto.cc,v 1.9 2007-12-05 10:51:29 schubert Exp $ 
//      
// CLASS IMPLEMENTATION:  @CLASS_NAME@.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * this is the class for the whole Gerda enviroument setup
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Markus Knapp
 * CONTACT: @msknapp@pit.physik.uni-tuebingen.de@
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 *
 * This file includes the main parts for the Cherenkov Veto, that are, several
 * distributions for the Photomultipliers and several VM2000 foils.
 *
 * 06-07-2006 Some calls to detectordb variables which were not used were 
 *            commented to avoid problems with the new geometry (these 
 *            variables do not exist anymore), Claudia and Assunta
 *
 * 09-11-2006 Added new distribution of PMTs, as standard distribution.
 *
 * 12-18-2006 Corrected an issue that caused MaGe to crash, if the MGGeneratorData variable was not set
 *            Changed the standard distribution number to zero, so that it is used, if not manually deactivated
 *
 * 03-15-2007 Added the cable tray for PMT cabling
 * 10-17-2007 Replace G4cout with MGLog, Luciano
 * 12-05-2007 Changes due to the changes of the cryostat, Jens
 */
//---------------------------------------------------------------------------//
//
// G4 includes
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trap.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"


// MG include
#include "io/MGLogger.hh"

// Gerda include

#include "gerdageometry/GEGeometryCherenkovVeto.hh"
#include "gerdageometry/GEGeometryPMTSD.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//

GEGeometryCherenkovVeto::GEGeometryCherenkovVeto(GEGeometryDetectorDB* detectordb)
{
  fShieldingWaterTankHeight            = detectordb -> GetShieldingWaterTankHeight(); 
  fShieldingWaterHeight                = detectordb -> GetShieldingWaterHeight(); 
  fShieldingWaterRadius                = 500*cm;
  fShieldingAirBufferHeight            = detectordb -> GetShieldingAirBufferHeight(); 

  fShieldingFootHeight                 = detectordb -> GetShieldingFootHeight(); 
  fShieldingCopperShieldingLargeHeight = detectordb -> GetShieldingCopperShieldingLargeHeight(); 
  fShieldingCopperShieldingSmallHeight = detectordb -> GetShieldingCopperShieldingSmallHeight(); 

  fShieldingFootIR                     = detectordb -> GetShieldingFootIR();  
  fReflectionFoilThickness             = detectordb -> GetReflectionFoilThickness(); 

  //especially for muon veto
  fPhotocathodeInnerRadius             = detectordb -> GetPhotocathodeInnerRadius();
  fPhotocathodeOuterRadius             = detectordb -> GetPhotocathodeOuterRadius();
  fPMTInnerRadius                      = detectordb -> GetPMTInnerRadius();
  fPMTOuterRadius                      = detectordb -> GetPMTOuterRadius();
  fPMTSteelConeThickness               = detectordb -> GetPMTSteelConeThickness();
  fPMTSteelConeHeight                  = detectordb -> GetPMTSteelConeHeight();
  fPMTSteelConeUpperRmin               = detectordb -> GetPMTSteelConeUpperRmin();
  fPMTSteelConeUpperRmax               = detectordb -> GetPMTSteelConeUpperRmax();
  fPMTSteelConeLowerRmin               = detectordb -> GetPMTSteelConeLowerRmin();
  fPMTSteelConeLowerRmax               = detectordb -> GetPMTSteelConeLowerRmax();
  fPMTSteelBottomHeight                = detectordb -> GetPMTSteelBottomHeight();
  fReflectionFoilThickness             = detectordb -> GetReflectionFoilThickness();
  fPMTCableTrayThickness               = detectordb -> GetPMTCableTrayThickness();
  fPMTVerticalCableTrayHeight          = detectordb -> GetPMTVerticalCableTrayHeight();
  fPMTHorizontalCableTrayWidth         = detectordb -> GetPMTHorizontalCableTrayWidth();

  fWaterLogical                        = detectordb -> GetWaterLogical(); 
  fPMTSteelConeLogical                 = detectordb -> GetPMTSteelConeLogical();
  fPMTSteelBottomLogical               = detectordb -> GetPMTSteelBottomLogical();
  fPhotocathodeLogical                 = detectordb -> GetPhotocathodeLogical();
  fReflectionFoilWaterTankTubeLogical  = detectordb -> GetReflectionFoilWaterTankTubeLogical();
  fReflectionFoilWaterTankBottomLogical  = detectordb -> GetReflectionFoilWaterTankBottomLogical();
  fReflectionFoilCryoFootLogical       = detectordb -> GetReflectionFoilCryoFootLogical();
  fReflectionFoilCryoFootTopLogical    = detectordb -> GetReflectionFoilCryoFootTopLogical();
  fReflectionFoilCryoFootBottomLogical = detectordb -> GetReflectionFoilCryoFootBottomLogical();
  fHorizontalPMTCableTrayLogical       = detectordb -> GetHorizontalPMTCableTrayLogical();
  fVerticalPMTCableTrayLogical         = detectordb -> GetVerticalPMTCableTrayLogical(); 

  dist_number                          = detectordb -> GetSelectPMTDistribution();

    WLSyield = 0.6;
    successfulInitialization = false;
    InitializeVM2000Spectrum();

}

// ---------------------------------------------------------------------------

GEGeometryCherenkovVeto::~GEGeometryCherenkovVeto()
{
  
}

// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------

void GEGeometryCherenkovVeto::ConstructCherenkovVeto()
{

    G4double distancetobottom = 20.0 * cm;

// ----------------------------------------------------------------------------------------------------
// ************************************** Photomultiplier: ********************************************
// ----------------------------------------------------------------------------------------------------


    G4double xpos = 0;
    G4double ypos = 0;    
    G4double zpos = 0;

    G4RotationMatrix rm_x, rm_y, rm_z, rm_corr, rm_0;           // Declaration of several rotation-metrices

    rm_0.rotateZ(0);               // To rotate everything back to zero
    rm_y.rotateY(M_PI);            // 180 degrees around Y FOR  TOP PMTs
    rm_z.rotateY(M_PI/2.);         //  90 degrees around Y FOR SIDE PMTs


    


// --------------------------------------------------------------------
// PMT definition  ****************************************************
// --------------------------------------------------------------------
    
    G4int  Num_PMTs = 0;                  // Counts total number of PMTs, serves as copynumber

    G4int  PMTDistribution = 1;           // Selecting PMT distribution, hardcoded until now

    G4int  N_PMT_per_ring  = 10;          // Number of PMT's per ring (standard is 10)
    
    //G4double Phi_Ini;
    G4double dPhi, dPhi_c, R_pos;

    char namephotocathode[50];
    char namesteelcone[50];
    char namesteelbottom[50];

    if(dist_number==-1)
      {
	MGLog(routine) << "Warning: photomultipliers deactivated" << endlog ;
	PMTDistribution = -1;
      }
    else if(dist_number==0)
      {
	MGLog(routine) << " Standard distribution of photomultipliers selected" << endlog ;
	PMTDistribution = 0;
      }
    else if(dist_number==1)
      {
	MGLog(routine) << " Alternative distribution 1 of photomultipliers selected" << endlog;
	PMTDistribution = 1;
      }
    else
      {
	MGLog(routine) << "No valid distribution of photomultipliers selected, using standard distribution" << endlog;
	PMTDistribution = 0; 
      }

    // ************************************************************************************************
    // Standard distribution, 66 PMTs, 20 on the bottom, 40 on the wall and 6 in the pillbox
    // ************************************************************************************************

    if(PMTDistribution==0)
      {
	// -------------------------------------------------------------------------
	//  Bottom PMT's (at R=425 cm, 12 PMTs) ************************************
	// -------------------------------------------------------------------------

	R_pos=425.0*cm;
	N_PMT_per_ring = 12; 
	
	//Phi_Ini = 0.0; 
	dPhi=2.*M_PI/N_PMT_per_ring; 
	dPhi_c = dPhi; 

     


	// ------ Placement of the PMTs ------------------------------
	
	for (G4int k=1; k<(N_PMT_per_ring+1); k++) 
	  {
	    xpos=R_pos*cos(dPhi_c); 
	    ypos=R_pos*sin(dPhi_c); 
	    zpos=-390.0*cm;

	    sprintf(namephotocathode,"PMTphotocathode_%d", Num_PMTs++);
	    sprintf(namesteelcone,"PMTsteelcone_%d", Num_PMTs);
	    sprintf(namesteelbottom,"PMTsteelbottom_%d", Num_PMTs);
	    
	    fPhotocathodePhysical = new G4PVPlacement(0,
						      G4ThreeVector(xpos,ypos,zpos),
						      fPhotocathodeLogical,
						      namephotocathode,
						      fWaterLogical,
						      false,
						      Num_PMTs);
	    
	    fPMTSteelConePhysical = new G4PVPlacement(0,
						      G4ThreeVector(xpos,ypos,zpos-fPMTSteelConeHeight*0.5),
						      fPMTSteelConeLogical,
						      namesteelcone,
						      fWaterLogical,
						      false,
						      Num_PMTs);
	 
	    fPMTSteelBottomPhysical = new G4PVPlacement(0,
							G4ThreeVector(xpos,ypos,zpos-fPMTSteelConeHeight-fPMTSteelBottomHeight*0.5),
							fPMTSteelBottomLogical,
							namesteelbottom,
							fWaterLogical,
							false,
							Num_PMTs);
	    
	    
	    dPhi_c += dPhi;
	  }
      
    
    // -------------------------------------------------------------------------
    //  Bottom PMT's ( at R=275 cm, 8 PMTs *************************************
    // -------------------------------------------------------------------------

	R_pos=275.0*cm;
	N_PMT_per_ring = 8;
	
	//Phi_Ini=0.0; 
	dPhi=2.*M_PI/N_PMT_per_ring; 
	dPhi_c = dPhi; 

	
	// ------ Placement of the PMTs ------------------------------
	
	for (G4int k=1; k<(N_PMT_per_ring+1); k++) 
	  {
	    xpos=R_pos*cos(dPhi_c); 
	    ypos=R_pos*sin(dPhi_c); 
	    zpos=-390.0*cm;

	    sprintf(namephotocathode,"PMTphotocathode_%d", Num_PMTs++);
	    sprintf(namesteelcone,"PMTsteelcone_%d", Num_PMTs);
	    sprintf(namesteelbottom,"PMTsteelbottom_%d", Num_PMTs);


	    fPhotocathodePhysical = new G4PVPlacement(0,
						      G4ThreeVector(xpos,ypos,zpos),
						      fPhotocathodeLogical,
						      namephotocathode,
						      fWaterLogical,
						      false,
						      Num_PMTs);
	    
	    fPMTSteelConePhysical = new G4PVPlacement(0,
						      G4ThreeVector(xpos,ypos,zpos-fPMTSteelConeHeight*0.5),
						      fPMTSteelConeLogical,
						      namesteelcone,
						      fWaterLogical,
						      false,
						      Num_PMTs);
	 
	    fPMTSteelBottomPhysical = new G4PVPlacement(0,
							G4ThreeVector(xpos,ypos,zpos-fPMTSteelConeHeight-fPMTSteelBottomHeight*0.5),
							fPMTSteelBottomLogical,
							namesteelbottom,
							fWaterLogical,
							false,
							Num_PMTs);

	    dPhi_c += dPhi;
	  }
  
     
	// -------------------------------------------------------------------------
	//  Placement of Side PMTs  ***********************************************
	// -------------------------------------------------------------------------
	
	G4int NzPMT=4;  // Number of PM-rings on the wall of the watertank
	N_PMT_per_ring=10;
	R_pos=fShieldingWaterRadius-fReflectionFoilThickness-fPMTSteelConeHeight-fPMTSteelBottomHeight;  

	dPhi=2.*M_PI/N_PMT_per_ring; 
     
	for (G4int i=0; i<NzPMT+1; i++) 
	  {
	    if(i==0)
	      {
		zpos = -245.0*cm;
		dPhi_c=dPhi*0.5;       
	      }
	    else if (i==1)
	      {
		zpos = -95.0*cm;
		dPhi_c=0;
	      }
	    else if (i==2) 
	      {
		zpos = 55.0*cm;
		dPhi_c=dPhi*0.5;
	      }
	    else if (i==3) 
	      {
		zpos = 205.0*cm;
		dPhi_c=0;
	      }
	    else if (i==4) 
	      {
		N_PMT_per_ring = 6;
		R_pos =  fShieldingFootIR-fReflectionFoilThickness-fPMTSteelConeHeight-fPMTSteelBottomHeight;
		zpos = 0.0-(fShieldingWaterHeight/2.)+distancetobottom*3; 
		dPhi = 2*M_PI/N_PMT_per_ring;
		dPhi_c=0.0;
	      }
	    
	    rm_corr=rm_0;                         // setting everything back to zero
	    if (i==1 || i==3 || i==4  )
	      {
		rm_corr.rotateZ(M_PI-dPhi);           // rotating into the right starting position
	      }
	    else
	      {
		rm_corr.rotateZ(M_PI-dPhi*0.5);           // rotating into the right starting position
	      }
	    
	    for (G4int k=i*N_PMT_per_ring; k<(i+1)*N_PMT_per_ring; k++) 
	      {
		xpos=R_pos*cos(dPhi_c); 
		ypos=R_pos*sin(dPhi_c); 
		rm_corr.rotateZ(dPhi);          // +M_PI depending on the orientation of the PMTs ??


		sprintf(namephotocathode,"PMTphotocathode_%d", Num_PMTs++);
		sprintf(namesteelcone,"PMTsteelcone_%d", Num_PMTs);
		sprintf(namesteelbottom,"PMTsteelbottom_%d", Num_PMTs);		

		fPhotocathodePhysical = new G4PVPlacement(G4Transform3D(rm_corr*rm_z,
									G4ThreeVector(xpos,ypos,zpos)),
							  fPhotocathodeLogical,
							  namephotocathode,
							  fWaterLogical,
							  false,
							  Num_PMTs);
		
		
		xpos=(R_pos+fPMTSteelConeHeight*0.5)*cos(dPhi_c); 
		ypos=(R_pos+fPMTSteelConeHeight*0.5)*sin(dPhi_c); 


		fPMTSteelConePhysical = new G4PVPlacement(G4Transform3D(rm_corr*rm_z,
									G4ThreeVector(xpos,ypos,zpos)),
							  fPMTSteelConeLogical,
							  namesteelcone,
							  fWaterLogical,
							  false,
							  Num_PMTs);

	
		xpos=(R_pos+fPMTSteelConeHeight+fPMTSteelBottomHeight*0.5)*cos(dPhi_c); 
		ypos=(R_pos+fPMTSteelConeHeight+fPMTSteelBottomHeight*0.5)*sin(dPhi_c);

		fPMTSteelBottomPhysical = new G4PVPlacement(G4Transform3D(rm_corr*rm_z,
									  G4ThreeVector(xpos,ypos,zpos)),						
							    fPMTSteelBottomLogical,
							    namesteelbottom,
							    fWaterLogical,
							    false,
							    Num_PMTs);
		dPhi_c += dPhi;
	      }
	  }
     
     
	MGLog(routine) << "Number of PMTs: " << Num_PMTs << endlog ;
      }
 
    // end of standard distribution of PMTs

    // ************************************************************************************************
    // Alternative distribution 1, 74 PMTs, 20 on the bottom, 50 on the wall and 4 in the pillbox
    // ************************************************************************************************


    if(PMTDistribution==1)
      {
	

	// -------------------------------------------------------------------------
	//  Bottom PMT's (at R=460 cm) ***********************************************
	// -------------------------------------------------------------------------
	
	//Phi_Ini = 0.0; 
	dPhi=2.*M_PI/N_PMT_per_ring; 
	dPhi_c = dPhi; 
	R_pos=460.0*cm;
     


	// ------ Placement of the PMTs ------------------------------
	
	for (G4int k=1; k<(N_PMT_per_ring+1); k++) 
	  {
	    xpos=R_pos*cos(dPhi_c); 
	    ypos=R_pos*sin(dPhi_c); 
	    zpos=-390.0*cm;

	    sprintf(namephotocathode,"PMTphotocathode_%d", Num_PMTs++);
	    sprintf(namesteelcone,"PMTsteelcone_%d", Num_PMTs);
	    sprintf(namesteelbottom,"PMTsteelbottom_%d", Num_PMTs);
	    
	    fPhotocathodePhysical = new G4PVPlacement(0,
						      G4ThreeVector(xpos,ypos,zpos),
						      fPhotocathodeLogical,
						      namephotocathode,
						      fWaterLogical,
						      false,
						      Num_PMTs);
	    
	    fPMTSteelConePhysical = new G4PVPlacement(0,
						      G4ThreeVector(xpos,ypos,zpos-fPMTSteelConeHeight*0.5),
						      fPMTSteelConeLogical,
						      namesteelcone,
						      fWaterLogical,
						      false,
						      Num_PMTs);
	 
	    fPMTSteelBottomPhysical = new G4PVPlacement(0,
							G4ThreeVector(xpos,ypos,zpos-fPMTSteelConeHeight-fPMTSteelBottomHeight*0.5),
							fPMTSteelBottomLogical,
							namesteelbottom,
							fWaterLogical,
							false,
							Num_PMTs);
	    
	    
	    dPhi_c += dPhi;
	  }
      
    
    // -------------------------------------------------------------------------
    //  Bottom PMT's ( at R=320 cm) ***********************************************
    // -------------------------------------------------------------------------
	
	//Phi_Ini=0.0; 
	dPhi=2.*M_PI/N_PMT_per_ring; 
	dPhi_c = dPhi; 
	R_pos=320.0*cm;
	
	// ------ Placement of the PMTs ------------------------------
	
	for (G4int k=1; k<(N_PMT_per_ring+1); k++) 
	  {
	    xpos=R_pos*cos(dPhi_c); 
	    ypos=R_pos*sin(dPhi_c); 
	    zpos=-390.0*cm;

	    sprintf(namephotocathode,"PMTphotocathode_%d", Num_PMTs++);
	    sprintf(namesteelcone,"PMTsteelcone_%d", Num_PMTs);
	    sprintf(namesteelbottom,"PMTsteelbottom_%d", Num_PMTs);


	    fPhotocathodePhysical = new G4PVPlacement(0,
						      G4ThreeVector(xpos,ypos,zpos),
						      fPhotocathodeLogical,
						      namephotocathode,
						      fWaterLogical,
						      false,
						      Num_PMTs);
	    
	    fPMTSteelConePhysical = new G4PVPlacement(0,
						      G4ThreeVector(xpos,ypos,zpos-fPMTSteelConeHeight*0.5),
						      fPMTSteelConeLogical,
						      namesteelcone,
						      fWaterLogical,
						      false,
						      Num_PMTs);
	 
	    fPMTSteelBottomPhysical = new G4PVPlacement(0,
							G4ThreeVector(xpos,ypos,zpos-fPMTSteelConeHeight-fPMTSteelBottomHeight*0.5),
							fPMTSteelBottomLogical,
							namesteelbottom,
							fWaterLogical,
							false,
							Num_PMTs);

	    dPhi_c += dPhi;
	  }
  
     
	// -------------------------------------------------------------------------
	//  Placement of Side PMTs  ***********************************************
	// -------------------------------------------------------------------------
	
	G4int NzPMT=5;  // Number of PM-rings on the wall of the watertank
	N_PMT_per_ring=10;
	R_pos=465.0*cm;  
	dPhi=2.*M_PI/N_PMT_per_ring; 
     
	for (G4int i=0; i<NzPMT+1; i++) 
	  {
	    if(i==0)
	      {
		zpos = -280.0*cm;
		dPhi_c=0;       
	      }
	    else if (i==1)
	      {
		zpos = -170.0*cm;
		dPhi_c=dPhi*0.5;
	      }
	    else if (i==2) 
	      {
		zpos = -35.0*cm;
		dPhi_c=0.0;
	      }
	    else if (i==3) 
	      {
		zpos = 115.0*cm;
		dPhi_c=dPhi*0.5;
	      }
	    else if (i==4) 
	      {
		zpos = 295.0*cm;
		dPhi_c=0.0;
	      }
	    else if (i==5) 
	      {
		N_PMT_per_ring = 4;
		R_pos =150*cm;
		zpos = 0.0-(fShieldingWaterHeight/2.)+distancetobottom*3; 
		dPhi = dPhi*10/N_PMT_per_ring;
		dPhi_c=0.0;
	      }
	    
	    rm_corr=rm_0;                         // setting everything back to zero
	    if (i==0 || i==2 || i==4 || i==5 )
	      {
		rm_corr.rotateZ(M_PI-dPhi);           // rotating into the right starting position
	      }
	    else
	      {
		rm_corr.rotateZ(M_PI-dPhi*0.5);           // rotating into the right starting position
	      }
	    
	    for (G4int k=i*N_PMT_per_ring; k<(i+1)*N_PMT_per_ring; k++) 
	      {
		xpos=R_pos*cos(dPhi_c); 
		ypos=R_pos*sin(dPhi_c); 
		rm_corr.rotateZ(dPhi);          // +M_PI depending on the orientation of the PMTs ??


		sprintf(namephotocathode,"PMTphotocathode_%d", Num_PMTs++);
		sprintf(namesteelcone,"PMTsteelcone_%d", Num_PMTs);
		sprintf(namesteelbottom,"PMTsteelbottom_%d", Num_PMTs);		

		fPhotocathodePhysical = new G4PVPlacement(G4Transform3D(rm_corr*rm_z,
									G4ThreeVector(xpos,ypos,zpos)),
							  fPhotocathodeLogical,
							  namephotocathode,
							  fWaterLogical,
							  false,
							  Num_PMTs);
		
		
		xpos=(R_pos+fPMTSteelConeHeight*0.5)*cos(dPhi_c); 
		ypos=(R_pos+fPMTSteelConeHeight*0.5)*sin(dPhi_c); 


		fPMTSteelConePhysical = new G4PVPlacement(G4Transform3D(rm_corr*rm_z,
									G4ThreeVector(xpos,ypos,zpos)),
							  fPMTSteelConeLogical,
							  namesteelcone,
							  fWaterLogical,
							  false,
							  Num_PMTs);

	
		xpos=(R_pos+fPMTSteelConeHeight+fPMTSteelBottomHeight*0.5)*cos(dPhi_c); 
		ypos=(R_pos+fPMTSteelConeHeight+fPMTSteelBottomHeight*0.5)*sin(dPhi_c);

		fPMTSteelBottomPhysical = new G4PVPlacement(G4Transform3D(rm_corr*rm_z,
									  G4ThreeVector(xpos,ypos,zpos)),						
							    fPMTSteelBottomLogical,
							    namesteelbottom,
							    fWaterLogical,
							    false,
							    Num_PMTs);
		dPhi_c += dPhi;
	      }
	  }
     
     
	MGLog(routine) << "Number of PMTs: " << Num_PMTs << endlog ;
      }
  


    


    //  Set the PMTs as sensitive Detectors
    
    G4SDManager* SDman = G4SDManager::GetSDMpointer();

    GEGeometryPMTSD* PMTSD = new GEGeometryPMTSD("Phcathode",Num_PMTs,"PhCathodeHC" );    
    SDman->AddNewDetector(PMTSD); 
    fPhotocathodeLogical->SetSensitiveDetector(PMTSD);








// ---------Introducing several VM2000 reflection foils ----------- //

    // All optical properties of VM 2000 
    // Energy grid goes from UV (115 nm) to green (650 nm) 
    // !!!! VM2000 is no longer available
    G4int ji = 0;
    G4double ee=0;
    const G4int num = 500;
    const G4int num1 = 501;

    static const G4double LambdaE = twopi*1.973269602e-16 * m * GeV;

    G4double PPSCHighE = LambdaE /(115*nanometer);
    G4double PPSCLowE = LambdaE /(650*nanometer); //598
    G4double dee = ((PPSCHighE - PPSCLowE) / ((G4double)(num-1)));
    G4double LAr_SCPP[num1];
    for (ji = 1; ji < num1; ji++)
      {
	ee=PPSCLowE+ ((G4double)ji) * dee;
	LAr_SCPP[ji]=ee;
      }
    LAr_SCPP[0] = 1.8*eV;

    G4double Reflectivity[num1];
    G4double Efficiency[num1];
    G4double WLS_absorption[num1];
    G4double WLS_emission[num1];

    for (ji=1;ji < num1; ji++)
      {
	if (LAr_SCPP[ji] < (LambdaE/(370*nanometer)))
	  {
	    Reflectivity[ji] = 0.95; //visible
	  }
	else
	  {
	    Reflectivity[ji] = 0.15; //UV
	  }
	
	Efficiency[ji] = 0.0;

	if (LAr_SCPP[ji] > 5*eV)
	  {
	    WLS_absorption[ji] = CalculateWLSmfp(WLSyield); //absorbs UV (sometimes...)
	  }
	else
	  {
	  WLS_absorption[ji] = 10.*m; //otherwise imperturbed
	  }

	WLS_emission[ji] = VM2000EmissionSpectrum(LAr_SCPP[ji]);
      }

    Reflectivity[0] = Reflectivity[1];
    Efficiency[0]   = Efficiency[1];
    WLS_absorption[0] = WLS_absorption[1];
    WLS_emission[0] = WLS_emission[1];


    G4MaterialPropertiesTable *VM2000OptTable = new G4MaterialPropertiesTable();
    VM2000OptTable->AddProperty("REFLECTIVITY",LAr_SCPP,Reflectivity,num1);
    VM2000OptTable->AddProperty("EFFICIENCY",LAr_SCPP,Efficiency,num1);  
    
    G4OpticalSurface* VM2000OptSurface 
      = new G4OpticalSurface("Refl surface",glisur,polished,dielectric_metal);
    
    VM2000OptSurface->SetMaterialPropertiesTable(VM2000OptTable);

    G4double Refraction[num1];
    G4double Rayl[num1];
    G4double AbsorptionL[num1];
    G4MaterialPropertiesTable *nylonTable = new G4MaterialPropertiesTable();
    for (ji=1;ji < num1; ji++)
      {
	Refraction[ji] = 1.15; //this is just a guess
	Rayl[ji] = 10*m;
	AbsorptionL[ji] = 50*m;
      }

    Refraction[0] = Refraction[1];
    Rayl[0] = Rayl[1];
    AbsorptionL[0] = AbsorptionL[1];

    // add new optical properties for VM2000

    nylonTable->AddProperty("RINDEX",LAr_SCPP,Refraction,num1);
    nylonTable->AddProperty("ABSLENGTH",LAr_SCPP,AbsorptionL,num1);
    nylonTable->AddProperty("WLSABSLENGTH",LAr_SCPP,WLS_absorption,num1);
    nylonTable->AddProperty("WLSCOMPONENT",LAr_SCPP,WLS_emission,num1);
    nylonTable->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);

    G4Material::GetMaterial("Nylon")->SetMaterialPropertiesTable(nylonTable);

    
    // -------------------------------------------------------------------
    // volume: foil covering the water tank tube
    // -------------------------------------------------------------------

    xpos=0.0;
    ypos=0.0;
    zpos=0.0;

    fReflectionFoilWaterTankTubePhysical= new G4PVPlacement(0,
							    G4ThreeVector(xpos,ypos,zpos),
							    fReflectionFoilWaterTankTubeLogical,
							    "fReflectionFoilWaterTankTubePhysical",
							    fWaterLogical,
							    false,
							    0);
    
    new G4LogicalSkinSurface("WaterTankTubeFoilSurface",fReflectionFoilWaterTankTubeLogical,VM2000OptSurface);
    
    // -------------------------------------------------------------------
    // volume: foil covering the water tank bottom and the copper shield
    // -------------------------------------------------------------------    
    
    xpos=0.0;
    ypos=0.0;
    zpos=-fShieldingWaterHeight*0.5+fReflectionFoilThickness*0.5;

    fReflectionFoilWaterTankBottomPhysical = new G4PVPlacement(0,
							      G4ThreeVector(xpos,ypos,zpos),
							      fReflectionFoilWaterTankBottomLogical,
							      "fReflectionFoilWaterTankBottomPhysical",
							      fWaterLogical,
							      false,
							      0);

    new G4LogicalSkinSurface("WaterTankBottomFoilSurface",fReflectionFoilWaterTankBottomLogical,VM2000OptSurface);

    // -------------------------------------------------------------------
    // volume: foil covering the inside of the foot of the cryo tank
    // -------------------------------------------------------------------    
    
    xpos=0.0;
    ypos=0.0;
    zpos = - 0.5*fShieldingWaterHeight + fShieldingCopperShieldingLargeHeight + 0.5*fShieldingFootHeight;

    fReflectionFoilCryoFootPhysical = new G4PVPlacement(0,
 							G4ThreeVector(xpos,ypos,zpos),
 							fReflectionFoilCryoFootLogical,
 							"fReflectionFoilCryoFootPhysical",
 							fWaterLogical,
 							false,
 							0);

    new G4LogicalSkinSurface("WaterTankBottomFoilSurface",fReflectionFoilCryoFootLogical ,VM2000OptSurface);

    
    // -------------------------------------------------------------------
    // volume: foil covering the inside top of the foot of the cryo tank
    // -------------------------------------------------------------------    
    
    xpos=0.0;
    ypos=0.0;
    zpos = - 0.5*fShieldingWaterHeight + fShieldingCopperShieldingLargeHeight + fShieldingFootHeight;

    fReflectionFoilCryoFootTopPhysical = new G4PVPlacement(0,
							   G4ThreeVector(xpos,ypos,zpos),
							   fReflectionFoilCryoFootTopLogical,
							   "fReflectionFoilCryoFootTopPhysical",
							   fWaterLogical,
							   false,
							   0);

    new G4LogicalSkinSurface("WaterTankBottomFoilSurface",fReflectionFoilCryoFootTopLogical ,VM2000OptSurface);
    

    // -------------------------------------------------------------------
    // volume: foil covering the inside bottom of the foot of the cryo tank
    // -------------------------------------------------------------------    
    
    xpos=0.0;
    ypos=0.0;
    zpos = - 0.5 * fShieldingWaterHeight + fShieldingCopperShieldingLargeHeight + fShieldingCopperShieldingSmallHeight + 0.5 * fReflectionFoilThickness;

    fReflectionFoilCryoFootBottomPhysical = new G4PVPlacement(0,
							      G4ThreeVector(xpos,ypos,zpos),
							      fReflectionFoilCryoFootBottomLogical,
							      "fReflectionFoilCryoFootBottomPhysical",
							      fWaterLogical,
							      false,
							      0);

    new G4LogicalSkinSurface("WaterTankBottomFoilSurface",fReflectionFoilCryoFootBottomLogical ,VM2000OptSurface);


    // -------------------------------------------------------------------
    // volume: cable trays for PMT cables on top of water
    // -------------------------------------------------------------------    
    
    xpos=0.0;
    ypos=0.0;
    zpos = +0.5*fShieldingWaterHeight-0.5*fPMTCableTrayThickness;

    fHorizontalPMTCableTrayPhysical = new G4PVPlacement(0,
  							G4ThreeVector(xpos,ypos,zpos),
   							fHorizontalPMTCableTrayLogical,
  							"fHorizontalPMTCableTrayPhysical",
   							fWaterLogical,
  							false,
   							0);



    rm_corr=rm_0;                         // setting everything back to zero
    dPhi = 2*M_PI/36.;
    dPhi_c = 0;
    rm_corr.rotateZ(M_PI-dPhi);           // rotating into the right starting position
    R_pos = fShieldingWaterRadius - fReflectionFoilThickness - 0.5*fPMTHorizontalCableTrayWidth;
    zpos = +0.5*fShieldingWaterHeight-0.5*fPMTVerticalCableTrayHeight-fPMTCableTrayThickness;

    for (G4int k=0; k<36; k++) 
      {
	xpos=R_pos*cos(dPhi_c); 
	ypos=R_pos*sin(dPhi_c); 
	rm_corr.rotateZ(dPhi);          // +M_PI depending on the orientation of the PMTs ??
	
	fVerticalPMTCableTrayPhysical = new G4PVPlacement(G4Transform3D(rm_corr,
									G4ThreeVector(xpos,ypos,zpos)),
							  fVerticalPMTCableTrayLogical,
							  "fVerticalPMTCableTrayPhysical",
							  fWaterLogical,
							  false,
							  k);
	dPhi_c = dPhi_c+dPhi;


      }


  
    
}



void GEGeometryCherenkovVeto::InitializeVM2000Spectrum()
{
  char* path = getenv("MGGENERATORDATA");
  if (!path)
    {
      MGLog(warning)<< "MGGENERATORDATA environment variable not set!, using simplified data for radiant mirror foil" << endlog;
      for (G4int i=0; i<251; i++)
	{
	  frequencyV[i] = (i+350)*nanometer;
	  efficiencyV[i] = 20;
	}
      MGLog(warning) << "Data file not found!" << endlog;
      successfulInitialization = true;
    }
  else
    {
      G4String pathString(path);
      G4String pathFile = pathString + "/VM2000_em.dat";
      std::ifstream file(pathFile);
      if (file.is_open())
	{
	  G4double aa,bb,cc,dd,ee,ff,gg,hh;      
	  for (G4int i=0;i<npoints;i++)
	    {
	      file >> aa >> bb >> cc >> dd >> ee >> ff >> gg >> hh;
	      frequencyV[i] = gg*nanometer;
	      efficiencyV[i] = hh;
	    }
	  file.close();
	  successfulInitialization = true;
	  MGLog(routine) << "VM2000 data successfully read from file" << endlog;
	}
      else
	{
	  MGLog(warning) << "Data file not found!" << endlog;
	  successfulInitialization = false;
	}
    }
}


G4double GEGeometryCherenkovVeto::VM2000EmissionSpectrum(G4double energy)
{
  G4int j=0;
  if (successfulInitialization)
    {
      //Here the data is loaded
      static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
      G4double targetf = LambdaE/energy;

      if (targetf < frequencyV[0] || targetf > frequencyV[npoints-1])
        return 0.0;

      for(j=0;j<npoints-1;j++)
        {
          if (frequencyV[j]<targetf && targetf <= frequencyV[j+1]) break;
        }
      G4double eff = (targetf-frequencyV[j])
        *(efficiencyV[j+1]-efficiencyV[j])/(frequencyV[j+1]-frequencyV[j]);
      eff += efficiencyV[j];
      return eff;
    }
  else
    {
      return 0.2;
    }
}
G4double GEGeometryCherenkovVeto::CalculateWLSmfp(G4double yield)
{
  G4double totalPath = 1*mm; //hardcoded, for now
  if (yield < 0. || yield > 1.) 
    {
      MGLog(warning) << "Yield cannot be larger than 100%!" << endlog;
      MGLog(warning) << "Yield scaled to 100%" << endlog;
    }
  if (yield == 0) return 10.*m;
  if (yield == 1) return 0.01*mm;
  G4double help = std::log(1.-yield);
  G4double mfp = -1*totalPath/help;
  return mfp;
}
