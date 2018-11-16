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
 *      
 * CLASS DECLARATION:  MJGeometryPbShield.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Geometry class for CERN NA55 muon beam dump experiment. See
 * [1] V. Chazal et al., Nucl. Inst. Meth. Phys. Res. A 490, p 334-343 (2002).
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
 * 02-2006: Created, M. Marino
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYPbShield_HH
#define _MJGEOMETRYPbShield_HH

#include "geometry/MGGeometryDetector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include <vector>
using namespace std;

class MJGeometryPbShieldMessenger;
class G4VSolid;

class MJGeometryPbShield : public MGGeometryDetector
{
  public:

    MJGeometryPbShield();
    ~MJGeometryPbShield();

    // builds the detector geometry
    void ConstructDetector();
    bool UsesMJPSQLDatabase() { return true; }  
    
    // Sets the beam dump type according to the options: kGraphite, kCopper, kLead.
    inline void SetOuterShieldDim(G4double aSize) 
      { fOuterShieldDimension = aSize; }
    inline void SetInnerShieldDim(G4double aSize) 
      { fInnerShieldDimension = aSize; }
    inline void SetImpSampRatio(G4double aRatio) 
      { fImpSampRatio = aRatio; }
    inline void SetNumberImpSampRegions(G4int aNum)
      { fNumberImpSampRegions = aNum; }
    
    G4String GetShieldName(G4int);
    
  protected:
   
    G4VSolid* fWorldBox;
    G4double fOuterShieldDimension;
    G4double fInnerShieldDimension;
    G4double fImpSampRatio;
    G4int fNumberImpSampRegions;

    vector<G4VPhysicalVolume*> fPhysicalVolumeVec;
    vector<G4LogicalVolume*> fLogicalVolumeVec;
    vector<G4VSolid*> fSolidVec;

    MJGeometryPbShieldMessenger* fTheMessenger;
};

#endif

