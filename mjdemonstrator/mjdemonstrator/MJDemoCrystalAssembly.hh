//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
/**                                                            
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION:  MJDemo77mmCrystalAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The assembly consisting of a 77mm diameter crystal, with it's accompaniments.
 *
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 * 
 * 07-22-2010, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOCRYSTALASSEMBLY_HH
#define _MJDEMOCRYSTALASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"
#include "mjdemonstrator/MJDemoPuckBeGe.hh"
#include <vector>
class MJDemoPuckBeGe;
class MJDemoJSONDet;
class MJDemoOrtec;
class MJDemoHVRing77;
class MJDemoHVNut;
class MJDemoHVForkAssembly;
class MJDemoHVForkNut;
class MJDemoHollowHexRod;
class MJDemoCrystalInsulator;
class MJDemoFlexInsulator;
class MJDemoCableGuide;
class MJDemoCableGuideAssembly;
class MJDemoCrystalMountingPlate;
class MJDemoContactPin;
class MJDemoCenterBushing;
class MJDemoSpringFEMount;
class MJDemoLMFECoverPlate;
class MJDemoCableAssembly;
class MJDemoCrystalSpringNut;
class MJDemoLMFEAssembly;
//class MJDemoCopperSpacer;
#include "mjdemonstrator/MJDemoCopperSpacer.hh"

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoCrystalAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoCrystalAssembly(
	G4String fName, 
	G4String serialNumber
  );
  MJDemoCrystalAssembly(const MJDemoCrystalAssembly &);
  ~MJDemoCrystalAssembly();

  G4double GetAdditionalLength(){return fAddLength;}

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  void SetCrystalDiameter(G4double diameter)	{fCrystalDiameter = diameter*CLHEP::mm;}
  void SetCrystalHeight(G4double height)		{fCrystalHeight = height*CLHEP::mm;}
  void SetPostRadius(G4double radius)			{fPostRadius = radius*CLHEP::mm;}  
  G4double GetCrystalDiameter() 				{return fCrystalDiameter;}
  G4double GetCrystalHeight()					{return fCrystalHeight;}  
  G4double GetPostRadius()						{return fPostRadius;}
  G4bool IsOppe(){return fOppe;}

  MJVDemoPart* GetCrystal() {return fCrystalPtr;}
  MJDemoHVRing77* GetHVRing() {return fHVRingPtr;}
  MJDemoHVNut* GetHVNut1() {return fHVNut1Ptr;}
  MJDemoHVNut* GetHVNut2() {return fHVNut2Ptr;}
  MJDemoHVNut* GetHVNut3() {return fHVNut3Ptr;}
  MJDemoHollowHexRod* GetHexRod1() {return fHexRod1Ptr;}
  MJDemoHollowHexRod* GetHexRod2() {return fHexRod2Ptr;}
  MJDemoHollowHexRod* GetHexRod3() {return fHexRod3Ptr;}
  MJDemoHVForkAssembly* GetHVFork() {return fHVForkPtr;}
  MJDemoHVForkNut* GetHVForkNut() {return fHVForkNutPtr;}
  MJDemoCrystalInsulator* GetInsulator1() {return fInsulator1Ptr;}
  MJDemoCrystalInsulator* GetInsulator2() {return fInsulator2Ptr;}
  MJDemoCrystalInsulator* GetInsulator3() {return fInsulator3Ptr;}
  MJDemoCableGuideAssembly* GetCableGuideAssembly() {return fCableGuideAssemPtr;}
  MJDemoCableGuide* GetCableGuide1() {return fCableGuide1Ptr;}
  MJDemoCableGuide* GetCableGuide2() {return fCableGuide2Ptr;}
  MJDemoCrystalMountingPlate* GetMountingPlate() {return fMountingPlatePtr;}
  MJDemoContactPin* GetContactPin() {return fContactPinPtr;}
  MJDemoCenterBushing* GetBushing() {return fBushingPtr;}
  MJDemoSpringFEMount* GetFEMount() {return fFEMountPtr;}
  MJDemoLMFECoverPlate* GetCoverPlate() {return fCoverPlatePtr;}
  MJDemoCableAssembly* GetCable1() {return fCable1Ptr;}
  MJDemoCableAssembly* GetCable2() {return fCable2Ptr;}
  MJDemoCrystalSpringNut* GetNut1() {return fSNut1Ptr;}
  MJDemoCrystalSpringNut* GetNut2() {return fSNut2Ptr;}
  MJDemoCrystalSpringNut* GetNut3() {return fSNut3Ptr;}
  MJDemoLMFEAssembly* GetLMFE() {return fLMFEPtr;}
  MJDemoCopperSpacer* GetSpacer1() {return fSpacer1Ptr;}
  MJDemoCopperSpacer* GetSpacer2() {return fSpacer2Ptr;}
  MJDemoCopperSpacer* GetSpacer3() {return fSpacer3Ptr;}
  const std::vector<G4String>& GetSpacerType() const { return fSpacerType; }
  void ConstructParts();
  
private:
  G4double fCrystalDiameter;
  G4double fCrystalHeight;
  G4double fPostRadius;
  G4double fAddLength;
  G4bool fOppe;
  std::vector<G4String> fSpacerType;

  void AllocateParts();
  void UnallocateParts();

  MJVDemoDetector* fCrystalPtr; //this polymorphs into whatever detector
  MJDemoHVRing77* fHVRingPtr;
  MJDemoHVNut* fHVNut1Ptr;
  MJDemoHVNut* fHVNut2Ptr;
  MJDemoHVNut* fHVNut3Ptr;
  MJDemoHollowHexRod* fHexRod1Ptr;
  MJDemoHollowHexRod* fHexRod2Ptr;
  MJDemoHollowHexRod* fHexRod3Ptr;
  MJDemoHVForkAssembly* fHVForkPtr;
  MJDemoHVForkNut* fHVForkNutPtr;
  MJDemoCrystalInsulator* fInsulator1Ptr;
  MJDemoCrystalInsulator* fInsulator2Ptr;
  MJDemoCrystalInsulator* fInsulator3Ptr;
  MJDemoFlexInsulator* fFlexInsulator1Ptr;
  MJDemoFlexInsulator* fFlexInsulator2Ptr;
  MJDemoFlexInsulator* fFlexInsulator3Ptr;
  MJDemoCableGuideAssembly* fCableGuideAssemPtr;
  MJDemoCableGuide* fCableGuide1Ptr;
  MJDemoCableGuide* fCableGuide2Ptr;
  MJDemoCrystalMountingPlate* fMountingPlatePtr;
  MJDemoContactPin* fContactPinPtr;
  MJDemoCenterBushing* fBushingPtr;
  MJDemoSpringFEMount* fFEMountPtr;
  MJDemoLMFECoverPlate* fCoverPlatePtr;
  MJDemoCableAssembly* fCable1Ptr;
  MJDemoCableAssembly* fCable2Ptr;
  MJDemoCrystalSpringNut* fSNut1Ptr;
  MJDemoCrystalSpringNut* fSNut2Ptr;
  MJDemoCrystalSpringNut* fSNut3Ptr;
  MJDemoLMFEAssembly* fLMFEPtr;
  MJDemoCopperSpacer* fSpacer1Ptr;
  MJDemoCopperSpacer* fSpacer2Ptr;
  MJDemoCopperSpacer* fSpacer3Ptr;
};
//
#endif
