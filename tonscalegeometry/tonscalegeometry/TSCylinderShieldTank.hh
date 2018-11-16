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
 * CLASS DECLARATION:  TSCylinderShieldTank.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
/**
 *
 * Big cylinder shield tank that can be filled with eg. LAr, LS or H20
 * 
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
 * 02-14-2011, Created, R. Martin
 * 06-15-2011, Added steel tank, R. Martin
 *
 */
//---------------------------------------------------------------------------//
//



#ifndef _TSCYLINDERSHIELDTANK_HH_
#define _TSCYLINDERSHIELDTANK_HH_

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"

class TSCylinderShieldTankMessenger; 

class TSCylinderShieldTank 
{
  public:
    TSCylinderShieldTank();
    virtual ~TSCylinderShieldTank();
    
    void ConstructTankMaterials();
    void Construct();
  	
    G4double GetTankClearanceRadius(){ return fTankRadius+fTankWallThickness; }
    G4double GetTankClearanceHeight(){ return fTankHeight+2*fTankWallThickness; }
  
    G4double GetTankRadius(){ return fTankRadius; }
    G4double GetTankHeight(){ return fTankHeight; }
    
    void SetShieldMaterial(G4String argMaterialName){ fTankContentMaterial=G4Material::GetMaterial(argMaterialName); }
    void SetTankWallMaterial(G4String argMaterialName){ fTankWallMaterial=G4Material::GetMaterial(argMaterialName); }
    void SetTankRadius(G4double argRadius){ fTankRadius=argRadius; }
    void SetTankHeight(G4double argHeight){ fTankHeight=argHeight; }
    void SetTankWallThickness(G4double argT){ fTankWallThickness=argT; }
  
    G4LogicalVolume*  GetTankWallLogical(){ return fTankWallLogical; }
    G4LogicalVolume*  GetTankContentLogical(){ return fTankContentLogical; }
  
  private:
    G4Material* fTankContentMaterial;
    G4Material* fTankWallMaterial;
    
    G4double fTankWallThickness;
    
    G4double fTankRadius;
    G4double fTankHeight;
    
    
    G4LogicalVolume* fTankContentLogical;
    G4LogicalVolume* fTankWallLogical;
    
    TSCylinderShieldTankMessenger* fG4Messenger;
};

#endif // _TSSHIELDTANK_HH_ not defined
