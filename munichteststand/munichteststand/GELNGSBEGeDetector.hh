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
 * $Id: GELNGSBEGeDetector.hh,v 1.3 2009-05-26 08:58:38 pandola Exp $
 *      
 * CLASS DECLARATION:  GELNGSBEGeDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 * Geometry class for the BEGe detectors at LNGS 
 *
 * REVISION:
 *
 * 14-05-2009, Added in MaGe, Luciano
 * 26-05-2009, Added messenger, Luciano
 * 15-09-2009, Added new coincident coaxial detector (controlled by a flag), 
 *             Luciano
 * 28-04-2010, Main revision to accomodate depGe BEGes. Now the basic 
 *             construction via MGGeometryBEGeDetector is used, Luciano
 *
 */
// --------------------------------------------------------------------------//

#ifndef _GELNGSBEGEDETECTORDETECTOR_HH
#define _GELNGSBEGEDETECTORDETECTOR_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class G4Material;
class GELNGSBEGeDetectorMessenger;

class GELNGSBEGeDetector : public MGGeometryDetector
{

private:
  

public:

  //default constructor
  GELNGSBEGeDetector();

  //destructor
  ~GELNGSBEGeDetector();

  //public interface
  void ConstructDetector();
  void SetTopLateralDeadLayer(G4double val){ top_lateral_deadlayer = val;};
  G4double GetTopLateralDeadLayer(){return top_lateral_deadlayer;};
  //
  void SetBottomDeadLayer(G4double val){bottom_deadlayer = val;};
  G4double GetBottomDeadLayer(){return bottom_deadlayer;};
  //
  void SetUseCoincidentDetector(G4bool val){fUseCoincidentDetector = val;};
  G4bool GetUseCoincidentDetector(){return fUseCoincidentDetector;};
  //
  void SetDetectorType(G4String type){fDetectorType = type;};
  G4String GetDetectorType(){return fDetectorType;};

private:
  GELNGSBEGeDetectorMessenger* fMessenger;

  G4double top_lateral_deadlayer;
  G4double bottom_deadlayer;
  G4bool fUseCoincidentDetector;

  G4String fDetectorType;

};
#endif
