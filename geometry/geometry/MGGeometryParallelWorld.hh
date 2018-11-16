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
 *
 * DESCRIPTION: 
 *
 * This is a parallel world base class that can be used for importance sampling.
 * Derived classes must overload MGGeometryParallelWorld::GetWorldVolume() and
 * G4VUserParallelWorld::Construct().
 *
 * AUTHOR: alexis
 * CONTACT: Alexis Schubert (alexis3@uw.edu)
 * FIRST SUBMISSION: Mon Nov 14 10:57:49 PST 2011
 * 
 * REVISION:
 * 
 * 16 Nov 2011, creation, A. Schubert
 * 25 Nov 2011, made destructor virtual, A. Schubert
 *
 */

#ifndef _MGGEOMETRYPARALLELWORLD_HH
#define _MGGEOMETRYPARALLELWORLD_HH


#include "G4VUserParallelWorld.hh"
#include "MGGeometryImportance.hh"


/// Base class to use for parallel geometry implementation
class MGGeometryParallelWorld : public G4VUserParallelWorld, public MGGeometryImportance 
{
public:

  /// Constructor
  MGGeometryParallelWorld(G4String name="ParallelSamplingWorld");

  /// Destructor
  virtual ~MGGeometryParallelWorld() {}

  /// Get world volume
  /**
  * Derived class must overload to provide access to world physical volume;
  * this is accessed in MGManagerDetectorConstruction */
  virtual G4VPhysicalVolume* GetWorldVolume() const = 0;

};
#endif
