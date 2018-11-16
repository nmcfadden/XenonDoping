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
/**
 * SPECIAL NOTES:
 * Part origin:  Box Center
 *
 */ 
//---------------------------------------------------------------------------//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoConstrRadShieldN2Volume.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoConstrRadShieldN2Volume::MJDemoConstrRadShieldN2Volume(G4String partName, G4String serialNumber, G4bool innerCu) :
  MJVDemoPart(partName, serialNumber, "ShieldN2VolumeDwg", "Nitrogen-Gas"), fInnerCu(innerCu)
{;}

MJDemoConstrRadShieldN2Volume::MJDemoConstrRadShieldN2Volume(const MJDemoConstrRadShieldN2Volume & rhs) : 
  MJVDemoPart(rhs), fInnerCu(rhs.fInnerCu)
{;}

MJDemoConstrRadShieldN2Volume::~MJDemoConstrRadShieldN2Volume()
{;}

G4LogicalVolume* MJDemoConstrRadShieldN2Volume::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    
    G4double xDist, yDist, zDist;
    if(fInnerCu){
      xDist = 16.030*25.4*mm;
      yDist = 7.905*25.4*mm;
      zDist = 9.953*25.4*mm;
      }
    else{
      MGLog(routine)<<"The inner cavity is being made larger since there is no inner Cu shield"<<endl; 
      xDist = 18.030*25.4*mm;
      yDist = 10.030*25.4*mm;
      zDist = 12.030*25.4*mm;
      }  
    G4Box* body = new G4Box("body", xDist, yDist, zDist);
    
    G4Tubs* cryostatTube = new G4Tubs("cryostatTube", 0, 1.75*25.4*mm, 10.0*25.4*mm, 0, 2*pi);
    /*G4RotationMatrix* rotation1 = new G4RotationMatrix();
    rotation1->rotateY(pi/2);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, cryostatTube, rotation1,
    								G4ThreeVector(15.0*25.4*mm, 0, 4.4265*25.4*mm));
    G4RotationMatrix* rotation2 = new G4RotationMatrix();
    rotation2->rotateZ(pi/2);
    rotation2->rotateY(pi/2); 
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, cryostatTube, rotation2, 
    								G4ThreeVector(-8.1417*25.4*mm, -5*25.4*mm, 4.4265*25.4*mm));*/
 
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateY(pi/2); 
    G4SubtractionSolid* bodyMinusTube = new G4SubtractionSolid("bodyMinusTube", body, cryostatTube, rotation,
    								G4ThreeVector(15.0*25.4*mm, 0, 4.4265*25.4*mm));
 
    G4double zConePlanes[] = {-13.68*25.4*mm, -3.9*25.4*mm, -3.9*25.4*mm, -2.17*25.4*mm,
    							-2.17*25.4*mm, 1.99*25.4*mm, 1.99*25.4*mm, 3.7*25.4*mm,
    							3.7*25.4*mm, 4.76*25.4*mm};
    G4double rConeInner[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    G4double rConeOuter[] = {7.39*25.4*mm, 7.39*25.4*mm, 7.39*25.4*mm, 7.39*25.4*mm,
    							7.39*25.4*mm, 7.39*25.4*mm, 7.39*25.4*mm, 7.39*25.4*mm,
    							7.39*25.4*mm, 7.39*25.4*mm};
    G4Polycone* cryostat = new G4Polycone("cryostat", 0, 2*pi, 10, zConePlanes, rConeInner, rConeOuter);

    /*G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, cryostat, 0,
    								G4ThreeVector(8.1417*25.4*mm, 0, 4.4265*25.4*mm));

    G4SubtractionSolid* nitrogen = new G4SubtractionSolid("nitrogen", body4, cryostat, 0,
    								G4ThreeVector(-8.1417*25.4*mm, 0, 4.4265*25.4*mm));*/
    
    G4SubtractionSolid* nitrogen = new G4SubtractionSolid("nitrogen", bodyMinusTube, cryostat, 0,
    								G4ThreeVector(8.1417*25.4*mm, 0, 4.4265*25.4*mm));
   
   MGLog(routine)<<"Volume of inner cavity "<<nitrogen->GetCubicVolume()/(m*m*m)<<"m3"<<endl; 
    
    G4VisAttributes* whiteVisAtt = new G4VisAttributes(G4Colour(1, 1, 1)); // white
    whiteVisAtt->SetForceWireframe(true);
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(nitrogen, material, logicalName);
    pVol->SetVisAttributes(whiteVisAtt); 
    MGLog(debugging) << "Created N2 Volume Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing N2 Volume Logical" << endlog; 
  return pVol;
}  
