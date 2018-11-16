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
// CLASS IMPLEMENTATION:  MJGeometryKURFBEGeDetectorMessenger.cc
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
 * AUTHOR: alexis
 * CONTACT: 
 * FIRST SUBMISSION: Fri Aug 27 07:08:08 PDT 2010
 * 
 * REVISION:
 *
 * 08-27-2010, Creation, A. Schubert
 * 08 Sep 2011, Added messenger commands for a collimator, with these
 *             one can turn it on/off and position it, P. Finnerty
 */
//---------------------------------------------------------------------------//
//

#include "G4ApplicationState.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryKURFBEGeDetector.hh"      
#include "mjgeometry/MJGeometryKURFBEGeDetectorMessenger.hh"      

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometryKURFBEGeDetectorMessenger::MJGeometryKURFBEGeDetectorMessenger(
  MJGeometryKURFBEGeDetector *kurfBEGeDetector ):
  fKURFBEGeDetector(kurfBEGeDetector)
{
  // /MG/geometry/kurfBEGeDetector
  fDirectory = new G4UIdirectory("/MG/geometry/kurfBEGeDetector/");

  // /MG/geometry/kurfBEGeDetector/cylinderAngleInUnitsOfPi
  fCylinderAngleCmd = new 
    G4UIcmdWithADouble("/MG/geometry/kurfBEGeDetector/cylinderAngleInUnitsOfPi",this);
  fCylinderAngleCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/kurfBEGeDetector/useCollimator
  fUseCollimatorCmd = new 
    G4UIcmdWithABool("/MG/geometry/kurfBEGeDetector/useCollimator",this);
  fUseCollimatorCmd->SetGuidance("True if you want to use the collimator.");  
  fUseCollimatorCmd->SetDefaultValue(false);  
  fUseCollimatorCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/kurfBEGeDetector/collimatorSideOrTop
  fCollimatorPositionCmd = new 
    G4UIcmdWithAString("/MG/geometry/kurfBEGeDetector/collimatorSideOrTop",this);
  fCollimatorPositionCmd->SetDefaultValue("top");
  fCollimatorPositionCmd->SetGuidance("side or 'top' position:");
  fCollimatorPositionCmd->SetGuidance("  side: vertical scans along side of cryostat.");
  fCollimatorPositionCmd->SetGuidance("   top: horizontal scans across top of endcap.");
  G4String candidates = "side top";
  fCollimatorPositionCmd->SetCandidates(candidates);
  fCollimatorPositionCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/kurfBEGeDetector/collimatorXPosInUnitsOf_mm
  fCollimatorXCoordinateIn_mmCmd = new 
    G4UIcmdWithADouble("/MG/geometry/kurfBEGeDetector/collimatorXPosInUnitsOf_mm",this);
  fCollimatorXCoordinateIn_mmCmd->SetDefaultValue(0.0);  
  fCollimatorXCoordinateIn_mmCmd->SetGuidance("For top position, this is the distance");
  fCollimatorXCoordinateIn_mmCmd->SetGuidance("  from the center of the collimator hole");
  fCollimatorXCoordinateIn_mmCmd->SetGuidance("  relative to the center of the cryostat in X.");
  fCollimatorXCoordinateIn_mmCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/kurfBEGeDetector/collimatorYPosInUnitsOf_mm
  fCollimatorYCoordinateIn_mmCmd = new 
    G4UIcmdWithADouble("/MG/geometry/kurfBEGeDetector/collimatorYPosInUnitsOf_mm",this);
  fCollimatorYCoordinateIn_mmCmd->SetDefaultValue(0.0);  
  fCollimatorYCoordinateIn_mmCmd->SetGuidance("For top position, this is the distance");
  fCollimatorYCoordinateIn_mmCmd->SetGuidance("  from the center of the collimator hole");
  fCollimatorYCoordinateIn_mmCmd->SetGuidance("  relative to the center of the cryostat in Y.");
  fCollimatorYCoordinateIn_mmCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/kurfBEGeDetector/collimatorZPosInUnitsOf_mm
  fCollimatorZCoordinateIn_mmCmd = new 
    G4UIcmdWithADouble("/MG/geometry/kurfBEGeDetector/collimatorZPosInUnitsOf_mm",this);
  fCollimatorZCoordinateIn_mmCmd->SetDefaultValue(20.0);  
  fCollimatorZCoordinateIn_mmCmd->SetGuidance("For top position, this is the distance");
  fCollimatorZCoordinateIn_mmCmd->SetGuidance("  from the bottom of the collimator to the");
  fCollimatorZCoordinateIn_mmCmd->SetGuidance("  endcap.");
  fCollimatorZCoordinateIn_mmCmd->SetGuidance("For side position, this is the distance");
  fCollimatorZCoordinateIn_mmCmd->SetGuidance("  from the collimator hole relative to the");
  fCollimatorZCoordinateIn_mmCmd->SetGuidance("  endcap.");
  fCollimatorZCoordinateIn_mmCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/kurfBEGeDetector/collimatorAngleInDegrees
  fCollimatorAngleIn_degCmd = new 
    G4UIcmdWithADouble("/MG/geometry/kurfBEGeDetector/collimatorAngleInDegrees",this);
  fCollimatorAngleIn_degCmd->SetDefaultValue(0.0);
  fCollimatorAngleIn_degCmd->AvailableForStates(G4State_PreInit);
  
  // /MG/geometry/kurfBEGeDetector/collimatorRPosInUnitsOf_mm
  fCollimatorRCoordinateIn_mmCmd = new 
    G4UIcmdWithADouble("/MG/geometry/kurfBEGeDetector/collimatorRPosInUnitsOf_mm",this);
  fCollimatorRCoordinateIn_mmCmd->SetGuidance("Radial distance from bottom of collimator");
  fCollimatorRCoordinateIn_mmCmd->SetGuidance("  to cryostat can.");
  fCollimatorRCoordinateIn_mmCmd->SetDefaultValue(100.0);
  fCollimatorRCoordinateIn_mmCmd->AvailableForStates(G4State_PreInit);
  
}

MJGeometryKURFBEGeDetectorMessenger::~MJGeometryKURFBEGeDetectorMessenger()
{
  delete fCylinderAngleCmd;
  delete fUseCollimatorCmd;
  delete fCollimatorPositionCmd;
  delete fCollimatorXCoordinateIn_mmCmd;
  delete fCollimatorYCoordinateIn_mmCmd;
  delete fCollimatorZCoordinateIn_mmCmd;
  delete fCollimatorRCoordinateIn_mmCmd;
  delete fCollimatorAngleIn_degCmd;
  delete fDirectory;
}

void MJGeometryKURFBEGeDetectorMessenger::SetNewValue(G4UIcommand *cmd, 
							 G4String nV)
{
  if(cmd == fCylinderAngleCmd)
    fKURFBEGeDetector->SetCylinderAngleInUnitsOfPi(
      fCylinderAngleCmd->GetNewDoubleValue(nV)
    );
    
  if(cmd == fUseCollimatorCmd) {
    fKURFBEGeDetector->SetIsCollimatorUsed( fUseCollimatorCmd->GetNewBoolValue(nV) );
  }
  
  if(cmd == fCollimatorPositionCmd) {
    G4String position = nV; 
    fKURFBEGeDetector->SetCollimatorSideOrTop(position);
  }
  
  if(cmd == fCollimatorXCoordinateIn_mmCmd)
    fKURFBEGeDetector->SetCollimatorXCoordinateIn_mm(
      fCollimatorXCoordinateIn_mmCmd->GetNewDoubleValue(nV)
    );
    
  if(cmd == fCollimatorYCoordinateIn_mmCmd)
    fKURFBEGeDetector->SetCollimatorYCoordinateIn_mm(
      fCollimatorYCoordinateIn_mmCmd->GetNewDoubleValue(nV)
    );
    
  if(cmd == fCollimatorZCoordinateIn_mmCmd)
    fKURFBEGeDetector->SetCollimatorZCoordinateIn_mm(
      fCollimatorZCoordinateIn_mmCmd->GetNewDoubleValue(nV)
    );
    
  if(cmd == fCollimatorRCoordinateIn_mmCmd)
    fKURFBEGeDetector->SetCollimatorRCoordinateIn_mm(
      fCollimatorRCoordinateIn_mmCmd->GetNewDoubleValue(nV)
    );
    
  if(cmd == fCollimatorAngleIn_degCmd)
    fKURFBEGeDetector->SetCollimatorAngleIn_deg(
      fCollimatorAngleIn_degCmd->GetNewDoubleValue(nV)
    );
}
