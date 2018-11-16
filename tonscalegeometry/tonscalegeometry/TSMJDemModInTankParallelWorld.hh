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
 * 3-12-2012 R. Martin created
 */
//---------------------------------------------------------------------------//
//


#ifndef _TSMJDEMMODINTANKPARALLELWORLD_HH
#define _TSMJDEMMODINTANKPARALLELWORLD_HH


//---------------------------------------------------------------------------//
#include <string>
#include <vector>

#include "globals.hh"
#include "geometry/MGGeometryParallelWorld.hh"
#include "tonscalegeometry/TSMJDemModInTankParallelWorldMessenger.hh"

//---------------------------------------------------------------------------//
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;
class G4PVPlacement;

class TSMJDemModInTankParallelWorld : public MGGeometryParallelWorld
{
  public:
    TSMJDemModInTankParallelWorld();
    virtual ~TSMJDemModInTankParallelWorld(){};
  
    virtual void Construct();
  
    virtual G4VPhysicalVolume* GetWorldVolume() const { return fWorldVolume; }
    
    inline void SetnRockWorlds(G4int n){fnRockWorlds=n;}
    inline void SetnShotcreteWorlds(G4int n){fnShotcreteWorlds=n;}
    inline void SetnLabSpaceWorlds(G4int n){fnLabSpaceWorlds=n;}
    inline void SetnTankWorlds(G4int n){fnTankWorlds=n;}
  
    inline void SetOrigin(G4double x, G4double y, G4double z) { fOriginX = x, fOriginY = y; fOriginZ = z; }
  
    inline void SetTankOriginZ(G4double z) { fTankOriginZ = z; }
  
    inline void SetTankDimensions(G4double R, G4double H){ fTankRadius=R; fTankHeight=H; }
  
    inline void SetModuleClearanceDimensions(G4double R, G4double H){
      fModuleClearanceRadius=R;fModuleClearanceHeight=H;
    }
    
    inline void SetLabModuleSpaceDimensions(G4double X, G4double Y, G4double Z){
      fLabModuleSpaceX=X;fLabModuleSpaceY=Y;fLabModuleSpaceZ=Z;
    }
    
    inline void SetShotcreteDepth(G4double t){fShotcreteDepth=t;}
    inline void SetRockDepth(G4double t){fRockDepth=t;}

  private:
    TSMJDemModInTankParallelWorldMessenger *fG4Messenger;
    G4VPhysicalVolume* fWorldVolume;
    
    // Center of the tank:
    G4double fOriginX;
    G4double fOriginY;
    G4double fOriginZ;
    G4double fTankOriginZ;
  
    //Size of the tank
    G4double fTankRadius;
    G4double fTankHeight;
  
    //Volume around the cryostats (boundary of inner most parallel world)
    G4double fModuleClearanceRadius;
    G4double fModuleClearanceHeight;
  
    //Size of the lab module (inner, without the shot crete):
    G4double fLabModuleSpaceX;
    G4double fLabModuleSpaceY;
    G4double fLabModuleSpaceZ;
    
    G4double fShotcreteDepth;
    G4double fRockDepth;
  
    G4int fnRockWorlds;
    G4int fnShotcreteWorlds;
    G4int fnLabSpaceWorlds;
    G4int fnTankWorlds;
};
#endif
