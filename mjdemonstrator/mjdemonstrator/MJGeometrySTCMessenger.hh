//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//                                                          
//      
// CLASS IMPLEMENTATION:  MJGeometrySTCMessenger.hh
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday June 12 2013
 * 
 * REVISION:
 * 06-12-2013, Created, J. Strain
 * 10-17-2013, Updated Coldplate and added Dewar. J. MacMullin
 * 11-14-2013, 1. Made the STC an assembly so it could place in the CharStand.
 *			   2. Added a string into the STC. J. MacMullin
 * 01-21-2014, 1. Created a "shell" w/in the rock cavern in the e-forming room for 
 *				  simulating the rock around the STC. 
 *			   2. Added in walls of the e-forming room. 
 *             3. Added ability to rotate ColdPlate, CMS, and IR shield from messenger. J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#ifndef _MJGEOMETRYSTCMESSENGER_HH
#define _MJGEOMETRYSTCMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

class MJGeometrySTC;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;


//---------------------------------------------------------------------------//

class MJGeometrySTCMessenger : public G4UImessenger
{
public:

  //default constructor
  MJGeometrySTCMessenger(MJGeometrySTC*);

  //destructor
  ~MJGeometrySTCMessenger();

  void SetNewValue(G4UIcommand*, G4String);

  //protected members
protected:


private:
  G4UIdirectory               *fSTCDirectory;
  MJGeometrySTC               *fSTC;
  G4UIcmdWithABool			  *fEFROnCmd;
  G4UIcmdWithABool            *fCavernOnCmd;
  G4UIcmdWithABool			  *fEFRwallsOnCmd;
  G4UIcmdWithABool			  *fStringTestCryoOnCmd;
  G4UIcmdWithADouble		  *fColdPlateRotationInDegreesCmd;
  G4UIcmdWithADouble		  *fCMSRotationInDegreesCmd;
  G4UIcmdWithADouble		  *fThermalRotationInDegreesCmd;
  G4UIcmdWithABool			  *fStringOnCmd;
//  G4UIcmdWithoutParameter     *fSetDefaultsCmd;
};
#endif
