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
// $Id: MGOutputRootPreWF.cc,v 1.1 2007-06-08 13:06:25 jliu Exp $ 
//      
// CLASS IMPLEMENTATION:  MGOutputRootPreWF.cc
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
 * AUTHOR: Jing Liu
 * CONTACT: jingliu at mppmu.mpg.de
 * FIRST SUBMISSION: 06-08-2007
 * 
 * REVISION:
 *
 * 06-08-2007, Created, Jing Liu
 */
//---------------------------------------------------------------------------//

// MJ header
#include "io/MGLogger.hh"
#include "io/MGOutputRootPreWF.hh"
#include "management/MGManager.hh"
#include "management/MGManagerDetectorConstruction.hh"
//---------------------------------------------------------------------------//

MGOutputRootPreWF::MGOutputRootPreWF(G4bool isMother) : MGOutputRoot(isMother)
{
	// Must be G4 compliant here
	// http://geant4.web.cern.ch/geant4/collaboration/coding_guidelines.html
	// e.g. geant4/source/volume/src/G4Navigator.cc as an example
}

MGOutputRootPreWF::~MGOutputRootPreWF()
{
   delete container;
}
//---------------------------------------------------------------------------//
void MGOutputRootPreWF::BeginOfRunAction()
{
   if (IsMother()) OpenFile();                  
   
   container = new MGTCrystalDataContainer("container","Crystal Data Container");

   MGManager::GetMGManager()->GetManagerDetectorConstruction()->GetDetector()
      ->FillCrystalDataVector(container->GetCrystalDataVector());
   
   container->Write();

   DefineSchema();              // for hits
}
//---------------------------------------------------------------------------//
void MGOutputRootPreWF::BeginOfEventAction(const G4Event*) 
{;
}
//---------------------------------------------------------------------------//
void MGOutputRootPreWF::EndOfEventAction(const G4Event*)
{;//write hits data to file
}
//---------------------------------------------------------------------------//
void MGOutputRootPreWF::EndOfRunAction()
{
   if (IsMother()) CloseFile();
   MGLog(trace) << "EndOfRunAction finished." << endlog;
}
//---------------------------------------------------------------------------//
void MGOutputRootPreWF::RootSteppingAction(const G4Step*)
{
   MGLog(debugging) << "Please define me, RootSteppingAction() in MGOutputRootPreWF." << endlog;   
}
void MGOutputRootPreWF::DefineSchema()
{
   MGLog(debugging) << "Please define me, DefineSchema() in MGOutputRootPreWF" << endlog;   
}
// //---------------------------------------------------------------------------//
// void MGOutputRootPreWF::OpenPreWFFile()
// {
//    fRootFile = new TFile(fFileName.c_str(), "RECREATE");
//    MGLog(routine) <<" Opened ROOT file for pre-waveform data: " << fFileName << endlog;   
// }
// //---------------------------------------------------------------------------//
// void MGOutputRootPreWF::ClosePreWFFile()
// {
//    if(fRootFile != NULL) {
//       fRootFile->Close();
//       delete fRootFile;
//       fRootFile = NULL;
//    }
// }

