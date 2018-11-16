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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonLNVolume.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  COM of box end
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
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoThermosyphonLNVolume.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonLNVolume::MJDemoThermosyphonLNVolume(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "ThermosyphonLNVolumeDwg", "Nitrogen-Liq")
{;}

MJDemoThermosyphonLNVolume::MJDemoThermosyphonLNVolume(const MJDemoThermosyphonLNVolume & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermosyphonLNVolume::~MJDemoThermosyphonLNVolume()
{;}

G4LogicalVolume* MJDemoThermosyphonLNVolume::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* tube = new G4Tubs("tube", 0, 0.545*25.4*mm, 20.0*25.4*mm, 0, 2*pi); //0.55
    G4Box* box = new G4Box("box", 2.077*25.4*mm, 0.698*25.4*mm, 0.448*25.4*mm);

    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateY(pi/2);
    G4UnionSolid* lnVolume = new G4UnionSolid("lnVolume", box, tube, rotation,
    									G4ThreeVector(22.360*25.4*mm, 0, 0.1013*25.4*mm));
    
    G4VisAttributes* blueVisAtt = new G4VisAttributes(G4Colour(0, 0, 0.8)); // blue
    blueVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(lnVolume, material, logicalName);
    pVol->SetVisAttributes(blueVisAtt); 
    MGLog(debugging) << "Created Thermosyphon LN Volume Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermosyphon LN Volume Logical" << endlog; 
  return pVol;
}  
