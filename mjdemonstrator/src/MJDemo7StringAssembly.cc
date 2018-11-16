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
// CLASS IMPLEMENTATION:  MJDemoCrystalAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Bottom face of cold plate
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 * 
 * 07-22-2010, Created, M. Green
 */
//---------------------------------------------------------------------------//
//
#include <math.h>
#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjio/MJJSONReader.hh"
#include "mjdemonstrator/MJDemo7StringAssembly.hh"
#include "mjdemonstrator/MJDemoStringAssembly.hh"
#include "mjdemonstrator/MJDemoColdPlate.hh"
#include "mjdemonstrator/MJDemoColdPlatePin.hh"
#include "mjdemonstrator/MJDemoHVTwist.hh"
#include "mjdemonstrator/MJDemoSilicaStack.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemo7StringAssembly::MJDemo7StringAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "7Strings")
{
  //fColdPlatePtr = new MJDemoColdPlate(fName+"_ColdPlate_001", "867-5309");
  
  MJJSONReader* reader = new MJJSONReader();
  std::vector<G4String> stringList;
  stringList = reader->ReadAssemblyStringList(serialNumber);
  delete reader;
  
  fNumStrings = stringList.size();
  for(size_t i = fNumStrings; i < 7; i++)
    stringList.push_back("867-5309"); //Don't want the next few lines to throw an error
  if(fNumStrings == 0)
    fNumStrings = 7;
  
  if(stringList.at(0) == "NULL")
    fString1Ptr = NULL;
  else
    fString1Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_001", stringList.at(0));

  if(stringList.at(1) == "NULL")
    fString2Ptr = NULL;
  else
    fString2Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_002", stringList.at(1));

  if(stringList.at(2) == "NULL")
    fString3Ptr = NULL;
  else
    fString3Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_003", stringList.at(2));
 
  if(stringList.at(3) == "NULL")
    fString4Ptr = NULL;
  else
    fString4Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_004", stringList.at(3));  
  
  if(stringList.at(4) == "NULL")
    fString5Ptr = NULL;
  else
    fString5Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_005", stringList.at(4));  
  
  if(stringList.at(5) == "NULL")
    fString6Ptr = NULL;
  else
    fString6Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_006", stringList.at(5));  
  
  if(stringList.at(6) == "NULL")
    fString7Ptr = NULL;
  else
    fString7Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_007", stringList.at(6));  
  
  /*fHVTwist1Ptr = new MJDemoHVTwist(fName+"_HVTwist_001","867-5309");//Wenqin
  fHVTwist2Ptr = new MJDemoHVTwist(fName+"_HVTwist_002","867-5309");//Wenqin
  fHVTwist3Ptr = new MJDemoHVTwist(fName+"_HVTwist_003","867-5309");//Wenqin
  fHVTwist4Ptr = new MJDemoHVTwist(fName+"_HVTwist_004","867-5309");//Wenqin
  fHVTwist5Ptr = new MJDemoHVTwist(fName+"_HVTwist_005","867-5309");//Wenqin
  fHVTwist6Ptr = new MJDemoHVTwist(fName+"_HVTwist_006","867-5309");//Wenqin
  fHVTwist7Ptr = new MJDemoHVTwist(fName+"_HVTwist_007","867-5309");//Wenqin
  fSilicaStack1Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_001","867-5309");//Wenqin
  fSilicaStack2Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_002","867-5309");//Wenqin
  fSilicaStack3Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_003","867-5309");//Wenqin
  fSilicaStack4Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_004","867-5309");//Wenqin
  fSilicaStack5Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_005","867-5309");//Wenqin
  fSilicaStack6Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_006","867-5309");//Wenqin
  fSilicaStack7Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_007","867-5309");//Wenqin
  fPin1Ptr = new MJDemoColdPlatePin(fName+"_ColdPlatePin_001", "867-5309");
  fPin2Ptr = new MJDemoColdPlatePin(fName+"_ColdPlatePin_002", "867-5309");
  fPin3Ptr = new MJDemoColdPlatePin(fName+"_ColdPlatePin_003", "867-5309");*/
}

MJDemo7StringAssembly::MJDemo7StringAssembly(const MJDemo7StringAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  //fColdPlatePtr = new MJDemoColdPlate(fName+"_ColdPlate_001", "867-5309");
  fString1Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_001", "867-5309");  
  fString2Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_002", "867-5309");  
  fString3Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_003", "867-5309");  
  fString4Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_004", "867-5309");  
  fString5Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_005", "867-5309");  
  fString6Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_006", "867-5309");  
  fString7Ptr = new MJDemoStringAssembly(fName+"_StringAssembly_007", "867-5309");  
  /*fHVTwist1Ptr = new MJDemoHVTwist(fName+"_HVTwist_001","867-5309");//Wenqin
  fHVTwist2Ptr = new MJDemoHVTwist(fName+"_HVTwist_002","867-5309");//Wenqin
  fHVTwist3Ptr = new MJDemoHVTwist(fName+"_HVTwist_003","867-5309");//Wenqin
  fHVTwist4Ptr = new MJDemoHVTwist(fName+"_HVTwist_004","867-5309");//Wenqin
  fHVTwist5Ptr = new MJDemoHVTwist(fName+"_HVTwist_005","867-5309");//Wenqin
  fHVTwist6Ptr = new MJDemoHVTwist(fName+"_HVTwist_006","867-5309");//Wenqin
  fHVTwist7Ptr = new MJDemoHVTwist(fName+"_HVTwist_007","867-5309");//Wenqin
  fSilicaStack1Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_001","867-5309");//Wenqin
  fSilicaStack2Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_002","867-5309");//Wenqin
  fSilicaStack3Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_003","867-5309");//Wenqin
  fSilicaStack4Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_004","867-5309");//Wenqin
  fSilicaStack5Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_005","867-5309");//Wenqin
  fSilicaStack6Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_006","867-5309");//Wenqin
  fSilicaStack7Ptr = new MJDemoSilicaStack(fName+"_SilicaStack_007","867-5309");//Wenqin
  fPin1Ptr = new MJDemoColdPlatePin(fName+"_ColdPlatePin_001", "867-5309");
  fPin2Ptr = new MJDemoColdPlatePin(fName+"_ColdPlatePin_002", "867-5309");
  fPin3Ptr = new MJDemoColdPlatePin(fName+"_ColdPlatePin_003", "867-5309");*/
}

MJDemo7StringAssembly::~MJDemo7StringAssembly()
{
  //delete fColdPlatePtr;
  delete fString1Ptr;
  delete fString2Ptr;
  delete fString3Ptr;
  delete fString4Ptr;
  delete fString5Ptr;
  delete fString6Ptr;
  delete fString7Ptr;
  /*delete fHVTwist1Ptr;//Wenqin
  delete fHVTwist2Ptr;//Wenqin
  delete fHVTwist3Ptr;//Wenqin
  delete fHVTwist4Ptr;//Wenqin
  delete fHVTwist5Ptr;//Wenqin
  delete fHVTwist6Ptr;//Wenqin
  delete fHVTwist7Ptr;//Wenqin
  delete fSilicaStack1Ptr;//Wenqin
  delete fSilicaStack2Ptr;//Wenqin
  delete fSilicaStack3Ptr;//Wenqin
  delete fSilicaStack4Ptr;//Wenqin
  delete fSilicaStack5Ptr;//Wenqin
  delete fSilicaStack6Ptr;//Wenqin
  delete fSilicaStack7Ptr;//Wenqin
  delete fPin1Ptr;
  delete fPin2Ptr;
  delete fPin3Ptr;*/
}

void MJDemo7StringAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);


  /*G4ThreeVector *ColdPlatelocalPos = new G4ThreeVector(0, 0, 0.2*25.4*mm);
  G4RotationMatrix *ColdPlatelocalRot = new G4RotationMatrix();
  G4AffineTransform *ColdPlateaffine = new G4AffineTransform(ColdPlatelocalRot,*ColdPlatelocalPos);
  *ColdPlateaffine*=*assemAffine;  
  G4ThreeVector *ColdPlateglobalPos = new G4ThreeVector(ColdPlateaffine->NetTranslation());
  G4RotationMatrix *ColdPlateglobalRot= new G4RotationMatrix(ColdPlateaffine->NetRotation());
  fColdPlatePtr->Place(ColdPlateglobalPos, ColdPlateglobalRot, motherLogical);
  delete ColdPlatelocalPos;
  delete ColdPlatelocalRot;
  delete ColdPlateaffine;

  G4ThreeVector *ColdPlatePin1localPos = new G4ThreeVector(6.1*25.4*mm*cos(20*deg), 
							   6.1*25.4*mm*sin(20*deg), 0.2*25.4*mm);
  G4RotationMatrix *ColdPlatePin1localRot = new G4RotationMatrix();
  ColdPlatePin1localRot->rotateZ(-20*deg);
  ColdPlatePin1localRot->rotateY(pi/2);
  G4AffineTransform *ColdPlatePin1affine = new G4AffineTransform(ColdPlatePin1localRot,*ColdPlatePin1localPos);
  *ColdPlatePin1affine*=*assemAffine;  
  G4ThreeVector *ColdPlatePin1globalPos = new G4ThreeVector(ColdPlatePin1affine->NetTranslation());
  G4RotationMatrix *ColdPlatePin1globalRot= new G4RotationMatrix(ColdPlatePin1affine->NetRotation());
  fPin1Ptr->Place(ColdPlatePin1globalPos, ColdPlatePin1globalRot, motherLogical);
  delete ColdPlatePin1localPos;
  delete ColdPlatePin1localRot;
  delete ColdPlatePin1affine;
  
  G4ThreeVector *ColdPlatePin2localPos = new G4ThreeVector(6.1*25.4*mm*cos(-20*deg), 
							   6.1*25.4*mm*sin(-20*deg), 0.2*25.4*mm);
  G4RotationMatrix *ColdPlatePin2localRot = new G4RotationMatrix();
  ColdPlatePin2localRot->rotateZ(20*deg);
  ColdPlatePin2localRot->rotateY(pi/2);
  G4AffineTransform *ColdPlatePin2affine = new G4AffineTransform(ColdPlatePin2localRot,*ColdPlatePin2localPos);
  *ColdPlatePin2affine*=*assemAffine;  
  G4ThreeVector *ColdPlatePin2globalPos = new G4ThreeVector(ColdPlatePin2affine->NetTranslation());
  G4RotationMatrix *ColdPlatePin2globalRot= new G4RotationMatrix(ColdPlatePin2affine->NetRotation());
  fPin2Ptr->Place(ColdPlatePin2globalPos, ColdPlatePin2globalRot, motherLogical);
  delete ColdPlatePin2localPos;
  delete ColdPlatePin2localRot;
  delete ColdPlatePin2affine;
  
  G4ThreeVector *ColdPlatePin3localPos = new G4ThreeVector(6.1*25.4*mm*cos(190*deg), 
  														   6.1*25.4*mm*sin(190*deg), 0.2*25.4*mm);
  G4RotationMatrix *ColdPlatePin3localRot = new G4RotationMatrix();
  ColdPlatePin3localRot->rotateZ(-190*deg);
  ColdPlatePin3localRot->rotateY(pi/2);
  G4AffineTransform *ColdPlatePin3affine = new G4AffineTransform(ColdPlatePin3localRot,*ColdPlatePin3localPos);
  *ColdPlatePin3affine*=*assemAffine;  
  G4ThreeVector *ColdPlatePin3globalPos = new G4ThreeVector(ColdPlatePin3affine->NetTranslation());
  G4RotationMatrix *ColdPlatePin3globalRot= new G4RotationMatrix(ColdPlatePin3affine->NetRotation());
  fPin3Ptr->Place(ColdPlatePin3globalPos, ColdPlatePin3globalRot, motherLogical);
  delete ColdPlatePin3localPos;
  delete ColdPlatePin3localRot;
  delete ColdPlatePin3affine;  

 //Wenqin: The HV Twists and slica stacks. One set for each string. Should be added/commented out according to string configuration
  
  G4double xHexhole[21]={
			1.8*25.4*mm,1.74*25.4*mm, 
		        4.854*25.4*mm,4.581*25.4*mm,1.614*25.4*mm,1.108*25.4*mm, -2.219*25.4*mm, -1.563*25.4*mm,
                        -1.708*25.4*mm,-1.112*25.4*mm,2.016*25.4*mm,1.887*25.4*mm,4.609*25.4*mm,4.882*25.4*mm,
                        //Wenqin: These are the 2x7=14 hex tapped holes for HV Twists, Clock-wise+first one in the center        
			-0.10*25.4*mm,
                        2.415*25.4*mm,-1.664*25.4*mm,-4.49*25.4*mm,-3.74*25.4*mm,-0.35*25.4*mm,2.566*25.4*mm
                        //Wenqin: These are the 7 hex tapped holes for slica stacks     
                        };//Wenqin: They are organized as Clock-wise+first one in the center        

  G4double yHexhole[21]={
                       -0.01*25.4*mm, 1.529*25.4*mm,
		       -1.106*25.4*mm,-2.624*25.4*mm,-3.386*25.4*mm,-4.84*25.4*mm,-3.066*25.4*mm,-1.673*25.4*mm,
                       3.478*25.4*mm,2.058*25.4*mm,3.893*25.4*mm,5.428*25.4*mm,0.801*25.4*mm,2.316*25.4*mm,
		       //Wenqin: These are the 2x7=14 hex tapped holes for HV Twists, Clock-wise+first one in the center	
			1.65*25.4*mm,
                       -0.05*25.4*mm,-4.000*25.4*mm,-2.23*25.4*mm,3.72*25.4*mm,5.39*25.4*mm,3.646*25.4*mm
                      }; 
                       //Wenqin: The coordinates in the Solid Work is a reflection of GEANT4, likely caused by an upside-down orintation in the Solid Work       
                       //Wenqin: As a result, one of the x or y coordiates read from the Solid Work must be applied a -1   

  //Wenqin: There are 21 hex tapped holes. These coordinates are with respect to the center of the cold plate, 
  //and they must be the same as the hex cut defined in MJDemoColdPlate.cc, which is based on Solid Work Coldplate 77mm SLDPRT 
  
  //Wenqin: the distance between the two bolts for a HV Twist is 1.54"  
  
   
  G4ThreeVector *HVTwist1localPos = new G4ThreeVector(-1*(xHexhole[0]+xHexhole[1])/2, -1*(yHexhole[0]+yHexhole[1])/2, (0.2+0.2+0.0465)*25.4*mm);
  //Wenqin: -1 is needed as in ColdPlate.cc, /2 is needed to get the center, note cold plate is at (0,0,0.2*25.4*mm);the HV Twist is right on top, center to center is 0.2+0.0465" 
  G4RotationMatrix *HVTwist1localRot = new G4RotationMatrix();
  HVTwist1localRot->rotateZ(-1*atan((yHexhole[0]-yHexhole[1])/(xHexhole[0]-xHexhole[1])));
  G4AffineTransform *HVTwist1affine = new G4AffineTransform(HVTwist1localRot,*HVTwist1localPos); 
  *HVTwist1affine*=*assemAffine;
  G4ThreeVector *HVTwist1globalPos = new G4ThreeVector(HVTwist1affine->NetTranslation());
  G4RotationMatrix *HVTwist1globalRot= new G4RotationMatrix(HVTwist1affine->NetRotation());
  fHVTwist1Ptr->Place(HVTwist1globalPos,HVTwist1globalRot,motherLogical);
  delete HVTwist1localPos;
  delete HVTwist1localRot;
  delete HVTwist1affine;

  G4ThreeVector *SilicaStack1localPos = new G4ThreeVector(-1*xHexhole[14], -1*yHexhole[14], (0.2+0.2+0.042)*25.4*mm);
  //Wenqin: -1 is needed as in ColdPlate.cc,note cold plate is at (0,0,0.2*25.4*mm);the Silica is right on top, center to center is 0.2+0.042" 
  G4RotationMatrix *SilicaStack1localRot = new G4RotationMatrix();
  SilicaStack1localRot->rotateZ(0.3*pi);
  G4AffineTransform *SilicaStack1affine = new G4AffineTransform(SilicaStack1localRot,*SilicaStack1localPos); 
  *SilicaStack1affine*=*assemAffine;
  G4ThreeVector *SilicaStack1globalPos = new G4ThreeVector(SilicaStack1affine->NetTranslation());
  G4RotationMatrix *SilicaStack1globalRot= new G4RotationMatrix(SilicaStack1affine->NetRotation());
  fSilicaStack1Ptr->Place(SilicaStack1globalPos,SilicaStack1globalRot,motherLogical);
  delete SilicaStack1localPos;
  delete SilicaStack1localRot;
  delete SilicaStack1affine;*/

  G4double stringRadius = 3.73*25.4*mm;
  G4String tempSN;
  if(fString1Ptr)
    {
      G4ThreeVector *StringAssembly1localPos = new G4ThreeVector(0, 0, -0.001*25.4*mm);//0
      G4RotationMatrix *StringAssembly1localRot = new G4RotationMatrix();
      //StringAssembly1localRot->rotateZ(130*deg); //Add 180 to all rotations...
      tempSN = fString2Ptr->GetSerialNumber();
      if(!(tempSN[0]=='C'))
	StringAssembly1localRot->rotateZ(130*deg);
      G4AffineTransform *StringAssembly1affine = new G4AffineTransform(StringAssembly1localRot,*StringAssembly1localPos);
      *StringAssembly1affine*=*assemAffine;  
      G4ThreeVector *StringAssembly1globalPos = new G4ThreeVector(StringAssembly1affine->NetTranslation());
      G4RotationMatrix *StringAssembly1globalRot= new G4RotationMatrix(StringAssembly1affine->NetRotation());
      fString1Ptr->Place(StringAssembly1globalPos, StringAssembly1globalRot, motherLogical);
      delete StringAssembly1localPos;
      delete StringAssembly1localRot;
      delete StringAssembly1affine;
    }

   
  /*G4ThreeVector *HVTwist2localPos = new G4ThreeVector(-1*(xHexhole[2]+xHexhole[3])/2, -1*(yHexhole[2]+yHexhole[3])/2, (0.2+0.2+0.0465)*25.4*mm);
  G4RotationMatrix *HVTwist2localRot = new G4RotationMatrix();
  HVTwist2localRot->rotateZ(-1*atan((yHexhole[2]-yHexhole[3])/(xHexhole[2]-xHexhole[3])));
  G4AffineTransform *HVTwist2affine = new G4AffineTransform(HVTwist2localRot,*HVTwist2localPos); 
  *HVTwist2affine*=*assemAffine;
  G4ThreeVector *HVTwist2globalPos = new G4ThreeVector(HVTwist2affine->NetTranslation());
  G4RotationMatrix *HVTwist2globalRot= new G4RotationMatrix(HVTwist2affine->NetRotation());
  fHVTwist2Ptr->Place(HVTwist2globalPos,HVTwist2globalRot,motherLogical);
  delete HVTwist2localPos;
  delete HVTwist2localRot;
  delete HVTwist2affine;

  G4ThreeVector *SilicaStack2localPos = new G4ThreeVector(-1*xHexhole[15], -1*yHexhole[15], (0.2+0.2+0.042)*25.4*mm);
  //Wenqin: -1 is needed as in ColdPlate.cc,note cold plate is at (0,0,0.2*25.4*mm);the Silica is right on top, center to center is 0.2+0.042" 
  G4RotationMatrix *SilicaStack2localRot = new G4RotationMatrix();
  SilicaStack2localRot->rotateZ(0.3*pi);
  G4AffineTransform *SilicaStack2affine = new G4AffineTransform(SilicaStack2localRot,*SilicaStack2localPos); 
  *SilicaStack2affine*=*assemAffine;
  G4ThreeVector *SilicaStack2globalPos = new G4ThreeVector(SilicaStack2affine->NetTranslation());
  G4RotationMatrix *SilicaStack2globalRot= new G4RotationMatrix(SilicaStack2affine->NetRotation());
  fSilicaStack2Ptr->Place(SilicaStack2globalPos,SilicaStack2globalRot,motherLogical);
  delete SilicaStack2localPos;
  delete SilicaStack2localRot;
  delete SilicaStack2affine;


  G4ThreeVector *HVTwist3localPos = new G4ThreeVector(-1*(xHexhole[4]+xHexhole[5])/2, -1*(yHexhole[4]+yHexhole[5])/2, (0.2+0.2+0.0465)*25.4*mm);
  G4RotationMatrix *HVTwist3localRot = new G4RotationMatrix();
  HVTwist3localRot->rotateZ(-1*atan((yHexhole[4]-yHexhole[5])/(xHexhole[4]-xHexhole[5])));
  G4AffineTransform *HVTwist3affine = new G4AffineTransform(HVTwist3localRot,*HVTwist3localPos); 
  *HVTwist3affine*=*assemAffine;
  G4ThreeVector *HVTwist3globalPos = new G4ThreeVector(HVTwist3affine->NetTranslation());
  G4RotationMatrix *HVTwist3globalRot= new G4RotationMatrix(HVTwist3affine->NetRotation());
  fHVTwist3Ptr->Place(HVTwist3globalPos,HVTwist3globalRot,motherLogical);
  delete HVTwist3localPos;
  delete HVTwist3localRot;
  delete HVTwist3affine;

  G4ThreeVector *SilicaStack3localPos = new G4ThreeVector(-1*xHexhole[16], -1*yHexhole[16], (0.2+0.2+0.042)*25.4*mm);
  //Wenqin: -1 is needed as in ColdPlate.cc,note cold plate is at (0,0,0.2*25.4*mm);the Silica is right on top, center to center is 0.2+0.042" 
  G4RotationMatrix *SilicaStack3localRot = new G4RotationMatrix();
  G4AffineTransform *SilicaStack3affine = new G4AffineTransform(SilicaStack3localRot,*SilicaStack3localPos); 
  *SilicaStack3affine*=*assemAffine;
  G4ThreeVector *SilicaStack3globalPos = new G4ThreeVector(SilicaStack3affine->NetTranslation());
  G4RotationMatrix *SilicaStack3globalRot= new G4RotationMatrix(SilicaStack3affine->NetRotation());
  fSilicaStack3Ptr->Place(SilicaStack3globalPos,SilicaStack3globalRot,motherLogical);
  delete SilicaStack3localPos;
  delete SilicaStack3localRot;
  delete SilicaStack3affine;



  G4ThreeVector *HVTwist4localPos = new G4ThreeVector(-1*(xHexhole[6]+xHexhole[7])/2, -1*(yHexhole[6]+yHexhole[7])/2, (0.2+0.2+0.0465)*25.4*mm);
  G4RotationMatrix *HVTwist4localRot = new G4RotationMatrix();
  HVTwist4localRot->rotateZ(-1*atan((yHexhole[6]-yHexhole[7])/(xHexhole[6]-xHexhole[7])));
  G4AffineTransform *HVTwist4affine = new G4AffineTransform(HVTwist4localRot,*HVTwist4localPos); 
  *HVTwist4affine*=*assemAffine;
  G4ThreeVector *HVTwist4globalPos = new G4ThreeVector(HVTwist4affine->NetTranslation());
  G4RotationMatrix *HVTwist4globalRot= new G4RotationMatrix(HVTwist4affine->NetRotation());
  fHVTwist4Ptr->Place(HVTwist4globalPos,HVTwist4globalRot,motherLogical);
  delete HVTwist4localPos;
  delete HVTwist4localRot;
  delete HVTwist4affine;

  G4ThreeVector *SilicaStack4localPos = new G4ThreeVector(-1*xHexhole[17], -1*yHexhole[17], (0.2+0.2+0.042)*25.4*mm);
  //Wenqin: -1 is needed as in ColdPlate.cc,note cold plate is at (0,0,0.2*25.4*mm);the Silica is right on top, center to center is 0.2+0.042" 
  G4RotationMatrix *SilicaStack4localRot = new G4RotationMatrix();
  SilicaStack4localRot->rotateZ(pi);
  G4AffineTransform *SilicaStack4affine = new G4AffineTransform(SilicaStack4localRot,*SilicaStack4localPos); 
  *SilicaStack4affine*=*assemAffine;
  G4ThreeVector *SilicaStack4globalPos = new G4ThreeVector(SilicaStack4affine->NetTranslation());
  G4RotationMatrix *SilicaStack4globalRot= new G4RotationMatrix(SilicaStack4affine->NetRotation());
  fSilicaStack4Ptr->Place(SilicaStack4globalPos,SilicaStack4globalRot,motherLogical);
  delete SilicaStack4localPos;
  delete SilicaStack4localRot;
  delete SilicaStack4affine;



  G4ThreeVector *HVTwist5localPos = new G4ThreeVector(-1*(xHexhole[8]+xHexhole[9])/2, -1*(yHexhole[8]+yHexhole[9])/2, (0.2+0.2+0.0465)*25.4*mm);
  G4RotationMatrix *HVTwist5localRot = new G4RotationMatrix();
  HVTwist5localRot->rotateZ(-1*atan((yHexhole[8]-yHexhole[9])/(xHexhole[8]-xHexhole[9])));
  G4AffineTransform *HVTwist5affine = new G4AffineTransform(HVTwist5localRot,*HVTwist5localPos); 
  *HVTwist5affine*=*assemAffine;
  G4ThreeVector *HVTwist5globalPos = new G4ThreeVector(HVTwist5affine->NetTranslation());
  G4RotationMatrix *HVTwist5globalRot= new G4RotationMatrix(HVTwist5affine->NetRotation());
  fHVTwist5Ptr->Place(HVTwist5globalPos,HVTwist5globalRot,motherLogical);
  delete HVTwist5localPos;
  delete HVTwist5localRot;
  delete HVTwist5affine;


  G4ThreeVector *SilicaStack5localPos = new G4ThreeVector(-1*xHexhole[18], -1*yHexhole[18], (0.2+0.2+0.042)*25.4*mm);
  //Wenqin: -1 is needed as in ColdPlate.cc,note cold plate is at (0,0,0.2*25.4*mm);the Silica is right on top, center to center is 0.2+0.042" 
  G4RotationMatrix *SilicaStack5localRot = new G4RotationMatrix();
  SilicaStack5localRot->rotateZ(-0.5*pi);
  G4AffineTransform *SilicaStack5affine = new G4AffineTransform(SilicaStack5localRot,*SilicaStack5localPos); 
  *SilicaStack5affine*=*assemAffine;
  G4ThreeVector *SilicaStack5globalPos = new G4ThreeVector(SilicaStack5affine->NetTranslation());
  G4RotationMatrix *SilicaStack5globalRot= new G4RotationMatrix(SilicaStack5affine->NetRotation());
  fSilicaStack5Ptr->Place(SilicaStack5globalPos,SilicaStack5globalRot,motherLogical);
  delete SilicaStack5localPos;
  delete SilicaStack5localRot;
  delete SilicaStack5affine;


  G4ThreeVector *HVTwist6localPos = new G4ThreeVector(-1*(xHexhole[10]+xHexhole[11])/2, -1*(yHexhole[10]+yHexhole[11])/2, (0.2+0.2+0.0465)*25.4*mm);
  G4RotationMatrix *HVTwist6localRot = new G4RotationMatrix();
  HVTwist6localRot->rotateZ(-1*atan((yHexhole[10]-yHexhole[11])/(xHexhole[10]-xHexhole[11])));
  G4AffineTransform *HVTwist6affine = new G4AffineTransform(HVTwist6localRot,*HVTwist6localPos); 
  *HVTwist6affine*=*assemAffine;
  G4ThreeVector *HVTwist6globalPos = new G4ThreeVector(HVTwist6affine->NetTranslation());
  G4RotationMatrix *HVTwist6globalRot= new G4RotationMatrix(HVTwist6affine->NetRotation());
  fHVTwist6Ptr->Place(HVTwist6globalPos,HVTwist6globalRot,motherLogical);
  delete HVTwist6localPos;
  delete HVTwist6localRot;
  delete HVTwist6affine;

  G4ThreeVector *SilicaStack6localPos = new G4ThreeVector(-1*xHexhole[19], -1*yHexhole[19], (0.2+0.2+0.042)*25.4*mm);
  //Wenqin: -1 is needed as in ColdPlate.cc,note cold plate is at (0,0,0.2*25.4*mm);the Silica is right on top, center to center is 0.2+0.042" 
  G4RotationMatrix *SilicaStack6localRot = new G4RotationMatrix();
  G4AffineTransform *SilicaStack6affine = new G4AffineTransform(SilicaStack6localRot,*SilicaStack6localPos); 
  *SilicaStack6affine*=*assemAffine;
  G4ThreeVector *SilicaStack6globalPos = new G4ThreeVector(SilicaStack6affine->NetTranslation());
  G4RotationMatrix *SilicaStack6globalRot= new G4RotationMatrix(SilicaStack6affine->NetRotation());
  fSilicaStack6Ptr->Place(SilicaStack6globalPos,SilicaStack6globalRot,motherLogical);
  delete SilicaStack6localPos;
  delete SilicaStack6localRot;
  delete SilicaStack6affine;



  G4ThreeVector *HVTwist7localPos = new G4ThreeVector(-1*(xHexhole[12]+xHexhole[13])/2, -1*(yHexhole[12]+yHexhole[13])/2, (0.2+0.2+0.0465)*25.4*mm);
  G4RotationMatrix *HVTwist7localRot = new G4RotationMatrix();
  HVTwist7localRot->rotateZ(atan(-1*(yHexhole[12]-yHexhole[13])/(xHexhole[12]-xHexhole[13])));
  G4AffineTransform *HVTwist7affine = new G4AffineTransform(HVTwist7localRot,*HVTwist7localPos); 
  *HVTwist7affine*=*assemAffine;
  G4ThreeVector *HVTwist7globalPos = new G4ThreeVector(HVTwist7affine->NetTranslation());
  G4RotationMatrix *HVTwist7globalRot= new G4RotationMatrix(HVTwist7affine->NetRotation());
  fHVTwist7Ptr->Place(HVTwist7globalPos,HVTwist7globalRot,motherLogical);
  delete HVTwist7localPos;
  delete HVTwist7localRot;
  delete HVTwist7affine;

  G4ThreeVector *SilicaStack7localPos = new G4ThreeVector(-1*xHexhole[20], -1*yHexhole[20], (0.2+0.2+0.042)*25.4*mm);
  //Wenqin: -1 is needed as in ColdPlate.cc,note cold plate is at (0,0,0.2*25.4*mm);the Silica is right on top, center to center is 0.2+0.042" 
  G4RotationMatrix *SilicaStack7localRot = new G4RotationMatrix();
  SilicaStack7localRot->rotateZ(0);
  G4AffineTransform *SilicaStack7affine = new G4AffineTransform(SilicaStack7localRot,*SilicaStack7localPos); 
  *SilicaStack7affine*=*assemAffine;
  G4ThreeVector *SilicaStack7globalPos = new G4ThreeVector(SilicaStack7affine->NetTranslation());
  G4RotationMatrix *SilicaStack7globalRot= new G4RotationMatrix(SilicaStack7affine->NetRotation());
  fSilicaStack7Ptr->Place(SilicaStack7globalPos,SilicaStack7globalRot,motherLogical);
  delete SilicaStack7localPos;
  delete SilicaStack7localRot;
  delete SilicaStack7affine;*/

  if((fNumStrings > 1) && fString2Ptr)
    {
      G4ThreeVector *StringAssembly2localPos;
      G4RotationMatrix *StringAssembly2localRot = new G4RotationMatrix();
      tempSN = fString2Ptr->GetSerialNumber();
      if(!(tempSN[0]=='C'))
	{
	  StringAssembly2localPos = new G4ThreeVector(stringRadius*cos(330*deg), stringRadius*sin(330*deg), -0.001*25.4*mm); //0
	  StringAssembly2localRot->rotateZ(-110*deg);
	}
      else
	{
	  StringAssembly2localPos = new G4ThreeVector(stringRadius*cos(210*deg), stringRadius*sin(210*deg), -0.001*25.4*mm); //0
	  StringAssembly2localRot->rotateZ(79*deg);
	}
      G4AffineTransform *StringAssembly2affine = new G4AffineTransform(StringAssembly2localRot,*StringAssembly2localPos);
      *StringAssembly2affine*=*assemAffine;  
      G4ThreeVector *StringAssembly2globalPos = new G4ThreeVector(StringAssembly2affine->NetTranslation());
      G4RotationMatrix *StringAssembly2globalRot= new G4RotationMatrix(StringAssembly2affine->NetRotation());
      fString2Ptr->Place(StringAssembly2globalPos, StringAssembly2globalRot, motherLogical);
      delete StringAssembly2localPos;
      delete StringAssembly2localRot;
      delete StringAssembly2affine;
    }

  if((fNumStrings > 2) && fString3Ptr)
    {
      G4ThreeVector *StringAssembly3localPos;
      G4RotationMatrix *StringAssembly3localRot = new G4RotationMatrix();
      tempSN = fString3Ptr->GetSerialNumber();
      if(!(tempSN[0]=='C'))
	{
	  StringAssembly3localPos = new G4ThreeVector(stringRadius*cos(270*deg), stringRadius*sin(270*deg), -0.001*25.4*mm); //0
	  StringAssembly3localRot->rotateZ(-110*deg);
	}
      else
	{
	  StringAssembly3localPos = new G4ThreeVector(stringRadius*cos(270*deg), stringRadius*sin(270*deg), -0.001*25.4*mm); //0
	  StringAssembly3localRot->rotateZ(70*deg);
	}
      G4AffineTransform *StringAssembly3affine = new G4AffineTransform(StringAssembly3localRot,*StringAssembly3localPos);
      *StringAssembly3affine*=*assemAffine;  
      G4ThreeVector *StringAssembly3globalPos = new G4ThreeVector(StringAssembly3affine->NetTranslation());
      G4RotationMatrix *StringAssembly3globalRot= new G4RotationMatrix(StringAssembly3affine->NetRotation());
      fString3Ptr->Place(StringAssembly3globalPos, StringAssembly3globalRot, motherLogical);
      delete StringAssembly3localPos;
      delete StringAssembly3localRot;
      delete StringAssembly3affine;
    }

  if((fNumStrings >3) && fString4Ptr)
    {
      G4ThreeVector *StringAssembly4localPos;
      G4RotationMatrix *StringAssembly4localRot = new G4RotationMatrix();
      //StringAssembly4localRot->rotateZ(10*deg);
      tempSN = fString4Ptr->GetSerialNumber();
      if(!(tempSN[0]=='C'))
	{
	  StringAssembly4localPos = new G4ThreeVector(stringRadius*cos(210*deg), stringRadius*sin(210*deg), -0.001*25.4*mm); //0
	  StringAssembly4localRot->rotateZ(10*deg);
	}
      else
	{
	  StringAssembly4localPos = new G4ThreeVector(stringRadius*cos(330*deg), stringRadius*sin(330*deg), -0.001*25.4*mm); //0
	  StringAssembly4localRot->rotateZ(70*deg);
	}
      G4AffineTransform *StringAssembly4affine = new G4AffineTransform(StringAssembly4localRot,*StringAssembly4localPos);
      *StringAssembly4affine*=*assemAffine;  
      G4ThreeVector *StringAssembly4globalPos = new G4ThreeVector(StringAssembly4affine->NetTranslation());
      G4RotationMatrix *StringAssembly4globalRot= new G4RotationMatrix(StringAssembly4affine->NetRotation());
      fString4Ptr->Place(StringAssembly4globalPos, StringAssembly4globalRot, motherLogical);
      delete StringAssembly4localPos;
      delete StringAssembly4localRot;
      delete StringAssembly4affine;
    }
  
  if((fNumStrings > 4) && fString5Ptr)
    {
      G4ThreeVector *StringAssembly5localPos;
      G4RotationMatrix *StringAssembly5localRot = new G4RotationMatrix();
      //StringAssembly5localRot->rotateZ(10*deg);
      tempSN = fString5Ptr->GetSerialNumber();
      if(!(tempSN[0]=='C'))
	{
	  StringAssembly5localPos = new G4ThreeVector(stringRadius*cos(150*deg), stringRadius*sin(150*deg), -0.001*25.4*mm); //0
	  StringAssembly5localRot->rotateZ(10*deg);
	}
      else
	{
	  StringAssembly5localPos = new G4ThreeVector(stringRadius*cos(30*deg), stringRadius*sin(30*deg), -0.001*25.4*mm); //0
	  StringAssembly5localRot->rotateZ(-70*deg);
	}
      G4AffineTransform *StringAssembly5affine = new G4AffineTransform(StringAssembly5localRot,*StringAssembly5localPos);
      *StringAssembly5affine*=*assemAffine;  
      G4ThreeVector *StringAssembly5globalPos = new G4ThreeVector(StringAssembly5affine->NetTranslation());
      G4RotationMatrix *StringAssembly5globalRot= new G4RotationMatrix(StringAssembly5affine->NetRotation());
      fString5Ptr->Place(StringAssembly5globalPos, StringAssembly5globalRot, motherLogical);
      delete StringAssembly5localPos;
      delete StringAssembly5localRot;
      delete StringAssembly5affine;
    }

  if((fNumStrings > 5) && fString6Ptr)
    {
      G4ThreeVector *StringAssembly6localPos;
      G4RotationMatrix *StringAssembly6localRot = new G4RotationMatrix();
      //StringAssembly6localRot->rotateZ(130*deg);
      tempSN = fString6Ptr->GetSerialNumber();
      if(!(tempSN[0]=='C'))
	{
	  StringAssembly6localPos = new G4ThreeVector(stringRadius*cos(90*deg), stringRadius*sin(90*deg), -0.001*25.4*mm); //0
	  StringAssembly6localRot->rotateZ(130*deg);
	}
      else
	{
	  StringAssembly6localPos = new G4ThreeVector(stringRadius*cos(90*deg), stringRadius*sin(90*deg), -0.001*25.4*mm); //0
	  StringAssembly6localRot->rotateZ(-70*deg);
	}
      G4AffineTransform *StringAssembly6affine = new G4AffineTransform(StringAssembly6localRot,*StringAssembly6localPos);
      *StringAssembly6affine*=*assemAffine;  
      G4ThreeVector *StringAssembly6globalPos = new G4ThreeVector(StringAssembly6affine->NetTranslation());
      G4RotationMatrix *StringAssembly6globalRot= new G4RotationMatrix(StringAssembly6affine->NetRotation());
      fString6Ptr->Place(StringAssembly6globalPos, StringAssembly6globalRot, motherLogical);
      delete StringAssembly6localPos;
      delete StringAssembly6localRot;
      delete StringAssembly6affine;
    }

  if((fNumStrings > 6) && fString7Ptr)
    {
      G4ThreeVector *StringAssembly7localPos;
      G4RotationMatrix *StringAssembly7localRot = new G4RotationMatrix();
      //StringAssembly7localRot->rotateZ(130*deg);
      tempSN = fString7Ptr->GetSerialNumber();
      if(!(tempSN[0]=='C'))
	{
	  StringAssembly7localPos = new G4ThreeVector(stringRadius*cos(30*deg), stringRadius*sin(30*deg), -0.001*25.4*mm); //0
	  StringAssembly7localRot->rotateZ(130*deg);
	}
      else
	{
	  StringAssembly7localPos = new G4ThreeVector(stringRadius*cos(150*deg), stringRadius*sin(150*deg), -0.001*25.4*mm); //0
	  StringAssembly7localRot->rotateZ(-79*deg);
	}
      G4AffineTransform *StringAssembly7affine = new G4AffineTransform(StringAssembly7localRot,*StringAssembly7localPos);
      *StringAssembly7affine*=*assemAffine;  
      G4ThreeVector *StringAssembly7globalPos = new G4ThreeVector(StringAssembly7affine->NetTranslation());
      G4RotationMatrix *StringAssembly7globalRot= new G4RotationMatrix(StringAssembly7affine->NetRotation());
      fString7Ptr->Place(StringAssembly7globalPos, StringAssembly7globalRot, motherLogical);
      delete StringAssembly7localPos;
      delete StringAssembly7localRot;
      delete StringAssembly7affine;
    }
  
  delete assemAffine;
}
