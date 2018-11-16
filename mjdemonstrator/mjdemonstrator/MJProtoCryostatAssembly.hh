#ifndef _MJPROTOCRYOSTATASSEMBLY_HH
#define _MJPROTOCRYOSTATASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoCryostatBottomLid;
class MJDemoCryostatTopLid;
class MJDemoCryostatHoop;
class MJDemoCryostatCrossArmTube;
class MJProtoCryostatRailAssembly;

using namespace std;
//---------------------------------------------------------------------------//

class MJProtoCryostatAssembly: public MJVDemoAssembly
{
  
public:
  MJProtoCryostatAssembly(G4String, G4String);
  MJProtoCryostatAssembly(const MJProtoCryostatAssembly &);
  ~MJProtoCryostatAssembly();

  MJDemoCryostatBottomLid* GetBottomLid() {return fBottomLidPtr;}
  MJDemoCryostatTopLid* GetTopLid() {return fTopLidPtr;}
  MJDemoCryostatHoop* GetHoop() {return fHoopPtr;}
  MJDemoCryostatCrossArmTube* GetTube() {return fCrossArmTubePtr;}
  MJProtoCryostatRailAssembly* GetRailAssembly1() {return fRailAssembly1Ptr;}
  MJProtoCryostatRailAssembly* GetRailAssembly2() {return fRailAssembly2Ptr;}

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemoCryostatBottomLid* fBottomLidPtr;
  MJDemoCryostatTopLid* fTopLidPtr;
  MJDemoCryostatHoop* fHoopPtr;
  MJDemoCryostatCrossArmTube* fCrossArmTubePtr;
  MJProtoCryostatRailAssembly* fRailAssembly1Ptr;
  MJProtoCryostatRailAssembly* fRailAssembly2Ptr; 

};
//
#endif
