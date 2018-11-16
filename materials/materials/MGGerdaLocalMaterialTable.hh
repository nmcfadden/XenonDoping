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
// * DESCRIPTION: 
// *
// */ 
// Begin description of class here
/**
 *
 * In this class one should instantiate the materials, if one does not want 
 * to read them from the database. It must contain "Air", since the world 
 * volume is made of air.
 *
 *
 */
// End class description
//
// 11-02-2004 First Implementation, Luciano
// 10-12-2006 added method to define materials on-the-fly from external files, Luciano
//




#ifndef MGGERDALOCALMATERIALTABLE_H
#define MGGERDALOCALMATERIALTABLE_H 1

//---------------------------------------------------------------------------//

#include <vector>

#include "globals.hh"
#include "G4Material.hh"


//---------------------------------------------------------------------------//
class MGGerdaLocalMaterialTable 
{
public:

  //default constructor
  MGGerdaLocalMaterialTable();

  //destructor
  ~MGGerdaLocalMaterialTable();

  //public interface

  void CreateMaterials();
  void AddNewMaterial(G4String);


 private:

};
#endif
