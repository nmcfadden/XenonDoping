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
 * $Id: MJGeometryUWLArGe.hh,v 1.1 2005-08-16 00:25:55 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometryUWLArGe.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geometry class for LArGe Setup at UW.
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
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
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYUWLARGE_HH
#define _MJGEOMETRYUWLARGE_HH

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class  G4Material;
class  MGGeometryIdealCoaxCrystal;


//---------------------------------------------------------------------------//

class MJGeometryUWLArGe : public MGGeometryDetector
{
public:

  MJGeometryUWLArGe();

  ~MJGeometryUWLArGe();

  void ConstructDetector();
  void SetDefaults();
  bool UsesMJPSQLDatabase() { return true; }  

private:
  MGGeometryIdealCoaxCrystal *fIdealCoaxCrystal;

  G4String     fShieldMaterialName;
  G4Material   *fShieldMaterial;
  G4double     fRadius;
  G4double     fHeight;

};
#endif
