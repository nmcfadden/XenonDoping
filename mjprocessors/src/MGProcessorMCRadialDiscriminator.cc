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
// $Id: MGProcessorMCRadialDiscriminator.cc,v 1.5 2006-09-28 00:30:13 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessorMCRadialDiscriminator.cc
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
 * FIRST SUBMISSION: Thu Jan 12 11:13:51 PST 2006
 * 
 * REVISION:
 *
 * 09-07-2006, Added FillDetectorEventHistograms(), R. Henning
 * 05-25-2006, Added Z and phi discriminators.
 *             Minor other improvements, R. Henning.
 * 01-12-2006, Created, R. Henning.
 *
 */
//---------------------------------------------------------------------------//
//

#include <list>

#include "TH1F.h"
#include "TMath.h"

#include "io/MGLogger.hh"
#include "io/MGOutputG4StepsData.hh"
#include "mjprocessors/MGProcessorMCReadTree.hh"
#include "mjprocessors/MGProcessorMCSegmentation.hh"

//---------------------------------------------------------------------------//

#include "mjprocessors/MGProcessorMCRadialDiscriminator.hh"

//---------------------------------------------------------------------------//

MGProcessorMCRadialDiscriminator::MGProcessorMCRadialDiscriminator():
  MGProcessor("", "MGProcessorMCRadialDiscriminator"),
  fRadialDistribution(0), fPhiDistribution(0), fZDistribution(0), fMEPolarDistribution(0), 
  fHEMultiplicityOne(0), fHEWRadCut(0), fHEWPhiCut(0), 
  fHEWZCut(0), fHEWRadAndZCut(0), fHEWAllCuts(0), fHMEEMultiplicityOne(0),
  fHMEEWPolarCut(0), fMCReadTree(0), fG4StepsData(0)
{;}

//---------------------------------------------------------------------------//

MGProcessorMCRadialDiscriminator::MGProcessorMCRadialDiscriminator(string pname):
  MGProcessor(pname, "MGProcessorMCRadialDiscriminator"),
  fRadialDistribution(0),  fPhiDistribution(0), fZDistribution(0), fMEPolarDistribution(0), 
  fHEMultiplicityOne(0), fHEWRadCut(0), fHEWPhiCut(0), 
  fHEWZCut(0), fHEWRadAndZCut(0), fHEWAllCuts(0), fHMEEMultiplicityOne(0),
  fHMEEWPolarCut(0), fMCReadTree(0), fG4StepsData(0)
{;}

//---------------------------------------------------------------------------//

MGProcessorMCRadialDiscriminator::~MGProcessorMCRadialDiscriminator()
{;}

//---------------------------------------------------------------------------//

void MGProcessorMCRadialDiscriminator::BeginOfRunAction()
{
  MGProcessor::BeginOfRunAction();
  SetDefaults();

  // Charge Distribution Histograms.
  fRadialDistribution = new TH1F(("HRad"+fProcessorName).c_str(), ("HRad"+fProcessorName).c_str(), 320, 0., 3.2);
  fPhiDistribution = new TH1F(("HPhi"+fProcessorName).c_str(), ("HPhi"+fProcessorName).c_str(), 360, 0., 2.*TMath::Pi());
  fZDistribution = new TH1F(("HZ"+fProcessorName).c_str(), ("HZ"+fProcessorName).c_str(), 1000, -5.0, 5.0);
  fMEPolarDistribution = new TH1F(("HMEPolar"+fProcessorName).c_str(), ("HMEPolar"+fProcessorName).c_str(), 1000, -5.0, 5.0);

  // Energy spectrum with cuts.
  fHEMultiplicityOne = new TH1F(("HMultCut"+fProcessorName).c_str(), ("HMultCut"+fProcessorName).c_str(), 3000, -0.5, 2999.5);
  fHEWRadCut = new TH1F(("HRadCut"+fProcessorName).c_str(), ("HRadCut"+fProcessorName).c_str(), 3000, -0.5, 2999.5);
  fHEWPhiCut = new TH1F(("HPhiCut"+fProcessorName).c_str(), ("HPhiCut"+fProcessorName).c_str(), 3000, -0.5, 2999.5);
  fHEWZCut = new TH1F(("HZCut"+fProcessorName).c_str(), ("HZCut"+fProcessorName).c_str(), 3000, -0.5, 2999.5);
  fHEWRadAndZCut = new TH1F(("HRadAndZCut"+fProcessorName).c_str(), ("HRadAndZCut"+fProcessorName).c_str(), 3000, -0.5, 2999.5);
  fHEWAllCuts = new TH1F(("HWAllCuts"+fProcessorName).c_str(), ("HWAllCuts"+fProcessorName).c_str(), 3000, -0.5, 2999.5);
  fHMEEMultiplicityOne = new TH1F(("HMEMultCut"+fProcessorName).c_str(), ("HMEMultCut"+fProcessorName).c_str(), 3000, -0.5, 2999.5);
  fHMEEWPolarCut = new TH1F(("HMEWPolarCut"+fProcessorName).c_str(), ("HMEWPolarCut"+fProcessorName).c_str(), 3000, -0.5, 2999.5);

  // Check if required processors exist. If not, abort program.
  if(!(fMCReadTree = (MGProcessorMCReadTree *) MGProcessor::GetProcessor("MGProcessorMCReadTree"))){
    MGLog(error) << "MGProcessorMCReadTree not found!" << endlog;
    MGLog(fatal) << endlog;
  }
  if(!(fMCSegmentation = (MGProcessorMCSegmentation *) MGProcessor::GetProcessor("MGProcessorMCSegmentation1x1"))) {
    MGLog(error) << "MGProcessorMCSegmentation1x1 not found!" << endlog;
    MGLog(fatal) << endlog;
  }
  fTotalNumberOfEvents = 0;
}

//---------------------------------------------------------------------------//

void MGProcessorMCRadialDiscriminator::EndOfEventAction()
{
  MGProcessor::EndOfEventAction();
  if(fMCReadTree->GetStatus() == ProcessorNameSpace::kDoneProcessing)
    fStatus = ProcessorNameSpace::kDoneProcessing;
}

//---------------------------------------------------------------------------//

void MGProcessorMCRadialDiscriminator::EndOfRunAction()
{
  MGProcessor::EndOfRunAction();

  // Write all summary histograms to file.
  fHEMultiplicityOne->Write();
  fHEWRadCut->Write();
  fHEWPhiCut->Write();
  fHEWZCut->Write();
  fHEWRadAndZCut->Write();
  fHEWAllCuts->Write();
  fHMEEMultiplicityOne->Write();
  fHMEEWPolarCut->Write();

  EndOfRunReport();
}

//---------------------------------------------------------------------------//

void MGProcessorMCRadialDiscriminator::EndOfRunReport()
{
  Stat_t integralMultiplicityOne = fHEMultiplicityOne->Integral(2035, 2045, "");
  Stat_t integralRad = fHEWRadCut->Integral(2035, 2045, "");
  Stat_t integralPhi = fHEWPhiCut->Integral(2035, 2045, "");
  Stat_t integralZ = fHEWZCut->Integral(2035, 2045, "");
  Stat_t integralRadAndZ = fHEWRadAndZCut->Integral(2035, 2045, "");
  Stat_t integralAllCuts = fHEWAllCuts->Integral(2035, 2045, "");
  Stat_t integralMEGranCut = fHMEEMultiplicityOne->Integral(2035, 2045, "");
  Stat_t integralMEGranAndPolarCut = fHMEEWPolarCut->Integral(2035, 2045, "");
  
  MGLog(routine) << "End of Run Report for " << fProcessorName << "\n\n"
		 << "Total Number Of Events Processed: "
		 << fTotalNumberOfEvents << '\n'
		 << "Total Number Of Events in ROI (mult == 1) (2033.5-2044.5keV): "
		 << integralMultiplicityOne << '\n'
		 << "Events in ROI w/ Radial cut: " 
		 << integralRad << '\n'
		 << "Events in ROI w/ Phi cuts: "
		 << integralPhi << '\n'
		 << "Events in ROI w/ Z cuts: "
		 << integralZ << '\n'
		 << "Events in ROI w/ Rad and Z cuts: "
		 << integralRadAndZ << '\n'
		 << "Events in ROI w/ all cuts : "
		 << integralAllCuts << '\n'
		 << "Efficiency, mult == 1: " 
		 << (Double_t) ((integralMultiplicityOne/fTotalNumberOfEvents)/11.0) << '\n'
		 << "Efficiency, mult == 1 & Rad Cut: " 
		 << (Double_t) ((integralRad/fTotalNumberOfEvents)/11.0) << '\n'
		 << "Efficiency, mult == 1 & Phi Cut: " 
		 << (Double_t) ((integralPhi/fTotalNumberOfEvents)/11.0) << '\n'
		 << "Efficiency, mult == 1 & Z Cut: " 
		 << (Double_t) ((integralZ/fTotalNumberOfEvents)/11.0) << '\n'
		 << "Efficiency, mult == 1 & Rad and Z cut: " 
		 << (Double_t) ((integralRadAndZ/fTotalNumberOfEvents)/11.0) << '\n'
		 << "Efficiency, mult == 1 & All cuts: " 
		 << (Double_t) ((integralAllCuts/fTotalNumberOfEvents)/11.0) << '\n'
		 << "Efficiency, Modifed electrode mult == 1 cut: " 
		 << (Double_t) ((integralMEGranCut/fTotalNumberOfEvents)/11.0) << '\n'
		 << "Efficiency, Modifed electrode mult == 1 & polar PSD cut: " 
		 << (Double_t) ((integralMEGranAndPolarCut/fTotalNumberOfEvents)/11.0) << '\n'
		 << "=================================================================\n\n" << endlog;
}

//---------------------------------------------------------------------------//

void MGProcessorMCRadialDiscriminator::FillDetectorEventHistograms()
{

  if(fStatus != ProcessorNameSpace::kDoneProcessing) {

    // Fill summary histograms
    if(!fMCSegmentation->GetMultiCrystalHit()) {
      fHEMultiplicityOne->Fill(fSumEdep);
      if(!fClustersResolvedRad) fHEWRadCut->Fill(fSumEdep);
      if(!fClustersResolvedPhi) fHEWPhiCut->Fill(fSumEdep);
      if(!fClustersResolvedZ) fHEWZCut->Fill(fSumEdep);
      if(!fClustersResolvedRad && !fClustersResolvedZ) {
	fHEWRadAndZCut->Fill(fSumEdep);
	if(!fClustersResolvedPhi) fHEWAllCuts->Fill(fSumEdep);
      }
    }
    
    // Fill histograms for the Modified electrode study.
    if(fMCSegmentation->CrystalMultiplicity(fMEGranularityThreshold) == 1 && !fGranularityCutME) {
      fHMEEMultiplicityOne->Fill(fSumEdep);
      if(!fClustersResolvedPolar)
	fHMEEWPolarCut->Fill(fSumEdep);
    }
  }
}

//---------------------------------------------------------------------------//


void MGProcessorMCRadialDiscriminator::ProcessEventAction()
{
  MGProcessor::ProcessEventAction();

  if(fMCReadTree->GetStatus() == ProcessorNameSpace::kDoneProcessing)
    fStatus = ProcessorNameSpace::kDoneProcessing;
  else {
    fRadialDistribution->Reset();
    fZDistribution->Reset();
    fPhiDistribution->Reset();
    fMEPolarDistribution->Reset();
    const MGOutputG4StepsData *sD = fMCReadTree->GetG4StepsData();
    Int_t i, j;
    Double_t  phi = 0.0;
    fSumEdep = 0.0;

    fTotalNumberOfEvents++;

    // Fill spatial charge distribution histograms
    // Note that this only has meaning for single crystal events. 
    for(i = 0; i < sD->fTotalNumberOfSteps; i++) 
      if(sD->fStatus01[i] == 1 && sD->fEdep[i] > 0.0 && 
	 TMath::Abs(sD->fGlobalTime[i] - fTime) < fTimeResolution) {
	phi = sD->fLocalX[i]*sD->fLocalX[i] + sD->fLocalY[i]*sD->fLocalY[i]; // use phi as temp storage here. 
	fRadialDistribution->Fill(TMath::Sqrt(phi), sD->fEdep[i]);
	fZDistribution->Fill(sD->fLocalZ[i], sD->fEdep[i]);
	fMEPolarDistribution->Fill(((sD->fLocalZ[i] < 0.0) ? -1.0 : 1.0) * 
				   TMath::Sqrt(phi + sD->fLocalZ[i]*sD->fLocalZ[i]), sD->fEdep[i]);
 
	// Find phi in range (0, 2*pi)
	phi = TMath::ATan(TMath::Abs(sD->fLocalY[i]/sD->fLocalX[i]));
	phi = (sD->fLocalX[i] < 0.0) ? 
	  ((sD->fLocalY[i] < 0.0) ?
	   phi + TMath::Pi()
	   : TMath::Pi() - phi)
	  : ((sD->fLocalY[i] < 0.) ?
	   2.*TMath::Pi() - phi : phi);
	fPhiDistribution->Fill(phi, sD->fEdep[i]);
	fSumEdep += sD->fEdep[i];
      }

    // Test if radial spatial separation is resolved. Assume 100micron bins in histogram
    Double_t binSize = 0.01; // 100um/1cm
    Int_t    binResolution = (Int_t)(fRadialResolution / binSize);
    Double_t integratedLowE = 0.0;
    Double_t integratedHighE = 0.0;
    Int_t    nBins = fRadialDistribution->GetNbinsX() - 1; // ignore overflow bin 
    for(i = 1; (nBins - i) > binResolution && integratedLowE < fEnergyThreshold; i++) 
      integratedLowE += fRadialDistribution->GetBinContent(i);
    for(j = nBins; j > binResolution && integratedHighE < fEnergyThreshold; j--) 
      integratedHighE += fRadialDistribution->GetBinContent(j);
    fClustersResolvedRad = integratedLowE > fEnergyThreshold && integratedHighE > fEnergyThreshold &&
      j - i > binResolution;

    // Test if azimuthal (z) separation is resolved.
    binSize = 0.01;
    binResolution = (Int_t)(fZResolution / binSize);
    integratedLowE = integratedHighE = 0.0;
    nBins = fZDistribution->GetNbinsX();
    for(i = 1; (nBins - i) > binResolution && integratedLowE < fEnergyThreshold; i++) 
      integratedLowE += fZDistribution->GetBinContent(i);
    for(j = nBins; j > binResolution && integratedHighE < fEnergyThreshold; j--) 
      integratedHighE += fZDistribution->GetBinContent(j);
    fClustersResolvedZ = integratedLowE > fEnergyThreshold && integratedHighE > fEnergyThreshold &&
      j - i > binResolution;

    // Test if phi separation is resolved. 
    binSize = TMath::Pi()/180.;
    binResolution = (Int_t)(fPhiResolution / binSize);
    integratedLowE = integratedHighE = 0.0;
    nBins = fPhiDistribution->GetNbinsX();
    for(i = 1; (nBins - i) > binResolution && integratedLowE < fEnergyThreshold; i++) 
      integratedLowE += fPhiDistribution->GetBinContent(i);
    for(j = nBins; j > binResolution && integratedHighE < fEnergyThreshold; j--) 
      integratedHighE += fPhiDistribution->GetBinContent(j);
    fClustersResolvedPhi = integratedLowE > fEnergyThreshold && integratedHighE > fEnergyThreshold &&
      j - i > binResolution;

    // Test if modified electrode detectors would have resolved.
    // Compute added granularity cut. Should be same as 1x2 segmentation.
    Double_t  lowerCrystalE = fMEPolarDistribution->Integral(1, 500, "");
    Double_t  upperCrystalE = fMEPolarDistribution->Integral(501, 1000, "");
    fGranularityCutME = lowerCrystalE > fMEGranularityThreshold && upperCrystalE > fMEGranularityThreshold;
    if(!fGranularityCutME && (lowerCrystalE > 2.*fMEEnergyThreshold || upperCrystalE > 2.*fMEEnergyThreshold)) {
      Int_t offset;
      if(lowerCrystalE > 2.*fMEEnergyThreshold) 
	offset = 0;
      else 
	offset = 500; 

      // Test if polar radial cut resolved charges.
      binSize = 0.01;
      binResolution = (Int_t)(fMERadialResolution / binSize);
      integratedLowE = integratedHighE = 0.0;
      nBins = 500;
      for(i = 1; (nBins - i) > binResolution && integratedLowE < fMEEnergyThreshold; i++) 
	integratedLowE += fMEPolarDistribution->GetBinContent(i + offset);
      for(j = nBins; j > binResolution && integratedHighE < fMEEnergyThreshold; j--) 
	integratedHighE += fMEPolarDistribution->GetBinContent(j + offset);
      fClustersResolvedPolar = integratedLowE > fMEEnergyThreshold && integratedHighE > fMEEnergyThreshold &&
	j - i > binResolution;
    }
  } //else
}

//---------------------------------------------------------------------------//

void MGProcessorMCRadialDiscriminator::SetDefaults()
{
  fRadialResolution = 0.4; // 4 mm
  fZResolution = 0.2; // 2 mm
  fPhiResolution = TMath::Pi()/6.; // 30 deg.
  fEnergyThreshold = 50.0; // 50 keV
  fMEEnergyThreshold = 50.0; // 50.0 keV
  fMERadialResolution = 0.2; // 2mm
  fMEGranularityThreshold = 1.0; // 1.0 keV
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
