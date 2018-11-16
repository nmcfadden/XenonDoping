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
 * $Id: MJVGeometrySupportStructure.hh,v 1.1 2005-03-11 02:21:46 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJVGeometrySupportStructure.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Virtual base class for support structures in simulations. Serves as base 
 * class for different support structure configurations in detectors that can
 * have different support structure configurations. 
 *
 * Note that this is not a full detector geometry. It only places support
 * structures in previously existing detectors. The inherited class must be 
 * instantiated and deleted in a pre-existing detector.
 *
 * It is the programmer's responsibility to ensure that the support structures
 * are consistent with the detector geometry.
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
 * FIRST SUBMISSION: Wed Mar  2 13:08:07 PST 2005
 * 
 * REVISION:
 * 
 * 03-03-2005, Created, R. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MJVGEOMETRYSUPPORTSTRUCTURE_HH
#define _MJVGEOMETRYSUPPORTSTRUCTURE_HH

//---------------------------------------------------------------------------//

class G4String;
class MGGeometryDetector;

class MJVGeometrySupportStructure 
{
public:

  MJVGeometrySupportStructure(MGGeometryDetector *mother, G4String name);

  virtual ~MJVGeometrySupportStructure();

  G4String              GetName() { return fName; }
  MGGeometryDetector    *GetMotherDetector() { return fMotherDetector; }
  virtual void          Construct() = 0;
  virtual   bool UsesMJPSQLDatabase() { return true; }  

protected:
  void                  SetName(G4String name) { fName = name; }

private:
  G4String              fName;
  MGGeometryDetector    *fMotherDetector;

};
#endif
