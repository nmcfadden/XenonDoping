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
 * $Id: MGG4StepViewerGraphic.hh,v 1.1 2006-09-20 23:16:45 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGG4StepViewerGraphic.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Graphical windows displaying particle trajectory.
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
 * FIRST SUBMISSION: Tue Jul 11 14:13:46 PDT 2006
 * 
 * REVISION:
 * 
 * 07-12-2006, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGG4STEPVIEWERGRAPHIC_HH
#define _MGG4STEPVIEWERGRAPHIC_HH

//---------------------------------------------------------------------------//

#include "Gtypes.h"
#include "RQ_OBJECT.h"

//---------------------------------------------------------------------------//

class MGG4StepViewer;
class MGOutputG4StepsData;
class TGMainFrame;
class TGWindow;
class TRootEmbeddedCanvas;

class MGG4StepViewerGraphic
{ 
  RQ_OBJECT("MGG4StepViewerGraphic")

public:

  MGG4StepViewerGraphic(const TGWindow *p, 
			const MGOutputG4StepsData *sdata,
			const MGG4StepViewer *viewer);
  ~MGG4StepViewerGraphic();

  void ClearAllGraphs();
  void CreateGUI();
  void DrawNextStep();
  void DrawStepSequence();
  void SetDefaults();
  void SetEndStep(Int_t estep) { fEndStep = estep; }
  void SetStartStep(Int_t sstep) { fStartStep = sstep; }
  void UpdateGUI();

private:
  Color_t    GetParticleColor(Int_t particle_ID);

  Int_t                        fStartStep;  // First step to be drawn.
  Int_t                        fEndStep;    // Last step drawn 
  const TGWindow              *fRootWindow;
  Bool_t                       fUseLocalCoords;
  TGMainFrame                 *fMainFrame;
  Int_t                       fMainFrameHeight;
  Int_t                       fMainFrameWidth;
  TRootEmbeddedCanvas         *fGraphXYCanvas;
  const MGOutputG4StepsData   *fS;
  const MGG4StepViewer        *fG4StepViewer;

  // Colors for particle tracks.
  Color_t                      fElectronColor;
  Color_t                      fGammaColor;
  Color_t                      fPositronColor;
  Color_t                      fNeutronColor;
  Color_t                      fNucleusColor;
  Color_t                      fAlphaColor;
  Color_t                      fOtherColor;
};
#endif
