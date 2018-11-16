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
* $Id: MGGeneratorRDMRadioactiveDecayGunMessenger.hh,v 1.1 2004-12-07 08:45:48 pandola Exp $
*
* CLASS DECLARATION:  MGGeneratorRDMRadioactiveDecayGunMessenger.hh
*
*---------------------------------------------------------------------------//
*
* DESCRIPTION:
*
*/
// Begin description of class here
/**
*  Base class for database inteface
*
*
*
*/
// End class description
//
/**
* SPECIAL NOTES:
*
* This is part of the G4 rdm demo package. Original contact is P. Truscott (DERA)
*/
//
// --------------------------------------------------------------------------//
/**
* AUTHOR: 
* CONTACT: ydc@lbl.gov
* FIRST SUBMISSION: Wed Mar 10 14:42:07 PST 2004
*
* REVISION:
*
* 06-02-2004, Initial port to MG
* mm-dd-yyyy, What is changed, Whoami
*/
// --------------------------------------------------------------------------//
#ifndef MGGeneratorRDMRadioactiveDecayGunMessenger_h
#define MGGeneratorRDMRadioactiveDecayGunMessenger_h 1
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MODULE:              MGGeneratorRDMRadioactiveDecayGunMessenger.hh
//
// Version:             0.b.3
// Date:                29/02/00
// Author:              F Lei & P R Truscott
// Organisation:        DERA UK
// Customer:            ESA/ESTEC, NOORDWIJK
// Contract:            12115/96/JG/NL Work Order No. 3
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// CHANGE HISTORY
// --------------
//
// 29 February 2000, P R Truscott, DERA UK
// 0.b.3 release.
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
////////////////////////////////////////////////////////////////////////////////
//
#include "G4UImessenger.hh"

#include "generators/MGGeneratorRDMUIcmdWithNucleusAndUnit.hh"
#include "globals.hh"

#include "generators/MGGeneratorRDMRadioactiveDecayGun.hh"

class MGGeneratorRDMRadioactiveDecayGun;
////////////////////////////////////////////////////////////////////////////////
//
class MGGeneratorRDMRadioactiveDecayGunMessenger: public G4UImessenger
{
  // class description
  //   The MGGeneratorRDMRadioactiveDecayGunMessenger is instatiated by the
  //   MGGeneratorRDMRadioactiveDecayGun and introduces into the UI menu commands options
  //   to allow user to specify the isotope which will be treated by
  //   G4MGGeneratorRDMRadioactiveDecay
  // class description - end

public: // with description
  MGGeneratorRDMRadioactiveDecayGunMessenger (MGGeneratorRDMRadioactiveDecayGun *theMGGeneratorRDMRadioactiveDecayGun1);
  //    Constructor introduces commands into the UI menu to control
  //    MGGeneratorRDMRadioactiveDecayGun.  theG4MGGeneratorRDMRadioactiveDecayGun1 is used to identify
  //    to this class (when instatiated) the associated MGGeneratorRDMRadioactiveDecayGun,
  //    whose parameters are going to be changed as a result of the UI commands.
  //
  ~MGGeneratorRDMRadioactiveDecayGunMessenger ();
  //    Destructor deletes G4UIdirectory and G4UIcommand objects.
  //
  void SetNewValue (G4UIcommand *command, G4String newValues);
  //    Identifies the command which has been invoked by the user, extracts the
  //    parameters associated with that command (held in newValues, and uses
  //    these values with the appropriate member function of
  //    MGGeneratorRDMRadioactiveDecayGun.
  //
private:
  MGGeneratorRDMRadioactiveDecayGun          *theRadioactiveDecayGun;
  MGGeneratorRDMUIcmdWithNucleusAndUnit      *ionCmd;
};
////////////////////////////////////////////////////////////////////////////////
#endif




