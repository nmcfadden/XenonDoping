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
 * $Id: MGGeneratorSPS.hh,v 1.1 2005-06-30 17:40:43 rhenning Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorSPS.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Interface to Geant 4 SPS generator. 
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
 * FIRST SUBMISSION: Mon Jun 20 17:52:06 PDT 2005
 * 
 * REVISION:
 * 
 * 06-20-2005, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORSPS_HH
#define _MGGENERATORSPS_HH

//---------------------------------------------------------------------------//

#include "G4ThreeVector.hh"
#include "generators/MGVGenerator.hh"

//---------------------------------------------------------------------------//

class G4Event;
class G4GeneralParticleSource;

class MGGeneratorSPS : public MGVGenerator
{
public:

  MGGeneratorSPS();

  ~MGGeneratorSPS();
  void GeneratePrimaryVertex(G4Event *event);
  void SetParticlePosition(G4ThreeVector vec);

private:
  G4GeneralParticleSource    *fParticleSource;
  
};
#endif
