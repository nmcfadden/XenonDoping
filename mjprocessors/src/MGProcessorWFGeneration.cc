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
// $Id: MGProcessorWFGeneration.cc,v 1.2 2007-07-19 02:10:12 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessorWFGeneration.cc
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
 * FIRST SUBMISSION: Sun Dec  4 14:58:46 PST 2005
 * 
 * REVISION:
 *
 * 12-4-2005, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//


#include "io/MGLogger.hh"
#include "io/MGOutputG4StepsData.hh"
#include "mjprocessors/MGProcessorMCReadTree.hh"


#include "mjprocessors/MGProcessorWFGeneration.hh"


using namespace std;


MGProcessorWFGeneration::MGProcessorWFGeneration(MGVWaveformGenerator* waveformGenerator, string name) :
MGProcessor(name, "MGProcessorWFGeneration")
{
  fWFManager.AddWaveformGenerator(waveformGenerator);
}

void MGProcessorWFGeneration::BeginOfRunAction()
{
  MGProcessor::BeginOfRunAction();
  if(!(fReadTreeProcessor = (MGProcessorMCReadTree*)
       MGProcessor::GetProcessor("MGProcessorMCReadTree"))) {
    MGLog(error) << "MGProcessorMCReadTree not found!" << endlog;
    MGLog(fatal) << endlog;
  }
}


 
void MGProcessorWFGeneration::ProcessEventAction()
{
  MGProcessor::ProcessEventAction();
  if(fReadTreeProcessor->GetStatus() == ProcessorNameSpace::kDoneProcessing) {
    fStatus = ProcessorNameSpace::kDoneProcessing; 
    return;
  }

  const MGOutputG4StepsData *sD = fReadTreeProcessor->GetG4StepsData();
  MGCrystalHits* crystalHits = NULL;
  //crystalHits = sD->GetCrystalHits(); 
  fWFManager.GenerateWaveforms(crystalHits);
}


void MGProcessorWFGeneration::EndOfRunAction()
{
  MGProcessor::EndOfRunAction();
  fWFManager.EndOfRunAction();
}

