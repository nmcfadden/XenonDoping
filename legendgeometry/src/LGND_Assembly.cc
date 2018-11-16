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
// $Id: MJcodeTemplate.cc,v 1.3 2005-02-15 23:26:55 akbarm Exp $ 
//      
// CLASS IMPLEMENTATION:  LGND_Assembly.cc
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
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: May 25 2017
 * 
 * REVISION:
 *
 * 05-25-2017, Created, M. Green.
 *
 */
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//

#include "legendgeometry/LGND_Assembly.hh" 

using namespace CLHEP;


//---------------------------------------------------------------------------//

LGND_Assembly::LGND_Assembly():
  fName(""), fSerialNumber(""), fDrawingNumber(""), fHasBeenPlaced(false)
{
}

//---------------------------------------------------------------------------//

LGND_Assembly::LGND_Assembly(G4String assemName, G4String serialNumber, 
                                 G4String drawingNumber):
  fName(assemName), fSerialNumber(serialNumber), fDrawingNumber(drawingNumber), 
  fHasBeenPlaced(false)
{;}

//---------------------------------------------------------------------------//

LGND_Assembly::LGND_Assembly(const LGND_Assembly & rhs):
  fName(rhs.fName),
  fSerialNumber(rhs.fSerialNumber),
  fDrawingNumber(rhs.fDrawingNumber),
  fHasBeenPlaced(rhs.fHasBeenPlaced)
{;}

//---------------------------------------------------------------------------//

LGND_Assembly::~LGND_Assembly()
{;}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

