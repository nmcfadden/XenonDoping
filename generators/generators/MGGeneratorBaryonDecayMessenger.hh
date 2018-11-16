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
//
// $Id: MGGeneratorBaryonDecay.cc,v 1.4 2007-02-23 09:24:09 mgmarino Exp $
//
// CLASS IMPLEMENTATION:  MGGeneratorBaryonDecay.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * This generator should help us to understand several decay modes that were 
 * investigated for the baryon decay search
 * deltaB = 3
 * case 1 3n->73Ge pi0 neutrino
 * Double_t particle_mass[3] = {72.9235*amu, 0.135, 0.000000000120};
 * case 2 2np->73Ga pi0 e+ 
 * Double_t particle_mass[3] = {72.9251*amu, 0.135, 0.000000511};
 * case 3 2pn->73Zn pi+ e+ 
 * Double_t particle_mass[3] = {72.9298*amu, 0.1395, 0.000000511};
 * case 4 3p->73Cu pi+ pi+ e+ 
 * Double_t particle_mass[4] = {72.9367*amu, 0.1395, 0.1395, 0.000000511};
 *
 * deltaB = 2
 * case 5 nn->74Ge pi0 pi0
 * Double_t particle_mass[3] = {73.9211*amu, 0.135, 0.135};
 * case 6 nn->74Ge pi+ pi-
 * Double_t particle_mass[3] = {73.9211*amu, 0.1395, 0.1395};
 * case 7 np->74Ga pi0 pi+
 * Double_t particle_mass[3] = {73.9269*amu, 0.135, 0.1395};
 * case 8 pp->74Zn pi+ pi+
 * Double_t particle_mass[3] = {73.9294*amu, 0.1395, 0.1395};
*/
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: massarczyk@lanl.gov
 * CONTACT:
 * FIRST SUBMISSION: 2018
 *
 * REVISION:
 *
 * 02-15-2018, Created, R. Massarczyk
 *
 */
//---------------------------------------------------------------------------//
//

#ifndef _MGGeneratorBaryonDecayMESSENGER_HH
#define _MGGeneratorBaryonDecayMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class MGGeneratorBaryonDecay;


class MGGeneratorBaryonDecayMessenger : public G4UImessenger
{
public:

  //default constructor
  MGGeneratorBaryonDecayMessenger(MGGeneratorBaryonDecay *generator);

  //copy constructor
  MGGeneratorBaryonDecayMessenger(const MGGeneratorBaryonDecayMessenger &);

  //destructor
  ~MGGeneratorBaryonDecayMessenger();

  //public interface

  void SetNewValue(G4UIcommand *cmd, G4String newValue);

  //protected members
protected:


  //private  members
private:
  MGGeneratorBaryonDecay      *fBaryonDecayGenerator;

  G4UIdirectory              *fBaryonDecayDirectory;
  G4UIcmdWithAnInteger       *fDecayMode;

};
#endif
