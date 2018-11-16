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
// $Id: MJGeometryUWLArGe.cc,v 1.1 2005-08-16 00:25:55 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryUWLArGe.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: J.Orrell
 * CONTACT: 
 * FIRST SUBMISSION: Fri Aug 12 14:01:32 PDT 2005
 * 
 * REVISION:
 *
 * 08-12-2005, Created, J. Orrell & R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"

#include "geometry/MGGeometryIdealCoaxCrystal.hh"
#include "io/MGLogger.hh"


#include "mjgeometry/MJGeometryUWLArGe.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometryUWLArGe::MJGeometryUWLArGe():
  MGGeometryDetector("JO"), fShieldMaterial(0)
{
  fIdealCoaxCrystal = new MGGeometryIdealCoaxCrystal("LArGe", true);
  fIdealCoaxCrystal->SetDefaults();
  SetDefaults();
}

//---------------------------------------------------------------------------//

MJGeometryUWLArGe::~MJGeometryUWLArGe()
{
  delete fIdealCoaxCrystal;
}

//---------------------------------------------------------------------------//

void MJGeometryUWLArGe::ConstructDetector()
{
  fIdealCoaxCrystal->ConstructDetector();
  
  G4Tubs *shieldCylinderSolid = new G4Tubs("shieldCylinderSolid", 0., fRadius, 
				      fHeight/2., 0*deg, 360.0 * deg);

  if(!(fShieldMaterial = G4Material::GetMaterial("Nitrogen-Liq"))) {
    MGLog(error) << "Material not found : " << fShieldMaterialName << endlog;
    MGLog(fatal) << endlog;
  }

  theDetectorLogical = new G4LogicalVolume(shieldCylinderSolid, 
							fShieldMaterial,
							"shieldLogical",
							0, 0, 0, true);

  new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                    fIdealCoaxCrystal->GetDetectorLogical(),
                    "coaxInShield", theDetectorLogical, false, 0);
}

//---------------------------------------------------------------------------//

void MJGeometryUWLArGe::SetDefaults()
{
  fRadius = 50.0 * cm;
  fHeight = 100.0 * cm;
  fShieldMaterialName = "Nitrogen-Liq";
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
