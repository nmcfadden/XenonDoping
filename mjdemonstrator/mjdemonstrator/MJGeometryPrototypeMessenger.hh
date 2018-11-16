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
 * CLASS DECLARATION:  MJGeometryPrototypeMessenger.hh
 *
 *---------------------------------------------------------------------------//
 **
 * SPECIAL NOTES:
 * 
 * If the cables are included, there are overlapping geometries. -Jacquie
 *
 */ 
// --------------------------------------------------------------------------//

#ifndef _MGGEOMETRYPROTOTYPEMESSENGER_HH
#define _MGGEOMETRYPROTOTYPEMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

class MJGeometryPrototype;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;


//---------------------------------------------------------------------------//

class MJGeometryPrototypeMessenger : public G4UImessenger
{
public:

  //default constructor
  MJGeometryPrototypeMessenger(MJGeometryPrototype*);

  //destructor
  ~MJGeometryPrototypeMessenger();

  void SetNewValue(G4UIcommand*, G4String);

  //protected members
protected:


private:
  G4UIdirectory               *fPrototypeDirectory;
  MJGeometryPrototype      	  *fPrototype; 
  G4UIcmdWithAString          *fPCpositionCmd;
  G4UIcmdWithABool			  *fDRshellOnCmd;
  G4UIcmdWithABool            *fCavernOnCmd;
  G4UIcmdWithABool			  *fDRwallsOnCmd;
  G4UIcmdWithABool			  *fMuonVetoOnCmd;
  G4UIcmdWithAString		  *fVetoConfigCmd;
  G4UIcmdWithABool			  *fRadShieldOnCmd;
  G4UIcmdWithAString		  *fRadConfigCmd;
  G4UIcmdWithABool			  *fCalOnCmd;
  G4UIcmdWithABool            *fMonoCuPlateOnCmd;
  G4UIcmdWithABool            *fCryoOnCmd;
  G4UIcmdWithABool            *fThermosiphonOnCmd;
  G4UIcmdWithABool            *fThermalShieldOnCmd;
  //G4UIcmdWithABool            *fCablesOnCmd;
  G4UIcmdWithABool            *fStringOnCmd;
  G4UIcmdWithABool			  *fTempSensorOnCmd;
//  G4UIcmdWithoutParameter     *fSetDefaultsCmd;
};
#endif
