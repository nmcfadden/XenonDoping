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
// CLASS IMPLEMENTATION:  MJChar1420Screw.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Class for a generic 1/4-20 hex-head (round slotted) machine screw.  Part origin is 
 * coaxial with screw, coincident with shoulder.
 * 
 * Modified from MJDemo832Screw for size and material (stainless steel)
 */
// 
// --------------------------------------------------------------------------//
/**
 * AUTHOR: A. Wasserman
 * CONTACT: adw74@eden.rutgers.edu
 * FIRST SUBMISSION: July 22, 2013
 *
 * REVISION:
 * 
 * 07-22-2013, Created, A. Wasserman
 *
 */
// --------------------------------------------------------------------------//
//

#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UIcommand.hh"
#include "G4Tubs.hh"
#include "G4Polyhedra.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJChar1420Screw.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

MJChar1420Screw::MJChar1420Screw(G4String partName, G4String serialNumber, 
								G4double length) :
  MJVDemoPart(partName, serialNumber, "1/4-20", "Copper-EF"), 
  fScrewLength(length*mm)
{;}

MJChar1420Screw::MJChar1420Screw(const MJChar1420Screw & rhs) : 
  MJVDemoPart(rhs), fScrewLength(rhs.fScrewLength)
{;}

MJChar1420Screw::~MJChar1420Screw()
{;}

G4LogicalVolume* MJChar1420Screw::ConstructPart()
{

  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "x" + G4UIcommand::ConvertToString(fScrewLength) 
  										+ "_" + fPartMaterial;  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){

    // Definition of stainless steel, to replace Copper-EF from MJVDemoPart call
    // Compostions taken from azom.com for Grade 316 SS (UNS S31600)
    G4double z, a, density;
    G4String name, symbol;
    G4int ncomponents, natoms;

    G4Element* elC = new G4Element(name="Carbon",symbol="C",z=1.,a=12.011*g/mole);
    G4Element* elMn = new G4Element(name="Maganese",symbol="Mn",z=25.,a=54.938*g/mole);
    G4Element* elSi = new G4Element(name="Silicon",symbol="Si",z=14.,a=28.085*g/mole);
    G4Element* elP = new G4Element(name="Phosphorus",symbol="P",z=15.,a=30.974*g/mole);
    G4Element* elS = new G4Element(name="Sulfur",symbol="S",z=16.,a=32.06*g/mole);
    G4Element* elCr = new G4Element(name="Chromium",symbol="Cr",z=24.,a=51.996*g/mole);
    G4Element* elMo = new G4Element(name="Molybdenum",symbol="Mo",z=42.,a=95.96*g/mole);
    G4Element* elNi = new G4Element(name="Nickel",symbol="Ni",z=28.,a=58.693*g/mole);
    G4Element* elN = new G4Element(name="Nitrogen",symbol="N",z=7.,a=14.007*g/mole);
    G4Element* elFe = new G4Element(name="Iron",symbol="Fe",z=26.,a=55.845*g/mole);

    G4Material* stainlessSteel = new G4Material(name="StainlessSteel316Grade",density=8000.0*kg/m3,ncomponents=10);
    stainlessSteel->AddElement(elC, natoms = 80);
    stainlessSteel->AddElement(elMn, natoms = 2000);
    stainlessSteel->AddElement(elSi, natoms = 750);
    stainlessSteel->AddElement(elP, natoms = 22);
    stainlessSteel->AddElement(elS, natoms = 30);
    stainlessSteel->AddElement(elCr, natoms = 17000);
    stainlessSteel->AddElement(elMo, natoms = 2500);
    stainlessSteel->AddElement(elNi, natoms = 14000);
    stainlessSteel->AddElement(elN, natoms = 100);
    stainlessSteel->AddElement(elFe, natoms = 65518);

    // Definition of screw, hexagonal prism on top of cylinder
    // Dimensions of screw based on McMaster Carr 92185A541 Socket Head Cap Screw
    G4double zPlanes[] = {0*25.4*mm, 0.25*25.4*mm};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {0.375/2.0*25.4*mm, 0.375/2.0*25.4*mm};
    G4Polyhedra* hex = new G4Polyhedra("hex", pi/2, 2*pi, 6, 2, zPlanes, rInner, rOuter);    

    G4Tubs* thread = new G4Tubs("thread", 0, 0.125*25.4*mm, fScrewLength/2 + 0.1*mm, 0, 2*pi);

    G4UnionSolid* bolt = new G4UnionSolid("bolt", hex, thread, 0, G4ThreeVector(0, 0, -fScrewLength/2 + 0.1*mm)); 

    G4VisAttributes* grayVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // gray
    grayVisAtt->SetForceWireframe( false );
    // material lookup is overridden
    //G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(bolt, stainlessSteel, logicalName);
    pVol->SetVisAttributes(grayVisAtt); 
    MGLog(debugging) << "Created 1/4-20x " << fScrewLength << "mm Screw Logical" << endlog;
  }
  else  MGLog(debugging) << "Using pre-existing 1/4-20x " << fScrewLength << "mm Screw Logical" << endlog;
  return pVol;
}


