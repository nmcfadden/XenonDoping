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
// CLASS IMPLEMENTATION:  MJDemoHVConnector.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Part Origin:  Center of cold plate mating face
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jan 14, 2011
 * 
 * REVISION:
 * 
 * 07-22-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4DisplacedSolid.hh"
#include "G4SubtractionSolid.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoHVConnector.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoHVConnector::MJDemoHVConnector(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "HVConnectorDWG", "Teflon")
{;}

MJDemoHVConnector::MJDemoHVConnector(const MJDemoHVConnector & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoHVConnector::~MJDemoHVConnector()
{;}

G4LogicalVolume* MJDemoHVConnector::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* body = new G4Box("body", 1.125*25.4*mm, 0.2525*25.4*mm, 0.234*25.4*mm);

    G4Box* cornerCut = new G4Box("cornerCut", 0.3623*25.4*mm, 0.3*25.4*mm, 0.375*25.4*mm);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, cornerCut, 0,
    										G4ThreeVector(1.125*25.4*mm, 0, 0.234*25.4*mm));
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, cornerCut, 0,
    										G4ThreeVector(-1.125*25.4*mm, 0, 0.234*25.4*mm));
    										
    G4DisplacedSolid* body4 = new G4DisplacedSolid("body4", body3, 0, 
    										G4ThreeVector(0, 0, 0.234*25.4*mm));

    G4Tubs* thruHole = new G4Tubs("thruHole", 0, 0.094*25.4*mm, 0.3*25.4*mm, 0, 2*pi);
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateX(pi/2);
    G4SubtractionSolid* body5 = new G4SubtractionSolid("body5", body4, thruHole, rotation,
    										G4ThreeVector(-0.468*25.4*mm, 0, 0.1403*25.4*mm));
    G4SubtractionSolid* body6 = new G4SubtractionSolid("body6", body5, thruHole, rotation,
    										G4ThreeVector(0.468*25.4*mm, 0, 0.1403*25.4*mm));
    G4SubtractionSolid* body7 = new G4SubtractionSolid("body7", body6, thruHole, rotation,
    										G4ThreeVector(-0.156*25.4*mm, 0, 0.1403*25.4*mm));    										
    G4SubtractionSolid* HVConnector = new G4SubtractionSolid("HVConnector", body7, thruHole, rotation,
    										G4ThreeVector(0.156*25.4*mm, 0, 0.1403*25.4*mm));    										
    										
    										
 
    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.7,0.7,0.7)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(HVConnector, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created HV Connector Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing HV Connector Logical" << endlog; 
  return pVol;
}  
