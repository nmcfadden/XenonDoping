//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//      
// CLASS IMPLEMENTATION:  MJSTCMainBodyCan.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * Geometry made from UW SolidWorks file.
 * Part Origin: At the top of the Can, (x,y) centered.
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday June 12 2013
 * 
 * REVISION:
 * 06-12-2013, Created, J. Strain
 * 11-14-2013, Updated visualization properties to make compatible with Ray Tracer, J. MacMullin
 * 10-02-2016, Changed material from SS303 to Aluminum to reflect true material, C. O'Shaughnessy
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polycone.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCMainBodyCan.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCMainBodyCan::MJSTCMainBodyCan(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "STCMainBodyCanDwg", "Aluminum") //partName, serialNumber, drawingNumber, material
{;}

MJSTCMainBodyCan::MJSTCMainBodyCan(const MJSTCMainBodyCan & rhs) : 
  MJVDemoPart(rhs)
{;}

MJSTCMainBodyCan::~MJSTCMainBodyCan()
{;}

G4LogicalVolume* MJSTCMainBodyCan::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; //STCMainBodyCanDwg_SS303
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
  
  	G4double zPlanes[] = {0*mm, -12.7*mm, -12.7*mm, -22.352*mm, -22.352*mm, 
  							-295.91*mm, -295.91*mm, -305.562*mm, -305.562*mm, -318.262*mm};
  	G4double rInner[] = {76.454*mm, 76.454*mm, 73.025*mm, 73.025*mm, 73.025*mm, 
  							73.025*mm, 73.025*mm, 73.025*mm, 76.454*mm, 76.454*mm};
	G4double rOuter[] =   {101.219*mm, 101.219*mm, 101.219*mm, 101.219*mm, 76.2*mm,
							76.2*mm, 101.219*mm, 101.219*mm, 101.219*mm, 101.219*mm};
    G4Polycone* CryostatCan = new G4Polycone("CryostatCan", 0, 2*pi, 10, zPlanes, rInner, rOuter);

    //G4VisAttributes* SSVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0)); // cyan
    G4VisAttributes* SSVisAtt = new G4VisAttributes(G4Colour(0.75, 0.75, 0.75)); // light gray
    SSVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(CryostatCan, material, logicalName);
    pVol->SetVisAttributes(SSVisAtt); 
    MGLog(debugging) << "Created STC Main Body Can Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing STC Main Body Can Logical" << endlog; 
  return pVol;
}  
