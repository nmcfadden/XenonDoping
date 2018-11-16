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
// $Id: MGAnalysisMCSegmentation.cc,v 1.7 2006-09-28 00:30:13 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MGAnalysisMCSegmentation.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jan  4 11:48:06 PST 2006
 * 
 * REVISION:
 *
 * 1-04-2005, Created, R. Henning
 * 3-28-2006, Modified Additions by MGM, R. Henning
 * 9-07-2006, Added support for FillDetectorEventHistograms(), R. Henning
 * 9-17-2006, Fixed small bug that caused steps with seed times == 0.0
 *            to be ignored, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include "TFile.h"
#include "TTree.h"

#include "io/MGLogger.hh"
#include "mjprocessors/MGProcessorMCSegmentation.hh"
#include "mjprocessors/MGProcessorMCRadialDiscriminator.hh"
#include "mjprocessors/MGProcessorMCReadTree.hh"
#include "mjprocessors/MGProcessorMCTimeCluster.hh"

//---------------------------------------------------------------------------//

#include "mjprocessors/MGAnalysisMCSegmentation.hh"

//---------------------------------------------------------------------------//

MGAnalysisMCSegmentation::MGAnalysisMCSegmentation() :
  MGProcessorAnalysis("MGAnalysisMCSegmentation", "MGAnalysisMCSegmentation")
{
  // Note that order of instantiation is important. It is the order in which the
  // processor's methods will be evaluated. 

  //  Create Processor to read file
  fProcessorMCReadTree = new MGProcessorMCReadTree("MGProcessorMCReadTree");

  // Create time clustering processor
  (fProcessorMCTimeCluster = new MGProcessorMCTimeCluster())->SetTimeResolution(0.001);

  // Create Radial discriminator. Has to be before segmentation scheme. 
   fProcessorMCRadialDiscriminator = 
     new MGProcessorMCRadialDiscriminator("MGProcessorMCRadialDiscriminator");

  // Create segmentation schemes of interest.
  fProcessorMCSegmentation.clear();
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation1x1", 1, 1, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation1x2", 1, 2, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation1x3", 1, 3, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation1x5", 1, 5, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation1x7", 1, 7, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation1x10", 1, 10, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation2x1", 2, 1, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation2x2", 2, 2, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation2x3", 2, 3, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation2x5", 2, 5, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation2x7", 2, 7, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation2x10", 2, 10, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation3x1", 3, 1, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation4x1", 4, 1, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation4x2", 4, 2, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation4x4", 4, 4, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation4x8", 4, 8, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation5x1", 5, 1, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation6x1", 6, 1, 57));
   fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation6x6", 6, 6, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation7x1", 7, 1, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation8x1", 8, 1, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation8x2", 8, 2, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation8x5", 8, 5, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation10x1", 10, 1, 57));
  fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation10x10", 10, 10, 57));


  // LLNL detector segmentation scheme. 
  /* 
     fProcessorMCSegmentation.push_back( 
    new MGProcessorMCSegmentation("MGProcessorMCSegmentation8x7", 8, 7, 1)); 
  */
 }

//---------------------------------------------------------------------------//

MGAnalysisMCSegmentation::~MGAnalysisMCSegmentation()
{;}

//---------------------------------------------------------------------------//

void MGAnalysisMCSegmentation::BeginOfRunAction()
{
  MGProcessorAnalysis::BeginOfRunAction();
  fFile = new TFile(fFileName.c_str(), "RECREATE");
  MGLog(routine) << "Opened: " << fFileName << endlog;
  fTree = new TTree("ProcessedData", "ProcessedData");
  for(size_t i = 0; i < fProcessorMCSegmentation.size(); i++){
    fProcessorMCSegmentation[i]->SetTree(fTree);
    // NOTE: Splitlevel MUST not be set to 2, otherwise ROOT spits out all 
    // the same values for each segmentation scheme.
    fProcessorMCSegmentation[i]->AddToTree(32000, 0);
    fProcessorMCSegmentation[i]->SetTimeResolution(0.001);
  }
  fProcessorMCRadialDiscriminator->SetTree(fTree);
  fProcessorMCRadialDiscriminator->AddToTree(32000,0);
  fProcessorMCRadialDiscriminator->SetTimeResolution(0.001);
}

//---------------------------------------------------------------------------//

void MGAnalysisMCSegmentation::EndOfRunAction()
{
  MGProcessorAnalysis::EndOfRunAction();
  fTree->Write();
  fFile->Close();
}

//---------------------------------------------------------------------------//

void MGAnalysisMCSegmentation::ProcessEventAction()
{
  MGProcessor::ProcessEventAction(); // Note ... we are calling the Processor.
  fProcessorMCReadTree->ProcessEventAction(); // Get Event Tree
  fProcessorMCTimeCluster->ProcessEventAction(); // Find time clusters

  // Create an event in the output tree for each seed time.
  Double_t  seedTime;
  fProcessorMCTimeCluster->ResetSeedTimeIterator();
  // if seed time == -1, then we are done.
  while((seedTime = fProcessorMCTimeCluster->GetSeedTimeAndIncrement()) > -0.99) {
    for(size_t j = 0; j < fProcessorMCSegmentation.size(); j++) {
      fProcessorMCSegmentation[j]->SetTime(seedTime);
      fProcessorMCSegmentation[j]->ProcessEventAction();
    }
    fProcessorMCRadialDiscriminator->SetTime(seedTime);
    fProcessorMCRadialDiscriminator->ProcessEventAction(); 
    for(size_t j = 0; j < fProcessorMCSegmentation.size(); j++)
      fProcessorMCSegmentation[j]->FillDetectorEventHistograms();
    fProcessorMCRadialDiscriminator->FillDetectorEventHistograms();
    fTree->Fill(); 
  }

  // MGM adding code ... Modifed RH 3/28/2006
    if(fProcessorMCReadTree->GetStatus() == ProcessorNameSpace::kDoneProcessing) {
    for(size_t j = 0; j < fProcessorMCSegmentation.size(); j++) {                 
      fProcessorMCSegmentation[j]->SetStatus(ProcessorNameSpace::kDoneProcessing);                          
    }  
    fProcessorMCRadialDiscriminator->SetStatus(ProcessorNameSpace::kDoneProcessing);         
    }
  // end MGM adding code
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
