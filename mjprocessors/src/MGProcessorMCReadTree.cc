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
// $Id: MGProcessorMCReadTree.cc,v 1.8 2006-09-28 00:30:13 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessorMCReadTree.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Processor dependencies: none
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Sun Dec  4 15:01:33 PST 2005
 * 
 * REVISION:
 *
 * 12-04-2005, Created, R. Henning
 * 09-18-2006, Fixed minor bug that prevented last entry from being read. 
 *             R. Henning.
 *
 */
//---------------------------------------------------------------------------//
//

#include "io/MGOutputG4StepsData.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjprocessors/MGProcessorMCReadTree.hh"

//---------------------------------------------------------------------------//

MGProcessorMCReadTree::MGProcessorMCReadTree():
  MGProcessor("MGProcessorMCReadTree", "MGProcessorMCReadTree")
{
  fG4StepsData = new MGOutputG4StepsData();
  if(!(fTChain = new TChain("fTree"))){
    MGLog(error) << "Could not allocate TChain!" << endlog;
    MGLog(fatal) << endlog;
  }
  fNEntriesToProcess = -1;
}

//---------------------------------------------------------------------------//

MGProcessorMCReadTree::MGProcessorMCReadTree(string name):
  MGProcessor(name, "MGProcessorMCReadTree")
{
  fG4StepsData = new MGOutputG4StepsData();
  if(!(fTChain = new TChain("fTree"))){
    MGLog(error) << "Could not allocate TChain!" << endlog;
    MGLog(fatal) << endlog;
  }
  fNEntriesToProcess = -1;
}

//---------------------------------------------------------------------------//

MGProcessorMCReadTree::~MGProcessorMCReadTree()
{
  fG4StepsData->DeallocateArrays();
  delete fG4StepsData;
  // Don't delete ROOT classes. It causes a core dump.
}

//---------------------------------------------------------------------------//

void MGProcessorMCReadTree::BeginOfRunAction()
{
  MGProcessor::BeginOfRunAction();
  fMaxNumberOfSteps = 25000;
  fG4StepsData->AllocateArrays(fMaxNumberOfSteps);
  fTChain->SetBranchStatus("*", 1); // Read all by default.
  fTChain->SetBranchAddress("RawMC", &fG4StepsData);
  MGLog(routine) << "Chain has " << fTChain->GetEntries() << " events." << endlog;
  fEntryNumber = 0;
}

//---------------------------------------------------------------------------//

void MGProcessorMCReadTree::BeginOfEventAction()
{
  MGProcessor::BeginOfEventAction();
  if(fEntryNumber > fTChain->GetEntries() - 1 || (fNEntriesToProcess > -1 && fEntryNumber >= fNEntriesToProcess)) {
    fStatus = ProcessorNameSpace::kDoneProcessing;
  }
  else {
    fTChain->GetEntry(fEntryNumber++);
  }
}

//---------------------------------------------------------------------------//

void MGProcessorMCReadTree::AddMCFile(string name)
{
  fTChain->Add(name.c_str());
  fMCFileNames.push_back(name);
}

