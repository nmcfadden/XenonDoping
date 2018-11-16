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
 * $Id: MJheaderTemplate.hh,v 1.3 2005-02-15 23:26:55 akbarm Exp $
 *
 * CLASS DECLARATION:  LGND_Part.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**

 This is the virtual base class for all parts of the LEGeND geometries.

 **/
// End class description
//
/**
 * SPECIAL NOTES:
 *
 */
//
// --------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: May 25 2017
 *
 * REVISION:
 *
 * 05-25-2017, Created, M. Green
 *
 */
// --------------------------------------------------------------------------//

#ifndef _LGND_PART_HH
#define _LGND_PART_HH

//---------------------------------------------------------------------------//

#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include <vector>
class G4LogicalVolume;
//class G4PVPlacement;
class G4VPhysicalVolume;

using namespace std;

//---------------------------------------------------------------------------//


class LGND_Part
{

public:

  LGND_Part();
  LGND_Part(G4String partName, G4String serialNumber, G4String drawingNumber, G4String material);
  LGND_Part(const LGND_Part &);

  virtual ~LGND_Part();

  //public interface
  void Place(G4ThreeVector* partPosition, G4RotationMatrix* partRotation,
             G4LogicalVolume* motherLogical); //Places part in mother logical volume

  virtual G4LogicalVolume* BuildLogicalVolume() {return 0;}
                //checks to see if part logical volume exists
                //creates it if not

  G4String   GetName() { return fPartName; }
  G4String   GetSerialNumber() { return fSerialNumber; }
  G4String   GetDrawingNumber() { return fDrawingNumber; }
  G4String   GetMaterial() { return fPartMaterial; }
  G4LogicalVolume* GetPartLogical() { return fPartLogical; }
  G4bool     HasBeenPlaced() { return fHasBeenPlaced; }
  G4String   GetLogicalName() { return fLogicalName; }
  G4VPhysicalVolume* GetPhysicalVolume() {return fPartPhysical;} 
  //G4PVPlacement* GetPhysicalVolume() {return fPartPhysical; } //part must be placed before can be called
	G4ThreeVector GetPartPosition() { return fPartPosition;}

  void SetName(G4String pname) { fPartName = pname; }
  void SetSerialNumber(G4String snum) { fSerialNumber = snum; }
  void SetDrawingNumber(G4String dnum) { fDrawingNumber = dnum; }
  void SetPartMaterial(G4String mat) { fPartMaterial = mat; }
  void SetPartCustomMaterial(G4Material * mat) { fCustomPartMaterial = mat;}
	


protected:
  G4String     fPartName;           // Name or type of part.
  G4String     fSerialNumber;       // Serial number or part number of part.
  G4String     fDrawingNumber;      // Engineering drawing number of part.
  G4String     fPartMaterial;       // Material
  G4Material   *fCustomPartMaterial; //Custom defined material
  G4String     fLogicalName;
  G4LogicalVolume* fPartLogical;    // Part logical volume
  G4String     fPhysName;           // Physical volume name
  G4VPhysicalVolume* fPartPhysical; // Part physical volume
//  G4PVPlacement*   fPartPhysical;   // Part physical volume
  G4bool       fHasBeenPlaced;      // Set to true if Place() method has been invoked.
  G4VSolid*    fSolid;
	G4ThreeVector fPartPosition;

};
#endif
