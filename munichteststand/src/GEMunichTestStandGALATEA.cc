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
 * AUTHOR: Janicsko Jozsef
 * CONTACT: janicsko *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 12-11-2007
 *
 * REVISION:
 * First rev. J.J. 30.11.07.  
 *                     
 * 10-23-2009, Neslihan, fCollimatorHalfLength parameter is added to set the collimator length in the macro,  
 *                       source dimensions are changed to a realistic one.
 */

 //------------------------------------------------------------------------------------------------

 #include "globals.hh"
 #include "G4LogicalVolume.hh"
 #include "G4PVPlacement.hh"
 #include "G4VPhysicalVolume.hh"
 #include "G4SDManager.hh"

 #include "munichteststand/GEMunichTestStandGALATEA.hh" 
 #include "gerdageometry/GEGeometrySD.hh"

using namespace CLHEP;

 //------------------------------------------------------------------------------------------------

 GEMunichTestStandGALATEA::GEMunichTestStandGALATEA(GEMunichTestStandDB* teststanddb){

 fMotherVolume  	  = teststanddb -> GetMotherVolume(); 
 
 fCrystal_InnerRadius	  = teststanddb -> GetSiegfriedCrystalInnerRadius();
 fCrystal_OuterRadius	  = teststanddb -> GetSiegfriedCrystalOuterRadius();
 fCrystal_Height	  = teststanddb -> GetSiegfriedCrystalHeight();
 
 //fCrystalDeadLayerLogical = teststanddb -> GetSiegfriedCrystalDeadLayerLogical();
 //fCrystalSegmentLogical   = teststanddb -> GetSiegfriedCrystalSegmentLogical();
 //fCrystalSiliconLogical   = teststanddb -> GetSiegfriedCrystalSiliconLogical(); 
 //fCrystal_SiliconThickness= teststanddb -> GetSiegfriedCrystalSiliconThickness();
 //iSegment_Nphi  	  = teststanddb -> GetSiegfriedSegmentNphi();
 //iSegment_Nz		  = teststanddb -> GetSiegfriedSegmentNz();
 //fSegment_Height	  = teststanddb -> GetSiegfriedSegmentHeight();
 //fSegment_dPhi  	  = 360.0 * deg / float(iSegment_Nphi);
 
 //---------------------Collimator_par-------------------------------------------------------------
 
 fCollimatorInnerRadius   = teststanddb -> GetCollimatorInnerRadius();
 fCollimatorPosition      = teststanddb -> GetCollimatorPosition();
 fCollimatorSystem_R      = teststanddb -> GetCollimatorSystemR();
 fCollimator_System_Rotation = teststanddb -> GetCollimatorSystemRotation(); 
 fCollimator_LowerHeight  = teststanddb -> GetCollimatorLowerHeight();
 fCollimatorHalfLength   = teststanddb -> GetCollimatorHalfLength(); // new parameter to set the length of the collimator
 								     // default 25 mm 
 }

 //--------------------------------------------------------------------------- 

 GEMunichTestStandGALATEA::~GEMunichTestStandGALATEA(){

 delete fCollVolPhysical;
 delete fVacuumTopPhysical;
 delete fVacuumBottomPhysical;
 delete fVacuumRimPhysical;
 delete fVacuumChamberPhysical;
 delete fRasterPlatePhysical;
 delete fTeflonBox;
 delete fTnkhltrPhysical;
 delete KryoTankPhysical;
 delete KryoTankTopPhysical;
 delete KryoTankCuPhysical;
 delete KuehlFingrPhysical;
 delete fShieldPhysical;
 delete fColl_Plate_Physical;
 delete fColl_Plate_Physical_2;
 delete fCollPhysical;
 delete Coll5Physical;
 delete Coll6Physical;
 delete SourcePhysical;
 delete fCrystalPhysical;
 } 

 //--------------------------------------------------------------------------- 

 void  GEMunichTestStandGALATEA::ConstructTestStand(){
  
 // --------------------------------------------------------------------------- 
 // materials 
 // --------------------------------------------------------------------------- 
 
 G4Material* iron	     = G4Material::GetMaterial("MetalIron");
 G4Material* copper	     = G4Material::GetMaterial("MetalCopper");
 G4Material* vacuum	     = G4Material::GetMaterial("Vacuum"); 
 G4Material* natGe           = G4Material::GetMaterial("NaturalGe");
// G4Material* enrGe	     = G4Material::GetMaterial("EnrichedGe");
// G4Material* si 	     = G4Material::GetMaterial("MetalSilicon");
// G4Material* aluminium       = G4Material::GetMaterial("MetalAluminium"); 
 G4Material* teflon	     = G4Material::GetMaterial("Teflon"); 
 G4Material* tungsten	     = G4Material::GetMaterial("MetalTungsten");
 
 // --------------------------------------------------------------------------- 
 // colors 
 // --------------------------------------------------------------------------- 

 G4Colour red	  (1.0,  0.0,  0.0); 
 G4Colour darkred (0.5,  0.0,  0.0); 
 G4Colour orange  (0.75, 0.55, 0.0);
 //G4Colour darkgrey(0.7,  0.7,  0.7); 
 G4Colour silver  (0.2,  0.2,  0.2); 
 G4Colour white   (1.0,  1.0,  1.0); 
 G4Colour lgrey   (0.8,  0.8,  0.8); 
 //G4Colour lgrey2  (0.85, 0.85, 0.85); 
 G4Colour lblue   (0.0,  0.0,  0.75); 
 G4Color blue(0.0, 0.0, 1.0);		   // blue
 // --------------------------------------------------------------------------- 
 // visual attributes 
 // --------------------------------------------------------------------------- 

 G4VisAttributes* VacuumVisAtt = new G4VisAttributes(white); 
 VacuumVisAtt -> SetVisibility(true); 
 VacuumVisAtt -> SetForceSolid(true); 

 G4VisAttributes* LN2VisAtt = new G4VisAttributes(lgrey); 
 LN2VisAtt -> SetVisibility(true); 
 LN2VisAtt -> SetForceSolid(true); 

 G4VisAttributes* CrystalSegmentVisAtt = new G4VisAttributes(red);
 CrystalSegmentVisAtt -> SetVisibility(true);
 CrystalSegmentVisAtt -> SetForceSolid(true);

 G4VisAttributes* CrystalDeadLayerVisAtt = new G4VisAttributes(darkred);
 CrystalDeadLayerVisAtt -> SetVisibility(true);
 CrystalDeadLayerVisAtt -> SetForceSolid(true);

 G4VisAttributes* AlVisAtt = new G4VisAttributes(silver); 
 AlVisAtt -> SetVisibility(true); 
 AlVisAtt -> SetForceSolid(true); 

 G4VisAttributes* SiVisAtt = new G4VisAttributes(silver); 
 SiVisAtt -> SetVisibility(true); 
 SiVisAtt -> SetForceSolid(true); 

 G4VisAttributes* CuVisAtt = new G4VisAttributes(orange);
 CuVisAtt -> SetVisibility(true);
 CuVisAtt -> SetForceSolid(true);
 
 G4VisAttributes* IronVisAtt = new G4VisAttributes(lgrey); 
 VacuumVisAtt -> SetVisibility(true); 
 VacuumVisAtt -> SetForceSolid(true); 
 
 G4VisAttributes* SoVisAtt = new G4VisAttributes(blue); 
 SoVisAtt -> SetVisibility(true); 
 SoVisAtt -> SetForceSolid(true);

 // ------------------------------------------------------------------------------------------------
 // variables 
 // ------------------------------------------------------------------------------------------------

 G4double Actual_Z(0);  

 //************************************************************************************************
 // Vacuum Chamber with spherical top 
 //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
 
 G4double VacuChambInnerRadius   = 29.5*cm; 
 G4double VacuChambOuterRadius   = 30.0*cm; 
 G4double VacuChambHalfHeight	= 28.0 *cm;  

 G4Tubs* VacuumChamber = new G4Tubs("VacuumChamber", VacuChambInnerRadius, 
				       VacuChambOuterRadius,VacuChambHalfHeight,0., twopi);  			      
 G4LogicalVolume* fVacuumChamberLogical = new G4LogicalVolume(VacuumChamber, iron  , "VacuumChamberLogical");   
 fVacuumChamberLogical -> SetVisAttributes(IronVisAtt); 

 G4double phi_angle(0);
 G4double sphere_radius(0);
 
 phi_angle = atan(5.*cm/VacuChambOuterRadius); // 5 cm is the height of the spherical section
 sphere_radius = VacuChambOuterRadius * VacuChambOuterRadius/(5.*cm) + 5.*cm;
 
 G4Sphere* VacuumTop = new G4Sphere("VacuumTop",sphere_radius - 5.*mm,sphere_radius, 0.,twopi, 
								        pi - phi_angle,  phi_angle);
 G4LogicalVolume* VacuumTopLogical = new G4LogicalVolume(VacuumTop,iron , "VacuumTopLogical");
 VacuumTopLogical -> SetVisAttributes(IronVisAtt); 
 
 G4Tubs* VacuumBottom = new G4Tubs("VacuumBottom", 0., VacuChambOuterRadius + 2.*cm,1.*cm,0., twopi);			      
 G4LogicalVolume* fVacuumBottomLogical = new G4LogicalVolume(VacuumBottom, iron  , 
								            "VacuumChamberLogical");
 
 fVacuumBottomLogical-> SetVisAttributes(IronVisAtt);  
 
 G4RotationMatrix* rot1 = new G4RotationMatrix();
 rot1->rotateX(180.0*deg); 
  fVacuumTopPhysical = new G4PVPlacement(rot1, G4ThreeVector(0.0, 0.0, 
				              -1*sphere_radius + VacuChambHalfHeight + 3.2*cm), 
        			              VacuumTopLogical, "vac_Top", fMotherVolume, false, 0); 
			  
        		  
 fVacuumChamberPhysical = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), fVacuumChamberLogical, 
						       	    "vac_chamber", fMotherVolume, false, 0); 
        			  
        			  
  fVacuumBottomPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0.,
					        -VacuChambHalfHeight - 1.*cm), fVacuumBottomLogical, 
        				        "vac_Bot", fMotherVolume, false, 0); 
 Actual_Z -= VacuChambHalfHeight;
 
 //decorative elements
 G4Tubs* VacuumChamberRim = new G4Tubs("VacuumChamberRim",VacuChambOuterRadius , 
				       VacuChambOuterRadius + 1.5*cm,18.*mm,0., twopi);
        			       
 G4LogicalVolume* fRimLogical = new G4LogicalVolume(VacuumChamberRim, iron ,"RimLogical");
  fVacuumRimPhysical = new G4PVPlacement(0, 
				       G4ThreeVector(0.0, 0.0, VacuChambHalfHeight - 1.8*cm), 
        			       fRimLogical, "vac_Rim", fMotherVolume, false, 0);
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 //crystal support, cooling finger, all that 
 //##################################################################################################
 // optical raster plate
 //--------------------------------------------------------------------------------------------------
 
 G4double RP_thickns(1.*cm);
 G4Tubs* RasterPlate = new G4Tubs("RasterPlate", 0., VacuChambOuterRadius - 1.*cm,RP_thickns,0., twopi); 
 G4LogicalVolume* fRasterPlateLogical = new G4LogicalVolume(RasterPlate, iron,"RasterPlateLogical"); 
 fRasterPlateLogical-> SetVisAttributes(IronVisAtt);
 
   fRasterPlatePhysical = new G4PVPlacement(0, G4ThreeVector(0.,0.,Actual_Z + RP_thickns), 
        				    fRasterPlateLogical,"RasterPlate", fMotherVolume, 
        				    false, 0);
 Actual_Z += 2*RP_thickns;     
				   
 //-------------------------------------------------------------------------------------------------
 // "Grundplatte" Teflon
 //-------------------------------------------------------------------------------------------------					  
 
 G4double Tfln_thickns(3.*cm); //half of the teflon holder height
 G4Box* TeflonBox	 = new G4Box("TeflonBox",	14.*cm , 14.*cm, Tfln_thickns);
 G4Box* TeflonBoxCutOff  = new G4Box("TeflonBoxCutOff", 50.*mm , 60.*mm, 3.2*cm);
 G4Box* TeflonBoxCutOff2 = new G4Box("TeflonBoxCutOff2",20.*mm , 44.*mm, 3.2*cm);

 G4Tubs* TeflonTubeCutOff = new G4Tubs("TeflonTubeCutOff", 0.,50.*mm,3.1*cm,0., twopi);
 G4Tubs* TeflonTubeCutOff2 = new G4Tubs("TeflonTubeCutOff2", 0.,90.*mm,1.1*cm,0., twopi);
 
 G4SubtractionSolid* Tfln1 = new G4SubtractionSolid("Tfln1",TeflonBox,TeflonBoxCutOff, 0,
					                               G4ThreeVector(96.*mm,0.,0.));
 G4SubtractionSolid* Tfln2 = new G4SubtractionSolid("Tfln2",Tfln1,TeflonTubeCutOff, 0,G4ThreeVector());
 G4SubtractionSolid* Tfln3 = new G4SubtractionSolid("Tfln2",Tfln2,TeflonTubeCutOff2, 0, 
					                               G4ThreeVector(0.,0.,2.1*cm));
 G4SubtractionSolid* Tfln4 = new G4SubtractionSolid("Tfln3",Tfln3,TeflonBoxCutOff2, 0, 
					                            G4ThreeVector(40.*mm,0.,0.*cm));
  
 G4LogicalVolume* TeflonBoxLogical = new G4LogicalVolume(Tfln4, teflon , "TeflonBoxLogical");
  
  
 fTeflonBox = new G4PVPlacement(0, G4ThreeVector(0.,0.,Actual_Z + Tfln_thickns), 
			      	             TeflonBoxLogical,"TeflonBox", fMotherVolume, false, 0);
 Actual_Z += 2*Tfln_thickns;
  
 //-------------------------------------------------------------------------------------------------
 // "Tankhalter" Edelstahl
 //-------------------------------------------------------------------------------------------------
 
 //TODO prmetrize z coord
 
 G4double tnkhalter_half_height(53.5*mm); 
 
 G4Tubs* Tnkhltr1 = new G4Tubs("Tnkhltr1", 43.*mm,85.*mm,tnkhalter_half_height, 60.*deg, 240.*deg);
 G4Tubs* Tnkhltr2 = new G4Tubs("Tnkhltr2", 55.*mm,86.*mm,tnkhalter_half_height, 0., twopi);
 G4Tubs* Tnkhltr3 = new G4Tubs("Tnkhltr3", 42.*mm,50.*mm,tnkhalter_half_height, 0., twopi);
 
 G4SubtractionSolid* Tnkhltr4 = new G4SubtractionSolid("Tnkhltr4",Tnkhltr1,Tnkhltr2, 0,
  						                        G4ThreeVector(0.,0.,5.*mm));
 
 G4SubtractionSolid* Tnkhltr5 = new G4SubtractionSolid("Tnkhltr5",Tnkhltr4,Tnkhltr3, 0,
  						                      G4ThreeVector(0.,0.,-17.*mm));
 
 G4LogicalVolume* TnkhltrLogical = new G4LogicalVolume(Tnkhltr5, iron , "TnkhltrLogical");
 
 TnkhltrLogical-> SetVisAttributes(AlVisAtt);
 
 fTnkhltrPhysical = new G4PVPlacement(0,G4ThreeVector(0.,0.,Actual_Z + tnkhalter_half_height -2.*cm), 
 				                 TnkhltrLogical,"Tnkhltr", fMotherVolume, false, 0); 
			
 Actual_Z += (2*tnkhalter_half_height - 2.*cm);
 
  //------------------------------------------------------------------------------------------------
  // Kryotank 
  //------------------------------------------------------------------------------------------------
  
  G4double kryotank_bottom_half_height(55.0*mm); 
  
  G4Tubs* KryoTank1 = new G4Tubs("KryoTank1", 0.,100.*mm,kryotank_bottom_half_height, 0, twopi);
  G4Tubs* KryoTank2 = new G4Tubs("KryoTank2", 0., 98.*mm,kryotank_bottom_half_height -0.5*cm,0,twopi);
  
  
  G4SubtractionSolid* KryoTank3 = new G4SubtractionSolid("KryoTank3",KryoTank1,KryoTank2, 0,
  						G4ThreeVector(0.,0., 5.*mm));
  
  
  G4Tubs* KryoTank4 = new G4Tubs("KryoTank4", 34.*mm, 100.*mm, 5.*mm, 0, twopi);
  
  
  G4LogicalVolume* KryoTankLogical = new G4LogicalVolume(KryoTank3 , iron , "KryoTankLogical");
  
  KryoTankLogical-> SetVisAttributes(IronVisAtt);
  
  KryoTankPhysical = new G4PVPlacement(0,G4ThreeVector(0.,0.,Actual_Z + kryotank_bottom_half_height),
 					        KryoTankLogical,"KryoTank", fMotherVolume, false, 0); 
					
  G4LogicalVolume* KryoTankTopLogical = new G4LogicalVolume(KryoTank4 , iron , "KryoTankLogical");

  KryoTankTopLogical-> SetVisAttributes(IronVisAtt);

  KryoTankTopPhysical = new G4PVPlacement(0, 
 			     G4ThreeVector(0.,0., Actual_Z + 2*kryotank_bottom_half_height + 5.*mm), 
 	                 	           KryoTankTopLogical,"KryoTankT", fMotherVolume, false, 0); 
  
  //copper cylinder
  G4Tubs* KryoTank6 = new G4Tubs("KryoTank6", 55.*mm,60.*mm, kryotank_bottom_half_height - 5.*mm, 0, twopi);
  
  G4LogicalVolume* KryoTankCuLogical = new G4LogicalVolume(KryoTank6 , copper  , "KryoTankLogical");
  
  KryoTankCuLogical-> SetVisAttributes(CuVisAtt);
  
  KryoTankCuPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., 5.*mm), 
 					 KryoTankCuLogical,"KryoTankCu",KryoTankLogical , false, 0); 
 
  Actual_Z += (2*kryotank_bottom_half_height + 10.*mm);
  
  //------------------------------------------------------------------------------------------------
  // Kuehlfinger 
  //------------------------------------------------------------------------------------------------
  
  G4double KuehlFingr_half_height(73.5*mm);
  
  G4Tubs* KuehlFingr = new G4Tubs("KuehlFingr", 0.,8.*mm,KuehlFingr_half_height , 0, twopi);
  G4LogicalVolume* KuehlFingrLogical = new G4LogicalVolume(KuehlFingr ,copper  , "KuehlFingrLogical");					
  KuehlFingrLogical-> SetVisAttributes(CuVisAtt);
  KuehlFingrPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0., Actual_Z - 31.5*mm), 
 					   KuehlFingrLogical,"KuehlFingr", fMotherVolume, false, 0);
					
  				
  //------------------------------------------------------------------------------------------------
  //Copper shield
  //------------------------------------------------------------------------------------------------
  
  G4double shield_half_height(67.*mm); 
 
  G4Tubs* Shield1 = new G4Tubs("Shield1", 0. *mm,120.*mm,shield_half_height, 0.,twopi);
  G4Tubs* Shield2 = new G4Tubs("Shield2", 0. *mm, 45.*mm,shield_half_height, 0.,twopi);
  G4Tubs* Shield3 = new G4Tubs("Shield3", 50.*mm,121.*mm,shield_half_height, 0.,twopi);
  
  G4SubtractionSolid* Shield4 = new G4SubtractionSolid("Shield4",Shield1,Shield2, 0,
  						                       G4ThreeVector(0.,0.,-7.*mm));
  
  G4SubtractionSolid* Shield5 = new G4SubtractionSolid("Shield5",Shield4,Shield3, 0,
  						                        G4ThreeVector(0.,0.,7.*mm));
  G4Box*  Slit(0); G4SubtractionSolid* Shield6(0);
  	
         if (fCollimatorPosition == 1){
	 
  Slit    = new G4Box ("Slit",    3.*mm, 45*mm, 10.*mm);
  Shield6 = new G4SubtractionSolid("Shield6",Shield5,Slit,0,G4ThreeVector(0.,22.5*mm,shield_half_height));
  
  } else if (fCollimatorPosition == 2){
  
  Slit    = new G4Box ("Slit",    3.*mm, 10.*mm, 50.*mm);
  Shield6 = new G4SubtractionSolid("Shield6",Shield5,Slit, 0, G4ThreeVector(0.,47.5*mm, 0.));
  
  }							     
								     
								     
								     
  G4LogicalVolume* ShieldLogical = new G4LogicalVolume(Shield6, copper , "ShieldLogical");						 
  
  ShieldLogical-> SetVisAttributes(CuVisAtt);  
  
  fShieldPhysical = new G4PVPlacement(0, G4ThreeVector(0.,0.,Actual_Z + shield_half_height), 
		                            ShieldLogical,"ShieldPhysical", fMotherVolume,false, 0);
		
  Actual_Z += (KuehlFingr_half_height - 31.5*mm); //this is the top of the structure

  //------------------------------------------------------------------------------------------------
  //Collimator , hardcoded tungsten material,  all put in a vacuum filled box for easy manip.
  //------------------------------------------------------------------------------------------------
  
////////  G4Box* Coll_Vol = new G4Box("Coll_Vol", 24.*mm , 70.*mm, 41.*mm); // for 50 mm collimator length
////////  G4Box* Coll_Vol = new G4Box("Coll_Vol", 24.*mm , 70.*mm, 31.*mm); //for 30mm collimator length
  //modify// add fCollimatorHalfLength parameter
  G4Box* Coll_Vol = new G4Box("Coll_Vol", 24.*mm , 70.*mm, fCollimatorHalfLength + 16.*mm); 
  G4LogicalVolume* CollVolLogical = new G4LogicalVolume(Coll_Vol, vacuum ,"CollVolLogical");
 
/////////  G4Tubs* Coll1      = new G4Tubs("Coll1",fCollimatorInnerRadius,20.*mm,25.*mm,0.,twopi);  // for 50 mm collimator length
/////////  G4Tubs* Coll1      = new G4Tubs("Coll1",fCollimatorInnerRadius,20.*mm,15.*mm,0.,twopi); //for 30mm collimator length
  //modify// add fCollimatorHalfLength parameter
  G4Tubs* Coll1      = new G4Tubs("Coll1",fCollimatorInnerRadius,20.*mm,fCollimatorHalfLength,0.,twopi);
  G4Box*  Coll_plate = new G4Box ("Coll_plate", 24.*mm , 70.*mm, 1.5*mm);
  G4Tubs* Coll2      = new G4Tubs("Coll2", 0, 3.*mm, 8.*mm, 0.,twopi);
  
  G4SubtractionSolid* Coll3 = new G4SubtractionSolid("Coll3",Coll_plate,Coll2, 0,G4ThreeVector());
  G4LogicalVolume* Coll_Plate_Logical = new G4LogicalVolume(Coll3, tungsten , "CollimLogical");
////////  fColl_Plate_Physical = new G4PVPlacement(0,  G4ThreeVector(0.,0.,-35.5*mm), // for 50 mm collimator length
////////  fColl_Plate_Physical = new G4PVPlacement(0,  G4ThreeVector(0.,0.,-25.5*mm), //for 30mm collimator length
  //modify// add fCollimatorHalfLength parameter
  fColl_Plate_Physical = new G4PVPlacement(0,  G4ThreeVector(0.,0.,-(fCollimatorHalfLength + 10.5*mm)), 
	       	                        Coll_Plate_Logical,"Collimator01",CollVolLogical, false, 0);
  
  //------------------------------ Teflon base ------------
  G4Box*  Coll_plate_2 = new G4Box ("Coll_plate", 24.*mm , 70.*mm, 2.*mm);
  G4SubtractionSolid* Coll3_2 = new G4SubtractionSolid("Coll3",Coll_plate_2,Coll2, 0,G4ThreeVector());
  G4LogicalVolume* Coll_Plate_Logical_2 = new G4LogicalVolume(Coll3_2, teflon , "CollimLogical");
////////  fColl_Plate_Physical_2 = new G4PVPlacement(0,  G4ThreeVector(0.,0.,-39.*mm), // for 50 mm collimator length
////////  fColl_Plate_Physical_2 = new G4PVPlacement(0,  G4ThreeVector(0.,0.,-29.*mm), //for 30mm collimator length
  //modify// add fCollimatorHalfLength parameter
  fColl_Plate_Physical_2 = new G4PVPlacement(0,  G4ThreeVector(0.,0.,-(fCollimatorHalfLength + 14.*mm)), 
		                      Coll_Plate_Logical_2,"Collimator02",CollVolLogical, false, 0);
  
  //-----------------------------
  
  G4LogicalVolume* CollLogical = new G4LogicalVolume(Coll1, tungsten , "CollimLogical");
   fCollPhysical = new G4PVPlacement(0,  G4ThreeVector(0.,0.,-9.*mm), 
		                                CollLogical,"Collimator0",CollVolLogical, false, 0);
  
  G4Tubs* Coll5 = new G4Tubs("Coll5",4.*mm,20.*mm, 5. *mm,0.,twopi);
  G4Tubs* Coll6 = new G4Tubs("Coll6",0.   ,20.*mm, 7.5*mm,0.,twopi);
  G4LogicalVolume* Coll5Logical = new G4LogicalVolume(Coll5, tungsten ,"CollVolLogical");
  G4LogicalVolume* Coll6Logical = new G4LogicalVolume(Coll6, tungsten ,"CollVolLogical");
  
////////   Coll5Physical = new G4PVPlacement(0,  G4ThreeVector(0.,0.,21.*mm), // for 50 mm collimator length
////////   Coll5Physical = new G4PVPlacement(0,  G4ThreeVector(0.,0.,11.*mm),  // for 30 mm collimator length
   //modify//  add fCollimatorHalfLength parameter
   Coll5Physical = new G4PVPlacement(0,  G4ThreeVector(0.,0.,(fCollimatorHalfLength - 4.*mm)),  
		                               Coll5Logical,"Collimator1",CollVolLogical, false, 0);
				     
///////   Coll6Physical = new G4PVPlacement(0,  G4ThreeVector(0.,0.,33.5*mm), // for 50 mm collimator length
///////   Coll6Physical = new G4PVPlacement(0,  G4ThreeVector(0.,0.,23.5*mm),  // for 30 mm collimator length
   //modify//  add fCollimatorHalfLength parameter
   Coll6Physical = new G4PVPlacement(0,  G4ThreeVector(0.,0.,(fCollimatorHalfLength + 8.5*mm)),
		                               Coll6Logical,"Collimator2",CollVolLogical, false, 0);
  
  //G4Tubs* Source = new G4Tubs("Source", 0, 1.*mm, 1.*mm, 0.,twopi);
  //G4Tubs* Source = new G4Tubs("Source", 0, 1.5*mm, 1.5*mm, 0.,twopi); 
  G4Tubs* Source = new G4Tubs("Source", 0, 1.5*mm, 0.25*mm, 0.,twopi); // realistic source !!
  G4LogicalVolume* SourceLogical = new G4LogicalVolume(Source, vacuum ,"SourceLogical");
  SourceLogical-> SetVisAttributes(SoVisAtt);

   //SourcePhysical = new G4PVPlacement(0, G4ThreeVector(0,0,18.*mm), 
///////   SourcePhysical = new G4PVPlacement(0, G4ThreeVector(0,0,16.5*mm), // source became 0.5mm (nesli) 16.25 - 16.75 -> at the end of the cylinder  
///////    SourcePhysical = new G4PVPlacement(0, G4ThreeVector(0,0,6.5*mm), // for 30mm collimator length
   //modify// add fCollimatorHalfLength parameter
   SourcePhysical = new G4PVPlacement(0, G4ThreeVector(0,0,(fCollimatorHalfLength - 8.5*mm)), 
			                          SourceLogical,"Source", CollVolLogical, false, 0);
				      
  G4RotationMatrix* rot2 = new G4RotationMatrix();
  rot2->rotateY(90.0*deg);
  rot2->rotateX(fCollimator_System_Rotation);
 
	  	if (fCollimatorPosition == 1){ // above the detector
  
/////////  fCollVolPhysical = new G4PVPlacement(0, G4ThreeVector(0.,fCollimatorSystem_R ,Actual_Z + 14.*cm), // for 50mm collimator length
/////////  fCollVolPhysical = new G4PVPlacement(0, G4ThreeVector(0.,fCollimatorSystem_R ,Actual_Z + 13.*cm), // for 30mm collimator length
  //modify// add fCollimatorHalfLength parameter
  fCollVolPhysical = new G4PVPlacement(0, G4ThreeVector(0.,fCollimatorSystem_R ,Actual_Z + fCollimatorHalfLength + 115.*mm), 
		                             CollVolLogical,"Collimator_00",fMotherVolume,false, 0);
  					
	 } else if (fCollimatorPosition == 2){ // at the side of the detector
  
  fCollVolPhysical = new G4PVPlacement(rot2, 
  			      G4ThreeVector(-fCollimatorSystem_R * cos(fCollimator_System_Rotation),
				             fCollimatorSystem_R * sin(fCollimator_System_Rotation), // should be modified as well, parametrise with fCollimatorHalfLength or change it when you want to use position 2
				             Actual_Z + fCollimator_LowerHeight), 
		                             CollVolLogical,"Collimator_00",fMotherVolume,false, 0);
       					     }

  //G4cout << "Collimator position   =  " <<   Actual_Z + 3.5*cm	<< G4endl;	
  
  
  
 //=================================================================================================
 // Non-segmented crystal with the dimensions of Siegfried
 //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
  G4Tubs* CrystalTubs = new G4Tubs("Crystal",fCrystal_InnerRadius,fCrystal_OuterRadius,
  									fCrystal_Height/2.,0,twopi);
  
  G4LogicalVolume *fCrystalLogical = new G4LogicalVolume(CrystalTubs,natGe,"CrystalLogical");

  G4VisAttributes* CrystalVisAtt = new G4VisAttributes(darkred);
  
  CrystalVisAtt -> SetVisibility(true);
  CrystalVisAtt -> SetForceSolid(true);
  fCrystalLogical -> SetVisAttributes(CrystalDeadLayerVisAtt);
  
  fCrystalPhysical = new G4PVPlacement(0,G4ThreeVector(0.,0.,Actual_Z + fCrystal_Height/2.),
                                       fCrystalLogical,"sensitive_Siegfried",fMotherVolume,false,0);
     			
/*

 //=================================================================================================
 // Siegfried segmented
 //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
 G4ThreeVector fComponentsPosition;
 fComponentsPosition.setX(0.0*cm);
 fComponentsPosition.setY(0.0*cm);
 fComponentsPosition.setZ(0.0*cm);
 
  Actual_Z += (fCrystal_Height+fCrystal_SiliconThickness + 1.*mm)/2.; // 1mm for safety 
  
  fCrystalDeadLayerLogical -> SetVisAttributes(CrystalDeadLayerVisAtt); 
 
  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(Actual_Z);
  fCrystalDeadLayerPhysical = new G4PVPlacement(0,fComponentsPosition,
                                                fCrystalDeadLayerLogical,
                                                "CrystalDeadLayer",
                                                fMotherVolume,
                                                false,
                                                0);


  // silicon 

  fCrystalSiliconLogical -> SetVisAttributes(SiVisAtt); 

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ(Actual_Z + (fCrystal_Height - fCrystal_SiliconThickness) * 0.5);
  fCrystalSiliconTopPhysical = new G4PVPlacement(0,fComponentsPosition,
                                                 fCrystalSiliconLogical,
                                                 "CrystalSiliconTop",
                                                 fMotherVolume,
                                                 false,
                                                 0);

  fComponentsPosition.setX(0.0*cm);
  fComponentsPosition.setY(0.0*cm);
  fComponentsPosition.setZ( Actual_Z - (fCrystal_Height - fCrystal_SiliconThickness) * 0.5);
  fCrystalSiliconBottomPhysical = new G4PVPlacement(0,fComponentsPosition,
                                                 fCrystalSiliconLogical,
                                                 "CrystalSiliconBottom",
                                                 fMotherVolume,
                                                 false,
                                                 0);


  // segments 
	  
  fCrystalSegmentLogical -> SetVisAttributes(CrystalSegmentVisAtt); 
  
  G4RotationMatrix fSegmentRotation;
  int icopynumber=0;

  char volumename[30];
  for (int iz=0; iz<iSegment_Nz; iz++) {

    fComponentsPosition.setX(0.0*cm);
    fComponentsPosition.setY(0.0*cm);
    fComponentsPosition.setZ(Actual_Z + float(iz-1)*fSegment_Height);

    for (int iphi=0; iphi<iSegment_Nphi; iphi++) { 

      fSegmentRotation.rotateZ(float(iphi) * fSegment_dPhi);
      fSegmentRotation.rotateX(0.0 * deg);
      fSegmentRotation.rotateY(0.0 * deg);

      sprintf(volumename, "segment_z%d_phi%d", iz, iphi);
      fCrystalSegmentPhysical[icopynumber] = new G4PVPlacement(
                                       G4Transform3D(fSegmentRotation, fComponentsPosition),
                                       fCrystalSegmentLogical,
                                       volumename,
                                       fMotherVolume,
                                       false,
                                       icopynumber);
      fSegmentRotation.rotateZ(0.0 - float(iphi) * fSegment_dPhi);

      icopynumber++;
    }
  }

*/


}
