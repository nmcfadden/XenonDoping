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
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION:  MJGeometryOrtecCrystal.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for an enriched ORTEC PPC detector for the Majorana Demonstrator.
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
 * AUTHOR: Ryan Martin
 * CONTACT: rdmartin@lbl.gov
 * FIRST SUBMISSION: Feb 26, 2013
 * 
 * REVISION:
 * 
 * 02-26-2013, Created, R. Martin
 * 
 * 06-14-2013, added GetCrystalDiameter, W. Xu
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYORTECCRYSTAL_HH
#define _MJGEOMETRYORTECCRYSTAL_HH

//---------------------------------------------------------------------------//



using namespace std;
//---------------------------------------------------------------------------//

class MJGeometryOrtecCrystal
{
  
public:
  MJGeometryOrtecCrystal(G4String argName="OrtecCrystal");
  ~MJGeometryOrtecCrystal();
  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,G4LogicalVolume *motherlogical);
  void LoadCrystalGeomFromFile(G4String argFileName);
  void SaveCrystalGeomToFile(G4String argFileName);
  
  G4double GetCrystalLength(){return fCrystalLength;}
  G4double GetCrystalDiameter(){return fCrystalDiameter;} //added by wenqin
  G4double GetCrystalDimpleHoleDepth() {return fCrystalDimpleHoleDepth;}
  G4double GetUsableCrystalDimpleHoleDepth() {return fCrystalDimpleHoleDepth-fCrystalDimpleNominalRadius+sqrt(fCrystalDimpleNominalRadius*fCrystalDimpleNominalRadius-fCrystalDimpleHoleDiameter*fCrystalDimpleHoleDiameter/4);}//this removes the depth that comes from having a rounded end in the bottom of the dimple
  
  void SetCrystalLength(G4double argD){fCrystalLength=argD;}
  void SetCrystalDiameter(G4double argD) {fCrystalDiameter=argD;}
  void SetCrystalDimpleRadialOffset(G4double argD){ fDimpleRadialOffset=argD;}
  void SetCrystalDimpleNominalRadius(G4double argD){ fCrystalDimpleNominalRadius=argD;}
  void SetCrystalDimpleHoleDepth(G4double argD){ fCrystalDimpleHoleDepth=argD;}
  void SetCrystalDimpleHoleDiameter(G4double argD){ fCrystalDimpleHoleDiameter=argD;}
  
private:
  G4String fName;
  G4bool fLoadGeometryFromFile;
  //Parameters to describe the shape (mostly from the ORTEC spec sheet)
  G4double fCrystalDimpleNominalRadius;//Radius at the end of the bore hole for the dimple (Quantity E on the Ortec spec sheet) (kind
  G4double fCrystalDimpleHoleDepth;//Quantity D on ORTEC spec sheet, the diameter you can actually see
  G4double fCrystalDimpleHoleDiameter;//Quantity C on the ORTEC spec sheet, the depth of the dimple "bore hole"
  G4double fDimpleRadialOffset;//offset of the dimple from the center of the crystal NOT ON SPEC SHEET
  G4double fCrystalDiameter; //Outer diameter of the crystal (at the middle) (Quantity A on the Ortec spec sheet)
  G4double fCrystalLength; //Length of the crystal (from one face to the other) (Quantity B on the Ortec spec sheet)
  
  
  vector<double> fzPlanes;//use the parameters above to define the detector as a polygone
  vector<double> frOuterPlanes;
  
  
};
//
#endif
