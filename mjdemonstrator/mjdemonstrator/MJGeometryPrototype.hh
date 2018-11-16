#ifndef _MJGEOMETRYPROTOTYPE_HH
#define _MJGEOMETRYPROTOTYPE_HH


//---------------------------------------------------------------------------//
//SPECIAL NOTES:
//  If the cables are included, there are overlapping geometries. -Jacquie
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

class MJGeometryPrototypeMessenger;

using namespace std;

class MJGeometryPrototype: public MGGeometryDetector
{
public:
  MJGeometryPrototype(G4String);
  virtual ~MJGeometryPrototype();

  bool UsesMJPSQLDatabase() { return true; }  
  void ConstructDetector();
  
  G4String 		   GetPCposition(){ return fPCposition; }
  G4bool		   IsDRshellOn() { return fDRshellOn; }
  G4bool           IsCavernOn() { return fCavernOn; }
  G4bool		   IsDRwallsOn() { return fDRwallsOn; }
  G4bool		   IsMuonVetoOn() { return fMuonVetoOn; }
  G4String		   GetVetoConfig() { return fVetoConfig; }
  G4bool		   IsRadShieldOn() { return fRadShieldOn; }
  G4String		   GetRadConfig() { return fRadConfig; }
  G4bool           IsMonoCuPlateOn() { return fMonoCuPlateOn; }
  G4bool           IsCalOn() { return fCalOn; }
  G4bool           IsCryoOn() { return fCryoOn; }
  G4bool           IsThermosiphonOn() { return fThermosiphonOn; }
  G4bool           IsThermalShieldOn() { return fThermalShieldOn; }
  //G4bool           IsCablesOn() { return fCablesOn; }
  G4bool           IsStringOn() { return fStringOn; }
  G4bool		   IsTempSensorOn() { return fTempSensorOn; }

  void SetPCposition(G4String PCposition) {fPCposition = PCposition;}
  void SetDRshell(G4bool DRshellOn) { fDRshellOn = DRshellOn; }
  void SetCavern(G4bool cavernOn) { fCavernOn = cavernOn; }
  void SetDRwalls(G4bool DRwallsOn) { fDRwallsOn = DRwallsOn; }
  void SetMuonVeto(G4bool muonVetoOn) { fMuonVetoOn = muonVetoOn; }
  void SetVetoConfig(G4String vetoConfig) { fVetoConfig = vetoConfig; }
  void SetRadShield(G4bool radShieldOn) { fRadShieldOn = radShieldOn; }
  void SetRadConfig(G4String radConfig) { fRadConfig = radConfig; }
  void SetMonoCuPlate(G4bool monoCuPlateOn) { fMonoCuPlateOn = monoCuPlateOn; }
  void SetCal(G4bool calOn) { fCalOn = calOn; }
  void SetCryo(G4bool cryoOn) { fCryoOn = cryoOn; }
  void SetThermosiphon(G4bool thermosiphonOn) { fThermosiphonOn = thermosiphonOn; }
  void SetThermalShield(G4bool thermalShieldOn) { fThermalShieldOn = thermalShieldOn; }
  //void SetCables(G4bool cablesOn) { fCablesOn = cablesOn; }
  void SetString(G4bool stringOn) { fStringOn = stringOn; }
  void SetTempSensor(G4bool tempSensorOn) { fTempSensorOn = tempSensorOn; }

  virtual int GetSensitiveVolumeID(const std::string& volName);

private:

	G4VPhysicalVolume* cavernPhys;
	G4LogicalVolume* cavernLog;
	G4VPhysicalVolume* DRShellPhys;
    G4LogicalVolume* DRShellLog;
    G4VPhysicalVolume* DRwallsPhys;
    G4LogicalVolume* DRwallsLog;
	
	MJGeometryPrototypeMessenger *fMessenger;

  G4String fPCposition; // PC position in room (in shield or at glovebox)
  G4bool   fDRshellOn; // turn the detector room rock shell on
  G4bool   fCavernOn; // turn the cavern on 
  G4bool   fDRwallsOn; // turn the detector room walls (and floor) on
  G4bool   fMuonVetoOn; // turn muon veto on
  G4String fVetoConfig; // which panels of the veto are enabled
  G4bool   fRadShieldOn; // turn rad shield on
  G4String fRadConfig; // which components of the rad shield are enabled
  G4bool   fMonoCuPlateOn; // turn the monolith Cu plate on  
  G4bool   fCalOn; // turn calibration system on
  G4bool   fCryoOn; // turn cryostat one on 
  G4bool   fThermosiphonOn; // turn Thermosiphon one on 
  G4bool   fThermalShieldOn; // turn thermal shield assembly one on 
  //G4bool   fCablesOn; // turn cables assembly one on 
  G4bool   fStringOn; // turn detector string assembly one on 
  G4bool   fTempSensorOn; 

};
#endif

