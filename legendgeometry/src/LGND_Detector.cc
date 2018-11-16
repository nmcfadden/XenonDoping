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
// CLASS IMPLEMENTATION:  LGND_Detector.cc
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
 * FIRST SUBMISSION: Mon Jul  26 09:19:15 EST 2010
 *
 * REVISION:
 *
 * 07-26-2010, Created, M. Green.
 *
 */
//---------------------------------------------------------------------------//
//

#include "G4String.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "legendgeometry/LGND_Detector.hh"      //Present MJ Class Headers

using namespace CLHEP;


//---------------------------------------------------------------------------//

LGND_Detector::LGND_Detector():
  LGND_Part()
{;}

//---------------------------------------------------------------------------//

LGND_Detector::LGND_Detector(G4String partName, G4String serialNumber,
                         G4String drawingNumber, G4String material):
  LGND_Part(partName, serialNumber, drawingNumber, material)
{;}

//---------------------------------------------------------------------------//

LGND_Detector::LGND_Detector(const LGND_Detector & rhs):
  LGND_Part(rhs)
{;}

//---------------------------------------------------------------------------//

LGND_Detector::~LGND_Detector()
{;}

//---------------------------------------------------------------------------//
