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
// CLASS IMPLEMENTATION:  MJDemoRadShieldAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  COM of Shielding Box.
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Mary Kidd
 * CONTACT: mkidd@lanl.gov
 * FIRST SUBMISSION: never
 * 
 * REVISION:
 * 
 * 12-09-2010, Created, M. Kidd
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
#include "mjdemonstrator/MJDemoMuonVetoOverFloorAssembly.hh"
#include "mjdemonstrator/MJDemoMuonVetoOverFloorPanel.hh"

using namespace CLHEP;



//---------------------------------------------------------------------------//

MJDemoMuonVetoOverFloorAssembly::MJDemoMuonVetoOverFloorAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "MuonVetoOverFloor")
{

  fOverFloorPanelPtr1 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_001", "867-5309");
  fOverFloorPanelPtr2 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_002", "867-5309");
  fOverFloorPanelPtr3 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_003", "867-5309");
  fOverFloorPanelPtr4 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_004", "867-5309");
  fOverFloorPanelPtr5 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_005", "867-5309");
  fOverFloorPanelPtr6 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_006", "867-5309");

  }

MJDemoMuonVetoOverFloorAssembly::MJDemoMuonVetoOverFloorAssembly(const MJDemoMuonVetoOverFloorAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fOverFloorPanelPtr1 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_001", "867-5309");
  fOverFloorPanelPtr2 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_002", "867-5309");
  fOverFloorPanelPtr3 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_003", "867-5309");
  fOverFloorPanelPtr4 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_004", "867-5309");
  fOverFloorPanelPtr5 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_005", "867-5309");
  fOverFloorPanelPtr6 = new MJDemoMuonVetoOverFloorPanel(fName+"_ActiveMuonVetoOverFloorPanel_006", "867-5309");
}

MJDemoMuonVetoOverFloorAssembly::~MJDemoMuonVetoOverFloorAssembly()
{
  delete fOverFloorPanelPtr1;
  delete fOverFloorPanelPtr2;
  delete fOverFloorPanelPtr3;
  delete fOverFloorPanelPtr4;
  delete fOverFloorPanelPtr5;
  delete fOverFloorPanelPtr6;
}

void MJDemoMuonVetoOverFloorAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4double epsilon = 25.4*0.3*mm;  //this is the separation between panels = 0.3 inches

  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  G4ThreeVector *overFloorPanelLocalPos1 = new G4ThreeVector(2.0*5*(25.4*6.85*mm + epsilon)-5.0*(25.4*6.85*mm + epsilon), 0, 0);
  G4RotationMatrix *overFloorLocalRot1 = new G4RotationMatrix();
  overFloorLocalRot1->rotateZ(90.0*deg);
  G4AffineTransform *overFloorVetoAffine1 = new G4AffineTransform(overFloorLocalRot1,*overFloorPanelLocalPos1);
  *overFloorVetoAffine1*=*assemAffine;  
  G4ThreeVector *overFloorGlobalPos1 = new G4ThreeVector(overFloorVetoAffine1->NetTranslation());
  G4RotationMatrix *overFloorVetoGlobalRot1= new G4RotationMatrix(overFloorVetoAffine1->NetRotation());
  fOverFloorPanelPtr1->Place(overFloorGlobalPos1, overFloorVetoGlobalRot1, motherLogical);
  delete overFloorPanelLocalPos1;
  delete overFloorLocalRot1;
  delete overFloorVetoAffine1;

  G4ThreeVector *overFloorPanelLocalPos2 = new G4ThreeVector(2.0*4*(25.4*6.85*mm + epsilon)-5.0*(25.4*6.85*mm + epsilon), 0, 0);
  G4RotationMatrix *overFloorLocalRot2 = new G4RotationMatrix();
  overFloorLocalRot2->rotateZ(90.0*deg);
  G4AffineTransform *overFloorVetoAffine2 = new G4AffineTransform(overFloorLocalRot2,*overFloorPanelLocalPos2);
  *overFloorVetoAffine2*=*assemAffine;  
  G4ThreeVector *overFloorGlobalPos2 = new G4ThreeVector(overFloorVetoAffine2->NetTranslation());
  G4RotationMatrix *overFloorVetoGlobalRot2= new G4RotationMatrix(overFloorVetoAffine2->NetRotation());
  fOverFloorPanelPtr2->Place(overFloorGlobalPos2, overFloorVetoGlobalRot2, motherLogical);
  delete overFloorPanelLocalPos2;
  delete overFloorLocalRot2;
  delete overFloorVetoAffine2;

  G4ThreeVector *overFloorPanelLocalPos3 = new G4ThreeVector(2.0*3*(25.4*6.85*mm + epsilon)-5.0*(25.4*6.85*mm + epsilon), 0, 0);
  G4RotationMatrix *overFloorLocalRot3 = new G4RotationMatrix();
  overFloorLocalRot3->rotateZ(90.0*deg);
  G4AffineTransform *overFloorVetoAffine3 = new G4AffineTransform(overFloorLocalRot3,*overFloorPanelLocalPos3);
  *overFloorVetoAffine3*=*assemAffine;  
  G4ThreeVector *overFloorGlobalPos3 = new G4ThreeVector(overFloorVetoAffine3->NetTranslation());
  G4RotationMatrix *overFloorVetoGlobalRot3= new G4RotationMatrix(overFloorVetoAffine3->NetRotation());
  fOverFloorPanelPtr3->Place(overFloorGlobalPos3, overFloorVetoGlobalRot3, motherLogical);
  delete overFloorPanelLocalPos3;
  delete overFloorLocalRot3;
  delete overFloorVetoAffine3;

  G4ThreeVector *overFloorPanelLocalPos4 = new G4ThreeVector(2.0*2*(25.4*6.85*mm + epsilon)-5.0*(25.4*6.85*mm + epsilon), 0, 0);
  G4RotationMatrix *overFloorLocalRot4 = new G4RotationMatrix();
  overFloorLocalRot4->rotateZ(90.0*deg);
  G4AffineTransform *overFloorVetoAffine4 = new G4AffineTransform(overFloorLocalRot4,*overFloorPanelLocalPos4);
  *overFloorVetoAffine4*=*assemAffine;  
  G4ThreeVector *overFloorGlobalPos4 = new G4ThreeVector(overFloorVetoAffine4->NetTranslation());
  G4RotationMatrix *overFloorVetoGlobalRot4= new G4RotationMatrix(overFloorVetoAffine4->NetRotation());
  fOverFloorPanelPtr4->Place(overFloorGlobalPos4, overFloorVetoGlobalRot4, motherLogical);
  delete overFloorPanelLocalPos4;
  delete overFloorLocalRot4;
  delete overFloorVetoAffine4;

  G4ThreeVector *overFloorPanelLocalPos5 = new G4ThreeVector(2.0*1*(25.4*6.85*mm + epsilon)-5.0*(25.4*6.85*mm + epsilon), 0, 0);
  G4RotationMatrix *overFloorLocalRot5 = new G4RotationMatrix();
  overFloorLocalRot5->rotateZ(90.0*deg);
  G4AffineTransform *overFloorVetoAffine5 = new G4AffineTransform(overFloorLocalRot5,*overFloorPanelLocalPos5);
  *overFloorVetoAffine5*=*assemAffine;  
  G4ThreeVector *overFloorGlobalPos5 = new G4ThreeVector(overFloorVetoAffine5->NetTranslation());
  G4RotationMatrix *overFloorVetoGlobalRot5= new G4RotationMatrix(overFloorVetoAffine5->NetRotation());
  fOverFloorPanelPtr5->Place(overFloorGlobalPos5, overFloorVetoGlobalRot5, motherLogical);
  delete overFloorPanelLocalPos5;
  delete overFloorLocalRot5;
  delete overFloorVetoAffine5;

  G4ThreeVector *overFloorPanelLocalPos6 = new G4ThreeVector(2.0*0*(25.4*6.85*mm + epsilon)-5.0*(25.4*6.85*mm + epsilon), 0, 0);
  G4RotationMatrix *overFloorLocalRot6 = new G4RotationMatrix();
  overFloorLocalRot6->rotateZ(90.0*deg);
  G4AffineTransform *overFloorVetoAffine6 = new G4AffineTransform(overFloorLocalRot6,*overFloorPanelLocalPos6);
  *overFloorVetoAffine6*=*assemAffine;  
  G4ThreeVector *overFloorGlobalPos6 = new G4ThreeVector(overFloorVetoAffine6->NetTranslation());
  G4RotationMatrix *overFloorVetoGlobalRot6= new G4RotationMatrix(overFloorVetoAffine6->NetRotation());
  fOverFloorPanelPtr6->Place(overFloorGlobalPos6, overFloorVetoGlobalRot6, motherLogical);
  delete overFloorPanelLocalPos6;
  delete overFloorLocalRot6;
  delete overFloorVetoAffine6;
}
