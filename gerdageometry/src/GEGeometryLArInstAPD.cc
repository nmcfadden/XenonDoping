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
// CLASS IMPLEMENTATION:  GEGeometryLArInstPMT.cc
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
 * AUTHOR: Bjoern Lehnert
 * CONTACT: bjoern *dot* lehnert *at* tu-dresden *dot* de
 * FIRST SUBMISSION: 2012-25-11, blehnert
 *
 * REVISION:
 * 05-29-2012
 */
//---------------------------------------------------------------------------//
#include "gerdageometry/GEGeometryLArInstAPD.hh"
#include "gerdageometry/GEGeometryCryostatParts.hh"
#include "io/MGLogger.hh"

#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"


// -- ROOT include
#include "TGraph.h"

// -- C++ include
#include <vector>

using namespace CLHEP;


GEGeometryLArInstAPD::GEGeometryLArInstAPD(GEGeometryDetectorDB* theGeometryDB) :
		fMotherPhysical(0) {
	fDetectorDB = theGeometryDB;
	fMotherLogical = fDetectorDB->GetDetectorMotherVolume();
	// For the rough surfaces we also need the physical volume of the mother
	fMotherPhysical =
			fDetectorDB->GetCryostatParts()->GetCryoInnerwall_Inside()->GetPhysicalVolume();
	MGLog(warning)<< "Logical and Physical volumes imported. Describing:" << endlog;
	MGLog(warning)<< "LOG : " << fMotherLogical->GetName() << " MAT " << fMotherLogical->GetMaterial()->GetName() << endlog;
	MGLog(warning)<< "PHYS : " << fMotherPhysical->GetName() << " LOG " << fMotherPhysical->GetLogicalVolume()->GetName() << endlog;

	// mini shroud initialization
	fMiniShroudColumn = fDetectorDB->GetMiniShroudColumn();
	fMiniShroudRow = fDetectorDB->GetMiniShroudRow();
	fMiniShroudPosition = fDetectorDB->GetMiniShroudPosition();

	fMiniShroudHeight = fDetectorDB->GetMiniShroudHeight();
	fMiniShroudOuterRadius = fDetectorDB->GetMiniShroudOuterRadius();

	return;
}

GEGeometryLArInstAPD::~GEGeometryLArInstAPD() {
	// delete all instances stored in the arrays
	for (std::vector<G4LogicalVolume*>::iterator it = fSDLogicVec.begin();
			it != fSDLogicVec.end(); ++it) {
		delete *it;
	}
}

void GEGeometryLArInstAPD::ConstructDesign() {
	// TODO APD construct only of MS flag is set

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Material and visualization definitions

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Color definitions
	G4Colour fVis_red(255 / 255., 1 / 255., 1 / 255.);  //red
	G4Colour fVis_greenG4Colour(1 / 255., 255 / 255., 33 / 255.);  //green !
	G4Colour fVis_lighGray(1 / 255., 1 / 255., 255 / 255.);   //light gray
	G4Colour fVis_blue(226 / 255., 163 / 255., 29 / 255.);  // blue
	G4Colour fVis_moje(210 / 255., 210 / 255., 210 / 255.); //light blue-grey , moje
	G4Colour fVis_blueGreen(154 / 255., 237 / 255., 193 / 255.);  //blue-green
	G4Colour fVis_gray(171 / 255., 171 / 255., 195 / 255.);  //gray
	G4Colour fVis_darkGray(187 / 255., 28 / 255., 0 / 255.); //almost black ( -> 'dead'...)
	G4Colour fVis_lightBlue(150 / 255., 150 / 255., 255 / 255.);  //light blue
	G4Colour fVis_orangeBrown(226 / 255., 163 / 255., 29 / 255.); //orange-brown
	G4Colour fVis_orange(209 / 255., 134 / 255., 0 / 255.);  //orange

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Definition of optical properties
	static const G4double LambdaE = twopi * 1.973269602e-16 * m * GeV;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Optical properties of the SiPM:
	// TODO: change to proper values of SiPM
	const G4int num_entries_SiPM = 11;
	G4double SiPMEnergy[num_entries_SiPM] = {
			LambdaE / (670.* nanometer),
			LambdaE / (595.* nanometer),
			LambdaE / (525.* nanometer),
			LambdaE / (505.* nanometer),
			LambdaE / (435.* nanometer),
			LambdaE / (400.* nanometer),
			LambdaE / (350.* nanometer),
			LambdaE / (310.* nanometer),
			LambdaE / (280.* nanometer),
			LambdaE / (180.* nanometer),
			LambdaE / (100.* nanometer)
	};

	G4double SiPMReflectivity[num_entries_SiPM] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; // no reflectivity

	//TODO APD change to efficiency of APDs
	G4double SiPMEfficiency[num_entries_SiPM] = {0.07,0.12,0.19,0.27,0.32,0.33,0.32,0.30,0.19,0.0,0.0};
	G4MaterialPropertiesTable *SiPMOptTable =
			new G4MaterialPropertiesTable();
	SiPMOptTable->AddProperty("REFLECTIVITY", SiPMEnergy,
			SiPMReflectivity, num_entries_SiPM);
	SiPMOptTable->AddProperty("EFFICIENCY", SiPMEnergy,
			SiPMEfficiency, num_entries_SiPM);

	G4OpticalSurface* SiPMOptSurface = new G4OpticalSurface(
			"LArInstrSiPMSurface", glisur, polished, dielectric_metal);
	SiPMOptSurface->SetMaterialPropertiesTable(SiPMOptTable);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Optical properties of the PMT glass:
	// TODO: change to proper values of APD glass
	const G4int num_entries_PMT = 24;
	G4double PMTGlassEnergy[num_entries_PMT] = { LambdaE / (115 * nanometer),
			LambdaE / (160.8 * nanometer), LambdaE / (170 * nanometer), LambdaE
					/ (182.3 * nanometer), LambdaE / (200 * nanometer), LambdaE
					/ (230 * nanometer), LambdaE / (250 * nanometer), LambdaE
					/ (270 * nanometer), LambdaE / (300 * nanometer), LambdaE
					/ (320 * nanometer), LambdaE / (340 * nanometer), LambdaE
					/ (360 * nanometer), LambdaE / (380 * nanometer), LambdaE
					/ (400 * nanometer), LambdaE / (450 * nanometer), LambdaE
					/ (500 * nanometer), LambdaE / (550 * nanometer), LambdaE
					/ (580 * nanometer), LambdaE / (600 * nanometer), LambdaE
					/ (630 * nanometer), LambdaE / (660 * nanometer), LambdaE
					/ (700 * nanometer), LambdaE / (730 * nanometer), LambdaE
					/ (750 * nanometer) };
	G4double PMTGlassReflectivity[num_entries_PMT] =
			{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	//TODO APD change to efficiency of APDs
	G4double PMTGlassEfficiency[num_entries_PMT] = { 0.0000, 0.0034, 0.0322,
			0.0741, 0.1297, 0.1450, 0.1673, 0.1965, 0.2348, 0.2473, 0.2467,
			0.2399, 0.2368, 0.2264, 0.1847, 0.1309, 0.0692, 0.0371, 0.0231,
			0.0104, 0.0036, 0.0006, 0.0001, 0.0000 };

	G4MaterialPropertiesTable *PMTGlassOptTable =
			new G4MaterialPropertiesTable();
	PMTGlassOptTable->AddProperty("REFLECTIVITY", PMTGlassEnergy,
			PMTGlassReflectivity, num_entries_PMT);
	PMTGlassOptTable->AddProperty("EFFICIENCY", PMTGlassEnergy,
			PMTGlassEfficiency, num_entries_PMT);

	G4OpticalSurface* PMTGlassOptSurface = new G4OpticalSurface(
			"LArInstrPMTGlassSurface", glisur, polished, dielectric_metal);
	PMTGlassOptSurface->SetMaterialPropertiesTable(PMTGlassOptTable);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Optical properties of window-less APD (silicon)
	// TODO: get proper values. so far for testing efficiency 1 is assumed
	const G4int num_entries_VUV = 11;
	G4double APD_VUV_Energy[num_entries_VUV] = { LambdaE / (120. * nanometer),
			LambdaE / (132. * nanometer), LambdaE / (140. * nanometer), LambdaE
					/ (145. * nanometer), LambdaE / (155. * nanometer), LambdaE
					/ (120. * nanometer), LambdaE / (170. * nanometer), LambdaE
					/ (200. * nanometer), LambdaE / (220. * nanometer), LambdaE
					/ (235 * nanometer), LambdaE / (255 * nanometer)};
	G4double APD_VUV_Reflectivity[num_entries_VUV] =
			{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	//TODO APD change to efficiency of APDs
	G4double APD_VUV_Efficiency[num_entries_VUV] = { 1., 1., 1., 1., 1., 1., 1.,
			1., 1., 1., 1.};

	G4MaterialPropertiesTable *APD_VUV_OptTable =
			new G4MaterialPropertiesTable();
	APD_VUV_OptTable->AddProperty("REFLECTIVITY", APD_VUV_Energy,
			APD_VUV_Reflectivity, num_entries_VUV);
	APD_VUV_OptTable->AddProperty("EFFICIENCY", APD_VUV_Energy,
			APD_VUV_Efficiency, num_entries_VUV);

	G4OpticalSurface* APD_VUV_OptSurface = new G4OpticalSurface(
			"LArInstrAPDVUVSurface", glisur, polished, dielectric_metal);
	APD_VUV_OptSurface->SetMaterialPropertiesTable(APD_VUV_OptTable);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TODO: Find out proper materials
	// Defining materials

	//G4Material* fMaterial_Al = G4Material::GetMaterial("MetalAluminium");
	//G4VisAttributes * fVis_Al = new G4VisAttributes(fVis_blue);

	G4Material* fMaterial_Si = G4Material::GetMaterial("MetalSilicon");
	G4VisAttributes * fVis_Si = new G4VisAttributes(fVis_blueGreen);

	G4Material* fMaterial_PMTGlass = G4Material::GetMaterial("Quartz");
	//G4VisAttributes * fVis_PMTGlass = new G4VisAttributes(fVis_gray);

	G4Material* fMaterial_SiPM = G4Material::GetMaterial("Quartz");
	//G4VisAttributes * fVis_SiPM = new G4VisAttributes(fVis_gray);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set APD design (dimensions and optical properties)

	G4double APDFrame_lenght = 0.;
	G4double APDFrame_height = 0.;
	G4double APDFrame_depth = 0.;
	G4double APDOptSurf_thickness = 0.01*cm; // set default to remove compiler warning --J. Detwiler 2015 Dec 17

	//material of the optical surface
	G4Material* fMaterial_Opt;  // to be linked later according to designv
	G4VisAttributes * fVis_Opt = new G4VisAttributes(fVis_red);

	G4OpticalSurface * OptSurface = NULL; // to be linked later according to design

	switch (fDetectorDB->GetLArInstrumentationAPDDesign()) {
	case LArInstAPDDesign_VUV:
		MGLog(trace)<< "Building VUV APD design." << endlog;
		APDFrame_lenght = 1.*cm;
		APDFrame_height = 1.*cm;
		APDFrame_depth = 0.3*cm;
		APDOptSurf_thickness = 0.01*cm;
		fMaterial_Opt = fMaterial_Si;
		fMaterial_Opt->SetMaterialPropertiesTable(APD_VUV_OptTable);
		OptSurface = APD_VUV_OptSurface;

		break;
		case LArInstAPDDesign_nonVUV:
		MGLog(trace) << "Building nonVUV APD design." << endlog;
		APDFrame_lenght = 1.*cm;
		APDFrame_height = 1.*cm;
		APDFrame_depth = 0.2*cm;
		APDOptSurf_thickness = 0.01*cm;
		fMaterial_Opt = fMaterial_PMTGlass;
		fMaterial_Opt->SetMaterialPropertiesTable(PMTGlassOptTable);
		OptSurface = PMTGlassOptSurface;

		break;
		case LArInstAPDDesign_SiPM:
		MGLog(trace) << "Building SiPM design." << endlog;
		APDFrame_lenght = 1.3*cm;
		APDFrame_height = 1.3*cm;
		APDFrame_depth = 0.2*cm;
		APDOptSurf_thickness = 0.01*cm;
		fMaterial_Opt = fMaterial_SiPM;
		fMaterial_Opt->SetMaterialPropertiesTable(SiPMOptTable);
		OptSurface = SiPMOptSurface;

		break;
		default:
		MGLog(error) << "Failure building APD design ( " << fDetectorDB->GetLArInstrumentationAPDDesign()
		<< " )" << endlog;
		break;
	}

	MGLog(debugging)<< Form("APD length: %2.1f mm",APDFrame_lenght/mm) << endlog;
	MGLog(debugging)<< Form("APD height: %2.1f mm",APDFrame_height/mm) << endlog;
	MGLog(debugging)<< Form("APD depth: %2.1f mm",APDFrame_depth/mm) << endlog;
	MGLog(debugging)<< Form("Optical surface thickness: %2.1f mm",APDOptSurf_thickness/mm) << endlog;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// volume construction chain

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// construct one volume for all APD instances
	G4Box * APDFrame;
	APDFrame = new G4Box("APDFrame", APDFrame_lenght / 2., APDFrame_height / 2.,
			APDFrame_depth / 2.);

	G4Box * APDOptSurf;
	APDOptSurf = new G4Box("APDOptSurf", APDFrame_lenght / 2.,
			APDFrame_height / 2., APDOptSurf_thickness / 2.);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// determine number of MS, APDs per minishroud and total number of APDs
	G4int numMS = fMiniShroudRow->size(); // number of MS
	G4int numAPD_BEGe = 5; // number of APDs  per BEGe string MS
	//G4int numAPD_Coax = 4; // number of APDs  per Coax string MS
	G4int totNumAPD = numMS * numAPD_BEGe; // total number of APDs

	MGLog(trace)<< Form("Building %i APDs per MS in %i MSs = %i APD instances.",numAPD_BEGe, numMS, totNumAPD) << endlog;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// construct vector of logical volumes for each APD instance and push into vector
	std::vector<G4LogicalVolume*> APDFrameLogical;
	std::vector<G4VPhysicalVolume*> APDFramePhysical;

	std::vector<G4LogicalVolume*> APDOptSurfLogical;
	std::vector<G4VPhysicalVolume*> APDOptSurfPhysical;

	for (G4int i_totAPD = 0; i_totAPD < totNumAPD; i_totAPD++)
	// loop over all APDs has the equivalent sequence as looping first over MS, then over APDs in MS
	{
		// APD frame
		G4LogicalVolume* tempVolumeLogical = new G4LogicalVolume(APDFrame,
				fMaterial_Si, Form("APDFrameLogical_%i", i_totAPD));
		tempVolumeLogical->SetVisAttributes(fVis_Si);
		APDFrameLogical.push_back(tempVolumeLogical);

		// APD optical surface
		G4LogicalVolume* tempVolumeLogical2 = new G4LogicalVolume(APDOptSurf,
				fMaterial_Si, Form("APDOptSurf_Logical_%i", i_totAPD));
		tempVolumeLogical2->SetVisAttributes(fVis_Opt);
		APDOptSurfLogical.push_back(tempVolumeLogical2);

	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//calculate APD positions: looping over all MS and APDs per MS
	std::vector<G4ThreeVector*> APDFramePosition;
	std::vector<G4RotationMatrix*> APDFrameRotation;

	G4int aCounter = 0;
	for (G4int i_MS = 0; i_MS < numMS; i_MS++) {

		// getting MS position from detDB
		G4ThreeVector* MSVector = new G4ThreeVector(
				fMiniShroudPosition->at(i_MS));

		// determine kind of string and set number of APDs per string
		G4int numAPD_MS = 0;
		G4int MSColumn = fDetectorDB->GetMiniShroudColumn()->at(i_MS);
		G4int crystalType = fDetectorDB->GetCrystalType(i_MS, 0);
		G4int crystalGeometry = fDetectorDB->GetDetectorParameters(crystalType)->GetGeometry();
		MGLog(debugging)<< Form("MS %i @ column %i: First detector with geometry %i",i_MS,MSColumn,crystalGeometry ) << endlog;

		std::vector<G4double> APDPosition;

		if(crystalGeometry==3){
			numAPD_MS=5;
			G4double gapBetweenAPD = fDetectorDB->GetMiniShroudHeight()/(numAPD_MS-1);
			APDPosition.push_back(fDetectorDB->GetMiniShroudPosition()->at(i_MS).getZ() - fDetectorDB->GetMiniShroudHeight()/2 + 1.5*cm);
			APDPosition.push_back(APDPosition.at(0) + gapBetweenAPD - 1.5*cm);
			APDPosition.push_back(APDPosition.at(1) + gapBetweenAPD);
			APDPosition.push_back(APDPosition.at(2) + gapBetweenAPD);
			APDPosition.push_back(APDPosition.at(3) + gapBetweenAPD - 1.5*cm);
		}
		else if(crystalGeometry==1){
			numAPD_MS=4;
			G4double gapBetweenAPD = fDetectorDB->GetMiniShroudHeight()/(numAPD_MS-1);
			APDPosition.push_back(fDetectorDB->GetMiniShroudPosition()->at(i_MS).getZ() - fDetectorDB->GetMiniShroudHeight()/2 + 2*cm);
			APDPosition.push_back(APDPosition.at(0) + gapBetweenAPD - 2*cm);
			APDPosition.push_back(APDPosition.at(1) + gapBetweenAPD);
			APDPosition.push_back(APDPosition.at(2) + gapBetweenAPD - 2*cm);
		}
		else {MGLog(error)<<Form("Unsupported xtal geometry in string %i: Geometry %i",i_MS, crystalGeometry)<<endlog;}

		// loop over all APDs
		for (G4int i_APD = 0; i_APD < numAPD_MS; i_APD++) {

			G4int i_totAPD = aCounter;

			G4ThreeVector* APDVector = new G4ThreeVector(0, 0, 0);
			G4double phi = i_APD * (360 / numAPD_MS) * deg;
			APDVector->setX(
					MSVector->getX()
							+ cos(phi) * (fMiniShroudOuterRadius - 5. * mm));

			APDVector->setY(
					MSVector->getY()
							+ sin(phi) * (fMiniShroudOuterRadius - 5. * mm));

			if(i_APD==0)
				APDVector->setZ(
						APDPosition.at(i_APD));
			else if(i_APD==numAPD_MS-1)
				APDVector->setZ(APDPosition.at(i_APD));
			else
				APDVector->setZ(APDPosition.at(i_APD));

			APDFramePosition.push_back(APDVector);

			// calculating rotation from MS and APD positions
			G4RotationMatrix* APDRotation = new G4RotationMatrix();
			MGLog(debugging)<< Form("phi: %2.2f,  theta: %2.2f ",APDRotation->getPhi(), APDRotation->getTheta()) << endlog;
			APDRotation->rotateZ(-1 * phi);
			MGLog(debugging)<< Form("phi: %2.2f,  theta: %2.2f ",APDRotation->getPhi(), APDRotation->getTheta()) << endlog;
			APDRotation->rotateY(90 * deg);
			MGLog(debugging)<< Form("phi: %2.2f,  theta: %2.2f ",APDRotation->getPhi(), APDRotation->getTheta()) << endlog;

			APDFrameRotation.push_back(APDRotation);

			// APD frame
			G4VPhysicalVolume * tempPhysical = new G4PVPlacement(
					APDFrameRotation.at(i_totAPD),
					*APDFramePosition.at(i_totAPD),
					APDFrameLogical.at(i_totAPD),
					Form("APDFramePhysical_%i", i_totAPD), fMotherLogical,
					false, 0);

			APDFramePhysical.push_back(tempPhysical);

			// APD optical surface
			G4VPhysicalVolume * tempPhysical2 = new G4PVPlacement(0,
					G4ThreeVector(0, 0,
							APDFrame_depth / 2. - APDOptSurf_thickness / 2.),
					APDOptSurfLogical.at(i_totAPD),
					Form("APDOptSurfPhysical_%i", i_totAPD),
					APDFrameLogical.at(i_totAPD), false, 0);

			APDOptSurfPhysical.push_back(tempPhysical2);

			MGLog(debugging)<< Form("i: %i,  phi: %2.1f ",i_totAPD, phi/deg) << endlog;
			MGLog(debugging)<< Form("x=%2.1f, y=%2.1f, z=%2.1f, ",APDVector->getX()/mm, APDVector->getY()/mm, APDVector->getZ()/mm) << endlog;

			// optical surface
			new G4LogicalBorderSurface(Form("APDSurface_%i", i_totAPD),
					fMotherPhysical, APDOptSurfPhysical.at(i_totAPD),
					OptSurface);

			// registering sensitive detector volumes (optical surface volumes)
			fSDLogicVec.push_back(APDOptSurfLogical.at(i_totAPD));
			fSDLogicNameVec.push_back(Form("APD_MS%02d_APD%02d", i_MS, i_APD));
			fSDLogicInstanceVec.push_back(1);

			MGLog(debugging)<< Form("Sensitive Detector registered as \"%s\" ",Form("APD_MS%02d_APD%02d",i_MS,i_APD)) << endlog;

			aCounter++; // increasing total counter of double loop
		}

	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// linking optical surface
	for (G4int i_totAPD = 0; i_totAPD < totNumAPD; i_totAPD++) {
		new G4LogicalSkinSurface("OptSurface", APDOptSurfLogical.at(i_totAPD),
				OptSurface);
	}

}

G4LogicalVolume *GEGeometryLArInstAPD::GetLArInstrSDLogical(G4int index) {
	return fSDLogicVec.at(index);
}
G4int GEGeometryLArInstAPD::GetNumSDLogicVols() {
	return fSDLogicVec.size();
}
G4String GEGeometryLArInstAPD::GetLogicalSDName(G4int index) {
	return fSDLogicNameVec.at(index);
}
G4int GEGeometryLArInstAPD::GetNLogicalInstances(G4int index) {
	return fSDLogicInstanceVec.at(index);
}

