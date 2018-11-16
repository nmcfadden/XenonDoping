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
 * $Id: MGGeneratorPrimaryMessenger.hh,v 1.7 2008-06-26 15:47:22 pandola Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorPrimaryMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Messenger class for commands /MG/generator
 *
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jul  7 17:14:54 PDT 2004
 * 
 * REVISION:
 * 
 * 07-07-2004, Created, R. Henning
 * 11-08-2004, Added commands for the management of source confinement, Luciano
 * 01-31-2005, Added volumelist command for sampling in lists of volumes, Kevin 
 * 02-07-2005, Added command to set position for point-like sources, Luciano
 * 06-26-2008, Added commands for volumelist sampling (add and clear), Luciano
 */

#ifndef _MGGENERATORPRIMARYMESSENGER_HH
#define _MGGENERATORPRIMARYMESSENGER_HH

#include "globals.hh"
#include "G4UImessenger.hh"


class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger; 
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class MGGeneratorPrimary;


class MGGeneratorPrimaryMessenger : public G4UImessenger
{
  public:
    MGGeneratorPrimaryMessenger(MGGeneratorPrimary *generator);
    ~MGGeneratorPrimaryMessenger();

    G4String GetCurrentValue(G4UIcommand *command);
    void SetNewValue(G4UIcommand *command, G4String newValues);


  private:
    MGGeneratorPrimary* fGeneratorPrimary;

    G4UIdirectory* fGeneratorDirectory;
    // /MG/generator

    G4UIcmdWithoutParameter* fNameCmd;
    // /MG/generator/name

    G4UIcmdWithAString* fSelectCmd;
    // /MG/generator/select

    G4UIcmdWithAString* fConfineCmd; 
    // /MG/generator/confine

    G4UIcmdWithAString* fVolumeCmd; 
    // /MG/generator/volume

    G4UIcmdWithAString* fVolumeListCmd; 
    // /MG/generator/volumelist

    //G4UIcmdWithAString* fSurfaceListCmd; 
    // /MG/generator/surfacelist

    G4UIcmdWithAnInteger* fVolumeListFromCmd;
    // /MG/generator/volumelistfrom 

    G4UIcmdWithoutParameter* fVolumeListClearCmd;
    // /MG/generator/volumelistclear

    G4UIcmdWithAnInteger* fVolumeListAddCmd;
    // /MG/generator/volumelistadd

    G4UIcmdWithAnInteger* fVolumeListToCmd;
    // /MG/generator/volumelistto 
    
    G4UIcmdWithAString* fVolumeArrayAddCmd; 
    // /MG/generator/volumearrayadd
  
    G4UIcmdWith3VectorAndUnit* fPositionCmd;
    // /MG/generator/position

    G4UIcmdWithAString* fGSSPostionsFileCmd; 
    // /MG/generator/gsspositionsfile

    G4UIcmdWithADoubleAndUnit* fGSSOffsetCmd;
    // /MG/generator/gssoffset

    G4UIcmdWithAnInteger* fGSSEventNumberCmd;
    // /MG/generator/gsseventnumber
};
#endif
