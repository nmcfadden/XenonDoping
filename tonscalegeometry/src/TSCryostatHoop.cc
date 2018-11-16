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
// CLASS IMPLEMENTATION:  MJDemoCryostatHoop.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Part origin:  Coincident with upper rim, coaxial with barrel
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Sept 10, 2010
 * 
 * REVISION:
 * 
 * 09-10-2010, Created, M. Green
 * 03-02-2011, Modified by R. Martin to remove hole in the hoop for cross arm
 * 07-30-2012, Changed color of various parts from red to copper color, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "tonscalegeometry/TSCryostatHoop.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

TSCryostatHoop::TSCryostatHoop(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "CryostatHoopDwg", "Copper-EF")
{;}

// -----------------------------------------------------------------------

TSCryostatHoop::TSCryostatHoop(const TSCryostatHoop & rhs) : 
  MJVDemoPart(rhs)
{;}

// -----------------------------------------------------------------------

TSCryostatHoop::~TSCryostatHoop()
{;}

// -----------------------------------------------------------------------

G4LogicalVolume* TSCryostatHoop::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zConePlanes[] = {0*25.4*mm, -0.1*25.4*mm, -0.1*25.4*mm,
    			      -0.18*25.4*mm, -0.18*25.4*mm, -0.679*25.4*mm,//-0.68
    			      -0.679*25.4*mm, -0.871*25.4*mm, -0.871*25.4*mm,//-0.87
    			      -5.299*25.4*mm, -5.299*25.4*mm, -5.491*25.4*mm,//-5.3
    			      -5.491*25.4*mm, -5.81*25.4*mm, -5.81*25.4*mm,//-5.49
    			      -5.92*25.4*mm, -5.92*25.4*mm, -5.99*25.4*mm};
    G4double rConeInner[] = {6.5*25.4*mm, 6.5*25.4*mm, 6.5*25.4*mm,
    			     6.5*25.4*mm, 6.5*25.4*mm, 6.5*25.4*mm,
    			     6.5*25.4*mm, 6.5*25.4*mm, 6.5*25.4*mm,
    			     6.5*25.4*mm, 6.5*25.4*mm, 6.5*25.4*mm,
    			     6.5*25.4*mm, 6.5*25.4*mm, 6.555*25.4*mm,
    			     6.555*25.4*mm, 6.6459*25.4*mm, 6.6582*25.4*mm};
    G4double rConeOuter[] = {6.55*25.4*mm, 6.55*25.4*mm, 6.6459*25.4*mm,
                             6.66*25.4*mm, 6.874*25.4*mm, 6.874*25.4*mm, 
                             6.809*25.4*mm, 6.809*25.4*mm, 6.875*25.4*mm,//6.81 
                             6.875*25.4*mm, 6.809*25.4*mm, 6.809*25.4*mm,//6.81, 6.875
                             6.874*25.4*mm, 6.874*25.4*mm, 6.874*25.4*mm, 
                             6.874*25.4*mm, 6.874*25.4*mm, 6.874*25.4*mm};
    G4Polycone* barrel = new G4Polycone("Barrel", 0, 2*pi, 18, zConePlanes, rConeInner, rConeOuter);
        
    //G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8, 0, 0)); // red
    //redVisAtt->SetForceWireframe( false );
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)); // FIXME: Changed color attribute from 
                                                                                          //        red to copper, K. Nguyen 
    copperVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(barrel, material, logicalName);
    //pVol->SetVisAttributes(redVisAtt); 
    pVol->SetVisAttributes(copperVisAtt); // FIXME: Changed color attribute from red to copper, K. Nguyen
    MGLog(debugging) << "Created Cryostat Hoop Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cryostat Hoop Logical" << endlog; 
  return pVol;
}  

// -----------------------------------------------------------------------

