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
 * $Id: MJGeometrySolidBlock.hh,v 1.4 2007-08-16 18:33:17 rojo Exp $
 *      
 * CLASS DECLARATION:  MJGeometrySolidBlock.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Solid, uniform block, size 2x2x2 m^3,  of user selected material. 
 * For energy deposit studies.
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Tue Aug 24 12:39:35 PDT 2004
 * 
 * REVISION:
 * 
 * 08-24-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYSOLIDBLOCK_HH
#define _MJGEOMETRYSOLIDBLOCK_HH

//---------------------------------------------------------------------------//

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class G4VisAttributes;
class G4LogicalVolume;
class G4PhysicalVolume;

class MJGeometrySolidBlockMessenger;

class MJGeometrySolidBlock : public MGGeometryDetector
{
public:

  //default constructor
  MJGeometrySolidBlock();

  //destructor
  ~MJGeometrySolidBlock();

  bool UsesMJPSQLDatabase() { return true; }  

  //public interface
  void ConstructDetector();
  void SetMaterialName(G4String name) { fMaterialName = name; }
  void SetEdgeLength(G4double length) { fEdgeLength = length; }
  void SetActive(G4bool active)       { fActive = active; }
  void SetDeadLayerDepth(G4double dl)      { fDeadLayerDepth = dl; }
  //protected members
protected:


  //private  members
private:
  
  MJGeometrySolidBlockMessenger     *fMessenger;
  G4String                          fMaterialName;
  G4double                          fEdgeLength;
  G4bool                            fActive;
  G4bool                            fDeadLayer;
  G4double                          fDeadLayerDepth;

  //G4VisAttributes                   *blockVisAtt;
  G4VisAttributes                   *activeblockVisAtt;

  G4LogicalVolume                   *fActiveLogical;
  G4VPhysicalVolume                 *fActivePhysical;
};
#endif
