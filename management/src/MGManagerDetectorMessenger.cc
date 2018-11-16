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
//                                                          
// $Id: MGManagerDetectorMessenger.cc,v 1.28 2009-07-23 18:01:49 awpoon Exp $ 
//      
// CLASS IMPLEMENTATION:  MGManagerDetectorMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: kareem
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jun 30 15:23:21 PDT 2004
 * 
 * REVISION:
 *
 * 06-30-2004, Kareem
 *             Initial file creation and submission
 * 07-04-2004, R. Henning, Added /MJ/geometry/dumpmaterials
 * 11-04-2004  L. Pandola, added command to skip database
 * 11-09-2004  X. Liu, registered new geometry
 * 11-18-2004  X. Liu, registered Gerda and GerdaCrystal
 * 11-24-2004  C. Tomei, registered GerdaCrystalHole
 * 11-26-2004  C. Tomei, registered GerdaArrayHole
 * 12-01-2004  R. Henning, registered LArGe 
 * 01-07-2005  L. Pandola, registered GerdaArrayHolePhaseI
 * 02-16-2005  L. Pandola, revised according to the new MGGeometryDetectorConstruction
 * 04-07-2005  L. Pandola, registered MPIK_LArGe
 * 05-13-2005  L. Pandola, registered GS_LArGe
 * 11-21-2005, R. Henning, registered MJRDCrystalColumn and MJRDCryostat
 * 01-18-2006  L. Pandola, registered MCNPTest
 * 07-25-2006, R. Henning, registered LLNL8x5
 * 10-12-2006, L. Pandola, added new command to define materials from data files
 * 12-12-2006, L. Pandola, added new command to read (simple) geometries from an 
 *                         external file
 * 02-21-2007, A. Schubert, registered MJMEGA
 * 02-22-2007  D. Budjas, added Bruno, Dario and Corrado
 * 06-28-2007  K. Kroeninger, added HadesScintillator
 * 06-28-2007  L. Pandola, registered GDLStand
 * 09-04-2007  L. Pandola, added blank space after candidate "HadesScintillator" 
 *                         (bug fix)
 * 09-28-2007  J. Liu, added GDML reading and writing commands
 * 10-16-2007  A. Schubert, registered UWCountingFacility
 * 10-16-2007  A. Schubert, registered UWOrtecGMX
 * 10-30-2007  J. Liu, added GDML configuration setup commands
 * 01-28-2008  L. Pandola, added command to check for overlapping volumes
 * 01-30-2008  L. Pandola, added command to set verbosity for overlapping volume
 * 06-17-2008  P. Finnerty, registered Melissa-KURF
 * 09-16-2008  R. Henning, added VT-1 and removed redundant candidates for 
 *                          MGGeometryDetectorChoiceCommand.
 * 09-25-2008  K. Macon, added spaces at the end of candidate += strings
 *                        was causing an error for VT-1.
 * 03-02-2008  K. Macon, registered BEGe-KURF detector
 * 05-14-2009  L. Pandola, registered HdBEGe detector
 * 05-22-2009  L. Pandola, registered LNGSBEGe detector
 * 06-09-2009  A. Poon, added Giove
 * 01-26-2010  A. Schubert, registered BEGe-KURF-InShield
 * 03-08-2010  A. Volynets, changes in GDML according to G4GDML - writing to file[s], reading from file
 * 04-06-2010  M. Laubenstein, added GSOR detector
 * 04-07-2010  M. Laubenstein, added GEPAOLO, GEPV, GEMI detectors
 * 07-07-2010  M. Laubenstein, added GECRIS detector
 * 04-07-2010  A. Schubert, registered BEGe-KURF-InRoom
 * 08-31-2010  A. Schubert, registered MGGeometryBEGeDetector
 * 09-07-2010  M. Green, registered MJDemonstrator
 * 02-15-2011  R. Martin, registered TSMJDemModInTank
 * 11-18-2011  A. Schubert, added command to use parallel world
 * 11-23-2011  K. Nguyen, added command to set visibility of physical volumes
 * 11-29-2011 B. Lehnert, registered HADESBEGeTests
 * 06-22-2012 M. Kidd, registered OPPI2 and COPPI geometries
 * 07-30-2012  K. Nguyen, added long-overdue command to get list of physical volumes
 * 08-08-2012  K. Nguyen, changed name of GetListOfPhysicalVolumes() to PrintListOfPhysicalVolumes()
 * 02-25-2013 R. Martin Added PopTop
 * 03-08-2013 W. Xu Added PopTopXYscan
 * 06-12-2013 J. MacMullin, added STC geometry
 * 07-22-2013 A. Wasserman, added Characterization Stand geometry
 * 03-26-2014 J. MacMullin, added Prototype geometry
 * 05-25-2017 M. Green, added LGND_200_Baseline & LGND_1T_Baseline geometries
*/
//---------------------------------------------------------------------------//
//

//  MG headers
#include "management/MGManagerDetectorMessenger.hh"
#include "management/MGManagerDetectorConstruction.hh"
#include "io/MGLogger.hh"
#include "geometry/MGPhysVolNavigator.hh"

//  GEANT4 headers
#include "G4Material.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ParticleTable.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4UImanager.hh"


using namespace std;

//---------------------------------------------------------------------------//
MGManagerDetectorMessenger::MGManagerDetectorMessenger( MGManagerDetectorConstruction *theDet ) :
theDetector( theDet ) 
{
  MGGeometryDir = new G4UIdirectory( "/MG/geometry/" );
  MGGeometryDir->SetGuidance( "UI commands to control which detector is chosen for simulation" );

  // /MG/geometry/detector
  MGGeometryDetectorChoiceCommand = new G4UIcmdWithAString( "/MG/geometry/detector", this );
  MGGeometryDetectorChoiceCommand->SetGuidance( "Select detector type" );
  MGGeometryDetectorChoiceCommand->SetParameterName( "detType", false );
  G4String candidates = "LArGe solidBlock idealCoax ";
  candidates += "GerdaArray MunichTestStand MJ57Banger MPIK_ArGe GS_LArGe ";
  candidates += "UWLArGe SLACBD MJRDCrystalColumn MJRDCryostat ";
  candidates += "MJ17A MJRDBasicShield MCNPTest CERN_NA55 WIPPn LLNL8x5 "; 
  candidates += "GeometryFile MJMEGA Dario Bruno Corrado HadesScintillator ";
  candidates += "GDLStand doubleBlock PbShield VT1-KURF ";
  candidates += "Melissa-KURF BEGe-KURF BEGe-KURF-InShield BEGe-KURF-InRoom "; 
  candidates += "UWCountingFacility UWOrtecGMX SANTA Thermosiphon OPPI2 COPPIs ";
  candidates += "PopTop PopTopXYscan MJCharacterization ";
  candidates += "SANTACollimator simpleString HdBEGe LNGSBEGe Giove ";
  candidates += "MGEDSG MGSOR MGEPAOLO MGEPV MGEMI MGECRIS MGEBER ";
  candidates += "BEGe MJDemonstrator MJSTC MJPrototype ";
  candidates += "LGND_200_Baseline LGND_1T_Baseline ";
  candidates += "BACON ";
  candidates += "TSMJDemModInTank HADESBEGeTests GS_LArGeOptical ";
  MGGeometryDetectorChoiceCommand->SetCandidates(candidates);
  MGGeometryDetectorChoiceCommand->AvailableForStates( G4State_PreInit, 
                                                       G4State_Idle );

  // /MG/geometry/dumpG4materials
  fDumpG4MaterialsCmd = 
    new G4UIcmdWithoutParameter( "/MG/geometry/dumpG4materials", this);
  fDumpG4MaterialsCmd->SetGuidance("Dump materials defined in G4.");
  fDumpG4MaterialsCmd->AvailableForStates(G4State_Idle);

  fUseDatabaseCmd = new G4UIcmdWithABool("/MG/geometry/database",this);
  fUseDatabaseCmd->SetGuidance("Use or not the Database for materials");
  fUseDatabaseCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  fWorldHalfLenCmd = 
    new G4UIcmdWithADoubleAndUnit("/MG/geometry/setWorldHalfLen", this);
  fWorldHalfLenCmd->SetGuidance("Set half-length of world box");
  fWorldHalfLenCmd->SetParameterName("halfLength", false);
  fWorldHalfLenCmd->SetUnitCategory("Length");
  fWorldHalfLenCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fDetectorOffsetCmd =
    new G4UIcmdWith3VectorAndUnit("/MG/geometry/setDetectorOffset", this);
  fDetectorOffsetCmd->SetGuidance("Set the offset of theDetector in World");
  fDetectorOffsetCmd->SetParameterName("x", "y", "z", false);
  fDetectorOffsetCmd->SetUnitCategory("Length");
  fDetectorOffsetCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fAddNewMaterialCmd = new G4UIcmdWithAString( "/MG/geometry/addMaterial",this);
  fAddNewMaterialCmd->SetGuidance("Add a new material from a user file");
  fAddNewMaterialCmd->SetGuidance("The file name should be given as argument");
  fAddNewMaterialCmd->AvailableForStates(G4State_PreInit);

  fAddNewGeometryCmd = new G4UIcmdWithAString( "/MG/geometry/geometryFileName",this);
  fAddNewGeometryCmd->SetGuidance("Set the file name for a file-based geometry");
  fAddNewGeometryCmd->SetGuidance("The file name should be given as argument");
  fAddNewGeometryCmd->AvailableForStates(G4State_PreInit);

  fWorldMaterialCmd =
    new G4UIcmdWithAString( "/MG/geometry/WorldMaterial", this );
  fWorldMaterialCmd->SetGuidance( "Select world material" );
  G4String materialCandidates = "Air Vacuum Lead-Ain Nitrogen-Gas";
  fWorldMaterialCmd->SetCandidates(materialCandidates);
  fWorldMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fEventBiasingDir = new G4UIdirectory("/MG/geometry/EventBias/");
  fEventBiasingDir->SetGuidance("Include event biasing commands");
         
  // /MG/geometry/EventBias/useImportanceSampling
  fUseImportanceSamplingCmd = 
    new G4UIcmdWithABool("/MG/geometry/EventBias/useImportanceSampling",this);
  fUseImportanceSamplingCmd->SetGuidance("Turns on Importance Sampling");
  fUseImportanceSamplingCmd->AvailableForStates(G4State_Idle);

  // /MG/geometry/EventBias/setScorer
  fSetScorerCmd = new G4UIcmdWithAString("/MG/geometry/EventBias/setScorer", this);
  fSetScorerCmd->SetGuidance("Sets scorer algorithm");
  fSetScorerCmd->AvailableForStates(G4State_Idle);
  G4String scorerCandidates = "G4Scorer";
  fSetScorerCmd->SetCandidates(scorerCandidates);
    
  // /MG/geometry/EventBias/setImpSampAlgorithm
  fSetImpSampAlgorithmCmd = 
    new G4UIcmdWithAString("/MG/geometry/EventBias/setImpSampAlgorithm",this);
  fSetImpSampAlgorithmCmd->SetGuidance("Sets Alogrithm for Importance Sampling");
  fSetImpSampAlgorithmCmd->AvailableForStates(G4State_Idle);
  G4String algorithmCandidates = "G4default MGdefault";
  fSetImpSampAlgorithmCmd->SetCandidates(algorithmCandidates);

  // /MG/geometry/EventBias/initializeVarRed
  fInitializeVarRedCmd = 
    new G4UIcmdWithoutParameter("/MG/geometry/EventBias/initializeVarRed",this);
  fInitializeVarRedCmd->SetGuidance("Initializes Variance Reduction.  Rerun after setting change");  
  fInitializeVarRedCmd->AvailableForStates(G4State_Idle);

  // /MG/geometry/EventBias/setBiasedParticle
  fSetBiasedParticleCmd = 
    new G4UIcmdWithAString("/MG/geometry/EventBias/setBiasedParticle",this);
  fSetBiasedParticleCmd->SetGuidance("sets particle to bias");
  fSetBiasedParticleCmd->AvailableForStates(G4State_Idle);
  /*
  G4String biasPartCandidates;

  G4ParticleTable* theParticleTable;
  theParticleTable = G4ParticleTable::GetParticleTable();
  G4int nPtcl = theParticleTable->entries();
  for(G4int i=0;i<nPtcl;i++) {
    G4ParticleDefinition* pd = theParticleTable->GetParticle(i);
    if( !(pd->IsShortLived()) || pd->GetDecayTable() )
    {
      biasPartCandidates += pd->GetParticleName();
      biasPartCandidates += " ";
    }
  }
  fSetBiasedParticleCmd->SetCandidates(biasPartCandidates);
  */
  // /MG/geometry/EventBias/useScoring
  fUseScoringCmd = new G4UIcmdWithABool("/MG/geometry/EventBias/useScoring",this);
  fUseScoringCmd->SetGuidance("Turns scoring on or off");
  fUseScoringCmd->AvailableForStates(G4State_Idle); 
   
  // /MG/geometry/EventBias/printScoringTable
  fPrintScoreTableCmd = new G4UIcmdWithoutParameter("/MG/geometry/EventBias/printScoringTable", this);
  fPrintScoreTableCmd->SetGuidance("Prints Scoring Table");
  fPrintScoreTableCmd->AvailableForStates(G4State_Idle);

  // /MG/geometry/EventBias/deleteMGS -> temp hack!
  fDeleteMGSCmd = new G4UIcmdWithoutParameter("/MG/geometry/EventBias/deleteMGS", this);
  fDeleteMGSCmd->SetGuidance("Deletes mass geometry sampler.");
  fDeleteMGSCmd->AvailableForStates(G4State_Idle);  
  MGGeometryDir = new G4UIdirectory( "/MG/geometry/" );
  MGGeometryDir->SetGuidance( "UI commands to control which detector is chosen for simulation" );

  // /MG/geometry/useParallelWorld
  fUseParallelWorldCmd = new
  G4UIcmdWithABool("/MG/geometry/useParallelWorld", this);
  fUseParallelWorldCmd->SetGuidance("Whether or not to use parallel world.");
  fUseParallelWorldCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  // /MG/geometry/CheckOverlaps
  fOverlapCheckCmd = new G4UIcmdWithoutParameter("/MG/geometry/CheckOverlaps",this);
  fOverlapCheckCmd->SetGuidance("Checks if there are overlapping volumes");
  fOverlapCheckCmd->AvailableForStates(G4State_Idle);

  // /MG/geometry/OverlapVerbosity
  fOverlapCheckVerbosityCmd =  new G4UIcmdWithABool("/MG/geometry/OverlapVerbosity",this);
  fOverlapCheckVerbosityCmd->SetGuidance("Set the verbosity of the output of the check ");
  fOverlapCheckVerbosityCmd->SetGuidance("for overlapping volumes. ");
  fOverlapCheckVerbosityCmd->SetGuidance("default = false (non-overlapping volumes are NOT reported)");
  fOverlapCheckVerbosityCmd->AvailableForStates(G4State_Idle);

  // /MG/geometry/SetVisForPhysVolsMatching
  fSetVisForPhysVolsMatchingCmd = new G4UIcmdWithAString("/MG/geometry/SetVisForPhysVolsMatching",this);
  fSetVisForPhysVolsMatchingCmd->SetGuidance("Set the visibility on/off for all physical volumes that ");
  fSetVisForPhysVolsMatchingCmd->SetGuidance("match the specified name. Search is case-sensitive!");
  G4UIparameter* parameter;
  G4bool omittable;
  fSetVisForPhysVolsMatchingCmd->SetParameterName("physical-volume-name-to-be-matched", omittable = false);
  parameter = new G4UIparameter("depth", 'd', omittable = true);
  parameter->SetDefaultValue(-1);
  parameter->SetGuidance("Depth of propagation (-1 means unlimited depth).");
  fSetVisForPhysVolsMatchingCmd->SetParameter(parameter);
  parameter = new G4UIparameter("visibility", 'b', omittable = true);
  parameter->SetDefaultValue(true);
  fSetVisForPhysVolsMatchingCmd->SetParameter(parameter);
  fSetVisForPhysVolsMatchingCmd->AvailableForStates(G4State_Idle);
  
  // /MG/geometry/PrintListOfPhysicalVolumes
  fPrintListOfPhysVolumesCmd = new G4UIcmdWithoutParameter("/MG/geometry/PrintListOfPhysicalVolumes",this);
  fPrintListOfPhysVolumesCmd->SetGuidance("Print list of names of all physical volumes.");
  fPrintListOfPhysVolumesCmd->AvailableForStates(G4State_Idle);

#ifndef MG_NO_G4GDML
  // /MG/geometry/GDML/
  fGDMLdir = new G4UIdirectory("/MG/geometry/GDML/");
  fGDMLdir->SetGuidance("Commands related to GDML");

  // /MG/geometry/GDML/sourceFile
  fSetGDMLsourceFileCmd = new G4UIcmdWithAString("/MG/geometry/GDML/sourceFile",this);
  fSetGDMLsourceFileCmd->SetGuidance("Set the GDML source file.");
  fSetGDMLsourceFileCmd->AvailableForStates(G4State_PreInit);

  // /MG/geometry/GDML/schemaPath
  fSetGDMLschemaPathCmd = new G4UIcmdWithAString("/MG/geometry/GDML/schemaPath",this);
  fSetGDMLschemaPathCmd->SetParameterName("schema path", true);
  fSetGDMLschemaPathCmd->SetDefaultValue("http://service-spi.web.cern.ch/service-spi/app/releases/GDML/GDML_3_0_0/schema/gdml.xsd");
  fSetGDMLschemaPathCmd->SetGuidance("Set your own GDML schema definition file path.");
  fSetGDMLschemaPathCmd->SetGuidance("By default, it is set to http://service-spi.web.cern.ch/service-spi/app/releases/GDML/GDML_3_0_0/schema/gdml.xsd.xsd");
  fSetGDMLschemaPathCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // /MG/geometry/GDML/outputName
  fSetGDMLoutputNameCmd = new G4UIcmdWithAString("/MG/geometry/GDML/outputName",this);
  fSetGDMLoutputNameCmd->SetParameterName("output file name", true);
  fSetGDMLoutputNameCmd->SetGuidance("Set the name of the GDML output file.");
  fSetGDMLoutputNameCmd->SetGuidance("By default, the output name is \"detectorName.gdml\".");
  fSetGDMLoutputNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // /MG/geometry/GDML/outputFormat
  fSetGDMLoutputFormatCmd = new G4UIcmdWithAString("/MG/geometry/GDML/outputFormat",this);
  fSetGDMLoutputFormatCmd->SetCandidates("true false");
  fSetGDMLoutputFormatCmd->SetParameterName("output format", true);
  fSetGDMLoutputFormatCmd->SetDefaultValue("true");
  fSetGDMLoutputFormatCmd->SetGuidance("Set the format of the GDML output file. Candidates are \"true false\"");
  fSetGDMLoutputFormatCmd->SetGuidance(" true - The names of volumes, materials, etc. in the GDML output file will be concatenated with their logical address in hexadecimal format (save if you have duplicated names)");
  fSetGDMLoutputFormatCmd->SetGuidance(" false - The names will correspond exactly to the ones you have in GEANT4.");
  fSetGDMLoutputFormatCmd->SetGuidance("By default, true is used.");
  fSetGDMLoutputFormatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // /MG/geometry/GDML/modularizeLevels
  fSetGDMLmodularizeLevelsCmd = new G4UIcmdWithAString("/MG/geometry/GDML/modularizeLevels",this);
  fSetGDMLmodularizeLevelsCmd->SetParameterName("List of levels to be modularized", true);
  fSetGDMLmodularizeLevelsCmd->SetGuidance("Specify the geometry levels that you want to modulize.");
  fSetGDMLmodularizeLevelsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
     
  // /MG/geometry/GDML/write
  fGDMLwriteCmd = new G4UIcmdWithoutParameter("/MG/geometry/GDML/write", this);
  fGDMLwriteCmd->SetGuidance("Dump Geant4 geometry to GDML file[s].");
  fGDMLwriteCmd->AvailableForStates(G4State_Idle);
#endif
}

//---------------------------------------------------------------------------//

MGManagerDetectorMessenger::~MGManagerDetectorMessenger() 
{
  delete MGGeometryDetectorChoiceCommand;
  delete fDumpG4MaterialsCmd;
  delete MGGeometryDir;
  delete fUseDatabaseCmd;
  delete fWorldHalfLenCmd;
  delete fDetectorOffsetCmd;
  delete fUseImportanceSamplingCmd;
  delete fSetScorerCmd;
  delete fSetImpSampAlgorithmCmd;
  delete fInitializeVarRedCmd;
  delete fEventBiasingDir;
  delete fSetBiasedParticleCmd;
  delete fUseScoringCmd;
  delete fPrintScoreTableCmd;
  delete fDeleteMGSCmd;
  delete fUseParallelWorldCmd;
  delete fAddNewMaterialCmd;
  delete fAddNewGeometryCmd;
  delete fOverlapCheckCmd;
  delete fOverlapCheckVerbosityCmd;
  delete fSetVisForPhysVolsMatchingCmd;
  delete fPrintListOfPhysVolumesCmd;
#ifndef MG_NO_G4GDML
  delete fGDMLdir;
  delete fSetGDMLsourceFileCmd;
  delete fSetGDMLschemaPathCmd;
  delete fSetGDMLoutputNameCmd;
  delete fSetGDMLoutputFormatCmd;
  delete fSetGDMLmodularizeLevelsCmd;
  delete fGDMLwriteCmd;
#endif
}

//---------------------------------------------------------------------------//

void MGManagerDetectorMessenger::SetNewValue( G4UIcommand *command, G4String newValue ) 
{
  if( command == MGGeometryDetectorChoiceCommand ) {
    theDetector->SetDetector( newValue );
  }
  else if (command == fDumpG4MaterialsCmd) {
    G4cout << *(G4Material::GetMaterialTable());
  }
  else if (command == fAddNewMaterialCmd)  {
    theDetector->AddNewMaterial(newValue);
  }
  else if (command == fAddNewGeometryCmd) {
    theDetector->SetFileNameForGeometry(newValue);
    MGLog(trace) << "File name has been set to " << newValue << endlog;
  }
  else if (command == fUseDatabaseCmd) {
    theDetector->SetDatabaseFlag(fUseDatabaseCmd->GetNewBoolValue(newValue));
  }
  else if (command == fWorldHalfLenCmd) {
    G4double halfLen = fWorldHalfLenCmd->GetNewDoubleValue(newValue);
    theDetector->SetWorldHalfLength(halfLen);
  }
  else if (command == fDetectorOffsetCmd) {
    G4ThreeVector offset = fDetectorOffsetCmd->GetNew3VectorValue(newValue);
    theDetector->SetDetectorOffset(offset);
  }
  else if (command == fWorldMaterialCmd) {
    G4String worldmaterial;
    theDetector->SetWorldMaterial( newValue );
  }
  else if (command == fUseImportanceSamplingCmd) {
    theDetector->UseImportanceSampling(fUseImportanceSamplingCmd->GetNewBoolValue(newValue));
  }
  else if (command == fSetScorerCmd) {
    theDetector->SetScorer(newValue);
  }
  else if (command == fSetImpSampAlgorithmCmd) {
    theDetector->SetImportanceSamplingAlgorithm(newValue);
  }
  else if (command == fInitializeVarRedCmd) {
    theDetector->InitializeVarianceReduction();  
  }
  else if (command ==fSetBiasedParticleCmd) {
    theDetector->SetTheBiasedParticle(newValue);
  }
  else if (command == fUseScoringCmd) {
    theDetector->UseScoring(fUseScoringCmd->GetNewBoolValue(newValue));  
  }
  else if (command == fPrintScoreTableCmd) {
    theDetector->PrintScoreTable();
  }
  else if (command == fDeleteMGSCmd) {
    theDetector->DeleteMGS();
  }
  else if (command == fUseParallelWorldCmd) {
    theDetector->SetUseParallelWorld(newValue);
  }
  else if (command == fOverlapCheckCmd){
    theDetector->CheckOverlaps();
  }
  else if (command == fOverlapCheckVerbosityCmd){
    theDetector->SetCheckOverlapsVerbosity(fOverlapCheckVerbosityCmd->GetNewBoolValue(newValue));
  }
  else if (command == fSetVisForPhysVolsMatchingCmd){
    G4String logVolName = "";
    G4String physVolName = "";
    G4String depth = "";
    G4String boolean = "";

    std::stringstream cmdString(newValue);
    cmdString >> physVolName >> depth >> boolean;
    vector<G4VPhysicalVolume*> physVolsMatching = MGPhysVolNavigator::GetVolumesMatching(physVolName);

    G4String volumeRetrieved = "";
    map<G4String, int> numberFound;
    for(size_t i = 0; i < physVolsMatching.size(); i++){
      volumeRetrieved = (physVolsMatching)[i]->GetName();
      logVolName = physVolsMatching[i]->GetLogicalVolume()->GetName();
      MGLog(debugging) << "Found logical volume named \"" << logVolName 
                       << "\" associated with the physical volume " << volumeRetrieved << endlog;
      numberFound[logVolName] += 1;
    }

    map<G4String, int>::iterator iter;
    G4UImanager* UI = G4UImanager::GetUIpointer();
    for( iter = numberFound.begin(); iter != numberFound.end(); iter++ ){
      MGLog(routine) << "Applying command for " << iter->second 
                     << " physical volumes: /vis/geometry/set/visibility " + iter->first + " " + depth + " " + boolean << endlog;
      UI->ApplyCommand("/vis/geometry/set/visibility " + iter->first + " " + depth + " " + boolean); 
    }
    MGLog(routine) << "=====================================" << endlog;
    MGLog(routine) << "Total number of commands set: " << physVolsMatching.size() << endl;
    MGLog(routine) << "=====================================" << endlog;
  }
  else if (command == fPrintListOfPhysVolumesCmd){
    MGPhysVolNavigator::PrintListOfPhysicalVolumes();
  }

#ifndef MG_NO_G4GDML
  else if (command == fSetGDMLsourceFileCmd) {
    theDetector->setGDMLsourceFile(newValue);
  }
  else if (command == fSetGDMLschemaPathCmd) {
    theDetector->setGDMLschemaPath(newValue);
  }
  else if (command == fSetGDMLoutputNameCmd) {
    theDetector->setGDMLoutputName(newValue);
  }
  else if (command == fSetGDMLoutputFormatCmd) {
    theDetector->setGDMLoutputFormat(newValue);
  }
  else if (command == fSetGDMLmodularizeLevelsCmd) {
    theDetector->setGDMLmodularizeLevels(newValue);
  }
  else if (command == fGDMLwriteCmd) {
    theDetector->GDMLwrite();
  }
#endif
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
