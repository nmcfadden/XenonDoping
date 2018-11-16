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
 * CLASS DECLARATION:  MJDemoPuckBeGe.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for a generic BeGe
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
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 * 
 * 07-22-2010, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOPUCKBEGE_HH
#define _MJDEMOPUCKBEGE_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoDetector.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoPuckBeGe: public MJVDemoDetector
{
  
public:
  MJDemoPuckBeGe(G4String partName, G4String serialNumber, 
					G4double diameter, G4double height);
  MJDemoPuckBeGe(const MJDemoPuckBeGe &);
  ~MJDemoPuckBeGe();

  G4LogicalVolume* ConstructPart();

  void SetDiameter(G4double diameter)	{fCrystalDiameter = diameter*CLHEP::mm;}
  void SetHeight(G4double height)		{fCrystalHeight = height*CLHEP::mm;}
  //G4double GetDiameter() 				{return fCrystalDiameter;}  

private:
  //G4double fCrystalDiameter;
};
//
#endif
