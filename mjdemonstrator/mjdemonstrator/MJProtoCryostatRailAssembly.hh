#ifndef _MJPROTOCRYOSTATRAILASSEMBLY_HH
#define _MJPROTOCRYOSTATRAILASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoCryostatLidRail;
class MJDemoCryostatCenterRail;
class MJProtoCryostatBolt;
class MJProtoHVNut;

using namespace std;
//---------------------------------------------------------------------------//

class MJProtoCryostatRailAssembly: public MJVDemoAssembly
{
  
public:
  MJProtoCryostatRailAssembly(G4String, G4String);
  MJProtoCryostatRailAssembly(const MJProtoCryostatRailAssembly &);
  ~MJProtoCryostatRailAssembly();

  MJDemoCryostatLidRail* GetLidRail1() {return fLidRail1Ptr;}
  MJDemoCryostatLidRail* GetLidRail2() {return fLidRail2Ptr;}
  MJDemoCryostatLidRail* GetLidRail3() {return fLidRail3Ptr;}
  MJDemoCryostatLidRail* GetLidRail4() {return fLidRail4Ptr;}
  MJDemoCryostatCenterRail* GetCenterRail1() {return fCenterRail1Ptr;} 
  MJDemoCryostatCenterRail* GetCenterRail2() {return fCenterRail1Ptr;} 
  MJDemoCryostatCenterRail* GetCenterRail3() {return fCenterRail1Ptr;} 
  MJDemoCryostatCenterRail* GetCenterRail4() {return fCenterRail1Ptr;} 
  MJProtoCryostatBolt* GetBolt1() {return fBolt1Ptr;}
  MJProtoCryostatBolt* GetBolt2() {return fBolt1Ptr;}
  MJProtoCryostatBolt* GetBolt3() {return fBolt1Ptr;}
  MJProtoCryostatBolt* GetBolt4() {return fBolt1Ptr;}
  MJProtoCryostatBolt* GetBolt5() {return fBolt1Ptr;}
  MJProtoCryostatBolt* GetBolt6() {return fBolt1Ptr;}
  MJProtoCryostatBolt* GetBolt7() {return fBolt1Ptr;}
  MJProtoCryostatBolt* GetBolt8() {return fBolt1Ptr;}
  MJProtoCryostatBolt* GetBolt9() {return fBolt1Ptr;}
  MJProtoCryostatBolt* GetBolt10() {return fBolt1Ptr;}
  MJProtoCryostatBolt* GetBolt11() {return fBolt1Ptr;}
  MJProtoCryostatBolt* GetBolt12() {return fBolt1Ptr;}
  MJProtoHVNut* GetNut1() {return fNut1Ptr;}
  MJProtoHVNut* GetNut2() {return fNut1Ptr;}
  MJProtoHVNut* GetNut3() {return fNut1Ptr;}
  MJProtoHVNut* GetNut4() {return fNut1Ptr;}
  MJProtoHVNut* GetNut5() {return fNut1Ptr;}
  MJProtoHVNut* GetNut6() {return fNut1Ptr;}
  MJProtoHVNut* GetNut7() {return fNut1Ptr;}
  MJProtoHVNut* GetNut8() {return fNut1Ptr;}
  MJProtoHVNut* GetNut9() {return fNut1Ptr;}
  MJProtoHVNut* GetNut10() {return fNut1Ptr;}
  MJProtoHVNut* GetNut11() {return fNut1Ptr;}
  MJProtoHVNut* GetNut12() {return fNut1Ptr;}


  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

    
private:
  MJDemoCryostatLidRail*    fLidRail1Ptr;
  MJDemoCryostatLidRail*    fLidRail2Ptr;
  MJDemoCryostatLidRail*    fLidRail3Ptr;
  MJDemoCryostatLidRail*    fLidRail4Ptr;
  MJDemoCryostatCenterRail* fCenterRail1Ptr; 
  MJDemoCryostatCenterRail* fCenterRail2Ptr; 
  MJDemoCryostatCenterRail* fCenterRail3Ptr; 
  MJDemoCryostatCenterRail* fCenterRail4Ptr; 
  MJProtoCryostatBolt*       fBolt1Ptr;
  MJProtoCryostatBolt*       fBolt2Ptr;
  MJProtoCryostatBolt*       fBolt3Ptr;
  MJProtoCryostatBolt*       fBolt4Ptr;
  MJProtoCryostatBolt*       fBolt5Ptr;
  MJProtoCryostatBolt*       fBolt6Ptr;
  MJProtoCryostatBolt*       fBolt7Ptr;
  MJProtoCryostatBolt*       fBolt8Ptr;
  MJProtoCryostatBolt*       fBolt9Ptr;
  MJProtoCryostatBolt*       fBolt10Ptr;
  MJProtoCryostatBolt*       fBolt11Ptr;
  MJProtoCryostatBolt*       fBolt12Ptr;  
  MJProtoHVNut*              fNut1Ptr;
  MJProtoHVNut*              fNut2Ptr;
  MJProtoHVNut*              fNut3Ptr;
  MJProtoHVNut*              fNut4Ptr;
  MJProtoHVNut*              fNut5Ptr;
  MJProtoHVNut*              fNut6Ptr;
  MJProtoHVNut*              fNut7Ptr;
  MJProtoHVNut*              fNut8Ptr;
  MJProtoHVNut*              fNut9Ptr;
  MJProtoHVNut*              fNut10Ptr;
  MJProtoHVNut*              fNut11Ptr;
  MJProtoHVNut*              fNut12Ptr;  
};
//
#endif
