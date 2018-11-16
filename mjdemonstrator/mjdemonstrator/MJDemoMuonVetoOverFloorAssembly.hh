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
 * CLASS DECLARATION:  MJDemoMuonVetoAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The muon veto sheild assembly
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
 * AUTHOR: Mary Kidd
 * CONTACT: mkidd@lanl.gov
 * FIRST SUBMISSION: never
 * 
 * REVISION:
 * 
 * 12-09-2010, Created, M. Kidd
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOMUONVETOOVERFLOORASSEMBLY_HH
#define _MJDEMOMUONVETOOVERFLOORASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoMuonVetoOverFloorPanel;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoMuonVetoOverFloorAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoMuonVetoOverFloorAssembly(G4String, G4String);
  MJDemoMuonVetoOverFloorAssembly(const MJDemoMuonVetoOverFloorAssembly &);
  ~MJDemoMuonVetoOverFloorAssembly();

  MJDemoMuonVetoOverFloorPanel* GetOverFloorPanel1() {return fOverFloorPanelPtr1;}
  MJDemoMuonVetoOverFloorPanel* GetOverFloorPanel2() {return fOverFloorPanelPtr2;}
  MJDemoMuonVetoOverFloorPanel* GetOverFloorPanel3() {return fOverFloorPanelPtr3;}
  MJDemoMuonVetoOverFloorPanel* GetOverFloorPanel4() {return fOverFloorPanelPtr4;}
  MJDemoMuonVetoOverFloorPanel* GetOverFloorPanel5() {return fOverFloorPanelPtr5;}
  MJDemoMuonVetoOverFloorPanel* GetOverFloorPanel6() {return fOverFloorPanelPtr6;}

  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemoMuonVetoOverFloorPanel* fOverFloorPanelPtr1;
  MJDemoMuonVetoOverFloorPanel* fOverFloorPanelPtr2;
  MJDemoMuonVetoOverFloorPanel* fOverFloorPanelPtr3;
  MJDemoMuonVetoOverFloorPanel* fOverFloorPanelPtr4;
  MJDemoMuonVetoOverFloorPanel* fOverFloorPanelPtr5;
  MJDemoMuonVetoOverFloorPanel* fOverFloorPanelPtr6;
};
//
#endif
