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
#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoConstrMuonVetoAssembly.hh"
#include "mjdemonstrator/MJDemoMuonVetoSolidPanel.hh"
#include "mjdemonstrator/MJDemoMuonVetoThroughPanel.hh"
#include "mjdemonstrator/MJDemoMuonVetoOverFloorAssembly.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoConstrMuonVetoAssembly::MJDemoConstrMuonVetoAssembly(G4String fName, G4String serialNumber, G4String constrConfig) :
  MJVDemoAssembly(fName, serialNumber, "MuonVeto"), fConstrConfig(constrConfig)
{
  G4double in2mm = 25.4;  //inches to millimeters

  G4double panel_w = in2mm*0.5;

  G4double shortSolidTop_l =    in2mm*69.0/2.0;
  G4double shortSolidTop_h =    in2mm*(29.950+0.25)/2.0;  //correction for changed cryostat position w/in shield - may need to be finessed
  G4double shortSolidBottom_l = shortSolidTop_l;	
  G4double shortSolidBottom_h = in2mm*(37.3-0.25)/2.0;  //correction for changed cryostat position w/in shield - may need to be finessed

  G4double shortSolidLeft_l =  in2mm*70.25/2.0;
  G4double shortSolidLeft_h =  in2mm*34.5/2.0;
  G4double shortSolidRight_l = shortSolidLeft_l;  
  G4double shortSolidRight_h = shortSolidLeft_h;  

  G4double longSolidTop_l =    in2mm*(81.0)/2.0; 
  G4double longSolidTop_h =    shortSolidTop_h; 
  G4double longSolidBottom_l = longSolidTop_l;  
  G4double longSolidBottom_h = shortSolidBottom_h;  
 
  G4double longSolidLeft_l = shortSolidLeft_l;  
  G4double longSolidLeft_h = in2mm*(33.5 - 1.15)/2.0;//correction for changed cryostat position w/in shield - may need to be finessed
  G4double longSolidRight_l = shortSolidLeft_l;  
  G4double longSolidRight_h = in2mm*(47.5 + 1.15)/2.0;//correction for changed cryostat position w/in shield - may need to be finessed

  G4double topOuter_l = in2mm*69.0/2.0;  
  G4double topOuter_h = in2mm*41.0/2.0;
  G4double topInner_l = in2mm*85.0/2.0;  
  G4double topInner_h = in2mm*34.5/2.0;

  fShortSolidTopPtr =    new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_001", "867-5309", shortSolidTop_l,    shortSolidTop_h);
  fShortSolidBottomPtr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_002", "867-5309", shortSolidBottom_l, shortSolidBottom_h);
  fShortSolidLeftPtr =   new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_003", "867-5309", shortSolidLeft_l,   shortSolidLeft_h);
  fShortSolidRightPtr =  new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_004", "867-5309", shortSolidRight_l,  shortSolidRight_h);

  fLongSolidTopPtr =    new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_005", "867-5309", longSolidTop_l,    longSolidTop_h);
  fLongSolidBottomPtr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_006", "867-5309", longSolidBottom_l, longSolidBottom_h);
  fLongSolidLeftPtr =   new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_007", "867-5309", longSolidLeft_l,   longSolidLeft_h);
  fLongSolidRightPtr =  new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_008", "867-5309", longSolidRight_l,  longSolidRight_h);

  fTopOuter1Ptr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_009", "867-5309", topOuter_l, topOuter_h);
  fTopOuter2Ptr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_010", "867-5309", topOuter_l, topOuter_h);
  fTopInner1Ptr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_011", "867-5309", topInner_l, topInner_h);
  fTopInner2Ptr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_012", "867-5309", topInner_l, topInner_h);

  //set cutout position to be outside the volume - don't want a cut out for the top panels                     length              height              hole pos x   hole pos y          cutout pos x       cutout pos y        cutout angle
  fShortThroughTopPtr =    new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_001", "867-5309", shortSolidTop_l,    shortSolidTop_h,    0.0,         shortSolidTop_h,    5000.0,            0.0,                0.0);  
  fShortThroughBottomPtr = new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_002", "867-5309", shortSolidBottom_l, shortSolidBottom_h, 0.0,         shortSolidBottom_h, 0.0,              -shortSolidBottom_h, 0.0);
  fShortThroughLeftPtr =   new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_003", "867-5309", shortSolidLeft_l,   shortSolidLeft_h,   4.0*panel_w, shortSolidLeft_h,  -shortSolidLeft_l,  shortSolidLeft_h,  90.0);
  fShortThroughRightPtr =  new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_004", "867-5309", shortSolidRight_l,  shortSolidRight_h,  4.0*panel_w, shortSolidRight_h, -shortSolidRight_l, shortSolidRight_h, 90.0);

  fLongThroughTopPtr =    new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_005", "867-5309", longSolidTop_l,    longSolidTop_h,    -2.0*longSolidLeft_h + longSolidTop_l,    longSolidTop_h,     5000.0, 0.0, 0.0);
  fLongThroughBottomPtr = new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_006", "867-5309", longSolidBottom_l, longSolidBottom_h, -2.0*longSolidLeft_h + longSolidBottom_l, longSolidBottom_h, -2.0*longSolidLeft_h + 25.4*longSolidTop_l, -longSolidBottom_h, 0.0);
  fLongThroughLeftPtr =   new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_008", "867-5309", longSolidLeft_l,   longSolidLeft_h,  4.0*panel_w, longSolidLeft_h,  -longSolidLeft_l,  longSolidLeft_h,  90.0);
  fLongThroughRightPtr =  new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_007", "867-5309", longSolidRight_l,  longSolidRight_h, 4.0*panel_w, longSolidRight_h, -longSolidRight_l, longSolidRight_h, 90.0);

  fOverFloorAssmPtr1 = new MJDemoMuonVetoOverFloorAssembly(fName+"_MuonVetoOverFloorAssm_001", "867-5309");
  fOverFloorAssmPtr2 = new MJDemoMuonVetoOverFloorAssembly(fName+"_MuonVetoOverFloorAssm_002", "867-5309");
  }

MJDemoConstrMuonVetoAssembly::MJDemoConstrMuonVetoAssembly(const MJDemoConstrMuonVetoAssembly & rhs) : 
  MJVDemoAssembly(rhs), fConstrConfig(rhs.fConstrConfig)
{
  G4double in2mm = 25.4;  //inches to millimeters

  G4double panel_w = in2mm*0.5;

	
	G4double shortSolidTop_l =    in2mm*69.0/2.0;
	G4double shortSolidTop_h =    in2mm*(29.950+0.25)/2.0;  //correction for changed cryostat position w/in shield - may need to be finessed
	G4double shortSolidBottom_l = shortSolidTop_l;
	G4double shortSolidBottom_h = in2mm*(37.3-0.25)/2.0;  //correction for changed cryostat position w/in shield - may need to be finessed
	
	G4double shortSolidLeft_l =  in2mm*70.25/2.0;
	G4double shortSolidLeft_h =  in2mm*34.5/2.0;
	G4double shortSolidRight_l = shortSolidLeft_l;
	G4double shortSolidRight_h = shortSolidLeft_h;
	
	G4double longSolidTop_l =    in2mm*(81.0)/2.0; 
	G4double longSolidTop_h =    shortSolidTop_h;  //correction for changed cryostat position w/in shield - may need to be finessed
	G4double longSolidBottom_l = longSolidTop_l;
	G4double longSolidBottom_h = shortSolidBottom_h;  //correction for changed cryostat position w/in shield - may need to be finessed
	
	G4double longSolidLeft_l = shortSolidLeft_l;
	G4double longSolidLeft_h = in2mm*(33.5 - 1.15)/2.0;//correction for changed cryostat position w/in shield - may need to be finessed
	G4double longSolidRight_l = shortSolidLeft_l;
	G4double longSolidRight_h = in2mm*(47.5 + 1.15)/2.0;//correction for changed cryostat position w/in shield - may need to be finessed
	
	G4double topOuter_l = in2mm*69.0/2.0;
	G4double topOuter_h = in2mm*41.0/2.0;
	G4double topInner_l = in2mm*85.0/2.0;
	G4double topInner_h = in2mm*34.5/2.0;

  fShortSolidTopPtr =    new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_001", "867-5309", shortSolidTop_l,    shortSolidTop_h);
  fShortSolidBottomPtr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_002", "867-5309", shortSolidBottom_l, shortSolidBottom_h);
  fShortSolidLeftPtr =   new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_003", "867-5309", shortSolidLeft_l,   shortSolidLeft_h);
  fShortSolidRightPtr =  new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_004", "867-5309", shortSolidRight_l,  shortSolidRight_h);

  fLongSolidTopPtr =    new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_005", "867-5309", longSolidTop_l,    longSolidTop_h);
  fLongSolidBottomPtr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_006", "867-5309", longSolidBottom_l, longSolidBottom_h);
  fLongSolidLeftPtr =   new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_007", "867-5309", longSolidLeft_l,   longSolidLeft_h);
  fLongSolidRightPtr =  new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_008", "867-5309", longSolidRight_l,  longSolidRight_h);

  fTopOuter1Ptr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_009", "867-5309", topOuter_l, topOuter_h);
  fTopOuter2Ptr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_010", "867-5309", topOuter_l, topOuter_h);
  fTopInner1Ptr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_011", "867-5309", topInner_l, topInner_h);
  fTopInner2Ptr = new MJDemoMuonVetoSolidPanel(fName+"_ActiveMuonVetoSolidPanel_012", "867-5309", topInner_l, topInner_h);

  //set cutout position to be outside the volume - don't want a cut out for the top panels
  fShortThroughTopPtr =    new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_001", "867-5309", shortSolidTop_l,    shortSolidTop_h,    0.0,         shortSolidTop_h,    5000.0,            0.0,                0.0);  
  fShortThroughBottomPtr = new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_002", "867-5309", shortSolidBottom_l, shortSolidBottom_h, 0.0,         shortSolidBottom_h, 0.0,              -shortSolidBottom_h, 0.0);
  fShortThroughLeftPtr =   new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_003", "867-5309", shortSolidLeft_l,   shortSolidLeft_h,   4.0*panel_w, shortSolidLeft_h,  -shortSolidLeft_l,  shortSolidLeft_h,  90.0);
  fShortThroughRightPtr =  new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_004", "867-5309", shortSolidRight_l,  shortSolidRight_h,  4.0*panel_w, shortSolidRight_h, -shortSolidRight_l, shortSolidRight_h, 90.0);

  fLongThroughTopPtr =    new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_005", "867-5309", longSolidTop_l,    longSolidTop_h,    -2.0*longSolidLeft_h + longSolidTop_l,    longSolidTop_h,     5000.0, 0.0, 0.0);
  fLongThroughBottomPtr = new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_006", "867-5309", longSolidBottom_l, longSolidBottom_h, -2.0*longSolidLeft_h + longSolidBottom_l, longSolidBottom_h, -2.0*longSolidLeft_h + 25.4*longSolidTop_l, -longSolidBottom_h, 0.0);
  fLongThroughLeftPtr =   new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_008", "867-5309", longSolidLeft_l,   longSolidLeft_h,  4.0*panel_w, longSolidLeft_h,  -longSolidLeft_l,  longSolidLeft_h,  90.0);
  fLongThroughRightPtr =  new MJDemoMuonVetoThroughPanel(fName+"_ActiveMuonVetoThroughPanel_007", "867-5309", longSolidRight_l,  longSolidRight_h, 4.0*panel_w, longSolidRight_h, -longSolidRight_l, longSolidRight_h, 90.0);

  fOverFloorAssmPtr1 = new MJDemoMuonVetoOverFloorAssembly(fName+"_MuonVetoOverFloorAssm_001", "867-5309");
  fOverFloorAssmPtr2 = new MJDemoMuonVetoOverFloorAssembly(fName+"_MuonVetoOverFloorAssm_002", "867-5309");
}

MJDemoConstrMuonVetoAssembly::~MJDemoConstrMuonVetoAssembly()
{
  delete fShortSolidTopPtr;
  delete fShortSolidBottomPtr;
  delete fShortSolidLeftPtr;
  delete fShortSolidRightPtr;

  delete fLongSolidTopPtr;
  delete fLongSolidBottomPtr;
  delete fLongSolidLeftPtr;
  delete fLongSolidRightPtr;

  delete fTopOuter1Ptr;
  delete fTopOuter2Ptr;
  delete fTopInner1Ptr;
  delete fTopInner2Ptr;

  delete fShortThroughTopPtr;
  delete fShortThroughBottomPtr;
  delete fShortThroughLeftPtr;
  delete fShortThroughRightPtr;

  delete fLongThroughTopPtr;
  delete fLongThroughBottomPtr;
  delete fLongThroughLeftPtr;
  delete fLongThroughRightPtr;

  delete fOverFloorAssmPtr1;
  delete fOverFloorAssmPtr2;
}

void MJDemoConstrMuonVetoAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
    
  G4bool topPanels, bottomPanels, northPanels, southPanels, westPanels, eastPanels;
  if(fConstrConfig.empty())
    topPanels = bottomPanels = northPanels = southPanels = westPanels = eastPanels = true;  
  else{
    MGLog(routine)<<"You have chosen to construct the veto with only the given sides:"<<endl; 
    topPanels = bottomPanels = northPanels = southPanels = westPanels = eastPanels = false; 
    if(fConstrConfig[0]=='1') { topPanels = true; MGLog(routine)<<"top"<<endl; }
    if(fConstrConfig[1]=='1') { bottomPanels = true; MGLog(routine)<<"bottom"<<endl; }
    if(fConstrConfig[2]=='1') { northPanels = true; MGLog(routine)<<"north"<<endl; }
    if(fConstrConfig[3]=='1') { southPanels = true; MGLog(routine)<<"south"<<endl; }
    if(fConstrConfig[4]=='1') { westPanels = true; MGLog(routine)<<"west"<<endl; }
    if(fConstrConfig[5]=='1') { eastPanels = true; MGLog(routine)<<"east"<<endl; }
    }
    
  G4double offset = 115.0*mm;

  G4double in2mm = 25.4;  //inches to millimeters

  G4double panel_w = in2mm*0.5;
	
	G4double shortSolidTop_l =    in2mm*69.0/2.0;
	G4double shortSolidTop_h =    in2mm*(29.950+0.25)/2.0;  //correction for changed cryostat position w/in shield - may need to be finessed
//	G4double shortSolidBottom_l = shortSolidTop_l;  //just a reminder of how the dimensions work out
	G4double shortSolidBottom_h = in2mm*(37.3-0.25)/2.0;  //correction for changed cryostat position w/in shield - may need to be finessed
	
	G4double shortSolidLeft_l =  in2mm*70.25/2.0;
	G4double shortSolidLeft_h =  in2mm*34.5/2.0;
//	G4double shortSolidRight_l = shortSolidLeft_l;  //just a reminder of how the dimensions work out
//	G4double shortSolidRight_h = shortSolidLeft_h;  //just a reminder of how the dimensions work out
	
	G4double longSolidTop_l =    in2mm*(81.0)/2.0; 
//	G4double longSolidTop_h =    shortSolidTop_h;  //just a reminder of how the dimensions work out
	G4double longSolidBottom_l = longSolidTop_l;
	G4double longSolidBottom_h = shortSolidBottom_h;  //just a reminder of how the dimensions work out
	
//	G4double longSolidLeft_l = shortSolidLeft_l;  //just a reminder of how the dimensions work out
	G4double longSolidLeft_h = in2mm*(33.5 - 1.15)/2.0;//correction for changed cryostat position w/in shield - may need to be finessed
//	G4double longSolidRight_l = shortSolidLeft_l;  //just a reminder of how the dimensions work out
	G4double longSolidRight_h = in2mm*(47.5 + 1.15)/2.0;//correction for changed cryostat position w/in shield - may need to be finessed
	
//	G4double topOuter_l = in2mm*69.0/2.0;  //just a reminder of how the dimensions work out
	G4double topOuter_h = in2mm*41.0/2.0;  
//	G4double topInner_l = in2mm*85.0/2.0;  //just a reminder of how the dimensions work out
	G4double topInner_h = in2mm*34.5/2.0;
	
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

//cross arm panels

  G4ThreeVector *shortThroughTopLocalPos = new G4ThreeVector((longSolidTop_l*mm + 3.0*panel_w*mm), 0, shortSolidTop_h*mm+ offset);
  G4RotationMatrix *shortThroughTopLocalRot = new G4RotationMatrix();
  shortThroughTopLocalRot->rotateZ(90*deg);
  shortThroughTopLocalRot->rotateX(90*deg);
  G4AffineTransform *shortThroughTopAffine = new G4AffineTransform(shortThroughTopLocalRot,*shortThroughTopLocalPos);
  *shortThroughTopAffine*=*assemAffine;  
  G4ThreeVector *shortThroughTopGlobalPos = new G4ThreeVector(shortThroughTopAffine->NetTranslation());
  G4RotationMatrix *shortThroughTopGlobalRot= new G4RotationMatrix(shortThroughTopAffine->NetRotation());
  if(eastPanels) fShortThroughTopPtr->Place(shortThroughTopGlobalPos, shortThroughTopGlobalRot, motherLogical);
  delete shortThroughTopLocalPos;
  delete shortThroughTopLocalRot;
  delete shortThroughTopAffine;

  G4ThreeVector *shortThroughBottomLocalPos = new G4ThreeVector((longSolidBottom_l*mm + 3.0*panel_w*mm), 0, -longSolidBottom_h*mm+ offset);
  G4RotationMatrix *shortThroughBottomLocalRot = new G4RotationMatrix();
  shortThroughBottomLocalRot->rotateX(180*deg); 
  shortThroughBottomLocalRot->rotateZ(90*deg);
  shortThroughBottomLocalRot->rotateX(90*deg);
  G4AffineTransform *shortThroughBottomAffine = new G4AffineTransform(shortThroughBottomLocalRot,*shortThroughBottomLocalPos);
  *shortThroughBottomAffine*=*assemAffine;  
  G4ThreeVector *shortThroughBottomGlobalPos = new G4ThreeVector(shortThroughBottomAffine->NetTranslation());
  G4RotationMatrix *shortThroughBottomGlobalRot= new G4RotationMatrix(shortThroughBottomAffine->NetRotation());
  if(eastPanels) fShortThroughBottomPtr->Place(shortThroughBottomGlobalPos, shortThroughBottomGlobalRot, motherLogical);
  delete shortThroughBottomLocalPos;
  delete shortThroughBottomLocalRot;
  delete shortThroughBottomAffine;

  G4ThreeVector *shortThroughLeftLocalPos = new G4ThreeVector(longSolidTop_l*mm + panel_w*mm, -shortSolidLeft_h*mm, -4.0*panel_w*mm + offset);
  G4RotationMatrix *shortThroughLeftLocalRot = new G4RotationMatrix();
  shortThroughLeftLocalRot->rotateY(90*deg);
  G4AffineTransform *shortThroughLeftAffine = new G4AffineTransform(shortThroughLeftLocalRot,*shortThroughLeftLocalPos);
  *shortThroughLeftAffine*=*assemAffine;  
  G4ThreeVector *shortThroughLeftGlobalPos = new G4ThreeVector(shortThroughLeftAffine->NetTranslation());
  G4RotationMatrix *shortThroughLeftGlobalRot= new G4RotationMatrix(shortThroughLeftAffine->NetRotation());
  if(eastPanels) fShortThroughLeftPtr->Place(shortThroughLeftGlobalPos, shortThroughLeftGlobalRot, motherLogical);
  delete shortThroughLeftLocalPos;
  delete shortThroughLeftLocalRot;
  delete shortThroughLeftAffine;

  G4ThreeVector *shortThroughRightLocalPos = new G4ThreeVector(longSolidTop_l*mm + panel_w*mm, shortSolidLeft_h*mm,  -4.0*panel_w*mm + offset);
  G4RotationMatrix *shortThroughRightLocalRot = new G4RotationMatrix();
  shortThroughRightLocalRot->rotateZ(180*deg); 
  shortThroughRightLocalRot->rotateY(90*deg);
  G4AffineTransform *shortThroughRightAffine = new G4AffineTransform(shortThroughRightLocalRot,*shortThroughRightLocalPos);
  *shortThroughRightAffine*=*assemAffine;  
  G4ThreeVector *shortThroughRightGlobalPos = new G4ThreeVector(shortThroughRightAffine->NetTranslation());
  G4RotationMatrix *shortThroughRightGlobalRot= new G4RotationMatrix(shortThroughRightAffine->NetRotation());
  if(eastPanels) fShortThroughRightPtr->Place(shortThroughRightGlobalPos, shortThroughRightGlobalRot, motherLogical);
  delete shortThroughRightLocalPos;
  delete shortThroughRightLocalRot;
  delete shortThroughRightAffine;


  G4ThreeVector *longThroughTopLocalPos = new G4ThreeVector(0, (-shortSolidTop_l*mm + panel_w*mm), shortSolidTop_h*mm+offset);
  G4RotationMatrix *longThroughTopLocalRot = new G4RotationMatrix();
  longThroughTopLocalRot->rotateZ(180*deg);
  longThroughTopLocalRot->rotateX(90*deg);
  G4AffineTransform *longThroughTopAffine = new G4AffineTransform(longThroughTopLocalRot,*longThroughTopLocalPos);
  *longThroughTopAffine*=*assemAffine;  
  G4ThreeVector *longThroughTopGlobalPos = new G4ThreeVector(longThroughTopAffine->NetTranslation());
  G4RotationMatrix *longThroughTopGlobalRot= new G4RotationMatrix(longThroughTopAffine->NetRotation());
  if(southPanels) fLongThroughTopPtr->Place(longThroughTopGlobalPos, longThroughTopGlobalRot, motherLogical);
  delete longThroughTopLocalPos;
  delete longThroughTopLocalRot;
  delete longThroughTopAffine;

  G4ThreeVector *longThroughBottomLocalPos = new G4ThreeVector(0, (-shortSolidTop_l*mm + panel_w*mm), -shortSolidBottom_h*mm+offset);
  G4RotationMatrix *longThroughBottomLocalRot = new G4RotationMatrix();
  longThroughBottomLocalRot->rotateY(180*deg);
  longThroughBottomLocalRot->rotateX(90*deg);
  G4AffineTransform *longThroughBottomAffine = new G4AffineTransform(longThroughBottomLocalRot,*longThroughBottomLocalPos);
  *longThroughBottomAffine*=*assemAffine;  
  G4ThreeVector *longThroughBottomGlobalPos = new G4ThreeVector(longThroughBottomAffine->NetTranslation());
  G4RotationMatrix *longThroughBottomGlobalRot= new G4RotationMatrix(longThroughBottomAffine->NetRotation());
  if(southPanels) fLongThroughBottomPtr->Place(longThroughBottomGlobalPos, longThroughBottomGlobalRot, motherLogical);
  delete longThroughBottomLocalPos;
  delete longThroughBottomLocalRot;
  delete longThroughBottomAffine;

  G4ThreeVector *longThroughRightLocalPos = new G4ThreeVector(longSolidLeft_h*mm, (-shortSolidTop_l*mm + 3.0*panel_w*mm), -4.0*panel_w*mm+offset);
  G4RotationMatrix *longThroughRightLocalRot = new G4RotationMatrix();
  longThroughRightLocalRot->rotateZ(180*deg);
  longThroughRightLocalRot->rotateY(90*deg);
  longThroughRightLocalRot->rotateX(90*deg);
  G4AffineTransform *longThroughRightAffine = new G4AffineTransform(longThroughRightLocalRot,*longThroughRightLocalPos);
  *longThroughRightAffine*=*assemAffine;  
  G4ThreeVector *longThroughRightGlobalPos = new G4ThreeVector(longThroughRightAffine->NetTranslation());
  G4RotationMatrix *longThroughRightGlobalRot= new G4RotationMatrix(longThroughRightAffine->NetRotation());
  if(southPanels) fLongThroughRightPtr->Place(longThroughRightGlobalPos, longThroughRightGlobalRot, motherLogical);
  delete longThroughRightLocalPos;
  delete longThroughRightLocalRot;
  delete longThroughRightAffine;
	
  G4ThreeVector *longThroughLeftLocalPos = new G4ThreeVector(-longSolidRight_h*mm, (-shortSolidTop_l*mm + 3.0*panel_w*mm), -4.0*panel_w*mm+offset);
  G4RotationMatrix *longThroughLeftLocalRot = new G4RotationMatrix();
  longThroughLeftLocalRot->rotateY(90*deg);
  longThroughLeftLocalRot->rotateX(90*deg);
  G4AffineTransform *longThroughLeftAffine = new G4AffineTransform(longThroughLeftLocalRot,*longThroughLeftLocalPos);
  *longThroughLeftAffine*=*assemAffine;  
  G4ThreeVector *longThroughLeftGlobalPos = new G4ThreeVector(longThroughLeftAffine->NetTranslation());
  G4RotationMatrix *longThroughLeftGlobalRot= new G4RotationMatrix(longThroughLeftAffine->NetRotation());
  if(southPanels) fLongThroughLeftPtr->Place(longThroughLeftGlobalPos, longThroughLeftGlobalRot, motherLogical);
  delete longThroughLeftLocalPos;
  delete longThroughLeftLocalRot;
  delete longThroughLeftAffine;

//solid panels

  G4ThreeVector *shortSolidTopLocalPos = new G4ThreeVector(-(longSolidTop_l*mm + 3.0*panel_w*mm), 0, shortSolidTop_h*mm+ offset);
  G4RotationMatrix *shortSolidTopLocalRot = new G4RotationMatrix();
  shortSolidTopLocalRot->rotateZ(90*deg);
  shortSolidTopLocalRot->rotateX(90*deg);
  G4AffineTransform *shortSolidTopAffine = new G4AffineTransform(shortSolidTopLocalRot,*shortSolidTopLocalPos);
  *shortSolidTopAffine*=*assemAffine;  
  G4ThreeVector *shortSolidTopGlobalPos = new G4ThreeVector(shortSolidTopAffine->NetTranslation());
  G4RotationMatrix *shortSolidTopGlobalRot= new G4RotationMatrix(shortSolidTopAffine->NetRotation());
  if(westPanels) fShortSolidTopPtr->Place(shortSolidTopGlobalPos, shortSolidTopGlobalRot, motherLogical);
  delete shortSolidTopLocalPos;
  delete shortSolidTopLocalRot;
  delete shortSolidTopAffine;

  G4ThreeVector *shortSolidBottomLocalPos = new G4ThreeVector(-(longSolidTop_l*mm + 3.0*panel_w*mm), 0, -longSolidBottom_h*mm+ offset);
  G4RotationMatrix *shortSolidBottomLocalRot = new G4RotationMatrix();
  shortSolidBottomLocalRot->rotateX(180*deg); 
  shortSolidBottomLocalRot->rotateZ(90*deg);
  shortSolidBottomLocalRot->rotateX(90*deg);
  G4AffineTransform *shortSolidBottomAffine = new G4AffineTransform(shortSolidBottomLocalRot,*shortSolidBottomLocalPos);
  *shortSolidBottomAffine*=*assemAffine;  
  G4ThreeVector *shortSolidBottomGlobalPos = new G4ThreeVector(shortSolidBottomAffine->NetTranslation());
  G4RotationMatrix *shortSolidBottomGlobalRot= new G4RotationMatrix(shortSolidBottomAffine->NetRotation());
  if(westPanels) fShortSolidBottomPtr->Place(shortSolidBottomGlobalPos, shortSolidBottomGlobalRot, motherLogical);
  delete shortSolidBottomLocalPos;
  delete shortSolidBottomLocalRot;
  delete shortSolidBottomAffine;

  G4ThreeVector *shortSolidLeftLocalPos = new G4ThreeVector(-(longSolidTop_l*mm + panel_w*mm), -shortSolidLeft_h*mm, -4.0*panel_w*mm + offset);
  G4RotationMatrix *shortSolidLeftLocalRot = new G4RotationMatrix();
  shortSolidLeftLocalRot->rotateY(90*deg);
  G4AffineTransform *shortSolidLeftAffine = new G4AffineTransform(shortSolidLeftLocalRot,*shortSolidLeftLocalPos);
  *shortSolidLeftAffine*=*assemAffine;  
  G4ThreeVector *shortSolidLeftGlobalPos = new G4ThreeVector(shortSolidLeftAffine->NetTranslation());
  G4RotationMatrix *shortSolidLeftGlobalRot= new G4RotationMatrix(shortSolidLeftAffine->NetRotation());
  if(westPanels) fShortSolidLeftPtr->Place(shortSolidLeftGlobalPos, shortSolidLeftGlobalRot, motherLogical);
  delete shortSolidLeftLocalPos;
  delete shortSolidLeftLocalRot;
  delete shortSolidLeftAffine;

  G4ThreeVector *shortSolidRightLocalPos = new G4ThreeVector(-(longSolidTop_l*mm + panel_w*mm), shortSolidLeft_h*mm,  -4.0*panel_w*mm + offset); 
  G4RotationMatrix *shortSolidRightLocalRot = new G4RotationMatrix();
  shortSolidRightLocalRot->rotateZ(180*deg); 
  shortSolidRightLocalRot->rotateY(90*deg);
  G4AffineTransform *shortSolidRightAffine = new G4AffineTransform(shortSolidRightLocalRot,*shortSolidRightLocalPos);
  *shortSolidRightAffine*=*assemAffine;  
  G4ThreeVector *shortSolidRightGlobalPos = new G4ThreeVector(shortSolidRightAffine->NetTranslation());
  G4RotationMatrix *shortSolidRightGlobalRot= new G4RotationMatrix(shortSolidRightAffine->NetRotation());
  if(westPanels) fShortSolidRightPtr->Place(shortSolidRightGlobalPos, shortSolidRightGlobalRot, motherLogical);
  delete shortSolidRightLocalPos;
  delete shortSolidRightLocalRot;
  delete shortSolidRightAffine;

  G4ThreeVector *longSolidTopLocalPos = new G4ThreeVector(0, -(-shortSolidTop_l*mm + panel_w*mm), shortSolidTop_h*mm+offset);
  G4RotationMatrix *longSolidTopLocalRot = new G4RotationMatrix();
  longSolidTopLocalRot->rotateZ(180*deg);
  longSolidTopLocalRot->rotateX(90*deg);
  G4AffineTransform *longSolidTopAffine = new G4AffineTransform(longSolidTopLocalRot,*longSolidTopLocalPos);
  *longSolidTopAffine*=*assemAffine;  
  G4ThreeVector *longSolidTopGlobalPos = new G4ThreeVector(longSolidTopAffine->NetTranslation());
  G4RotationMatrix *longSolidTopGlobalRot= new G4RotationMatrix(longSolidTopAffine->NetRotation());
  if(northPanels) fLongSolidTopPtr->Place(longSolidTopGlobalPos, longSolidTopGlobalRot, motherLogical);
  delete longSolidTopLocalPos;
  delete longSolidTopLocalRot;
  delete longSolidTopAffine;

  G4ThreeVector *longSolidBottomLocalPos = new G4ThreeVector(0, -(-shortSolidTop_l*mm + panel_w*mm), -shortSolidBottom_h*mm+offset);
  G4RotationMatrix *longSolidBottomLocalRot = new G4RotationMatrix();
  longSolidBottomLocalRot->rotateY(180*deg);
  longSolidBottomLocalRot->rotateX(90*deg);
  G4AffineTransform *longSolidBottomAffine = new G4AffineTransform(longSolidBottomLocalRot,*longSolidBottomLocalPos);
  *longSolidBottomAffine*=*assemAffine;  
  G4ThreeVector *longSolidBottomGlobalPos = new G4ThreeVector(longSolidBottomAffine->NetTranslation());
  G4RotationMatrix *longSolidBottomGlobalRot= new G4RotationMatrix(longSolidBottomAffine->NetRotation());
  if(northPanels) fLongSolidBottomPtr->Place(longSolidBottomGlobalPos, longSolidBottomGlobalRot, motherLogical);
  delete longSolidBottomLocalPos;
  delete longSolidBottomLocalRot;
  delete longSolidBottomAffine;

  G4ThreeVector *longSolidLeftLocalPos = new G4ThreeVector(longSolidRight_h*mm, -(-shortSolidTop_l*mm + 3.0*panel_w*mm), -4.0*panel_w*mm+offset);
  G4RotationMatrix *longSolidLeftLocalRot = new G4RotationMatrix();
  longSolidLeftLocalRot->rotateZ(180*deg);
  longSolidLeftLocalRot->rotateY(90*deg);
  longSolidLeftLocalRot->rotateX(90*deg);
  G4AffineTransform *longSolidLeftAffine = new G4AffineTransform(longSolidLeftLocalRot,*longSolidLeftLocalPos);
  *longSolidLeftAffine*=*assemAffine;  
  G4ThreeVector *longSolidLeftGlobalPos = new G4ThreeVector(longSolidLeftAffine->NetTranslation());
  G4RotationMatrix *longSolidLeftGlobalRot= new G4RotationMatrix(longSolidLeftAffine->NetRotation());
  if(northPanels) fLongSolidLeftPtr->Place(longSolidLeftGlobalPos, longSolidLeftGlobalRot, motherLogical);
  delete longSolidLeftLocalPos;
  delete longSolidLeftLocalRot;
  delete longSolidLeftAffine;

  G4ThreeVector *longSolidRightLocalPos = new G4ThreeVector(-longSolidLeft_h*mm, -(-shortSolidTop_l*mm + 3.0*panel_w*mm), -4.0*panel_w*mm+offset);
  G4RotationMatrix *longSolidRightLocalRot = new G4RotationMatrix();
  longSolidRightLocalRot->rotateY(90*deg);
  longSolidRightLocalRot->rotateX(90*deg);
  G4AffineTransform *longSolidRightAffine = new G4AffineTransform(longSolidRightLocalRot,*longSolidRightLocalPos);
  *longSolidRightAffine*=*assemAffine;  
  G4ThreeVector *longSolidRightGlobalPos = new G4ThreeVector(longSolidRightAffine->NetTranslation());
  G4RotationMatrix *longSolidRightGlobalRot= new G4RotationMatrix(longSolidRightAffine->NetRotation());
  if(northPanels) fLongSolidRightPtr->Place(longSolidRightGlobalPos, longSolidRightGlobalRot, motherLogical);
  delete longSolidRightLocalPos;
  delete longSolidRightLocalRot;
  delete longSolidRightAffine;


  G4ThreeVector *topOuter1LocalPos = new G4ThreeVector(topOuter_h*mm-3.0*panel_w*mm, 0, shortSolidLeft_l*mm - panel_w*mm+offset);
  G4RotationMatrix *topOuter1LocalRot = new G4RotationMatrix();
  topOuter1LocalRot->rotateZ(90*deg);
  G4AffineTransform *topOuter1Affine = new G4AffineTransform(topOuter1LocalRot,*topOuter1LocalPos);
  *topOuter1Affine*=*assemAffine;  
  G4ThreeVector *topOuter1GlobalPos = new G4ThreeVector(topOuter1Affine->NetTranslation());
  G4RotationMatrix *topOuter1GlobalRot= new G4RotationMatrix(topOuter1Affine->NetRotation());
  if(topPanels) fTopOuter1Ptr->Place(topOuter1GlobalPos, topOuter1GlobalRot, motherLogical);
  delete topOuter1LocalPos;
  delete topOuter1LocalRot;
  delete topOuter1Affine;

  G4ThreeVector *topOuter2LocalPos = new G4ThreeVector(-topOuter_h*mm-3.0*panel_w*mm, 0, shortSolidLeft_l*mm - panel_w*mm+offset);
  G4RotationMatrix *topOuter2LocalRot = new G4RotationMatrix();
  topOuter2LocalRot->rotateZ(90*deg);
  G4AffineTransform *topOuter2Affine = new G4AffineTransform(topOuter2LocalRot,*topOuter2LocalPos);
  *topOuter2Affine*=*assemAffine;  
  G4ThreeVector *topOuter2GlobalPos = new G4ThreeVector(topOuter2Affine->NetTranslation());
  G4RotationMatrix *topOuter2GlobalRot= new G4RotationMatrix(topOuter2Affine->NetRotation());
  if(topPanels) fTopOuter2Ptr->Place(topOuter2GlobalPos, topOuter2GlobalRot, motherLogical);
  delete topOuter2LocalPos;
  delete topOuter2LocalRot;
  delete topOuter2Affine;

  G4ThreeVector *topInner1LocalPos = new G4ThreeVector(0, -topInner_h*mm, shortSolidLeft_l*mm-3.0*panel_w*mm+offset);
  G4RotationMatrix *topInner1LocalRot = new G4RotationMatrix();
  G4AffineTransform *topInner1Affine = new G4AffineTransform(topInner1LocalRot,*topInner1LocalPos);
  *topInner1Affine*=*assemAffine;  
  G4ThreeVector *topInner1GlobalPos = new G4ThreeVector(topInner1Affine->NetTranslation());
  G4RotationMatrix *topInner1GlobalRot= new G4RotationMatrix(topInner1Affine->NetRotation());
  if(topPanels) fTopInner1Ptr->Place(topInner1GlobalPos, topInner1GlobalRot, motherLogical);
  delete topInner1LocalPos;
  delete topInner1LocalRot;
  delete topInner1Affine;

  G4ThreeVector *topInner2LocalPos = new G4ThreeVector(0, topInner_h*mm, shortSolidLeft_l*mm-3.0*panel_w*mm+offset);
  G4RotationMatrix *topInner2LocalRot = new G4RotationMatrix();
  G4AffineTransform *topInner2Affine = new G4AffineTransform(topInner2LocalRot,*topInner2LocalPos);
  *topInner2Affine*=*assemAffine;  
  G4ThreeVector *topInner2GlobalPos = new G4ThreeVector(topInner2Affine->NetTranslation());
  G4RotationMatrix *topInner2GlobalRot= new G4RotationMatrix(topInner2Affine->NetRotation());
  if(topPanels) fTopInner2Ptr->Place(topInner2GlobalPos, topInner2GlobalRot, motherLogical);
  delete topInner2LocalPos;
  delete topInner2LocalRot;
  delete topInner2Affine;

//over floor panel assemblies

  G4double op_fr_h = in2mm*18.65*mm;
  G4double overFloorzOffset = 1.66*in2mm*mm;
  G4double overFloorzSpacing = 8.0*in2mm*mm;
  
  G4ThreeVector *overFloorAssmlocalPos1 = new G4ThreeVector(0, 0, -2.0*op_fr_h - overFloorzOffset);
  G4RotationMatrix *overFloorAssmlocalRot1 = new G4RotationMatrix();
  G4AffineTransform *overFloorAssmAffine1 = new G4AffineTransform(overFloorAssmlocalRot1,*overFloorAssmlocalPos1);
  *overFloorAssmAffine1*=*assemAffine;  
  G4ThreeVector *overFloorAssmglobalPos1 = new G4ThreeVector(overFloorAssmAffine1->NetTranslation());
  G4RotationMatrix *overFloorAssmglobalRot1= new G4RotationMatrix(overFloorAssmAffine1->NetRotation());
  if(bottomPanels) fOverFloorAssmPtr1->Place(overFloorAssmglobalPos1, overFloorAssmglobalRot1, motherLogical);
  delete overFloorAssmlocalPos1;
  delete overFloorAssmlocalRot1;
  delete overFloorAssmAffine1;
  
  G4ThreeVector *overFloorAssmlocalPos2 = new G4ThreeVector(0, 0, -2.0*op_fr_h-overFloorzOffset-overFloorzSpacing);
  G4RotationMatrix *overFloorAssmlocalRot2 = new G4RotationMatrix();
  overFloorAssmlocalRot2->rotateZ(90*deg);
  G4AffineTransform *overFloorAssmAffine2 = new G4AffineTransform(overFloorAssmlocalRot2,*overFloorAssmlocalPos2);
  *overFloorAssmAffine2*=*assemAffine;  
  G4ThreeVector *overFloorAssmglobalPos2 = new G4ThreeVector(overFloorAssmAffine2->NetTranslation());
  G4RotationMatrix *overFloorAssmglobalRot2= new G4RotationMatrix(overFloorAssmAffine2->NetRotation());
  if(bottomPanels) fOverFloorAssmPtr2->Place(overFloorAssmglobalPos2, overFloorAssmglobalRot2, motherLogical);
  delete overFloorAssmlocalPos2;
  delete overFloorAssmlocalRot2;
  delete overFloorAssmAffine2;


}
