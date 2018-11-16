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
// $Id: MJOutputShieldingMessenger.cc,v 1.1 2005-12-28 04:48:58 kareem Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputShieldingMessenger.cc
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
 * AUTHOR: kareem
 * CONTACT: 
 * FIRST SUBMISSION: Tue Dec 27 15:30:06 PST 2005
 * 
 * REVISION:
 *
 * 12-27-2005, Initial submission, Kareem
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4ios.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

#include "mjio/MJOutputShielding.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjio/MJOutputShieldingMessenger.hh"      //Present MJ Class Headers 

//---------------------------------------------------------------------------//

MJOutputShieldingMessenger::MJOutputShieldingMessenger( MJOutputShielding *output ) : fOutputShielding( output )
{
  // /MG/eventaction/shielding/ 
  fOutputShieldingDirectory = new G4UIdirectory("/MG/eventaction/");

  // /MG/eventaction/shielding/volumeName
  fSetVolumeNameCmd = new G4UIcmdWithAString( "/MG/eventaction/shieldingVolume", this );
  fSetVolumeNameCmd->SetGuidance( "Set the volume of interest for shielding studies." );
  fSetVolumeNameCmd->SetGuidance( "This volume is where the track information is recorded." );
  fSetVolumeNameCmd->SetParameterName( "VolumeOfInterest", false );
  fSetVolumeNameCmd->AvailableForStates(G4State_PreInit);
}

MJOutputShieldingMessenger::~MJOutputShieldingMessenger()
{
  delete fSetVolumeNameCmd;
  delete fOutputShieldingDirectory;
}

//---------------------------------------------------------------------------//

void MJOutputShieldingMessenger::SetNewValue( G4UIcommand* cmd, G4String str )
{
  if( cmd == fSetVolumeNameCmd ) fOutputShielding->SetVolumeName( str );
}
