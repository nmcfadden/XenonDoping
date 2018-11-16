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
// CLASS IMPLEMENTATION:  MJDemoHVTwist.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Solid Work Drawing: hv twist v 3 blind.sldasm  
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR:  Wenqin Xu 
 * CONTACT: wxu@lanl.gov
 * FIRST SUBMISSION: Jan 09, 2013 
 * 
 * REVISION:
 * 
 * 01-09-2013, Created, W. Xu
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4Trd.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoHVTwist.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoHVTwist::MJDemoHVTwist(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "HVTwist Solid Work", "Teflon")
{;}
//Wenqin: Cannot find PTFE, so use Teflon for now

MJDemoHVTwist::MJDemoHVTwist(const MJDemoHVTwist & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoHVTwist::~MJDemoHVTwist()
{;}

G4LogicalVolume* MJDemoHVTwist::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){    
    G4Box * HV_twist_base = new G4Box("HV_twist_base",0.895*25.4*mm,0.2525*25.4*mm,0.0465*25.4*mm); //x/2, y/2, z/2, X is the wider side
    G4Trd * HV_twist_body = new G4Trd("HV_twist_body",0.583*25.4*mm,0.5075*25.4*mm,0.2525*25.4*mm,0.2525*25.4*mm,0.146*25.4*mm);
    G4Trd * HV_twist_top  = new G4Trd("HV_twist_top", 0.5075*25.4*mm,0.4825*25.4*mm,0.2525*25.4*mm,0.1595*25.4*mm,0.0465*25.4*mm);
    G4UnionSolid* HV_twist1= new G4UnionSolid("HV_twist1",HV_twist_base,HV_twist_body,0,G4ThreeVector(0,0,(0.0465+0.146)*25.4*mm));
    G4UnionSolid* HV_twist2= new G4UnionSolid("HV_twist2",HV_twist1,HV_twist_top,0,G4ThreeVector(0,0,(0.0465+0.146*2+0.0465)*25.4*mm));


    G4Tubs * tappedhole = new G4Tubs ("tappedhole",0,0.0725*25.4*mm,1*25.4*mm,0,2*pi);	 
    //Wenqin: Hole Major diameter = 0.155", minor D=0.135, so pitch D = 0.145"
    //Wenqin: from ColdPlate.cc, the bolt major diameter = 0.138", minor diameter = 0.098", so pitch diameter = 0.118

    G4SubtractionSolid* HV_twist3= new G4SubtractionSolid("HV_twist3",HV_twist2,tappedhole,0,G4ThreeVector(0.77*25.4*mm,0,0)); 
    G4SubtractionSolid* HV_twist4= new G4SubtractionSolid("HV_twist4",HV_twist3,tappedhole,0,G4ThreeVector(-0.77*25.4*mm,0,0)); 
    //Wenqin: hole to hole distence = 1.54"

    G4Tubs * edgecut = new G4Tubs ("edgecut",0.125*25.4*mm,0.2*25.4*mm,1*25.4*mm,0,0.5*pi); 
    G4SubtractionSolid* HV_twist5= new G4SubtractionSolid("HV_twist5",HV_twist4,edgecut,0,G4ThreeVector(0.77*25.4*mm,0.126*24.4*mm,0)); 


    G4RotationMatrix* cutRotation1 = new G4RotationMatrix();
    cutRotation1->rotateZ(90*deg);
    G4SubtractionSolid* HV_twist6= new G4SubtractionSolid("HV_twist6",HV_twist5,edgecut,cutRotation1,G4ThreeVector(0.77*25.4*mm,-0.126*24.4*mm,0));  

    cutRotation1->rotateZ(90*deg);
    G4SubtractionSolid* HV_twist7= new G4SubtractionSolid("HV_twist7",HV_twist6,edgecut,cutRotation1,G4ThreeVector(-0.77*25.4*mm,-0.126*24.4*mm,0));  

    cutRotation1->rotateZ(90*deg);
    G4SubtractionSolid* HV_twist8= new G4SubtractionSolid("HV_twist8",HV_twist7,edgecut,cutRotation1,G4ThreeVector(-0.77*25.4*mm,0.126*24.4*mm,0));  

    G4Tubs * toptub = new G4Tubs ("toptub",0,0.057*25.4*mm,0.0515*25.4*mm,0,2*pi);
    G4UnionSolid* HV_twist9 = new G4UnionSolid("HV_twist9", HV_twist8,toptub,0,G4ThreeVector(0,              -0.067*25.4*mm,(0.0515+0.146*2+0.0465+0.0465)*25.4*mm));
    G4UnionSolid* HV_twist10= new G4UnionSolid("HV_twist10",HV_twist9,toptub,0,G4ThreeVector(-0.197*25.4*mm, -0.067*25.4*mm,(0.0515+0.146*2+0.0465+0.0465)*25.4*mm));
    G4UnionSolid* HV_twist11= new G4UnionSolid("HV_twist11",HV_twist10,toptub,0,G4ThreeVector(0.197*25.4*mm, -0.067*25.4*mm,(0.0515+0.146*2+0.0465+0.0465)*25.4*mm));
    G4UnionSolid* HV_twist12= new G4UnionSolid("HV_twist12",HV_twist11,toptub,0,G4ThreeVector(-0.394*25.4*mm,-0.067*25.4*mm,(0.0515+0.146*2+0.0465+0.0465)*25.4*mm));
    G4UnionSolid* HV_twist13= new G4UnionSolid("HV_twist13",HV_twist12,toptub,0,G4ThreeVector(0.394*25.4*mm, -0.067*25.4*mm,(0.0515+0.146*2+0.0465+0.0465)*25.4*mm));


    //5 holes on the side is 0.445" deep, D=0.093". Note the body is only 0.2525*2=0.505" long along Y. 
    G4Tubs * sidehole = new G4Tubs ("sidehole",0,0.0465*25.4*mm,0.445*25.4*mm,0,2*pi); //The cut is 0.445", so make the hole has a center-to-edge length=0.445" 
    G4RotationMatrix* cutRotation2 = new G4RotationMatrix();
    cutRotation2->rotateX(90*deg);//horizontal
    G4SubtractionSolid* HV_twist14 = new G4SubtractionSolid("HV_twist14",HV_twist13,sidehole,cutRotation2,G4ThreeVector(             0, 0.2525*25.4*mm, (0.28-0.0465)*25.4*mm)); 
    G4SubtractionSolid* HV_twist15 = new G4SubtractionSolid("HV_twist15",HV_twist14,sidehole,cutRotation2,G4ThreeVector(-0.197*25.4*mm, 0.2525*25.4*mm, (0.28-0.0465)*25.4*mm)); 
    G4SubtractionSolid* HV_twist16 = new G4SubtractionSolid("HV_twist16",HV_twist15,sidehole,cutRotation2,G4ThreeVector( 0.197*25.4*mm, 0.2525*25.4*mm, (0.28-0.0465)*25.4*mm)); 
    G4SubtractionSolid* HV_twist17 = new G4SubtractionSolid("HV_twist17",HV_twist16,sidehole,cutRotation2,G4ThreeVector(-0.394*25.4*mm, 0.2525*25.4*mm, (0.28-0.0465)*25.4*mm)); 
    G4SubtractionSolid* HV_twist18 = new G4SubtractionSolid("HV_twist18",HV_twist17,sidehole,cutRotation2,G4ThreeVector( 0.394*25.4*mm, 0.2525*25.4*mm, (0.28-0.0465)*25.4*mm)); 


    G4Trd * bottomgroove = new G4Trd("bottomgroove",0.0365*25.4*mm,0.031*25.4*mm,5*25.4*mm,5*25.4*mm,0.03*25.4*mm);
    G4SubtractionSolid* HV_twist19 = new G4SubtractionSolid("HV_twist19",HV_twist18,bottomgroove,0,G4ThreeVector(             0, 0, (-0.0465)*25.4*mm)); 
    G4SubtractionSolid* HV_twist20 = new G4SubtractionSolid("HV_twist20",HV_twist19,bottomgroove,0,G4ThreeVector(-0.197*25.4*mm, 0, (-0.0465)*25.4*mm)); 
    G4SubtractionSolid* HV_twist21 = new G4SubtractionSolid("HV_twist21",HV_twist20,bottomgroove,0,G4ThreeVector( 0.197*25.4*mm, 0, (-0.0465)*25.4*mm)); 
    G4SubtractionSolid* HV_twist22 = new G4SubtractionSolid("HV_twist22",HV_twist21,bottomgroove,0,G4ThreeVector(-0.394*25.4*mm, 0, (-0.0465)*25.4*mm)); 
    G4SubtractionSolid* HV_twist23 = new G4SubtractionSolid("HV_twist23",HV_twist22,bottomgroove,0,G4ThreeVector( 0.394*25.4*mm, 0, (-0.0465)*25.4*mm)); 

    //Wenqin: All dimensions Checked. Note, the bottom is at -0.0465"z. (Facing the wider side of the Twist, with the tubes pointing to me)
    //Wenqin: Omission, the twisted tubes (10) and the small "metal" tubes inside each hole

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // gray, since it looks gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(HV_twist23, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created HV Twist Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing HV Twist Logical" << endlog; 
  return pVol;
}  
