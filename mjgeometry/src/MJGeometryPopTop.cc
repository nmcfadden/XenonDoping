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
/**                                                            
 * $Id: MJheaderTemplate.hh,v 1.3 2005-02-15 23:26:55 akbarm Exp $
 *      
 * CLASS DECLARATION:  MJGeometryPopTop.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *Geometry for simulating an ORTEC detector in the PopTop cryostat
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ryan Martin
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * 02-25-2013, Created, R. Martin
 *
 */
//---------------------------------------------------------------------------//
//

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
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4Polycone.hh"
#include "G4AffineTransform.hh"

#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryPopTop.hh"
#include "mjgeometry/MJGeometryOrtecCrystal.hh"

using namespace CLHEP;


MJGeometryPopTop::MJGeometryPopTop(G4String version): 
MGGeometryDetector(version)
{
  ConstructMaterials();
  fCrystal= new MJGeometryOrtecCrystal();
  fCrystalOffsetFromInsideOfVacuumShroud=4.0*mm;//Quantity G in the ORTEC spec sheets
  
  fVacuumShroudOuterRadius=0.5*95.2*mm;//from Brandon's measurement on 2/20/13
  fVacuumShroudRadialThickness=1.0*mm;//(95.2-91.4)/2.0;;//Quantity L in the ORTEC spec sheets is 1mm, disagrees with Brandon's measurement
  fVacuumShroudLength=224.0*mm;
  fVacuumShroudEndcapThickness=1.5*mm;
  
  fIRCupOffsetFromInsideOfVacuumShroud=5.0*mm;//guess
  fIRCupType=1;
  fIRCupThickness=0.8*mm;
  fIRCupOuterRadius=0.5*72.3*mm;
  fIRCupLength=130.0*mm;
  fIRCupBottomThickness=3.0*mm;
  fIRCupZplanes.resize(0);
  fIRCupROuterplanes.resize(0);
  fIRCupRInnerplanes.resize(0);
  fSetScrewHeightFromTopOfCup	= 20.0*mm;//totally random
  fSetScrewLength=5.0*mm;
  fSetScrewDiameter=5.0*mm;
  
  fMylarFoilThickness=0.03*mm;//from spec sheet
  
  fTeflonRingLength=6.0*mm;//guesses for the teflon ring that holds the mylar foil
  fTeflonRingThickness=0.8*mm;
  fTeflonRingDiameter=2.0*fIRCupOuterRadius;
  
  fContactPinDiameter=1.5*mm;//random
  fContactPinLength=21.0*mm;//This gets re-calculated based on the 76*mm
  fContactPinHolderDiameter=4.0*mm;//random
  fContactPinHolderLength=76*mm-21*mm;//illegible
  
  fSpaceBelowIRCup=6.5*mm;//from Brandon's measurement on 2/20/13
  fDiskBelowCupDiameter=76.2*mm;//from Brandon's measurement on 2/20/13
  fDiskBelowCupLength=2.5*mm;//from Brandon's measurement on 2/20/13
  fBlockBelowCupDiameter=76.2*mm;//from Brandon's measurement on 2/20/13
  fBlockBelowCupLength=41.5*mm;//from Brandon's measurement on 2/20/13
  fTeflonBelowCupDiameter=0.6*fIRCupOuterRadius*2.0;//just a guess
  fTeflonBelowCupLength=6.5*mm;//length to make it fill up the space
  
  fGoldFoilLength=25.0*mm;
  fGoldFoilWidth=15.0*mm;
  fGoldFoilThickness=0.5*mm;
  
  fSource = new MJGeometryAcrylicSource();
  fPlaceSource=true;
  fSourceX=0.*mm;
  fSourceY=0.*mm;
  fSourceZ=25.0*cm;
  
  //Check All dimensions for the source holder
  fPlaceSourceHolder=true;
  fSourceHolderRadialThickness=0.5*25.4*mm;//check this
  fSourceHolderRadialThicknessBottom=0.25*25.4*mm;//check this
  fSourceHolderSourceOpeningDiameter=0.75*25.4*mm;//Diameter of the hole that the source sees
  fSourceHolderLidThickness=0.5*25.4*mm;
  fSourceHolderSourceRecessDepth=5.0*mm;
  fSourceHolderLip=1.0*25.4*mm;//check
  fSourceHolderLength=25.0*cm+fSourceHolderLidThickness+fSourceHolderLip;//check
  
  fG4Messenger=new MJGeometryPopTopMessenger(this);
  
}

MJGeometryPopTop::~MJGeometryPopTop()
{
  
  
}

void MJGeometryPopTop::ConstructMaterials()
{
  G4String symbol;  
  G4double a, z, density;     
  G4int ncomponents,natoms;
  G4double fractionmass;
  
  G4Element* C = new G4Element("Carbon", symbol="C", z=6., a=12.01*g/mole);
  G4Element* H = new G4Element("Hydrogen",   symbol="H", z=1., a=1.007*g/mole);
  G4Element* O = new G4Element("Oxygen",   symbol="O", z=8., a=16.00*g/mole);
  G4Element* Fe = new G4Element("Iron",   symbol="Fe", z=26., a=55.85*g/mole);
  /*G4Material* Aluminum =*/ new G4Material("Aluminum", z=13.,  a=26.98*g/mole, density=2.70*g/cm3);
    cout<<"Aluminum added"<<endl;
  /*G4Material* Copper =*/ new G4Material("Copper", z=29.,  a=63.55*g/mole, density=8.96*g/cm3);
    cout<<"Copper added"<<endl;

  /*G4Material* Gold =*/ new G4Material("Gold", z=79.,  a=196.97*g/mole, density=19.30*g/cm3);
    cout<<"Gold added"<<endl;

    
  G4Material* steel = new G4Material("Steel", density=7.85*g/cm3, ncomponents=2);
    cout<<"Steel added"<<endl;

  steel->AddElement(C, fractionmass=0.01);
    cout<<"C added"<<endl;

  steel->AddElement(Fe, fractionmass=0.99);
    cout<<"Fe added"<<endl;

  
  G4Material* Mylar = new G4Material("Mylar", density=1.38*g/cm3, ncomponents=3);//Used BoPET formula, without the n
    cout<<"Mylar added"<<endl;

  Mylar->AddElement(C, natoms=10);
  Mylar->AddElement(H, natoms=8);
  Mylar->AddElement(O, natoms=4);

}

void MJGeometryPopTop::ConstructDetector()
{
	//Origin is the top center of the PopTop cryostat vacuum shroud (the outside of the vacuum shroud)

  G4Material *vacuum = G4Material::GetMaterial("Vacuum");
  G4Box* exptBox = new G4Box("exptBox",5*m,5*m,5*m);
  
  G4VisAttributes* MagVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0)); // magenta
  MagVisAtt->SetForceWireframe( true );
  theDetectorLogical = new G4LogicalVolume(exptBox, vacuum, "theDetectorLogical");
  theDetectorLogical->SetVisAttributes(MagVisAtt);
  
  //
  //Build the various components that go into the PopTop
  //
  G4String name = "VacuumShroud";
  
  //Vacuum shroud:
  G4Tubs* vacuumShroudTub = new G4Tubs("vacuumShroudTub",fVacuumShroudOuterRadius-fVacuumShroudRadialThickness,fVacuumShroudOuterRadius,0.5*fVacuumShroudLength,0,2*pi); 
  G4Tubs* vacuumShroudLid = new G4Tubs("vacuumShroudLid",0,fVacuumShroudOuterRadius,0.5*fVacuumShroudEndcapThickness,0,2*pi);
  G4UnionSolid* vacuumShroudSolid = new  G4UnionSolid(name+"_Solid",vacuumShroudTub,vacuumShroudLid,0,G4ThreeVector(0,0,0.5*(fVacuumShroudLength+fVacuumShroudEndcapThickness)));
  
  G4LogicalVolume *vacuumShroudLogical = new G4LogicalVolume(vacuumShroudSolid,G4Material::GetMaterial("Aluminum"),name+"_Logical");
  vacuumShroudLogical->SetVisAttributes(new G4VisAttributes(G4Colour(1,0,0.))); 
  G4ThreeVector  *VacuumShroudPosition = new G4ThreeVector(0,0,-(0.5*fVacuumShroudLength+fVacuumShroudEndcapThickness));
  G4RotationMatrix *VacuumShroudRotation = new G4RotationMatrix();	
  new G4PVPlacement(VacuumShroudRotation, *VacuumShroudPosition, vacuumShroudLogical,name+"_Physical",theDetectorLogical,false,0,false);
  
  //Detector cup
  name="IRCup";
  //Use Polycones to make the IR Cup. Set Z=0 at the origin, and build the cup downwards
  //!! THE ORIGIN OF THE CUP IS ABOVE THE CUP!!!
  G4double IRCupTolerance=0.001*mm;
  G4double IRCupInnerR=fIRCupOuterRadius-fIRCupThickness;
  G4double IRCupZTop=-(fVacuumShroudEndcapThickness+fIRCupOffsetFromInsideOfVacuumShroud);
  G4double IRCupZBottom=IRCupZTop-fIRCupLength;
  
  fIRCupZplanes.push_back(IRCupZTop);
  fIRCupROuterplanes.push_back(fIRCupOuterRadius);
  fIRCupRInnerplanes.push_back(IRCupInnerR);
  
  switch(fIRCupType){
    //Figure out how many rings need to be added
  case 1://The same type as PONaMa1 (need to specify this with cryostat serial numbers)
    AddIRCupRing(0.*mm,6.5*mm,0.4*mm,IRCupTolerance);
    AddIRCupRing(19.5*mm,6.5*mm,0.4*mm,IRCupTolerance);		
    AddIRCupRing(100.*mm,6.5*mm,0.4*mm,IRCupTolerance);
    fSetScrewHeightFromTopOfCup=19.5*mm+0.5*6.5*mm;			
    fTeflonRingDiameter=2.0*fIRCupOuterRadius+IRCupTolerance+0.4*mm;
    break;
    
  case 2://The same type as PONaMa2
    AddIRCupRing(0.*mm,6.5*mm,0.4*mm,IRCupTolerance);
    AddIRCupRing(13.0*mm,6.5*mm,0.4*mm,IRCupTolerance);	
    fSetScrewHeightFromTopOfCup=13.0*mm+0.5*6.5*mm;	
    fTeflonRingDiameter=2.0*fIRCupOuterRadius+IRCupTolerance+0.4*mm;
    break;
    
  case 3://The same type as P42538B
    AddIRCupRing(22.5*mm,6.5*mm,0.4*mm,IRCupTolerance);
    AddIRCupRing(84.0*mm,6.5*mm,0.4*mm,IRCupTolerance);	
    fSetScrewHeightFromTopOfCup=22.5*mm+0.5*6.5*mm;	
    fTeflonRingDiameter=2.0*fIRCupOuterRadius+IRCupTolerance;	
    break;
    
  default:
    AddIRCupRing(22.5*mm,6.5*mm,0.4*mm,IRCupTolerance);
    AddIRCupRing(84.0*mm,6.5*mm,0.4*mm,IRCupTolerance);	
    fSetScrewHeightFromTopOfCup=22.5*mm+0.5*6.5*mm;	
    break;
  }
  
  fIRCupZplanes.push_back(IRCupZTop-fIRCupLength+fIRCupBottomThickness+IRCupTolerance);
  fIRCupROuterplanes.push_back(fIRCupOuterRadius);
  fIRCupRInnerplanes.push_back(IRCupInnerR);
  
  fIRCupZplanes.push_back(IRCupZTop-fIRCupLength+fIRCupBottomThickness);
  fIRCupROuterplanes.push_back(fIRCupOuterRadius);
  fIRCupRInnerplanes.push_back(0.);
  
  fIRCupZplanes.push_back(IRCupZTop-fIRCupLength);
  fIRCupROuterplanes.push_back(fIRCupOuterRadius);
  fIRCupRInnerplanes.push_back(0.);
  
  G4int nIRCupPlanes=fIRCupZplanes.size();
  G4Polycone* IRCupSolidNoHoles = new G4Polycone("IRCupSolidNoHoles", 0.,2*pi,nIRCupPlanes,&fIRCupZplanes[0],&fIRCupRInnerplanes[0],&fIRCupROuterplanes[0]);
  
  //Hole for the contact pin assembly
  G4Tubs* IRCupHoleForContactPin= new G4Tubs("IRCupHoleForContactPin",0.,0.5*fContactPinHolderDiameter+IRCupTolerance,0.5*fIRCupBottomThickness+2.*IRCupTolerance,0.,2.*pi);
  G4SubtractionSolid* IRCupWithHoleForContactPin=new G4SubtractionSolid("IRCupWithHoleForContactPin",IRCupSolidNoHoles,IRCupHoleForContactPin,0,G4ThreeVector(0,0,IRCupZBottom+0.5*fIRCupBottomThickness));
  
  //Holes for set screws
  G4double SetScrewRadialPosition=fIRCupOuterRadius-0.5*fIRCupThickness;
  G4Tubs* SetScrewHole= new G4Tubs("SetScrewHole",0.,0.5*fSetScrewDiameter+IRCupTolerance,0.5*fSetScrewLength+IRCupTolerance,0.,2.*pi);
  
  G4double SetScrew1Angle=0.75*pi;
  G4ThreeVector  *SetScrew1Position = new G4ThreeVector(SetScrewRadialPosition*cos(SetScrew1Angle),SetScrewRadialPosition*sin(SetScrew1Angle),IRCupZTop-fSetScrewHeightFromTopOfCup);	
  G4RotationMatrix *SetScrew1Rotation1 = new G4RotationMatrix();	
  SetScrew1Rotation1->rotateY(90.0*deg);	
  G4AffineTransform* SetScrew1Affine1 = new G4AffineTransform(SetScrew1Rotation1,	G4ThreeVector(0,0,0));
  G4RotationMatrix *SetScrew1Rotation2 = new G4RotationMatrix();	
  SetScrew1Rotation2->rotateZ(45.0*deg);	
  G4AffineTransform* SetScrew1Affine2 = new G4AffineTransform(SetScrew1Rotation2,	G4ThreeVector(0,0,0));
  *SetScrew1Affine1 *= *SetScrew1Affine2;  
  G4RotationMatrix *SetScrew1Rotation=new G4RotationMatrix(SetScrew1Affine1->NetRotation());
  G4SubtractionSolid* IRCupWithHoleFor1SetScrew=new G4SubtractionSolid("IRCupWithHoleFor1SetScrew",IRCupWithHoleForContactPin,SetScrewHole,SetScrew1Rotation,*SetScrew1Position);
  
  G4double SetScrew2Angle=1.25*pi;
  G4ThreeVector  *SetScrew2Position = new G4ThreeVector(SetScrewRadialPosition*cos(SetScrew2Angle),SetScrewRadialPosition*sin(SetScrew2Angle),IRCupZTop-fSetScrewHeightFromTopOfCup);	
  G4RotationMatrix *SetScrew2Rotation1 = new G4RotationMatrix();	
  SetScrew2Rotation1->rotateY(90.0*deg);	
  G4AffineTransform* SetScrew2Affine1 = new G4AffineTransform(SetScrew2Rotation1,	G4ThreeVector(0,0,0));
  G4RotationMatrix *SetScrew2Rotation2 = new G4RotationMatrix();	
  SetScrew2Rotation2->rotateZ(315.0*deg);	
  G4AffineTransform* SetScrew2Affine2 = new G4AffineTransform(SetScrew2Rotation2,	G4ThreeVector(0,0,0));
  *SetScrew2Affine1 *= *SetScrew2Affine2;  
  G4RotationMatrix *SetScrew2Rotation=new G4RotationMatrix(SetScrew2Affine1->NetRotation());
  G4SubtractionSolid* IRCupWithHoleFor2SetScrew=new G4SubtractionSolid("IRCupWithHoleFor2SetScrew",IRCupWithHoleFor1SetScrew,SetScrewHole,SetScrew2Rotation,*SetScrew2Position);
  
  
  //Place the IR Cup
  G4SubtractionSolid* IRCupSolid=IRCupWithHoleFor2SetScrew;
  G4LogicalVolume *IRCupLogical = new G4LogicalVolume(IRCupSolid,G4Material::GetMaterial("Aluminum"),name+"_Logical");    
  IRCupLogical->SetVisAttributes(new G4VisAttributes(G4Colour(1,0,1))); 
  G4ThreeVector  *IRCupPosition = new G4ThreeVector(0,0,0);
  G4RotationMatrix *IRCupRotation = new G4RotationMatrix();	
  new G4PVPlacement(IRCupRotation, *IRCupPosition, IRCupLogical,name+"_Physical",theDetectorLogical,false,0,false);
  
  //Place the setscrews
  name="SetScrew";
  G4Tubs* SetScrewSolid= new G4Tubs("SetScrewSolid",0.,0.5*fSetScrewDiameter,0.5*fSetScrewLength,0.,2.*pi);
  G4LogicalVolume *SetScrewLogical = new G4LogicalVolume(SetScrewSolid,G4Material::GetMaterial("Steel"),name+"_Logical");    
  SetScrewLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0,0,1))); 	
  new G4PVPlacement(SetScrew1Rotation, *SetScrew1Position, SetScrewLogical,name+"1_Physical",theDetectorLogical,false,0,false);
  new G4PVPlacement(SetScrew2Rotation, *SetScrew2Position, SetScrewLogical,name+"2_Physical",theDetectorLogical,false,0,false);	
  
  
  
  //The Gold Foil, positioned at X=0, so that the set screws are opposite (the gold foil being on the bissector of the set screw positions)	
  name="GoldFoil";
  G4Tubs* GoldFoilSolid = new G4Tubs("GoldFoilSolid",IRCupInnerR-fGoldFoilThickness,IRCupInnerR,0.5*fGoldFoilLength,0,fGoldFoilWidth/IRCupInnerR); 
  G4LogicalVolume *GoldFoilLogical = new G4LogicalVolume(GoldFoilSolid,G4Material::GetMaterial("Gold"),name+"_Logical");    
  GoldFoilLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0,0.5,1))); 
  G4ThreeVector  *GoldFoilPosition = new G4ThreeVector(0,0,IRCupZTop-0.5*fGoldFoilLength);
  G4RotationMatrix *GoldFoilRotation = new G4RotationMatrix();	
  new G4PVPlacement(GoldFoilRotation, *GoldFoilPosition, GoldFoilLogical,name+"_Physical",theDetectorLogical,false,0,false);
  
  
  //The crystal
  G4double ZCrystalBottom=-(fVacuumShroudEndcapThickness+fCrystal->GetCrystalLength()+fCrystalOffsetFromInsideOfVacuumShroud);
  G4double ZCrystalTop=-(fVacuumShroudEndcapThickness+fCrystalOffsetFromInsideOfVacuumShroud);
  G4ThreeVector* crystalPosition = new G4ThreeVector(0, 0, ZCrystalBottom);
  G4RotationMatrix* crystalRotation = new G4RotationMatrix();	
  
  fCrystal->Place(crystalPosition,crystalRotation,theDetectorLogical);
  fCrystal->SaveCrystalGeomToFile("latestCrystalGeomUsed.dat");  
  fIRCupZplanes.resize(0);
  fIRCupROuterplanes.resize(0);
  
  //Contact Pin
  name="ContactPinAssembly";
  fContactPinHolderLength+=fSpaceBelowIRCup;
  fContactPinLength=(ZCrystalBottom+fCrystal->GetUsableCrystalDimpleHoleDepth())-(IRCupZBottom-fSpaceBelowIRCup)-fContactPinHolderLength;//this makes the pin the right length to stick into the detector dimple
  G4Tubs* ContactPin = new G4Tubs("ContactPin",0,0.5*fContactPinDiameter,0.5*fContactPinLength,0,2*pi); 
  G4Tubs* ContactPinHolder = new G4Tubs("ContactPinHolder",0,0.5*fContactPinHolderDiameter,0.5*fContactPinHolderLength,0,2*pi); 
  G4UnionSolid* ContactPinAssemblySolid = new  G4UnionSolid(name+"_Solid",ContactPinHolder,ContactPin,0,G4ThreeVector(0,0,0.5*(fContactPinLength+fContactPinHolderLength)));
  G4LogicalVolume *ContactPinAssemblyLogical = new G4LogicalVolume(ContactPinAssemblySolid,G4Material::GetMaterial("Copper"),name+"_Logical");
  ContactPinAssemblyLogical->SetVisAttributes(new G4VisAttributes(G4Colour(1,0,0.))); 
  G4ThreeVector  *ContactPinAssemblyPosition = new G4ThreeVector(0,0,IRCupZBottom+0.5*fContactPinHolderLength-fSpaceBelowIRCup);
  G4RotationMatrix *ContactPinAssemblyRotation = new G4RotationMatrix();	
  new G4PVPlacement(ContactPinAssemblyRotation, *ContactPinAssemblyPosition, ContactPinAssemblyLogical,name+"_Physical",theDetectorLogical,false,0,false);
  
  //Teflon piece below the cup
  name="TeflonBelowCup";
  G4Tubs* TeflonBelowCupSolid = new G4Tubs(name+"_Solid",0.5*fContactPinHolderDiameter+IRCupTolerance,0.5*fTeflonBelowCupDiameter,0.5*fTeflonBelowCupLength-2.0*IRCupTolerance,0,2*pi); 
  G4LogicalVolume *TeflonBelowCupLogical = new G4LogicalVolume(TeflonBelowCupSolid,G4Material::GetMaterial("Teflon"),name+"_Logical");    
  TeflonBelowCupLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.5,0.5,0.5))); 
  G4ThreeVector  *TeflonBelowCupPosition = new G4ThreeVector(0,0,IRCupZBottom-0.5*fTeflonBelowCupLength);
  G4RotationMatrix *TeflonBelowCupRotation = new G4RotationMatrix();	
  new G4PVPlacement(TeflonBelowCupRotation, *TeflonBelowCupPosition, TeflonBelowCupLogical,name+"_Physical",theDetectorLogical,false,0,false);
  
  //Mylar foil on top of the detector
  name="MylarFoil";
  G4Tubs* MylarFoilSolid = new G4Tubs(name+"_Solid",0.,fIRCupOuterRadius,0.5*fMylarFoilThickness,0,2*pi); 
  G4LogicalVolume *MylarFoilLogical = new G4LogicalVolume(MylarFoilSolid,G4Material::GetMaterial("Mylar"),name+"_Logical");    
  MylarFoilLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.5,0.5,0.5))); 
  G4ThreeVector  *MylarFoilPosition = new G4ThreeVector(0,0,(IRCupZTop>=ZCrystalTop ? IRCupZTop: ZCrystalTop)+0.5001*fMylarFoilThickness);
  G4RotationMatrix *MylarFoilRotation = new G4RotationMatrix();	
  new G4PVPlacement(MylarFoilRotation, *MylarFoilPosition, MylarFoilLogical,name+"_Physical",theDetectorLogical,false,0,false);
  
  //Teflon ring that holds the mylar
  name="TeflonRing";
  G4Tubs* TeflonRingSolid = new G4Tubs(name+"_Solid",0.5*fTeflonRingDiameter,0.5*fTeflonRingDiameter+fTeflonRingThickness,0.5*fTeflonRingLength,0,2*pi); 
  G4LogicalVolume *TeflonRingLogical = new G4LogicalVolume(TeflonRingSolid,G4Material::GetMaterial("Teflon"),name+"_Logical");    
  TeflonRingLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.5,0.5,0.5))); 
  G4ThreeVector  *TeflonRingPosition = new G4ThreeVector(0,0,IRCupZTop-0.5*fTeflonRingLength);
  G4RotationMatrix *TeflonRingRotation = new G4RotationMatrix();	
  new G4PVPlacement(TeflonRingRotation, *TeflonRingPosition, TeflonRingLogical,name+"_Physical",theDetectorLogical,false,0,false);
  
  //Electronics block below cup:
  name = "ElectronicsBlock";
  G4Tubs* ElectronicsBlockDisk = new G4Tubs("ElectronicsBlockDisk",0,0.5*fDiskBelowCupDiameter,0.5*fDiskBelowCupLength,0,2*pi); 
  G4Tubs* ElectronicsBlockHalfDisk = new G4Tubs("ElectronicsBlockHalfDisk",0,0.5*fBlockBelowCupDiameter,0.5*fBlockBelowCupLength,SetScrew1Angle,pi); 
  G4UnionSolid* ElectronicsBlock = new G4UnionSolid(name+"_Solid",ElectronicsBlockDisk,ElectronicsBlockHalfDisk,0,G4ThreeVector(0,0,-0.5*(fDiskBelowCupLength+fBlockBelowCupLength)));
  G4LogicalVolume *ElectronicsBlockLogical = new G4LogicalVolume(ElectronicsBlock,G4Material::GetMaterial("Aluminum"),name+"_Logical");
  ElectronicsBlockLogical->SetVisAttributes(new G4VisAttributes(G4Colour(1,0,0.))); 
  G4ThreeVector  *ElectronicsBlockPosition = new G4ThreeVector(0,0,IRCupZBottom-fSpaceBelowIRCup-0.5*fDiskBelowCupLength);
  G4RotationMatrix *ElectronicsBlockRotation = new G4RotationMatrix();	
  new G4PVPlacement(ElectronicsBlockRotation, *ElectronicsBlockPosition, ElectronicsBlockLogical,name+"_Physical",theDetectorLogical,false,0,false);
  
  
  //Place the source:
  if(fPlaceSource){
    G4ThreeVector* sourcePosition = new G4ThreeVector(fSourceX, fSourceY,fSourceZ);
    G4RotationMatrix* sourceRotation = new G4RotationMatrix();	
    fSource->Place(sourcePosition,sourceRotation,theDetectorLogical);
  }
  if(fPlaceSourceHolder){
    name="SourceHolder";
    G4double SourceHolderTol=0.001*mm;
    G4double SourceHolderZTop=25.0*cm+fSourceHolderSourceRecessDepth;
    G4double SourceHolderOuterR=fSourceHolderRadialThicknessBottom+fVacuumShroudOuterRadius;
    G4double SourceHolderThicknessBelowSource=fSourceHolderLidThickness-fSourceHolderSourceRecessDepth;
    vector<double> SourceHolderZPlanes, SourceHolderInnerRPlanes, SourceHolderOuterRPlanes;
    G4double currentZ;
    
    //Start from the top, use the origin of the PopTop (top of the vacuum shroud)
    currentZ=SourceHolderZTop;
    SourceHolderZPlanes.push_back(currentZ);
    SourceHolderInnerRPlanes.push_back(0.5*fSource->GetSourceDiameter()+SourceHolderTol);
    SourceHolderOuterRPlanes.push_back(SourceHolderOuterR);
    
    currentZ=currentZ-fSourceHolderSourceRecessDepth-SourceHolderTol;
    SourceHolderZPlanes.push_back(currentZ);
    SourceHolderInnerRPlanes.push_back(0.5*fSource->GetSourceDiameter()+SourceHolderTol);
    SourceHolderOuterRPlanes.push_back(SourceHolderOuterR);
    
    currentZ=currentZ-SourceHolderTol;
    SourceHolderZPlanes.push_back(currentZ);
    SourceHolderInnerRPlanes.push_back(0.5*fSourceHolderSourceOpeningDiameter);
    SourceHolderOuterRPlanes.push_back(SourceHolderOuterR);
    
    currentZ=currentZ-SourceHolderThicknessBelowSource-SourceHolderTol;
    SourceHolderZPlanes.push_back(currentZ);
    SourceHolderInnerRPlanes.push_back(0.5*fSourceHolderSourceOpeningDiameter);
    SourceHolderOuterRPlanes.push_back(SourceHolderOuterR);	
    
    currentZ=currentZ-SourceHolderTol;
    SourceHolderZPlanes.push_back(currentZ);
    SourceHolderInnerRPlanes.push_back(SourceHolderOuterR-fSourceHolderRadialThickness);
    SourceHolderOuterRPlanes.push_back(SourceHolderOuterR);	
    
    currentZ=SourceHolderTol;//top of the cryostat + epsilon
    SourceHolderZPlanes.push_back(currentZ);
    SourceHolderInnerRPlanes.push_back(SourceHolderOuterR-fSourceHolderRadialThickness);
    SourceHolderOuterRPlanes.push_back(SourceHolderOuterR);	
    
    currentZ=0;//top of the cryostat
    SourceHolderZPlanes.push_back(currentZ);
    SourceHolderInnerRPlanes.push_back(fVacuumShroudOuterRadius+SourceHolderTol);
    SourceHolderOuterRPlanes.push_back(SourceHolderOuterR);	
    
    currentZ=-fSourceHolderLip;//top of the cryostat
    SourceHolderZPlanes.push_back(currentZ);
    SourceHolderInnerRPlanes.push_back(fVacuumShroudOuterRadius+SourceHolderTol);
    SourceHolderOuterRPlanes.push_back(SourceHolderOuterR);	
    MGLog(routine)<<"Source holder geometry: "<<endl; 
    for(size_t i=0;i<SourceHolderZPlanes.size();i++){
      MGLog(routine)<< SourceHolderZPlanes.at(i)<<" "<<SourceHolderInnerRPlanes.at(i)<<" "<<SourceHolderOuterRPlanes.at(i)<<endl;
    }

    
    G4int nSHPlanes=SourceHolderZPlanes.size();
    G4Polycone* SourceHolderSolid = new G4Polycone(name+"_Solid", 0.,2*pi,nSHPlanes,&SourceHolderZPlanes[0],&SourceHolderInnerRPlanes[0],&SourceHolderOuterRPlanes[0]);
    G4LogicalVolume *SourceHolderLogical = new G4LogicalVolume(SourceHolderSolid,G4Material::GetMaterial("Teflon"),name+"_Logical");    
    SourceHolderLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.5,0.5,0.5))); 
    G4ThreeVector  *SourceHolderPosition = new G4ThreeVector(0,0,0);
    G4RotationMatrix *SourceHolderRotation = new G4RotationMatrix();	
    new G4PVPlacement(SourceHolderRotation, *SourceHolderPosition, SourceHolderLogical,name+"_Physical",theDetectorLogical,false,0,false);
    
  }
  
}



void MJGeometryPopTop::AddIRCupRing(G4double argDistanceFromTopOfCup, G4double argWidthOfRing,  G4double argThicknessOfRing, G4double argTolerance)
{
  G4double IRCupInnerR=fIRCupOuterRadius-fIRCupThickness;
  G4double IRCupZTop=-(fVacuumShroudEndcapThickness+fIRCupOffsetFromInsideOfVacuumShroud);
  
  fIRCupZplanes.push_back(IRCupZTop-argDistanceFromTopOfCup-argTolerance);
  fIRCupROuterplanes.push_back(fIRCupOuterRadius);
  fIRCupRInnerplanes.push_back(IRCupInnerR);
  
  fIRCupZplanes.push_back(IRCupZTop-argDistanceFromTopOfCup-2.0*argTolerance);
  fIRCupROuterplanes.push_back(fIRCupOuterRadius+argThicknessOfRing);
  fIRCupRInnerplanes.push_back(IRCupInnerR);
  
  fIRCupZplanes.push_back(IRCupZTop-argDistanceFromTopOfCup-argWidthOfRing+argTolerance);
  fIRCupROuterplanes.push_back(fIRCupOuterRadius+argThicknessOfRing);
  fIRCupRInnerplanes.push_back(IRCupInnerR);
  
  fIRCupZplanes.push_back(IRCupZTop-argDistanceFromTopOfCup-argWidthOfRing);
  fIRCupROuterplanes.push_back(fIRCupOuterRadius);
  fIRCupRInnerplanes.push_back(IRCupInnerR);
  
}

int MJGeometryPopTop::GetSensitiveVolumeID(const string& volName)
{
//Note that a volume must have the word "Active" in its physical name, or it will get ignored.
  int sensVolID = 0;
  
  size_t found = volName.find("Active");
  if(found == string::npos) return 0;
  
  G4PhysicalVolumeStore *storePtr = G4PhysicalVolumeStore::GetInstance();
  G4VPhysicalVolume* pVol = storePtr->GetVolume(volName, false);
  G4String material = pVol->GetLogicalVolume()->GetMaterial()->GetName();
  //MGLog(routine)<<"Found volume "<<volName.c_str()<<" made of material "<<material.c_str()<<endl;
  if(material == "Germanium-Enr" || material == "Germanium-Nat" || material == "Germanium" || material == "enrGe"){
    sensVolID = 1000000;
    //MGLog(routine)<<"returning sensitive volume ID "<<sensVolID<<endl;
  }  
  
  
  return sensVolID;
}






