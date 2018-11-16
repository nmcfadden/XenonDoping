

#ifndef _MJPROTO832SCREW_HH
#define _MJPROTO832SCREW_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJProto832Screw: public MJVDemoPart
{
  
public:
  MJProto832Screw(G4String partName, G4String serialNumber, 
					G4double length);
  MJProto832Screw(const MJProto832Screw &);
  ~MJProto832Screw();

  G4LogicalVolume* ConstructPart();

  void SetLength(G4double length)		{fScrewLength = length*CLHEP::mm;}
  G4double GetLength()					{return fScrewLength;}  

private:
  G4double fScrewLength;
};
//
#endif
