/* TestSiggenUtils - test script for MGSiggenUtils.
 * AUTHORS: Khang Nguyen & Jason Detwiler
 * CONTACT: khangdnguyen@lbl.gov, jadetwiler@lbl.gov
 * LAST MODIFIED: Wednesday, January 12, 2011
 */

#include <iostream>
#include <fstream>
#include <string.h>

#include "waveform/MGSiggenUtils.hh"

using namespace std;

int main()
{
  cout << "=======================================================" << endl;
  cout << "          Testing ReadSetupLine() function...          " << endl;
  cout << "=======================================================" << endl;
  ifstream file("/Users/khangnguyen/Desktop/NEWRadfordCode/siggen/trunk/geometry_setup_symm.dat");
  string line = MGSiggenUtils::ReadSetupLine(file);   
  while(line != ""){
     cout << line << endl;
     line = MGSiggenUtils::ReadSetupLine(file);
  }
  file.close();

  cout << "=============================================================" << endl;
  cout << "         Testing InitializePPCGeometry() function...         " << endl;
  cout << "=============================================================" << endl;
  MGCrystalData* ppcGeometry;
  ppcGeometry = MGSiggenUtils::InitializeNewPPCGeometry("/Users/khangnguyen/Desktop/NEWRadfordCode/siggen/trunk/geometry_setup_ppc.dat", 0);   
  delete ppcGeometry;


  cout << "=============================================================" << endl;
  cout << "      Testing InitializeSymmetricGeometry() function...      " << endl;
  cout << "=============================================================" << endl;
  MGCrystalData* symmGeometry;
  symmGeometry = MGSiggenUtils::InitializeNewSymmetricGeometry( "/Users/khangnguyen/Desktop/NEWRadfordCode/siggen/trunk/geometry_setup_symm.dat", 
                                                                0, MGCrystalData::kPType, MGCrystalData::kClosedEnd, 0.0, 0.0, 0.0, 0.0 );   
  delete symmGeometry;

  return 0;
}

