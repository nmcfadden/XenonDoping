/*
 * G4DemoDetector.cc
 *
 *  Created on: Sep 23, 2010
 *      Author: fraenkle
 */

#include "../include/G4DemoDetector.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"

#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4SDManager.hh"

G4DemoDetector::G4DemoDetector() {

}

G4DemoDetector::~G4DemoDetector() {

}

G4VPhysicalVolume* G4DemoDetector::Construct() {

	// define materials for detector
	G4NistManager* man = G4NistManager::Instance();
	G4Material* Si = man->FindOrBuildMaterial("G4_Si");
	G4Material* Vacuum = man->FindOrBuildMaterial("G4_Galactic");

	// define "the World"
	G4double theworld_x = 0.2 * m;
	G4double theworld_y = 0.1 * m;
	G4double theworld_z = 0.1 * m;

	G4Box* theworld_box = new G4Box("theworld_box", theworld_x, theworld_y, theworld_z);
	theworld_log = new G4LogicalVolume(theworld_box, Vacuum, "theworld_log,0,0,0");
	theworld_phys = new G4PVPlacement(0, G4ThreeVector(), theworld_log, "theworld", 0, false, 0);

	//PS 64PD (silicon wafer only)
	G4double SPDlength = 40.0 * mm;
	G4double SPDthick = 0.2 * mm;
	G4double spd_x = 0.0 * m;
	G4double spd_y = 0.0 * m;
	G4double spd_z = 0.0 * m;

	//create logical Volume (Volume + Material)
	G4Box* spd_box = new G4Box("spd_box", SPDthick / 2.0, SPDlength / 2.0, SPDlength/ 2.0);
	spd_log = new G4LogicalVolume(spd_box, Si, "spd_log");
	spd_phys = new G4PVPlacement(0, G4ThreeVector(spd_x, spd_y, spd_z), spd_log, "64PD", theworld_log, false, 0);

	//add sensitive volumes for energy deposit
	G4MultiFunctionalDetector* mfdet = new G4MultiFunctionalDetector("mfdet");
	G4VPrimitiveScorer* edep;
    edep = new G4PSEnergyDeposit("eDep");
    mfdet->RegisterPrimitive(edep);
    G4SDManager::GetSDMpointer()->AddNewDetector(mfdet);
    spd_log->SetSensitiveDetector(mfdet);

	return theworld_phys;
}
