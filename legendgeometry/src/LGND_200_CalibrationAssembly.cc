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
// $Id: LGND_200_BaselineMessenger.cc,v 1.4 2005/03/31 01:02:29 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  LGND_200_CalibrationAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * follow GERDA design
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R. Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION: May 2018
 * 
 * REVISION:
 *
 * 05-2018, Created, R. Massarczyk
 */
//---------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "legendgeometry/LGND_200_CalibrationAssembly.hh"
#include "legendgeometry/LGND_200_CalibrationTaAbsorber.hh"
#include "legendgeometry/LGND_200_CalibrationCuAbsorber.hh"
#include "legendgeometry/LGND_200_CalibrationSourceInner.hh"
#include "legendgeometry/LGND_200_CalibrationSourceOuter.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

LGND_200_CalibrationAssembly::LGND_200_CalibrationAssembly(G4String fName, G4String serialNumber) :
  LGND_Assembly(fName, serialNumber, "Calibration")
{
	fSourceNumber = 0;
	fSourcePosition = new G4ThreeVector(0,0,0);
	fAbsorber = false;
}

LGND_200_CalibrationAssembly::LGND_200_CalibrationAssembly(const LGND_200_CalibrationAssembly & rhs) : 
  LGND_Assembly(rhs)
{

}

LGND_200_CalibrationAssembly::~LGND_200_CalibrationAssembly()
{

}

void LGND_200_CalibrationAssembly::Place(G4ThreeVector* assemPosition, 
				      G4RotationMatrix* assemRotation,
				      G4LogicalVolume* motherLogical){
	G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

 	G4double radius;
	G4double angle;
	if(fSourceNumber){
		switch(fSourceNumber){
			case 1:
				radius = 141*mm;
				angle = 120*deg;
				break;
			case 2:
				radius = 141*mm;
				angle = 60*deg;
				break;
			case 3:
				radius = 141*mm;
				angle = 0*deg;
				break;
			case 4:
				radius = 141*mm;
				angle = 300*deg;
				break;
			case 5:
				radius = 141*mm;
				angle = 240*deg;
				break;
			case 6:
				radius = 141*mm;
				angle = 180*deg;
				break;
			case 11:
				radius = 107*mm;
				angle = 30*deg;
				break;
			case 12:
				radius = 107*mm;
				angle = 150*deg;
				break;
			case 13:
				radius = 107*mm;
				angle = 0*deg;
				break;
			case 21:
				radius = 170*mm;
				angle = 120*deg;
				break;
			case 22:
				radius = 170*mm;
				angle =  0*deg;
				break;
			case 23:
				radius = 170*mm;
				angle = 240*deg;
				break;
			default:
				radius = 141*mm;
				angle = 0*deg;
		}

		fSourcePosition->setX(radius*sin(angle));
		fSourcePosition->setY(radius*cos(angle));

		
		MGLog(routine) << "--> Calibration Source " << fSourceNumber << " used" << endlog;
		MGLog(routine) << "--> Position X = " << fSourcePosition->x()/CLHEP::mm << " mm "  << endlog;
		MGLog(routine) << "--> Position Y = " << fSourcePosition->y()/CLHEP::mm << " mm "  << endlog;
		MGLog(routine) << "--> Position Z = " << fSourcePosition->z()/CLHEP::mm << " mm "  << endlog;

		//Ta absorber
 		fLGND_200_CalibrationTaAbsorberPtr = new LGND_200_CalibrationTaAbsorber("LGND_200_CalibrationTaAbsorber","000-0000");
  	G4ThreeVector *LGND_200_CalibrationTaAbsorberlocalPos = new G4ThreeVector(0,0,0);
		LGND_200_CalibrationTaAbsorberlocalPos->setX(fSourcePosition->x());
		LGND_200_CalibrationTaAbsorberlocalPos->setY(fSourcePosition->y());
		LGND_200_CalibrationTaAbsorberlocalPos->setZ(fSourcePosition->z()-35.1*mm);
		MGLog(debugging) << "--> Position X = " << LGND_200_CalibrationTaAbsorberlocalPos->x()/CLHEP::mm << " mm "  << endlog;
		MGLog(debugging) << "--> Position Y = " << LGND_200_CalibrationTaAbsorberlocalPos->y()/CLHEP::mm << " mm "  << endlog;
		MGLog(debugging) << "--> Position Z = " << LGND_200_CalibrationTaAbsorberlocalPos->z()/CLHEP::mm << " mm "  << endlog;
  	G4RotationMatrix *LGND_200_CalibrationTaAbsorberlocalRot = new G4RotationMatrix();
	  G4AffineTransform *LGND_200_CalibrationTaAbsorberaffine = new G4AffineTransform(LGND_200_CalibrationTaAbsorberlocalRot,
  																																	               *LGND_200_CalibrationTaAbsorberlocalPos);
  	*LGND_200_CalibrationTaAbsorberaffine*=*assemAffine;
  	G4ThreeVector *LGND_200_CalibrationTaAbsorberglobalPos = new G4ThreeVector(LGND_200_CalibrationTaAbsorberaffine->NetTranslation());
	  G4RotationMatrix *LGND_200_CalibrationTaAbsorberglobalRot= new G4RotationMatrix(LGND_200_CalibrationTaAbsorberaffine->NetRotation());
	  fLGND_200_CalibrationTaAbsorberPtr->Place(LGND_200_CalibrationTaAbsorberglobalPos, LGND_200_CalibrationTaAbsorberglobalRot, motherLogical);
  	delete LGND_200_CalibrationTaAbsorberlocalPos;
  	delete LGND_200_CalibrationTaAbsorberlocalRot;
  	delete LGND_200_CalibrationTaAbsorberaffine;

		//Inner Source , this is the reference volume for positioning
 		fLGND_200_CalibrationSourceInnerPtr = new LGND_200_CalibrationSourceInner("LGND_200_CalibrationSourceInner","000-0000");
  	G4ThreeVector *LGND_200_CalibrationSourceInnerlocalPos = new G4ThreeVector(0,0,0);
		LGND_200_CalibrationSourceInnerlocalPos->setX(fSourcePosition->x());
		LGND_200_CalibrationSourceInnerlocalPos->setY(fSourcePosition->y());
		LGND_200_CalibrationSourceInnerlocalPos->setZ(fSourcePosition->z());
		MGLog(debugging) << "--> Position X = " << LGND_200_CalibrationSourceInnerlocalPos->x()/CLHEP::mm << " mm "  << endlog;
		MGLog(debugging) << "--> Position Y = " << LGND_200_CalibrationSourceInnerlocalPos->y()/CLHEP::mm << " mm "  << endlog;
		MGLog(debugging) << "--> Position Z = " << LGND_200_CalibrationSourceInnerlocalPos->z()/CLHEP::mm << " mm "  << endlog;
  	G4RotationMatrix *LGND_200_CalibrationSourceInnerlocalRot = new G4RotationMatrix();
	  G4AffineTransform *LGND_200_CalibrationSourceInneraffine = new G4AffineTransform(LGND_200_CalibrationSourceInnerlocalRot,
  																																	               *LGND_200_CalibrationSourceInnerlocalPos);
  	*LGND_200_CalibrationSourceInneraffine*=*assemAffine;
  	G4ThreeVector *LGND_200_CalibrationSourceInnerglobalPos = new G4ThreeVector(LGND_200_CalibrationSourceInneraffine->NetTranslation());
	  G4RotationMatrix *LGND_200_CalibrationSourceInnerglobalRot= new G4RotationMatrix(LGND_200_CalibrationSourceInneraffine->NetRotation());
	  fLGND_200_CalibrationSourceInnerPtr->Place(LGND_200_CalibrationSourceInnerglobalPos, LGND_200_CalibrationSourceInnerglobalRot, motherLogical);
  	delete LGND_200_CalibrationSourceInnerlocalPos;
  	delete LGND_200_CalibrationSourceInnerlocalRot;
  	delete LGND_200_CalibrationSourceInneraffine;

		//Outer Source
 		fLGND_200_CalibrationSourceOuterPtr = new LGND_200_CalibrationSourceOuter("LGND_200_CalibrationSourceOuter","000-0000");
  	G4ThreeVector *LGND_200_CalibrationSourceOuterlocalPos = new G4ThreeVector(0,0,0);
		LGND_200_CalibrationSourceOuterlocalPos->setX(fSourcePosition->x());
		LGND_200_CalibrationSourceOuterlocalPos->setY(fSourcePosition->y());
		LGND_200_CalibrationSourceOuterlocalPos->setZ(fSourcePosition->z()-4.15*mm);
		MGLog(debugging) << "--> Position X = " << LGND_200_CalibrationSourceOuterlocalPos->x()/CLHEP::mm << " mm "  << endlog;
		MGLog(debugging) << "--> Position Y = " << LGND_200_CalibrationSourceOuterlocalPos->y()/CLHEP::mm << " mm "  << endlog;
		MGLog(debugging) << "--> Position Z = " << LGND_200_CalibrationSourceOuterlocalPos->z()/CLHEP::mm << " mm "  << endlog;
  	G4RotationMatrix *LGND_200_CalibrationSourceOuterlocalRot = new G4RotationMatrix();
	  G4AffineTransform *LGND_200_CalibrationSourceOuteraffine = new G4AffineTransform(LGND_200_CalibrationSourceOuterlocalRot,
  																																	               *LGND_200_CalibrationSourceOuterlocalPos);
  	*LGND_200_CalibrationSourceOuteraffine*=*assemAffine;
  	G4ThreeVector *LGND_200_CalibrationSourceOuterglobalPos = new G4ThreeVector(LGND_200_CalibrationSourceOuteraffine->NetTranslation());
	  G4RotationMatrix *LGND_200_CalibrationSourceOuterglobalRot= new G4RotationMatrix(LGND_200_CalibrationSourceOuteraffine->NetRotation());
	  fLGND_200_CalibrationSourceOuterPtr->Place(LGND_200_CalibrationSourceOuterglobalPos, LGND_200_CalibrationSourceOuterglobalRot, motherLogical);
  	delete LGND_200_CalibrationSourceOuterlocalPos;
  	delete LGND_200_CalibrationSourceOuterlocalRot;
  	delete LGND_200_CalibrationSourceOuteraffine;

		if(fAbsorber){
			//Cu absorber
	 		fLGND_200_CalibrationCuAbsorberPtr = new LGND_200_CalibrationCuAbsorber("LGND_200_CalibrationCuAbsorber","000-0000");
  		G4ThreeVector *LGND_200_CalibrationCuAbsorberlocalPos = new G4ThreeVector(0,0,0);
			LGND_200_CalibrationCuAbsorberlocalPos->setX(fSourcePosition->x());
			LGND_200_CalibrationCuAbsorberlocalPos->setY(fSourcePosition->y());
			LGND_200_CalibrationCuAbsorberlocalPos->setZ(fSourcePosition->z()+1.65*mm);
			MGLog(debugging) << "--> Position X = " << LGND_200_CalibrationCuAbsorberlocalPos->x()/CLHEP::mm << " mm "  << endlog;
			MGLog(debugging) << "--> Position Y = " << LGND_200_CalibrationCuAbsorberlocalPos->y()/CLHEP::mm << " mm "  << endlog;
			MGLog(debugging) << "--> Position Z = " << LGND_200_CalibrationCuAbsorberlocalPos->z()/CLHEP::mm << " mm "  << endlog;
  		G4RotationMatrix *LGND_200_CalibrationCuAbsorberlocalRot = new G4RotationMatrix();
		  G4AffineTransform *LGND_200_CalibrationCuAbsorberaffine = new G4AffineTransform(LGND_200_CalibrationCuAbsorberlocalRot,
  																																		               *LGND_200_CalibrationCuAbsorberlocalPos);
  		*LGND_200_CalibrationCuAbsorberaffine*=*assemAffine;
  		G4ThreeVector *LGND_200_CalibrationCuAbsorberglobalPos = new G4ThreeVector(LGND_200_CalibrationCuAbsorberaffine->NetTranslation());
		  G4RotationMatrix *LGND_200_CalibrationCuAbsorberglobalRot= new G4RotationMatrix(LGND_200_CalibrationCuAbsorberaffine->NetRotation());
		  fLGND_200_CalibrationCuAbsorberPtr->Place(LGND_200_CalibrationCuAbsorberglobalPos, LGND_200_CalibrationCuAbsorberglobalRot, motherLogical);
  		delete LGND_200_CalibrationCuAbsorberlocalPos;
  		delete LGND_200_CalibrationCuAbsorberlocalRot;
  		delete LGND_200_CalibrationCuAbsorberaffine;
		}
		
	}



}
