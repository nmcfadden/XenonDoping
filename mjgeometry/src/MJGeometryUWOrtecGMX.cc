//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
// $Id: MJGeometryUWOrtecGMX.cc,v 1.4 2008-08-24 22:39:10 alexis3 Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryUWOrtecGMX.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * An Ortec GMX CFG-GG 1.2L detector 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: alexis3
 * CONTACT: 
 * FIRST SUBMISSION: Tue Oct 16 14:36:17 PDT 2007
 * 
 * REVISION:
 *
 * 10-16-2007, Initial submission, A. Schubert
 * 05-22-2008, corrected outer dead layer - reduced by factor of 10^3,
 *             added methods to set height and diameter,  A. Schubert
 */
//---------------------------------------------------------------------------//
//

//#include <systemG4Stuff.hh>             //additional system Headers
//#include "globalsStuff.hh"              //additional Global Headers
//#include "G4Stuff.hh"		          //additional G4 Class Headers
//#include "MJStuff.hh"                   //additional MJ Headers

//---------------------------------------------------------------------------//
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4PVParameterised.hh"
#include "G4Color.hh"
#include "G4UnionSolid.hh"


#include "geometry/MGGeometryIdealCoaxCrystal.hh"
#include "mjgeometry/MJGeometryUWOrtecGMX.hh"      //Present MJ Class Headers 
#include "io/MGLogger.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometryUWOrtecGMX::MJGeometryUWOrtecGMX(G4String serNum):
  MGGeometryDetector(serNum),
  fCrystalCanCavity_phys(NULL),
  fCrystal_phys(NULL), 
  fCrystalCanCavity_log(NULL),
  fCrystalLength(70.7*mm),   // uw lab det #1
  fCrystalDiameter(64.8*mm)  // uw lab det #1 
{

}
//MJGeometryUWOrtecGMX::MJGeometryUWOrtecGMX(const MJGeometryUWOrtecGMX & other)
//{
//
//}

MJGeometryUWOrtecGMX::~MJGeometryUWOrtecGMX()
{
}

void MJGeometryUWOrtecGMX::ConstructDetector()
{
  //-------------------------------------------------------------------------//
  // MATERIALS:
  //-------------------------------------------------------------------------//
  G4Material *detectorBodyMaterial = G4Material::GetMaterial("Aluminum");
  G4Material *vacuum = G4Material::GetMaterial("Vacuum");

  if(!(detectorBodyMaterial && vacuum)) {
    MGLog(error) << "NULL G4Material pointer!" << endlog;
    MGLog(fatal) << endlog;
  }

  //-------------------------------------------------------------------------//
  // DIMENSIONS 
  //-------------------------------------------------------------------------//

  // can over crystal:
  G4double crystalCanRadius = 3.25/2*2.54*cm;
  G4double crystalCanHalfHeight = 5.25/2*2.54*cm; 
  G4double crystalCanThickness = 0.76*mm; 

  //cavity in can over crystal:
  G4double crystalCanCavityRadius = crystalCanRadius - crystalCanThickness;
  G4double crystalCanCavityHalfHeight = 
    crystalCanHalfHeight - crystalCanThickness; 

  // can over preamp:
  G4double preampCanRadius = 3.45/2*2.54*cm;
  G4double preampCanHalfHeight = 5.25/2*2.54*cm; 
  //G4double preampCanThickness = 1.0*mm; 
  
  // cryostat
  G4double cryostatRadius = 6.00/2*2.54*cm; // reduced by 0.25", ags 06 May 2008
  G4double cryostatHalfHeight = 9.0/2*2.54*cm;
  //G4double cryostatThickness = 4.0*mm;

  //-------------------------------------------------------------------------//
  // POSITIONS:
  //-------------------------------------------------------------------------//

  G4double crystalCanToCrystalZ = 3.0*mm;
  G4double crystalZ = crystalCanHalfHeight - fCrystalLength/2 - crystalCanToCrystalZ; 

  G4ThreeVector preampCanToCryostat_pos(0,0,-(preampCanHalfHeight+cryostatHalfHeight));
  G4ThreeVector crystalCanToPreampCan_pos(0,0,-(preampCanHalfHeight+crystalCanHalfHeight));
  G4ThreeVector ortecCrystal_pos(0,0,crystalZ);
  
  //-------------------------------------------------------------------------//
  // PLACEMENT:
  //-------------------------------------------------------------------------//


  // Al cans:
  G4Tubs* crystalCan = new G4Tubs("crystalCan",0,crystalCanRadius,
    crystalCanHalfHeight, 0*deg, 360*deg);
  G4Tubs* preampCan = new G4Tubs("preampCan",0,preampCanRadius,
    preampCanHalfHeight, 0*deg, 360*deg);
  G4Tubs* cryostat = new G4Tubs("cryostat",0,cryostatRadius,
    cryostatHalfHeight, 0*deg, 360*deg);

  G4UnionSolid *cryostatAndPreamp = new G4UnionSolid("cryostatAndPreamp",
    preampCan, cryostat,0,preampCanToCryostat_pos);
  G4UnionSolid *ortecBody = new G4UnionSolid("ortecBody",crystalCan,
    cryostatAndPreamp,0,crystalCanToPreampCan_pos);

  theDetectorLogical = new G4LogicalVolume(ortecBody, 
    detectorBodyMaterial, "ortecLogical",0, 0, 0, true);

  G4Tubs* crystalCanCavity = new G4Tubs("crystalCanCavity",0,
    crystalCanCavityRadius,crystalCanCavityHalfHeight, 0*deg, 360*deg);
  fCrystalCanCavity_log = new G4LogicalVolume(crystalCanCavity,
    vacuum, "fCrystalCanCavity_log",0,0,0);
  fCrystalCanCavity_phys = new G4PVPlacement(0,G4ThreeVector(), 
    fCrystalCanCavity_log,"crystalCanCavity",theDetectorLogical,false,0);

  
  MGGeometryIdealCoaxCrystal *ortecCrystal;

  ortecCrystal = new MGGeometryIdealCoaxCrystal(serialNumber, true);
  ortecCrystal->SetDefaults();
  ortecCrystal->SetRadius(fCrystalDiameter/2);
  ortecCrystal->SetHeight(fCrystalLength);
  ortecCrystal->SetCoreRadius(5.0*mm);
  ortecCrystal->SetCoreDepth(25.0*mm);
  ortecCrystal->SetCornerRadius(5.0*mm);
  ortecCrystal->SetDeadLayerConsidered(true);
  ortecCrystal->SetCoreDeadLayerThickness(750*micrometer);
  ortecCrystal->SetOuterDeadLayerThickness(0.3*micrometer);
  ortecCrystal->ConstructDetector();

  G4RotationMatrix* crystalRotation = new G4RotationMatrix;
  crystalRotation->rotateY(270*deg);

  fCrystal_phys = new G4PVPlacement(0,ortecCrystal_pos,
    ortecCrystal->GetDetectorLogical(),serialNumber,
    fCrystalCanCavity_log, false, 0);
}
