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
* $Id: MGGeneratorRDMUIcmdWithNucleusAndUnit.hh,v 1.1 2004-12-07 08:45:48 pandola Exp $
*
* CLASS DECLARATION:  MGGeneratorRDMUIcmdWithNucleusAndUnit.hh
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
#ifndef MGGeneratorRDMUIcmdWithNucleusAndUnit_h
#define MGGeneratorRDMUIcmdWithNucleusAndUnit_h 1
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MODULE:              MGGeneratorRDMUIcmdWithNucleusAndUnit.hh
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
// Class Description
//
// The G4MGGeneratorRDMUIcmdWithNucleusAndUnit permits input of the nucleus definition
// in terms of its (atomic weight, atomic number, excitation energy).
// Input is expected in the form:
//
//			A, Z, E (energy unit)
//
// where A, Z, E are respectively the atomic weight, atomic number and
// excitation energy.  The energy unit can be any of the geant4 defined energy
// units.  The default is "keV"
//
// class description - end
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
//
#include "G4UIcommand.hh"
#include "globals.hh"

#include "generators/MGGeneratorRDMNucleus.hh"
////////////////////////////////////////////////////////////////////////////////
//
class MGGeneratorRDMUIcmdWithNucleusAndUnit : public G4UIcommand
{
public:  // with description
  MGGeneratorRDMUIcmdWithNucleusAndUnit
  (const char * theCommandPath,G4UImessenger * theMessenger);
  //    Constructor identifying the command path in the User Interface and the
  //    associated G4UImessenger which will use this G4UIcommand object.
  //
  ~MGGeneratorRDMUIcmdWithNucleusAndUnit();
  //    Desctructor
  //
  MGGeneratorRDMNucleus GetNewNucleusValue(G4String paramString);
  //    Extracts the values A, Z, E and unit from paramString.
  //
  G4double GetNewUnitValue(G4String paramString);
  //    Returns the value of the unit (paramString) as defined in geant4
  //
  G4String ConvertToString(MGGeneratorRDMNucleus nuc, const char * unit);
  //    Converts the Nucleus defined by nuc and the associated units of
  //    energy *unit into a G4String.
  void SetParameterName(const char * theNameA,const char * theNameZ,
                        const char * theNameE,
                        G4bool omittable, G4bool currentAsDefault=true);
  //    Identifies the parameter names associated with A, Z, E
  //
  void SetDefaultValue(MGGeneratorRDMNucleus defVal);
  //    Sets the default Nucleus if the command is invoked without any
  //    parameters.
  void SetUnitCandidates(const char * candidateList);
  //    Sets the list of unit candidates
  //
  void SetDefaultUnit(const char * defUnit);
  //    Sets the default unit if the command is invoked without any
  //    parameters.
};
////////////////////////////////////////////////////////////////////////////////
#endif

