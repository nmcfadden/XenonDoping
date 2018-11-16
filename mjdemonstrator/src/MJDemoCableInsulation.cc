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
// CLASS IMPLEMENTATION:  MJDemoCableInsulation.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Class for the insulation of a parylene cable
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Oct 27, 2010
 * 
 * REVISION:
 * 
 * 10-27-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UIcommand.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemoCableInsulation.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCableInsulation::MJDemoCableInsulation(G4String partName, G4String serialNumber, 
								G4double length) :
  MJVDemoPart(partName, serialNumber, "insulation", "Parylene-N"), 
  fWireLength(length)
{;}

MJDemoCableInsulation::MJDemoCableInsulation(const MJDemoCableInsulation & rhs) : 
  MJVDemoPart(rhs), fWireLength(rhs.fWireLength)
{;}

MJDemoCableInsulation::~MJDemoCableInsulation()
{;}

G4LogicalVolume* MJDemoCableInsulation::ConstructPart()
{

  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "x" + G4UIcommand::ConvertToString(fWireLength) 
  										+ "_" + fPartMaterial;  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4Box* body = new G4Box("body", 0.025*25.4*mm, 0.006*25.4*mm, fWireLength/2);

    G4Tubs* wire = new G4Tubs("wire", 0, 0.0016*25.4*mm, fWireLength/2+1.0*mm, 0, 2*pi);

    G4SubtractionSolid* body2 = new G4SubtractionSolid("body2", body, wire, 0, 
    												G4ThreeVector(-0.009*25.4*mm, 0, 0));
    G4SubtractionSolid* body3 = new G4SubtractionSolid("body3", body2, wire, 0, 
    												G4ThreeVector(-0.003*25.4*mm, 0, 0));
    G4SubtractionSolid* body4 = new G4SubtractionSolid("body4", body3, wire, 0, 
    												G4ThreeVector(0.003*25.4*mm, 0, 0));    												
    G4SubtractionSolid* insulation = new G4SubtractionSolid("insulation", body4, wire, 0, 
    												G4ThreeVector(0.009*25.4*mm, 0, 0));

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(insulation, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created " << fWireLength << "mm Insulation Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing " << fWireLength << "mm Insulation Logical" << endlog;
  return pVol;
}  
