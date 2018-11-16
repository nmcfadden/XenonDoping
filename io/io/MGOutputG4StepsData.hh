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
 * $Id: MGOutputG4StepsData.hh,v 1.14 2007-05-03 18:14:42 jasondet Exp $
 *      
 * CLASS DECLARATION:  MGOutputG4Steps.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * ROOT output that write all G4 relevant G4 step data to tree. This can be 
 * used as a base class for a more detailed output.
 * This class is designed for speed and simplicity, but is not extremely
 * flexible. 
 * Units: cm, keV, keV/c, ns
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
 * FIRST SUBMISSION: Thu Dec 16 17:03:08 PST 2004
 * 
 * REVISION:
 * 
 * 12-16-2004, Created, R. Henning
 * 08-03-2005, Fixed bug with fTrackID not being written to root tree.R.Henning
 * 12-22-2005, Added location in local coordinate systems to output.
 *             Increased version numbr to 2. R. Henning
 * 01-02-2006, Added physical volume name. Made corrections to comments. 
 *             Increased version number to 3. R. Henning
 * 09-20-2006, Increased version number to 40. R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGOUTPUTG4STEPSDATA_HH
#define _MGOUTPUTG4STEPSDATA_HH

#include "TObject.h"
#include <cstring>
class MGCrystalHits;

class MGOutputG4StepsData : public TObject
{
public:
  MGOutputG4StepsData();
  ~MGOutputG4StepsData();

  inline bool ArraysAreAllocated() { return fParticleID != NULL; }
  size_t AllocateArrays(size_t maxArrayLength = 50000, size_t avgLengthProcessName = 30); // returns maxProcArrayLength
  void DeallocateArrays();
  void FillCrystalHits(MGCrystalHits* xtalHits);

  // function relevant for generating waveforms - want an integer
  // associated with each crystal.
  Int_t GetCrystalID(size_t iStep) const;

  // Arrays that save step data for each event during G4 tracking. 

  Int_t      fTotalNumberOfSteps;  // Total number of steps for this event.
  Float_t     fTotalSensitiveEdep;   // Quick & dirty sum of energy deposit in
                                     // all sensitive detectors
  Int_t       *fParticleID;          //[fTotalNumberOfSteps]

  // Geant 4 Particle ID
  Int_t       *fPreviousTrackID;     //[fTotalNumberOfSteps] 

  // Link to step that led to this one.
  Int_t       *fTrackID;  //[fTotalNumberOfSteps]

  // Position units are cm.
  Float_t     *fX;  //[fTotalNumberOfSteps] 
  Float_t     *fY;  //[fTotalNumberOfSteps]
  Float_t     *fZ;  //[fTotalNumberOfSteps]
  // World coordinates at end of step.

  // Momentum units are keV/c
  Float_t     *fPx;      //[fTotalNumberOfSteps]
  Float_t     *fPy;      //[fTotalNumberOfSteps]
  Float_t     *fPz;      //[fTotalNumberOfSteps]
  // Momentum at end of steps

  Float_t     *fEdep;                //[fTotalNumberOfSteps]
  // Particle energy deposit at this step in keV.

  Int_t       fProcessNamePos; 
  //Length of array containing names of processes.

  Char_t      *fG4Process;           //[fProcessNamePos] 
  //G4 process that terminated step.

  Int_t       fVolumeNamePos;    
  // Length of arrays containing names of physical volumes.

  Char_t      *fPhysicalVolume;     //[fVolumeNamePos]

  Float_t     *fLocalX;  //[fTotalNumberOfSteps]
  Float_t     *fLocalY;  //[fTotalNumberOfSteps]
  Float_t     *fLocalZ;  //[fTotalNumberOfSteps]
  // Location in local physical volumes coordinate system (cm).

  Int_t       *fStatus01;            //[fTotalNumberOfSteps] 
  //Status word.

  Int_t       *fMaterialIndex;       //[fTotalNumberOfSteps]
  // Geant 4 material Index

  Double_t     *fGlobalTime;          //[fTotalNumberOfSteps] 
  //Time since start of current event.  This may be reset if an event is to be
  //broken into separate events with an appropriate TimeWindow.
  Double_t     *fOffsetTime;          //[fTotalNumberOfSteps] 
  //Should an event be reset and broken into separate events, this stores the
  //time that gets reset.

  ClassDef(MGOutputG4StepsData, 4)
};
#endif
