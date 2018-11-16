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
 * CLASS DECLARATION:  MJDemoMuonVetoThroughPanel.hh
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

#ifndef _MJDEMOMUONVETOTHROUGHPANEL_HH
#define _MJDEMOMUONVETOTHROUGHPANEL_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoMuonVetoThroughPanel: public MJVDemoPart
{
  
public:
  MJDemoMuonVetoThroughPanel(G4String, G4String, G4double length, G4double height, G4double holePosX, G4double holePosY, G4double cutOutX, G4double cutOutY, G4double cutOutAng);
  MJDemoMuonVetoThroughPanel(const MJDemoMuonVetoThroughPanel &);
  ~MJDemoMuonVetoThroughPanel();

  G4LogicalVolume* ConstructPart();

  void SetLengthandHeight(G4double length, G4double height, G4double holePosX, G4double holePosY, 
                          G4double cutOutX, G4double cutOutY, G4double cutOutAng)
    {  fPanelLength = length*CLHEP::mm;  fPanelHeight = height*CLHEP::mm;  
       fholePosX = holePosX*CLHEP::mm; fholePosY = holePosY*CLHEP::mm; 
       fcutOutX = cutOutX*CLHEP::mm; fcutOutY = cutOutY*CLHEP::mm; 
       fcutOutAng = cutOutAng*CLHEP::deg;
    }
  G4double GetLength()              {return fPanelLength;}
  G4double GetHeight()              {return fPanelHeight;}
  G4double GetHolePosX()              {return fholePosX;}
  G4double GetHolePosY()              {return fholePosY;}
  G4double GetcutOutX()              {return fcutOutX;}
  G4double GetcutOutY()              {return fcutOutY;}
  G4double GetcutOutAng()              {return fcutOutAng;}

private:
  G4double fPanelLength;
  G4double fPanelHeight;
  G4double fholePosX;
  G4double fholePosY;
  G4double fcutOutX;
  G4double fcutOutY;
  G4double fcutOutAng;
};
//
#endif
