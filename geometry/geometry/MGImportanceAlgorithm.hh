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
 * $Id: MGImportanceAlgorithm.hh,v 1.2 2007-01-25 06:14:42 mgmarino Exp $
 *      
 * CLASS DECLARATION:  MGImportanceAlgorithm.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Based upon G4ImportanceAlgorithm with additions in order to provide better
// debugging information.  This class will output errors if the importance ratio
// of a particle leaving a region and entering another exceeds a certain value.
/**
 *
 *
 *
 *
 */
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: M. Marino
 * CONTACT: mgmarino@u.washington.edu
 * FIRST SUBMISSION: 4 Nov 2005
 * 
 * REVISION:
 * 
 */
// --------------------------------------------------------------------------//


#ifndef MGImportanceAlgorithm_hh
#define MGImportanceAlgorithm_hh MGImportanceAlgorithm_hh

#include "G4VImportanceAlgorithm.hh"

class MGImportanceAlgorithm : public G4VImportanceAlgorithm
{

public:  

  MGImportanceAlgorithm();
    // simple construction

  virtual ~MGImportanceAlgorithm();
    // repeat warning at the end of the run if triggered
    // at anytime during the run.

  virtual G4Nsplit_Weight Calculate(G4double ipre, 
				    G4double ipost, 
				    G4double init_w) const;
    // calculate the number of tracks and their weight according 
    // to the pre and post importance value and the weight of
    // the mother track.
  void SetLowerBound(G4double aLowerBound) {fLowerBound = aLowerBound;};
    // sets lower bound of importance value ratio (ipre_over_ipost)
    // to report on  
private:

  void Error(const G4String &m) const;
  void Warning(const G4String &m) const;

private:
  G4double fLowerBound;
  mutable G4bool fWarnedLesser;
  mutable G4bool fWarnedGreater;
};

#endif
