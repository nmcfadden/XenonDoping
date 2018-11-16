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
 *      
 * CLASS DECLARATION:  MJGeometryCERN_NA55.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geometry class for CERN NA55 muon beam dump experiment. See
 * [1] V. Chazal et al., Nucl. Inst. Meth. Phys. Res. A 490, p 334-343 (2002).
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: M. Marino
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 * 
 * 02-2006: Created, M. Marino
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYCERN_NA55_HH
#define _MJGEOMETRYCERN_NA55_HH

#include "geometry/MGGeometryDetector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include <vector>
using namespace std;

class G4Box;
class G4Tubs;
class G4Sphere;
class MJGeometryCERN_NA55Messenger;
class G4VSolid;


enum ECERN_NA55BeamDumpType { kGraphite, kCopper, kLead};
// enum constants for specifying beam dump type.

class MJGeometryCERN_NA55 : public MGGeometryDetector
{
  public:

  MJGeometryCERN_NA55();
  ~MJGeometryCERN_NA55();

  // builds the detector geometry
  void ConstructDetector();
    
  // Sets the beam dump type according to the options: kGraphite, kCopper, kLead.
  void SetBeamDumpType(ECERN_NA55BeamDumpType aBeamDumpType) 
      { fBeamDumpType = aBeamDumpType; }
    
    // returns the length of the beam dump.
  G4double GetBeamDumpHalfZLength() {return fTheBeamDumpHalfZLength;}
    
  // returns material density of beam dump in atoms/volume;
  G4double GetMaterialDensity();  
  bool UsesMJPSQLDatabase() { return false; }  
    
  private:
   
    G4Box* fWorldBox;
    
    G4Sphere* fExpSphere;
    G4LogicalVolume* fExpLog;

    G4Tubs* fTheBDTube;
    G4LogicalVolume* fTheBDLog;
    G4VPhysicalVolume* fTheBDPhys;

    vector<G4VPhysicalVolume*> fPhysicalVolumeVec;
    vector<G4LogicalVolume*> fLogicalVolumeVec;
    vector<G4VSolid*> fSolidVec;
    ECERN_NA55BeamDumpType fBeamDumpType;

    MJGeometryCERN_NA55Messenger* fTheMessenger;
    G4double fTheBeamDumpHalfZLength;
};

#endif

