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
// $Id: MJGeometryPPCCryostat.cc,v 1.1 2008-09-18 17:39:34 mitzi Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryPPCCryostat.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: M.Boswell
 * CONTACT: 
 * FIRST SUBMISSION: Wed. July 23, 2008
 * 
 * REVISION:
 *
 * 07/23/2008, Created, M. Boswell
 *
 */
//---------------------------------------------------------------------------//
//

#include <sstream>
#include <vector>


#include "globals.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4AssemblyVolume.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"

#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"
#include "mjgeometry/MJGeometryPPCCryostatMessenger.hh"
#include "mjgeometry/MJGeometryPPCDetector.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryPPCCryostat.hh"
#include "mjgeometry/MJGeometryPPCCrystalColumn.hh"

using namespace std;

using namespace CLHEP;
using namespace MaGe;


//---------------------------------------------------------------------------//
MJGeometryPPCCryostat::MJGeometryPPCCryostat():
  MGGeometryDetector("") 
{;}
MJGeometryPPCCryostat::MJGeometryPPCCryostat(G4String serNum):
  MGGeometryDetector(serNum) 
{
  //  if(!messOff)
  //theDetectorLogical = 0;
  fG4Messenger = new MJGeometryPPCCryostatMessenger(this);
  fTemplateCrystal = new MJGeometryPPCDetector("Template", false);
  fTemplateCrystal->SetDefaults();
  fTemplateCrystalColumn = new MJGeometryPPCCrystalColumn("Template");
  fTemplateCrystalColumn->SetDefaults();
  fTemplateCrystalColumn->SetTemplateCrystal(fTemplateCrystal);
  //fTemplateCrystalColumn->Initialize();
    //  else 
    //    fG4Messenger = 0;
  SetDefaults();
}

//---------------------------------------------------------------------------//

MJGeometryPPCCryostat::MJGeometryPPCCryostat(const MJGeometryPPCCryostat &rhs):
  MGGeometryDetector(rhs.serialNumber)
{;}

//---------------------------------------------------------------------------//

MJGeometryPPCCryostat::~MJGeometryPPCCryostat()
{
//   if(!fIsOneOfMany) {
//     delete fCanVisAtt;
//     delete fColdplateVisAtt;
//   }
  delete fG4Messenger;
  size_t sizeCC = fCrystalColumns.size();
  for(size_t i=0; i<sizeCC; i++) 
    delete fCrystalColumns[i];
  
  //fCrystalColumns.clear();
  //delete fCrystalColumns;
  //delete fCrystalColumns;
  delete fTemplateCrystalColumn;
  delete CuAttributes;
  delete EFCuAttributes;
  delete fTemplateCrystal;
  
}

//---------------------------------------------------------------------------//

void MJGeometryPPCCryostat::ConstructDetector()
{
  G4ThreeVector noOffset(0,0,0);
  G4Translate3D noOffset3D(0,0,0);
  G4RotationMatrix noRotate;// = new G4RotationMatrix();
  //noRotate->rotateZ(0*deg);
  G4Transform3D noTransform = noOffset3D;

  Initialize();
  MGLog(routine) << "Constructing Majorana PPC cryostat geometry" << endlog;
  size_t i;
  

  //build world 
  G4Tubs* innerCryostatSolid = 
    new G4Tubs("innerCryostatSolid",0, fAssemblyRadius, 
	       fAssemblyHeight/2+2*fAssemTopThick, 0, twopi);
  G4LogicalVolume* innerCryostatLogical = 
    new G4LogicalVolume(innerCryostatSolid, fOpenSpaceMaterial, 
			"innerCryostatLogical");
  innerCryostatLogical->SetVisAttributes(G4VisAttributes::Invisible);
  
  
  //build mount plate
  G4int AssemTopDim = 4;
  G4double AssemTopHeight[4] = {0,fAssemTopCutawayThick, fAssemTopCutawayThick,
				fAssemTopThick};
  G4double AssemTopIRad[4] = {fAssemTopIRad, fAssemTopIRad, 0, 0};
  G4double AssemTopORad[4] = {fAssemTopORad, fAssemTopORad, fAssemTopORad, 
			      fAssemTopORad};
  G4VSolid* AssemblyTopSolid = 
    new G4Polycone("PPCAssemblyTop", 0*deg, twopi, AssemTopDim, 
		   AssemTopHeight, AssemTopIRad, AssemTopORad);
  G4Tubs* AssemblyTopCutaway = 
    new G4Tubs("PPCAssemblyTopCutaway", 0*inch, fAssemTopIRad,
	       fAssemTopCutawayThick, 0*deg, twopi);
  G4ThreeVector MoveCutaway(fAssemTopOffset, 0,0);
  G4RotationMatrix rotateCutaway;
  rotateCutaway.rotateZ(0*deg);
  G4Transform3D moveCutaway;
  for (i=0; i<=6; i++) {
    moveCutaway = G4Transform3D(noRotate,rotateCutaway*MoveCutaway);
    AssemblyTopSolid = 
      new G4SubtractionSolid("AssemblyTop-Cutaway", AssemblyTopSolid, 
			     AssemblyTopCutaway, moveCutaway); 
    rotateCutaway.rotateZ(fAssemTopOffsetAng);
  }
  G4LogicalVolume* AssemblyTopLogical = 
    new G4LogicalVolume(AssemblyTopSolid,CanMaterial, "AssemblyTopLogical");

  new G4PVPlacement(0, G4ThreeVector(0,0,fAssemblyOffset), AssemblyTopLogical, 
		    "PPCAssemblyTop", innerCryostatLogical, false, 0);

  G4Transform3D moveCrystalColumn(noRotate,noOffset);
  G4RotationMatrix rotateColumn;
  rotateColumn.rotateZ(0*deg);

  //need to figure out how many crystal columns you can arrange in a ring
  G4double outerRadius = fAssemTopOffset;
  G4int fullCircle=0;  //so we start from scratch when we fill a circle


  //build the crystal columns
  for (i=0; i< fNOfCrystalColumns; i++) {
    fCrystalColumns[i]->SetDetectorLogical(0);
    MGLog(routine) << "Constructing crystal column: " 
		   << fCrystalColumns[i]->GetSerialNumber() << endlog;
    fCrystalColumns[i]->ConstructDetector();
    MGLog(debugging) << "Placing crystal columns  -" 
		     << fCrystalColumns[i]->GetDetectorLogical() << endlog;
    new G4PVPlacement(moveCrystalColumn, 
		      fCrystalColumns[i]->GetDetectorLogical(), 
		      G4String(fCrystalColumns[i]->GetSerialNumber()), 
		      innerCryostatLogical, false, 0);
    if ((i-fullCircle)*fAssemTopOffsetAng>360*deg) { 
      rotateColumn.rotateZ(0*deg);
      outerRadius += outerRadius;
      MoveCutaway += MoveCutaway;
      fullCircle = i-1;
      moveCrystalColumn = G4Transform3D(noRotate,rotateColumn*MoveCutaway);
    } else {
      rotateColumn.rotateZ(fAssemTopOffsetAng);
      moveCrystalColumn = G4Transform3D(noRotate,rotateColumn*MoveCutaway);
    }

  }


  //build logical detector volume, and place cryostat in it
  G4Tubs* cryostatSolid = 
    new G4Tubs("cryostatSolid",0,2*fAssemblyRadius,fAssemblyHeight,0,twopi);
  theDetectorLogical = 
    new G4LogicalVolume(cryostatSolid, fOpenSpaceMaterial,"cryostatLogical");
  new G4PVPlacement(noTransform, innerCryostatLogical, "InnerCryostatPhysical",
		    theDetectorLogical, false, 0);
				   




}
void MJGeometryPPCCryostat::SetDefaults()
{
  fAssemTopCutawayThick =0.30*inch;
  fAssemTopThick = 0.50*inch;
  fAssemTopORad = 10*inch/2;
  fAssemTopIRad=1.53*inch;
  fAssemTopOffset= 3.06*inch;
  fAssemTopOffsetAng = 60*deg;
  fAssemblyOffset = 10*inch/2+ fAssemTopThick;
  fNOfCrystalColumns = 7;
  fOpenSpaceMaterialName = "Vacuum";





}
void MJGeometryPPCCryostat::Initialize()
{


  MGLog(debugging) <<"@@@@@Cryostat Init@@@@@@@@" << endlog;
  //MGLog(debugging) << *(G4Material::GetMaterialTable());

  
  EFCuAttributes =   //dark green copper
    new G4VisAttributes(G4Colour(0.29,0.46,0.43,0.65));
  EFCuAttributes->SetVisibility(true);
  CuAttributes =   // copper color
    new G4VisAttributes(G4Colour(0.72,0.45,0.2,0.65));
  CuAttributes->SetVisibility(true);


  //material definitions
  //define some things that we need to test first
  G4String symbol;             
  //G4double a, z; //, density;     
  //G4int ncomponents;
  //G4double fractionmass;

  //G4Element* N = new G4Element("Nitrogen", symbol="N", z=7., a=14.01*g/mole);
  //G4Element* O = new G4Element("Oxygen",   symbol="O", z=8., a=16.00*g/mole);
 

  try {
    fOpenSpaceMaterial = G4Material::GetMaterial(fOpenSpaceMaterialName);
    if (!fOpenSpaceMaterial) throw "OpenSpaceMaterial";
  }
  catch (const char* matName) {
    MGLog(debugging) << matName << " is not a defined G4Material." << endlog;
    MGLog(error) << endlog;
  }
  try {
    CanMaterial = G4Material::GetMaterial("Copper-EF");  
    if (!CanMaterial) throw "Copper-EF";
  }
  catch (const char* matName) {
    MGLog(debugging) << matName << " is not a defined G4Material." << endlog;
    MGLog(error) << endlog;
  }

  // Construct templates and compute useful variables. 
  MGLog(routine) << "Initializing template crystal." << endlog;
  fTemplateCrystal->SetIsOneOfMany(false);
  fTemplateCrystal->SetCrystalMaterial(CrystalMaterial);
  //fTemplateCrystal->SetG4MaterialName("Germanium-Enr");
  fTemplateCrystal->ConstructDetector();
  fTemplateCrystal->Dump();

  MGLog(routine) << "Constructing template crystal column." << endlog;
  fTemplateCrystalColumn->SetOneOfMany(false);
  fTemplateCrystalColumn->SetDetectorLogical(0);
  fTemplateCrystalColumn->ConstructDetector();
  MGLog(debugging) << "??????????? daughters" << fTemplateCrystalColumn->GetDetectorLogical()->GetNoDaughters()
		   <<endlog;


  //build crystal columns
  stringstream crystalColumnName;
  fCrystalColumns.clear();
  for (size_t i=0; i<fNOfCrystalColumns; i++) {
    MGLog(debugging) << "Initializing crystal column "  << i << endlog;
    crystalColumnName.str("");
    crystalColumnName << "CrystalColumn" << i;
    fCrystalColumns.push_back(new MJGeometryPPCCrystalColumn(*fTemplateCrystalColumn));
    fCrystalColumns[i]->SetOneOfMany(true);
    fCrystalColumns[i]->SetSerialNumber(crystalColumnName.str());
    fCrystalColumns[i]->SetTemplateCrystalColumnSerNum(fTemplateCrystal->GetSerialNumber());
  }


  fEps = 0.0001*inch;
  fAssemblyHeight = fTemplateCrystalColumn->GetCrystalColumnHeight()
    + fAssemTopThick + fEps;
  fAssemblyRadius = fAssemTopORad + fAssemTopThick + fEps;
  fAssemblyOffset = fTemplateCrystalColumn->GetCrystalColumnHeight()/2;


}
