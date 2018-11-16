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
 * CLASS DECLARATION:  MJGeometryKURFBEGeDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: P. Finnerty
 * CONTACT: paddy@physics.unc.edu 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 01-28-2010 Made this a derived class of MJGeometryKURFBEGeDetector, A.
 *            Schubert
 * 04-19-2010 Modified geometry of shielding according to Gary Swift's drawings,
 *            A. Schubert
 * 12-03-2010 Removed borated poly layer per P. Finnerty email from 19 July
 *            2010, made scintillator into 1 m x 1m panel, removed some old
 *            unused code, A. Schubert 
 * 03 Feb 2011 Added rock, A. Schubert
 * 15 Aug 2011 Added rock sampling volume, A. Schubert
 * 30 Aug 2011 Added trailer, more realistic poly geometry, A. Schubert
 * 16 Nov 2011 Added parallel geometry for importance sampling, A. Schubert
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYKURFBEGeDETECTORINSHIELD_HH
#define _MJGEOMETRYKURFBEGeDETECTORINSHIELD_HH

//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"
#include "mjgeometry/MJGeometryKURFBEGeDetector.hh"
//---------------------------------------------------------------------------//

#include "geometry/MGGeometryParallelWorld.hh"
// parallel geometry class, for importance sampling:
// http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch04s07.html
class MJGeometryKURFBEGeDetectorInShieldParallelWorld : public MGGeometryParallelWorld
{
  public:

    MJGeometryKURFBEGeDetectorInShieldParallelWorld();

    virtual ~MJGeometryKURFBEGeDetectorInShieldParallelWorld() {}

    // overloaded from G4VUserParallelWorld: 
    virtual void Construct();  

    // overloaded from MGGeometryParallelWorld:
    virtual G4VPhysicalVolume* GetWorldVolume() const { return fWorldVolume; }

    inline void SetOrigin(G4double x, G4double y, G4double z)
      { fX = x, fY = y; fZ = z; }

    // set up Pb volumes:
    void SetPbMinLength(G4double val) { fPbMinLength = val; }
    void SetNPbVolumes(G4int val) { fNPbVolumes = val; }
    void SetPbDeltaLength(G4double val) { fPbDeltaLength = val; }

    // set up poly volumes:
    void SetPolyMinLength(G4double val) { fPolyMinLength = val; }
    void SetNPolyVolumes(G4int val) { fNPolyVolumes = val; }
    void SetPolyDeltaLength(G4double val) { fPolyDeltaLength = val; }


  private:
    G4VPhysicalVolume* fWorldVolume;

    // top of the cryostat:
    G4double fX;
    G4double fY;
    G4double fZ;

    // volumes:
    G4double fPbMinLength;
    G4double fPbDeltaLength;
    G4int fNPbVolumes;
    G4double fPolyMinLength;
    G4double fPolyDeltaLength;
    G4int fNPolyVolumes;
};

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

class G4Material;
class G4LogicalVolume;
class G4VisAttributes;
class G4VPhysicalVolume;
class MJGeometryKURFBEGeDetectorInShieldMessenger;
class MJGeometryKURFBEGeDetectorInShield : public MJGeometryKURFBEGeDetector
{
public:

  MJGeometryKURFBEGeDetectorInShield(G4String serNum, G4bool messOff);
  virtual ~MJGeometryKURFBEGeDetectorInShield();

  // overloaded from MGGeometryDetector:
  void ConstructDetector();
  bool UsesMJPSQLDatabase() { return true; }  
  virtual MGGeometryParallelWorld* GimmeParallelWorld(); 

  void SetDefaults();

  virtual inline bool GetIsPolyPresent() const { return fIsPolyPresent; }
  virtual inline void SetIsPolyPresent(bool val) { fIsPolyPresent = val; }

  virtual inline bool GetIsDetectorInShield() const { return fIsDetectorInShield; }
  virtual inline void SetIsDetectorInShield(bool val) { fIsDetectorInShield = val; }
 
  // set up Pb volumes:
  void SetPbMinLength(G4double val) { fParallelPbMinLength = val; }
  void SetNPbVolumes(G4int val) { fParallelNPbVolumes = val; }
  void SetPbDeltaLength(G4double val) { fParallelPbDeltaLength = val; }

  // set up poly volumes:
  void SetPolyMinLength(G4double val) { fParallelPolyMinLength = val; }
  void SetNPolyVolumes(G4int val) { fParallelNPolyVolumes = val; }
  void SetPolyDeltaLength(G4double val) { fParallelPolyDeltaLength = val; }

private:

  MJGeometryKURFBEGeDetectorInShieldMessenger* fMessenger;

  // options:
  bool fIsPolyPresent; // whether poly shield is present

  // if false, detector is in position used for source measurements in Aug 2011
  bool fIsDetectorInShield; 

  MJGeometryKURFBEGeDetectorInShieldParallelWorld* fParallelWorld;
  

  G4LogicalVolume* fRockLogical;
  G4LogicalVolume* fRockSamplingLogical;
  G4LogicalVolume* fExperimentalHallLogical;
  G4LogicalVolume* fTrailerLogical;
  G4LogicalVolume* fTrailerAirSpaceLogical;
  G4LogicalVolume* fLeadShieldLogical;
  G4LogicalVolume* fPolyLogical;  
  G4LogicalVolume* fPolyTopLogical;  
  G4LogicalVolume* fInnerAinLeadLogical;
  G4LogicalVolume* fScintillatorLogical;

  G4VPhysicalVolume* fRockPhysical;
  G4VPhysicalVolume* fRockSamplingPhysical;
  G4VPhysicalVolume* fExperimentalHallPhysical;
  G4VPhysicalVolume* fTrailerPhysical;
  G4VPhysicalVolume* fTrailerAirSpacePhysical;
  G4VPhysicalVolume* fLeadShieldPhysical;
  G4VPhysicalVolume* fCryostatPhysical;
  G4VPhysicalVolume* fPolyPhysical;
  G4VPhysicalVolume* fPolyTopPhysical;
  G4VPhysicalVolume* fInnerAinLeadPhysical;
  G4VPhysicalVolume* fScintillatorPhysical;

  G4double fWorldVolumeRadius;
  G4double fPolyThickness;
  G4double fScintillatorThickness;
  G4double fScintillatorWidth;

  // ancient lead 
  // largest dimension of the ancient lead:
  G4double fAncLeadHeight;
  G4double fAncLeadWidth;
  // dimensions of the narrow outer top section:
  G4double fAncLeadNarrowLength;
  G4double fAncLeadNarrowWidth;
  // dimensions of cavity:
  G4double fAncLeadCavityHeight;
  G4double fAncLeadCavityWidth;
  G4double fAncLeadCavityOffsetFromTop;
  G4double fAncLeadHoleRadius;
  G4double fAncLeadZOffset;

  // normal lead
  G4double fLeadShieldWidth; // outer width of normal pb shield
  G4double fLeadShieldHeight;// outer height of nurmal pb shield
  // make a cavity in the pb to accomodate cryostat and cold finger:
  G4double fLeadShieldCavityLength; 
  G4double fLeadShieldCavityWidth; 
  // lower portion of the cavity is wider:
  G4double fLeadShieldWiderCavityLength; 
  G4double fLeadShieldWiderCavityWidth; 
  G4double fLeadShieldWaistLength;
  G4double fLeadShieldWaistWidth;
  G4double fLeadShieldUnderCryostatHeight;

  G4double fInnerCavityWidth;
  G4double fInnerCavityHeight;
  G4double fTotalShieldWidth;
  G4double fTotalShieldHeight;

  G4double fDetectorStandHeight;
  G4double fDetectorStandWidth;
  G4double fRadonExclusionBoxHeight;

  // experimental hall
  G4double fHallHeight;
  G4double fHallLength;
  G4double fHallWidth;


  // small distance to prevent overlap or intersecting surfaces:
  G4double fEps; 

  // parallel world 
  G4double fParallelPbMinLength;
  G4double fParallelPbDeltaLength;
  G4int fParallelNPbVolumes;
  G4double fParallelPolyMinLength;
  G4double fParallelPolyDeltaLength;
  G4int fParallelNPolyVolumes;

 };
#endif
