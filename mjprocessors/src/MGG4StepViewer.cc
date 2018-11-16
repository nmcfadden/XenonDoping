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
// $Id: MGG4StepViewer.cc,v 1.2 2006-09-07 23:27:18 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MGG4StepViewer.cc
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
 * FIRST SUBMISSION: Thu Jun 29 17:14:18 PDT 2006
 * 
 * REVISION:
 *
 * 06-29-2006, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include <iostream>

#include "TChain.h"
#include "TGButton.h"
#include "TGFrame.h"
#include "TGLabel.h"
#include "TGNumberEntry.h"

#include "io/MGOutputG4StepsData.hh"

//---------------------------------------------------------------------------//

#include "mjprocessors/MGG4StepViewer.hh"

//---------------------------------------------------------------------------//

MGG4StepViewer::MGG4StepViewer() :
  fS(0), fChain(0), fMainFrame(0), fEventNumberBox(0), fStartStepBox(0), 
  fEndStepBox(0), fRootWindow(0), fDumpWindow(0), fGraphicWindow(0)
{;}

//---------------------------------------------------------------------------//


MGG4StepViewer::MGG4StepViewer(const TGWindow *p, TChain *chain, Int_t eventnum):
  fCurrentEventN(eventnum), fChain(chain), fRootWindow(p), fDumpWindow(0), fGraphicWindow(0)
{
  fS = new MGOutputG4StepsData;
  SetDefaults();
  PrepChain();
  CreateGUI();
  LoadEvent(0);
}

//---------------------------------------------------------------------------//

MGG4StepViewer::~MGG4StepViewer()
{
  DeallocateArrays();
  delete fS;
  fMainFrame->Cleanup();
  delete fMainFrame;
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::CreateGUI()
{
  // Create Main Frame
  fMainFrame = new TGMainFrame(fRootWindow, fMainFrameWidth, fMainFrameHeight);

  // Create window selector frame
  // ----------------------------

  TGGroupFrame *windowSelectorFrame = new TGGroupFrame(fMainFrame, "Other Windows", kVerticalFrame);
  fMainFrame->AddFrame(windowSelectorFrame,
		       new TGLayoutHints(kLHintsTop | kLHintsCenterX, 5, 5, 3, 4));

  // Create window option text buttons.
  TGTextButton *createStepDumpWindow = new TGTextButton(windowSelectorFrame, "&Step Dump");
  windowSelectorFrame->AddFrame(createStepDumpWindow, 
				new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 4, 4, 4, 1));
  createStepDumpWindow->Connect("Clicked()", "MGG4StepViewer", this, "CreateStepDumpWindow()");

  TGTextButton *createGraphicalDisplayWindow = new TGTextButton(windowSelectorFrame, "&Graphical Display");
  windowSelectorFrame->AddFrame(createGraphicalDisplayWindow, 
				new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 4, 4, 4, 1));
  createGraphicalDisplayWindow->Connect("Clicked()", "MGG4StepViewer", this, "CreateGraphicalWindow()");


  // Create Status Frame.
  // --------------------

  TGGroupFrame *statusFrame = new TGGroupFrame(fMainFrame, "Status and Actions", kVerticalFrame);
  fMainFrame->AddFrame(statusFrame, new TGLayoutHints(kLHintsBottom, 5, 5, 3, 4));

  // Create Buttons for Status frame.
  TGTextButton *doOneStep = new TGTextButton(statusFrame, "&Do One Step");
  statusFrame->AddFrame(doOneStep, new TGLayoutHints(kLHintsExpandY | kLHintsTop, 4, 4, 4, 1));
  doOneStep->Connect("Clicked()", "MGG4StepViewer", this, "DoOneStep()");

  TGTextButton *doAllSteps = new TGTextButton(statusFrame, "&Do All Steps");
  statusFrame->AddFrame(doAllSteps, new TGLayoutHints(kLHintsExpandY | kLHintsTop, 4, 4, 4, 1));
  doAllSteps->Connect("Clicked()", "MGG4StepViewer", this, "DoAllSteps()");

  TGTextButton *nextEvent = new TGTextButton(statusFrame, "&Next Event");
  statusFrame->AddFrame(nextEvent, new TGLayoutHints(kLHintsExpandY | kLHintsTop, 4, 4, 4, 1));
  nextEvent->Connect("Clicked()", "MGG4StepViewer", this, "LoadNextEvent()");

  // Create labelled Event number box.
  TGHorizontalFrame *eventNumberFrame = new TGHorizontalFrame(statusFrame);
  statusFrame->AddFrame(eventNumberFrame, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 4, 4, 4, 1));
  TGLabel *eventNumberLabel = new TGLabel(eventNumberFrame, "Event Number");
  eventNumberFrame->AddFrame(eventNumberLabel, new TGLayoutHints(kLHintsExpandY | kLHintsLeft, 4, 4, 4, 1));
  fEventNumberBox = new TGNumberEntry(eventNumberFrame);
  fEventNumberBox->SetText("0");
  eventNumberFrame->AddFrame(fEventNumberBox, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 4, 4, 4, 1));

  // Create labelled start step box.
  TGHorizontalFrame *startStepNumberFrame = new TGHorizontalFrame(statusFrame);
  statusFrame->AddFrame(startStepNumberFrame, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 4, 4, 4, 1));
  TGLabel *startStepNumberLabel = new TGLabel(startStepNumberFrame, "Start Step Number");
  startStepNumberFrame->AddFrame(startStepNumberLabel, new TGLayoutHints(kLHintsExpandY | kLHintsLeft, 4, 4, 4, 1));
  fStartStepBox = new TGNumberEntry(startStepNumberFrame);
  fStartStepBox->SetText("0");
  startStepNumberFrame->AddFrame(fStartStepBox, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 4, 4, 4, 1));

  // Create labelled end step box.
  TGHorizontalFrame *endStepNumberFrame = new TGHorizontalFrame(statusFrame);
  statusFrame->AddFrame(endStepNumberFrame, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 4, 4, 4, 1));
  TGLabel *endStepNumberLabel = new TGLabel(endStepNumberFrame, "End Step Number");
  endStepNumberFrame->AddFrame(endStepNumberLabel, new TGLayoutHints(kLHintsExpandY | kLHintsLeft, 4, 4, 4, 1));
  fEndStepBox = new TGNumberEntry(endStepNumberFrame);
  fEndStepBox->SetText("0");
  endStepNumberFrame->AddFrame(fEndStepBox, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 4, 4, 4, 1));

  // Create labelled current step box.
  TGHorizontalFrame *currentStepNumberFrame = new TGHorizontalFrame(statusFrame);
  statusFrame->AddFrame(currentStepNumberFrame, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 4, 4, 4, 1));
  TGLabel *currentStepNumberLabel = new TGLabel(currentStepNumberFrame, "Current Step Number");
  currentStepNumberFrame->AddFrame(currentStepNumberLabel,  new TGLayoutHints(kLHintsExpandY | kLHintsLeft, 4, 4, 4, 1));
  fCurrentStepBox = new TGNumberEntry(currentStepNumberFrame);
  fCurrentStepBox->SetText("0");
  currentStepNumberFrame->AddFrame(fCurrentStepBox, new TGLayoutHints(kLHintsExpandY | kLHintsRight, 4, 4, 4, 1));

  // Create "Integrate On" check button.
  fIntegrateButton = new TGCheckButton(statusFrame, "Integrate On", 1);
  statusFrame->AddFrame(fIntegrateButton, new TGLayoutHints(kLHintsExpandY | kLHintsBottom, 4, 4, 4, 1));


  // Finish and display GUI.
  // -----------------------

  fMainFrame->SetWindowName("MGOutputG4StepsData Viewer");
  fMainFrame->MapSubwindows();
  fMainFrame->Resize(fMainFrame->GetDefaultSize());
  fMainFrame->MapWindow();
  fMainFrame->Print();
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::DoAllSteps()
{
  ReadFields();
  LoadEvent(fCurrentEventN);
  WriteFields();
  UpdateWindows();
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::DoOneStep()
{
  ReadFields();
  LoadEvent(fCurrentEventN);
  fCurrentStepN++;
  WriteFields();
  UpdateWindows();
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::LoadEvent(Long64_t eventnum)
{
  fCurrentEventN = (eventnum < 0) ? 0 : eventnum;
  fChain->GetEntry(fCurrentEventN);
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::LoadNextEvent()
{
  ReadFields();
  LoadEvent(fCurrentEventN + 1);
  WriteFields();
  UpdateWindows();
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::PrepChain()
{
  Int_t avgLengthProcessName = 20;
  Int_t maxNumberOfSteps = 10000;
  size_t maxProcArrayLength = maxNumberOfSteps * avgLengthProcessName;
  fS->fParticleID = new Int_t[maxNumberOfSteps];
  fS->fPreviousTrackID = new Int_t[maxNumberOfSteps];
  fS->fTrackID = new Int_t[maxNumberOfSteps];
  fS->fX = new Float_t[maxNumberOfSteps];
  fS->fY = new Float_t[maxNumberOfSteps];
  fS->fZ = new Float_t[maxNumberOfSteps];
  fS->fLocalX = new Float_t[maxNumberOfSteps];
  fS->fLocalY = new Float_t[maxNumberOfSteps];
  fS->fLocalZ = new Float_t[maxNumberOfSteps];
  fS->fPx = new Float_t[maxNumberOfSteps];
  fS->fPy = new Float_t[maxNumberOfSteps];
  fS->fPz = new Float_t[maxNumberOfSteps];
  fS->fEdep = new Float_t[maxNumberOfSteps];
  fS->fG4Process = new Char_t[maxProcArrayLength];
  fS->fStatus01 = new Int_t[maxNumberOfSteps];
  fS->fMaterialIndex = new Int_t[maxNumberOfSteps];
  fS->fGlobalTime = new Double_t[maxNumberOfSteps];
  
  if(!(fS->fParticleID && fS->fPreviousTrackID && fS->fX && 
       fS->fY && fS->fZ && fS->fPx && fS->fPy && 
       fS->fPz && fS->fEdep && fS->fG4Process && 
       fS->fStatus01 && fS->fTrackID && fS->fMaterialIndex &&
       fS->fGlobalTime && fS->fLocalX && fS->fLocalY && 
       fS->fLocalZ)) {
    std::cout << "Memory Allocation Error!\nClose ROOT and try again!" << std::endl;
  } 
  if(fChain) {
    fChain->SetBranchStatus("RawMC", 1); 
    fChain->SetBranchAddress("RawMC", &fS);
    std::cout << "Chain has " << fChain->GetEntries() << " entries.\n" << std::endl;
  } else 
    std::cout << "WARNING! Chain pointer NULL!\n" << std::endl;
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::DeallocateArrays()
{
  delete [] fS->fParticleID;
  delete [] fS->fPreviousTrackID;
  delete [] fS->fTrackID;
  delete [] fS->fX;
  delete [] fS->fY;
  delete [] fS->fZ;
  delete [] fS->fLocalX;
  delete [] fS->fLocalY;
  delete [] fS->fLocalZ;
  delete [] fS->fPx;
  delete [] fS->fPy;
  delete [] fS->fPz;
  delete [] fS->fEdep;
  delete [] fS->fG4Process;
  delete [] fS->fStatus01;
  delete [] fS->fMaterialIndex;
  delete [] fS->fGlobalTime;
  SetArrayPointersToNULL();
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::ReadFields()
{
  fCurrentEventN = (Long64_t) fEventNumberBox->GetNumber();
  fStartStep = (Long64_t) fStartStepBox->GetNumber();
  fEndStep = (Long64_t) fEndStepBox->GetNumber();
  fCurrentStepN = (Long64_t) fCurrentStepBox->GetNumber();
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::SetArrayPointersToNULL()
{
  fS->fParticleID = fS->fPreviousTrackID = 0;
  fS->fTrackID = fS->fStatus01 = 0;
  fS->fMaterialIndex = 0;
  fS->fG4Process = 0;
  fS->fX = fS->fY = fS->fZ = fS->fPx = 0;
  fS->fLocalX = fS->fLocalY = fS->fLocalZ = 0;
  fS->fPy = fS->fPz = fS->fEdep = 0;
  fS->fGlobalTime = 0;
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::SetDefaults()
{
  fMainFrameWidth = 200;
  fMainFrameHeight = 400;
  fStatusFrameHeight = 200;
  fWindowSelectorFrameHeight = fMainFrameHeight - fStatusFrameHeight; 
  fIntegrateOn = true;
  fCurrentStepN = 0;
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::UpdateWindows()
{
  if(fDumpWindow) {
    fDumpWindow->SetStepNumber(fCurrentStepN);
    fDumpWindow->UpdateGUI();
  }
  if(fGraphicWindow) {
    fGraphicWindow->SetStartStep(fStartStep);
    fGraphicWindow->SetEndStep(fEndStep);
    fGraphicWindow->UpdateGUI();
  }
}

//---------------------------------------------------------------------------//

void MGG4StepViewer::WriteFields()
{
  fEventNumberBox->SetNumber(fCurrentEventN);
  fStartStepBox->SetNumber(fStartStep);
  fEndStepBox->SetNumber(fEndStep);
  fCurrentStepBox->SetNumber(fCurrentStepN);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
