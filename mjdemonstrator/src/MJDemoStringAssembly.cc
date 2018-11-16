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
// CLASS IMPLEMENTATION:  MJDemoStringAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Coaxial with string, coincident with top face of string
 *adapter plate.
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
 * 10-29-2013, Added flexible string functionality
 * 11-14-2013, Updated string adapter plate bolt and added in the string adapter plate nut, J. MacMullin
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"
#include "G4UIcommand.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjio/MJJSONReader.hh"
#include "mjdemonstrator/MJDemoStringAssembly.hh"
#include "mjdemonstrator/MJDemoStringAdapterPlate.hh"
#include "mjdemonstrator/MJDemo1032CapturedBolt.hh"
#include "mjdemonstrator/MJDemo1032Nut.hh"
//#include "mjdemonstrator/MJDemoCopperSpacer.hh"
#include "mjdemonstrator/MJDemoSplitNutTieRod.hh"
#include "mjdemonstrator/MJDemoStringTieRod.hh"
#include "mjdemonstrator/MJDemoCrystalAssembly.hh"
#include "mjdemonstrator/MJDemoBottomNutTieRod.hh"
#include "mjdemonstrator/MJDemoCableAssembly.hh"
#include "mjdemonstrator/MJDemoHVCableAssembly.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoStringAssembly::MJDemoStringAssembly(G4String fName, G4String serialNumber, size_t crystalNum) :
  MJVDemoAssembly(fName, serialNumber, "MJ80-02-200"), fCrystalNum(crystalNum)
{
  if(fCrystalNum > 5)
    fCrystalNum = 5;
  if(fCrystalNum < 1)
    fCrystalNum = 1;
  fAdapterPlatePtr = new MJDemoStringAdapterPlate(fName+"_StringAdapterPlate_001", "867-5309");  
  fAdapterBolt1Ptr = new MJDemo1032CapturedBolt(fName+"_AdapterPlateBolt_001", "867-5309");
  fAdapterBolt2Ptr = new MJDemo1032CapturedBolt(fName+"_AdapterPlateBolt_002", "867-5309");
  fAdapterBolt3Ptr = new MJDemo1032CapturedBolt(fName+"_AdapterPlateBolt_003", "867-5309");
  fAdapterNut1Ptr = new MJDemo1032Nut(fName+"_AdapterPlateNut_001", "867-5309");
  fAdapterNut2Ptr = new MJDemo1032Nut(fName+"_AdapterPlateNut_002", "867-5309");
  fAdapterNut3Ptr = new MJDemo1032Nut(fName+"_AdapterPlateNut_003", "867-5309");
  fSplitNut1Ptr = new MJDemoSplitNutTieRod(fName+"_SplitNutTieRod_001", "867-5309");  
  fSplitNut2Ptr = new MJDemoSplitNutTieRod(fName+"_SplitNutTieRod_002", "867-5309");  
  fSplitNut3Ptr = new MJDemoSplitNutTieRod(fName+"_SplitNutTieRod_003", "867-5309");
//  fCopperSpacer1Ptr = new MJDemoCopperSpacer(fName+"_CopperSpacer_001", "867-5309");
//  fCopperSpacer2Ptr = new MJDemoCopperSpacer(fName+"_CopperSpacer_002", "867-5309");
//  fCopperSpacer3Ptr = new MJDemoCopperSpacer(fName+"_CopperSpacer_003", "867-5309");
  fTieRod1Ptr = new MJDemoStringTieRod(fName+"_StringTieRod_001", "867-5309");  
  fTieRod2Ptr = new MJDemoStringTieRod(fName+"_StringTieRod_002", "867-5309");  
  fTieRod3Ptr = new MJDemoStringTieRod(fName+"_StringTieRod_003", "867-5309");
  MJDemoCrystalAssembly** temp = new MJDemoCrystalAssembly*[fCrystalNum];
  fCrystalAssemblyPtr = temp;  
  fBottomNut1Ptr = new MJDemoBottomNutTieRod(fName+"_BottomNutTieRod_001", "867-5309");  
  fBottomNut2Ptr = new MJDemoBottomNutTieRod(fName+"_BottomNutTieRod_002", "867-5309");  
  fBottomNut3Ptr = new MJDemoBottomNutTieRod(fName+"_BottomNutTieRod_003", "867-5309");	
}

MJDemoStringAssembly::MJDemoStringAssembly(const MJDemoStringAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{ 
  fAdapterPlatePtr = new MJDemoStringAdapterPlate(fName+"_StringAdapterPlate_001", "867-5309");  
  fAdapterBolt1Ptr = new MJDemo1032CapturedBolt(fName+"_AdapterPlateBolt_001", "867-5309");
  fAdapterBolt2Ptr = new MJDemo1032CapturedBolt(fName+"_AdapterPlateBolt_002", "867-5309");
  fAdapterBolt3Ptr = new MJDemo1032CapturedBolt(fName+"_AdapterPlateBolt_003", "867-5309");
  fAdapterNut1Ptr = new MJDemo1032Nut(fName+"_AdapterPlateNut_001", "867-5309");
  fAdapterNut2Ptr = new MJDemo1032Nut(fName+"_AdapterPlateNut_002", "867-5309");
  fAdapterNut3Ptr = new MJDemo1032Nut(fName+"_AdapterPlateNut_003", "867-5309");
  fSplitNut1Ptr = new MJDemoSplitNutTieRod(fName+"_SplitNutTieRod_001", "867-5309");  
  fSplitNut2Ptr = new MJDemoSplitNutTieRod(fName+"_SplitNutTieRod_002", "867-5309");  
  fSplitNut3Ptr = new MJDemoSplitNutTieRod(fName+"_SplitNutTieRod_003", "867-5309");
//  fCopperSpacer1Ptr = new MJDemoCopperSpacer(fName+"_CopperSpacer_001", "867-5309");
//  fCopperSpacer2Ptr = new MJDemoCopperSpacer(fName+"_CopperSpacer_002", "867-5309");
//  fCopperSpacer3Ptr = new MJDemoCopperSpacer(fName+"_CopperSpacer_003", "867-5309");
  fTieRod1Ptr = new MJDemoStringTieRod(fName+"_StringTieRod_001", "867-5309");  
  fTieRod2Ptr = new MJDemoStringTieRod(fName+"_StringTieRod_002", "867-5309");  
  fTieRod3Ptr = new MJDemoStringTieRod(fName+"_StringTieRod_003", "867-5309");
  fCrystalAssemblyPtr = rhs.fCrystalAssemblyPtr;
  fBottomNut1Ptr = new MJDemoBottomNutTieRod(fName+"_BottomNutTieRod_001", "867-5309");  
  fBottomNut2Ptr = new MJDemoBottomNutTieRod(fName+"_BottomNutTieRod_002", "867-5309");  
  fBottomNut3Ptr = new MJDemoBottomNutTieRod(fName+"_BottomNutTieRod_003", "867-5309");
}

MJDemoStringAssembly::~MJDemoStringAssembly()
{
  delete fAdapterPlatePtr;
  delete fAdapterBolt1Ptr;
  delete fAdapterBolt2Ptr;
  delete fAdapterBolt3Ptr;
  delete fAdapterNut1Ptr;
  delete fAdapterNut2Ptr;
  delete fAdapterNut3Ptr;
  delete fSplitNut1Ptr;
  delete fSplitNut2Ptr;
  delete fSplitNut3Ptr;
//  delete fCopperSpacer1Ptr;
//  delete fCopperSpacer2Ptr;
//  delete fCopperSpacer3Ptr;
  delete fTieRod1Ptr;
  delete fTieRod2Ptr;
  delete fTieRod3Ptr;
  delete fBottomNut1Ptr;
  delete fBottomNut2Ptr;
  delete fBottomNut3Ptr;
  delete[] fCrystalAssemblyPtr;
}

void MJDemoStringAssembly::SetNumberOfCrystals(size_t crystalNum)
{
  if(crystalNum > 5)
    crystalNum = 5;
  if(crystalNum < 1)
    crystalNum = 1;
  fCrystalNum = crystalNum;
}

void MJDemoStringAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
 
  

  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *stringAdapterPlate1localPos = new G4ThreeVector(0, 0, -0.175*25.4*mm);   
  G4RotationMatrix *stringAdapterPlate1localRot = new G4RotationMatrix();
  G4AffineTransform *stringAdapterPlate1affine = new G4AffineTransform(stringAdapterPlate1localRot, *stringAdapterPlate1localPos);
  *stringAdapterPlate1affine *= *assemAffine;  
  G4ThreeVector *stringAdapterPlate1globalPos = new G4ThreeVector(stringAdapterPlate1affine->NetTranslation());
  G4RotationMatrix *stringAdapterPlate1globalRot= new G4RotationMatrix(stringAdapterPlate1affine->NetRotation());
  fAdapterPlatePtr->Place(stringAdapterPlate1globalPos,stringAdapterPlate1globalRot, motherLogical);
  delete stringAdapterPlate1localPos;
  delete stringAdapterPlate1localRot;
  delete stringAdapterPlate1affine;

  G4ThreeVector *adapterBolt1localPos = new G4ThreeVector(0.75*25.4*mm, 
  														  0.*mm, -(0.18+0.12)*25.4*mm);
  G4RotationMatrix *adapterBolt1localRot = new G4RotationMatrix();
  G4AffineTransform *adapterBolt1affine = new G4AffineTransform(adapterBolt1localRot, *adapterBolt1localPos);
  *adapterBolt1affine*=*assemAffine;  
  G4ThreeVector *adapterBolt1globalPos = new G4ThreeVector(adapterBolt1affine->NetTranslation());
  G4RotationMatrix *adapterBolt1globalRot= new G4RotationMatrix(adapterBolt1affine->NetRotation());
  fAdapterBolt1Ptr->Place(adapterBolt1globalPos, adapterBolt1globalRot, motherLogical);
  delete adapterBolt1localPos;
  delete adapterBolt1localRot;
  delete adapterBolt1affine;

  G4ThreeVector *adapterBolt2localPos = new G4ThreeVector(0.75*25.4*cos(120*deg)*mm, 
  														  0.75*25.4*sin(120*deg)*mm, 
  														  -(0.18+0.12)*25.4*mm);
  G4RotationMatrix *adapterBolt2localRot = new G4RotationMatrix();
  G4AffineTransform *adapterBolt2affine = new G4AffineTransform(adapterBolt2localRot, *adapterBolt2localPos);
  *adapterBolt2affine*=*assemAffine;  
  G4ThreeVector *adapterBolt2globalPos = new G4ThreeVector(adapterBolt2affine->NetTranslation());
  G4RotationMatrix *adapterBolt2globalRot= new G4RotationMatrix(adapterBolt2affine->NetRotation());
  fAdapterBolt2Ptr->Place(adapterBolt2globalPos, adapterBolt2globalRot, motherLogical);
  delete adapterBolt2localPos;
  delete adapterBolt2localRot;
  delete adapterBolt2affine;  
  
  G4ThreeVector *adapterBolt3localPos = new G4ThreeVector(0.75*25.4*cos(240*deg)*mm, 
  														  0.75*25.4*sin(240*deg)*mm, 
  														  -(0.18+0.12)*25.4*mm);
  G4RotationMatrix *adapterBolt3localRot = new G4RotationMatrix();
  G4AffineTransform *adapterBolt3affine = new G4AffineTransform(adapterBolt3localRot, *adapterBolt3localPos);
  *adapterBolt3affine*=*assemAffine;  
  G4ThreeVector *adapterBolt3globalPos = new G4ThreeVector(adapterBolt3affine->NetTranslation());
  G4RotationMatrix *adapterBolt3globalRot= new G4RotationMatrix(adapterBolt3affine->NetRotation());
  fAdapterBolt3Ptr->Place(adapterBolt3globalPos, adapterBolt3globalRot, motherLogical);
  delete adapterBolt3localPos;
  delete adapterBolt3localRot;
  delete adapterBolt3affine;     
  
  G4ThreeVector *adapterNut1localPos = new G4ThreeVector(0.75*25.4*mm, 0*mm, 0*mm);
  G4RotationMatrix *adapterNut1localRot = new G4RotationMatrix();
  G4AffineTransform *adapterNut1affine = new G4AffineTransform(adapterNut1localRot, *adapterNut1localPos);
  *adapterNut1affine*=*assemAffine;  
  G4ThreeVector *adapterNut1globalPos = new G4ThreeVector(adapterNut1affine->NetTranslation());
  G4RotationMatrix *adapterNut1globalRot= new G4RotationMatrix(adapterNut1affine->NetRotation());
  fAdapterNut1Ptr->Place(adapterNut1globalPos, adapterNut1globalRot, motherLogical);
  delete adapterNut1localPos;
  delete adapterNut1localRot;
  delete adapterNut1affine;  
  
  G4ThreeVector *adapterNut2localPos = new G4ThreeVector(0.75*25.4*cos(120*deg)*mm, 
  														 0.75*25.4*sin(120*deg)*mm, 0*mm);
  G4RotationMatrix *adapterNut2localRot = new G4RotationMatrix();
  G4AffineTransform *adapterNut2affine = new G4AffineTransform(adapterNut2localRot, *adapterNut2localPos);
  *adapterNut2affine*=*assemAffine;  
  G4ThreeVector *adapterNut2globalPos = new G4ThreeVector(adapterNut2affine->NetTranslation());
  G4RotationMatrix *adapterNut2globalRot= new G4RotationMatrix(adapterNut2affine->NetRotation());
  fAdapterNut2Ptr->Place(adapterNut2globalPos, adapterNut2globalRot, motherLogical);
  delete adapterNut2localPos;
  delete adapterNut2localRot;
  delete adapterNut2affine; 

  G4ThreeVector *adapterNut3localPos = new G4ThreeVector(0.75*25.4*cos(240*deg)*mm, 
  														 0.75*25.4*sin(240*deg)*mm, 0*mm);
  G4RotationMatrix *adapterNut3localRot = new G4RotationMatrix();
  G4AffineTransform *adapterNut3affine = new G4AffineTransform(adapterNut3localRot, *adapterNut3localPos);
  *adapterNut3affine*=*assemAffine;  
  G4ThreeVector *adapterNut3globalPos = new G4ThreeVector(adapterNut3affine->NetTranslation());
  G4RotationMatrix *adapterNut3globalRot= new G4RotationMatrix(adapterNut3affine->NetRotation());
  fAdapterNut3Ptr->Place(adapterNut3globalPos, adapterNut3globalRot, motherLogical);
  delete adapterNut3localPos;
  delete adapterNut3localRot;
  delete adapterNut3affine; 

  G4double postRadius = 1.84*25.4*mm;

  G4ThreeVector *splitNut1localPos = new G4ThreeVector(postRadius, 0, -0.03*25.4*mm);   
  G4RotationMatrix *splitNut1localRot = new G4RotationMatrix();
  G4AffineTransform *splitNut1affine = new G4AffineTransform(splitNut1localRot, *splitNut1localPos);
  *splitNut1affine*=*assemAffine;  
  G4ThreeVector *splitNut1globalPos = new G4ThreeVector(splitNut1affine->NetTranslation());
  G4RotationMatrix *splitNut1globalRot= new G4RotationMatrix(splitNut1affine->NetRotation());
  fSplitNut1Ptr->Place(splitNut1globalPos, splitNut1globalRot, motherLogical);
  delete splitNut1localPos;
  delete splitNut1localRot;
  delete splitNut1affine;

  G4ThreeVector *splitNut2localPos =  
             new G4ThreeVector(postRadius*cos(120*deg), postRadius*sin(120*deg), -0.03*25.4*mm);   
  G4RotationMatrix *splitNut2localRot = new G4RotationMatrix();
  splitNut2localRot->rotateZ(240*deg);
  G4AffineTransform *splitNut2affine = new G4AffineTransform(splitNut2localRot, *splitNut2localPos);
  *splitNut2affine*=*assemAffine;  
  G4ThreeVector *splitNut2globalPos = new G4ThreeVector(splitNut2affine->NetTranslation());
  G4RotationMatrix *splitNut2globalRot= new G4RotationMatrix(splitNut2affine->NetRotation());
  fSplitNut2Ptr->Place(splitNut2globalPos, splitNut2globalRot, motherLogical);
  delete splitNut2localPos;
  delete splitNut2localRot;
  delete splitNut2affine;

  G4ThreeVector *splitNut3localPos = 
             new G4ThreeVector(postRadius*cos(240*deg), postRadius*sin(240*deg), -0.03*25.4*mm);   
  G4RotationMatrix *splitNut3localRot = new G4RotationMatrix();
  splitNut3localRot->rotateZ(120*deg);
  G4AffineTransform *splitNut3affine = new G4AffineTransform(splitNut3localRot,*splitNut3localPos);
  *splitNut3affine*=*assemAffine;  
  G4ThreeVector *splitNut3globalPos = new G4ThreeVector(splitNut3affine->NetTranslation());
  G4RotationMatrix *splitNut3globalRot= new G4RotationMatrix(splitNut3affine->NetRotation());
  fSplitNut3Ptr->Place(splitNut3globalPos, splitNut3globalRot, motherLogical);
  delete splitNut3localPos;
  delete splitNut3localRot;
  delete splitNut3affine;

  //---For proper tie rods:
  MJJSONReader* reader = new MJJSONReader();
  std::vector<G4String> crysAssemList = reader->ReadCrystalAssemblyList(fSerialNumber);
//  std::vector<G4String> spacerType = reader->ReadCustom(fSerialNumber, "string", "spacer");
  delete reader;
  
  //if(spacerType.size() == 0)
    //spacerType.push_back("ignore");
  if(crysAssemList.size() > 0) //otherwise it's preset to 5;
    fCrystalNum = crysAssemList.size();
  else //else we are making crystals with dummy serial numbers.
    {
      crysAssemList.clear();
      for(size_t i = 0; i<fCrystalNum; i++)
	crysAssemList.push_back("867-5309");
    }
  std::cout << fName << " has " << "Number of Crystals: " << fCrystalNum << std::endl;
  if(fCrystalNum == 1)
    {
      fTieRod1Ptr->SetAdditionalLength(-7.41*25.4*mm);
      fTieRod1Ptr->SetDrawingNumber("MJ80-02-420");
      fTieRod2Ptr->SetAdditionalLength(-7.41*25.4*mm);
      fTieRod2Ptr->SetDrawingNumber("MJ80-02-420");
      fTieRod3Ptr->SetAdditionalLength(-7.41*25.4*mm);
      fTieRod3Ptr->SetDrawingNumber("MJ80-02-420");
    }

  G4ThreeVector *stringTieRod1localPos = new G4ThreeVector(postRadius, 0, -0.04*25.4*mm);   
  G4RotationMatrix *stringTieRod1localRot = new G4RotationMatrix();
  G4AffineTransform *stringTieRod1affine = new G4AffineTransform(stringTieRod1localRot,
								 *stringTieRod1localPos);
  *stringTieRod1affine*=*assemAffine;  
  G4ThreeVector *stringTieRod1globalPos = new G4ThreeVector(
							    stringTieRod1affine->NetTranslation());
  G4RotationMatrix *stringTieRod1globalRot= new G4RotationMatrix(
								 stringTieRod1affine->NetRotation());
  fTieRod1Ptr->Place(stringTieRod1globalPos, stringTieRod1globalRot, motherLogical);
  delete stringTieRod1localPos;
  delete stringTieRod1localRot;
  delete stringTieRod1affine;

  G4ThreeVector *stringTieRod2localPos =  
             new G4ThreeVector(postRadius*cos(120*deg), postRadius*sin(120*deg), -0.04*25.4*mm);   
  G4RotationMatrix *stringTieRod2localRot = new G4RotationMatrix();
  G4AffineTransform *stringTieRod2affine = new G4AffineTransform(stringTieRod2localRot,
  																	*stringTieRod2localPos);
  *stringTieRod2affine*=*assemAffine;  
  G4ThreeVector *stringTieRod2globalPos = new G4ThreeVector(
  														stringTieRod2affine->NetTranslation());
  G4RotationMatrix *stringTieRod2globalRot= new G4RotationMatrix(
  														stringTieRod2affine->NetRotation());
  fTieRod2Ptr->Place(stringTieRod2globalPos, stringTieRod2globalRot, motherLogical);
  delete stringTieRod2localPos;
  delete stringTieRod2localRot;
  delete stringTieRod2affine;

  G4ThreeVector *stringTieRod3localPos = 
             new G4ThreeVector(postRadius*cos(240*deg), postRadius*sin(240*deg), -0.04*25.4*mm);   
  G4RotationMatrix *stringTieRod3localRot = new G4RotationMatrix();
  G4AffineTransform *stringTieRod3affine = new G4AffineTransform(stringTieRod3localRot,
								 *stringTieRod3localPos);
  *stringTieRod3affine*=*assemAffine;  
  G4ThreeVector *stringTieRod3globalPos = new G4ThreeVector(
							    stringTieRod3affine->NetTranslation());
  G4RotationMatrix *stringTieRod3globalRot= new G4RotationMatrix(
								 stringTieRod3affine->NetRotation());
  fTieRod3Ptr->Place(stringTieRod3globalPos, stringTieRod3globalRot, motherLogical);
  delete stringTieRod3localPos;
  delete stringTieRod3localRot;
  delete stringTieRod3affine;

  G4double crystalSpacing = 2.057*25.4*mm; //2.056
  
  G4double* crystalZ = new G4double[fCrystalNum];
  fCrystalAssemblyPtr[0] = new MJDemoCrystalAssembly(fName+"_CrystalAssembly_001", crysAssemList.at(0));
  fCrystalAssemblyPtr[0]->ConstructParts();
//Micah, this is the spacer placement code. vvvvvvvvvvv
//We need to set the crystal assembly (detector unit) z positions appropriately; need to know spacers to place the DUs.

/*
  if(fCrystalNum < 5 && spacerType.at(0) != "none") //For the spacers, we don't need one if we have 5 BeGes
    {
      if(fCrystalNum == 1) //test this
	{
	  fCopperSpacer1Ptr->SetAdditionalLength(0.308*25.4*mm);
	  fCopperSpacer1Ptr->SetDrawingNumber("MJ80-02-421");
	  fCopperSpacer2Ptr->SetAdditionalLength(0.308*25.4*mm);
	  fCopperSpacer2Ptr->SetDrawingNumber("MJ80-02-421");
	  fCopperSpacer3Ptr->SetAdditionalLength(0.308*25.4*mm);
	  fCopperSpacer3Ptr->SetDrawingNumber("MJ80-02-421");
	}
      if(spacerType.at(0) == "short")
	{
	  fCopperSpacer1Ptr->SetAdditionalLength(-0.23*25.4*mm);
	  fCopperSpacer1Ptr->SetDrawingNumber("MJ80-11-184");
	  fCopperSpacer2Ptr->SetAdditionalLength(-0.23*25.4*mm);
	  fCopperSpacer2Ptr->SetDrawingNumber("MJ80-11-184");
	  fCopperSpacer3Ptr->SetAdditionalLength(-0.23*25.4*mm);
	  fCopperSpacer3Ptr->SetDrawingNumber("MJ80-11-184");
	}
      G4double spacerZpos = -0.1375*25.4*mm - 0.727*25.4*mm - fCopperSpacer1Ptr->GetAdditionalLength(); // = pos of adapter plate minus length of the spacer
      G4ThreeVector *copperSpacer1localPos = 
	new G4ThreeVector(postRadius, 0, spacerZpos);   
      G4RotationMatrix *copperSpacer1localRot = new G4RotationMatrix();
      //copperSpacer1localRot->rotateX(pi);
      G4AffineTransform *copperSpacer1affine = new G4AffineTransform(copperSpacer1localRot,
								     *copperSpacer1localPos);
      *copperSpacer1affine*=*assemAffine;  
      G4ThreeVector *copperSpacer1globalPos = new G4ThreeVector(
								copperSpacer1affine->NetTranslation());
      G4RotationMatrix *copperSpacer1globalRot= new G4RotationMatrix(
								     copperSpacer1affine->NetRotation());
      fCopperSpacer1Ptr->Place(copperSpacer1globalPos, copperSpacer1globalRot, motherLogical);
      delete copperSpacer1localPos;
      delete copperSpacer1localRot;
      delete copperSpacer1affine;
      //------number 2-----
      G4ThreeVector *copperSpacer2localPos = 
	new G4ThreeVector(postRadius*cos(120*deg), postRadius*sin(120*deg), spacerZpos);   
      G4RotationMatrix *copperSpacer2localRot = new G4RotationMatrix();
      G4AffineTransform *copperSpacer2affine = new G4AffineTransform(copperSpacer2localRot,
								     *copperSpacer2localPos);
      *copperSpacer2affine*=*assemAffine;  
      G4ThreeVector *copperSpacer2globalPos = new G4ThreeVector(
								copperSpacer2affine->NetTranslation());
      G4RotationMatrix *copperSpacer2globalRot= new G4RotationMatrix(
								     copperSpacer2affine->NetRotation());
      fCopperSpacer2Ptr->Place(copperSpacer2globalPos, copperSpacer2globalRot, motherLogical);
      delete copperSpacer2localPos;
      delete copperSpacer2localRot;
      delete copperSpacer2affine;
      //-----number 3 -----
      G4ThreeVector *copperSpacer3localPos = 
	new G4ThreeVector(postRadius*cos(240*deg), postRadius*sin(240*deg), spacerZpos);   
      G4RotationMatrix *copperSpacer3localRot = new G4RotationMatrix();
      G4AffineTransform *copperSpacer3affine = new G4AffineTransform(copperSpacer3localRot,
								     *copperSpacer3localPos);
      *copperSpacer3affine*=*assemAffine;  
      G4ThreeVector *copperSpacer3globalPos = new G4ThreeVector(
								copperSpacer3affine->NetTranslation());
      G4RotationMatrix *copperSpacer3globalRot= new G4RotationMatrix(
								     copperSpacer3affine->NetRotation());
      fCopperSpacer3Ptr->Place(copperSpacer3globalPos, copperSpacer3globalRot, motherLogical);
      delete copperSpacer3localPos;
      delete copperSpacer3localRot;
      delete copperSpacer3affine;
*/
  if(fCrystalAssemblyPtr[0]->GetSpacerType().size()>0 && fCrystalAssemblyPtr[0]->GetSpacerType().at(0)!="none")
    {
      G4double spacerZpos = -0.35*25.4*mm - 0.512*25.4*mm - fCrystalAssemblyPtr[0]->GetSpacer1()->GetAdditionalLength(); // = pos of adapter plate minus length of the spacer except for part that sits inside adapter plate
      crystalZ[0] = -1.5792*25.4*mm + spacerZpos - fCrystalAssemblyPtr[0]->GetAdditionalLength();
    }
  else
    crystalZ[0] = -1.8792*25.4*mm - fCrystalAssemblyPtr[0]->GetAdditionalLength();
  //---now continue with drawing that first crystal
  G4ThreeVector *crystalAssemblyLocalPos = new G4ThreeVector(0, 0, crystalZ[0]);
  G4RotationMatrix *crystalAssemblyLocalRot = new G4RotationMatrix();
  crystalAssemblyLocalRot->rotateX(pi);
  G4AffineTransform *crystalAssemblyAffine = new G4AffineTransform(crystalAssemblyLocalRot,
								   *crystalAssemblyLocalPos);
  *crystalAssemblyAffine *= *assemAffine;  
  G4ThreeVector *crystalAssemblyGlobalPos = new G4ThreeVector(
							      crystalAssemblyAffine->NetTranslation());
  G4RotationMatrix *crystalAssemblyGlobalRot= new G4RotationMatrix(
								   crystalAssemblyAffine->NetRotation());
  fCrystalAssemblyPtr[0]->Place(crystalAssemblyGlobalPos, crystalAssemblyGlobalRot, motherLogical);
  
  delete crystalAssemblyLocalPos;
  delete crystalAssemblyLocalRot;
  delete crystalAssemblyAffine;

  //for the rest of the crystals:
  for(size_t i = 1; i<fCrystalNum; i++){
    fCrystalAssemblyPtr[i] = new MJDemoCrystalAssembly(fName+"_CrystalAssembly_00"+G4UIcommand::ConvertToString((G4int)i+1), crysAssemList.at(i));
    crystalZ[i] = crystalZ[i-1] - crystalSpacing - fCrystalAssemblyPtr[i]->GetAdditionalLength();
    fCrystalAssemblyPtr[i]->ConstructParts();
    if(fCrystalAssemblyPtr[i]->GetSpacerType().size()>0 && fCrystalAssemblyPtr[i]->GetSpacerType().at(0)!="none")
      {
        crystalZ[i] -= 0.512*25.4*mm - fCrystalAssemblyPtr[i]->GetSpacer1()->GetAdditionalLength();
      }
    crystalAssemblyLocalPos = new G4ThreeVector(0, 0, crystalZ[i]);
    crystalAssemblyLocalRot = new G4RotationMatrix();
    crystalAssemblyLocalRot->rotateX(pi);
    crystalAssemblyAffine = new G4AffineTransform(crystalAssemblyLocalRot,
								      *crystalAssemblyLocalPos);
    *crystalAssemblyAffine *= *assemAffine;  
    crystalAssemblyGlobalPos = new G4ThreeVector(
								 crystalAssemblyAffine->NetTranslation());
    crystalAssemblyGlobalRot= new G4RotationMatrix(
								      crystalAssemblyAffine->NetRotation());
    fCrystalAssemblyPtr[i]->Place(crystalAssemblyGlobalPos, crystalAssemblyGlobalRot, motherLogical);
    
    delete crystalAssemblyLocalPos;
    delete crystalAssemblyLocalRot;
    delete crystalAssemblyAffine;
  }

  G4double bottomNutZ = crystalZ[fCrystalNum-1]-0.5289*25.4*mm;

  G4ThreeVector *bottomNut1localPos = new G4ThreeVector(postRadius, 0, bottomNutZ);   
  G4RotationMatrix *bottomNut1localRot = new G4RotationMatrix();
  G4AffineTransform *bottomNut1affine = new G4AffineTransform(bottomNut1localRot,
  																	*bottomNut1localPos);
  *bottomNut1affine*=*assemAffine;  
  G4ThreeVector *bottomNut1globalPos = new G4ThreeVector(bottomNut1affine->NetTranslation());
  G4RotationMatrix *bottomNut1globalRot= new G4RotationMatrix(bottomNut1affine->NetRotation());
  fBottomNut1Ptr->Place(bottomNut1globalPos, bottomNut1globalRot, motherLogical);
  delete bottomNut1localPos;
  delete bottomNut1localRot;
  delete bottomNut1affine;

  G4ThreeVector *bottomNut2localPos =  
             new G4ThreeVector(postRadius*cos(120*deg), postRadius*sin(120*deg), bottomNutZ);   
  G4RotationMatrix *bottomNut2localRot = new G4RotationMatrix();
  G4AffineTransform *bottomNut2affine = new G4AffineTransform(bottomNut2localRot,
  																	*bottomNut2localPos);
  *bottomNut2affine*=*assemAffine;  
  G4ThreeVector *bottomNut2globalPos = new G4ThreeVector(bottomNut2affine->NetTranslation());
  G4RotationMatrix *bottomNut2globalRot= new G4RotationMatrix(bottomNut2affine->NetRotation());
  fBottomNut2Ptr->Place(bottomNut2globalPos, bottomNut2globalRot, motherLogical);
  delete bottomNut2localPos;
  delete bottomNut2localRot;
  delete bottomNut2affine;

  G4ThreeVector *bottomNut3localPos = 
             new G4ThreeVector(postRadius*cos(240*deg), postRadius*sin(240*deg), bottomNutZ);   
  G4RotationMatrix *bottomNut3localRot = new G4RotationMatrix();
  G4AffineTransform *bottomNut3affine = new G4AffineTransform(bottomNut3localRot,
  																	*bottomNut3localPos);
  *bottomNut3affine*=*assemAffine;  
  G4ThreeVector *bottomNut3globalPos = new G4ThreeVector(bottomNut3affine->NetTranslation());
  G4RotationMatrix *bottomNut3globalRot= new G4RotationMatrix(bottomNut3affine->NetRotation());
  fBottomNut3Ptr->Place(bottomNut3globalPos, bottomNut3globalRot, motherLogical);
  delete bottomNut3localPos;
  delete bottomNut3localRot;
  delete bottomNut3affine;
  
  delete assemAffine;
}
