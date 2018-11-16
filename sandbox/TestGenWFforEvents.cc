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
/*
 * DESCRIPTION:
 *      Program for generating waveforms according to energy deposits from simulation.
 *
 * AUTHOR:
 *      Alex Volynets
 *
 * REVISION: MM-DD-YYYY
 *
 *      29-03-2010, Created, Alex
 *      02-10-2011, Restructured. Creating crystal and main objects are now
 *                  handled in the program, not in MGGenWFForEvents.   Alex
 *
 */

#include "io/MGLogger.hh"
#include "waveform/MGBruyneelDriftVelocity.hh"
#include "waveform/MGGenWFForEvents.hh"
#include "waveform/MGWFGenFromDrift.hh"

using namespace std;
using namespace CLHEP;

int main(int argc, char** argv)
{
  if (argc!=2)
  {
    cout<<"Usage: "<<argv[0]<<" [ini file]. Exiting..."<<endl;
    return -1;
  }
  MGLogger::SetSeverity(MGLogger::routine);
  //MGLogger::SetSeverity(MGLogger::debugging);

  MGGenWFForEvents *myWFgen = new MGGenWFForEvents();
  myWFgen->ReadSetupDataFromFile(argv[1]);

  cout<<" ----- Here comes the Waveform part ----- "<<endl;

  cout<<"Setup Waveform Manager"<<endl;
    MGWaveformWriterROOT* wfWriter = new MGWaveformWriterROOT();
    myWFgen->SetWaveformWriter(wfWriter);

    MGWaveformManager* wfManager = new MGWaveformManager(wfWriter);
    myWFgen->SetWaveformManager(wfManager);
    wfManager->SetCoincidenceTime(1*ns);
    wfManager->SetCoincidenceLength(1*mm);
  cout<<"Setup Waveform Manager - DONE"<<endl;

  cout<<"Setup Crystal Data"<<endl;
    int id = 0;
    double ri = 5.0*mm, ro = 37.5*mm, hi = 70.0*mm;
    MGCrystalData* crystal = new MGCrystalData(
         id,            // crystal ID
         MGCrystalData::kNType,
         MGCrystalData::kCoaxial,
         ri,            // inner radius
         ro,            // outer radius
         hi,            // height
         hi,            // well depth
         0.0,           // global x
         0.0,           // global y
         0.0,           // global z
         3,             // Nseg in z
         6);            // Nseg in phi

    crystal->SetCrystalAxesAngle(0.0*CLHEP::degree);
    crystal->SetSegmentationAngle(0.0*CLHEP::degree);
    myWFgen->SetCrystal(crystal);
  cout<<"Setup Crystal Data - DONE"<<endl;

  cout<<"Setup Crystal Fields"<<endl;
    myWFgen->InitCrystalFields();
  cout<<"Setup Crystal Fields - DONE"<<endl;

  cout<<"Setup Drift Velocity"<<endl;
    MGBruyneelDriftVelocity drift(*myWFgen->GetCrystalFields());
  cout<<"Setup Drift Velocity - DONE"<<endl;

  cout<<"Setup Waveform Generator"<<endl;
    MGWFGenFromDrift* wfGenerator = new MGWFGenFromDrift(drift, 1000,1*CLHEP::GHz,100*ns,2.95*eV, MGWaveform::kCharge);
//    MGWFGenFromDrift* wfGenerator = new MGWFGenFromDrift(crystal,myWFgen->GetCrystalFields(), 200, 100*CLHEP::MHz,10*ns,2.95*eV, MGWaveform::kCharge);
    myWFgen->SetWaveformGenerator(wfGenerator);
    wfGenerator->SetPrecision(2);
    wfGenerator->SetSaveOptions( MGWFGenFromDrift::kSaveHits | MGWFGenFromDrift::kSaveEndPoints | MGWFGenFromDrift::kSaveWaveforms );
  cout<<"Setup Waveform Generator - DONE "<<endl;

  cout<<"Adding Waveform Generator to Manager"<<endl;
    wfManager->AddWaveformGenerator(wfGenerator);
  cout<<"Adding Waveform Generator to Manager - DONE"<<endl;

  cout<<"Set output tree and file"<<endl;
    myWFgen->SetOutput();
  cout<<"Set output tree and file - DONE"<<endl;

//  cout<<(MGWFGenFromDrift::kSaveHits|MGWFGenFromDrift::kSaveEndPoints)<<endl;
//  myWFgen->GetWaveformGenerator()->SetSaveOptions((unsigned char)(MGWFGenFromDrift::kSaveHits | MGWFGenFromDrift::kSaveEndPoints));

/*
// This is for generating events homogeniously
  myWFgen->GenerateHomogeniousHits();
  myWFgen->Finalize();
  return 1;
*/
// This is for simulating WFs based on simulated hits
  //
  // Now commented since this case is not used now
  //

  myWFgen->OpenTree();
  myWFgen->ProcessEvents();
  myWFgen->Finalize();

/*
  //
  // For tests of the output tree
  //

  TApplication fA("test",&argc,argv);
  TBrowser *fB = new TBrowser("testB",fOut);
  fA.Run();
  fOut->Close();
  delete fOut;
*/
   return 1;
}
