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
// CLASS IMPLEMENTATION:  LGND_1T_Baseline.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: matthew_green@ncsu.edu
 * FIRST SUBMISSION: May 25 2017
 *
 * REVISION:
 * 05-25-2017, Created, M. Green
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"

#include "legendgeometry/LGND_1T_Baseline.hh"
#include "legendgeometry/LGND_Part.hh"
#include "legendgeometry/LGND_Assembly.hh"
#include "legendgeometry/LGND_1T_Cryostat.hh"
#include "legendgeometry/LGND_1T_ArrayAssembly.hh"
#include "legendio/LGND_JSONReader.hh"

#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"


//---------------------------------------------------------------------------//

using namespace CLHEP;


LGND_1T_Baseline::LGND_1T_Baseline(G4String version):
     MGGeometryDetector(version)

{
}

//---------------------------------------------------------------------------//

LGND_1T_Baseline::~LGND_1T_Baseline()
{
}

//---------------------------------------------------------------------------//

void LGND_1T_Baseline::ConstructDetector()
{
  G4Material *argon = G4Material::GetMaterial("Argon-Liq");
  G4Box* exptBox = new G4Box("exptBox", 100.*m, 100.*m, 100.*m);
  G4VisAttributes* ArVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0,0.01)); // magenta, 1% opaque
  ArVisAtt->SetForceWireframe( true );
  theDetectorLogical = new G4LogicalVolume(exptBox, argon, "theDetectorLogical");
  theDetectorLogical->SetVisAttributes(ArVisAtt);

  G4Material *rock = G4Material::GetMaterial("Rock");
  G4Box* outerBox = new G4Box("outerBox", 99.9999*m, 99.9999*m, 99.9999*m);
  G4Box* innerBox = new G4Box("innerBox", 20.0*m, 20.0*m, 20.0*m);
  G4SubtractionSolid *cavern = new G4SubtractionSolid("cavern", outerBox, innerBox);
  G4VisAttributes* rockVisAtt = new G4VisAttributes(G4Colour(0.64, 0.64, 0.64, 0.1));//gray, 10% opaque
  G4LogicalVolume* cavernLog = new G4LogicalVolume(cavern, rock, "CavernRockLogical");
  rockVisAtt->SetForceWireframe( true );
  cavernLog->SetVisAttributes(rockVisAtt);
  /*G4PVPlacement* cavernPhys =*/ new G4PVPlacement(0, G4ThreeVector(0,0,0), cavernLog, "cavern", theDetectorLogical, false, 0);

  G4Material *air = G4Material::GetMaterial("Air");
  G4Box* airboundary = new G4Box("airboundary", 19.9999*m, 19.9999*m, 19.9999*m);
  G4Tubs* cryostatvoid = new G4Tubs("cryostatvoid", 0, 2.0001*m, 2.0001*m, 0, 2*pi);
  G4SubtractionSolid *airspace = new G4SubtractionSolid("airspace", airboundary, cryostatvoid);
  G4VisAttributes* airVisAtt = new G4VisAttributes(G4Colour(0.64, 0.64, 0.64, 0.01));//gray, 1% opaque
  G4LogicalVolume* airLog = new G4LogicalVolume(airspace, air, "LabAirLogical");
  airVisAtt->SetForceWireframe(true);
  airLog->SetVisAttributes(airVisAtt);
  /*G4PVPlacement* airSpacePhys =*/ new G4PVPlacement(0, G4ThreeVector(0,0,0), airLog, "airSpace", theDetectorLogical, false, 0);

  LGND_1T_Cryostat* cryostat1 = new LGND_1T_Cryostat("Cryostat_001", "867-5309");
  G4ThreeVector* positionA = new G4ThreeVector(0);
  G4RotationMatrix* rotationA = new G4RotationMatrix();
  cryostat1->Place(positionA, rotationA, theDetectorLogical);

  std::vector<G4String> cryostatList;
  LGND_JSONReader* reader = new LGND_JSONReader("LGND_1T.json");
  cryostatList = reader->ReadCryostats("LGND1T");
  delete reader;
  MGLog(trace) << "Number of arrays: " << cryostatList.size() << endlog;
  if((cryostatList.size()>0)&&(cryostatList.at(0)!="NULL")){
    LGND_1T_ArrayAssembly* detArray1 = new LGND_1T_ArrayAssembly("DetectorArray_001", cryostatList.at(0));
    G4ThreeVector* position1 = new G4ThreeVector(500*mm, 0, 400.0*mm);
    G4RotationMatrix* rotation1 = new G4RotationMatrix();
    detArray1->Place(position1, rotation1, theDetectorLogical);
  }
  if((cryostatList.size()>1)&&(cryostatList.at(1)!="NULL")){
    LGND_1T_ArrayAssembly* detArray2 = new LGND_1T_ArrayAssembly("DetectorArray_002", cryostatList.at(1));
    G4ThreeVector* position2 = new G4ThreeVector(0, 500*mm, 400.0*mm);
    G4RotationMatrix* rotation2 = new G4RotationMatrix();
    detArray2->Place(position2, rotation2, theDetectorLogical);
  }
  if((cryostatList.size()>2)&&(cryostatList.at(2)!="NULL")){
    LGND_1T_ArrayAssembly* detArray3 = new LGND_1T_ArrayAssembly("DetectorArray_003", cryostatList.at(2));
    G4ThreeVector* position3 = new G4ThreeVector(-500*mm, 0, 400.0*mm);
    G4RotationMatrix* rotation3 = new G4RotationMatrix();
    detArray3->Place(position3, rotation3, theDetectorLogical);
  }
  if((cryostatList.size()>3)&&(cryostatList.at(3)!="NULL")){
    LGND_1T_ArrayAssembly* detArray4 = new LGND_1T_ArrayAssembly("DetectorArray_004", cryostatList.at(3));
    G4ThreeVector* position4 = new G4ThreeVector(0, -500*mm, 400.0*mm);
    G4RotationMatrix* rotation4 = new G4RotationMatrix();
    detArray4->Place(position4, rotation4, theDetectorLogical);
  }
}


int LGND_1T_Baseline::GetSensitiveVolumeID(const string& volName)
{
  int sensVolID = 0;

  if (volName == "Detector")
    return 201;

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
    found = volName.find("DetUnit_");
    if (found != string::npos) {
    MGLog(debugging) << "Crystal No: " << volName.substr(found+16, 3) << endlog;
    sensVolID += atoi(volName.substr(found+8, 3).c_str()) * 1;
    }
  }

  return sensVolID;
}
