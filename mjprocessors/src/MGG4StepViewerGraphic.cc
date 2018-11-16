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
// $Id: MGG4StepViewerGraphic.cc,v 1.1 2006-09-20 23:16:45 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MGG4StepViewerGraphic.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Tue Jul 11 14:13:46 PDT 2006
 * 
 * REVISION:
 *
 * 07-20-2006, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include <iostream>

#include "TCanvas.h"
#include "TGFrame.h"
#include "TGraph.h"
#include "TLine.h"
#include "TPad.h"
#include "TRootEmbeddedCanvas.h"
 
#include "io/MGOutputG4StepsData.hh"
#include "mjprocessors/MGG4StepViewer.hh"

//---------------------------------------------------------------------------//

#include "mjprocessors/MGG4StepViewerGraphic.hh" 

//---------------------------------------------------------------------------//

MGG4StepViewerGraphic::MGG4StepViewerGraphic(const TGWindow *p, 
					     const MGOutputG4StepsData *sdata, 
					     const MGG4StepViewer *viewer):
  fRootWindow(p), fMainFrame(0), fGraphXYCanvas(0), fS(sdata), fG4StepViewer(viewer)
{
  SetDefaults();
  CreateGUI();
}

//---------------------------------------------------------------------------//

MGG4StepViewerGraphic::~MGG4StepViewerGraphic()
{
  fMainFrame->Cleanup();
  delete fMainFrame;
}


//---------------------------------------------------------------------------//

void MGG4StepViewerGraphic::ClearAllGraphs()
{
}

//---------------------------------------------------------------------------//

void MGG4StepViewerGraphic::CreateGUI()
{
   // Create Main Frame
  fMainFrame = new TGMainFrame(fRootWindow, fMainFrameWidth, fMainFrameHeight);

  // Create canvas widget for XY graph.
  fGraphXYCanvas = new TRootEmbeddedCanvas("XYGraphCanvas", fMainFrame, 
					   fMainFrameWidth/2, fMainFrameHeight/2);
  fMainFrame->AddFrame(fGraphXYCanvas, 
		       new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 1));
  
  // Finish and display GUI.
  // -----------------------

  fMainFrame->SetWindowName("MGOutputG4StepsData Graphical View");
  fMainFrame->MapSubwindows();
  fMainFrame->Resize(fMainFrame->GetDefaultSize());
  fMainFrame->MapWindow();
  fMainFrame->Print();
}

//---------------------------------------------------------------------------//

void MGG4StepViewerGraphic::DrawNextStep()
{
}

//---------------------------------------------------------------------------//

void MGG4StepViewerGraphic::DrawStepSequence()
{
  if(fStartStep > fEndStep)
    std::cout << "Start step > end step! Try again." << std::endl;
  else if (fEndStep > fS->fTotalNumberOfSteps || fEndStep < 0 || fStartStep < 0)
    std::cout << "End step out of range." << std::endl;
  else {
    TCanvas *canvas = fGraphXYCanvas->GetCanvas();
    canvas->Clear();
    canvas->cd();
    Double_t minX = 1e6;
    Double_t minY = 1e6;
    Double_t maxX = -1e6;
    Double_t maxY = -1e6;
    Float_t *x1, *y1;
    TLine *currentLine;
    if(fUseLocalCoords) {
      x1 = fS->fLocalX;
      y1 = fS->fLocalY;
    } else {
      x1 = fS->fX;
      y1 = fS->fY;
    }
    for(Int_t i = fStartStep; i <= fEndStep; i++) 
      if(TMath::Abs(fS->fParticleID[i]) != 12 &&  // Ignore neutrinos 
	 fS->fTrackID[i] == fS->fTrackID[i+1]) {
	currentLine = new TLine(x1[i], y1[i], x1[i+1], y1[i+1]);
	currentLine->SetLineColor(GetParticleColor(fS->fParticleID[i]));
	currentLine->Draw();
	if(x1[i] > maxX) 
	  maxX = x1[i]; 
	if(x1[i] < minX) 
	  minX = x1[i];
	if(y1[i] > maxY) 
	  maxY = y1[i]; 
	if(y1[i] < minY) 
	  minY = y1[i];
      }

    std::cout << minX << " " << minY << " " << maxX << " " << maxY << std::endl;

    // Maintain aspect ratio.
    Double_t  deltaX = maxX - minX;
    Double_t  deltaY = maxY - minY;
    if(deltaY > deltaX)
      canvas->Range(minX - 0.1*deltaX, minY - 0.1*deltaY, 
		    minX + maxY - minY + 0.1*deltaX, maxY + 0.1*deltaY);
    else
      canvas->Range(minX - 0.1*deltaX, minY - 0.1*deltaY, 
		    maxX + 0.1*deltaX, minY + maxX - minX + 0.1*deltaY);
    canvas->Update();
  }
}

//---------------------------------------------------------------------------//

Color_t MGG4StepViewerGraphic::GetParticleColor(Int_t particle_ID)
{
  switch(particle_ID) {
    case 11:   return fElectronColor;
    case 22:   return fGammaColor;
    case -11:  return fPositronColor;
    case 2112: return fNeutronColor;
  }
  if(particle_ID == 100004002) return fAlphaColor;
  if(particle_ID > 100000000) return fNucleusColor;
  return fOtherColor;
}

//---------------------------------------------------------------------------//

void MGG4StepViewerGraphic::SetDefaults()
{
  fMainFrameWidth = 1000;
  fMainFrameHeight = 1000;
  fUseLocalCoords = false;
  fElectronColor = 2; // Red
  fGammaColor = 3;    // Light Green
  fPositronColor = 4; // Blue
  fNeutronColor = 1;  // Black
  fNucleusColor = 7;  // Light blue
  fAlphaColor = 6;    // Purple
  fOtherColor = 16;   // Grey
}

//---------------------------------------------------------------------------//

void MGG4StepViewerGraphic::UpdateGUI()
{
  DrawStepSequence();
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
