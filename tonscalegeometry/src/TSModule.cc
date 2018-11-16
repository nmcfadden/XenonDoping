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
// CLASS IMPLEMENTATION:  TSModule.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * Everything is placed relative to the origin of the cryoclearance logical that 
 * is passed (this is a G4Tubs based volume).
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ryan Martin
 * CONTACT: 
 * 
 * REVISION:
 *
 * 06-13-2011, Created, R. Martin
 * 07-30-2012, Added 1HEXRing and 2HEXRing array configurations, K. Nguyen
 *
 */

//---------------------------------------------------------------------------//

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"                                                                                                                                               
#include "G4RotationMatrix.hh"
#include "G4AffineTransform.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"

#include "tonscalegeometry/TSModule.hh"
#include "tonscalegeometry/TSStringAssembly.hh"
#include "tonscalegeometry/TSCrystalAssembly.hh"
#include "tonscalegeometry/TSVisAttributes.hh"

#include "mjdemonstrator/MJVDemoPart.hh"
#include "mjdemonstrator/MJDemo7StringAssembly.hh"
#include "mjdemonstrator/MJDemoBottomThermalShieldAssembly.hh"
#include "tonscalegeometry/TSCryostatAssembly.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"

#include <cmath>

using namespace CLHEP;

//---------------------------------------------------------------------------//

TSModule::TSModule(G4String arg)
{
  fModuleType=kMJDemonstrator;
  fCrystalAssemblyType="MJDemonstrator";
  fCrystalDiameter=77.*mm;
  fCrystalHeight=33.*mm;
	
  fName=arg;
	
  fnStrings=0;
  fStrings=NULL;	

  fEpsilon=0.1*mm;

  //Need to be determined after size of string
  fThermalShieldInnerRadius=0.;
  fThermalShieldLength=0.;
  fStringArrayClearanceRadius=0.;
  fStringArrayClearanceLength=0.;
  fCryostatInnerRadius=0.;
  fCryostatLength = 0.;

  //default values
  fStringLengthClearance=1.*mm;//extra length to envelope string
  fStringRadialClearance=5*mm;//envelope around string assembly (gets added to RadialSpacingBetweenStrings)
  fRadialSpacingBetweenStrings=5.*mm;
  fStringBottomToThermalClearance=10.*mm;	
  fThermalShieldThickness=1.*mm;
  fThermalToCryostatClearance=10.*mm;
  fThermalShieldBottomThickness=fThermalShieldThickness;
  fColdPlateThickness=1.*cm;
  fColdPlateVerticalClearance=5.*cm;
  fCryostatClearanceDistance=2.*mm;//clearance distance for vacuum volume that will surround the cryostat
  fCryostatThickness=2*mm;		
  fCryostatLidThickness=fCryostatThickness;
  fCryostatBottomThickness=fCryostatLidThickness;
}

// -----------------------------------------------------------------------

TSModule::~TSModule()
{	
  delete[] fStrings;
}

// -----------------------------------------------------------------------

void TSModule::Initialize()
{
  //Needs to be able to know how much clearance is needed before it is placed
  if(fModuleType == kMJDemonstrator){
    //these are only approximate, but give a good enough clearance
    fnStrings=7;
    fnCrystalsPerString=5;
    fCryostatInnerRadius=200.*mm;
    fCryostatLidThickness=2.*mm;
    fCryostatLength=481.*mm;
    fCryostatBottomThickness=2.*mm;
    fCryostatLidThickness=2.*mm;		
  }
  else{
    if(fModuleType == k1Ring){
      fnStrings=7;	
      fStrings = new TSStringAssembly* [fnStrings];
      char snumber[100];
      G4String sname;
      for(int i=0;i<fnStrings;i++){		
	sprintf(snumber,"%.3d",i+1);
	sname=fName+"_StringAssembly_"+snumber;
	fStrings[i] = new TSStringAssembly(sname,"867-5309");
	fStrings[i]->SetCrystalAssemblyType(fCrystalAssemblyType);
	fStrings[i]->SetnCrystals(fnCrystalsPerString);
	fStrings[i]->SetCrystalDiameter(fCrystalDiameter);
	fStrings[i]->SetCrystalHeight(fCrystalHeight);
	fStrings[i]->Initialize();
      }
      // *Set these for each string configuration //
      fStringArrayClearanceRadius=3.*(fStrings[0]->GetStringRadius()+fStringRadialClearance)+2.*fRadialSpacingBetweenStrings;
      // *************************************** //
    }
    if(fModuleType == k2Ring){
      fnStrings=7+12;	
      fStrings = new TSStringAssembly * [fnStrings];
      char snumber[100];
      G4String sname;
      for(int i=0;i<fnStrings;i++){		
	sprintf(snumber,"%.3d",i+1);
	sname=fName+"_StringAssembly_"+snumber;
	fStrings[i] = new TSStringAssembly(sname,"867-5309");
	fStrings[i]->SetCrystalAssemblyType(fCrystalAssemblyType);
	fStrings[i]->SetnCrystals(fnCrystalsPerString);
	fStrings[i]->SetCrystalDiameter(fCrystalDiameter);
	fStrings[i]->SetCrystalHeight(fCrystalHeight);				
	fStrings[i]->Initialize();
      }
      // *Set these for each string configuration //
      fStringArrayClearanceRadius=5.*(fStrings[0]->GetStringRadius()+fStringRadialClearance)+3.*fRadialSpacingBetweenStrings;
    }
    if(fModuleType == k1String){
      fnStrings=1;	
      fStrings = new TSStringAssembly* [fnStrings];
      char snumber[100];
      G4String sname;
      for(int i=0;i<fnStrings;i++){		
	sprintf(snumber,"%.3d",i+1);
	sname=fName+"_StringAssembly_"+snumber;
	fStrings[i] = new TSStringAssembly(sname,"867-5309");
	fStrings[i]->SetCrystalAssemblyType(fCrystalAssemblyType);
	fStrings[i]->SetnCrystals(fnCrystalsPerString);
	fStrings[i]->SetCrystalDiameter(fCrystalDiameter);
	fStrings[i]->SetCrystalHeight(fCrystalHeight);					
	fStrings[i]->Initialize();
      }
      // *Set these for each string configuration //
      fStringArrayClearanceRadius=fStrings[0]->GetStringRadius()+fStringRadialClearance;
    }	
    if(fModuleType == k2String){
      fnStrings=2;	
      fStrings = new TSStringAssembly* [fnStrings];
      char snumber[100];
      G4String sname;
      for(int i=0;i<fnStrings;i++){		
	sprintf(snumber,"%.3d",i+1);
	sname=fName+"_StringAssembly_"+snumber;
	fStrings[i] = new TSStringAssembly(sname,"867-5309");
	fStrings[i]->SetCrystalAssemblyType(fCrystalAssemblyType);
	fStrings[i]->SetnCrystals(fnCrystalsPerString);
	fStrings[i]->SetCrystalDiameter(fCrystalDiameter);
	fStrings[i]->SetCrystalHeight(fCrystalHeight);					
	fStrings[i]->Initialize();
      }
      // *Set these for each string configuration //
      fStringArrayClearanceRadius=2.*(fStrings[0]->GetStringRadius()+fStringRadialClearance)+0.5*fRadialSpacingBetweenStrings;
    }	

    // FIXME: Added by K. Nguyen
    if(fModuleType == k1HEXRing){
      fnStrings=7;	
      fStrings = new TSStringAssembly* [fnStrings];
      char snumber[100];
      G4String sname;
      for(int i=0;i<fnStrings;i++){		
	sprintf(snumber,"%.3d",i+1);
	sname=fName+"_StringAssembly_"+snumber;
	fStrings[i] = new TSStringAssembly(sname,"867-5309");
	fStrings[i]->SetCrystalAssemblyType(fCrystalAssemblyType);
	fStrings[i]->SetnCrystals(fnCrystalsPerString);
	fStrings[i]->SetCrystalDiameter(fCrystalDiameter);
	fStrings[i]->SetCrystalHeight(fCrystalHeight);
	fStrings[i]->Initialize();
      }
      // *Set these for each string configuration //
      fStringArrayClearanceRadius=3.*(fStrings[0]->GetStringRadius()+fStringRadialClearance)+2.*fRadialSpacingBetweenStrings;
      // *************************************** //
    }  
    if(fModuleType == k2HEXRing){
      fnStrings=7+12;	
      fStrings = new TSStringAssembly* [fnStrings];
      char snumber[100];
      G4String sname;
      for(int i=0;i<fnStrings;i++){		
	sprintf(snumber,"%.3d",i+1);
	sname=fName+"_StringAssembly_"+snumber;
	fStrings[i] = new TSStringAssembly(sname,"867-5309");
	fStrings[i]->SetCrystalAssemblyType(fCrystalAssemblyType);
	fStrings[i]->SetnCrystals(fnCrystalsPerString);
	fStrings[i]->SetCrystalDiameter(fCrystalDiameter);
	fStrings[i]->SetCrystalHeight(fCrystalHeight);				
	fStrings[i]->Initialize();
      }
      // *Set these for each string configuration //
      fStringArrayClearanceRadius=5.*(fStrings[0]->GetStringRadius()+fStringRadialClearance)+3.*fRadialSpacingBetweenStrings;
    }
		///
		//Add more string configurations here
	  //(...)
    ////

    //Now that the string configuration is initialized, one can determine the size of the cryostat
			
    fStringArrayClearanceLength=fStrings[0]->GetStringLength()+fStringLengthClearance;  
    
    fThermalShieldInnerRadius=fStringArrayClearanceRadius;
    fThermalShieldLength=fStringArrayClearanceLength+fStringBottomToThermalClearance;
    
    fCryostatInnerRadius=fThermalShieldInnerRadius + fThermalShieldThickness + fThermalToCryostatClearance;
    fCryostatLength = fThermalShieldBottomThickness + fThermalShieldLength + fColdPlateThickness + 
                      fColdPlateVerticalClearance +fThermalToCryostatClearance;
  }

}

// -----------------------------------------------------------------------

void TSModule::Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation, G4LogicalVolume *cryoclearancelogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
	
  if(fModuleType == kMJDemonstrator){
    //Place a MJDemonstrator module (7String assembly + Thermal shield + cryostat
    //assembly) into the "clearance" volume.
    MGLog(routine)<<"Building an array like MJDemonstrator"<<endl;
    TSCryostatAssembly* cryostat1 = new TSCryostatAssembly(fName+"_CryostatAssembly_001", "867-5309");
    G4ThreeVector* position2 = new G4ThreeVector(0, 0, 5.4*25.5*mm);
    G4RotationMatrix* rotation2 = new G4RotationMatrix();
    cryostat1->Place(position2, rotation2, cryoclearancelogical);
	  //
    MJDemoBottomThermalShieldAssembly* shield1 = new MJDemoBottomThermalShieldAssembly(fName+"_ThermalShieldAssembly_001", "867-5309");
    G4ThreeVector* position4 = new G4ThreeVector(0, 0, 3.874*25.5*mm);
    G4RotationMatrix* rotation4 = new G4RotationMatrix(); 
    shield1->Place(position4, rotation4, cryoclearancelogical);  /**/
    //
    MJDemo7StringAssembly* stringAssem1 = new MJDemo7StringAssembly(fName+"_DetectorArray_001", "867-5309");
    G4ThreeVector* position5 = new G4ThreeVector(0, 0, 4*25.4*mm);
    G4RotationMatrix* rotation5 = new G4RotationMatrix();
    stringAssem1->Place(position5, rotation5, cryoclearancelogical);/**/
  }
  else{
    //Uses different conventions to define a cryostat, thermal shield and clearance around all the parts, then arranges the strings
    G4double halfcryoclearance=0.5*GetCryostatLengthClearance();
    G4double StringTopZPosition =halfcryoclearance-fCryostatLidThickness-fColdPlateVerticalClearance-fColdPlateThickness;

    //top lid of thermal shield (=Cold plate), shifted up by epsilon
    G4Tubs *ColdPlateTub = new G4Tubs(fName+"_ColdPlateTub",0.,fThermalShieldInnerRadius+fThermalShieldThickness,0.5*fColdPlateThickness,0,2.*pi);
    G4LogicalVolume *ColdPlateLogical = new G4LogicalVolume(ColdPlateTub,G4Material::GetMaterial("Copper-EF"),fName+"_ColdPlateLogical");
    //ColdPlateLogical->SetVisAttributes(new TSVisAttributesSolid("red"));		
    ColdPlateLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0))); // FIXME: Changed color attribute from
                                                                                              //        red to copper, K. Nguyen  	
    new G4PVPlacement(0, G4ThreeVector(0,0,StringTopZPosition+0.5*fColdPlateThickness+fEpsilon),ColdPlateLogical,
                      fName+"_ColdPlatePhysical",cryoclearancelogical,false,0,false);

    //body of thermal shield
    G4Tubs *ThermalShieldTub = new G4Tubs(fName+"_ThermalShieldTub",fThermalShieldInnerRadius,
    fThermalShieldInnerRadius+fThermalShieldThickness, 0.5*fThermalShieldLength,0,2*pi);
    G4LogicalVolume* ThermalShieldLogical = new G4LogicalVolume(ThermalShieldTub,G4Material::GetMaterial("Copper-EF"),
		                                                fName+"_ThermalShieldLogical");
    //ThermalShieldLogical->SetVisAttributes(new TSVisAttributesWireFrame("red"));		
    ThermalShieldLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0))); // FIXME: Changed color attribute from
                                                                                                  //        red to copper, K. Nguyen	    
    new G4PVPlacement(0, G4ThreeVector(0,0,StringTopZPosition-0.5*fThermalShieldLength),ThermalShieldLogical,
                      fName+"_ThermalShieldPhysical",cryoclearancelogical,false,0,false);

    //bottom lid of thermal shield, shifted down by epsilon
    G4Tubs *ThermalShieldBottomTub = new G4Tubs(fName+"_ThermalShieldBottom",0.,fThermalShieldInnerRadius+fThermalShieldThickness,
                                                0.5*fThermalShieldBottomThickness,0,2.*pi);
    G4LogicalVolume* ThermalShieldBottomLogical = new G4LogicalVolume(ThermalShieldBottomTub,G4Material::GetMaterial("Copper-EF"),
		                                                      fName+"_ThermalShieldBottomLogical");
    //ThermalShieldBottomLogical->SetVisAttributes(new TSVisAttributesWireFrame("red"));		
    ThermalShieldBottomLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0)));	// FIXME: Changed color attribute from
                                                                                                        //        red to copper, K. Nguyen	
    new G4PVPlacement(0, G4ThreeVector(0,0,StringTopZPosition-fThermalShieldLength-0.5*fThermalShieldBottomThickness-fEpsilon),
                      ThermalShieldBottomLogical,fName+"_ThermalShieldBottomPhysical",cryoclearancelogical,false,0,false);

    //lid of cryostat, shifted up by epsilon
    G4Tubs *CryostatLidTub = new G4Tubs(fName+"_CryostatLid",0.,fCryostatInnerRadius+fCryostatThickness,0.5*fCryostatLidThickness,0,2.*pi);
    G4LogicalVolume* CryostatLidLogical = new G4LogicalVolume(CryostatLidTub,G4Material::GetMaterial("Copper-EF"),
		                                              fName+"_CryostatLidLogical");
    //CryostatLidLogical->SetVisAttributes(new TSVisAttributesWireFrame("red"));
    CryostatLidLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0))); // FIXME: Changed color attribute from
                                                                                                //        red to copper, K. Nguyen
    new G4PVPlacement(0, G4ThreeVector(0,0,0.5*fCryostatLength+0.5*fCryostatLidThickness+fEpsilon),CryostatLidLogical,
                      fName+"_CryostatLidPhysical",cryoclearancelogical,false,0,false);
		  
    //body of cryostat  
    G4Tubs *CryostatTub = new G4Tubs(fName+"_CryostatTub",fCryostatInnerRadius, fCryostatInnerRadius+fCryostatThickness, 
                                     0.5*fCryostatLength,0,2*pi);
    G4LogicalVolume* CryostatLogical = new G4LogicalVolume(CryostatTub,G4Material::GetMaterial("Copper-EF"),
		                                           fName+"_CryostatLogical");
    //CryostatLogical->SetVisAttributes(new TSVisAttributesWireFrame("red"));
    CryostatLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0))); // FIXME: Changed color attribute from
                                                                                             //        red to copper, K. Nguyen
    new G4PVPlacement(0, G4ThreeVector(0,0,0),CryostatLogical,fName+"_CryostatPhysical",cryoclearancelogical,false,0,false);
    MGLog(routine)<<"Constructed a cryostat with inner diameter = "<<2*fCryostatInnerRadius/mm<<"mm, "<<2*fCryostatInnerRadius/mm/25.4
                  <<" inches and" <<"outer diameter = "<<2*(fCryostatInnerRadius+fCryostatThickness)/mm<<" mm, "
                  <<2*(fCryostatInnerRadius+fCryostatThickness)/mm/25.4<<" inches"<<endl;

    //bottom of cryostat, shifted down by epsilon
    G4Tubs *CryostatBottomTub = new G4Tubs(fName+"_CryostatBottom",0.,fCryostatInnerRadius+fCryostatThickness,
                                           0.5*fCryostatBottomThickness,0,2.*pi);
    G4LogicalVolume* CryostatBottomLogical = new G4LogicalVolume(CryostatBottomTub,G4Material::GetMaterial("Copper-EF"),
		                                                 fName+"_CryostatBottomLogical");
    //CryostatBottomLogical->SetVisAttributes(new TSVisAttributesWireFrame("red"));
    CryostatBottomLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0))); // FIXME: Changed color attribute from
                                                                                                   //        red to copper, K. Nguyen
    new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*fCryostatLength-0.5*fCryostatBottomThickness-fEpsilon),CryostatBottomLogical,
                                       fName+"_CryostatBottomPhysical",cryoclearancelogical,false,0,false);

    /// ****************************************  
    //Build the different string configurations:
    /// ****************************************  
    G4ThreeVector** StringTopLocalPosition = new G4ThreeVector * [fnStrings];
    G4RotationMatrix** StringLocalRotation = new G4RotationMatrix * [fnStrings];
    G4AffineTransform **StringAssemblyAffine = new G4AffineTransform * [fnStrings];
    G4ThreeVector** StringTopGlobalPosition = new G4ThreeVector * [fnStrings];
    G4RotationMatrix** StringGlobalRotation = new G4RotationMatrix * [fnStrings];		
         		
    if(fModuleType == k1Ring){			
      //1 ring of string surrounding a central string, similar to MJDemonstrator, but with TSStringAssembly
      MGLog(routine)<<"Building a 1 ring array with "<<fnStrings<<" strings and "<<fnCrystalsPerString<<" crystals per string"<<endl;	
      //radius of ring with center of each string on it
      G4double Ring1Radius = 2.0*(fStrings[0]->GetStringRadius()+fStringRadialClearance)+fRadialSpacingBetweenStrings;
      G4double angle = 60*deg;
			
      //place central string
      StringTopLocalPosition[0] = new G4ThreeVector(0.,0.,StringTopZPosition);
      StringLocalRotation[0] = new G4RotationMatrix();
      StringAssemblyAffine[0] = new G4AffineTransform(StringLocalRotation[0], *StringTopLocalPosition[0]);
      *StringAssemblyAffine[0] *= *assemAffine;  
      StringTopGlobalPosition[0]=new G4ThreeVector(StringAssemblyAffine[0]->NetTranslation());
      StringGlobalRotation[0]= new G4RotationMatrix(StringAssemblyAffine[0]->NetRotation());
			
      fStrings[0]->Place(StringTopGlobalPosition[0],StringGlobalRotation[0],cryoclearancelogical);	
      //place strings in the ring
      for(int i=1;i<fnStrings;i++){
	StringTopLocalPosition[i] = new G4ThreeVector(Ring1Radius*cos((i-1)*angle),Ring1Radius*sin((i-1)*angle),StringTopZPosition);
 	StringLocalRotation[i] = new G4RotationMatrix();			 
        StringAssemblyAffine[i] = new G4AffineTransform(StringLocalRotation[i],	*StringTopLocalPosition[i]);
        *StringAssemblyAffine[i] *= *assemAffine;  
	StringTopGlobalPosition[i]=new G4ThreeVector(StringAssemblyAffine[i]->NetTranslation());
        StringGlobalRotation[i]= new G4RotationMatrix(StringAssemblyAffine[i]->NetRotation());
			
        fStrings[i]->Place(StringTopGlobalPosition[i],StringGlobalRotation[i],cryoclearancelogical);
      }
    }

    if(fModuleType == k2Ring){			
      //1 ring of string surrounding a central string, similar to MJDemonstrator, but with TSStringAssembly
      MGLog(routine)<<"Building a 2 ring array with "<<fnStrings<<" strings and "<<fnCrystalsPerString<<" crystals per string"<<endl;	

      //radius of ring with center of each string on it 		
      G4double Ring1Radius = 2.0*(fStrings[0]->GetStringRadius()+fStringRadialClearance)+fRadialSpacingBetweenStrings; 
      G4double angleRing1 = 60*deg;
      //radius of ring with center of each string on it
      G4double Ring2Radius = 4.0*(fStrings[0]->GetStringRadius()+fStringRadialClearance)+2*fRadialSpacingBetweenStrings;
      G4double angleRing2 = 30*deg;	
			
      //place central string
      StringTopLocalPosition[0] = new G4ThreeVector(0.,0.,StringTopZPosition);
      StringLocalRotation[0] = new G4RotationMatrix();		
      StringAssemblyAffine[0] = new G4AffineTransform(StringLocalRotation[0],	*StringTopLocalPosition[0]);
      *StringAssemblyAffine[0] *= *assemAffine;  
      StringTopGlobalPosition[0]=new G4ThreeVector(StringAssemblyAffine[0]->NetTranslation());
      StringGlobalRotation[0]= new G4RotationMatrix(StringAssemblyAffine[0]->NetRotation());
			
      fStrings[0]->Place(StringTopGlobalPosition[0],StringGlobalRotation[0],cryoclearancelogical);	
      //place strings in the first ring
      for(int i=1;i<7;i++){
	StringTopLocalPosition[i] = new G4ThreeVector(Ring1Radius*cos((i-1)*angleRing1),Ring1Radius*sin((i-1)*angleRing1),StringTopZPosition);
 	StringLocalRotation[i] = new G4RotationMatrix();			 
        StringAssemblyAffine[i] = new G4AffineTransform(StringLocalRotation[i],	*StringTopLocalPosition[i]);
        *StringAssemblyAffine[i] *= *assemAffine;  
	StringTopGlobalPosition[i]=new G4ThreeVector(StringAssemblyAffine[i]->NetTranslation());
        StringGlobalRotation[i]= new G4RotationMatrix(StringAssemblyAffine[i]->NetRotation());
			
        fStrings[i]->Place(StringTopGlobalPosition[i],StringGlobalRotation[i],cryoclearancelogical);
      }
      //place strings in the second ring
      for(int i=7;i<fnStrings;i++){
	StringTopLocalPosition[i] = new G4ThreeVector(Ring2Radius*cos((i-1)*angleRing2),Ring2Radius*sin((i-1)*angleRing2),StringTopZPosition);
 	StringLocalRotation[i] = new G4RotationMatrix();			 
        StringAssemblyAffine[i] = new G4AffineTransform(StringLocalRotation[i],	*StringTopLocalPosition[i]);
        *StringAssemblyAffine[i] *= *assemAffine;  
	StringTopGlobalPosition[i]=new G4ThreeVector(StringAssemblyAffine[i]->NetTranslation());
        StringGlobalRotation[i]= new G4RotationMatrix(StringAssemblyAffine[i]->NetRotation());
			
        fStrings[i]->Place(StringTopGlobalPosition[i],StringGlobalRotation[i],cryoclearancelogical);
      }
    }
//////CODE TO PLACE 3 AND 4 RINGS SHOULD GO HERE
		
    if(fModuleType == k1String){			
      //1 string of crystals
      MGLog(routine)<<"Building a 1 string array with "<<fnCrystalsPerString<<" crystals in the string"<<endl;	

      //place central string
      StringTopLocalPosition[0] = new G4ThreeVector(0.,0.,StringTopZPosition);
      StringLocalRotation[0] = new G4RotationMatrix();			
      StringAssemblyAffine[0] = new G4AffineTransform(StringLocalRotation[0],	*StringTopLocalPosition[0]);
      *StringAssemblyAffine[0] *= *assemAffine;  			
      StringTopGlobalPosition[0]=new G4ThreeVector(StringAssemblyAffine[0]->NetTranslation());
      StringGlobalRotation[0]= new G4RotationMatrix(StringAssemblyAffine[0]->NetRotation());
			
      fStrings[0]->Place(StringTopGlobalPosition[0],StringGlobalRotation[0],cryoclearancelogical);	
    }

    if(fModuleType == k2String){			
      //1 string of crystals
      MGLog(routine)<<"Building a 2 string array with "<<fnCrystalsPerString<<" crystals per string"<<endl;	
			
      StringTopLocalPosition[0] = new G4ThreeVector(0.5*fRadialSpacingBetweenStrings+(fStrings[0]->GetStringRadius()+fStringRadialClearance),
                                                    0.,StringTopZPosition);
      StringLocalRotation[0] = new G4RotationMatrix();
      StringAssemblyAffine[0] = new G4AffineTransform(StringLocalRotation[0], *StringTopLocalPosition[0]);
      *StringAssemblyAffine[0] *= *assemAffine;  	
      StringTopGlobalPosition[0]=new G4ThreeVector(StringAssemblyAffine[0]->NetTranslation());
      StringGlobalRotation[0]= new G4RotationMatrix(StringAssemblyAffine[0]->NetRotation());
			
      fStrings[0]->Place(StringTopGlobalPosition[0],StringGlobalRotation[0],cryoclearancelogical);	

      StringTopLocalPosition[1] = new G4ThreeVector(-(0.5*fRadialSpacingBetweenStrings+(fStrings[0]->GetStringRadius()+fStringRadialClearance)),
                                                    0.,StringTopZPosition);
      StringLocalRotation[1] = new G4RotationMatrix();			
      StringAssemblyAffine[1] = new G4AffineTransform(StringLocalRotation[1], *StringTopLocalPosition[1]);
      *StringAssemblyAffine[1] *= *assemAffine;  
      StringTopGlobalPosition[1]=new G4ThreeVector(StringAssemblyAffine[1]->NetTranslation());
      StringGlobalRotation[1]= new G4RotationMatrix(StringAssemblyAffine[1]->NetRotation());
			
      fStrings[1]->Place(StringTopGlobalPosition[1],StringGlobalRotation[1],cryoclearancelogical);	
    }

    // FIXME: Added by K. Nguyen. Test code for module type 1HEXRing
    if(fModuleType == k1HEXRing){			
      //1 ring of string surrounding a central string, in a hexagonal shape
      MGLog(routine)<<"Building a 1 ring hex array with "<<fnStrings<<" strings and "<<fnCrystalsPerString<<" crystals per string"<<endl;	
      //radius of ring with center of each string on it
      G4double Ring1Radius = 2.0*(fStrings[0]->GetStringRadius()+fStringRadialClearance)+fRadialSpacingBetweenStrings;
      G4double angle = 60*deg;
			
      //place central string
      StringTopLocalPosition[0] = new G4ThreeVector(0.,0.,StringTopZPosition);
      StringLocalRotation[0] = new G4RotationMatrix();
      StringAssemblyAffine[0] = new G4AffineTransform(StringLocalRotation[0], *StringTopLocalPosition[0]);
      *StringAssemblyAffine[0] *= *assemAffine;  
      StringTopGlobalPosition[0]=new G4ThreeVector(StringAssemblyAffine[0]->NetTranslation());
      StringGlobalRotation[0]= new G4RotationMatrix(StringAssemblyAffine[0]->NetRotation());
			
      fStrings[0]->Place(StringTopGlobalPosition[0],StringGlobalRotation[0],cryoclearancelogical);	
      //place strings in the ring
      for(int i=1;i<fnStrings;i++){
	StringTopLocalPosition[i] = new G4ThreeVector(Ring1Radius*cos((i-1)*angle),Ring1Radius*sin((i-1)*angle),StringTopZPosition);
 	StringLocalRotation[i] = new G4RotationMatrix();			 
        StringAssemblyAffine[i] = new G4AffineTransform(StringLocalRotation[i],	*StringTopLocalPosition[i]);
        *StringAssemblyAffine[i] *= *assemAffine;  
	StringTopGlobalPosition[i]=new G4ThreeVector(StringAssemblyAffine[i]->NetTranslation());
        StringGlobalRotation[i]= new G4RotationMatrix(StringAssemblyAffine[i]->NetRotation());
			
        fStrings[i]->Place(StringTopGlobalPosition[i],StringGlobalRotation[i],cryoclearancelogical);
      }
    }

    // FIXME: Added by K. Nguyen. Test code for module type k2HEXRing
    if(fModuleType == k2HEXRing){			
      //1 ring of string surrounding a central string, similar to MJDemonstrator, but with TSStringAssembly
      MGLog(routine)<<"Building a 2 ring hex array with "<<fnStrings<<" strings and "<<fnCrystalsPerString<<" crystals per string"<<endl;	

      //radius of ring with center of each string on it 		
      G4double Ring1Radius = 2.0*(fStrings[0]->GetStringRadius()+fStringRadialClearance)+fRadialSpacingBetweenStrings; 
      G4double angleRing1 = 60*deg;
      //radius of ring with center of each string on it
      G4double Ring2Radius = 4.0*(fStrings[0]->GetStringRadius()+fStringRadialClearance)+2*fRadialSpacingBetweenStrings;
      G4double Ring2Apothem = Ring2Radius * sqrt( sin(60*deg)/(2 * tan(30*deg)) );
      G4double angleRing2 = 30*deg;	
			
      //place central string
      StringTopLocalPosition[0] = new G4ThreeVector(0.,0.,StringTopZPosition);
      StringLocalRotation[0] = new G4RotationMatrix();		
      StringAssemblyAffine[0] = new G4AffineTransform(StringLocalRotation[0], *StringTopLocalPosition[0]);
      *StringAssemblyAffine[0] *= *assemAffine;  
      StringTopGlobalPosition[0]=new G4ThreeVector(StringAssemblyAffine[0]->NetTranslation());
      StringGlobalRotation[0]= new G4RotationMatrix(StringAssemblyAffine[0]->NetRotation());
			
      fStrings[0]->Place(StringTopGlobalPosition[0],StringGlobalRotation[0],cryoclearancelogical);	
      //place strings in the first ring
      for(int i=1;i<7;i++){
	StringTopLocalPosition[i] = new G4ThreeVector(Ring1Radius*cos((i-1)*angleRing1),Ring1Radius*sin((i-1)*angleRing1),StringTopZPosition);
 	StringLocalRotation[i] = new G4RotationMatrix();			 
        StringAssemblyAffine[i] = new G4AffineTransform(StringLocalRotation[i],	*StringTopLocalPosition[i]);
        *StringAssemblyAffine[i] *= *assemAffine;  
	StringTopGlobalPosition[i]=new G4ThreeVector(StringAssemblyAffine[i]->NetTranslation());
        StringGlobalRotation[i]= new G4RotationMatrix(StringAssemblyAffine[i]->NetRotation());
			
        fStrings[i]->Place(StringTopGlobalPosition[i],StringGlobalRotation[i],cryoclearancelogical);
      }
      //place strings in the second ring
      G4double j = 1.0;
      G4double correctRadius = 0.0;
      for(int i=7;i<fnStrings;i++){
	if( !fmod( ((j-1.0)*30), 60.0) ) correctRadius = Ring2Apothem;
	else correctRadius = Ring2Radius;
	cout << "angle in radians: " << (j-1)*angleRing2 << endl;
	cout << "angle in degrees: " << (j-1)*angleRing2/deg << endl;
        cout << "correctRadius: " << correctRadius << "   Ring2Radius: " << Ring2Radius << endl;
	cout << "===========================================================" << endl;
	j++;

	StringTopLocalPosition[i] = new G4ThreeVector(correctRadius*cos((j-1)*angleRing2),correctRadius*sin((j-1)*angleRing2),StringTopZPosition);
 	StringLocalRotation[i] = new G4RotationMatrix();			 
        StringAssemblyAffine[i] = new G4AffineTransform(StringLocalRotation[i],	*StringTopLocalPosition[i]);
        *StringAssemblyAffine[i] *= *assemAffine;  
	StringTopGlobalPosition[i]=new G4ThreeVector(StringAssemblyAffine[i]->NetTranslation());
        StringGlobalRotation[i]= new G4RotationMatrix(StringAssemblyAffine[i]->NetRotation());
			
        fStrings[i]->Place(StringTopGlobalPosition[i],StringGlobalRotation[i],cryoclearancelogical);
      }
    }

    ///
		//Add more string configurations here
	  //(...)
    ////
		
    delete[] StringTopLocalPosition;
    delete[] StringLocalRotation ;
    delete[] StringAssemblyAffine ;
    delete[] StringTopGlobalPosition ;
    delete[] StringGlobalRotation ;
			
  }// end of else (! MJDemonstrator)
	
}

// -----------------------------------------------------------------------

