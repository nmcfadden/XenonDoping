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
// CLASS IMPLEMENTATION:  MJCharCollimator.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Collimator, box with subtracted cylinder
 * Part Origin: center of original box, (hx,hy,hz) from corner
 */
// 
// --------------------------------------------------------------------------//
/**
 * AUTHOR: A. Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 *
 * REVISION:
 * 
 * 07-22-2013, Created, A. Wasserman
 *
 */
// --------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJCharCollimator.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

MJCharCollimator::MJCharCollimator(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "Collimator", "Lead-Ain")
{;}

MJCharCollimator::MJCharCollimator(const MJCharCollimator & rhs) : 
  MJVDemoPart(rhs)
{;}

MJCharCollimator::~MJCharCollimator()
{;}

G4LogicalVolume* MJCharCollimator::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;

  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    // Define the dimensions of the block of lead
    G4double hx = 0.9 * 25.4 * mm; // half x
    G4double hy = 0.9 * 25.4 * mm; // half y
    G4double hz = 1.7 * 25.4 * mm; // half z
    G4Box* block = new G4Box("Block", hx, hy, hz);

    // Define the bore, subtracted from the block
    G4double boreRadius = 0.059 * 25.4 * mm;
    G4Tubs* bore = new G4Tubs("Bore", 0, boreRadius, hz + 1.0*mm, 0, 2*pi);

    G4SubtractionSolid* blockMinusBore = new G4SubtractionSolid("BlockMinusBore", block, bore);
    
    // Cut out an insert for the source
    G4double sourceHalfDepth = 0.1125 * 25.4 * mm;
    G4double sourceRadius = 0.505 * 25.4 * mm;
    G4Tubs* sourceInsert = new G4Tubs("SourceInsert", 0, sourceRadius, sourceHalfDepth, 0, 2*pi);

    G4RotationMatrix* rotate = new G4RotationMatrix;
    G4ThreeVector translate(0,0,1.5875*25.4*mm);
    G4SubtractionSolid* collimator = new G4SubtractionSolid("Collimator", blockMinusBore, sourceInsert, rotate, translate);

    G4VisAttributes* greyVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // grey
    greyVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(collimator, material, logicalName);
    pVol->SetVisAttributes(greyVisAtt);  
    MGLog(debugging) << "Created Collimator Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Collimator Logical" << endlog; 
  return pVol;
}  
