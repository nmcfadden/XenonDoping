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
 * $Id: MGAnalysisWFGeneration.hh,v 1.1 2006-05-02 05:38:01 jasondet Exp $
 *      
 * CLASS DECLARATION:  MGAnalysisWFGeneration.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Simple segmentation scheme analysis
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
 * FIRST SUBMISSION: Wed Jan  4 11:48:06 PST 2006
 * 
 * REVISION:
 * 
 * 01-04-2006, Created, R. Henning
 *
 */

#ifndef _MGANALYSISWFGENERATION_HH
#define _MGANALYSISWFGENERATION_HH


#include <string>

#include "mjprocessors/MGProcessorAnalysis.hh"
#include "mjprocessors/MGProcessorMCReadTree.hh"
#include "mjprocessors/MGProcessorWFGeneration.hh"

class MGAnalysisWFGeneration : public MGProcessorAnalysis
{
public:

  MGAnalysisWFGeneration(MGVWaveformGenerator* wfGenerator);
  virtual ~MGAnalysisWFGeneration() {}

  // Adders, Setters.
  void AddMCFile(string name) { fProcessorMCReadTree->AddMCFile(name); }
  void SetNEntriesToProcess(int nEntries) { fProcessorMCReadTree->SetNEntriesToProcess(nEntries); }

private:
  MGProcessorMCReadTree* fProcessorMCReadTree; 
  MGProcessorWFGeneration* fProcessorWFGeneration; 
};
#endif
