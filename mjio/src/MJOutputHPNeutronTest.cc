//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
// $Id: MJOutputHPNeutronTest.cc,v 1.5 2008-02-26 18:26:20 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MJOutputHPNeutronTest.cc
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Mar 12 15:15:32 EDT 2007
 * 
 * REVISION:
 *
 * 03-12-2007, Created, R. Henning
 * 01-29-2012, S. MacMullin - Changed elastic xs to work with any target, not just n,p scattering. 
 * Fixed error in theta histogram to allow calculation of the differential elastic cross section. Changed axis on theta histogram to degrees.
 */
//---------------------------------------------------------------------------//
//

#include <algorithm>
#include <list>

#include "TH1F.h"
#include "TH2F.h"
#include "globals.hh"
#include "TMath.h"

#include "G4Box.hh"
#include "G4Event.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ios.hh"
#include "G4PrimaryParticle.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4ThreeVector.hh"
#include "G4VParticleChange.hh"
#include "G4VPhysicalVolume.hh"

#include "io/MGLogger.hh"
#include "io/MGOutputG4Steps.hh"

//---------------------------------------------------------------------------//

#include "mjio/MJOutputHPNeutronTest.hh" 

using namespace CLHEP;

//---------------------------------------------------------------------------//

MJOutputHPNeutronTest::MJOutputHPNeutronTest() :
  MGOutputRoot(true), 
  fHInitNSpectrum(0), fHCaptureXSec(0), fHN_NXSec(0), fHN_2NXSec(0), 
  fHN_MNXSec(0), fHN_NGXSec(0), fHN_OtherXSec(0),
  fHGammaSpect(0), fHNSecondaries(0), fHNNeutrons(0), fHNProtons(0),
  fHProcess(0), fHNTheta(0), fHELossInBlock(0), fHEfDivEi(0),
  fHRecoilPEnergy(0), fHNuclearRecoilE(0), fHNNuclei(0)
{
  fSaveG4Steps = false;
  if(fSaveG4Steps)
    fG4Steps = new MGOutputG4Steps(false);
  else
    fG4Steps = 0;
}

//---------------------------------------------------------------------------//

MJOutputHPNeutronTest::~MJOutputHPNeutronTest()
{
  delete fG4Steps;
}

//---------------------------------------------------------------------------//

void MJOutputHPNeutronTest::BeginOfEventAction(const G4Event *event)
{
  fELossInBlock = 0.0;
  fStepN = 0;
  fNGammas = fNNuclei =  fNProtons = fNNeutrons = fNOther = fNSecondaries = 0;  
  fNoNInteraction = true;
  fRecoilPEnergy = -1.0;
  if(fSaveG4Steps)
    fG4Steps->BeginOfEventAction(event);
  fNuclearTrackID.clear();
  fNuclearRecoilEnergy.clear();
}

//---------------------------------------------------------------------------//

void MJOutputHPNeutronTest::BeginOfRunAction()
{
  if(IsMother()) {
    MGLog(routine) << "ROOT file for output opened : " << GetFileName()
		   << endlog;
    OpenFile();
  }

  MGLog(routine) << "Creating ROOT histograms." << endlog;

  G4int nXSecBins = 40;
  G4double upperBin = 40.0;
  G4double lowerBin = 0.0;

  fHInitNSpectrum = new TH1F("InitSpectrum", "Initial Spectrum", nXSecBins, lowerBin, upperBin);
  fHInitNSpectrum->GetXaxis()->SetTitle("Energy (MeV)");
  fHInitNSpectrum->GetYaxis()->SetTitle("Counts");
  fHCaptureXSec = new TH1F("n Capture", "Capture Cross-section", nXSecBins, lowerBin, upperBin);
  fHCaptureXSec->GetXaxis()->SetTitle("Energy (MeV)");
  fHCaptureXSec->GetYaxis()->SetTitle("Barns");
  fHN_NXSec = new TH1F("N_NXSec", "(n,n) Cross-section", nXSecBins, lowerBin, upperBin);
  fHN_NXSec->GetXaxis()->SetTitle("Energy (MeV)");
  fHN_NXSec->GetYaxis()->SetTitle("Barns");
  fHN_MNXSec = new TH1F("N_MNXSec", "(n,an) Cross-section", nXSecBins, lowerBin, upperBin);
  fHN_MNXSec->GetXaxis()->SetTitle("Energy (MeV)");
  fHN_MNXSec->GetYaxis()->SetTitle("Barns");
  fHN_2NXSec = new TH1F("N_2NXSec", "(n,2n) Cross-section", nXSecBins, lowerBin, upperBin);
  fHN_2NXSec->GetXaxis()->SetTitle("Energy (MeV)");
  fHN_2NXSec->GetYaxis()->SetTitle("Barns");
  fHN_NGXSec = new TH1F("N_NGXSec", "(n,nG) Cross-section", nXSecBins, lowerBin, upperBin);
  fHN_NGXSec->GetXaxis()->SetTitle("Energy (MeV)");
  fHN_NGXSec->GetYaxis()->SetTitle("Barns");
  fHN_GXSec = new TH1F("N_GXSec", "(n,G) Cross-section", nXSecBins, lowerBin, upperBin);
  fHN_GXSec->GetXaxis()->SetTitle("Energy (MeV)");
  fHN_GXSec->GetYaxis()->SetTitle("Barns");
  fHN_PXSec = new TH1F("N_PXSec", "(n,p) Cross-section", nXSecBins, lowerBin, upperBin);
  fHN_PXSec->GetXaxis()->SetTitle("Energy (MeV)");
  fHN_PXSec->GetYaxis()->SetTitle("Barns");
  fHN_OtherXSec = new TH1F("N_NOtherXSec", "(n,other) Cross-section", nXSecBins, lowerBin, upperBin);
  fHN_OtherXSec->GetXaxis()->SetTitle("Energy (MeV)");
  fHN_OtherXSec->GetYaxis()->SetTitle("Barns");
  fHGammaSpect = new TH1F("GammaSpect", "Gamma Spectrum", 4000, 0., 40.);
  fHGammaSpect->GetXaxis()->SetTitle("Energy (MeV)");
  fHGammaSpect->GetYaxis()->SetTitle("Counts");
  fHNSecondaries = new TH1F("NSecondaries", "Number of Secondaries", 20, -0.5, 19.5);
  fHNNeutrons = new TH1F("NNeutrons", "Number of Neutrons", 20, -0.5, 19.5);
  fHNProtons = new TH1F("NProtons", "Number of Protons", 20, -0.5, 19.5);
  fHProcess = new TH1F("Process", "Physics Process", 40, -0.5, 39.5); 
  fHNTheta = new TH2F("theta", "theta", 180, 0.0, 180., 
		      nXSecBins, lowerBin, upperBin);
  fHNTheta->GetXaxis()->SetTitle("theta");
  fHNTheta->GetYaxis()->SetTitle("Energy (MeV)");
  fHELossInBlock = new TH1F("HELossInBlock", "Energy loss in block", 400, -.05, 40.05);
  fHELossInBlock->GetXaxis()->SetTitle("Energy (MeV)");
  fHELossInBlock->GetYaxis()->SetTitle("Counts");
  fHEfDivEi = new TH1F("HEfDivEi", "Final Energy / Initial Energy", 100, -.1, 0.1);
  fHEfDivEi->GetYaxis()->SetTitle("Counts");
  fHRecoilPEnergy = new TH1F("HRecoilPEnergy", "Recoil proton energy", 200, 0.0, 10.0);
  fHRecoilPEnergy->GetXaxis()->SetTitle("Energy (MeV)");
  fHRecoilPEnergy->GetYaxis()->SetTitle("Counts");
  fHNuclearRecoilE = new TH1F("HNuclearRecoilE", "Recoil nucleus energy", 200, 0.0, 1000.0);
  fHNuclearRecoilE->GetXaxis()->SetTitle("Energy (keV)");
  fHNuclearRecoilE->GetYaxis()->SetTitle("Counts");
  fHNNuclei = new TH1F("HNNuclei", "Num. Recoil Nuclei", 20, -0.5, 19.5);
  fHNNuclei->GetXaxis()->SetTitle("Num.");
  fHNNuclei->GetYaxis()->SetTitle("Counts");

  if(fSaveG4Steps)
    fG4Steps->BeginOfRunAction();
}

//---------------------------------------------------------------------------//

void MJOutputHPNeutronTest::EndOfEventAction(const G4Event* event)
{
  //  DumpEndOfEvent();
  fHNSecondaries->Fill(fNSecondaries);
  fHNNeutrons->Fill(fNNeutrons);
  fHNProtons->Fill(fNProtons);
  fHELossInBlock->Fill(fELossInBlock);
  fHNNuclei->Fill(fNuclearRecoilEnergy.size());

  // Fill cross-section histograms based on number of particles produced. 
  if(!fNoNInteraction) {
    if(fNNeutrons == 0)
      fHCaptureXSec->Fill(fEnergyMeV); //S.M. 01/20/12 Why is this capture? Can't it be (n,p) for example?
  
//Fill an elastic scattering histogram for differential and total cross section S.M. 01/20/12
  if(fNNeutrons ==1 && fNSecondaries == 1){ //elastic, 1 neutron and 1 secondary (ie the secondary is a neutron); inelastic always has at least a gamma as a secondary unless IC
      fHN_NXSec->Fill(fEnergyMeV); 
      fHNTheta->Fill(fNNTheta, fEnergyMeV);
 	}
  
  if(fNNeutrons == 1 && 
    (fNSecondaries == 1 || (fNSecondaries == 2 && fNProtons == 1) ||
	(fNSecondaries == 2 && fNNuclei == 1))) {
      // (n,n), elastic for H
		//S.M. 01/20/12 STis only works for n,p scattering. Changed the elastic process to the if statement above. I'll work on the nuc recoils later. 
      //fHN_NXSec->Fill(fEnergyMeV); //this only works for elastic scattering from hydrogen
      //fHNTheta->Fill(fNNTheta, fEnergyMeV); //this only works for elastic scattering from hydrogen
    if(fNuclearRecoilEnergy.size() == 1) {
	Double_t nuclearEnergy = fNuclearRecoilEnergy.front();
	fHNuclearRecoilE->Fill(nuclearEnergy / keV);
	fHEfDivEi->Fill((fEnergyMeV*MeV-fNNFinalEnergy-nuclearEnergy)/(fEnergyMeV*MeV));
	fHRecoilPEnergy->Fill(fRecoilPEnergy / MeV);
      }
    }

    if(fNNeutrons == 0 && fNGammas > 0)       // (n,G) 
      fHN_GXSec->Fill(fEnergyMeV);
    if(fNNeutrons > 0)                        // (n, an), inelastic
      fHN_MNXSec->Fill(fEnergyMeV);
    if(fNNeutrons == 1 && fNGammas > 0)       // (n, Gn)
      fHN_NGXSec->Fill(fEnergyMeV);
    if(fNNeutrons == 2)                       // (n, 2n)
      fHN_2NXSec->Fill(fEnergyMeV);
    if(fNProtons == 1) 
      fHN_PXSec->Fill(fEnergyMeV);            // (n, p)
  }

  if(fSaveG4Steps) {
    fG4Steps->EndOfEventAction(event);
    fG4Steps->FillTree();
  }
}

//---------------------------------------------------------------------------//

void MJOutputHPNeutronTest::DumpEndOfEvent()
{
  G4cout << "fEnergyMeV    : " << fEnergyMeV << G4endl
         << "fStepN        : " << fStepN << G4endl
         << "fNGammas      : " << fNGammas << G4endl
         << "fNNuclei      : " << fNNuclei << G4endl
	 << "fNProtons     : " << fNProtons << G4endl
	 << "fNNeutrons    : " << fNNeutrons << G4endl
	 << "fNOther       : " << fNOther << G4endl
	 << "fNSecondaries : " << fNSecondaries << G4endl;
}

//---------------------------------------------------------------------------//

void MJOutputHPNeutronTest::RootSteppingAction(const G4Step *step,
					       G4SteppingManager* /*steppingManager*/)
{
  if(fSaveG4Steps)
    fG4Steps->RootSteppingAction(step);

  G4Track *track = step->GetTrack();
  fHProcess->Fill(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName(), 1.0);
  if(step->GetTrack()->GetVolume()->GetName() == "bigBlockPhysical")
    fELossInBlock += step->GetTotalEnergyDeposit();
  if(fRecoilPEnergy < 0.0 && 
     track->GetDefinition()->GetPDGEncoding() == 2212) // Proton recoil for H-1
    fRecoilPEnergy = step->GetPreStepPoint()->GetKineticEnergy();

  // Check for recoil nuclei. 
  // Save initial energy of all nuclei. 
  if(track->GetDefinition()->GetParticleType() == "nucleus" ||
     track->GetDefinition()->GetPDGEncoding() == 2212)  // Protons
    if(find(fNuclearTrackID.begin(), fNuclearTrackID.end(), track->GetTrackID()) ==
       fNuclearTrackID.end()) {
      fNuclearTrackID.push_back(track->GetTrackID());
      fNuclearRecoilEnergy.push_back(step->GetPreStepPoint()->GetKineticEnergy());
    }

  // Get initial vertex and energy
  if(fStepN == 0) {
    fEnergyMeV = step->GetPreStepPoint()->GetKineticEnergy() / MeV;
    fHInitNSpectrum->Fill(fEnergyMeV);

  // Collect secondaries.
  } else if(track->GetPosition().mag() > 1.0 * m) {

    // If particle has veered more than 0.001 rad from primary track, then it is scattered.
    // Also, if is not a neutron, then it must be a secondary. 
	   if((track->GetPosition().x()*track->GetPosition().x() +
	track->GetPosition().y()*track->GetPosition().y())/
	       (track->GetPosition().z()*track->GetPosition().z()) > 1e-3 ||	
		track->GetDefinition()->GetPDGEncoding() != 2112) { //If it scattered and is a neutron or if it is not a neutron then some interaction must have happened.
	      fNoNInteraction = false;
	      fNSecondaries++;
	      if(track->GetDefinition()->GetPDGEncoding() == 2112){  // if the track is a neutron
	  fNNeutrons++;
	  
	fNNTheta = 
	  TMath::ACos(track->GetPosition().z()/
	  		      TMath::Sqrt(track->GetPosition().x()*track->GetPosition().x() +
	  				  track->GetPosition().y()*track->GetPosition().y() +
						track->GetPosition().z()*track->GetPosition().z()))*180/TMath::Pi(); //Get theta in degrees
				
	fNNFinalEnergy = step->GetPreStepPoint()->GetKineticEnergy();
      } else if(track->GetDefinition()->GetPDGEncoding() == 22) { // Gammas
	fNGammas++;
	fHGammaSpect->Fill(track->GetMomentum().mag() / MeV * c_light);
      }

      else if(track->GetDefinition()->GetParticleType() == "nucleus") 
	fNNuclei++;
      else if(track->GetDefinition()->GetPDGEncoding() == 2212) { // Protons
	fNProtons++;
	if(fRecoilPEnergy < 0.0)
	  fRecoilPEnergy = step->GetPreStepPoint()->GetKineticEnergy() / keV;
      }
    }

    // Finally, kill track. 
    track->SetTrackStatus(fStopAndKill);
  }
  fStepN++;
  
  if(fStepN > 10000) {
    track->SetTrackStatus(fStopAndKill);
    MGLog(trace) << "Killed event with track steps == " << track->GetCurrentStepNumber()
						     << endlog;
  }
}

//---------------------------------------------------------------------------//

void MJOutputHPNeutronTest::EndOfRunAction()
{

  // Find required info about material to compute cross-section.
  // Assume single element materials for now. 

  G4double thickness = 0.;
  G4double density = 0;
  G4LogicalVolumeStore *logicalStore = G4LogicalVolumeStore::GetInstance();
  for(unsigned int i = 0; i < logicalStore->size(); i++) 
    if((*logicalStore)[i]->GetName() == "block") {
      thickness = ((G4Box*)((*logicalStore)[i]->GetSolid()))->GetZHalfLength() * 2.0;
      density = (*logicalStore)[i]->GetMaterial()->GetTotNbOfAtomsPerVolume();
      break;
    }

	
  if(density <= 0. || thickness <= 0.) {
    MGLog(error) << "Something's fishy with block material." << endlog;
    MGLog(fatal) << endlog;
  }

  G4double thden = thickness * density * barn; // Convert to 1./barns.

	MGLog(routine) << "thickness * density * barn : " << thden
		   << endlog;

  // Compute errors in histograms for subsequent analysis.
  fHInitNSpectrum->Sumw2();
  fHN_NXSec->Sumw2();
  fHN_2NXSec->Sumw2();
  fHN_NGXSec->Sumw2();
  fHN_MNXSec->Sumw2();
  fHN_GXSec->Sumw2();
  fHN_PXSec->Sumw2();
  fHN_OtherXSec->Sumw2();

  // Convert counts to cross-sections.
  // void Divide(const TH1* h1, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option = "")
  // this = c1*h1/(c2*h2)
  fHCaptureXSec->Divide(fHCaptureXSec, fHInitNSpectrum, 1.0, thden);
  fHN_NXSec->Divide(fHN_NXSec, fHInitNSpectrum, 1.0, thden);
  fHN_2NXSec->Divide(fHN_2NXSec, fHInitNSpectrum, 1.0, thden);
  fHN_NGXSec->Divide(fHN_NGXSec, fHInitNSpectrum, 1.0, thden);
  fHN_MNXSec->Divide(fHN_MNXSec, fHInitNSpectrum, 1.0, thden);
  fHN_GXSec->Divide(fHN_GXSec, fHInitNSpectrum, 1.0, thden);
  fHN_PXSec->Divide(fHN_PXSec, fHInitNSpectrum, 1.0, thden);
  fHN_OtherXSec->Divide(fHN_OtherXSec, fHInitNSpectrum, 1.0, thden);

  // Save to ROOT file.  
  GetRootFile()->cd();
  fHInitNSpectrum->Write();
  fHCaptureXSec->Write();
  fHN_NXSec->Write();
  fHN_2NXSec->Write();
  fHN_MNXSec->Write();
  fHN_NGXSec->Write();
  fHN_GXSec->Write();
  fHN_PXSec->Write();
  fHN_OtherXSec->Write();
  fHGammaSpect->Write();
  fHNSecondaries->Write();
  fHNNeutrons->Write();
  fHNProtons->Write();
  fHProcess->Write();
  fHNTheta->Write();
  fHELossInBlock->Write();
  fHEfDivEi->Write();
  fHRecoilPEnergy->Write();
  fHNuclearRecoilE->Write();
  fHNNuclei->Write();

  if(fSaveG4Steps)
    fG4Steps->EndOfRunAction();
  if(IsMother()) 
    CloseFile();
}

//---------------------------------------------------------------------------//

void MJOutputHPNeutronTest::PreUserTrackingAction(const G4Track* /*track*/)
{ 
  //  std::cout << "It works!" <<std::endl;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

