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
// $Id: MJVGeometrySupportStructure.cc,v 1.1 2005-03-11 02:21:46 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MJVGeometrySupportStructure.cc
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
 * FIRST SUBMISSION: Wed Mar  2 13:08:07 PST 2005
 * 
 * REVISION:
 *
 * 03-02-2005, Created, R. Henning
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJVGeometrySupportStructure.hh"

//---------------------------------------------------------------------------//

MJVGeometrySupportStructure::MJVGeometrySupportStructure(MGGeometryDetector 
   *mother, G4String name) : fName(name), fMotherDetector(mother)
{;}

//---------------------------------------------------------------------------//

MJVGeometrySupportStructure::~MJVGeometrySupportStructure()
{;}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
