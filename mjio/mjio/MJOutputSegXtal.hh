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
 * $Id: MJOutputSegXtal.hh,v 1.2 2005-02-16 18:29:00 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJOutputSegXtal.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Class written to file by ROOT that corresponds to data from a single 
 * event in a single crystal. This would be an energy deposits for each
 * segment and pulseshapes.
 * This class will also contain calibration constant for generating
 * detector-like spectra.
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
 * FIRST SUBMISSION: Sat Jan 29 12:05:15 PST 2005
 * 
 * REVISION:
 * 
 * 01-29-2005, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTSEGXTAL_HH
#define _MJOUTPUTSEGXTAL_HH

//---------------------------------------------------------------------------//

#include <string>

#include "TObject.h"

//---------------------------------------------------------------------------//

class MJOutputSegXtal : public TObject
{
public:
  // Constructors
  MJOutputSegXtal();
  MJOutputSegXtal(std::string name, UInt_t num);

  ~MJOutputSegXtal();

  std::string     fXtalName;       // Name of crystal
  Double_t        fEdep;           // Energy deposit during this event.

  void   SetXtalNumber(UInt_t num) { fXtalNumber = num; }
  UInt_t GetXtalNumber() { return fXtalNumber; }

  // Other feature (pulseshapes, etc.) will be added here. 

private:
  UInt_t    fXtalNumber;     
  // Number of crystal in list. 

  ClassDef(MJOutputSegXtal, 1)
};
#endif
