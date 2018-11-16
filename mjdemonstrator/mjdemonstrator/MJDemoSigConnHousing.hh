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
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION:  MJDemoSigConnPin.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geometry code for signal connector vespel housing
 * Male and female housing are not differentiated because connectors are
 * far from detectors and shielded so that fine details are not critical
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
 * AUTHOR: Ian Guinn
 * CONTACT: iguinn@uw.edu
 * FIRST SUBMISSION: Jun 4, 2014
 * 
 * REVISION:
 * 
 * 06-04-2014, Created, I. Guinn
 * 08-20-2014, Added flat and pin holes, I. Guinn
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOSIGCONNHOUSING_HH
#define _MJDEMOSIGCONNHOUSING_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJDemoSigConnPins.hh"
#include "mjdemonstrator/MJDemoSigConnSolder.hh"
#include "mjdemonstrator/MJVDemoPart.hh"
#include "geometry/MGGeometryGlobals.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoSigConnHousing: public MJVDemoPart
{
  
public:
  MJDemoSigConnHousing(G4String, G4String);
  MJDemoSigConnHousing(const MJDemoSigConnHousing &);
  ~MJDemoSigConnHousing();

  G4LogicalVolume* ConstructPart();
  static inline size_t GetNPlanes() {return kNPlanes;}
  static inline const G4double* GetRInner() {return kRInner;}
  static inline const G4double* GetROuter() {return kROuter;}
  static inline const G4double* GetZPlanes() {return kZPlanes;}
  static inline G4double  GetFlatLength() {return 0.2*MaGe::in;}//0.144
  static inline G4double  GetFlatDepth() {return 0.035*MaGe::in;}
  static inline G4double  GetFlatWidth() {return 0.18*MaGe::in;}
  static inline G4double  GetTolerance() {return G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();}
  static inline G4double  GetPinHoleLength() {return MJDemoSigConnPins::GetLength()+MJDemoSigConnSolder::GetLength()+2.*GetTolerance();}
  static inline G4double  GetPinHoleDiam() {return MJDemoSigConnPins::GetDiam()+2.*GetTolerance();}
  static inline G4double  GetPinHoleRho() {return MJDemoSigConnPins::GetRho();}
private:
  static const size_t kNPlanes=4;
  static const G4double kRInner[kNPlanes]; //defined in MJDemoSigConnHousing.cc
  static const G4double kROuter[kNPlanes];
  static const G4double kZPlanes[kNPlanes];
};
//
#endif
