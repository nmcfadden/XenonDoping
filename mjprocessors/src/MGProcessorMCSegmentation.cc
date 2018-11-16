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
// $Id: MGProcessorMCSegmentation.cc,v 1.9 2007-06-06 19:10:09 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessorMCSegmentation.cc
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
 * FIRST SUBMISSION: Sun Dec  4 14:58:46 PST 2005
 * 
 * REVISION:
 *
 * 12-4-2005, Created, R. Henning
 * 09-14-2006, moved SetDefaults() invocation to constructor, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include <map>
#include <string>

#include "TH1F.h"
#include "TMath.h"

#include "io/MGLogger.hh"
#include "io/MGOutputG4StepsData.hh"
#include "mjprocessors/MGProcessorMCRadialDiscriminator.hh"
#include "mjprocessors/MGProcessorMCReadTree.hh"

//---------------------------------------------------------------------------//

#include "mjprocessors/MGProcessorMCSegmentation.hh"

//---------------------------------------------------------------------------//

ClassImp(MGProcessorMCSegmentation)

MGProcessorMCSegmentation::MGProcessorMCSegmentation():
  MGProcessor("","MGProcessorMCSegmentation"),  
  fNumberOfPhiSegments(0), fNumberOfZSegments(0), 
  fNumberOfCrystals(0), fCrystalNumber(0), fSegmentNumberZ(0), fSegmentNumberPhi(0), fEnergy(0),
  fHSumEnergy(0), fHEnergyWSegCut(0), fHEnergyWRadCut(0), fHEnergyWSegAndRadCut(0),
  fHEnergyWXtalCut(0), fHMultiplicity(0), fRadialDiscriminatorProcessor(0), fReadTreeProcessor(0)
{;}

//---------------------------------------------------------------------------//

MGProcessorMCSegmentation::MGProcessorMCSegmentation(string name, 
						     Int_t NPhiSeg, 
						     Int_t NZSeg,
						     Int_t NXtals):
  MGProcessor(name, "MGProcessorMCSegmentation"),
  fNumberOfPhiSegments(NPhiSeg), fNumberOfZSegments(NZSeg),
  fNumberOfCrystals(NXtals), fCrystalNumber(0), fSegmentNumberZ(0), fSegmentNumberPhi(0), fEnergy(0),
  fHSumEnergy(0), fHEnergyWSegCut(0), fHEnergyWRadCut(0), fHEnergyWSegAndRadCut(0),
  fHEnergyWXtalCut(0), fHMultiplicity(0), fRadialDiscriminatorProcessor(0), fReadTreeProcessor(0)
{
  fNumberOfSegments = fNumberOfZSegments * fNumberOfPhiSegments;
  SetDefaults();
}

//---------------------------------------------------------------------------//

MGProcessorMCSegmentation::~MGProcessorMCSegmentation()
{
  DeallocateArrays();
  delete fHSumEnergy;
  delete fHEnergyWSegCut;
  delete fHEnergyWRadCut;
  delete fHEnergyWSegAndRadCut;
  delete fHEnergyWXtalCut;
  delete fHMultiplicity;
}

//---------------------------------------------------------------------------//

void MGProcessorMCSegmentation::AllocateArrays()
{
  fCrystalNumber = new Int_t[fNumberOfSegments*fNumberOfCrystals];
  fSegmentNumberZ = new Char_t[fNumberOfSegments*fNumberOfCrystals];
  fSegmentNumberPhi = new Char_t[fNumberOfSegments*fNumberOfCrystals];
  fEnergy = new Float_t[fNumberOfSegments*fNumberOfCrystals];
  MGLog(routine) << "Allocated arrays of size : " 
		 << fNumberOfSegments*fNumberOfCrystals << endlog;
  if(!(fEnergy && fSegmentNumberZ && fSegmentNumberPhi && fCrystalNumber)) {
    MGLog(error) << "Memory allocation error!" << endlog;
    MGLog(fatal) << endlog;
  }
}      

//---------------------------------------------------------------------------//

void MGProcessorMCSegmentation::BeginOfRunAction()
{
  MGProcessor::BeginOfRunAction();
  fLastEventNumber = 0;
  AllocateArrays();
  fTotalNumberOfEvents = 0;
  if(!(fReadTreeProcessor = (MGProcessorMCReadTree*)
       MGProcessor::GetProcessor("MGProcessorMCReadTree"))) {
    MGLog(error) << "MGProcessorMCReadTree not found!" << endlog;
    MGLog(fatal) << endlog;
  }
  fRadialDiscriminatorProcessor = (MGProcessorMCRadialDiscriminator*) 
    MGProcessor::GetProcessor("MGProcessorMCRadialDiscriminator");
  if(fRadialDiscriminatorProcessor) {
    fHEnergyWSegAndRadCut = new TH1F(("H"+fProcessorName+"SegAndRad").c_str(), 
			 ("H"+fProcessorName+"SegAndRad").c_str(), 
			 5000, -0.5, 4999.5);
    fHEnergyWRadCut = new TH1F(("H"+fProcessorName+"Rad").c_str(), 
			 ("H"+fProcessorName+"Rad").c_str(), 
			 5000, -0.5, 4999.5);
  }
  fHSumEnergy = new TH1F(("H"+fProcessorName+"All").c_str(), 
			 ("H"+fProcessorName+"All").c_str(), 
			 5000, -0.5, 4999.5);
  fHEnergyWSegCut = new TH1F(("H"+fProcessorName+"Seg").c_str(), 
			     ("H"+fProcessorName+"Seg").c_str(),
			     5000, -0.5, 4999.5);
  fHEnergyWXtalCut = new TH1F(("H"+fProcessorName+"Gran").c_str(), 
			      ("H"+fProcessorName+"Gran").c_str(), 
			      5000, -0.5, 4999.5);
  fHMultiplicity = new TH1F(("H"+fProcessorName+"Mult").c_str(), 
			      ("H"+fProcessorName+"Mult").c_str(), 
			      101, -0.5, 100.5);
}

//---------------------------------------------------------------------------//

Int_t MGProcessorMCSegmentation::CrystalMultiplicity(Double_t threshold)
{
  // Find all crystals hit.
  map<Int_t, Double_t>  crystalsHit;
  map<Int_t, Double_t>::iterator  iter;
  for(Int_t i = 0; i < fNumberOfSegmentsHit; i++) {
    if((iter = crystalsHit.find(fCrystalNumber[i])) != crystalsHit.end())
       iter->second += fEnergy[i];
    else
      crystalsHit.insert(pair<Int_t, Double_t>(fCrystalNumber[i], fEnergy[i]));
  } // for

  // Find number that has energy greater than threshold. 
  Int_t numCrystalsHit = 0;
  iter = crystalsHit.begin();
  while(iter != crystalsHit.end())
    if(iter++->second > threshold) numCrystalsHit++; 
  return numCrystalsHit;
}

//---------------------------------------------------------------------------//

void MGProcessorMCSegmentation::DeallocateArrays()
{
  delete [] fCrystalNumber;
  fCrystalNumber = 0;
  delete [] fSegmentNumberZ;
  fSegmentNumberZ = 0;
  delete [] fSegmentNumberPhi;
  fSegmentNumberPhi = 0;
  delete [] fEnergy;
  fEnergy = 0;
}

//---------------------------------------------------------------------------//

void MGProcessorMCSegmentation::EndOfRunAction()
{
  MGProcessor::EndOfRunAction();
  fHSumEnergy->Write();
  fHEnergyWSegCut->Write();
  fHEnergyWXtalCut->Write();
  fHEnergyWSegAndRadCut->Write();
  fHEnergyWRadCut->Write();
  fHMultiplicity->Write();
  EndOfRunReport();
}

//---------------------------------------------------------------------------//

void MGProcessorMCSegmentation::EndOfRunReport()
{
  Stat_t integralAll = fHSumEnergy->Integral(2035, 2045, "");
  Stat_t integralGran = fHEnergyWXtalCut->Integral(2035, 2045, "");
  Stat_t integralSeg = fHEnergyWSegCut->Integral(2035, 2045, "");
  Stat_t integralRad = fHEnergyWRadCut->Integral(2035, 2045, "");
  Stat_t integralSegAndRad = fHEnergyWSegAndRadCut->Integral(2035, 2045, "");

  MGLog(routine) << "Report for " << fProcessorName << "\n\n"
		 << "Number Of MC Events Processed: "
		 << fTotalNumberOfEvents << '\n'
		 << "Total Number Of Events in ROI (2033.5-2044.5keV): "
		 << integralAll << '\n'
		 << "Total Number Of Events in ROI (2033.5-2044.5keV) w/ Granularity cut: "
		 << integralGran << '\n'
		 << "Total Number Of Events in ROI (2033.5-2044.5keV) w/ Seg cut: "
		 << integralSeg << '\n'
		 << "Total Number Of Events in ROI (2033.5-2044.5keV) w/ Rad cut: "
		 << integralRad << '\n'
		 << "Total Number Of Events in ROI (2033.5-2044.5keV) w/ Seg and Rad cuts: "
		 << integralSegAndRad << '\n'
		 << "Efficiency, no cuts: " 
		 << (Double_t) (integralAll/fTotalNumberOfEvents)/11. << '\n'
		 << "Efficiency, granularity cut: " 
		 << (Double_t) (integralGran/fTotalNumberOfEvents)/11. << '\n'
		 << "Efficiency, segmentation cut: " 
		 << (Double_t) (integralSeg/fTotalNumberOfEvents)/11. << '\n'
		 << "Efficiency, radial cut: " 
		 << (Double_t) (integralRad/fTotalNumberOfEvents)/11. << '\n'
		 << "Efficiency, segmentation and radial cuts: " 
		 << (Double_t) (integralSegAndRad/fTotalNumberOfEvents)/11. << '\n'
		 << "=========================================================================\n\n"
		 << endlog;
}

//---------------------------------------------------------------------------//

void MGProcessorMCSegmentation::FillDetectorEventHistograms()
{ 
  fHMultiplicity->Fill(fNumberOfSegmentsHit);
  if(fSegmentMultiplicity > 0) {
    fHSumEnergy->Fill(fSumEdep);
    if(fSegmentMultiplicity < 2) {
      fHEnergyWSegCut->Fill(fSumEdep);
      if(fHEnergyWSegAndRadCut)
	if(!fRadialDiscriminatorProcessor->DidDiscriminate())
	  fHEnergyWSegAndRadCut->Fill(fSumEdep);
    }
    if(!fMultiCrystalHit) {
      fHEnergyWXtalCut->Fill(fSumEdep);
      if(fHEnergyWRadCut)
	if(!fRadialDiscriminatorProcessor->DidDiscriminate())      
	  fHEnergyWRadCut->Fill(fSumEdep);
    } //if(!multiCrystalHit) {
  } //if(fNumberOfSegmentsHit > 0) {
}

//---------------------------------------------------------------------------//

void MGProcessorMCSegmentation::ProcessEventAction()
{
  fNumberOfSegmentsHit = 0;
  MGProcessor::ProcessEventAction();
  if(fReadTreeProcessor->GetStatus() == ProcessorNameSpace::kDoneProcessing)
    fStatus = ProcessorNameSpace::kDoneProcessing;
  else {
    Int_t volumeNamePos = 0;
    Int_t crystalNumber, segmentNumberZ, segmentNumberPhi;
    Double_t phi;
    Double_t pi2 = TMath::Pi() * 2.0;
    const MGOutputG4StepsData *sD = fReadTreeProcessor->GetG4StepsData();
    fMCEventNumber = fReadTreeProcessor->GetEntryNumber();
    fTotalNumberOfEvents++;
    for(Int_t i = 0; i < sD->fTotalNumberOfSteps; i++) {
      //      std::cout << i << " " << sD->fTotalNumberOfSteps << " "
      //	<< sD->fStatus01[i] << " " <<  sD->fEdep[i]
      //	<< sD->fGlobalTime[i] << " " << fTime << " " << fTimeResolution << std::endl;
      if(sD->fStatus01[i] == 1 && sD->fEdep[i] > 0.0 && 
	 TMath::Abs(sD->fGlobalTime[i] - fTime) < fTimeResolution) {

	// Get Crystal number
	// Computed as (layer number) * fNCrystalIn Layer + column number
	// Note this only works in the reference design geometry. 
	if(fNumberOfCrystals > 1) 
	  crystalNumber = (Int_t) (sD->fPhysicalVolume[volumeNamePos + 13] - '0') * fNCrystalsInLayer + 
	    (Int_t) ((sD->fPhysicalVolume[volumeNamePos + 28] == '\0') 
		     ? sD->fPhysicalVolume[volumeNamePos + 27] - '0':
		     10 * (sD->fPhysicalVolume[volumeNamePos + 27] - '0') + 
		     sD->fPhysicalVolume[volumeNamePos + 28] - '0');
	else
	  crystalNumber = -1;

	// Get Segment number 
	if(fNumberOfPhiSegments > 1) {
	  if(sD->fLocalY[i] > 0 && sD->fLocalX[i] < 0)
	    phi = TMath::ATan(TMath::Abs(sD->fLocalX[i]/sD->fLocalY[i])) + TMath::Pi() * .5;
	  else if(sD->fLocalY[i] < 0 && sD->fLocalX[i] < 0)
	    phi = TMath::ATan(sD->fLocalY[i]/sD->fLocalX[i]) + TMath::Pi();
	  else if(sD->fLocalY[i] < 0 && sD->fLocalX[i] > 0)
	    phi = TMath::ATan(TMath::Abs(sD->fLocalX[i]/sD->fLocalY[i])) + TMath::Pi() * 1.5;	
	  else
	    phi = TMath::ATan(sD->fLocalY[i]/sD->fLocalX[i]);
	  if(phi < 0) phi += pi2;
	  segmentNumberPhi = (Int_t)(phi/pi2 * fNumberOfPhiSegments);
	} else
	  segmentNumberPhi = 0;
	segmentNumberZ = (fNumberOfZSegments > 1) ? 
	  (Int_t)((sD->fLocalZ[i]/fHeight + 0.5) * fNumberOfZSegments) : 0;

	// Add to list of hits, if not already there. 
	Int_t segIndex = SegmentHit(crystalNumber, segmentNumberZ, segmentNumberPhi);
	if(segIndex >= 0) 
	  fEnergy[segIndex] += sD->fEdep[i];
	else {
	  fCrystalNumber[fNumberOfSegmentsHit] = crystalNumber;
	  fSegmentNumberZ[fNumberOfSegmentsHit] = segmentNumberZ;
	  fSegmentNumberPhi[fNumberOfSegmentsHit] = segmentNumberPhi;
	  fEnergy[fNumberOfSegmentsHit++] = sD->fEdep[i];
	} // if(segIndex.. 
      } // if(sD->...)
      volumeNamePos += strlen(&sD->fPhysicalVolume[volumeNamePos]) + 1;
    } // for(Int_t i = 0; i < sD->fTotalNumberOfSteps; i++) {

    // Find segments that are above threshold.
    // Compute sum energy using those hits only. 
    fSegmentMultiplicity = 0;
    fSumEdep = 0.0;
    fMultiCrystalHit = false;
    Int_t firstCrystalHit = -1;
    for(Int_t i = 0; i < fNumberOfSegmentsHit; i++) 
      if(fEnergy[i] > fThreshold) {
	if(firstCrystalHit < 0) 
	  firstCrystalHit = i;
	else if(fCrystalNumber[i] != fCrystalNumber[firstCrystalHit])
	  fMultiCrystalHit = true;
	fSumEdep += fEnergy[i];
	fSegmentMultiplicity++;
      } // for
  } // else
}

//---------------------------------------------------------------------------//

void MGProcessorMCSegmentation::SetDefaults()
{
  fHeight = 7.0;
  fCoreRadius = 0.4;
  fRadius = 3.1 ;  // Size in cm
  fNCrystalsInLayer = 19;
  fThreshold = 5.0; // keV
}

//---------------------------------------------------------------------------//

Int_t MGProcessorMCSegmentation::SegmentHit(Int_t crystalNum, 
					    Int_t segmentNumZ,
					    Int_t segmentNumPhi)
{
  Int_t index = -1;
  for(Int_t i = 0; index < 0 && i < fNumberOfSegmentsHit; i++)
    if(fCrystalNumber[i] == crystalNum && fSegmentNumberZ[i] == segmentNumZ
       && fSegmentNumberPhi[i] == segmentNumPhi)
      index = i;

  return index;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
