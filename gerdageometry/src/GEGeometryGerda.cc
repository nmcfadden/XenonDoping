//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//
// $Id: GEGeometryGerda.cc,v 1.21 2009-07-14 08:27:26 jliu Exp $
//
// CLASS IMPLEMENTATION:  GEGeometryGerda.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * this is the class for the whole Gerda enviroument setup
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Xiang Liu
 * CONTACT: @CONTACT@
 * FIRST SUBMISSION: @START_DATE@
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 11-17-2004, created
 * 11-18-2004, Luciano, removed blank space in the detector name
 * 12-06-2004, C.Tomei, selects AirBox as most external logical volume
 * 01-18-2004, K. Kroeninger, new geometry file structure implemented 
 * 03-23-2005  Added flag for cryogenic liquid: LN2 (true), LAr (false). Marie
 * 09-11-2006, Added protection against crashes if the calibrationArray is not
 *             defined in the geometry, Luciano
 * 09-04-2007, Added definition of SensitiveRegion (not done anymore in the 
 *             base class), Luciano
 * 12-05-2007, the DetectorMotherVolume must be set correctly after the changes 
 *             in the cryostat, Jens
 * 04-29-2008, the most external logical volume of the GERDA setup is not anymore 
 *             fAirBoxLogical but fExternalRockLogical, since the geometry includes 
 *             Hall A of LNGS
 * 14-07-2009, changed the impurity setup function. Jing
 * 14-06-2011, added possibility of detector encapsulation, Sabine
 *   26-03-12012, added possibility of inserting mini-shrouds, Giovanni
 * 30-05-2012 - Added collimated calibration source for the study of the feasibility of deadlayer estimation. Giovanni Benato
 *
 */
//---------------------------------------------------------------------------//

// --------------------------------------------------------------------------- 

#include "globals.hh"

#include <TMath.h> 

#include "geometry/MGGeometryDetector.hh"
//#include "geometry/MGGeometryDetectorConstruction.hh"

#include "gerdageometry/GEGeometryGerda.hh"
#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometryCryostatParts.hh"



// --------------------------------------------------------------------------- 

GEGeometryGerda::GEGeometryGerda() : MGGeometryDetector(""),
				     fGERDADetector(0),fGERDAShielding(0),
				     fGERDACalibrationSourceArray(0), 
				     fGERDACollimatedCalibrationSourceArray(0), 
				     fGERDAEncapsulation(0),
				     fGERDAMiniShroud(0),
				     fGERDALArInstrumentation(0)
{

  fGERDADetectorDB = new GEGeometryDetectorDB(); 
  fSensitiveRegion = new G4Region("SensitiveRegion");
  MGLog(trace) << "Sensitive Region registered" << endlog;
}

// --------------------------------------------------------------------------- 

GEGeometryGerda::~GEGeometryGerda()
{
  if (fGERDADetectorDB)
    delete fGERDADetectorDB; 
  if (fGERDADetector)
    delete fGERDADetector; 
  if (fGERDAShielding) 
    delete fGERDAShielding; 
  if (fGERDACalibrationSourceArray)
    delete fGERDACalibrationSourceArray; 
  if (fGERDACollimatedCalibrationSourceArray)
    delete fGERDACollimatedCalibrationSourceArray; 
  if (fGERDAEncapsulation)
    delete fGERDAEncapsulation; 
  if (fGERDAMiniShroud)
    delete fGERDAMiniShroud;
  if (fSensitiveRegion) 
    delete fSensitiveRegion;
  if (fGERDALArInstrumentation)
	  delete fGERDALArInstrumentation;
}

// --------------------------------------------------------------------------- 

void GEGeometryGerda::ConstructDetector()
{

  // construct database 

  fGERDADetectorDB -> ConstructGEGeometryDetectorDB(); 

  // construct shielding 

  if (fGERDADetectorDB -> GetFlagConstructShielding() == true)
    {

      fGERDAShielding = new GEGeometryShielding(fGERDADetectorDB); 

      fGERDAShielding -> ConstructShielding();

      fGERDADetectorDB -> SetDetectorMotherVolume(fGERDADetectorDB -> GetCryostatParts()->GetCryoInnerwall_Inside()->GetLogicalVolume()); 

    }
  else
    {
      fGERDADetectorDB -> SetDetectorMotherVolume(fGERDADetectorDB -> GetAirBoxLogical()); 
    }
  
  // construct detector 

  if (fGERDADetectorDB -> GetFlagConstructDetector() == true) 
    {

      fGERDADetector = new GEGeometryDetector(fGERDADetectorDB); 

      fGERDADetector -> ConstructDetector(); 
    }

  // construct calibration sources 

  if (fGERDADetectorDB -> GetFlagConstructCalibrationSource() == true) 
    {
      fGERDACalibrationSourceArray = new GEGeometryCalibrationSourceArray(fGERDADetectorDB); 

      fGERDACalibrationSourceArray -> ConstructCalibrationSourceArray(); 
    }

  // construct calibration sources 

  if (fGERDADetectorDB -> GetFlagConstructCollimatedCalibrationSource() == true) 
    {
      fGERDACollimatedCalibrationSourceArray = new GEGeometryCollimatedCalibrationSourceArray(fGERDADetectorDB); 

      fGERDACollimatedCalibrationSourceArray -> ConstructCollimatedCalibrationSourceArray(); 
    }

  // construct copper encapsulation

  if (fGERDADetectorDB -> GetFlagConstructCopperEncapsulation() == true) 
    {
      fGERDAEncapsulation = new GEGeometryEncapsulation(fGERDADetectorDB); 

      fGERDAEncapsulation -> ConstructCopperEncapsulation(); 
    }

  // construct teflon encapsulation

  if (fGERDADetectorDB -> GetFlagConstructTeflonEncapsulation() == true) 
    {
      fGERDAEncapsulation = new GEGeometryEncapsulation(fGERDADetectorDB); 

      fGERDAEncapsulation -> ConstructTeflonEncapsulation(); 
    }

  // construct mini-shrouds

  if (fGERDADetectorDB -> GetFlagConstructMiniShroud() == true) 
    {
      fGERDAMiniShroud = new GEGeometryMiniShroud(fGERDADetectorDB); 

      fGERDAMiniShroud -> ConstructMiniShroud(); 
    }

  //TODO: Add an additional iteration to build the LAr instrumentation
  //      Check that the LAr instrumentation flag is set. If so build it
  if ( fGERDADetectorDB->GetFlagConstructLArInstrumentation() == true )
  {
	  fGERDALArInstrumentation = new GEGeometryLArInstrumentation(fGERDADetectorDB);
	  fGERDALArInstrumentation -> ConstructLArInstrumentation();
  }

  // set detector properties 

  SetDetectorLogical(fGERDADetectorDB ->GetExternalRockLogical()); 

  SetDetectorName("GERDA"); 

}

// --------------------------------------------------------------------------- 

void GEGeometryGerda::FillCrystalDataVector(MGCrystalDataVector* crystaldatavector)
{

  // reset vector of crystal data  

  if (crystaldatavector == 0) 
    crystaldatavector = new MGCrystalDataVector(); 

  crystaldatavector -> clear(); 

  // loop over matrix

  for (G4int irow = 0; irow < 7; irow++) 
    for (G4int icolumn = 0; icolumn < 19; icolumn++)
      {
	// get detector type on matrix position 

	G4int typeposition = fGERDADetectorDB -> GetCrystalType(icolumn, irow); 

	if (typeposition < 0)
	  continue; 

	// get GEGeometryDetectorParameters from database 
	
	GEGeometryDetectorParameters* parameters = fGERDADetectorDB -> GetDetectorParameters(typeposition); 

	// create new MGCrystalData 
	
	MGCrystalData* crystaldata = new MGCrystalData(); 

	// get properties 
	
	G4int detectorID = parameters -> GetDetectorID(); 
	
	//  enum EDiodeType { kPType, kNType };
	//  enum EGeometryType { kCoaxial, kClosedEnd, kPointContact };
	
	// Default value is kNType
	MGCrystalData::EDiodeType diodetype = MGCrystalData::kNType;
	
	if (parameters -> GetType() == 0)
	  diodetype = MGCrystalData::kNType; 
	
	else if (parameters -> GetType() == 1) 
	  diodetype = MGCrystalData::kPType; 
	
	// Default value is kCoaxial
	MGCrystalData::EGeometryType geometrytype = MGCrystalData::kCoaxial;

	if (parameters -> GetGeometry() == 1) 
	  geometrytype = MGCrystalData::kClosedEnd; 
	
	else if (parameters -> GetGeometry() == 2) 
	  geometrytype = MGCrystalData::kCoaxial; 
	
	G4double innerradius   = parameters -> GetInnerRadius(); 
	G4double outerradius   = parameters -> GetOuterRadius(); 
	G4double crystalheight = parameters -> GetHeight(); 
	G4double welldepth     = parameters -> GetBoreDepth(); 
	
	G4double outerradial = parameters -> GetDLOuterThickness(); 
	G4double outertop    = parameters -> GetDLTopThickness(); 
	G4double outerbottom = parameters -> GetDLBottomThickness(); 
	G4double innerradial = parameters -> GetDLInnerThickness(); 
	G4double innertop    = parameters -> GetDLBottomThickness(); 
	
	G4double passivation = parameters -> GetPassivationThickness(); 
	
	G4double impuritytop    = parameters -> GetImpurityDensityTop(); 
	G4double impuritybottom = parameters -> GetImpurityDensityBottom(); 
	
	G4ThreeVector position = fGERDADetectorDB -> GetCrystalPosition(icolumn, irow); 

	G4double globalx = position.getX(); 
	G4double globaly = position.getY(); 
	G4double globalz = position.getZ(); 
	
	G4double crystalangle = parameters -> GetCrystalAngle(); 

	G4int nsegmentsphi = TMath::Max(1, parameters -> GetNPhiSegments()); 
	G4int nsegmentsz   = TMath::Max(1, parameters -> GetNZSegments()); 

	G4double segmentationangle = parameters -> GetSegmentationAngle(); 

	// copy properties 
	  
	crystaldata -> SetCrystalID(detectorID); 
	crystaldata -> SetDiodeType(diodetype); 
	crystaldata -> SetGeometryType(geometrytype); 
	crystaldata -> SetCrystalDimensions(innerradius, outerradius, crystalheight, welldepth); 
	crystaldata -> SetDeadLayer(outerradial, outertop, outerbottom, innerradial, innertop); 
	crystaldata -> SetPassivationThickness(passivation); 
	crystaldata -> SetImpurityDensity(impuritytop, impuritybottom); 
	//crystaldata -> SetImpurityDensity(); 
	crystaldata -> SetGlobalPosition(globalx, globaly, globalz); 
	crystaldata -> SetCrystalAxesAngle(crystalangle); 
	crystaldata -> SetSegmentation(nsegmentsz, nsegmentsphi); 
	crystaldata -> SetSegmentationAngle(segmentationangle); 

	// add to vector 
	
	crystaldatavector -> push_back(crystaldata); 
      }

}

// --------------------------------------------------------------------------- 
 

