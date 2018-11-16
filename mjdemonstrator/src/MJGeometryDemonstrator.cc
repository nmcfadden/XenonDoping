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
// $Id: MJcodeTemplate.cc,v 1.3 2005-02-15 23:26:55 akbarm Exp $
//
// CLASS IMPLEMENTATION:  MJGemometryDemonstrator.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * If the cables are included, there are overlapping geometries. -Jacquie
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT:
 * FIRST SUBMISSION: Thu Feb  4 17:20:54 EST 2010
 *
 * REVISION:
 * 02-24-2017, Added polyshield R. Massarczyk
 * 02-24-2016, Add messengers for the cryostat assemblies, revert the
 *             world material to vacuum, T. Caldwell
 * 04-24-2014, Fixed overlapping geometries, J. MacMullin
 * 10-12-2011, Added realistically-sized cavern, MJDemo is placed in correct
 *             location inside cavern, M. Kidd
 * 12-17-2010, Added SensVolID for muon veto panels, M. Kidd
 * 07-21-2010, Created, M. Green
 *
 */
//---------------------------------------------------------------------------//
//


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

#include "mjdemonstrator/MJGeometryDemonstrator.hh"
#include "mjdemonstrator/MJVDemoPart.hh"
#include "mjdemonstrator/MJDemo7StringAssembly.hh"
#include "mjdemonstrator/MJDemoStringAssembly.hh"
#include "mjdemonstrator/MJProto7StringAssembly.hh"
#include "mjdemonstrator/MJDemoRadShieldAssembly.hh"
#include "mjdemonstrator/MJDemoCalibrationAssembly.hh"
#include "mjdemonstrator/MJDemoCryostatAssembly.hh"
#include "mjdemonstrator/MJDemoCrystalAssembly.hh"
#include "mjdemonstrator/MJDemoMuonVetoAssembly.hh"
#include "mjdemonstrator/MJDemoPolyAssembly.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "mjdemonstrator/MJDemoJSONDet.hh"
#include "io/MGLogger.hh"
#include "mjio/MJJSONReader.hh"

#include "mjdemonstrator/MJGeometryDemonstratorMessenger.hh"
#include "mjdemonstrator/MJDemoCryostatAssemblyMessenger.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryDemonstrator::MJGeometryDemonstrator(G4String version):
     MGGeometryDetector(version),
     cavernPhys(0),
     cavernLog(0),
     fMessenger(0),
     fCryostatChoice("MJD"),
     fCavernOn(0),
     fMuonVetoOn(1),
     fRadShieldOn(1),
     fPolyShield("DS6"),
     fCryoWSN("NULL"),
     fCryoESN("NULL"),
     fInnerCopperOn(1),
     fTSShieldOn(1),
     fShadowShieldThickness(12)
{
  //fMessenger = new MJGeometryDemonstratorMessenger(this);
  fCryoAssemblyMessengerW = new MJDemoCryostatAssemblyMessenger("W");
  fCryoAssemblyMessengerE = new MJDemoCryostatAssemblyMessenger("E");
  fCryo1Pos = G4ThreeVector(-8.1417 * 25.4 * mm, 0.0, 4.4265 * 25.4 * mm);
  fCryo1Rot = pi / 2;
  fCryo2Pos = G4ThreeVector(-fCryo1Pos.x(), fCryo1Pos.y(), fCryo1Pos.z());
  fCryo2Rot = 0.0;
  fMessenger = new MJGeometryDemonstratorMessenger(this);
}

//---------------------------------------------------------------------------//

MJGeometryDemonstrator::~MJGeometryDemonstrator()
{
  delete fMessenger;
  delete fCryoAssemblyMessengerW;
  delete fCryoAssemblyMessengerE;
}

//---------------------------------------------------------------------------//

void MJGeometryDemonstrator::ConstructDetector()
{
  std::vector<G4String> cryostatList;
  MJJSONReader* reader = new MJJSONReader();
  cryostatList = reader->ReadCryostats(fCryostatChoice);
  delete reader;
  if(!cryostatList.size())
    {
      fCryoWSN = "NULL";
      fCryoESN = "NULL";
    }
  if(cryostatList.size() < 2)
    {
      fCryoWSN = cryostatList.at(0);
      fCryoESN = "NULL";
    }
  if(cryostatList.size() >= 2)
    {
      fCryoWSN = cryostatList.at(0);
      fCryoESN = cryostatList.at(1);
    }
  G4Material *vacuum = G4Material::GetMaterial("Vacuum");
  G4Box* exptBox = new G4Box("exptBox", 200.*m, 200.*m, 200.*m);
  G4VisAttributes* MagVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0)); // magenta
  MagVisAtt->SetForceWireframe( true );
  theDetectorLogical = new G4LogicalVolume(exptBox, vacuum, "theDetectorLogical");
  //theDetectorLogical->SetVisAttributes(MagVisAtt);
  theDetectorLogical->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4Material *rock = G4Material::GetMaterial("Rock");
  G4Box* outerBox = new G4Box("outerBox", 150.*m, 150.*m, 150.*m);
  G4Box* innerBox = new G4Box("innerBox", 7.1628*m, 20.5232*m, 1.524*m);
  G4SubtractionSolid *cavern = new G4SubtractionSolid("cavern", outerBox, innerBox, 0, G4ThreeVector(4.6478*m,-16.5611*m,0));
  G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.64, 0.64, 0.64, 0.15));//gray, 15% transparency
  cavernLog = new G4LogicalVolume(cavern, rock, "theDetectorLogical");
  cavernLog->SetVisAttributes(grayVisAtt);

  if(fCavernOn) cavernPhys = new G4PVPlacement(0, G4ThreeVector(0,0,0), cavernLog, "cavern", theDetectorLogical, false, 0);

  MJDemoMuonVetoAssembly* muonVeto1 = new MJDemoMuonVetoAssembly("MuonVetoAssembly_001", "867-5309");
  G4ThreeVector* positionA = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix* rotationA = new G4RotationMatrix();
  if(fMuonVetoOn) muonVeto1->Place(positionA, rotationA, theDetectorLogical);

  G4String radShieldSN = "RadShieldSN_";
  if (fCryoESN!="NULL") {
    radShieldSN.append("E");
    if (fTSShieldOn) fCryoESN.append("TSShield");
  }
  if (fCryoWSN!="NULL") {
    radShieldSN.append("W");
    if (fTSShieldOn) fCryoWSN.append("TSShield");
  }
  if (fInnerCopperOn) radShieldSN.append("IC");
  if (fShadowShieldThickness) {
    char thick[99];
    sprintf(thick, "ShadowShield%d", GetShadowShieldThickness());
    radShieldSN.append(thick);
  }

  MJDemoRadShieldAssembly* radShield1 = new MJDemoRadShieldAssembly("RadShieldAssembly_001", radShieldSN);
  G4ThreeVector* positionB = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix* rotationB = new G4RotationMatrix();
  if(fRadShieldOn) radShield1->Place(positionB, rotationB, theDetectorLogical);

  MJDemoCryostatAssembly* cryostatW = new MJDemoCryostatAssembly("CryostatAssembly_001", fCryoWSN);
  fCryoAssemblyMessengerW->SetValues(cryostatW);
  G4ThreeVector* positionC = new G4ThreeVector(fCryo1Pos);
  G4RotationMatrix* rotationC = new G4RotationMatrix();
  rotationC->rotateZ(fCryo1Rot);
  if(fCryoWSN != "NULL") cryostatW->Place(positionC, rotationC, theDetectorLogical);


  MJDemoCryostatAssembly* cryostatE = new MJDemoCryostatAssembly("CryostatAssembly_002", fCryoESN);
  fCryoAssemblyMessengerE->SetValues(cryostatE);
  G4ThreeVector* positionD = new G4ThreeVector(fCryo2Pos);
  G4RotationMatrix* rotationD = new G4RotationMatrix();
  rotationD->rotateZ(fCryo2Rot);
  if(fCryoESN != "NULL") cryostatE->Place(positionD, rotationD, theDetectorLogical);

	MJDemoPolyAssembly* thePolyShield = new MJDemoPolyAssembly("PolyAssembly_001", fPolyShield);
  G4ThreeVector* positionPoly = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix* rotationPoly = new G4RotationMatrix();
  if(fPolyShield != "NULL") thePolyShield->Place(positionPoly, rotationPoly, theDetectorLogical);
}


int MJGeometryDemonstrator::GetSensitiveVolumeID(const string& volName)
{
  int sensVolID = 0;
  int vetonum = 0;

  size_t found = volName.find("Active");
  if(found == string::npos) return 0;

  G4PhysicalVolumeStore *storePtr = G4PhysicalVolumeStore::GetInstance();
  G4VPhysicalVolume* pVol = storePtr->GetVolume(volName, false);
  G4String material = pVol->GetLogicalVolume()->GetMaterial()->GetName();

  if (material == "Germanium-Enr" || material == "Germanium-Nat"){
  	sensVolID = 1000000;
    MGLog(debugging) << "Germanium Crystal:" << endlog;
    found = volName.find("CryostatAssembly_");
    if (found != string::npos){
    MGLog(debugging) << "Array No: " << volName.substr(found+14, 3) << endlog;
    sensVolID += atoi(volName.substr(found+17, 3).c_str()) * 10000;
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

  if (material == "Scintillator-Pla"){

    sensVolID = 9000000;
    MGLog(routine) << "Muon Veto Cross-Arm Panel:" << volName.substr(found+21, 3) << endlog;
    found = volName.find("MuonVetoThroughPanel_");
    vetonum = atoi(volName.substr(found+21, 3).c_str());
    if (found != string::npos){
    MGLog(routine) << "Through Panel No: " << volName.substr(found+21, 3) << endlog;
       if (vetonum <= 4)         sensVolID += 10000;  //short side with cross arm
       else if (vetonum <= 8)    sensVolID += 20000;  //long side with cross arm
       if      (vetonum == 1 || vetonum == 5)   sensVolID += 201;  //outer top panels
       else if (vetonum == 2 || vetonum == 6)   sensVolID += 202;  //outer bottom panels
       else if (vetonum == 3 || vetonum == 7)   sensVolID +=101;   //inner right panels
       else if (vetonum == 4 || vetonum == 8)   sensVolID += 102;  //inner left panels
    MGLog(routine) << "sensVolID =  " << sensVolID << endlog;
    }
    MGLog(routine) << "Muon Veto Solid Panel:" << volName.substr(found+19, 3) << endlog;
    found = volName.find("MuonVetoSolidPanel_");
    vetonum = atoi(volName.substr(found+19, 3).c_str());
    if (found != string::npos){
    MGLog(routine) << "Solid Panel No: " << volName.substr(found+19, 3) << endlog;
       if (vetonum <= 4)         sensVolID += 30000;  //short side solid panels
       else if (vetonum <= 8)    sensVolID += 40000;  //long side solid panels
       else if (vetonum <= 12)   sensVolID += 50000;  //top panels
       if      (vetonum == 1 || vetonum == 5 || vetonum == 9)   sensVolID += 201;  //outer top panels (or outer front for lid)
       else if (vetonum == 2 || vetonum == 6 || vetonum ==10)   sensVolID += 202;  //outer bottom panels  (or outer back for lid)
       else if (vetonum == 3 || vetonum == 7 || vetonum ==11)   sensVolID += 101;  //inner right panels
       else if (vetonum == 4 || vetonum == 8 || vetonum ==12)   sensVolID += 102;  //inner left panels
    MGLog(routine) << "sensVolID =  " << sensVolID << endlog;
    }
    MGLog(routine) << "Muon Veto OverFloor Assembly No:" << volName.substr(found+22, 3) << endlog;
    found = volName.find("MuonVetoOverFloorAssm_");
    vetonum = atoi(volName.substr(found+22, 3).c_str());
    if (found != string::npos){
    MGLog(routine) << "Over Floor Panel Assembly No: " << volName.substr(found+22, 3) << endlog;
       if (vetonum == 1)         sensVolID += 60100;  //overfloor top layer
       else if (vetonum == 2)    sensVolID += 60200;  //overfloor bottom layer
    }
    MGLog(routine) << "Muon Veto OverFloor Panel No:" << volName.substr(found+23, 3) << endlog;
    found = volName.find("MuonVetoOverFloorPanel_");
    vetonum = atoi(volName.substr(found+23, 3).c_str());
    if (found != string::npos){
    MGLog(routine) << "Over Floor Panel No: " << volName.substr(found+23, 3) << endlog;
    sensVolID += atoi(volName.substr(found+23, 3).c_str()) * 1;
    MGLog(routine) << "sensVolID =  " << sensVolID << endlog;
    }
  }
  return sensVolID;
}
