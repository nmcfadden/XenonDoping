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
//                                                          
// $Id: MGcodeTemplate.cc,v 1.1 2004-12-09 08:58:35 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MJDemoCrystalAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Crystal Contact
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 * 
 * 07-22-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"
#include "G4UIcommand.hh"
//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjio/MJJSONReader.hh"
#include "mjdemonstrator/MJDemoCrystalAssembly.hh"
#include "mjdemonstrator/MJDemoPuckBeGe.hh"
#include "mjdemonstrator/MJDemoOrtec.hh"
#include "mjdemonstrator/MJDemoJSONDet.hh"
#include "mjdemonstrator/MJDemoHVRing77.hh"
#include "mjdemonstrator/MJDemoHVNut.hh"
#include "mjdemonstrator/MJDemoHollowHexRod.hh"
#include "mjdemonstrator/MJDemoHVForkAssembly.hh"
#include "mjdemonstrator/MJDemoHVForkNut.hh"
#include "mjdemonstrator/MJDemoCrystalInsulator.hh"
#include "mjdemonstrator/MJDemoFlexInsulator.hh"
#include "mjdemonstrator/MJDemoCableGuideAssembly.hh"
#include "mjdemonstrator/MJDemoCableGuide.hh"
#include "mjdemonstrator/MJDemoCrystalMountingPlate.hh"
#include "mjdemonstrator/MJDemoContactPin.hh"
#include "mjdemonstrator/MJDemoCenterBushing.hh"
#include "mjdemonstrator/MJDemoSpringFEMount.hh"
#include "mjdemonstrator/MJDemoLMFECoverPlate.hh"
#include "mjdemonstrator/MJDemoCableAssembly.hh"
#include "mjdemonstrator/MJDemoCrystalSpringNut.hh"
#include "mjdemonstrator/MJDemoLMFEAssembly.hh"
#include "mjdemonstrator/MJDemoCopperSpacer.hh"

using namespace CLHEP;



//---------------------------------------------------------------------------//

MJDemoCrystalAssembly::MJDemoCrystalAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "MJ80-02-100B"), fPostRadius(1.8400*25.4*mm), fAddLength(0.0), fOppe(0)
{
  MJJSONReader* reader = new MJJSONReader();
  std::vector<G4String> detectorSerial = reader->ReadCrystalAssembly(serialNumber);
  delete reader;
  if(detectorSerial.size() == 0)
    {
      fCrystalDiameter = 77*mm;
      fCrystalHeight = 33*mm;
      fOppe = 0;
      fCrystalPtr = new MJDemoPuckBeGe(fName+"_ActiveCrystal_001", "867-5309", fCrystalDiameter, fCrystalHeight);
      AllocateParts();
    }
  else
    {
      MJDemoJSONDet* tempPtr = new MJDemoJSONDet(fName + "_ActiveJSONDet_001", detectorSerial.at(0));
      if(tempPtr->GetDetParameters().size() < 3)
	{
	  fCrystalDiameter = tempPtr->GetDiameter();
	  fCrystalHeight = tempPtr->GetHeight();
	  if((fCrystalHeight - 33*mm) > 0.1*mm)
	    fAddLength = 13*mm;
	  if((fCrystalHeight - 33*mm) > 13*mm)
	    fAddLength = 20*mm;
	  //fAddLength = fCrystalHeight-33*mm;
	  delete tempPtr;
	  fOppe = 0;
	  //std::cout << fName << ": JSON det determined to be Oppe: fOppe = " << fOppe << std::endl;
	  //std::cout << "The additional length for hex rod: " << fAddLength << std::endl;
	  fCrystalPtr = new MJDemoPuckBeGe(fName+"_ActiveCrystal_001", "867-5309", fCrystalDiameter, fCrystalHeight);
	}
      else
	{
	  fCrystalPtr = tempPtr;
	  fCrystalHeight = fCrystalPtr->GetHeight();
	  fCrystalDiameter = fCrystalPtr->GetDiameter(); //2.0*crystalData->GetOuterRadius(-fCrystalHeight/2);
	  if((fCrystalHeight - 33*mm) > 0.1*mm)
	    fAddLength = 13*mm;
	  if((fCrystalHeight - 33*mm) > 13*mm)
	    fAddLength = 20*mm;
	  fOppe = 1;
	  //std::cout << "At the JSON Det: fOppe = " << fOppe << std::endl;
	}
      AllocateParts();
      }
}

MJDemoCrystalAssembly::MJDemoCrystalAssembly(const MJDemoCrystalAssembly & rhs) : 
  MJVDemoAssembly(rhs), fCrystalDiameter(rhs.fCrystalDiameter), 
  fCrystalHeight(rhs.fCrystalHeight), fPostRadius(rhs.fPostRadius), fAddLength(rhs.fAddLength), fOppe(rhs.fOppe)
{
  AllocateParts();
}

MJDemoCrystalAssembly::~MJDemoCrystalAssembly()
{
  UnallocateParts();
}

void MJDemoCrystalAssembly::AllocateParts()
{
  fHVRingPtr = new MJDemoHVRing77(fName+"_HVRing77_001", "867-5309");  
  fPartsList.push_back(fHVRingPtr);
  fHVNut1Ptr = new MJDemoHVNut(fName+"_HVNut_001", "867-5309");  
  fPartsList.push_back(fHVNut1Ptr);
  fHVNut2Ptr = new MJDemoHVNut(fName+"_HVNut_002", "867-5309");  
  fPartsList.push_back(fHVNut2Ptr);
  fHVNut3Ptr = new MJDemoHVNut(fName+"_HVNut_003", "867-5309");  
  fPartsList.push_back(fHVNut3Ptr);
  fHexRod1Ptr = new MJDemoHollowHexRod(fName+"_HollowHexRod_001", "867-5309");  
  fPartsList.push_back(fHexRod1Ptr);
  fHexRod2Ptr = new MJDemoHollowHexRod(fName+"_HollowHexRod_002", "867-5309");  
  fPartsList.push_back(fHexRod2Ptr);
  fHexRod3Ptr = new MJDemoHollowHexRod(fName+"_HollowHexRod_003", "867-5309");
  fPartsList.push_back(fHexRod3Ptr);
  fSpacer1Ptr = new MJDemoCopperSpacer(fName+"_CopperSpacer_001", "867-5309");
  fPartsList.push_back(fSpacer1Ptr);
  fSpacer2Ptr = new MJDemoCopperSpacer(fName+"_CopperSpacer_002", "867-5309");
  fPartsList.push_back(fSpacer2Ptr);
  fSpacer3Ptr = new MJDemoCopperSpacer(fName+"_CopperSpacer_003", "867-5309");
  fPartsList.push_back(fSpacer3Ptr);
  fHVForkPtr = new MJDemoHVForkAssembly(fName+"_HVFork", "867-5309");
  fAssembliesList.push_back(fHVForkPtr);
  fHVForkNutPtr = new MJDemoHVForkNut(fName+"_HVForkNut", "867-5309");
  fPartsList.push_back(fHVForkNutPtr);
  fInsulator1Ptr = new MJDemoCrystalInsulator(fName+"_CrystalInsulator_001", "867-5309");  
  fPartsList.push_back(fInsulator1Ptr);
  fInsulator2Ptr = new MJDemoCrystalInsulator(fName+"_CrystalInsulator_002", "867-5309");  
  fPartsList.push_back(fInsulator2Ptr);
  fInsulator3Ptr = new MJDemoCrystalInsulator(fName+"_CrystalInsulator_003", "867-5309");
  fPartsList.push_back(fInsulator3Ptr);
  fFlexInsulator1Ptr = new MJDemoFlexInsulator(fName+"_FlexInsulator_001", "867-5309");
  fPartsList.push_back(fFlexInsulator1Ptr);
  fFlexInsulator2Ptr = new MJDemoFlexInsulator(fName+"_FlexInsulator_002", "867-5309");
  fPartsList.push_back(fFlexInsulator2Ptr);
  fFlexInsulator3Ptr = new MJDemoFlexInsulator(fName+"_FlexInsulator_003", "867-5309");
  fPartsList.push_back(fFlexInsulator3Ptr);
  fCableGuideAssemPtr = new MJDemoCableGuideAssembly(fName+"_CableGuideAssembly_001", "867-5309");
  fAssembliesList.push_back(fCableGuideAssemPtr);
  fCableGuide1Ptr = new MJDemoCableGuide(fName+"_CableGuide_001","867-5309");  
  fPartsList.push_back(fCableGuide1Ptr);
  fCableGuide2Ptr = new MJDemoCableGuide(fName+"_CableGuide_002","867-5309");  
  fPartsList.push_back(fCableGuide2Ptr);
  fMountingPlatePtr = new MJDemoCrystalMountingPlate(fName+"_CrystalMountingPlate_001", "867-5309");  
  fPartsList.push_back(fMountingPlatePtr);
  fContactPinPtr = new MJDemoContactPin(fName+"_ContactPin_001", "867-5309");  
  fPartsList.push_back(fContactPinPtr);
  fBushingPtr = new MJDemoCenterBushing(fName+"_CenterBushing_001", "867-5309");  
  fPartsList.push_back(fBushingPtr);
  fFEMountPtr = new MJDemoSpringFEMount(fName+"_SpringFEMount_001", "867-5309");  
  fPartsList.push_back(fFEMountPtr);
  fCoverPlatePtr = new MJDemoLMFECoverPlate(fName+"_LMFECoverPlate_001", "867-5309");  
  fPartsList.push_back(fCoverPlatePtr);
  fCable1Ptr = new MJDemoCableAssembly(fName+"_Cable_001", "867-5309", 0.912*25.4*mm);
  fAssembliesList.push_back(fCable1Ptr);
  fCable2Ptr = new MJDemoCableAssembly(fName+"_Cable_002", "867-5309", 0.54*25.4*mm);
  fAssembliesList.push_back(fCable2Ptr);
  fSNut1Ptr = new MJDemoCrystalSpringNut(fName+"_SpringNut_001", "867-5309");
  fPartsList.push_back(fSNut1Ptr);
  fSNut2Ptr = new MJDemoCrystalSpringNut(fName+"_SpringNut_002", "867-5309");
  fPartsList.push_back(fSNut2Ptr);
  fSNut3Ptr = new MJDemoCrystalSpringNut(fName+"_SpringNut_003", "867-5309");
  fPartsList.push_back(fSNut3Ptr);
  fLMFEPtr = new MJDemoLMFEAssembly(fName+"_LMFEAssembly_001", "867-5309"); 
  fAssembliesList.push_back(fLMFEPtr);
}

void MJDemoCrystalAssembly::UnallocateParts()
{
  delete fCrystalPtr;
  delete fHVRingPtr;
  delete fHVNut1Ptr;
  delete fHVNut2Ptr;
  delete fHVNut3Ptr;
  delete fHexRod1Ptr;
  delete fHexRod2Ptr;
  delete fHexRod3Ptr;
  delete fHVForkPtr;
  delete fHVForkNutPtr;
  delete fInsulator1Ptr;
  delete fInsulator2Ptr;
  delete fInsulator3Ptr;
  delete fFlexInsulator1Ptr;
  delete fFlexInsulator2Ptr;
  delete fFlexInsulator3Ptr;
  delete fCableGuide1Ptr;
  delete fCableGuide2Ptr;
  delete fMountingPlatePtr;
  delete fContactPinPtr;
  delete fBushingPtr;
  delete fFEMountPtr;
  delete fCoverPlatePtr;
  delete fCable1Ptr;
  delete fCable2Ptr;
  delete fSNut1Ptr;
  delete fSNut2Ptr;
  delete fSNut3Ptr;
  delete fLMFEPtr;
  fPartsList.resize(0);
  fAssembliesList.resize(0);
}

void MJDemoCrystalAssembly::ConstructParts()
{
  //Put the spacers adjacent to the ends of the Hollow Hex Rods, leaving a gap
  //of 0.002 in. The origin of the coordinate system for each spacer is located
  //in the center of the plane demarcated by the flat (non-bulletized) end of
  //the spacer. The spacers need to be rotated 180 degrees before being placed
  //next to the HHRs. The part of the HHR that fits inside the spacer begins
  //2.056 inches below the top of the hex-shaped head of the bolt for a 53mm
  //HHR.
  MJJSONReader* reader = new MJJSONReader();
  fSpacerType = reader->ReadCustom(fSerialNumber, "crystalassembly", "spacer");
  if(fSpacerType.size()>0 && fSpacerType.at(0) != "none") //For the spacers, we don't need one if we have 5 BeGes
    {
      cout << "Spacer type: " << fSpacerType.at(0) << endl;
      if(fSpacerType.at(0) == "singleDU") //test this
        {
          fSpacer1Ptr->SetAdditionalLength(0.308*25.4*mm);
          fSpacer1Ptr->SetDrawingNumber("MJ80-02-421");
          fSpacer2Ptr->SetAdditionalLength(0.308*25.4*mm);
          fSpacer2Ptr->SetDrawingNumber("MJ80-02-421");
          fSpacer3Ptr->SetAdditionalLength(0.308*25.4*mm);
          fSpacer3Ptr->SetDrawingNumber("MJ80-02-421");
        }
      if(fSpacerType.at(0) == "short")
        {
          fSpacer1Ptr->SetAdditionalLength(-0.23*25.4*mm);
          fSpacer1Ptr->SetDrawingNumber("MJ80-11-184");
          fSpacer2Ptr->SetAdditionalLength(-0.23*25.4*mm);
          fSpacer2Ptr->SetDrawingNumber("MJ80-11-184");
          fSpacer3Ptr->SetAdditionalLength(-0.23*25.4*mm);
          fSpacer3Ptr->SetDrawingNumber("MJ80-11-184");
        }
      if(fSpacerType.at(0) == "2BEGeAnd2Ortec")
        {
          //This is the default length for the class
          fSpacer1Ptr->SetDrawingNumber("MJ80-02-121");
          fSpacer2Ptr->SetDrawingNumber("MJ80-02-121");
          fSpacer3Ptr->SetDrawingNumber("MJ80-02-121");
        }
      if(fSpacerType.at(0) == "3Ortec")
        {
          fSpacer1Ptr->SetAdditionalLength(1.2*25.4*mm);
          fSpacer1Ptr->SetDrawingNumber("MJ80-02-117");
          fSpacer2Ptr->SetAdditionalLength(1.2*25.4*mm);
          fSpacer2Ptr->SetDrawingNumber("MJ80-02-117");
          fSpacer3Ptr->SetAdditionalLength(1.2*25.4*mm);
          fSpacer3Ptr->SetDrawingNumber("MJ80-02-117");
        }
  }
  MJVDemoAssembly::ConstructParts();
}

void MJDemoCrystalAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);


  //Place the crystal so that the point-contact is at the origin of the
  //coordinate system.
  G4ThreeVector *crystal1localPos = new G4ThreeVector(0, 0, -fCrystalHeight/2-0.001*25.4*mm);//no-0.001
  G4RotationMatrix *crystal1localRot;
  //if(fOppe)
  crystal1localRot = new G4RotationMatrix(0, pi, 0);
  //else
    //  crystal1localRot = new G4RotationMatrix();
  G4AffineTransform *crystal1affine = new G4AffineTransform(crystal1localRot,*crystal1localPos);
  *crystal1affine *= *assemAffine;  
  G4ThreeVector *crystal1globalPos = new G4ThreeVector(crystal1affine->NetTranslation());
  G4RotationMatrix *crystal1globalRot= new G4RotationMatrix(crystal1affine->NetRotation());
  fCrystalPtr->Place(crystal1globalPos, crystal1globalRot, motherLogical);
//Wenqin
  delete crystal1localPos;
  delete crystal1localRot;
  delete crystal1affine;
  
  //Set HVRing z position so that the lip sits below the detector. The origin
  //of coordinate system of the HV ring is located in the center of the plane
  //demarcated by the top of the part of the ring that goes down the side of
  //the crystal.
  G4double HVRingZposition = 0.19*25.4*mm - fCrystalHeight-0.002*25.4*mm;//no -0.002
  G4ThreeVector *HVRing1localPos = new G4ThreeVector(0, 0, HVRingZposition);
  G4RotationMatrix *HVRing1localRot = new G4RotationMatrix();
  G4AffineTransform *HVRing1affine = new G4AffineTransform(HVRing1localRot,*HVRing1localPos);
  *HVRing1affine *= *assemAffine;  
  G4ThreeVector *HVRing1globalPos = new G4ThreeVector(HVRing1affine->NetTranslation());
  G4RotationMatrix *HVRing1globalRot= new G4RotationMatrix(HVRing1affine->NetRotation());
  fHVRingPtr->Place(HVRing1globalPos, HVRing1globalRot, motherLogical);
  delete HVRing1localPos;
  delete HVRing1localRot;
  delete HVRing1affine;
  
  G4double HVNutZposition = HVRingZposition - .061*25.4*mm;//0.06
  G4ThreeVector *HVNut1localPos = new G4ThreeVector(fPostRadius, 0, HVNutZposition);
  G4RotationMatrix *HVNut1localRot = new G4RotationMatrix();
  G4AffineTransform *HVNut1affine = new G4AffineTransform(HVNut1localRot,*HVNut1localPos);
  *HVNut1affine *= *assemAffine;  
  G4ThreeVector *HVNut1globalPos = new G4ThreeVector(HVNut1affine->NetTranslation());
  G4RotationMatrix *HVNut1globalRot= new G4RotationMatrix(HVNut1affine->NetRotation());
  fHVNut1Ptr->Place(HVNut1globalPos, HVNut1globalRot, motherLogical);
  delete HVNut1localPos;
  delete HVNut1localRot;
  delete HVNut1affine;
  
  G4ThreeVector *HVNut2localPos = new G4ThreeVector(
  				fPostRadius*cos(120*deg), fPostRadius*sin(120*deg), HVNutZposition);
  G4RotationMatrix *HVNut2localRot = new G4RotationMatrix();
  G4AffineTransform *HVNut2affine = new G4AffineTransform(HVNut2localRot,*HVNut2localPos);
  *HVNut2affine*=*assemAffine;  
  G4ThreeVector *HVNut2globalPos = new G4ThreeVector(HVNut2affine->NetTranslation());
  G4RotationMatrix *HVNut2globalRot= new G4RotationMatrix(HVNut2affine->NetRotation());
  fHVNut2Ptr->Place(HVNut2globalPos, HVNut2globalRot, motherLogical);
  delete HVNut2localPos;
  delete HVNut2localRot;
  delete HVNut2affine;
  
  G4ThreeVector *HVNut3localPos = new G4ThreeVector(
  				fPostRadius*cos(240*deg), fPostRadius*sin(240*deg), HVNutZposition);
  G4RotationMatrix *HVNut3localRot = new G4RotationMatrix();
  G4AffineTransform *HVNut3affine = new G4AffineTransform(HVNut3localRot,*HVNut3localPos);
  *HVNut3affine*=*assemAffine;  
  G4ThreeVector *HVNut3globalPos = new G4ThreeVector(HVNut3affine->NetTranslation());
  G4RotationMatrix *HVNut3globalRot= new G4RotationMatrix(HVNut3affine->NetRotation());
  fHVNut3Ptr->Place(HVNut3globalPos, HVNut3globalRot, motherLogical);
  delete HVNut3localPos;
  delete HVNut3localRot;
  delete HVNut3affine;
  
  G4double HollowHexRodZposition = 0.5279*25.4*mm;//0.5269
  fHexRod1Ptr->SetDrawingNumber("HHR_"+G4UIcommand::ConvertToString((G4int)fAddLength+53)+"mm");
  fHexRod2Ptr->SetDrawingNumber("HHR_"+G4UIcommand::ConvertToString((G4int)fAddLength+53)+"mm");
  fHexRod3Ptr->SetDrawingNumber("HHR_"+G4UIcommand::ConvertToString((G4int)fAddLength+53)+"mm");
  G4ThreeVector *HollowHexRod1localPos = new G4ThreeVector(fPostRadius, 0, HollowHexRodZposition);
  G4RotationMatrix *HollowHexRod1localRot = new G4RotationMatrix();
  G4AffineTransform *HollowHexRod1affine = new G4AffineTransform(HollowHexRod1localRot, 
                                                                 *HollowHexRod1localPos);
  *HollowHexRod1affine*=*assemAffine;  
  G4ThreeVector *HollowHexRod1globalPos = new G4ThreeVector(HollowHexRod1affine->NetTranslation());
  G4RotationMatrix *HollowHexRod1globalRot= new G4RotationMatrix(
                                                               HollowHexRod1affine->NetRotation());
  fHexRod1Ptr->SetAdditionalLength(fAddLength);
  fHexRod1Ptr->Place(HollowHexRod1globalPos, HollowHexRod1globalRot, motherLogical);
  delete HollowHexRod1localPos;
  delete HollowHexRod1localRot;
  delete HollowHexRod1affine;
  
  G4ThreeVector *HollowHexRod2localPos = new G4ThreeVector(
                       	fPostRadius*cos(120*deg), fPostRadius*sin(120*deg), HollowHexRodZposition);
  G4RotationMatrix *HollowHexRod2localRot = new G4RotationMatrix();
  G4AffineTransform *HollowHexRod2affine = new G4AffineTransform(HollowHexRod2localRot,
                                                                 *HollowHexRod2localPos);
  *HollowHexRod2affine*=*assemAffine;  
  G4ThreeVector *HollowHexRod2globalPos = new G4ThreeVector(HollowHexRod2affine->NetTranslation());
  G4RotationMatrix *HollowHexRod2globalRot= new G4RotationMatrix(
                                                               HollowHexRod2affine->NetRotation());
  fHexRod2Ptr->SetAdditionalLength(fAddLength);
  fHexRod2Ptr->Place(HollowHexRod2globalPos, HollowHexRod2globalRot, motherLogical);
  delete HollowHexRod2localPos;
  delete HollowHexRod2localRot;
  delete HollowHexRod2affine;
  
  G4ThreeVector *HollowHexRod3localPos = new G4ThreeVector(
  						fPostRadius*cos(240*deg), fPostRadius*sin(240*deg), HollowHexRodZposition);
  G4RotationMatrix *HollowHexRod3localRot = new G4RotationMatrix();
  G4AffineTransform *HollowHexRod3affine = new G4AffineTransform(HollowHexRod3localRot,
                                                                 *HollowHexRod3localPos);
  *HollowHexRod3affine*=*assemAffine;  
  G4ThreeVector *HollowHexRod3globalPos = new G4ThreeVector(HollowHexRod3affine->NetTranslation());
  G4RotationMatrix *HollowHexRod3globalRot= new G4RotationMatrix(
                                                               HollowHexRod3affine->NetRotation());
  fHexRod3Ptr->SetAdditionalLength(fAddLength);
  fHexRod3Ptr->Place(HollowHexRod3globalPos, HollowHexRod3globalRot, motherLogical);
  delete HollowHexRod3localPos;
  delete HollowHexRod3localRot;
  delete HollowHexRod3affine;
 
  if(fSpacerType.size()>0 && fSpacerType.at(0)!="none")
    {
      G4double CopperSpacerZposition = HollowHexRodZposition -2.056*25.4*mm - fHexRod1Ptr->GetAdditionalLength() - 0.002*25.4*mm;//no -0.002
      G4ThreeVector *CopperSpacer1localPos = new G4ThreeVector(fPostRadius*cos(0), fPostRadius*sin(0), CopperSpacerZposition);
      G4RotationMatrix *CopperSpacer1localRot = new G4RotationMatrix();
      CopperSpacer1localRot->rotateX(pi);
      G4AffineTransform *CopperSpacer1affine = new G4AffineTransform(CopperSpacer1localRot,*CopperSpacer1localPos);
      *CopperSpacer1affine *= *assemAffine;
      G4ThreeVector *CopperSpacer1globalPos = new G4ThreeVector(CopperSpacer1affine->NetTranslation());
      G4RotationMatrix *CopperSpacer1globalRot = new G4RotationMatrix(CopperSpacer1affine->NetRotation());
      fSpacer1Ptr->Place(CopperSpacer1globalPos, CopperSpacer1globalRot, motherLogical);
      delete CopperSpacer1localPos;
      delete CopperSpacer1localRot;
      delete CopperSpacer1affine;

      G4ThreeVector *CopperSpacer2localPos = new G4ThreeVector(fPostRadius*cos(120*deg), fPostRadius*sin(120*deg), CopperSpacerZposition);
      G4RotationMatrix *CopperSpacer2localRot = new G4RotationMatrix();
      CopperSpacer2localRot->rotateX(pi);
      G4AffineTransform *CopperSpacer2affine = new G4AffineTransform(CopperSpacer2localRot,*CopperSpacer2localPos);
      *CopperSpacer2affine *= *assemAffine;
      G4ThreeVector *CopperSpacer2globalPos = new G4ThreeVector(CopperSpacer2affine->NetTranslation());
      G4RotationMatrix *CopperSpacer2globalRot = new G4RotationMatrix(CopperSpacer2affine->NetRotation());
      fSpacer2Ptr->Place(CopperSpacer2globalPos, CopperSpacer2globalRot, motherLogical);
      delete CopperSpacer2localPos;
      delete CopperSpacer2localRot;
      delete CopperSpacer2affine;

      G4ThreeVector *CopperSpacer3localPos = new G4ThreeVector(fPostRadius*cos(240*deg), fPostRadius*sin(240*deg), CopperSpacerZposition);
      G4RotationMatrix *CopperSpacer3localRot = new G4RotationMatrix();
      CopperSpacer3localRot->rotateX(pi);
      G4AffineTransform *CopperSpacer3affine = new G4AffineTransform(CopperSpacer3localRot,*CopperSpacer3localPos);
      *CopperSpacer3affine *= *assemAffine;
      G4ThreeVector *CopperSpacer3globalPos = new G4ThreeVector(CopperSpacer3affine->NetTranslation());
      G4RotationMatrix *CopperSpacer3globalRot = new G4RotationMatrix(CopperSpacer3affine->NetRotation());
      fSpacer3Ptr->Place(CopperSpacer3globalPos, CopperSpacer3globalRot, motherLogical);
      delete CopperSpacer3localPos;
      delete CopperSpacer3localRot;
      delete CopperSpacer3affine;
    }
  else cout << "No spacer for this crystal assembly" << endl;

  G4ThreeVector* HVForkNutLocalPos = new G4ThreeVector(fPostRadius, 0, 0.22*25.4*mm + 0.32/2*25.4*mm-fCrystalHeight);
  G4RotationMatrix* HVForkNutLocalRot = new G4RotationMatrix();
  HVForkNutLocalRot->rotateY(pi);
  G4AffineTransform* HVForkNutAffine = new G4AffineTransform(HVForkNutLocalRot, *HVForkNutLocalPos);
  *HVForkNutAffine*=*assemAffine;
  G4ThreeVector *HVForkNutGlobalPos = new G4ThreeVector(HVForkNutAffine->NetTranslation());
  G4RotationMatrix *HVForkNutGlobalRot= new G4RotationMatrix(
                                                               HVForkNutAffine->NetRotation());
  fHVForkNutPtr->Place(HVForkNutGlobalPos, HVForkNutGlobalRot, motherLogical);
  delete HVForkNutLocalPos;
  delete HVForkNutLocalRot;
  delete HVForkNutAffine;

  G4ThreeVector* HVForkLocalPos = new G4ThreeVector(fPostRadius, 0.07625*25.4*mm, 0.185*25.4*mm -fCrystalHeight);
  G4RotationMatrix* HVForkLocalRot = new G4RotationMatrix();
  HVForkLocalRot->rotateZ(pi/2);
  HVForkLocalRot->rotateY(pi);
  G4AffineTransform* HVForkAffine = new G4AffineTransform(HVForkLocalRot, *HVForkLocalPos);
  *HVForkAffine*=*assemAffine;
  G4ThreeVector *HVForkGlobalPos = new G4ThreeVector(HVForkAffine->NetTranslation());
  G4RotationMatrix *HVForkGlobalRot= new G4RotationMatrix(
                                                               HVForkAffine->NetRotation());
  fHVForkPtr->Place(HVForkGlobalPos, HVForkGlobalRot, motherLogical);
  delete HVForkLocalPos;
  delete HVForkLocalRot;
  delete HVForkAffine;
 
  G4double insulatorRadius = 1.4900*25.4*mm;
  //std::cout << "Before the if statement!" << std::endl;
  if(fOppe) //if using a JSON Detector
    {
      //std::cout << "We are in the if statement!!!" << std::endl;
      insulatorRadius += 0.111*25.4*mm; //the zero on the flex insulator is 0.111 in right of the hole.
      fFlexInsulator1Ptr->SetDetectorRadius(fCrystalDiameter/2.0);
      fFlexInsulator1Ptr->SetDrawingNumber("FlexInsulator"+G4UIcommand::ConvertToString((G4int)(fCrystalDiameter*100)));
      fFlexInsulator2Ptr->SetDetectorRadius(fCrystalDiameter/2.0);
      fFlexInsulator2Ptr->SetDrawingNumber("FlexInsulator"+G4UIcommand::ConvertToString((G4int)(fCrystalDiameter*100)));
      fFlexInsulator3Ptr->SetDetectorRadius(fCrystalDiameter/2.0);
      fFlexInsulator3Ptr->SetDrawingNumber("FlexInsulator"+G4UIcommand::ConvertToString((G4int)(fCrystalDiameter*100)));
  
      G4double FlexInsulatorZposition = -5.4/2*mm;
      G4ThreeVector *FlexInsulator1localPos = new G4ThreeVector(insulatorRadius, 0, 
								FlexInsulatorZposition);
      G4RotationMatrix *FlexInsulator1localRot = new G4RotationMatrix();
      FlexInsulator1localRot->rotateX(pi);
      G4AffineTransform *FlexInsulator1affine = new G4AffineTransform(FlexInsulator1localRot,
								      *FlexInsulator1localPos);
      *FlexInsulator1affine*=*assemAffine;  
      G4ThreeVector *FlexInsulator1globalPos = new G4ThreeVector(
								 FlexInsulator1affine->NetTranslation());
      G4RotationMatrix *FlexInsulator1globalRot= new G4RotationMatrix(
								      FlexInsulator1affine->NetRotation());
      fFlexInsulator1Ptr->Place(FlexInsulator1globalPos, FlexInsulator1globalRot, motherLogical);
      delete FlexInsulator1localPos;
      delete FlexInsulator1localRot;
      delete FlexInsulator1affine;
      
      G4ThreeVector *FlexInsulator2localPos = new G4ThreeVector(insulatorRadius*cos(120*deg), 
								   insulatorRadius*sin(120*deg), FlexInsulatorZposition);
      G4RotationMatrix *FlexInsulator2localRot = new G4RotationMatrix();
      FlexInsulator2localRot->rotateX(pi);
      FlexInsulator2localRot->rotateZ(120*deg);
      G4AffineTransform *FlexInsulator2affine = new G4AffineTransform(FlexInsulator2localRot,
									 *FlexInsulator2localPos);
      *FlexInsulator2affine*=*assemAffine;  
      G4ThreeVector *FlexInsulator2globalPos = new G4ThreeVector(
								    FlexInsulator2affine->NetTranslation());
      G4RotationMatrix *FlexInsulator2globalRot= new G4RotationMatrix(
									 FlexInsulator2affine->NetRotation());
      fFlexInsulator2Ptr->Place(FlexInsulator2globalPos, FlexInsulator2globalRot, motherLogical);
      delete FlexInsulator2localPos;
      delete FlexInsulator2localRot;
      delete FlexInsulator2affine;
      
      G4ThreeVector *FlexInsulator3localPos = new G4ThreeVector(insulatorRadius*cos(240*deg), 
								   insulatorRadius*sin(240*deg), FlexInsulatorZposition);
      G4RotationMatrix *FlexInsulator3localRot = new G4RotationMatrix();
      FlexInsulator3localRot->rotateX(pi);
      FlexInsulator3localRot->rotateZ(240*deg);
      G4AffineTransform *FlexInsulator3affine = new G4AffineTransform(FlexInsulator3localRot,
									 *FlexInsulator3localPos);
      *FlexInsulator3affine*=*assemAffine;  
      G4ThreeVector *FlexInsulator3globalPos = new G4ThreeVector(
								    FlexInsulator3affine->NetTranslation());
      G4RotationMatrix *FlexInsulator3globalRot= new G4RotationMatrix(
									 FlexInsulator3affine->NetRotation());
      fFlexInsulator3Ptr->Place(FlexInsulator3globalPos, FlexInsulator3globalRot, motherLogical);
      delete FlexInsulator3localPos;
      delete FlexInsulator3localRot;
      delete FlexInsulator3affine;
    }
  else // if using a BEGe
    {
      //std::cout << "We are in the else statement!" << std::endl;
      G4double CrystalInsulatorZposition = 0;
      G4ThreeVector *CrystalInsulator1localPos = new G4ThreeVector(insulatorRadius, 0, 
								   CrystalInsulatorZposition);
      G4RotationMatrix *CrystalInsulator1localRot = new G4RotationMatrix();
      G4AffineTransform *CrystalInsulator1affine = new G4AffineTransform(CrystalInsulator1localRot,
									 *CrystalInsulator1localPos);
      *CrystalInsulator1affine*=*assemAffine;  
      G4ThreeVector *CrystalInsulator1globalPos = new G4ThreeVector(
								    CrystalInsulator1affine->NetTranslation());
      G4RotationMatrix *CrystalInsulator1globalRot= new G4RotationMatrix(
									 CrystalInsulator1affine->NetRotation());
      fInsulator1Ptr->Place(CrystalInsulator1globalPos, CrystalInsulator1globalRot, motherLogical);
      delete CrystalInsulator1localPos;
      delete CrystalInsulator1localRot;
      delete CrystalInsulator1affine;
      
      G4ThreeVector *CrystalInsulator2localPos = new G4ThreeVector(insulatorRadius*cos(120*deg), 
								   insulatorRadius*sin(120*deg), CrystalInsulatorZposition);
      G4RotationMatrix *CrystalInsulator2localRot = new G4RotationMatrix();
      CrystalInsulator2localRot->rotateZ(-120*deg);
      G4AffineTransform *CrystalInsulator2affine = new G4AffineTransform(CrystalInsulator2localRot,
									 *CrystalInsulator2localPos);
      *CrystalInsulator2affine*=*assemAffine;  
      G4ThreeVector *CrystalInsulator2globalPos = new G4ThreeVector(
								    CrystalInsulator2affine->NetTranslation());
      G4RotationMatrix *CrystalInsulator2globalRot= new G4RotationMatrix(
									 CrystalInsulator2affine->NetRotation());
      fInsulator2Ptr->Place(CrystalInsulator2globalPos, CrystalInsulator2globalRot, motherLogical);
      delete CrystalInsulator2localPos;
      delete CrystalInsulator2localRot;
      delete CrystalInsulator2affine;
      
      G4ThreeVector *CrystalInsulator3localPos = new G4ThreeVector(insulatorRadius*cos(240*deg), 
								   insulatorRadius*sin(240*deg), CrystalInsulatorZposition);
      G4RotationMatrix *CrystalInsulator3localRot = new G4RotationMatrix();
      CrystalInsulator3localRot->rotateZ(-240*deg);
      G4AffineTransform *CrystalInsulator3affine = new G4AffineTransform(CrystalInsulator3localRot,
									 *CrystalInsulator3localPos);
      *CrystalInsulator3affine*=*assemAffine;  
      G4ThreeVector *CrystalInsulator3globalPos = new G4ThreeVector(
								    CrystalInsulator3affine->NetTranslation());
      G4RotationMatrix *CrystalInsulator3globalRot= new G4RotationMatrix(
									 CrystalInsulator3affine->NetRotation());
      fInsulator3Ptr->Place(CrystalInsulator3globalPos, CrystalInsulator3globalRot, motherLogical);
      delete CrystalInsulator3localPos;
      delete CrystalInsulator3localRot;
      delete CrystalInsulator3affine;
    }
      
  G4double CableGuideZposition = /*4.25*mm*/5*mm-0.25*25.4*mm-.001*25.4*mm;//4.213
  G4ThreeVector *CableGuide1localPos = new G4ThreeVector(fPostRadius+0.013*25.4*mm, 0, CableGuideZposition);
  G4RotationMatrix *CableGuide1localRot = new G4RotationMatrix();
  CableGuide1localRot->rotateZ(pi/2);
  CableGuide1localRot->rotateX(pi);
  G4AffineTransform *CableGuide1affine = new G4AffineTransform(CableGuide1localRot,
                                                                     *CableGuide1localPos);
  *CableGuide1affine*=*assemAffine;  
  G4ThreeVector *CableGuide1globalPos = new G4ThreeVector(CableGuide1affine->NetTranslation());
  G4RotationMatrix *CableGuide1globalRot= new G4RotationMatrix(CableGuide1affine->NetRotation());
  fCableGuideAssemPtr->Place(CableGuide1globalPos, CableGuide1globalRot, motherLogical);
  delete CableGuide1localPos;
  delete CableGuide1localRot;
  delete CableGuide1affine;
  
  
  /*G4double CableGuideZposition = 4.25*mm;
  G4ThreeVector *CableGuide1localPos = new G4ThreeVector(fPostRadius, 0, CableGuideZposition);
  G4RotationMatrix *CableGuide1localRot = new G4RotationMatrix();
  CableGuide1localRot->rotateY(pi);
  G4AffineTransform *CableGuide1affine = new G4AffineTransform(CableGuide1localRot,
                                                                     *CableGuide1localPos);
  *CableGuide1affine*=*assemAffine;  
  G4ThreeVector *CableGuide1globalPos = new G4ThreeVector(CableGuide1affine->NetTranslation());
  G4RotationMatrix *CableGuide1globalRot= new G4RotationMatrix(CableGuide1affine->NetRotation());
  fCableGuide1Ptr->Place(CableGuide1globalPos, CableGuide1globalRot, motherLogical);
  delete CableGuide1localPos;
  delete CableGuide1localRot;
  delete CableGuide1affine;

  G4ThreeVector *CableGuide2localPos = new G4ThreeVector(fPostRadius*cos(240*deg), 
  												fPostRadius*sin(240*deg), CableGuideZposition);
  G4RotationMatrix *CableGuide2localRot = new G4RotationMatrix();
  CableGuide2localRot->rotateY(pi);
  CableGuide2localRot->rotateZ(240*deg);
  G4AffineTransform *CableGuide2affine = new G4AffineTransform(CableGuide2localRot,
                                                                     *CableGuide2localPos);
  *CableGuide2affine*=*assemAffine;  
  G4ThreeVector *CableGuide2globalPos = new G4ThreeVector(CableGuide2affine->NetTranslation());
  G4RotationMatrix *CableGuide2globalRot= new G4RotationMatrix(CableGuide2affine->NetRotation());
  fCableGuide2Ptr->Place(CableGuide2globalPos, CableGuide2globalRot, motherLogical);
  delete CableGuide2localPos;
  delete CableGuide2localRot;
  delete CableGuide2affine;*/

  G4ThreeVector *CrystalMountingPlatelocalPos = new G4ThreeVector(0, 0, ((0.126*25.4)+5)*mm);//0.125
  G4RotationMatrix *CrystalMountingPlatelocalRot = new G4RotationMatrix();
  G4AffineTransform *CrystalMountingPlateaffine = new G4AffineTransform(
  									CrystalMountingPlatelocalRot,*CrystalMountingPlatelocalPos);
  *CrystalMountingPlateaffine*=*assemAffine;  
  G4ThreeVector *CrystalMountingPlateglobalPos = new G4ThreeVector(
  													CrystalMountingPlateaffine->NetTranslation());
  G4RotationMatrix *CrystalMountingPlateglobalRot= new G4RotationMatrix(
  													CrystalMountingPlateaffine->NetRotation());
  fMountingPlatePtr->Place(CrystalMountingPlateglobalPos, 
  							  CrystalMountingPlateglobalRot, motherLogical);
  delete CrystalMountingPlatelocalPos;
  delete CrystalMountingPlatelocalRot;
  delete CrystalMountingPlateaffine;

  G4ThreeVector *ContactPinlocalPos = new G4ThreeVector(0, 0, 0.001*25.4*mm);//0.00
  G4RotationMatrix *ContactPinlocalRot = new G4RotationMatrix();
  G4AffineTransform *ContactPinaffine = new G4AffineTransform(ContactPinlocalRot,*ContactPinlocalPos);
  *ContactPinaffine*=*assemAffine;  
  G4ThreeVector *ContactPinglobalPos = new G4ThreeVector(ContactPinaffine->NetTranslation());
  G4RotationMatrix *ContactPinglobalRot= new G4RotationMatrix(ContactPinaffine->NetRotation());
  fContactPinPtr->Place(ContactPinglobalPos, ContactPinglobalRot, motherLogical);
  delete ContactPinlocalPos;
  delete ContactPinlocalRot;
  delete ContactPinaffine;
  
  G4ThreeVector *CenterBushinglocalPos = new G4ThreeVector(0, 0, 0.2779*25.4*mm);//.2769
  G4RotationMatrix *CenterBushinglocalRot = new G4RotationMatrix();
  G4AffineTransform *CenterBushingaffine = new G4AffineTransform(CenterBushinglocalRot,*CenterBushinglocalPos);
  *CenterBushingaffine*=*assemAffine;  
  G4ThreeVector *CenterBushingglobalPos = new G4ThreeVector(CenterBushingaffine->NetTranslation());
  G4RotationMatrix *CenterBushingglobalRot= new G4RotationMatrix(CenterBushingaffine->NetRotation());
  fBushingPtr->Place(CenterBushingglobalPos, CenterBushingglobalRot, motherLogical);
  delete CenterBushinglocalPos;
  delete CenterBushinglocalRot;
  delete CenterBushingaffine;
  
  G4ThreeVector *SpringFEMountlocalPos = new G4ThreeVector(1.183*25.4*mm, 0, 0.3679*25.4*mm);//0.3669
  G4RotationMatrix *SpringFEMountlocalRot = new G4RotationMatrix();
  SpringFEMountlocalRot->rotateZ(pi);
  G4AffineTransform *SpringFEMountaffine = new G4AffineTransform(SpringFEMountlocalRot,*SpringFEMountlocalPos);
  *SpringFEMountaffine*=*assemAffine;  
  G4ThreeVector *SpringFEMountglobalPos = new G4ThreeVector(SpringFEMountaffine->NetTranslation());
  G4RotationMatrix *SpringFEMountglobalRot= new G4RotationMatrix(SpringFEMountaffine->NetRotation());
  fFEMountPtr->Place(SpringFEMountglobalPos, SpringFEMountglobalRot, motherLogical);
  delete SpringFEMountlocalPos;
  delete SpringFEMountlocalRot;
  delete SpringFEMountaffine;

  G4ThreeVector *LMFECoverPlatelocalPos = new G4ThreeVector(1.183*25.4*mm, 0, 0.4819*25.4*mm);//0.4799
  G4RotationMatrix *LMFECoverPlatelocalRot = new G4RotationMatrix();
  G4AffineTransform *LMFECoverPlateaffine = new G4AffineTransform(LMFECoverPlatelocalRot,*LMFECoverPlatelocalPos);
  *LMFECoverPlateaffine*=*assemAffine;  
  G4ThreeVector *LMFECoverPlateglobalPos = new G4ThreeVector(LMFECoverPlateaffine->NetTranslation());
  G4RotationMatrix *LMFECoverPlateglobalRot= new G4RotationMatrix(LMFECoverPlateaffine->NetRotation());
  fCoverPlatePtr->Place(LMFECoverPlateglobalPos, LMFECoverPlateglobalRot, motherLogical);
  delete LMFECoverPlatelocalPos;
  delete LMFECoverPlatelocalRot;
  delete LMFECoverPlateaffine;
  
  G4ThreeVector *Cable1localPos = new G4ThreeVector(0.58*25.4*mm, 0, 0.408*25.4*mm);
  G4RotationMatrix *Cable1localRot = new G4RotationMatrix();
  Cable1localRot->rotateY(pi/2);
  Cable1localRot->rotateZ(pi/2);
  G4AffineTransform *Cable1affine = new G4AffineTransform(Cable1localRot,*Cable1localPos);
  *Cable1affine*=*assemAffine;  
  G4ThreeVector *Cable1globalPos = new G4ThreeVector(Cable1affine->NetTranslation());
  G4RotationMatrix *Cable1globalRot= new G4RotationMatrix(Cable1affine->NetRotation());
  fCable1Ptr->Place(Cable1globalPos, Cable1globalRot, motherLogical);
  delete Cable1localPos;
  delete Cable1localRot;
  delete Cable1affine;  
  
  G4ThreeVector *Cable2localPos = new G4ThreeVector(1.49*25.4*mm, 0, 0.54*25.4*mm);
  G4RotationMatrix *Cable2localRot = new G4RotationMatrix();
  Cable2localRot->rotateZ(-pi/4);
  Cable2localRot->rotateY(pi/2);
  Cable2localRot->rotateZ(pi/2);
  G4AffineTransform *Cable2affine = new G4AffineTransform(Cable2localRot,*Cable2localPos);
  *Cable2affine*=*assemAffine;  
  G4ThreeVector *Cable2globalPos = new G4ThreeVector(Cable2affine->NetTranslation());
  G4RotationMatrix *Cable2globalRot= new G4RotationMatrix(Cable2affine->NetRotation());
  fCable2Ptr->Place(Cable2globalPos, Cable2globalRot, motherLogical);
  delete Cable2localPos;
  delete Cable2localRot;
  delete Cable2affine;   

  G4ThreeVector *SNut1localPos = new G4ThreeVector(-0.3466*25.4*mm, 0, 0.402*25.4*mm);//0.4
  G4RotationMatrix *SNut1localRot = new G4RotationMatrix();
  G4AffineTransform *SNut1affine = new G4AffineTransform(SNut1localRot,*SNut1localPos);
  *SNut1affine*=*assemAffine;  
  G4ThreeVector *SNut1globalPos = new G4ThreeVector(SNut1affine->NetTranslation());
  G4RotationMatrix *SNut1globalRot= new G4RotationMatrix(SNut1affine->NetRotation());
  fSNut1Ptr->Place(SNut1globalPos, SNut1globalRot, motherLogical);
  delete SNut1localPos;
  delete SNut1localRot;
  delete SNut1affine;

  G4ThreeVector *SNut2localPos = new G4ThreeVector(1.0534*25.4*mm, 0.150*25.4*mm, 0.482*25.4*mm);//.480
  G4RotationMatrix *SNut2localRot = new G4RotationMatrix();
  G4AffineTransform *SNut2affine = new G4AffineTransform(SNut2localRot,*SNut2localPos);
  *SNut2affine*=*assemAffine;  
  G4ThreeVector *SNut2globalPos = new G4ThreeVector(SNut2affine->NetTranslation());
  G4RotationMatrix *SNut2globalRot= new G4RotationMatrix(SNut2affine->NetRotation());
  fSNut2Ptr->Place(SNut2globalPos, SNut2globalRot, motherLogical);
  delete SNut2localPos;
  delete SNut2localRot;
  delete SNut2affine;
  
  G4ThreeVector *SNut3localPos = new G4ThreeVector(1.0534*25.4*mm, -0.150*25.4*mm, 0.482*25.4*mm);//.480
  G4RotationMatrix *SNut3localRot = new G4RotationMatrix();
  G4AffineTransform *SNut3affine = new G4AffineTransform(SNut3localRot,*SNut3localPos);
  *SNut3affine*=*assemAffine;  
  G4ThreeVector *SNut3globalPos = new G4ThreeVector(SNut3affine->NetTranslation());
  G4RotationMatrix *SNut3globalRot= new G4RotationMatrix(SNut3affine->NetRotation());
  fSNut3Ptr->Place(SNut3globalPos, SNut3globalRot, motherLogical);
  delete SNut3localPos;
  delete SNut3localRot;
  delete SNut3affine;
  
  G4ThreeVector *LMFEAssemblylocalPos = new G4ThreeVector(0*mm, 0, 9.069*mm+0.0005*25.4*mm);
  G4RotationMatrix *LMFEAssemblylocalRot = new G4RotationMatrix();
  LMFEAssemblylocalRot->rotateX(pi);
  G4AffineTransform *LMFEAssemblyaffine = new G4AffineTransform(LMFEAssemblylocalRot,*LMFEAssemblylocalPos);
  *LMFEAssemblyaffine*=*assemAffine;  
  G4ThreeVector *LMFEAssemblyglobalPos = new G4ThreeVector(LMFEAssemblyaffine->NetTranslation());
  G4RotationMatrix *LMFEAssemblyglobalRot= new G4RotationMatrix(LMFEAssemblyaffine->NetRotation());
  fLMFEPtr->Place(LMFEAssemblyglobalPos, LMFEAssemblyglobalRot, motherLogical);
  delete LMFEAssemblylocalPos;
  delete LMFEAssemblylocalRot;
  delete LMFEAssemblyaffine;
  delete assemAffine;
}
