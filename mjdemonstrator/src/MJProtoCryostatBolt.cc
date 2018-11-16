#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polyhedra.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJProtoCryostatBolt.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJProtoCryostatBolt::MJProtoCryostatBolt(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "CryostatBoltDwg", "SS303")
{;}

MJProtoCryostatBolt::MJProtoCryostatBolt(const MJProtoCryostatBolt & rhs) : 
  MJVDemoPart(rhs)
{;}

MJProtoCryostatBolt::~MJProtoCryostatBolt()
{;}

G4LogicalVolume* MJProtoCryostatBolt::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zPlanes[] = {-0.12*25.4*mm, -0.001*25.4*mm, -0.001*25.4*mm, 0.77*25.4*mm};//0.0
    G4double rInner[] = {0, 0, 0, 0};
    G4double rOuter[] = {0.156*25.4*mm, 0.156*25.4*mm, 0.125*25.4*mm, 0.125*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("hex", pi/2, 2*pi, 6, 4, zPlanes, rInner, rOuter); 
    
    G4Tubs* threads = new G4Tubs("threads", 0, 0.1131*25.4*mm, 0.75*25.4*mm, 0, 2*pi);//.1141
    G4UnionSolid* bolt = new G4UnionSolid("bolt", hex, threads, 0, G4ThreeVector(0,0,0.77*25.4*mm));
    
    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(bolt, material, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created Cryostat Bolt Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Cryostat Bolt Logical" << endlog; 
  return pVol;
}  
