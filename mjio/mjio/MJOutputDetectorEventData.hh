//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
 * $Id: MJOutputDetectorEventData.hh,v 1.3 2006-06-02 08:12:01 jasondet Exp $
 *      
 * CLASS DECLARATION:  MJOutputDetectorEventData.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Root tree for Detector output
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Sun Jan 30 10:42:00 PST 2005
 * 
 * REVISION:
 * 
 * 01-30-2005, Created, R. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTDETECTOREVENTDATA_HH
#define _MJOUTPUTDETECTOREVENTDATA_HH

//---------------------------------------------------------------------------//

#include "TClonesArray.h"
#include "TObject.h"

#include "mjio/MJOutputSegXtal.hh"

//---------------------------------------------------------------------------//

class MJOutputDetectorEventData : public TObject
{
public:

  MJOutputDetectorEventData();

  ~MJOutputDetectorEventData();
  
  void Clear(Option_t* /*option*/ = "");

  Int_t         fDetectorEventNum;   // Event number of this event
  Int_t         fMCEventNum;         // MC event number associated with this
  Int_t         fNumberOfCrystals;   // Number of hit crystal in the event.
  Double_t      fSeedTime;           // Seed time for this event.
  TClonesArray  *fCrystals;          //-> Hit crystals.

  ClassDef(MJOutputDetectorEventData, 1)

};
#endif
