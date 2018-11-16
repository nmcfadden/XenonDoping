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
// CLASS IMPLEMENTATION:  TSMJDemModInTank.cc
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
 * AUTHOR: Ryan Martin
 * CONTACT: 
 * FIRST SUBMISSION: Thu Feb  4 17:20:54 EST 2010
 * 
 * REVISION:
 *
 * 02-14-2011, Created, R. Martin
 * 06-13-2011, Added stuff to make variable sized modules, R. Martin
 * 06-15-2011, Added changes to have a steel tank, R. Martin
 * 03-12-2012, Added parallel world stuff, R. Martin
 * 03-14-2012, Added height of world volume member for messenger, R.Martin
 * 07-30-2012, Added functions to set string radial clearance and radial
 *             spacing between strings, K. Nguyen
 */
//---------------------------------------------------------------------------//

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"


#include "tonscalegeometry/TSMJDemModInTank.hh"
#include "tonscalegeometry/TSCylinderShieldTank.hh"
#include "tonscalegeometry/TSLabModuleBox.hh"
#include "tonscalegeometry/TSVisAttributes.hh"

//#include "mjdemonstrator/MJGeometryDemonstrator.hh" 
#include "mjdemonstrator/MJVDemoPart.hh"
#include "mjdemonstrator/MJDemo7StringAssembly.hh"
#include "mjdemonstrator/MJDemoStringAssembly.hh"
#include "mjdemonstrator/MJDemoCrystalAssembly.hh"
#include "mjdemonstrator/MJDemoBottomThermalShieldAssembly.hh"
#include "mjdemonstrator/MJDemoRadShieldAssembly.hh"
#include "tonscalegeometry/TSCryostatAssembly.hh"
//#include "mjdemonstrator/MJDemoThermosiphonAssembly.hh"
#include "mjdemonstrator/MJDemoCrystalAssembly.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"
#include "G4VisAttributes.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

TSMJDemModInTank::TSMJDemModInTank(G4String version): 
MGGeometryDetector(version)
{
  fCylinderTank = new TSCylinderShieldTank();
  fLabModule = new TSLabModuleBox();
  fParallelWorld=NULL;
  fnModules=1;
  fCleranceBetweenModules=10*cm;

  fHeightOfWorldVolume=-1.0;
  fnCrystalsPerString=5;
  fModuleType="MJDemonstrator";
  fCrystalAssemblyType="MJDemonstrator";	
  fCrystalDiameter=77.*mm;
  fCrystalHeight=33.*mm;
  fStringRadialClearance=5.*mm;       // FIXME: Added by K. Nguyen  
  fRadialSpacingBetweenStrings=5.*mm; // FIXME: Added by K. Nguyen
  
  fTankIsSensitive=false;
  fTankVerticalOffset=0;
  fG4Messenger=new TSMJDemModInTankMessenger(this);
}

// -----------------------------------------------------------------------

TSMJDemModInTank::~TSMJDemModInTank()
{
  delete fCylinderTank;
  delete fLabModule;
  delete[] fModules;
  delete fG4Messenger;
}

// -----------------------------------------------------------------------

MGGeometryParallelWorld* TSMJDemModInTank::GimmeParallelWorld()
{
  if(fParallelWorld){
    MGLog(warning) << "parallel world already exists!" << endlog;
  } else {
    MGLog(routine) << "creating parallel world" << endlog;
    fParallelWorld = new TSMJDemModInTankParallelWorld();
  }
  return fParallelWorld;
}

// -----------------------------------------------------------------------

void TSMJDemModInTank::ConstructDetector()
{
  if(fHeightOfWorldVolume<0)fHeightOfWorldVolume=1.5*fLabModule->GetLabModuleBoxHeight();
  
  G4Material *vacuum = G4Material::GetMaterial("Vacuum");
  G4Box* exptBox = new G4Box("exptBox", 1.5*fLabModule->GetLabModuleBoxLength(),
                             1.5*fLabModule->GetLabModuleBoxWidth(),
                             fHeightOfWorldVolume);
  
  G4VisAttributes* MagVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0)); // magenta
  MagVisAtt->SetForceWireframe( true );
  theDetectorLogical = new G4LogicalVolume(exptBox, vacuum, "theDetectorLogical");
  theDetectorLogical->SetVisAttributes(MagVisAtt);
  
  //Build the lab module and shielding tank, with a vacuum "clearance" volume
  //to hold a 'Module'
  G4RotationMatrix* rotationshield = new G4RotationMatrix();
  
  fCylinderTank->Construct();
  fLabModule->Construct();
  
  //MGLog(routine)<<"MADE IT HERE!!!!!!!!!!!!!!!!!!!!!!"<<endl;
  
  G4double TankOriginZ = -0.5*fLabModule->GetLabSpaceBoxHeight()+0.5*fCylinderTank->GetTankClearanceHeight()+fTankVerticalOffset;
  new G4PVPlacement(rotationshield, G4ThreeVector(0,0,0),fLabModule->GetRockBoxLogicalVolume(),
                    "labmodulephysical",theDetectorLogical,false,0,false);
  new G4PVPlacement(rotationshield, G4ThreeVector(0,0,TankOriginZ), fCylinderTank->GetTankWallLogical(),
                    "shieldtankwallphysical", fLabModule->GetLabSpaceLogicalVolume(), false, 0, false);
  
  fModules = new TSModule* [fnModules];
  
  char name[100];
  //Build and initialize the modules
  for(int i=0;i<fnModules;i++){
    
    sprintf(name,"DetectorArray_%.3d",i+1);
    fModules[i] = new TSModule(name);
    fModules[i]->SetnCrystalsPerString(fnCrystalsPerString);
    fModules[i]->SetModuleType(fModuleType);
    fModules[i]->SetCrystalAssemblyType(fCrystalAssemblyType);	
    fModules[i]->SetCrystalDiameter(fCrystalDiameter);
    fModules[i]->SetCrystalHeight(fCrystalHeight);
    fModules[i]->SetStringRadialClearance(fStringRadialClearance); // FIXME: Added by K. Nguyen
    fModules[i]->SetRadialSpacingBetweenStrings(fRadialSpacingBetweenStrings); // FIXME: Added by K. Nguyen
    
    fModules[i]->Initialize();//intialize the detector array (with cryostat) so that the size for the clearance can be determined
  }
  
  //Place the modules
  G4Tubs** cryoclearancetub = new G4Tubs* [fnModules];
  G4LogicalVolume** cryoclearancelogical = new G4LogicalVolume* [fnModules];
  
  G4ThreeVector** moduleposition = new G4ThreeVector* [fnModules];
  G4RotationMatrix** modulerotation = new G4RotationMatrix* [fnModules];
  G4ThreeVector**    cryoclearanceposition = new G4ThreeVector* [fnModules];	
  G4RotationMatrix** cryoclearancerotation = new G4RotationMatrix* [fnModules];	
  G4double cryoclearanceradius=0.;
  G4double cryoclearanceangle=0.;
  
  if(fnModules == 2){
    cryoclearanceradius=fModules[0]->GetCryostatRadialClearance()+0.5*fCleranceBetweenModules;
    cryoclearanceangle=180*deg;	
  }
  if(fnModules == 3){
    cryoclearanceradius=(fModules[0]->GetCryostatRadialClearance()+0.5*fCleranceBetweenModules)/cos(30*deg);
    cryoclearanceangle=120*deg;	
  }	
  if(fnModules == 4){
    cryoclearanceradius=sin(45*deg)*(2*fModules[0]->GetCryostatRadialClearance()+fCleranceBetweenModules);
    cryoclearanceangle=90*deg;	
  }		
  
  if(cryoclearanceradius+fModules[0]->GetCryostatRadialClearance() > fCylinderTank->GetTankClearanceRadius()){
    MGLog(error) << "Detector array will not fit in this tank!!!" << endl;
  }

  for(int i=0;i<fnModules;i++){		
    sprintf(name,"CryoCleranceTub_%.3d",i+1);
    cryoclearancetub[i] = new G4Tubs(name,0,fModules[i]->GetCryostatRadialClearance(),
				     0.5*fModules[i]->GetCryostatLengthClearance(),0,2*pi);
    sprintf(name,"CryoCleranceLogical_%.3d",i+1);		
    cryoclearancelogical[i] = new G4LogicalVolume(cryoclearancetub[i], vacuum, name); 
    cryoclearancelogical[i]->SetVisAttributes(new TSVisAttributesWireFrame("white"));
    
    cryoclearanceposition[i] = new G4ThreeVector(cryoclearanceradius*cos(i*cryoclearanceangle), cryoclearanceradius*sin(i*cryoclearanceangle), 0);
    cryoclearancerotation[i] = new G4RotationMatrix();	
    
    sprintf(name,"CryoClerancePhysical_%.3d",i+1);		
    new G4PVPlacement(cryoclearancerotation[i], *(cryoclearanceposition[i]),cryoclearancelogical[i],
                      name,fCylinderTank->GetTankContentLogical(),false,0,false);
    
    moduleposition[i] = new G4ThreeVector(0, 0, 0);
    modulerotation[i] = new G4RotationMatrix();	
    fModules[i]->Place(moduleposition[i],modulerotation[i] ,cryoclearancelogical[i]);
  }
  
  //MGLog(routine)<<"MADE IT HERE!!!!!!!!!!!!!!!!!!!!!!"<<endl;

  if(fParallelWorld){
    fParallelWorld->SetOrigin(0.,0.,0.);
    fParallelWorld->SetTankOriginZ(TankOriginZ);
    fParallelWorld->SetLabModuleSpaceDimensions(fLabModule->GetLabSpaceBoxLength(),fLabModule->GetLabSpaceBoxWidth(),
                                                fLabModule->GetLabSpaceBoxHeight());
    fParallelWorld->SetShotcreteDepth(fLabModule->GetShotcreteDepth());
    fParallelWorld->SetRockDepth(fLabModule->GetRockDepth()); 
    fParallelWorld->SetTankDimensions(fCylinderTank->GetTankClearanceRadius(),fCylinderTank->GetTankClearanceHeight());
    if(fnModules > 1){
      fParallelWorld->SetModuleClearanceDimensions(2.*cryoclearanceradius,fModules[0]->GetCryostatLengthClearance());
    }
    else{
      fParallelWorld->SetModuleClearanceDimensions(fModules[0]->GetCryostatRadialClearance(),fModules[0]->GetCryostatLengthClearance());
    }
  }

}

// -----------------------------------------------------------------------

int TSMJDemModInTank::GetSensitiveVolumeID(const string& volName)
{
  int sensVolID = 0;
  
  //If the shield tank is sensitive, it returns its ID
  size_t found = volName.find("tankphysical");
  if(found != string::npos && fTankIsSensitive) return 8000000; 
  
  found = volName.find("Active");
  if(found == string::npos) return 0;
  
  G4PhysicalVolumeStore *storePtr = G4PhysicalVolumeStore::GetInstance();
  G4VPhysicalVolume* pVol = storePtr->GetVolume(volName, false);
  G4String material = pVol->GetLogicalVolume()->GetMaterial()->GetName();
  
  if(material == "Germanium-Enr" || material == "Germanium-Nat" || material == "Germanium"){
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
  
  if(material == "Scintillator-Pla"){
    sensVolID = 9000000;
  }  
  
  return sensVolID;
}

// -----------------------------------------------------------------------

