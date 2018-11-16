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
// $Id: MJcodeTemplate.cc,v 1.3 2005-02-15 23:26:55 akbarm Exp $ p
//      
// CLASS IMPLEMENTATION:  MJGeometryCharacterization.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Modified from the MJGeometryDemonstrator class for a collimated
 * button source surrounding a test cryostat with a detector string.
 *
 * The position of the source can be specified in cylindrical coordinates, 
 * with the source always pointing inwards. There is about 90 mm of collimator
 * between the source and the central z-axis.
 * 
 * The characterization stand design was made from the engineering drawings for the 
 * "Collimator w/PMT Assy" from Matthew Busch
 *
 * For usage, see $MAGEDIR/macros/Majorana/charStandGeometry.mac
 *
 * This is a work in progress.  Still to do:
 * - get the correct button source design
 * - correctly place the detector string
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Asher Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 * 
 * REVISION:
 * 7-22-2013, Created, A. Wasserman 
 * 10-17-2013, Updated quickly so MaGe will compile with the new changes made from the STC.
 *             I will look at it in more detail and update later. J. MacMullin 
 * 11-14-2013, Updated with the STC Assembly, J. MacMullin
 * 01-21-2014, Added ability to rotate ColdPlate, CMS, and IR shield of the STC from messenger. J. MacMullin
 *			   For more information see: https://majorana.npl.washington.edu/elog/Simulations+and+Analysis/54
 * 09-28-2016, Added capability to choose a thin IR shield in the STC. This is chosen by passing the string "STCCryoThinIR"
 *             through the serial number of the MJSTCAssembly object. I've added a boolean command to issue this call.
 *             C. O'Shaughnessy
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include <math.h>
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"

#include "mjdemonstrator/MJGeometryCharacterization.hh" 
#include "mjdemonstrator/MJVDemoPart.hh"
#include "mjdemonstrator/MJDemoStringAssembly.hh"
#include "mjdemonstrator/MJDemoPuckBeGe.hh"
#include "mjdemonstrator/MJCharStandAssembly.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"

#include "mjdemonstrator/MJGeometryCharacterizationMessenger.hh"

#include "mjdemonstrator/MJSTCAssembly.hh"
// include the path to new PopTop cryostat when ready
//#include "path/to/PopTopAssembly.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryCharacterization::MJGeometryCharacterization(G4String version): 
     MGGeometryDetector(version), 
     cavernPhys(0), 
     cavernLog(0),
     fMessenger(0), 
     fCavernOn(0),
     fStandOn(1),
     fCryostatOn(1),
     fCryostat("STC"),
     fColdPlateRotationInDegrees(0.0*deg),
     fCMSRotationInDegrees(0.0*deg),
     fThermalRotationInDegrees(0.0*deg),
     fStringOn(1),
     fThinIR(0),
     fSourceAngle(0.0 * rad),
     fSourceRadius(30.0 * cm),
     fSourceZ(-10 * cm),
     sourceMaterialName("Barium-133"),
     fStringName("C1S1")
{
fMessenger = new MJGeometryCharacterizationMessenger(this);
}

//---------------------------------------------------------------------------//

MJGeometryCharacterization::~MJGeometryCharacterization()
{
delete fMessenger;
}

//---------------------------------------------------------------------------//

void MJGeometryCharacterization::ConstructDetector()
{
  G4double eps = 0.001 * mm;
  G4String fSerial = "867-5309";

  G4Material *vacuum = G4Material::GetMaterial("Air");
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

  MJCharStandAssembly* calibrationStand = new MJCharStandAssembly("StandAssembly_001", "867-5309", sourceMaterialName);
  G4ThreeVector* positionA = new G4ThreeVector(fSourceRadius * cos(fSourceAngle), fSourceRadius * sin(fSourceAngle), fSourceZ);
  G4RotationMatrix* rotationA = new G4RotationMatrix();
  // note order of rotation is "backwards"
  // order of rotations is about z-axis by fSourceAngle, then about the line perpendicular to position vector in x-y plane by pi/2 rad
  // Geant4 seems to do compositions of rotations opposite this order
  rotationA->rotate(-pi/2.0, G4ThreeVector(-sin(fSourceAngle),cos(fSourceAngle),0));
  rotationA->rotateZ(-fSourceAngle - pi/2.0);
  // attempted rotation of stand by specifying resulting axes, opted for two-step rotation instead
  //rotationA->rotateAxes(G4ThreeVector(0,0,-1), G4ThreeVector(-sin(fSourceAngle),cos(fSourceAngle),0), G4ThreeVector(cos(fSourceAngle),sin(fSourceAngle),0));
  if(fStandOn) calibrationStand->Place(positionA, rotationA, theDetectorLogical);  

  if(fCryostatOn)
    {
      if (fCryostat == "STC")
	{
	  //NOTE: To choose a thin IR assembly we pass the string "STCCryoThinIR" through the serial number of MJSTCAssembly
	  if (fThinIR)
		  fSerial = "STCCryoThinIR";
	  std::cout<<"STCAssembly fSerial: "<<fSerial<<std::endl; 
	  MJSTCAssembly* StringTestCryoAssembly = new MJSTCAssembly("StringTestCryoAssembly_001", fSerial, fColdPlateRotationInDegrees, fCMSRotationInDegrees, fThermalRotationInDegrees); 
	  G4ThreeVector* positionB = new G4ThreeVector(0, 0, 0);
	  G4RotationMatrix* rotationB = new G4RotationMatrix();
	  StringTestCryoAssembly->Place(positionB, rotationB, theDetectorLogical);

          MJDemoStringAssembly* String = new MJDemoStringAssembly("StringAssembly_001", fStringName); 
          G4ThreeVector* positionC = new G4ThreeVector(0, 0, -eps); 
	  G4RotationMatrix* rotationC = new G4RotationMatrix(); 
          rotationC->rotateZ(pi/6.0); 
          if(fStringOn) String->Place(positionC, rotationC, theDetectorLogical);
	}
      else if(fCryostat == "PopTop")
	{
	  //not implemented yet
	  /*
	    MJCharPopTopAssembly* popTop = new MJCharPopTopAssembly("PopTopAssembly_001", "867-5309");
	  G4ThreeVector* positionB = new G4ThreeVector(0,0,0);
	  G4RotationMatrix* rotationB = new G4RotationMatrix();
	  popTop->Place(positionB, rotationB, theDetectorLogical);
	  */
	}
    }

}

void MJGeometryCharacterization::PrintSourcePosition()
{
  G4cout << "Source radius:  " << fSourceRadius/cm << " cm" << G4endl;
  G4cout << "Source angle:  " << fSourceAngle/rad << " rad" << G4endl;
  G4cout << "Source height:  " << fSourceZ/cm << " cm" << G4endl;
}

// identical method from MJGeometryDemonstrator
int MJGeometryCharacterization::GetSensitiveVolumeID(const string& volName)
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
