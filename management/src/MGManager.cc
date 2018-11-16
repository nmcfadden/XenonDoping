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
// $Id: MGManager.cc,v 1.4 2007-02-23 09:24:09 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGManager.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Tue Jul  6 16:15:44 PDT 2004
 * 
 * REVISION:
 *
 * 07-06-2004, Created, R. Henning
 * 07-19-2004, Converted to singleton with global access, R. Henning
 * 11-08-2004, Added TrackingAction, Xiang
 * 12-07-2004, Modified for Gerda Integration, R. Henning
 * 12-15-2004, Added StackingAction, L. Pandola
 *
 */
//---------------------------------------------------------------------------//

#include "G4RunManager.hh"
#include "G4VisManager.hh"

#include "management/MGManagerMessenger.hh"
#include "io/MGLogger.hh"
//#include "waveform/MJWaveformManager.hh"

//---------------------------------------------------------------------------//

#include "management/MGManager.hh"

//---------------------------------------------------------------------------//

MGManager* MGManager::fMGManager = 0;

//---------------------------------------------------------------------------//

MGManager::MGManager():
  fG4RunManager(0), fG4VisManager(0), 
  fGeneratorPrimary(0), fManagerDetectorConstruction(0),
  fManagementEventAction(0),   fManagementRunAction(0),
  fManagementSteppingAction(0), fManagementTrackingAction(0),
  fManagementStackingAction(0)
   //, fWaveformManager(0)
{
  if(fMGManager) {
    MGLog(error) << "MGManager must be singleton!" << endlog;
    MGLog(fatal) << endlog;
  }
  fMGManager = this;
  fG4Messenger = new MGManagerMessenger(this);
}

//---------------------------------------------------------------------------//

MGManager::MGManager(const MGManager&)
{;}

//---------------------------------------------------------------------------//

MGManager::~MGManager()
{

  delete fG4Messenger;
  //delete fG4VisManager;			//in main, thats where it was defined !
  //delete fWaveformManager;
  //delete fG4RunManager; // G4RunManager deletes all the other objects.
													// in main as well !!
}

//---------------------------------------------------------------------------//

MGManager* MGManager::GetMGManager()
{
  return fMGManager;
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
