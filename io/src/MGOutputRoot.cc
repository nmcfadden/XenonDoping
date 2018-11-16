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
//                                                          
// $Id: MGOutputRoot.cc,v 1.9 2007-07-20 08:43:15 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MGOutputRoot.cc
//
/**
 * AUTHOR: RHenning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Apr  7 16:12:33 PDT 2004
 * 
 * REVISION:
 *
 * 04-08-2004 Initialized, R. Henning
 * 05-28-2004 Added MGLogger, R. Henning.
 * 11-08-2004 Class inheriting from MGVOutputManager. I had to add OpenFile() 
 *            and CloseFile() for the name matching
 * 03-09-2005 Added CheckRootFileSize(), R. Henning.
 * 01-21-2006 Don't need to make a separate Open/CloseRootFile, and don't
 *            need a separate filename. Jason
 * 07-20-2007 Added a call to the virtual methods PrepareFileForClosing() at 
 *            the CloseFile(), to allow to perform actions from the concrete 
 *            output class. Luciano
 *
 */

#include "TFile.h"
#include "TTree.h"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

#include "io/MGLogger.hh" 

#include "io/MGOutputRoot.hh"

TTree* MGOutputRoot::fTree = NULL;
TFile* MGOutputRoot::fRootFile = NULL;

MGOutputRoot::MGOutputRoot(G4bool isMother): fIsMother(isMother)
{



}

void MGOutputRoot::OpenFile()
{
  if(fIsMother) {
    fRootFile = new TFile(fFileName.c_str(), "RECREATE");
    MGLog(routine) <<" Opened ROOT file for event data: " << fFileName << endlog;
  }
}

void MGOutputRoot::CloseFile()
{ 
  PrepareFileForClosing();
  WriteFile();
  if(fTree && MGLogger::GetSeverity() <= MGLogger::trace) fTree->Print();
  if(fRootFile != NULL) {
    fRootFile->Close(); // deletes fTree for us
    delete fRootFile;
    fRootFile = NULL;
    fTree = NULL;
  }
}

void MGOutputRoot::WriteFile()
{
  // Write out root file.  Useful for writing out data during runs.
  if(fTree) fTree->Write(fTree->GetName(),TObject::kOverwrite);
}

