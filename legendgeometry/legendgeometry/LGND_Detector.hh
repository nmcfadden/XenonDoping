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
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *
 * CLASS DECLARATION:  LGND_Detector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 *
 *Creates a detector from parameters read in MGCrystalData
 *
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
 * AUTHOR: Kris Vorren
 * CONTACT: krisvorren@unc.edu
 * FIRST SUBMISSION: Oct 29, 2013
 *
 * REVISION:
 * 5-26-2017, Adapted for legend geometry, M. Green
 * 10-29-2013, Created, K. Vorren
 */
// --------------------------------------------------------------------------//

#ifndef _LGND_DETECTOR_HH
#define _LGND_DETECTOR_HH

//---------------------------------------------------------------------------//

#include "legendgeometry/LGND_Part.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class LGND_Detector: public LGND_Part
{

public:
  LGND_Detector();
  LGND_Detector(G4String partName, G4String serialNumber, G4String drawingNumber, G4String material);
  LGND_Detector(const LGND_Detector & rhs);
  ~LGND_Detector();

  virtual G4double GetHeight() const {return fCrystalHeight;}
  virtual G4double GetDiameter() const {return fCrystalDiameter;}

protected:
  G4double fCrystalHeight; //height of detector
  G4double fCrystalDiameter;
};
//
#endif
