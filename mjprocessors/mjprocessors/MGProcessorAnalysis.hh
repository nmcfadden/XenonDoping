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
 * $Id: MGProcessorAnalysis.hh,v 1.3 2006-01-09 18:19:51 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGProcessorAnalysis.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Base class for analyses. An analysis is a special processor that instantiates
 * all processors required for specific analysis and executes them.
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
 * FIRST SUBMISSION: Mon Dec  5 17:23:56 PST 2005
 * 
 * REVISION:
 * 
 * 12-05-2005, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGPROCESSORANALYSIS_HH
#define _MGPROCESSORANALYSIS_HH

//---------------------------------------------------------------------------//

#include <string>
#include "mjprocessors/MGProcessor.hh"

//---------------------------------------------------------------------------//

using namespace std;

class TTree;

class MGProcessorAnalysis : public MGProcessor
{
public:

  MGProcessorAnalysis();
  MGProcessorAnalysis(string pname, string cname);
  MGProcessorAnalysis(string pname, string cname, TTree *tree);
  virtual ~MGProcessorAnalysis();

  // Inherited Members.
  virtual void BeginOfRunAction();
  virtual void BeginOfEventAction();
  virtual void ProcessEventAction();
  virtual void EndOfEventAction();
  virtual void EndOfRunAction();
                                  
  // Non-inherited Members
  virtual void RunAllProcessors();

  ClassDef(MGProcessorAnalysis, 1)
};
#endif
