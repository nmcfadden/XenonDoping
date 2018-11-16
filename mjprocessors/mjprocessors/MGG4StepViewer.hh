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
 * $Id: MGG4StepViewer.hh,v 1.2 2006-09-07 23:27:18 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGG4StepViewer.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Class that creates a GUI to view MGoutputG4Steps data from a simulatin run
 * Instantiate from ROOT. Based on GUI example in ROOT UG.
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
 * FIRST SUBMISSION: Thu Jun 29 17:14:18 PDT 2006
 * 
 * REVISION:
 * 
 * 06-29-2006, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGG4STEPVIEWER_HH
#define _MGG4STEPVIEWER_HH

//---------------------------------------------------------------------------//

#include "TQObject.h"
#include "RQ_OBJECT.h"

#include "mjprocessors/MGG4StepViewerDump.hh"
#include "mjprocessors/MGG4StepViewerGraphic.hh"

//---------------------------------------------------------------------------//

class MGOutputG4StepsData;
class TChain;
class TGCheckButton;
class TGMainFrame;
class TGNumberEntry; 
class TGWindow;

class MGG4StepViewer
{
  RQ_OBJECT("MGG4StepViewer")

public:
  MGG4StepViewer();
  MGG4StepViewer(const TGWindow *p, TChain *chain, Int_t stepnum);

  ~MGG4StepViewer();

// Note: methods associated with buttons have to be public

  void CreateGraphicalWindow() 
  {
    if(!fGraphicWindow) 
      fGraphicWindow = new MGG4StepViewerGraphic(fRootWindow, fS, this);
  }

  void CreateStepDumpWindow() 
  { 
    if(!fDumpWindow) 
      fDumpWindow = new MGG4StepViewerDump(fRootWindow, fS); 
  }

  void DoAllSteps(); 
  void DoOneStep();
  void LoadEvent(Long64_t eventnum);
  void LoadNextEvent();
  void SetDefaults();
  void SetChain(TChain *chain) { fChain = chain; }
  void SetCurrentStepN(Int_t step) { fCurrentStepN = step; }

private:
  void CreateGUI();
  void DeallocateArrays();
  void PrepChain();
  void ReadFields();
  void SetArrayPointersToNULL();
  void UpdateWindows();
  void WriteFields();

  Long64_t              fCurrentEventN;
  Int_t                 fCurrentStepN;
  Bool_t                fIntegrateOn;
  Int_t                 fStartStep;
  Int_t                 fEndStep;
  MGOutputG4StepsData   *fS;
  TChain                *fChain;

  // GUI classes.
  Int_t                 fMainFrameWidth;
  Int_t                 fMainFrameHeight;
  Int_t                 fStatusFrameHeight;
  Int_t                 fWindowSelectorFrameHeight;
  TGMainFrame           *fMainFrame;
  TGNumberEntry         *fEventNumberBox;
  TGNumberEntry         *fStartStepBox;
  TGNumberEntry         *fEndStepBox;
  TGNumberEntry         *fCurrentStepBox;
  TGCheckButton         *fIntegrateButton;
  const TGWindow        *fRootWindow;

  MGG4StepViewerDump       *fDumpWindow;
  MGG4StepViewerGraphic    *fGraphicWindow;
};
#endif
