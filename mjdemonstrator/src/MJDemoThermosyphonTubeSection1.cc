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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonTubeSection1.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Box Center
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Max Hays
 * CONTACT: maxhays8@gmail.com
 * FIRST SUBMISSION: June 18, 2014
 * 
 * REVISION:
 * 
 * 18-06-2010, Created, M. Hays
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh" 
#include "G4LogicalVolumeStore.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4GenericTrap.hh"
#include "G4UnionSolid.hh"
#include "G4Trd.hh"
#include "G4Cons.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoThermosyphonTubeSection1.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonTubeSection1::MJDemoThermosyphonTubeSection1(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-343-R002", "Copper-EF")
{;}

MJDemoThermosyphonTubeSection1::MJDemoThermosyphonTubeSection1(const MJDemoThermosyphonTubeSection1 & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoThermosyphonTubeSection1::~MJDemoThermosyphonTubeSection1()
{;}

G4LogicalVolume* MJDemoThermosyphonTubeSection1::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4Tubs* mainCyl = new G4Tubs("Main Cyl", 25.4/2*mm, 34.061/2*mm, 450.367/2*mm, 0, 2*M_PI);
    G4Tubs* subCyl1 = new G4Tubs("Subtracted Cylinder 1", 0.0*mm, 26.416/2*mm, 3.0/2*mm, 0, 2*M_PI);
    G4Tubs* subCyl2 = new G4Tubs("Subtracted Cylinder 2", 29.9/2*mm, 35.0/2*mm, 10.0/2*mm, 0, 2*M_PI);

    G4RotationMatrix* rotN = new G4RotationMatrix();
    G4SubtractionSolid* subSolid;

    subSolid = new G4SubtractionSolid("Subtraction Solid", mainCyl, subCyl1, rotN, G4ThreeVector(0, 0, (450.367-2.286)/2*mm));
    subSolid = new G4SubtractionSolid("Subtraction Solid", subSolid, subCyl2, rotN, G4ThreeVector(0, 0, (450.367/2-145.567-4.318/2)*mm));
    subSolid = new G4SubtractionSolid("Subtraction Solid", subSolid, subCyl2, rotN, G4ThreeVector(0, 0, (450.367/2-372.389-4.318/2)*mm));

    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // New copper color
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(subSolid, material, logicalName);
    pVol->SetVisAttributes(copperVisAtt);
    MGLog(debugging) << "Created Thermosyphon Tube Section 1 Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Thermosyphon Tube Section 1 Logical" << endlog; 
  return pVol;
}  
