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
/**                                                            
 * $Id: MJheaderTemplate.hh,v 1.3 2005-02-15 23:26:55 akbarm Exp $
 *      
 * CLASS DECLARATION:  MJGeometryPopTopXYscan.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *Geometry for simulating an ORTEC detector in the PopTop cryostat
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ryan Martin
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * 02-25-2013, Created, R. Martin
 *
 */
//---------------------------------------------------------------------------//
//

#ifndef _MJPOPTOPXYSCAN_HH
#define  _MJPOPTOPXYSCAN_HH


#include <string>
#include <vector>

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"
#include "mjgeometry/MJGeometryOrtecCrystal.hh"
#include "mjgeometry/MJGeometryAcrylicSource.hh"
#include "mjgeometry/MJGeometryPopTopXYscanMessenger.hh"
//---------------------------------------------------------------------------//
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;
class G4PVPlacement;

using namespace std;

class MJGeometryPopTopXYscan : public MGGeometryDetector
{
  
public:
  MJGeometryPopTopXYscan(G4String);
  virtual ~MJGeometryPopTopXYscan();
  
  void ConstructMaterials();
  void ConstructDetector();
//  bool UsesMJPSQLDatabase() { return false; }
  bool UsesMJPSQLDatabase() { return true; }
  
  void AddIRCupRing(G4double argDistanceFromTopOfCup, G4double argWidthOfRing,  G4double argThicknessOfRing, G4double argTolerance);
  
  void PlaceSource(G4ThreeVector argVector){fPlaceSource=true;fSourceX=argVector.x();fSourceY=argVector.y();fSourceZ=argVector.z();}
  void PlaceCollimator(G4bool argb=true){fPlaceCollimator=argb;}
  void PlaceSourceHolder(G4bool argb=true){fPlaceSourceHolder=argb;}

  void SetCollimatorGap(G4double arggap){fCollimatorGap=arggap;}
  void SetCrystalOffset(G4double argOffset){fCrystalOffsetFromInsideOfVacuumShroud=argOffset;}
  void SetCryostatType(G4int argType){fIRCupType=argType;}
  
  //Mostly so the messenger can interact with the crystal
  void LoadCrystalGeomFromFile(G4String argFileName){fCrystal->LoadCrystalGeomFromFile(argFileName);}
  void SetCrystalDimpleNominalRadius(G4double argD){ fCrystal->SetCrystalDimpleNominalRadius(argD);}
  void SetCrystalDimpleHoleDepth(G4double argD){fCrystal->SetCrystalDimpleHoleDepth(argD);}
  void SetCrystalDimpleHoleDiameter(G4double argD){fCrystal->SetCrystalDimpleHoleDiameter(argD);}
  void SetCrystalDimpleRadialOffset(G4double argD){fCrystal->SetCrystalDimpleRadialOffset(argD);}
  void SetCrystalDiameter(G4double argD){fCrystal->SetCrystalDiameter(argD);}
  void SetCrystalLength(G4double argD){fCrystal->SetCrystalLength(argD);} 
  virtual int GetSensitiveVolumeID(const std::string& volName);
  
private:
  
  MJGeometryOrtecCrystal *fCrystal;
  G4double fCrystalOffsetFromInsideOfVacuumShroud;//Quantity G in the ORTEC spec sheets
  
  //Vacuum Shroud
  G4double fVacuumShroudOuterRadius;
  G4double fVacuumShroudRadialThickness;//Quantity L in the ORTEC spec sheets
  G4double fVacuumShroudLength;
  G4double fVacuumShroudEndcapThickness;//Quantity I in the ORTEC spec sheets
  
  //IR Cup
  G4int fIRCupType;//Type of IR cup. 1 = PONaMa1, 2= PONaMa2, 3=P42538B 
  G4double fIRCupOffsetFromInsideOfVacuumShroud;//
  G4double fIRCupOuterRadius;
  G4double fIRCupThickness;//Quantity K in the ORTEC spec sheets
  G4double fIRCupLength;//Quantity F in the ORTEC spec sheets
  G4double fIRCupBottomThickness;//3mm in the ORTEC spec sheets
  
  vector<double> fIRCupZplanes;
  vector<double> fIRCupROuterplanes;
  vector<double> fIRCupRInnerplanes;
  G4double fSetScrewHeightFromTopOfCup;
  
  //Set screws in the IR cup
  G4double fSetScrewLength;
  G4double fSetScrewDiameter;
  
  //Mylar Foil
  G4double fMylarFoilThickness;
  
  //Teflon ring that holds the mylar on (the diameter is fixed by the can).
  G4double fTeflonRingLength;
  G4double fTeflonRingThickness;
  G4double fTeflonRingDiameter;
  
  //Contact pin assembly
  G4double fContactPinDiameter;
  G4double fContactPinLength;
  G4double fContactPinHolderDiameter;
  G4double fContactPinHolderLength;//Length of the pin holder inside the cup (a length gets added later to extend below the cup)
  
  //Space at bottom of IR cup
  G4double fSpaceBelowIRCup;
  G4double fDiskBelowCupDiameter;
  G4double fDiskBelowCupLength;
  G4double fBlockBelowCupDiameter;
  G4double fBlockBelowCupLength;
  G4double fTeflonBelowCupDiameter;
  G4double fTeflonBelowCupLength;
  
  //Gold HV Foil
  G4double fGoldFoilLength;
  G4double fGoldFoilWidth;
  G4double fGoldFoilThickness;
  
  //If placing a source, its position:
  MJGeometryAcrylicSource* fSource;
  G4bool fPlaceSource;
  G4double fSourceX;
  G4double fSourceY;
  G4double fSourceZ;
  //Place the 25cm source holder:
  G4bool fPlaceSourceHolder;
  G4double fSourceHolderRadialThickness;//thickness on the tubularpart
  G4double fSourceHolderRadialThicknessBottom;//thickness on the tubularpart
  G4double fSourceHolderSourceOpeningDiameter;//Diameter of the hole that the source sees
  G4double fSourceHolderLidThickness;//total thickness of the lid
  G4double fSourceHolderSourceRecessDepth;//depth at which the source sits, below the top of the lid
  G4double fSourceHolderLength;//total length
  G4double fSourceHolderLip;//length of the lip that holds onto the vacuum shroud
    
  //Place the 5cm Collimator:
  G4bool fPlaceCollimator;
  G4double fCollimatorGap;//the gap between the bottom of the collimator and the top of the cryostat 
  
  
  MJGeometryPopTopXYscanMessenger* fG4Messenger;
};
#endif

