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
 * CLASS DECLARATION:  LGND_Assembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**

 This is the virtual base class for all assemblies of the LEGeND geometries.

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
 * CONTACT: matthew_green.edu
 * FIRST SUBMISSION: May  25 2017
 *
 * REVISION:
 *
 * 05-25-2017, Created, M. Green
 *
 */
// --------------------------------------------------------------------------//

#ifndef _LGND_ASSEMBLY_HH
#define _LGND_ASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "G4String.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "LGND_Part.hh"
#include <vector>
class G4LogicalVolume;
class G4PVPlacement;

using namespace std;

//---------------------------------------------------------------------------//


class LGND_Assembly
{

public:

  LGND_Assembly();
  LGND_Assembly(G4String pname, G4String serialNumber, G4String drawingNumber);
  LGND_Assembly(const LGND_Assembly &);

  virtual ~LGND_Assembly();

  virtual void Place(G4ThreeVector* /*assemPosition*/, G4RotationMatrix* /*assemRotation*/,
                     G4LogicalVolume* /*motherLogical*/) {};  //Place all constituents in
                                                       //mother logical volume

  G4String GetName() { return fName; }
  G4String GetSerialNumber() { return fSerialNumber; }
  G4String GetDrawingNumber() { return fDrawingNumber; }
  G4bool   HasBeenPlaced() { return fHasBeenPlaced; }
  G4VSolid* GetSolid();

  void SetName(G4String aname) { fName = aname; }
  void SetSerialNumber(G4String snum) { fSerialNumber = snum; }
  void SetDrawingNumber(G4String dnum) { fDrawingNumber = dnum; }

  //private  members
protected:
  G4String     fName;        // Name or type of part.
  G4String     fSerialNumber;    // Serial number or part number of part.
  G4String     fDrawingNumber;   // Engineering drawing number of part.
  G4String     fJSONfile;     //JSON file used for assembly

  G4bool       fHasBeenPlaced; // Set to true if Initialize() method has
                                    // invoked.
  vector<LGND_Part*> fPartsList; // List of parts in the assembly
  vector<LGND_Assembly*> fAssembliesList;  // List of assemblies in the assembly
};
#endif
