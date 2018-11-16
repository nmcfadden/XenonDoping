#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJProtoHVNut.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJProtoHVNut::MJProtoHVNut(G4String partName, G4String serialNumber) :
  MJVDemoPart(partName, serialNumber, "MJ80-02-001", "SiBronze")
{;}

MJProtoHVNut::MJProtoHVNut(const MJProtoHVNut & rhs) : 
  MJVDemoPart(rhs)
{;}

MJProtoHVNut::~MJProtoHVNut()
{;}

G4LogicalVolume* MJProtoHVNut::ConstructPart()
{

  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4double zPlanes[] = {-0.05*25.4*mm, -0.315*25.4*mm};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {0.156*25.4*mm, 0.156*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("Hex", 0, 2*pi, 6, 2, zPlanes, rInner, rOuter);
    
    G4double zConePlanes[] = {-0.1*25.4*mm, 0*25.4*mm, 0*25.4*mm, 0.257*25.4*mm};
    G4double rConeInner[] = {0, 0, 0, 0};
    G4double rConeOuter[] = {0.18*25.4*mm, 0.18*25.4*mm, 0.1375*25.4*mm, 0.1375*25.4*mm};
    G4Polycone* barrel = new G4Polycone("Barrel", 0, 2*pi, 4, zConePlanes, rConeInner, rConeOuter);

    G4UnionSolid* hexBarrel = new G4UnionSolid("hexBarrel", hex, barrel, 0, G4ThreeVector(0,0,0));

    G4Tubs* threadHole = new G4Tubs("threadHole", 0, 0.1141*25.4*mm, 0.35*25.4*mm, 0, 2*pi);       

    G4SubtractionSolid* HVNut = new G4SubtractionSolid("HV Nut", hexBarrel, threadHole);   
    
    G4VisAttributes* redVisAtt = new G4VisAttributes(G4Colour(0.8, 0, 0)); // red                                                               
    redVisAtt->SetForceWireframe( false );
    
    //G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    G4String symbol;  
    G4double a, z, density, fractionmass;     
    G4int ncomponents;
    G4Element* Cu = new G4Element("Copper", symbol="Cu", z=29., a=63.546*g/mole);
    G4Element* Si = new G4Element("Silicon", symbol="Si", z=14., a=28.0855*g/mole);
    G4Material* material = new G4Material("SiBronze", density=8.53*g/cm3, ncomponents=2);
    material->AddElement(Cu,fractionmass= 0.97);
    material->AddElement(Si,fractionmass= 0.03);
    
    pVol = new G4LogicalVolume(HVNut, material, logicalName);
    pVol->SetVisAttributes(redVisAtt); 
    MGLog(debugging) << "Created HV Nut Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing HV Nut Logical" << endlog; 
  return pVol;
}  
