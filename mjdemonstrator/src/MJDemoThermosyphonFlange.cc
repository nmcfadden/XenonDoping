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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonFlange.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  coaxial with tube section, co-planer with mate to corner piece
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@gmail.com
 * FIRST SUBMISSION: July 31, 2014
 * 
 * REVISION:
 * 
 * 31-07-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh" 
#include "G4LogicalVolumeStore.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoThermosyphonFlange.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonFlange::MJDemoThermosyphonFlange(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-341-R000", "SS303")
{;}

MJDemoThermosyphonFlange::MJDemoThermosyphonFlange(const MJDemoThermosyphonFlange & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermosyphonFlange::~MJDemoThermosyphonFlange()
{;}

G4LogicalVolume* MJDemoThermosyphonFlange::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4double zPlanes[] = {0*25.4*mm, 0.6*25.4*mm, 0.6*25.4*mm, 1.07*25.4*mm};
    G4double rInner[] = {1.0/2*25.4*mm, 1.0/2*25.4*mm, 0.875/2*25.4*mm, 0.875/2*25.4*mm};
    G4double rOuter[] = {1.341/2*25.4*mm, 1.341/2*25.4*mm, 2.125/2*25.4*mm, 2.125/2*25.4*mm};
    G4Polycone* body = new G4Polycone("body", 0, 2*pi, 4, zPlanes, rInner, rOuter);
    
    G4Tubs* bolthole = new G4Tubs("bolthole", 0, 0.1141*25.4*mm, 1.5*25.4*mm, 0, 2*pi);

    G4RotationMatrix* rotation = new G4RotationMatrix();
    G4SubtractionSolid* partSub = new G4SubtractionSolid("hole1", body, bolthole, rotation, G4ThreeVector(0.8125*25.4*mm, 0, 0));
    partSub = new G4SubtractionSolid("hole1", partSub, bolthole, rotation, G4ThreeVector(-0.8125*25.4*mm, 0, 0));
    partSub = new G4SubtractionSolid("hole1", partSub, bolthole, rotation, G4ThreeVector(0, 0.8125*25.4*mm, 0));
    partSub = new G4SubtractionSolid("hole1", partSub, bolthole, rotation, G4ThreeVector(0, -0.8125*25.4*mm, 0));
      
      
    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(partSub, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Thermosyphon Stainless Flange Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermosyphon Stainless Flange Logical" << endlog;
  return pVol;
}  
