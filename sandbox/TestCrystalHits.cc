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
 * $Id: TestCrystalHits.cc,v 1.2 2007-08-31 08:04:57 jasondet Exp $
 *      
 * CLASS DECLARATION:  MJAnalysisWF.cc
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Sample wf generation program for Majorana.
 *
 * AUTHOR: J. Detwiler
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jan  4 15:01:33 PST 2006
 * 
 * REVISION:
 * 
 * 05-01-2006, Created, J. Detwiler
 *
 */

#include "TChain.h"
#include "io/MGLogger.hh"
#include "MGCrystalHits.hh"
#include "io/MGOutputG4StepsData.hh"

using namespace CLHEP;

// Example Usage: TestCrystalHits /auto/majorana1/MJ/data/ProposedGeometries/ReferenceDesign_Realistic/ActiveRegions/ActiveCrystal1CrystalColumn1Co60_7_1.root

int main(int argc, char *argv[])
{
  MGLog(routine) << "Going..." << endlog;
  MGLogger::SetSeverity(MGLogger::routine);
  if(argc == 1) {
    MGLog(error) << "No file specified!" << endlog;
    return 1;
  } 

  MGOutputG4StepsData* stepsData = new MGOutputG4StepsData;
  int maxSteps = 25000;
  stepsData->AllocateArrays(maxSteps);

  TChain* chain = new TChain("fTree");
  for(int i = 1; i < argc; i++) {
    chain->Add(argv[i]);
  }
  MGLog(routine) << "Chain has " << chain->GetEntries() << " events." << endlog;
  chain->SetBranchStatus("*", 1); // Read all by default.
  chain->SetBranchAddress("RawMC", &stepsData);

  MGCrystalHits xtalHits;
  for(int iEntry = 0; iEntry < 1; iEntry++) {
    chain->GetEntry(iEntry);
    MGLog(routine) << "Entry " << iEntry << ": nSteps = " << stepsData->fTotalNumberOfSteps << endlog;
    stepsData->FillCrystalHits(&xtalHits);
    xtalHits.Dump();
    MGLog(routine) << "Combining hits..." << endlog;
    xtalHits.CombineHits(1*ns, 0.2*mm, 512*ns);
    xtalHits.Dump();
  }

  MGLog(routine) << "Done..." << endlog;

  stepsData->DeallocateArrays();
  delete stepsData;
}

