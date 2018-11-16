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
// $Id: MJGeometrySolidBlock.cc,v 1.5 2009-02-24 15:04:48 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometrySolidBlock.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Tue Aug 24 12:39:35 PDT 2004
 * 
 * REVISION:
 *
 * 08-24-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 * 02-24-2009, Removed deletion of viz attirbutes from desttructor. R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"

#include "geometry/MGGeometryGlobals.hh"
#include "mjgeometry/MJGeometrySolidBlockMessenger.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometrySolidBlock.hh"

using namespace CLHEP;
using namespace MaGe;


//---------------------------------------------------------------------------//

MJGeometrySolidBlock::MJGeometrySolidBlock(): MGGeometryDetector(""),
fActiveLogical(0), fActivePhysical(0)
{
  fMessenger = new MJGeometrySolidBlockMessenger(this);
  fMaterialName = "Germanium-Nat"; // Default material is natural germanium
  fEdgeLength = 2.0*m; // Default block is 2x2x2 m^3
  fActive = false; // Default is no active region
  fDeadLayer = false; // Default is no dead layer
  fDeadLayerDepth = 0.0 * cm; // which better be the same as zero depth
}

//---------------------------------------------------------------------------//

MJGeometrySolidBlock::~MJGeometrySolidBlock()
{
  delete fMessenger;
}

//---------------------------------------------------------------------------//

void MJGeometrySolidBlock::ConstructDetector()
{
  if(fDeadLayerDepth > 0.0*cm && fDeadLayerDepth < fEdgeLength/2) {
    SetActive(true); // can't have a dead layer without an active region
    MGLog(routine) << "Using active volume for SolidBlock" << endlog;
  }

  if(fDeadLayerDepth < -1.0e-9*cm || fDeadLayerDepth >= fEdgeLength/2) {
    SetActive(false);     //not a physical dead layer.  default to non-active
    MGLog(warning) << "Dead layer depth of " << fDeadLayerDepth << endlog;
    MGLog(warning) << "is not physical! Setting dead layer to zero." << endlog;
    SetDeadLayerDepth(0.0*cm); 
  }

  fDeadLayer = (fDeadLayerDepth > 0.0*cm); //
  MGLog(routine) << "Use of dead layer set to " << fDeadLayer << endlog;
  MGLog(routine) << "the dead layer is set at " << fDeadLayerDepth / cm 
  << " cm." << endlog;
  // Visualization attributes for block, and active region if necessary.
  G4VisAttributes *blockVisAtt = new G4VisAttributes( blue );
  blockVisAtt->SetVisibility( true );
  blockVisAtt->SetForceSolid( false );
  if(fDeadLayer) {
    activeblockVisAtt = new G4VisAttributes( green );
    activeblockVisAtt->SetVisibility( true );
    activeblockVisAtt->SetForceSolid( false );
  }

  G4Material *boxMaterial = G4Material::GetMaterial(fMaterialName);
  if(!boxMaterial) {
    MGLog(error) << "Material " <<fMaterialName<<" not in database!"<<endlog;
    MGLog(fatal) << endlog;
  }

  // if active is set, then active block has "active" in LogicalVolume Name
  // First make outer box(theDetectorLogical)
  G4Box *theBox = new G4Box("solidBlock", fEdgeLength/2.0, fEdgeLength/2.0, fEdgeLength/2.0);
  if(!fActive){ // no dead layer, no active volume
    theDetectorLogical = new G4LogicalVolume(theBox, boxMaterial, "block");
  } else if(!fDeadLayer) { 
    // active volume, but no dead layer
    theDetectorLogical = new G4LogicalVolume(theBox, boxMaterial, "activeblock");
  } else { 
    // active layer and dead layer.  build an active box and place it in outer
    // box 
    theDetectorLogical = new G4LogicalVolume(theBox, boxMaterial, "block");
    G4double ActiveLength = fEdgeLength - 2.0*fDeadLayerDepth;

    G4Box *theActiveBox = new G4Box("solidActiveBlock", 
                                    ActiveLength/2.0, 
                                    ActiveLength/2.0, 
                                    ActiveLength/2.0);

    fActiveLogical = new G4LogicalVolume(theActiveBox, 
                                         boxMaterial, 
                                         "activeblock");

    fActivePhysical = new G4PVPlacement(0,G4ThreeVector(0,0,0),
                                        fActiveLogical, "ActivePhysical",
                                        theDetectorLogical,
                                        false,
                                        0);

  }

  //visualization
  theDetectorLogical->SetVisAttributes(blockVisAtt);
  if(fDeadLayer) fActiveLogical->SetVisAttributes(activeblockVisAtt);

  MGLog(routine) << "Solid Block created out of: " << fMaterialName << endlog;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

