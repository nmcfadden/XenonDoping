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
 * $Id: MGProcessorMCReadTree.hh,v 1.5 2006-07-08 00:01:28 jasondet Exp $
 *      
 * CLASS DECLARATION:  MGProcessorMCReadTree.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Read MaGe MC RD Tree and save event data in internal structure. 
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
 * FIRST SUBMISSION: Sun Dec  4 15:01:33 PST 2005
 * 
 * REVISION:
 * 
 * 12-04-2005, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGPROCESSORMCREADTREE_HH
#define _MGPROCESSORMCREADTREE_HH

//---------------------------------------------------------------------------//

#include <list>
#include <string>

#include "TChain.h"

#include "io/MGLogger.hh"
#include "mjprocessors/MGProcessor.hh"

//---------------------------------------------------------------------------//

class MGOutputG4StepsData;

class MGProcessorMCReadTree : public MGProcessor
{
public:

  MGProcessorMCReadTree();
  MGProcessorMCReadTree(string name);
  virtual ~MGProcessorMCReadTree();

  // Inherited:
  virtual void AddToTree(){;}        // Do not write this class to tree.
  virtual void BeginOfRunAction();   // Allocate Arrays.
  virtual void BeginOfEventAction(); // Read tree from file

  // Unique to the class
  void AddMCFile(string name); // Add MC data file to chain.
  void SetBranchStatus(const Char_t* status, Int_t val){fTChain->SetBranchStatus(status, val);}

  //Setters/Getters;
  const MGOutputG4StepsData* GetG4StepsData() { return fG4StepsData; } // Other processors access
  void  SetMaxNumberOfSteps(Int_t nstep) { fMaxNumberOfSteps = nstep; }
  Long64_t GetEntryNumber() { return fEntryNumber; }
  void  SetNEntriesToProcess(Int_t nEntries) { fNEntriesToProcess = nEntries; }

private:
  Long64_t                fEntryNumber;  //! Entry # to be processed
  Long64_t                fNEntriesToProcess; //! Number of entries to process
  MGOutputG4StepsData     *fG4StepsData;  //! MC data from file is written here per event.
  Int_t                   fMaxNumberOfSteps; //! Max size for arrays in fG4StepsData
  list<string>            fMCFileNames; //! MC output files to chain together.
  TChain                  *fTChain; //! Chain of MC files.

  // This processor has no class variables that have to written to a root tree.
};
#endif
