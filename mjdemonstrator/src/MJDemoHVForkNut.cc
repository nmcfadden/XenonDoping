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
 * CLASS DECLARATION:  MJDemoHVForkNut.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for the copper cable guide. Part origin: Center of the hex hole
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
 * AUTHOR: Kris Vorren
 * CONTACT: krisvorren@unc.edu
 * FIRST SUBMISSION:
 * 
 * REVISION:
 * 
 * 04-23-2015, Created, K. Vorren
 */
// --------------------------------------------------------------------------//
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
#include "mjdemonstrator/MJDemoHVForkNut.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoHVForkNut::MJDemoHVForkNut(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-11-020", "Teflon")
{;}

MJDemoHVForkNut::MJDemoHVForkNut(const MJDemoHVForkNut & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemoHVForkNut::~MJDemoHVForkNut()
{;}

G4LogicalVolume* MJDemoHVForkNut::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    G4RotationMatrix* rot = new G4RotationMatrix();
    //hex body
    G4double zPlanes[] = {-0.320/2*25.4*mm, 0.320/2*25.4*mm};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {0.2165*25.4*mm, 0.2165*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("hex", pi/3, 2*pi, 6, 2, zPlanes, rInner, rOuter);
    //thru hole
    G4Tubs* threadHole = new G4Tubs("thread", 0, 0.229/2*25.4*mm, 0.33*25.4*mm, 0, 2*pi); //Had to make r bigger due to r of hollow hex rod
    G4SubtractionSolid* hvNut = new G4SubtractionSolid("hole", hex, threadHole, rot, G4ThreeVector(0, 0, 0));
    //wide part
    G4Tubs* bigHole = new G4Tubs("hole", 0, 0.285/2*25.4*mm, 0.21*25.4*mm, 0, 2*pi);
    hvNut = new G4SubtractionSolid("hvNut", hvNut, bigHole, rot, G4ThreeVector(0, 0, 0.320/2*25.4*mm));
    //Below makes the logical volume
    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.7,0.7,0.7)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(hvNut, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created HV Nut Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Inner Cable Guide Logical" << endlog; 
  return pVol;
}  
