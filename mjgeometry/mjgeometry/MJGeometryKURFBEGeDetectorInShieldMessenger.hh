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
 * CLASS DECLARATION:  MJGeometryKURFBEGeDetectorInShieldMessenger.hh
 *
 * DESCRIPTION: 
 *
 * Messenger for the MJGeometryKURFBEGeDetectorInShield class.
 *
 * AUTHOR: alexis
 * 
 * REVISION:
 * 08-27-2010, Creation, A. Schubert
 */

#ifndef _MJGEOMETRYKURFBEGEDETECTORMESSENGERINSHIELD_HH
#define _MJGEOMETRYKURFBEGEDETECTORMESSENGERINSHIELD_HH

#include "G4UImessenger.hh"


class MJGeometryKURFBEGeDetectorInShield;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcommand;


class MJGeometryKURFBEGeDetectorInShieldMessenger : public G4UImessenger 
{
public:

  //default constructor
  MJGeometryKURFBEGeDetectorInShieldMessenger(
      MJGeometryKURFBEGeDetectorInShield* kurfBEGeDetectorInShield);

  //destructor
  ~MJGeometryKURFBEGeDetectorInShieldMessenger();

  //public interface
  void SetNewValue(G4UIcommand* cmd, G4String nV);

  //protected members
protected:

  G4UIdirectory*                        fDirectory;
  MJGeometryKURFBEGeDetectorInShield*   fKURFBEGeDetectorInShield;
  G4UIcmdWithABool*                     fIsPolyPresentCmd;
  G4UIcmdWithABool*                     fIsDetectorInShieldCmd;

};

#endif

