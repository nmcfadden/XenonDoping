//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                         MAJORANA Simulation                               //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA Collaboration. It is based on Geant4, an intellectual       //
//      property of the RD44 GEANT4 collaboration.                           //
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
 * $Id: MJOutputLArGe.hh,v 1.3 2004-12-17 23:08:39 dimarcom Exp $
 *      
 * CLASS DECLARATION:  MJOutputLArGe.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Root output class for LANL clover. 
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
 * AUTHOR: Reyco Henning
 * CONTACT: 
 * FIRST SUBMISSION: Thu Apr  8 12:33:26 PDT 2004
 * 
 * REVISION:
 * 
 * 04-08-2004, Created, R. Henning
 * 06-29-2004, Changed to Output format for detector from Dongming Mei, RH.
 * 07-20-2004, Added MC Info header, R. Henning
 * 07-23-2204, Added noise and calibration data, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTLARGE_HH
#define _MJOUTPUTLARGE_HH

//---------------------------------------------------------------------------//

#include "TMath.h"
#include "TObject.h"

#include "globals.hh"
 
#include "io/MGOutputRoot.hh"
#include "mjio/MJOutputLArGeNoPS.hh"

//---------------------------------------------------------------------------//

  /** 
   * Root output class for LANL Clover.
   */

class MJOutputLArGe: public MGOutputRoot
{
public:

  //default constructor
  MJOutputLArGe(G4String serNum, G4bool isMother);

  //destructor
  ~MJOutputLArGe();

  //public interface
  //MJOutputRoot inherited virtual functions.
  void DefineSchema();
  void BeginOfEventAction(const G4Event *event);
  void BeginOfRunAction();
  void EndOfEventAction(const G4Event *event);
  void EndOfRunAction();
  void RootSteppingAction(const G4Step *step);

  //protected members
protected:


  //private  members
private:

  // Accumulator for energy deposits. 
  // Use G4double for this, since we will accumulate many small
  // contributions and do not want round-off errors to accumulate. 
  // Typecast when saved. 
  G4double fE1_MC, fE2_MC, fE3_MC, fE4_MC, fLAr_MC, fGe_MC, fNoCal_MC;
  
  MJOutputLArGeNoPS    *fTreePointer; // Pointer to Root tree
  
// Material in which to perform stepping action.
  size_t fSensitiveMaterialIndex; 
  size_t fScintillatingMaterialIndex; 

};

#endif
