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
 * $Id: MGAnalysisMCSegmentation.hh,v 1.3 2006-01-23 08:37:20 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGAnalysisMCSegmentation.hh
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
// --------------------------------------------------------------------------//

#ifndef _MGANALYSISMCSEGMENTATION_HH
#define _MGANALYSISMCSEGMENTATION_HH

//---------------------------------------------------------------------------//

#include <string>
#include <vector>

#include "mjprocessors/MGProcessorAnalysis.hh"
#include "mjprocessors/MGProcessorMCReadTree.hh"

//---------------------------------------------------------------------------//

class TFile;

class MGProcessorMCRadialDiscriminator;
class MGProcessorMCSegmentation;
class MGProcessorMCTimeCluster;

class MGAnalysisMCSegmentation : public MGProcessorAnalysis
{
public:

  MGAnalysisMCSegmentation();
  virtual ~MGAnalysisMCSegmentation();

  // Inherited Methods
  virtual void BeginOfRunAction();
  virtual void EndOfRunAction();
  virtual void ProcessEventAction();

  // Adders, Setters.
  void AddMCFile(string name) { fProcessorMCReadTree->AddMCFile(name); }
  void SetOutputFileName(string name) { fFileName = name; }

private:
  MGProcessorMCReadTree            *fProcessorMCReadTree; //!
  MGProcessorMCTimeCluster         *fProcessorMCTimeCluster; //!
  MGProcessorMCRadialDiscriminator *fProcessorMCRadialDiscriminator; //!
  string   fFileName; //!
  TFile    *fFile; //!
  vector<MGProcessorMCSegmentation*>  fProcessorMCSegmentation; //!
};
#endif
