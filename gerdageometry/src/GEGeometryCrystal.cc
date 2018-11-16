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
 * 01-23-2005, added crystal segments, Kevin 
 * 04-14-2005, changed the crystal deadlayer name
 *             so that it can be used by macro command volumelist, Xiang
 * 04-15-2005, corrected the bug without getdetectormothervolume, xiang
 * 07-13-2005, added bond pads, Kevin 
 * 03-03-2006, Si top and bottom physical volumes now have the same name, Xiang
 * 11-13-2007, Removed the segments from the crystal, Daniel 
               Removed naming inconsistency, Daniel 
 * 30-04-2008 - changes due to a new implementation of Phase-1 crystals, 
 *              the hold version did cause some problems (an overlap between the crystal and its CopperContactPlate),
 *            - (Jens S.)
 * 05-05-2008 - Turn around phase-1 crystal to be consistent with the component names "Upper" and "Lower"
 *              (Jens)
 * 06-13-2008 - Changed Crystal_geometry1 such that Crystal is daughter of liquid argon. Daniel
 * 11-03-2011 - Added a new detector type to define the back-to-back BEGe's. (N. Barros)
 * 				Changes summarized in ConstructCrystal().
 * 				Added an optional parameter to the constructor to trigger a 180 degrees rotation of the crystal parts to build the crystal upside down.
 * 30-05-2012 - Added the possibility to reverse non true coaxial crystals to match the real confituration in GERDA (Giovanni)
 * 29-09-2012 - Added debug messages to indicate the positioning of each crystal built (Nuno).
 * 23-01-2012 - Added BEGe (Type 4) detectors (B. Lehnert)
 * 04-10-2016 - Adapted dead layer positioning for type 5 and 11.
 *	      - For 5 and 11 set rotation flag such that with macro command only Coax are turned, Janina
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
#include "io/MGLogger.hh"

#include "gerdageometry/GEGeometryCrystal.hh"       

using namespace CLHEP;

// --------------------------------------------------------------------------- 

GEGeometryCrystal::GEGeometryCrystal(GEGeometryDetectorDB* detectordb,G4int col, G4int row,
				     G4int multiplet) //:
  //fDetectorDB(detectordb)
{
  
  fCrystalColumn           = col; 
  fCrystalRow              = row; 
  fCrystalMultiplet        = multiplet;
  fCrystalGeometry3Rotate  = false;

  fCrystalType             = detectordb -> GetCrystalType(fCrystalColumn, fCrystalRow, multiplet); 
  fIsInAPair               = detectordb ->IsAPair(fCrystalColumn, fCrystalRow);

  fDetectorParameters      = detectordb -> GetDetectorParameters(fCrystalType); 

  
  fCrystalGeometry         = fDetectorParameters -> GetGeometry(); 
  fCrystalHeight           = fDetectorParameters -> GetHeight();
  fCrystalDeadLayerTop     = fDetectorParameters -> GetDLTopThickness(); 
  fCrystalDeadLayerBottom  = fDetectorParameters -> GetDLBottomThickness(); 
  fCrystalPassivation      = fDetectorParameters -> GetPassivationThickness(); 
  
  fPhiSegmentsPerCrystal   = 1;
  fZSegmentsPerCrystal     = 1; 

  fCrystalGeometry3PairGap = detectordb -> GetCrystalPairGap();

  fHolderInnerHeight       = detectordb -> GetHolderInnerHeight(); 

  fCrystalPosition         = detectordb -> GetCrystalPosition(fCrystalColumn, 
  							      fCrystalRow); 


  fCrystalNumber           = detectordb -> GetCrystalNumber();

  fDetectorMotherVolume    = detectordb -> GetDetectorMotherVolume();
  fCrystalPassivationLogical = detectordb -> GetCrystalPassivationLogical(fCrystalType); 
  fCrystalDeadLayerLogical = detectordb -> GetCrystalDeadLayerLogical(fCrystalType); 
  fCrystalLogical = detectordb -> GetCrystalSegmentLogical(fCrystalType); 

  fCrystalGeometry1Number   = detectordb -> GetCrystalGeometry1Number(); 
  fCrystalGeometry2Number   = detectordb -> GetCrystalGeometry2Number(); 
  fCrystalGeometry3Number   = detectordb -> GetCrystalGeometry3Number();
  fCrystalGeometry4Number   = detectordb -> GetCrystalGeometry4Number();
  fCrystalGeometry5Number   = detectordb -> GetCrystalGeometry5Number();
  fCrystalGeometry11Number   = detectordb -> GetCrystalGeometry11Number();

}

// --------------------------------------------------------------------------- 

GEGeometryCrystal::~GEGeometryCrystal()
{

    // Never delete Physical Volumes, this is done by Geant4
    // -----------------------------------------------------
    //  delete fCrystalDeadLayerPhysical; 

  fCrystalSegmentContainer.clear(); 

} 

// --------------------------------------------------------------------------- 

void GEGeometryCrystal::ConstructCrystal()
{



  // non-true coaxial geometry 

  if (fCrystalGeometry == 1)
    {
            
	  MGLog(debugging) << " Constructing a crystal of geometry type " << fCrystalGeometry << " with number "
			   << fCrystalGeometry1Number  << " ( total so far : " << fCrystalNumber << " )"<< endlog;
	  MGLog(debugging) << 
	    " Detector type number: " << fCrystalType << endlog;

      // passivation 

      char name[50]; 

      sprintf(name,"CrystalPassivation_geometry1_%d", fCrystalGeometry1Number);

      G4double z = fCrystalPosition.getZ() + 0.5 * (  fCrystalHeight - fCrystalPassivation);
      ////////////
      G4RotationMatrix *pRot = NULL;
      G4double zCrystal = z;

      if ( fCrystalGeometry3Rotate ) {
	z +=  (  fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
	zCrystal -= (  fCrystalHeight );
	pRot = new G4RotationMatrix();
	pRot -> rotateX(180*deg);
      }
      ////////////
      G4ThreeVector myVec;
      myVec = fCrystalPosition;
      myVec.setZ(z); 
     myVec = fCrystalPosition;
      myVec.setZ(zCrystal); 


      fCrystalPassivationPhysical = new G4PVPlacement(pRot,
						      myVec,
						      fCrystalPassivationLogical,
						      name,
						      fDetectorMotherVolume,
						      false,
						      fCrystalNumber);

      // dead layer

      sprintf(name,"CrystalDeadLayer_geometry1_%d", fCrystalGeometry1Number);
      
      fCrystalDeadLayerPhysical = new G4PVPlacement(pRot,
						    fCrystalPosition,
						    fCrystalDeadLayerLogical,
						    name,
						    fDetectorMotherVolume,
						    false,
						    fCrystalNumber);



      sprintf(name,"Crystal_geometry1_%d", fCrystalGeometry1Number);

      // The commented part is the default behaviour. The uncommented is to have visualisation with
      // phase-I crystals.
      fCrystalPhysical = new G4PVPlacement(pRot,
					   fCrystalPosition,
					   fCrystalLogical,
					   name,
					   fDetectorMotherVolume,
					   false,
					   fCrystalNumber);

      MGLog(debugging) << " Placing crystal " << fCrystalNumber << " in " << 
	fCrystalPosition.getX() << " , "  << fCrystalPosition.getY() << " , " << 
	fCrystalPosition.getZ() << endlog;

    }
  
  // true coaxial geometry 

  else if (fCrystalGeometry == 2)
    {
      

      // passivation 

      char name[50];
      
      sprintf(name,"CrystalPassivation_geometry2_%d", fCrystalGeometry2Number);
      
      G4double z = fCrystalPosition.getZ(); //  - 0.5 * (fCrystalDeadLayerBottom 
      //						    - fCrystalDeadLayerTop); 
      fCrystalPosition.setZ(z); 

      fCrystalPassivationPhysical = new G4PVPlacement(0,
						      fCrystalPosition,
						      fCrystalPassivationLogical,
						      name,
						      fDetectorMotherVolume,
						      false,
						      fCrystalNumber);
      // dead layer

      sprintf(name,"CrystalDeadLayer_geometry2_%d", fCrystalGeometry2Number);
      
      fCrystalDeadLayerPhysical = new G4PVPlacement(0,
						    fCrystalPosition,
						    fCrystalDeadLayerLogical,
						    name,
						    fDetectorMotherVolume,
						    false,
						    fCrystalNumber);

      sprintf(name,"Crystal_geometry2_%d", fCrystalGeometry2Number);

      fCrystalPhysical = new G4PVPlacement(0,
					   fCrystalPosition,
					   fCrystalLogical,
					   name,
					   fDetectorMotherVolume,
					   false,
					   fCrystalNumber);

      MGLog(debugging) << " Placing crystal " << fCrystalNumber << " in " << fCrystalPosition.getX() << " , "  
		       << fCrystalPosition.getY() << " , " << fCrystalPosition.getZ() << endlog;

      
    }
  // phase-II BEGe geometry back-to-back
  // Use the same as as true coaxial geometry but segmented into two sub-detectors
  // Therefore, each call of this method implements not one but two detectors (BEGe's, most likely)
  // TODO: Finish the implementation. Later re-check to be sure the holders are placed properly implemented and placed
  else if (fCrystalGeometry == 3)
  {
	  // First place the first detector on the upper part.
	  // The distance between the segments is hardcoded for now.

	  MGLog(debugging) << " Constructing a crystal of geometry type " << fCrystalGeometry << " with number "
			  	  << fCrystalGeometry3Number  << " ( total so far : " << fCrystalNumber << " )"<< endlog;
	  MGLog(debugging) << " Detector type number: " << fCrystalType << endlog;


	  char name[50];



      // The CrystalPosition corresponds to the center of the pair.
      // Therefore the position is actually outside both segments, right in the middle,as shown below.
      //
      // +---+
      // |   |
      // +---+
      //   *
      // +---+
      // |   |
      // +---+

      // -- Upper detector. Simple positioning.

	  // passivation -- For the moment, BEGe's are assumed as having no passivation

      sprintf(name,"CrystalPassivation_geometry3_%d", fCrystalGeometry3Number);

      // if we want it rotated it means it is the lower crystal.
      G4double z = fCrystalPosition.getZ();
      G4RotationMatrix *pRot = NULL;
      G4double zCrystal = z;

      if ( fCrystalGeometry3Rotate ) {
    	  z +=  (  fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
    	  zCrystal += (  0.5*fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
      } else {
    	  pRot = new G4RotationMatrix();
          pRot -> rotateX(180*deg);
    	  z -=  (  fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
    	  zCrystal -= (  0.5*fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
      }
      G4ThreeVector myVec;
      myVec = fCrystalPosition;
      myVec.setZ(z);
      MGLog(debugging) << " Placing crystal " << fCrystalNumber << " in " << myVec.getX() << " , "  << 
	myVec.getY() << " , " << myVec.getZ() << endlog;
      /** For now BEGe's do not have passivation layer in MC.
      fCrystalPassivationPhysical = new G4PVPlacement(pRot,
						      myVec,
						      fCrystalPassivationLogical,
						      name,
						      fDetectorMotherVolume,
						      false,
						      fCrystalNumber);
       */
      // dead layer
      myVec = fCrystalPosition;
      myVec.setZ(zCrystal);
      sprintf(name,"CrystalDeadLayer_geometry3_%d", fCrystalGeometry3Number);

      fCrystalDeadLayerPhysical = new G4PVPlacement(pRot,
						    myVec,
						    fCrystalDeadLayerLogical,
						    name,
						    fDetectorMotherVolume,
						    false,
						    fCrystalNumber);


      sprintf(name,"Crystal_geometry3_%d", fCrystalGeometry3Number);

      fCrystalPhysical = new G4PVPlacement(pRot,
					   myVec,
					   fCrystalLogical,
					   name,
					   fDetectorMotherVolume,
					   false,
					   fCrystalNumber);

  }

  // individual Phase I BEGe
  // TODO implement individual placement settigs. so far only copied from type 1 xttal placement
  if (fCrystalGeometry == 4 )
    {

      MGLog(debugging) << " Constructing a crystal of geometry type " << fCrystalGeometry << " with number "
		       << fCrystalGeometry4Number  << " ( total so far : " << fCrystalNumber << " )"<< endlog;
      MGLog(debugging) << " Detector type number: " << fCrystalType << endlog;
      
      // passivation
      char name[100];
      sprintf(name,"CrystalPassivation_geometry4_%d", fCrystalGeometry4Number);

      G4double z = fCrystalPosition.getZ();// + 0.5 * (  fCrystalHeight - fCrystalPassivation);
      ////////////
      G4RotationMatrix *pRot = NULL;
      G4double zCrystal = z;

      if ( fCrystalGeometry3Rotate && !fIsInAPair) 
	{
	  z +=  (  fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
	  pRot = new G4RotationMatrix();
	  pRot -> rotateX(180*deg);
	}

      if (fIsInAPair)
	{
	  if (fCrystalMultiplet==0) //upper crystal
	    {
	      z +=  (  fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
	      zCrystal += ( 0.5*fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
	      fCrystalGeometry3Rotate = true;
	    }
	  else if (fCrystalMultiplet==1) //lower crystal
	    {
	      z -=  (  fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
	      zCrystal -= ( 0.5*fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
	    }



	  if ( fCrystalGeometry3Rotate)
	    {
	      pRot = new G4RotationMatrix();
	      pRot -> rotateX(180*deg);
	    }
	}

      ////////////
      G4ThreeVector myVec;
      myVec = fCrystalPosition;
      myVec.setZ(z);


      // dead layer
      myVec = fCrystalPosition;
      myVec.setZ(zCrystal);
      sprintf(name,"CrystalDeadLayer_geometry4_%d", fCrystalGeometry4Number);

      fCrystalDeadLayerPhysical = new G4PVPlacement(pRot,
    		  	  	  	  	myVec,
						    fCrystalDeadLayerLogical,
						    name,
						    fDetectorMotherVolume,
						    false,
						    fCrystalNumber);



      sprintf(name,"Crystal_geometry4_%d", fCrystalGeometry4Number);

      // AV
      fCrystalPhysical = new G4PVPlacement(0,
					   G4ThreeVector(0,0,0),
					   fCrystalLogical,
					   name,
					   fCrystalDeadLayerLogical,
					   false,
					   fCrystalNumber);

      MGLog(debugging) << " Placing crystal " << name << " with number " << fCrystalNumber << " multiplet "<<
    		  fCrystalMultiplet << " in " << fCrystalPosition.getX() <<
    		  " , "  << fCrystalPosition.getY() << " , " << myVec.getZ() <<
    		  endlog;

    }
    
  // individual Phase II BEGe
  // TODO implement individual placement settigs. so far only copied from type 1 xttal placement
  if (fCrystalGeometry == 5 )
  {
    MGLog(debugging) << " Constructing a crystal of geometry type " << fCrystalGeometry << " with number "
    << fCrystalGeometry5Number  << " ( total so far : " << fCrystalNumber << " )"<< endlog;
    MGLog(debugging) << " Detector type number: " << fCrystalType << endlog;
    
    // passivation
    char name[100];
    sprintf(name,"CrystalPassivation_geometry5_%d", fCrystalGeometry5Number);
    
    G4double z = fCrystalPosition.getZ();// + 0.5 * (  fCrystalHeight - fCrystalPassivation);
    ////////////
    G4RotationMatrix *pRot = NULL;
    G4double zCrystal = z;
    if (!fIsInAPair) fCrystalGeometry3Rotate = false;
    
    if ( fCrystalGeometry3Rotate && !fIsInAPair) 
    {
      z +=  (  fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
      pRot = new G4RotationMatrix();
      pRot -> rotateX(180*deg);
    }
    
    if (fIsInAPair)
    {
      if (fCrystalMultiplet==0) //upper crystal
      {
        z +=  (  fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
        zCrystal += ( 0.5*fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
        fCrystalGeometry3Rotate = true;
      }
      else if (fCrystalMultiplet==1) //lower crystal
      {
        z -=  (  fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
        zCrystal -= ( 0.5*fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
	fCrystalGeometry3Rotate = false;
      }
      
      
      
      if ( fCrystalGeometry3Rotate)
      {
        pRot = new G4RotationMatrix();
        pRot -> rotateX(180*deg);
      }
    }
    
    ////////////
    G4ThreeVector myVec;
    myVec = fCrystalPosition;
    myVec.setZ(z);
    
    
    // dead layer
    myVec = fCrystalPosition;
    myVec.setZ(zCrystal);
    sprintf(name,"CrystalDeadLayer_geometry5_%d", fCrystalGeometry5Number);
    
    fCrystalDeadLayerPhysical = new G4PVPlacement(pRot,
                                                  myVec,
                                                  fCrystalDeadLayerLogical,
                                                  name,
                                                  fDetectorMotherVolume,
                                                  false,
                                                  fCrystalNumber);
    
    
    
    sprintf(name,"Crystal_geometry5_%d", fCrystalGeometry5Number);
    
    // AV
    fCrystalPhysical = new G4PVPlacement(0,
                                         G4ThreeVector(0,0,0),
                                         fCrystalLogical,
                                         name,
                                         fCrystalDeadLayerLogical,
                                         false,
                                         fCrystalNumber);
    
    MGLog(debugging) << " Placing crystal " << name << " with number " << fCrystalNumber << " multiplet "<<
    fCrystalMultiplet << " in " << fCrystalPosition.getX() <<
    " , "  << fCrystalPosition.getY() << " , " << myVec.getZ() <<
    endlog;
    
  }
  
  // non-true coaxial geometry for PhaseII
  else if (fCrystalGeometry == 11)
  {
    MGLog(debugging) << " Constructing a crystal of geometry type " << fCrystalGeometry << " with number "
    << fCrystalGeometry11Number  << " ( total so far : " << fCrystalNumber << " )"<< endlog;
    MGLog(debugging) << 
    " Detector type number: " << fCrystalType << endlog;
    
    // passivation
    
    char name[50];
    
    sprintf(name,"CrystalPassivation_geometry11_%d", fCrystalGeometry11Number);
    
    G4double z = fCrystalPosition.getZ() + 0.5 * (  fCrystalHeight - fCrystalPassivation);
    ////////////
    G4RotationMatrix *pRot = NULL;
    G4double zCrystal = z;
    
    if ( fCrystalGeometry3Rotate ) {
      z +=  (   fCrystalHeight  + 0.5*fCrystalGeometry3PairGap);
      zCrystal -= (  fCrystalHeight );
      pRot = new G4RotationMatrix();
      pRot -> rotateX(180*deg);
    }
    ////////////
    G4ThreeVector myVec;
    myVec = fCrystalPosition;
    myVec.setZ(z);
    
    myVec = fCrystalPosition;
    myVec.setZ(zCrystal);
    fCrystalPassivationPhysical = new G4PVPlacement(pRot,
                                                    myVec,
                                                    fCrystalPassivationLogical,
                                                    name,
                                                    fDetectorMotherVolume,
                                                    false,
                                                    fCrystalNumber);
    
    // dead layer
    
    sprintf(name,"CrystalDeadLayer_geometry11_%d", fCrystalGeometry11Number);
    
    fCrystalDeadLayerPhysical = new G4PVPlacement(pRot,
                                                  fCrystalPosition,
                                                  fCrystalDeadLayerLogical,
                                                  name,
                                                  fDetectorMotherVolume,
                                                  false,
                                                  fCrystalNumber);
    
    
    
    sprintf(name,"Crystal_geometry11_%d", fCrystalGeometry11Number);
    
    // The commented part is the default behaviour. The uncommented is to have visualisation with
    // phase-I crystals.
    fCrystalPhysical = new G4PVPlacement(pRot,
                                         fCrystalPosition,
                                         fCrystalLogical,
                                         name,
                                         fDetectorMotherVolume,
                                         false,
                                         fCrystalNumber);
    
    MGLog(debugging) << " Placing crystal " << fCrystalNumber << " in " << 
    fCrystalPosition.getX() << " , "  << fCrystalPosition.getY() << " , " << 
    fCrystalPosition.getZ() << endlog;
    
  }


} 

// --------------------------------------------------------------------------- 


