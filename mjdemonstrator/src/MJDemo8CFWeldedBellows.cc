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
// CLASS IMPLEMENTATION:  MJDemo8CFWeldedBellows.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Center of 8CF flange face
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: Sept 28, 2015
 *
 * REVISION:
 *
 * 09-28-2015, Created, M. Green
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJDemo8CFWeldedBellows.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemo8CFWeldedBellows::MJDemo8CFWeldedBellows(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "8CFWeldedBellowsDwg", "SS303")
{;}

MJDemo8CFWeldedBellows::MJDemo8CFWeldedBellows(const MJDemo8CFWeldedBellows & rhs) : 
  MJVDemoPart(rhs)
{;}

MJDemo8CFWeldedBellows::~MJDemo8CFWeldedBellows()
{;}

G4LogicalVolume* MJDemo8CFWeldedBellows::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zConePlanes[] = {0, 0.88*25.4*mm, 0.88*25.4*mm, 3.94*25.4*mm, 3.94*25.4*mm, 4.72*25.4*mm};
    G4double rConeInner[] = {1.875*25.4*mm, 1.875*25.4*mm, 1.875*25.4*mm, 1.875*25.4*mm, 1.875*25.4*mm, 1.875*25.4*mm};
    G4double rConeOuter[] = {4.0*25.4*mm, 4.0*25.4*mm, 2.375*25.4*mm, 2.375*25.4*mm, 3.0*25.4*mm, 3.0*25.4*mm};
    G4Polycone* bellows = new G4Polycone("bellows", 0, 2*pi, 6, zConePlanes, rConeInner, rConeOuter);
      
    G4VisAttributes* blueVisAtt = new G4VisAttributes(G4Colour(0, 0.8, 0.8)); // blue
    blueVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(bellows, material, logicalName);
    pVol->SetVisAttributes(blueVisAtt); 
    MGLog(debugging) << "Created Welded Bellows Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Welded Bellows Logical" << endlog;
  return pVol;
}  
