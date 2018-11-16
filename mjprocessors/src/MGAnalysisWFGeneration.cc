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
// $Id: MGAnalysisWFGeneration.cc,v 1.1 2006-05-02 05:38:01 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MGAnalysisWFGeneration.cc
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
 * FIRST SUBMISSION: Wed Jan  4 11:48:06 PST 2006
 * 
 * REVISION:
 *
 * 1-4-2005, Created, R. Henning
 *
 */

#include "io/MGLogger.hh"
#include "mjprocessors/MGAnalysisWFGeneration.hh"

MGAnalysisWFGeneration::MGAnalysisWFGeneration(MGVWaveformGenerator* wfGenerator) : 
MGProcessorAnalysis("MGAnalysisWFGeneration", "MGAnalysisWFGeneration")
{
  // Note that order of intantiation is important. It is the order in which the
  // processor's methods will be evaluated. 

  //  Create Processor to read file
  fProcessorMCReadTree = new MGProcessorMCReadTree("MGProcessorMCReadTree");

  //  Create a waveform generation processor
  fProcessorWFGeneration = new MGProcessorWFGeneration(wfGenerator, "MGProcessorWFGeneration");
}


