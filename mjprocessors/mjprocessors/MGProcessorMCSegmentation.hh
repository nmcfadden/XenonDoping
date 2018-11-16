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
 * $Id: MGProcessorMCSegmentation.hh,v 1.8 2006-09-28 00:30:13 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGProcessorMCSegmentation.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Simple, fast processor to convert Raw MC data into segmented crystal hits. 
 * Requires MGProcessorMCReadTree
 * Incorporates results from MGProcessorMCRadialDiscriminator, if instantiated.
 * Assumptions:
 *   1.) All crystal have same segmentation.
 *   2.) No relative rotation between crystals.
 *   3.) Segmentation scheme is MxN where M is the # of phi segments and 
 *       N is the number of z-segments.
 *   4.) Crystal is divided into segments and energy deposit in each segment
 *       is reported. 
 *   5.) Crystal is coaxial.
 *   6.) Crystal axis is parallel to z-axis.
 *   7.) First segment starts at +x-axis
 *   8.) Segments counted as (m,n) : 0 - (0,0); 1 - (0,1); ...; N - (0,N);
 *       N+1 - (1, 0); ...; MxN-1 - (M, N).
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Sun Dec  4 14:58:46 PST 2005
 * 
 * REVISION:
 * 
 * 12-04-2005, Created, R. Henning
 * 06-29-2006, Added CrystalMultiplicity(), R. Henning
 * 09-07-2006, Added FillDetectorEventHistograms(), R. Henning
 * 09-14-2006, Added SetThreshold() & GetNumberOfSegmentsHit(), R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGPROCESSORMCSEGMENTATION_HH
#define _MGPROCESSORMCSEGMENTATION_HH

//---------------------------------------------------------------------------//

#include "mjprocessors/MGProcessor.hh"

//---------------------------------------------------------------------------//

class TH1F;

class MGProcessorMCRadialDiscriminator;
class MGProcessorMCReadTree;

class MGProcessorMCSegmentation : public MGProcessor
{
public:

  MGProcessorMCSegmentation();
  MGProcessorMCSegmentation(string name, Int_t NPhiSeg, Int_t NZSeg, Int_t NXtals);
  virtual ~MGProcessorMCSegmentation();

  // Inherited methods.
  virtual void BeginOfRunAction();
  virtual void EndOfRunAction();
  virtual void ProcessEventAction();

  // Local methods.
  void  AllocateArrays();      // Allocate memory for ROOT output variables
  Int_t CrystalMultiplicity(Double_t thresh); // Return crystal multiplicty of hit.
  void  DeallocateArrays();
  void  EndOfRunReport();
  void  FillDetectorEventHistograms();
  Int_t SegmentHit(Int_t crystalNum, Int_t segmentNumZ, Int_t segmentNumPhi);
  void  SetDefaults();

  // Getters
  Int_t  GetSegmentMultiplicity() { return fSegmentMultiplicity; }
  Bool_t GetMultiCrystalHit() { return fMultiCrystalHit; }

  // Setters
  void SetHeight(Double_t height) { fHeight = height; } 
  void SetCoreRadius(Double_t inner_r) { fCoreRadius = inner_r; }
  void SetRadius(Double_t outer_r) { fRadius = outer_r; }
  void SetThreshold(Double_t thresh) { fThreshold = thresh; }
  void SetTime(Double_t time) { fTime = time; }
  void SetTimeResolution(Double_t tr) { fTimeResolution = tr; }

private:

  // Variables NOT written to ROOT tree.
  Int_t      fNumberOfSegments;        //! Total number of segments per crystal
  Int_t      fNumberOfPhiSegments;     //!
  Int_t      fNumberOfZSegments;       //!
  Int_t      fNumberOfCrystals;        //!
  Double_t   fTimeResolution;          //!
  Stat_t     fLastEventNumber;         //!

  // Crystal dimensions. Provided by user or set via SetDefaults.
  Double_t   fHeight;                  //!
  Double_t   fCoreRadius;              //!
  Double_t   fRadius;                  //!
  Int_t      fNCrystalsInLayer;        //!

public:
  // Variables to be written to ROOT tree.
  Double_t   fTime;                    // Seed time. Only steps within fTimeResolution
                                       // sec. of this time are considered.
  Long64_t   fMCEventNumber;           // Number of corresponding MC event.
  Int_t      fNumberOfSegmentsHit;
  Bool_t     fMultiCrystalHit;         // true if multicrystal hit. 
  Int_t      *fCrystalNumber;          //[fNumberOfSegmentsHit]
  Char_t     *fSegmentNumberZ;         //[fNumberOfSegmentsHit]
  Char_t     *fSegmentNumberPhi;       //[fNumberOfSegmentsHit]
  Float_t    *fEnergy;                 //[fNumberOfSegmentsHit]
  // fEnergy is in keV

private:
  TH1F       *fHSumEnergy;            //! Histograms of energy spectrs with diferent cuts.
  TH1F       *fHEnergyWSegCut;        //!
  TH1F       *fHEnergyWRadCut;        //!
  TH1F       *fHEnergyWSegAndRadCut;  //!
  TH1F       *fHEnergyWXtalCut;       //!
  TH1F       *fHMultiplicity;         //! Histo of event segment multplicity. 
  Double_t   fThreshold;              //! Energy threshold for each segment to be considered in multiplicity calculations. 
  Long64_t   fTotalNumberOfEvents;    //! Total number of event processed by this processor.
  Int_t      fSegmentMultiplicity;    //! Total number of segments hit, exluding underthreshold.
  Double_t   fSumEdep;                //! Sumer of energy in all segments above threshold. 

  MGProcessorMCRadialDiscriminator  *fRadialDiscriminatorProcessor; //!
  MGProcessorMCReadTree             *fReadTreeProcessor; //!
 
  ClassDef(MGProcessorMCSegmentation, 2)
};
#endif
