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
// $Id: MJGeometryRDCrystalColumn.cc,v 1.8 2009-04-23 18:32:24 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryRDCrystalColumn.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.
 * CONTACT: Henning
 * FIRST SUBMISSION: Wed Nov  9 15:43:44 PST 2005
 * 
 * REVISION:
 *
 * 11-09-2005, Created, R. Henning.
 * 12-16-2005, Removed '=' operator. R. Henning
 * 12-28-2005, Corrected GeTray & Contact ring placement names to be 
 *             consistent w/ other placement names, R. Henning
 * 04-14-2009, Added SetDefaultsDEMONSTRATOR() method as an approx
 *
 */
//---------------------------------------------------------------------------//
//

#include <sstream>
#include <vector>

//#include "CLHEP/GenericFunctions/Cos.hh"
//#include "CLHEP/GenericFunctions/GenericFunctions.hh"
//#include "CLHEP/GenericFunctions/Sin.hh"
#include "TMath.h"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4VSolid.hh"

#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryIdealCoaxWithShield.hh"
#include "mjgeometry/MJGeometryRDCrystalColumnMessenger.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryRDCrystalColumn.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometryRDCrystalColumn::MJGeometryRDCrystalColumn():
  MGGeometryDetector(""),
  fStringerLidMaterial(0), fSupportRodMaterial(0), fContactPressureRingMaterial(0),
  fGeTrayMaterial(0), fLFEPAsmMaterial(0), 
  fTemplateCrystal(0), fCuObjectsVisAtt(0), fTeflonObjectsVisAtt(0), 
  fMotherVolumeVisAtt(0), fMessenger(0)
{;}

//---------------------------------------------------------------------------//

MJGeometryRDCrystalColumn::MJGeometryRDCrystalColumn(G4String serNum, G4bool messOff):
  MGGeometryDetector(serNum),
  fStringerLidMaterial(0), fSupportRodMaterial(0), fContactPressureRingMaterial(0),
  fGeTrayMaterial(0), fLFEPAsmMaterial(0), fTemplateCrystal(0), fCuObjectsVisAtt(0), 
  fTeflonObjectsVisAtt(0), fMotherVolumeVisAtt(0) 
{
  theDetectorLogical = 0;
  if(messOff)
    fMessenger = 0;
  else
    fMessenger = new MJGeometryRDCrystalColumnMessenger(this);
  fGeTrayZPositions.clear();
  fContactPressureRingsZPositions.clear();
}

//---------------------------------------------------------------------------//

// The assigment operator should be used only during the init phase of Geant 4
// Each MJGeometryRDCrystalColumn should be assigned a unique serNum and
// have their ConstructDetector() executed before use.

MJGeometryRDCrystalColumn::MJGeometryRDCrystalColumn(const MJGeometryRDCrystalColumn &rhs) :
  MGGeometryDetector(rhs.serialNumber),
  fStringerLidThickness(rhs.fStringerLidThickness),
  fStringerLidRadius(rhs.fStringerLidRadius),
  fStringerLidMaterialName(rhs.fStringerLidMaterialName),
  fSupportRodLength(rhs.fSupportRodLength),
  fSupportRodInnerRadius(rhs.fSupportRodInnerRadius),
  fSupportRodOuterRadius(rhs.fSupportRodOuterRadius),
  fSupportRodMaterialName(rhs.fSupportRodMaterialName),
  fContactPressureRingHeight(rhs.fContactPressureRingHeight),
  fContactPressureRingInnerRadius(rhs.fContactPressureRingInnerRadius),
  fContactPressureRingOuterRadius(rhs.fContactPressureRingOuterRadius),
  fContactPressureRingMaterialName(rhs.fContactPressureRingMaterialName),
  fGeTrayHeight(rhs.fGeTrayHeight),
  fGeTrayInnerRadius(rhs.fGeTrayInnerRadius),
  fGeTrayOuterRadius(rhs.fGeTrayOuterRadius),
  fGeTrayArmWidth(rhs.fGeTrayArmWidth),
  fGeTrayStubRadius(rhs.fGeTrayStubRadius),
  fGeTrayStubHeight(rhs.fGeTrayStubHeight),
  fGeTrayMaterialName(rhs.fGeTrayMaterialName),
  fLFEPAsmHeight(rhs.fLFEPAsmHeight),
  fLFEPAsmWidth(rhs.fLFEPAsmWidth),
  fLFEPAsmThickness(rhs.fLFEPAsmThickness),
  fLFEPAsmMaterialName(rhs.fLFEPAsmMaterialName),
  fOffset(rhs.fOffset),
  fIsOneOfMany(rhs.fIsOneOfMany),
  fTemplateCrystalColumnSerNum(rhs.fTemplateCrystalColumnSerNum),
  fSupportRodRadialPosition(rhs.fSupportRodRadialPosition),
  fColumnRotationZ(rhs.fColumnRotationZ), 
  fLFEPAsmRadialPosition(rhs.fLFEPAsmRadialPosition), 
  fLFEPAsmRotationZ(rhs.fLFEPAsmRotationZ), 
  fLFEPAsmSpacing(rhs.fLFEPAsmSpacing),
  fGeTrayZPositions(rhs.fGeTrayZPositions), 
  fContactPressureRingsZPositions(rhs.fContactPressureRingsZPositions),
  fEps(rhs.fEps),  
  fTemplateCrystal(rhs.fTemplateCrystal),

  // These pointers are unique to each instance and should not be used by 
  // another instance. 
  fCuObjectsVisAtt(0),
  fTeflonObjectsVisAtt(0),
  fMotherVolumeVisAtt(0), 
  fMessenger(0)
{;}

//---------------------------------------------------------------------------//

MJGeometryRDCrystalColumn::~MJGeometryRDCrystalColumn()
{
  delete fMessenger;
  delete fCuObjectsVisAtt;
  delete fTeflonObjectsVisAtt;
  delete fMotherVolumeVisAtt;
}

//---------------------------------------------------------------------------//

void MJGeometryRDCrystalColumn::ConstructDetector()
{
  /* 
     If crystal column is created stand-alone, then define the mother volume to be 2 disjointed,
     enclosing cylinders with 3 ridges on the bottom culinder containing the support tubes, 
     otherwise use a user-provided mother volume and offset.
     This functionality of placing the parts inside a user-provided volume would be
     used if the crystal column and supports acquire a shape that cannot be simply encapsulated. 
     However, it should be used with caution, since it would slow down tracking inside the
     cryostat user-provide mother volume if there are many crystal columns. 
     Note the stand-alone coordinate system's origin is that of the first cylinder, ie. the middle 
     of the support rods.
  */

  // This is slightly unnecessary, but porting over to TMath would otherwise
  // involve a large number of substitutions.  Here we essentially just alias.
  G4double (*myCos)(G4double) = TMath::Cos;
  G4double (*mySin)(G4double) = TMath::Sin;

  if(!theDetectorLogical) {
    fOffset.set(0, 0, -fSupportRodLength/2.0);
    Initialize();
    G4VSolid *motherCCSolid = new G4Tubs("motherCCSolid01" + serialNumber, 0,
    fContactPressureRingOuterRadius + fEps, 
				      (fSupportRodLength + fEps)/2.0, 0*deg, 360.0 * deg);
    G4Tubs *lidSolid = new G4Tubs("lidSolid" + serialNumber, 0*cm, fStringerLidRadius + fEps,
				  (3.1*fEps + fStringerLidThickness + fLFEPAsmHeight)/2., 
				  0, 360*deg);
    motherCCSolid = new G4UnionSolid("motherCCSolid02" + serialNumber, motherCCSolid, lidSolid,
					   G4Transform3D(G4RotationMatrix(), G4ThreeVector(0,0,
					   fSupportRodLength/2. + fStringerSupportThickness +
				   (fEps + fStringerLidThickness + fLFEPAsmHeight)/2.)));
    G4Tubs *ridgeSolid = new G4Tubs("ridgeSolid" + serialNumber, 0, fSupportRodOuterRadius + fEps,
				    (fSupportRodLength + fEps)/2., 0*deg, 360.0*deg);
    motherCCSolid = new G4UnionSolid("motherCCSolid03" + serialNumber, motherCCSolid, ridgeSolid, 
				     G4Transform3D(G4RotationMatrix(),
						   G4ThreeVector(mySin(fColumnRotationZ) * fSupportRodRadialPosition, 
								 myCos(fColumnRotationZ) * fSupportRodRadialPosition,
								 0))); 
    motherCCSolid = new G4UnionSolid("motherCCSolid04" + serialNumber, motherCCSolid, ridgeSolid, 
				     G4Transform3D(G4RotationMatrix(), 
						   G4ThreeVector(mySin(fColumnRotationZ + 120*deg)*fSupportRodRadialPosition, 
								 myCos(fColumnRotationZ + 120*deg)*fSupportRodRadialPosition,
								 0))); 
    motherCCSolid = new G4UnionSolid("motherCCSolid05" + serialNumber, motherCCSolid, ridgeSolid, 
				     G4Transform3D(G4RotationMatrix(), 
						   G4ThreeVector(mySin(fColumnRotationZ + 240*deg)*fSupportRodRadialPosition, 
								 myCos(fColumnRotationZ + 240*deg)*fSupportRodRadialPosition,
								 0))); 
    theDetectorLogical = new G4LogicalVolume(motherCCSolid, G4Material::GetMaterial("Vacuum"),
					     "motherCCLogical" + serialNumber);
    theDetectorLogical->SetVisAttributes(fMotherVolumeVisAtt);
  } else
    Initialize();

  /*
    If this crystal column is one of many, then we extract its component logical volumes
    out of the Geant4 volume store. If it's not, then the component logical volumes are
    created. 
    Note if this is one of many, then the template crystal column must be constructed before 
    this method is invoked, otherwise the volumes store will not have the right logical
    volumes.
  */

  size_t i; 
  stringstream   nameStream; 
  G4LogicalVolume *supportRodLogical = 0;
  G4LogicalVolume *contactPressureRingLogical = 0;
  G4LogicalVolume *GeTrayLogical = 0;
  G4LogicalVolume *LFEPAsmLogical = 0;
  G4LogicalVolume *stringerLidLogical = 0;
  G4LogicalVolumeStore *logicalVolumeStore = G4LogicalVolumeStore::GetInstance();

  if(fIsOneOfMany) {
    G4bool allLogicalsFound = false;
    for(i = 0; i < logicalVolumeStore->size() && !allLogicalsFound; i++) {
      if((*logicalVolumeStore)[i]->GetName() == G4String("supportRodLogical" + fTemplateCrystalColumnSerNum))
         supportRodLogical = (*logicalVolumeStore)[i];
      else if((*logicalVolumeStore)[i]->GetName() == G4String("contactPressureRingLogical" 
							      + fTemplateCrystalColumnSerNum))
         contactPressureRingLogical = (*logicalVolumeStore)[i];
      else if((*logicalVolumeStore)[i]->GetName() == G4String("GeTrayLogical"  + fTemplateCrystalColumnSerNum))
	GeTrayLogical = (*logicalVolumeStore)[i];
      else if((*logicalVolumeStore)[i]->GetName() == G4String("LFEPAsmLogical"  + fTemplateCrystalColumnSerNum))
	LFEPAsmLogical = (*logicalVolumeStore)[i];
      else if((*logicalVolumeStore)[i]->GetName() == G4String("stringerLidLogical"  + fTemplateCrystalColumnSerNum))
	stringerLidLogical = (*logicalVolumeStore)[i];
      allLogicalsFound = supportRodLogical && contactPressureRingLogical && GeTrayLogical && LFEPAsmLogical && 
	stringerLidLogical;
    }
    if(!allLogicalsFound) {
      MGLog(error) << "Error finding supposedly pre-existing logical volumes." 
		   << supportRodLogical << " " << contactPressureRingLogical <<  " "
		   << GeTrayLogical << " " << LFEPAsmLogical << " " << stringerLidLogical << endlog;
      MGLog(fatal) << endlog;
    }
  } else { // Not one of many ... create logical volumes & give unique names.

    // Create support rods.
    G4Tubs *supportRodSolid = new G4Tubs("supportRodSolid" + serialNumber, fSupportRodInnerRadius, 
					 fSupportRodOuterRadius, fSupportRodLength/2.,
					 0*deg, 360*deg);
    supportRodLogical = 
      new G4LogicalVolume(supportRodSolid, fSupportRodMaterial, "supportRodLogical" + serialNumber);
    supportRodLogical->SetVisAttributes(fCuObjectsVisAtt);

    // Create contact rings
    G4Tubs *contactPressureRingSolid = new G4Tubs("contactPressureRingSolid" + serialNumber, 
						  fContactPressureRingInnerRadius, 
						  fContactPressureRingOuterRadius, fContactPressureRingHeight/2.,
						  0*deg, 360*deg);
    contactPressureRingLogical = new G4LogicalVolume(contactPressureRingSolid, fContactPressureRingMaterial, 
						     "contactPressureRingLogical" + serialNumber);
    contactPressureRingLogical->SetVisAttributes(fTeflonObjectsVisAtt);

    // Create Ge Trays.
    G4Box *GeTrayArm = new G4Box("GeTrayArmSolid" + serialNumber, fGeTrayArmWidth/2., fGeTrayInnerRadius/2., 
				 fGeTrayHeight/2.);
    G4Tubs *GeTrayOuterRingSolid = new G4Tubs("GeTrayOuterRingSolid" + serialNumber, fGeTrayInnerRadius, 
					      fGeTrayOuterRadius, fGeTrayHeight/2., 0*deg, 360*deg);
    G4Tubs *GeTrayStubSolid = new G4Tubs("GeTrayOuterRingSolid" + serialNumber, 0., fGeTrayStubRadius, 
					 fGeTrayStubHeight/2., 0*deg, 360*deg);
    G4UnionSolid *GeTraySolid = new G4UnionSolid("GeTrayRingsSolid" + serialNumber, GeTrayOuterRingSolid, 
						 GeTrayStubSolid, G4Transform3D(G4RotationMatrix(),
						 G4ThreeVector(0,0,(fGeTrayStubHeight - fGeTrayHeight)/2.)));
    G4RotationMatrix   armRotation;
    G4ThreeVector armTranslation(mySin(0) * fGeTrayInnerRadius/2. + fEps, 
				 myCos(0) * fGeTrayInnerRadius/2. + fEps, 0);
    G4Transform3D transformArm(armRotation, armTranslation);
    GeTraySolid = new G4UnionSolid("GeTraySolid1Arm" + serialNumber, GeTraySolid, GeTrayArm, transformArm);
    armRotation.rotateZ(120*deg);
    armTranslation.set(mySin(-120*deg) * fGeTrayInnerRadius/2. + fEps, 
		       myCos(-120*deg) * fGeTrayInnerRadius/2. + fEps, 0);
    transformArm = G4Transform3D(armRotation, armTranslation);
    GeTraySolid = new G4UnionSolid("GeTraySolid2Arm" + serialNumber, GeTraySolid, GeTrayArm, transformArm);
    armRotation.rotateZ(120*deg);
    armTranslation.set(mySin(-240*deg) * fGeTrayInnerRadius/2. + fEps, 
		       myCos(-240*deg) * fGeTrayInnerRadius/2. + fEps, 0);
    transformArm = G4Transform3D(armRotation, armTranslation);
    GeTraySolid = new G4UnionSolid("GeTraySolid" + serialNumber, GeTraySolid, GeTrayArm, transformArm);
    GeTrayLogical = new G4LogicalVolume(GeTraySolid, fGeTrayMaterial, "GeTrayLogical" + serialNumber);
    GeTrayLogical->SetVisAttributes(fTeflonObjectsVisAtt);

    // Create stringer lid.
    G4Tubs *stringerLidSolid = new G4Tubs("stringerLidSolid", 0, fStringerLidRadius, 
						  fStringerLidThickness/2., 0, 360*deg);
    stringerLidLogical = new G4LogicalVolume(stringerLidSolid, fStringerLidMaterial,"stringerLidLogical"+serialNumber);
    stringerLidLogical->SetVisAttributes(fCuObjectsVisAtt);
						  
    // Create LFEP Assembly.
    G4Box *LFEPAsmSolid = new G4Box("LFEPAsmSolid", fLFEPAsmThickness/2., fLFEPAsmWidth/2., fLFEPAsmHeight/2.);
    LFEPAsmLogical = new G4LogicalVolume(LFEPAsmSolid, fLFEPAsmMaterial, "LFEPAsmLogical" + serialNumber);
    LFEPAsmLogical->SetVisAttributes(fTeflonObjectsVisAtt);
  }

  MGLog(trace) << "Placing support rods." << endlog;
  // Place Support Rods. 
  new G4PVPlacement(0, G4ThreeVector(mySin(fColumnRotationZ) * fSupportRodRadialPosition, 
				     myCos(fColumnRotationZ) * fSupportRodRadialPosition,
				     fSupportRodLength/2.) + fOffset, 
		    supportRodLogical, "SupportRod01" + serialNumber, 
		    theDetectorLogical, false, 0);
  new G4PVPlacement(0, G4ThreeVector(mySin(fColumnRotationZ + 120*deg) * fSupportRodRadialPosition, 
				     myCos(fColumnRotationZ + 120*deg) * fSupportRodRadialPosition,
				     fSupportRodLength/2.) + fOffset, 
		    supportRodLogical, "SupportRod02" + serialNumber, 
		    theDetectorLogical, false, 0);
  new G4PVPlacement(0, G4ThreeVector(mySin(fColumnRotationZ + 240*deg) * fSupportRodRadialPosition, 
				     myCos(fColumnRotationZ + 240*deg) * fSupportRodRadialPosition,
				     fSupportRodLength/2.) + fOffset, 
		    supportRodLogical, "SupportRod03" + serialNumber, 
		    theDetectorLogical, false, 0);
		 
  MGLog(trace) << "Placing contact rings." << endlog;
  // Place Contact Rings and Ge Trays
  G4RotationMatrix   GeTrayRotation;
  GeTrayRotation.rotateZ(fColumnRotationZ + 60*deg);
  for(i = 0; i < fNumberOfCrystals; i++) {
    nameStream.str("");
    nameStream << "ContactRing"  << i << serialNumber;
    new G4PVPlacement(0, fContactPressureRingPositions[i] + fOffset, contactPressureRingLogical, 
		      nameStream.str(), theDetectorLogical, false, 0);
    nameStream.str("");
    nameStream << "GeTray"  << i << serialNumber;
    new G4PVPlacement(G4Transform3D(GeTrayRotation, fGeTrayPositions[i] + G4ThreeVector(0,0,fGeTrayHeight/2.) + fOffset), 
		      GeTrayLogical, nameStream.str(), theDetectorLogical, false, 0);
  }

  MGLog(trace) << "Placing stringer lids." << endlog;
  // Place Stringer Lid
  new G4PVPlacement(0, G4ThreeVector(0,0,fSupportRodLength+fStringerLidThickness/2.+fStringerSupportThickness+fEps) + 
		    fOffset, stringerLidLogical, "StringerLid" + serialNumber, theDetectorLogical, false, 0);

  // Place LFEP assemblies.
    MGLog(trace) << "Placing LFEP Assemblies." << endlog;
    G4RotationMatrix  LFEPRotation;
    G4ThreeVector     LFEPTranslation(fLFEPAsmRadialPosition, 0, fSupportRodLength + fStringerSupportThickness +
				      fStringerLidThickness + fLFEPAsmHeight/2. + 1.5*fEps);
    LFEPTranslation += fOffset;
    G4ThreeVector     LFEPUnit(fLFEPAsmSpacing,0,0); 
    
    LFEPRotation.rotateZ(-fLFEPAsmRotationZ + fColumnRotationZ);
    G4Transform3D      LFEPTransform(LFEPRotation, LFEPRotation*(LFEPTranslation + LFEPUnit));
    new G4PVPlacement(LFEPTransform, LFEPAsmLogical, "LFEP01A" + serialNumber, theDetectorLogical, false, 0);
    LFEPTransform = G4Transform3D(LFEPRotation, LFEPRotation*(LFEPTranslation));
    new G4PVPlacement(LFEPTransform, LFEPAsmLogical, "LFEP01B" + serialNumber, theDetectorLogical, false, 0);
    LFEPTransform = G4Transform3D(LFEPRotation, LFEPRotation*(LFEPTranslation - LFEPUnit));
    new G4PVPlacement(LFEPTransform, LFEPAsmLogical, "LFEP01C" + serialNumber, theDetectorLogical, false, 0);
    
    LFEPRotation.rotateZ(120*deg);
    LFEPTransform = G4Transform3D(LFEPRotation, LFEPRotation*(LFEPTranslation + LFEPUnit));
    new G4PVPlacement(LFEPTransform, LFEPAsmLogical, "LFEP02A" + serialNumber, theDetectorLogical, false, 0);
    LFEPTransform = G4Transform3D(LFEPRotation, LFEPRotation*(LFEPTranslation));
    new G4PVPlacement(LFEPTransform, LFEPAsmLogical, "LFEP02B" + serialNumber, theDetectorLogical, false, 0);
    LFEPTransform = G4Transform3D(LFEPRotation, LFEPRotation*(LFEPTranslation - LFEPUnit));
    new G4PVPlacement(LFEPTransform, LFEPAsmLogical, "LFEP02C" + serialNumber, theDetectorLogical, false, 0);
    
    LFEPRotation.rotateZ(120*deg);
    LFEPTransform = G4Transform3D(LFEPRotation, LFEPRotation*(LFEPTranslation + LFEPUnit));
    new G4PVPlacement(LFEPTransform, LFEPAsmLogical, "LFEP03A" + serialNumber, theDetectorLogical, false, 0);
    LFEPTransform = G4Transform3D(LFEPRotation, LFEPRotation*(LFEPTranslation));
    new G4PVPlacement(LFEPTransform, LFEPAsmLogical, "LFEP03B" + serialNumber, theDetectorLogical, false, 0);
    LFEPTransform = G4Transform3D(LFEPRotation, LFEPRotation*(LFEPTranslation - LFEPUnit));
    new G4PVPlacement(LFEPTransform, LFEPAsmLogical, "LFEP03C" + serialNumber, theDetectorLogical, false, 0);
 

  // Create and place crystals.
  // --------------------------
  stringstream crystalNameStream;
  fCrystals.clear();
  for(i = 0; i < fGeTrayPositions.size(); i++){
    crystalNameStream.str("");
    crystalNameStream  << "Crystal" << i<< GetSerialNumber();
    fCrystals.push_back(new MJGeometryIdealCoaxWithShield(*fTemplateCrystal));
    fCrystals[i]->SetIsOneOfMany(true);
    fCrystals[i]->SetSerialNumber(crystalNameStream.str());
    fCrystals[i]->ConstructDetector();
    MGLog(routine) << "Placing crystal " << fCrystals[i]->GetSerialNumber() << " " 
		   << (fGeTrayPositions[i] + fOffset + 
		       G4ThreeVector(0,0,fGeTrayHeight+fCrystals[i]->GetAssemblyHeight()/2.+fEps))/cm << endlog; 
    new G4PVPlacement(0, fGeTrayPositions[i] + fOffset + 
		      G4ThreeVector(0, 0, fGeTrayHeight+fCrystals[i]->GetAssemblyHeight()/2. + fEps), 
		      fCrystals[i]->GetDetectorLogical(), crystalNameStream.str(), 
		      theDetectorLogical, false, 0);
  }
}

//---------------------------------------------------------------------------//

void MJGeometryRDCrystalColumn::Initialize()
{
  // Compute internal variables from user defined quantities and do consistency 
  // checks.

  size_t i;
  fNumberOfCrystals = fGeTrayZPositions.size();
  if(fNumberOfCrystals != fContactPressureRingsZPositions.size()) {
    MGLog(error) << "Number of crystals and crystal parts inconsistent!"
		 << " fNumberOfCrystals = " << fNumberOfCrystals
		 << " | fContactRingsZPositions.size() = " << fContactPressureRingsZPositions.size() 
		 << endlog;
    MGLog(fatal) << endlog;
  }
  fStringerLidMaterial = G4Material::GetMaterial(fStringerLidMaterialName);
  if(!fStringerLidMaterial) {
    MGLog(error) << "Material not found : " << fStringerLidMaterialName << endlog;
    MGLog(fatal) << endlog;
  }
  fSupportRodMaterial = G4Material::GetMaterial(fSupportRodMaterialName);
  if(!fSupportRodMaterial) {
    MGLog(error) << "Material not found : " << fSupportRodMaterialName << endlog;
    MGLog(fatal) << endlog;
  }
  fContactPressureRingMaterial = G4Material::GetMaterial(fContactPressureRingMaterialName);
  if(!fContactPressureRingMaterial) {
    MGLog(error) << "Material not found : " << fContactPressureRingMaterialName << endlog;
    MGLog(fatal) << endlog;
  }
  fGeTrayMaterial = G4Material::GetMaterial(fGeTrayMaterialName);
  if(!fGeTrayMaterial) {
    MGLog(error) << "Material not found : " << fGeTrayMaterialName << endlog;
    MGLog(fatal) << endlog;
  }
  fLFEPAsmMaterial = G4Material::GetMaterial(fLFEPAsmMaterialName);
  if(!fLFEPAsmMaterial) {
    MGLog(error) << "Material not found : " << fLFEPAsmMaterialName << endlog;
    MGLog(fatal) << endlog;
  }
  if(!fTemplateCrystal) {
    MGLog(error) << "No template crystal specified!" << endlog;
    MGLog(fatal) << endlog;
  }
  if(3*(fGeTrayHeight + fTemplateCrystal->GetAssemblyHeight()) > fSupportRodLength) { 
      fSupportRodLength = 3*(fGeTrayHeight + fTemplateCrystal->GetAssemblyHeight());    
      MGLog(error) << "Support Rods are not long enough to reach bottom tray!" << endlog;
//      MGLog(fatal) << endlog;
  }

  for(i = 0; i < fNumberOfCrystals; i++) {
    fGeTrayPositions.push_back(G4ThreeVector(0,0,fGeTrayZPositions[i]));
    fContactPressureRingPositions.push_back(G4ThreeVector(0,0,fContactPressureRingsZPositions[i]));
    fCrystalPositions.push_back(G4ThreeVector(0,0, fGeTrayZPositions[i] + fEps +
				(fGeTrayHeight + fTemplateCrystal->GetAssemblyHeight())/2.));
  }  

  if((fSupportRodRadialPosition-fSupportRodOuterRadius) < fGeTrayOuterRadius) {
    fSupportRodRadialPosition = fSupportRodOuterRadius + fGeTrayOuterRadius + fEps;
    MGLog(error) << "Rods too close to Ge Tray! Resetting fSupportRodRadialPosition to: "
		 << fSupportRodRadialPosition << endlog;
  }

  if((fSupportRodRadialPosition-fSupportRodOuterRadius) < fContactPressureRingOuterRadius) {
    fSupportRodRadialPosition = fSupportRodOuterRadius + fContactPressureRingOuterRadius + fEps;
    MGLog(error) << "Rods too close to Contact Pressure Ring! Resetting fSupportRodRadialPosition to: "
		 << fSupportRodRadialPosition << endlog;
  }
  fAssemblyHeight = fSupportRodLength + fStringerLidThickness + fStringerSupportThickness + fLFEPAsmHeight;
  fAssemblyRadius = fSupportRodOuterRadius + fSupportRodRadialPosition;

  fTeflonObjectsVisAtt = new G4VisAttributes(G4Colour(0.1, 0.9, 0.0));
  fTeflonObjectsVisAtt->SetVisibility(true);
  fTeflonObjectsVisAtt->SetForceSolid(true); 
  fCuObjectsVisAtt = new G4VisAttributes(G4Colour(0.8, 0.2, 0.0));
  fCuObjectsVisAtt->SetVisibility(true);
  fCuObjectsVisAtt->SetForceSolid(true);
  fMotherVolumeVisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
  fMotherVolumeVisAtt->SetVisibility(false);
  fMotherVolumeVisAtt->SetForceSolid(false);
}

//---------------------------------------------------------------------------//

void MJGeometryRDCrystalColumn::SetDefaults()
{
  // Reference Plan as of 11/9/2005:
  fEps = 0.001 * mm;
  fIsDEMO = false;
  fStringerLidThickness = 6.5 * mm;
  fStringerLidRadius = 33.5 * mm;
  fStringerLidMaterialName = "Copper-EF";
  fStringerSupportThickness = 9.5 * mm;
  fSupportRodLength = 255.5 * mm;
  fSupportRodInnerRadius = 1.4 * mm;
  fSupportRodOuterRadius = 1.6 * mm;
  fSupportRodMaterialName = "Copper-EF";
  fContactPressureRingHeight = 3.0 * mm;
  fContactPressureRingInnerRadius = 35.3 * mm;
  fContactPressureRingOuterRadius = 36.8 * mm;
  fContactPressureRingMaterialName = "Teflon";
  fGeTrayHeight = 3.0 * mm;
  fGeTrayInnerRadius = 29.2 * mm;
  fGeTrayOuterRadius = 33.3 * mm;
  fGeTrayArmWidth = 2.5 * mm;
  fGeTrayStubRadius = 7.9 / 2. * mm;
  fGeTrayStubHeight = 9.1 * mm;
  fGeTrayMaterialName = "Teflon";
  fLFEPAsmHeight = 16 * mm;
  fLFEPAsmWidth = 23 * mm;
  fLFEPAsmThickness = 0.4 * mm;
  fLFEPAsmMaterialName = "Teflon";
  fOffset = G4ThreeVector(0,0,0);
  fSupportRodRadialPosition = 37.7 * mm; 
  fColumnRotationZ = 0.0 * deg;
  fLFEPAsmRotationZ = 60.0 * deg;
  fLFEPAsmSpacing = 1.0 * mm;
  fLFEPAsmRadialPosition = 25.0 * mm;
  fGeTrayZPositions.clear();
  fGeTrayZPositions.push_back(170.0 * mm);
  fGeTrayZPositions.push_back(85.0 * mm);
  fGeTrayZPositions.push_back(0.0 * mm);
  fContactPressureRingsZPositions.clear();
  fContactPressureRingsZPositions.push_back(198.0 * mm);
  fContactPressureRingsZPositions.push_back(113.0 * mm);
  fContactPressureRingsZPositions.push_back(28.0 * mm);
  fTemplateCrystalColumnSerNum = "";
  fIsOneOfMany = false;
}

//---------------------------------------------------------------------------//

void MJGeometryRDCrystalColumn::SetDefaultsDEMONSTRATOR()
{
  // Rough approximation for DEMONSTRATOR
  fEps = 0.001 * mm;
  fIsDEMO = true;
  fStringerLidThickness = 6.5 * mm;
  fStringerLidRadius = 35.1 * mm;
  fStringerLidMaterialName = "Copper-EF";
  fStringerSupportThickness = 9.5 * mm;
  fSupportRodLength = 200.5 * mm;
  fSupportRodInnerRadius = 1.4 * mm;
  fSupportRodOuterRadius = 1.6 * mm;
  fSupportRodMaterialName = "Copper-EF";
  fContactPressureRingHeight = 3.0 * mm;
  fContactPressureRingInnerRadius = 35.3 * mm;
  fContactPressureRingOuterRadius = 35.5 * mm;
  fContactPressureRingMaterialName = "Teflon";
  fGeTrayHeight = 3.0 * mm;
  fGeTrayInnerRadius = 31.5 * mm;
  fGeTrayOuterRadius = 35.0 * mm;
  fGeTrayArmWidth = 2.5 * mm;
  fGeTrayStubRadius = 7.9 / 2. * mm;
  fGeTrayStubHeight = 3.0 * mm;
  fGeTrayMaterialName = "Teflon";
  fLFEPAsmHeight = 16 * mm;
  fLFEPAsmWidth = 23 * mm;
  fLFEPAsmThickness = 0.4 * mm;
  fLFEPAsmMaterialName = "Teflon";
  fOffset = G4ThreeVector(0,0,0);
  fSupportRodRadialPosition = 37.2 * mm; 
  fColumnRotationZ = 0.0 * deg;
  fLFEPAsmRotationZ = 60.0 * deg;
  fLFEPAsmSpacing = 1.0 * mm;
  fLFEPAsmRadialPosition = 25.0 * mm;
  fGeTrayZPositions.clear();
  fGeTrayZPositions.push_back(160.0 * mm);
  fGeTrayZPositions.push_back(120.0 * mm);
  fGeTrayZPositions.push_back(80.0 * mm);
  fGeTrayZPositions.push_back(40.0 * mm);
  fGeTrayZPositions.push_back(0.0 * mm);
  fContactPressureRingsZPositions.clear();
  fContactPressureRingsZPositions.push_back(175.0 * mm);  
  fContactPressureRingsZPositions.push_back(135.0 * mm);  
  fContactPressureRingsZPositions.push_back(95.0 * mm);
  fContactPressureRingsZPositions.push_back(55.0 * mm);
  fContactPressureRingsZPositions.push_back(15.0 * mm);
  fTemplateCrystalColumnSerNum = "";
  fIsOneOfMany = false;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
