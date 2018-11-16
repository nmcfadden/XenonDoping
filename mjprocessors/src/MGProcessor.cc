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
// $Id: MGProcessor.cc,v 1.3 2006-02-21 19:42:32 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessor.cc
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
 * FIRST SUBMISSION: Sun Dec  4 13:56:56 PST 2005
 * 
 * REVISION:
 *
 * 12/4/2005, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include "TObject.h"
#include "TTree.h"

#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjprocessors/MGProcessor.hh"

ClassImp(MGProcessor)

using namespace std;

//---------------------------------------------------------------------------//

list<MGProcessor*> MGProcessor::fgProcessorList;

//---------------------------------------------------------------------------//

MGProcessor::MGProcessor():
  fStatus(ProcessorNameSpace::kInstantiated),
  fProcessorName(""), fClassName("MGProcessor"), fTree(0)
{
  MGLog(routine) << "Instantiated MGProcessor w/ defaults constructor."<<endlog;
  fgProcessorList.push_back(this);
  fSelfPtr = this;
}

//---------------------------------------------------------------------------//

MGProcessor::MGProcessor(string pname, string cname):
  fStatus(ProcessorNameSpace::kInstantiated),
  fProcessorName(pname), fClassName(cname), fTree(0) 
{
  MGLog(routine) << "Instantiated " << pname << " of type " << cname << endlog;
  fgProcessorList.push_back(this);
  fSelfPtr = this;
}

//---------------------------------------------------------------------------//

MGProcessor::MGProcessor(string pname, string cname, TTree *tree):
  fStatus(ProcessorNameSpace::kInstantiated),
  fProcessorName(pname), fClassName(cname), fTree(tree)
{  
  fgProcessorList.push_back(this);
  fSelfPtr = this;
}

//---------------------------------------------------------------------------//

MGProcessor::~MGProcessor()
{
  MGLog(routine) << "Deleting: " << fProcessorName << endlog;
}

//---------------------------------------------------------------------------//

void MGProcessor::AddToTree(Int_t bufferSize, Int_t splitLevel)
{
  MGLog(routine) << fProcessorName << " added to tree as class " << fClassName
		 << " with bufferSize and split level: " << bufferSize
		 << " " << splitLevel << endlog;
  if(!fTree) 
    MGLog(error) << "fTree = NULL. Create tree before adding processor." << endlog;
  else if(fTree->GetBranch(fProcessorName.c_str()))
    MGLog(warning) << "Branch for this processor already exists." << endlog;
  else
    fTree->Branch(fProcessorName.c_str(), fClassName.c_str(), &fSelfPtr, 
		  bufferSize, splitLevel);
}

//---------------------------------------------------------------------------//

void MGProcessor::DeleteAllProcessors()
{
  // Delete all, but first processor, which should be the analysis processor that
  // calls this method.
  list<MGProcessor*>::iterator  pIter = fgProcessorList.end();
  while(fgProcessorList.begin() != --pIter){
    delete *pIter;
    fgProcessorList.pop_back();
  }
}

//---------------------------------------------------------------------------//

MGProcessor* MGProcessor::GetProcessor(string name)
{
  MGProcessor *ptr = 0;
  list<MGProcessor*>::iterator  pIter = fgProcessorList.begin();
  while(!ptr && pIter != fgProcessorList.end()) {
    if((*pIter)->GetName() == name)
      ptr = *pIter;
    pIter++;
  }
  return ptr;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
