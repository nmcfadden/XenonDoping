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
 * CLASS DECLARATION:  MJDemoThermosyphonSupportThinHalfMale.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for the Top IR Shield Support
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
 * AUTHOR: Max Hays
 * CONTACT: maxhays8@gmail.com
 * FIRST SUBMISSION: June 23, 2014
 * 
 * REVISION:
 * 
 * 23-06-2014, Created, M. Hays
 */
// --------------------------------------------------------------------------//

#ifndef _MJDemoThermosyphonSupportThinHalfMale_HH
#define _MJDemoThermosyphonSupportThinHalfMale_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoThermosyphonSupportThinHalfMale: public MJVDemoPart
{
  
public:
  MJDemoThermosyphonSupportThinHalfMale(G4String, G4String);
  MJDemoThermosyphonSupportThinHalfMale(const MJDemoThermosyphonSupportThinHalfMale &);
  ~MJDemoThermosyphonSupportThinHalfMale();

  G4LogicalVolume* ConstructPart();

  
private:
};
//
#endif
