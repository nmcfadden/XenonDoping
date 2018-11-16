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
 *      This module uses a look-up table to compute drift velocities.
 *      The code is based on fields_ppc.c by Karin Lagergren.
 *
 * AUTHOR:
 *      Khang Nguyen & Jason Detwiler
 *
 * CONTACT:
 *      khangdnguyen@lbl.gov, jadetwiler@lbl.gov
 *
 * FIRST SUBMISSION:
 *      03-26-2011 
 *
 * REVISION: MM-DD-YYYY
 *      05-26-2011: Use MGVDriftVelocity base class, J. Detwiler
 *      05-28-2011: Adapt to const protections and const& storage of
 *                  MGVCrystalFields in base class. J. Detwiler
 *
 *
 */


#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "io/MGLogger.hh"
#include "waveform/MGSiggenUtils.hh"
#include "waveform/MGSiggenDriftVelocityTable.hh"

using namespace std;
using namespace CLHEP;

void MGSiggenDriftVelocityData::CalculateAnisotropyCorrections()
{
  double sumBp = 0., sumCp = 0.;
  for(size_t i=0; i<fData.size(); i++) {
    MGSiggenDriftVelocityPoint& p = fData[i];
    double a =  0.5 * p.Get100() -  4.0 * p.Get110() +  4.5 * p.Get111();
    double b = -2.5 * p.Get100() + 16.0 * p.Get110() - 13.5 * p.Get111();
    double c =  3.0 * p.Get100() - 12.0 * p.Get110() +  9.0 * p.Get111();
    fData[i].SetABC(a, b, c);

    if(i == 0) fData[i].SetBpCp(0., 0.);
    else {
      MGSiggenDriftVelocityPoint& p0 = fData[i-1];
      double dE = p.GetE() - p0.GetE();
      double bAve = (p0.GetB() + p.GetB())/2.;
      double cAve = (p0.GetC() + p.GetC())/2.;
      sumBp += dE*bAve;
      sumCp += dE*cAve;
      fData[i].SetBpCp(sumBp/p.GetE(), sumCp/p.GetE());
    }
  }
}

double MGSiggenDriftVelocityData::Interpolate(MGSiggenDriftVelocityPoint::GetFunction getFunc, double eField) const
{
  // must have more than 1 point to interpolate
  if(fData.size() <= 1){
    MGLog(error) << "Interpolate(): Size of drift velocity vector is <= 1" << endlog;
    return 0.;
  }

  // don't accept negative E-field strengths
  if(eField < 0){
    MGLog(error) << "Interpolate(): Given electric field is negative!" << endlog;
    return 0.0;
  }

  // cache for use by both holes and electrons by making these static
  // WARNING! If this class is ever changed so that holes and electrons use
  // different tables with different E-spacings, these variables will need to
  // be made non-static and made into class variables!
  static double lastE = -1.0;
  static size_t i = 0;
  static double x = 0.0;

  // cache interpolation calculations for speed
  if(eField != lastE) {
    // chances are that eField and lastEField are close. 
    // Search backwards then forwards from i.
    while(i > 0 && fData[i].GetE() > eField) i--;
    while(i+2 < fData.size() && fData[i+1].GetE() < eField) i++;

    // now recalculate x. E[i] and E[i+1] are valid at this point
    // Note: 0 < x < 1 for E within table, but not at the edges.
    // Allowing x < 0 for E < E[0] and x > 1 for E > E[size-1] gives linear
    // extrapolation at the range edges based on the nearest two data points.
    x = (eField - fData[i].GetE())/(fData[i+1].GetE() - fData[i].GetE());

    lastE = eField;
  }

  // Linear interpolation 
  double b = (fData[i].*getFunc)();
  double slope = (fData[i+1].*getFunc)() - b;
  return slope*x + b;
}

MGSiggenDriftVelocityTable::MGSiggenDriftVelocityTable(const MGVCrystalFields& fields) : 
  MGVDriftVelocity(fields), fTemperature(77.*CLHEP::kelvin)
{
  ReadDriftVelocity();
}

/* LoadFieldsFile()
 * Reads in siggen-format field_setup file, which specifies in addition to the
 * fields the crystal temperature and the location of the drift velocity table.
 * We really only take the temperature from this file because the table is
 * expected to be found in $MAGEDIR/waveform/drift_vel_tcorr.dat
 */

void MGSiggenDriftVelocityTable::LoadFieldsFile(const char* fieldsFileName)
{
  ifstream fieldsFile(fieldsFileName);
  MGLog(routine) << "Reading field data from file: " << fieldsFileName << endlog;
  if(!fieldsFile.good()){
    MGLog(error) << "Failed to open field init file: " << fieldsFileName << endlog;
    return;
  }

  // min, max, and step (r direction) - just skip over it, but give a warning if not found
  double rMin, rMax, rStep;
  if ( sscanf(MGSiggenUtils::ReadSetupLine(fieldsFile).c_str(), "%lf %lf %lf", &rMin, &rMax, &rStep) != 3 ){
    MGLog(warning) << "Limits for r and number of r steps are not specified properly in file: "
                   << fieldsFileName << endlog;
  }

  // min, max, and step (z direction) - just skip over it, but give a warning if not found
  double zMin, zMax, zStep;
  if( sscanf(MGSiggenUtils::ReadSetupLine(fieldsFile).c_str(), "%lf %lf %lf", &zMin, &zMax, &zStep) != 3 ){
    MGLog(warning) << "Limits for z and number of z steps are not specified properly in file: " 
                   << fieldsFileName << endlog;
  }

  // temperature
  double temperature;
  if( sscanf(MGSiggenUtils::ReadSetupLine(fieldsFile).c_str(), "%lf", &temperature) != 1 ){
    MGLog(error) << "Failed to read temperature from file: " << fieldsFileName << endlog;
    fieldsFile.close();
    return;
  }
  temperature *= kelvin; // read in as degrees Kelvin (GRETINA convention)
  SetTemperature(temperature);

  // drift velocity lookup table
  string driftVelocityFileName = MGSiggenUtils::ReadSetupLine(fieldsFile);
  if( driftVelocityFileName == "" ){
    MGLog(error) << "Failed to read drift velocity data from file: " << driftVelocityFileName << endlog;
    fieldsFile.close();
    return;
  }
  // Make sure the file is the same as the one found in MaGe (checking just the
  // name for now)
  if(driftVelocityFileName != "drift_vel_tcorr.dat") {
    MGLog(error) << "Unrecognized drift velocity file name: " << driftVelocityFileName << endlog;
    fieldsFile.close();
    return;
  }

  // electric field data - just skip over it, but give a warning if not found
  string eFieldLine = MGSiggenUtils::ReadSetupLine(fieldsFile);
  if( eFieldLine == "" ){
    MGLog(warning) << "Location of electric field data is not specified!" << endlog;
  }

  // weighting potential data - just skip over it, but give a warning if not found
  string wPotentialLine = MGSiggenUtils::ReadSetupLine(fieldsFile);
  if( wPotentialLine == "" ){
    MGLog(warning) << "Location of weighting potential data is not specified!" << endlog;
  }
 
  fieldsFile.close();
  return;
}

// -----------------------------------------------------------------------

/* ReadDriftVelocity()
 * Set up drift velocity calculations (read in table).
 * table is found in $MAGEDIR/waveform/drift_vel_tcorr.dat
 */

void MGSiggenDriftVelocityTable::ReadDriftVelocity()
{
  const char* magedir = getenv("MAGEDIR");
  if(magedir == NULL) {
    MGLog(error) << "Couldn't access $MAGEDIR..." << endl;
    return;
  }
  string driftVelocityFileName = magedir;
  driftVelocityFileName += "/waveform/drift_vel_tcorr.dat";
  ifstream driftVelocityFile(driftVelocityFileName.c_str());
  if(!driftVelocityFile.good()){
    MGLog(error) << "Failed to read drift velocity lookup table from file: " << driftVelocityFileName << endlog;
    driftVelocityFile.close();
    return;
  }

  MGLog(trace) << "Reading drift velocity table from file: " << driftVelocityFileName << endlog;
  MGLog(debugging) << "----------------------------- Drift velocity table ------------------------------" << endlog; 
  MGLog(debugging) << "   e     e100     e110     e111     h100     h110     h111     ecorr     hcorr   " << endlog;    
  MGLog(debugging) << "---------------------------------------------------------------------------------" << endlog; 
  
  size_t lineNumber = 0;
  double eField = 0.0, e100 = 0.0, e110 = 0.0, e111 = 0.0;
  double h100 = 0.0, h110 = 0.0, h111 = 0.0;
  double eCorr = 0.0, hCorr = 0.0;  
  double lastEField = -1.0;
  MGSiggenDriftVelocityPoint driftVelocityPoint;
  while(1){
    string line = MGSiggenUtils::ReadSetupLine(driftVelocityFile);
    if(line == "") break;
    lineNumber++;
    if( sscanf(line.c_str(), "%lf" "%lf" "%lf" "%lf" "%lf" "%lf" "%lf" "%lf" "%lf", 
                             &eField, &e100, &e110, &e111, &h100, &h110, &h111, &eCorr, &hCorr) != 9 ){
      if( sscanf(line.c_str(), "%lf" "%lf" "%lf" "%lf" "%lf" "%lf" "%lf", 
                               &eField, &e100, &e110, &e111, &h100, &h110, &h111) != 7 ) break;  // assume end of file
      else{
        MGLog(routine) << "Setting tcorr (temperature correction) terms to 0." << endlog;
        eCorr = hCorr = 0.0; 
      }
    }
    eField *= volt/cm;
    e100 *= mm/ns;
    e110 *= mm/ns;
    e111 *= mm/ns;
    h100 *= mm/ns;
    h110 *= mm/ns;
    h111 *= mm/ns;
    eCorr *= 1.0/kelvin;
    hCorr *= 1.0/kelvin;

    if(eField < lastEField){
      MGLog(warning) << "The electric field values being read in may not be in increasing order!" << endlog;
    }
    lastEField = eField; 
 
    // set drift velocity values
    driftVelocityPoint.SetBasicData(eField, e100, e110, e111, eCorr);
    fEData.AddDatum(driftVelocityPoint);

    driftVelocityPoint.SetBasicData(eField, h100, h110, h111, hCorr);
    fHData.AddDatum(driftVelocityPoint);

    MGLog(debugging) << eField/(volt/cm) << "    " << e100/(mm/ns) << "    " << e110/(mm/ns) << "    " << e111/(mm/ns)
                     << "    " << h100/(mm/ns) << "    "  << h110/(mm/ns) << "    " << h111/(mm/ns) 
                     << "    " << eCorr << "    " << hCorr << endlog; 
  }
  MGLog(trace) << "Done reading " << lineNumber << " lines of drift velocity data." << endlog;
  driftVelocityFile.close();

  fEData.CalculateAnisotropyCorrections();
  fHData.CalculateAnisotropyCorrections();

  return;
}

// -----------------------------------------------------------------------

/* GetMobilityVector()
 * Get the mobilities in the x, y and z direction by locating given 
 * electric field in drift velocity table. Interpolation or 
 * extrapolation if necessary.
 */

Hep3Vector MGSiggenDriftVelocityTable::GetMobilityVector( const double eField, double charge ) const
{
  const MGSiggenDriftVelocityData* data = &fHData;
  if(charge < 0) data = &fEData;
  Hep3Vector mobility( data->Interpolate(&MGSiggenDriftVelocityPoint::Get100, eField), 
                       data->Interpolate(&MGSiggenDriftVelocityPoint::Get110, eField), 
                       data->Interpolate(&MGSiggenDriftVelocityPoint::Get111, eField));
  return mobility;
}

// -----------------------------------------------------------------------

/* GetDriftVelocity()
 * Calculate drift velocity for given charge at specified point.
 */

Hep3Vector MGSiggenDriftVelocityTable::GetDriftVelocity(const Hep3Vector& eField, double charge) const
{
  double eFieldMag = eField.mag();

  // v ~ E so if E = 0, v = E, so just return it
  if(eFieldMag == 0.0) return eField; 

  // FIXME: this cacluation assumes an orientation for x, y, z
  if(fCrystalFields->GetCrystalData().GetCrystalAxesAngle() != 0.0) {
    MGLog(warning) << "GetDriftVelocity(): crystal axes angle != 0.0 is not implemented." << endlog;
  }

  Hep3Vector vd = eField.unit();
  double eNormX = vd.x();
  double eNormY = vd.y();
  double eNormZ = vd.z();
  double eNorm4 = pow(eNormX, 4)  + pow(eNormY, 4) + pow(eNormZ, 4);
  double eNorm6 = pow(eNormX, 6)  + pow(eNormY, 6) + pow(eNormZ, 6);

  const MGSiggenDriftVelocityData* data = &fHData;
  if(charge < 0) data = &fEData;
  double a = data->Interpolate(&MGSiggenDriftVelocityPoint::GetA, eFieldMag);
  double b = data->Interpolate(&MGSiggenDriftVelocityPoint::GetB, eFieldMag);
  double c = data->Interpolate(&MGSiggenDriftVelocityPoint::GetC, eFieldMag);
  double bp = data->Interpolate(&MGSiggenDriftVelocityPoint::GetBp, eFieldMag);
  double cp = data->Interpolate(&MGSiggenDriftVelocityPoint::GetCp, eFieldMag);
  
  // approximation: velocity is assumed to be in the direction of electric field 
  double vMag = a + b * eNorm4 + c * eNorm6;

  double sign = ( charge < 0 ? -1 : 1 );
  double tCorr = GetTemperatureCorrection( eFieldMag, charge );
  vd.setX(sign*tCorr*eNormX * (vMag + 4.*bp*(eNormX*eNormX-eNorm4) + 6.*cp*(pow(eNormX,4)-eNorm6)));
  vd.setY(sign*tCorr*eNormY * (vMag + 4.*bp*(eNormY*eNormY-eNorm4) + 6.*cp*(pow(eNormY,4)-eNorm6)));
  vd.setZ(sign*tCorr*eNormZ * (vMag + 4.*bp*(eNormZ*eNormZ-eNorm4) + 6.*cp*(pow(eNormZ,4)-eNorm6)));

  return vd;
}

// -----------------------------------------------------------------------

/* SetTemperature()
 * Set temperature and check if within range of min and max temperatures as 
 * defined in header (.hh) file.
 */

void MGSiggenDriftVelocityTable::SetTemperature(double temperature)
{
  const double kMinTemp = 77.*kelvin;
  const double kMaxTemp = 110.*kelvin;
  if( temperature < kMinTemp || temperature > kMaxTemp ){
    MGLog(error) << "Temperature (=" << temperature/kelvin << " Kelvin) is out of range!" << endlog; 
    return;
  }   

  fTemperature = temperature;
  MGLog(trace) << "Temperature is set to: " << temperature/kelvin << " Kelvin." << endlog;
  
  return;
}

// -----------------------------------------------------------------------

/* GetTemperatureCorrection()
 * Calculate temperature correction for given charge and electric field magnitude.
 */

double MGSiggenDriftVelocityTable::GetTemperatureCorrection( const double eField, double charge ) const
{
  const MGSiggenDriftVelocityData* data = &fHData;
  if(charge < 0) data = &fEData;
  double tCorr = data->Interpolate(&MGSiggenDriftVelocityPoint::GetTCorr, eField);
  const double kRefTemp = 77.*kelvin;
  double dT = fTemperature - kRefTemp;
  return 1. + tCorr*dT/kelvin/100.; 
}

