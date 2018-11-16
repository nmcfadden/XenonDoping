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
 * AUTHOR: Giovanni Benato
 * CONTACT: gbenato *at* physik *dot* uzh *dot* ch
 * FIRST SUBMISSION: 26-03-2012
 *
 * REVISION:
 * 23-04-2016 Modified mini-shrouds for Phase2, Janina
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
#include "G4SubtractionSolid.hh"
#include "G4PVPlacement.hh"

#include "gerdageometry/GEGeometryMiniShroud.hh"

// ---------------------------------------------------------------------------
GEGeometryMiniShroud::GEGeometryMiniShroud(GEGeometryDetectorDB* detectordb)
{
  fMiniShroudColumn = new std::vector<G4int>();
  fMiniShroudRow = new std::vector<G4int>();
  fMiniShroudPosition = new std::vector<G4ThreeVector>();

  fMiniShroudLogical = new std::vector<G4LogicalVolume*>();
  fMiniShroudPhysical = new std::vector<G4VPhysicalVolume*>();
  fMiniShroudPhase2TubLogical = new std::vector<G4LogicalVolume*>();
  fMiniShroudPhase2TubPhysical = new std::vector<G4VPhysicalVolume*>();
  fMiniShroudPhase2BottomLogical = new std::vector<G4LogicalVolume*>();
  fMiniShroudPhase2BottomPhysical = new std::vector<G4VPhysicalVolume*>();
  fMiniShroudPhase2TopLogical = new std::vector<G4LogicalVolume*>();
  fMiniShroudPhase2TopPhysical = new std::vector<G4VPhysicalVolume*>();
  // MS glued rings
  fMiniShroudPhase2TopGluedRingLogical = new std::vector<G4LogicalVolume*>();
  fMiniShroudPhase2TopGluedRingPhysical = new std::vector<G4VPhysicalVolume*>();
  fMiniShroudPhase2BottomGluedRingLogical = new std::vector<G4LogicalVolume*>();
  fMiniShroudPhase2BottomGluedRingPhysical = new std::vector<G4VPhysicalVolume*>();


  fMSMaterialFlag=detectordb-> GetFlagForMSMaterial();

  fMiniShroudColumn = detectordb->GetMiniShroudColumn();
  fMiniShroudRow = detectordb->GetMiniShroudRow();
  fMiniShroudPosition = detectordb->GetMiniShroudPosition();
  fMiniShroudPhase2TubPosition = detectordb->GetMiniShroudPhase2TubPosition();
  fMiniShroudPhase2BottomPosition = detectordb->GetMiniShroudPhase2BottomPosition();
  fMiniShroudPhase2TopPosition = detectordb->GetMiniShroudPhase2TopPosition();
  fMiniShroudPhase2TopGluedRingPosition = detectordb->GetMiniShroudPhase2TopGluedRingPosition();
  fMiniShroudPhase2BottomGluedRingPosition = detectordb->GetMiniShroudPhase2BottomGluedRingPosition();

  fDetectorMotherVolume = detectordb->GetDetectorMotherVolume();
  if (fMSMaterialFlag){
  fMiniShroudPhase2TubLogical = detectordb->GetMiniShroudPhase2TubLogical();
  fMiniShroudPhase2BottomLogical = detectordb->GetMiniShroudPhase2BottomLogical();
  fMiniShroudPhase2TopLogical = detectordb->GetMiniShroudPhase2TopLogical();
  fMiniShroudPhase2TopGluedRingLogical = detectordb->GetMiniShroudPhase2TopGluedRingLogical();
  fMiniShroudPhase2BottomGluedRingLogical = detectordb->GetMiniShroudPhase2BottomGluedRingLogical();
  }
  else fMiniShroudLogical = detectordb->GetMiniShroudLogical();



}

// ---------------------------------------------------------------------------

GEGeometryMiniShroud::~GEGeometryMiniShroud()
{
    // Never delete Physical Volumes, this is done by Geant4
    // -----------------------------------------------------

}

// ---------------------------------------------------------------------------


void GEGeometryMiniShroud::ConstructMiniShroud()
{

  G4int nMiniShrouds = fMiniShroudRow->size();

  for( G4int ii=0; ii<nMiniShrouds; ii++ ){

    if (!fMSMaterialFlag){//volume only for copper mini-shouds

    char index[7];
    sprintf( index, "%i", ii );

    std::string *PhysicalVolumeName = new std::string( "MiniShroud_" );
    PhysicalVolumeName->append( index );

    fMiniShroudPhysical->push_back( new G4PVPlacement( 0,
						       fMiniShroudPosition->at(ii),
						       fMiniShroudLogical->at(ii),
						       PhysicalVolumeName->c_str() ,
						       fDetectorMotherVolume,
						      false,
						      0) );
    }


    if (fMSMaterialFlag){//volume only for nylon mini-shouds

    char index1[7];
    sprintf( index1, "%i", ii );

    std::string *PhysicalVolumeName1 = new std::string( "MiniShroudPhase2Tub_" );
    PhysicalVolumeName1->append( index1 );

    fMiniShroudPhase2TubPhysical->push_back( new G4PVPlacement( 0,
						       fMiniShroudPhase2TubPosition->at(ii),
						       fMiniShroudPhase2TubLogical->at(ii),
						       PhysicalVolumeName1->c_str() ,
						       fDetectorMotherVolume,
						      false,
						      0) );

    std::string *PhysicalVolumeName2 = new std::string( "MiniShroudPhase2Bottom_" );
    PhysicalVolumeName2->append( index1 );

    fMiniShroudPhase2BottomPhysical->push_back( new G4PVPlacement( 0,
						       fMiniShroudPhase2BottomPosition->at(ii),
						       fMiniShroudPhase2BottomLogical->at(ii),
						       PhysicalVolumeName2->c_str() ,
						       fDetectorMotherVolume,
						      false,
						      0) );

    std::string *PhysicalVolumeName3 = new std::string( "MiniShroudPhase2Top_" );
    PhysicalVolumeName3->append( index1 );

    fMiniShroudPhase2TopPhysical->push_back( new G4PVPlacement( 0,
						       fMiniShroudPhase2TopPosition->at(ii),
						       fMiniShroudPhase2TopLogical->at(ii),
						       PhysicalVolumeName3->c_str() ,
						       fDetectorMotherVolume,
						      false,
						      0) );

    std::string *PhysicalVolumeName4 = new std::string( "MiniShroudPhase2TopGluedRing_" );
    PhysicalVolumeName4->append( index1 );

    fMiniShroudPhase2TopGluedRingPhysical->push_back( new G4PVPlacement( 0,
                                fMiniShroudPhase2TopGluedRingPosition->at(ii),
                                fMiniShroudPhase2TopGluedRingLogical->at(ii),
                                PhysicalVolumeName4->c_str(),
                                fDetectorMotherVolume,
                                false,
                                0) );

    std::string *PhysicalVolumeName5 = new std::string( "MiniShroudPhase2BottomGluedRing_" );
    PhysicalVolumeName5->append( index1 );

    fMiniShroudPhase2BottomGluedRingPhysical->push_back( new G4PVPlacement( 0,
                                fMiniShroudPhase2BottomGluedRingPosition->at(ii),
                                fMiniShroudPhase2BottomGluedRingLogical->at(ii),
                                PhysicalVolumeName5->c_str(),
                                fDetectorMotherVolume,
                                false,
                                0) );

    }



  }

  return;
}

// ---------------------------------------------------------------------------
