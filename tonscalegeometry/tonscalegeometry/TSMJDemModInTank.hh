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
 * $Id: MJheaderTemplate.hh,v 1.3 2005-02-15 23:26:55 akbarm Exp $
 *      
 * CLASS DECLARATION:  TSMjDemModInTank.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Tonne scale geometry builder. Places a MJDemonstrator cryostat (without the
 * cold arm), inside a cylinder of vacuum, inside a cylinder of sheilding,                                                             
 * inside a lab module `box' made of rock.
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ryan Martin
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * 02-14-2010, Created, R. Martin
 * 06-13-2011, Added stuff to make variable sized modules, R. Martin
 * 03-12-2012, R.Martin added parallel world
 * 03-14-2012, Added height of world volume member for messenger, R.Martin
 * 07-30-2012, Added functions to set string radial clearance and radial
 *             spacing between strings, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#ifndef _TSMJDEMMODINTANK_HH
#define _TSMJDEMMODINTANK_HH

//---------------------------------------------------------------------------//
#include <string>
#include <vector>

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"
#include "tonscalegeometry/TSMJDemModInTankParallelWorld.hh"
#include "tonscalegeometry/TSCylinderShieldTank.hh"
#include "tonscalegeometry/TSLabModuleBox.hh"
#include "tonscalegeometry/TSMJDemModInTankMessenger.hh"
#include "tonscalegeometry/TSModule.hh"

//---------------------------------------------------------------------------//
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;
class G4PVPlacement;

class TSMJDemModInTank : public MGGeometryDetector
{
  public:
    TSMJDemModInTank(G4String);
    virtual ~TSMJDemModInTank();
  
    void ConstructDetector();
    bool UsesMJPSQLDatabase() { return true; }  
    
    virtual int GetSensitiveVolumeID(const std::string& volName);
  
    virtual MGGeometryParallelWorld* GimmeParallelWorld(); 
  
    void TankIsSensitive(G4bool arg=true){ fTankIsSensitive=arg; }
    G4bool IsTankSensitive(){ return fTankIsSensitive; }
    void SetTankVerticalOffset(G4double arg){ fTankVerticalOffset=arg; }
    G4double GetTankVerticalOffset(){ return fTankVerticalOffset; }
  
    void SetHeightOfWorldVolume(G4double arg){ fHeightOfWorldVolume=arg; }
    void SetnModules(int arg){ fnModules=arg; }
    void SetnCrystalsPerString(int arg){ fnCrystalsPerString=arg; }
    void SetModuleType(G4String arg){ fModuleType=arg; }
    void SetCrystalAssemblyType(G4String arg){ fCrystalAssemblyType=arg; }
    void SetCrystalDiameter(G4double arg){ fCrystalDiameter=arg; }
    void SetCrystalHeight(G4double arg){ fCrystalHeight=arg; }
    void SetStringRadialClearance(G4double arg) { fStringRadialClearance=arg; } // FIXME: Added by K. Nguyen
    void SetRadialSpacingBetweenStrings(G4double arg) { fRadialSpacingBetweenStrings=arg; } // FIXME: Added by K. Nguyen
  
  private:
    TSModule** fModules;
    G4int fnModules;
    G4int fnCrystalsPerString;
    G4double fCleranceBetweenModules;
    G4String fModuleType;
    G4String fCrystalAssemblyType;
    G4double fCrystalDiameter;
    G4double fCrystalHeight;
    G4double fStringRadialClearance; // FIXME: Added by K. Nguyen
    G4double fRadialSpacingBetweenStrings; // FIXME: Added by K. Nguyen
    
    G4double fHeightOfWorldVolume;//allow access through messenger to make this big enough to have muons from far up
    TSMJDemModInTankParallelWorld* fParallelWorld;
    TSCylinderShieldTank *fCylinderTank;
    TSLabModuleBox *fLabModule;
    G4bool fTankIsSensitive;
    G4double fTankVerticalOffset;
    
    TSMJDemModInTankMessenger* fG4Messenger;
};
#endif

