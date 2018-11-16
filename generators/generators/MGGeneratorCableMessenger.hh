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
 * $Id: MGGeneratorCableMessenger.hh,v 1.1 2014-14-07 08:45:48 pandola Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorCableMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger for Cable generator
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
 * AUTHOR: B. Zhu
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * 09-2016, Created, B. Zhu
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORCABLEMESSENGER_HH
#define _MGGENERATORCABLEMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class MGGeneratorCable;

class MGGeneratorCableMessenger : public G4UImessenger
{
public:

  //default constructor
  MGGeneratorCableMessenger(MGGeneratorCable *generator);

  //copy constructor
  MGGeneratorCableMessenger(const MGGeneratorCableMessenger &);

  //destructor
  ~MGGeneratorCableMessenger();

  //public interface
  void SetNewValue(G4UIcommand *cmd, G4String newValue);

  //protected members
protected:

  //private  members
private:
  MGGeneratorCable *fCableGenerator;

  G4UIdirectory *fCableDirectory;

  G4UIcmdWithAnInteger *fACmd;
  G4UIcmdWithAnInteger *fZCmd;
  G4UIcmdWithAString *fSourcePosCmd;
  G4UIcmdWithAString *fSourceTypeCmd;
};
#endif
