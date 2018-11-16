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
 * $Id: MJheaderTemplate.hh,v 1.3 2005-02-15 23:26:55 akbarm Exp $
 *      
 * CLASS DECLARATION:  MJGeometryKURFBEGeDetectorMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Messenger for the MJGeometryKURFBEGeDetector class.
 *
 */
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: alexis
 * CONTACT: 
 * FIRST SUBMISSION: Fri Aug 27 07:08:08 PDT 2010
 * 
 * REVISION:
 * 
 * 08-27-2010, Creation, A. Schubert
 * 08-31-2011, Added messenger commands for a collimator, with these
 *             one can turn it on/off and position it, P. Finnerty
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYKURFBEGEDETECTORMESSENGER_HH
#define _MJGEOMETRYKURFBEGEDETECTORMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class MJGeometryKURFBEGeDetector;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcommand;

class MJGeometryKURFBEGeDetectorMessenger : public G4UImessenger 
{
public:

  //default constructor
  MJGeometryKURFBEGeDetectorMessenger(MJGeometryKURFBEGeDetector* kurfBEGeDetector);

  //destructor
  ~MJGeometryKURFBEGeDetectorMessenger();

  //public interface
  void SetNewValue(G4UIcommand* cmd, G4String nV);

  //protected members
protected:

  G4UIdirectory*                fDirectory;
  MJGeometryKURFBEGeDetector*   fKURFBEGeDetector;
  G4UIcmdWithADouble*           fCylinderAngleCmd;
  G4UIcmdWithABool*             fUseCollimatorCmd;
  G4UIcmdWithAString*           fCollimatorPositionCmd;
  G4UIcmdWithADouble*           fCollimatorXCoordinateIn_mmCmd;
  G4UIcmdWithADouble*           fCollimatorYCoordinateIn_mmCmd;
  G4UIcmdWithADouble*           fCollimatorZCoordinateIn_mmCmd;
  G4UIcmdWithADouble*           fCollimatorRCoordinateIn_mmCmd;
  G4UIcmdWithADouble*           fCollimatorAngleIn_degCmd;

};
#endif
