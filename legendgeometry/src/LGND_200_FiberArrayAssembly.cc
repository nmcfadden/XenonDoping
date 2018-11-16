//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
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
// $Id: MGcodeTemplate.cc,v 1.1 2004-12-09 08:58:35 pandola Exp $
//
// CLASS IMPLEMENTATION:  LGND_200_FiberArrayAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Neil McFadden
 * CONTACT: nmcfadde@unm.edu
 * FIRST SUBMISSION: 
 *
 * REVISION:
 *
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SubtractionSolid.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "legendgeometry/LGND_200_FiberArrayAssembly.hh"
#include "legendgeometry/LGND_FiberCore.hh"
#include "legendgeometry/LGND_FiberCladding.hh"
#include "legendgeometry/LGND_FiberWLSCoating.hh"
#include "legendgeometry/LGND_Part.hh"
#include "legendgeometry/LGND_FiberSiPM.hh"

#include "legendgeometry/LGND_200_OpticalSurfaces.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;

LGND_200_FiberArrayAssembly::LGND_200_FiberArrayAssembly(G4String name, G4String serialNumber, G4double length,G4String shape) :
   fLength(length*mm), fName(name), fShape(shape)
{
  //intialize parts so that dimensions and strings can be passed in Place()
  fFiberCore = new LGND_FiberCore(fName+"FiberCore","000-001",fLength,fShape);
  fFiberCladdingInner = new LGND_FiberCladding(fName+"FiberCladdingInner","000-002",fLength,fShape,"PMMA");
  fFiberCladdingOuter = new LGND_FiberCladding(fName+"FiberCladdingOuter","000-002",fLength,fShape,"Fluor-acrylic");
  fFiberWLSCoating = new LGND_FiberWLSCoating(fName+"FiberWLSCoating","000-003",fLength,fShape);
  //note SiPM have different length than fibers
  fFiberSiPM = new LGND_FiberSiPM(fName+"FiberSiPM","000-004",0.1*mm,fShape);
  fFiberReflector = new LGND_FiberSiPM(fName+"FiberRelfector","000-005",0.1*mm,fShape);
}


LGND_200_FiberArrayAssembly::~LGND_200_FiberArrayAssembly()
{
  UnallocateParts();
}

void LGND_200_FiberArrayAssembly::AllocateParts()
{
}

void LGND_200_FiberArrayAssembly::UnallocateParts()
{
  delete fFiberCore;
  delete fFiberCladdingInner;
  delete fFiberCladdingOuter;
  delete fFiberWLSCoating;
  delete fFiberSiPM;
  delete fFiberReflector;
}

void LGND_200_FiberArrayAssembly::Place(G4ThreeVector* assemPosition,G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  LGND_200_OpticalSurfaces *opticalSurfaces = new LGND_200_OpticalSurfaces();

  //Place Fiber Array
  G4double deltaAngle = 0.; 
  G4double rotationAngle = 0;  
  G4double placement_radius = 0.295*m;//intial guess
  //TODO-what is the real spacing?
  G4double fiber_spacing = 0.;
  G4ThreeVector* fiberPosition = new G4ThreeVector(0);
  if(fFiberCore->GetShape() == "circle"){
    //cord between two cirlces of fiber is radius1+radius2 or 2*radius if circles are the same
    //cord = 2*R*sin(theta/2)
    //Also FibberCladding is one class to GetRadius returns total thickness
    fiber_spacing = 2.0*(fFiberCore->GetRadius() + fFiberCladdingInner->GetRadius() + fFiberWLSCoating->GetRadius() );
    //G4cout<<fiber_spacing<<"  !!!!!!!!!!!!!!!"<<fFiberCore->GetRadius()<<" "<<fFiberCladding->GetRadius()<<" "<<fFiberWLSCoating->GetRadius()<<G4endl;
    deltaAngle = 2.*asin(fiber_spacing/(2.*placement_radius));
    G4int nFibers =floor(2*pi/deltaAngle);
    //make sure that the last fiber does not overlap with the first
    deltaAngle = 2*pi/nFibers;
    placement_radius = fiber_spacing/(2.*sin(deltaAngle/2.));
    fiberPosition = new G4ThreeVector(placement_radius,0.*m,0.*m);
    fiberPosition->setX(fiberPosition->x() + assemPosition->x());
    fiberPosition->setY(fiberPosition->y() + assemPosition->y());
    fiberPosition->setZ(fiberPosition->z() + assemPosition->z());
    MGLog(routine) << "placing "<< nFibers <<" fibers" << " at angle "<<deltaAngle<<endlog;
  }
  else if (fFiberCore->GetShape() == "square"){
    fiber_spacing = 2.0*(fFiberCore->GetRadius() + fFiberCladdingInner->GetRadius() + fFiberWLSCoating->GetRadius() );
    //deltaAngle = 4.*asin(sqrt(2)*fiber_spacing/(2*placement_radius));
    deltaAngle = 2.*asin(sqrt(2)*fiber_spacing/(2*placement_radius));
    G4int nFibers =floor(2*pi/deltaAngle);
    //make sure that the last fiber does not overlap with the first
    deltaAngle = 2*pi/nFibers;
    //placement_radius = sqrt(2)*fiber_spacing/(2*sin(deltaAngle/4.));
    placement_radius = sqrt(2)*fiber_spacing/(2*sin(deltaAngle/2.));
    fiberPosition = new G4ThreeVector(placement_radius,0.*m,0.*m);
    fiberPosition->setX(fiberPosition->x() + assemPosition->x());
    fiberPosition->setY(fiberPosition->y() + assemPosition->y());
    fiberPosition->setZ(fiberPosition->z() + assemPosition->z());
    MGLog(routine) << "placing "<< nFibers <<" fibers" << " at angle "<<deltaAngle<<endlog;

  }
  
  if(fiber_spacing == 0){
    MGLog(fatal)<< "fiber spacing is zero! Shape is "<<fFiberCore->GetShape()<<"..."<<endlog;
    return;
  }

    //
  int num = 0;
  do{
    //No copies, each solid has to have an individual name
    //creating unique names
    fFiberCore = new LGND_FiberCore(fName+"FiberCore_"+to_string(num),"000-001",fLength,fShape);
    fFiberCladdingInner = new LGND_FiberCladding(fName+"FiberCladdingInner_"+to_string(num),"000-002",fLength,fShape,"PMMA");
    fFiberCladdingOuter = new LGND_FiberCladding(fName+"FiberCladdingOuter_"+to_string(num),"000-002",fLength,fShape,"Fluor-acrylic");
    fFiberWLSCoating = new LGND_FiberWLSCoating(fName+"FiberWLSCoating_"+to_string(num),"000-003",fLength,fShape);
    fFiberSiPM = new LGND_FiberSiPM(fName+"FiberSiPM_"+to_string(num),"000-004",0.1*mm,fShape);
    fFiberReflector = new LGND_FiberSiPM(fName+"FiberReflector_"+to_string(num),"000-005",0.1*mm,fShape);
    //for vis
    /*
    if(num%10 == 0){
    G4cout<<"/vis/scene/add/volume "<<fName+"FiberCore_"+to_string(num)<<endl;
    G4cout<<"/vis/scene/add/volume "<<fName+"FiberCladdingInner_"+to_string(num)<<endl;
    G4cout<<"/vis/scene/add/volume "<<fName+"FiberCladdingOuter_"+to_string(num)<<endl;
    G4cout<<"/vis/scene/add/volume "<<fName+"FiberWLSCoating_"+to_string(num)<<endl;
    G4cout<<"/vis/scene/add/volume "<<fName+"FiberSiPM_"+to_string(num)<<endl;
    G4cout<<"/vis/scene/add/volume "<<fName+"FiberReflector_"+to_string(num)<<endl;
    }
    */
    num++;
    if(num > 1e5){
      MGLog(error)<<" Stuck in fiber placement loop"<<endlog;
      MGLog(fatal)<<endlog;
    }
    G4RotationMatrix* rotationFiber = new G4RotationMatrix();
    //rotation is for square fibers to create maximum coverage
    //keeping it for circular fibers too since rotating a circle does nothing
    rotationFiber->rotateZ(-rotationAngle-pi/4);

    fFiberWLSCoating->Place(fiberPosition,rotationFiber,motherLogical);
    G4VPhysicalVolume* wlsPhysical = fFiberWLSCoating->GetPhysicalVolume();
    new G4LogicalSkinSurface("fiberWLSLogicalSkinSurface"+to_string(num),wlsPhysical->GetLogicalVolume(),opticalSurfaces->getWLSSurface());

    fFiberCladdingInner->Place(fiberPosition,rotationFiber,motherLogical);
    G4VPhysicalVolume* claddingInnerPhysical = fFiberCladdingInner->GetPhysicalVolume();
    //new G4LogicalSkinSurface("fiberCladdingInnerLogicalSkinSurface"+to_string(num),claddingInnerPhysical->GetLogicalVolume(),opticalSurfaces->getFiberCladdingSurface());
    
    fFiberCladdingOuter->Place(fiberPosition,rotationFiber,motherLogical);
    G4VPhysicalVolume* claddingOuterPhysical = fFiberCladdingOuter->GetPhysicalVolume();
    //new G4LogicalSkinSurface("fiberCladdingOuterLogicalSkinSurface"+to_string(num),claddingOuterPhysical->GetLogicalVolume(),opticalSurfaces->getFiberCladdingSurface());

    fFiberCore->Place(fiberPosition,rotationFiber,motherLogical);
    G4VPhysicalVolume* corePhysical = fFiberCore->GetPhysicalVolume();
    //new G4LogicalSkinSurface("fiberCoreLogicalSkinSurface"+to_string(num),corePhysical->GetLogicalVolume(),opticalSurfaces->getFiberCoreSurface());
  
    G4ThreeVector * r = fiberPosition;
//    corePhysical->CheckOverlaps(1000000, 0, true);
    r->setZ(r->getZ() + fLength +fFiberSiPM->GetLength());
    fFiberSiPM->Place(r,rotationFiber,motherLogical);
    G4VPhysicalVolume* SiPMPhysical = fFiberSiPM->GetPhysicalVolume();

    new G4LogicalBorderSurface("SiPM_core_logical_border"+to_string(r->getX()),corePhysical,SiPMPhysical,opticalSurfaces->getSiPMSurface());
    new G4LogicalBorderSurface("SiPM_claddingInner_logical_border"+to_string(r->getX()),claddingInnerPhysical,SiPMPhysical,opticalSurfaces->getSiPMSurface());
    new G4LogicalBorderSurface("SiPM_claddingOuter_logical_border"+to_string(r->getX()),claddingOuterPhysical,SiPMPhysical,opticalSurfaces->getSiPMSurface());
    new G4LogicalBorderSurface("SiPM_TPB_logical_border"+to_string(r->getX()),wlsPhysical,SiPMPhysical,opticalSurfaces->getSiPMSurface());

    r->setZ(-1.*r->getZ());
    fFiberReflector->Place(r,rotationFiber,motherLogical);
    G4VPhysicalVolume* reflectorPhysical = fFiberReflector->GetPhysicalVolume();

    new G4LogicalBorderSurface("Reflector_core_logical_border"+to_string(r->getX()),corePhysical,reflectorPhysical,opticalSurfaces->getFiberReflSurface());
    new G4LogicalBorderSurface("Reflector_claddingInner_logical_border"+to_string(r->getX()),claddingInnerPhysical,reflectorPhysical,opticalSurfaces->getFiberReflSurface());
    new G4LogicalBorderSurface("Reflector_claddingOuter_logical_border"+to_string(r->getX()),claddingOuterPhysical,reflectorPhysical,opticalSurfaces->getFiberReflSurface());
    new G4LogicalBorderSurface("Reflector_tpb_logical_border"+to_string(r->getX()),wlsPhysical,reflectorPhysical,opticalSurfaces->getFiberReflSurface());
    r->setZ(0.);
    rotationAngle += deltaAngle;
    fiberPosition->setX(placement_radius*cos(rotationAngle));fiberPosition->setY(placement_radius*sin(rotationAngle));fiberPosition->setZ(0.);
    fiberPosition->setX(fiberPosition->x() + assemPosition->x());
    fiberPosition->setY(fiberPosition->y() + assemPosition->y());
    fiberPosition->setZ(fiberPosition->z() + assemPosition->z());
    MGLog(debugging)<<"core position"<<fFiberCore->GetPartPosition()<<"... ("<<assemPosition->getX()<<","<<assemPosition->getY()<<","<<assemPosition->getZ()<<")"<<endlog;
  } while (rotationAngle < 2.*pi);
}
