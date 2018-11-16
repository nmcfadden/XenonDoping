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
 * FIRST SUBMISSION: Fri Jul  23 09:19:15 EST 2010
 * 
 * REVISION:
 * 
 * 07-23-2010, Created, M. Green
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJVDEMOASSEMBLY_HH
#define _MJVDEMOASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "G4String.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "MJVDemoPart.hh"
#include <vector>
class G4LogicalVolume;
class G4PVPlacement;

using namespace std;

//---------------------------------------------------------------------------//


class MJVDemoAssembly 
{

public:

  MJVDemoAssembly();
  MJVDemoAssembly(G4String pname, G4String serialNumber, G4String drawingNumber);
  MJVDemoAssembly(const MJVDemoAssembly &);

  virtual ~MJVDemoAssembly();

  //public interface
  virtual void Construct()
    { ConstructParts(); ConstructAssemblies(); }
  virtual void ConstructParts()
    { for(auto part : fPartsList) if(part) part->ConstructPart(); }
  virtual void ConstructAssemblies()
    { for(auto assembly : fAssembliesList) if(assembly) assembly->ConstructParts(); }
  virtual void Place(G4ThreeVector* /*assemPosition*/, G4RotationMatrix* /*assemRotation*/,
                     G4LogicalVolume* /*motherLogical*/) {};  //Place all constituents in 
                                                       //mother logical volume 

  G4String GetName() { return fName; }
  G4String GetSerialNumber() { return fSerialNumber; }
  G4String GetDrawingNumber() { return fDrawingNumber; }
  G4bool   HasBeenPlaced() { return fHasBeenPlaced; }

  void SetName(G4String aname) { fName = aname; }
  void SetSerialNumber(G4String snum) { fSerialNumber = snum; }
  void SetDrawingNumber(G4String dnum) { fDrawingNumber = dnum; }

  //private  members
protected:
  G4String     fName;        // Name or type of part. 
  G4String     fSerialNumber;    // Serial number or part number of part.
  G4String     fDrawingNumber;   // Engineering drawing number of part. 

  G4bool       fHasBeenPlaced; // Set to true if Initialize() method has 
                                    // invoked. 
  vector<MJVDemoPart*> fPartsList; // List of parts in the assembly
  vector<MJVDemoAssembly*> fAssembliesList;  // List of assemblies in the assembly
};
#endif
