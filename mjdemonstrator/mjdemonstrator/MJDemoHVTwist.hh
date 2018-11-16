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
 * CLASS DECLARATION:  MJDemoHVTwist.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * The PTFE part that twist the HV cables around. one such part for each string
 *
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 * Solid Work Drawing: hv twist v 3 blind.sldasm
 */
// 
// --------------------------------------------------------------------------//
/**
 * CONTACT: wxu@lanl.gov
 * FIRST SUBMISSION: Jan 09, 2013 
 * 
 * REVISION:
 * 
 * 01-09-2013, Created, W. Xu 
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOHVTWIST_HH
#define _MJDEMOHVTWIST_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoHVTwist: public MJVDemoPart
{
  
public:
  MJDemoHVTwist(G4String, G4String);
  MJDemoHVTwist(const MJDemoHVTwist &);
  ~MJDemoHVTwist();

  G4LogicalVolume* ConstructPart();

  
private:
};
//
#endif
