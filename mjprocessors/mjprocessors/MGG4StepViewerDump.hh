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
 * $Id: MGG4StepViewerDump.hh,v 1.1 2006-07-10 00:53:41 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGG4StepViewerDump.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Creates ROOT GUI window showing all data from one step in MGOutputG4StepsData class.
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
 * FIRST SUBMISSION: Thu Jul  6 14:34:19 PDT 2006
 * 
 * REVISION:
 * 
 * 07-06-2006, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGG4STEPVIEWERDUMP_HH
#define _MGG4STEPVIEWERDUMP_HH

//---------------------------------------------------------------------------//

#include "RQ_OBJECT.h"

//---------------------------------------------------------------------------//

class MGOutputG4StepsData;
class TGMainFrame;
class TGTextView;
class TGWindow;

class MGG4StepViewerDump 
{
  RQ_OBJECT("MGG4StepViewerDump")

public:

  MGG4StepViewerDump(const TGWindow *p, const MGOutputG4StepsData *sdata);
  ~MGG4StepViewerDump();

  void CreateGUI();
  void SetDefaults();
  void SetG4Steps(const MGOutputG4StepsData *sdata) { fS = sdata; }
  void SetStepNumber(Int_t stepn) { fStepN = stepn; }
  void UpdateGUI();

private:

  Int_t        FindProcessIndex();
  Int_t        FindVolumeIndex();

  Int_t        fStepN;
  Int_t        fMainFrameWidth;
  Int_t        fMainFrameHeight;

  const TGWindow     *fRootWindow;
  TGMainFrame  *fMainFrame;
  TGTextView   *fTextViewer;

  const MGOutputG4StepsData   *fS;

};
#endif
