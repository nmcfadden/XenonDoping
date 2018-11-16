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
// $Id: MJOutputSegXtal.cc,v 1.2 2005-02-16 18:29:00 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputSegXtal.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Sat Jan 29 12:05:15 PST 2005
 * 
 * REVISION:
 *
 * 01-29-2005, Created, R. Henning
 */
//---------------------------------------------------------------------------//
//

#include <string>

//---------------------------------------------------------------------------//

#include "mjio/MJOutputSegXtal.hh"

using namespace std;
ClassImp(MJOutputSegXtal)

//---------------------------------------------------------------------------//
MJOutputSegXtal::MJOutputSegXtal():
  fEdep(0.0), fXtalNumber(99999)
{;}

//---------------------------------------------------------------------------//

MJOutputSegXtal::MJOutputSegXtal(string name, UInt_t num): 
  fXtalName(name), fEdep(0.0), fXtalNumber(num)
{;}  

//---------------------------------------------------------------------------//

MJOutputSegXtal::~MJOutputSegXtal()
{;}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
