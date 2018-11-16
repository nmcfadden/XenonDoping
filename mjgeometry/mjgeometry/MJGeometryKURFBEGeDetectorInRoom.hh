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
 * CLASS DECLARATION:  MJGeometryKURFBEGeDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 * The MALBEK detector is placed in an air-filled space, suitable for
 * simulations when the detector is out of the shield.  The top of the cryostat
 * is at the origin; the crystal is in the -z direction.
 *
 */
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: A. Schubert
 * CONTACT: alexis3@u.washington.edu
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 04-07-2010 Initial submission, A. Schubert
 * 08-31-2011, Added collimator geometry, position is done with messenger 
 *             commands and this geometry is disabled by default, P. Finnerty
 */
// --------------------------------------------------------------------------//

#ifndef _MJGeometryKURFBEGeDetectorInRoom_HH
#define _MJGeometryKURFBEGeDetectorInRoom_HH

//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"
#include "mjgeometry/MJGeometryKURFBEGeDetector.hh"

//---------------------------------------------------------------------------//
class G4Material;
class G4LogicalVolume;
class G4VisAttributes;
class G4VPhysicalVolume;
class MJGeometryKURFBEGeDetectorInRoom : public MJGeometryKURFBEGeDetector
{
public:

  MJGeometryKURFBEGeDetectorInRoom(G4String serNum);
  ~MJGeometryKURFBEGeDetectorInRoom();

  void ConstructDetector();
  void SetDefaults();
  bool UsesMJPSQLDatabase() { return true; }  
 
private:

  G4double fWorldVolumeRadius;
  G4PVPlacement* fCryostatPhysical;
  G4PVPlacement* fCollimatorPhysical;
  G4PVPlacement* fSourceDiskPhysical;
  
};
#endif
