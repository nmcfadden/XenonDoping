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
 *      Test code for MGSiggenDriftVelocityTable.cc.  Must run from the
 *      directory containing the configuration files. Please contact the authors
 *      to obtain the files.
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


#include "io/MGLogger.hh"
#include "waveform/MGM3dcrPPCCrystalFields.hh"
#include "waveform/MGSiggenDriftVelocityTable.hh"
#include "waveform/MGSiggenUtils.hh"

using namespace std;
using namespace CLHEP;

int main()
{
  MGLogger::SetSeverity(MGLogger::debugging);
  //MGLogger::SetSeverity(MGLogger::routine);

  cout << "============================================================" << endl;
  cout << "           Testing GetDriftVelocity() function...           " << endl;
  cout << "============================================================" << endl;
  MGCrystalData* ppcGeometry;
  ppcGeometry = MGSiggenUtils::InitializeNewPPCGeometry("geometry_setup_ppc.dat", 0);
  MGM3dcrPPCCrystalFields* crystalFields = new MGM3dcrPPCCrystalFields(*ppcGeometry);
  crystalFields->SetFieldsFileName("field_setup_ppc.dat");
  crystalFields->LoadFields();


  MGSiggenDriftVelocityTable* velocity = new MGSiggenDriftVelocityTable(*crystalFields);
  velocity->LoadFieldsFile("field_setup_ppc.dat");

  Hep3Vector coordinates;
  //coordinates.setRhoPhiZ(0.0, 0.0, -23.0);
  //coordinates.setRhoPhiZ(0.0, 0.0, 5.0*mm); // look at (0.0, 0.0, 28.0*mm) in table, line number: 282
  //coordinates.setRhoPhiZ(0.0, 0.0, 7.0*mm); // look at (0.0, 0.0, 30.0*mm) in table, line number: 302
  //coordinates.setRhoPhiZ(1*mm, 0.0, 1.5*mm); // look at (1*mm, 0.0, 24.5*mm) in table, line number: 4867
  //coordinates.setRhoPhiZ(2.7*mm, 0.0, 38.6*mm); // look at line number: 12862 in table
  coordinates.setRhoPhiZ(2.7*mm, 0.0, 15.6*mm); // look at (2.7*mm, 0.0, 38.6*mm) in table, line number: 12862
  //coordinates.set(27.5*mm, 0.0, 5.0*mm);  // look at line number: 127102 in table
  //coordinates.setRhoPhiZ(30.8*mm, 0.0, 23.0*mm); // look at (30.8*mm, 0.0, 23.0*mm) in table, line number: 142758
  //coordinates.setRhoPhiZ(30.9*mm, 0.0, 11.2*mm); // look at (30.9*mm, 0.0, 34.2*mm) in table, line number: 143102
  //coordinates.setRhoPhiZ(30.9*mm, 0.0, 23.0*mm); // look at line number: 143220 in table
  //coordinates.setRhoPhiZ(31.0*mm, 0.0, 0.0*mm); // look at line number: 143222 in table
  //coordinates.setRhoPhiZ(31.0*mm, 0.0, -22.0*mm); // look at (31.0*mm, 0.0, 1.0*mm) in table, line number: 143232
  double rho = coordinates.getRho();
  double phi = coordinates.getPhi();
  double z = coordinates.getZ();
  double x = coordinates.getX();
  double y = coordinates.getY();


  Hep3Vector driftVelocity = velocity->GetDriftVelocity(coordinates, eplus);
  double dZ = driftVelocity.getZ();
  double dX = driftVelocity.getX();
  double dY = driftVelocity.getY();
  cout << "Rho value is: " << rho << endl;
  cout << "Phi value is: " << phi << endl;
  cout << "Z value is: " << z << endl;
  cout << "Coordinates (in Cylindrical) used are: (" << rho << ", " << phi << ", " << z << ")." << endl; 
  cout << "Coordinates (in Cartesian) used are: (" << x << ", " << y << ", " << z << ")." << endl; 
  cout << "Hep3Vector of calculated drift velocity at given Cartesian point is: (" 
       << dX/(mm/ns) << " mm/ns, " << dY/(mm/ns) << " mm/ns, " << dZ/(mm/ns) << " mm/ns)." << endl;

  delete velocity;
  delete crystalFields;
  delete ppcGeometry;

  return 0;
}
