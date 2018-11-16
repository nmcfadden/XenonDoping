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
 * $Id: MJheaderTemplate.hh,v 1.3 2005-02-15 23:26:55 akbarm Exp $
 *      
 * CLASS DECLARATION:  MGMJLocalMaterialTable.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 

 * This class build materials for the MJ simulations of MaGe. The materais
 * constrcuted here are based on the old PSQL database and the code in this 
 * class was generated directly from it. This class should be used when adding new 
 * materials for MJ-related simulations. 
 * 
 * R. Henning 7/6/13
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Sat Jul  6 18:18:00 EDT 2013
 * 
 * REVISION:
 * 
 * 07-06-2013, Created T. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MGMJLOCALMATERIALTABLE_HH
#define _MGMJLOCALMATERIALTABLE_HH

//---------------------------------------------------------------------------//


//---------------------------------------------------------------------------//

class MGMJLocalMaterialTable 
{
public:

  MGMJLocalMaterialTable();
  ~MGMJLocalMaterialTable();

  //public interface
  void BuildIsotopes();
  void BuildElements();
  void BuildMaterials();
  void BuildAll();

private:
  bool fIsotopesBuilt;
  bool fElementsBuilt;
  bool fMaterialsBuilt;

};
#endif
