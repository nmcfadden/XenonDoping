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
// $Id: MGGeneratorPrimaryMessenger.cc,v 1.21 2008-06-26 15:47:00 pandola Exp $
//
// CLASS IMPLEMENTATION:  MGGeneratorPrimaryMessenger.cc
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT:
 * FIRST SUBMISSION: Wed Jul  7 17:14:54 PDT 2004
 *
 * REVISION:
 *
 * 07-07-2004, Created, R. Henning
 * 11-05-2004, Registered decay0 generator, Luciano
 * 11-08-2004, Added commands for the management of the source confinement
 * 12-09-2004, Registered cosmicrays generator, Luciano
 * 01-31-2005, Added volumelist command for sampling in lists of volumes, Kevin
 * 04-07-2005, Added command to set position for point-like sources, Luciano
 * 04-08-2005, Bug fix with /MG/generator/position, Luciano
 * 04-27-2005, Changed constructor of MGGeneratorCosmicRays, Luciano
 * 07-14-2005, Added neutron generator, Luciano
 * 09-02-2005, Added Wang neutrons as implemented by K. Hudek, R. Henning
 * 09-07-2005, Added AmBe Generator from K. Kazkaz, R. Henning
 * 12-15-2005, Added MuonGun for dangerous muons, from M.Knapp
 * 12-28-2005, Added enum MGMuonGeneratorType to MGGeneratorCosmicRays.hh, Luciano
 *             added enum MGNeutronGeneratorType to MGGeneratorNeutrons.hh,
 *             Registered interface to Sources4A files
 * 01-21-2006, Added Generic Surface Sampler, J. Detwiler
 * 07-10-2007, Added musun in the candidate list, L. Pandola
 * 03-12-2007, Added two new codes in "confine" to include geometrical volumes or surfaces, Luciano
 * 04-24-2008, Reset the previous initialization if volumelists commands are invoked, Luciano
 * 06-26-2008, Added commands for volumelist sampling (add and clear), Luciano
 * 02-02-1010, Registered MGGeneratorPb210, P. Finnerty
 * 12-12-2015, Added shower from file option to cosmicrays, R. Massarczyk
 * 03-03-2016, Added neutrinoless double beta decay, R. Massarczyk
 * 02-15-2018, Added baryon decay modes, R. Massarczyk
 * 26-22-2018, Added LGND200 calibration, R. Massarczyk
 */

#include "globals.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "generators/MGGeneratorPNNL.hh"
#include "generators/MGGeneratorPrimary.hh"
#include "generators/MGGeneratorRDM.hh"
#include "generators/MGGeneratorG4Gun.hh"
#include "generators/MGGeneratorTUNLFEL.hh"
#include "generators/MGGeneratorMJDCalibration.hh"
#include "generators/MGGeneratorDecay0.hh"
#include "generators/MGGeneratorCosmicRays.hh"
#include "generators/MGGeneratorSPS.hh"
#include "generators/MGGeneratorNeutrons.hh"
#include "generators/MGGeneratorWangNeutrons.hh"
#include "generators/MGGeneratorAmBe.hh"
#include "generators/MGGeneratorGeneralSurfaceSampler.hh"
#include "generators/MGGeneratorPb210.hh"
#include "generators/MGGeneratorMeiHimeMu.hh"
#include "generators/MGGeneratorShowersFromFile.hh"
#include "generators/MGGeneratorDoubleBeta.hh"
#include "generators/MGGeneratorBaryonDecay.hh"
#include "generators/MGGeneratorCable.hh"
#include "generators/MGGeneratorLGNDLiquidArgon.hh"
#include "generators/MGGeneratorBACoNSurfaceMuons.hh"
#include "generators/MGGeneratorLGND200Calibration.hh"

#include "io/MGLogger.hh"

#include "generators/MGGeneratorPrimaryMessenger.hh"

using namespace CLHEP;

MGGeneratorPrimaryMessenger::MGGeneratorPrimaryMessenger
(MGGeneratorPrimary *generator):
  fGeneratorPrimary(generator)
{
  // /MG/generator/ directory definition
  fGeneratorDirectory = new G4UIdirectory("/MG/generator/");
  fGeneratorDirectory->SetGuidance("Control commands for generators:");
  fGeneratorDirectory->SetGuidance("/MG/generator/select: Select generator.");

  // /MG/generator/select command
  fSelectCmd = new G4UIcmdWithAString("/MG/generator/select", this);
  fSelectCmd->SetGuidance("Selects generator for events.");
  fSelectCmd->SetGuidance("Options are:");
  fSelectCmd->SetGuidance("PNNLiso : PNNL model for radioactive isotopes.");
  fSelectCmd->SetGuidance("RDMiso: G4 model for radioactive isotopes.");
  fSelectCmd->SetGuidance("TUNLFEL: TUNL FEL beam.");
  fSelectCmd->SetGuidance("MJDCalibration: Helical Event generator simulating MJD calibration track");
  fSelectCmd->SetGuidance("G4gun: Standard G4 gun.");
  fSelectCmd->SetGuidance("decay0: decay0 generator.");
  fSelectCmd->SetGuidance("cosmicrays: cosmic-ray muons generator.");
  fSelectCmd->SetGuidance("cosmicsurf: cosmics ray muons on surface generator.");
  fSelectCmd->SetGuidance("musun: MUSUN cosmic-ray muons generator.");
  fSelectCmd->SetGuidance("MuonsFromFile: G4 gun for dangerous muons.");
  fSelectCmd->SetGuidance("MeiHimeMu: Mei-Hime cosmic-ray muons generator.");
  fSelectCmd->SetGuidance("ShowersFromFile: Muons and shower from File");
  fSelectCmd->SetGuidance("SPS: Geant 4 SPS Generator.");
  fSelectCmd->SetGuidance("neutronsGS: neutron generator");
  fSelectCmd->SetGuidance("wangneutrons: Wang distribution for cosmogenic neutrons.");
  fSelectCmd->SetGuidance("AmBe: Neutrons from an AmBe source.");
  fSelectCmd->SetGuidance("sources4a: neutron spectra (fission and a,n) read from sources4A.");
  fSelectCmd->SetGuidance("GSS: generic surface sampler.");
  fSelectCmd->SetGuidance("Pb210: photons from Pb210 decay in lead shield.");
  fSelectCmd->SetGuidance("DoubleBeta0: simulate neutrinoless double beta decay");
  fSelectCmd->SetGuidance("DoubleBeta2: simulate double beta decay with neutrinos");
  fSelectCmd->SetGuidance("Cable: Signal and HV cables");
  fSelectCmd->SetGuidance("BaryonDecay: decay of 76Ge via two or three baryon decay");
  fSelectCmd->SetGuidance("LGNDLiquidArgon: Generates events inside the cryostat's liquid argon");
  fSelectCmd->SetGuidance("BACoNSurfaceMuons: Generates muons over BACON");



  G4String candidates = "PNNLiso RDMiso TUNLFEL MJDCalibration G4gun decay0 cosmicrays musun MuonsFromFile MeiHimeMu ShowersFromFile ";
  candidates += "SPS neutronsGS wangneutrons AmBe sources4a GSS Pb210 DoubleBeta0 DoubleBeta2 Cable BaryonDecay LGNDLiquidArgon BACoNSurfaceMuons";
  fSelectCmd->SetGuidance("LGND200Calibration: source in a GERDA-style source container");

  // /MG/generator/name
  fNameCmd = new G4UIcmdWithoutParameter("/MG/generator/name", this);
  fNameCmd->SetGuidance("Returns name of current generator.");

  // /MG/generator/confine
  fConfineCmd = new G4UIcmdWithAString("/MG/generator/confine", this);
  fConfineCmd->SetGuidance("Selects confinement for the source.");
  fConfineCmd->SetGuidance("Options are:");
  fConfineCmd->SetGuidance("noconfined : source not confined");
  fConfineCmd->SetGuidance("volume : source confined in a (physical) volume.");
  fConfineCmd->SetGuidance("volumelist : source confined in a set of volumes with the same part name.");
  fConfineCmd->SetGuidance("volumearray : source confined in a set of volumes.");
  fConfineCmd->SetGuidance("surface : source confined in a surface.");
  fConfineCmd->SetGuidance("surfacelist : source confined in a set of surfaces.");
  fConfineCmd->SetGuidance("geometricalvolume: source confined in a geometrical volume.");
  fConfineCmd->SetGuidance("geometricalsurface: source confined in a geometrical surface.");
  candidates = "noconfined volume volumelist volumearray surface surfacelist ";
  candidates += "geometricalvolume geometricalsurface ";
  fConfineCmd->SetCandidates(candidates);

  // /MG/generator/volume
  fVolumeCmd = new G4UIcmdWithAString("/MG/generator/volume", this);
  fVolumeCmd->SetGuidance("Selects the volume where the source is confined");
  fVolumeCmd->AvailableForStates(G4State_Init,G4State_Idle);

  fVolumeListCmd =  new G4UIcmdWithAString("/MG/generator/volumelist", this);
  fVolumeListCmd -> SetGuidance("Selects the volumelist where the source is confined");
  //  fVolumeListCmd -> AvailableForStates(G4State_Init,G4State_Idle);
  fVolumeListFromCmd =  new G4UIcmdWithAnInteger("/MG/generator/volumelistfrom", this);
  fVolumeListFromCmd -> SetGuidance("Selects the first volume in the list");
  fVolumeListToCmd =  new G4UIcmdWithAnInteger("/MG/generator/volumelistto", this);
  fVolumeListToCmd -> SetGuidance("Selects the last volume in the list");
  fVolumeListAddCmd = new G4UIcmdWithAnInteger("/MG/generator/volumelistadd", this);
  fVolumeListAddCmd->SetGuidance("Add a given volume number in the list");
  fVolumeListAddCmd->SetGuidance("Notice: this is ALTERNATIVE to give the list with from/to");
  fVolumeListClearCmd = new G4UIcmdWithoutParameter("/MG/generator/volumelistclear",this);
  fVolumeListClearCmd->SetGuidance("Clear the current volume list");

  fVolumeArrayAddCmd = new G4UIcmdWithAString("/MG/generator/volumearrayadd",this);
  fVolumeArrayAddCmd->SetGuidance("Add a given volume name in the array");
  fVolumeArrayAddCmd->SetGuidance("Notice: To add a given volume number to a volume list use /MG/generator/volumelistadd");

  // /MG/generator/position
  fPositionCmd = new G4UIcmdWith3VectorAndUnit("/MG/generator/position",this);
  fPositionCmd->SetGuidance("Set starting (fixed) position of the particle.");
  fPositionCmd->SetParameterName("X","Y","Z",true,true);
  fPositionCmd->SetDefaultUnit("cm");
  fPositionCmd->SetUnitCategory("Length");
  fPositionCmd->SetUnitCandidates("microm mm cm m km");

  // /MG/generator/gsspositionsfile
  fGSSPostionsFileCmd = new G4UIcmdWithAString("/MG/generator/gsspositionsfile", this);
  fGSSPostionsFileCmd->SetGuidance("Use vertex positions generated by MaGe's General Surface Sampler.");
  fGSSPostionsFileCmd->SetGuidance("The required argument is the name of the root file holding the GSSTree.");
  fGSSPostionsFileCmd->SetGuidance("If you add more than one file, they will be chained together.");
  fGSSPostionsFileCmd->SetParameterName("filename", false);

  // /MG/generator/gssoffset
  fGSSOffsetCmd = new G4UIcmdWithADoubleAndUnit("/MG/generator/gssoffset", this);
  fGSSOffsetCmd->SetGuidance("Set a possible offset for use with GSS position ");
  fGSSOffsetCmd->SetGuidance("root files. Default value is 0.0 mm");
  fGSSOffsetCmd->SetParameterName("gssoffset", true, true);
  fGSSOffsetCmd->SetDefaultUnit("mm");
  fGSSOffsetCmd->SetUnitCandidates("mm cm m km");
  fGSSOffsetCmd->AvailableForStates(G4State_Idle);

  // /MG/generator/gsseventnumber
  fGSSEventNumberCmd =  new G4UIcmdWithAnInteger("/MG/generator/gsseventnumber", this);
  fGSSEventNumberCmd->SetGuidance("Sets the first event in a GSSTree to use");
  fGSSEventNumberCmd->AvailableForStates(G4State_Idle);
}

MGGeneratorPrimaryMessenger::~MGGeneratorPrimaryMessenger()
{
  delete fGeneratorDirectory;
  delete fNameCmd;
  delete fSelectCmd;
  delete fConfineCmd;
  delete fVolumeCmd;
  delete fVolumeListCmd;
  delete fVolumeListFromCmd;
  delete fVolumeListToCmd;
  delete fVolumeListAddCmd;
  delete fVolumeListClearCmd;
  delete fVolumeArrayAddCmd;
  delete fPositionCmd;
  delete fGSSOffsetCmd;
  delete fGSSEventNumberCmd;
}


G4String MGGeneratorPrimaryMessenger::GetCurrentValue(G4UIcommand *command)
{
  G4String cv;

  // /MG/generator/name
  if(command == fNameCmd) {
    cv = "Current generator is : " +
      fGeneratorPrimary->GetMGGenerator()->GetGeneratorName();
  }

  return cv;
}


void MGGeneratorPrimaryMessenger::SetNewValue(G4UIcommand *command,
					      G4String newValues)
{
  // /MG/generator/select
  if(command == fSelectCmd) {
    if(newValues == "PNNLiso") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorPNNL);
    }
    else if(newValues == "RDMiso") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorRDM);
    }
    else if(newValues == "G4gun") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorG4Gun);
    }
    else if(newValues == "TUNLFEL") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorTUNLFEL);
    }
    else if(newValues == "MJDCalibration") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorMJDCalibration);
    }
    else if(newValues == "decay0") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorDecay0);
    }
    else if(newValues == "cosmicsurf") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorCosmicRays(surf));
    }
    else if(newValues == "cosmicrays") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorCosmicRays(standard));
    }
    else if(newValues == "musun") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorCosmicRays(musun));
    }
    else if(newValues == "MuonsFromFile") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorCosmicRays(file));
    }
    else if(newValues == "MeiHimeMu") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorCosmicRays(meihime));
    }
    else if(newValues == "ShowersFromFile") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorCosmicRays(showerfile));
	  }
    else if(newValues == "SPS") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorSPS);
    }
    else if(newValues == "neutronsGS") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorNeutrons(gransasso));
    }
    else if(newValues == "sources4a") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorNeutrons(sources));
    }
    else if(newValues == "AmBe") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorAmBe);
    }
    else if(newValues == "wangneutrons") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorWangNeutrons);
    }
    else if(newValues == "GSS") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorGeneralSurfaceSampler);
    }
    else if(newValues == "Pb210") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorPb210);
    }
    else if(newValues == "DoubleBeta0") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorDoubleBeta(0));
    }
    else if(newValues == "DoubleBeta2") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorDoubleBeta(2));
    }
    else if(newValues == "Cable") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorCable);
    } 
    else if(newValues == "BaryonDecay") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorBaryonDecay);
    }
    else if(newValues == "LGNDLiquidArgon"){
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorLGNDLiquidArgon);
    }
    else if(newValues == "BACoNSurfaceMuons"){
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorBACoNSurfaceMuons);
    }
    else if(newValues == "LGND200Calibration") {
      fGeneratorPrimary->SetMGGenerator(new MGGeneratorLGND200Calibration() );
    }  
    else MGLog(warning) << "Unknown generator. Please try again." << endlog;
  } 

  // /MG/generator/confine
  if(command == fConfineCmd) {
    if(newValues == "noconfined") {
      fGeneratorPrimary->SetConfinementCode(MGGeneratorPrimary::noconfined);
      MGLog(trace) << "Source not confined" << endlog;
    }
    else if(newValues == "volume") {
      fGeneratorPrimary->SetConfinementCode(MGGeneratorPrimary::volume);
      MGLog(trace) << "Source confined in volume" << endlog;
    }
    else if(newValues == "volumelist") {
      fGeneratorPrimary->SetConfinementCode(MGGeneratorPrimary::volumelist);
      MGLog(trace) << "Source confined in volume list" << endlog;
    }
    else if(newValues == "volumearray") {
      fGeneratorPrimary->SetConfinementCode(MGGeneratorPrimary::volumearray);
      MGLog(trace) << "Source confined in volume array" << endlog;
    }
    else if(newValues == "surface") {
      fGeneratorPrimary->SetConfinementCode(MGGeneratorPrimary::surface);
      MGLog(trace) << "Source confined in skin surface" << endlog;
    }
    else if(newValues == "surfacelist") {
      fGeneratorPrimary->SetConfinementCode(MGGeneratorPrimary::surfacelist);
      MGLog(trace) << "Source confined in skin surface list" << endlog;
    }
    else if(newValues == "geometricalvolume") {
      fGeneratorPrimary->SetConfinementCode(MGGeneratorPrimary::geometricalvolume);
      MGLog(trace) << "Source confined in geometrical volume" << endlog;
    }
    else if (newValues == "geometricalsurface"){
      fGeneratorPrimary->SetConfinementCode(MGGeneratorPrimary::geometricalsurface);
      MGLog(trace) << "Source confined in geometrical surface" << endlog;
    }
  }

  // /MG/generator/volume
  if (command == fVolumeCmd) {
    MGLog(trace) << " choose volume command "<<endlog;
    fGeneratorPrimary->SetVolumeName(newValues);

    G4bool ifound = false;
    G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
    G4int nVolumes = (G4int) volumeStore->size();
    G4String candidateList;
    for(G4int i=0;i<nVolumes;i++) {
      candidateList += (*volumeStore)[i]->GetName();
      candidateList += ", ";
      if ((*volumeStore)[i]->GetName() == newValues) ifound = true;
    }

    if (ifound) {
      if (fGeneratorPrimary->GetConfinementCode() != MGGeneratorPrimary::noconfined) {
        fGeneratorPrimary->SetVolumeName(newValues);
        MGLog(trace) << "Source confined in " << newValues << endlog;
      }
      else MGLog(warning) << "Source not confined: nothing happens " << endlog;
    }
    else {
      MGLog(warning) << "Volume not found " << endlog;
      MGLog(warning) << "The list of volumes is: " << candidateList << endlog;
    }
  }

  // /MG/generator/volumelistfrom
  if (command == fVolumeListFromCmd) {
    G4int n = fVolumeListFromCmd -> GetNewIntValue(newValues);
    fGeneratorPrimary -> SetVolumeListFrom(n);
    fGeneratorPrimary->SetVolumeListInitialized(false);
  }

  // /MG/generator/volumelistto
  if (command == fVolumeListToCmd) {
    G4int n = fVolumeListToCmd -> GetNewIntValue(newValues);
    fGeneratorPrimary -> SetVolumeListTo(n);
    fGeneratorPrimary->SetVolumeListInitialized(false);
  }
  // /MG/generator/volumelistadd
  if (command == fVolumeListAddCmd){
    fGeneratorPrimary->AddVolumeNumberToList(fVolumeListAddCmd->GetNewIntValue(newValues));
    fGeneratorPrimary->SetVolumeListInitialized(false);
  }
  // /MG/generator/volumelistclear
  if (command == fVolumeListClearCmd){
    fGeneratorPrimary->ClearList();
    fGeneratorPrimary->SetVolumeListInitialized(false);
  }
  // /MG/generator/volumelist
  if (command == fVolumeListCmd) {
    MGLog(trace) << " choose volume command "<<endlog;

    G4bool                 ifound      = false;
    G4bool                 jfound      = false;
    G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
    G4int                  nVolumes    = (G4int) volumeStore->size();
    G4String               candidateList;
    G4String               StartVolume;
    G4String               EndVolume;
    char                   fromstring[4];
    char                   tostring[4];

    sprintf(fromstring, "%d", fGeneratorPrimary -> GetVolumeListFrom());
    StartVolume =  newValues;
    StartVolume += "_";
    StartVolume += fromstring;

    sprintf(tostring, "%d", fGeneratorPrimary -> GetVolumeListTo());
    EndVolume =  newValues;
    EndVolume += "_";
    EndVolume += tostring;

    for(G4int i=0;i<nVolumes;i++) {
      candidateList += (*volumeStore)[i]->GetName();
      candidateList += ", ";
      if ((*volumeStore)[i] -> GetName() == StartVolume) ifound = true;
      if ((*volumeStore)[i] -> GetName() == EndVolume) jfound = true;
    }

    if (ifound && jfound) {
      if (fGeneratorPrimary->GetConfinementCode() != MGGeneratorPrimary::noconfined) {
        fGeneratorPrimary -> SetVolumeListName(newValues);
        MGLog(trace) << "Source confined in " << StartVolume << " to " << EndVolume << endlog;
      }
      else MGLog(warning) << "Source not confined: nothing happens " << endlog;
    }
    else {
      if (ifound == false) MGLog(warning) << "Volume " << StartVolume << " not found " << endlog;
      if (jfound == false) MGLog(warning) << "Volume " << EndVolume << " not found " << endlog;
      MGLog(warning) << "The list of volumes is: " << candidateList << endlog;
    }
    fGeneratorPrimary->SetVolumeListInitialized(false);
  }

  // /MG/generator/volumearrayadd
  if (command == fVolumeArrayAddCmd) {
    fGeneratorPrimary->AddVolumeNameToArray(newValues);
  }

  // /MG/generator/position
  if (command == fPositionCmd)
    {
      fGeneratorPrimary->GetMGGenerator()->SetParticlePosition(fPositionCmd->GetNew3VectorValue(newValues));
      MGLog(trace) << "Default starting position set to " << fPositionCmd->GetNew3VectorValue(newValues)/cm
		   << " cm " << endlog;
    }

  // /MG/generator/gsspositionsfile
  if (command == fGSSPostionsFileCmd) {
    fGeneratorPrimary->AddGSSFile(newValues);
  }

  // /MG/generator/gssoffset
  if (command == fGSSOffsetCmd) {
    fGeneratorPrimary->SetGSSOffset(fGSSOffsetCmd->GetNewDoubleValue(newValues));
  }

  // /MG/generator/gsseventnumber
  if (command == fGSSEventNumberCmd) {
    fGeneratorPrimary->SetGSSEventNumber(fGSSEventNumberCmd->GetNewIntValue(newValues));
  }
}

