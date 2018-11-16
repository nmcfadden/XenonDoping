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
 * CLASS DECLARATION:  MJGeometryCharacterization.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * MJ Characterization Stand geometry configuration
 *
 */
//
/**  
 * SPECIAL NOTES:
 * 
 * Modified from MJGeometryDemonstrator.hh
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Asher Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 * 
 * REVISION:
 *
 * 07-22-2013, Created, A. Wasserman
 * 01-21-2014, Added ability to rotate ColdPlate, CMS, and IR shield of the STC from messenger. J. MacMullin
 * 09-28-2016, Added the capability to create an STC with a thin IR Shield. This is chosen by a 
 * 	       boolean messenger command 'thinIR' which instantiates the MJSTCAssembly with a serial number
 * 	       "STCCryoThinIR" That tells the MJSTCAssembly to use MJThermalAssemblyThin. O'Shaughnessy
 */
//---------------------------------------------------------------------------//
//


#ifndef _MJGEOMETRYCHARACTERIZATION_HH
#define _MJGEOMETRYCHARACTERIZATION_HH


//---------------------------------------------------------------------------//
#include <string>
#include <vector>

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"



//---------------------------------------------------------------------------//
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;
class G4PVPlacement;

class MJGeometryCharacterizationMessenger;

using namespace std;

class MJGeometryCharacterization: public MGGeometryDetector
{
public:

  MJGeometryCharacterization(G4String);
  virtual ~MJGeometryCharacterization();

  bool UsesMJPSQLDatabase() { return true; }  
  void ConstructDetector();

  G4bool           IsCavernOn() { return fCavernOn; }
  G4double		   ColdPlateRotation() { return fColdPlateRotationInDegrees; }
  G4double		   CMSRotation() { return fCMSRotationInDegrees; }
  G4double		   ThermalRotation() { return fThermalRotationInDegrees; }

  void SetCavern(G4bool cavernOn) { fCavernOn = cavernOn; }
  void SetStand(G4bool standOn) { fStandOn = standOn; }
  void SetCryostat(G4bool cryostatOn) { fCryostatOn = cryostatOn; }
  void SetCryostatChoice(G4String cryostat) { fCryostat = cryostat; }
  void SetColdPlateAngleInDegrees(G4double coldPlateRotationInDegrees) { fColdPlateRotationInDegrees = coldPlateRotationInDegrees; }
  void SetCMSAngleInDegrees(G4double CMSRotationInDegrees) { fCMSRotationInDegrees = CMSRotationInDegrees; }
  void SetThermalAngleInDegrees(G4double ThermalRotationInDegrees) { fThermalRotationInDegrees = ThermalRotationInDegrees; }
  void SetString(G4bool stringOn) { fStringOn = stringOn; }
  void SetThinIR(G4bool thinIR) { fThinIR = thinIR; }
  void SetSourceAngle(G4double sourceAngle) { fSourceAngle = sourceAngle; }
  void SetSourceRadius(G4double sourceRadius) { fSourceRadius = sourceRadius; }
  void SetSourceZ(G4double sourceZ) { fSourceZ = sourceZ; }
  void PrintSourcePosition();
  void SetSourceMaterial(G4String sourceMaterial) { sourceMaterialName = sourceMaterial; }
  void SetStringName(G4String stringName) { fStringName = stringName; }

  virtual int GetSensitiveVolumeID(const std::string& volName);

private:

	G4VPhysicalVolume* cavernPhys;
	G4LogicalVolume* cavernLog;
	MJGeometryCharacterizationMessenger *fMessenger;

  G4bool   fCavernOn; // turn the cavern on 
  G4bool   fStandOn; // turn the characterization stand on
  G4bool   fCryostatOn; // turn the cryostat on 
  G4String fCryostat; // set the choice of which cryostat to use
  G4double fColdPlateRotationInDegrees; // rotation of the STC's cold plate (and SAP screws and shunt straps) wrt the string
  G4double fCMSRotationInDegrees; // rotation of the STC's CMS wrt the string
  G4double fThermalRotationInDegrees; //rotation of the STC's IR shield wrt the string
  G4bool   fStringOn; // turn the string on 
  G4bool   fThinIR; // instantiate the STC using a thin IR shield
  G4double fSourceAngle; // set button source angle
  G4double fSourceRadius; // set button source radius
  G4double fSourceZ; // set button source height
  G4String sourceMaterialName; // set the button source material
  G4String fStringName; // set the button source material

};
#endif

