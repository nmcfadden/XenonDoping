/*
 * G4DemoDetector.hh
 *
 *  Created on: Sep 23, 2010
 *      Author: fraenkle
 */

#ifndef G4DEMODETECTOR_HH_
#define G4DEMODETECTOR_HH_

#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;


class G4DemoDetector : public G4VUserDetectorConstruction{
public:
	G4DemoDetector();
	~G4DemoDetector();

	G4VPhysicalVolume* Construct();
private:
	// logical volumes
	G4LogicalVolume* theworld_log;
	G4LogicalVolume* spd_log;

	// physical volume
	G4VPhysicalVolume* theworld_phys;
	G4VPhysicalVolume* spd_phys;
};

#endif /* G4DEMODETECTOR_HH_ */
