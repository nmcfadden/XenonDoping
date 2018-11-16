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
 * CLASS DECLARATION:  GEGeometryLArDesignBase.hh
 *
 * AUTHOR: Nuno Barros
 *
 * CONTACT: nuno *dot* barros *at* tu-dresden *dot* de
 *
 ** DESCRIPTION:
 *
 *    Base class for any LAr Instrumentation design.
 *    All designs should derive from this class and implement the virtual methods.
 *
 * FIRST SUBMISSION: 02-11-2011
 *
 * REVISION: MM-DD-YYYY
 *
 * 08-25-2012, Nuno, Moved pointers to detectorDB and mother logical volume to abstract base class. These are needed for all instrumentation designs.
 */


#ifndef GEGEOMETRYLARDESIGNBASE_HH_
#define GEGEOMETRYLARDESIGNBASE_HH_

/** @class GEGeometryLArDesignBase
 *  @brief Abstract class defining the API for the concrete implementations of the instrumentation class.
 *
 *  This class serves as a prototype for LAr Instrumentation.
 *  In particular it is responsible for the following tasks:
 *  	- Call the apropriate LAr instrumentation design (as per user choice).
 *  	- Set LAr as a sensitive detector and define its optical properties.
 *  	- Activate the physics processes for optical photons.
 *
 *  @author N. Barros, TU-Dresden
 *  @date November 2011
 *
 */
class G4LogicalVolume;
class GEGeometryDetectorDB;

#include "globals.hh"
#include <vector>

class GEGeometryLArDesignBase {
public:
	GEGeometryLArDesignBase() { };
	virtual ~GEGeometryLArDesignBase() { };
	virtual void ConstructDesign() = 0;
	/// Pure virtual methods. Have to be implemented in the
	/// concrete class. The objective of each class is described below
	/// NOTE: The arrays have to be defined in the concrete implementation.
	/** Returns a logical volume of the sensitive detector
	 * @param index defines the position in the array of logical volumes for which we want sensitive detectors.
	 * 		  Defaults to zero to simplify calls in geometries with a single sensitive detector (eg. Single PMT).
	 * @return pointer to the logical volume stored in array of sensitive detectors at position index.
	 */
	virtual G4LogicalVolume *GetLArInstrSDLogical(G4int index = 0) = 0;
	/** Gets the number of logical volumes for which sensitive detectors should be built
	 * @return number of entries in array
	 */
	virtual G4int GetNumSDLogicVols() = 0;
	/** Gets the name for requested entry. No consistency check is performed.
	 * @return name of the entry
	 */
	virtual G4String GetLogicalSDName(G4int index = 0) = 0;

	/** Gets the number of intances of the requested volume were physically placed (eg. number of PMTs of same type).
	 * @return number of instances
	 */
	virtual G4int GetNLogicalInstances(G4int index = 0) = 0;

protected:
	std::vector<G4LogicalVolume*> fSDLogicVec;
	std::vector<G4String> fSDLogicNameVec;
	std::vector<G4int> fSDLogicInstanceVec;

	// All designs will need these two variables.

	GEGeometryDetectorDB*		fDetectorDB;
	G4LogicalVolume*			fMotherLogical; // mother volume for the instrumentation


};

#endif /* GEGEOMETRYLARDESIGNBASE_HH_ */
