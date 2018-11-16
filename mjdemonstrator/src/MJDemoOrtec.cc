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
// CLASS IMPLEMENTATION:  MJDemoOrtec.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Class for a generic BeGe.  Default diameter ?mm, Default height ?mm.
 *
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Wenqin Xu
 * CONTACT: wxu@lanl.gov
 * FIRST SUBMISSION: Jan 22, 2013
 * 
 * REVISION:
 * 
 * 01-22-2013, Created, W. Xu
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UIcommand.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoOrtec.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoOrtec::MJDemoOrtec(G4String partName, G4String serialNumber, 
G4double diameter, G4double height, G4double topdiameter,G4double topheight,G4double bottomdiameter1, G4double bottomheight1,G4double bottomdiameter2, G4double bottomheight2,G4double bottomdiameter3, G4double bottomheight3) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-011", "Germanium-Enr"), 
  fCrystalDiameter(diameter*mm), fCrystalHeight(height*mm),fCrystalTopDiameter(topdiameter*mm),fCrystalTopHeight(topheight*mm),fCrystalBottomDiameter1(bottomdiameter1*mm),fCrystalBottomHeight1(bottomheight1*mm),fCrystalBottomDiameter2(bottomdiameter2*mm),fCrystalBottomHeight2(bottomheight2*mm),fCrystalBottomDiameter3(bottomdiameter3*mm),fCrystalBottomHeight3(bottomheight3*mm)
{;}

MJDemoOrtec::MJDemoOrtec(const MJDemoOrtec & rhs) : 
  MJVDemoPart(rhs), fCrystalDiameter(rhs.fCrystalDiameter), fCrystalHeight(rhs.fCrystalHeight),
  fCrystalTopDiameter(rhs.fCrystalTopDiameter),fCrystalTopHeight(rhs.fCrystalTopHeight),fCrystalBottomDiameter1(rhs.fCrystalBottomDiameter1),fCrystalBottomHeight1(rhs.fCrystalBottomHeight1),fCrystalBottomDiameter2(rhs.fCrystalBottomDiameter2),fCrystalBottomHeight2(rhs.fCrystalBottomHeight2),fCrystalBottomDiameter3(rhs.fCrystalBottomDiameter3),fCrystalBottomHeight3(rhs.fCrystalBottomHeight3)
{;}

MJDemoOrtec::~MJDemoOrtec()
{;}

G4LogicalVolume* MJDemoOrtec::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + G4UIcommand::ConvertToString(fCrystalDiameter) 
  					+ "x" + G4UIcommand::ConvertToString(fCrystalHeight) + "_" + fPartMaterial;  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* crystal = new G4Tubs("crystal", 0, fCrystalDiameter/2.,
                                fCrystalHeight/2., 0, 2.*pi);
    G4Cons* topcone = new G4Cons("topcone",0,fCrystalDiameter/2.,0,fCrystalTopDiameter/2,fCrystalTopHeight/2.,0, 2*pi );
    G4Cons* bottomcone1 = new G4Cons("bottomcone1",0,fCrystalBottomDiameter1/2.,0,fCrystalDiameter/2.,fCrystalBottomHeight1/2.,0, 2*pi );
    G4Cons* bottomcone2 = new G4Cons("bottomcone2",0,fCrystalBottomDiameter2/2.,0,fCrystalBottomDiameter1/2.,fCrystalBottomHeight2/2.,0, 2*pi );
    G4Cons* bottomcone3 = new G4Cons("bottomcone3",0,fCrystalBottomDiameter3/2.,0,fCrystalBottomDiameter2/2.,fCrystalBottomHeight3/2.,0, 2*pi );
    G4Tubs* ditch = new G4Tubs("ditch", 0.*mm, 1.592*mm , 2.*mm, 0, 2.*pi);

    G4UnionSolid* crystaltop =  new G4UnionSolid("crystaltop",crystal,topcone,0,G4ThreeVector(0, 0, fCrystalTopHeight/2.+fCrystalHeight/2.)); 
    G4UnionSolid* crystalbottom1 =  new G4UnionSolid("crystalbottom1",crystaltop,bottomcone1,0,G4ThreeVector(0, 0, -fCrystalBottomHeight1/2.-fCrystalHeight/2.)); 
    G4UnionSolid* crystalbottom2 =  new G4UnionSolid("crystalbottom2",crystalbottom1,bottomcone2,0,G4ThreeVector(0, 0, -fCrystalBottomHeight2/2.-fCrystalBottomHeight1-fCrystalHeight/2.)); 
    G4UnionSolid* crystalbottom3 =  new G4UnionSolid("crystalbottom3",crystalbottom2,bottomcone3,0,G4ThreeVector(0, 0, -fCrystalBottomHeight3/2.-fCrystalBottomHeight1-fCrystalBottomHeight2-fCrystalHeight/2.)); 
 
    G4SubtractionSolid* Ortec = new G4SubtractionSolid("Ortec", crystalbottom3, ditch, 0,
                                                          G4ThreeVector(0, 0, -fCrystalBottomHeight1-fCrystalBottomHeight2-fCrystalBottomHeight3-fCrystalHeight/2.));
    
    // G4VisAttributes* blueVisAtt = new G4VisAttributes(G4Colour(0.2,0.8,1.0)); // Turq.
    G4VisAttributes* germaniumVisAtt = new G4VisAttributes(G4Colour(0.878,0.878,0.867)); // New germanium color
    // blueVisAtt->SetForceWireframe( false );
    germaniumVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(Ortec, material, logicalName);
//    pVol = new G4LogicalVolume(crystalbottom3, material, logicalName);
    // pVol->SetVisAttributes(blueVisAtt); 
    pVol->SetVisAttributes(germaniumVisAtt); 
    MGLog(debugging) << "Created " << fCrystalDiameter << "mm x " << fCrystalHeight 
      				<< "mm Crystal Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing" << fCrystalDiameter << "mm x " << fCrystalHeight 
      				<< "mm Crystal Logical" << endlog; 
  return pVol;
}  
