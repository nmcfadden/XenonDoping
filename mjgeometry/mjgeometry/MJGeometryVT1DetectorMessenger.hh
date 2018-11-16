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
 *      
 * CLASS DECLARATION: MJGeometryVT1DetectorMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *  Modeled After MJGeometryWIPPnNewShieldMessenger.hh -rhenning
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
 * AUTHOR: K. Macon
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 
 * 
 * 
 *
 */
// --------------------------------------------------------------------------//
#ifndef _MJGEOMETRYSOLIDBLOCKMESSENGER_HH
#define _MJGEOMETRYSOLIDBLOCKMESSENGER_HH
//---------------------------------------------------------------------------//
#include "G4UImessenger.hh"
//---------------------------------------------------------------------------//
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class MJGeometryVT1Detector;
class MJGeometryVT1DetectorMessenger : public G4UImessenger
{
public:
  //default constructor
  MJGeometryVT1DetectorMessenger(MJGeometryVT1Detector *VT1);
  //destructor
  ~MJGeometryVT1DetectorMessenger();
  //public interface
  void SetNewValue(G4UIcommand *cmd, G4String cmdstr);
  //protected members
protected:
  //private  members
private:
  G4UIdirectory             *fVT1DetectorDir;
  G4UIcmdWithABool          *fUseSampleCmd;
  G4UIcmdWithAString        *fChooseSampleCmd;
  G4UIcmdWithAString        *fPlaceSampleCmd;
  MJGeometryVT1Detector     *fVT1Detector;
};
#endif
