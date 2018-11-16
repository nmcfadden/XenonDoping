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
 * CLASS DECLARATION:  GEGeometryStringStuff.hh
 *
 * DESCRIPTION:
 *
 *   A class that should contain all stuff that differs from one string to another,
 *   like e.g. the electronics bar length
 *
 * AUTHOR: Jens Schubert
 *
 * REVISION: MM-DD-YYYY
 *
 *   06-05-2008, first version, Jens
 *   07-09-2008, (jens) added phase1-cables
 */


#ifndef _GEGEOMETRYSTRINGSTUFF_HH
#define _GEGEOMETRYSTRINGSTUFF_HH

//---------------------------------------------------------------------------

#include <G4Types.hh>
#include <vector>

//---------------------------------------------------------------------------

class G4UnionSolid;
class G4SubtractionSolid;
class G4Trap;
class G4LogicalVolume;
class G4Material;
class G4VisAttributes;

class GEGeometryStringStuff {

public:
  GEGeometryStringStuff();
  ~GEGeometryStringStuff();

  // Getters
  // -----------
  G4double GetElectronicsSideBarZPosition() const
    { return fElectronicsSideBarZPosition; }

  G4double GetElectronicsSideBarHeight() const
    { return fElectronicsSideBarHeight; }

  G4LogicalVolume* GetElectronicsSideBarLogical() const
    { return fElectronicsSideBarLogical; }

  G4double GetElectronicsSideBarXOffset() const
    { return fElectronicsSideBarXOffset; }

  G4LogicalVolume* Get_Phase1_CableLowerPart_Logical(int iRow)
    { return fPhase1_CableLowerPart_Logical[iRow]; }

  std::vector<G4LogicalVolume*>* Get_Phase1_CableLowerPart_Logical()
    { return &fPhase1_CableLowerPart_Logical; }

  G4LogicalVolume* Get_Phase1_CableUpperPart_Logical(int iRow)
    { return fPhase1_CableUpperPart_Logical[iRow]; }

  std::vector<G4LogicalVolume*>* Get_Phase1_CableUpperPart_Logical()
    { return &fPhase1_CableUpperPart_Logical; }  

  // Setters
  // -----------
  void SetElectronicsSideBarZPosition(const G4double z)
    { fElectronicsSideBarZPosition = z; }

  void SetElectronicsSideBarHeight(const G4double height)
    { fElectronicsSideBarHeight = height; }

  void SetElectronicsSideBarXOffset(const G4double offset)
    { fElectronicsSideBarXOffset = offset; }

  void SetElectronicsSideBarLogical(G4LogicalVolume* vol)
    { fElectronicsSideBarLogical = vol; }

  void Set_Phase1_CableLowerPart_Logical(G4LogicalVolume* logVol,int iRow)
    { fPhase1_CableLowerPart_Logical[iRow] = logVol; }

  void Set_Phase1_CableUpperPart_Logical(G4LogicalVolume* logVol,int iRow)
    { fPhase1_CableUpperPart_Logical[iRow] = logVol; }

private:
    // data members
    // ------------------
  G4double             fElectronicsSideBarZPosition;
  G4double             fElectronicsSideBarHeight;
  G4double             fElectronicsSideBarXOffset;

  G4LogicalVolume*     fElectronicsSideBarLogical;

  std::vector<G4LogicalVolume*>  fPhase1_CableLowerPart_Logical;
  std::vector<G4LogicalVolume*>  fPhase1_CableUpperPart_Logical;
};


#endif

