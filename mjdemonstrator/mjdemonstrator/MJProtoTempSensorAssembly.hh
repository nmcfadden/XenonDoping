#ifndef _MJPROTOTEMPSENSORASSEMBLY_HH
#define _MJPROTOTEMPSENSORASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJProtoTempSensorClamp;
class MJProtoTempSensorScrew;

using namespace std;
//---------------------------------------------------------------------------//

class MJProtoTempSensorAssembly: public MJVDemoAssembly
{
  
public:
  MJProtoTempSensorAssembly(G4String, G4String);
  MJProtoTempSensorAssembly(const MJProtoTempSensorAssembly &);
  ~MJProtoTempSensorAssembly();

  MJProtoTempSensorClamp* GetClamp() {return fClampPtr;}
  MJProtoTempSensorScrew* GetScrew() {return fScrewPtr;}


  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

    
private:
  MJProtoTempSensorClamp*    fClampPtr;
  MJProtoTempSensorScrew*    fScrewPtr;
 
};
//
#endif
