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
// CLASS IMPLEMENTATION:  MJDemoCryostatAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *Assembly origin:  Coaxial with barrel, coincident with crossarm axis
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Sept 10, 2010
 *
 * REVISION:
 *
 * 09-10-2010, Created, M. Green
 * 10-24-2014, Reformed as macro assembly of all cryostat components - M. Green
 * 02-24-2016, Allow the cryostat to be offset from messenter, T. Caldwell
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCryostatAssembly.hh"
#include "mjdemonstrator/MJDemoVacuumVesselAssembly.hh"
#include "mjdemonstrator/MJDemoVacuumCFAssembly.hh"
#include "mjdemonstrator/MJDemoColdPlateAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonAndShieldAssembly.hh"
#include "mjdemonstrator/MJDemoCalibrationAssembly.hh"
#include "mjdemonstrator/MJDemo7StringAssembly.hh"


//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoCryostatAssembly::MJDemoCryostatAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "Cryostat")
{
  fVesselPtr = new MJDemoVacuumVesselAssembly(fName+"_VacuumVessel_001", "867-5309");
  fCFPtr = new MJDemoVacuumCFAssembly(fName+"_CFVacAssem_001", serialNumber);
  fCPPtr = new MJDemoColdPlateAssembly(fName+"_ColdPlateAssem_001", "867-5309");
  fTSPtr = new MJDemoThermosyphonAndShieldAssembly(fName+"_ThermosyphonAndShield_001", serialNumber);
  G4String newSerial = serialNumber.substr(0, serialNumber.find("TSShield"));
  fCalPtr = new MJDemoCalibrationAssembly(fName+"_CalAssembly_001", "867-5309");
  fStrPtr = new MJDemo7StringAssembly(fName+"_7StrAssembly_001", newSerial);
  fCalAssemblyPos = G4ThreeVector(0.0, 0.0, -110 * mm);
}

MJDemoCryostatAssembly::MJDemoCryostatAssembly(const MJDemoCryostatAssembly & rhs) :
  MJVDemoAssembly(rhs)
{
    fVesselPtr = new MJDemoVacuumVesselAssembly(fName+"_VacuumVessel_001", "867-5309");
    fCFPtr = new MJDemoVacuumCFAssembly(fName+"_CFVacAssem_001", rhs.GetConflat()->GetSerialNumber());
    fCPPtr = new MJDemoColdPlateAssembly(fName+"_ColdPlateAssem_001", "867-5309");
    fTSPtr = new MJDemoThermosyphonAndShieldAssembly(fName+"_ThermosyphonAndShield_001", rhs.GetTSandS()->GetSerialNumber());
    fCalPtr = new MJDemoCalibrationAssembly(fName+"_CalAssembly_001", "867-5309");
    fStrPtr = new MJDemo7StringAssembly(fName+"_7StringAssembly_001", rhs.Get7StringAssembly()->GetSerialNumber());
}

MJDemoCryostatAssembly::~MJDemoCryostatAssembly()
{
  delete fVesselPtr;
  delete fCFPtr;
  delete fCPPtr;
  delete fTSPtr;
  delete fCalPtr;
  delete fStrPtr;
}

void MJDemoCryostatAssembly::Place(G4ThreeVector* assemPosition,
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  G4double eps = 0.01*mm;

  G4ThreeVector *VessellocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *VessellocalRot = new G4RotationMatrix();
  G4AffineTransform *Vesselaffine = new G4AffineTransform(VessellocalRot,*VessellocalPos);
  *Vesselaffine*=*assemAffine;
  G4ThreeVector *VesselglobalPos = new G4ThreeVector(Vesselaffine->NetTranslation());
  G4RotationMatrix *VesselglobalRot= new G4RotationMatrix(Vesselaffine->NetRotation());
  fVesselPtr->Place(VesselglobalPos, VesselglobalRot, motherLogical);
  delete VessellocalPos;
  delete VessellocalRot;
  delete Vesselaffine;

  G4ThreeVector *CFlocalPos = new G4ThreeVector(42.125*25.4*mm, 0, 0);
  G4RotationMatrix *CFlocalRot = new G4RotationMatrix();
  CFlocalRot->rotateZ(pi);
  G4AffineTransform *CFaffine = new G4AffineTransform(CFlocalRot,*CFlocalPos);
  *CFaffine*=*assemAffine;
  G4ThreeVector *CFglobalPos = new G4ThreeVector(CFaffine->NetTranslation());
  G4RotationMatrix *CFglobalRot= new G4RotationMatrix(CFaffine->NetRotation());
  fCFPtr->Place(CFglobalPos, CFglobalRot, motherLogical);
  delete CFlocalPos;
  delete CFlocalRot;
  delete CFaffine;

  G4ThreeVector *CPlocalPos = new G4ThreeVector(0, 0, -1.05*25.4*mm-eps);
  G4RotationMatrix *CPlocalRot = new G4RotationMatrix();
  CPlocalRot->rotateZ(pi);
  G4AffineTransform *CPaffine = new G4AffineTransform(CPlocalRot,*CPlocalPos);
  *CPaffine*=*assemAffine;
  G4ThreeVector *CPglobalPos = new G4ThreeVector(CPaffine->NetTranslation());
  G4RotationMatrix *CPglobalRot= new G4RotationMatrix(CPaffine->NetRotation());
  fCPPtr->Place(CPglobalPos, CPglobalRot, motherLogical);
  delete CPlocalPos;
  delete CPlocalRot;
  delete CPaffine;

  G4ThreeVector *SAlocalPos = new G4ThreeVector(0, 0, -1.25*25.4*mm-eps);
  G4RotationMatrix *SAlocalRot = new G4RotationMatrix();
  SAlocalRot->rotateZ(pi);
  G4AffineTransform *SAaffine = new G4AffineTransform(SAlocalRot,*SAlocalPos);
  *SAaffine*=*assemAffine;
  G4ThreeVector *SAglobalPos = new G4ThreeVector(SAaffine->NetTranslation());
  G4RotationMatrix *SAglobalRot= new G4RotationMatrix(SAaffine->NetRotation());
  fStrPtr->Place(SAglobalPos, SAglobalRot, motherLogical);
  delete SAlocalPos;
  delete SAlocalRot;
  delete SAaffine;

  G4ThreeVector *TSlocalPos = new G4ThreeVector(0, 0, -0.85*25.4*mm);
  G4RotationMatrix *TSlocalRot = new G4RotationMatrix();
  TSlocalRot->rotateZ(pi);
  G4AffineTransform *TSaffine = new G4AffineTransform(TSlocalRot,*TSlocalPos);
  *TSaffine*=*assemAffine;
  G4ThreeVector *TSglobalPos = new G4ThreeVector(TSaffine->NetTranslation());
  G4RotationMatrix *TSglobalRot= new G4RotationMatrix(TSaffine->NetRotation());
  fTSPtr->Place(TSglobalPos, TSglobalRot, motherLogical);
  delete TSlocalPos;
  delete TSlocalRot;
  delete TSaffine;

  G4ThreeVector *CallocalPos = new G4ThreeVector(fCalAssemblyPos);
  G4RotationMatrix *CallocalRot = new G4RotationMatrix();
  G4AffineTransform *Calaffine = new G4AffineTransform(CallocalRot,*CallocalPos);
  *Calaffine*=*assemAffine;
  G4ThreeVector *CalglobalPos = new G4ThreeVector(Calaffine->NetTranslation());
  G4RotationMatrix *CalglobalRot= new G4RotationMatrix(Calaffine->NetRotation());
  fCalPtr->Place(CalglobalPos, CalglobalRot, motherLogical);
  delete CallocalPos;
  delete CallocalRot;
  delete Calaffine;


  delete assemAffine;
}
