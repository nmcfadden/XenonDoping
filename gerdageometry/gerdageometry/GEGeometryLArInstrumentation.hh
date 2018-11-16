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
 * CLASS DECLARATION:  GEGeometryLArInstrumentation.hh
 *
 * DESCRIPTION:
 *
 *    A class to act as a factory for the LAr Instrumentation.
 *    Implements the optical properties of common elements to the rest of the detector
 *
 * AUTHOR:  Nuno Barros
 * CONTACT: nuno *dot* barros *at* tu-dresden *dot* de
 *
 * REVISION: MM-DD-YYYY
 *
  */

#ifndef _GEGEOMETRYLARINSTRUMENTATION_HH_
#define _GEGEOMETRYLARINSTRUMENTATION_HH_

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "TGraph.h"

//#include "GEGeometryDetectorDB.hh"

// ---------------------------------------------------------------------------
/** Some forward declarations **/

class GEGeometryDetectorDB;
class GEGeometryLArDesignBase;
class G4OpticalSurface;
// ---------------------------------------------------------------------------
/** @class GEGeometryLArInstrumentation
 *  @brief Implements basic infrastructure for the LAr Instrumentation.
 *
 *  This class serves as a LAr Instrumentation factory.
 *  In particular it is responsible for the following tasks:
 *  	- Call the apropriate LAr instrumentation design (as per user choice).
 *  	- Set LAr as a sensitive detector and define its optical properties.
 *  	- Activate the physics processes for optical photons.
 *
 *  @author N. Barros, TU-Dresden
 *  @date November 2011
 *
 */
// Base factory
class GEGeometryLArInstrumentation {
public:
	/** Default constructor
	 *
	 * @param theGeometryDB - detector options as set by macro file.
	 *
	 * @todo Implement cross-checks for consistency between the LAr
	 * instrumentation and the detector design (such as proper instrumentation
	 * placement with respect to crystal position).
	 */
	GEGeometryLArInstrumentation(GEGeometryDetectorDB* theGeometryDB);
	/// Destructor
	virtual ~GEGeometryLArInstrumentation();
	/// Geometry constructor
	void ConstructLArInstrumentation();

	void ConstructMiniShroud();

  /// Auxiliary method to simplify attaching optical surfaces on many detector parts
  void AttachOpticalSurface(G4LogicalVolume *log_vol);

private:
	/// Disallow constructor with empty messenger
	GEGeometryLArInstrumentation();


	///! Auxiliary functions imported from MPIKLarGe setup.
	/// Register LAr optical properties
	void RegisterArgonOpticalProperties();
	static const G4int npoints = 250;
	G4double frequencyV[npoints];
	G4double efficiencyV[npoints];
	G4bool successfulInitialization;

	//G4double WLSyield; // re-declared in .cc and not used elsewhere;
        //comment out here to avoid compiler warning. --J. Detwiler 2015 Dec 17

	G4double LArRefIndex(const G4double);
	G4double LArEpsilon(const G4double);
	G4double LArRayLength(const G4double,const G4double temp);
	G4double ArScintillationSpectrum(const G4double);
	G4double CalculateWLSmfp(G4double);

	static const G4double LambdaE;

	G4double TPBEmissionSpectrum(G4double);
	void InitializeTPBSpectra();
	G4Material *fTPB;
	G4Material *fTPBOnNylon;

	TGraph  *fTPBspec; // VM2000 re-emission spectrum

	/// Instantiation of the instrumentation class
	/// The base class is used.
	std::vector<GEGeometryLArDesignBase*>	fInstrumentation;

	/// Geometry DB options passed through shell
	GEGeometryDetectorDB*		fDetectorDB;
	/// mother volume for the instrumentation
	G4LogicalVolume*			fMotherLogical;
	G4VPhysicalVolume*			fMotherPhysical;

	// Common optical surfaces for several detector parts. They are effectively attached to the materials
	// rather than volumes, but geant4 optics are messy and do not allow to attach reflectivity to a material
	// without an index of refraction
	G4OpticalSurface* fCuOptSurface;
	G4OpticalSurface* fSiOptSurface;
	G4OpticalSurface* fTefOptSurface;
	G4OpticalSurface* fVM2000OptSurface;
	G4OpticalSurface* fWLSoptSurf;
	G4OpticalSurface* fNylonOptSurf;


};

#endif /* GEGEOMETRYLARINSTRUMENTATION_HH_ */
