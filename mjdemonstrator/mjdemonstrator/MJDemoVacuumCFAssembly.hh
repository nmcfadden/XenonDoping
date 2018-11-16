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
 * CLASS DECLARATION:  MJDemoVacuumCFAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *This assembly contains the stainless steel vacuum hardware attached to the
 *end of the cryostat cross-arm tube.
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

#ifndef _MJDEMOVACUUMCFASSEMBLY_HH
#define _MJDEMOVACUUMCFASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemo8CFCube;
class MJDemo8CFFlange;
class MJDemo8CFWeldedBellows;
class MJDemo8CFFlangeShield;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoVacuumCFAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoVacuumCFAssembly(G4String, G4String);
  MJDemoVacuumCFAssembly(const MJDemoVacuumCFAssembly &);
  ~MJDemoVacuumCFAssembly();

  MJDemo8CFCube* GetCube() {return fCubePtr;}
  MJDemo8CFWeldedBellows* GetBellows() {return fBellowsPtr;}
  MJDemo8CFFlange* GetBackFlange() {return fBackFlangePtr;}
  MJDemo8CFFlange* GetTopFlange() {return fTopFlangePtr;}
  MJDemo8CFFlange* GetBottomFlange() {return fBottomFlangePtr;}
  MJDemo8CFFlange* GetLeftFlange() {return fLeftFlangePtr;}
  MJDemo8CFFlange* GetRightFlange() {return fRightFlangePtr;}
  MJDemo8CFFlangeShield* GetFlangeShield() {return fFlangeShieldPtr;}
  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemo8CFCube* fCubePtr;
  MJDemo8CFWeldedBellows* fBellowsPtr;
  MJDemo8CFFlange* fBackFlangePtr;
  MJDemo8CFFlange* fTopFlangePtr;
  MJDemo8CFFlange* fBottomFlangePtr;
  MJDemo8CFFlange* fLeftFlangePtr;
  MJDemo8CFFlange* fRightFlangePtr;
  MJDemo8CFFlangeShield* fFlangeShieldPtr;


};
//
#endif
