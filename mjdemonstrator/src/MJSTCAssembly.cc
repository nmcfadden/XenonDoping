//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//      
// CLASS IMPLEMENTATION:  MJSTCAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Assembly Origin: The bottom of the coldplate / top of the IR shield
 * 
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Thursday November 14 2013
 * 
 * REVISION:
 * 11-14-2013, Created, J. MacMullin
 * 01-21-2014, Added ability to rotate ColdPlate, CMS, and IR shield from messenger, J. MacMullin
 *             For more information see: https://majorana.npl.washington.edu/elog/Simulations+and+Analysis/54
 * 03-25-2014, Modified spacing between "touching" surfaces, J. MacMullin
 * 09-28-2014, If the serial number of this cryostat is STCCryoThinIR, use the thin IR shield assembly, 
 * 		C. O'Shaughnessy
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//

#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCAssembly.hh"
#include "mjdemonstrator/MJSTCThermalAssembly.hh"
#include "mjdemonstrator/MJSTCThermalAssemblyThin.hh"
#include "mjdemonstrator/MJSTCColdPlateAssembly.hh"
#include "mjdemonstrator/MJSTCCMSAssembly.hh"
#include "mjdemonstrator/MJSTCMainBodyAssembly.hh"
#include "mjdemonstrator/MJSTCDipstickAssembly.hh"
#include "mjdemonstrator/MJSTCDewarAssembly.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCAssembly::MJSTCAssembly(G4String fName, G4String serialNumber, G4double coldPlateAngle, G4double CMSAngle, G4double thermalAngle) :
  MJVDemoAssembly(fName, serialNumber, "STCAssembly"), fColdPlateAngle(coldPlateAngle*deg), fCMSAngle(CMSAngle*deg), fThermalAngle(thermalAngle*deg)
{ 
  fThin = false;
  if(serialNumber=="STCCryoThinIR")
	  fThin = true;
  if(fThin)
     fThermalThinPtr = new MJSTCThermalAssemblyThin(fName+"_ThermalAssembly_001", "867-5309");
  else
     fThermalPtr = new MJSTCThermalAssembly(fName+"_ThermalAssembly_001", "867-5309");
  fColdPlatePtr = new MJSTCColdPlateAssembly(fName+"_ColdPlateAssembly_001", "867-5309");
  fCMSPtr = new MJSTCCMSAssembly(fName+"_CMSAssembly_001", "867-5309");
  fMainBodyPtr = new MJSTCMainBodyAssembly(fName+"_MainBodyAssembly_001", "867-5309");
  fDipstickPtr = new MJSTCDipstickAssembly(fName+"_DipstickAssembly_001", "867-5309");
  fDewarPtr = new MJSTCDewarAssembly(fName+"_DewarAssembly_001", "867-5309");
}

MJSTCAssembly::MJSTCAssembly(const MJSTCAssembly & rhs) : 
  MJVDemoAssembly(rhs), fColdPlateAngle(rhs.fColdPlateAngle), fCMSAngle(rhs.fCMSAngle), fThermalAngle(rhs.fThermalAngle)
{ 
  if(fThin)
     fThermalThinPtr = new MJSTCThermalAssemblyThin(fName+"_ThermalAssembly_001", "867-5309");
  else
     fThermalPtr = new MJSTCThermalAssembly(fName+"_ThermalAssembly_001", "867-5309");
  fColdPlatePtr = new MJSTCColdPlateAssembly(fName+"_ColdPlateAssembly_001", "867-5309");
  fCMSPtr = new MJSTCCMSAssembly(fName+"_CMSAssembly_001", "867-5309");
  fMainBodyPtr = new MJSTCMainBodyAssembly(fName+"_MainBodyAssembly_001", "867-5309");
  fDipstickPtr = new MJSTCDipstickAssembly(fName+"_DipstickAssembly_001", "867-5309");
  fDewarPtr = new MJSTCDewarAssembly(fName+"_DewarAssembly_001", "867-5309");
}

MJSTCAssembly::~MJSTCAssembly()
{
  delete fThermalPtr;
  delete fThermalThinPtr;
  delete fColdPlatePtr;
  delete fCMSPtr;
  delete fMainBodyPtr;
  delete fDipstickPtr;
  delete fDewarPtr;
}

//---------------------------------------------------------------------------//

void MJSTCAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4double eps = 0.001 * mm;

  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *thermallocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *thermallocalRot = new G4RotationMatrix();
  thermallocalRot->rotateZ(fThermalAngle);
  G4AffineTransform *thermalaffine = new G4AffineTransform(thermallocalRot,*thermallocalPos);
  *thermalaffine*=*assemAffine;  
  G4ThreeVector *thermalglobalPos = new G4ThreeVector(thermalaffine->NetTranslation());
  G4RotationMatrix *thermalglobalRot= new G4RotationMatrix(thermalaffine->NetRotation());
  if(fThin)
      fThermalThinPtr->Place(thermalglobalPos, thermalglobalRot, motherLogical);
  else
      fThermalPtr->Place(thermalglobalPos, thermalglobalRot, motherLogical);
  delete thermallocalPos;
  delete thermallocalRot;
  delete thermalaffine;

  G4ThreeVector *coldplatelocalPos = new G4ThreeVector(0, 0, 3.175*mm+eps);
  G4RotationMatrix *coldplatelocalRot = new G4RotationMatrix();
  coldplatelocalRot->rotateZ(fColdPlateAngle);
  G4AffineTransform *coldplateaffine = new G4AffineTransform(coldplatelocalRot,*coldplatelocalPos);
  *coldplateaffine*=*assemAffine;  
  G4ThreeVector *coldplateglobalPos = new G4ThreeVector(coldplateaffine->NetTranslation());
  G4RotationMatrix *coldplateglobalRot= new G4RotationMatrix(coldplateaffine->NetRotation());
  fColdPlatePtr->Place(coldplateglobalPos, coldplateglobalRot, motherLogical);
  delete coldplatelocalPos;
  delete coldplatelocalRot;
  delete coldplateaffine;
  
  //I don't know how high the CMS Stand is above the cold plate, so the 4 mm of the z position is a guess.
  G4ThreeVector *cmslocalPos = new G4ThreeVector(0, 0, (3.175+3.175*3.0+4.0)*mm);
  G4RotationMatrix *cmslocalRot = new G4RotationMatrix();
  cmslocalRot->rotateZ(fCMSAngle);
  G4AffineTransform *cmsaffine = new G4AffineTransform(cmslocalRot,*cmslocalPos);
  *cmsaffine*=*assemAffine;  
  G4ThreeVector *cmsglobalPos = new G4ThreeVector(cmsaffine->NetTranslation());
  G4RotationMatrix *cmsglobalRot= new G4RotationMatrix(cmsaffine->NetRotation());
  fCMSPtr->Place(cmsglobalPos, cmsglobalRot, motherLogical);
  delete cmslocalPos;
  delete cmslocalRot;
  delete cmsaffine;
  
  G4ThreeVector *mainbodylocalPos = new G4ThreeVector(0, 0, 10.16*mm);
  G4RotationMatrix *mainbodylocalRot = new G4RotationMatrix();
  G4AffineTransform *mainbodyaffine = new G4AffineTransform(mainbodylocalRot,*mainbodylocalPos);
  *mainbodyaffine*=*assemAffine;  
  G4ThreeVector *mainbodyglobalPos = new G4ThreeVector(mainbodyaffine->NetTranslation());
  G4RotationMatrix *mainbodyglobalRot= new G4RotationMatrix(mainbodyaffine->NetRotation());
  fMainBodyPtr->Place(mainbodyglobalPos, mainbodyglobalRot, motherLogical);
  delete mainbodylocalPos;
  delete mainbodylocalRot;
  delete mainbodyaffine;  
  
  G4ThreeVector *dipsticklocalPos = new G4ThreeVector(0, 0, (-318.262+10.16)*mm-eps);
  G4RotationMatrix *dipsticklocalRot = new G4RotationMatrix();
  G4AffineTransform *dipstickaffine = new G4AffineTransform(dipsticklocalRot,*dipsticklocalPos);
  *dipstickaffine*=*assemAffine;  
  G4ThreeVector *dipstickglobalPos = new G4ThreeVector(dipstickaffine->NetTranslation());
  G4RotationMatrix *dipstickglobalRot= new G4RotationMatrix(dipstickaffine->NetRotation());
  fDipstickPtr->Place(dipstickglobalPos, dipstickglobalRot, motherLogical);
  delete dipsticklocalPos;
  delete dipsticklocalRot;
  delete dipstickaffine;  

  //I'm not exactly sure how high the STC sits above the dewar so the 20 mm is a guess
  G4ThreeVector *dewarlocalPos = new G4ThreeVector(0, 0, (-318.262+10.16-22.098-20.0)*mm-eps);
  G4RotationMatrix *dewarlocalRot = new G4RotationMatrix();
  G4AffineTransform *dewaraffine = new G4AffineTransform(dewarlocalRot,*dewarlocalPos);
  *dewaraffine*=*assemAffine;  
  G4ThreeVector *dewarglobalPos = new G4ThreeVector(dewaraffine->NetTranslation());
  G4RotationMatrix *dewarglobalRot= new G4RotationMatrix(dewaraffine->NetRotation());
  fDewarPtr->Place(dewarglobalPos, dewarglobalRot, motherLogical);
  delete dewarlocalPos;
  delete dewarlocalRot;
  delete dewaraffine;
    
  delete assemAffine;  
}
