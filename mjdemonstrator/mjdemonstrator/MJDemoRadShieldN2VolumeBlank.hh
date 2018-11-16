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
 * CLASS DECLARATION:  MJDemoRadShieldN2VolumeBlank.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for the Nitrogen volume inside the copper shield.
 *
 *This model corresponds to 1/2 of the inner copper shield volume, with no
 *allowance for cryostats (to be used with 'blank monolith')
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
 AUTHOR: Matthew Green
 * CONTACT: greenmp@ornl.gov
 * FIRST SUBMISSION: June 11, 2015
 *
 * REVISION:
 *
 * 06-11-2015, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMORADSHIELDN2VOLUMEBLANK_HH
#define _MJDEMORADSHIELDN2VOLUMEBLANK_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoRadShieldN2VolumeBlank: public MJVDemoPart
{
  
public:
  MJDemoRadShieldN2VolumeBlank(G4String, G4String);
  MJDemoRadShieldN2VolumeBlank(const MJDemoRadShieldN2VolumeBlank &);
  ~MJDemoRadShieldN2VolumeBlank();

  G4LogicalVolume* ConstructPart();

  
private:
};
//
#endif
