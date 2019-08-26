
#ifndef _BACON_BASELINE_HH
#define _BACON_BASELINE_HH

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

class BACON_BaselineMessenger;

using namespace std;

class BACON_Baseline: public MGGeometryDetector
{
public:
  BACON_Baseline(G4String);
  virtual ~BACON_Baseline();
  G4String GetMaterialsList() { return "BACON"; }
  void ConstructDetector();

  virtual int GetSensitiveVolumeID(const std::string& volName);

  G4double GetCryostatID(){return fCryoID;}
  G4double GetCryostatHeight() {return fCryoHeight;}

  void SetLiquid(G4String str){fLiquid = str;} 



private:
  static const G4double LambdaE;
  static const G4double inch;
  G4double fCryoOD; 
  G4double fCryoThickness; 
  G4double fCryoID; 
  G4double fCryoHeight; 
  G4double fDelta; 

  BACON_BaselineMessenger * fMessenger;
  G4String fLiquid;
};
#endif
