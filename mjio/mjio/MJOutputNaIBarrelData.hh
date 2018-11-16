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
 * $Id: MJOutputNaIBarrelData.hh,v 1.1 2004-12-08 02:02:21 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJOutputNaIBarrelData.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Output data saved in ROOT tree for NaI barrel
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
 * FIRST SUBMISSION: Mon Aug  9 10:19:07 PDT 2004
 * 
 * REVISION:
 * 
 * 09-21-2004, Added SegEdep, R. Henning
 * 08-09-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTNAIBARRELDATA_HH
#define _MJOUTPUTNAIBARRELDATA_HH

//---------------------------------------------------------------------------//

class MJOutputNaIBarrelData 
{
public:

  MJOutputNaIBarrelData(){;}
  virtual ~MJOutputNaIBarrelData() {;}

  Float_t  TotEdepNaI;
  Float_t  SegEdep[16];
  UInt_t   FlagNaI;
};
#endif
