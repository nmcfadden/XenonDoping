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
 * $Id: MGProcessorWFGeneration.hh,v 1.1 2006-05-02 05:38:01 jasondet Exp $
 *      
 * CLASS DECLARATION:  MGProcessorWFGeneration.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Simple, fast processor to convert Raw MC data into segmented crystal hits. 
 * Requires MGProcessorMCReadTree
 * Incorporates results from MGProcessorMCRadialDiscriminator, if instantiated.
 * Assumptions:
 *   1.) All crystal have same segmentation.
 *   2.) No relative rotation between crystals.
 *   3.) Segmentation scheme is MxN where M is the # of phi segments and 
 *       N is the number of z-segments.
 *   4.) Crystal is divided into segments and energy deposit in each segment
 *       is reported. 
 *   5.) Crystal is coaxial.
 *   6.) Crystal axis is parallel to z-axis.
 *   7.) First segment starts at +x-axis
 *   8.) Segments counted as (m,n) : 0 - (0,0); 1 - (0,1); ...; N - (0,N);
 *       N+1 - (1, 0); ...; MxN-1 - (M, N).
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
 * FIRST SUBMISSION: Sun Dec  4 14:58:46 PST 2005
 * 
 * REVISION:
 * 
 * 12-04-2005, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGPROCESSORMCSEGMENTATION_HH
#define _MGPROCESSORMCSEGMENTATION_HH


#include "mjprocessors/MGProcessor.hh"
#include "waveform/MGWaveformManager.hh"


class TH1F;

class MGProcessorMCReadTree;

class MGProcessorWFGeneration : public MGProcessor
{
public:

  MGProcessorWFGeneration(MGVWaveformGenerator* waveformGenerator, std::string name = "MGProcessorWFGeneration");
  virtual ~MGProcessorWFGeneration() {}

  // Inherited methods.
  virtual void BeginOfRunAction();
  virtual void ProcessEventAction();
  virtual void EndOfRunAction();


private:
  MGWaveformManager fWFManager;

  MGProcessorMCReadTree* fReadTreeProcessor;
};
#endif
