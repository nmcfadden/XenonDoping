#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJProtoTempSensorScrew.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJProtoTempSensorScrew::MJProtoTempSensorScrew(G4String partName, G4String serialNumber) :	
  MJVDemoPart(partName, serialNumber, "MJProtoTempSensorScrewDwg", "SS303") 
{;}

MJProtoTempSensorScrew::MJProtoTempSensorScrew(const MJProtoTempSensorScrew & rhs) : 
  MJVDemoPart(rhs)
{;}

MJProtoTempSensorScrew::~MJProtoTempSensorScrew()
{;}

G4LogicalVolume* MJProtoTempSensorScrew::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial; 
  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){  
    G4double radius = 0.0958*25.4*0.5*mm; //4-40 effective dia
    
    G4Tubs* screw = new G4Tubs("screw", 0.*mm, radius, 4.*mm, 0, 2*pi);    

    G4VisAttributes* blueVisAtt = new G4VisAttributes(G4Colour(0.,0.,1.)); // blue
    blueVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(screw, material, logicalName);
    pVol->SetVisAttributes(blueVisAtt); 
    MGLog(debugging) << "Created Temperature Sensor Screw Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing Temperature Sensor Screw Logical" << endlog; 
  return pVol;
}
