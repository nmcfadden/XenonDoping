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
 * $Id: MJGeometryRDCrystalColumnMessenger.hh,v 1.1 2005-11-17 00:28:48 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometryRDCrystalColumnMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *
 *
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
 * FIRST SUBMISSION: Thu Nov 10 09:08:30 PST 2005
 * 
 * REVISION:
 * 
 * 11-11-2005, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYRDCRYSTALCOLUMNMESSENGER_HH
#define _MJGEOMETRYRDCRYSTALCOLUMNMESSENGER_HH

//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//

class MJGeometryRDCrystalColumn;

class MJGeometryRDCrystalColumnMessenger 
{
public:

  MJGeometryRDCrystalColumnMessenger(MJGeometryRDCrystalColumn *parent);
  MJGeometryRDCrystalColumnMessenger(const MJGeometryRDCrystalColumnMessenger &);
  ~MJGeometryRDCrystalColumnMessenger();

private:

};
#endif
