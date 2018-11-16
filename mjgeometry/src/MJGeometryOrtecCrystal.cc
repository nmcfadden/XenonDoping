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
// CLASS IMPLEMENTATION:  MJGeometryOrtecCrystal.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 *
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ryan Martin
 * CONTACT: rdmartin@lbl.gov
 * FIRST SUBMISSION: Feb 26, 2013
 * 
 * REVISION:
 * 
 * 02-26-2013, Created, R. Martin
 *
 * 06-14-2013, added GetCrystalDiameter, W. Xu
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"                                                                                                                                               
#include "G4RotationMatrix.hh"
#include "G4UIcommand.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4Polycone.hh"
#include "G4Sphere.hh"
#include "G4AffineTransform.hh"
//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryOrtecCrystal.hh"


//---------------------------------------------------------------------------//
#include <vector>
#include <fstream>
#include <stdio.h>


using namespace CLHEP;


MJGeometryOrtecCrystal::MJGeometryOrtecCrystal(G4String argName)
{
	//Defaults from the PONaMa1 detector
	fCrystalDimpleNominalRadius=3.0*mm;
	fCrystalDimpleHoleDepth=2.0*mm;
	fCrystalDimpleHoleDiameter=3.2*mm;
	fDimpleRadialOffset=0.;
	fCrystalDiameter=68.9*mm;
	fCrystalLength=52*mm;
	fName=argName;
	fzPlanes.resize(0);
	frOuterPlanes.resize(0);
  fLoadGeometryFromFile=false;
}



MJGeometryOrtecCrystal::~MJGeometryOrtecCrystal()
{

}


void MJGeometryOrtecCrystal::LoadCrystalGeomFromFile(G4String argFileName)
{
  fLoadGeometryFromFile=true;
	fzPlanes.resize(0);
	frOuterPlanes.resize(0);

	ifstream infile(argFileName.c_str());
	G4double r,z,zprev=-1e6,rmax;
	rmax=0;
	while(infile.good() && !infile.eof()){
		infile>>z>>r;
	  if(infile.eof())break;
		if(r>rmax)rmax=r;//assumes the radius of the crystal is the maximal radius
		if(z==zprev)continue;
		MGLog(routine)<<"Loaded z = "<<z<<" r = "<<r<<endl;
		fzPlanes.push_back(z*mm);
		frOuterPlanes.push_back(r*mm);
	}
	infile.close();
	
	fCrystalLength=fabs(fzPlanes[0]-fzPlanes[fzPlanes.size()-1])*mm;
	fCrystalDiameter=2.0*rmax*mm;
  MGLog(routine)<<"Diameter = "<<fCrystalDiameter<< "mm, Length = "<<fCrystalLength<<"mm"<<endl;
}

void MJGeometryOrtecCrystal::SaveCrystalGeomToFile(G4String argFileName)
{
	ofstream outfile(argFileName.c_str());
	for( size_t i=0;i<fzPlanes.size();i++){
		outfile<<fzPlanes.at(i)<<" "<<frOuterPlanes.at(i)<<endl;
	}
	outfile.close();
}

void MJGeometryOrtecCrystal::Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,G4LogicalVolume *motherlogical)
{
	//The origin of the crystal is the center of the point-contact face (note that the point contact could be off center!)
	//Build the crystal using a polygon
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);


  //Build using properties: (must load from file to get a more detailed geometry, for MJ users only)
	if(!fLoadGeometryFromFile){

		//The point contact face:
		fzPlanes.push_back(0);
		frOuterPlanes.push_back(0.5*fCrystalDiameter);

		//The cylinderpart
		fzPlanes.push_back(fCrystalLength);
		frOuterPlanes.push_back(0.5*fCrystalDiameter);
	}

	//Create the polygone (crystal with no dimple yet)
	int nPlanes=fzPlanes.size();
	vector<double> rInnerPlanes(nPlanes,0);
  G4Polycone* crystalNoDimple = new G4Polycone("crystalNoDimple", 0.,2*pi,nPlanes,&fzPlanes[0],&rInnerPlanes[0],&frOuterPlanes[0]);

	//Create the dimple (cylinder with part of a sphere on the end)
	G4double dimpleStartTheta=asin(fCrystalDimpleHoleDiameter/2./fCrystalDimpleNominalRadius);
	G4Sphere* dimpleEnd = new G4Sphere("dimpleEnd",0,fCrystalDimpleNominalRadius,0,2*pi,0,dimpleStartTheta);
	G4double dimpleTubLength= fCrystalDimpleHoleDepth-fCrystalDimpleNominalRadius+sqrt(fCrystalDimpleNominalRadius*fCrystalDimpleNominalRadius-fCrystalDimpleHoleDiameter*fCrystalDimpleHoleDiameter/4);
	G4Tubs* dimpleTub= new G4Tubs("dimpleTub",0.,fCrystalDimpleHoleDiameter/2., dimpleTubLength/2.,0,2*pi); 
                 
	//Subtract the dimple to create the crystal. Note that any offset in the dimple position is forced to be in the positive x-direction

  G4SubtractionSolid* crystalWithTubDimple = new G4SubtractionSolid("crystalWithTubDimple", crystalNoDimple, dimpleTub, 0,G4ThreeVector(fDimpleRadialOffset, 0,dimpleTubLength/2.-0.001*mm));
	G4SubtractionSolid* crystal = new G4SubtractionSolid("crystal", crystalWithTubDimple, dimpleEnd, 0,G4ThreeVector(fDimpleRadialOffset, 0, fCrystalDimpleHoleDepth - fCrystalDimpleNominalRadius-0.001*mm));


	//Create the logical volume for the crystal, color it, and place it
  G4double z, a, density;
  density = 5.323*g/cm3;
  a = 76.0*g/mole;//is that right??? Does it matter?
  /*G4Material* enrGe =*/ new G4Material("enrGe", z=32., a, density);//for some reason it cannot get it from the already registered "Germaniun-Enr" (I guess it's not already registered)

	//G4LogicalVolume *crystalLogical = new G4LogicalVolume(crystal,G4Material::GetMaterial("Germanium-Enr"),fName+"_Logical");
  G4LogicalVolume *crystalLogical = new G4LogicalVolume(crystal,G4Material::GetMaterial("enrGe"),fName+"_Logical");
  crystalLogical->SetVisAttributes(new G4VisAttributes(G4Colour(0.878,0.878,0.867))); //Khang's "germanium color" :-)


  G4ThreeVector* CrystalLocalPosition = new G4ThreeVector(0.,0.,0);
  G4RotationMatrix* CrystalLocalRotation = new G4RotationMatrix();
  G4AffineTransform* CrystalAssemblyAffine = new G4AffineTransform(CrystalLocalRotation, *CrystalLocalPosition);
  *CrystalAssemblyAffine *= *assemAffine;  			
  G4ThreeVector* CrystalGlobalPosition=new G4ThreeVector(CrystalAssemblyAffine->NetTranslation());
  G4RotationMatrix* CrystalGlobalRotation= new G4RotationMatrix(CrystalAssemblyAffine->NetRotation());				
  new G4PVPlacement(CrystalGlobalRotation, *CrystalGlobalPosition,crystalLogical,fName+"_ActivePhysical",motherlogical,false,0,false);//<- needs to have "Active"


}



