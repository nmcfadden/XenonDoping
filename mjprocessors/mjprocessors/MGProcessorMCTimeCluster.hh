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
 * $Id: MGProcessorMCTimeCluster.hh,v 1.2 2006-06-29 17:14:53 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGProcessorMCTimeCluster.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Created a <vector> of seed times from MGOutputG4StepsData clustered within 
 * fTimeResolution seconds. 
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
 * FIRST SUBMISSION: Tue Jan 10 16:46:26 PST 2006
 * 
 * REVISION:
 * 
 * 01-10-2006, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGPROCESSORMCTIMECLUSTER_HH
#define _MGPROCESSORMCTIMECLUSTER_HH

//---------------------------------------------------------------------------//

#include <list>

#include "mjprocessors/MGProcessor.hh"

//---------------------------------------------------------------------------//

class MGOutputG4StepsData;
class MGProcessorMCReadTree;

class MGProcessorMCTimeCluster : public MGProcessor
{
public:

  MGProcessorMCTimeCluster();
  virtual ~MGProcessorMCTimeCluster();

  // Inherited Methods.
  virtual void BeginOfRunAction();
  virtual void ProcessEventAction();

  // Setters, Getters
  Double_t  GetSeedTimeAndIncrement() 
  { 
    return (fSeedTimesIter == fSeedTimes.end() || fSeedTimes.size() == 0) ? -1.0 : *fSeedTimesIter++;
  }
  size_t    GetNumberOfSeedTimes() { return fSeedTimes.size(); }
  void      ResetSeedTimeIterator() { fSeedTimesIter = fSeedTimes.begin(); }
  void      SetTimeResolution(Double_t tres) { fTimeResolution = tres; }

private:
  list<Double_t>               fSeedTimes;            //!
  list<Double_t>::iterator     fSeedTimesIter;        //!
  Double_t                     fTimeResolution;       //!
  const MGOutputG4StepsData    *fStepsData;           //!
  MGProcessorMCReadTree        *fProcessorMCReadTree; //!
};
#endif
