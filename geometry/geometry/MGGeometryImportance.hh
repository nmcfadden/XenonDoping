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
 * CLASS DECLARATION:  MGGeometryImportance.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * This class constructs an importance store and sets importance values of
 * physical volumes.  This code was removed from MGGeometryDetector so it could
 * be used in MGGeometryDetector and MGGeometryParallelWorld.
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
 * AUTHOR: alexis
 * CONTACT: Alexis Schubert (alexis3@uw.edu)
 * FIRST SUBMISSION: Mon Nov 14 12:35:25 PST 2011
 * 
 * REVISION:
 * 
 * 16 Nov 2011 created from MGGeometryDetector, A. Schubert
 */
// --------------------------------------------------------------------------//

#ifndef _MGGEOMETRYIMPORTANCE_HH
#define _MGGEOMETRYIMPORTANCE_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4VPhysicalVolume.hh"
#include "G4IStore.hh"
#include <map>

//---------------------------------------------------------------------------//

class MGGeometryImportance 
{
public:

  enum EMGGeometryImportanceConsts {kMinImpValue = 1};


  //default constructor
  MGGeometryImportance() {}

  //destructor
  ~MGGeometryImportance() {}


  // Construct the Importance Store given a passed in pointer to an allocated store.
  void ConstructImportanceStore(G4IStore&);

  // Sets the importance value for a region within the geometry.  
  void SetImportanceValueForRegion(G4VPhysicalVolume*, G4double = kMinImpValue, G4int aRepNum = 0);


  //private  members
private:

  // importance sampling map of physical volume ->  pair(importance, replica no.)
  std::map<G4VPhysicalVolume* , std::pair<G4double, G4int>* > fImpSampRegionMap;

};
#endif
