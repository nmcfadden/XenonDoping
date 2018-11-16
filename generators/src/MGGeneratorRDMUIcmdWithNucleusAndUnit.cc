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
* $Id: MGGeneratorRDMUIcmdWithNucleusAndUnit.cc,v 1.2 2005-12-20 00:00:33 jasondet Exp $
*
* CLASS DECLARATION:  MGGeneratorRDMUIcmdWithNucleusAndUnit.cc
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
#include "G4UIcommand.hh"

#include "generators/MGGeneratorRDMUIcmdWithNucleusAndUnit.hh"

#include <sstream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//
MGGeneratorRDMUIcmdWithNucleusAndUnit::MGGeneratorRDMUIcmdWithNucleusAndUnit
(const char * theCommandPath,G4UImessenger * theMessenger)
:G4UIcommand(theCommandPath,theMessenger)
{
  G4UIparameter * intParamA = new G4UIparameter('i');
  SetParameter(intParamA);
  G4UIparameter * intParamZ = new G4UIparameter('i');
  SetParameter(intParamZ);
  G4UIparameter * dblParamE = new G4UIparameter('d');
  SetParameter(dblParamE);
  G4UIparameter * untParam = new G4UIparameter('s');
  SetParameter(untParam);
  untParam->SetParameterName("Unit");
}
////////////////////////////////////////////////////////////////////////////////
//
MGGeneratorRDMUIcmdWithNucleusAndUnit::~MGGeneratorRDMUIcmdWithNucleusAndUnit()
{
  ;
}
////////////////////////////////////////////////////////////////////////////////
//
MGGeneratorRDMNucleus MGGeneratorRDMUIcmdWithNucleusAndUnit::GetNewNucleusValue(G4String paramString)
{
  G4int a;
  G4int z;
  G4double e;
  char unts[30];

  istringstream is(paramString);
  is >> a >> z >> e >>unts;
  G4String unt = unts;

  return MGGeneratorRDMNucleus(a,z,e*ValueOf(unt));
}

G4double MGGeneratorRDMUIcmdWithNucleusAndUnit::GetNewUnitValue(G4String paramString)
{
  G4int a;
  G4int z;
  G4double e;  

  char unts[30];
  
  istringstream is(paramString);
  is >> a >> z >> e  >> unts;

  G4String unt = unts;
  
  return ValueOf(unt);
}

////////////////////////////////////////////////////////////////////////////////
//
G4String MGGeneratorRDMUIcmdWithNucleusAndUnit::ConvertToString(MGGeneratorRDMNucleus def, 
						    const char *unitName)
{
  G4double uv = ValueOf(unitName);

  ostringstream os;
  os << def.GetA() << " " << def.GetZ()
     << " "<< def.GetE()/uv<<" "<< unitName <<  '\0';
  return G4String(os.str());
}                         
////////////////////////////////////////////////////////////////////////////////
//
void MGGeneratorRDMUIcmdWithNucleusAndUnit::SetParameterName
(const char * theNameA, const char * theNameZ,
const char * theNameE,G4bool omittable,G4bool currentAsDefault)
{
  G4UIparameter * theParamA = GetParameter(0);
  theParamA->SetParameterName(theNameA);
  theParamA->SetOmittable(omittable);
  theParamA->SetCurrentAsDefault(currentAsDefault);
  G4UIparameter * theParamZ = GetParameter(1);
  theParamZ->SetParameterName(theNameZ);
  theParamZ->SetOmittable(omittable);
  theParamZ->SetCurrentAsDefault(currentAsDefault);
  G4UIparameter * theParamE = GetParameter(2);
  theParamE->SetParameterName(theNameE);
  theParamE->SetOmittable(omittable);
  theParamE->SetCurrentAsDefault(currentAsDefault);
}
////////////////////////////////////////////////////////////////////////////////
//
void MGGeneratorRDMUIcmdWithNucleusAndUnit::SetDefaultValue(MGGeneratorRDMNucleus def)
{
  G4UIparameter * theParamA = GetParameter(0);
  theParamA->SetDefaultValue(def.GetA());
  G4UIparameter * theParamZ = GetParameter(1);
  theParamZ->SetDefaultValue(def.GetZ());
  G4UIparameter * theParamE = GetParameter(2);
  theParamE->SetDefaultValue(def.GetE());
}


void MGGeneratorRDMUIcmdWithNucleusAndUnit::SetUnitCandidates(const char * candidateList)
{
  G4UIparameter * untParam = GetParameter(3);
  G4String canList = candidateList;
  untParam->SetParameterCandidates(canList);
}

void MGGeneratorRDMUIcmdWithNucleusAndUnit::SetDefaultUnit(const char * defUnit)
{
  G4UIparameter * untParam = GetParameter(3);
  untParam->SetOmittable(true);
  untParam->SetDefaultValue(defUnit);
}












