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
//                                                          
// $Id: MJOutputDetectorEventData.cc,v 1.3 2006-06-02 08:12:02 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputDetectorEventData.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Sun Jan 30 10:42:00 PST 2005
 * 
 * REVISION:
 *
 * 01-30-2005, Created, R. Henning
 */
//---------------------------------------------------------------------------//
//

#include "TClonesArray.h"
#include "TObject.h"

#include "mjio/MJOutputDetectorEventData.hh"

ClassImp(MJOutputDetectorEventData)

//---------------------------------------------------------------------------//

MJOutputDetectorEventData::MJOutputDetectorEventData()
{
  fCrystals = new TClonesArray("MJOutputSegXtal", 1000);
}

//---------------------------------------------------------------------------//

MJOutputDetectorEventData::~MJOutputDetectorEventData()
{
  fCrystals->Delete();
  delete fCrystals;
}

//---------------------------------------------------------------------------//

void MJOutputDetectorEventData::Clear(Option_t* /*option*/)
{
  fCrystals->Delete();
  fNumberOfCrystals = 0;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
