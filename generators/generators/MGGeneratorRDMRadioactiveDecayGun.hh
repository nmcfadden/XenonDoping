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
* $Id: MGGeneratorRDMRadioactiveDecayGun.hh,v 1.1 2004-12-07 08:45:48 pandola Exp $
*
* CLASS DECLARATION:  MGGeneratorRDMRadioactiveDecayGun.hh
*
*---------------------------------------------------------------------------//
*
* DESCRIPTION:
*
*/
// Begin description of class here
/**
*  MG Class build from the G4 RDM (Radioactive Decay Model) package. 
*  
*  Usage:
*  G4ParticleGun *aGun = new MGGeneratorRDMRadioactiveDecayGun();
*  
*  Once activiated, one can do "help" within G4, and look under the
*  /grdm/ category for various options. Consult the G4 manuals for detail.
*
*  Set up parent nuclues:
*  /grdm/ion A Z 0	(e.g. Ge68 will be - /grdm/ion 68 32 0)
*
*  Other than that it inheritted the behavior from the G4ParticlGun.
*
*	ydc 20040610
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
#ifndef MGGeneratorRDMRadioactiveDecayGun_h
#define MGGeneratorRDMRadioactiveDecayGun_h 1
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MODULE:              MGGeneratorRDMRadioactiveDecayGun.hh
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
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "globals.hh"

#include "generators/MGGeneratorRDMNucleus.hh"
#include "generators/MGGeneratorRDMRadioactiveDecayGunMessenger.hh"

class MGGeneratorRDMRadioactiveDecayGunMessenger;
////////////////////////////////////////////////////////////////////////////////
//
class MGGeneratorRDMRadioactiveDecayGun : public G4ParticleGun
{
  // class description
  // The MGGeneratorRDMRadioactiveDecayGun is an inherited version of G4ParticleGun
  // to allow user to specify an isotope as the initial tracking particle.
  // class description - end

public:
  MGGeneratorRDMRadioactiveDecayGun();
  ~MGGeneratorRDMRadioactiveDecayGun();

public: // with description

  void  SetNucleus(MGGeneratorRDMNucleus theIon1);
  // Sets the isotope.
  //
    inline MGGeneratorRDMNucleus GetNucleus() {return theIon;}
  // Returns the specified isotope.
  //
private:

  MGGeneratorRDMRadioactiveDecayGunMessenger  *theMGGeneratorRDMRadioactiveDecayGunMessenger;

  MGGeneratorRDMNucleus theIon;

};
#endif
////////////////////////////////////////////////////////////////////////////////




