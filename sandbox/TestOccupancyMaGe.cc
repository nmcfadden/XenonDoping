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
 * DESCRIPTION:
 *      Calculate end points of holes (occupancy) for a set of hits from a MaGe .root file.
 *      Very similar to TestGenWFforEvents.cc
 *
 * AUTHOR:
 *      Alex Volynets
 *
 * REVISION: MM-DD-YYYY
 *
 *      07-05-2011, Created, Alex
 *
 */

#include "io/MGLogger.hh"
#include "waveform/MGBruyneelDriftVelocity.hh"
#include "waveform/MGGenWFForEvents.hh"
#include "waveform/MGWFGenFromDrift.hh"
#include "waveform/MGWaveformManager.hh"
#include "TSystem.h"

#include "stdlib.h"
using namespace std;
using namespace CLHEP;

int main(int argc, char** argv)
{
  // Reading parameters
  if (argc<4)
  {
    MGLog(error)<<"Too few arguments! Usage: "<<argv[0]<<" [ElectricFieldFilename] [InputFilenameWithHits] [OutputDir]"<<endlog;
    exit(0);
  }
  char* fieldfilename = argv[1];
  char* inputfilename = argv[2];
  char* outputdir = argv[3];

  MGLogger::SetSeverity(MGLogger::routine);
  //MGLogger::SetSeverity(MGLogger::debugging);

  MGGenWFForEvents *myWFgen = new MGGenWFForEvents();
  myWFgen->SetFieldsFileName(fieldfilename);
  myWFgen->SetEventFileName(inputfilename);

  TString temp = gSystem->BaseName(inputfilename);
  TString outputfilename = TString(temp, temp.Index(".root"));
  myWFgen->SetOutputFileName(Form("%s/Occ_%s.root", outputdir, outputfilename.Data()));

  cout<<" ----- Here comes the Waveform part ----- "<<endl;

  cout<<"Setup Waveform Manager"<<endl;
    MGWaveformWriterROOT* wfWriter = new MGWaveformWriterROOT();
    myWFgen->SetWaveformWriter(wfWriter);

    MGWaveformManager* wfManager = new MGWaveformManager(wfWriter);
    myWFgen->SetWaveformManager(wfManager);
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
    myWFgen->SetWaveformGenerator(wfGenerator);

    // Use Runge-Kutta method
    wfGenerator->SetPrecision(2);

    // Specify to save hits and end points of the holes
    wfGenerator->SetSaveOptions( MGWFGenFromDrift::kSaveHits | MGWFGenFromDrift::kSaveEndPointsHoles );
  cout<<"Setup Waveform Generator - DONE "<<endl;

/*
  cout<<"Adding Waveform Generator to Manager"<<endl;
    wfManager->AddWaveformGenerator(wfGenerator);
  cout<<"Adding Waveform Generator to Manager - DONE"<<endl;
*/

  cout<<"Set output tree and file"<<endl;
    myWFgen->SetOutput();
  cout<<"Set output tree and file - DONE"<<endl;

  myWFgen->OpenTree();

  cout<<"Calculate occupancy now"<<endl<<endl;
    myWFgen->Occupancy();
  cout<<"Calculate occupancy now - DONE"<<endl<<endl;

  myWFgen->Finalize();

   return 1;
}
