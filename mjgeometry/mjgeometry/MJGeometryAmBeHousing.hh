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
 * $Id: MJGeometryAmBeHousing.hh,v 1.1 2007-02-23 10:54:54 alexis3 Exp $
 *      
 * CLASS DECLARATION:  MJGeometryAmBeHousing.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * This class creates a housing to surround the AmBe point source.  The housing
 * consists of a steel cylindrical shell with endcaps surrounded by a lead
 * cylindrical shell with endcaps.
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
 * AUTHOR: alexis3
 * CONTACT: 
 * FIRST SUBMISSION: Fri Feb 17 13:09:40 PST 2006
 * 
 * REVISION:
 * 
 * 02-23-2007, Initial submission, A. Schubert
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYAMBEHOUSING_HH
#define _MJGEOMETRYAMBEHOUSING_HH

//---------------------------------------------------------------------------//

class MJGeometryAmBeHousingMessenger;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VisAttributes;

class MJGeometryAmBeHousing 
{
public:

  //default constructor
  MJGeometryAmBeHousing();

  //destructor
  ~MJGeometryAmBeHousing();

  G4ThreeVector GetHousingPosition() { return fHousingPosition; }
  void SetHousingPosition(G4ThreeVector position) {fHousingPosition = position;}
  inline G4LogicalVolume* GetHousingLogical() { return fLeadHousing_log; } 

private:
  // Logical volumes:
  G4LogicalVolume* fSteelHousing_log;
  G4LogicalVolume* fSteelCavity_log;
  G4LogicalVolume* fLeadHousing_log;
  G4LogicalVolume* fLeadCavity_log;

  // Physical volumes:
  G4VPhysicalVolume* fSteelHousing_phys;
  G4VPhysicalVolume* fSteelCavity_phys;
  G4VPhysicalVolume* fLeadCavity_phys;

  // messenger:
  MJGeometryAmBeHousingMessenger* fMessenger;

  G4ThreeVector fHousingPosition;
};
#endif
