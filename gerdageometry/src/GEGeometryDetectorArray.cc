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
 * 11/14/2005 : Kevin, changed structure of crystal placement 
 * 08-05-2008 : Jens, added lines for putting Electronics of Phase1- and Phase2-Strings correctly
 *              (online counters for Column of Geometry1 and Geometry2 were needed)
 * 30-05-2012 - Added the possibility to reverse non true coaxial crystals to match the real confituration in GERDA (Giovanni)
 *
 */

// ---------------------------------------------------------------------------

#include "globals.hh"

#include "gerdageometry/GEGeometryDetectorArray.hh"
#include "geometry/MGGeometryBEGeDetector.hh"
#include "io/MGLogger.hh"

// --------------------------------------------------------------------------- 


GEGeometryDetectorArray::GEGeometryDetectorArray(GEGeometryDetectorDB* detectordb)
{
  fFlagConstructCrystals = detectordb -> GetFlagConstructCrystals(); 
  fFlagConstructSupport  = detectordb -> GetFlagConstructSupport(); 
  fFlagConstructCabling  = detectordb -> GetFlagConstructCabling(); 
  fFlagReverseNonTrueCoaxial = detectordb->GetFlagReverseNonTrueCoaxial();

  // loop over detector matrix 

  for (G4int i = 0; i < detectordb->GetNumberOfStrings(); i++)
    for (G4int j = 0; j < detectordb->GetNumberOfRows(); j++)
      {
	// get crystal type 
	fCrystalType[i][j] = detectordb -> GetCrystalType(i, j); 
	if (fCrystalType[i][j] >= 0) 
	  {
	    GEGeometryDetectorParameters* detectorparameters = 
	      detectordb -> GetDetectorParameters(fCrystalType[i][j]); 

	    G4int crystalgeometry = detectorparameters -> GetGeometry(); 

	    if (crystalgeometry == 1)
	      {
		detectordb -> IncreaseCrystalGeometry1Number(); 
		detectordb -> IncreaseCrystalNumber(); 	
	      }
	    
	    else if (crystalgeometry == 2)
	      {
		detectordb -> IncreaseCrystalGeometry2Number(); 
		detectordb -> IncreaseCrystalNumber(); 
	      }
	    else if (crystalgeometry == 3)
	      {
	    	// Type 3 are back-to-back detectors. Each entry in the database corresponds to
	    	// two crystals being built in the array for this type.
		detectordb -> IncreaseCrystalGeometry3Number();
		detectordb -> IncreaseCrystalNumber();
	      }
	    else if (crystalgeometry == 4)
	      {
		detectordb -> IncreaseCrystalGeometry4Number();
		detectordb -> IncreaseCrystalNumber();
	      }
	    else if (crystalgeometry == 5)
            {
              detectordb -> IncreaseCrystalGeometry5Number();
              detectordb -> IncreaseCrystalGeometry5SlotNumber(); // does not count the total number of type 5 crystals but the number of slots occupied
              detectordb -> IncreaseCrystalNumber();
            }
            else if (crystalgeometry == 11)
            {
              detectordb -> IncreaseCrystalGeometry11Number();
              detectordb -> IncreaseCrystalNumber();
            }
	  }
	
	// crystals 
	
	if (fFlagConstructCrystals == true && fCrystalType[i][j] >= 0)
	  {
		// This call should be different depending on the geometry type.
	    G4int crystalgeometry = (detectordb -> GetDetectorParameters(fCrystalType[i][j]))->
	      GetGeometry();

              if (crystalgeometry == 1 || crystalgeometry == 2 || crystalgeometry == 11)
	      {
		GEGeometryCrystal* crystal = new GEGeometryCrystal(detectordb, i, j);
		crystal->SetCrystalGeometryRotation(fFlagReverseNonTrueCoaxial);
		fCrystalContainer.push_back(crystal);
	      }
	    else if ( crystalgeometry == 3) 
	      {
	    	// Should be implemented directly in the GEGeometryCrystal
		GEGeometryCrystal* crystal = new GEGeometryCrystal(detectordb, i, j);
		fCrystalContainer.push_back(crystal);
		// Construct a rotated crystal - back-to-back
		detectordb -> IncreaseCrystalGeometry3Number();
		detectordb -> IncreaseCrystalNumber();
		crystal = new GEGeometryCrystal(detectordb,i, j);
		crystal->SetCrystalGeometryRotation(true);
		fCrystalContainer.push_back(crystal);
	      } 
	    else if (crystalgeometry == 4 ) {
	      GEGeometryCrystal* crystal = 
		new GEGeometryCrystal(detectordb, i, j);
	      crystal->SetCrystalGeometryRotation(fFlagReverseNonTrueCoaxial);
	      fCrystalContainer.push_back(crystal);
	      if (detectordb->IsAPair(i,j))
		{
		  for (G4int kk=1;kk<detectordb->GetNumberOfMultiplets();kk++)
		    {
		      detectordb -> IncreaseCrystalGeometry4Number();
		      detectordb -> IncreaseCrystalNumber();
		      GEGeometryCrystal* othercrystal =  new GEGeometryCrystal(detectordb,i, j,kk);
		      othercrystal->SetCrystalGeometryRotation(fFlagReverseNonTrueCoaxial);
		      fCrystalContainer.push_back(othercrystal);
		    }
		}
	    
	    }
            else if (crystalgeometry == 5) {
              GEGeometryCrystal* crystal =
                new GEGeometryCrystal(detectordb, i, j);
              crystal->SetCrystalGeometryRotation(fFlagReverseNonTrueCoaxial);
              fCrystalContainer.push_back(crystal);
              if (detectordb->IsAPair(i,j))
              {
                for (G4int kk=1;kk<detectordb->GetNumberOfMultiplets();kk++)
                {
                  detectordb -> IncreaseCrystalGeometry5Number();
                  detectordb -> IncreaseCrystalNumber();
                  GEGeometryCrystal* othercrystal =  new GEGeometryCrystal(detectordb,i, j,kk);
                  othercrystal->SetCrystalGeometryRotation(fFlagReverseNonTrueCoaxial);
                  fCrystalContainer.push_back(othercrystal);
                }
              }
            }
/*
	    GEGeometryCrystal* crystal = new GEGeometryCrystal(i, j, detectordb);
	    
	    fCrystalContainer.push_back(crystal); 
*/
	  }

	// support 

	if (fFlagConstructSupport == true && fCrystalType[i][j] >= 0)
	  {
	    GEGeometryHolder* holder = new GEGeometryHolder(i, j, detectordb); 

	    fHolderContainer.push_back(holder); 
	  }

	// cabling 

	if (fFlagConstructCabling == true && fCrystalType[i][j] >= 0)
	  {
	    GEGeometryCable* cable = new GEGeometryCable(i, j, detectordb); 
	    
	    fCableContainer.push_back(cable); 
	  }
      }
 
  // loop over strings 


  for (G4int i = 0; i < detectordb->GetNumberOfStrings(); i++)
    {
      G4bool flag_column = false;

      for (G4int j = 0; j < detectordb->GetNumberOfRows(); j++) 
	if (fCrystalType[i][j] >= 0)
	  flag_column = true; 
      
      // electronics 
      //FIXME: now the electronics are built together with the cables as it is neccesary to build 
      //PhaseII b2b BEGe holders without electronics (electronics for PhaseII not yet implemented)
      if (fFlagConstructCabling == true && flag_column == true)
	{
	  detectordb -> IncreaseColumnNumber(); 
          GEGeometryDetectorParameters* detParm            = 
	    detectordb ->GetDetectorParameters( fCrystalType[i][0] );
          assert(detParm);
          if(detParm->GetGeometry()==1) detectordb -> IncreaseColumnGeom1Number(); 
          if(detParm->GetGeometry()==2) detectordb -> IncreaseColumnGeom2Number(); 
          if(detParm->GetGeometry()==3) { detectordb -> IncreaseColumnGeom2Number(); 
	    detectordb -> IncreaseColumnGeom2Number(); }
          if(detParm->GetGeometry()==4) 	    
	    detectordb -> IncreaseColumnGeom1Number(); // type 4 xtals with type 1 column properties	 
/*          if(detParm->GetGeometry()==5)             
            detectordb -> IncreaseColumnGeom2Number(); // type 5 xtals with type 3 column properties ?! */  

	  GEGeometryElectronics* electronics = new GEGeometryElectronics(i, detectordb); 

	  fElectronicsContainer.push_back(electronics); 
	}
    }

}

// --------------------------------------------------------------------------- 

GEGeometryDetectorArray::~GEGeometryDetectorArray()
{

  fCrystalContainer.clear(); 
  fHolderContainer.clear(); 
  fCableContainer.clear(); 
  fElectronicsContainer.clear(); 

} 

// --------------------------------------------------------------------------- 

void GEGeometryDetectorArray::ConstructDetectorArray() 
{

  // crystals 

  if (fFlagConstructCrystals == true)
    for (G4int i = 0; i < int(fCrystalContainer.size()); i++)
      fCrystalContainer[i] -> ConstructCrystal(); 

  // holders 

  if (fFlagConstructSupport == true) 
    for (G4int i = 0; i < int(fHolderContainer.size()); i++)
      fHolderContainer[i] -> ConstructHolder(); 

  // cables 

  if (fFlagConstructCabling == true)
    for (G4int i = 0; i < int(fCableContainer.size()); i++)
      fCableContainer[i] -> ConstructCable(); 

  // electronics 

  if (fFlagConstructCabling == true ) 
      for (G4int i = 0; i < int(fElectronicsContainer.size()); i++) {
          MGLog(debugging) << "Start to construct Electronics of String "<< i <<endlog;
          fElectronicsContainer[i] -> ConstructElectronics(); 
          MGLog(debugging) << "End to construct Electronics of String "<< i <<endlog;
      }
  

} 

// --------------------------------------------------------------------------- 

