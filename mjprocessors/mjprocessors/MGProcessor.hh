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
 * $Id: MGProcessor.hh,v 1.6 2006-06-29 17:14:53 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGProcessor.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Base class of processors that analyze Monte Carlo and real data.
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
 * FIRST SUBMISSION: Sun Dec  4 13:56:56 PST 2005
 * 
 * REVISION:
 * 
 * 06-22-2006, Cast Getters as const, R. Henning. 
 * 12-o4-2005, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGPROCESSOR_HH
#define _MGPROCESSOR_HH

//---------------------------------------------------------------------------//

#include <list>
#include <string>

#include "TObject.h"
#include "TTree.h"

//---------------------------------------------------------------------------//

namespace ProcessorNameSpace {

  enum ProcessorStatus_t {
    kInstantiated, // Processor has been instantiated, but nothing more. 
    kBeginOfRunAction, // BeginofRunAction() is being or has been executed last.
    kBeginOfEventAction, // BeginOfEventAction() is being or has been executed last.
    kProcessEventAction, // ProcessEventAction() is being or has been executed last.
    kEndOfEventAction,  // EndOfEventAction() is being or has been executed last.
    kDoneProcessing, // All event have been processed. 
    kEndOfRunAction}; // EndOfRunAction is being or hhas been executed last.
}

using namespace std;

//---------------------------------------------------------------------------//

class MGProcessor : public TObject
{
public:
  MGProcessor();
  MGProcessor(string pname, string cname);
  MGProcessor(string pname, string cname, TTree *tree);
  virtual ~MGProcessor();

  virtual void AddToTree(Int_t bufferSize = 32000, Int_t splitLevel = 2);
  virtual void BeginOfRunAction() 
  { 
    if(fStatus != ProcessorNameSpace::kDoneProcessing)
      fStatus = ProcessorNameSpace::kBeginOfRunAction; 
  }

  virtual void BeginOfEventAction()
  { 
    if(fStatus != ProcessorNameSpace::kDoneProcessing)
      fStatus = ProcessorNameSpace::kBeginOfEventAction; 
  }

  virtual void ProcessEventAction()
  { 
    if(fStatus != ProcessorNameSpace::kDoneProcessing)
      fStatus = ProcessorNameSpace::kProcessEventAction; 
  }

  virtual void EndOfEventAction()
  { 
    if(fStatus != ProcessorNameSpace::kDoneProcessing)
      fStatus = ProcessorNameSpace::kEndOfEventAction; 
  }

  virtual void EndOfRunAction()
  { 
    fStatus = ProcessorNameSpace::kEndOfRunAction; 
  }

  // Static Methods.
  static  MGProcessor *GetProcessor(string name);
  static  void DeleteAllProcessors();

  // Setters/Getters
  const string            GetName() { return fProcessorName; }
  const ProcessorNameSpace::ProcessorStatus_t GetStatus() { return fStatus; }
  void              SetStatus(ProcessorNameSpace::ProcessorStatus_t status) { fStatus = status; }
  void              SetTree(TTree* tree) { fTree = tree; }

protected:

  static list<MGProcessor*>   fgProcessorList; //! Static list of all instantiated prcoessors.
  ProcessorNameSpace::ProcessorStatus_t    fStatus; //! Status of the processor.
  string               fProcessorName; //! Name of this processor.
  string               fClassName;     //! Literal name of derived class. Used by ROOT.
  TTree                *fTree; //! Tree that variable from derived class are written to, if req.

private:
  MGProcessor         *fSelfPtr; //!

  ClassDef(MGProcessor,1)
};
#endif
