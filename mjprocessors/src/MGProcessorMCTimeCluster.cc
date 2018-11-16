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
// $Id: MGProcessorMCTimeCluster.cc,v 1.4 2006-07-08 21:08:15 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessorMCTimeCluster.cc
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
 * FIRST SUBMISSION: Tue Jan 10 16:46:26 PST 2006
 * 
 * REVISION:
 *
 * 07-08-2006, Bugfix involving increment of timeIter, thanks Jason! R. Henning
 * 01-10-2006, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include "TMath.h"

#include "io/MGLogger.hh"
#include "io/MGOutputG4StepsData.hh"
#include "mjprocessors/MGProcessorMCReadTree.hh"

//---------------------------------------------------------------------------//

#include "mjprocessors/MGProcessorMCTimeCluster.hh"

//---------------------------------------------------------------------------//

MGProcessorMCTimeCluster::MGProcessorMCTimeCluster() : 
  MGProcessor("MGProcessorMCTimeCluster","MGProcessorMCTimeCluster"),
  fStepsData(0), fProcessorMCReadTree(0)
{;}

//---------------------------------------------------------------------------//

MGProcessorMCTimeCluster::~MGProcessorMCTimeCluster()
{;}

//---------------------------------------------------------------------------//

void MGProcessorMCTimeCluster::BeginOfRunAction()
{
  MGProcessor::BeginOfRunAction();
  fProcessorMCReadTree = (MGProcessorMCReadTree*)
    MGProcessor::GetProcessor("MGProcessorMCReadTree");
  if(!fProcessorMCReadTree) {
    MGLog(error) << " MGProcessorMCReadTree not found!" << endlog;
    MGLog(fatal) << endlog;
  }
  if(!(fStepsData = fProcessorMCReadTree->GetG4StepsData())) {
    MGLog(error) << " Steps data not found!" << endlog;
    MGLog(fatal) << endlog;
  }
}

//---------------------------------------------------------------------------//

void MGProcessorMCTimeCluster::ProcessEventAction()
{
  MGProcessor::ProcessEventAction();
  fSeedTimes.clear();
  if(fProcessorMCReadTree->GetStatus() == ProcessorNameSpace::kDoneProcessing)
    fStatus = ProcessorNameSpace::kDoneProcessing;
  else {
    Bool_t seedFound;
    list<Double_t>::iterator  timeIter;
    for(Int_t i = 0; i < fStepsData->fTotalNumberOfSteps; i++) {
      seedFound = false;
      timeIter = fSeedTimes.begin();
      while(timeIter != fSeedTimes.end()) {
	if(TMath::Abs(fStepsData->fGlobalTime[i] - *timeIter) < fTimeResolution){
	  seedFound = true;
	  break;
	}
	timeIter++;
      }
      if(!seedFound) fSeedTimes.push_back(fStepsData->fGlobalTime[i]);
    }
    fSeedTimes.sort();
  }
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
