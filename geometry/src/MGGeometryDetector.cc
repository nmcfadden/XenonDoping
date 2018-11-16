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
//                                                          
// $Id: MGGeometryDetector.cc,v 1.8 2007-06-06 07:41:47 kroening Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeometryDetector.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *  This class is meant to have the individual types of detectors inherit
 *  from it. See the header file for more details.
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: kareem
 * CONTACT: 
 * FIRST SUBMISSION: Fri Jul  2 10:52:55 PDT 2004
 * 
 * REVISION:
 *
 * 07-02-2004, Kareem
 *             First file creation and submission
 * 11-04-2004, Luciano
 *             Added methods to set/get the detector name and to 
 *             set the logicalDetector; "Detector" is the default
 * 09-14-2011, A.Schubert
 *             Changed logging verbosity to routine when volume is added to the
 *             importance store.
 * 11-16-2011, A. Schubert. Separated importance sampling into new class,
 *             MGGeometryImportance.  
 */
//---------------------------------------------------------------------------//
//

//---------------------------------------------------------------------------//

#include "geometry/MGGeometryDetector.hh"
#include "io/MGLogger.hh"
using namespace std;
//---------------------------------------------------------------------------//

MGGeometryDetector::MGGeometryDetector( G4String serNum ) :
  MGGeometryImportance(),
  serialNumber( serNum ), 
  theDetectorLogical( 0 ), 
  detectorName("Detector"),
  theDetectorPhysical( 0 )
{}

//---------------------------------------------------------------------------//

