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
 * $Id: MGGeneratorTUNLFELMessenger.hh,v 1.1 2004-12-07 08:45:48 pandola Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorTUNLFELMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger for TUNL FEL generator.
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Aug  4 11:23:24 PDT 2004
 * 
 * REVISION:
 * 
 * 08-04-2004, Created, R. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORTUNLFELMESSENGER_HH
#define _MGGENERATORTUNLFELMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class MGGeneratorTUNLFEL;

class MGGeneratorTUNLFELMessenger : public G4UImessenger
{
public:

  //default constructor
  MGGeneratorTUNLFELMessenger(MGGeneratorTUNLFEL *generator);

  //copy constructor
  MGGeneratorTUNLFELMessenger(const MGGeneratorTUNLFELMessenger &);

  //destructor
  ~MGGeneratorTUNLFELMessenger();

  //public interface

  G4String GetCurrentValue(G4UIcommand *cmd);
  void SetNewValue(G4UIcommand *cmd, G4String newValue);

  //protected members
protected:


  //private  members
private:
  MGGeneratorTUNLFEL         *fTUNLFELGenerator;

  G4UIdirectory              *fTUNLFELDirectory;

  G4UIcmdWithoutParameter    *fDumpCmd;
  G4UIcmdWithADoubleAndUnit  *fEnergySigmaCmd;
  G4UIcmdWithADoubleAndUnit  *fMajorSigmaCmd;
  G4UIcmdWithADoubleAndUnit  *fMeanEnergyCmd;
  G4UIcmdWithADoubleAndUnit  *fMinorSigmaCmd;
  G4UIcmdWith3VectorAndUnit  *fOriginCmd;
  G4UIcmdWithADouble         *fRhoCmd;

};
#endif
