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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonInsertionSubAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *Assembly origin:  Co-axial with thermosyphon tube, front face of cold plate adapter
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@gmail.com
 * FIRST SUBMISSION: July 30, 2014
 *
 * REVISION:
 *
 * 07-30-2014, Created, M. Green
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
#include "mjdemonstrator/MJDemoThermosyphonInsertionSubAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonWeldmentAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonSupportThickAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonSupportThinAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonCableThermalShuntAssembly.hh"
#include "mjdemonstrator/MJDemoClampPlateSignalConnector.hh"
#include "mjdemonstrator/MJDemoBulkheadCableClamp.hh"
#include "mjdemonstrator/MJDemo832Screw.hh"
#include "mjdemonstrator/MJDemoConnectorandClampSubAssembly.hh"
#include "mjdemonstrator/MJDemoTSShieldBlock.hh"
#include "mjdemonstrator/MJDemoCubeShieldPlates.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonInsertionSubAssembly::MJDemoThermosyphonInsertionSubAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "MJ80-11-119-R000")
{
  fWeldmentPtr = new MJDemoThermosyphonWeldmentAssembly(fName+"_ThermosyphonWeldment_001", "867-5309");
  fThickSupportPtr = new MJDemoThermosyphonSupportThickAssembly(fName+"_ThickSupport_001", "867-5309");
  fThinSupportPtr = new MJDemoThermosyphonSupportThinAssembly(fName+"_ThinSupport_001", "867-5309");
  fShunt01Ptr = new MJDemoThermosyphonCableThermalShuntAssembly(fName+"_ThermalShunt_001", "867-5309");
  fShunt02Ptr = new MJDemoThermosyphonCableThermalShuntAssembly(fName+"_ThermalShunt_002", "867-5309");
  fPlatePtr = new MJDemoClampPlateSignalConnector(fName+"_ClampPlate_001", "867-5309");
  fCableClamp01Ptr = new MJDemoBulkheadCableClamp(fName+"_CableClamp_001", "867-5309");
  fCableClamp02Ptr = new MJDemoBulkheadCableClamp(fName+"_CableClamp_002", "867-5309");
  fScrew01Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_001", "867-5309", 6.35);
  fScrew02Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_002", "867-5309", 6.35);
  fScrew03Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_003", "867-5309", 6.35);
  fScrew04Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_004", "867-5309", 6.35);
  fConnectors01Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_001", "867-5309");
  fConnectors02Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_002", "867-5309");
  fConnectors03Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_003", "867-5309");
  fConnectors04Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_004", "867-5309");
  fConnectors05Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_005", "867-5309");
  fConnectors06Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_006", "867-5309");
  fConnectors07Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_007", "867-5309");
  fConnectors08Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_008", "867-5309");
  if(fSerialNumber.contains("TSShield")) {
    fShieldBlock01Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_001", "Copper-EF");
    fShieldBlock02Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_002", "Copper-EF");
    fShieldBlock03Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_003", "Copper-EF");
    fShieldBlock04Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_004", "Copper-EF");
  }
  else {
    fShieldBlock01Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_001", "Vacuum");
    fShieldBlock02Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_002", "Vacuum");
    fShieldBlock03Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_003", "Vacuum");
    fShieldBlock04Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_004", "Vacuum");
  }
  fCubeShieldPlatesPtr = new MJDemoCubeShieldPlates(fName+"_CubeShieldPlates_001", "867-5309");
}

MJDemoThermosyphonInsertionSubAssembly::MJDemoThermosyphonInsertionSubAssembly(const MJDemoThermosyphonInsertionSubAssembly & rhs) :
  MJVDemoAssembly(rhs)
{
  fWeldmentPtr = new MJDemoThermosyphonWeldmentAssembly(fName+"_ThermosyphonWeldment_001", "867-5309");
  fThickSupportPtr = new MJDemoThermosyphonSupportThickAssembly(fName+"_ThickSupport_001", "867-5309");
  fThinSupportPtr = new MJDemoThermosyphonSupportThinAssembly(fName+"_ThinSupport_001", "867-5309");
  fShunt01Ptr = new MJDemoThermosyphonCableThermalShuntAssembly(fName+"_ThermalShunt_001", "867-5309");
  fShunt02Ptr = new MJDemoThermosyphonCableThermalShuntAssembly(fName+"_ThermalShunt_002", "867-5309");
  fPlatePtr = new MJDemoClampPlateSignalConnector(fName+"_ClampPlate_001", "867-5309");
  fCableClamp01Ptr = new MJDemoBulkheadCableClamp(fName+"_CableClamp_001", "867-5309");
  fCableClamp02Ptr = new MJDemoBulkheadCableClamp(fName+"_CableClamp_002", "867-5309");
  fScrew01Ptr = new MJDemo832Screw(fName+"_8_32x0.5Screw_001", "867-5309", 12.70);
  fScrew02Ptr = new MJDemo832Screw(fName+"_8_32x0.5Screw_002", "867-5309", 12.70);
  fScrew03Ptr = new MJDemo832Screw(fName+"_8_32x0.5Screw_003", "867-5309", 12.70);
  fScrew04Ptr = new MJDemo832Screw(fName+"_8_32x0.5Screw_004", "867-5309", 12.70);
  fConnectors01Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_001", "867-5309");
  fConnectors02Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_002", "867-5309");
  fConnectors03Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_003", "867-5309");
  fConnectors04Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_004", "867-5309");
  fConnectors05Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_005", "867-5309");
  fConnectors06Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_006", "867-5309");
  fConnectors07Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_007", "867-5309");
  fConnectors08Ptr = new MJDemoConnectorandClampSubAssembly(fName+"_Connectors_008", "867-5309");
  if(fSerialNumber.contains("TSShield")) {
    fShieldBlock01Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_001", "Copper-EF");
    fShieldBlock02Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_002", "Copper-EF");
    fShieldBlock03Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_003", "Copper-EF");
    fShieldBlock04Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_004", "Copper-EF");
  }
  else {
    fShieldBlock01Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_001", "Vacuum");
    fShieldBlock02Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_002", "Vacuum");
    fShieldBlock03Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_003", "Vacuum");
    fShieldBlock04Ptr = new MJDemoTSShieldBlock(fName+"_ShieldBlock_004", "Vacuum");
  }
  fCubeShieldPlatesPtr = new MJDemoCubeShieldPlates(fName+"_CubeShieldPlates_001", "867-5309");
}

MJDemoThermosyphonInsertionSubAssembly::~MJDemoThermosyphonInsertionSubAssembly()
{
    delete fWeldmentPtr;
    delete fThickSupportPtr;
    delete fThinSupportPtr;
    delete fShunt01Ptr;
    delete fShunt02Ptr;
    delete fPlatePtr;
    delete fCableClamp01Ptr;
    delete fCableClamp02Ptr;
    delete fScrew01Ptr;
    delete fScrew02Ptr;
    delete fScrew03Ptr;
    delete fScrew04Ptr;
    delete fConnectors01Ptr;
    delete fConnectors02Ptr;
    delete fConnectors03Ptr;
    delete fConnectors04Ptr;
    delete fConnectors05Ptr;
    delete fConnectors06Ptr;
    delete fConnectors07Ptr;
    delete fConnectors08Ptr;
    delete fShieldBlock01Ptr;
    delete fShieldBlock02Ptr;
    delete fShieldBlock03Ptr;
    delete fShieldBlock04Ptr;
    delete fCubeShieldPlatesPtr;
}

void MJDemoThermosyphonInsertionSubAssembly::Place(G4ThreeVector* assemPosition,
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  G4double eps = 0.01*mm;

  G4ThreeVector *WeldmentlocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *WeldmentlocalRot = new G4RotationMatrix();
  WeldmentlocalRot->rotateX(0);
  WeldmentlocalRot->rotateY(0);
  G4AffineTransform *Weldmentaffine = new G4AffineTransform(WeldmentlocalRot,*WeldmentlocalPos);
  *Weldmentaffine*=*assemAffine;
  G4ThreeVector *WeldmentglobalPos = new G4ThreeVector(Weldmentaffine->NetTranslation());
  G4RotationMatrix *WeldmentglobalRot= new G4RotationMatrix(Weldmentaffine->NetRotation());
  fWeldmentPtr->Place(WeldmentglobalPos, WeldmentglobalRot, motherLogical);
  delete WeldmentlocalPos;
  delete WeldmentlocalRot;
  delete Weldmentaffine;

  G4ThreeVector *ThickSupportlocalPos = new G4ThreeVector(-803.364-3.2, 0, 0);
  G4RotationMatrix *ThickSupportlocalRot = new G4RotationMatrix();
  ThickSupportlocalRot->rotateX(0);
  ThickSupportlocalRot->rotateY(pi/2);
  G4AffineTransform *ThickSupportaffine = new G4AffineTransform(ThickSupportlocalRot,*ThickSupportlocalPos);
  *ThickSupportaffine*=*assemAffine;
  G4ThreeVector *ThickSupportglobalPos = new G4ThreeVector(ThickSupportaffine->NetTranslation());
  G4RotationMatrix *ThickSupportglobalRot= new G4RotationMatrix(ThickSupportaffine->NetRotation());
  fThickSupportPtr->Place(ThickSupportglobalPos, ThickSupportglobalRot, motherLogical);
  delete ThickSupportlocalPos;
  delete ThickSupportlocalRot;
  delete ThickSupportaffine;

  G4ThreeVector *ThinSupportlocalPos = new G4ThreeVector(-195.021-1.2, 0, 0);
  G4RotationMatrix *ThinSupportlocalRot = new G4RotationMatrix();
  ThinSupportlocalRot->rotateX(0);
  ThinSupportlocalRot->rotateY(pi/2);
  G4AffineTransform *ThinSupportaffine = new G4AffineTransform(ThinSupportlocalRot,*ThinSupportlocalPos);
  *ThinSupportaffine*=*assemAffine;
  G4ThreeVector *ThinSupportglobalPos = new G4ThreeVector(ThinSupportaffine->NetTranslation());
  G4RotationMatrix *ThinSupportglobalRot= new G4RotationMatrix(ThinSupportaffine->NetRotation());
  fThinSupportPtr->Place(ThinSupportglobalPos, ThinSupportglobalRot, motherLogical);
  delete ThinSupportlocalPos;
  delete ThinSupportlocalRot;
  delete ThinSupportaffine;

  G4ThreeVector *Shunt01localPos = new G4ThreeVector(-423.24, 0, 0);
  G4RotationMatrix *Shunt01localRot = new G4RotationMatrix();
  Shunt01localRot->rotateX(0);
  Shunt01localRot->rotateY(pi/2);
  G4AffineTransform *Shunt01affine = new G4AffineTransform(Shunt01localRot,*Shunt01localPos);
  *Shunt01affine*=*assemAffine;
  G4ThreeVector *Shunt01globalPos = new G4ThreeVector(Shunt01affine->NetTranslation());
  G4RotationMatrix *Shunt01globalRot= new G4RotationMatrix(Shunt01affine->NetRotation());
  fShunt01Ptr->Place(Shunt01globalPos, Shunt01globalRot, motherLogical);
  delete Shunt01localPos;
  delete Shunt01localRot;
  delete Shunt01affine;

  G4ThreeVector *Shunt02localPos = new G4ThreeVector(-649.313, 0, 0);
  G4RotationMatrix *Shunt02localRot = new G4RotationMatrix();
  Shunt02localRot->rotateX(0);
  Shunt02localRot->rotateY(pi/2);
  G4AffineTransform *Shunt02affine = new G4AffineTransform(Shunt02localRot,*Shunt02localPos);
  *Shunt02affine*=*assemAffine;
  G4ThreeVector *Shunt02globalPos = new G4ThreeVector(Shunt02affine->NetTranslation());
  G4RotationMatrix *Shunt02globalRot= new G4RotationMatrix(Shunt02affine->NetRotation());
  fShunt02Ptr->Place(Shunt02globalPos, Shunt02globalRot, motherLogical);
  delete Shunt02localPos;
  delete Shunt02localRot;
  delete Shunt02affine;

  G4ThreeVector *PlatelocalPos = new G4ThreeVector((166.751-295.275/2)*mm, 0, (38.1/2-2.286+10.16/2)*mm+eps);
  G4RotationMatrix *PlatelocalRot = new G4RotationMatrix();
  PlatelocalRot->rotateZ(-pi/2);
  PlatelocalRot->rotateX(-pi/2);
  G4AffineTransform *Plateaffine = new G4AffineTransform(PlatelocalRot,*PlatelocalPos);
  *Plateaffine*=*assemAffine;
  G4ThreeVector *PlateglobalPos = new G4ThreeVector(Plateaffine->NetTranslation());
  G4RotationMatrix *PlateglobalRot= new G4RotationMatrix(Plateaffine->NetRotation());
  fPlatePtr->Place(PlateglobalPos, PlateglobalRot, motherLogical);
  delete PlatelocalPos;
  delete PlatelocalRot;
  delete Plateaffine;

  G4ThreeVector *CableClamp01localPos = new G4ThreeVector((-76.187-9.525/2)*mm, 0, (38.1/2-2.286+10.16+6.35/2)*mm+2*eps);
  G4RotationMatrix *CableClamp01localRot = new G4RotationMatrix();
  CableClamp01localRot->rotateZ(pi);
  CableClamp01localRot->rotateX(pi/2);
  G4AffineTransform *CableClamp01affine = new G4AffineTransform(CableClamp01localRot,*CableClamp01localPos);
  *CableClamp01affine*=*assemAffine;
  G4ThreeVector *CableClamp01globalPos = new G4ThreeVector(CableClamp01affine->NetTranslation());
  G4RotationMatrix *CableClamp01globalRot= new G4RotationMatrix(CableClamp01affine->NetRotation());
  fCableClamp01Ptr->Place(CableClamp01globalPos, CableClamp01globalRot, motherLogical);
  delete CableClamp01localPos;
  delete CableClamp01localRot;
  delete CableClamp01affine;

  G4ThreeVector *CableClamp02localPos = new G4ThreeVector((-99.682-9.525/2)*mm, 0, (38.1/2-2.286+10.16+6.35/2)*mm+2*eps);
  G4RotationMatrix *CableClamp02localRot = new G4RotationMatrix();
  CableClamp02localRot->rotateZ(pi);
  CableClamp02localRot->rotateX(pi/2);
  G4AffineTransform *CableClamp02affine = new G4AffineTransform(CableClamp02localRot,*CableClamp02localPos);
  *CableClamp02affine*=*assemAffine;
  G4ThreeVector *CableClamp02globalPos = new G4ThreeVector(CableClamp02affine->NetTranslation());
  G4RotationMatrix *CableClamp02globalRot= new G4RotationMatrix(CableClamp02affine->NetRotation());
  fCableClamp02Ptr->Place(CableClamp02globalPos, CableClamp02globalRot, motherLogical);
  delete CableClamp02localPos;
  delete CableClamp02localRot;
  delete CableClamp02affine;

  G4ThreeVector *Screw01localPos = new G4ThreeVector((-76.187-9.525/2)*mm, 12.87*mm, (38.1/2-2.286+10.16+6.35)*mm+3*eps);
  G4RotationMatrix *Screw01localRot = new G4RotationMatrix();
  Screw01localRot->rotateZ(0);
  Screw01localRot->rotateX(0);
  G4AffineTransform *Screw01affine = new G4AffineTransform(Screw01localRot,*Screw01localPos);
  *Screw01affine*=*assemAffine;
  G4ThreeVector *Screw01globalPos = new G4ThreeVector(Screw01affine->NetTranslation());
  G4RotationMatrix *Screw01globalRot= new G4RotationMatrix(Screw01affine->NetRotation());
  fScrew01Ptr->Place(Screw01globalPos, Screw01globalRot, motherLogical);
  delete Screw01localPos;
  delete Screw01localRot;
  delete Screw01affine;

  G4ThreeVector *Screw02localPos = new G4ThreeVector((-76.187-9.525/2)*mm, -12.87*mm, (38.1/2-2.286+10.16+6.35)*mm+3*eps);
  G4RotationMatrix *Screw02localRot = new G4RotationMatrix();
  Screw02localRot->rotateZ(0);
  Screw02localRot->rotateX(0);
  G4AffineTransform *Screw02affine = new G4AffineTransform(Screw02localRot,*Screw02localPos);
  *Screw02affine*=*assemAffine;
  G4ThreeVector *Screw02globalPos = new G4ThreeVector(Screw02affine->NetTranslation());
  G4RotationMatrix *Screw02globalRot= new G4RotationMatrix(Screw02affine->NetRotation());
  fScrew02Ptr->Place(Screw02globalPos, Screw02globalRot, motherLogical);
  delete Screw02localPos;
  delete Screw02localRot;
  delete Screw02affine;


  G4ThreeVector *Screw03localPos = new G4ThreeVector((-99.682-9.525/2)*mm, 12.87*mm, (38.1/2-2.286+10.16+6.35)*mm+3*eps);
  G4RotationMatrix *Screw03localRot = new G4RotationMatrix();
  Screw03localRot->rotateZ(0);
  Screw03localRot->rotateX(0);
  G4AffineTransform *Screw03affine = new G4AffineTransform(Screw03localRot,*Screw03localPos);
  *Screw03affine*=*assemAffine;
  G4ThreeVector *Screw03globalPos = new G4ThreeVector(Screw03affine->NetTranslation());
  G4RotationMatrix *Screw03globalRot= new G4RotationMatrix(Screw03affine->NetRotation());
  fScrew03Ptr->Place(Screw03globalPos, Screw03globalRot, motherLogical);
  delete Screw03localPos;
  delete Screw03localRot;
  delete Screw03affine;

  G4ThreeVector *Screw04localPos = new G4ThreeVector((-99.682-9.525/2)*mm, -12.87*mm, (38.1/2-2.286+10.16+6.35)*mm+3*eps);
  G4RotationMatrix *Screw04localRot = new G4RotationMatrix();
  Screw04localRot->rotateZ(0);
  Screw04localRot->rotateX(0);
  G4AffineTransform *Screw04affine = new G4AffineTransform(Screw04localRot,*Screw04localPos);
  *Screw04affine*=*assemAffine;
  G4ThreeVector *Screw04globalPos = new G4ThreeVector(Screw04affine->NetTranslation());
  G4RotationMatrix *Screw04globalRot= new G4RotationMatrix(Screw04affine->NetRotation());
  fScrew04Ptr->Place(Screw04globalPos, Screw04globalRot, motherLogical);
  delete Screw04localPos;
  delete Screw04localRot;
  delete Screw04affine;

  G4ThreeVector *Connectors01localPos = new G4ThreeVector(-67.462*mm, 15.8765*mm, (38.1/2-2.286+10.16+0.05)*mm);
  G4RotationMatrix *Connectors01localRot = new G4RotationMatrix();
  Connectors01localRot->rotateZ(0);
  Connectors01localRot->rotateX(0);
  G4AffineTransform *Connectors01affine = new G4AffineTransform(Connectors01localRot,*Connectors01localPos);
  *Connectors01affine*=*assemAffine;
  G4ThreeVector *Connectors01globalPos = new G4ThreeVector(Connectors01affine->NetTranslation());
  G4RotationMatrix *Connectors01globalRot= new G4RotationMatrix(Connectors01affine->NetRotation());
  fConnectors01Ptr->Place(Connectors01globalPos, Connectors01globalRot, motherLogical);
  delete Connectors01localPos;
  delete Connectors01localRot;
  delete Connectors01affine;

  G4ThreeVector *Connectors02localPos = new G4ThreeVector(2.388*mm, 15.8765*mm, (38.1/2-2.286+10.16+0.05)*mm);
  G4RotationMatrix *Connectors02localRot = new G4RotationMatrix();
  Connectors02localRot->rotateZ(0);
  Connectors02localRot->rotateX(0);
  G4AffineTransform *Connectors02affine = new G4AffineTransform(Connectors02localRot,*Connectors02localPos);
  *Connectors02affine*=*assemAffine;
  G4ThreeVector *Connectors02globalPos = new G4ThreeVector(Connectors02affine->NetTranslation());
  G4RotationMatrix *Connectors02globalRot= new G4RotationMatrix(Connectors02affine->NetRotation());
  fConnectors02Ptr->Place(Connectors02globalPos, Connectors02globalRot, motherLogical);
  delete Connectors02localPos;
  delete Connectors02localRot;
  delete Connectors02affine;

  G4ThreeVector *Connectors03localPos = new G4ThreeVector(57.963*mm, 15.8765*mm, (38.1/2-2.286+10.16+0.05)*mm);
  G4RotationMatrix *Connectors03localRot = new G4RotationMatrix();
  Connectors03localRot->rotateZ(0);
  Connectors03localRot->rotateX(0);
  G4AffineTransform *Connectors03affine = new G4AffineTransform(Connectors03localRot,*Connectors03localPos);
  *Connectors03affine*=*assemAffine;
  G4ThreeVector *Connectors03globalPos = new G4ThreeVector(Connectors03affine->NetTranslation());
  G4RotationMatrix *Connectors03globalRot= new G4RotationMatrix(Connectors03affine->NetRotation());
  fConnectors03Ptr->Place(Connectors03globalPos, Connectors03globalRot, motherLogical);
  delete Connectors03localPos;
  delete Connectors03localRot;
  delete Connectors03affine;

  G4ThreeVector *Connectors04localPos = new G4ThreeVector(113.538*mm, 15.8765*mm, (38.1/2-2.286+10.16+0.05)*mm);
  G4RotationMatrix *Connectors04localRot = new G4RotationMatrix();
  Connectors04localRot->rotateZ(0);
  Connectors04localRot->rotateX(0);
  G4AffineTransform *Connectors04affine = new G4AffineTransform(Connectors04localRot,*Connectors04localPos);
  *Connectors04affine*=*assemAffine;
  G4ThreeVector *Connectors04globalPos = new G4ThreeVector(Connectors04affine->NetTranslation());
  G4RotationMatrix *Connectors04globalRot= new G4RotationMatrix(Connectors04affine->NetRotation());
  fConnectors04Ptr->Place(Connectors04globalPos, Connectors04globalRot, motherLogical);
  delete Connectors04localPos;
  delete Connectors04localRot;
  delete Connectors04affine;

  G4ThreeVector *Connectors05localPos = new G4ThreeVector(-21.438*mm, -15.8765*mm, (38.1/2-2.286+10.16+0.05)*mm);
  G4RotationMatrix *Connectors05localRot = new G4RotationMatrix();
  Connectors05localRot->rotateZ(pi);
  Connectors05localRot->rotateX(0);
  G4AffineTransform *Connectors05affine = new G4AffineTransform(Connectors05localRot,*Connectors05localPos);
  *Connectors05affine*=*assemAffine;
  G4ThreeVector *Connectors05globalPos = new G4ThreeVector(Connectors05affine->NetTranslation());
  G4RotationMatrix *Connectors05globalRot= new G4RotationMatrix(Connectors05affine->NetRotation());
  fConnectors05Ptr->Place(Connectors05globalPos, Connectors05globalRot, motherLogical);
  delete Connectors05localPos;
  delete Connectors05localRot;
  delete Connectors05affine;

  G4ThreeVector *Connectors06localPos = new G4ThreeVector(48.412*mm, -15.8765*mm, (38.1/2-2.286+10.16+0.05)*mm);
  G4RotationMatrix *Connectors06localRot = new G4RotationMatrix();
  Connectors06localRot->rotateZ(pi);
  Connectors06localRot->rotateX(0);
  G4AffineTransform *Connectors06affine = new G4AffineTransform(Connectors06localRot,*Connectors06localPos);
  *Connectors06affine*=*assemAffine;
  G4ThreeVector *Connectors06globalPos = new G4ThreeVector(Connectors06affine->NetTranslation());
  G4RotationMatrix *Connectors06globalRot= new G4RotationMatrix(Connectors06affine->NetRotation());
  fConnectors06Ptr->Place(Connectors06globalPos, Connectors06globalRot, motherLogical);
  delete Connectors06localPos;
  delete Connectors06localRot;
  delete Connectors06affine;

  G4ThreeVector *Connectors07localPos = new G4ThreeVector(103.988*mm, -15.8765*mm, (38.1/2-2.286+10.16+0.05)*mm);
  G4RotationMatrix *Connectors07localRot = new G4RotationMatrix();
  Connectors07localRot->rotateZ(pi);
  Connectors07localRot->rotateX(0);
  G4AffineTransform *Connectors07affine = new G4AffineTransform(Connectors07localRot,*Connectors07localPos);
  *Connectors07affine*=*assemAffine;
  G4ThreeVector *Connectors07globalPos = new G4ThreeVector(Connectors07affine->NetTranslation());
  G4RotationMatrix *Connectors07globalRot= new G4RotationMatrix(Connectors07affine->NetRotation());
  fConnectors07Ptr->Place(Connectors07globalPos, Connectors07globalRot, motherLogical);
  delete Connectors07localPos;
  delete Connectors07localRot;
  delete Connectors07affine;

  G4ThreeVector *Connectors08localPos = new G4ThreeVector(159.563*mm, -15.8765*mm, (38.1/2-2.286+10.16+0.05)*mm);
  G4RotationMatrix *Connectors08localRot = new G4RotationMatrix();
  Connectors08localRot->rotateZ(pi);
  Connectors08localRot->rotateX(0);
  G4AffineTransform *Connectors08affine = new G4AffineTransform(Connectors08localRot,*Connectors08localPos);
  *Connectors08affine*=*assemAffine;
  G4ThreeVector *Connectors08globalPos = new G4ThreeVector(Connectors08affine->NetTranslation());
  G4RotationMatrix *Connectors08globalRot= new G4RotationMatrix(Connectors08affine->NetRotation());
  fConnectors08Ptr->Place(Connectors08globalPos, Connectors08globalRot, motherLogical);
  delete Connectors08localPos;
  delete Connectors08localRot;
  delete Connectors08affine;

  G4ThreeVector *ShieldBlock01localPos = new G4ThreeVector(-250*mm, 0, 0);
  G4RotationMatrix *ShieldBlock01localRot = new G4RotationMatrix();
  ShieldBlock01localRot->rotateX(pi);
  ShieldBlock01localRot->rotateY(-pi/2);
  ShieldBlock01localRot->rotateZ(pi/2);
  G4AffineTransform *ShieldBlock01affine = new G4AffineTransform(ShieldBlock01localRot,*ShieldBlock01localPos);
  *ShieldBlock01affine*=*assemAffine;
  G4ThreeVector *ShieldBlock01globalPos = new G4ThreeVector(ShieldBlock01affine->NetTranslation());
  G4RotationMatrix *ShieldBlock01globalRot= new G4RotationMatrix(ShieldBlock01affine->NetRotation());
  fShieldBlock01Ptr->Place(ShieldBlock01globalPos, ShieldBlock01globalRot, motherLogical);
  delete ShieldBlock01localPos;
  delete ShieldBlock01localRot;
  delete ShieldBlock01affine;

  G4ThreeVector *ShieldBlock02localPos = new G4ThreeVector(-465*mm, 0, 0);
  G4RotationMatrix *ShieldBlock02localRot = new G4RotationMatrix();
  ShieldBlock02localRot->rotateX(-pi/2);
  ShieldBlock02localRot->rotateY(-pi/2);
  ShieldBlock02localRot->rotateZ(pi/2);
  G4AffineTransform *ShieldBlock02affine = new G4AffineTransform(ShieldBlock02localRot,*ShieldBlock02localPos);
  *ShieldBlock02affine*=*assemAffine;
  G4ThreeVector *ShieldBlock02globalPos = new G4ThreeVector(ShieldBlock02affine->NetTranslation());
  G4RotationMatrix *ShieldBlock02globalRot= new G4RotationMatrix(ShieldBlock02affine->NetRotation());
  fShieldBlock02Ptr->Place(ShieldBlock02globalPos, ShieldBlock02globalRot, motherLogical);
  delete ShieldBlock02localPos;
  delete ShieldBlock02localRot;
  delete ShieldBlock02affine;

  G4ThreeVector *ShieldBlock03localPos = new G4ThreeVector(-600*mm, 0, 0);
  G4RotationMatrix *ShieldBlock03localRot = new G4RotationMatrix();
  ShieldBlock03localRot->rotateX(pi/2);
  ShieldBlock03localRot->rotateY(-pi/2);
  ShieldBlock03localRot->rotateZ(pi/2);
  G4AffineTransform *ShieldBlock03affine = new G4AffineTransform(ShieldBlock03localRot,*ShieldBlock03localPos);
  *ShieldBlock03affine*=*assemAffine;
  G4ThreeVector *ShieldBlock03globalPos = new G4ThreeVector(ShieldBlock03affine->NetTranslation());
  G4RotationMatrix *ShieldBlock03globalRot= new G4RotationMatrix(ShieldBlock03affine->NetRotation());
  fShieldBlock03Ptr->Place(ShieldBlock03globalPos, ShieldBlock03globalRot, motherLogical);
  delete ShieldBlock03localPos;
  delete ShieldBlock03localRot;
  delete ShieldBlock03affine;

  G4ThreeVector *ShieldBlock04localPos = new G4ThreeVector(-690*mm, 0, 0);
  G4RotationMatrix *ShieldBlock04localRot = new G4RotationMatrix();
  ShieldBlock04localRot->rotateX(0);
  ShieldBlock04localRot->rotateY(-pi/2);
  ShieldBlock04localRot->rotateZ(pi/2);
  G4AffineTransform *ShieldBlock04affine = new G4AffineTransform(ShieldBlock04localRot,*ShieldBlock04localPos);
  *ShieldBlock04affine*=*assemAffine;
  G4ThreeVector *ShieldBlock04globalPos = new G4ThreeVector(ShieldBlock04affine->NetTranslation());
  G4RotationMatrix *ShieldBlock04globalRot= new G4RotationMatrix(ShieldBlock04affine->NetRotation());
  fShieldBlock04Ptr->Place(ShieldBlock04globalPos, ShieldBlock04globalRot, motherLogical);
  delete ShieldBlock04localPos;
  delete ShieldBlock04localRot;
  delete ShieldBlock04affine;

  G4ThreeVector *CubeShieldlocalPos = new G4ThreeVector(-1009.7*mm, 0, 0);
  G4RotationMatrix *CubeShieldlocalRot = new G4RotationMatrix();
  CubeShieldlocalRot->rotateX(pi);
  CubeShieldlocalRot->rotateY(-pi/2);
  CubeShieldlocalRot->rotateZ(pi/2);
  G4AffineTransform *CubeShieldaffine = new G4AffineTransform(CubeShieldlocalRot,*CubeShieldlocalPos);
  *CubeShieldaffine*=*assemAffine;
  G4ThreeVector *CubeShieldglobalPos = new G4ThreeVector(CubeShieldaffine->NetTranslation());
  G4RotationMatrix *CubeShieldglobalRot= new G4RotationMatrix(CubeShieldaffine->NetRotation());
  fCubeShieldPlatesPtr->Place(CubeShieldglobalPos, CubeShieldglobalRot, motherLogical);
  delete CubeShieldlocalPos;
  delete CubeShieldlocalRot;
  delete CubeShieldaffine;

  delete assemAffine;
}
