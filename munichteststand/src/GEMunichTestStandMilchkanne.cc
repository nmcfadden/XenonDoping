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
 * AUTHOR: Daniel Lenz
 * CONTACT: lenz *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 22-05-2007
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 *
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"

#include "munichteststand/GEMunichTestStandMilchkanne.hh"

using namespace CLHEP;

// ---------------------------------------------------------------------------

GEMunichTestStandMilchkanne::GEMunichTestStandMilchkanne(GEMunichTestStandDB* teststanddb)
{
  fMilchkanne_LidHole_XPosition       = teststanddb -> GetLidHoleXPosition();
  fMilchkanne_LidHole_YPosition       = teststanddb -> GetLidHoleYPosition();

  fMotherVolume                       = teststanddb -> GetMotherVolume();

  fMkDewarLogical                     = teststanddb -> GetMkDewarLogical();
  fMkMikeschLogical                   = teststanddb -> GetMkMikeschLogical();
  fMkHolderLogical                    = teststanddb -> GetMkHolderLogical();
  fMkTeflonWrapLogical                = teststanddb -> GetMkTeflonWrapLogical();
  fMkCoolingStickInsideLogical        = teststanddb -> GetMkCoolingStickInsideLogical();
  fMkCoolingStickOutsideLogical       = teststanddb -> GetMkCoolingStickOutsideLogical();
  fMkDeadLayerLogical                 = teststanddb -> GetMkDeadLayerLogical();
  fMkCoreElectrodeLogical             = teststanddb -> GetMkCoreElectrodeLogical();
  fMkSourceAEALogical                 = teststanddb -> GetSourceAEALogical();
  fMkSourceHolderAEALogical           = teststanddb -> GetSourceHolderAEALogical();
  //  fMkSourceMotherVolumeLogical        = teststanddb -> GetSiegfriedSourceMotherVolume();
  fMkLn2Logical                       = teststanddb -> GetMkLn2Logical();
  fMkDewarBetweenWallsVacuumLogical   = teststanddb -> GetMkDewarBetweenWallsVacuumLogical();
  fMilchkanne_SourceXPosition         = teststanddb -> GetMkSourceXPosition();
  fMikesch_Height                     = teststanddb -> GetMkMikeschHeight();
  fMikesch_SubtrHeight                = teststanddb -> GetMkMikeschSubtrHeight();
  fMikesch_CoolingStickInside_Height  = teststanddb -> GetMkCoolingstickInsideHeight();
  fMikesch_CoolingStickOutside_Height = teststanddb -> GetMkCoolingstickOutsideHeight();
 
}
// --------------------------------------------------------------------------- 

GEMunichTestStandMilchkanne::~GEMunichTestStandMilchkanne()
{
  delete fMkDewarPhysical;
  delete fMkMikeschPhysical;  
  delete fMkHolderPhysical;
  delete fMkCoolingStickInsidePhysical;  
  delete fMkCoolingStickOutsidePhysical;  
  delete fMkDeadLayerPhysical;  
  delete fMkCoreElectrodePhysical;
  delete fMkSourceAEAPhysical;
  delete fMkSourceHolderAEAPhysical;
  delete fMkLn2Physical;
  delete fMkDewarBetweenWallsVacuumPhysical;
  delete fMkTeflonWrapPhysical;
} 

// --------------------------------------------------------------------------- 

void GEMunichTestStandMilchkanne::ConstructTestStand()
{

  G4ThreeVector crystalposition; 
  G4ThreeVector holderposition;
  G4ThreeVector teflonwrapposition;
  G4ThreeVector dewarposition; 
  G4ThreeVector sourceholderposition;
  G4ThreeVector sourceposition; 
  G4ThreeVector coreelectrodeposition;
  G4ThreeVector coolingstickinsideposition;
  G4ThreeVector coolingstickoutsideposition;
  G4ThreeVector ln2position;
  G4ThreeVector sourcemotherposition;

  //  G4double x, y, z; 

  char volumename[30]; 


  dewarposition.setX(0.0 * mm); 
  dewarposition.setY(0.0 * mm); 
  dewarposition.setZ(0.0 * mm); 

  sprintf(volumename, "dewar"); 

  fMkDewarPhysical = new G4PVPlacement(0,
				       dewarposition,
				       fMkDewarLogical, 
				       volumename, 
				       fMotherVolume, 
				       false, 
				       0); 

  ln2position.setX(0.0 * mm ); 
  ln2position.setY(0.0 * mm); 
  ln2position.setZ(0.0 * mm); 


  sprintf(volumename, "ln2"); 
    
  fMkLn2Physical = new G4PVPlacement(0,
				     ln2position,
				     fMkLn2Logical, 
				     volumename, 
				     fMotherVolume, 
				     false, 
				     0); 


  sprintf(volumename, "vacuum"); 
    
  fMkDewarBetweenWallsVacuumPhysical = new G4PVPlacement(0,
							 ln2position,
							 fMkDewarBetweenWallsVacuumLogical, 
							 volumename, 
							 fMotherVolume, 
							 false, 
							 0); 



  // construct physical volume: crystal (active) 
  crystalposition.setX(fMilchkanne_LidHole_XPosition); 
  crystalposition.setY(fMilchkanne_LidHole_YPosition); 
  crystalposition.setZ(0.0 * mm); 

  sprintf(volumename, "mikesch_crystal"); 

  fMkMikeschPhysical = new G4PVPlacement(0,
					 crystalposition,
					 fMkMikeschLogical, 
					 volumename, 
					 fMkLn2Logical, 
					 false, 
					 0); 



  coreelectrodeposition.setX(fMilchkanne_LidHole_XPosition); 
  coreelectrodeposition.setY(fMilchkanne_LidHole_YPosition); 
  coreelectrodeposition.setZ(fMikesch_Height - fMikesch_SubtrHeight ); 

  sprintf(volumename, "coreelectrode"); 

  fMkCoreElectrodePhysical = new G4PVPlacement(0,
					       coreelectrodeposition,
					       fMkCoreElectrodeLogical, 
					       volumename, 
					       fMkLn2Logical, 
					       false, 
					       0); 
  

  holderposition.setX(fMilchkanne_LidHole_XPosition); 
  holderposition.setY(fMilchkanne_LidHole_YPosition); 
  holderposition.setZ(10 * mm); 

  sprintf(volumename, "holder"); 

  fMkHolderPhysical = new G4PVPlacement(0,
					holderposition,
					fMkHolderLogical, 
					volumename, 
					fMkLn2Logical, 
					false, 
					0); 

  teflonwrapposition.setX(fMilchkanne_LidHole_XPosition); 
  teflonwrapposition.setY(fMilchkanne_LidHole_YPosition); 
  teflonwrapposition.setZ(0.0 * mm); 

  sprintf(volumename, "teflonwrap"); 

  fMkTeflonWrapPhysical = new G4PVPlacement(0,
					teflonwrapposition,
					fMkTeflonWrapLogical, 
					volumename, 
					fMkLn2Logical, 
					false, 
					0); 


  coolingstickinsideposition.setX(fMilchkanne_LidHole_XPosition); 
  coolingstickinsideposition.setY(fMilchkanne_LidHole_YPosition); 
  coolingstickinsideposition.setZ(fMikesch_Height + fMikesch_CoolingStickInside_Height); 

  sprintf(volumename, "coolingstickinside"); 

  fMkCoolingStickInsidePhysical = new G4PVPlacement(0,
					      coolingstickinsideposition,
					      fMkCoolingStickInsideLogical, 
					      volumename, 
					      fMkLn2Logical, 
					      false, 
					      0); 
  
  coolingstickoutsideposition.setX(fMilchkanne_LidHole_XPosition); 
  coolingstickoutsideposition.setY(fMilchkanne_LidHole_YPosition); 
  coolingstickoutsideposition.setZ(fMikesch_Height + fMikesch_CoolingStickInside_Height * 2 + fMikesch_CoolingStickOutside_Height); 

  sprintf(volumename, "coolingstickoutside"); 

  fMkCoolingStickOutsidePhysical = new G4PVPlacement(0,
					      coolingstickoutsideposition,
					      fMkCoolingStickOutsideLogical, 
					      volumename, 
					      fMotherVolume, 
					      false, 
					      0); 


  //  std::cout << "Milchkanne source X Position = "<<fMilchkanne_SourceXPosition<<std::endl<<std::endl;
//   sourcemotherposition.setX(fMilchkanne_SourceXPosition * mm ); 
//   sourcemotherposition.setY(0.0 * mm); 
//   sourcemotherposition.setZ(0.0 * mm); 


//   sprintf(volumename, "sourceMotherVolumeAEA"); 
//   fMkSourceMotherVolumePhysical = new G4PVPlacement(0,
// 						    sourcemotherposition,
// 						    fMkSourceMotherVolumeLogical,
// 						    volumename, 
// 						    fMotherVolume, 
// 						    false, 
// 						    0); 


  sourceholderposition.setX(fMilchkanne_SourceXPosition * mm ); 
  sourceholderposition.setY(0.0 * mm); 
  sourceholderposition.setZ(7.5 * mm); 

  sprintf(volumename, "sourceholder");     
  fMkSourceHolderAEAPhysical = new G4PVPlacement(0,
						 sourceholderposition,
						 fMkSourceHolderAEALogical, 
						 volumename, 
						 fMotherVolume,
						 false, 
						 0); 


  sourceposition.setX(fMilchkanne_SourceXPosition * mm); 
  sourceposition.setY(0.0 * mm); 
  sourceposition.setZ(0.0 * mm); 

  sprintf(volumename, "source"); 
    
  fMkSourceAEAPhysical = new G4PVPlacement(0,
					   sourceposition,
					   fMkSourceAEALogical, 
					   volumename, 
					   fMotherVolume,
					   false, 
					   0); 







} 

// --------------------------------------------------------------------------- 

