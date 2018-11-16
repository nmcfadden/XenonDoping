#include "tonscalegeometry/TSLabModuleBox.hh"
#include "tonscalegeometry/TSVisAttributes.hh"

#include "G4VisAttributes.hh"
#include "io/MGLogger.hh"

using namespace std;

using namespace CLHEP;

// -----------------------------------------------------------------------

TSLabModuleBox::TSLabModuleBox()
{
  ConstructLabModuleMaterials();
  fG4Messenger = new TSLabModuleBoxMessenger(this);
  
  fRockMaterial=G4Material::GetMaterial("Rock");
  fShotcreteMaterial=G4Material::GetMaterial("Rock");	
  fLabSpaceMaterial=G4Material::GetMaterial("Air");	
  
  fLabSpaceBoxWidth=15*m;
  fLabSpaceBoxLength=25*m;
  fLabSpaceBoxHeight=17*m;
  fLabSpaceDomeOverhead=0.00*m;
  fLabModuleRockDepth=5*m;
  fShotcreteDepth=0.001*m;
}

// -----------------------------------------------------------------------

void TSLabModuleBox::ConstructLabModuleMaterials()
{
  G4String symbol;  
  G4double a, z, density;     
  G4int ncomponents;
  G4double fractionmass;
  
  //G4Element* C = new G4Element("Carbon", symbol="C", z=6., a=12.01*g/mole);
  //G4Element* H = new G4Element("Hydrogen",   symbol="H", z=1., a=1.007*g/mole);
  G4Element* O = new G4Element("Oxygen",   symbol="O", z=8., a=16.00*g/mole);	
  G4Element* Si = new G4Element("Silicon", symbol="Si", z=14., a=28.0855*g/mole);
  G4Element* N = new G4Element("Nitrogen", symbol="N", z=7., a=14.0067*g/mole);
  G4Element* Ar = new G4Element("Argon",   symbol="Ar", z=18., a=39.948*g/mole);
  
  //same as in database
  G4Material* Rock = new G4Material("Rock", density= 2.6*g/cm3, ncomponents=2);
  Rock->AddElement(Si, fractionmass=0.4674);
  Rock->AddElement(O, fractionmass=0.5326);
  //same as in database:
  G4Material* Air = new G4Material("Air", density=0.001161*g/cm3, ncomponents=3);
  Air->AddElement(N, fractionmass=0.767);
  Air->AddElement(O, fractionmass=0.233);
  Air->AddElement(Ar, fractionmass=0.0);
}

// -----------------------------------------------------------------------

void TSLabModuleBox::Construct()
{
  G4double ShotcreteLength=fLabSpaceBoxLength + 2.*fShotcreteDepth;
  G4double ShotcreteWidth=fLabSpaceBoxWidth + 2.*fShotcreteDepth;
  G4double ShotcreteHeight=fLabSpaceBoxHeight + 2.*fShotcreteDepth;
  
  G4double TotalLength=ShotcreteLength + 2.*fLabModuleRockDepth;
  G4double TotalWidth=ShotcreteWidth + 2.*fLabModuleRockDepth;
  G4double TotalHeight=ShotcreteHeight + 2.*fLabModuleRockDepth;
  
  G4Box *LabSpaceBox = new G4Box("LabSpaceBox",fLabSpaceBoxLength/2.,fLabSpaceBoxWidth/2., fLabSpaceBoxHeight/2.);
  G4Box *ShotcreteBox = new G4Box("ShotcreteBox",ShotcreteLength/2. ,ShotcreteWidth/2., ShotcreteHeight/2.);	
  G4Box *RockBox = new G4Box("RockBox",TotalLength/2.,TotalWidth/2.,TotalHeight/2.);
  
  /* Part to create a domed ceiling:
     G4double domeangle=pi-2.*atan(fLabSpaceBoxWidth/2./fLabSpaceDomeOverhead);
     G4double domeoffset=fLabSpaceBoxWidth/2./tan(domeangle);
     G4double domeradius=domeoffset+fLabSpaceDomeOverhead;
     
     G4Tubs *LabSpaceDomeTub = new G4Tubs("LabSpaceDomeTub",0,domeradius,fLabSpaceBoxLength,0,2*pi);
     G4RotationMatrix* domerotation = new G4RotationMatrix();
     domerotation->rotateY(90*deg);
     G4SubtractionSolid *LabSpaceDome = new G4SubtractionSolid("LabSpaceDome",LabSpaceDome,LabSpaceBox,domerotation,G4ThreeVector(domeoffset,0,0));
     G4UnionSolid *LabSpace = new G4UnionSolid("LabSpace",LabSpaceBox,LabSpaceDome);
  */
  
  //if(fRockBoxLogical!=NULL)
  fRockBoxLogical = new G4LogicalVolume(RockBox,fRockMaterial,"RockBoxLogical");
  //if(fLabSpaceLogical!=NULL)
  fLabSpaceLogical = new G4LogicalVolume(LabSpaceBox,fLabSpaceMaterial,"LabSpaceLogical");
  fShotcreteLogical = new G4LogicalVolume(ShotcreteBox,fShotcreteMaterial,"ShotcreteLogical");
  
  fRockBoxLogical->SetVisAttributes(new TSVisAttributesSolid("black"));
  fLabSpaceLogical->SetVisAttributes(new TSVisAttributesWireFrame("white"));
  fShotcreteLogical->SetVisAttributes(new TSVisAttributesSolid("grey"));
  
  G4RotationMatrix* rotation = new G4RotationMatrix();
  //rotation->rotateY(-90*deg);
  fShotcretePlacement= new G4PVPlacement(rotation, G4ThreeVector(0,0,0), fShotcreteLogical,"ShotCretePhysical",fRockBoxLogical,false,0,false);
  fLabSpacePlacement= new G4PVPlacement(rotation, G4ThreeVector(0,0,0), fLabSpaceLogical,"LabSpacePhysical",fShotcreteLogical,false,0,false);
  
  MGLog(routine)<<"Volume of lab module "<<LabSpaceBox->GetCubicVolume()/(m*m*m)<<"m3"<<endl;
  MGLog(routine)<<"Volume of shotcrete "<<ShotcreteBox->GetCubicVolume()/(m*m*m)<<"m3"<<endl;
  MGLog(routine)<<"Volume of rock "<<RockBox->GetCubicVolume()/(m*m*m)<<"m3"<<endl;
}

// -----------------------------------------------------------------------

