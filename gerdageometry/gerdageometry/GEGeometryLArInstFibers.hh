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
 * CLASS DECLARATION:  GEGeometryLArInstFibers.hh
 *
 * AUTHOR: Nuno Barros
 *
 * CONTACT: nuno *dot* barros *at* tu-dresden *dot* de
 *
 ** DESCRIPTION:
 *
 *    Declaration of a test LAr geometry composed simply of a cylinder collecting the incident photons.
 *    The optical parameters do not represent any physical meaning for the moment.
 *
 * FIRST SUBMISSION: 02-11-2011
 *
 * REVISION: MM-DD-YYYY
 *
 *
 */

#ifndef GEGEOMETRYLARINSTFIBERS_HH_
#define GEGEOMETRYLARINSTFIBERS_HH_

#include "gerdageometry/GEGeometryLArDesignBase.hh"

class G4LogicalVolume;
class G4VSolid;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;

class GEGeometryLArInstFibers: public GEGeometryLArDesignBase {
public:
	 GEGeometryLArInstFibers(GEGeometryDetectorDB* theGeometryDB);
	virtual ~GEGeometryLArInstFibers();
	void ConstructDesign();
	virtual G4LogicalVolume *GetLArInstrSDLogical(G4int index = 0);
	virtual G4int GetNumSDLogicVols();
	virtual G4String GetLogicalSDName(G4int index = 0);
	virtual G4int GetNLogicalInstances(G4int index = 0);

protected:


private:
	G4VSolid*			fFiberSolid;
	G4LogicalVolume*	fFiberLogical;
	G4VPhysicalVolume*	fFiberPhysical;
	G4LogicalVolume*	fMotherLogical; // mother volume for the instrumentation
	GEGeometryDetectorDB* fDetectorDB;
	G4Material* 		fMaterialFiber;
	G4VisAttributes*	fFiberVisAtt;

	//G4bool fInitDone;
//	// Some more aux tables:
//	G4double frequencyV[npoints];
//	G4double efficiencyV[npoints];

};

#endif /* GEGEOMETRYLARINSTFIBERS_HH_ */
