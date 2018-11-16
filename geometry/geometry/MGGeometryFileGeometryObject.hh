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
 * $Id: MGGeometryFileGeometryObject.hh,v 1.1 2006-12-14 12:20:52 pandola Exp $
 *      
 * CLASS DECLARATION:  @CLASS_NAME@.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *  This class contains the information for a geometry object (= physical volume) 
 *  defined from an external file
 *  The volumes are read and created by MGGeometryFromFile
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
 * AUTHOR:  Luciano Pandola
 * CONTACT: @CONTACT@
 * FIRST SUBMISSION: @START_DATE@
 * 
 * REVISION:
 * 
 * 12-11-2006, First implementation, Luciano 
 */
// --------------------------------------------------------------------------//

#ifndef _MGGEOMETRYFILEGEOMETRYOBJECT_HH
#define _MGGEOMETRYFILEGEOMETRYOBJECT_HH

//---------------------------------------------------------------------------//

#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

//---------------------------------------------------------------------------//

class MGGeometryFileGeometryObject 
{
 public:
  
  MGGeometryFileGeometryObject();
  ~MGGeometryFileGeometryObject();
  MGGeometryFileGeometryObject(const MGGeometryFileGeometryObject &right);
  const MGGeometryFileGeometryObject& operator=(const MGGeometryFileGeometryObject &right);
  G4int operator==(const MGGeometryFileGeometryObject &right) const;
  
 private:
  G4int fShapeCode;
  G4int fDetectorCode;
  G4int fDaughterCode;
  G4String fMaterialName;
  G4ThreeVector fCenterPosition;
  G4ThreeVector fDimensions;
  G4ThreeVector fRotationAngles;
  G4String fVolumeName;

 public:
  G4int GetShapeCode(){return fShapeCode;};
  G4int GetDetectorCode(){return fDetectorCode;};
  G4int GetDaughterCode(){return fDaughterCode;};
  G4String GetMaterialName(){return fMaterialName;};
  G4ThreeVector GetCenterPosition(){return fCenterPosition;};
  G4ThreeVector GetDimensions(){return fDimensions;};
  G4ThreeVector GetRotationAngles() {return fRotationAngles;};
  G4String GetVolumeName(){return fVolumeName;};

  void SetShapeCode(G4int sh){fShapeCode= sh;};
  void SetDetectorCode(G4int sh){fDetectorCode = sh;};
  void SetDaughterCode(G4int sh){fDaughterCode = sh;};
  void SetMaterialName(G4String matn){fMaterialName = matn;};
  void SetCenterPosition(G4ThreeVector pos){fCenterPosition = pos;};
  void SetDimensions(G4ThreeVector dim){fDimensions = dim;};
  void SetRotationAngles(G4ThreeVector rotan){fRotationAngles = rotan;};     
  void SetVolumeName(G4String voln){fVolumeName = voln;};
};


#endif

