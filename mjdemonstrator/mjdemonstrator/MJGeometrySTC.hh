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
// CLASS IMPLEMENTATION:  MJGeometrySTC.hh
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
#ifndef _MJGEOMETRYSTC_HH
#define _MJGEOMETRYSTC_HH


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

class MGCrystalData;
class MJGeometrySTCMessenger;

using namespace std;

class MJGeometrySTC: public MGGeometryDetector
{
public:
  MJGeometrySTC(G4String);
  virtual ~MJGeometrySTC();

  bool UsesMJPSQLDatabase() { return true; } 
  void ConstructDetector();

  G4bool		   IsEFROn() { return fEFROn; }
  G4bool           IsCavernOn() { return fCavernOn; }
  G4bool		   IsEFRwallsOn() { return fEFRwallsOn; }
  G4bool		   IsStringTestCryoOn() { return fStringTestCryoOn; }
  G4double		   ColdPlateRotation() { return fColdPlateRotationInDegrees; }
  G4double		   CMSRotation() { return fCMSRotationInDegrees; }
  G4double		   ThermalRotation() { return fThermalRotationInDegrees; }
  G4bool		   IsStringOn() { return fStringOn; }

  void SetEFR(G4bool EFROn) { fEFROn = EFROn; }
  void SetCavern(G4bool cavernOn) { fCavernOn = cavernOn; }
  void SetEFRwalls(G4bool EFRwallsOn) { fEFRwallsOn = EFRwallsOn; }
  void SetStringTestCryo(G4bool stringTestCryoOn) { fStringTestCryoOn = stringTestCryoOn; }
  void SetColdPlateAngleInDegrees(G4double coldPlateRotationInDegrees) { fColdPlateRotationInDegrees = coldPlateRotationInDegrees; }
  void SetCMSAngleInDegrees(G4double CMSRotationInDegrees) { fCMSRotationInDegrees = CMSRotationInDegrees; }
  void SetThermalAngleInDegrees(G4double thermalRotationInDegrees) { fThermalRotationInDegrees = thermalRotationInDegrees; }
  void SetString(G4bool stringOn) { fStringOn = stringOn; }

  virtual int GetSensitiveVolumeID(const std::string& volName);

private:

     G4VPhysicalVolume* cavernPhys;
     G4LogicalVolume* cavernLog;
     G4VPhysicalVolume* EFRShellPhys;
     G4LogicalVolume* EFRShellLog;
     G4VPhysicalVolume* EFRwallsPhys;
     G4LogicalVolume* EFRwallsLog;
    
     MJGeometrySTCMessenger *fMessenger;

  //G4bool   fIsoOn; // turn the radioactive isotope rock shell on
  G4bool   fEFROn; // turn the e-forming room rock shell on
  G4bool   fCavernOn; // turn the cavern on
  G4bool   fEFRwallsOn; // turn the e-forming room walls on
  G4bool   fStringTestCryoOn; // turn the STC on
  G4double fColdPlateRotationInDegrees; // rotation of cold plate (and SAP screws and shunt straps) wrt the string
  G4double fCMSRotationInDegrees; // rotation of CMS wrt the string
  G4double fThermalRotationInDegrees; // rotation of the IR shield (and Cu foil) wrt the string
  G4bool   fStringOn; // turn string on
};
#endif
