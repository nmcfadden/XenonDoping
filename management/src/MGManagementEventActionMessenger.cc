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
//                                                          
// $Id: MGManagementEventActionMessenger.cc,v 1.37 2009-07-23 18:01:49 awpoon Exp $ 
//      
// CLASS IMPLEMENTATION:  MGManagementEventActionMessenger.cc
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Apr 12 16:26:42 PDT 2004
 * 
 * REVISION:
 *
 * 04-16-2004, Created, R. Henning
 * 07-01-2004, Minor bugfixes, removed hacks for clover testing, R. Henning
 * 07-04-2004, Added commands, R. Henning.
 * 11-08-2004, Changed names to G4UIcommand variables (because now OutputRoot 
 *             inherits from VOutputManager)
 * 11-08-2004, Registered new output scheme (gerda), Xiang
 * 12-07-2004, Modified for Gerda Integration, R. Henning
 * 12-20-2004, Added G4Steps output class, R. Henning
 * 01-30-2005, Added MCEvent, R. Henning
 * 02-05-2005, Added LANLCloverSteps output class, Kareem
 * 02-25-2005, Added GerdaArrayOptical, Luciano
 * 04-07-2005, Added new output scheme for MPIK LArGe stand, Luciano
 * 06/01-2005, Added new output scheme for coincidence teststand, Kevin 
 * 12-27-2005, Added new output scheme for shielding studies, Kareem
 * 01-18-2006, Added a new output scheme (non ROOT-based), Luciano
 * 30-01-2006, Save trajectories for GerdaTeststandCoincidence setup, Xiang
 * 28-02-2006, Added new output scheme for siegfried teststand, Xiang
 * 10-18-2006, Added a new output scheme (ASCII), Luciano
 * 12-12-2006, Added a new output scheme for Detector Efficiencies (ASCII), Luciano
 * 02-22-2007, Added Dario output scheme (written by Luciano), Dusan
 * 03-12-2007, Added neutronHP test output scheme. R. Henning
               Commited to CVS 06-26-2007
 * 04-30-2007, GEOutputTestStand cleaned up, Xiang
 * 05-30-2007, GEOutputTestStand fixed some typo, Daniel
 * 06-08-2007, Added two commands for pre-waveform, Jing Liu
 * 06-29-2007, Added output scheme for HadesScintillator setup, Kevin 
 * 26-11-2007, Added commands for class GEOutputCrystal, Jing Liu
 * 26-11-2007, fixed some typos of GerdaTestStand, Jing Liu
 * 05-02-2008, Added MGOutputParticleValidation, R. Johnson
 * 04-08-2008, Added TimeWindow commands for GEOutputCrystal, Jing Liu
 * 09-06-2009, Added GIOVE test stand, Alan Poon
 * 02-03-2010, Added MGOutputPb210, P. Finnerty
 * 03-31-2009, Added MCRun, A. Schubert
 * 02-11-2011, Added command to handle time windows in GEOutputTestStand, A. Volynets
 * 04-22-2011, Removed LANL clover as part of making MaGe independent of PSQL, R. Henning
 * 12-14-2011, Added HADESBEGeTests output scheme
 * 01-26-2012, Added Fastn output class, M. Kidd
 * 05-23-2012, Removed the option to store trajectories while LAr instrumentation is on, N. Barros
 *
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"        
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh" 
#include "G4UIcommand.hh"   
#include "G4UIcmdWithABool.hh"
#include "G4UImessenger.hh"

#include "management/MGManagementEventAction.hh"
#include "io/MGLogger.hh"
#include "io/MGOutputG4Steps.hh"
#include "io/MGOutputMCRun.hh"
#include "io/MGOutputMCOpticalRun.hh"
#include "mjio/MJOutputLArGe.hh"
#include "mjio/MJOutputMCEvent.hh"
#include "mjio/MJOutputSolidBlock.hh"
#include "mjio/MJOutputFastn.hh"
#include "gerdaio/GEOutputGermaniumArray.hh"
#include "gerdaio/GEOutputLArGeOptical.hh"
#include "gerdaio/GEOutputGermaniumArrayCR.hh"
//#include "gerdaio/GEHbookGermaniumArray.hh"
#include "gerdaio/GEOutputTestStand.hh" 
#include "gerdaio/GEOutputGermaniumArrayOptical.hh"
#include "gerdaio/GEOutputLArGe.hh"
#include "gerdaio/GEOutputMCNPTest.hh"
#include "gerdaio/GEOutputNeutronYield.hh"
#include "gerdaio/GEOutputDetectorEfficiency.hh"
#include "mjio/MJOutputSLACBD.hh"
#include "mjio/MJOutputCERN_NA55.hh"
#include "mjio/MJOutputPbShield.hh"
#include "mjio/MJOutputShielding.hh"
#include "mjio/MJOutputHPNeutronTest.hh"
#include "io/MGOutputGeneralSurfaceSampler.hh"
#include "gerdaio/GEOutputDario.hh"
#include "gerdaio/GEOutputHadesScintillator.hh"
#include "gerdaio/GEOutputCrystal.hh"
#include "gerdaio/GEOutputGiove.hh"
#include "gerdaio/GEOutputHADESBEGeTests.hh"
#include "io/MGOutputRootPreWF.hh"
#include "io/MGOutputParticleValidation.hh"
#include "io/MGOutputPb210.hh"


//---------------------------------------------------------------------------//

#include "management/MGManagementEventActionMessenger.hh"

//---------------------------------------------------------------------------//

MGManagementEventActionMessenger::
  MGManagementEventActionMessenger(MGManagementEventAction *eventaction):
  fEventAction(eventaction)
{
  // /MG/eventaction directory definition.
  fEventDirectory = new G4UIdirectory("/MG/eventaction/");
  fEventDirectory->
    SetGuidance("Parameters for the MGManagementEventAction class");
  fEventDirectory->
    SetGuidance("Controls what happens before, during and after each event.");
  fEventDirectory->
    SetGuidance("Determines output format (Root, etc.) and schema.");

  // /MG/eventaction/getoutputschema
  fGetOutputSchemaCmd = new
    G4UIcmdWithoutParameter("/MG/eventaction/getrootschema",this);
  fGetOutputSchemaCmd->SetGuidance("Returns name of output schema.");

  // /MG/eventaction/rootfilename
  fSetFileNameCmd = new 
    G4UIcmdWithAString("/MG/eventaction/rootfilename", this);
  fSetFileNameCmd->SetGuidance("Name for output file.");

  // /MG/eventaction/rootschema
  fSetSchemaCmd = new 
    G4UIcmdWithAString("/MG/eventaction/rootschema", this);
  fSetSchemaCmd->SetGuidance("Selects output schema for Output file.");
  fSetSchemaCmd->
    SetGuidance("Must be consistent with geometry selected.");
  fSetSchemaCmd->SetParameterName("rootoutputschema", true);
  fSetSchemaCmd->SetDefaultValue("LANLCloverNoPS");
  G4String schemacandidates;
  schemacandidates = 
    "Fastn solidBlock GerdaArray ";
  schemacandidates += "LArGeOptical LArGeOpticalWithTrajectory GerdaArrayWithTrajectory GerdaHbook LArGeNoPS G4Steps MCEvent MCRun MCOpticalRun ";
  schemacandidates += "GerdaArrayOptical GerdaArrayCalibration ";
  schemacandidates += "GerdaTestStandEnergyOnly GerdaTestStandEnergyandHits GerdaTestStandEnergyHitsTrajectories ";
  schemacandidates += "GerdaTestStandCoincidence GerdaTestStandSiegfriedCoincidence ";
  schemacandidates += "MPIK_LArGe SLACBD Shielding GSS MCNPTest CERN_NA55 PbShield ";
  schemacandidates += "NeutronYield DetectorEfficiency HPNeutronTest Dario HadesScintillator GerdaArrayCR ";
  schemacandidates += "Crystal Giove ParticleValidation Pb210 GerdaInstrumentedArray HADESBEGeTests";

  fSetSchemaCmd->SetCandidates(schemacandidates);

  // /MG/eventaction/reportingfrequency
  fSetReportingFrequencyCmd = new 
    G4UIcmdWithAnInteger("/MG/eventaction/reportingfrequency", this);
  fSetReportingFrequencyCmd->SetGuidance(
    "Set number of events between reporting current event #");
  
  fSetWriteOutFrequencyCmd = 
    new G4UIcmdWithAnInteger("/MG/eventaction/writeOutFrequency", this);
  fSetWriteOutFrequencyCmd->SetGuidance("Set number of events between writing out data file.  Will default to reporting frequency.");

  fSetWriteOutFileDuringRunCmd = 
    new G4UIcmdWithABool("/MG/eventaction/writeOutFileDuringRun", this);
  fSetWriteOutFileDuringRunCmd->SetGuidance("Sets data file writing during run.");
  fSetWriteOutFileDuringRunCmd->SetDefaultValue(true);

  // /MG/eventaction/PreWaveformFormat
  fSetPreWaveformFormatCmd = 
     new G4UIcmdWithAString("/MG/eventaction/PreWaveformFormat", this);
  fSetPreWaveformFormatCmd->SetGuidance("Specify the output format for pre-waveform data");
  fSetPreWaveformFormatCmd->SetCandidates("root ascii");
  
  // /MG/eventaction/PreWaveformFileName
  fSetPreWaveformFileNameCmd = new 
    G4UIcmdWithAString("/MG/eventaction/PreWaveformFileName", this);
  fSetPreWaveformFileNameCmd->SetGuidance("Name for pre-waveform output file.");
  fSetPreWaveformFileNameCmd->SetDefaultValue("prewf.root");

  // /MG/eventaction/GerdaTestStand/ directory definition.
  fGerdaTestStandDirectory = new G4UIdirectory("/MG/eventaction/GerdaTestStand/");
  fGerdaTestStandDirectory->SetGuidance("Commands to control simulation using \"GerdaTestStand\" rootschema");

  // /MG/eventaction/GerdaTestStand/useTimeWindow
  fGerdaTestStandUseTimeWindowCmd = new G4UIcmdWithABool("/MG/eventaction/GerdaTestStand/useTimeWindow", this);
  fGerdaTestStandUseTimeWindowCmd->SetGuidance("Set whether to window events into time windows");
  fGerdaTestStandUseTimeWindowCmd->SetDefaultValue(true);
  fGerdaTestStandUseTimeWindowCmd->SetParameterName("switch of using time window", true);

  // /MG/eventaction/GerdaTestStand/setTimeWindow
  fGerdaTestStandSetTimeWindowCmd = new G4UIcmdWithADoubleAndUnit("/MG/eventaction/GerdaTestStand/setTimeWindow", this);
  fGerdaTestStandSetTimeWindowCmd->SetGuidance("Set a time window");
  fGerdaTestStandSetTimeWindowCmd->SetDefaultUnit("second");
  fGerdaTestStandSetTimeWindowCmd->SetUnitCandidates("second millisecond microsecond nanosecond");

  // /MG/eventaction/Crystal/ directory definition.
  fCrystalDirectory = new G4UIdirectory("/MG/eventaction/Crystal/");
  fCrystalDirectory->SetGuidance("Commands to control simulation using \"Crystal\" rootschema");

  // /MG/eventaction/Crystal/save
  fCrystalSaveCmd = new G4UIcmdWithAString("/MG/eventaction/Crystal/save", this);
  fCrystalSaveCmd->SetGuidance("choose information you want to save.");
  fCrystalSaveCmd->SetGuidance("currently available:.");
  fCrystalSaveCmd->SetGuidance("* source: \t info. of source.");
  fCrystalSaveCmd->SetGuidance("* hits: \t info. of hits in sensitive volumes.");
  fCrystalSaveCmd->SetGuidance("* tracks: \t info. of hits everywhere.");
  fCrystalSaveCmd->SetGuidance("* all: \t\t info. of everything.");
  fCrystalSaveCmd->SetDefaultValue("hits");
  fCrystalSaveCmd->SetParameterName("Crystal save command", true);
  fCrystalSaveCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/eventaction/Crystal/useTimeWindow
  fCrystalUseTimeWindowCmd = new G4UIcmdWithABool("/MG/eventaction/Crystal/useTimeWindow", this);
  fCrystalUseTimeWindowCmd->SetGuidance("Set whether to window events into time windows");
  fCrystalUseTimeWindowCmd->SetDefaultValue(true);
  fCrystalUseTimeWindowCmd->SetParameterName("switch of using time window", true);

  // /MG/eventaction/Crystal/setTimeWindow
  fCrystalSetTimeWindowCmd = new G4UIcmdWithADoubleAndUnit("/MG/eventaction/Crystal/setTimeWindow", this);
  fCrystalSetTimeWindowCmd->SetGuidance("Set a time window");
  fCrystalSetTimeWindowCmd->SetDefaultUnit("second");
  fCrystalSetTimeWindowCmd->SetUnitCandidates("second millisecond microsecond nanosecond");

  MGLog(trace) << "Event Action Messenger Defined." << endlog;
}

//---------------------------------------------------------------------------//

MGManagementEventActionMessenger::MGManagementEventActionMessenger
   (const MGManagementEventActionMessenger & other) : G4UImessenger(other)
{;}

//---------------------------------------------------------------------------//

MGManagementEventActionMessenger::~MGManagementEventActionMessenger()
{
  delete fEventDirectory;
  delete fGerdaTestStandDirectory;
  delete fCrystalDirectory;
  delete fGetOutputSchemaCmd;
  delete fSetFileNameCmd;
  delete fSetSchemaCmd;
  delete fSetReportingFrequencyCmd;
  delete fSetWriteOutFrequencyCmd;
  delete fSetWriteOutFileDuringRunCmd;
  delete fSetPreWaveformFormatCmd;
  delete fSetPreWaveformFileNameCmd;
  delete fGerdaTestStandUseTimeWindowCmd;
  delete fGerdaTestStandSetTimeWindowCmd;
  delete fCrystalSaveCmd;
  delete fCrystalUseTimeWindowCmd;
  delete fCrystalSetTimeWindowCmd;
}

//---------------------------------------------------------------------------//

void MGManagementEventActionMessenger::SetNewValue(G4UIcommand *command,
						   G4String newValues)
{
  // /MG/eventaction/reportingfrequency
  if(command == fSetReportingFrequencyCmd) {
    fEventAction->SetReportingFrequency(
       fSetReportingFrequencyCmd->GetNewIntValue(newValues));

  // /MG/eventaction/writeOutFrequency
  } else if(command == fSetWriteOutFrequencyCmd) {
    fEventAction->SetWriteOutFrequency(
      fSetWriteOutFrequencyCmd->GetNewIntValue(newValues));
  // /MG/eventaction/writeOutFileDuringRun
  } else if(command == fSetWriteOutFileDuringRunCmd) {
    fEventAction->SetWriteOutFileDuringRun(
     fSetWriteOutFileDuringRunCmd->GetNewBoolValue(newValues));
    
  // /MG/eventaction/rootfilename 
  } else if(command == fSetFileNameCmd) {
     if(fEventAction->GetOutputManager()) {
        fEventAction->GetOutputManager()->SetFileName(newValues);
        MGLog(debugging) <<"the name of the root file: "<<newValues<<endlog;
     }
     else if (!fEventAction->GetOutputManagerPreWF()) 
        MGLog(error) << "Neither pre-waveform nor normal output class defined."
                     << "If you want to create a normal output file, please define a schema via"
                     << "    /MG/eventaction/rootschema"
                     << "If you want to create a pre-waveform output file, please choose a format via"
                     << "    /MG/eventaction/PreWaveformFormat" << endlog;     
  } 
  // /MG/eventaction/PreWaveformFileName
  else if (command == fSetPreWaveformFileNameCmd) {
     if(fEventAction->GetOutputManagerPreWF()) {
        fEventAction->GetOutputManagerPreWF()->SetFileName(newValues);
        MGLog(debugging) <<"the name of the wf root file: "<<newValues<<endlog;
     }
     else if (!fEventAction->GetOutputManager()) 
        MGLog(error) << "Neither pre-waveform nor normal output class defined."
                     << "If you want to create a normal output file, please define a schema via"
                     << "    /MG/eventaction/rootschema"
                     << "If you want to create a pre-waveform output file, please choose a format via"
                     << "    /MG/eventaction/PreWaveformFormat" << endlog;     
  }
  // /MG/eventaction/rootschema 
  else if(command == fSetSchemaCmd) {
    if(newValues == "solidBlock") {
      fEventAction->SetOutputManager(new MJOutputSolidBlock(true));
      fEventAction->SetOutputName("solidBlock");
      MGLog(routine) << "Solid block output format selected" << endlog; 
    } 
    else if (newValues == "LArGeOptical") {
      fEventAction->SetOutputManager(new GEOutputLArGeOptical(true,false,false));
      fEventAction->SetOutputName("LArGe");
      MGLog(routine) << "LArGe output format selected" << endlog; 
    }
    else if (newValues == "LArGeOpticalWithTrajectory") {
      fEventAction->SetOutputManager(new GEOutputLArGeOptical(true,true,false));
      fEventAction->SetOutputName("LArGe");
      MGLog(routine) << "LArGe output format with trajectory selected" << endlog; 
    }
    else if(newValues == "Fastn") {
      fEventAction->SetOutputManager(new MJOutputFastn(true));
      fEventAction->SetOutputName("Fastn");
      MGLog(routine) << "Fast neutron output format selected" << endlog; 
    } 
    else if (newValues == "GerdaArray") {
      fEventAction->SetOutputManager(new GEOutputGermaniumArray(true,false,false));
      fEventAction->SetOutputName("GerdaArray");
      MGLog(routine) << "Gerda Array output format selected" << endlog; 
    }
    else if (newValues == "GerdaArrayCalibration") {
      GEOutputGermaniumArray* outputarray = new GEOutputGermaniumArray(true, false,false);
      outputarray -> RequireHit(true); 
      fEventAction->SetOutputManager(outputarray);
      fEventAction->SetOutputName("GerdaArray");
      MGLog(routine) << "Gerda Array output format selected" << endlog; 
    }
    else if (newValues == "GerdaArrayOptical") {
      fEventAction->SetOutputManager(new GEOutputGermaniumArrayOptical(true,false));
      fEventAction->SetOutputName("GerdaArrayOptical");
      MGLog(routine) << "Gerda Array Optical output format selected" << endlog; 
    }
    else if (newValues == "GerdaArrayWithTrajectory") {
      fEventAction->SetOutputManager(new GEOutputGermaniumArray(true,true,false));
      fEventAction->SetOutputName("GerdaArray");
      MGLog(routine) << "Gerda Array output format with trajectory selected" << endlog; 
    }
    else if (newValues == "GerdaHbook") {
      //fEventAction->SetOutputManager(new GEHbookGermaniumArray());
      fEventAction->SetOutputName("GerdaArray");
      MGLog(routine) << "Gerda Array Hbook format selected" << endlog; 
    }
    else if (newValues == "GerdaTestStandEnergyOnly") {
      GEOutputTestStand* outputteststand = new GEOutputTestStand(true);
      outputteststand->SetSaveEnergyFlag(true); 
      outputteststand->SetSaveHitsFlag(false);
      outputteststand->SetSaveTrajectoriesFlag(false);
      outputteststand->SetCoincidenceFlag(false);
      outputteststand->SetSiegfriedCoincidenceFlag(false);
      fEventAction->SetOutputManager(outputteststand);
      fEventAction->SetOutputName("GerdaTestStandEnergyOnly");
      MGLog(routine) << "Gerda test stand energy only output format selected" << endlog; 
    }
    else if (newValues == "GerdaTestStand") {
      //      fEventAction->SetOutputManager(new GEOutputTestStand(true,false, false));
      fEventAction->SetOutputName("GerdaTestStand");
      MGLog(routine) << "Gerda test stand output format selected" << endlog; 
    }
    else if (newValues == "GerdaTestStandSimple") {
      //      fEventAction->SetOutputManager(new GEOutputTestStand(true, false, false));
      fEventAction->SetOutputName("GerdaTestStandSimple");
      MGLog(routine) << "Gerda test stand output format selected" << endlog; 
    }
    else if (newValues == "GerdaTestStandEnergyandHits") {
      GEOutputTestStand* outputteststand = new GEOutputTestStand(true);
      outputteststand->SetSaveEnergyFlag(true);
      outputteststand->SetSaveHitsFlag(true);
      outputteststand->SetSaveTrajectoriesFlag(false);
      outputteststand->SetCoincidenceFlag(false);
      outputteststand->SetSiegfriedCoincidenceFlag(false);
      fEventAction->SetOutputManager(outputteststand);
      fEventAction->SetOutputName("GerdaTestStandEnergyandHits");
      MGLog(routine) << "Gerda test stand energy and hits output format selected" << endlog;
    }
    else if (newValues == "GerdaTestStandEnergyHitsTrajectories") {
      GEOutputTestStand* outputteststand = new GEOutputTestStand(true);
      outputteststand->SetSaveEnergyFlag(true);
      outputteststand->SetSaveHitsFlag(true);
      outputteststand->SetSaveTrajectoriesFlag(true);
      outputteststand->SetCoincidenceFlag(false);
      outputteststand->SetSiegfriedCoincidenceFlag(false);
      fEventAction->SetOutputManager(outputteststand);
      fEventAction->SetOutputName("GerdaTestStandEnergyHitsTrajectories");
      MGLog(routine) << "Gerda test stand energy and hits and trajectories output format selected" << endlog;
    }
    else if (newValues == "GerdaTestStandCoincidence") {
      GEOutputTestStand* outputteststand = new GEOutputTestStand(true);
      outputteststand->SetSaveEnergyFlag(true);
      outputteststand->SetSaveHitsFlag(true);
      outputteststand->SetSaveTrajectoriesFlag(true);
      outputteststand->SetCoincidenceFlag(true);
      outputteststand->SetSiegfriedCoincidenceFlag(false);
      fEventAction->SetOutputManager(outputteststand);
      fEventAction->SetOutputName("GerdaTestStandCoincidence");
      MGLog(routine) << "Gerda teststand coincidence output format selected" << endlog; 
    }
    else if (newValues == "GerdaTestStandSiegfriedCoincidence") {
      GEOutputTestStand* outputteststand = new GEOutputTestStand(true);
      outputteststand->SetSaveEnergyFlag(true);
      outputteststand->SetSaveHitsFlag(true);
      outputteststand->SetSaveTrajectoriesFlag(true);
      outputteststand->SetCoincidenceFlag(false);
      outputteststand->SetSiegfriedCoincidenceFlag(true);
      fEventAction->SetOutputManager(outputteststand);
      fEventAction->SetOutputName("GerdaTestStandSiegfriedCoincidence");
      MGLog(routine) << "Gerda teststand siegfried coincidence output format selected" << endlog;
    }
    else if (newValues == "MCNPTest") {
      fEventAction->SetOutputManager(new GEOutputMCNPTest());
      fEventAction->SetOutputName("GerdaMCNPTest");
      MGLog(routine) << "Gerda ASCII output format for MCNP test selected" << endlog; 
    }
    else if (newValues == "G4Steps") {
      fEventAction->SetOutputManager(new MGOutputG4Steps(true));
      fEventAction->SetOutputName("G4Steps");
      MGLog(routine) << "Generic G4 Steps Output selected" << endlog; 
    }
    else if (newValues == "MCRun") {
      fEventAction->SetOutputManager(new MGOutputMCRun(true));
      fEventAction->SetOutputName("MGOutputMCRun");
      MGLog(routine) << " MCRun Output selected" << endlog; 
    }
    else if (newValues == "MCOpticalRun") {
      fEventAction->SetOutputManager(new MGOutputMCOpticalRun(true));
      fEventAction->SetOutputName("MGOutputMCOpticalRun");
      MGLog(routine) << " MCOpticalRun Output selected" << endlog; 
    }
    else if (newValues == "MCEvent") {
      fEventAction->SetOutputManager(new MJOutputMCEvent(true));
      fEventAction->SetOutputName("MCEvent");
      MGLog(routine) << "Generic MC Output selected" << endlog; 
    } 
    else if (newValues == "MPIK_LArGe") {
      fEventAction->SetOutputManager(new GEOutputLArGe(true,false));
      fEventAction->SetOutputName("MPIK_LArGe");
      MGLog(routine) << "MPIK LArGe output format selected" << endlog; 
    }
    else if (newValues == "SLACBD") {
      fEventAction->SetOutputManager(new MJOutputSLACBD(true));
      fEventAction->SetOutputName("SLACBD");
      MGLog(routine) << "SLACBD Output selected" << endlog; 
    } 
    else if(newValues == "Shielding") {
      fEventAction->SetOutputManager(new MJOutputShielding(true));
      fEventAction->SetOutputName("Shielding");
      MGLog( routine ) << "Shielding output selected" << endlog;
    }
    else if(newValues == "GSS") {
      fEventAction->SetOutputManager(new MGOutputGeneralSurfaceSampler);
      fEventAction->SetOutputName("GSS");
      MGLog(routine) << "General Surface Sampler output selected" << endlog;
    }
    else if (newValues == "CERN_NA55") {
      fEventAction->SetOutputManager(new MJOutputCERN_NA55(true));
      fEventAction->SetOutputName("CERN_NA55");
      MGLog(routine) << "CERN_NA55 Output selected" << endlog; 
    } 
    else if (newValues == "PbShield") {
      fEventAction->SetOutputManager(new MJOutputPbShield(true));
      fEventAction->SetOutputName("PbShield");
      MGLog(routine) << "PbShield Output selected" << endlog; 
    } 
    else if (newValues == "NeutronYield") {
      fEventAction->SetOutputManager(new GEOutputNeutronYield());
      fEventAction->SetOutputName("GerdaNeutronYield");
      MGLog(routine) << "Gerda ASCII output format for neutron yield selected" << endlog; 
    }
    else if (newValues == "HPNeutronTest") {
      fEventAction->SetOutputManager(new MJOutputHPNeutronTest());
      fEventAction->SetOutputName("HPNeutronTest");
      MGLog(routine) << "MJ HP Neutron Test" << endlog; 
    }
    else if (newValues == "DetectorEfficiency") {
      fEventAction->SetOutputManager(new GEOutputDetectorEfficiency());
      fEventAction->SetOutputName("DetectorEfficiency");
      MGLog(routine) << "Gerda ASCII output format for detector efficiency selected" << endlog; 
    }
    else if (newValues == "Dario"){
      fEventAction->SetOutputManager(new GEOutputDario());
      fEventAction->SetOutputName("Dario");
      MGLog(routine) << "Dario-detector output scheme for Heidelberg geometries selected " << endlog;
    }
    else if (newValues == "HADESBEGeTests"){
      fEventAction->SetOutputManager(new GEOutputHADESBEGeTests());
      fEventAction->SetOutputName("HADESBEGeTests");
      MGLog(routine) << "HADESBEGeTests output scheme selected " << endlog;
    }    
    else if (newValues == "HadesScintillator"){
      fEventAction -> SetOutputManager(new GEOutputHadesScintillator());
      fEventAction -> SetOutputName("HadesScintillator");
      MGLog(routine) << "Hades Ge/scintillator sandwich setup selected " << endlog;
    }
    else if (newValues == "GerdaArrayCR") {
      fEventAction->SetOutputManager(new GEOutputGermaniumArrayCR(true,false));
      fEventAction->SetOutputName("GerdaArrayCR");
      MGLog(routine) << "Gerda Array Cosmic Ray output format selected" << endlog; 
    }
    else if (newValues == "Crystal") {
      GEOutputCrystal* crystaloutput = new GEOutputCrystal(true);
      fEventAction->SetOutputName("Crystal");
      fEventAction->SetOutputManager(crystaloutput);
      MGLog(routine)<<"\"Crystal\" ROOT schema selected.\n\t "
                    <<"Please use commands in /MG/eventaction/Crystal/ to fine tune your simulation."
                    <<endlog; 
    }
	else if (newValues == "Giove"){
		fEventAction->SetOutputManager(new GEOutputGiove());
		fEventAction->SetOutputName("Giove");
		MGLog(routine) << "Giove-detector output scheme for Heidelberg geometries selected " << endlog;
    }	  
	else if (newValues == "ParticleValidation") {
      fEventAction->SetOutputManager(new MGOutputParticleValidation());
      fEventAction->SetOutputName("ParticleValidation");
      MGLog(routine) << "Particle Validation Output Defined" << endlog; 
    }
	else if (newValues == "Pb210") {
			fEventAction->SetOutputManager(new MGOutputPb210(true));
			fEventAction->SetOutputName("Pb210");
			MGLog(routine) << "Pb210 Output selected" << endlog; 
		} 
    else if (newValues == "GerdaInstrumentedArray") {
      fEventAction->SetOutputManager(new GEOutputGermaniumArray(true,false,true));
      fEventAction->SetOutputName("GerdaArray");
      MGLog(routine) << "Gerda Array output format selected with instrumentation." << endlog;
    }
    /* NB: Disallow trajectory storage with instrumentation. takes too much storage.
    else if (newValues == "GerdaInstrumentedArrayWithTrajectory") {
      fEventAction->SetOutputManager(new GEOutputGermaniumArray(true,true,true));
      fEventAction->SetOutputName("GerdaArray");
      MGLog(routine) << "Gerda Array output format selected with instrumentation and trajectory storing." << endlog;
    }
    */
	else {
      MGLog(warning) << "Unknown Schema. Try again." << endlog;
    }
  }
  // /MG/eventaction/GerdaTestStand/useTimeWindow
  else if (command == fGerdaTestStandUseTimeWindowCmd) {
    if (fEventAction->GetOutputManager()) {
      GEOutputTestStand* outputteststand = dynamic_cast<GEOutputTestStand*>(fEventAction->GetOutputManager());
      if(outputteststand == NULL) {
        MGLog(error)<<"/MG/eventaction/GerdaTestStand/useTimeWindow: output class was not GEOutputTestStand!"<<endlog;
      }
      else outputteststand->SetUseTimeWindow(fGerdaTestStandUseTimeWindowCmd->GetNewBoolValue(newValues));
    }
    else
      MGLog(error)<<"Please specify the ROOT schema first before setting time window flag"<<endlog;

  }
  // /MG/eventaction/GerdaTestStand/setTimeWindow
  else if(command == fGerdaTestStandSetTimeWindowCmd) {
    if (fEventAction->GetOutputManager()) {
      GEOutputTestStand* outputteststand = dynamic_cast<GEOutputTestStand*>(fEventAction->GetOutputManager());
      if(outputteststand == NULL) {
        MGLog(error)<<"/MG/eventaction/GerdaTestStand/setTimeWindow: output class was not GEOutputTestStand!"<<endlog;
      }
      else outputteststand->SetTimeWindow(fGerdaTestStandSetTimeWindowCmd->GetNewDoubleValue(newValues));
    }
    else
      MGLog(error)<<"Please specify the ROOT schema first before setting time window flag"<<endlog;

  }
  // /MG/eventaction/Crystal/save
  else if (command == fCrystalSaveCmd) {
     GEOutputCrystal* crystaloutput;
     
     if (fEventAction->GetOutputManager()) 
        crystaloutput = dynamic_cast<GEOutputCrystal*>(fEventAction->GetOutputManager());
     else {
        crystaloutput = new GEOutputCrystal(true);
        fEventAction->SetOutputName("Crystal");
        fEventAction->SetOutputManager(crystaloutput);        
        MGLog(routine)<<"\"Crystal\" ROOT schema selected."<<endlog;
     }
     
     if (newValues.find("source") != std::string::npos)
        crystaloutput->SetSaveSourceFlag(true);        
     if (newValues.find("hit") != std::string::npos) 
        crystaloutput->SetSaveHitsInSensitiveVolumeFlag(true);
     if (newValues.find("track") != std::string::npos) {
        crystaloutput->SetSaveHitsInSensitiveVolumeFlag(true);
        crystaloutput->SetSaveHitsInOtherVolumeFlag(true);
     }
     if (newValues.find("all") != std::string::npos) {
        crystaloutput->SetSaveSourceFlag(true);        
        crystaloutput->SetSaveHitsInSensitiveVolumeFlag(true);
        crystaloutput->SetSaveHitsInOtherVolumeFlag(true);
     }
     if (newValues == "")
        crystaloutput->SetSaveHitsInSensitiveVolumeFlag(true);
     
     G4String saved = " ";
     if (crystaloutput->SaveSource()) saved += "source ";
     if (crystaloutput->SaveHitsInSensitiveVolume()) saved += "hits ";
     if (crystaloutput->SaveHitsInOtherVolume()) saved += "tracks ";
        MGLog(routine)<<"Information you chose to save:"<<saved<<endlog;
  }
  // /MG/eventaction/Crystal/useTimeWindow
  else if (command == fCrystalUseTimeWindowCmd) {
     GEOutputCrystal* crystaloutput;
     if (fEventAction->GetOutputManager()) 
        crystaloutput = dynamic_cast<GEOutputCrystal*>(fEventAction->GetOutputManager());
     else {
        crystaloutput = new GEOutputCrystal(true);
        fEventAction->SetOutputName("Crystal");
        fEventAction->SetOutputManager(crystaloutput);        
        MGLog(routine)<<"\"Crystal\" ROOT schema selected."<<endlog;
     }
       crystaloutput->SetUseTimeWindow(fCrystalUseTimeWindowCmd->GetNewBoolValue(newValues));
  }
  // /MG/eventaction/Crystal/setTimeWindow
  else if(command == fCrystalSetTimeWindowCmd) {
     GEOutputCrystal* crystaloutput;
     if (fEventAction->GetOutputManager()) 
        crystaloutput = dynamic_cast<GEOutputCrystal*>(fEventAction->GetOutputManager());
     else {
        crystaloutput = new GEOutputCrystal(true);
        fEventAction->SetOutputName("Crystal");
        fEventAction->SetOutputManager(crystaloutput);        
        MGLog(routine)<<"\"Crystal\" ROOT schema selected."<<endlog;
     }
     crystaloutput->SetTimeWindow(fCrystalSetTimeWindowCmd->GetNewDoubleValue(newValues));
  }
  // /MG/eventaction/PreWaveformFormat
  else if(command == fSetPreWaveformFormatCmd) {
     if (newValues == "root") {
        fEventAction->SetOutputManagerPreWF(new MGOutputRootPreWF(true));
        MGLog(routine) << "The pre-waveform data are outputing to a root file" << endlog;
     }
     else if (newValues == "ascii") {
        //fEventAction->SetOutputManagerPreWF(new MGOutputASCIIPreWF(true));        
        MGLog(routine) << "The pre-waveform data are outputing to an ascii file" << endlog;
     }
  }
  
}

//---------------------------------------------------------------------------//

G4String MGManagementEventActionMessenger::GetCurrentValue(G4UIcommand *comm)
{
  G4String cv;
  if(comm == fGetOutputSchemaCmd) {
    cv = fEventAction->GetOutputName();
  }

  return cv;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
