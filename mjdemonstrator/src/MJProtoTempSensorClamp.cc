#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Polyhedra.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJProtoTempSensorClamp.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJProtoTempSensorClamp::MJProtoTempSensorClamp(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJProtoTempSensorClampDwg", "Teflon") 
{;}

MJProtoTempSensorClamp::MJProtoTempSensorClamp(const MJProtoTempSensorClamp & rhs) : 
  MJVDemoPart(rhs)
{;}

MJProtoTempSensorClamp::~MJProtoTempSensorClamp()
{;}

G4LogicalVolume* MJProtoTempSensorClamp::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){  
    G4double eps = 0.001 * mm; 
    G4double innerRadius = 0.165*25.4*mm; 
    G4double outerRadius = 0.265*25.4*mm;
    G4double radius = 0.0958*25.4*0.5*mm; //4-40 effective dia
    
    G4double zPlanes[] = {0*mm, 0.180*25.4*mm};
    G4double rInner[] = { innerRadius, innerRadius};
    G4double rOuter[] = { outerRadius, outerRadius};
    G4Polyhedra* body = new G4Polyhedra("body", 0, 2*pi, 6, 2, zPlanes, rInner, rOuter);    
    
    G4Box* cut = new G4Box("cut",30.*mm,30.*mm,30.*mm);
    G4double outerHexSide = 2.*outerRadius/sqrt(3.);
    G4SubtractionSolid* halfClamp = new G4SubtractionSolid("halfClamp", body, cut, 0, G4ThreeVector(-30.*mm+outerHexSide*0.5-0.199*25.4*mm,0,0));
    
    G4Tubs* screwHole = new G4Tubs("screwHole", 0.*mm, radius+eps, 4.*mm, 0, 2*pi);
    G4RotationMatrix *rotationHole = new G4RotationMatrix();
    rotationHole->rotateZ(-90*deg);
    rotationHole->rotateY(-90*deg);
    G4SubtractionSolid* clamp = new G4SubtractionSolid("clamp", halfClamp, screwHole, 
    							rotationHole, G4ThreeVector(0, 4.*mm+outerRadius-0.100*25.4*mm, 0.5*0.180*25.4*mm)); 

    G4VisAttributes* magentaVisAtt = new G4VisAttributes(G4Colour(1.,0.,1.)); // magenta
    magentaVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(clamp, material, logicalName);
    pVol->SetVisAttributes(magentaVisAtt); 
    MGLog(debugging) << "Created Temperature Sensor Clamp Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Temperature Sensor Clamp Logical" << endlog; 
  return pVol;
}
