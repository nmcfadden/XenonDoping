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
// CLASS IMPLEMENTATION:  MJGeometrySTC.cc
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
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday June 12 2013
 * 
 * REVISION:
 * 06-12-2013, Created, J. Strain
 * 10-17-2013, Updated Coldplate and added Dewar. J. MacMullin
 * 11-14-2013, 1. Made the STC an assembly so it could place in the CharStand.
 *			   2. Updated so that MJSTC is placed in correct location inside cavern.
 *			   The STC cavern placement is consistent with the MJD cavern placement. 
 *			   3. Added a string into the STC. J. MacMullin
 * 01-21-2014, 1. Created a "shell" w/in the rock cavern in the e-forming room for 
 *				  simulating the rock around the STC. 
 *			   2. Added in walls of the e-forming room.
 *			   3. Updated z position of STC in cavern. 
 *             4. Added ability to rotate ColdPlate, CMS, and IR shield from messenger. J. MacMullin
 *			      For more information see: https://majorana.npl.washington.edu/elog/Simulations+and+Analysis/54
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"

#include "mjdemonstrator/MJGeometrySTC.hh"
#include "mjdemonstrator/MJVDemoPart.hh"
#include "mjdemonstrator/MJSTCAssembly.hh"
#include "mjdemonstrator/MJDemoStringAssembly.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"
#include "mjdemonstrator/MJGeometrySTCMessenger.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometrySTC::MJGeometrySTC(G4String version):
     MGGeometryDetector(version),
     cavernPhys(0),
     cavernLog(0),
     EFRShellPhys(0),
     EFRShellLog(0),
     EFRwallsPhys(0),
     EFRwallsLog(0),
     fMessenger(0),
     fEFROn(1),
     fCavernOn(1),
     fEFRwallsOn(1),
     fStringTestCryoOn(1),
     fColdPlateRotationInDegrees(0.0*deg),
     fCMSRotationInDegrees(0.0*deg),
     fThermalRotationInDegrees(0.0*deg),
     fStringOn(1)
{
fMessenger = new MJGeometrySTCMessenger(this);
}

//---------------------------------------------------------------------------//

MJGeometrySTC::~MJGeometrySTC()
{
delete fMessenger;
}

//---------------------------------------------------------------------------//

void MJGeometrySTC::ConstructDetector()
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
  G4Box* innerBox = new G4Box("innerBox", 7.1628*m, 20.5232*m, 1.524*m);
      
  if(fEFROn) { 
  G4double mfp = 5.0*0.099*m; //~5*Mean free path of 2614 keV gamma
  G4Box* EFRBox = new G4Box("EFRBox", 3.048*m+mfp/2.+eps, 4.3688*m+eps, 1.524*m+mfp+eps);
  G4Box* EFRBox2 = new G4Box("EFRBox2", 3.048*m+mfp/2., 4.3688*m, 1.524*m+mfp);
  
  G4SubtractionSolid *cavernB = new G4SubtractionSolid("cavernB", outerBox, innerBox, 
  											0, G4ThreeVector((7.1628-0.6096)*m, -(20.5232-11.5824)*m, (1.524-0.998)*m));
  G4SubtractionSolid *cavern = new G4SubtractionSolid("cavern", cavernB, EFRBox, 
  											0, G4ThreeVector((3.048-0.6096)*m-mfp/2., (-4.3688+1.6256)*m, (1.524-0.998)*m));
  G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.64, 0.64, 0.64, 0.15));//gray, 15% transparency
  cavernLog = new G4LogicalVolume(cavern, rock, "theDetectorLogical");
  cavernLog->SetVisAttributes(grayVisAtt);
  if(fCavernOn) cavernPhys = new G4PVPlacement(0, G4ThreeVector(0,0,0), cavernLog, "cavern", theDetectorLogical, false, 0);

  G4SubtractionSolid* EFRShell = new G4SubtractionSolid("EFRShell", EFRBox2, innerBox, 
  											0, G4ThreeVector((7.1628-0.6096-(3.048-0.6096))*m+mfp/2.,
  											(-(20.5232-11.5824)-(-4.3688+1.6256))*m, 0*m));
  G4VisAttributes* cyanVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 1.0));
  EFRShellLog = new G4LogicalVolume(EFRShell, rock, "theDetectorLogical");
  EFRShellLog->SetVisAttributes(cyanVisAtt); 
  EFRShellPhys = new G4PVPlacement(0, 
  							G4ThreeVector((3.048-0.6096)*m-mfp/2., (-4.3688+1.6256)*m, (1.524-0.998)*m), 
  							EFRShellLog, "EFRShell", theDetectorLogical, false, 0);
  MGLog(routine)<<"Volume of EFR Shell "<<EFRShell->GetCubicVolume()/(m*m*m)<<"m3"<<endl;
  }
  
  else {
  G4SubtractionSolid *cavern = new G4SubtractionSolid("cavern", outerBox, innerBox, 
  											0, G4ThreeVector((7.1628-0.6096)*m, -(20.5232-11.5824)*m, (1.524-0.998)*m));
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
  
  G4Box* EFRwWalls = new G4Box("EFRwWalls", (3.048+0.1016)*m, (4.3688+0.2032)*m, 1.524*m-eps);
  G4Box* EFR = new G4Box("EFR", (3.048+.1016)*m, 4.3688*m, (1.524+0.2)*m);
  G4SubtractionSolid *EFRwalls = new G4SubtractionSolid("EFRwalls", EFRwWalls, EFR, 
  											0, G4ThreeVector(-0.2032*m, 0, 0));
  G4VisAttributes* WhiteVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.85));
  EFRwallsLog = new G4LogicalVolume(EFRwalls, concrete, "theDetectorLogical");
  EFRwallsLog->SetVisAttributes(WhiteVisAtt);
  if(fEFRwallsOn) EFRwallsPhys = new G4PVPlacement(0, 
  							G4ThreeVector(((3.048+0.1016)-0.6096)*m+eps, (-(4.3688+0.2032)+1.6256+0.2032)*m, (1.524-0.998)*m), 
  							EFRwallsLog, "EFRwalls", theDetectorLogical, false, 0); 
  MGLog(routine)<<"Volume of EFR Walls "<<EFRwalls->GetCubicVolume()/(m*m*m)<<"m3"<<endl; 
     
  MJSTCAssembly* StringTestCryoAssembly = new MJSTCAssembly("StringTestCryoAssembly_001", "867-5309", fColdPlateRotationInDegrees, fCMSRotationInDegrees, fThermalRotationInDegrees);
  G4ThreeVector* positionA = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix* rotationA = new G4RotationMatrix();
  if(fStringTestCryoOn) StringTestCryoAssembly->Place(positionA, rotationA, theDetectorLogical);
  
  MJDemoStringAssembly* String = new MJDemoStringAssembly("DetectorArray_001", "P37N7", 4);
  G4ThreeVector* positionB = new G4ThreeVector(0, 0, -eps);
  G4RotationMatrix* rotationB = new G4RotationMatrix();
  rotationB->rotateZ(pi/6.0);
  if(fStringOn) String->Place(positionB, rotationB, theDetectorLogical);

}

int MJGeometrySTC::GetSensitiveVolumeID(const string& volName)
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
