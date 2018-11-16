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
 * $Id: MJOutputShielding.hh,v 1.2 2007-02-23 15:27:18 mgmarino Exp $
 *      
 * CLASS DECLARATION:  MJOutputShielding.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * ROOT output class intended for use in shielding studies.
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
 * AUTHOR: kareem
 * CONTACT: 
 * FIRST SUBMISSION: Tue Dec 27 09:14:19 PST 2005
 * 
 * REVISION:
 * 
 * 12-27-2005, Initial submission, Kareem
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTSHIELDING_HH
#define _MJOUTPUTSHIELDING_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "io/MGOutputRoot.hh"
#include "mjio/MJOutputShieldingData.hh"

//---------------------------------------------------------------------------//

class MJOutputShieldingMessenger;

class MJOutputShielding : public MGOutputRoot
{
public:

  //default constructor
  MJOutputShielding( G4bool isMother );

  //destructor
  ~MJOutputShielding();

  //public interface
  void DefineSchema();
  void BeginOfEventAction(const G4Event *event);
  void BeginOfRunAction();
  void EndOfEventAction(const G4Event *event);
  void EndOfRunAction();
  void RootSteppingAction(const G4Step *step);
  void SetVolumeName( G4String volume ) { strncpy( fVolumeName, (const char*)volume, volume.size()); };

  //protected members
protected:

  //private  members
private:
  MJOutputShieldingData    *fTreePointer;
  Char_t fVolumeName[100];
  
  MJOutputShieldingMessenger *fMessenger;

};
#endif
