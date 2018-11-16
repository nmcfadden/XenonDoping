#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJProtoTempSensorAssembly.hh"
#include "mjdemonstrator/MJProtoTempSensorClamp.hh"
#include "mjdemonstrator/MJProtoTempSensorScrew.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJProtoTempSensorAssembly::MJProtoTempSensorAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "TempSensor")
{
  fClampPtr = new MJProtoTempSensorClamp(fName+"_Clamp_001", "867-5309");
  fScrewPtr = new MJProtoTempSensorScrew(fName+"_Screw_001", "867-5309");
}

MJProtoTempSensorAssembly::MJProtoTempSensorAssembly(const MJProtoTempSensorAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fClampPtr = new MJProtoTempSensorClamp(fName+"_Clamp_001", "867-5309");
  fScrewPtr = new MJProtoTempSensorScrew(fName+"_Screw_001", "867-5309");
}

MJProtoTempSensorAssembly::~MJProtoTempSensorAssembly()
{
  delete fClampPtr;
  delete fScrewPtr;
}

void MJProtoTempSensorAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4double eps = 0.001 * mm;  
  G4double outerRadius = 0.265*25.4*mm;
  G4double screwHalfLength = 4.*mm;
  
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *clamplocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *clamplocalRot = new G4RotationMatrix();
  G4AffineTransform *clampaffine = new G4AffineTransform(clamplocalRot,*clamplocalPos);
  *clampaffine*=*assemAffine;  
  G4ThreeVector *clampglobalPos = new G4ThreeVector(clampaffine->NetTranslation());
  G4RotationMatrix *clampglobalRot= new G4RotationMatrix(clampaffine->NetRotation());
  fClampPtr->Place(clampglobalPos, clampglobalRot, motherLogical);
  delete clamplocalPos;
  delete clamplocalRot;
  delete clampaffine;
  
  G4ThreeVector *screwlocalPos = new G4ThreeVector(0, screwHalfLength+outerRadius+eps, 0.5*0.180*25.4*mm);
  G4RotationMatrix *screwlocalRot = new G4RotationMatrix();
  screwlocalRot->rotateZ(-90*deg);
  screwlocalRot->rotateY(-90*deg);
  G4AffineTransform *screwaffine = new G4AffineTransform(screwlocalRot,*screwlocalPos);
  *screwaffine*=*assemAffine;  
  G4ThreeVector *screwglobalPos = new G4ThreeVector(screwaffine->NetTranslation());
  G4RotationMatrix *screwglobalRot= new G4RotationMatrix(screwaffine->NetRotation());
  fScrewPtr->Place(screwglobalPos, screwglobalRot, motherLogical);
  delete screwlocalPos;
  delete screwlocalRot;
  delete screwaffine;
    
  delete assemAffine;  
}
