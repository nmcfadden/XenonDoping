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
 * CLASS DECLARATION:  MJDemoCalibrationAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The calibration assembly
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
 * 02-24-2016, Add members for disabling and positioning 
 *             source and track, T. Caldwell
 * 12-07-2010, Created, M. Boswell
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOCALIBRATIONASSEMBLY_HH
#define _MJDEMOCALIBRATIONASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoCalibrationSource;
class MJDemoCalibrationTrack;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoCalibrationAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoCalibrationAssembly(G4String, G4String);
  MJDemoCalibrationAssembly(const MJDemoCalibrationAssembly &);
  ~MJDemoCalibrationAssembly();

  G4bool GetTrackOn() const {return fTrackOn;}
  G4bool GetSourceOn() const {return fSourceOn;}
  G4ThreeVector GetTrackOffset() const {return fTrackOffset;}
  G4ThreeVector GetSourceOffset() const {return fSourceOffset;}

  void SetTrackOn(G4bool trackOn) {fTrackOn = trackOn;}
  void SetSourceOn(G4bool sourceOn) {fSourceOn = sourceOn;}
  void SetTrackOffset(G4ThreeVector offset) {fTrackOffset = offset;}
  void SetSourceOffset(G4ThreeVector offset) {fSourceOffset = offset;}

  MJDemoCalibrationTrack* GetTrack() {return fTrackPtr;}
  MJDemoCalibrationSource* GetSource()  {return fSourcePtr;}

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
	     G4LogicalVolume* motherLogical);
  
private:
  G4bool fTrackOn;
  G4bool fSourceOn;
  G4ThreeVector fTrackOffset;
  G4ThreeVector fSourceOffset;
  MJDemoCalibrationTrack* fTrackPtr;
  MJDemoCalibrationSource* fSourcePtr;

};
//
#endif
