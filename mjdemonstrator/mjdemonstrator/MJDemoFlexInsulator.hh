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
 * CLASS DECLARATION:  MJDemoFlexInsulator.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for flexible crystal insulator
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
 * FIRST SUBMISSION: Mar 13, 2014
 * 
 * REVISION:
 * 
 * 03-13-2014, Created, K. Vorren
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOFLEXINSULATOR_HH
#define _MJDEMOFLEXINSULATOR_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoFlexInsulator: public MJVDemoPart
{
  
public:
  MJDemoFlexInsulator(G4String, G4String);
  MJDemoFlexInsulator(const MJDemoFlexInsulator &);
  ~MJDemoFlexInsulator();

  //void SetBevelRadius(G4double r){fBevelRadius = r;}
  void SetDetectorRadius(G4double r){fDetectorRadius = r;}

  //G4double GetBevelRadius(){return fBevelRadius;}
  G4double GetDetectorRadius(){return fDetectorRadius;}

  G4LogicalVolume* ConstructPart();

  
private:
  //G4double fBevelRadius;
  G4double fDetectorRadius;
};
//
#endif
