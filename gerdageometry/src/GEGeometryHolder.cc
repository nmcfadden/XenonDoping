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
 * FIRST SUBMISSION: 01-19-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 07-05-2005, Added protection against NULL logical volumes, Luciano
 * 10-09-2007, Added cable guides for HV and Signal cables, Daniel
 * 02-04-2008, phase-1 crystals got individual holders and z positions, Jens S.
 * 07-29-2008 (jens)
 *            - changed positions and geometry of phase-2 cables, i.e. signal cable, signal connection cable, hv cable
 *              --> position of teflon holders had to be changed
 * 20-06-2011  Added possibility to not construct PTFERing in Phase1 holders -> 
 *             to be used with the TeflonEncapsulation in Phase0 to avoid overlaps
 * 23-04-2016 - Added holders from top detectors to electronics plate (stars and bars)
 *	      - new counter for phase2 holders to take into account that BEGe Pairs have two holder plates, all others only one, Janina
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

#include "gerdageometry/GEGeometryHolder.hh"       
#include "gerdageometry/GEGeometryHolderPhase1Parts.hh"
#include "gerdageometry/GEGeometryHolderPhase2Parts.hh"
#include "io/MGLogger.hh"
// --------------------------------------------------------------------------- 

using namespace CLHEP;

GEGeometryHolder::GEGeometryHolder(G4int col, G4int row, GEGeometryDetectorDB* detectordb) :
  fDetectorDB(detectordb)
{
  
  fHolderColumn                = col; 
  fHolderRow                   = row; 

  fCrystalType                 = detectordb -> GetCrystalType(fHolderColumn, fHolderRow); 
  fDetectorParameters          = detectordb -> GetDetectorParameters(fCrystalType); 

  fCrystalGeometry             = fDetectorParameters -> GetGeometry(); 
  fHolderCrystalRadius         = fDetectorParameters -> GetOuterRadius();
 
  fHolderGeometry1Number       = detectordb -> GetCrystalGeometry1Number();

  fHolderGeometry2Number       = detectordb -> GetCrystalGeometry2Number(); 
  fHolderGeometry3Number       = detectordb -> GetCrystalGeometry3Number(); 
  fHolderGeometry4Number       = detectordb -> GetCrystalGeometry4Number(); // BEGe counter for Geometry1 holders
  fHolderGeometry5NumberAll    = detectordb -> GetCrystalGeometry5Number(); 
  fHolderGeometry5Number       = detectordb -> GetCrystalGeometry5SlotNumber(); 
  fHolderGeometry11Number       = detectordb -> GetCrystalGeometry11Number(); // non-true coaxial counter for Phase2 holders
  
  fHolderInnerHeight           = detectordb -> GetHolderInnerHeight(); 
  fHolderUpperBarHeight        = detectordb -> GetHolderUpperBarHeight(); 
  fHolderTeflonTubeHeight      = detectordb -> GetHolderTeflonTubeHeight(); 
  fHolderHVCableOuterHeight    = detectordb -> GetHolderHVCableOuterHeight(); 
  fHolderSignalCableInnerHeight= detectordb -> GetHolderSignalCableInnerHeight();

  fDetectorMotherVolume        = detectordb -> GetDetectorMotherVolume(); 
  fHolderCopperLogical         = detectordb -> GetHolderCopperLogical(); 
  fHolderTeflonLogical         = detectordb -> GetHolderTeflonLogical(); 
  fHolderSignalCableLogical    = detectordb -> GetHolderSignalCableLogical(); 
  fHolderHVCableLogical        = detectordb -> GetHolderHVCableLogical(); 

  fHolderPhase1Parts           = detectordb -> GetHolderPhase1Parts(fCrystalType);
  fHolderPhase2Parts           = detectordb -> GetHolderPhase2Parts(fCrystalType);

  fHolderPosition              = detectordb -> GetCrystalPosition(fHolderColumn, 
								  fHolderRow); 

  fFlagTeflonEncapsulation     = detectordb -> GetFlagConstructTeflonEncapsulation();
  fTeflonEncapsulationRow      = detectordb -> GetTeflonEncapsulationRow();
  fTeflonEncapsulationColumn   = detectordb -> GetTeflonEncapsulationColumn();
  
  fPhase2HolderStarLogical     = detectordb -> GetPhase2HolderStarLogical();
  fPhase2HolderGreatStarLogical     = detectordb -> GetPhase2HolderGreatStarLogical();
  fPhase2HolderBarsToEPlateLogical = detectordb -> GetPhase2HolderBarsToEPlateLogical();
  
  fPhase2HolderStarPos         = detectordb ->GetPhase2HolderStarPlacement();  
  fPhase2HolderBartoEPlateLength  = detectordb ->GetPhase2HolderBartoEPlateLength();
  fPhase2HolderStarHeight= detectordb ->GetPhase2HolderStarHeight();
  fPhase2HolderStarAngle = detectordb->GetAngleForPhase2CablePosition(fHolderColumn);
}

// --------------------------------------------------------------------------- 

GEGeometryHolder::~GEGeometryHolder()
{

    // Never delete Physical Volumes, this is done by Geant4
    // -----------------------------------------------------
    //delete fHolderCopperPhysical; 
    //delete fHolderTeflonPhysical; 
    //delete fHolderSignalCablePhysical; 
    //delete fHolderHVCablePhysical; 
} 

// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryHolder::GetHolderCopperPosition()
{
  
  G4ThreeVector position = fHolderPosition; 

  G4double z = position.getZ() + 0.5 * (fHolderInnerHeight + fHolderUpperBarHeight); 

  position.setZ(z); 

  return position; 
  
}

// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryHolder::GetHolderTeflonPosition()
{
  
  G4ThreeVector position = fHolderPosition; 

  G4double z = position.getZ() + 0.5 * (fHolderInnerHeight - fHolderTeflonTubeHeight); 

  position.setZ(z); 

  return position; 
  
}

// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryHolder::GetHolderSignalCablePosition()
{
  
  G4ThreeVector position = fHolderPosition; 

  G4double z = position.getZ() + 0.5 * (fHolderInnerHeight + 2 * fHolderSignalCableInnerHeight + 2 * 1.25 * mm ); 
  G4double x = position.getX();
  position.setZ(z); 
  position.setX(x); 
  return position; 
  
}

// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryHolder::GetHolderHVCablePosition()
{
  
  G4ThreeVector position = fHolderPosition; 

  G4double z = position.getZ() -  0.5 * (fHolderInnerHeight - fHolderHVCableOuterHeight + 5 * mm);  
  G4double x = position.getX() - 2.50 * mm; 
  position.setZ(z); 
  position.setX(x); 

  return position; 
  
}


// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryHolder::GetPhase2HolderStarPosition()
{
  
  G4ThreeVector position = fHolderPosition; 

  G4double z = fPhase2HolderStarPos;  
  G4double x = position.getX() + 24.2*mm*cos(fPhase2HolderStarAngle);
  G4double y = position.getY() + 24.2*mm*sin(fPhase2HolderStarAngle);
  position.setZ(z);  
  position.setX(x); 
  position.setY(y); 
 
  return position; 
  
}
// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryHolder::GetPhase2HolderGreatStarPosition()
{
  
  G4ThreeVector position = fHolderPosition; 

  G4double z = fPhase2HolderStarPos;  
  G4double x = position.getX() + 26.7*mm*cos(fPhase2HolderStarAngle);
  G4double y = position.getY() + 26.7*mm*sin(fPhase2HolderStarAngle);
  position.setZ(z);  
  position.setX(x); 
  position.setY(y); 
 
  return position; 
  
}
// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryHolder::GetPhase2HolderBartoEPlatePosition()
{
  
  G4ThreeVector position = fHolderPosition; 

  G4double z = fPhase2HolderStarPos+0.5*fPhase2HolderStarHeight+0.5*fPhase2HolderBartoEPlateLength;  
  position.setZ(z);  
 
  return position; 
  
}

// --------------------------------------------------------------------------- 

void GEGeometryHolder::ConstructHolder()
{
  fHolderGeometryP2Number = fHolderGeometry5Number + fHolderGeometry11Number + fHolderGeometry3Number/2;
  MGLog(debugging) << "fHolderGeometryP2Number = "  << fHolderGeometryP2Number << " fHolderGeometry5Number " << fHolderGeometry5Number << " fHolderGeometry11Number " << fHolderGeometry11Number 
  << " fHolderGeometry3Number/2 " << fHolderGeometry3Number/2 << endlog;
  fHolderGeometryP2NumberAll = fHolderGeometry5NumberAll + fHolderGeometry11Number + fHolderGeometry3Number/2;

  if (fCrystalGeometry == 1)
    {
      if(fFlagTeflonEncapsulation==false)
        fHolderPhase1Parts->CreatePhysicalVolume(fDetectorMotherVolume,fHolderColumn,
						 fHolderRow,fHolderGeometry1Number,fHolderPosition, 
						 false);
      else
	{
	  if((fHolderColumn == fTeflonEncapsulationColumn) && (fHolderRow == fTeflonEncapsulationRow))
	    fHolderPhase1Parts->CreatePhysicalVolume_NoUpperPTFERing(fDetectorMotherVolume,fHolderColumn,
								     fHolderRow,fHolderGeometry1Number,
								     fHolderPosition);
	  else
	    fHolderPhase1Parts->CreatePhysicalVolume(fDetectorMotherVolume,fHolderColumn,fHolderRow,
						     fHolderGeometry1Number,fHolderPosition, false);
	}
    }
  if (fCrystalGeometry == 4) // call same physical volume function in holder class as for type xtals 
    //but with BEGeFlag = true to avoid placing some volumes
    {
      if (fDetectorDB->IsAPair(fHolderColumn,fHolderRow))	
	fHolderPhase1Parts->ResetParametersForAPair(fDetectorDB,fHolderColumn,fHolderRow);
	
      if(fFlagTeflonEncapsulation==false)
        fHolderPhase1Parts->CreatePhysicalVolume(fDetectorMotherVolume,fHolderColumn,
						 fHolderRow,fHolderGeometry4Number+1000,
						 fHolderPosition, true);
      else
	{
	  if((fHolderColumn == fTeflonEncapsulationColumn) && (fHolderRow == fTeflonEncapsulationRow))
	    fHolderPhase1Parts->CreatePhysicalVolume_NoUpperPTFERing(fDetectorMotherVolume,
								     fHolderColumn,fHolderRow,
								     fHolderGeometry4Number+1000,fHolderPosition);
	  else
	    fHolderPhase1Parts->CreatePhysicalVolume(fDetectorMotherVolume,fHolderColumn,
						     fHolderRow,fHolderGeometry4Number+1000,
						     fHolderPosition, true);
	}
    }
  
  else if (fCrystalGeometry == 2) 
  {
      char volumename[100];
      
      G4ThreeVector position; 
      G4int         partnumber; 
      
      G4RotationMatrix rm; 

      rm.rotateZ(0.0 * deg); 
      
      // volume: copper part 
      
      partnumber = fHolderGeometry2Number; 
      
      sprintf(volumename,"Holder_copper_%d", partnumber);
      
      position = this -> GetHolderCopperPosition(); 
      
      if (fHolderCopperLogical)
	fHolderCopperPhysical = new G4PVPlacement(G4Transform3D(rm, position),
						  fHolderCopperLogical, 
						  volumename, 
						  fDetectorMotherVolume, 
						  false, 
						  partnumber); 
      
      // volume: teflon part 
      
      partnumber = fHolderGeometry2Number; 
      
      sprintf(volumename,"Holder_teflon_%d", partnumber);
      
      position = this -> GetHolderTeflonPosition(); 
      
      if (fHolderTeflonLogical)
	fHolderTeflonPhysical = new G4PVPlacement(G4Transform3D(rm, position),
						  fHolderTeflonLogical, 
						  volumename, 
						  fDetectorMotherVolume, 
						  false, 
						  partnumber); 


      // volume: teflon cable guides 
      
      partnumber = fHolderGeometry2Number; 
       
      sprintf(volumename,"Holder_Signal_Cableguide_%d", partnumber);
      
      position = this -> GetHolderSignalCablePosition(); 
      rm.rotateZ( 0.0* 180.0 * deg); 
      if (fHolderSignalCableLogical)
	fHolderSignalCablePhysical = new G4PVPlacement(G4Transform3D(rm, position),
						       fHolderSignalCableLogical, 
						       volumename, 
						       fDetectorMotherVolume, 
						       false, 
						       partnumber); 


      partnumber = fHolderGeometry2Number; 
      
      sprintf(volumename,"Holder_HV_Cableguide_%d", partnumber);
      
      position = this -> GetHolderHVCablePosition(); 
      rm.rotateZ(0.0 * 180.0 * deg); 
      if (fHolderHVCableLogical)
	fHolderHVCablePhysical = new G4PVPlacement(G4Transform3D(rm, position),
						   fHolderHVCableLogical, 
						   volumename, 
						   fDetectorMotherVolume, 
						   false, 
						   partnumber); 

  }  
   
  else if (fCrystalGeometry == 3) 
  {
    fHolderPhase2Parts->ResetParametersForType3Pair(fDetectorDB,fHolderColumn,fHolderRow);
    
    fHolderPhase2Parts->CreatePhysicalVolume(fDetectorDB, fDetectorMotherVolume,fHolderColumn,
                                             fHolderRow,fHolderGeometryP2NumberAll,fHolderGeometryP2Number,
                                             fHolderPosition);
  }
  
  else if (fCrystalGeometry == 5)
  {
    if (fDetectorDB->IsAPair(fHolderColumn,fHolderRow))
      fHolderPhase2Parts->ResetParametersForAPair(fDetectorDB,fHolderColumn,fHolderRow);
    else  fHolderPhase2Parts->ResetParametersForSingle(fDetectorDB,fHolderColumn,fHolderRow);
    fHolderPhase2Parts->CreatePhysicalVolume(fDetectorDB, fDetectorMotherVolume,fHolderColumn,
                                             fHolderRow,fHolderGeometryP2NumberAll,fHolderGeometryP2Number,
                                             fHolderPosition);
  }
  
  else if (fCrystalGeometry == 11)
  {
    fHolderPhase2Parts->ResetParametersForCoax(fDetectorDB,fHolderColumn,fHolderRow); 	 
    fHolderPhase2Parts->CreatePhysicalVolume(fDetectorDB, fDetectorMotherVolume,fHolderColumn,
                                             fHolderRow,fHolderGeometryP2NumberAll,fHolderGeometryP2Number,
                                             fHolderPosition);
  }

  if (fCrystalGeometry == 3 ||fCrystalGeometry == 5||fCrystalGeometry == 11 )
  {
   char volumename[100];
   char volumename1[100];
   char volumename2[100];
   G4RotationMatrix Phase2HolderStarRot; 
   G4ThreeVector positionStar;
   G4ThreeVector positionGreatStar;
   G4ThreeVector positionBar; 
   
   if (fHolderRow==0){
      Phase2HolderStarRot.rotateZ(fPhase2HolderStarAngle); 
      sprintf(volumename,"Phase2HolderStar_%d", fHolderColumn);
      sprintf(volumename1,"Phase2HolderGreatStar_%d", fHolderColumn);
      sprintf(volumename2,"Phase2HolderBarsToEPlate_%d", fHolderColumn);
      positionStar = this -> GetPhase2HolderStarPosition(); 
      positionGreatStar = this -> GetPhase2HolderGreatStarPosition(); 
      positionBar = this -> GetPhase2HolderBartoEPlatePosition();
      
      //radius condition identifies GTFs 
      if (fHolderCrystalRadius < 41.*mm)  { if (fPhase2HolderStarLogical){
				  	      new G4PVPlacement(G4Transform3D(Phase2HolderStarRot, positionStar),
						  		fPhase2HolderStarLogical, 
						  		volumename, 
						  		fDetectorMotherVolume, 
						  		false, 
						  		fHolderColumn); }
	 MGLog(debugging) << "Physical Volume Phase2HolderStar_" << fHolderColumn<< " has been created. " << endlog;} 
      else { if (fPhase2HolderGreatStarLogical){
				  	      new G4PVPlacement(G4Transform3D(Phase2HolderStarRot, positionGreatStar),
						  		fPhase2HolderGreatStarLogical, 
						  		volumename1, 
						  		fDetectorMotherVolume, 
						  		false, 
						  		fHolderColumn); }
	 MGLog(debugging) << "Physical Volume Phase2HolderGreatStar_" << fHolderColumn<< " has been created. " << endlog;} 
     
     
     
     
      if (fPhase2HolderBarsToEPlateLogical){
				  	      new G4PVPlacement(0, positionBar,
						  fPhase2HolderBarsToEPlateLogical, 
						  volumename2, 
						  fDetectorMotherVolume, 
						  false, 
						  fHolderColumn); 
         
	MGLog(debugging) << "Phase2HolderBarsToEPlate_" << fHolderColumn<< " has been created. " << endlog;}  
  }  
  }
} 

// --------------------------------------------------------------------------- 

