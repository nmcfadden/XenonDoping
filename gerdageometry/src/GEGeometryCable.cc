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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 02-06-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 10-09-2007, Added aluminum bondpads on detector, Daniel
 * 10-25-2007, Fixed not unique cable naming, Daniel
 * 11-27-2007, Added Copperlines on the cable, Daniel
 * 09-07-2008, - Added phase-1 cables, Jens
 *             - the old phase-2 cables (Cable_geometry2_xxx) are now without the HV-Strip
 *               there are now the following phase-2 cables:
 *                 o HV cable connection (one per String -> see GEGEometryElectronics.cc/.hh)
 *                 o Signal cable connection (one per String -> see GEGEometryElectronics.cc/.hh)
 *                 o HV Vertical part
 *                 o HV Horizontal part
 *                 o signal cable (the one surrounding the crystal)
 * 07-29-2008 (jens)
 *            - changed positions and geometry of phase-2 cables, i.e. signal cable, signal connection cable, hv cable
 *            - removed hv connection cable (hv-vertical cable has been extended)
 * 04-10-2016 - Added physical volumes of signal and HV cables of phase 2 (status december 2015).
 *		Cables consist of pads on holders and cable connections to the electronics plate, Janina
 * 17-04-2016 - Changed counter of phase2 cables for continuous volumelist, Janina
 *
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"

//#include "gerdageometry/GEGeometrySD.hh"
#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometryCable.hh"       
#include "gerdageometry/GEGeometryStringStuff.hh"       
#include "gerdageometry/GEGeometryDetectorDB.hh"
#include "gerdageometry/GEGeometryHolderPhase2Parts.hh"
// --------------------------------------------------------------------------- 

using namespace CLHEP;

GEGeometryCable::GEGeometryCable(G4int col, G4int row, GEGeometryDetectorDB* detectordb)
{
  
  fCableColumn                  = col; 
  fCableRow                     = row; 

  fCrystalType                  = detectordb -> GetCrystalType(fCableColumn, fCableRow); 
  fCrystalTypeL                  = detectordb -> GetCrystalType(fCableColumn, fCableRow,1);
  fDetectorParameters           = detectordb -> GetDetectorParameters(fCrystalType); 
  fDetectorParametersL           = detectordb -> GetDetectorParameters(fCrystalTypeL); 
  fCrystalGeometry              = fDetectorParameters -> GetGeometry(); 
  fCrystalPairGap		= detectordb -> GetCrystalPairGap();
  fCableGeometry1Number         = detectordb -> GetCrystalGeometry1Number(); 
  fCableGeometry2Number         = detectordb -> GetCrystalGeometry2Number(); 
  fCableCrystal5SlotNumber	= detectordb -> GetCrystalGeometry5SlotNumber();
  fCableCrystal5Number          = detectordb -> GetCrystalGeometry5Number();
  fCableCrystal11Number		= detectordb -> GetCrystalGeometry11Number();
  fCableThickness               = detectordb -> GetCableThickness();
  fCrystalNumber                = detectordb -> GetCrystalNumber(); 
  fIsPair			= detectordb -> IsAPair(fCableColumn, fCableRow); 
  fCrystalHeight                = fDetectorParameters -> GetHeight();
  if (fIsPair) fCrystalHeightPairLowerOne    = fDetectorParametersL -> GetHeight();
  fCrystalOuterRadius           = fDetectorParameters -> GetOuterRadius(); 
  fCrystalDLTopThickness        = fDetectorParameters -> GetDLTopThickness();
  fCrystalDLBottomThickness     = fDetectorParameters -> GetDLBottomThickness();
  fCrystalPassivationThickness  = fDetectorParameters -> GetPassivationThickness();
  fHolderInnerHeight            = detectordb -> GetHolderInnerHeight(); 
  fCableLoopHeight              = detectordb -> GetCableLoopHeight(); 
  fCableStringWidth             = detectordb -> GetCableStringWidth(); 
  fHolderSideBarHeight          = detectordb -> GetHolderSideBarHeight(); 
  fElectronicsBoardHeight       = detectordb -> GetElectronicsBoardHeight(); 
  fCrystalDeadLayerTopThickness = fDetectorParameters -> GetDLTopThickness(); 
  fSegmentHeight                = fDetectorParameters -> GetSegmentHeight(); 
  fZSegmentsPerCrystal          = fDetectorParameters -> GetNZSegments(); 
  fPhiSegmentsPerCrystal        = fDetectorParameters -> GetNPhiSegments(); 
  fSegmentStartingAngle         = fDetectorParameters -> GetSegmentationAngle(); 
  fSegmentStartingAngleDataBase = detectordb -> GetSegmentationOffsetAngle();
  fRelativeSegmentZPosition     = detectordb -> GetRelativeSegmentZPosition(fDetectorParameters, 0); 
  fRelativeSegmentZMiddlePosition = detectordb -> GetRelativeSegmentZPosition(fDetectorParameters, 1); 
  fRelativeSegmentZBottomPosition = detectordb -> GetRelativeSegmentZPosition(fDetectorParameters, 2); 
  fCablePosition                = detectordb -> GetCrystalPosition(fCableColumn, 
								   fCableRow); 
  fDetectorMotherVolume         = detectordb -> GetDetectorMotherVolume(); 

  
  fPhase2Cable_thickness	= detectordb -> GetPhase2Cable_thickness();
  fPhase2Cable_LengthOnHolderHV	= detectordb ->	GetPhase2Cable_LengthOnHolderHV();			
  fPhase2Cable_LengthOnHolderSignal	= detectordb ->	GetPhase2Cable_LengthOnHolderSignal();	
  fHolderPlateRot_angle		= detectordb->GetAngleForPhase2CablePosition(fCableColumn);
  
  G4int ngeometry1 = 0; 
  G4int ngeometry2 = 0; 

  for (G4int i = 0; i < detectordb -> GetNDetectorTypes(); i++){
    if (detectordb -> GetDetectorParameters(i) -> GetGeometry() == 1){ngeometry1++;}
    if (detectordb -> GetDetectorParameters(i) -> GetGeometry() == 4){ngeometry1++;}
    if (detectordb -> GetDetectorParameters(i) -> GetGeometry() == 2){ngeometry2++;}
  }

  if (fCrystalGeometry == 2)
    {
      fCableLogical                 = detectordb -> GetCableLogicalContainer(fCrystalType); 
      fCableCopperLinesLogical      = detectordb -> GetCableCopperLinesLogicalContainer(fCrystalType);
      fBondPadAlLogical             = detectordb -> GetBondPadAlLogicalContainer(fCrystalType);


      fHVCablePhase2Vertical_ZPos           = detectordb -> GetHVCablePhase2Vertical_ZPos(fCableRow);
      fHVCableVerticalLogical               = detectordb -> GetHVCablePhase2VerticalLogical(fCableRow); 

      fHVCablePhase2Horizontal_relZPos      = detectordb -> GetHVCablePhase2Horizontal_relZPos();
      fHVCablePhase2Horizontal_RadiusOffset = detectordb -> GetHVCablePhase2Horizontal_RadiusOffset();
      fHVCableHorizontalLogical             = detectordb -> GetHVCablePhase2HorizontalLogical(); 

      fHVCable_AngleRotZ                    = detectordb -> GetHVCablePhase2_AnglePosition(fCableRow);
    }

  if (fCrystalGeometry == 1 || fCrystalGeometry == 4)
    {
        fCrystalPosition_Uppermost_Crystal  = detectordb -> GetCrystalPosition(fCableColumn,0); 
        fStringStuff                        = detectordb->GetStringStuff(fCableColumn);
        fElectronicsSideBarZPosition        = fStringStuff -> GetElectronicsSideBarZPosition(); 
        fElectronicsSideBarXOffset          = fStringStuff -> GetElectronicsSideBarXOffset(); 
        fPhase1CableLower                   = fStringStuff -> Get_Phase1_CableLowerPart_Logical(fCableRow);
        fPhase1CableUpper                   = fStringStuff -> Get_Phase1_CableUpperPart_Logical(fCableRow);
        fPhase1Cable_thickness              = detectordb -> GetPhase1Cable_thickness();
        fElectronicsPhaseIHorizontalBar_YFrameThickness  = detectordb -> GetElectronicsPhaseIHorizontalBar_YFrameThickness();
    }
    
   if (fCrystalGeometry == 5 || fCrystalGeometry == 11) 
    { 
     	fPhaseHVCableAtHolderLogical = detectordb -> GetHVCableAtHolderPhase2Logical(fCableColumn, fCableRow);
     	fPhaseHVCableToEPlateLogical = detectordb -> GetHVCableToEPlatePhase2Logical(fCableColumn, fCableRow);
   	if (fIsPair) fPhaseHVCableAtHolderTopLogical = detectordb -> GetHVCableAtHolderTopPhase2Logical(fCableColumn, fCableRow);
   	if (fIsPair) fPhaseHVCableTopToEPlateLogical = detectordb -> GetHVCableTopToEPlatePhase2Logical(fCableColumn, fCableRow);
    
     	fPhaseSignalCableAtHolderLogical = detectordb -> GetSignalCableAtHolderPhase2Logical(fCableColumn, fCableRow);
     	fPhaseSignalCableToEPlateLogical = detectordb -> GetSignalCableToEPlatePhase2Logical(fCableColumn, fCableRow);
    	if (fIsPair) fPhaseSignalCableAtHolderTopLogical = detectordb -> GetSignalCableAtHolderTopPhase2Logical(fCableColumn, fCableRow);
    	if (fIsPair) fPhaseSignalCableTopToEPlateLogical = detectordb -> GetSignalCableTopToEPlatePhase2Logical(fCableColumn, fCableRow);
    
    	fHVPhase2ToEPlate_LengthZ    = detectordb -> GetHVPhase2ToEPlate_LengthZ(fCableColumn, fCableRow);
    	if (fIsPair) fHVTopPhase2ToEPlate_LengthZ  = detectordb -> GetHVTopPhase2ToEPlate_LengthZ(fCableColumn, fCableRow);
    
    	holderPhase2			= detectordb -> GetHolderPhase2Parts(fCrystalType);
    	fZDistanceToEndOfSiliconPlate	= holderPhase2 -> GetZDistanceToEndOfSiliconPlate();
    	fZDistanceToEndOfSiliconPlatePair = holderPhase2 -> GetZDistanceToEndOfSiliconPlatePair();
    	fHolderPhase2lenghtY = holderPhase2 -> GetHolderPhase2Width();
    }   
}

// --------------------------------------------------------------------------- 

GEGeometryCable::~GEGeometryCable()
{

    //delete fCablePhysical; 
    //delete fCableCopperLinesPhysical;
    //fBondPadAlContainer.clear(); 

} 

// --------------------------------------------------------------------------- 

G4double GEGeometryCable::GetSegmentAngle(G4int seg) 
{

  G4double angle; 
  G4double segmentangle = 360.0 * deg / fPhiSegmentsPerCrystal;  

  angle =  fSegmentStartingAngle 
    + fSegmentStartingAngleDataBase
    + 0.5 * segmentangle 
    + float(seg) * segmentangle; 

  return angle; 

} 
// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryCable::GetSegmentPosition(G4int segrow)
{

  G4ThreeVector position  = fCablePosition; 
  G4double      z;

  if (fZSegmentsPerCrystal == 1)
    {
      z = position.getZ() + 0.0 * mm;
    }
  else 
    {
      if(segrow == 0) {z = fCablePosition.getZ() + fRelativeSegmentZPosition;}
      else if(segrow == 1){ z = fCablePosition.getZ() + fRelativeSegmentZMiddlePosition;}
      else {z = fCablePosition.getZ() + fRelativeSegmentZBottomPosition;}
    }

  position.setZ(z); 

  return position; 

}

// --------------------------------------------------------------------------- 
G4ThreeVector GEGeometryCable::Get_CablePosition_Phase1_Lower()
{
    return Get_CablePosition_Phase1_UL(false);
}


// --------------------------------------------------------------------------- 
G4ThreeVector GEGeometryCable::Get_CablePosition_Phase1_Upper()
{
    return Get_CablePosition_Phase1_UL(true);
}


// --------------------------------------------------------------------------- 
G4ThreeVector GEGeometryCable::Get_CablePosition_Phase1_UL(G4bool isUpper)
{

  G4double delta(0.5);  
  if(isUpper) delta=0.0;

  G4ThreeVector position = fCrystalPosition_Uppermost_Crystal; 

  //G4double alpha = ( G4double(fCableRow)+delta+3./*for offset*/ ) * 1.5*mm / fElectronicsSideBarXOffset;
  G4double deltaY        = 1.2* fPhase1Cable_thickness;
  G4double deltaY_offset = 1.1* (0.5*fElectronicsPhaseIHorizontalBar_YFrameThickness+0.5*fPhase1Cable_thickness);

  G4double alpha = ( deltaY_offset + deltaY* (G4double(fCableRow)+delta) ) / fElectronicsSideBarXOffset;
  fRotationAngle_Phase1Cable = alpha;

  G4double x = position.getX() + std::cos(alpha) * fElectronicsSideBarXOffset; 
  G4double y = position.getY() + std::sin(alpha) * fElectronicsSideBarXOffset; 
  G4double z = fElectronicsSideBarZPosition;

  position.setX(x); 
  position.setY(y); 
  position.setZ(z); 

  return position; 
  
}

// --------------------------------------------------------------------------- 

void GEGeometryCable::ConstructCable()
{

  char namecable[100];

  MGLog(debugging) << "Start to create Cables for geometry-type " << fCrystalGeometry << "." <<endlog;

  if (fCrystalGeometry == 1 || fCrystalGeometry == 4)
    {
        G4ThreeVector position = Get_CablePosition_Phase1_Lower();
        G4RotationMatrix* rotMatLower = new G4RotationMatrix();
        rotMatLower->rotateZ( -fRotationAngle_Phase1Cable );
        sprintf(namecable, "CableLower_geometry1_%d", fCableGeometry1Number); 
        new G4PVPlacement(rotMatLower,
			  position, 
			  fPhase1CableLower, 
			  namecable,
			  fDetectorMotherVolume, 
			  false, 
			  0);
        position = Get_CablePosition_Phase1_Upper();
        G4RotationMatrix* rotMatUpper = new G4RotationMatrix();
        rotMatUpper->rotateZ( -fRotationAngle_Phase1Cable );
        sprintf(namecable, "CableUpper_geometry1_%d", fCableGeometry1Number); 
        new G4PVPlacement(rotMatUpper,
			  position, 
			  fPhase1CableUpper, 
			  namecable,
			  fDetectorMotherVolume, 
			  false, 
			  0);
    }    
      


  if (fCrystalGeometry == 2)
    {   
     

      // HV Cable - Horizontal 
      // --------------------

      G4ThreeVector translation;
      G4RotationMatrix* tmpRotMat1 = new G4RotationMatrix();
      tmpRotMat1->rotateZ(-fHVCable_AngleRotZ);
      G4double tmpX =  fHVCablePhase2Horizontal_RadiusOffset*cos(-fHVCable_AngleRotZ);
      G4double tmpY = -fHVCablePhase2Horizontal_RadiusOffset*sin(-fHVCable_AngleRotZ);
      G4double tmpZ = fHVCablePhase2Horizontal_relZPos;
      translation  = fCablePosition + G4ThreeVector(tmpX,tmpY,tmpZ);

      sprintf(namecable, "HVCableHorizontal_geometry2_%d", 
	      fCableGeometry2Number); 
      new G4PVPlacement(tmpRotMat1,
			translation,
			fHVCableHorizontalLogical, 
			namecable,
			fDetectorMotherVolume, 
			false, 
			0 ); //fCrystalNumber);
      
      
      // HV Cable - Vertical 
      // --------------------
      translation  = fCablePosition;
      translation.setZ( fHVCablePhase2Vertical_ZPos );
      sprintf(namecable, "HVCableVertical_geometry2_%d", fCableGeometry2Number); 
      new G4PVPlacement(tmpRotMat1,
			translation,
			fHVCableVerticalLogical, 
			namecable,
			fDetectorMotherVolume, 
			false, 
			0 ); //fCrystalNumber);
      



      G4RotationMatrix* tmpRotMatCbl = new G4RotationMatrix();
      tmpRotMatCbl->rotateZ(-fSegmentStartingAngleDataBase);

      fCablePosition.setZ(fCablePosition.getZ() + fRelativeSegmentZPosition); 
      translation  = fCablePosition; 

      sprintf(namecable, "Cable_geometry2_%d", fCableGeometry2Number); 
      

      fCablePhysical = new G4PVPlacement(tmpRotMatCbl,
					 translation, 
					 fCableLogical, 
					 namecable,
					 fDetectorMotherVolume, 
					 false, 
					 fCrystalNumber);

      sprintf(namecable, "CableCopperLines_geometry2_%d", fCableGeometry2Number); 

      fCableCopperLinesPhysical = new G4PVPlacement(tmpRotMatCbl,
                                                    translation, 
                                                    fCableCopperLinesLogical, 
                                                    namecable,
                                                    fDetectorMotherVolume, 
                                                    false, 
                                                    fCrystalNumber);


      fCablePosition.setZ(fCablePosition.getZ() - fRelativeSegmentZPosition); 

      for (G4int i = 0; i < fZSegmentsPerCrystal; i++)
	{

	  for (G4int j = 0; j < fPhiSegmentsPerCrystal; j++)
	    {
	      G4ThreeVector    segmentposition ; 
	      G4double         angle; 
	      G4RotationMatrix rm; 
	      
	      angle = GetSegmentAngle(j); 
	      
	      rm.rotateZ(angle); 

	      segmentposition = GetSegmentPosition(i); 

	      
	      //	      segmentposition.setZ(translation.getZ()); 	    

	      
	      G4int fCable3Number =  (fCableGeometry2Number * fZSegmentsPerCrystal * fPhiSegmentsPerCrystal 
				      + i * fPhiSegmentsPerCrystal 
				      + j); 


	      // bond pads 


	      G4VPhysicalVolume* bondpadalphysical; 

	      char namebondpad[100]; 

	      sprintf(namebondpad, "BondPadAl_%d", fCable3Number); 

	      G4ThreeVector bondpadposition = segmentposition; 

	      bondpadalphysical = new G4PVPlacement(G4Transform3D(rm, bondpadposition), 
						    fBondPadAlLogical, 
						    namebondpad,
						    fDetectorMotherVolume, 
						    false, 
						    fCable3Number);   

	      fBondPadAlContainer.push_back(bondpadalphysical); 

	    }
	}
      
    }
    
   if (fCrystalGeometry == 5 ||fCrystalGeometry == 11 ){ 
      //counter
      //fCablePhase2Total= fCableCrystal5SlotNumber + fCableCrystal11Number;
      fCablePhase2TotalAll= fCableCrystal5Number + fCableCrystal11Number;//to include cables at top detectors in pairs into the volumelist
      
      G4double tmp=0.0;//variable for the level of the cable under the lower holder
      G4double tmp1=0.0;//variable for the level of the cable above the upper holder
     
      //position in z direction
      if (fIsPair) {tmp=-(fCrystalHeightPairLowerOne+fZDistanceToEndOfSiliconPlate+ 0.5*fCrystalPairGap+0.5*fPhase2Cable_thickness);
                   tmp1=fZDistanceToEndOfSiliconPlatePair+ 0.5*fCrystalPairGap+0.5*fPhase2Cable_thickness;
                   }
      else tmp=-(0.5*fCrystalHeight+fZDistanceToEndOfSiliconPlate+0.5*fPhase2Cable_thickness);
     
      //rotation to match with rotation of holders
      G4RotationMatrix* HolderPlateRot = new G4RotationMatrix();
      HolderPlateRot->rotateZ(fHolderPlateRot_angle);
      G4RotationMatrix* SignalPlateRot = new G4RotationMatrix();
      SignalPlateRot->rotateZ(180*deg+fHolderPlateRot_angle);
   
  
    //+---------------------------------------------------------------------+
    //| HV cable: pad on holder and cable from holder to electronics plate  |
    //+---------------------------------------------------------------------+ 
     //pad on holder
      char physvolnameHVholder[100]; 
      G4ThreeVector positionHVholder;
    
      G4ThreeVector HVonHolderTranslation;
      HVonHolderTranslation.setZ(tmp);
      HVonHolderTranslation.setX((0.5*fHolderPhase2lenghtY-0.5*fPhase2Cable_LengthOnHolderHV)*sin(fHolderPlateRot_angle));
      HVonHolderTranslation.setY((0.5*fHolderPhase2lenghtY-0.5*fPhase2Cable_LengthOnHolderHV)*cos(fHolderPlateRot_angle));

      positionHVholder= fCablePosition + HVonHolderTranslation; //fCablePosition = position of crystal
    
      sprintf(physvolnameHVholder,"HVCableAtHolder_Phase2_%d",fCablePhase2TotalAll-1);
   
      new G4PVPlacement( HolderPlateRot,
    		         positionHVholder,
		         fPhaseHVCableAtHolderLogical,
   		         physvolnameHVholder, 
		         fDetectorMotherVolume,
 		         false, 
                         fCablePhase2Total);
	   MGLog(debugging) << "test3" << endlog;	       
      //for top detector in pair
      if (fIsPair){ 
      	G4ThreeVector positionHVholderTop;
    
      	char physvolnameHVholdertop[100]; 
      	sprintf(physvolnameHVholdertop,"HVCableAtHolder_Phase2_%d",fCablePhase2TotalAll-2);
    
      	G4ThreeVector HVonHolderTopTranslation;
      	HVonHolderTopTranslation.setZ(tmp1);
      	HVonHolderTopTranslation.setX((0.5*fHolderPhase2lenghtY-0.5*fPhase2Cable_LengthOnHolderHV)*sin(fHolderPlateRot_angle));
      	HVonHolderTopTranslation.setY((0.5*fHolderPhase2lenghtY-0.5*fPhase2Cable_LengthOnHolderHV)*cos(fHolderPlateRot_angle));
      	positionHVholderTop= fCablePosition + HVonHolderTopTranslation;
        
      	new G4PVPlacement( HolderPlateRot,
    		           positionHVholderTop,
		           fPhaseHVCableAtHolderTopLogical,
   		           physvolnameHVholdertop, 
		           fDetectorMotherVolume,
 		           false, 
                           fCablePhase2Total);
     	}
		       
      //cable from holder to electronics plate 
      char physvolnameHVtoPlate[100];
      G4ThreeVector positionHVtoPlate;
    
      sprintf(physvolnameHVtoPlate,"HVCableFromHolderToElectronicsPlate_Phase2_%d",fCablePhase2TotalAll-1);
    
      G4ThreeVector HVtoPlateTranslation;
      HVtoPlateTranslation.setZ(tmp+0.5*fHVPhase2ToEPlate_LengthZ);
      HVtoPlateTranslation.setX((0.5*fHolderPhase2lenghtY+(1.5+2*fCableRow)*fPhase2Cable_thickness)*sin(fHolderPlateRot_angle));
      HVtoPlateTranslation.setY((0.5*fHolderPhase2lenghtY+(1.5+2*fCableRow)*fPhase2Cable_thickness)*cos(fHolderPlateRot_angle));
      //with each row the cables are moved further outside to avoid overlays, with enough space for cable of a possible top detector in pair 

      positionHVtoPlate=fCablePosition + HVtoPlateTranslation;
    
      new G4PVPlacement( HolderPlateRot,
    		         positionHVtoPlate,
		         fPhaseHVCableToEPlateLogical,
   		         physvolnameHVtoPlate, 
		         fDetectorMotherVolume,
 		         false, 
                         fCablePhase2Total);
		       
      //for top detector in pair	       
      if (fIsPair){ 	       
      	char physvolnameHVToptoPlate[100];
      	G4ThreeVector positionHVToptoPlate;	       
		       
      	sprintf(physvolnameHVToptoPlate,"HVCableFromHolderToElectronicsPlate_Phase2_%d",fCablePhase2TotalAll-2);
    
      	G4ThreeVector HVToptoPlateTranslation;
      	HVToptoPlateTranslation.setZ(tmp1+0.5*fHVTopPhase2ToEPlate_LengthZ);
      	HVToptoPlateTranslation.setX((0.5*fHolderPhase2lenghtY+(0.5+2*fCableRow)*fPhase2Cable_thickness)*sin(fHolderPlateRot_angle));
      	HVToptoPlateTranslation.setY((0.5*fHolderPhase2lenghtY+(0.5+2*fCableRow)*fPhase2Cable_thickness)*cos(fHolderPlateRot_angle));
      	//with each row the cables are moved further outside to avoid overlays

      	positionHVToptoPlate=fCablePosition + HVToptoPlateTranslation;
    
      	new G4PVPlacement( HolderPlateRot,
    		           positionHVToptoPlate,
		           fPhaseHVCableTopToEPlateLogical,
   		           physvolnameHVToptoPlate, 
		           fDetectorMotherVolume,
 		           false, 
                           fCablePhase2Total);
      	}	       
		       
     //+------------------------------------------------------------------------+
     //| Signal cable: pad on holder and cable from holder to electronics plate |
     //+------------------------------------------------------------------------+ 
      //pad on holder
      char physvolnameSignalholder2[100]; 
      G4ThreeVector positionSignalholder;
    
      G4ThreeVector SignalonHolderTranslation;
      SignalonHolderTranslation.setZ(tmp);
      SignalonHolderTranslation.setX((0.5*fHolderPhase2lenghtY-0.5*fPhase2Cable_LengthOnHolderSignal)*sin(180*deg+fHolderPlateRot_angle));
      SignalonHolderTranslation.setY((0.5*fHolderPhase2lenghtY-0.5*fPhase2Cable_LengthOnHolderSignal)*cos(180*deg+fHolderPlateRot_angle));

      positionSignalholder= fCablePosition + SignalonHolderTranslation; //fCablePosition = position of crystal
    
      sprintf(physvolnameSignalholder2,"SignalCableAtHolder_Phase2_%d",fCablePhase2TotalAll-1);
    
      new G4PVPlacement( SignalPlateRot,
    		         positionSignalholder,
		         fPhaseSignalCableAtHolderLogical,
   		         physvolnameSignalholder2, 
		         fDetectorMotherVolume,
 		         false, 
                         fCablePhase2Total);
		       
      //for top detector in pair
      if (fIsPair){ 
      	G4ThreeVector positionSignalholderTop;
    
      	char physvolnameSignalholdertop2[100]; 
      	sprintf(physvolnameSignalholdertop2,"SignalCableAtHolder_Phase2_%d",fCablePhase2TotalAll-2);
    
      	G4ThreeVector SignalonHolderTopTranslation;
    	SignalonHolderTopTranslation.setZ(tmp1);
      	SignalonHolderTopTranslation.setX((0.5*fHolderPhase2lenghtY-0.5*fPhase2Cable_LengthOnHolderSignal)*sin(180*deg+fHolderPlateRot_angle));
    	SignalonHolderTopTranslation.setY((0.5*fHolderPhase2lenghtY-0.5*fPhase2Cable_LengthOnHolderSignal)*cos(180*deg+fHolderPlateRot_angle));
     	positionSignalholderTop= fCablePosition + SignalonHolderTopTranslation;
        
     	new G4PVPlacement( SignalPlateRot,
    		           positionSignalholderTop,
		           fPhaseSignalCableAtHolderTopLogical,
   		           physvolnameSignalholdertop2, 
		           fDetectorMotherVolume,
 		           false, 
                           fCablePhase2Total);
      	}
		       
      //cable from holder to electronics plate 
      char physvolnameSignaltoPlate3[100];
      G4ThreeVector positionSignaltoPlate;
    
      sprintf(physvolnameSignaltoPlate3,"SignalCableFromHolderToElectronicsPlate_Phase2_%d",fCablePhase2TotalAll-1);
    
      G4ThreeVector SignaltoPlateTranslation;
      SignaltoPlateTranslation.setZ(tmp+0.5*fHVPhase2ToEPlate_LengthZ);
      SignaltoPlateTranslation.setX((0.5*fHolderPhase2lenghtY+(1.5+2*fCableRow)*fPhase2Cable_thickness)*sin(180*deg+fHolderPlateRot_angle));
      SignaltoPlateTranslation.setY((0.5*fHolderPhase2lenghtY+(1.5+2*fCableRow)*fPhase2Cable_thickness)*cos(180*deg+fHolderPlateRot_angle));
      //with each row the cables are moved further outside to avoid overlays, with enough space for cable of a possible top detector in pair 

      positionSignaltoPlate=fCablePosition + SignaltoPlateTranslation;
    
      new G4PVPlacement( SignalPlateRot,
    		         positionSignaltoPlate,
		         fPhaseSignalCableToEPlateLogical,
   		         physvolnameSignaltoPlate3, 
		         fDetectorMotherVolume,
 		         false, 
                         fCablePhase2Total);
	    
      //for top detector in pair	       
      if (fIsPair){ 	       
      	char physvolnameSignalToptoPlate3[100];
      	G4ThreeVector positionSignalToptoPlate;	       
		       
      	sprintf(physvolnameSignalToptoPlate3,"SignalCableFromHolderToElectronicsPlate_Phase2_%d",fCablePhase2TotalAll-2);
     
      	G4ThreeVector SignalToptoPlateTranslation;
      	SignalToptoPlateTranslation.setZ(tmp1+0.5*fHVTopPhase2ToEPlate_LengthZ);
      	SignalToptoPlateTranslation.setX((0.5*fHolderPhase2lenghtY+(0.5+2*fCableRow)*fPhase2Cable_thickness)*sin(180*deg+fHolderPlateRot_angle));
      	SignalToptoPlateTranslation.setY((0.5*fHolderPhase2lenghtY+(0.5+2*fCableRow)*fPhase2Cable_thickness)*cos(180*deg+fHolderPlateRot_angle));
      	//with each row the cables are moved further outside to avoid overlays

      	positionSignalToptoPlate=fCablePosition + SignalToptoPlateTranslation;
    
      	new G4PVPlacement( SignalPlateRot,
    		           positionSignalToptoPlate,
		           fPhaseSignalCableTopToEPlateLogical,
   		           physvolnameSignalToptoPlate3, 
		           fDetectorMotherVolume,
 		           false, 
                           fCablePhase2Total);
      	}
		       
      //MGLog(debugging) << fCablePhase2Total << endlog;
     }

} 

// --------------------------------------------------------------------------- 


