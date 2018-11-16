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
// CLASS IMPLEMENTATION:  MJVDemoPart.cc
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
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Mon Jul  26 09:19:15 EST 2010
 *
 * REVISION:
 *
 * 07-26-2010, Created, M. Green.
 *
 */
//---------------------------------------------------------------------------//
//

#include "G4String.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJVDemoPart.hh"      //Present MJ Class Headers

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJVDemoPart::MJVDemoPart():
  fPartName(""), fSerialNumber(""), fDrawingNumber(""), fPartMaterial(""),
  fPartLogical(0), fPhysName(""), fPartPhysical(0),
  fHasBeenPlaced(false)
{;}

//---------------------------------------------------------------------------//

MJVDemoPart::MJVDemoPart(G4String partName, G4String serialNumber,
                         G4String drawingNumber, G4String material):
  fPartName(partName), fSerialNumber(serialNumber), fDrawingNumber(drawingNumber),
  fPartMaterial(material), fPartLogical(0), fPhysName(""),
  fPartPhysical(0), fHasBeenPlaced(false)
{;}

//---------------------------------------------------------------------------//

MJVDemoPart::MJVDemoPart(const MJVDemoPart & rhs):
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

MJVDemoPart::~MJVDemoPart()
{;}

//---------------------------------------------------------------------------//

void MJVDemoPart::Place(G4ThreeVector* partPosition, G4RotationMatrix* partRotation,
                        G4LogicalVolume* motherLogical)
{
  fPartLogical = this->ConstructPart();
  //Place the part in the Mother Logical Volume
  fPhysName = fPartName;
  fPartPhysical = new G4PVPlacement(partRotation, *partPosition, fPartLogical, fPhysName,
                                    motherLogical, false, 0, false);
  // cout<<">>>>>>>>>>> calling the overlapping <<<<<<<<<<<<<<<"<<endl;
  //fPartPhysical->CheckOverlaps(1000000, 0, true);    //uncomment to check for overlaps in parts - slows things down!
  MGLog(debugging) << "Placed " << fPhysName << endlog;
  //Mark it as having been placed
  fHasBeenPlaced= true;
}


//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
