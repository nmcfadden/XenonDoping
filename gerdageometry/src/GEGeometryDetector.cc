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

/**
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 01-19-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 *
 * 03-06-2006, added Cherenkov constructor, Markus
 */

// ---------------------------------------------------------------------------

#include "globals.hh"

#include "gerdageometry/GEGeometryDetector.hh"
#include "io/MGLogger.hh"

// --------------------------------------------------------------------------- 

GEGeometryDetector::GEGeometryDetector(GEGeometryDetectorDB* detectordb)
{

  
  MGLog(debugging) << "START of \"GEGeometryDetectorArray()\"" <<endlog;
  fDetectorArray = new GEGeometryDetectorArray(detectordb); 
  MGLog(debugging) << "End of \"GEGeometryDetectorArray()\"  -- START of \"GEGeometryScintillator()\"" <<endlog;
  fScintillator  = new GEGeometryScintillator(detectordb); 
  MGLog(debugging) << "End of \"GEGeometryScintillator()\"  -- START of \"GEGeometryCherenkovVeto()\"" <<endlog;
  fCherenkovVeto = new GEGeometryCherenkovVeto(detectordb);
  MGLog(debugging) << "End of \"GEGeometryCherenkovVeto()\"" <<endlog;

}

// --------------------------------------------------------------------------- 

GEGeometryDetector::~GEGeometryDetector()
{

  delete fDetectorArray; 
  delete fScintillator; 
  delete fCherenkovVeto;

} 

// --------------------------------------------------------------------------- 

void GEGeometryDetector::ConstructDetector()
{
  MGLog(debugging) << "START of \"ConstructDetectorArray()\"" <<endlog;
  fDetectorArray -> ConstructDetectorArray(); 
  MGLog(debugging) << "End of \"ConstructDetectorArray()\"  -- START of \"ConstructScintillator()\"" <<endlog;

  fScintillator  -> ConstructScintillator(); 
  MGLog(debugging) << "End of \"ConstructScintillator()\"  -- START of \"ConstructCherenkovVeto()\"" <<endlog;

  fCherenkovVeto -> ConstructCherenkovVeto();
  MGLog(debugging) << "End of \"ConstructCherenkovVeto()\"" <<endlog;

} 

// --------------------------------------------------------------------------- 


