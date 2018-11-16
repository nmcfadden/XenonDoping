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
// CLASS IMPLEMENTATION:  MJDemoCrystalInsulator.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Part Origin: Coaxial with hole, coincident with crystal surface
 *
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
#include "G4LogicalVolumeStore.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCrystalInsulator.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoCrystalInsulator::MJDemoCrystalInsulator(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-003", "Teflon")
{;}

MJDemoCrystalInsulator::MJDemoCrystalInsulator(const MJDemoCrystalInsulator & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCrystalInsulator::~MJDemoCrystalInsulator()
{;}

G4LogicalVolume* MJDemoCrystalInsulator::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = /*fPartName + "_" +*/ fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* body = new G4Box("body", 0.2218*25.4*mm, 0.1460*25.4*mm, 5*mm);    //5mm = 0.19685 * 25.4 * mm
    G4Tubs* bodyTurn = new G4Tubs("bodyTurn", 0.1750*25.4*mm, 4.5*25.4*mm, 0.25*25.4*mm, pi/2, pi);      
    G4SubtractionSolid* turnedBody = new G4SubtractionSolid("turnedBody", body, bodyTurn);
    
    G4Tubs* cylinderCut = new G4Tubs("cylinderCut", 0.1*25.4*mm, 1.3880*25.4*mm, 0.2*25.4*mm,
                                     -pi/2, pi);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", turnedBody, cylinderCut,
                                      0, G4ThreeVector(-1.35*25.4*mm, 0, -0.2*25.4*mm));                                     

    G4Tubs* snapFitting = new G4Tubs("snapFitting", 0, 0.1075*25.4*mm, 0.04*25.4*mm, 0, 2*pi);
    G4UnionSolid* bodyWithSnap = new G4UnionSolid("bodyWithSnap", body2, snapFitting, 0, 
                                                  G4ThreeVector(0, 0, (0.197+0.02)*25.4*mm));

    G4Tubs* thruHole = new G4Tubs("thruHole", 0, 0.085*25.4*mm, 0.26*25.4*mm, 0, 2*pi);
    G4SubtractionSolid *body3 = new G4SubtractionSolid("body3", bodyWithSnap, thruHole);

    G4Box* cornerCut = new G4Box("cornerCut", 0.08*25.4*mm, 0.25*25.4*mm, 0.08*25.4*mm);
    G4SubtractionSolid *body4 = new G4SubtractionSolid("body4", body3,
                                      cornerCut, 0, G4ThreeVector(0.2218*25.4*mm, 0, 5*mm));

    G4Box* chamfer = new G4Box("chamfer", 0.246*25.4*mm, 0.246*25.4*mm, 10*mm);
    G4RotationMatrix *rotation1 = new G4RotationMatrix();
    rotation1->rotateZ(pi/4);
    G4IntersectionSolid *crystalInsulatorOld = new G4IntersectionSolid("CrystalInsulatorOld", body4,
    			chamfer, rotation1, G4ThreeVector(0,0,0));

    G4Box* fitCut = new G4Box("fitCut", 0.025/2*25.4*mm, 0.155*25.4*mm, 10*mm); //Need to saw off the back so that the overlap with InnerCableGuide goes away
    G4SubtractionSolid *crystalInsulator = new G4SubtractionSolid("CrystalInsulator", crystalInsulatorOld, fitCut, 0, G4ThreeVector(0.2218*25.4*mm, 0, 0));

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(crystalInsulator, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Crystal Insulator Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Crystal Insulator Logical" << endlog; 
  return pVol;
}  
