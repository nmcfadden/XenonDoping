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
// CLASS IMPLEMENTATION:  MJProto832Screw.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Same as MJDemo832Screw but made of SS instead of EFCu
 */
// 
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UIcommand.hh"
#include "G4Polyhedra.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJProto832Screw.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJProto832Screw::MJProto832Screw(G4String partName, G4String serialNumber, 
								G4double length) :
  MJVDemoPart(partName, serialNumber, "8-32", "SS303"), 
  fScrewLength(length*mm)
{;}

MJProto832Screw::MJProto832Screw(const MJProto832Screw & rhs) : 
  MJVDemoPart(rhs), fScrewLength(rhs.fScrewLength)
{;}

MJProto832Screw::~MJProto832Screw()
{;}

G4LogicalVolume* MJProto832Screw::ConstructPart()
{

  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "x" + G4UIcommand::ConvertToString(fScrewLength) 
  										+ "_" + fPartMaterial;  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4double zPlanes[] = {0*25.4*mm, 0.110*25.4*mm};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {0.1095*25.4*mm, 0.1095*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("hex", pi/2, 2*pi, 6, 2, zPlanes, rInner, rOuter);    

    G4Tubs* thread = new G4Tubs("thread", 0, 0.0714*25.4*mm, fScrewLength/2 + 0.1*mm, 0, 2*pi);//0.0724

    G4UnionSolid* bolt = new G4UnionSolid("bolt", hex, thread, 0, 
    										G4ThreeVector(0, 0, -fScrewLength/2 + 0.1*mm)); 

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(bolt, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created 8-32x " << fScrewLength << "mm Screw Logical" << endlog;
    MGLog(routine)<<"Volume of 8-32x "<<fScrewLength<<"mm = "<<bolt->GetCubicVolume()/(cm*cm*cm)<<"cm3"<<endl;
  }
  else  MGLog(debugging) << "Using pre-existing 8-32x " << fScrewLength << "mm Screw Logical" << endlog;
  return pVol;
}  
