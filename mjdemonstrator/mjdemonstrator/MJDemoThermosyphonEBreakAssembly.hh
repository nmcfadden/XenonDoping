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
 * CLASS DECLARATION:  MJDemoThermosyphonEBreakAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *This assembly contains the flanges and glass section of the thermosyphon
 *electrical break assembly.
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
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: Sept 28, 2015
 *
 * REVISION:
 *
 * 09-28-2015, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOTHERMOSYPHONEBREAKASSEMBLY_HH
#define _MJDEMOTHERMOSYPHONEBREAKASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoThermosyphonFlange;
class MJDemoThermosyphonGlass;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoThermosyphonEBreakAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoThermosyphonEBreakAssembly(G4String, G4String);
  MJDemoThermosyphonEBreakAssembly(const MJDemoThermosyphonEBreakAssembly &);
  ~MJDemoThermosyphonEBreakAssembly();

  MJDemoThermosyphonFlange* GetBottomFlange() {return fBottomFlangePtr;}
  MJDemoThermosyphonFlange* GetTopFlange() {return fTopFlangePtr;}
  MJDemoThermosyphonGlass* GetGlass() {return fGlassPtr;}
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemoThermosyphonFlange* fBottomFlangePtr;
  MJDemoThermosyphonFlange* fTopFlangePtr;
  MJDemoThermosyphonGlass* fGlassPtr;

};
//
#endif
