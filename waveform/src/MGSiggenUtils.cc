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
 * 	This module includes misc. I/O functions.
 * 	based on signal_calc_utils.c by Karin Lagergren
 *
 * AUTHOR:
 * 	Khang Nguyen & Jason Detwiler
 *
 * CONTACT:
 * 	khangdnguyen@lbl.gov, jadetwiler@lbl.gov
 *
 * FIRST SUBMISSION:
 *
 * REVISION: MM-DD-YYYY
 * 	01-13-2011 
 *
 *
 */

#include <cmath>
#include <iostream>
#include <cstdio>
#include <sstream>

#include "io/MGLogger.hh"
#include "waveform/MGSiggenUtils.hh"

using namespace std;
using namespace CLHEP;

/* ReadSetupLine()
   Reads one line from config file as a string. Returns 
   the string for success, error message for failure. */

string MGSiggenUtils::ReadSetupLine(ifstream &file)
{
   string line = "";   // each line read from the file is stored as a string
   if(file.is_open()){   // make sure that the file is open 
      while(file.good()){   // make sure that file is in a state ready for processing
         getline(file, line);
         if(!file.good()) break;

         // if a # (hash) symbol is found, its position in the line is
         // returned and contents are deleted from this position forward
         size_t hashPosition;
         hashPosition = line.find('#');
         if(hashPosition != string::npos){
            line.erase(hashPosition);
         }

         // remove white space(s) at the beginning and end of string
         size_t firstNonspace = line.find_first_not_of(' ');
         if(firstNonspace == string::npos) continue;   // move on if string is all spaces (or empty)
         size_t lastNonspace = line.find_last_not_of(' ');
         line = line.substr(firstNonspace, lastNonspace - firstNonspace + 1);
     
         break; 
       }
   } else cout << "ERROR: Unable to open file.\n";
   return line;
}

// -----------------------------------------------------------------------

/* InitializeNewPPCGeometry() 
 * Reads information about PPC crystal geometry from file given by
 * geometryFileName. Returns pointer to MGCrystalData class. */

MGCrystalData* MGSiggenUtils::InitializeNewPPCGeometry( const char* geometryFileName, int crystalID,
                                                        double globalX, double globalY, double globalZ, double wrapAroundDepth )
{
  MGCrystalData* crystalData = new MGCrystalData( crystalID, MGCrystalData::kPType, MGCrystalData::kPointContact, wrapAroundDepth );
  crystalData->SetGlobalPosition( globalX, globalY, globalZ );

  ifstream geometryFile(geometryFileName);
  MGLog(routine) << "Opening PPC crystal geometry file: " << geometryFileName << endlog; 
  if(!geometryFile.good()){
    MGLog(error) << "Failed to open PPC geometry configuration file: " << geometryFileName << endlog;
    return crystalData;
  } 

  // crystal geometry data
  double crystalHeight = 0.0;
  if( sscanf(ReadSetupLine(geometryFile).c_str(), "%lf", &crystalHeight) != 1 ){
    MGLog(error) << "Failed to read z length (crystal height) from file: " << geometryFileName << endlog;
  }
  crystalHeight *= mm;

  double outerRadius = 0.0;
  if( sscanf(ReadSetupLine(geometryFile).c_str(), "%lf", &outerRadius) != 1 ){
    MGLog(error) << "Failed to read detector radius from file: " << geometryFileName << endlog;
  }
  outerRadius *= mm;

  double bulletizationRadius = 0.0;
  if( sscanf(ReadSetupLine(geometryFile).c_str(), "%lf", &bulletizationRadius) != 1 ){
    MGLog(error) << "Failed to read bulletization radius from file: " << geometryFileName << endlog;
  } 
  bulletizationRadius *= mm;

  // contact dimensions
  double contactLength = 0.0, contactRadius = 0.0;
  if( sscanf(ReadSetupLine(geometryFile).c_str(), "%lf %lf", &contactLength, &contactRadius) != 2 ){
    MGLog(error) << "Failed to read contact dimensions from file: " << geometryFileName << endlog;
  }
  contactLength *= mm;
  contactRadius *= mm;

  double centralHoleLength = 0.0, centralHoleRadius = 0.0;
  if( sscanf(ReadSetupLine(geometryFile).c_str(), "%lf %lf", &centralHoleLength, &centralHoleRadius) != 2 ){
    MGLog(error) << "Failed to read central hole size from file: " << geometryFileName << endlog;
  }
  centralHoleLength *= mm;
  centralHoleRadius *= mm; 
  if( centralHoleLength != 0.0 && centralHoleRadius != 0.0 ){
    MGLog(error) << "Central hole length (=" << centralHoleLength/cm << " cm) and central hole radius (=" 
                 << centralHoleRadius/cm << " cm) are not both 0; not implemented at this time!" << endlog;
  }

  // set all the following unused parameters to 0.0 before we set all crystal dimensions
  double innerRadius = 0.0, wellDepth = 0.0, taperVertex = 0.0, taperAngle = 0.0; 
  double outerRadial = 0.0, outerTop = 0.0, outerBottom = 0.0, innerRadial = 0.0, innerTop = 0.0; 
  innerRadial = contactRadius; // in GRETINA convention, "contact radius" refers to Majorana's dead layer's "inner radial"
  innerTop = contactLength;    // in GRETINA convention, "contact length" refers to Majorana's dead layer's "inner top" 

  crystalData->SetCrystalDimensions( innerRadius, outerRadius, crystalHeight, wellDepth,
                                     wrapAroundDepth, taperVertex, taperAngle, bulletizationRadius );
  crystalData->SetDeadLayer( outerRadial, outerTop, outerBottom, innerRadial, innerTop );
  MGLog(routine) << "Crystal length (height): " << crystalHeight/cm << " cm" << endl;
  MGLog(routine) << "Crystal outer radius: " << outerRadius/cm << " cm" << endl;
  MGLog(routine) << "Crystal bulletization radius: " << bulletizationRadius/cm << " cm" << endl;
  MGLog(routine) << "Contact length: " << contactLength/cm << " cm, radius: " << contactRadius/cm << " cm" << endl;
  MGLog(routine) << "Central hole length: " << centralHoleLength/cm << " cm, radius: " << centralHoleRadius/cm << " cm" << endl;

  geometryFile.close();
  if( MGLogger::GetSeverity() <= MGLogger::routine ) crystalData->Dump();
  return crystalData;
} 

// -----------------------------------------------------------------------

/* InitializeNewSymmetricGeometry() 
 * Reads information about symmetric crystal geometry from file given by
 * geometryFileName. Returns pointer to MGCrystalData class. */

MGCrystalData* MGSiggenUtils::InitializeNewSymmetricGeometry( const char* geometryFileName, int crystalID,
                                                              MGCrystalData::EDiodeType diodeType, MGCrystalData::EGeometryType geomType, 
                                                              double globalX, double globalY, double globalZ, double wrapAroundDepth )
{
  MGCrystalData* crystalData = new MGCrystalData( crystalID, diodeType, geomType, wrapAroundDepth );
  crystalData->SetGlobalPosition( globalX, globalY, globalZ );

  ifstream geometryFile(geometryFileName);
  MGLog(routine) << "Opening symmetric crystal geometry file: " << geometryFileName << endlog; 
  if(!geometryFile.good()){
    MGLog(error) << "Failed to open symmetric geometry configuration file: " << geometryFileName << endlog;
    return crystalData;
  } 

  // segmentation data
  size_t nZ = 0, nPhi = 0;
  if( sscanf(ReadSetupLine(geometryFile).c_str(), "%lu %lu", &nZ, &nPhi) != 2 ){
    MGLog(error) << "Failed to read number of segments from " << geometryFileName << endlog;
    return crystalData;
  }

  string segmentHeightsLine = ReadSetupLine(geometryFile);
  if( segmentHeightsLine == "" ){
    MGLog(error) << "Failed to read segment thickness info from file: " << geometryFileName
                 << endlog;
    return crystalData;
  }
  // thickness of segments, front to back (z direction); also known as "segment height"
  vector<double> segmentHeights;
  stringstream segmentHeightsStream(segmentHeightsLine);
  for(int i = 0; i < int(nZ); i++){
    double segmentThickness;
    segmentHeightsStream >> segmentThickness;
    segmentThickness *= mm;
    segmentHeights.push_back(segmentThickness);
  }

  // crystal geometry data
  double crystalHeight = 0.0;
  if( sscanf(ReadSetupLine(geometryFile).c_str(), "%lf", &crystalHeight) != 1 ){
    MGLog(error) << "Failed to read crystal length (height) from file: " << geometryFileName
                 << endlog;
    return crystalData;
  }
  crystalHeight *= mm; // read in as mm (GRETINA convention)

  double outerRadius = 0.0; 
  if( sscanf(ReadSetupLine(geometryFile).c_str(), "%lf", &outerRadius) != 1 ){
    MGLog(error) << "Failed to read crystal outer radius from file: " << geometryFileName
                 << endlog;
    return crystalData;
  }
  outerRadius *= mm; 

  double innerRadius = 0.0;
  if( sscanf(ReadSetupLine(geometryFile).c_str(), "%lf", &innerRadius) != 1 ){
    MGLog(error) << "Failed to read hole radius (inner radius) from file: " << geometryFileName
                 << endlog;
    return crystalData;
  }
  innerRadius *= mm; 

  double holeStartZ = 0.0; // This variable holeStartZ is defined to be consistent with the GRETINA 
                           // geometry file format. It is the z-coordinate of the top of the crystal well.
  if( sscanf(ReadSetupLine(geometryFile).c_str(), "%lf", &holeStartZ) != 1 ){
    MGLog(error) << "Failed to read z-coordinate for start of central hole from file: "
                 << geometryFileName << endlog;
    return crystalData;
  }
  holeStartZ *= mm; 
  double wellDepth = crystalHeight/2.0 + holeStartZ;

  double taperVertex = 0.0;
  if ( sscanf(ReadSetupLine(geometryFile).c_str(), "%lf", &taperVertex) != 1 ){
    MGLog(error) << "Failed to read taper vertex position from file: " << geometryFileName
                 << endlog;
    return crystalData;
  }
  taperVertex *= mm; 

  double taperAngle = 0.0; 
  if ( sscanf(ReadSetupLine(geometryFile).c_str(), "%lf", &taperAngle) != 1 ){
    MGLog(error) << "Failed to read taper angle from file: " << geometryFileName 
                 << endlog;
    return crystalData;
  }
  taperAngle *= degree; // read in as degrees (GRETINA convention)

  crystalData->SetSegmentation( nZ, nPhi );
  crystalData->SetCrystalDimensions( innerRadius, outerRadius, crystalHeight, wellDepth,
                                    wrapAroundDepth, taperVertex, taperAngle );
  crystalData->SetSegmentHeights(segmentHeights);
  MGLog(routine) << "Segments in z-direction: " << nZ << ", phi: " << nPhi << endl;
  MGLog(routine) << "Segment thicknesses, z direction: ";                    
  for(int i = 0; i < int(nZ); i++){
    cout << segmentHeights[i]/cm << " cm  ";
  } 
  cout << endl;
  MGLog(routine) << "Crystal length (height): " << crystalHeight/cm << " cm" << endl;
  MGLog(routine) << "Crystal outer radius: " << outerRadius/cm << " cm" << endl;
  MGLog(routine) << "Crystal hole radius (inner radius): " << innerRadius/cm << " cm" << endl;
  MGLog(routine) << "Central hole starts at z = " << (-crystalHeight/2.0 + wellDepth)/cm << " cm" << endl;
  MGLog(routine) << "Taper vertex distance from detector front: " << taperVertex/cm << " cm" << endl;
  MGLog(routine) << "Taper angle, degrees: " << taperAngle/degree << endl;

  geometryFile.close();
  if( MGLogger::GetSeverity() <= MGLogger::routine ) crystalData->Dump();
  return crystalData;
} 


