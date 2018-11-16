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
// CLASS IMPLEMENTATION:  TSStringAssembly.cc
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
 * AUTHOR: Ryan Martin
 * FIRST SUBMISSION: Jun 13, 2011
 * 
 * REVISION:
 * 
 * 06-13-2011, Created, R. Martin
 * 07-30-2012, Changed color of various component parts, K. Nguyen
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"
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
#include "G4SubtractionSolid.hh"


#include "tonscalegeometry/TSVisAttributes.hh"


//---------------------------------------------------------------------------//
#include "tonscalegeometry/TSCrystalAssembly.hh"
#include "io/MGLogger.hh"
#include "tonscalegeometry/TSStringAssembly.hh"
#include "mjdemonstrator/MJDemoStringAdapterPlate.hh"
#include "mjdemonstrator/MJDemoCrystalAssembly.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

TSStringAssembly::TSStringAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "MJ80-02-200")
{
 Defaults();
}

// -----------------------------------------------------------------------

TSStringAssembly::TSStringAssembly(const TSStringAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{  
  Defaults();
}

// -----------------------------------------------------------------------

void TSStringAssembly::Defaults()
{
  fnCrystals=5;
  fCrystalAssemblyType= kTS;
  fCrystalDiameter=77.*mm;
  fCrystalHeight=33.*mm;
  fSpaceBetweenTSCrystalAssemblies=5*mm;
  fColdPlateStandoffLength=1.5*cm;//similar to tie rods, stands the string off the cold plate
  fColdPlateStandoffRadius=1*mm; //similar to tie rods
  fEpsilon=0.001*mm;
}

// -----------------------------------------------------------------------

TSStringAssembly::~TSStringAssembly()
{
  delete fMJAdapterPlatePtr;
  delete[] fMJCrystalAssemblies;
  delete[] fTSCrystalAssemblies;
}

// -----------------------------------------------------------------------

void TSStringAssembly::Initialize()
{ 
  char number[10];
  G4String crystalname;
  if(fCrystalAssemblyType == kMJDemonstrator){
     fMJCrystalAssemblies = new MJDemoCrystalAssembly* [fnCrystals];
     fMJAdapterPlatePtr = new MJDemoStringAdapterPlate(fName+"_StringAdapterPlate_001", "867-5309");  
     for(int i=0;i<fnCrystals;i++){
       sprintf(number,"%.3d",i+1);
       crystalname=fName+"_CrystalAssembly_"+number;
       fMJCrystalAssemblies[i]=new MJDemoCrystalAssembly(crystalname,"867-5309");  
     }
  }
  if(fCrystalAssemblyType == kTS){
    fTSCrystalAssemblies = new TSCrystalAssembly* [fnCrystals];
    for(int i=0;i<fnCrystals;i++){
      sprintf(number,"%.3d",i+1);
      crystalname=fName+"_CrystalAssembly_"+number;
      fTSCrystalAssemblies[i]=new TSCrystalAssembly(crystalname);  
      fTSCrystalAssemblies[i]->SetCrystalDiameter(fCrystalDiameter);
      fTSCrystalAssemblies[i]->SetCrystalHeight(fCrystalHeight);
      fTSCrystalAssemblies[i]->Initialize();
    }
  }
}

// -----------------------------------------------------------------------

void TSStringAssembly::Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector **crystalAssemblylocalPos = new G4ThreeVector* [fnCrystals];
  G4RotationMatrix **crystalAssemblylocalRot = new G4RotationMatrix* [fnCrystals];
  G4AffineTransform **crystalAssemblyaffine = new G4AffineTransform* [fnCrystals];
  G4ThreeVector **crystalAssemblyglobalPos = new G4ThreeVector* [fnCrystals];
  G4RotationMatrix **crystalAssemblyglobalRot= new G4RotationMatrix* [fnCrystals];
	
  if(fCrystalAssemblyType == kMJDemonstrator){
    //adapter plate
    G4ThreeVector *stringAdapterPlate1localPos = new G4ThreeVector(0, 0, -0.175*25.4*mm);   
    G4RotationMatrix *stringAdapterPlate1localRot = new G4RotationMatrix();
    G4AffineTransform *stringAdapterPlate1affine = new G4AffineTransform(stringAdapterPlate1localRot, *stringAdapterPlate1localPos);
    *stringAdapterPlate1affine *= *assemAffine;  
    G4ThreeVector *stringAdapterPlate1globalPos = new G4ThreeVector(stringAdapterPlate1affine->NetTranslation());
    G4RotationMatrix *stringAdapterPlate1globalRot= new G4RotationMatrix(stringAdapterPlate1affine->NetRotation());
    fMJAdapterPlatePtr->Place(stringAdapterPlate1globalPos,stringAdapterPlate1globalRot, motherLogical);
	
    delete stringAdapterPlate1localPos;
    delete stringAdapterPlate1localRot;
    delete stringAdapterPlate1affine;

    //string of detector unit assemblies:
    G4double crystalSpacing = 2.057*25.4*mm; //2.056
    G4double crystal1Z = -1.8792*25.4*mm;
    G4double crystalZ =0.; 
    for(int i=0;i<fnCrystals;i++){
      crystalZ=crystal1Z-i*crystalSpacing;
		
      crystalAssemblylocalPos[i] = new G4ThreeVector(0, 0, crystalZ);
      crystalAssemblylocalRot[i] = new G4RotationMatrix();
      crystalAssemblylocalRot[i]->rotateX(pi);
		
      crystalAssemblyaffine[i] = new G4AffineTransform(crystalAssemblylocalRot[i], *crystalAssemblylocalPos[i]);
      *crystalAssemblyaffine[i] *= *assemAffine;  
      crystalAssemblyglobalPos[i] = new G4ThreeVector(crystalAssemblyaffine[i]->NetTranslation());
      crystalAssemblyglobalRot[i] = new G4RotationMatrix(crystalAssemblyaffine[i]->NetRotation());
      MGLog(routine)<<"Placing crystal assembly at (xyz, in mm)  "<<crystalAssemblyglobalPos[i]->x()/mm
                    <<" "<<crystalAssemblyglobalPos[i]->y()/mm<<" "<<crystalAssemblyglobalPos[i]->z()/mm<<endl;
      fMJCrystalAssemblies[i]->Place(crystalAssemblyglobalPos[i], crystalAssemblyglobalRot[i], motherLogical);
    }
 }
 if(fCrystalAssemblyType==kTS){
    //string of detector unit assemblies:
    G4double crystalSpacing = fTSCrystalAssemblies[0]->GetAssemblyLength()+fSpaceBetweenTSCrystalAssemblies; //2.056
    G4double crystal1Z = -2*fEpsilon-fColdPlateStandoffLength;
    G4double crystalZ =0.; 

    //3 little stand off rods to replace the adapter plate, sort of like the tie rods		
    G4Tubs *ColdPlateStandoffTub = new G4Tubs(fName+"_ColdPlateStandoffTub",0.,fColdPlateStandoffRadius,0.5*fColdPlateStandoffLength,0,2.*pi);
    G4LogicalVolume *ColdPlateStandoffLogical = new G4LogicalVolume(ColdPlateStandoffTub,G4Material::GetMaterial("Copper-EF"),
                                                                    fName+"_ColdPlateStandoffLogical");
    //ColdPlateStandoffLogical->SetVisAttributes(new TSVisAttributesSolid("red"));
    ColdPlateStandoffLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.839,0.373,0.169,1.0))); // FIXME: Changed color attribute from
                                                                                                      //        red to copper, K. Nguyen

    G4double standoffradius=fTSCrystalAssemblies[0]->GetRodPlacememtRadius();
		
    G4ThreeVector **StandofflocalPos = new G4ThreeVector* [3];
    G4RotationMatrix **StandofflocalRot = new G4RotationMatrix* [3];
    G4AffineTransform **Standoffaffine = new G4AffineTransform* [3];
    G4ThreeVector **StandoffglobalPos = new G4ThreeVector* [3];
    G4RotationMatrix **StandoffglobalRot= new G4RotationMatrix* [3];

    char rnumber[30];
    G4String rname;
    for(int i=0;i<3;i++){
      StandofflocalPos[i] = new G4ThreeVector(standoffradius*cos(i*120*deg),standoffradius*sin(i*120*deg), 
                                              -0.5*fColdPlateStandoffLength-fEpsilon);
      StandofflocalRot[i] = new G4RotationMatrix();
      Standoffaffine[i] = new G4AffineTransform(StandofflocalRot[i], *StandofflocalPos[i]);
      *Standoffaffine[i] *= *assemAffine;  
      StandoffglobalPos[i] = new G4ThreeVector(Standoffaffine[i]->NetTranslation());
      StandoffglobalRot[i] = new G4RotationMatrix(Standoffaffine[i]->NetRotation());
      sprintf(rnumber,"%.3d",i);
      rname=fName+"_ColdPlateStandoffPhysical_"+rnumber;
      new G4PVPlacement(StandoffglobalRot[i], *StandoffglobalPos[i],ColdPlateStandoffLogical,rname,motherLogical,false,0,false);
    }

    /*
    delete[] StandofflocalPos ;
    delete[] StandofflocalRot;
    delete[] Standoffaffine ;
    delete[] StandoffglobalPos ;
    delete[] StandoffglobalRot;
    */
		
    for(int i=0;i<fnCrystals;i++){
      crystalZ=crystal1Z-i*crystalSpacing;
      crystalAssemblylocalPos[i] = new G4ThreeVector(0, 0, crystalZ);
      crystalAssemblylocalRot[i] = new G4RotationMatrix();
      crystalAssemblyaffine[i] = new G4AffineTransform(crystalAssemblylocalRot[i], *crystalAssemblylocalPos[i]);
      *crystalAssemblyaffine[i] *= *assemAffine;  
      crystalAssemblyglobalPos[i] = new G4ThreeVector(crystalAssemblyaffine[i]->NetTranslation());
      crystalAssemblyglobalRot[i] = new G4RotationMatrix(crystalAssemblyaffine[i]->NetRotation());
      MGLog(routine)<<"Placing crystal assembly at "<<crystalAssemblyglobalPos[i]->x()/mm
                    <<" "<<crystalAssemblyglobalPos[i]->y()/mm<<" "<<crystalAssemblyglobalPos[i]->z()/mm<<endl;
      fTSCrystalAssemblies[i]->Place(crystalAssemblyglobalPos[i], crystalAssemblyglobalRot[i], motherLogical);
    }
  }  

  delete[] crystalAssemblylocalPos;
  delete[] crystalAssemblylocalRot;
  delete[] crystalAssemblyaffine;
  delete[] crystalAssemblyglobalPos;
  delete[] crystalAssemblyglobalRot;
		
  delete assemAffine;
}

// -----------------------------------------------------------------------

