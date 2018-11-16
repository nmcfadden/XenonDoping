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
 * $Id: MGGeneratorDoubleBetaMessenger.hh,v 1.1 2004-12-07 08:45:48 pandola Exp $
 *
 * CLASS DECLARATION:  MGGeneratorDoubleBetaMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 *
 * Messenger for Double Beta generator.
 *
 */
// End class description
/**
 * SPECIAL NOTES:
 *  This generator is an implementation of BB-decay event generation via the
 *  theoretical phase space factors and related single electron spectral
 *  calculations of J. Kotila and F. Iachello. The datafiles it uses are
 *  provided directly by Kotila & Iachello and can be downloaded here:
 *  http://nucleartheory.yale.edu/double-beta-decay-phase-space-factors
 *  Their work is published in the article "Phase-space Factors for Double-B
 *  Decay": J. Kotila and F. Iachello, Physical Review C 85, 034316 (2012) and 
 *  nucleartheory.yale.edu
*/
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: massarczyk@lanl.gov
 * CONTACT:
 * FIRST SUBMISSION: 2016
 *
 * REVISION:
 *
 * 03-08-2016, Created, R. Massarczyk
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORDoubleBetaMESSENGER_HH
#define _MGGENERATORDoubleBetaMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class MGGeneratorDoubleBeta;


class MGGeneratorDoubleBetaMessenger : public G4UImessenger
{
public:

  //default constructor
  MGGeneratorDoubleBetaMessenger(MGGeneratorDoubleBeta *generator);

  //copy constructor
  MGGeneratorDoubleBetaMessenger(const MGGeneratorDoubleBetaMessenger &);

  //destructor
  ~MGGeneratorDoubleBetaMessenger();

  //public interface

  void SetNewValue(G4UIcommand *cmd, G4String newValue);

  //protected members
protected:


  //private  members
private:
  MGGeneratorDoubleBeta      *fDoubleBetaGenerator;

  G4UIdirectory              *fDoubleBetaDirectory;
  G4UIcmdWithAnInteger       *fNeutrinoCmd;

};
#endif
