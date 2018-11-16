

//---------------------------------------------------------------------------// 

#ifndef _MJGEOMETRYDOUBLESOLIDBLOCK_HH
#define _MJGEOMETRYDOUBLESOLIDBLOCK_HH

//---------------------------------------------------------------------------// 

#include "globals.hh"
#include "G4PVPlacement.hh"
#include "geometry/MGGeometryDetector.hh"
#include "mjgeometry/MJGeometrySolidBlock.hh"

//---------------------------------------------------------------------------// 

class G4LogicalVolume;
class G4PhysicalVolume;
class MJGeometrySolidBlock;

class MJGeometryDoubleSolidBlockMessenger;

class MJGeometryDoubleSolidBlock : public MGGeometryDetector
{
  public:

  //default constructor
  MJGeometryDoubleSolidBlock();

  //destructor
  ~MJGeometryDoubleSolidBlock();

  //public interface
  void ConstructDetector();
  void SetDefaults();

  void SetAlphaMaterialName(G4String name){fAlphaMaterialName = name;}
  void SetAlphaEdgeLength(G4double length){fAlphaLength = length;}
  void SetAlphaActive(G4bool active){fAlphaActive = active;}
  void SetAlphaDeadLayerDepth(G4double dl){fAlphaDeadLayerDepth = dl;}
  void SetAlphaPosition(G4ThreeVector pos){fAlphaPosition = pos;}

  void SetBetaMaterialName(G4String name){fBetaMaterialName = name;}
  void SetBetaEdgeLength(G4double length){fBetaLength = length;}
  void SetBetaActive(G4bool active){fBetaActive = active;}
  void SetBetaDeadLayerDepth(G4double dl){fBetaDeadLayerDepth = dl;}
  void SetBetaPosition(G4ThreeVector pos){fBetaPosition = pos;}

  void SetBoundingMaterialName(G4String name)
       {fBoundingMaterialName = name;}

  bool UsesMJPSQLDatabase() { return true; }  
 
  private:

  MJGeometryDoubleSolidBlockMessenger     *fMessenger;

  G4String                          fAlphaMaterialName;
  G4double                          fAlphaLength;
  G4bool                            fAlphaActive;
  G4bool                            fAlphaDeadLayer;
  G4double                          fAlphaDeadLayerDepth;
  G4ThreeVector                     fAlphaPosition;
  G4String                          fBetaMaterialName;
  G4double                          fBetaLength;
  G4bool                            fBetaActive;
  G4bool                            fBetaDeadLayer;
  G4double                          fBetaDeadLayerDepth;
  G4ThreeVector                     fBetaPosition;

  G4String                          fBoundingMaterialName;

  MJGeometrySolidBlock              *fSolidBlockAlpha;
  MJGeometrySolidBlock              *fSolidBlockBeta;
  G4PVPlacement                     *fPhysicalBlockAlpha;
  G4PVPlacement                     *fPhysicalBlockBeta;
};
#endif
