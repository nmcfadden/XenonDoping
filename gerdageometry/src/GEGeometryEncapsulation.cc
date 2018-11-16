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
 * AUTHOR: Sabine Hemmer
 * CONTACT: hemmer *at* pd *dot* infn *dot* it
 * FIRST SUBMISSION: 14-06-2011
 * 
 * REVISION:
 *
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"

#include "gerdageometry/GEGeometryEncapsulation.hh"       

// --------------------------------------------------------------------------- 

GEGeometryEncapsulation::GEGeometryEncapsulation(GEGeometryDetectorDB* detectordb)
{

  fCopperEncapsulationColumn = detectordb -> GetCopperEncapsulationColumn();
  fCopperEncapsulationRow = detectordb -> GetCopperEncapsulationRow();
  fCopperEncapsulationPosition     = detectordb -> GetCrystalPosition(fCopperEncapsulationColumn, fCopperEncapsulationRow); 

  fDetectorMotherVolume          = detectordb -> GetDetectorMotherVolume(); 
  fCopperEncapsulationLogical    = detectordb -> GetCopperEncapsulationLogical(); 

  fTeflonEncapsulationColumn = detectordb -> GetTeflonEncapsulationColumn();
  fTeflonEncapsulationRow = detectordb -> GetTeflonEncapsulationRow();
  fTeflonEncapsulationPosition     = detectordb -> GetCrystalPosition(fTeflonEncapsulationColumn, fTeflonEncapsulationRow); 

  fTeflonEncapsulationLogical    = detectordb -> GetTeflonEncapsulationLogical(); 
  fTeflonEncapsulationCopperLogical    = detectordb -> GetTeflonEncapsulationCopperLogical(); 
}

// --------------------------------------------------------------------------- 

GEGeometryEncapsulation::~GEGeometryEncapsulation()
{
    // Never delete Physical Volumes, this is done by Geant4
    // -----------------------------------------------------

} 

// --------------------------------------------------------------------------- 


void GEGeometryEncapsulation::ConstructCopperEncapsulation()
{
  
  G4ThreeVector    position; 
  char             volumename[40]; 

  // construct physical volume : copper encapsulation 

  position = GetCopperEncapsulationPosition(); 

  strcpy(volumename, "CopperEncapsulation");

  fCopperEncapsulationPhysical = new G4PVPlacement(0,
						   position, 
						   fCopperEncapsulationLogical, 
						   volumename,
						   fDetectorMotherVolume, 
						   false, 
						   0);   

} 

void GEGeometryEncapsulation::ConstructTeflonEncapsulation()
{
  
  G4ThreeVector    position; 
  char             volumename[40]; 

  // construct physical volume : teflon encapsulation 

  position = GetTeflonEncapsulationPosition(); 

  strcpy(volumename, "TeflonEncapsulation");

  fTeflonEncapsulationPhysical = new G4PVPlacement(0,
						   position, 
						   fTeflonEncapsulationLogical, 
						   volumename,
						   fDetectorMotherVolume, 
						   false, 
						   0);   


  // construct physical volume : copper part of teflon encapsulation 

  strcpy(volumename, "TeflonEncapsulationCopper");

  fTeflonEncapsulationCopperPhysical = new G4PVPlacement(0,
						   position, 
						   fTeflonEncapsulationCopperLogical, 
						   volumename,
						   fDetectorMotherVolume, 
						   false, 
						   0);   

} 

// --------------------------------------------------------------------------- 


