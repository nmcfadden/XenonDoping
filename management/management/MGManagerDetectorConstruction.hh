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
 * $Id: MGManagerDetectorConstruction.hh,v 1.8 2009-01-17 15:14:12 jasondet Exp $
 *
 * CLASS DECLARATION:  MGManagerDetectorConstruction.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 * Class passed to Geant 4 via SetUserInitialization in main.
 * Determines detector geometry selected by user and builds it.
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
 * FIRST SUBMISSION: Mon Apr 19 14:03:00 PDT 2004
 *
 * REVISION:
 *
 * 04-19-2004, Initialized, R. Henning
 * 05-03-2004, Kareem
 *             First editing
 * 07-02-2004, Kareem
 *	       Now uses the base class MJGeometryDetector instead of
 *             referencing each individual detector (e.g., Clover, 800g, MEGA).
 * 07-15-2004, Added BuildAllMaterialsInDatabase(), R. Henning
 * 11-02-2004, Added LocalTable to skip database, L. Pandola
 * 10-12-2006, Added vector containing the filenames of the new materials to be
 *             build on-the-fly, Luciano
 * 12-12-2006  Modified to include the possibility to read a simple geometry from
 *             an external file, L. Pandola
 * 09-04-2007  Removed the definition of SensitiveRegion, to get rid of the problem reported
 *             by M. Marino if /run/verbose is set to 2. Definition now must be done in the
 *             concrete istances of geometry. Luciano
 * 09-28-2007  Enabled GDML. Jing
 * 10-30-2007  added GDML configuration setup commands. Jing
 * 01-28-2008  added method to check for volume overlaps. Luciano
 * 02-01-2010  removed reference to MJGeometryShield, Reyco Henning
 * 03-08-2010  Modified G4GDML support. Alex
 * 11-18-2011  Added parallel geometry functionality, A. Schubert
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGMANAGERDETECTORCONSTRUCTION_HH
#define _MGMANAGERDETECTORCONSTRUCTION_HH

//---------------------------------------------------------------------------//
//  GEANT4 headers
#include "G4VUserDetectorConstruction.hh"

#include "G4VImportanceAlgorithm.hh"
#include "G4IStore.hh"
#include "G4GeometrySampler.hh"

//  MJ headers
#include "geometry/MGGeometryDetector.hh"
#include "geometry/MGGeometryParallelWorld.hh"

#ifndef MG_NO_G4GDML
#include <vector>
#include <map>

#endif

//  Forward declarations
class G4Box;
class G4LogicalVolume;
class G4VisAttributes;
class G4VPhysicalVolume;
//class MJMaterial;
class MGManagerDetectorMessenger;
class MGGerdaLocalMaterialTable;

//---------------------------------------------------------------------------//

class MGManagerDetectorConstruction : public G4VUserDetectorConstruction {
public:

  //default constructor
  MGManagerDetectorConstruction();


  //destructor
  ~MGManagerDetectorConstruction();

  //public interface
public:
  G4VPhysicalVolume* Construct();
  void BuildAllMJDMaterials();
  void BuildAllLGNDMaterials();
  G4VPhysicalVolume* ConstructDetector();

  inline MGGeometryDetector *GetDetector() {return theDetector;};

// We don't include if MaGe is not compiled with database support
  void SetDatabaseFlag(G4bool xflag) {fUseDatabase = xflag;}
  G4bool GetDatabaseFlag() {return fUseDatabase;}

  void SetDetector(const G4String&);
  void AddNewMaterial(G4String);
  void SetFileNameForGeometry(G4String fname){fFileNameForGeometry = fname;};

  G4String GetFileNameForGeometry(){return fFileNameForGeometry;};

  // Added by M. Marino, implementation of Importance Sampling
  void SetScorer(const G4String&);
  void SetImportanceSamplingAlgorithm(const G4String&);
  void InitializeVarianceReduction();

  void SetTheBiasedParticle( const G4String&);
  void UseImportanceSampling(G4bool theIS) {fUseImportanceSampling = theIS;};
  void UseScoring(G4bool theScoring) {fUseScoring = theScoring;};
  void PrintScoreTable();
  void DeleteMGS();

  // parallel world -- A. Schubert 14 Nov 2011

  // call CreateParallelWorld() in SetUseParallelWorld() and SetDetector() so
  // the code will work regardless of the order of macro commands
  void CreateParallelWorld();
  void SetUseParallelWorld(G4bool val) { fUseParallelWorld = val; CreateParallelWorld(); }

  G4double GetWorldHalfLength() { return fWorldHalfLen; }
  void SetWorldHalfLength(G4double halfLen) { fWorldHalfLen = halfLen; }
  void SetDetectorOffset(G4ThreeVector offset) { fDetectorOffset = offset; }
  void SetWorldMaterial(G4String worldmaterial){fWorldMaterial=worldmaterial;}

   // Added by Jing to deal with GDML reading and writing
#ifndef MG_NO_G4GDML
  void setGDMLsourceFile( G4String );
  void setGDMLschemaPath( G4String );
  void setGDMLoutputName( G4String );
  void setGDMLoutputFormat( G4String );
  void setGDMLmodularizeLevels( G4String );
  void GDMLwrite();
#endif

  //Methods to check for overlapping volumes
  void CheckOverlaps();
  void SetCheckOverlapsVerbosity(G4bool verb){fCheckOverlapsVerbosity = verb;};

  inline void SetMaxStepLimit(G4String physVolName, double maxStep) {
    fPhysVolStepLimits[physVolName] = maxStep;
  }

  //protected members
protected:


  //private  members
private:

  // Added by MGM, 29/09/2005
 // G4VScorer *fTheScorer;
  G4VImportanceAlgorithm *fTheImpSampAlgorithm;
  G4String fTheBiasedParticle;
  G4GeometrySampler *fTheGeomSampler;
  G4bool fUseImportanceSampling;
  G4bool fUseScoring;
  G4bool fImpSampIsInitialized;
  //G4bool fScoringIsInitialized;
  G4IStore* fTheImportanceStore;

  // added by A.G. Schubert, 15 Sep 2011
  G4bool fUseParallelWorld;

  // Added by RH, 7/4/2004
  //MJMaterial   **fMGMaterials;   // Pointer to array of pointers to materials
  //G4int        fNumberOfMaterials;

  G4Box *solidWorld;
  G4LogicalVolume *logicWorld;
  G4VPhysicalVolume *physiWorld;

  //G4VPhysicalVolume *theShieldPhysical;

  MGGeometryDetector *theDetector;
  G4VPhysicalVolume *theDetectorPhysical;
  MGGeometryParallelWorld *fParallelWorld;

  //  Messenger
  MGManagerDetectorMessenger* detectorMessenger;

private:
  // Added by L. Pandola in order to skip DataBase
  G4bool fUseDatabase;
  MGGerdaLocalMaterialTable* localTable;
  std::vector<G4String>* fExternalMaterials;

  G4VisAttributes   *fWorldVisAtt;
  G4double fWorldHalfLen;
  G4ThreeVector fDetectorOffset;
  G4String fWorldMaterial;

  G4String fFileNameForGeometry;

//  Added by Jing to deal with GDML reading and writing
//  Modified by Alex for G4-GDML
#ifndef MG_NO_G4GDML
  G4String fGDMLsourceFile;
  G4String fGDMLschemaPath;
  G4String fGDMLdefaultOutput;
  G4String fGDMLoutputName;
  G4String fGDMLoutputFormat;
  std::vector<int> fGDMLlevels;
#endif

  G4bool fCheckOverlapsVerbosity;

  std::map<G4String, double> fPhysVolStepLimits;
};
#endif
