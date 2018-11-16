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
// $Id: MGProcessorAnalysis.cc,v 1.5 2006-06-29 17:14:53 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MGProcessorAnalysis.cc
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
 * FIRST SUBMISSION: Mon Dec  5 17:23:56 PST 2005
 * 
 * REVISION:
 *
 * 12-05-2005, Created, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjprocessors/MGProcessorAnalysis.hh"

//---------------------------------------------------------------------------//

ClassImp(MGProcessorAnalysis)

MGProcessorAnalysis::MGProcessorAnalysis(): 
  MGProcessor()
{;}

//---------------------------------------------------------------------------//

MGProcessorAnalysis::MGProcessorAnalysis(string pname, string cname): 
  MGProcessor(pname, cname)
{;}

//---------------------------------------------------------------------------//

MGProcessorAnalysis::MGProcessorAnalysis(string pname, string cname, TTree *tree): 
  MGProcessor(pname, cname, tree)
{;}

//---------------------------------------------------------------------------//

MGProcessorAnalysis::~MGProcessorAnalysis()
{
  DeleteAllProcessors();
}

//---------------------------------------------------------------------------//

void MGProcessorAnalysis::BeginOfRunAction()
{
  MGProcessor::BeginOfRunAction();
  list<MGProcessor*>::iterator  iterProc = fgProcessorList.begin();
  while(iterProc != fgProcessorList.end()) {
    MGLog(routine) << "BeginOfRun : " << (*iterProc)->GetName() << endlog;
    if(*iterProc != this) 
      (*iterProc)->BeginOfRunAction();
    iterProc++;
  }
}

//---------------------------------------------------------------------------//

void MGProcessorAnalysis::BeginOfEventAction()
{
  MGProcessor::BeginOfEventAction();
  list<MGProcessor*>::iterator  iterProc = fgProcessorList.begin();
  while(iterProc != fgProcessorList.end()) {
    if(*iterProc != this)
      (*iterProc)->BeginOfEventAction();
    iterProc++;
  }
}

//---------------------------------------------------------------------------//

void MGProcessorAnalysis::ProcessEventAction()
{
  MGProcessor::ProcessEventAction();
  list<MGProcessor*>::iterator  iterProc = fgProcessorList.begin();
  while(iterProc != fgProcessorList.end()) {
    if(*iterProc != this)
    {
      (*iterProc)->ProcessEventAction();
      MGLog(trace) << "ProcessEventAction for: " << (*iterProc)->GetName().c_str() << endlog;
    }
    iterProc++;
  }
}

//---------------------------------------------------------------------------//

void MGProcessorAnalysis::EndOfEventAction()
{
  MGProcessor::EndOfEventAction();
  list<MGProcessor*>::iterator  iterProc = fgProcessorList.begin();
  while(iterProc != fgProcessorList.end()) {
    if(*iterProc != this)
      (*iterProc)->EndOfEventAction();
    iterProc++;
  }
}

//---------------------------------------------------------------------------//

void MGProcessorAnalysis::EndOfRunAction()
{
  MGProcessor::EndOfRunAction();
  list<MGProcessor*>::iterator  iterProc = fgProcessorList.begin();
  while(iterProc != fgProcessorList.end()) {
    if(*iterProc != this)
      (*iterProc)->EndOfRunAction();
    iterProc++;
  }
}

//---------------------------------------------------------------------------//

void MGProcessorAnalysis::RunAllProcessors()
{
  list<MGProcessor*>::iterator  iterProc;
  Bool_t processorsDone = false;
  size_t  i = 0;

  BeginOfRunAction();
  MGLog(routine) << "Done with BeginOfRunActions." << endlog;
  while(!processorsDone) {
    if(i++%10000 == 0)
      MGLog(routine) << "Processing Event # " << i << endlog;
    BeginOfEventAction();
    ProcessEventAction();
    EndOfEventAction();
    iterProc = fgProcessorList.begin();
    processorsDone = true;
    while(iterProc != fgProcessorList.end()) {
      if((*iterProc) != this)
      {
          processorsDone = processorsDone && 
	    ((*iterProc)->GetStatus() == ProcessorNameSpace::kDoneProcessing);
	  /*          if(i%1 == 0) {
            ProcessorNameSpace::ProcessorStatus_t theStatus = (*iterProc)->GetStatus();
            MGLog(trace) << "Processor name: " << (*iterProc)->GetName().c_str() << endlog;
            switch(theStatus)
            {
              case(ProcessorNameSpace::kInstantiated):
                MGLog(trace) << "Processor Inst." << endlog;
                break;
              case(ProcessorNameSpace::kBeginOfRunAction):
                MGLog(trace) << "Processor BOR" << endlog;
                break;
              case(ProcessorNameSpace::kBeginOfEventAction):
                MGLog(trace) << "Processor BOE" << endlog;
                break;
              case(ProcessorNameSpace::kProcessEventAction):
                MGLog(trace) << "Processor PE" << endlog;
                break;
              case(ProcessorNameSpace::kEndOfEventAction):
                MGLog(trace) << "Processor EOE" << endlog;
                break;
              case(ProcessorNameSpace::kDoneProcessing):
                MGLog(trace) << "Processor DP" << endlog;
                break;
              case(ProcessorNameSpace::kEndOfRunAction):
                MGLog(trace) << "Processor EOR" << endlog;
                break;
              default:
                MGLog(trace) << "Unknown Status!" << endlog;
            }
	    }*/
      }
      iterProc++;
    }
  }
  fStatus = ProcessorNameSpace::kDoneProcessing;
  EndOfRunAction();
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
