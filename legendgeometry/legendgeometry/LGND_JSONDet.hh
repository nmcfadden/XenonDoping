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
 * CLASS DECLARATION:  LGND_JSONDet.hh
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
 * 5-27-2017, Adapted for legendgeometry, M. Green
 * 10-29-2013, Created, K. Vorren
 */
// --------------------------------------------------------------------------//

#ifndef _LGND_JSONDET_HH
#define _LGND_JSONDET_HH

//---------------------------------------------------------------------------//

#include "legendgeometry/LGND_Detector.hh"
#include <vector>
#include "G4ThreeVector.hh"
//#include "MGCrystalData.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class LGND_JSONDet: public LGND_Detector
{

public:
  LGND_JSONDet(G4String partName, G4String serialNumber, G4String jsonFile);
  LGND_JSONDet(const LGND_JSONDet &);
  ~LGND_JSONDet();

  void ReadJSONFile(G4String jsonFile);

  vector<G4ThreeVector> GetDetParameters() {return fDetParameters;}

  G4LogicalVolume* BuildLogicalVolume();
  
private:
  vector<G4ThreeVector> fDetParameters; //vector of (r, z)
};
//
#endif
