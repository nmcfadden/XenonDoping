#ifndef _MJPROTOTEMPSENSORCLAMP_HH
#define _MJPROTOTEMPSENSORCLAMP_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJProtoTempSensorClamp: public MJVDemoPart
{
  
public:
  MJProtoTempSensorClamp(G4String, G4String);
  MJProtoTempSensorClamp(const MJProtoTempSensorClamp &);
  ~MJProtoTempSensorClamp();

  G4LogicalVolume* ConstructPart();

  
private:
};
//
#endif
