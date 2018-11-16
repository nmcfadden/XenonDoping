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
 * $Id: MJGeometryNaIBarrel.hh,v 1.2 2004-12-08 01:21:01 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometryNaIBarrel.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * NaI veto Barrel for TUNL FEL test.
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
 * FIRST SUBMISSION: Thu Aug  5 06:58:48 PDT 2004
 * 
 * REVISION:
 * 
 * 08-05-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYNAIBARREL_HH
#define _MJGEOMETRYNAIBARREL_HH

//---------------------------------------------------------------------------//

#include "globals.hh"

//---------------------------------------------------------------------------//

class G4LogicalVolume;

class MJGeometryNaIBarrel 
{
public:

  //default constructor
  MJGeometryNaIBarrel();

  //copy constructor
  MJGeometryNaIBarrel(const MJGeometryNaIBarrel &);

  //destructor
  ~MJGeometryNaIBarrel();

  //public interface
  void Construct();

  G4LogicalVolume *GetLogical() { return fLogical; }

  //protected members
protected:


  //private  members
private:
  G4LogicalVolume          *fLogical;
  G4double                 fLength;
  G4double                 fInnerRadius;
  G4double                 fOuterRadius;
  G4double                 fGapWidth;

};
#endif
