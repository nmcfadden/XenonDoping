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
 * CLASS DECLARATION:  MJDemoCalibrationSource.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for a conductor core of a HV paralene cable
 *
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
 * AUTHOR: Mitzi Boswell
 * CONTACT: mitzbo@gmail.com
 * FIRST SUBMISSION: Dec 07, 2010
 * 
 * REVISION:
 * 
 * 12-07-2010, Created, M. Boswell
 * 02-24-2016, Approximate helix with an arbitrary number of
 *             torus sections, modified default dimensions, T. Caldwell
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOCALIBRATIONSOURCE_HH
#define _MJDEMOCALIBRATIONSOURCE_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoCalibrationSource: public MJVDemoPart
{
  
public:
  MJDemoCalibrationSource(G4String partName, G4String serialNumber);
  MJDemoCalibrationSource(const MJDemoCalibrationSource &);
  ~MJDemoCalibrationSource();

  G4double GetHelixRadius() const;
  G4double GetTubeInnerRadius() const;
  G4double GetTubeOuterRadius() const;
  G4double GetStartAngle() const;
  G4double GetTotalAngle() const;
  G4double GetHelixAngle() const;
  G4double GetAngleBuffer() const;
  G4int GetNsegments() const;

  void SetHelixRadius(G4double helixRadius);
  void SetTubeInnerRadius(G4double tubeInnerRadius);
  void SetTubeOuterRadius(G4double tubeOuterRadius);
  void SetStartAngle(G4double startAngle);
  void SetTotalAngle(G4double totalAngle);
  void SetHelixAngle(G4double helixAngle);
  void SetAngleBuffer(G4double angleBuffer);
  void SetNsegments(G4int nsegments);

  G4LogicalVolume* ConstructPart();

private:
  G4double fHelixRadius;
  G4double fTubeInnerRadius;
  G4double fTubeOuterRadius;
  G4double fStartAngle;
  G4double fTotalAngle;
  G4double fHelixAngle;
  G4double fAngleBuffer;
  G4int fNsegments;
};

#endif
