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
 * $Id: MJGeometrySLACBD.hh,v 1.7 2007-02-21 08:11:52 mgmarino Exp $
 *      
 * CLASS DECLARATION:  MJGeometrySLACBD.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geometry class for SLAC FFTB facility beam dump experiment. See
 * [1] S. Taniguchi et al., Nucl. Inst. Meth. Phys. Res. A 503, p 595 (2003).
 * [2] S. Roesler et al., Nucl. Inst. Meth. Phys. Res. A 503, p 606 (2003).
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
 * AUTHOR: M. Marino
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 * 
 * 08-2005: Created, M. Marino
 * 08-24-2005: Ported to MaGe, J. Detwiler
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYSLACBD_HH
#define _MJGEOMETRYSLACBD_HH

#include "geometry/MGGeometryDetector.hh"
#include "G4VPhysicalVolume.hh"
#include <vector>
#include <map>
using namespace std;

class G4Box;
class G4Tubs;
class MJGeometrySLACBDMessenger;
class G4VSolid;
enum ESLACBDShieldThickness { kNoShield, kVeryThinShield, kThinShield, kMedShield, kThickShield };

class MJGeometrySLACBD : public MGGeometryDetector
{
  public:

    MJGeometrySLACBD();
    ~MJGeometrySLACBD();
    
    // construct detector materials.
    void ConstructMaterials();
    // construct detector geometry.
    void ConstructDetector();
    bool UsesMJPSQLDatabase() { return false; }  
    
    // Sets the thickness of the shield according to the options: None, Thin, Medium, Thick.
    inline void SetConcShieldThickness(ESLACBDShieldThickness concShieldThickness) 
      { fConcShieldThickness = concShieldThickness; }
    
    // Fills a map for use with the MJOutputSLACBD class
    void FillPhysVolMap(map<const G4VPhysicalVolume*, int> &);
  protected:
    G4String GetShieldName(G4int);
   
    G4int fNumImpSampRegions;
    G4int fNumImpSampSteelRegions;
    G4Box* fWorldBox;
    
    G4Box* fConcBlockBox;
    G4LogicalVolume* fConcBlockLog;
    G4VPhysicalVolume* fConcBlockPhys;

    G4Box* fAirRoomBox;
    G4LogicalVolume* fAirRoomLog;
    G4VPhysicalVolume* fAirRoomPhys;

    G4Box* fSteelSideBox;
    G4LogicalVolume* fSteelSideRightLog;
    G4LogicalVolume* fSteelSideLeftLog;
    G4VPhysicalVolume* fLeftSteelPhys;
    G4VPhysicalVolume* fRightSteelPhys;
    G4Box* fSteelTopBox;
    G4LogicalVolume* fSteelTopLog;
    G4VPhysicalVolume* fSteelTopPhys;
    G4Box* fSteelSideShieldBox;
    G4LogicalVolume* fSteelSideShieldLog; 

    G4Box* fAirSideBox;
    G4LogicalVolume* fAirSideLog;
    G4VPhysicalVolume* fAirSidePhys;

    G4Tubs* fAlumBDTube;
    G4LogicalVolume* fAlumBDLog;
    G4VPhysicalVolume* fAlumBDPhys;

    G4Tubs* fSteelBDTube;
    G4LogicalVolume* fSteelBDLog;
    G4VPhysicalVolume* fSteelBDPhys;

    G4Box* fSteelAwningBox;
    G4LogicalVolume* fSteelAwningLog;
    G4VPhysicalVolume* fSteelAwningPhys;

    G4Box* fConcSideShieldBox;
    G4LogicalVolume* fConcSideShieldLog;

    G4Box* fImpSampShieldBox;
    G4LogicalVolume* fImpSampShieldLog;
    G4VPhysicalVolume* fImpSampShieldPhys;

    vector<G4VPhysicalVolume*> fPhysicalVolumeVec;
    vector<G4LogicalVolume*> fLogicalVolumeVec;
    vector<G4VSolid*> fSolidVec;
    ESLACBDShieldThickness fConcShieldThickness;

    MJGeometrySLACBDMessenger* fTheMessenger;
};

#endif

