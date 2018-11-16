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
 * DESCRIPTION:
 *
 *
 * AUTHOR:
 *      Alex Volynets
 *
 * FIRST SUBMISSION:
 *      03-29-2010, Created, Alex
 *
 * REVISION: MM-DD-YYYY
 *
 *      10-15-2010. Minor clean-up. Alex
 *      10-02-2011. 1. Restructured. Now creating main objects (generator, writer, crystal etc.)
 *                     are handled in the program, TestGenWFForEvents
 *                  2. Creation of branches for hits migrated to MGWFGenFromDrift
 *                  Alex.
 *      07-08-2012. 1. Occupancy calculation executable class migrated from MGWFlibGenRK4.cc
 *                  2. Doxygen-friendly comments added. Alex
 *      10-19-2011 Added an option to specify the distance for hit combination. Alex
 *
 */

#include "io/MGLogger.hh"
#include "MGCrystalHits.hh"
#include "MGCrystalData.hh"

#include "waveform/MGWaveformManager.hh"
#include "waveform/MGWFGenFromDrift.hh"
#include "waveform/MGSORCrystalFields.hh"
#include "waveform/MGWaveformWriterROOT.hh"
#include "waveform/MGGenWFForEvents.hh"

#include "time.h"

#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TString.h"
#include "TEventList.h"


using namespace std;
using namespace CLHEP;


MGGenWFForEvents::MGGenWFForEvents():
  fNEvents (-1),
  fCombineHitsDistance (1*mm)
{
}

// ----------------------------------------------------------------------------

MGGenWFForEvents::~MGGenWFForEvents() {}

// ----------------------------------------------------------------------------

void MGGenWFForEvents::ReadSetupDataFromFile(char* filename)
{
/**
  Read setup data from file, line by line: \n
   - root file from mage \n
   - electrical field file name, without .root \n
   - output file name \n
   - number of events to process \n
   Additional for generating homogenious hits: \n
   - R: start, end; step \n
   - Phi: start, end; step \n
   - Z: start, end; step \n

*/
  ifstream inp(filename);
  char temp[100];
  inp>>temp>>fEventFileName;
  inp>>temp>>fFieldsFileName;
  inp>>temp>>fOutputFileName;
  inp>>temp>>fNEvents;
  inp>>temp>>fStartR   >>fEndR   >>fNR;
  inp>>temp>>fStartPhi >>fEndPhi >>fNPhi;
  inp>>temp>>fStartZ   >>fEndZ   >>fNZ;

  inp.close();
}

// ----------------------------------------------------------------------------

void MGGenWFForEvents::InitCrystalFields()
{
/**
  Initialize electrical field pointer and load fields

*/

  if (fCrystal==NULL)
  {
    cout<<"Crystal is not defined. Exiting... "<<endl;
    exit(0);
  }
  fFields = new MGSORCrystalFields(*fCrystal);
  fFields->SetFileName(fFieldsFileName.Data());
  cout<<fFieldsFileName.Data()<<endl;
  fFields->LoadFields();
}

// ----------------------------------------------------------------------------

void MGGenWFForEvents::SetOutput()
{
/**
  Create a tree and a file, open the file.\n
  Attach the tree and the file to fWFWriter.\n
  Define additional branches for the generator to store
  hit/end points/trajectories information

*/

  fOutputFile = new TFile(fOutputFileName.Data(),"recreate");
  fOutputTree = new TTree("wfTree","wfTree");

  fWFWriter->SetFile(fOutputFile);
  fWFWriter->SetTree(fOutputTree);

  fWFGenerator->SetExtraBranches(fWFWriter);
}

// ----------------------------------------------------------------------------

void MGGenWFForEvents::OpenTree()
{
/**
  Open the file from mage and set branch addresses. There is commented part
  for various root schemes

*/

  cout<<"Opening event file "<<fEventFileName.Data()<<endl;

  fEventFile = new TFile(fEventFileName.Data(),"read");
  fEventTree = (TTree*)fEventFile->Get("fTree");
//  fEventTree->Show(10);
  if (!fEventTree)
  {
    MGLog(error)<<"Filename with events was not open/found"<<endlog;
    cout<<"Filename with events was not open/found"<<endl;
    return ;
  }
//  Now set the branches
  int nentries = fEventTree->GetEntries();
  if (fNEvents<0 || fNEvents>nentries) fNEvents = nentries;
  cout<<fNEvents<<" events will be processed in the Tree with MaGe events"<<endl;
  fNSelectedEvents = 0;

  fEventTree->SetBranchStatus("*",0);   // set unneeded branches inactive to save time and disk space

  fEventTree->SetBranchStatus("hits_totnum", 1);  fEventTree->SetBranchAddress("hits_totnum", &hits_totnum);
  fEventTree->SetBranchStatus("hits_edep",   1);  fEventTree->SetBranchAddress("hits_edep",    hits_edep);
  fEventTree->SetBranchStatus("hits_xpos",   1);  fEventTree->SetBranchAddress("hits_xpos",    hits_xpos);
  fEventTree->SetBranchStatus("hits_ypos",   1);  fEventTree->SetBranchAddress("hits_ypos",    hits_ypos);
  fEventTree->SetBranchStatus("hits_zpos",   1);  fEventTree->SetBranchAddress("hits_zpos",    hits_zpos);
  fEventTree->SetBranchStatus("hits_time",   1);  fEventTree->SetBranchAddress("hits_time",    hits_time);

/*
  fEventTree->SetBranchStatus("Nhits",  1);  fEventTree->SetBranchAddress("Nhits",  &hits_totnum);
  fEventTree->SetBranchStatus("hitE",   1);  fEventTree->SetBranchAddress("hitE",    hits_edep);
  fEventTree->SetBranchStatus("hitX",   1);  fEventTree->SetBranchAddress("hitX",    hits_xpos);
  fEventTree->SetBranchStatus("hitY",   1);  fEventTree->SetBranchAddress("hitY",    hits_ypos);
  fEventTree->SetBranchStatus("hitZ",   1);  fEventTree->SetBranchAddress("hitZ",    hits_zpos);
  fEventTree->SetBranchStatus("hitT",   1);  fEventTree->SetBranchAddress("hitT",    hits_time);
*/

}

// ----------------------------------------------------------------------------

void MGGenWFForEvents::GenerateHomogeniousHits()
{
/**
  Generate waveforms for homogeniously distributed hits in the region defined
  in the ini file. See ReadSetupDataFromFile() for details on the input

*/

  float x,y,z,r,phi;

  for (int iR=0; iR<fNR; iR++)
  {
    r = mm*(fStartR + (iR+0.5)*(fEndR-fStartR)/fNR); // +0.5 means that you go to the bincenter, not the edge
//    cout<<"Processing event at r = "<<r<<endl;
    for (int iPhi=0; iPhi<fNPhi; iPhi++)
    {
      phi = fStartPhi + (iPhi+0.5)*(fEndPhi-fStartPhi)/fNPhi; // +0.5 means that you go to the bincenter, not the edge
//      cout<<"Processing event at phi = "<<phi<<endl;
      x = r * TMath::Cos(phi*pi/180);
      y = r * TMath::Sin(phi*pi/180);
      for (int iZ=0; iZ<fNZ; iZ++)
      {
        z = mm*(fStartZ + (iZ+0.5)*(fEndZ-fStartZ)/fNZ); // +0.5 means that you go to the bincenter, not the edge
        fHits.Reset();
//        cout<<"Processing event at (r,phi,z): ("<<r<<","<<phi<<","<<z<<") = (x,y,z): ("<<x<<","<<y<<","<<z<<")"<<endl;
        fHits.AddCrystalHit(x,y,z,0.,1.*MeV,0);

        // Generate waveforms for the hit
        fWFManager->GenerateWaveforms(&fHits);
      }
    }
  }
  fWFManager->EndOfRunAction();
}

// ----------------------------------------------------------------------------

void MGGenWFForEvents::ProcessEvents()
{
/**
  Generate waveforms for hits. There is an option to process only particular events
  from the previously created TEventList. This part is currently commented out

*/

/*
//////////////////////////////////////////////////////////////////////////////
//
//   This commented part was done for the possibility to create an event list,
//   thus to so to select only some events to save time and space
//
//////////////////////////////////////////////////////////////////////////////

// If event list was not created - create it. This usually takes long
// as it process all the events
//  if (fEventList==NULL) CreateEventList();

//  fEventTree -> SetEventList(fEventList);
//  fSelectedEventsTree = fEventTree -> CloneTree(0);

  for (int i=0;i<fNSelectedEvents;i++)
  {
    fSelectedEventsTree->GetEntry(i);
  }
*/

  int j;
  int id = 0;
  //float r = 3.3*cm;
  //float x,y;
  //float pi = TMath::Pi();
  //float phi = 80; // degrees
  for (int i=0;i<fNEvents;i++)
  {
    fEventTree->GetEntry(i);
    if (i%100==0) cout<<"Processing event "<<i<<endl;
    fHits.Reset();

    // Generate waveforms for the hits
    for (j=0;j<hits_totnum;j++)
    {
      fHits.AddCrystalHit(hits_xpos[j]*cm,hits_ypos[j]*cm,hits_zpos[j]*cm,hits_time[j],hits_edep[j]*MeV,id);
    }
    fHits.CombineHits(1*ns, fCombineHitsDistance, 512*ns);

    fWFManager->GenerateWaveforms(&fHits);

  }
  fWFManager->EndOfRunAction();
}

// ----------------------------------------------------------------------------

void MGGenWFForEvents::Occupancy()
{
/**
  Simulate holes drift and calculate occupancy of the segments from hits

*/

  time_t time_current;
  tm *ptm;

  int j;
//  fOutputFile->cd();
  for (int i=0; i<fNEvents; i++)
  {
    time_current = time(NULL);
    ptm = gmtime(&time_current);
    if (i%500==0){
      MGLog(routine)<<i<<" events processed at "
                    <<Form("%2d:%02d:%02d",ptm->tm_hour, ptm->tm_min, ptm->tm_sec)
                    <<endlog;
    }

    fEventTree->GetEntry(i);

    fHits.Reset();

    // Generate waveforms for the hits
    for (j=0;j<hits_totnum;j++)
    {
      fHits.AddCrystalHit(hits_xpos[j]*cm, hits_ypos[j]*cm, hits_zpos[j]*cm, hits_time[j]*ns, hits_edep[j]*MeV, 0);
    }
    fHits.CombineHits(1*ns, fCombineHitsDistance, 512*ns);

    fWFGenerator->CalculateOccupancyForHits(fHits);
    fOutputTree->Fill();
  }
  fOutputFile->Write();
}

// ----------------------------------------------------------------------------

void MGGenWFForEvents::Finalize()
{
/**
  Close the output file, delete the objects

*/

  if (fOutputFile!=NULL)
  {
    // do not write to, close and delete as it is done in EndOfRunAction of the fWFManager
//    fOutputFile->Write();
//    fOutputFile->Close();
//    delete fOutputFile;
  }

  if (fEventFile) fEventFile->Close();
  if (fEventFile) delete fEventFile;

  if (fWFGenerator) delete fWFGenerator;
  if (fFields) delete fFields;

  if (fCrystal) delete fCrystal;
  if (fWFManager) delete fWFManager;
  if (fWFWriter) delete fWFWriter;

/*
  delete fSelectedEventsTree;
  delete fEventList;
*/

}

// ----------------------------------------------------------------------------

void MGGenWFForEvents::CreateEventList()
{
/**
  Create an event list to select particular events. \warning Currently not used

*/

// To create the event list
  fEventList = new TEventList("evlist","evlist");
  for (int i=0;i<fNEvents;i++)
  {
    fEventTree->GetEntry(i);
    if (1) fEventList->Enter(i); // This string should be replaced with the cuts you need
  }
  fNSelectedEvents = fEventList->GetN();
}

