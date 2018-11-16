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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonTubeAdapter.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  box center of mass
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Oct 21, 2010
 * 
 * REVISION:
 * 
 * 10-21-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoThermosyphonTubeAdapter.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonTubeAdapter::MJDemoThermosyphonTubeAdapter(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-321-R004", "Copper-EF")
{;}

MJDemoThermosyphonTubeAdapter::MJDemoThermosyphonTubeAdapter(const MJDemoThermosyphonTubeAdapter & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermosyphonTubeAdapter::~MJDemoThermosyphonTubeAdapter()
{;}

G4LogicalVolume* MJDemoThermosyphonTubeAdapter::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* body = new G4Box("body", 0.12*25.4*mm, 50.8/2*mm, 38.1/2*mm);

    G4Box* boxCut = new G4Box("boxCut", 0.08*25.4*mm, 1.1*25.4*mm, 0.0206*25.4*mm);
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, boxCut, 0,
    										G4ThreeVector(-0.12*25.4*mm, 0, 0.760*25.4*mm));
    
    G4Tubs* cylCut = new G4Tubs("cylCut", 0, 0.5*25.4*mm, 0.5*25.4*mm, 0, 2*pi);
    G4RotationMatrix* rotation1 = new G4RotationMatrix();
    rotation1->rotateY(pi/2);
    G4SubtractionSolid* tubeAdapter = new G4SubtractionSolid("tubeAdapter", body2, cylCut, rotation1,
    										G4ThreeVector(0, 0, 0.09*25.4*mm));
    
    								

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(tubeAdapter, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt); 
    MGLog(debugging) << "Created Thermosyphon Tube Adapter Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermosyphon Tube Adapter Logical" << endlog; 
  return pVol;
}  
