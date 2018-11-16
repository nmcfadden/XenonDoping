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
 * $Id: GEHdBEGeDetector.hh,v 1.1 2009-05-14 21:19:06 pandola Exp $
 *      
 * CLASS DECLARATION:  GEHdBEGeDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 * Geometry class for the Dario detector in Heidelberg 
 *
 * REVISION:
 *
 * 14-05-2009, Added in MaGe, Luciano
 *
 */
// --------------------------------------------------------------------------//

#ifndef _GEHDBEGEDETECTORDETECTOR_HH
#define _GEHDBEGEDETECTORDETECTOR_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class G4Material;
class GEHdBEGeDetector : public MGGeometryDetector
{

private:
  

public:

  //default constructor
  GEHdBEGeDetector();

  //destructor
  ~GEHdBEGeDetector();

  //public interface
  void ConstructDetector();


};
#endif
