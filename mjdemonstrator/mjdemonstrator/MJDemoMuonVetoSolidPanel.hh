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
 * CLASS DECLARATION:  MJDemoMuonVetoSolidPanel.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for the Muon Veto Shield
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
 * 12-07-2010, Created, M. Kidd
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOMUONVETOSOLIDPANEL_HH
#define _MJDEMOMUONVETOSOLIDPANEL_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoMuonVetoSolidPanel: public MJVDemoPart
{
  
public:
  MJDemoMuonVetoSolidPanel(G4String, G4String, G4double length, G4double height);
  MJDemoMuonVetoSolidPanel(const MJDemoMuonVetoSolidPanel &);
  ~MJDemoMuonVetoSolidPanel();

  G4LogicalVolume* ConstructPart();

  void SetLengthandHeight(G4double length, G4double height)   {fPanelLength = length*CLHEP::mm;  fPanelHeight = height*CLHEP::mm;}
  G4double GetLength()              {return fPanelLength;}
  G4double GetHeight()              {return fPanelHeight;}

  
private:
  G4double fPanelLength;
  G4double fPanelHeight;
};
//
#endif
