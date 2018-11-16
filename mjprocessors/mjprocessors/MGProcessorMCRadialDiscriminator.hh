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
 * $Id: MGProcessorMCRadialDiscriminator.hh,v 1.4 2006-09-28 00:30:13 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGProcessorMCRadialDiscriminator.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Simple yes/no radial discrimnator to allow computation of radial discrimnation
 * power. Based on radial distribution of energy deposit. Returns true if integrate energy
 * deposit from outside and inside exceeds fEnergyThreshold and is separated by 
 * fRadialResolution. Note that this 
 * breaks down at the endcap of close-ended coaxial geometries. 
 * Requires MGProcessorMCReadTree class to be instantiated.
 * fTime can be computed with MGProcessorMCTimeCluster and passed to this class, 
 * but fTime does not have to be computed this way.
 * IMPORTANT : This class only works with single crystal events. 
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Thu Jan 12 11:13:51 PST 2006
 * 
 * REVISION:
 * 
 * 09-14-2006, Added Modified Electrode, R. Henning.
 * 09-07-2006, Added FillEndOfEventHistograms, R. Henning
 * 08-23-2006, Added Summary histograms, R. Henning
 * 06-22-2006, Recast Getters as const, Added phi and z discrimination, R. Henning
 * 01-12-2006, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGPROCESSORMCRADIALDISCRIMINATOR_HH
#define _MGPROCESSORMCRADIALDISCRIMINATOR_HH

//---------------------------------------------------------------------------//

#include <string>

#include "TH1F.h"

#include "mjprocessors/MGProcessor.hh"

//---------------------------------------------------------------------------//

class MGOutputG4StepsData;
class MGProcessorMCReadTree;
class MGProcessorMCSegmentation;

class MGProcessorMCRadialDiscriminator : public MGProcessor
{
public:

  MGProcessorMCRadialDiscriminator();
  MGProcessorMCRadialDiscriminator(string pname);
  virtual ~MGProcessorMCRadialDiscriminator();

  // Inherited methods.
  virtual void BeginOfRunAction();
  virtual void EndOfEventAction();
  virtual void EndOfRunAction();
  virtual void ProcessEventAction();

  // Local methods.
  Bool_t DidDiscriminate() { return fClustersResolvedRad; }
  Bool_t DidDiscriminatePhi() { return fClustersResolvedPhi; }
  Bool_t DidDiscriminatePolar() { return fClustersResolvedPolar; }
  Bool_t DidMEGranularityCut() { return fGranularityCutME; }
  Bool_t DidDiscriminateZ() { return fClustersResolvedZ; }
  void EndOfRunReport();
  void FillDetectorEventHistograms();
  const TH1F *GetPhiDistribution() { return fPhiDistribution; }
  const TH1F *GetPolarDistribution() { return fMEPolarDistribution; }
  const TH1F *GetRadialDistribution() { return fRadialDistribution; }
  const TH1F *GetZDistribution() { return fZDistribution; }
  void SetDefaults();
  void SetEnergyThreshold(Double_t ethresh) { fEnergyThreshold = ethresh; }
  void SetRadialResolution(Double_t radr) { fRadialResolution = radr; }
  void SetTime(Double_t time) { fTime = time; }
  void SetTimeResolution(Double_t tres) { fTimeResolution = tres; }

private:
  TH1F        *fRadialDistribution;      //-> Radial Distribution of energy deposits
  TH1F        *fPhiDistribution;         //-> Azimuthal (phi) Distribution of energy deposits
  TH1F        *fZDistribution;           //-> Radial Distribution of energy deposits
  TH1F        *fMEPolarDistribution;     //-> Polar distribution for Modified electrode.

  Long64_t    fTotalNumberOfEvents;      //! processed by this processor so far.

  // Summary histograms
  TH1F        *fHEMultiplicityOne;       //! Multiplicity == 1 energy spectrum
  TH1F        *fHEWRadCut;               //! Multiplicity == 1 & rad cut energy spectrum
  TH1F        *fHEWPhiCut;               //! Multiplicity == 1 & phi cut energy spectrum
  TH1F        *fHEWZCut;                 //! Multiplicity == 1 & Z cut energy spectrum
  TH1F        *fHEWRadAndZCut;           //! Multiplicity == 1 & rad & Z cut energy spectrum
  TH1F        *fHEWAllCuts;              //! Multiplicity == 1 & all above cuts energy spectrum

  TH1F        *fHMEEMultiplicityOne;     //! Multiplicity == 1 energy spectrum for Modified Electrode
  TH1F        *fHMEEWPolarCut;           //! Multiplicity == 1 & rad cut energy spectrum 
                                         //  for Modified Electrode
  

  Double_t    fSumEdep;                  //! Enegry to be histogrammed. 
  Bool_t      fClustersResolvedRad;      // true : event vetoed by radial multiplicty
  Bool_t      fClustersResolvedPhi;      // true : event vetoed by phi multiplicty
  Bool_t      fClustersResolvedZ;        // true : event vetoed by Z multiplicty
  Bool_t      fClustersResolvedPolar;    // true : event vetoed by polar multiplicty (for ME)
  Bool_t      fGranularityCutME;         // true : event vetoed by Me granularity cut
                                         //        each crystal is divided into 2 cylinders.
 
  Double_t    fTime;                     //! Seed time (sec.)
  Double_t    fTimeResolution;           //! Time resolution of electronics (sec.)
  Double_t    fRadialResolution;         //! see header description (cm.)
  Double_t    fPhiResolution;            //! see header description (cm.)
  Double_t    fZResolution;              //! see header description (cm.)
  Double_t    fEnergyThreshold;          //! see header description (keV).
  Double_t    fMEEnergyThreshold;        //! Modified electrode 50.0 keV
  Double_t    fMERadialResolution;       //! Modified electrode 2 mm
  Double_t    fMEGranularityThreshold;    //! Modified electrode energy theshold for granularity: 1.0 keV.

  MGProcessorMCReadTree   *fMCReadTree;  //!
  
  MGOutputG4StepsData          *fG4StepsData;     //!
  MGProcessorMCSegmentation    *fMCSegmentation;  //!

  ClassDef(MGProcessorMCRadialDiscriminator,2)
};
#endif
