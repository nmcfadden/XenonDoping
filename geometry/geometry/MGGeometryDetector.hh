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
 * $Id: MGGeometryDetector.hh,v 1.12 2007-08-17 00:52:34 mgmarino Exp $
 *
 * CLASS DECLARATION:  MGGeometryDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 *  This is the base class for any detector that will be included as part
 *  of the standard MJ distribution. It only contains those methods and member
 *  variables that are common to every conceivable detector: the logical
 *  detector and the detector serial number.
 */
// End class description
//
/**
 * SPECIAL NOTES:
 *  The derived classes, for the sake of consistency, should all have a method
 *  entitled GetDBDetector.
 */
//
// --------------------------------------------------------------------------//
/**
 * AUTHOR: kareem
 * CONTACT:
 * FIRST SUBMISSION: Fri Jul  2 10:52:55 PDT 2004
 *
 * REVISION:
 *
 * 07-02-2004, Kareem
 *             First file creation and submission
 * 11-04-2004, Luciano
 *             Added methods to set/get the detector name and to
 *             set theDetectorLogical
 * 02-21-2005, R. Henning. Added SetSerialNumber(). Removed extraneous comments
 * 09-29-2005, M. Marino. Added ConstructImportanceStore() for variance reduction
 *             techniques.
 * 06-04-2007, K. Kroeninger. Added MGTCrystalData container and FillCrystalData() method.
 * 06-08-2007, Jing Liu. removed local variable "crystaldatavector" in virtual function
 *             void FillCrystalDataVector(MGCrystalDataVector*)
 * 11-16-2011, A. Schubert. Separated importance sampling into new class,
 *             MGGeometryImportance.  Added GimmeParallelWorld() virtual method
 *             for importance sampling with parallel geometry.
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGGEOMETRYDETECTOR_HH
#define _MGGEOMETRYDETECTOR_HH

//---------------------------------------------------------------------------//
#include <utility>
#include "globals.hh"

#include "MGCrystalDataContainer.hh"

#include "MGGeometryParallelWorld.hh"
#include "MGGeometryImportance.hh"

class G4LogicalVolume;

//---------------------------------------------------------------------------//
class MGGeometryDetector : public MGGeometryImportance
{
public:

  MGGeometryDetector( G4String );

  virtual ~MGGeometryDetector() {}

  virtual void ConstructDetector() {;}

  inline G4String GetSerialNumber() { return serialNumber; };
  inline G4LogicalVolume *GetDetectorLogical() { return theDetectorLogical; };
  inline void SetDetectorLogical(G4LogicalVolume* dlogic) {theDetectorLogical = dlogic;}
  inline void SetDetectorName(G4String name) {detectorName = name;}
  inline void SetDetectorPhysical(G4VPhysicalVolume* dphysical) {theDetectorPhysical = dphysical;}
  inline G4VPhysicalVolume *GetDetectorPhysical() {return theDetectorPhysical;}
  inline G4String GetDetectorName() {return detectorName;}
  void SetSerialNumber(G4String sernum) { serialNumber = sernum; }
  virtual G4String GetMaterialsList() { return "NULL"; } // Override in all MJ classes.
  virtual void FillCrystalDataVector(MGCrystalDataVector*)
  { return; };

  // overload if a parallel geometry is used:
  virtual MGGeometryParallelWorld* GimmeParallelWorld() { return NULL; }

  // For MCRun and other output classes that don't use G4's sensitive volumes
  virtual int GetSensitiveVolumeID(const std::string& /*volName*/) { return 0; }

protected:

  G4String serialNumber;
  G4LogicalVolume *theDetectorLogical;
  G4VPhysicalVolume *theDetectorPhysical;

private:
  G4String detectorName;
};
#endif
