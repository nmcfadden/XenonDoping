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
 * CLASS DECLARATION:  MJCharButtonSourceAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * The decay source in a polyethylene case 
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
 * AUTHOR: A. Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 *
 * REVISION:
 * 
 * 07-22-2013, Created, A. Wasserman
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJCHARBUTTONSOURCEASSEMBLY_HH
#define _MJCHARBUTTONSOURCEASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJCharButtonSource;
class MJCharButtonSourceCase;

using namespace std;
//---------------------------------------------------------------------------//

class MJCharButtonSourceAssembly: public MJVDemoAssembly
{
  
public:
  MJCharButtonSourceAssembly(G4String, G4String, G4String);
  MJCharButtonSourceAssembly(const MJCharButtonSourceAssembly &);
  ~MJCharButtonSourceAssembly();

  G4String fSourceMaterialName;
  G4String GetSourceMaterialName() const {return fSourceMaterialName;}
  MJCharButtonSource* GetButtonSource() {return fButtonSourcePtr;}
  MJCharButtonSourceCase* GetButtonSourceCase() {return fButtonSourceCasePtr;}
  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
	     G4LogicalVolume* motherLogical);
  
private:
  MJCharButtonSource* fButtonSourcePtr;
  MJCharButtonSourceCase* fButtonSourceCasePtr;

};
//
#endif
