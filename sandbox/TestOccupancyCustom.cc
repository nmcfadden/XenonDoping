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
 *   Calculate end points of holes (occupancy) for a custom set of hits
 *
 * AUTHOR:
 *   Alex Volynets
 *
 * REVISION:
 *      07-08-2011, Created, Alex
 *
 */
#include "TMath.h"

#include "io/MGLogger.hh"
#include "waveform/MGBruyneelDriftVelocity.hh"
#include "waveform/MGWFGenFromDrift.hh"
#include "waveform/MGSORCrystalFields.hh"
#include "waveform/MGWaveformWriterROOT.hh"

#include "MGCrystalData.hh"
#include "MGCrystalHits.hh"

using namespace std;
using namespace CLHEP;

int main()
{
  MGLogger::SetSeverity(MGLogger::routine);
  //MGLogger::SetSeverity(MGLogger::debugging);

  cout<<"Setup Crystal Data"<<endl;
    int id = 0;
    double ri = 5.0*mm, ro = 37.5*mm, hi = 70.0*mm;
    MGCrystalData* crystal = new MGCrystalData(
      id,  // crystal ID
      MGCrystalData::kNType,
      MGCrystalData::kCoaxial,
      ri,  // inner radius
      ro,  // outer radius
      hi,  // height
      hi,  // well depth
      0.0, // global x
      0.0, // global y
      0.0, // global z
      3,   // Nseg in z
      6);  // Nseg in phi
    crystal->SetCrystalAxesAngle(0.0*CLHEP::degree);
    crystal->SetSegmentationAngle(0.0*CLHEP::degree);
  cout<<"Setup Crystal Data - DONE\n"<<endl;

  cout<<"Setup Crystal Fields"<<endl;
    MGSORCrystalFields* fields = new MGSORCrystalFields(*crystal);
    fields->SetFileName("/home/Work/MPI/mage/wp/Efield2kV33x181x71_0.63_0.63");
    fields->LoadFields();
  cout<<"Setup Crystal Fields - DONE\n"<<endl;

  cout<<"Setup Drift Velocity"<<endl;
    MGBruyneelDriftVelocity drift(*fields);
  cout<<"Setup Drift Velocity - DONE\n"<<endl;

  cout<<"Setup Waveform Writer"<<endl;
    MGWaveformWriterROOT* wfWriter = new MGWaveformWriterROOT();
  cout<<"Setup Waveform Writer - DONE"<<endl;

  cout<<"Setup output tree and file"<<endl;
    TFile *fOut = new TFile("testOcc.root","recreate");
    TTree *tOut = new TTree("wfTree","wfTree");
    wfWriter->SetFile(fOut);
    wfWriter->SetTree(tOut);
  cout<<"Setup output tree and file - DONE\n"<<endl;

  cout<<"Setup Waveform Generator"<<endl;
    MGWFGenFromDrift* wfGen = new MGWFGenFromDrift(drift,7000,10*CLHEP::GHz,10*ns,2.95*eV, MGWaveform::kCharge);
    wfGen->SetPrecision(2);

    // Specify to save hits and end points of the holes
    wfGen->SetSaveOptions( MGWFGenFromDrift::kSaveHits | MGWFGenFromDrift::kSaveEndPointsHoles );
    wfGen->SetExtraBranches(wfWriter);
  cout<<"Setup Waveform Generator - DONE\n"<<endl;


  cout<<"Setup Crystal Hits"<<endl;
    MGCrystalHits hits;
  cout<<"Setup Crystal Hits - DONE\n"<<endl;

  cout<<"Calculate occupancy event by event:"<<endl;

  int Nhits = 36;
  cout<<"Total hits: "<<Nhits<<endl;
  float dphi=TMath::Pi()/Nhits;

  for (int i=0; i<Nhits; i++) {
    cout<<"Processing hit "<<i<<endl; 
    float phi=dphi*i;
    float x=(ro-0.1*mm)*cos(phi);
    float y=(ro-0.1*mm)*sin(phi);
    cout<<"hit x, y: "<<x<<" \t "<<y<<endl;
    hits.AddCrystalHit(x,y,0*mm,0*ns,122*CLHEP::keV,id);
  }
  wfGen->CalculateOccupancyForHits(hits);
  tOut->Fill();
  fOut->Write();
  fOut->Close();

  cout<<"Calculate occupancy event by event - DONE"<<endl;

}

