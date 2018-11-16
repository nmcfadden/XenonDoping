//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
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
// $Id: MJGeometryNaIBarrel.cc,v 1.5 2007-02-23 09:24:09 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryNaIBarrel.cc
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Thu Aug  5 06:58:48 PDT 2004
 * 
 * REVISION:
 *
 * 08-05-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 *
 */
//---------------------------------------------------------------------------//

//#include <CLHEP/Units/PhysicalConstants.h>

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

//---------------------------------------------------------------------------//

#include "geometry/MGGeometryGlobals.hh"
#include "mjgeometry/MJGeometryNaIBarrel.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;
using namespace MaGe;


//---------------------------------------------------------------------------//

MJGeometryNaIBarrel::MJGeometryNaIBarrel():
  fLogical(0)
{
  fLength = 22.0 * inch;
  fInnerRadius = 10.35 * inch / 2.0;
  fOuterRadius = 14.35 * inch / 2.0;
  fGapWidth = 0.5 * inch;
}

//---------------------------------------------------------------------------//

MJGeometryNaIBarrel::MJGeometryNaIBarrel(const MJGeometryNaIBarrel&)
{;}

//---------------------------------------------------------------------------//

MJGeometryNaIBarrel::~MJGeometryNaIBarrel()
{;}

//---------------------------------------------------------------------------//
void MJGeometryNaIBarrel::Construct()
{
  G4double phi = fGapWidth / fInnerRadius;
  G4Tubs *barrelSolid = new G4Tubs("barrelSolid", fInnerRadius, fOuterRadius,
				   fLength/2., phi * 0.5, 2 * pi - phi * 0.5);
  G4Material *naI = G4Material::GetMaterial("NaI");
  if(!naI) {
    MGLog(error) << "NaI not a defined G4Material." << endlog;
    MGLog(fatal) << endlog;
  }
  fLogical = new G4LogicalVolume(barrelSolid, naI, "barrelLogical");
  G4VisAttributes *barrelVisAtt = new G4VisAttributes(ltgray);
  barrelVisAtt->SetVisibility(true);
  barrelVisAtt->SetForceSolid(true);
  fLogical->SetVisAttributes(barrelVisAtt);
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
