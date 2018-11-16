

#ifndef _MJPROTOTHERMALSHIELDASSEMBLY_HH
#define _MJPROTOTHERMALSHIELDASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoThermalShieldCan;
class MJDemoThermalShieldAnnulus;
class MJDemoThermalShieldSupport;
class MJDemoThermalShieldWedge;
class MJProto832Screw;

using namespace std;
//---------------------------------------------------------------------------//

class MJProtoThermalShieldAssembly: public MJVDemoAssembly
{
  
public:
  MJProtoThermalShieldAssembly(G4String, G4String);
  MJProtoThermalShieldAssembly(const MJProtoThermalShieldAssembly &);
  ~MJProtoThermalShieldAssembly();

  MJDemoThermalShieldCan* GetCan() {return fCanPtr;}
  MJDemoThermalShieldAnnulus* GetAnnulus() {return fAnnulusPtr;}
  MJDemoThermalShieldSupport* GetSupport1() {return fSupport1Ptr;}
  MJDemoThermalShieldSupport* GetSupport2() {return fSupport2Ptr;}
  MJDemoThermalShieldSupport* GetSupport3() {return fSupport3Ptr;}
  MJDemoThermalShieldWedge* GetWedge1() {return fWedge1Ptr;}
  MJDemoThermalShieldWedge* GetWedge2() {return fWedge2Ptr;}
  MJDemoThermalShieldWedge* GetWedge3() {return fWedge3Ptr;}
  MJProto832Screw* GetScrew1() {return fScrew1Ptr;}
  MJProto832Screw* GetScrew2() {return fScrew2Ptr;}
  MJProto832Screw* GetScrew3() {return fScrew3Ptr;}
  MJProto832Screw* GetScrew4() {return fScrew4Ptr;}
  MJProto832Screw* GetScrew5() {return fScrew5Ptr;}
  MJProto832Screw* GetScrew6() {return fScrew6Ptr;}
  MJProto832Screw* GetScrew7() {return fScrew7Ptr;}
  MJProto832Screw* GetScrew8() {return fScrew8Ptr;}
  MJProto832Screw* GetSideScrew1() {return fSideScrew1Ptr;}
  MJProto832Screw* GetSideScrew2() {return fSideScrew2Ptr;}
  MJProto832Screw* GetSideScrew3() {return fSideScrew3Ptr;}
  MJProto832Screw* GetSideScrew4() {return fSideScrew4Ptr;}
  MJProto832Screw* GetSideScrew5() {return fSideScrew5Ptr;}
  MJProto832Screw* GetSideScrew6() {return fSideScrew6Ptr;}  
  
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);
  
private:
  MJDemoThermalShieldCan* fCanPtr;
  MJDemoThermalShieldAnnulus* fAnnulusPtr;
  MJDemoThermalShieldSupport* fSupport1Ptr;
  MJDemoThermalShieldSupport* fSupport2Ptr;
  MJDemoThermalShieldSupport* fSupport3Ptr;
  MJDemoThermalShieldWedge* fWedge1Ptr;
  MJDemoThermalShieldWedge* fWedge2Ptr;
  MJDemoThermalShieldWedge* fWedge3Ptr;
  MJProto832Screw* fScrew1Ptr;
  MJProto832Screw* fScrew2Ptr;
  MJProto832Screw* fScrew3Ptr;
  MJProto832Screw* fScrew4Ptr;
  MJProto832Screw* fScrew5Ptr;
  MJProto832Screw* fScrew6Ptr;
  MJProto832Screw* fScrew7Ptr;    
  MJProto832Screw* fScrew8Ptr;    
  MJProto832Screw* fSideScrew1Ptr;
  MJProto832Screw* fSideScrew2Ptr;
  MJProto832Screw* fSideScrew3Ptr;
  MJProto832Screw* fSideScrew4Ptr;
  MJProto832Screw* fSideScrew5Ptr;
  MJProto832Screw* fSideScrew6Ptr;    

};
//
#endif
