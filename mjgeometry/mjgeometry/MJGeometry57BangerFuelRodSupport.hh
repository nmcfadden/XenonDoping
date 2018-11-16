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
 * $Id: MJGeometry57BangerFuelRodSupport.hh,v 1.1 2005-03-11 02:21:46 rhenning Exp $
 *      
 * CLASS DECLARATION:  MJGeometry57BangerFuelRodSupport.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Fuelrod support structure, as per PNNL.
 * Must be called AFTER the construct method of the temmplate crystal has 
 * been invoked. 
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
 * AUTHOR: R.
 * CONTACT: Henning
 * FIRST SUBMISSION: Mon Mar  7 15:18:41 PST 2005
 * 
 * REVISION:
 * 
 * 03-07-2005, Created, R. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRY57BANGERFUELRODSUPPORT_HH
#define _MJGEOMETRY57BANGERFUELRODSUPPORT_HH

//---------------------------------------------------------------------------//

#include "globals.hh"

#include "mjgeometry/MJVGeometrySupportStructure.hh"

//---------------------------------------------------------------------------//

class G4Material;
class G4VisAttributes;

class MJGeometry57BangerFuelRodSupport : public MJVGeometrySupportStructure
{
public:

   MJGeometry57BangerFuelRodSupport(MGGeometryDetector *mother, G4String name);
  ~MJGeometry57BangerFuelRodSupport();

  void SetTubeInnerRadius(G4double irad)  { fTubeInnerRadius = irad; }
  void SetTubeOuterRadius(G4double orad)  { fTubeOuterRadius = orad; }
  void SetTubeLength(G4double len)        { fTubeLength = len; }
  void SetTubeMaterialName(G4String name) { fTubeMaterialName = name; }

  void SetSupportRingInnerRadius(G4double irad){fSupportRingInnerRadius=irad;}
  void SetSupportRingOuterRadius(G4double ora) {fSupportRingOuterRadius = ora;}
  void SetSupportRingThickness(G4double thi)   {fSupportRingThickness = thi;}
  void SetSupportRingMaterialName(G4String name)
     { fSupportRingMaterialName = name;}

  void Construct();
  void SetDefaults();
  bool UsesMJPSQLDatabase() { return true; }  

private:

  // Support tube dimensions.
  G4double            fTubeInnerRadius;
  G4double            fTubeOuterRadius;
  G4double            fTubeLength;
  G4double            fTubeCrystalSeparation;
  G4String            fTubeMaterialName;
  G4Material          *fTubeMaterial;
  G4VisAttributes     *fTubeVisAtt;

  // Support rings that crystals rest on dimensions. 
  G4double            fSupportRingInnerRadius;
  G4double            fSupportRingOuterRadius;
  G4double            fSupportRingThickness;
  G4String            fSupportRingMaterialName;
  G4Material          *fSupportRingMaterial;
  G4VisAttributes     *fSupportRingVisAtt;
};
#endif
