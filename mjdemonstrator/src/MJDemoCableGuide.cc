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
// $Id: MGcodeTemplate.cc,v 1.1 2004-12-09 08:58:35 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MJDemoCableGuide.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Part Origin:  Center of hex hole
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 * 
 * 07-22-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4ExtrudedSolid.hh"
#include "G4Polyhedra.hh"
#include "G4DisplacedSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCableGuide.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCableGuide::MJDemoCableGuide(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-002", "Teflon")
{;}

MJDemoCableGuide::MJDemoCableGuide(const MJDemoCableGuide & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoCableGuide::~MJDemoCableGuide()
{;}

G4LogicalVolume* MJDemoCableGuide::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* body = new G4Box("body", 0.1875*25.4*mm, 0.3875*25.4*mm, 0.03*25.4*mm);
    
    G4double zPlanes[] = {-0.03*25.4*mm, 0.03*25.4*mm};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {0.135*25.4*mm, 0.135*25.4*mm};
    G4Polyhedra* hexCut = new G4Polyhedra("hexCut", pi/2, 2*pi, 6, 2, zPlanes, rInner, rOuter);
    G4SubtractionSolid* bodyHexCut = new G4SubtractionSolid("bodyHexCut", body, hexCut, 0,
                                                             G4ThreeVector(0, -0.2*25.4*mm, 0));   
    
    G4Tubs* cornerFillet = new G4Tubs("cornerFillet", 0.125*25.4*mm, 0.3*25.4*mm,
                                       0.035*25.4*mm,-pi/2, pi/2); 
    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", bodyHexCut, cornerFillet, 0, 
                                                G4ThreeVector(0.0625*25.4*mm, -0.2625*25.4*mm, 0));

    G4Tubs* cornerFillet2 = new G4Tubs("cornerFillet2", 0.125*25.4*mm, 0.3*25.4*mm,
                                       0.035*25.4*mm, pi, pi/2); 
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, cornerFillet2, 0, 
                                                G4ThreeVector(-0.0625*25.4*mm, -0.2625*25.4*mm, 0));

    G4Tubs* cornerFillet3 = new G4Tubs("cornerFillet3", 0, 0.171*25.4*mm, 0.035*25.4*mm, 0, 2*pi); 
    G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, cornerFillet3, 0, 
                                                G4ThreeVector(0.2535*25.4*mm, 0.1025*25.4*mm, 0));

    G4Box* millCut = new G4Box("millCut", 0.1*25.4*mm, 0.1425*25.4*mm, 0.035*25.4*mm);
    G4SubtractionSolid* body5 = new G4SubtractionSolid("body5", body4, millCut, 0, 
                                                G4ThreeVector(0.1825*25.4*mm, 0.245*25.4*mm, 0));

    G4Box* cornerCut = new G4Box("cornerCut", 0.1*25.4*mm, 0.048*25.4*mm, 0.035*25.4*mm);
    G4RotationMatrix* cornerRotation = new G4RotationMatrix;
    cornerRotation->rotateZ(120*deg);
    G4SubtractionSolid* body6 = new G4SubtractionSolid("body6", body5, cornerCut, cornerRotation, 
                                                G4ThreeVector(-0.1875*25.4*mm, 0.3875*25.4*mm, 0));    

    cornerRotation->rotateZ(90*deg);
    G4SubtractionSolid* body7 = new G4SubtractionSolid("body7", body6, cornerCut, cornerRotation, 
                                                G4ThreeVector(0.0825*25.4*mm, 0.3875*25.4*mm, 0));
    

    vector<G4TwoVector> notchPolygon;
    notchPolygon.push_back(G4TwoVector(0.065*25.4*mm, 0.670*25.4*mm));
    notchPolygon.push_back(G4TwoVector(0.105*25.4*mm, 0.740*25.4*mm));
    notchPolygon.push_back(G4TwoVector(0.166*25.4*mm, 0.705*25.4*mm));
    notchPolygon.push_back(G4TwoVector(0.195*25.4*mm, 0.757*25.4*mm));
    notchPolygon.push_back(G4TwoVector(0.213*25.4*mm, 0.747*25.4*mm));
    notchPolygon.push_back(G4TwoVector(0.183*25.4*mm, 0.695*25.4*mm));
    notchPolygon.push_back(G4TwoVector(0.235*25.4*mm, 0.665*25.4*mm));
    notchPolygon.push_back(G4TwoVector(0.195*25.4*mm, 0.596*25.4*mm));
    G4ExtrudedSolid* cableNotch = new G4ExtrudedSolid("cableNotch", notchPolygon, 0.035*25.4*mm, 
                                                  G4TwoVector(-0.18*25.4*mm, -0.38*25.4*mm), 1, 
                                                  G4TwoVector(-0.18*25.4*mm, -0.38*25.4*mm), 1);

    G4SubtractionSolid* part = new G4SubtractionSolid("part", body7, cableNotch);
 
    G4DisplacedSolid* cableGuide = new G4DisplacedSolid("CableGuide", part, 0, 
    														G4ThreeVector(0, 0.2*25.4*mm, 0));
 
    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.7,0.7,0.7)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(cableGuide, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Cable Guide Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cable Guide Logical" << endlog; 
  return pVol;
}  
