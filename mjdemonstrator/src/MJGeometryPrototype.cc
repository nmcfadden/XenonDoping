//SPECIAL NOTES:
//  If the cables are included, there are overlapping geometries. -Jacquie
//---------------------------------------------------------------------------//
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

#include "mjdemonstrator/MJGeometryPrototype.hh" 
#include "mjdemonstrator/MJVDemoPart.hh"
#include "mjdemonstrator/MJProto7StringAssembly.hh"
#include "mjdemonstrator/MJProtoThermalShieldAssembly.hh"
#include "mjdemonstrator/MJDemoConstrMuonVetoAssembly.hh"
#include "mjdemonstrator/MJDemoConstrRadShieldAssembly.hh"
#include "mjdemonstrator/MJProtoMonolithCuPlate.hh"
#include "mjdemonstrator/MJDemoCalibrationAssembly.hh"
#include "mjdemonstrator/MJProtoCryostatAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonAssembly.hh"
#include "mjdemonstrator/MJDemoCrystalAssembly.hh"
#include "mjdemonstrator/MJDemoCablingAssembly.hh"
#include "mjdemonstrator/MJDemoStringAssembly.hh"
#include "mjdemonstrator/MJProtoTempSensorAssembly.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "mjdemonstrator/MJDemoJSONDet.hh"
#include "io/MGLogger.hh"
#include "mjio/MJJSONReader.hh"

#include "mjdemonstrator/MJGeometryPrototypeMessenger.hh"

using namespace CLHEP;



//---------------------------------------------------------------------------//

MJGeometryPrototype::MJGeometryPrototype(G4String version): 
     MGGeometryDetector(version), 
     cavernPhys(0), 
     cavernLog(0), 
     DRShellPhys(0),
     DRShellLog(0),
     DRwallsPhys(0),
     DRwallsLog(0),
     fMessenger(0), 
     fPCposition("atGlovebox"), 
     fDRshellOn(1),
     fCavernOn(1), 
     fDRwallsOn(1),
     fMuonVetoOn(1),
     fVetoConfig("111111"),
     fRadShieldOn(1),
     fRadConfig("1111111"),
     fMonoCuPlateOn(1),
     fCalOn(1),
     fCryoOn(1),
     fThermosiphonOn(1),
     fThermalShieldOn(1),
     //fCablesOn(1),
     fStringOn(1),
     fTempSensorOn(1)

{
fMessenger = new MJGeometryPrototypeMessenger(this);
}

//---------------------------------------------------------------------------//

MJGeometryPrototype::~MJGeometryPrototype()
{
delete fMessenger;
}

//---------------------------------------------------------------------------//

void MJGeometryPrototype::ConstructDetector()
{   
  G4double eps = 0.001 * mm;

  G4Material *vacuum = G4Material::GetMaterial("Air");
  G4Box* exptBox = new G4Box("exptBox", 200.*m, 200.*m, 200.*m);
  G4VisAttributes* MagVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0)); // magenta
  MagVisAtt->SetForceWireframe( false );
  theDetectorLogical = new G4LogicalVolume(exptBox, vacuum, "theDetectorLogical");
  theDetectorLogical->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4Material *rock = G4Material::GetMaterial("Rock");
  G4Box* outerBox = new G4Box("outerBox", 150.*m, 150.*m, 150.*m);
  
  G4double labDim[3], PCpos[3], shieldDisp[3], expAngle, deltaX;
  labDim[0] = 7.1628*m; labDim[1] = 20.5232*m; labDim[2] = 1.6764*m+0.5*eps; //1.524+0.1524
  G4Box* innerBox = new G4Box("innerBox", labDim[0], labDim[1], labDim[2]);
  
  if(fPCposition == "atGlovebox"){
  PCpos[0] = 10.0584*m; //distance from DR's west wall
  PCpos[1] = 4.2672*m; //distance from DR's north wall
  PCpos[2] = 1.8288*m+eps; //distance from bottom of floor + thickness of floor (0.3048*m+eps)
  shieldDisp[0] = -(10.0584-2.515)*m;
  shieldDisp[1] = (4.2672-3.9621)*m;
  shieldDisp[2] = 0.*mm;
  expAngle = pi/2.;
  deltaX = 0.*mm;
  }
  
  if(fPCposition == "inShield"){
  PCpos[0] = 2.515*m; //7.1628-4.6478
  PCpos[1] = 3.9621*m; //20.5232-16.5611
  PCpos[2] = 1.8288*m+eps; //1.524+0.3048
  shieldDisp[0] = shieldDisp[1] = shieldDisp[2] = 0.*mm;
  expAngle = 0;
  deltaX = 8.1417*25.4*mm;
  MGLog(routine)<<"Turning off monolith Cu plate (fMonoCuPlateOn=0)"<<endl;
  fMonoCuPlateOn = 0;
  }
  
      
  if(fDRshellOn) { 
  G4double mfp = 5.0*0.099*m; //~5*Mean free path of 2614 keV gamma
  G4double DRdim[3];
  DRdim[0] = 6.096*m; DRdim[1] = 5.588*m; DRdim[2] = 1.6764*m;
  G4Box* DRBox = new G4Box("DRBox", DRdim[0]+mfp/2.+eps, DRdim[1]+mfp/2.+eps, DRdim[2]+mfp+1.5*eps);
  G4Box* DRBox2 = new G4Box("DRBox2", DRdim[0]+mfp/2., DRdim[1]+mfp/2., DRdim[2]+mfp+0.5*eps);
  G4SubtractionSolid *cavernB = new G4SubtractionSolid("cavernB", outerBox, innerBox, 0, 
  											G4ThreeVector(labDim[0]-PCpos[0], -labDim[1]+PCpos[1], labDim[2]-PCpos[2]));
  G4SubtractionSolid *cavern = new G4SubtractionSolid("cavern", cavernB, DRBox, 0,
  											G4ThreeVector(DRdim[0]-PCpos[0]-mfp/2., -DRdim[1]+PCpos[1]+mfp/2., DRdim[2]-PCpos[2]+0.5*eps));
  G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.64, 0.64, 0.64, 0.15));//gray, 15% transparency
  cavernLog = new G4LogicalVolume(cavern, rock, "theDetectorLogical");
  cavernLog->SetVisAttributes(grayVisAtt);
  if(fCavernOn) cavernPhys = new G4PVPlacement(0, G4ThreeVector(0,0,0), cavernLog, "cavern", theDetectorLogical, false, 0);

  G4SubtractionSolid* DRShell = new G4SubtractionSolid("DRShell", DRBox2, innerBox, 0,
  											G4ThreeVector(labDim[0]-DRdim[0]+mfp/2., -labDim[1]+DRdim[1]-mfp/2., labDim[2]-DRdim[2]-eps/2.));
  G4VisAttributes* cyanVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 1.0));
  DRShellLog = new G4LogicalVolume(DRShell, rock, "theDetectorLogical");
  DRShellLog->SetVisAttributes(cyanVisAtt); 
  DRShellPhys = new G4PVPlacement(0, 
  	G4ThreeVector(DRdim[0]-PCpos[0]-mfp/2., -DRdim[1]+PCpos[1]+mfp/2., DRdim[2]-PCpos[2]+eps/2.),
  	DRShellLog, "DRShell", theDetectorLogical, false, 0);
  MGLog(routine)<<"Volume of Detector Room Shell "<<DRShell->GetCubicVolume()/(m*m*m)<<"m3"<<endl;
  }  
  
  else {
  G4SubtractionSolid *cavern = new G4SubtractionSolid("cavern", outerBox, innerBox, 0, 
  											G4ThreeVector(labDim[0]-PCpos[0], -labDim[1]+PCpos[1], labDim[2]-PCpos[2]));
  G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.64, 0.64, 0.64, 0.15));//gray, 15% transparency
  cavernLog = new G4LogicalVolume(cavern, rock, "theDetectorLogical");
  cavernLog->SetVisAttributes(grayVisAtt);
  if(fCavernOn) cavernPhys = new G4PVPlacement(0, G4ThreeVector(0,0,0), cavernLog, "cavern", theDetectorLogical, false, 0);
  }
  
  //Create Concrete material
  G4String symbol;  
  G4double a, z, density, fractionmass;     
  G4int ncomponents;
  	
  G4Element* Si = new G4Element("Silicon", symbol="Si", z=14., a=28.0855*g/mole);
  G4Element* O = new G4Element("Oxygen", symbol="O", z=8., a=15.9994*g/mole);
  G4Element* H = new G4Element("Hydrogen", symbol="H", z=1., a=1.00794*g/mole);
  G4Element* Ca = new G4Element("Calcium", symbol="Ca", z=20., a=40.078*g/mole);
  G4Element* Al = new G4Element("Aluminum", symbol="Al", z=13., a=26.9815386*g/mole);
  G4Element* Fe = new G4Element("Iron", symbol="Fe", z=26., a=55.845*g/mole);
  G4Material* concrete = new G4Material("Concrete", density=2.3*g/cm3, ncomponents=6);
  concrete->AddElement(Si,fractionmass= 0.227915);
  concrete->AddElement(O,fractionmass= 0.60541);
  concrete->AddElement(H,fractionmass= 0.09972);
  concrete->AddElement(Ca,fractionmass= 0.04986);
  concrete->AddElement(Al,fractionmass= 0.014245);
  concrete->AddElement(Fe,fractionmass= 0.00285);
  
  G4double tab = 0.075 * m;
  G4Box* DR_CC = new G4Box("DR_CC", 0.1016*m, 11.176/2.*m, 1.524*m-eps);
  G4Box* DR_MS = new G4Box("DR_MS", (5.6896+0.2032)/2.*m+tab, 0.1016*m, 1.524*m-eps);
  G4UnionSolid* DR_CC_MS = new G4UnionSolid("DR_CC_MS", DR_CC, DR_MS, 
  											0, G4ThreeVector( (-(5.6896+0.2032)/2.-0.1016)*m+tab, 
  											(-11.176/2.+0.1016)*m, 0*m ) );
  G4Box* DR_SS = new G4Box("DR_SS", 0.1016*m, 1.2192/2.*m+tab, 1.524*m-eps);
  G4UnionSolid* DR_CC_MS_SS = new G4UnionSolid("DR_CC_MS_SS", DR_CC_MS, DR_SS, 
  											0, G4ThreeVector( -(5.6896+0.2032)*m, 
  											(-11.176/2.+1.2192/2.+0.2032)*m-tab, 0*m ) );
  G4Box* DR_ER = new G4Box("DR_ER", 6.096/2.*m+tab, 0.1016*m, 1.524*m-eps);
  G4UnionSolid* DRwalls = new G4UnionSolid("DRwalls", DR_CC_MS_SS, DR_ER, 
  											0, G4ThreeVector( -(0.1016+5.6896+0.2032+6.096/2.)*m+tab,
  											(-11.176/2.+1.2192+0.1016)*m, 0*m ) );
  G4Box* floor_main = new G4Box("floor_main", 6.096*m, 5.588*m, 0.1524*m);
  G4Box* floor_cutout = new G4Box("floor_subB", 4.*m, 4.*m, 4.*m);
  G4SubtractionSolid* DRfloor = new G4SubtractionSolid("DRfloor", floor_main, floor_cutout,
  											0, G4ThreeVector(-4.*m, (-5.588-4.+1.2192)*m, 0.*mm));
  G4UnionSolid* DR_walls_floor = new G4UnionSolid("DR_walls_floor", DRwalls, DRfloor,
  											0, G4ThreeVector( (-6.096+0.1016)*m, 0.*mm, -(1.524+0.1524)*m ) );
  G4VisAttributes* WhiteVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.85));
  DRwallsLog = new G4LogicalVolume(DRwalls, concrete, "theDetectorLogical");
  DRwallsLog->SetVisAttributes(WhiteVisAtt);
  if(fDRwallsOn) DRwallsPhys = new G4PVPlacement(0, 
  							G4ThreeVector((-0.1016+12.192)*m-PCpos[0]+eps, -11.176/2.*m+PCpos[1]-eps, 0*mm),
  							DRwallsLog, "DRwalls", theDetectorLogical, false, 0); 
  MGLog(routine)<<"Volume of Detector Room Walls (plus floor) "<<DR_walls_floor->GetCubicVolume()/(m*m*m)<<"m3"<<endl; 

  MJDemoConstrMuonVetoAssembly* muonVeto = new MJDemoConstrMuonVetoAssembly("ConstrMuonVetoAssembly_001", "867-5309", fVetoConfig);
  G4ThreeVector* positionA = new G4ThreeVector(shieldDisp[0], shieldDisp[1], shieldDisp[2]);
  G4RotationMatrix* rotationA = new G4RotationMatrix();
  if(fMuonVetoOn) muonVeto->Place(positionA, rotationA, theDetectorLogical);  

  MJDemoConstrRadShieldAssembly* radShield = new MJDemoConstrRadShieldAssembly("ConstrRadShieldAssembly_001", "867-5309", fRadConfig);
  G4ThreeVector* positionB = new G4ThreeVector(shieldDisp[0], shieldDisp[1], shieldDisp[2]);
  G4RotationMatrix* rotationB = new G4RotationMatrix();
 if(fRadShieldOn) radShield->Place(positionB, rotationB, theDetectorLogical);

  MJProtoMonolithCuPlate* monoCuPlate = new MJProtoMonolithCuPlate("MonolithCuPlate_001", "867-5309");
  G4ThreeVector* positionBB = new G4ThreeVector( 0, (((-1.645+.375)-6.875-4.0)*(25.4)-12.4968)*mm, 
  												(4.4265*25.4+106.058-26.1874)*mm );
  G4RotationMatrix* rotationBB = new G4RotationMatrix();
  rotationBB->rotateZ(expAngle+pi);
  if(fMonoCuPlateOn) monoCuPlate->Place(positionBB, rotationBB, theDetectorLogical);
 
  MJDemoCalibrationAssembly* calibration = new MJDemoCalibrationAssembly("CalibrationAssembly_001", "867-5309");
  G4ThreeVector* positionC = new G4ThreeVector(deltaX, 0, 4.4265*25.4*mm - 11*cm);
  G4RotationMatrix* rotationC = new G4RotationMatrix();
  if(fCalOn) calibration->Place(positionC, rotationC, theDetectorLogical);

  MJProtoCryostatAssembly* cryostat = new MJProtoCryostatAssembly("CryostatAssembly_001", "867-5309");
  G4ThreeVector* positionD = new G4ThreeVector(deltaX, 0, 4.4265*25.4*mm);
  G4RotationMatrix* rotationD = new G4RotationMatrix();
  rotationD->rotateZ(expAngle);
  if(fCryoOn) cryostat->Place(positionD, rotationD, theDetectorLogical);

  MJDemoThermosyphonAssembly* thermosiphon = new MJDemoThermosyphonAssembly("ThermosiphonAssembly_001", "867-5309");
  G4ThreeVector* positionE = new G4ThreeVector(deltaX+1.27*25.4*cos(expAngle)*mm, (-1.645+0.375)*25.4*sin(expAngle)*mm,  4.4266*25.4*mm);
  G4RotationMatrix* rotationE = new G4RotationMatrix();
  rotationE->rotateZ(expAngle);
  if(fThermosiphonOn) thermosiphon->Place(positionE, rotationE, theDetectorLogical);

  MJProtoThermalShieldAssembly* shield = new MJProtoThermalShieldAssembly("ThermalShieldAssembly_001", "867-5309");
  G4ThreeVector* positionF = new G4ThreeVector(deltaX, 0, (3.0265-0.2)*25.4*mm);
  G4RotationMatrix* rotationF = new G4RotationMatrix();
  rotationF->rotateZ(expAngle);
  if(fThermalShieldOn) shield->Place(positionF, rotationF, theDetectorLogical);

  /*MJDemoCablingAssembly* cables = new MJDemoCablingAssembly("CablingAssembly_001", "867-5309");
  //G4ThreeVector* positionG = new G4ThreeVector(8.1417*25.4*mm, 0, 3.4275*25.4*mm);
  G4ThreeVector* positionG = new G4ThreeVector(0, 0, 3.4275*25.4*mm);
  G4RotationMatrix* rotationG = new G4RotationMatrix();
  rotationG->rotateZ(expAngle);
  if(fCablesOn) cables->Place(positionG, rotationG, theDetectorLogical);*/

  MJProto7StringAssembly* stringAssem = new MJProto7StringAssembly("DetectorArray_001", "P3CK3");
  G4ThreeVector* positionH = new G4ThreeVector(deltaX, 0, 3.0265*25.4*mm);
  MGLog(routine)<<"string: "<<positionH[0]<<endl;
  G4RotationMatrix* rotationH = new G4RotationMatrix();
  rotationH->rotateZ(expAngle);
  if(fStringOn) stringAssem->Place(positionH, rotationH, theDetectorLogical);
  
  G4double hhr1x = 176.758*mm;
  G4double hhr1y = -35.8019*mm;
  
  MJProtoTempSensorAssembly* tempSensorS1D1 = new MJProtoTempSensorAssembly("TemperatureSensorAssembly_S1D1","867-5309");
  G4ThreeVector* positionI = new G4ThreeVector(hhr1x, hhr1y, 27.0*mm);
  MGLog(routine)<<"sensor1: "<<positionI[0]<<endl;
  G4RotationMatrix* rotationI = new G4RotationMatrix();
  rotationI->rotateZ(130*deg+30*deg);
  if(fTempSensorOn) tempSensorS1D1->Place(positionI, rotationI, theDetectorLogical);  
  
  MJProtoTempSensorAssembly* tempSensorS1D4 = new MJProtoTempSensorAssembly("TemperatureSensorAssembly_S1D4","867-5309");
  G4ThreeVector* positionJ = new G4ThreeVector(hhr1x, hhr1y, -170.0*mm);
  MGLog(routine)<<"sensor2: "<<positionJ[0]<<endl;
  G4RotationMatrix* rotationJ = new G4RotationMatrix();
  rotationJ->rotateZ(130*deg+30*deg);
  if(fTempSensorOn) tempSensorS1D4->Place(positionJ, rotationJ, theDetectorLogical);

  G4double hhr3x = 258.807*mm;
  G4double hhr3y = -83.1729*mm;

  MJProtoTempSensorAssembly* tempSensorS2D1 = new MJProtoTempSensorAssembly("TemperatureSensorAssembly_S2D1","867-5309");
  G4ThreeVector* positionK = new G4ThreeVector(hhr3x, hhr3y, 20.0*mm);//17
  MGLog(routine)<<"sensor3: "<<positionK[0]<<endl;
  G4RotationMatrix* rotationK = new G4RotationMatrix();
  rotationK->rotateZ(-110*deg+30*deg);
  rotationK->rotateY(180*deg);
  if(fTempSensorOn) tempSensorS2D1->Place(positionK, rotationK, theDetectorLogical);
  
  G4double hhr2x = 252.825;
  G4double hhr2y = -102.858;
  
  MJProtoTempSensorAssembly* tempSensorS3D2 = new MJProtoTempSensorAssembly("TemperatureSensorAssembly_S3D2","867-5309");
  G4ThreeVector* positionL = new G4ThreeVector(hhr2x, hhr2y, -20.0*mm);
  MGLog(routine)<<"sensor4: "<<positionL[0]<<endl;
  G4RotationMatrix* rotationL = new G4RotationMatrix();
  rotationL->rotateZ(-110*deg+120*deg);
  rotationL->rotateX(180*deg);
  if(fTempSensorOn) tempSensorS3D2->Place(positionL, rotationL, theDetectorLogical);   
  
  MJProtoTempSensorAssembly* tempSensorS3D5 = new MJProtoTempSensorAssembly("TemperatureSensorAssembly_S3D5","867-5309");
  G4ThreeVector* positionM = new G4ThreeVector(hhr2x, hhr2y, -170.0*mm);
  MGLog(routine)<<"sensor5: "<<positionM[0]<<endl;
  G4RotationMatrix* rotationM = new G4RotationMatrix();
  rotationM->rotateZ(-110*deg+150*deg);
  if(fTempSensorOn) tempSensorS3D5->Place(positionM, rotationM, theDetectorLogical);    

}

int MJGeometryPrototype::GetSensitiveVolumeID(const string& volName)
{
  int sensVolID = 0;

  size_t found = volName.find("Active");
  if(found == string::npos) return 0;

  G4PhysicalVolumeStore *storePtr = G4PhysicalVolumeStore::GetInstance();
  G4VPhysicalVolume* pVol = storePtr->GetVolume(volName, false);
  G4String material = pVol->GetLogicalVolume()->GetMaterial()->GetName();

  if (material == "Germanium-Enr" || material == "Germanium-Nat"){
  	sensVolID = 1000000;
    MGLog(debugging) << "Germanium Crystal:" << endlog;
    found = volName.find("DetectorArray_");
    if (found != string::npos){
    MGLog(debugging) << "Array No: " << volName.substr(found+14, 3) << endlog;
    sensVolID += atoi(volName.substr(found+14, 3).c_str()) * 10000;
    }
    found = volName.find("StringAssembly_");
    if (found != string::npos) {
    MGLog(debugging) << "String No: " << volName.substr(found+15, 3) << endlog;
    sensVolID += atoi(volName.substr(found+15, 3).c_str()) * 100;
    }
    found = volName.find("CrystalAssembly_");
    if (found != string::npos) {
    MGLog(debugging) << "Crystal No: " << volName.substr(found+16, 3) << endlog;
    sensVolID += atoi(volName.substr(found+16, 3).c_str()) * 1;
    }
  }
  
  return sensVolID;
}
