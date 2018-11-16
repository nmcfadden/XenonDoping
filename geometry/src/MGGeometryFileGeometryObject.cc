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
//                                                          
// $Id: MGGeometryFileGeometryObject.cc,v 1.1 2006-12-14 12:22:17 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  @CLASS_NAME@.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR:  Luciano Pandola
 * CONTACT: @CONTACT@
 * FIRST SUBMISSION: @START_DATE@
 * 
 * REVISION:
 *
 * 12-11-2006, Luciano, first implementation
 */
//---------------------------------------------------------------------------//
//
#include "geometry/MGGeometryFileGeometryObject.hh"


MGGeometryFileGeometryObject::MGGeometryFileGeometryObject()
{;}

MGGeometryFileGeometryObject::~MGGeometryFileGeometryObject()
{;}

MGGeometryFileGeometryObject::MGGeometryFileGeometryObject(const MGGeometryFileGeometryObject &right)
{
  fShapeCode = right.fShapeCode;
  fDetectorCode = right.fDetectorCode;
  fDaughterCode = right.fDaughterCode;
  fMaterialName = right.fMaterialName;
  fCenterPosition = right.fCenterPosition;
  fDimensions = right.fDimensions;
  fRotationAngles = right.fRotationAngles;
  fVolumeName = right.fVolumeName;
}

const MGGeometryFileGeometryObject& MGGeometryFileGeometryObject::operator=(const MGGeometryFileGeometryObject &right)
{
  fShapeCode = right.fShapeCode;
  fDetectorCode = right.fDetectorCode;
  fDaughterCode = right.fDaughterCode;
  fMaterialName = right.fMaterialName;
  fCenterPosition = right.fCenterPosition;
  fDimensions = right.fDimensions;
  fRotationAngles = right.fRotationAngles;
  fVolumeName = right.fVolumeName;
  return *this;
}

G4int MGGeometryFileGeometryObject::operator==(const MGGeometryFileGeometryObject &right) const
{
  return (this==&right) ? 1 : 0;
}




