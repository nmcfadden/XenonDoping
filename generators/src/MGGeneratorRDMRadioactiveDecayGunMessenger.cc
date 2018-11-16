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
* $Id: MGGeneratorRDMRadioactiveDecayGunMessenger.cc,v 1.1 2004-12-07 08:56:06 pandola Exp $
*
* CLASS DECLARATION:  MGGeneratorRDMRadioactiveDecayGunMessenger.cc
*
*---------------------------------------------------------------------------//
*
* DESCRIPTION:
*
*/
// Begin description of class here
/**
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
#include "generators/MGGeneratorRDMRadioactiveDecayGunMessenger.hh"
#include "generators/MGGeneratorRDMUIcmdWithNucleusAndUnit.hh"
#include <iostream>
////////////////////////////////////////////////////////////////////////////////
//
MGGeneratorRDMRadioactiveDecayGunMessenger::MGGeneratorRDMRadioactiveDecayGunMessenger
(MGGeneratorRDMRadioactiveDecayGun* theRadioactiveDecayGun1) :
theRadioactiveDecayGun(theRadioactiveDecayGun1)
{
  ionCmd = new MGGeneratorRDMUIcmdWithNucleusAndUnit("/grdm/ion",this);
  ionCmd->SetGuidance("define the primary ion (a,z,e)");
  ionCmd->SetParameterName("A","Z","E",true);

  ionCmd->SetDefaultUnit("keV");
  ionCmd->SetUnitCandidates("keV MeV");
}
////////////////////////////////////////////////////////////////////////////////
//
MGGeneratorRDMRadioactiveDecayGunMessenger::~MGGeneratorRDMRadioactiveDecayGunMessenger ()
{
  delete ionCmd;
}
////////////////////////////////////////////////////////////////////////////////
//
void MGGeneratorRDMRadioactiveDecayGunMessenger::SetNewValue
  (G4UIcommand *command, G4String newValues)
{

  if (command==ionCmd) {theRadioactiveDecayGun->
    SetNucleus(ionCmd->GetNewNucleusValue(newValues));
 }
}
////////////////////////////////////////////////////////////////////////////////
//
//G4String G4MGGeneratorRDMRadioactiveDecayGunMessenger::GetCurrentValue (G4UIcommand * command)
//{

//  else if (command==NpIntvCmd)
//    {cv = NpIntvCmd->ConvertToString(theRDMnnun->GetNumberOfPhiIntv());}
//  return cv;
//}
////////////////////////////////////////////////////////////////////////////////










