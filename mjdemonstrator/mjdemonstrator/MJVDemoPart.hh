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
 * CLASS DECLARATION:  MJVDemoPart.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 
 This is the virtual base class for all parts of the Majorana Demonstrator. 
 It is designed to be more generic than the MGGeometryDetector class used in
 previous implementations. The main difference is that this class can return
 more than one G4 logical volume that is to be provded a physicsal placement
 in the class that
 instantiates it. This allows easier descriptions of 'flat' geometry hierarchies
 and geometries that have many parts that span across the detector, ie. cables,
 rods, etc.  

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
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Mon Jul  26 09:19:15 EST 2010
 * 
 * REVISION:
 * 
 * 07-26-2010, Created, M. Green
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJVDEMOPART_HH
#define _MJVDEMOPART_HH

//---------------------------------------------------------------------------//

#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include <vector>
class G4LogicalVolume;
class G4PVPlacement;

using namespace std;

//---------------------------------------------------------------------------//


class MJVDemoPart 
{

public:

  MJVDemoPart();
  MJVDemoPart(G4String partName, G4String serialNumber, G4String drawingNumber, G4String material);
  MJVDemoPart(const MJVDemoPart &);

  virtual ~MJVDemoPart();

  //public interface
  void Place(G4ThreeVector* partPosition, G4RotationMatrix* partRotation,
             G4LogicalVolume* motherLogical); //Places part in mother logical volume
  virtual G4LogicalVolume* ConstructPart() {return 0;}
                //checks to see if part logical volume exists
                //creates it if not 

  G4String   GetName() const { return fPartName; }
  G4String   GetSerialNumber() const { return fSerialNumber; }
  G4String   GetDrawingNumber() const { return fDrawingNumber; }
  G4String   GetMaterial() const { return fPartMaterial; }
  G4LogicalVolume* GetPartLogical() const { return fPartLogical; }
  G4bool     HasBeenPlaced() const { return fHasBeenPlaced; }
  G4String   GetLogicalName() const { return fLogicalName; }

  G4String   GetName() { return fPartName; }
  G4String   GetSerialNumber() { return fSerialNumber; }
  G4String   GetDrawingNumber() { return fDrawingNumber; }
  G4String   GetMaterial() { return fPartMaterial; }
  G4LogicalVolume* GetPartLogical() { return fPartLogical; }
  G4bool     HasBeenPlaced() { return fHasBeenPlaced; }
  G4String   GetLogicalName() { return fLogicalName; }

  //virtual G4double GetHeight(){return 33*mm;}

  void SetName(G4String pname) { fPartName = pname; }
  void SetSerialNumber(G4String snum) { fSerialNumber = snum; }
  void SetDrawingNumber(G4String dnum) { fDrawingNumber = dnum; }
  void SetPartMaterial(G4String mat) { fPartMaterial = mat; }
  
protected:
  G4String     fPartName;           // Name or type of part. 
  G4String     fSerialNumber;       // Serial number or part number of part.
  G4String     fDrawingNumber;      // Engineering drawing number of part. 
  G4String     fPartMaterial;       // Material
  G4String     fLogicalName;
  G4LogicalVolume* fPartLogical;    // Part logical volume
  G4String     fPhysName;           // Physical volume name
  G4PVPlacement*   fPartPhysical;   // Part physical volume
  G4bool       fHasBeenPlaced;      // Set to true if Place() method has been invoked. 

};
#endif
