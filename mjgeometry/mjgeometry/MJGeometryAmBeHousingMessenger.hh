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
 * $Id: MJGeometryAmBeHousingMessenger.hh,v 1.1 2007-02-23 10:56:37 alexis3 Exp $
 *      
 * CLASS DECLARATION:  MJGeometryAmBeHousingMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Messenger to turn on housing for AmBe point source.
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
 * AUTHOR: alexis3
 * CONTACT: 
 * FIRST SUBMISSION: Fri Feb 17 13:09:53 PST 2006
 * 
 * REVISION:
 * 
 * 02-23-2007, Initial submission, A. Schubert
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYAMBEHOUSINGMESSENGER_HH
#define _MJGEOMETRYAMBEHOUSINGMESSENGER_HH

//---------------------------------------------------------------------------//

#include "globals.hh"                 
#include "G4UImessenger.hh"		       

class MJGeometryAmBeHousing; 
class G4UIdirectory; 
class G4UIcmdWith3VectorAndUnit;  

//---------------------------------------------------------------------------//

class MJGeometryAmBeHousingMessenger : public G4UImessenger  
{
public:

  //default constructor
  MJGeometryAmBeHousingMessenger(MJGeometryAmBeHousing* amBeHousing);
  ~MJGeometryAmBeHousingMessenger();

  //public interface
  void SetNewValue(G4UIcommand *command, G4String newValues);

  //private  members
private:
  G4UIdirectory* fAmBeHousingDirectory;
    // /MG/geometry/amBeHousing/
  MJGeometryAmBeHousing* fAmBeHousing;
  G4UIcmdWith3VectorAndUnit* fPositionCmd;
    // /MG/geometry/amBeHousing/position

};
#endif
