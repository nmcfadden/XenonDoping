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
 * CLASS DECLARATION:  GEGeometryLArInstPMT.hh
 *
 * AUTHOR: Anne Wegmann
 *
 * CONTACT: anne *dot* wegmann *at* mpi-hd *dot* mpg *dot* de
 *
 ** DESCRIPTION:
 *
 *    Declaration of a PMT LAr geometry composed simply of a cylinder with PMTs on the top and bottom collecting the incident photons.
 *
 * FIRST SUBMISSION: 2012-05-08
 *
 * REVISION: MM-DD-YYYY
 *
 *
 */

#ifndef GEGEOMETRYLARINSTPMT_HH_
#define GEGEOMETRYLARINSTPMT_HH_

#include "gerdageometry/GEGeometryLArDesignBase.hh"

class G4LogicalVolume;
class G4VSolid;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;
class TGraph;
class G4OpticalSurface;

class GEGeometryLArInstPMT: public GEGeometryLArDesignBase {
public:
  GEGeometryLArInstPMT(GEGeometryDetectorDB* theGeometryDB);
  virtual ~GEGeometryLArInstPMT();
  void ConstructDesign();
  virtual G4LogicalVolume *GetLArInstrSDLogical(G4int index = 0);
  virtual G4int GetNumSDLogicVols();
  virtual G4String GetLogicalSDName(G4int index = 0);
  virtual G4int GetNLogicalInstances(G4int index = 0);

private:
  G4double VM2000EmissionSpectrum(G4double);
  void InitializeVM2000Spectrum();
  
  G4double 	WLSyield;
  TGraph 	*fVM2000spec;	// VM2000 re-emission spectrum
  G4double 	CalculateWLSmfp(G4double);
  
  static const 	G4int npoints = 250;
  G4double 	frequencyV[npoints];
  G4double 	efficiencyV[npoints];
  
  G4bool 	successfulInitialization;
  
  // Define the rough optical surface between WLS and LAr. Valid for all designs
  G4OpticalSurface *fWLSoptSurf;
  G4OpticalSurface *fTefOptSurf;
  G4VPhysicalVolume *fMotherPhysical;

	
};

#endif /* GEGEOMETRYLARINSTPMT_HH_ */
