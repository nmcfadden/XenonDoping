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
// CLASS IMPLEMENTATION:  GEGeometryLArInstFibers.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES: Simple example implementation. See header for details.
 *
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: N. Barros
 * CONTACT: nuno *dot* barros *at* tu-dresden *dot* de
 * FIRST SUBMISSION: 11-02-2011, N. Barros
 *
 * REVISION:
 * 05-23-2012, Nuno, - Refactored the cpde to be more robust. The instrumentation should now be
 * 					 centered in the lock and crystals, independently of the lock selected (need further verification).
 * 					 - Attached additional optical properties (to Germanium and Copper).
 *
 * 05-25-2012, Nuno, - Corrected the length of the fiber design to 1.0 m (was 0.5 m)
 * 					 - Corrected a problem with the dimension of the circular caps of the fibers.
 *
 *
 */
//---------------------------------------------------------------------------//

#include "gerdageometry/GEGeometryLArInstFibers.hh"
#include "gerdageometry/GEGeometryCryostatParts.hh"
#include "io/MGLogger.hh"

#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4NistManager.hh"

using namespace CLHEP;

GEGeometryLArInstFibers::GEGeometryLArInstFibers(GEGeometryDetectorDB* theGeometryDB) : fFiberLogical(0) /*,
fInitDone(false)*/ {
	// TODO Auto-generated constructor stub
	fDetectorDB = theGeometryDB;
	fMotherLogical = fDetectorDB->GetDetectorMotherVolume();
	return;
}

GEGeometryLArInstFibers::~GEGeometryLArInstFibers() {
	if (fFiberLogical) delete fFiberLogical;
}

void GEGeometryLArInstFibers::ConstructDesign() {

	// The construction of the design is obtained by combining two different solids
	// 1. a tube which makes the body
	// 2. A circular shape that makes the bottom and the top.

	// The shape is basically a cylinder with rounded edges on the top and bottom.
    G4double design_innerRadius = 125.0*mm;
    G4double fiber_Thickness   	= 1.0*mm;
    G4double design_Height = 1.0*m; 		// height of the design
    G4double design_edge_Bend = 50.0*mm; 	// The height of the bending part

    // The design itself is then a cylinder glued to two
    // spherical caps.
    // For the upper part this is not entirely true, but it is an approximation

    // The radius of the circle defining the cap should be
    // l^2 = (l-design_edge_Bend)^2 + design_innerRadius^2
    // l = 0.5*(1./design_edge_Bend)*(design_edge_Bend^2 + design_innerRadius^2)
    G4double circ_radius_inner = 0.5*(1./design_edge_Bend)*(design_edge_Bend*design_edge_Bend + design_innerRadius*design_innerRadius);
    G4double circ_radius_outer = circ_radius_inner + 1.0*mm;

    // Both circumferences are centered in (0,0,zcenter = design_Height+design_edge_Bend)
    G4ThreeVector circ_center(0,0,-1.0*(0.5*design_Height+design_edge_Bend-circ_radius_inner));
    // The coordinates are taken from the lock, but they work simply as
    // a translation. For the moment the whole thing is being constructed in a referencial
    // of its own.


    // Use 1*mm steps to build the tables;
    const G4int poly_npts = 102;

    G4double rin[poly_npts];
    G4double rout[poly_npts];
    G4double zpos[poly_npts];

    G4double z_bottom_outer = -1.0 * (design_Height/2.0) - design_edge_Bend - fiber_Thickness;
    G4double z_bottom_inner = z_bottom_outer + fiber_Thickness;
    //G4double z_cyl_border 	= -1.0 * (design_Height/2.0);

    // Build the function to get the radius of the bottom part
    //G4double a,b;
    G4int pos = 0;
    for (int i = 0; i <poly_npts; ++ i) {
    	// Build from bottom to top.
    	// First 40 points is the curved shape at the bottom.

    	if (i<50) {
    		zpos[i] = z_bottom_outer + 1.0*mm*(G4double)i;

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
    		zpos[i] = (i==50)?(-1.0*design_Height/2.0):(design_Height/2.0);
    		rin[i] = design_innerRadius;
    		rout[i] = design_innerRadius + fiber_Thickness;
    	}
    	// upper part. Similar to lower with a few additional touches.
    	if (i > 51) {
    		// The center of the circumference is displaced in Z
    		//circ_center.setZ(circ_center.Z());
    		zpos[i] = zpos[i-1] + 1.0*mm;
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
    // DEBUG: Print eh points prior to creating the volume
    for (G4int i = 0; i < poly_npts; ++i) {
    	MGLog(debugging) << " Point " << i << " zpos " << zpos[i] << " radius " << rout[i] << endlog;
    }




    fFiberSolid = new G4Polycone("FiberInstrSolid",
            			0.0, 2*M_PI,
            			poly_npts,
            			zpos,rin,rout);

    G4double density;
    std::vector<G4int> natoms;
    std::vector<G4double> fractionMass;
    std::vector<G4String> elements;

    G4NistManager *nistMan = G4NistManager::Instance();

    //--------------------------------------------------
    // Polystyrene
    //--------------------------------------------------

    elements.push_back("C");     natoms.push_back(8);
    elements.push_back("H");     natoms.push_back(8);

    density = 1.050*g/cm3;

    // -- Rough approximation of BCF-10
    fMaterialFiber = nistMan->
            ConstructNewMaterial("Polystyrene", elements, natoms, density);

    elements.clear();
    natoms.clear();


    fFiberLogical = new G4LogicalVolume(fFiberSolid,
                		fMaterialFiber,
                		"LArInstrFiberLogical");

    // Get the number of locks
    G4int nFlanges = fDetectorDB->GetNFlanges();

    for (G4int idet = 0; idet < nFlanges; ++idet) {
    	G4ThreeVector center = fDetectorDB->GetStringDetectorCenter(idet);
    	// The Z position is not correct
    	// The uppermost surface is given by the class
    	G4double up_Pos = fDetectorDB->GetZUppermostCrystalSurface();
    	// Search for how many crystals are around the place

    	G4double low_Pos = fDetectorDB->GetZLowermostCrystalSurface();

    	// The axis for the LAr instrumentation stay in the average Z value
    	center.setZ(0.5*(up_Pos+low_Pos));

    	// The bottom is the lower surface of the bottommost detector

    	fFiberPhysical = new G4PVPlacement(0,
										center,
										fFiberLogical,
										"LArInstrFiberPhysical",
										fMotherLogical,
										false,
										idet);

    }
    G4Colour lgreen  (0.0,  0.4, 0.0) ;
    fFiberVisAtt = new G4VisAttributes(lgreen);
    fFiberVisAtt -> SetVisibility(true);
    fFiberVisAtt -> SetForceSolid(false);
    fFiberLogical->SetVisAttributes(fFiberVisAtt);



	 // -- End of geometry definition.
	 // Let's start attaching optical properties to the materials
	 //
	 // For this specific design there is no need for optical properties, as we want to record the photons upon touching the volume

	 // Therefore, the only thing necessary is to create an optical surface covering the design in the same way as the photocathode in LArGe

  	  // Fibers "sensitive" surface
	 // Flat acceptance for all wavelengths - only two distant points (it interpolates in between)
	 static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
	 G4int num = 2;
	 G4double Wavelength[2] = {100.,650.};
	 G4double PhotonEnergy[2];
	 for (G4int i = 0 ; i < 2; ++i) {
		 PhotonEnergy[i] = LambdaE/(Wavelength[(2-1)-i]*nm);
		 /* Keep this commented but activate it if necessary to debug
		 	 MGLog(debugging) << "Lambda "
				 << (LambdaE/PhotonEnergy[i])/nm
		                  << " nm  Energy " << PhotonEnergy[i]/eV << " eV."<<endlog;
		  */
	 }
	 G4double Reflectivity[2] = {0.0, 0.0};
	 G4double Efficiency[2] = {1.0, 1.0}; // collection efficiency
	 //G4double RIndex[2] = {1.6, 1.6}; // collection efficiency
	 for (G4int ji=0; ji < num; ji++)
  	    {
  		 // Zero reflectivity and 1.0 efficiency means that the
  		 // photons are all "absorbed and identified as hits"
	      //RIndex[ji] = 1.6;
  	      Reflectivity[ji] = 0.0;
  	      Efficiency[ji] = 1.0; // Quantum efficiency of the PMT
  	    }
  	  G4MaterialPropertiesTable *fiberOptTable = new G4MaterialPropertiesTable();
  	  fiberOptTable->AddProperty("REFLECTIVITY",PhotonEnergy,Reflectivity,2);
  	  fiberOptTable->AddProperty("EFFICIENCY",PhotonEnergy,Efficiency,2);
  	  //fiberOptTable->AddProperty("RINDEX",PhotonEnergy,RIndex,2);
  	  G4OpticalSurface* pmtOptSurface
  	    = new G4OpticalSurface("PMT surface",glisur,polished,dielectric_metal);
  	  pmtOptSurface->SetMaterialPropertiesTable(fiberOptTable);


  	  //Associates the OpticalSurface to the actual volumes
  	  new G4LogicalSkinSurface("PMT_surf",fFiberLogical,pmtOptSurface);


	  fSDLogicVec.push_back(fFiberLogical);
	  fSDLogicNameVec.push_back("Fiber");
	  fSDLogicInstanceVec.push_back(1);
}

 G4LogicalVolume *GEGeometryLArInstFibers::GetLArInstrSDLogical(G4int index) {
	return fSDLogicVec.at(index);
}
G4int GEGeometryLArInstFibers::GetNumSDLogicVols() {
	return fSDLogicVec.size();
}
G4String GEGeometryLArInstFibers::GetLogicalSDName(G4int index) {
	return fSDLogicNameVec.at(index);
}
G4int GEGeometryLArInstFibers::GetNLogicalInstances(G4int index) {
	return fSDLogicInstanceVec.at(index);
}


