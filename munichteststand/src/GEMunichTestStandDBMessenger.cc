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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 02-06-2005
 *
 * REVISION: MM-DD-YYYY
 *
 * 05-03-2005, Added a / to the crystaltype command, Luciano
 * 12-07-2006 & 01-15-2007, added Macro-Commands for "Collimator", Manuela
 * 10-23-2009, new command is added to set the collimator length in
 *               the macro, /MG/geometry/teststand/collimatorhalflength, Neslihan.
 * 10-14-2010, 1. Added parameters of the SourceHolderStick in /MG/geometry/teststand/:
 *                use(bool) and length (default = 11cm)
 *             2. Changed the way the source is position with respect to the vacuum can
                  in SiegfriedTestStand (K1)
 *             Alex.
 *
 */

// ---------------------------------------------------------------------------

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh" 
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

#include "munichteststand/GEMunichTestStandDBMessenger.hh"
#include "munichteststand/GEMunichTestStandDB.hh" 

// --------------------------------------------------------------------------- 

GEMunichTestStandDBMessenger::GEMunichTestStandDBMessenger(GEMunichTestStandDB *teststanddb) : fTestStandDB(teststanddb) 
{

  // directories 
  
  fTestStandDir =  new G4UIdirectory("/MG/geometry/teststand/"); 
  fTestStandDir -> SetGuidance("Tune teststand parameters for Munich test stand"); 

  // directory: /MG/geometry/teststand/

  fTestStandTypeCmd =  new G4UIcmdWithAString("/MG/geometry/teststand/teststandtype", this); 
  fTestStandTypeCmd -> SetParameterName("TestStandType", true); 
  fTestStandTypeCmd -> SetDefaultValue("ln2"); 
  G4String schemacandidates;
  schemacandidates = "ln2 vacuum simple coincidence gerdalinchen2 ";
  schemacandidates += "siegfried collimator siegfriedcoincidence milchkanne galatea";
  fTestStandTypeCmd -> SetCandidates(schemacandidates);
  //fTestStandTypeCmd -> SetCandidates("ln2 vacuum simple coincidence gerdalinchen2 siegfried collimator"); 

  fTestStandCrystalHeightCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/crystalheight", this); 
  fTestStandCrystalHeightCmd -> SetParameterName("CrystalHeight", true); 
  fTestStandCrystalHeightCmd -> SetDefaultValue(8.0); 
  fTestStandCrystalHeightCmd -> SetDefaultUnit("cm"); 

  fTestStandCrystalInnerRadiusCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/crystalinnerradius", this); 
  fTestStandCrystalInnerRadiusCmd -> SetParameterName("CrystalInnerRadius", true); 
  fTestStandCrystalInnerRadiusCmd -> SetDefaultValue(2.0); 
  fTestStandCrystalInnerRadiusCmd -> SetDefaultUnit("cm"); 

  fTestStandCrystalOuterRadiusCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/crystalouterradius", this); 
  fTestStandCrystalOuterRadiusCmd -> SetParameterName("CrystalOuterRadius", true); 
  fTestStandCrystalOuterRadiusCmd -> SetDefaultValue(2.0); 
  fTestStandCrystalOuterRadiusCmd -> SetDefaultUnit("cm"); 

  fTestStandCrystalDeadLayerCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/crystaldeadlayer", this); 
  fTestStandCrystalDeadLayerCmd -> SetParameterName("CrystalDeadLayer", true); 
  fTestStandCrystalDeadLayerCmd -> SetDefaultValue(0.1); 
  fTestStandCrystalDeadLayerCmd -> SetDefaultUnit("mm"); 

  fTestStandDepthOfSourceCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/depthofsource", this); 
  fTestStandDepthOfSourceCmd -> SetParameterName("DepthOfSource", true); 
  fTestStandDepthOfSourceCmd -> SetDefaultValue(9.9); 
  fTestStandDepthOfSourceCmd -> SetDefaultUnit("cm"); 

  fTestStandDepthOfCrystalCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/depthofcrystal", this); 
  fTestStandDepthOfCrystalCmd -> SetParameterName("DepthOfCrystal", true); 
  fTestStandDepthOfCrystalCmd -> SetDefaultValue(6.0); 
  fTestStandDepthOfCrystalCmd -> SetDefaultUnit("cm"); 

  fTestStandSourceHole =  new G4UIcmdWithAnInteger("/MG/geometry/teststand/sourcehole", this); 
  fTestStandSourceHole -> SetParameterName("SourceHole", true);
  fTestStandSourceHole -> SetDefaultValue(0); 

  fTestStandCrystalType = new G4UIcmdWithAString("/MG/geometry/teststand/crystaltype", this); 
  fTestStandCrystalType -> SetParameterName("CrystalType", true); 
  fTestStandCrystalType -> SetDefaultValue("DSGunsegmented"); 
  fTestStandCrystalType -> SetCandidates("DSGunsegmented DSG segmented"); 

  fTestStandCrystalDistanceToSourceCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/crystaltosource", this); 
  fTestStandCrystalDistanceToSourceCmd -> SetParameterName("CrystalDistanceToSource", true); 
  fTestStandCrystalDistanceToSourceCmd -> SetDefaultValue(3.0); 
  fTestStandCrystalDistanceToSourceCmd -> SetDefaultUnit("cm"); 

  fTestStandSourceDistanceToBrickCmd =  new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/sourcetobrick", this); 
  fTestStandSourceDistanceToBrickCmd -> SetParameterName("SourceDistanceToBrick", true); 
  fTestStandSourceDistanceToBrickCmd -> SetDefaultValue(3.0); 
  fTestStandSourceDistanceToBrickCmd -> SetDefaultUnit("cm"); 

  fTestStandSiegfriedUseCollimatorCmd = new G4UIcmdWithABool("/MG/geometry/teststand/siegfriedusecollimator", this);
  fTestStandSiegfriedUseCollimatorCmd -> SetGuidance("Use Source Collimator or not");
  fTestStandSiegfriedUseCollimatorCmd -> AvailableForStates(G4State_PreInit);

  fTestStandSiegfriedSourcezPositionCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/siegfriedsourcezposition", this);
  fTestStandSiegfriedSourcezPositionCmd -> SetParameterName("SiegfriedSourcezPosition", true);
  fTestStandSiegfriedSourcezPositionCmd -> SetDefaultValue(12.0);
  fTestStandSiegfriedSourcezPositionCmd -> SetDefaultUnit("cm");

  fTestStandSiegfriedSourceToCanTopCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/siegfriedsourcetocantop", this);
  fTestStandSiegfriedSourceToCanTopCmd -> SetParameterName("SiegfriedSourceToCanTop", true);
  fTestStandSiegfriedSourceToCanTopCmd -> SetDefaultValue(5.0);
  fTestStandSiegfriedSourceToCanTopCmd -> SetDefaultUnit("cm");

  fTestStandSiegfriedSourcerPositionCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/siegfriedsourcerposition", this);
  fTestStandSiegfriedSourcerPositionCmd -> SetParameterName("SiegfriedSourcerPosition", true);
  fTestStandSiegfriedSourcerPositionCmd -> SetDefaultValue(2.0);
  fTestStandSiegfriedSourcerPositionCmd -> SetDefaultUnit("cm");

  fTestStandSiegfriedSourceToCanSideCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/siegfriedsourcetocanside", this);
  fTestStandSiegfriedSourceToCanSideCmd -> SetParameterName("SiegfriedSourceToCanside", true);
  fTestStandSiegfriedSourceToCanSideCmd -> SetDefaultValue(1.0);
  fTestStandSiegfriedSourceToCanSideCmd -> SetDefaultUnit("cm");
  
  fTestStandSiegfriedSourcephiPositionCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/siegfriedsourcephiposition", this);
  fTestStandSiegfriedSourcephiPositionCmd -> SetParameterName("SiegfriedSourcephiPosition", true);
  fTestStandSiegfriedSourcephiPositionCmd -> SetDefaultValue(0.0);
  fTestStandSiegfriedSourcephiPositionCmd -> SetDefaultUnit("deg");

  fTestStandSiegfriedUseSourceHolderStickCmd = new G4UIcmdWithABool("/MG/geometry/teststand/siegfriedusesourceholderstick", this);
  fTestStandSiegfriedUseSourceHolderStickCmd -> SetGuidance("Use source holder stick or not");
  fTestStandSiegfriedUseSourceHolderStickCmd -> SetDefaultValue(false);
  fTestStandSiegfriedUseSourceHolderStickCmd -> AvailableForStates(G4State_PreInit);

  fTestStandSiegfriedSourceHolderStickLengthCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/siegfriedsourceholdersticklength", this);
  fTestStandSiegfriedSourceHolderStickLengthCmd -> SetParameterName("SiegfriedSourceHolderStickLength", true);
  fTestStandSiegfriedSourceHolderStickLengthCmd -> SetDefaultValue(11.0);
  fTestStandSiegfriedSourceHolderStickLengthCmd -> SetDefaultUnit("cm");


  fTestStandSiegfriedREGeDistanceCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/siegfriedregedistance", this);
  fTestStandSiegfriedREGeDistanceCmd -> SetParameterName("SiegfriedREGeDistance", true);
  fTestStandSiegfriedREGeDistanceCmd -> SetDefaultValue(20.0);
  fTestStandSiegfriedREGeDistanceCmd -> SetDefaultUnit("cm");

  fTestStandCollimatorCollimatorPositionCmd = new G4UIcmdWithAnInteger("/MG/geometry/teststand/collimatorposition", this);
  fTestStandCollimatorCollimatorPositionCmd -> SetParameterName("CollimatorPosition",true);
  fTestStandCollimatorCollimatorPositionCmd -> SetDefaultValue(1);

  fTestStandCollimatorCollimatorInnerRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/collimatorinnerradius", this);
  fTestStandCollimatorCollimatorInnerRadiusCmd -> SetParameterName("CollimatorInnerRadius",true);
  fTestStandCollimatorCollimatorInnerRadiusCmd -> SetDefaultValue(1.0);
  fTestStandCollimatorCollimatorInnerRadiusCmd -> SetDefaultUnit("mm");

  fTestStandCollimatorCollimatorOuterRadiusCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/collimatorouterradius", this);
  fTestStandCollimatorCollimatorOuterRadiusCmd -> SetParameterName("CollimatorOuterRadius",true);
  fTestStandCollimatorCollimatorOuterRadiusCmd -> SetDefaultValue(15.0);
  fTestStandCollimatorCollimatorOuterRadiusCmd -> SetDefaultUnit("mm");

  fTestStandCollimatorCollimatorLowerHeightCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/collimatorlowerheight", this);
  fTestStandCollimatorCollimatorLowerHeightCmd -> SetParameterName("CollimatorLowerHeight",true);
  fTestStandCollimatorCollimatorLowerHeightCmd -> SetDefaultValue(50.0);
  fTestStandCollimatorCollimatorLowerHeightCmd -> SetDefaultUnit("mm");
  
  //// new parameter to set the collimator length ///
  fTestStandCollimatorCollimatorHalfLengthCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/collimatorhalflength", this);
  fTestStandCollimatorCollimatorHalfLengthCmd -> SetParameterName("CollimatorHalfLength",true);
  fTestStandCollimatorCollimatorHalfLengthCmd -> SetDefaultValue(25.0);
  fTestStandCollimatorCollimatorHalfLengthCmd -> SetDefaultUnit("mm");
  ////

  fTestStandCollimatorCollimatorDetectorDistanceCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/collimatordetectordistance", this);
 fTestStandCollimatorCollimatorDetectorDistanceCmd -> SetParameterName("CollimatorDetectorDistance",true);
 fTestStandCollimatorCollimatorDetectorDistanceCmd -> SetDefaultValue(25.0);
 fTestStandCollimatorCollimatorDetectorDistanceCmd -> SetDefaultUnit("mm");

 
 fTestStandCollimatorSystemRotationCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/collimatorsystemrotation", this);
 fTestStandCollimatorSystemRotationCmd -> SetParameterName("CollimatorSystemRotation",true);
 fTestStandCollimatorSystemRotationCmd -> SetDefaultValue(0.0);
 fTestStandCollimatorSystemRotationCmd -> SetDefaultUnit("deg");

 fTestStandCollimatorDeltaCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/collimatordelta", this);
 fTestStandCollimatorDeltaCmd -> SetParameterName("CollimatorSystemR",true);
 fTestStandCollimatorDeltaCmd -> SetDefaultValue(0.0); 
 fTestStandCollimatorDeltaCmd -> SetDefaultUnit("mm");

 fTestStandCollimatorMaterialCmd = new G4UIcmdWithAString("/MG/geometry/teststand/collimatormaterial", this); 
 fTestStandCollimatorMaterialCmd -> SetParameterName("CollimatorMaterial", true); 
 fTestStandCollimatorMaterialCmd -> SetDefaultValue("Lead"); 
 fTestStandCollimatorMaterialCmd -> SetCandidates("Copper Lead Tungsten Vacuum"); 


 fTestStandCollimatorTypeCmd = new G4UIcmdWithAString("/MG/geometry/teststand/collimatortype", this); 
 fTestStandCollimatorTypeCmd -> SetParameterName("CollimatorType", true); 
 fTestStandCollimatorTypeCmd -> SetDefaultValue("Tube"); 
 fTestStandCollimatorTypeCmd -> SetCandidates("Tube Conical Slices"); 


 //Milchkanne 
 fMilchkanneSourceXPositionCmd = new G4UIcmdWithADoubleAndUnit("/MG/geometry/teststand/sourcedistance", this);
 fMilchkanneSourceXPositionCmd -> SetParameterName("SourceDistance",true);
 fMilchkanneSourceXPositionCmd -> SetDefaultValue(20.0); 
 fMilchkanneSourceXPositionCmd -> SetDefaultUnit("cm");




}

// --------------------------------------------------------------------------- 

GEMunichTestStandDBMessenger::~GEMunichTestStandDBMessenger()
{

  delete fTestStandDir; 

  delete fTestStandTypeCmd; 
  delete fTestStandCrystalHeightCmd; 
  delete fTestStandCrystalInnerRadiusCmd; 
  delete fTestStandCrystalOuterRadiusCmd; 
  delete fTestStandCrystalDeadLayerCmd; 
  delete fTestStandDepthOfSourceCmd; 
  delete fTestStandDepthOfCrystalCmd; 
  delete fTestStandCrystalDistanceToSourceCmd; 
  delete fTestStandSourceDistanceToBrickCmd; 
  delete fTestStandSourceHole; 
  delete fTestStandCrystalType; 

  delete fTestStandSiegfriedUseCollimatorCmd;
  delete fTestStandSiegfriedSourcezPositionCmd;
  delete fTestStandSiegfriedSourceToCanTopCmd;
  delete fTestStandSiegfriedSourcerPositionCmd;
  delete fTestStandSiegfriedSourceToCanSideCmd;
  delete fTestStandSiegfriedSourcephiPositionCmd;

  delete fTestStandSiegfriedUseSourceHolderStickCmd;
  delete fTestStandSiegfriedSourceHolderStickLengthCmd;

  delete fTestStandSiegfriedREGeDistanceCmd;

  delete  fTestStandCollimatorCollimatorPositionCmd;
  delete  fTestStandCollimatorCollimatorInnerRadiusCmd;
  delete  fTestStandCollimatorCollimatorOuterRadiusCmd;
  delete  fTestStandCollimatorCollimatorLowerHeightCmd;
  delete  fTestStandCollimatorCollimatorHalfLengthCmd;
  delete  fTestStandCollimatorCollimatorDetectorDistanceCmd;
  delete  fTestStandCollimatorSystemRotationCmd;
  delete  fTestStandCollimatorDeltaCmd;
  delete  fTestStandCollimatorMaterialCmd;
  delete  fTestStandCollimatorTypeCmd; 

  delete  fMilchkanneSourceXPositionCmd;
}

// --------------------------------------------------------------------------- 

void GEMunichTestStandDBMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
{

  // directory: /MG/geometry/teststand/

  if (cmd == fTestStandTypeCmd)
    {
      fTestStandDB -> SetTestStandType(newvalue);
    }
 

  else if (cmd == fTestStandCrystalHeightCmd)
    {
      G4double h = fTestStandCrystalHeightCmd -> GetNewDoubleValue(newvalue); 
      
      fTestStandDB -> SetCrystalHeight(h); 
    }

  else if (cmd == fTestStandCrystalInnerRadiusCmd)
    {
      G4double h = fTestStandCrystalInnerRadiusCmd -> GetNewDoubleValue(newvalue); 
      
      fTestStandDB -> SetCrystalInnerRadius(h); 
    }

  else if (cmd == fTestStandCrystalOuterRadiusCmd)
    {
      G4double h = fTestStandCrystalOuterRadiusCmd -> GetNewDoubleValue(newvalue); 
      
      fTestStandDB -> SetCrystalOuterRadius(h); 
    }

  else if (cmd == fTestStandCrystalDeadLayerCmd)
    {
      G4double d = fTestStandCrystalDeadLayerCmd -> GetNewDoubleValue(newvalue); 
      
      fTestStandDB -> SetCrystalDeadLayer(d); 
    }

  else if (cmd == fTestStandDepthOfSourceCmd)
    {
      G4double d = fTestStandDepthOfSourceCmd -> GetNewDoubleValue(newvalue); 
      
      fTestStandDB -> SetStringDetectorHeight(d); 
    }

  else if (cmd == fTestStandDepthOfCrystalCmd)
    {
      G4double d = fTestStandDepthOfCrystalCmd -> GetNewDoubleValue(newvalue); 
      
      fTestStandDB -> SetStringSourceHeight(d); 
    }

  else if (cmd == fTestStandCrystalDistanceToSourceCmd)
    {
      G4double d = fTestStandCrystalDistanceToSourceCmd -> GetNewDoubleValue(newvalue); 
      
      fTestStandDB -> SetCrystalDistanceToSource(d); 
    }

  else if (cmd == fTestStandSourceDistanceToBrickCmd)
    {
      G4double d = fTestStandSourceDistanceToBrickCmd -> GetNewDoubleValue(newvalue); 
      
      fTestStandDB -> SetSourceDistanceToBrick(d); 
    }

  else if (cmd == fTestStandSourceHole)
    {
      G4int n = fTestStandSourceHole -> GetNewIntValue(newvalue); 
      
      fTestStandDB -> SetSourceHoleNumber(n); 
    }

  else if (cmd == fTestStandCrystalType)
    {
      fTestStandDB -> SetCrystalType(newvalue); 
    }

  else if (cmd == fTestStandSiegfriedUseCollimatorCmd)
    {
      G4bool d = fTestStandSiegfriedUseCollimatorCmd -> GetNewBoolValue(newvalue); 
      fTestStandDB -> SetSiegfriedCollimatorStatus(d);
    }

  else if (cmd == fTestStandSiegfriedSourcezPositionCmd)
    {
      G4double d = fTestStandSiegfriedSourcezPositionCmd -> GetNewDoubleValue(newvalue); 
      fTestStandDB -> SetSiegfriedSourceSystemZ(d);
    }

  else if (cmd == fTestStandSiegfriedSourceToCanTopCmd)
    {
      G4double d = fTestStandSiegfriedSourceToCanTopCmd -> GetNewDoubleValue(newvalue); 
      fTestStandDB -> SetSiegfriedSourceSystemToCanTop(d);
    }

  else if (cmd == fTestStandSiegfriedSourcerPositionCmd)
    {
      G4double d = fTestStandSiegfriedSourcerPositionCmd -> GetNewDoubleValue(newvalue); 
      fTestStandDB -> SetSiegfriedSourceSystemR(d);
    }

  else if (cmd == fTestStandSiegfriedSourceToCanSideCmd)
    {
      G4double d = fTestStandSiegfriedSourceToCanSideCmd -> GetNewDoubleValue(newvalue); 
      fTestStandDB -> SetSiegfriedSourceSystemToCanSide(d);
    }

  else if (cmd == fTestStandSiegfriedSourcephiPositionCmd)
    {
      G4double d = fTestStandSiegfriedSourcephiPositionCmd -> GetNewDoubleValue(newvalue); 
      fTestStandDB -> SetSiegfriedSourceSystemPhi(d);
    }

  else if (cmd == fTestStandSiegfriedUseSourceHolderStickCmd)
    {
      G4bool d = fTestStandSiegfriedUseSourceHolderStickCmd -> GetNewBoolValue(newvalue); 
      fTestStandDB -> SetSiegfriedSourceHolderStickStatus(d);
    }
  else if (cmd == fTestStandSiegfriedSourceHolderStickLengthCmd)
    {
      G4double d = fTestStandSiegfriedSourceHolderStickLengthCmd -> GetNewDoubleValue(newvalue); 
      fTestStandDB -> SetSiegfriedSourceHolderStickLength(d);
    }

  else if (cmd == fTestStandSiegfriedREGeDistanceCmd)
    {
     G4double d = fTestStandSiegfriedREGeDistanceCmd -> GetNewDoubleValue(newvalue);
     fTestStandDB -> SetSiegfriedREGeToCanSurfaceDistance(d);
    }

  //Collimator
  else if (cmd == fTestStandCollimatorCollimatorPositionCmd)
  {
   G4int collimatorposition = fTestStandCollimatorCollimatorPositionCmd -> GetNewIntValue(newvalue);
   fTestStandDB -> SetCollimatorPosition(collimatorposition);
    }
  
  else if (cmd == fTestStandCollimatorCollimatorInnerRadiusCmd)
  {
  G4double d = fTestStandCollimatorCollimatorInnerRadiusCmd -> GetNewDoubleValue(newvalue);
  fTestStandDB -> SetCollimatorInnerRadius(d);
  }

  else if (cmd == fTestStandCollimatorCollimatorOuterRadiusCmd)
  {
  G4double d = fTestStandCollimatorCollimatorOuterRadiusCmd -> GetNewDoubleValue(newvalue);
  fTestStandDB -> SetCollimatorOuterRadius(d);
  }

  else if (cmd == fTestStandCollimatorCollimatorLowerHeightCmd)
  {
  G4double d = fTestStandCollimatorCollimatorLowerHeightCmd -> GetNewDoubleValue(newvalue);
  fTestStandDB -> SetCollimatorLowerHeight(d);
  }
  
  //// new parameter to set the collimator length ///
  else if (cmd == fTestStandCollimatorCollimatorHalfLengthCmd)
  {
  G4double d = fTestStandCollimatorCollimatorHalfLengthCmd -> GetNewDoubleValue(newvalue);
  fTestStandDB -> SetCollimatorHalfLength(d);
  }
  ////

  else if (cmd == fTestStandCollimatorCollimatorDetectorDistanceCmd)
  {
  G4double d = fTestStandCollimatorCollimatorDetectorDistanceCmd -> GetNewDoubleValue(newvalue);
  fTestStandDB -> SetCollimatorDetectorDistance(d);
  }

  else if (cmd ==  fTestStandCollimatorSystemRotationCmd)
  {
  G4double d =  fTestStandCollimatorSystemRotationCmd -> GetNewDoubleValue(newvalue);
  fTestStandDB ->SetCollimatorSystemRotation(d);
  }

  else if (cmd == fTestStandCollimatorDeltaCmd)
  {
  G4double z =  fTestStandCollimatorDeltaCmd -> GetNewDoubleValue(newvalue);
  fTestStandDB ->SetCollimatorSystemR(z);
  }

  else if (cmd ==fTestStandCollimatorMaterialCmd)
  {
  fTestStandDB ->SetCollimatorMaterial(newvalue);
  }

  else if (cmd == fTestStandCollimatorTypeCmd)
  { 
  fTestStandDB -> SetCollimatorType(newvalue); 
  }

  else if (cmd == fMilchkanneSourceXPositionCmd)
  { 
    G4double d = fMilchkanneSourceXPositionCmd -> GetNewDoubleValue(newvalue); 
    fTestStandDB -> SetMkSourceXPosition(d); 
  }

}

// --------------------------------------------------------------------------- 
