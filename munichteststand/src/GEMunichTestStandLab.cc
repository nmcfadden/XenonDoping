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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 02-06-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4Material.hh"
#include "G4Box.hh"

#include "munichteststand/GEMunichTestStandLab.hh"

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEMunichTestStandLab::GEMunichTestStandLab()
{

  fLabBoxX = 2.5 * m; 
  fLabBoxY = 2.5 * m; 
  fLabBoxZ = 2.5 * m; 

}

// --------------------------------------------------------------------------- 

GEMunichTestStandLab::~GEMunichTestStandLab()
{

  delete fLabLogical; 

} 

// --------------------------------------------------------------------------- 

void GEMunichTestStandLab::ConstructLab()
{

  G4Material* mat_air = G4Material::GetMaterial("Air");

  G4Box* labbox = new G4Box("labbox",
			    fLabBoxX,
			    fLabBoxY,
			    fLabBoxZ);
  
  fLabLogical = new G4LogicalVolume(labbox, mat_air, "LabLogical");

} 

// --------------------------------------------------------------------------- 

