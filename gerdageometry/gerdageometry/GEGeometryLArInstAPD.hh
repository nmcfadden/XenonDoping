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
 * CLASS DECLARATION:  GEGeometryLArInstAPD.hh
 *
 * AUTHOR: Bjoern Lehnert
 *
 * CONTACT: bjoern *dot* lehnert *at* tu-dresden *dot* de
 *
 * DESCRIPTION:
 *
 *    Declaration of a APD LAr geometry
 *
 * FIRST SUBMISSION: 2012-25-11, blehnert
 *
 * REVISION: MM-DD-YYYY
 *
 *
 */

#ifndef GEGEOMETRYLARINSTAPD_HH_
#define GEGEOMETRYLARINSTAPD_HH_

#include "gerdageometry/GEGeometryLArDesignBase.hh"
#include "GEGeometryDetectorDB.hh"

class G4LogicalVolume;
class G4VSolid;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;
class TGraph;
class G4OpticalSurface;

class GEGeometryLArInstAPD: public GEGeometryLArDesignBase {
public:
  GEGeometryLArInstAPD(GEGeometryDetectorDB* theGeometryDB);
  virtual ~GEGeometryLArInstAPD();
  void ConstructDesign();
  virtual G4LogicalVolume *GetLArInstrSDLogical(G4int index = 0);
  virtual G4int GetNumSDLogicVols();
  virtual G4String GetLogicalSDName(G4int index = 0);
  virtual G4int GetNLogicalInstances(G4int index = 0);

private:

  std::vector<G4ThreeVector>* fMiniShroudPosition;
  std::vector<G4int>* fMiniShroudColumn;
  std::vector<G4int>* fMiniShroudRow;

  G4double fMiniShroudHeight;
  G4double fMiniShroudOuterRadius;

  
  G4VPhysicalVolume *fMotherPhysical;

	
};

#endif /* GEGEOMETRYLARINSTAPD_HH_ */
