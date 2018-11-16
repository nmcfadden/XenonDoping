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
 *
 * DESCRIPTION:.
 *      This module handles the electric field and weighting potential calculations.
 *      The code is based on fields_ppc.c by Karin Lagergren.
 *
 * AUTHOR:
 *      Khang Nguyen & Jason Detwiler
 *
 * CONTACT:
 *      khangdnguyen@lbl.gov, jadetwiler@lbl.gov
 *
 * FIRST SUBMISSION:
 *
 * REVISION: MM-DD-YYYY
 *      03-26-2011 
 *
 *
 */

#include <cmath>
#include <cstdio>
#include <sstream>

#include "io/MGLogger.hh"
#include "waveform/MGSiggenUtils.hh"
#include "waveform/MGM3dcrPPCCrystalFields.hh"

using namespace std;
using namespace CLHEP;


/* LoadFields()
 * Reads in electric field and weighting potential tables from files
 * listed in directory. Check that number of contacts matches 
 * nContacts. */

void MGM3dcrPPCCrystalFields::LoadFields()
{
  if(fCrystalData == NULL) {
    MGLog(error) << "MGCrystalData object contains no data about the geometry of the crystal!" << endlog;
    return;
  }

  ifstream fieldsFile(fFieldsFileName.c_str());
  MGLog(routine) << "Reading field data from file: " << fFieldsFileName << endlog;
  if(!fieldsFile.good()){
    MGLog(error) << "Failed to open field init file: " << fFieldsFileName << endlog;
    return;
  }

  // min, max, and step (r direction)
  double rMin = 0.0, rMax = 0.0, rStep = 0.0;
  if ( sscanf(MGSiggenUtils::ReadSetupLine(fieldsFile).c_str(), "%lf %lf %lf", &rMin, &rMax, &rStep) != 3 ){
    MGLog(error) << "Failed to read r limits and number of r steps from file: "
                 << fFieldsFileName << endlog;
    fieldsFile.close();
    return;
  }
  rMin *= mm; // read in as mm (GRETINA convention)
  rMax *= mm; 
  SetRMin(rMin);
  SetRMax(rMax);
  SetRStep(rStep);
  MGLog(routine) << "rMin: " << rMin/cm << " cm  rMax: " << rMax/cm << " cm  rStep: " << rStep << endlog;

  // min, max, and step (z direction)
  double zMin = 0.0, zMax = 0.0, zStep = 0.0;
  double zMinAdjusted = 0.0, zMaxAdjusted = 0.0;
  if( sscanf(MGSiggenUtils::ReadSetupLine(fieldsFile).c_str(), "%lf %lf %lf", &zMin, &zMax, &zStep) != 3 ){
    MGLog(error) << "Failed to read z limits and number of z steps from file: " 
                 << fFieldsFileName << endlog;
    fieldsFile.close();
    return;
  }
  zMin *= mm; // read in as mm (GRETINA convention)
  zMax *= mm;
  zMinAdjusted = zMin - fCrystalData->GetCrystalHeight()/2.0; // z-min and z-max values must be adjusted to match Majorana
  zMaxAdjusted = zMax - fCrystalData->GetCrystalHeight()/2.0; // coordinate system, which is different from GRETINA's
  SetZMin(zMinAdjusted);
  SetZMax(zMaxAdjusted);
  SetZStep(zStep);
  MGLog(routine) << "zMin: " << zMinAdjusted/cm << " cm  zMax: " << zMaxAdjusted/cm << " cm  zStep: " << zStep << endlog;

  // temperature - just skip over it, but give a warning if not found
  double temperature;
  if( sscanf(MGSiggenUtils::ReadSetupLine(fieldsFile).c_str(), "%lf", &temperature) != 1 ){
    MGLog(warning) << "Temperature is not specified properly in file: " << fFieldsFileName << endlog;
  }

  // drift velocity lookup table - just skip over it, but give a warning if not found
  string driftVelocityLine = MGSiggenUtils::ReadSetupLine(fieldsFile);
  if( driftVelocityLine == "" ){
    MGLog(warning) << "Location of drift velocity table is not specified!" << endlog;
  }

  // electric field data
  string eFieldLine = MGSiggenUtils::ReadSetupLine(fieldsFile);
  if( eFieldLine == "" ){
    MGLog(error) << "Failed to read electric field data from file: " << eFieldLine << endlog;
    fieldsFile.close();
    return;
  }
  const char* eFieldFileName = eFieldLine.c_str();
  if( ReadEField(eFieldFileName) != true ){ 
    MGLog(error) << "Failed to read electric field data from file: " << eFieldFileName << endlog;
    fieldsFile.close();
    return;
  } 

  // weighting potential data
  string wPotentialLine = MGSiggenUtils::ReadSetupLine(fieldsFile);
  if( wPotentialLine == "" ){
    MGLog(error) << "Failed to read weighting potential data from file: " << wPotentialLine << endlog;
    fieldsFile.close();
    return;
  }
  const char* wPotentialFileName = wPotentialLine.c_str();
  if( ReadWPotential(wPotentialFileName) != true ){
    MGLog(error) << "Failed to read weighting potential data from file: " << wPotentialFileName << endlog;
    fieldsFile.close();
    return;
  } 

  MGLog(routine) << "All fields data loaded successfully." << endlog; 
  fieldsFile.close();
  return;
}

// -----------------------------------------------------------------------

/* ReadEField()
 * Read in electric field table for ppc detector. Return true if succeeded,
 * false if failed. This may or not not break if we switch to a non-integer
 * grid.
 */

bool MGM3dcrPPCCrystalFields::ReadEField(const char* eFieldFileName)
{
  ifstream eFieldFile(eFieldFileName);
  if(!eFieldFile.good()){
    MGLog(error) << "Failed to read electric field data from file: " << eFieldFileName << endlog;
    eFieldFile.close();
    return false;
  }

  MGLog(routine) << "Reading electric field data from file: " << eFieldFileName << endlog;
 
  // set up the array
  // ignore the last step in R, Z: Efield values are large, strange values
  // there. Instead, use linear interpolation to smoothly approach the detector
  // edge using the next inner grid points. (siggen does this too)
  size_t RTotalSteps = (size_t)rint( (fRMax - fRMin)/fRStep );
  size_t ZTotalSteps = (size_t)rint( (fZMax - fZMin)/fZStep ); 
  fEFieldVector.resize(RTotalSteps); 
  for( size_t iR = 0; iR < RTotalSteps; iR++ ){
    fEFieldVector[iR].resize(ZTotalSteps);
  }

  size_t lineNumber = 0;
  double coordinateR = 0.0, coordinateZ = 0.0;
  double coordinateZAdjusted = 0.0; // z must be adjusted to match Majorana's coordinate
                                    // system, which is different from GRETINA's
  double electricPotential = 0.0;
  double electricAbsValue = 0.0, electricFieldR = 0.0;
  double electricFieldZ = 0.0, electricFieldPhi = 0.0;
  MGM3dcrEFieldPoint eFieldPoint;
  while( !eFieldFile.eof() ){
    string line = MGSiggenUtils::ReadSetupLine(eFieldFile);
    if(line == "") continue;
    lineNumber++;
    if( sscanf(line.c_str(), "%lf" "%lf" "%lf" "%lf" "%lf" "%lf", &coordinateR, &coordinateZ, &electricPotential,
                                                                  &electricAbsValue, &electricFieldR, &electricFieldZ) != 6 ){
      MGLog(error) << "Failed to read electric field data from line number " << lineNumber << " of file " << eFieldFileName << endlog;    
      eFieldFile.close();
      return false;
    }
    coordinateR *= mm;
    coordinateZ *= mm;
    coordinateZAdjusted = coordinateZ - fCrystalData->GetCrystalHeight()/2.0; // z value must be adjusted to match Majorana's coordinate
                                                                              // system, which is different from GRETINA's
    electricPotential *= volt;                                                
    electricAbsValue *= volt/cm;
    electricFieldR *= volt/cm;
    electricFieldZ *= volt/cm;

    size_t iRStep = (size_t)rint( (coordinateR - fRMin)/fRStep );
    size_t iZStep = (size_t)rint( (coordinateZAdjusted - fZMin)/fZStep ); 
    if( iRStep >= RTotalSteps ){
      if( iRStep > RTotalSteps + 1 ){
        MGLog(error) << "Coordinate r (=" << coordinateR/cm << " cm), whose step size is " << iRStep << ", is not in grid table!\n"
                     << "The electric field corresponding to this r is " << electricFieldR/(volt/cm) << " V/cm.\n" 
                     << "Coordinate z here is: " << coordinateZ/cm << " cm." << endl << endlog; 
      } 
      // ignore the last step in R
      continue;
    }
    if( iZStep >= ZTotalSteps ){
      if( iZStep > ZTotalSteps + 1 ){
        MGLog(error) << "Coordinate z (=" << coordinateZ/cm << " cm), whose step size is " << iZStep << ", is not in grid table!\n"
                     << "The electric field corresponding to this z is " << electricFieldZ/(volt/cm) << " V/cm.\n"
                     << "Coordinate r here is: " << coordinateR/cm << " cm." << endl << endlog; 
      }
      // ignore the last step in Z
      continue;
    }

    // convert from cylindrical to cartesian coordinates
    // to check if point is inside crystal
    double coordinatePhi = 0.0;
    Hep3Vector eVector;
    eVector.setRhoPhiZ( coordinateR, coordinatePhi, coordinateZAdjusted );
    double x = eVector.getX();
    double y = eVector.getY();
    double z = eVector.getZ();
    if( fCrystalData->IsInCrystal(x, y, z) != true ){
      MGLog(error) << "Point (" << x/cm << " cm, " << y/cm << " cm, " << z/cm << " cm) is not inside crystal!" << endlog;
      MGLog(debugging) << "Error is at line number " << lineNumber << ", whose coordinates are: ("
                       << coordinateR/cm << " cm, " << coordinatePhi << " radians, " << coordinateZAdjusted/cm << " cm)." << endlog;
      continue;
    } 

    // set electric field values
    eFieldPoint.SetEFieldR(electricFieldR);
    eFieldPoint.SetEFieldPhi(electricFieldPhi);
    eFieldPoint.SetEFieldZ(electricFieldZ);

    fEFieldVector[iRStep][iZStep] = eFieldPoint;
  }

  MGLog(routine) << "Done reading " << lineNumber << " lines of electric field data." << endlog;
  if( lineNumber != ((1+RTotalSteps)*(1+ZTotalSteps)) ){
    MGLog(warning) << "Total number of lines read (=" << lineNumber 
                   << ") does not equal total number of steps (=" << RTotalSteps*ZTotalSteps << ")." << endlog;
  }

  eFieldFile.close();
  return true;
}

// -----------------------------------------------------------------------

/* ReadWPotential()
 * Read in weighting potential table for ppc.
 */

bool MGM3dcrPPCCrystalFields::ReadWPotential(const char* wPotentialFileName)
{
  ifstream wPotentialFile(wPotentialFileName);
  if(!wPotentialFile.good()){
    MGLog(error) << "Failed to read weighting potential data from file: " << wPotentialFileName << endlog;
    wPotentialFile.close();
    return false;
  }

  MGLog(routine) << "Reading weighting potential data from file: " << wPotentialFileName << endlog;

  // set up the array
  // It's probably not necessary to ignore the last step because the wpot is
  // well-behaved near the boundary, but we will just because siggen does
  size_t RTotalSteps = (size_t)rint( (fRMax - fRMin)/fRStep );
  size_t ZTotalSteps = (size_t)rint( (fZMax - fZMin)/fZStep ); 
  fWPotentialVector.resize(RTotalSteps);
  for( size_t iR = 0; iR < RTotalSteps; iR++ ){
    fWPotentialVector[iR].resize(ZTotalSteps);
  }

  size_t lineNumber = 0;
  double coordinateR = 0.0, coordinateZ = 0.0;
  double coordinateZAdjusted = 0.0; // z must be adjusted to match Majorana's coordinate
                                    // system, which is different from GRETINA's
  double weightingPotential = 0.0;
  MGM3dcrWPotentialPoint wPotentialPoint(1);
  while( !wPotentialFile.eof() ){
    string line = MGSiggenUtils::ReadSetupLine(wPotentialFile);
    if(line == "") continue;
    lineNumber++;
    if( sscanf(line.c_str(), "%lf" "%lf" "%lf", &coordinateR, &coordinateZ, &weightingPotential) != 3 ){
      MGLog(error) << "Failed to read weighting potential field data from line number " << lineNumber 
                   << " of file " << wPotentialFileName << endlog;    
      wPotentialFile.close();
      return false;
    }
    coordinateR *= mm;
    coordinateZ *= mm;
    coordinateZAdjusted = coordinateZ - fCrystalData->GetCrystalHeight()/2.0; // z value must be adjusted to match Majorana's coordinate
                                                                              // system, which is different from GRETINA's
    size_t iRStep = (size_t)rint( (coordinateR - fRMin)/fRStep );
    size_t iZStep = (size_t)rint( (coordinateZAdjusted - fZMin)/fZStep ); 
    if( iRStep >= RTotalSteps ){
      if( iRStep > RTotalSteps + 1 ){
        MGLog(error) << "Coordinate r (=" << coordinateR/cm << " cm), whose step size is " << iRStep << ", is not in grid table!\n"
                     << "The weighting potential corresponding to this r is " << weightingPotential << ".\n" 
                     << "Coordinate z here is: " << coordinateZ/cm << " cm." << endl << endlog; 
      } 
      // ignore the last step in R
      continue;
    }
    if( iZStep >= ZTotalSteps ){
      if( iZStep > ZTotalSteps + 1 ){
        MGLog(error) << "Coordinate z (=" << coordinateZ/cm << " cm), whose step size is " << iZStep << ", is not in grid table!\n"
                     << "The weighting potential corresponding to this z is " << weightingPotential << ".\n"
                     << "Coordinate r here is: " << coordinateR/cm << " cm." << endl << endlog; 
      } 
      // ignore the last step in Z
      continue;
    }

    // convert from cylindrical to cartesian coordinates
    // to check if point is inside crystal
    double coordinatePhi = 0.0;
    Hep3Vector wpVector;
    wpVector.setRhoPhiZ( coordinateR, coordinatePhi, coordinateZAdjusted );
    double x = wpVector.getX();
    double y = wpVector.getY();
    double z = wpVector.getZ();
    if( fCrystalData->IsInCrystal(x, y, z) != true ){
      MGLog(error) << "Point (" << x << " cm, " << y << " cm, " << z << " cm) is not inside crystal!" << endlog;
      MGLog(debugging) << "Error is at line number " << lineNumber << ", whose coordinates are: ("
                       << coordinateR/cm << " cm, " << coordinatePhi << " radians, " << coordinateZAdjusted/cm << " cm)." << endlog;
      continue;
    }

    // set weighting potential
    wPotentialPoint.SetWPotentialPoint(weightingPotential, 0);

    fWPotentialVector[iRStep][iZStep] = wPotentialPoint;
  }

  MGLog(routine) << "Done reading " << lineNumber << " lines of weighting potential data." << endlog;
  if( lineNumber != ((RTotalSteps+1)*(ZTotalSteps+1)) ){
    MGLog(warning) << "Total number of lines read (=" << lineNumber 
                   << ") does not equal total number of steps (=" << RTotalSteps*ZTotalSteps << ")." << endlog;
  }

  wPotentialFile.close();
  return true;
}

// -----------------------------------------------------------------------

/* NearestVoxelGridRIndex()
 * Find integer voxel grid index corresponding to given rho.
 */

size_t MGM3dcrPPCCrystalFields::NearestVoxelGridRIndex(double rho) const
{
  // always go to the lower bound of the voxel: can't use rint() here
  // or we get E-field cut-off before we reach the edge of the crystal
  size_t rIndex = (size_t) ((rho - fRMin)/fRStep);
  return rIndex;
}

// -----------------------------------------------------------------------

/* NearestVoxelGridZIndex()
 * Find integer voxel grid index corresponding to given z.
 */

size_t MGM3dcrPPCCrystalFields::NearestVoxelGridZIndex(double z) const
{
  // always go to the lower bound of the voxel: can't use rint() here
  // or we get E-field cut-off before we reach the edge of the crystal
  size_t zIndex = (size_t) ((z - fZMin)/fZStep);
  return zIndex;
}

// -----------------------------------------------------------------------

/* MoveToNearestValidVoxel()
 * Find and set the nearest valid r and z indices for the voxel which 
 * meets all the conditions for interpolation/extrapolation.
 */

bool MGM3dcrPPCCrystalFields::MoveToNearestValidVoxel(const Hep3Vector &coordinates) const
{
  MGM3dcrGridWeights* gridWeights = GetChangeableGridWeights();

  //double x = coordinates.getX();
  //double y = coordinates.getY();
  //double z = coordinates.getZ();

  // If given coordinates are the same as last coordinates sent in, don't bother checking again if the 
  // electric field exists there. Just go straight to calculating the field grid indices. This code
  // is included for efficiency. 
  if( coordinates.getRho() == gridWeights->fCurrentR && coordinates.getPhi() == gridWeights->fCurrentPhi
      && coordinates.getZ() == gridWeights->fCurrentZ ){
    gridWeights->fVoxelStartR   = NearestVoxelGridRIndex(coordinates.getRho());
    gridWeights->fVoxelStartPhi = NearestVoxelGridPhiIndex(0.0);
    gridWeights->fVoxelStartZ   = NearestVoxelGridZIndex(coordinates.getZ());
    //MGLog(trace) << "The coordinates (" << x/cm << " cm, " << y/cm << " cm, " << z/cm << " cm) were previously used." << endlog;
    return true;
  }

  // This check is now done in GetEField() / GetWPotential()
  //if( fCrystalData->IsInCrystal(x, y, z) != true ){ 
      //MGLog(error) << "Cannot find nearest r, phi, and z indices for point (" << x/cm << " cm, " 
                   //<< y/cm << " cm, " << z/cm << " cm) because it is outside of crystal." << endlog; 
    //return false;
  //} else {
    double d[3] = {0.0, -1.0, 1.0};
    for( size_t iZ = 0; iZ < 3; iZ++ ){
      double newZ = coordinates.getZ() + d[iZ]*fZStep;
      if( newZ < -fCrystalData->GetCrystalHeight()/2.0 || newZ > fCrystalData->GetCrystalHeight()/2.0 ) {
        continue;
      }
      for( size_t iR = 0; iR < 3; iR++ ){
        double newR = coordinates.getRho() + d[iR]*fRStep;
        if( newR < 0 || newR > fCrystalData->GetOuterRadius() ) {
          continue;
        }
        if( CheckVoxelEFieldExists(newR, newZ) ){
          gridWeights->fVoxelStartR = NearestVoxelGridRIndex(newR);
          gridWeights->fVoxelStartPhi = NearestVoxelGridPhiIndex(0.0);
          gridWeights->fVoxelStartZ = NearestVoxelGridZIndex(newZ);
          // store given coordinates used if we're able to get this far 
          gridWeights->fCurrentR = coordinates.getRho();
          gridWeights->fCurrentPhi = coordinates.getPhi();
          gridWeights->fCurrentZ = coordinates.getZ();
          /* for debugging purposes */
          //MGLog(debugging) << "fVoxelStartR is: " << gridWeights->fVoxelStartR << endlog;
          //MGLog(debugging) << "fVoxelStartPhi is: " << gridWeights->fVoxelStartPhi << endlog;
          //MGLog(debugging) << "fVoxelStartZ is: " << gridWeights->fVoxelStartZ << endlog;
          //MGLog(debugging) << "fCurrentR is: " << gridWeights->fCurrentR << endlog;
          //MGLog(debugging) << "fCurrentPhi is: " << gridWeights->fCurrentPhi << endlog;
          //MGLog(debugging) << "fCurrentZ is: " << gridWeights->fCurrentZ << endl << endlog;
          return true;
        }
      }                                                   
    }
  //}

  return false;
}

// -----------------------------------------------------------------------

/* CheckVoxelEFieldExists() 
 * Takes in coordinates r and z and checks in the table of 
 * data if the electric field exists at this particular point. 
 * Returns true if electric field exists, false otherwise.  
 */

bool MGM3dcrPPCCrystalFields::CheckVoxelEFieldExists( double voxelStartR, double voxelStartZ ) const
{
  size_t iRIndex = NearestVoxelGridRIndex(voxelStartR);
  size_t iZIndex = NearestVoxelGridZIndex(voxelStartZ); 
  size_t RTotalSteps = (size_t)rint( (fRMax - fRMin)/fRStep );
  size_t ZTotalSteps = (size_t)rint( (fZMax - fZMin)/fZStep ); 
  if( iRIndex + 1 >= RTotalSteps || iZIndex + 1 >= ZTotalSteps ){
    return false;
  } 

  MGM3dcrEFieldPoint iEFieldPoint;
  for( size_t iR = 0; iR < 2; iR++ ){
    size_t iRNewIndex = iRIndex + iR;
    for( size_t iZ = 0; iZ < 2; iZ++ ){
      size_t iZNewIndex = iZIndex + iZ;
      iEFieldPoint = fEFieldVector[iRNewIndex][iZNewIndex];
      if( iEFieldPoint.GetEFieldR() == 0.0 && iEFieldPoint.GetEFieldZ() == 0.0 ){
        MGLog(error) << "There is no electric field at new r index " << iRNewIndex 
                     << " and new z index " << iZNewIndex << "." << endlog;
        return false;
      }
    }
  }

  return true;
}

// -----------------------------------------------------------------------

/* CalculateGridWeights()
 * Find weights for 8 voxel corner points around given point for  
 * electric/weighting potential field. This method is credited to 
 * David C. Radford.
 */

void MGM3dcrPPCCrystalFields::CalculateGridWeights(const Hep3Vector &coordinates) const
{
  MGM3dcrGridWeights* gridWeights = GetChangeableGridWeights();

  if( MoveToNearestValidVoxel(coordinates) != true ){
    gridWeights->fWeights[0][0] = 0;
    gridWeights->fWeights[0][1] = 0;
    gridWeights->fWeights[1][0] = 0;
    gridWeights->fWeights[1][1] = 0;
    return;
  }  

  double r = ((coordinates.getRho() - fRMin)/fRStep) - (double)gridWeights->fVoxelStartR;
  double z = ((coordinates.getZ() - fZMin)/fZStep) - (double)gridWeights->fVoxelStartZ;

  gridWeights->fWeights[0][0] = (1.0 - r) * (1.0 - z);
  gridWeights->fWeights[0][1] = (1.0 - r) *        z;
  gridWeights->fWeights[1][0] =        r  * (1.0 - z);
  gridWeights->fWeights[1][1] =        r  *        z;

  return;
}

// -----------------------------------------------------------------------

/* GetEField()
 * Find (interpolated or extrapolated) electric field for this point.
 */

Hep3Vector MGM3dcrPPCCrystalFields::GetEField(const Hep3Vector &coordinates) const
{
  Hep3Vector eField(0,0,0);
  if(!fCrystalData->IsInCrystal(coordinates)) return eField;

  CalculateGridWeights(coordinates);

  double eFieldR = 0.0;
  double eFieldZ = 0.0;
  MGM3dcrGridWeights* gridWeights = GetChangeableGridWeights();
  MGM3dcrEFieldPoint iEField;
  for( size_t i = 0; i < 2; i++ ){
    for( size_t j = 0; j < 2; j++ ){
      iEField = fEFieldVector[gridWeights->fVoxelStartR + i][gridWeights->fVoxelStartZ + j];
      eFieldR += gridWeights->fWeights[i][j] * iEField.GetEFieldR();
      eFieldZ += gridWeights->fWeights[i][j] * iEField.GetEFieldZ(); // FIXME: Do we need to rescale vector???

      /* for debugging purposes */
      //MGLog(debugging) << "Coordinates used: (" << coordinates.getX()/cm << " cm, " 
      //                 << coordinates.getY()/cm << " cm, " << coordinates.getZ()/cm << " cm)" << endlog;
      //MGLog(debugging) << "Current i: " << i << "  current j: " << j << endlog;
      //MGLog(debugging) << "fVoxelStartR+i used: " << gridWeights->fVoxelStartR+i 
      //                 << "  fVoxelStartZ+j used: " << gridWeights->fVoxelStartZ+j << endlog;
      //MGLog(debugging) << "fWeights[0][0] = " << gridWeights->fWeights[0][0] << endlog;
      //MGLog(debugging) << "fWeights[0][1] = " << gridWeights->fWeights[0][1] << endlog;
      //MGLog(debugging) << "fWeights[1][0] = " << gridWeights->fWeights[1][0] << endlog;
      //MGLog(debugging) << "fWeights[1][1] = " << gridWeights->fWeights[1][1] << endlog;
      //MGLog(debugging) << "iEFieldR: " << iEField.GetEFieldR()/(volt/cm) << " volt/cm  iEFieldZ: " 
      //                                 << iEField.GetEFieldZ()/(volt/cm) << " volt/cm" << endlog;
      //MGLog(debugging) << "eFieldR: " << eFieldR/(volt/cm) << " volt/cm  eFieldZ: " 
      //                 << eFieldZ/(volt/cm) << " volt/cm" << endl << endlog;      
    }
  }
  double eFieldX = eFieldR * cos( coordinates.phi() ); // eFieldR can be negative, so Hep3Vector will produce an error message
  double eFieldY = eFieldR * sin( coordinates.phi() ); // saying we're using negative rho (not allowed). Hence we need to express
  if(eFieldX == -0.0) eFieldX = 0.0;                   // electric field in Cartesian coordinates to allow for negative values.
  if(eFieldY == -0.0) eFieldY = 0.0;
  if(eFieldZ == -0.0) eFieldZ = 0.0;

  eField.set( eFieldX, eFieldY, eFieldZ );

  return eField;
}

// -----------------------------------------------------------------------

/* GetWPotential()
 * Gives (interpolated) weighting potential for ONE contact at point
 * (x, y, z). This value is stored in wPotential, which is assumed to have
 * one element per contact. 
 */

double MGM3dcrPPCCrystalFields::GetWPotential( const Hep3Vector &coordinates, size_t contact ) const
{
  if(!fCrystalData->IsInCrystal(coordinates)) return 0.0;

  double wPotential = 0.0;
  if(contact != 0) return wPotential;

  CalculateGridWeights(coordinates);

  MGM3dcrGridWeights* gridWeights = GetChangeableGridWeights();
  MGM3dcrWPotentialPoint iWeightingPotential(1);
  for( size_t i = 0; i < 2; i++ ){
    for( size_t j = 0; j < 2; j++ ){
      iWeightingPotential = fWPotentialVector[gridWeights->fVoxelStartR + i][gridWeights->fVoxelStartZ + j];
      wPotential += gridWeights->fWeights[i][j] * iWeightingPotential.GetWPotentialPoint(0);

      /* for debugging purposes */
      ///MGLog(debugging) << "Coordinates used: (" << coordinates.getX()/cm << " cm, " 
      ///                 << coordinates.getY()/cm << " cm, " << coordinates.getZ()/cm << " cm)" << endlog;
      ///MGLog(debugging) << "Current i: " << i << "  current j: " << j << endlog;
      ///MGLog(debugging) << "fVoxelStartR+i used: " << gridWeights->fVoxelStartR+i 
      ///                 << "  fVoxelStartZ+j used: " << gridWeights->fVoxelStartZ+j << endlog;
      ///MGLog(debugging) << "fWeights[0][0] = " << gridWeights->fWeights[0][0] << endlog;
      ///MGLog(debugging) << "fWeights[0][1] = " << gridWeights->fWeights[0][1] << endlog;
      ///MGLog(debugging) << "fWeights[1][0] = " << gridWeights->fWeights[1][0] << endlog;
      ///MGLog(debugging) << "fWeights[1][1] = " << gridWeights->fWeights[1][1] << endlog;
      ///MGLog(debugging) << "iWeightingPotential = " << iWeightingPotential.GetWPotential(0) << endlog;
      ///MGLog(debugging) << "wPotential = " << wPotential << endl << endlog;
    }
  }
 
  return wPotential;
}

// -----------------------------------------------------------------------

/* GetWField()
 * Find (interpolated or extrapolated) electric potential for this point.
 */

Hep3Vector MGM3dcrPPCCrystalFields::GetWField( const Hep3Vector& /*coordinates*/, size_t contact ) const
{
  MGLog(error) << "GetWField is not implemented at this time!" << endlog;

  Hep3Vector wField;
  if(contact != 0) return wField;

  return wField;
}

