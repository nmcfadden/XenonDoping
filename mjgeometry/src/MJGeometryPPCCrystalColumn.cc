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
// $Id: MJGeometryPPCCrystalColumn.cc,v 1.1 2008-09-18 17:39:34 mitzi Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryPPCCrystalColumn.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: M.Boswell
 * CONTACT: 
 * FIRST SUBMISSION: Wed. July 23, 2008
 * 
 * REVISION:
 *
 * 07/23/2008, Created, M. Boswell
 *
 */
//---------------------------------------------------------------------------//
//

#include <sstream>


#include "globals.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"


#include "G4Material.hh"


#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4Vector3D.hh"


#include "G4AssemblyVolume.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"

#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryPPCCrystalColumnMessenger.hh"
#include "mjgeometry/MJGeometryPPCDetector.hh"


//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryPPCCrystalColumn.hh"
using namespace std;

using namespace CLHEP;
using namespace MaGe;


//---------------------------------------------------------------------------//
MJGeometryPPCCrystalColumn::MJGeometryPPCCrystalColumn():
  MGGeometryDetector("") 
{
  //fTemplateCrystal = new MGGeometryPPCDetector("Template", true);
  //fTemplateCrystal->SetDefaults();
  SetDefaults();


;}
MJGeometryPPCCrystalColumn::MJGeometryPPCCrystalColumn(G4String serNum):
  MGGeometryDetector(serNum) 
{
  //  if(!messOff)
  SetDefaults();
  theDetectorLogical = 0;
  fG4Messenger = new MJGeometryPPCCrystalColumnMessenger(this);
    //  else 
    //    fG4Messenger = 0;
  //fTemplateCrystal = new MGGeometryPPCDetector(serNum, true);
  //fTemplateCrystal->SetDefaults();


}

//---------------------------------------------------------------------------//

MJGeometryPPCCrystalColumn::MJGeometryPPCCrystalColumn
(const MJGeometryPPCCrystalColumn &rhs):
  MGGeometryDetector(rhs.serialNumber), 
  fTemplateCrystal(rhs.fTemplateCrystal),
  fTemplateCrystalColumnSerNum(rhs.fTemplateCrystalColumnSerNum),
  fIsOneOfMany(rhs.fIsOneOfMany)

{}

//---------------------------------------------------------------------------//

MJGeometryPPCCrystalColumn::~MJGeometryPPCCrystalColumn()
{
//   if(!fIsOneOfMany) {
//     delete fCanVisAtt;
//     delete fColdplateVisAtt;
//   }
  delete fG4Messenger;
  //delete fTemplateCrystal;
  //size_t sizeCC = fCrystals.size();
  //for (size_t i=0; i<sizeCC; i++) 
  //  delete fCrystals[i];
}

//---------------------------------------------------------------------------//

void MJGeometryPPCCrystalColumn::ConstructDetector()
{

  
  MGLog(debugging) << "Number of Crystals " << fNumberOfCrystals << endlog;

  G4ThreeVector noOffset(0,0,0);
  G4Translate3D noOffset3D(0,0,0);
  G4RotationMatrix noRotate;// = new G4RotationMatrix();
  //noRotate->rotateZ(0*deg);
  G4Transform3D noTransform = noOffset3D;

  //world volume
  //------------
  //only need to build if we are constructing a single string of detectors
  if (!theDetectorLogical){
    Initialize();
    G4Tubs* worldSolid = 
      new G4Tubs("worldTubsSolid",0,fCuCanORad +2*fCuCanThick,
		 2*fCuCanHeight,0,twopi);
    theDetectorLogical = 
      new G4LogicalVolume(worldSolid, G4Material::GetMaterial("Vacuum"),
			  "motherLogical"+serialNumber);
    //MGLog(debugging) << "mother material" << fOpenSpaceMaterial << endlog;
    theDetectorLogical->SetVisAttributes(G4VisAttributes::Invisible);
  } else
    Initialize();

  size_t j;
  stringstream nameStream;
  G4AssemblyVolume* crystalShield;
  G4LogicalVolume* FETLogical = 0;
  G4LogicalVolume* CuCanLogical = 0;
  G4LogicalVolume* CuCanLidLogical = 0;
  G4LogicalVolume* InsulatorLogical = 0;
  G4LogicalVolumeStore *logicalVolumeStore = 
    G4LogicalVolumeStore::GetInstance();

  //again, if we are building this for an assembly then we only need to make
  //everything once, then place it over and over again.  
  if (fIsOneOfMany) {
    G4bool allLogicalsFound =false;
    for (j=0; j < logicalVolumeStore->size(); j++) {
      if ((*logicalVolumeStore)[j]->GetName() ==
	  G4String("FETLogical"+fTemplateCrystalColumnSerNum))
	FETLogical = (*logicalVolumeStore)[j];
      else if ((*logicalVolumeStore)[j]->GetName() ==
	  G4String("CuCanLogical"+fTemplateCrystalColumnSerNum))
	CuCanLogical = (*logicalVolumeStore)[j];
      else if ((*logicalVolumeStore)[j]->GetName() ==
	  G4String("CuCanLidLogical"+fTemplateCrystalColumnSerNum))
	CuCanLidLogical = (*logicalVolumeStore)[j];
      else if ((*logicalVolumeStore)[j]->GetName() ==
	  G4String("InsulatorLogical"+fTemplateCrystalColumnSerNum))
	InsulatorLogical = (*logicalVolumeStore)[j];
      allLogicalsFound = FETLogical && CuCanLogical && CuCanLidLogical 
	&& InsulatorLogical;
    }
    if (!allLogicalsFound) {
      MGLog(error) << "Error finding supposedly pre-existing logical" 
		   << " volumes.  " << endlog;
      MGLog(error) << "FETLogical \t CuCanLogical \t CuCanLidLogical"
		   << "\t InsulatorLogical " << endlog;
      MGLog(error) << FETLogical << "\t" << CuCanLogical << "\t" 
		   << CuCanLidLogical << "\t"  << InsulatorLogical << endlog;
      MGLog(fatal) << endlog;
    }

  } else {
    MGLog(debugging) << "Build first instance of column" << endlog;
			
      
    //-----------------
      
    //build the FET
    //--------------
    G4Box* FETSolid = 
      new G4Box("FETSolid"+serialNumber, fFETLength, fFETWidth, fFETHeight);
    FETLogical = 
      new G4LogicalVolume(FETSolid, FETMaterial, "FETLogical"+serialNumber);
    FETLogical->SetVisAttributes(FETAttributes);
				


    //build the Cu Can
    //----------------
    size_t i,CuCanDim = 8;
    G4double CuCanIRad[8] ={fCuCanBaseIRad,fCuCanBaseIRad,fCuCanIRad,
			    fCuCanIRad,
			    fCuCanORad, fCuCanORad,fCuCanIRad, fCuCanIRad};
    G4double CuCanORad[8] ;//= {fCuCanORad, fCuCanORad};
    for (i=0; i<CuCanDim; i++) CuCanORad[i] = fCuCanORad +fCuCanThick;
    G4double CuCanHeight[8]={0,fCuCanThick,fCuCanThick,fCuCanBaseHeight, 
			     fCuCanBaseHeight,fCuCanRingOffset,
			     fCuCanRingOffset,
			     fCuCanRingOffset+fCuCanRingHeight};
    G4Polycone* CuCanBaseSolid= 
      new G4Polycone("CuCanBase+Ring",0,twopi,CuCanDim, CuCanHeight,
		     CuCanIRad, CuCanORad);
    size_t CuCanSupportDim = 2;
    G4double CuCanSupportHeight[2]={0,fCuCanHeight-fCuCanLipThick};//, 
    //  fCuCanHeight-fCuCanLipThick, 
    //			    fCuCanHeight};
    G4double CuCanSupportIRad[2]={fCuCanIRad,fCuCanIRad};//,fCuCanLipIRad, 
    // fCuCanLipIRad};
    G4double CuCanSupportORad[2] = {fCuCanORad,fCuCanORad};//,fCuCanORad,
    // fCuCanORad};
    G4Polycone* CuCanSupportSolid = 
      new G4Polycone("CuCanSupport", -fCuCanSupportAng, 2*fCuCanSupportAng, 
		     CuCanSupportDim, CuCanSupportHeight, CuCanSupportIRad, 
		     CuCanSupportORad);
    G4Tubs* CuCanBoundSolid = new G4Tubs("CanBoundarySolid",fCuCanORad,
					 2*fCuCanORad,fCuCanHeight,0,twopi);
    G4VSolid* CuCanSolid=
      new G4SubtractionSolid("CuCanSolid"+serialNumber,CuCanBaseSolid,
			     CuCanBoundSolid, noTransform);
    CuCanSolid = 
      new G4UnionSolid("Can+Support01",CuCanSolid,CuCanSupportSolid,
		       noTransform);
    G4RotationMatrix* RotateCan = new G4RotationMatrix();
    RotateCan -> rotateZ(120*deg);
    CuCanSolid = new G4UnionSolid("Can+Support02",CuCanSolid, 
				  CuCanSupportSolid,RotateCan, noOffset);
    RotateCan -> rotateZ(120*deg);
    CuCanSolid = new G4UnionSolid("Can+Support03",CuCanSolid, 
				  CuCanSupportSolid,RotateCan, noOffset);
    CuCanLogical = new G4LogicalVolume(CuCanSolid,CanMaterial,
				       "CuCanLogical"+serialNumber);
    CuCanLogical->SetVisAttributes(EFCuAttributes);
				  

    //-------------------
    //build the CuCan top
    //-------------------
    size_t CuCanLidDim = 4;
    G4double CuCanLidIRad[4]={fCuCanLidIRad, fCuCanLidIRad, 
			      fCuCanLidLipRad,fCuCanLidLipRad};
    G4double CuCanLidORad[4] = {fCuCanLidORad, fCuCanLidORad, fCuCanLidORad,
				fCuCanLidORad};
    G4double CuCanLidHeight[4] = {0, fCuCanLidLipThick, fCuCanLidLipThick, 
				  fCuCanLidThick};
    G4Polycone* CuCanLidSolid = 
      new G4Polycone("CanCanLid"+serialNumber,0,twopi,CuCanLidDim,
		     CuCanLidHeight,CuCanLidIRad, CuCanLidORad);
    CuCanLidLogical = new G4LogicalVolume(CuCanLidSolid,CanMaterial,
					  "CuCanLidLogical"+serialNumber);
    CuCanLidLogical->SetVisAttributes(EFCuAttributes);

    //-------------------
    //build the insulators
    //--------------------
    size_t InsulatorDim = 4;
    G4double InsulatorIRad[4] = {fInsulatorIRadius, fInsulatorIRadius,0,0};
    G4double InsulatorORad[4] = {fInsulatorRadius, fInsulatorRadius, 
				 fInsulatorRadius,fInsulatorRadius};
    G4double InsulatorHeight[4] = {0,fInsulatorIHeight,fInsulatorIHeight,
				   fInsulatorHeight};
    G4VSolid* InsulatorSolid=
      new G4Polycone("InsulatorSolid",0,twopi, InsulatorDim, 
		     InsulatorHeight,InsulatorIRad, InsulatorORad);

    G4double ISubIRad[4] = {0,0,fInsulatorCutAwayRad,fInsulatorCutAwayRad};
    G4double ISubORad[4] = {fCuCanLipIRad, fCuCanLipIRad, fCuCanLipIRad, 
			    fCuCanLipIRad};
    G4double ISubHeight[4] = {0, fInsulatorBaseHeight,fInsulatorBaseHeight,
			      fInsulatorHeight};
    G4Polycone* InsulatorBaseSolid = 
      new G4Polycone("InsulatorBound",0,twopi,4,ISubHeight,ISubIRad,
		     ISubORad);
    InsulatorSolid =  
      new G4IntersectionSolid("Insulator+Base",InsulatorSolid,
			      InsulatorBaseSolid, 0,
			      G4ThreeVector(fInsulatorCutAwayRad,0,0));
    InsulatorLogical = new G4LogicalVolume(InsulatorSolid,InsulatorMaterial,
					   "InsulatorLogical"+serialNumber);
    InsulatorLogical->SetVisAttributes(KelFAttributes);



  }
  
  //put volumes in assembly
  //assembly for building the Shield
  crystalShield = new G4AssemblyVolume();


  G4RotationMatrix FETRotation;  //fet
  FETRotation.rotateZ(fFETOffsetAng);
  G4cout << "fFETOffset " << fFETOffset/inch << G4endl;
  G4cout << "Can lid " << fCuCanLidOffset/inch << G4endl;
  G4ThreeVector MoveFET(0,fFETOffsetRad, fFETOffset);
  G4Transform3D moveFET(FETRotation,FETRotation*MoveFET);// 
  crystalShield->AddPlacedVolume(FETLogical,moveFET);
  crystalShield->AddPlacedVolume(CuCanLogical, noTransform);


  G4ThreeVector MoveCuCanLid(0,0,fCuCanLidOffset);
  G4Transform3D moveCuCanLid(noRotate,MoveCuCanLid);
  crystalShield->AddPlacedVolume(CuCanLidLogical,moveCuCanLid);



  //bottom insulator
  G4ThreeVector MoveInsulator(0,fInsulatorOffset, 1.5*fCuCanThick +fEps);
  G4Transform3D moveInsulator(noRotate,MoveInsulator);
  crystalShield->AddPlacedVolume(InsulatorLogical,moveInsulator);
  G4RotationMatrix rotateInsulator;
  rotateInsulator.rotateZ(120*deg);
  moveInsulator = G4Transform3D(rotateInsulator,
				rotateInsulator*MoveInsulator);
  crystalShield->AddPlacedVolume(InsulatorLogical,moveInsulator);
  rotateInsulator.rotateZ(120*deg);
  moveInsulator=G4Transform3D(rotateInsulator,
			      rotateInsulator*MoveInsulator);
  crystalShield->AddPlacedVolume(InsulatorLogical,moveInsulator);
  
  //top insulators
  G4RotationMatrix rotateTopInsulator;
  rotateTopInsulator.rotateX(180*deg);
  G4ThreeVector MoveTopInsulator(fInsulatorOffset,0,-fCuCanLidOffset +fCuCanThick/2);
  G4Transform3D moveTopInsulator(rotateTopInsulator,
				 rotateTopInsulator*MoveTopInsulator);
  crystalShield->AddPlacedVolume(InsulatorLogical, moveTopInsulator);
  rotateTopInsulator.rotateZ(120*deg);
  moveTopInsulator = G4Transform3D(rotateTopInsulator,
				   rotateTopInsulator*MoveTopInsulator);
  crystalShield->AddPlacedVolume(InsulatorLogical,moveTopInsulator);
  rotateTopInsulator.rotateZ(120*deg);
  moveTopInsulator = G4Transform3D(rotateTopInsulator,
				   rotateTopInsulator*MoveTopInsulator);
  crystalShield->AddPlacedVolume(InsulatorLogical,moveTopInsulator);




					  
  //vector for placing single crystal assembly in the world
  G4RotationMatrix rotateAssembly;// = new G4RotationMatrix();
  rotateAssembly.rotateZ(0*deg);
  G4ThreeVector assemblyOffset(0,0,fAssemblyOffset);
  G4Transform3D transformCuCan;

  //now put everything inside the world
  //the following placement vectors are defined so that 
  // the detector and can assembly don't overlap
  //they have nothing to do with placement in the column
  G4ThreeVector moveGeCrystal(0,0,0);
  G4ThreeVector moveCuCan(0,0,-fCuCanOffset);

  //to start off we shift the entire assembly back by two 
  //AssemblyOffsets
  moveGeCrystal -= 2*assemblyOffset;
  moveCuCan -= 2*assemblyOffset;


  nameStream.clear();
  //place CuCans in the world
  for (j=0; j<fNumberOfCrystals; j++) {
    nameStream.str("");
    moveCuCan += assemblyOffset;
    nameStream << j << serialNumber;
    transformCuCan = G4Transform3D(rotateAssembly,moveCuCan);
    crystalShield->MakeImprint(theDetectorLogical, transformCuCan, j);
    rotateAssembly.rotateZ(fAssemblyOffsetAng);
  }    



  //build the crystal
  //-----------------
  
  MGLog(debugging) << "Build the ge detector" << endlog;
  fCrystals.clear();
  
  stringstream crystalNameStream;
  crystalNameStream.clear();
  //now place everything in the world
  for (j=0; j<fNumberOfCrystals; j++) {
    moveGeCrystal += assemblyOffset;
    crystalNameStream.str("");
    crystalNameStream << "Crystal" << j << GetSerialNumber();
    fCrystals.push_back(new MJGeometryPPCDetector(*fTemplateCrystal));
    fCrystals[j]->SetIsOneOfMany(true);
    fCrystals[j]->SetSerialNumber(crystalNameStream.str());
    fCrystals[j]->ConstructDetector();
    MGLog(debugging) << "fTemplateCrystal = " << crystalNameStream.str() << "  "  
		     << fCrystals[j]->GetDetectorLogical() << endlog;
    MGLog(debugging) << "Crystal Radius  = " << fCrystals[j]->GetRadius()/cm
		     << "Height = " << fCrystals[j]->GetHeight()/cm << endlog;
    new G4PVPlacement(G4Transform3D(rotateAssembly,moveGeCrystal), 
		      fCrystals[j]->GetDetectorLogical(),
		      crystalNameStream.str(),theDetectorLogical, false,0);
    //new G4PVPlacement(moveCuCan, oneCrystalLogical,nameStream.str(),
    //		      theDetectorLogical,false,i);
    //rotateAssembly.rotateZ(fAssemblyOffsetAng);
  }

  MGLog(debugging) << "Number of Daughters - " << theDetectorLogical->GetNoDaughters() << endlog;


//   //put stuff in the world
//   G4ThreeVector moveGe (0,0,0);
//     //first the crystal
//     crystalNameStream.str("");
//     //now the shield
//     moveCuCan = G4Transform3D(noRotate, 
// 			      rotateAssembly*(MoveCuCan+assemblyOffset));
//     moveGe += assemblyOffset;
//     moveGe *= rotateAssembly;
//   }
}
  

  


void MJGeometryPPCCrystalColumn::Initialize()
{


  MGLog(debugging) << "@@@@  Column Init @@@@@@@@@@@@@@@@" << endlog;
  


  FETAttributes =   // steel blue
    new G4VisAttributes(G4Colour(0.33,0.33,0.33,0.65));
  FETAttributes->SetVisibility(true);
  EFCuAttributes =   //dark green copper
    new G4VisAttributes(G4Colour(0.29,0.46,0.43,0.65));
  EFCuAttributes->SetVisibility(true);
  KaptonAttributes =   //  - bright red for kapton
    new G4VisAttributes(G4Colour(0.54,0.14,0.14,0.65));
  KaptonAttributes->SetVisibility(true);
  CuAttributes =   // copper color
    new G4VisAttributes(G4Colour(0.72,0.45,0.2,0.65));
  CuAttributes->SetVisibility(true);
  KelFAttributes =   //white
    new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.65));//white
  KelFAttributes->SetVisibility(true);
  GeAttributes =   //grey
    new G4VisAttributes(G4Colour(0.75,0.75,0.75,0.15));//grey
  GeAttributes->SetVisibility(true);


//   G4VisAttributes* Vol2Attributes =   //magenta
//     new G4VisAttributes(G4Colour(0.41,0.35,0.80,0.65));
//   Vol2Attributes->SetVisibility(true);
//   //seventh volume - white
//   G4VisAttributes* Vol7Attributes = 
//     new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.65));//white
//   Vol7Attributes->SetVisibility(true);
//   //eigth volume - orange
//   G4VisAttributes* Vol9Attributes = 
//     new G4VisAttributes(G4Colour(1.0,0.549,0.,0.15));//orange
//   Vol9Attributes->SetVisibility(true);

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //build materials


  //MGLog(debugging) << *(G4Material::GetMaterialTable());



  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //define some materials
  //G4String* matName;
  G4String symbol;             
  G4double a, z, density;     
  G4int ncomponents;
  G4double fractionmass;

  G4Element* H = new G4Element("Hydrogen", symbol="H", z=1., a=1.007*g/mole);
  G4Element* N = new G4Element("Nitrogen", symbol="N", z=7., a=14.01*g/mole);
  G4Element* O = new G4Element("Oxygen",   symbol="O", z=8., a=16.00*g/mole);
  G4Element* Cl = new G4Element("Chlorine",symbol="Cl",z=17.,a=35.453*g/mole);
  G4Element* C = new G4Element("Carbon",   symbol="C", z=6., a=12.0107*g/mole);
  G4Element* F = new G4Element("Flourine", symbol="F", z=9., a=18.998*g/mole);
 
 
 
  try {
    CanMaterial = G4Material::GetMaterial("Copper-EF");  
    if (!CanMaterial) throw "Copper-EF";
  }
  catch (const char* matName) {
    MGLog(debugging) << matName << " is not a defined G4Material." << endlog;
    MGLog(error) << endlog;
  }
  // MGLog(debugging) << "Try the insulator material" << endlog;
  try {
    InsulatorMaterial = G4Material::GetMaterial("Kel-F");    
    if (!InsulatorMaterial) throw "Kel-F";
  }
  catch (const char* matName) {
    MGLog(debugging) << matName << " is not a defined G4Material."
		 << " Defining it Manually." << endlog;
      InsulatorMaterial = 
	new G4Material("Kel-F",density = 2.13*g/cm3,ncomponents=3);
      InsulatorMaterial -> AddElement(Cl, fractionmass=0.30);
      InsulatorMaterial -> AddElement(C, fractionmass=0.21);
      InsulatorMaterial -> AddElement(F,fractionmass=0.49);
  }
  try {
    KaptonMaterial = G4Material::GetMaterial("Kapton");
    if (!KaptonMaterial) throw "Kapton";
  }
  catch (const char* matName) {
    MGLog(debugging) << matName << " is not a defined G4Material."
		 << " Defining it Manually." << endlog;
      KaptonMaterial = 
	new G4Material("Kapton",1.42*g/cm3, ncomponents=4);
      KaptonMaterial->AddElement(H, fractionmass = 0.0273);
      KaptonMaterial->AddElement(C, fractionmass = 0.7213);
      KaptonMaterial->AddElement(N, fractionmass = 0.0765);
      KaptonMaterial->AddElement(O, fractionmass = 0.1749);    
  }
  try {
    FETMaterial = G4Material::GetMaterial("Kapton");
    if (!FETMaterial) throw "FET";
  }
  catch (const char* matName) {
    MGLog(debugging) << matName << " is not a defined G4Material."
		 << " Defining it Manually." << endlog;
    //   FETMaterial = KaptonMaterial;
      FETMaterial = 
	new G4Material("Kapton",1.42*g/cm3, ncomponents=4);
      FETMaterial->AddElement(H, fractionmass = 0.0273);
      FETMaterial->AddElement(C, fractionmass = 0.7213);
      FETMaterial->AddElement(N, fractionmass = 0.0765);
      FETMaterial->AddElement(O, fractionmass = 0.1749);    
  }


  fAssemblyOffsetAng = 20*deg;
  fNumberOfCrystals = 3; 

  
  //parameters describing crystal
  fCrystalHeight = 2.36*inch;
  fCrystalRad = 2.56/2*inch;
  


  //parameters describing the Cu Housing Can
  fCuCanORad = 1.501*inch;
  fCuCanThick = 0.025*inch;
  fCuCanIRad = fCuCanORad - fCuCanThick;
  fCuCanHeight = 3.396*inch;
  fCuCanBaseIRad = 1.156*inch;
  fCuCanBaseHeight = 0.222*inch;
  fCuCanRingHeight = 0.394*inch;
  fCuCanRingOffset = 2.781*inch;
  fCuCanLipIRad = 1.451*inch;
  fCuCanLipThick = 0.025*inch;
  fCuCanSupportAng = 10*deg;

  fAssemblyOffset = fCuCanHeight;


  //paramters describing the Cu Can lid
  fCuCanLidORad = 2.95/2*inch;
  fCuCanLidLipRad = 2.313/2*inch;
  fCuCanLidIRad = 1.500/2*inch;
  fCuCanLidLipThick = 0.025*inch;
  fCuCanLidThick = 0.026*inch;

  //parameters describing FET
  fFETHeight = 0.159/2*inch;
  fFETWidth = 0.50/2*inch;
  fFETLength = 0.75/2*inch;
  fFETOffsetRad  = 1.08*inch; //wrt center of crystal;
  fFETOffsetAng = 22*deg;



  //parameters describing the insulator pieces
  fInsulatorBaseHeight = 0.197*inch;
  fInsulatorHeight = 0.419*inch;
  fInsulatorRadius = 0.19*inch;
  fInsulatorIRadius = 0.188/2*inch;
  fInsulatorIHeight = 0.172*inch;
  fInsulatorCutAwayRad = 1.28*inch;
  fInsulatorAng = 195*deg;
  //fInsulatorOffset = 2.6/2*inch;
  fInsulatorOffset = 2.624/2*inch;
  fInsulatorOffsetAng = 20*deg;

  //offsets
  fCuCanLidOffset = fCuCanThick +2*fInsulatorBaseHeight+fCrystalHeight+fEps;
  fFETOffset = fCuCanLidOffset + fFETHeight + 2*fCuCanThick+fEps;//z offset wrt bottom of can
  fCuCanOffset = fCuCanThick+fInsulatorBaseHeight+fCrystalHeight/2+fEps;
  //moveInsulator = G4ThreeVector(fInsulatorOffset*cos(fInsulatorOffsetAng),
				//fInsulatorOffset*sin(fInsulatorOffsetAng),
				//fCuCanOffset+2*fCuCanThick);




  fCrystalColumnHeight = 3*fAssemblyOffset;
  fCrystalColumnRadius = fCuCanORad;
  //  fTemplateCrystal->SetCrystalMaterial(fCrystalMaterial);



}
void MJGeometryPPCCrystalColumn::SetDefaults()
{

  MGLog(debugging) << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endlog;
  MGLog(debugging) << "Setting Defaults" << endlog;

  fEps = 0.001*inch; //some generic offsets
  serialNumber = "Template";
  fTemplateCrystalColumnSerNum="Template";
  fIsOneOfMany = false;
  fAssemblyOffset = 3.35*inch;
  fAssemblyOffsetAng = 20*deg;
  fNumberOfCrystals = 3; 

  
  //parameters describing crystal
  fCrystalHeight = 2.36*inch;
  fCrystalRad = 2.56/2*inch;
  


  //parameters describing the Cu Housing Can
  fCuCanORad = 1.501*inch;
  fCuCanThick = 0.025*inch;
  fCuCanIRad = fCuCanORad - fCuCanThick;
  fCuCanHeight = 3.396*inch;
  fCuCanBaseIRad = 1.156*inch;
  fCuCanBaseHeight = 0.222*inch;
  fCuCanRingHeight = 0.394*inch;
  fCuCanRingOffset = 2.781*inch;
  fCuCanLipIRad = 1.451*inch;
  fCuCanLipThick = 0.025*inch;
  fCuCanSupportAng = 10*deg;

  //paramters describing the Cu Can lid
  fCuCanLidORad = 2.95/2*inch;
  fCuCanLidLipRad = 2.313/2*inch;
  fCuCanLidIRad = 1.500/2*inch;
  fCuCanLidLipThick = 0.025*inch;
  fCuCanLidThick = 0.026*inch;

  //parameters describing FET
  fFETHeight = 0.159/2*inch;
  fFETWidth = 0.50/2*inch;
  fFETLength = 0.75/2*inch;
  fFETOffsetRad  = 1.08*inch; //wrt center of crystal;
  fFETOffsetAng = 22*deg;



  //parameters describing the insulator pieces
  fInsulatorBaseHeight = 0.197*inch;
  fInsulatorHeight = 0.419*inch;
  fInsulatorRadius = 0.19*inch;
  fInsulatorIRadius = 0.188/2*inch;
  fInsulatorIHeight = 0.172*inch;
  fInsulatorCutAwayRad = 1.28*inch;
  fInsulatorAng = 195*deg;
  fInsulatorOffset = 2.624/2*inch;
  fInsulatorOffsetAng = 20*deg;
  //moveInsulator = G4ThreeVector(fInsulatorOffset*cos(fInsulatorOffsetAng),
				//fInsulatorOffset*sin(fInsulatorOffsetAng),
				//fCuCanOffset+fCuCanThick);


  //offsets
  fCuCanLidOffset = fCuCanThick +2*fInsulatorBaseHeight+2*fCrystalHeight+fEps;
  fFETOffset = fCuCanLidOffset + fFETHeight + fCuCanThick+fEps;//z offset wrt bottom of can
  fCuCanOffset = fCuCanThick+fInsulatorBaseHeight+fCrystalHeight+fEps;







}
