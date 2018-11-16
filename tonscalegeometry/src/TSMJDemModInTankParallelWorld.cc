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
 * FIRST SUBMISSION: 2012
 * 
 * REVISION:
 *
 * 03-12-2012, Created, R. Martin
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
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"

#include "tonscalegeometry/TSMJDemModInTankParallelWorld.hh"
#include "tonscalegeometry/TSVisAttributes.hh"

#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"

using namespace std;

using namespace CLHEP;

TSMJDemModInTankParallelWorld::TSMJDemModInTankParallelWorld(): MGGeometryParallelWorld()
{
  fWorldVolume=NULL;
  fG4Messenger=new TSMJDemModInTankParallelWorldMessenger(this);
  // Center of the tank:
  fOriginX=0.;
  fOriginY=0.;
  fOriginZ=0.;
  fTankOriginZ=0.;

  //Size of the tank
  fTankRadius=0.;
  fTankHeight=0.;

  //Volume around the cryostats (boundary of inner most parallel world)
  fModuleClearanceRadius=0.;
  fModuleClearanceHeight=0.;

  //Size of the lab module (inner, without the shot crete):
  fLabModuleSpaceX=0.;
  fLabModuleSpaceY=0.;
  fLabModuleSpaceZ=0.;
  
  fShotcreteDepth=0.;
  fRockDepth=0.;

  fnTankWorlds=25;
  fnShotcreteWorlds=1;
  fnRockWorlds=1;
  fnLabSpaceWorlds=1;
}

// -----------------------------------------------------------------------

void TSMJDemModInTankParallelWorld::Construct()
{
  fWorldVolume = GetWorld();
  G4LogicalVolume* worldLogical = fWorldVolume->GetLogicalVolume();
  G4LogicalVolume* motherVolume = worldLogical;
  //G4VisAttributes* cyan = new G4VisAttributes(G4Colour(0.58, 0.0, 0.82, 1.0));

  char name[200],protoname[100];

  G4int nbox=0;
  G4int nlevels=1;
  //G4double boxlength=0.,boxwidth=0.,boxheight=0.;
  G4double boxlengthMax=0.,boxwidthMax=0.,boxheightMax=0.;
  G4double boxlengthMin=0.,boxwidthMin=0.,boxheightMin=0.;
  G4double dboxlength=0.,dboxwidth=0.,dboxheight=0.;
  G4double tolerance=5*mm;

  MGLog(routine)<<"Creating parallel world:"<<endl;
  MGLog(routine)<<"Origin for rock and shot crete volumes: "<<fOriginX/m<<" "<<fOriginY/m<<" "<<fOriginZ/m<<" m"<<endl;
  MGLog(routine)<<"Origin for tank volumes: "<<fOriginX/m<<" "<<fOriginY/m<<" "<<fTankOriginZ/m<<" m"<<endl;
  MGLog(routine)<<"Tank dimensions R, H "<<fTankRadius/m<<" "<<fTankHeight/m<<endl;
  MGLog(routine)<<"Cryo dimensions R, H "<<fModuleClearanceRadius/m<<" "<<fModuleClearanceHeight/m<<endl;
  MGLog(routine)<<"Number of rock volumes "<<fnRockWorlds<<endl;
  MGLog(routine)<<"Number of shotcrete volumes "<<fnShotcreteWorlds<<endl;
  MGLog(routine)<<"Number of lab space volumes "<<fnLabSpaceWorlds<<endl;
  MGLog(routine)<<"Number of tank volumes "<<fnTankWorlds<<endl;

  ////////////////////////////////////
  /////////Rock parallel World////////
  ////////////////////////////////////
  sprintf(protoname,"ParrallelRockBox");
  nbox=fnRockWorlds;

  boxlengthMax=fLabModuleSpaceX+2.*(fShotcreteDepth+fRockDepth)-tolerance;
  boxwidthMax=fLabModuleSpaceY+2.*(fShotcreteDepth+fRockDepth)-tolerance; 
  boxheightMax=fLabModuleSpaceZ+2.*(fShotcreteDepth+fRockDepth)-tolerance;

  boxlengthMin=fLabModuleSpaceX+2.*(fShotcreteDepth);
  boxwidthMin=fLabModuleSpaceY+2.*(fShotcreteDepth); 
  boxheightMin=fLabModuleSpaceZ+2.*(fShotcreteDepth);

  dboxlength=(boxlengthMax-boxlengthMin)/nbox;
  dboxwidth=(boxwidthMax-boxwidthMin)/nbox;
  dboxheight=(boxheightMax-boxheightMin)/nbox;

  for(G4int i = 0; i < nbox; i++){
    //start with the outer volume
    sprintf(name,"%s_box_%d",protoname,i);
    G4Box* parallelSolid = new G4Box(name, 
				     (boxlengthMax-i*dboxlength)/2.,
				     (boxwidthMax-i*dboxwidth)/2., 
				     (boxheightMax-i*dboxheight)/2.);
    // create logical volume
    sprintf(name,"%s_logical_%d",protoname,i);
    G4LogicalVolume* parallelLogical = new G4LogicalVolume(parallelSolid, 0, name, 0, 0, 0);
    parallelLogical->SetVisAttributes(new TSVisAttributesWireFrame("red"));
    //create physical
    sprintf(name,"%s_physical_%d",protoname,i);
    G4VPhysicalVolume* physicalVolume = new G4PVPlacement(0,
							  G4ThreeVector(fOriginX,fOriginY,fOriginZ),
							  parallelLogical,
							  name,
							  motherVolume, 0, 0); 

    SetImportanceValueForRegion(physicalVolume, pow(2.0, nlevels) );
    nlevels++;
    MGLog(routine) << "Created parallel volume "<<i<<" of "<<nbox<<" in "<<protoname<<" with importance "
                   << pow(2.0, nlevels-1)<<" in "<<name<<" from Z = "<<(fOriginZ-(boxheightMax-i*dboxheight)/2.)/m 
                   << " to Z = "<<(fOriginZ+(boxheightMax-i*dboxheight)/2. )/m<<endl;
    // set motherVolume to parallelLogical for the next placement
    motherVolume = parallelLogical;
  } // end loop over shield volumes

  ////////////////////////////////////
  /////////Shotcrete parallel World///
  ////////////////////////////////////
  sprintf(protoname,"ParallelShotcreteBox");
  nbox=fnShotcreteWorlds;

  boxlengthMax=fLabModuleSpaceX+2.*(fShotcreteDepth)-tolerance;
  boxwidthMax=fLabModuleSpaceY+2.*(fShotcreteDepth)-tolerance; 
  boxheightMax=fLabModuleSpaceZ+2.*(fShotcreteDepth)-tolerance;

  boxlengthMin=fLabModuleSpaceX;
  boxwidthMin=fLabModuleSpaceY; 
  boxheightMin=fLabModuleSpaceZ;

  dboxlength=(boxlengthMax-boxlengthMin)/nbox;
  dboxwidth=(boxwidthMax-boxwidthMin)/nbox;
  dboxheight=(boxheightMax-boxheightMin)/nbox;

  for(G4int i = 0; i < nbox; i++) {
    //start with the outer volume
    sprintf(name,"%s_box_%d",protoname,i);
    G4Box* parallelSolid = new G4Box(name, 
				     (boxlengthMax-i*dboxlength)/2.,
				     (boxwidthMax-i*dboxwidth)/2., 
				     (boxheightMax-i*dboxheight)/2.);
    // create logical volume
    sprintf(name,"%s_logical_%d",protoname,i);
    G4LogicalVolume* parallelLogical = new G4LogicalVolume(parallelSolid, 0, name, 0, 0, 0);
    parallelLogical->SetVisAttributes(new TSVisAttributesWireFrame("red"));

    //create physical
    sprintf(name,"%s_physical_%d",protoname,i);
    G4VPhysicalVolume* physicalVolume = new G4PVPlacement(0,
							  G4ThreeVector(fOriginX,fOriginY,fOriginZ),
							  parallelLogical,
							  name,
							  motherVolume, 0, 0); 

    // set volume importance as 2^i
    SetImportanceValueForRegion(physicalVolume, pow(2.0, nlevels) );
    nlevels++;
    MGLog(routine) << "Created parallel volume "<<i<<" of "<<nbox<<" in "<<protoname<<" with importance "
                   << pow(2.0, nlevels-1)<<" in "<<name<<" from Z = "<<(fOriginZ-(boxheightMax-i*dboxheight)/2.)/m 
                   << " to Z = "<<(fOriginZ+(boxheightMax-i*dboxheight)/2.)/m <<endl;
    // set motherVolume to parallelLogical for the next placement
    motherVolume = parallelLogical;
  } // end loop over shield volumes

  ////////////////////////////////////
  /////////Lab space parallel World///
  ////////////////////////////////////
  sprintf(protoname,"ParallelLabSpaceBox");
  nbox=fnLabSpaceWorlds;

  boxlengthMax=fLabModuleSpaceX-tolerance;
  boxwidthMax=fLabModuleSpaceY-tolerance; 
  boxheightMax=fLabModuleSpaceZ-tolerance;

  boxlengthMin=fTankRadius;
  boxwidthMin=fTankRadius; 
  boxheightMin=fTankHeight;

  dboxlength=(boxlengthMax-boxlengthMin)/nbox;
  dboxwidth=(boxwidthMax-boxwidthMin)/nbox;
  dboxheight=(boxheightMax-boxheightMin)/nbox;

  //if the origin of the tank is not the same as that of the lab module,
  //need to shift each volume
  G4double offsetZ=1./nbox*(fOriginZ-fTankOriginZ);

  for(G4int i = 0; i < nbox; i++) {
    //start with the outer volume
    sprintf(name,"%s_box_%d",protoname,i);
    G4Box* parallelSolid = new G4Box(name, 
				     (boxlengthMax-i*dboxlength)/2.,
				     (boxwidthMax-i*dboxwidth)/2., 
				     (boxheightMax-i*dboxheight)/2.);
    // create logical volume
    sprintf(name,"%s_logical_%d",protoname,i);
    G4LogicalVolume* parallelLogical = new G4LogicalVolume(parallelSolid, 0,
        name, 0, 0, 0);
    parallelLogical->SetVisAttributes(new TSVisAttributesWireFrame("red"));

    //create physical
    sprintf(name,"%s_physical_%d",protoname,i);
    G4ThreeVector origin;
    if(i==0) origin= G4ThreeVector(fOriginX,fOriginY,fOriginZ);
    else origin= G4ThreeVector(0,0,-offsetZ); 
    G4VPhysicalVolume* physicalVolume = new G4PVPlacement(0,
							  origin,
							  parallelLogical,
							  name,
							  motherVolume, 0, 0); 

    // set volume importance as 2^i
    SetImportanceValueForRegion(physicalVolume, pow(2.0, nlevels) );
    nlevels++;
    MGLog(routine) << "Created parallel volume "<<i<<" of "<<nbox<<" in "<<protoname<<" with importance "
                   << pow(2.0, nlevels-1)<<" in "<<name<<" from Z = "<< offsetZ<<" to Z = "<<offsetZ <<endl;
    // set motherVolume to parallelLogical for the next placement
    motherVolume = parallelLogical;
  } // end loop over shield volumes

  /////////////////////////////////////////////////////
  /////////Tank parallel World using Tubs//////////////
  /////////!! Uses offsetZ from previous volumes!!!////
  /////////////////////////////////////////////////////
  //*
  sprintf(protoname,"ParallelTankTub");
  nbox=fnTankWorlds;

  boxlengthMax=fTankRadius-tolerance;//use as radius!!!! 
  boxheightMax=fTankHeight-tolerance;

  boxlengthMin=fModuleClearanceRadius; 
  boxheightMin=fModuleClearanceHeight;

  dboxlength=(boxlengthMax-boxlengthMin)/nbox;
  dboxheight=(boxheightMax-boxheightMin)/nbox;

  for(int i = 0; i < nbox; i++) {
    //start with the outer volume
    sprintf(name,"%s_box_%d",protoname,i);
    G4Tubs* parallelSolid = new G4Tubs(name, 0,
         			       (boxlengthMax-i*dboxlength), 
				       (boxheightMax-i*dboxheight)/2.,0*deg,360.*deg);
    // create logical volume
    sprintf(name,"%s_logical_%d",protoname,i);
    G4LogicalVolume* parallelLogical = new G4LogicalVolume(parallelSolid, 0, name, 0, 0, 0);
    parallelLogical->SetVisAttributes(new TSVisAttributesSolid("red"));
    //parallelLogical->SetVisAttributes(cyan);
    //create physical
    sprintf(name,"%s_physical_%d",protoname,i);

    G4ThreeVector origin;
    if(i==0) origin=  G4ThreeVector(0,0,-offsetZ);
    else origin=  G4ThreeVector(0,0,0);

    G4VPhysicalVolume* physicalVolume = new G4PVPlacement(0,
							  origin,
							  parallelLogical,
							  name,
							  motherVolume, 0, 0); 

    // set volume importance as 2^i
    SetImportanceValueForRegion(physicalVolume, pow(2.0, nlevels) );
    nlevels++;
    MGLog(routine) << "Created parallel volume "<<i<<" of "<<nbox<<" in "<<protoname<<" with importance "
                   << pow(2.0, nlevels-1)<<" in "<<name<<endl<<" with R = "<<(boxlengthMax-i*dboxlength)/m
                   << "m and H = "<<(boxheightMax-i*dboxheight)/m<<"m centered at Z="<<fTankOriginZ/m<<"m"<<endl;
    // set motherVolume to parallelLogical for the next placement
    motherVolume = parallelLogical;
  } // end loop over shield volumes
  /**/

}

// -----------------------------------------------------------------------

