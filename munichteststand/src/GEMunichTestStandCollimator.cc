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
 * AUTHOR:Manuela Jelen
 * CONTACT: mjelen *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 12-07-2006
 *
 * REVISION:
 *  
 */
// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"

#include "munichteststand/GEMunichTestStandCollimator.hh"
#include "gerdageometry/GEGeometrySD.hh"
#include "io/MGLogger.hh"

using namespace CLHEP;

// ---------------------------------------------------------------------------

GEMunichTestStandCollimator::GEMunichTestStandCollimator(GEMunichTestStandDB* teststanddb)
{
 fMotherVolume = teststanddb -> GetMotherVolume();

// ---------------------------------------------------------------------------
// Logical Volumes
// ---------------------------------------------------------------------------

  fCollimatorCrystalDeadLayerLogical   = teststanddb -> GetCollimatorCrystalDeadLayerLogical();
  fCollimatorCrystalSegmentLogical     = teststanddb -> GetCollimatorCrystalSegmentLogical();
  fCollimatorCrystalSiliconLogical     = teststanddb -> GetCollimatorCrystalSiliconLogical();
  fCollimatorVacuumLogical             = teststanddb -> GetCollimatorVacuumLogical();
  fCollimatorMotherVolumeLogical       = teststanddb -> GetCollimatorMotherVolumeLogical();
  fCollimatorSliceLogical              = teststanddb -> GetCollimatorSliceLogical();
  fCollimatorActiveLogical             = teststanddb -> GetCollimatorActiveLogical();
  fCollimatorFoilLogical               = teststanddb -> GetCollimatorFoilLogical();
  fCollimatorSourceHolderLogical       = teststanddb -> GetCollimatorSourceHolderLogical();
  fCollimatorSourceLogical             = teststanddb -> GetCollimatorSourceLogical();
// ---------------------------------------------------------------------------
// z-position
// ---------------------------------------------------------------------------
  
  fCollimatorPosition              =teststanddb ->GetCollimatorPosition();
  fCollimator_Crystal_CenterZ	   = teststanddb -> GetCollimatorCrystalCenterZ();
  fCollimatorSystem_Z              = teststanddb -> GetCollimatorSystemZ();

                                            
// ---------------------------------------------------------------------------
// geometry parameter
// ---------------------------------------------------------------------------

  fCollimator_Crystal_InnerRadius     = teststanddb -> GetCollimatorCrystalInnerRadius();
  fCollimator_Crystal_OuterRadius     = teststanddb -> GetCollimatorCrystalOuterRadius();
  fCollimator_Crystal_Height          = teststanddb -> GetCollimatorCrystalHeight();
  fCollimator_Crystal_SiliconThickness= teststanddb -> GetCollimatorCrystalSiliconThickness();
  iCollimator_Segment_Nphi            = teststanddb -> GetCollimatorSegmentNphi();
  iCollimator_Segment_Nz              = teststanddb -> GetCollimatorSegmentNz();
  fCollimator_Segment_Height          = teststanddb -> GetCollimatorSegmentHeight();
  fCollimator_Segment_dPhi            = 360.0 * deg / float(iCollimator_Segment_Nphi);
 //Collimator and Source
  fCollimatorSystem_R      = teststanddb -> GetCollimatorSystemR();
  fCollimatorSystem_Phi    = teststanddb -> GetCollimatorSystemPhi();
  fCollimator_MotherVolume_R = teststanddb -> GetCollimatorMotherVolumeR();
  fCollimator_MotherVolume_Height = teststanddb -> GetCollimatorMotherVolumeHeight();

  fCollimator_Height       = teststanddb -> GetCollimatorHeight();
  fCollimator_OuterRadius  = teststanddb -> GetCollimatorOuterRadius();
  fCollimator_LowerHeight  = teststanddb -> GetCollimatorLowerHeight();
  fCollimator_InnerRadius  = teststanddb -> GetCollimatorInnerRadius();
  fCollimator_Slice_Height = teststanddb -> GetCollimatorSliceHeight();
  iCollimator_Slice_Nz     = teststanddb -> GetCollimatorSliceNz();
  fCollimator_Foil_Height    = teststanddb -> GetCollimatorFoilHeight(); 
  fCollimator_SourceHolder_Height     = teststanddb -> GetCollimatorSourceHolderHeight();
  fCollimator_SourceHolder_InnerRadius= teststanddb -> GetCollimatorSourceHolderInnerRadius();
  fCollimator_SourceHolder_OuterRadius= teststanddb -> GetCollimatorSourceHolderOuterRadius();
  fCollimator_SourceHolder_rPosition  = teststanddb -> GetCollimatorSourceHolderrPosition();
  fCollimator_SourceHolder_phiPosition= teststanddb -> GetCollimatorSourceHolderphiPosition();
  fCollimator_SourceHolder_xPosition  = teststanddb -> GetCollimatorSourceHolderxPosition();
  fCollimator_SourceHolder_yPosition  = teststanddb -> GetCollimatorSourceHolderyPosition();
  fCollimator_SourceHolder_zPosition  = teststanddb -> GetCollimatorSourceHolderzPosition();
  fCollimator_Source_Radius           = fCollimator_SourceHolder_InnerRadius;
  fCollimator_Source_Height           = fCollimator_SourceHolder_InnerRadius;
  fCollimator_Detector_Distance       = teststanddb -> GetCollimatorDetectorDistance();
  fCollimator_System_Rotation         = teststanddb -> GetCollimatorSystemRotation();


}
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------

 GEMunichTestStandCollimator::~GEMunichTestStandCollimator()
 {
 /*
  delete fCollimatorVacuumPhysical;

   delete fCollimatorCrystalDeadLayerPhysical;
   delete fCollimatorCrystalSiliconTopPhysical; 
   delete fCollimatorCrystalSiliconBottomPhysical; 
   for (int i=0; i<18; i++) delete fCollimatorCrystalSegmentPhysical[i];
   delete fCollimatorMotherPhysical;
   delete fCollimatorPhysical;
   delete fCollimatorFoilPhysical;
  */
}
 
// ---------------------------------------------------------------------------
// Construct Teststand
// ---------------------------------------------------------------------------

void GEMunichTestStandCollimator::ConstructTestStand()
{
 
  G4cout << "Construct test stand" << G4endl; 

// --------------------------------------------------------------------------- 
// colors 
// --------------------------------------------------------------------------- 

  G4Colour red     (1.0,  0.0,  0.0,  0.5); //red
  G4Colour darkred (0.5,  0.0,  0.0,  0.5); //dark red
  G4Colour orange  (0.75, 0.55, 0.0,  0.5); //orange
  G4Colour darkgrey(0.7,  0.7,  0.7,  0.5); //dark gray
  G4Colour silver  (0.2,  0.2,  0.2,  0.5); //silver
  G4Colour white   (1.0,  1.0,  1.0,  0.5); //white
  G4Colour lgrey   (0.8,  0.8,  0.8,  0.5); //light grey
  G4Colour lgrey2  (0.85, 0.85, 0.85, 0.5); //medium grey
  G4Colour lblue   (0.0,  0.0,  0.75, 0.5); //light blue
  G4Color green(0.0, 1.0, 0.0);		    // green
  G4Color blue(0.0, 0.0, 1.0);		    // blue
  G4Color cyan(0.0, 1.0, 1.0);		    // cyan
  G4Color magenta(1.0, 0.0, 1.0);           // magenta 
  G4Color yellow(1.0, 1.0, 0.0);	    // yellow

// ---------------------------------------------------------------------------
// Visual Attributes
// ---------------------------------------------------------------------------
  G4VisAttributes* VacuumVisAtt = new G4VisAttributes(white); 
  VacuumVisAtt -> SetVisibility(true); 
  VacuumVisAtt -> SetForceSolid(true); 

  //G4VisAttributes* AirVisAtt = new G4VisAttributes(); 
  VacuumVisAtt -> SetVisibility(false); 

  G4VisAttributes* CrystalDeadLayerVisAtt = new G4VisAttributes(darkred);
  CrystalDeadLayerVisAtt -> SetVisibility(true);
  CrystalDeadLayerVisAtt -> SetForceSolid(true);

  G4VisAttributes* CrystalSegmentVisAtt = new G4VisAttributes(magenta);
  CrystalSegmentVisAtt -> SetVisibility(true);
  CrystalSegmentVisAtt -> SetForceSolid(true);
  
  G4VisAttributes* SiVisAtt = new G4VisAttributes(silver); 
  SiVisAtt -> SetVisibility(true); 
  SiVisAtt -> SetForceSolid(true); 
  
  G4VisAttributes* AlVisAtt = new G4VisAttributes(lgrey); 
  AlVisAtt -> SetVisibility(true); 
  AlVisAtt -> SetForceSolid(true);
  
  G4VisAttributes* SoVisAtt = new G4VisAttributes(blue); 
  SoVisAtt -> SetVisibility(true); 
  SoVisAtt -> SetForceSolid(true);

  G4VisAttributes* CuVisAtt = new G4VisAttributes(orange);
  CuVisAtt -> SetVisibility(true);
  CuVisAtt -> SetForceSolid(true);
 
// --------------------------------------------------------------------------- 
// variables 
// --------------------------------------------------------------------------- 


// --------------------------------------------------------------------------- 
// physical volumes 
// --------------------------------------------------------------------------- 

  G4ThreeVector fCollComponentsPosition;
  fCollComponentsPosition.setX(0.0*cm);
  fCollComponentsPosition.setY(0.0*cm);
  fCollComponentsPosition.setZ(0.0*cm);



 // vacuum

  fCollimatorVacuumLogical -> SetVisAttributes(VacuumVisAtt);

  fCollComponentsPosition.setX(0.0*cm);
  fCollComponentsPosition.setY(0.0*cm);
  fCollComponentsPosition.setZ(fCollimator_Crystal_CenterZ);
  fCollimatorVacuumPhysical = new G4PVPlacement(0,fCollComponentsPosition,
                                      fCollimatorVacuumLogical,
                                      "Vacuum",
                                      fMotherVolume,
                                      false,
                                      0);

 //  // dead layer 

  fCollimatorCrystalDeadLayerLogical -> SetVisAttributes(CrystalDeadLayerVisAtt); 
 
  fCollComponentsPosition.setX(0.0*cm);
  fCollComponentsPosition.setY(0.0*cm);
  fCollComponentsPosition.setZ(fCollimator_Crystal_CenterZ);
  fCollimatorCrystalDeadLayerPhysical = new G4PVPlacement(0,fCollComponentsPosition,
                                                fCollimatorCrystalDeadLayerLogical,
                                                "CrystalDeadLayer",
                                                fCollimatorVacuumLogical,
                                                false,
                                                0);


 // silicon 

  fCollimatorCrystalSiliconLogical -> SetVisAttributes(SiVisAtt); 

  fCollComponentsPosition.setX(0.0*cm);
  fCollComponentsPosition.setY(0.0*cm);
  fCollComponentsPosition.setZ( (fCollimator_Crystal_Height - fCollimator_Crystal_SiliconThickness) * 0.5-fCollimator_Crystal_CenterZ);
  fCollimatorCrystalSiliconTopPhysical = new G4PVPlacement(0,fCollComponentsPosition,
                                                 fCollimatorCrystalSiliconLogical,
                                                 "CrystalSiliconTop",
                                                 fCollimatorVacuumLogical,
                                                 false,
                                                 0);

  fCollComponentsPosition.setX(0.0*cm);
  fCollComponentsPosition.setY(0.0*cm);
  fCollComponentsPosition.setZ( 0.0 - (fCollimator_Crystal_Height - fCollimator_Crystal_SiliconThickness) * 0.5-fCollimator_Crystal_CenterZ);
  fCollimatorCrystalSiliconBottomPhysical = new G4PVPlacement(0,fCollComponentsPosition,
                                                 fCollimatorCrystalSiliconLogical,
                                                 "CrystalSiliconBottom",
                                                 fCollimatorVacuumLogical,
                                                 false,
                                                 0);

  // segments 
	  
  fCollimatorCrystalSegmentLogical -> SetVisAttributes(CrystalSegmentVisAtt); 

  G4RotationMatrix fSegmentRotation;
  int icopynumber=0;

  char volumename[30];
  for (int iz=0; iz<iCollimator_Segment_Nz; iz++) {

    fCollComponentsPosition.setX(0.0*cm);
    fCollComponentsPosition.setY(0.0*cm);
    fCollComponentsPosition.setZ(float(iz-1)*fCollimator_Segment_Height-fCollimator_Crystal_CenterZ);

    for (int iphi=0; iphi<iCollimator_Segment_Nphi; iphi++) 
{ 

      fSegmentRotation.rotateZ(float(iphi) * fCollimator_Segment_dPhi);
      fSegmentRotation.rotateX(0.0 * deg);
      fSegmentRotation.rotateY(0.0 * deg);

      sprintf(volumename, "segment_z%d_phi%d", iz, iphi);
      fCollimatorCrystalSegmentPhysical[icopynumber] = new G4PVPlacement(
                                       G4Transform3D(fSegmentRotation, fCollComponentsPosition),
                                       fCollimatorCrystalSegmentLogical,
                                       volumename,
                                       fCollimatorVacuumLogical,
                                       false,
                                       icopynumber);
      fSegmentRotation.rotateZ(0.0 - float(iphi) * fCollimator_Segment_dPhi);

      icopynumber++;
    }}


 //collimator mother volume
  fCollimatorMotherVolumeLogical -> SetVisAttributes(VacuumVisAtt);


  G4ThreeVector CollimatorSystemPosition;
/*  CollimatorSystemPosition.setX(fCollimatorSystem_R * cos(fCollimatorSystem_Phi));
  CollimatorSystemPosition.setY(fCollimatorSystem_R * sin(fCollimatorSystem_Phi));
  CollimatorSystemPosition.setZ(fCollimatorSystem_Z);
*/
 if (fCollimatorPosition == 1)
{
  CollimatorSystemPosition.setX(fCollimatorSystem_R * cos(fCollimatorSystem_Phi));
  CollimatorSystemPosition.setY(fCollimatorSystem_R * sin(fCollimatorSystem_Phi));
  CollimatorSystemPosition.setZ/*(fCollimatorSystem_Z + 0.5 * (fCollimator_MotherVolume_Height + fCollimator_Crystal_Height)) =*/( fCollimator_Detector_Distance + 0.5*(fCollimator_Crystal_Height + fCollimator_Height));


}
 if(fCollimatorPosition == 2)
{
  CollimatorSystemPosition.setX/*((0.5 * fCollimator_MotherVolume_Height + fCollimator_Crystal_OuterRadius) * cos(fCollimatorSystem_Phi)) = */((fCollimator_Detector_Distance + fCollimator_Crystal_OuterRadius + 0.5 * fCollimator_Height) * cos(fCollimatorSystem_Phi));
  CollimatorSystemPosition.setY(0.0);
  CollimatorSystemPosition.setZ(fCollimatorSystem_Z + fCollimatorSystem_R);


} 

  G4RotationMatrix* CollimatorSystemRotation = new G4RotationMatrix();
  if (fCollimatorPosition == 2)
  {CollimatorSystemRotation -> rotateY(-90.0 * deg);}
  CollimatorSystemRotation -> rotateZ(fCollimator_System_Rotation);

  

   fCollimatorMotherPhysical
      = new G4PVPlacement(CollimatorSystemRotation, 
			  CollimatorSystemPosition,
			  fCollimatorMotherVolumeLogical,
			  "CollimatorMotherVolume",
			  fMotherVolume,
			  false,
			  0);    
  


 //collimator
  fCollimatorActiveLogical -> SetVisAttributes(CuVisAtt);

  fCollimatorPhysical
      = new G4PVPlacement(0, 
			  G4ThreeVector(0,0,0),
			  fCollimatorActiveLogical,
			  "Collimator",
			  fCollimatorMotherVolumeLogical,
			  false,
			  0);

 fCollimatorFoilPhysical = new G4PVPlacement(0,
                                             G4ThreeVector(0,0,-0.5*fCollimator_Height),
                                             fCollimatorFoilLogical,
                                             "CollimatorFoil",
                                             fCollimatorMotherVolumeLogical,
                                             false,
                                             0);

 
  //Source holder
    fCollimatorSourceHolderLogical -> SetVisAttributes(AlVisAtt);
    fCollimatorSourceHolderPhysical 
      = new G4PVPlacement(0,
			  G4ThreeVector(0,0,0.5*(fCollimator_Height-fCollimator_SourceHolder_Height)),
			  fCollimatorSourceHolderLogical,
			  "SourceHolder",
			  fCollimatorMotherVolumeLogical,
			  false,
			  0);
    
    //Source
    fCollimatorSourceLogical -> SetVisAttributes(SoVisAtt);
 G4cout << "Construct source" << G4endl;
    fCollimatorSourcePhysical 
      = new G4PVPlacement(0,
			  G4ThreeVector(0,0,0.5*fCollimator_Height-fCollimator_SourceHolder_Height+0.3*mm),
			  fCollimatorSourceLogical,
			  "Source",
			  fCollimatorMotherVolumeLogical,
			  false,
			  0);
 G4cout << "all" << G4endl;
/*
 G4RotationMatrix* RotationZ = new G4RotationMatrix();
 RotationZ -> rotateZ(fCollimator_System_Rotation);

  

   fCollimatorMotherPhysical
      = new G4PVPlacement(RotationZ, 
			  CollimatorSystemPosition,
			  fCollimatorPhysical,
			  "MotherVolume",
			  fMotherVolume,
			  false,
			  0);    
*/
}





