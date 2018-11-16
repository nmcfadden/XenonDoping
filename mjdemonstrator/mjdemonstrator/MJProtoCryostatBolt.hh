#ifndef _MJPROTOCRYOSTATBOLT_HH
#define _MJPROTOCRYOSTATBOLT_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJProtoCryostatBolt: public MJVDemoPart
{
  
public:
  MJProtoCryostatBolt(G4String, G4String);
  MJProtoCryostatBolt(const MJProtoCryostatBolt &);
  ~MJProtoCryostatBolt();

  G4LogicalVolume* ConstructPart();

  
private:
};
//
#endif
