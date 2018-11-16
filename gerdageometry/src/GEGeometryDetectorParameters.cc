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
 * FIRST SUBMISSION: 11-18-2005
 *
 * REVISION: see include file 
 *
 *
 */

// ---------------------------------------------------------------------------

#include "globals.hh"

#include "gerdageometry/GEGeometryDetectorParameters.hh"       

// --------------------------------------------------------------------------- 

GEGeometryDetectorParameters::GEGeometryDetectorParameters()
{
  
}

// --------------------------------------------------------------------------- 

GEGeometryDetectorParameters::~GEGeometryDetectorParameters()
{

} 

// --------------------------------------------------------------------------- 

G4double GEGeometryDetectorParameters::GetSegmentHeight()
{

  G4double height = (this -> GetHeight()  
		     - this -> GetDLTopThickness() 
		     - this -> GetDLBottomThickness()
		     - 2.0 * this -> GetPassivationThickness())
    / double(this -> GetNZSegments());
  
  return height; 

}

// --------------------------------------------------------------------------- 

