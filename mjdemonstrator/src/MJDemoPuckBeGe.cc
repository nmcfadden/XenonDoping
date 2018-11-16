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
// CLASS IMPLEMENTATION:  MJDemoPuckBeGe.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Class for a generic BeGe.  Default diameter 77mm, Default height 33mm.
 *
 *
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
 * 01-18-2012, Changed color of germanium crystal, K. Nguyen
 * 09-14-2016, Note: This part is implemented as a G4SubtractionSolid between
 *             two G4Tubs, one for the crystal and one for the ditch. The
 *             origin of the coordinate system is at the center of the crystal
 *             G4Tubs. M. Buuck
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UIcommand.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoPuckBeGe.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoPuckBeGe::MJDemoPuckBeGe(G4String partName, G4String serialNumber, 
								G4double diameter, G4double height) :
  MJVDemoDetector(partName, serialNumber, "MJ80-02-011", "Germanium-Enr")
{
  SetHeight(height*mm);
  SetDiameter(diameter*mm);
}

MJDemoPuckBeGe::MJDemoPuckBeGe(const MJDemoPuckBeGe & rhs) : 
  MJVDemoDetector(rhs)
{
  SetHeight(rhs.fCrystalHeight);
  SetDiameter(rhs.fCrystalDiameter);
}

MJDemoPuckBeGe::~MJDemoPuckBeGe()
{;}

G4LogicalVolume* MJDemoPuckBeGe::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fPartName + "_"+fDrawingNumber + "_" + G4UIcommand::ConvertToString(fCrystalDiameter) 
  					+ "x" + G4UIcommand::ConvertToString(fCrystalHeight) + "_" + fPartMaterial;  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Tubs* crystal = new G4Tubs("crystal", 0, fCrystalDiameter/2.,
                                fCrystalHeight/2., 0, 2.*pi);
    G4Tubs* ditch = new G4Tubs("ditch", 8.*mm, 11.*mm , 2.*mm, 0, 2.*pi);
    
    G4SubtractionSolid* puckBeGe = new G4SubtractionSolid("puckBeGe", crystal, ditch, 0,
                                                          G4ThreeVector(0, 0, -fCrystalHeight/2.));
    
    // G4VisAttributes* blueVisAtt = new G4VisAttributes(G4Colour(0.2,0.8,1.0)); // Turq.
    G4VisAttributes* germaniumVisAtt = new G4VisAttributes(G4Colour(0.878,0.878,0.867)); // New germanium color
    // blueVisAtt->SetForceWireframe( false );
    germaniumVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(puckBeGe, material, logicalName);
    // pVol->SetVisAttributes(blueVisAtt); 
    pVol->SetVisAttributes(germaniumVisAtt); 
    MGLog(debugging) << "Created " << fCrystalDiameter << "mm x " << fCrystalHeight 
      				<< "mm Crystal Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing" << fCrystalDiameter << "mm x " << fCrystalHeight 
      				<< "mm Crystal Logical" << endlog; 
  return pVol;
}  
