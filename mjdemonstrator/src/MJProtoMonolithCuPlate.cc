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
//      
// CLASS IMPLEMENTATION:  MJProtoMonolithCuPlate.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Drawing No: MJ93-00-011 rev0
 * Assembly origin:  .
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday March 26, 2014
 * 
 * REVISION:
 * 03-26-2014, Created; adapted from MJGeometryDemonstrator, J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJProtoMonolithCuPlate.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJProtoMonolithCuPlate::MJProtoMonolithCuPlate(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ93-00-011-rev0", "Copper-OFHC") //partName, serialNumber, drawingNumber, material
{;}

MJProtoMonolithCuPlate::MJProtoMonolithCuPlate(const MJProtoMonolithCuPlate & rhs) : 
  MJVDemoPart(rhs)
{;}

MJProtoMonolithCuPlate::~MJProtoMonolithCuPlate()
{;}

G4LogicalVolume* MJProtoMonolithCuPlate::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
  
    G4double eps = 0.001 * mm;
     
    G4Box* topThinPlate = new G4Box("topThinPlate", 12.4968*mm, 250.825*mm, 106.058*mm);
    G4Box* topThickPlate = new G4Box("topThickPlate", (12.5095+0.1)*mm, 250.825*mm, 92.5703*mm);
    G4UnionSolid* topPlate = new G4UnionSolid("topPlate", topThinPlate, topThickPlate,
    										0, G4ThreeVector((-(12.4968+12.5095+0.1)+0.2)*mm, 0*mm, (106.058-92.5703)*mm));
    										
    G4Tubs* thermoCircleCut = new G4Tubs("thermoCircleCut", 0*mm, 45.212*mm, 26.0*mm, 0, 2*pi);
    G4Box* thermoBoxCut = new G4Box("thermoBoxCut", 46.0*mm, 45.212*mm, 27.0*mm);
    G4UnionSolid* thermoCut = new G4UnionSolid("thermoCut", thermoCircleCut, thermoBoxCut,
    										0, G4ThreeVector(46.0*mm, 0*mm, 0*mm));
 	G4RotationMatrix* rotationThermoCut = new G4RotationMatrix();
    rotationThermoCut->rotateY(-pi/2);									
    G4SubtractionSolid* tPwTC = new G4SubtractionSolid("tPwTC", topPlate, thermoCut, 
    										rotationThermoCut, G4ThreeVector(-12.5*mm, 0*mm, (-106.058+26.1874)*mm));
    
    G4Tubs* holeTypeA = new G4Tubs("holeTypeA", 0*mm, 3.9751*mm, 100.0*mm, 0, 2*pi);
    G4UnionSolid* hTA_4inSpace = new G4UnionSolid("hTA_4inSpace", holeTypeA, holeTypeA,
    										0, G4ThreeVector(0*mm, 101.6*mm, 0*mm));
    G4UnionSolid* hTA_6p5inSpace = new G4UnionSolid("hTA_6p5inSpace", holeTypeA, holeTypeA,
    										0, G4ThreeVector(0*mm, 165.1*mm, 0*mm));
    G4UnionSolid* hTA_2inSpace = new G4UnionSolid("hTA_2inSpace", holeTypeA, holeTypeA,
    										0, G4ThreeVector(0*mm, 50.8*mm, 0*mm));
    G4UnionSolid* hTA_1inStacked = new G4UnionSolid("hTA_1inStacked", holeTypeA, holeTypeA,
    										0, G4ThreeVector(-25.4*mm, 0*mm, 0*mm));
  	G4Tubs* holeTypeB = new G4Tubs("holeTypeB", 0*mm, 4.3688*mm, 100.0*mm, 0, 2*pi);
    G4UnionSolid* hTB_4inSpace = new G4UnionSolid("hTB_4inSpace", holeTypeB, holeTypeB,
    										0, G4ThreeVector(0*mm, 101.6*mm, 0*mm));  
    G4Tubs* holeTypeC = new G4Tubs("holeTypeC", 0*mm, 10.3251*mm, 100.0*mm, 0, 2*pi);	
    G4UnionSolid* hTC_4inSpace = new G4UnionSolid("hTC_4inSpace", holeTypeC, holeTypeC,
    										0, G4ThreeVector(0*mm, 101.6*mm, 0*mm));      
  	G4RotationMatrix* rotationHoles = new G4RotationMatrix();
    rotationHoles->rotateY(-pi/2);   
    																				
	G4SubtractionSolid* tPwHRB = new G4SubtractionSolid("tPwHRB", tPwTC, hTA_4inSpace,
											rotationHoles, G4ThreeVector(0*mm, 98.425*mm, (-106.058+12.7)*mm));
	G4SubtractionSolid* tPwHLB = new G4SubtractionSolid("tPwHLB", tPwHRB, hTA_4inSpace,
											rotationHoles, G4ThreeVector(0*mm, -(98.425+101.6)*mm, (-106.058+12.7)*mm));
	G4SubtractionSolid* tPwHRT = new G4SubtractionSolid("tPwHRT", tPwHLB, hTA_6p5inSpace,
											rotationHoles, G4ThreeVector((100.0+12.4968-38.1)*mm, 
											25.4*mm, (-106.058+116.865)*mm));
	G4SubtractionSolid* tPwHLT = new G4SubtractionSolid("tPwHLT", tPwHRT, hTA_6p5inSpace,
											rotationHoles, G4ThreeVector((100.0+12.4968-38.1)*mm, 
											-(25.4+165.1)*mm, (-106.058+116.865)*mm));
	G4SubtractionSolid* tPwHBR = new G4SubtractionSolid("tPwHBR", tPwHLT, hTA_2inSpace,
											rotationHoles, G4ThreeVector(-(100.0+12.4968+12.5095*2-38.1)*mm, 
											180.975*mm, (-106.058+186.715)*mm));
	G4SubtractionSolid* tPwHBL = new G4SubtractionSolid("tPwHBL", tPwHBR, hTA_2inSpace,
											rotationHoles, G4ThreeVector(-(100.0+12.4968+12.5095*2-38.1)*mm, 
											-(180.975+50.8)*mm, (-106.058+186.715)*mm));

    G4Box* bottomThinPlate = new G4Box("topThinPlate", 12.4968*mm, 250.825*mm, 205.105*mm);
    G4Box* bottomThickPlate = new G4Box("topThickPlate", (12.5095+0.1)*mm, 250.825*mm, 192.405*mm);
    G4UnionSolid* bottomPlate = new G4UnionSolid("bottomPlate", bottomThinPlate, bottomThickPlate,
    										0, G4ThreeVector((-(12.4968+12.5095+0.1)+0.2)*mm, 0*mm, -(205.105-192.405)*mm));
    G4SubtractionSolid* bPwTC = new G4SubtractionSolid("bPwTC", bottomPlate, thermoCircleCut, 
    										rotationThermoCut, G4ThreeVector(-12.5*mm, 0*mm, 205.105*mm));

	G4SubtractionSolid* bPwHRB = new G4SubtractionSolid("bPwHRB", bPwTC, hTA_2inSpace,
											rotationHoles, G4ThreeVector((100.0+12.4968-38.1)*mm, 
											180.975*mm, (205.105-384.785)*mm));
	G4SubtractionSolid* bPwHLB = new G4SubtractionSolid("bPwHLB", bPwHRB, hTA_4inSpace,
											rotationHoles, G4ThreeVector((100.0+12.4968-38.1)*mm, 
											-(180.975+50.8)*mm, (205.105-384.785)*mm));
	G4SubtractionSolid* bPwHRM = new G4SubtractionSolid("bPwHRM", bPwHLB, hTA_2inSpace,
											rotationHoles, G4ThreeVector((100.0+12.4968-38.1)*mm, 
											180.975*mm, (205.105-384.785+241.3)*mm));
	G4SubtractionSolid* bPwHLM = new G4SubtractionSolid("bPwHLM", bPwHRM, hTA_4inSpace,
											rotationHoles, G4ThreeVector((100.0+12.4968-38.1)*mm, 
											-(180.975+50.8)*mm, (205.105-384.785+241.3)*mm));    
    G4SubtractionSolid* bPwHSR = new G4SubtractionSolid("bPwHSR", bPwHLM, hTA_1inStacked,
    										rotationHoles, G4ThreeVector((100.0+12.4968-38.1)*mm, 
    										206.375*mm, (205.105-314.935)*mm));
    G4SubtractionSolid* bPwHSL = new G4SubtractionSolid("bPwHSL", bPwHSR, hTA_1inStacked,
    										rotationHoles, G4ThreeVector((100.0+12.4968-38.1)*mm, 
    										-206.375*mm, (205.105-314.935)*mm));
   	G4SubtractionSolid* bPwHFR = new G4SubtractionSolid("bPwHFR", bPwHSL, hTA_6p5inSpace,
    										rotationHoles, G4ThreeVector(-(100.0+12.49685+12.5095*2-38.1)*mm, 
    										25.4*mm, (205.105-314.935)*mm));
   	G4SubtractionSolid* bPwHFL = new G4SubtractionSolid("bPwHFL", bPwHFR, hTA_6p5inSpace,
    										rotationHoles, G4ThreeVector(-(100.0+12.49685+12.5095*2-38.1)*mm, 
    										-(25.4+165.1)*mm, (205.105-314.935)*mm));
   	G4SubtractionSolid* bPwHRT = new G4SubtractionSolid("bPwHRT", bPwHFL, hTB_4inSpace,
    										rotationHoles, G4ThreeVector(0*mm, 98.425*mm, (205.105-12.7)*mm));
   	G4SubtractionSolid* bPwHLT = new G4SubtractionSolid("bPwHLT", bPwHRT, hTB_4inSpace,
    										rotationHoles, G4ThreeVector(0*mm, -(98.425+101.6)*mm, (205.105-12.7)*mm));
   	G4SubtractionSolid* bPwOHRT = new G4SubtractionSolid("bPwOHRT", bPwHLT, hTC_4inSpace,
    										rotationHoles, G4ThreeVector((100.0+12.4968-6.35)*mm, 
    										98.425*mm, (205.105-12.7)*mm));
   	G4SubtractionSolid* bPwOHLT = new G4SubtractionSolid("bPwOHLT", bPwOHRT, hTC_4inSpace,
    										rotationHoles, G4ThreeVector((100.0+12.4968-6.35)*mm, 
    										-(98.425+101.6)*mm, (205.105-12.7)*mm));
    
    G4Tubs *REsquareCircle = new G4Tubs("REsquareCircle", 0*mm, 6.4262*mm, 100.0*mm, 0, 2*pi);
    G4UnionSolid *twoCircles = new G4UnionSolid("twoCircle", REsquareCircle, REsquareCircle,
    										0, G4ThreeVector(0*mm, (19.05-2*6.4262)*mm, 0*mm));	
    G4UnionSolid *fourCircles = new G4UnionSolid("fourCircles", twoCircles, twoCircles,
    										0, G4ThreeVector((31.75-2*6.4262)*mm, 0*mm, 0*mm));										
    G4Box *REsquareHeight = new G4Box("REsquareHeight", 15.875*mm, (9.525-6.4262)*mm, 100*mm);
    G4Box *REsquareWidth = new G4Box("REsquareWidth", (15.875-6.4262)*mm, 9.525*mm, 100*mm);
    G4UnionSolid *fCwH = new G4UnionSolid("fCwH", REsquareHeight, fourCircles,
    										0, G4ThreeVector(-(15.875-6.4262)*mm, -(9.525-6.4262)*mm, 0*mm));
    G4UnionSolid *fCwHwW = new G4UnionSolid("fCwHwW", fCwH, REsquareWidth, 0, G4ThreeVector());
   
    G4SubtractionSolid *bPwAllHoles = new G4SubtractionSolid("bPwAllHoles", bPwOHLT, fCwHwW,
    										rotationHoles, G4ThreeVector(0*mm, (250.825-87.3252)*mm, (205.105-102.21)*mm));
    										
    G4RotationMatrix* rotationBottomPlate = new G4RotationMatrix();
    rotationBottomPlate->rotateZ(pi);									 
  	G4UnionSolid* monoCuPlate = new G4UnionSolid("monoCuPlate",tPwHBL, bPwAllHoles,
  											rotationBottomPlate, G4ThreeVector( 
  											(-2.*(12.4968+12.5095)+12.4968+12.4968)*mm, 0*mm, 
  											(-106.058+2.*(106.058-92.5703)-205.105)*mm-eps ) );									
                                                       
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(1.0,0.373,0.169,1.0)); //copper color "more red"
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(monoCuPlate, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Prototype Monolith Cu Plate Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Prototype Monolith Cu Plate Logical" << endlog; 
  return pVol;
}  

