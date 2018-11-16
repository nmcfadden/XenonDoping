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
* $Id: MGGeneratorRDMNucleus.cc,v 1.1 2004-12-07 08:56:06 pandola Exp $
*
* CLASS DECLARATION:  MGGeneratorRDMNucleus.cc
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
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MODULE:              MGGeneratorRDMNucleus.cc
//
// Version:             0.b.3
// Date:                29/02/00
// Author:              F Lei & P R Truscott
// Organisation:        DERA UK
// Customer:            ESA/ESTEC, NOORDWIJK
// Contract:            12115/96/JG/NL Work Order No. 3
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// CHANGE HISTORY
// --------------
//
// 29 February 2000, P R Truscott, DERA UK
// 0.b.3 release.
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////
//
//
#include "generators/MGGeneratorRDMNucleus.hh"
////////////////////////////////////////////////////////////////////////////////
//
MGGeneratorRDMNucleus::MGGeneratorRDMNucleus ()
  : a(24), z(11), e(0.0)   //Default to 24Na radioactivity
{;}
///////////////////////////////////////////////////////////////////////////////
//
MGGeneratorRDMNucleus::MGGeneratorRDMNucleus (G4int a1, G4int z1, G4double e1)
{
  //
  //
  a = a1;
  z = z1;
  e = e1;
}
///////////////////////////////////////////////////////////////////////////////
//
MGGeneratorRDMNucleus::~MGGeneratorRDMNucleus ()
{;}
///////////////////////////////////////////////////////////////////////////////
//
std::ostream &operator << (std::ostream &ss, const MGGeneratorRDMNucleus &q)
//
//
// Definition of the insertion operator << to provide the nucleus limits to
// ostream.
//
{
  ss <<"Atomic weight: " <<q.GetA()
     <<"Atomic number: " <<q.GetZ()
     <<"Excitation energy: "<<q.GetE();
  return ss;
}
///////////////////////////////////////////////////////////////////////////////






