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
// CLASS IMPLEMENTATION:  MJVDemoAssembly.cc
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
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Fri Jul  23 09:19:15 EST 2010
 * 
 * REVISION:
 *
 * 07-23-2010, Created, M. Green.
 *
 */
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"      //Present MJ Class Headers 

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJVDemoAssembly::MJVDemoAssembly():
  fName(""), fSerialNumber(""), fDrawingNumber(""), fHasBeenPlaced(false)
{
}

//---------------------------------------------------------------------------//

MJVDemoAssembly::MJVDemoAssembly(G4String assemName, G4String serialNumber, 
                                 G4String drawingNumber):
  fName(assemName), fSerialNumber(serialNumber), fDrawingNumber(drawingNumber), 
  fHasBeenPlaced(false)
{;}

//---------------------------------------------------------------------------//

MJVDemoAssembly::MJVDemoAssembly(const MJVDemoAssembly & rhs):
  fName(rhs.fName),
  fSerialNumber(rhs.fSerialNumber),
  fDrawingNumber(rhs.fDrawingNumber),
  fHasBeenPlaced(rhs.fHasBeenPlaced)
{;}

//---------------------------------------------------------------------------//

MJVDemoAssembly::~MJVDemoAssembly()
{;}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

