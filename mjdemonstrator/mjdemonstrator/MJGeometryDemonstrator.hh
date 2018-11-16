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
 * $Id: MJheaderTemplate.hh,v 1.3 2005-02-15 23:26:55 akbarm Exp $
 *
 * CLASS DECLARATION:  MJVDEMOCrystalAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 *
 *MJ Demonstrator geometry builder
 *
 */
// End class description
//
/**
 * SPECIAL NOTES:
 *
 * If the cables are included, there are overlapping geometries. -Jacquie
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT:
 * FIRST SUBMISSION: Wed Jul  21 17:20:54 EST 2010
 *
 * REVISION:
 *
 * 02-24-2016, Add cryostat assembly messenger class and members
 *             for the cryostat locations, T. Caldwell
 * 04-24-2014, Fixed overlapping geometries, J. MacMullin
 * 07-21-2010, Created, M. Green
 *
 */
//---------------------------------------------------------------------------//
//


#ifndef _MJGEOMETRYDEMONSTRATOR_HH
#define _MJGEOMETRYDEMONSTRATOR_HH


//---------------------------------------------------------------------------//
#include <string>
#include <vector>

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"



//---------------------------------------------------------------------------//
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;
class G4PVPlacement;

class MJGeometryDemonstratorMessenger;
class MJDemoCryostatAssemblyMessenger;

using namespace std;

class MJGeometryDemonstrator: public MGGeometryDetector
{
public:
  MJGeometryDemonstrator(G4String);
  virtual ~MJGeometryDemonstrator();

  G4String GetMaterialsList() { return "MJD"; }
  void ConstructDetector();

  G4bool           IsCavernOn() { return fCavernOn; }
  G4bool           IsMuonVetoOn() { return fMuonVetoOn; }
  G4bool           IsRadShieldOn() { return fRadShieldOn; }
  G4bool           IsInnerCopperOn() {return fInnerCopperOn;}
  G4bool           IsTSShieldOn() {return fTSShieldOn;}
  G4int            GetShadowShieldThickness() { return fShadowShieldThickness; }
  G4String         IsPolyShield() {return fPolyShield;}

  void SetCryostatChoice(G4String choice) {fCryostatChoice = choice;}
  void SetCavern(G4bool cavernOn) { fCavernOn = cavernOn; }
  void SetMuonVeto(G4bool muonVetoOn) { fMuonVetoOn = muonVetoOn; }
  void SetRadShield(G4bool radShieldOn) { fRadShieldOn = radShieldOn; }
  void SetInnerCopper(G4bool IC2On) { fInnerCopperOn = IC2On; }
  void SetTSShield(G4bool TSSOn) { fTSShieldOn = TSSOn; }
  void SetShadowShieldThickness(G4int shadowShieldThickness) { fShadowShieldThickness = shadowShieldThickness; }
  void SetPolyShield(G4String PolyShield) {fPolyShield = PolyShield;}

  G4ThreeVector GetCryo1Pos() const { return fCryo1Pos; }
  G4ThreeVector GetCryo2Pos() const { return fCryo2Pos; }
  G4double GetCryo1Rot() const { return fCryo1Rot; }
  G4double GetCryo2Rot() const { return fCryo2Rot; }
  void SetCryo1Pos(G4ThreeVector cryo1Pos) { fCryo1Pos = cryo1Pos; }
  void SetCryo2Pos(G4ThreeVector cryo2Pos) { fCryo2Pos = cryo2Pos; }
  void SetCryo1Rot(G4double cryo1Rot) { fCryo1Rot = cryo1Rot; }
  void SetCryo2Rot(G4double cryo2Rot) { fCryo2Rot = cryo2Rot; }

  virtual int GetSensitiveVolumeID(const std::string& volName);

private:

  G4VPhysicalVolume* cavernPhys;
  G4LogicalVolume* cavernLog;
  MJGeometryDemonstratorMessenger* fMessenger;
  MJDemoCryostatAssemblyMessenger* fCryoAssemblyMessengerW;
  MJDemoCryostatAssemblyMessenger* fCryoAssemblyMessengerE;

  G4String fCryostatChoice; //for the choice of cryostat
  G4bool   fCavernOn; // turn the cavern on
  G4bool   fMuonVetoOn; // turn the muon veto on
  G4bool   fRadShieldOn; // turn the rad shield on
  G4String fPolyShield; //set PolyShield
  G4String fCryoWSN;
  G4String fCryoESN;
  G4bool   fInnerCopperOn;
  G4bool   fTSShieldOn;
  G4int    fShadowShieldThickness;

  G4ThreeVector fCryo1Pos;
  G4ThreeVector fCryo2Pos;
  G4double fCryo1Rot;
  G4double fCryo2Rot;

};
#endif
