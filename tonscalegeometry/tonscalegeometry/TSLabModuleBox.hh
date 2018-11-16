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
 * CLASS DECLARATION:  TSLabModuleBox.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
/**
 *
 *  Lab module shaped like a box, can specify thickness of rock that is modeled
 *  as well as rock material.
 *  Domed ceiling not yet implemented
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
 *  02-14-2010, Created, R. Martin
 * REVISION:
 *
 * 02-14-2010, Created, R. Martin
 *
 */
//---------------------------------------------------------------------------//
//

#ifndef _TSLABMODULEBOX_HH_
#define _TSLABMODULEBOX_HH_

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
#include "G4UnionSolid.hh"

#include "tonscalegeometry/TSLabModuleBoxMessenger.hh"

class TSLabModuleBox 
{
  public:
    TSLabModuleBox();
    virtual ~TSLabModuleBox(){
      delete fRockMaterial; delete fLabSpaceMaterial; delete fRockBoxLogical; delete fLabSpaceLogical; delete fLabSpacePlacement;
    }
    
    void ConstructLabModuleMaterials();
    void Construct();
    
    G4LogicalVolume* GetRockBoxLogicalVolume(){ return fRockBoxLogical; }
    G4LogicalVolume* GetLabSpaceLogicalVolume(){ return fLabSpaceLogical; }
    G4double GetLabSpaceBoxLength(){ return fLabSpaceBoxLength; }
    G4double GetLabSpaceBoxWidth(){ return fLabSpaceBoxWidth; }
    G4double GetLabSpaceBoxHeight(){ return fLabSpaceBoxHeight; }
    G4double GetRockDepth(){ return fLabModuleRockDepth; }
    G4double GetShotcreteDepth(){ return fShotcreteDepth; }	
    G4double GetLabSpaceDomeOverhead(){ return fLabSpaceDomeOverhead; }
    G4double GetLabModuleBoxLength(){ return fLabSpaceBoxLength+2.*fLabModuleRockDepth+2.*fShotcreteDepth; }
    G4double GetLabModuleBoxWidth(){ return fLabSpaceBoxWidth+2.*fLabModuleRockDepth+2.*fShotcreteDepth; }
    G4double GetLabModuleBoxHeight(){ return fLabSpaceBoxHeight+2.*fLabModuleRockDepth+2.*fShotcreteDepth; }
    
    void SetRockMaterialName(G4String argMaterialName){ fRockMaterial=G4Material::GetMaterial(argMaterialName); }
    void SetLabSpaceMaterialName(G4String argMaterialName){ fLabSpaceMaterial=G4Material::GetMaterial(argMaterialName); }	
    void SetShotcreteMaterialName(G4String argMaterialName){ fShotcreteMaterial=G4Material::GetMaterial(argMaterialName); }	
    void SetLabSpaceBoxWidth(G4double argD){ fLabSpaceBoxWidth=argD; }
    void SetLabSpaceBoxLength(G4double argD){ fLabSpaceBoxLength=argD; }
    void SetLabSpaceBoxHeight(G4double argD){ fLabSpaceBoxHeight=argD; }
    void SetRockDepth(G4double argD){ fLabModuleRockDepth=argD; }
    void SetShotcreteDepth(G4double argD){ fShotcreteDepth=argD; }
  
  private:
    G4LogicalVolume* fRockBoxLogical;
    G4LogicalVolume* fLabSpaceLogical;
    G4LogicalVolume* fShotcreteLogical;
    G4PVPlacement* fLabSpacePlacement;
    G4PVPlacement* fShotcretePlacement; 
    
    G4Material* fRockMaterial;
    G4Material* fLabSpaceMaterial;
    G4Material* fShotcreteMaterial;
    G4double fLabSpaceBoxWidth;
    G4double fLabSpaceBoxLength;
    G4double fLabSpaceBoxHeight;
    G4double fLabSpaceDomeOverhead;
    G4double fLabModuleRockDepth;
    G4double fShotcreteDepth;
    
    TSLabModuleBoxMessenger* fG4Messenger;
};

#endif // _TSLABMODULEBOX_HH_ not defined
