#ifndef _MJPROTOTEMPSENSORSCREW_HH
#define _MJPROTOTEMPSENSORSCREW_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJProtoTempSensorScrew: public MJVDemoPart
{
  
public:
  MJProtoTempSensorScrew(G4String, G4String);
  MJProtoTempSensorScrew(const MJProtoTempSensorScrew &);
  ~MJProtoTempSensorScrew();

  G4LogicalVolume* ConstructPart();

  
private:
};
//
#endif
