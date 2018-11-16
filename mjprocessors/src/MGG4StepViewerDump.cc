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
// $Id: MGG4StepViewerDump.cc,v 1.2 2006-09-07 23:27:18 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MGG4StepViewerDump.cc
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
 * FIRST SUBMISSION: Thu Jul  6 14:34:19 PDT 2006
 * 
 * REVISION:
 *
 * 07-06-2006, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include <iostream>
#include <sstream>
#include <string>

#include "TGFrame.h"
#include "TGTextView.h"

#include "io/MGOutputG4StepsData.hh"

//---------------------------------------------------------------------------//

#include "mjprocessors/MGG4StepViewerDump.hh"

//---------------------------------------------------------------------------//

MGG4StepViewerDump::MGG4StepViewerDump(const TGWindow *p, const MGOutputG4StepsData *sdata):
  fRootWindow(p), fMainFrame(0), fTextViewer(0), fS(sdata)
{
  SetDefaults();
  CreateGUI();
}

//---------------------------------------------------------------------------//

MGG4StepViewerDump::~MGG4StepViewerDump()
{
  delete fMainFrame;
}

//---------------------------------------------------------------------------//


void MGG4StepViewerDump::CreateGUI()
{
  // Create Main Frame
  fMainFrame = new TGMainFrame(fRootWindow, fMainFrameWidth, fMainFrameHeight);

  // Create Text Viewer.
  fTextViewer = new TGTextView(fMainFrame, fMainFrameWidth, fMainFrameHeight);
  fMainFrame->AddFrame(fTextViewer, new TGLayoutHints(kLHintsTop | kLHintsCenterX, 5, 5, 3, 4));

  // Finish and display GUI.
  // -----------------------

  fMainFrame->SetWindowName("MGOutputG4StepsData Dump");
  fMainFrame->MapSubwindows();
  fMainFrame->Resize(fMainFrame->GetDefaultSize());
  fMainFrame->MapWindow();
  fMainFrame->Print();
}

//---------------------------------------------------------------------------//

Int_t MGG4StepViewerDump::FindProcessIndex()
{
  Int_t  position = 0;
  Int_t  index = 0;
  while(index++ < fStepN)
    while(fS->fG4Process[position++] != '\0');

  return position;
}

//---------------------------------------------------------------------------//

Int_t MGG4StepViewerDump::FindVolumeIndex()
{
  Int_t  position = 0;
  Int_t  index = 0;
  while(index++ < fStepN)
    while(fS->fPhysicalVolume[position++] != '\0');

  return position;
}

//---------------------------------------------------------------------------//

void MGG4StepViewerDump::SetDefaults()
{
  fMainFrameWidth = 350;
  fMainFrameHeight = 310;
}

//---------------------------------------------------------------------------//

void MGG4StepViewerDump::UpdateGUI()
{
  std::stringstream dumpStream("");
  if(fS->fTotalNumberOfSteps <=0) 
    dumpStream << " <EMPTY EVENT> " << "\n" << " fTotalNumberOfSteps <= 0";
  else 
    dumpStream
      << "fTotalNumberOfSteps  " << fS->fTotalNumberOfSteps << "\n"
      << "fParticleID          " << fS->fParticleID[fStepN] << "\n"
      << "fPreviousTrackID     " << fS->fPreviousTrackID[fStepN] << "\n"
      << "fTrackID             " << fS->fTrackID[fStepN] << "\n"
      << "fX(cm)               " << fS->fX[fStepN] << "\n"
      << "fY(cm)               " << fS->fY[fStepN] << "\n"
      << "fZ(cm)               " << fS->fZ[fStepN] << "\n"
      << "fPx(keV/c)           " << fS->fPx[fStepN] << "\n"
      << "fPy(keV/c)           " << fS->fPy[fStepN] << "\n"
      << "fPz(keV/c)           " << fS->fPz[fStepN] << "\n"
      << "fEdep(keV)           " << fS->fEdep[fStepN] << "\n"
      << "fG4Process           " << &fS->fG4Process[FindProcessIndex()] << "\n"
      << "fPhysicalVolume      " << &fS->fPhysicalVolume[FindVolumeIndex()] << "\n"
      << "fLocalX(cm)          " << fS->fLocalX[fStepN] << "\n"
      << "fLocalY(cm)          " << fS->fLocalY[fStepN] << "\n"
      << "fLocalZ(cm)          " << fS->fLocalZ[fStepN] << "\n"
      << "fStatus01            " << fS->fStatus01[fStepN] << "\n"
      << "fMaterialIndex       " << fS->fMaterialIndex[fStepN] << "\n"
      << "fGlobalTime          " << fS->fGlobalTime[fStepN] << "\n";

  std::string lineString("");
  std::string dumpString = dumpStream.str();

  size_t j, i = 0;
  fTextViewer->SetText(new TGText("")); // Clear buffer. Viewer deletes old buffer.
  do {
    j = i;
    i = dumpString.find("\n", j);
    if(i != std::string::npos) 
      lineString.assign(dumpString, j, i - j); 
    else 
      break;
    fTextViewer->AddLineFast(lineString.c_str());
    i++;
  } while(true);
  fTextViewer->Update();
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

