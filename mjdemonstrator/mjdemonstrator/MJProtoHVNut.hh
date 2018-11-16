#ifndef _MJPROTOHVNUT_HH
#define _MJPROTOHVNUT_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJProtoHVNut: public MJVDemoPart
{
  
public:
  MJProtoHVNut(G4String, G4String);
  MJProtoHVNut(const MJProtoHVNut &);
  ~MJProtoHVNut();

  G4LogicalVolume* ConstructPart();

  
private:
};
//
#endif
