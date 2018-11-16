//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
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
/**                                                            
* $Id: MGGeneratorPb210Messenger.hh,v 1.0 2009/5/17
*      
* CLASS DECLARATION:  MGGeneratorPb210Messenger.hh
*
*---------------------------------------------------------------------------//
*
* DESCRIPTION: 
*
*/ 
// Begin description of class here
/**
*
* Messenger for Pb210 generator.
*
*/
// End class description
//
/**  
* SPECIAL NOTES:
*
*/
// 
// --------------------------------------------------------------------------//
/** 
* AUTHOR: A. Stevens
* CONTACT: austinst@physics.unc.edu
* FIRST SUBMISSION:
* 
* REVISION:
* 
* 
*/
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORPB210MESSENGER_HH
#define _MGGENERATORPB210MESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWithAString;
class MGGeneratorPb210;

class MGGeneratorPb210Messenger : public G4UImessenger
{
public:
	MGGeneratorPb210Messenger(MGGeneratorPb210 *generator);
	MGGeneratorPb210Messenger(const MGGeneratorPb210Messenger &);
	~MGGeneratorPb210Messenger();

	void SetNewValue(G4UIcommand *cmd, G4String newValue);

protected:

private:
	MGGeneratorPb210	*fPb210Generator;
	G4UIdirectory	*fPb210Directory;
	G4UIcmdWithAString	*fSetSurfaceVolumeCmd;

};
#endif

