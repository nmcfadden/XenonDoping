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

#ifndef MGPhysVolNavigator_h
#define MGPhysVolNavigator_h 1

#include "G4String.hh"
#include "G4ThreeVector.hh"

class G4VPhysicalVolume;

class MGPhysVolNavigator 
{
  public:
    static G4VPhysicalVolume* GetVolumeNamed(G4String volName, G4int copyNumber = -1);
    static std::vector<G4VPhysicalVolume*> GetVolumesMatching(G4String volName);
    static G4double GetBoundingRadius(G4VPhysicalVolume* volume);
    static G4ThreeVector GetGlobalTranslation(G4VPhysicalVolume* volume);
    static G4VPhysicalVolume* FindDirectMother(G4VPhysicalVolume* volume);
    static G4String GetListOfVolumes();
    static void PrintListOfPhysicalVolumes(); // FIXME: Added by K. Nguyen
};
#endif
