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
// $Id: MGManagementRunAction.cc,v 1.11 2009-02-27 19:41:10 alexis3 Exp $ 
//      
// CLASS IMPLEMENTATION:  MGManagementRunAction.cc
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
 * FIRST SUBMISSION: Mon Jul 12 11:49:43 PDT 2004
 * 
 * REVISION:
 *
 * 07-12-2004, Created, R. Henning
 * 07-19-2004, Modified to support singlet MJManager accessed via static 
 *             method, R. Henning
 * 11-08-2004, Replaced OutputRoot with VOutputManager methods, Luciano
 * 12-07-2004, Modified for Gerda Integration, R. Henning
 * 01-27-2005, Added automatic randomization seed if not explicitly set in the
 *             macro file, Kareem
 * 01-27-2005, Tracks real time since the start of the run for benchmarking
 *             purposes, Kareem
 * 06-08-2007, Added some codes handling pre-waveform output, Jing Liu
 * 11-26-2007, Changed pre-waveform warning message to routine, Jing Liu
 * 12-09-2008, Changed the algorithm to set the automatic random seed, in order 
 *             to allow for contemporaneous submission of multiple jobs to 
 *             different machines, Luciano
 * 01-06-2009, Fixed Bug in random seed causing seg fault,
 *             Report correct time instead of random seed, K. Macon
 * 02-27-2009, Included unistd.h so that getpid() is defined, A. Schubert             
 */
//---------------------------------------------------------------------------//
//

//#include "CLHEP/Random/Random.h"
// The appropriate headers are included in Randomize.hh
#include "Randomize.hh"

#include "generators/MGGeneratorPrimary.hh"
#include "generators/MGVGenerator.hh"
#include "management/MGManagementEventAction.hh"
#include "management/MGManager.hh"
#include "io/MGLogger.hh"

#include "G4Run.hh"

//---------------------------------------------------------------------------//

#include "management/MGManagementRunAction.hh"

//---------------------------------------------------------------------------//


#include <sys/time.h>
#include <time.h>
#include <unistd.h>

//---------------------------------------------------------------------------//

MGManagementRunAction::MGManagementRunAction() :
fControlledRandomization( false )
{;}

//---------------------------------------------------------------------------//

MGManagementRunAction::MGManagementRunAction(const MGManagementRunAction & other)
 : G4UserRunAction(other)
{
  fControlledRandomization = false;
}

//---------------------------------------------------------------------------//

MGManagementRunAction::~MGManagementRunAction()
{;}

//---------------------------------------------------------------------------//

void MGManagementRunAction::BeginOfRunAction(const G4Run *run)
{
  MGManager* manager = MGManager::GetMGManager();
  MGLog(trace) << "Performing MG beginning of run actions." << endlog;
  if(manager->GetMGGeneratorPrimary())
   manager->GetMGGeneratorPrimary()->GetMGGenerator()->BeginOfRunAction(run);
  else {
    MGLog(error) << "No generator specified!" << endlog;
    MGLog(fatal) << endlog;
  }
  if(manager->GetMGEventAction()->GetOutputManager())
     manager->GetMGEventAction()->GetOutputManager()->BeginOfRunAction(); 
  else
    MGLog(warning) << "No Output specified!" << endlog;

  if(manager->GetMGEventAction()->GetOutputManagerPreWF())
     manager->GetMGEventAction()->GetOutputManagerPreWF()->BeginOfRunAction(); 
  else
    MGLog(routine) << "No pre-waveform output specified." << endlog;

  /* Algorithm for automatic randomisation (by B. Schwingenheuer) */
  struct timeval tt;
  pid_t pid = getpid();
  gettimeofday(&tt,NULL);
  G4long timeSeed = tt.tv_sec*1000+(tt.tv_usec/1000);
  timeSeed += pid*3137; //different machines, same time --> different seed
  timeSeed = std::abs(timeSeed); //take only positive seeds.
  /* End of Algorithm */ 

  if( !fControlledRandomization ) {
    CLHEP::HepRandom::setTheSeed( timeSeed );
    MGLog( routine ) << "CLHEP::HepRandom seed for run number " << run->GetRunID()
                     << " set to: " << timeSeed << endlog;
  }
  fStartTime = tt.tv_sec;
  struct tm *timeInfo;
  timeInfo = gmtime( &fStartTime );
  MGLog( routine ) << "Run #" << run->GetRunID() << " started at GMT "
				   << asctime( timeInfo ) << endlog;
}

//---------------------------------------------------------------------------//

void MGManagementRunAction::EndOfRunAction(const G4Run *run)
{
  MGManager* manager = MGManager::GetMGManager();
  MGLog(trace) << "Performing MG end of run actions." << endlog;
  manager->GetMGGeneratorPrimary()->GetMGGenerator()->EndOfRunAction(run);
  if(manager->GetMGEventAction()->GetOutputManager())
    manager->GetMGEventAction()->GetOutputManager()->EndOfRunAction();
  if(manager->GetMGEventAction()->GetOutputManagerPreWF())
    manager->GetMGEventAction()->GetOutputManagerPreWF()->EndOfRunAction();
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
