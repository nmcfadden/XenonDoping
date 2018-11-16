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
 * $Id: MGGeneratorPNNLMessenger.hh,v 1.2 2004-12-16 23:40:24 dimarcom Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorPNNLMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger for PNNL generators.
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Fri Jul  9 10:41:06 PDT 2004
 * 
 * REVISION:
 * 
 * 07-09-2004, Created, R. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORPNNLMESSENGER_HH
#define _MGGENERATORPNNLMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3Vector;
class MGGeneratorPNNL;

//---------------------------------------------------------------------------//

class MGGeneratorPNNLMessenger : public G4UImessenger
{
public:

  //default constructor
  MGGeneratorPNNLMessenger(MGGeneratorPNNL *generator);

  //copy constructor
  MGGeneratorPNNLMessenger(const MGGeneratorPNNLMessenger &);

  //destructor
  ~MGGeneratorPNNLMessenger();

  //public interface
  void SetNewValue(G4UIcommand* cmd, G4String newValue);

  //protected members
protected:


  //private  members
private:
  MGGeneratorPNNL*           fPNNLGenerator;

  G4UIdirectory*             fPNNLDirectory;     // /MG/generator/PNNL/

  G4UIcmdWithAString*        fPNNLInitializeCmd; 
  // /MG/generator/PNNL/initialize

  G4UIcmdWithAnInteger       *fPNNLReportingFrequencyCmd;
  // /MG/generator/PNNL/reportingfrequency

  G4UIcmdWithABool         *fPNNLGenerateBetaCmd;
  // /MG/generator/PNNL/generatebeta

  G4UIcmdWithADouble         *fPNNLSourceAgeCmd;
  // /MG/generator/PNNL/sourceage

  G4UIcmdWith3Vector         *fPNNLPositionCmd;           
  // /MG/generator/PNNL/position
};
#endif
