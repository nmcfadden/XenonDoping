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
// $Id: MJcodeTemplate.cc,v 1.3 2005-02-15 23:26:55 akbarm Exp $
//
// CLASS IMPLEMENTATION:  LGND_Part.cc
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
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: May 25 2017
 *
 * REVISION:
 *
 * 05-25-2017, Created, M. Green.
 *
 */
//---------------------------------------------------------------------------//
//

#include "G4String.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "legendgeometry/LGND_Part.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

LGND_Part::LGND_Part():
  fPartName(""), fSerialNumber(""), fDrawingNumber(""), fPartMaterial(""),
  fPartLogical(0), fPhysName(""), fPartPhysical(0),fPartPosition(0),
  fHasBeenPlaced(false)
{
  BuildLogicalVolume();
}

//---------------------------------------------------------------------------//

LGND_Part::LGND_Part(G4String partName, G4String serialNumber,
                         G4String drawingNumber, G4String material):
  fPartName(partName), fSerialNumber(serialNumber), fDrawingNumber(drawingNumber),
  fPartMaterial(material), fPartLogical(0), fPhysName(""),
  fPartPhysical(0), fHasBeenPlaced(false)
{BuildLogicalVolume();}

//---------------------------------------------------------------------------//

LGND_Part::LGND_Part(const LGND_Part & rhs):
  fPartName(rhs.fPartName),
  fSerialNumber(rhs.fSerialNumber),
  fDrawingNumber(rhs.fDrawingNumber),
  fPartMaterial(rhs.fPartMaterial),
  fPartLogical(rhs.fPartLogical),
  fPhysName(rhs.fPhysName),
  fPartPhysical(rhs.fPartPhysical),
  fHasBeenPlaced(rhs.fHasBeenPlaced)
{;}

//---------------------------------------------------------------------------//

LGND_Part::~LGND_Part()
{;}

//---------------------------------------------------------------------------//

void LGND_Part::Place(G4ThreeVector* partPosition, G4RotationMatrix* partRotation,
                        G4LogicalVolume* motherLogical)
{
  //Place the part in the Mother Logical Volume
  fPartLogical = this->BuildLogicalVolume();
  fPhysName = fPartName;
  fPartPhysical = new G4PVPlacement(partRotation, *partPosition, fPartLogical, fPhysName,
                                    motherLogical, false, 0, false);
  // cout<<">>>>>>>>>>> calling the overlapping <<<<<<<<<<<<<<<"<<endl;
  //fPartPhysical->CheckOverlaps(10000, 0, true);    //uncomment to check for overlaps in parts - slows things down!
  MGLog(debugging) << "Placed " << fPhysName << endlog;
  //Mark it as having been placed
  fHasBeenPlaced= true;
	fPartPosition.setX(partPosition->x()); //fixed bug, replaced z to x --N. McFadden
	fPartPosition.setY(partPosition->y());
	fPartPosition.setZ(partPosition->z());
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
