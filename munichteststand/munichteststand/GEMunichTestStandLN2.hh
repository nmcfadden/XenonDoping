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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 02-06-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 
 */

// ---------------------------------------------------------------------------

#ifndef _GEMUNICHTESTSTANDLN2_HH 
#define _GEMUNICHTESTSTANDLN2_HH 

// --------------------------------------------------------------------------- 

#include "globals.hh"
#include "G4LogicalVolume.hh" 

#include "munichteststand/GEMunichTestStandDB.hh"

// --------------------------------------------------------------------------- 

class GEMunichTestStandLN2
{

public:
  
  // default constructor 

  GEMunichTestStandLN2(GEMunichTestStandDB* teststanddb); 

  // destructor 

  ~GEMunichTestStandLN2();

  // methods 

  void ConstructTestStand(); 

  G4ThreeVector GetSourcePosition(G4int n); 

private: 

  G4double fDewarHeight; 
  G4double fLN2Height; 
  //G4double fCrystalDistanceToSource; 
  G4double fStringDetectorHeight; 
  G4double fStringSourceHeight; 
  G4double fCrystalHeight; 
  //G4double fCrystalOuterRadius; 
  G4double fSourceHolderHeightAEA; 
  G4double fSourceHeightAEA; 
  G4int    fSourceHoleNumber; 

  G4double fCrystalHolderDSGUnsegmentedGap; 
  G4double fCrystalHolderDSGUnsegmentedRing1Height; 
  G4double fCrystalHolderDSGUnsegmentedRing2Height; 
  G4double fCrystalHolderDSGUnsegmentedSkrewHeight; 
  G4double fCrystalHolderDSGUnsegmentedBarLengthX; 
  G4double fCrystalHolderDSGUnsegmentedBarLengthY;  
  G4double fCrystalHolderDSGUnsegmentedBarLengthZ; 
  G4double fCrystalHolderDSGUnsegmentedPlateSmallHeight; 
  G4double fCrystalHolderDSGUnsegmentedRodHeight; 
  G4double fCrystalHolderDSGUnsegmentedClampHeight; 

  G4ThreeVector fSkrewPosition0; 
  G4ThreeVector fSkrewPosition1; 
  G4ThreeVector fSkrewPosition2; 

  G4LogicalVolume* fMotherVolume; 
  G4LogicalVolume* fCrystalDSGUnsegmentedActiveLogical; 
  G4LogicalVolume* fCrystalDSGUnsegmentedDeadLayerLogical; 
  G4LogicalVolume* fSourceAEALogical; 
  G4LogicalVolume* fSourceHolderAEALogical; 
  G4LogicalVolume* fStringDetectorLogical; 
  G4LogicalVolume* fStringSourceLogical; 
  G4LogicalVolume* fDewarLogical; 
  G4LogicalVolume* fLN2Logical; 

  G4LogicalVolume* fCrystalHolderDSGUnsegmentedRing1Logical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedRing2Logical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedSkrewLogical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedBarLogical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedStringLogical;
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedPlateLargeLogical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedPlateSmallLogical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedRodLogical; 
  G4LogicalVolume* fCrystalHolderDSGUnsegmentedClampLogical; 

  G4VPhysicalVolume* fCrystalActivePhysical; 
  G4VPhysicalVolume* fCrystalDeadLayerPhysical; 
  G4VPhysicalVolume* fSourceAEAPhysical; 
  G4VPhysicalVolume* fSourceHolderAEAPhysical; 
  G4VPhysicalVolume* fStringDetectorPhysical; 
  G4VPhysicalVolume* fStringSourcePhysical; 
  G4VPhysicalVolume* fDewarPhysical; 
  G4VPhysicalVolume* fLN2Physical; 

  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedUpperRing1Physical; 
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedUpperRing2Physical; 
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedLowerRing1Physical; 
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedLowerRing2Physical; 
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedSkrew1Physical;     
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedSkrew2Physical;     
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedSkrew3Physical;     
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedBarPhysical;        
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedString1Physical;    
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedString2Physical;    
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedString3Physical;    
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedClamp1Physical;    
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedPlateLargePhysical; 
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedPlateSmallPhysical; 
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedRodPhysical; 
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedClamp2Physical;    
  G4VPhysicalVolume* fCrystalHolderDSGUnsegmentedClamp3Physical;    

}; 

// --------------------------------------------------------------------------- 

#endif 

