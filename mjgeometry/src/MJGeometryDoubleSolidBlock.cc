

// GEANT4 headers
#include "globals.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

#include "geometry/MGGeometryGlobals.hh"
#include "mjgeometry/MJGeometryDoubleSolidBlockMessenger.hh"
#include "io/MGLogger.hh"


//---------------------------------------------------------------------------// 

#include "mjgeometry/MJGeometrySolidBlock.hh"
#include "mjgeometry/MJGeometryDoubleSolidBlock.hh"

//---------------------------------------------------------------------------// 


MJGeometryDoubleSolidBlock::MJGeometryDoubleSolidBlock():
MGGeometryDetector(""), fPhysicalBlockAlpha(0), fPhysicalBlockBeta(0)
{
  SetDefaults();
  fMessenger = new MJGeometryDoubleSolidBlockMessenger(this);
  fSolidBlockAlpha =  new MJGeometrySolidBlock();
  fSolidBlockBeta =  new MJGeometrySolidBlock();
}

//---------------------------------------------------------------------------// 

MJGeometryDoubleSolidBlock::~MJGeometryDoubleSolidBlock()
{
  delete fSolidBlockAlpha;
  delete fSolidBlockBeta;
  delete fMessenger;
}
 
//---------------------------------------------------------------------------// 

using namespace CLHEP;


void MJGeometryDoubleSolidBlock::ConstructDetector() {

  //Will create two blocks, block alpha and block beta.  Each block needs to 
  //have a dimension, position, and material.  They also need to be specified
  // as active or passive, and with or without a dead layer.



  //First create the bounding volume
  G4Material *BoundingMaterial =
  G4Material::GetMaterial(fBoundingMaterialName);
  G4Box* BoundingSolid = new G4Box("BoundingSolid", 24*m, 24*m, 24*m);
  theDetectorLogical = new G4LogicalVolume(BoundingSolid, 
					   BoundingMaterial,
					   "BoundingLogical");

  //instantiate and construct the two blocks

  //set the individual Block Attributes
  fSolidBlockAlpha->SetMaterialName(fAlphaMaterialName);
  fSolidBlockAlpha->SetEdgeLength(fAlphaLength);
  fSolidBlockAlpha->SetActive(fAlphaActive);
  fSolidBlockAlpha->SetDeadLayerDepth(fAlphaDeadLayerDepth);

  fSolidBlockBeta->SetMaterialName(fBetaMaterialName);
  fSolidBlockBeta->SetEdgeLength(fBetaLength);
  fSolidBlockBeta->SetActive(fBetaActive);
  fSolidBlockBeta->SetDeadLayerDepth(fBetaDeadLayerDepth);

  //then place them in the bounding volume place the physical blocks.  
  fSolidBlockAlpha->ConstructDetector();
  fSolidBlockBeta->ConstructDetector();
  fPhysicalBlockAlpha = new G4PVPlacement(0, fAlphaPosition,
                                          fSolidBlockAlpha->GetDetectorLogical(),
                                          "BlockAlpha", theDetectorLogical,
                                          false, 0);
                                         
  fPhysicalBlockBeta = new G4PVPlacement(0, fBetaPosition,
                                         fSolidBlockBeta->GetDetectorLogical(),
                                         "BlockBeta", theDetectorLogical,
                                         false, 0);
  
  // Set visualization parameters:
  G4VisAttributes* YellowVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* GreenVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,0.0));

  (fSolidBlockAlpha->GetDetectorLogical())->SetVisAttributes(GreenVisAtt);
  (fSolidBlockBeta->GetDetectorLogical())->SetVisAttributes(YellowVisAtt);
}

//---------------------------------------------------------------------------// 

void MJGeometryDoubleSolidBlock::SetDefaults()
{
  fAlphaLength = 10.0*cm;
  fBetaLength = 1.0*cm;
  fAlphaMaterialName = "Germanium-Nat";
  fBoundingMaterialName = "Vacuum";
  fBetaMaterialName = "Germanium-Nat";
  fAlphaPosition.set(0.0*cm, 0.0*cm, 0.0*cm);
  fBetaPosition.set(0.0*cm, 0.0*cm, 11.001*cm);
  fAlphaActive = false;
  fBetaActive = false;
  fAlphaDeadLayer = false;
  fBetaDeadLayer = false;
  fAlphaDeadLayerDepth = 0.0 * cm;
  fBetaDeadLayerDepth = 0.0 * cm;
}
