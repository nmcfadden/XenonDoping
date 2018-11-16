//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                         MAJORANA Simulation                               //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA Collaboration. It is based on Geant4, an intellectual       //
//      property of the RD44 GEANT4 collaboration.                           //
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
 * $Id: MJOutputLArGeNoPS.hh,v 1.3 2004-12-17 23:08:46 dimarcom Exp $
 *      
 * CLASS DECLARATION:  MJOutputLArGeNoPS.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Description of Root tree for LANL clover without pulse shape data. 
 * There is no accompaying .cc file.
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Tue Jun 29 14:51:40 PDT 2004
 * 
 * REVISION:
 * 
 * 06-29-2004, Created, R. Henning
 * 07-20-2004, Added MC Info header, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTLARGENOPS_HH
#define _MJOUTPUTLARGENOPS_HH

//---------------------------------------------------------------------------//

#define MAX_N_PARTICLES    20

#include "TObject.h"

//---------------------------------------------------------------------------//

class MJOutputLArGeNoPS 
{
public:
  MJOutputLArGeNoPS() {;}
  virtual ~MJOutputLArGeNoPS() {;}

  // MC data.
  UInt_t   EventNumber;
  Int_t    NumberOfParticles;
  Int_t    Particle[MAX_N_PARTICLES];
  Float_t  MomentumX[MAX_N_PARTICLES];
  Float_t  MomentumY[MAX_N_PARTICLES];
  Float_t  MomentumZ[MAX_N_PARTICLES];
  Float_t  PositionX[MAX_N_PARTICLES];
  Float_t  PositionY[MAX_N_PARTICLES];
  Float_t  PositionZ[MAX_N_PARTICLES];
  Float_t  E1_MC; // Energy deposits in keV from Monte Carlo truth.
  Float_t  E2_MC;
  Float_t  E3_MC;
  Float_t  E4_MC;
  Float_t  LAr_MC;
  Float_t  Ge_MC;
  Float_t  NoCal_MC;

};
#endif
