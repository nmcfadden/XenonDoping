//---------------------------------------------------------------------------//
//

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIdirectory.hh"

#include "mjgeometry/MJGeometryDoubleSolidBlock.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryDoubleSolidBlockMessenger.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryDoubleSolidBlockMessenger::MJGeometryDoubleSolidBlockMessenger(
   MJGeometryDoubleSolidBlock *doubleblock): fDoubleBlock(doubleblock)
{
  // needed commands:
  // SetMaterialName
  // SetEdgeLength
  // SetActive
  // SetDeadLayerDepth

  
  // /MG/geometry/doublesolidblock
  fDoubleSolidBlockDir = new G4UIdirectory("/MG/geometry/doublesolidblock/");
  fDoubleSolidBlockDir->SetGuidance(
    "Allows you to set the attributes of the 2 separate blocks in the");
  fDoubleSolidBlockDir->SetGuidance("MJGeometryDoubleSolidBlock class");

  // /MG/geometry/doublesolidblock/alphamaterial
  fAlphaMaterialSelectCmd = new G4UIcmdWithAString(
    "/MG/geometry/doublesolidblock/alphamaterial", this);
  fAlphaMaterialSelectCmd->SetGuidance("Set material of block alpha");

  // /MG/geometry/doublesolidblock/alphaedgeLength
  fAlphaSetEdgeLengthCmd = new G4UIcmdWithADoubleAndUnit(
    "/MG/geometry/doublesolidblock/alphaedgeLength", this);
  fAlphaSetEdgeLengthCmd->SetGuidance("Set the length of alpha block's edge");

  // /MG/geometry/doublesolidblock/alphasetActive
  fAlphaSetActiveCmd = new G4UIcmdWithABool(
    "/MG/geometry/doublesolidblock/alphasetActive", this);
  fAlphaSetActiveCmd->SetGuidance("The physical volume of the alpha block will");
  fAlphaSetActiveCmd->SetGuidance("contain the word active and output");
  fAlphaSetActiveCmd->SetGuidance("classes like G4Steps will treat it as such");

  // /MG/geometry/doublesolidblock/alphadeadLayer
  fAlphaSetDeadLayerCmd = new G4UIcmdWithADoubleAndUnit(
    "/MG/geometry/doublesolidblock/alphadeadLayer", this);
  fAlphaSetDeadLayerCmd->SetGuidance("If set to something non-zero, ");
  fAlphaSetDeadLayerCmd->SetGuidance("the outer skin of desired thickness ");
  fAlphaSetDeadLayerCmd->SetGuidance("will be a dead layer and the inner ");
  fAlphaSetDeadLayerCmd->SetGuidance("portion will be active.(a la G4Steps)");



  // /MG/geometry/doublesolidblock/betamaterial
  fBetaMaterialSelectCmd = new G4UIcmdWithAString(
    "/MG/geometry/doublesolidblock/betamaterial", this);
  fBetaMaterialSelectCmd->SetGuidance("Set material of block beta");

  // /MG/geometry/doublesolidblock/betaedgeLength
  fBetaSetEdgeLengthCmd = new G4UIcmdWithADoubleAndUnit(
    "/MG/geometry/doublesolidblock/betaedgeLength", this);
  fBetaSetEdgeLengthCmd->SetGuidance("Set the length of the beta block's edge");

  // /MG/geometry/doublesolidblock/betasetActive
  fBetaSetActiveCmd = new G4UIcmdWithABool(
    "/MG/geometry/doublesolidblock/betasetActive", this);
  fBetaSetActiveCmd->SetGuidance("The physical volume of the block will");
  fBetaSetActiveCmd->SetGuidance("contain the word active and output");
  fBetaSetActiveCmd->SetGuidance("classes like G4Steps will treat it as such");

  // /MG/geometry/doublesolidblock/betadeadLayer
  fBetaSetDeadLayerCmd = new G4UIcmdWithADoubleAndUnit(
    "/MG/geometry/doublesolidblock/betadeadLayer", this);
  fBetaSetDeadLayerCmd->SetGuidance("If set to something non-zero, ");
  fBetaSetDeadLayerCmd->SetGuidance("the outer skin of desired thickness ");
  fBetaSetDeadLayerCmd->SetGuidance("will be a dead layer and the inner ");
  fBetaSetDeadLayerCmd->SetGuidance("portion will be active.(a la G4Steps)");

  // /MG/geometry/doublesolidblock/boundingMaterial
  fBoundingMaterialSelectCmd = new G4UIcmdWithAString(
    "/MG/geometry/doublesolidblock/boundingmaterial", this);
  fBoundingMaterialSelectCmd->SetGuidance("Set material of bounding volume");

  // /MG/geometry/doublesolidblock/alphaplacement
  fAlphaPositionCmd = new
  G4UIcmdWith3VectorAndUnit("/MG/geometry/doublesolidblock/alphaplacement",this);
  fAlphaPositionCmd->SetGuidance("Set location of center of Alpha block.");
  fAlphaPositionCmd->SetParameterName("X","Y","Z",true,true);
  fAlphaPositionCmd->SetUnitCategory("Length");

  // /MG/geometry/doublesolidblock/betaplacement
  fBetaPositionCmd = new
  G4UIcmdWith3VectorAndUnit("/MG/geometry/doublesolidblock/betaplacement",this);
  fBetaPositionCmd->SetGuidance("Set location of center of Beta block.");
  fBetaPositionCmd->SetParameterName("X","Y","Z",true,true);
  fBetaPositionCmd->SetUnitCategory("Length");

}

//---------------------------------------------------------------------------//

MJGeometryDoubleSolidBlockMessenger::~MJGeometryDoubleSolidBlockMessenger()
{
  delete fDoubleSolidBlockDir;

  delete fAlphaMaterialSelectCmd;
  delete fAlphaSetEdgeLengthCmd;
  delete fAlphaSetActiveCmd;
  delete fAlphaSetDeadLayerCmd;
  delete fAlphaPositionCmd;

  delete fBetaMaterialSelectCmd;
  delete fBetaSetEdgeLengthCmd;
  delete fBetaSetActiveCmd;
  delete fBetaSetDeadLayerCmd;
  delete fBetaPositionCmd;
  
  delete fBoundingMaterialSelectCmd;
}

//---------------------------------------------------------------------------//

void MJGeometryDoubleSolidBlockMessenger::SetNewValue(
  G4UIcommand *cmd, G4String newValue ) 
{
  if(cmd == fAlphaMaterialSelectCmd) 
    fDoubleBlock->SetAlphaMaterialName(newValue);
  else if(cmd == fAlphaSetEdgeLengthCmd)
    fDoubleBlock->
    SetAlphaEdgeLength(fAlphaSetEdgeLengthCmd->GetNewDoubleValue(newValue));
  else if(cmd == fAlphaSetActiveCmd)
    fDoubleBlock->SetAlphaActive(fAlphaSetActiveCmd->GetNewBoolValue(newValue));
  else if(cmd == fAlphaSetDeadLayerCmd)
    fDoubleBlock->
    SetAlphaDeadLayerDepth(fAlphaSetDeadLayerCmd->GetNewDoubleValue(newValue));
  else if(cmd == fBetaMaterialSelectCmd) 
    fDoubleBlock->SetBetaMaterialName(newValue);
  else if(cmd == fBetaSetEdgeLengthCmd)
    fDoubleBlock->
    SetBetaEdgeLength(fBetaSetEdgeLengthCmd->GetNewDoubleValue(newValue));
  else if(cmd == fBetaSetActiveCmd)
    fDoubleBlock->SetBetaActive(fBetaSetActiveCmd->GetNewBoolValue(newValue));
  else if(cmd == fBetaSetDeadLayerCmd)
    fDoubleBlock->
    SetBetaDeadLayerDepth(fBetaSetDeadLayerCmd->GetNewDoubleValue(newValue));
  else if(cmd == fBoundingMaterialSelectCmd) 
    fDoubleBlock->SetBoundingMaterialName(newValue);
  else if(cmd == fAlphaPositionCmd)
    fDoubleBlock->SetAlphaPosition(                          
         fAlphaPositionCmd->GetNew3VectorValue(newValue));       
  else if(cmd == fBetaPositionCmd)
    fDoubleBlock->SetBetaPosition(                          
         fBetaPositionCmd->GetNew3VectorValue(newValue));       

}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
