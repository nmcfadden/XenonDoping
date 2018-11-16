//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
//                                                          
// $Id: MGGeneratorPb210Messenger.cc,v 1.0 2009/5/17
//      
// CLASS IMPLEMENTATION:  MGGeneratorPb210Messenger.cc
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
* AUTHOR: A. Stevens.
* CONTACT: austisnt@physics.unc.edu
* FIRST SUBMISSION:
* 
* REVISION:
*
* 
*/
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

#include "generators/MGGeneratorPb210.hh"

//---------------------------------------------------------------------------//

#include "generators/MGGeneratorPb210Messenger.hh"

//---------------------------------------------------------------------------//

MGGeneratorPb210Messenger::MGGeneratorPb210Messenger(MGGeneratorPb210 *generator) : fPb210Generator(generator)
{
	// /MG/generator/Pb210
	fPb210Directory = new G4UIdirectory("/MG/generator/Pb210/");
	fPb210Directory->SetGuidance("Lead shield decay generator control.");

	// MG/generator/Pb210/shieldSurface
	fSetSurfaceVolumeCmd = new G4UIcmdWithAString("/MG/generator/Pb210/shieldSurface", this);
	fSetSurfaceVolumeCmd->SetGuidance("Volume representing the surface of the lead shield");
	fSetSurfaceVolumeCmd->SetParameterName("VolumeName", false);
	fSetSurfaceVolumeCmd->AvailableForStates(G4State_Idle);

}

//---------------------------------------------------------------------------//

MGGeneratorPb210Messenger::MGGeneratorPb210Messenger(const MGGeneratorPb210Messenger & other) : G4UImessenger(other)
{;}

//---------------------------------------------------------------------------//

MGGeneratorPb210Messenger::~MGGeneratorPb210Messenger()
{
	delete fSetSurfaceVolumeCmd;
	delete fPb210Directory;
}

//---------------------------------------------------------------------------//

void MGGeneratorPb210Messenger::SetNewValue(G4UIcommand *cmd, G4String str)
{
	if( cmd == fSetSurfaceVolumeCmd )
	fPb210Generator->SetShieldSurfaceVolume(str);
}
