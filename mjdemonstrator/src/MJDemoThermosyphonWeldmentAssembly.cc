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
// CLASS IMPLEMENTATION:  MJDemoThermosyphonWeldmentAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin:  Co-axial with thermosyphon tube, front face of cold plate adapter
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@gmail.com
 * FIRST SUBMISSION: July 30, 2014
 *
 * REVISION:
 *
 * 07-30-2014, Created, M. Green
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
#include "mjdemonstrator/MJDemoThermosyphonWeldmentAssembly.hh"
#include "mjdemonstrator/MJDemoThermosyphonColdplateInterface.hh"
#include "mjdemonstrator/MJDemoThermosyphonTubeAdapter.hh"
#include "mjdemonstrator/MJDemoThermosyphonTubeSection1.hh"
#include "mjdemonstrator/MJDemoThermosyphonTubeSection2.hh"
#include "mjdemonstrator/MJDemoThermosyphonCornerPiece.hh"
#include "mjdemonstrator/MJDemoThermosyphonFlange.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoThermosyphonWeldmentAssembly::MJDemoThermosyphonWeldmentAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "MJ80-03-058-R002")
{
  fCPInterfacePtr = new MJDemoThermosyphonColdplateInterface(fName+"_CPInterface_001", "867-5309");
  fAdapterPtr = new MJDemoThermosyphonTubeAdapter(fName+"_TubeAdapter_001", "867-5309");
  fTube1Ptr = new MJDemoThermosyphonTubeSection1(fName+"_Tube1_001", "867-5309");
  fTube2Ptr = new MJDemoThermosyphonTubeSection2(fName+"_Tube2_001", "867-5309");
  fCornerPtr = new MJDemoThermosyphonCornerPiece(fName+"_CornerPiece_001", "867-5309");
  fFlangePtr = new MJDemoThermosyphonFlange(fName+"_Flange_001", "867-5309");
}

MJDemoThermosyphonWeldmentAssembly::MJDemoThermosyphonWeldmentAssembly(const MJDemoThermosyphonWeldmentAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fCPInterfacePtr = new MJDemoThermosyphonColdplateInterface(fName+"_CPInterface_001", "867-5309");
  fAdapterPtr = new MJDemoThermosyphonTubeAdapter(fName+"_TubeAdapter_001", "867-5309");
  fTube1Ptr = new MJDemoThermosyphonTubeSection1(fName+"_Tube1_001", "867-5309");
  fTube2Ptr = new MJDemoThermosyphonTubeSection2(fName+"_Tube2_001", "867-5309");
  fCornerPtr = new MJDemoThermosyphonCornerPiece(fName+"_CornerPiece_001", "867-5309");
  fFlangePtr = new MJDemoThermosyphonFlange(fName+"_Flange_001", "867-5309");
}

MJDemoThermosyphonWeldmentAssembly::~MJDemoThermosyphonWeldmentAssembly()
{
  delete fCPInterfacePtr;
  delete fAdapterPtr;
  delete fTube1Ptr;
  delete fTube2Ptr;
  delete fCornerPtr;
  delete fFlangePtr;
}

void MJDemoThermosyphonWeldmentAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  G4double eps = 0.001*mm;

  G4ThreeVector *CPInterfacelocalPos = new G4ThreeVector(-114.3/2*mm, 0, -0.09*25.4*mm);
  G4RotationMatrix *CPInterfacelocalRot = new G4RotationMatrix();
  CPInterfacelocalRot->rotateX(pi/2);
  CPInterfacelocalRot->rotateY(pi/2);
  G4AffineTransform *CPInterfaceaffine = new G4AffineTransform(CPInterfacelocalRot,*CPInterfacelocalPos);
  *CPInterfaceaffine*=*assemAffine;  
  G4ThreeVector *CPInterfaceglobalPos = new G4ThreeVector(CPInterfaceaffine->NetTranslation());
  G4RotationMatrix *CPInterfaceglobalRot= new G4RotationMatrix(CPInterfaceaffine->NetRotation());
  fCPInterfacePtr->Place(CPInterfaceglobalPos, CPInterfaceglobalRot, motherLogical);
  delete CPInterfacelocalPos;
  delete CPInterfacelocalRot;
  delete CPInterfaceaffine;

  G4ThreeVector *AdapterlocalPos = new G4ThreeVector((-114.3-6.096/2)*mm-eps, 0, -0.09*25.4*mm);
  G4RotationMatrix *AdapterlocalRot = new G4RotationMatrix();
  G4AffineTransform *Adapteraffine = new G4AffineTransform(AdapterlocalRot,*AdapterlocalPos);

  *Adapteraffine*=*assemAffine;
  G4ThreeVector *AdapterglobalPos = new G4ThreeVector(Adapteraffine->NetTranslation());
  G4RotationMatrix *AdapterglobalRot= new G4RotationMatrix(Adapteraffine->NetRotation());
  fAdapterPtr->Place(AdapterglobalPos, AdapterglobalRot, motherLogical);
  delete AdapterlocalPos;
  delete AdapterlocalRot;
  delete Adapteraffine;
    
  G4ThreeVector *Tube1localPos = new G4ThreeVector((-114.3-6.096-450.367/2)*mm-2*eps, 0, 0);
  G4RotationMatrix *Tube1localRot = new G4RotationMatrix();
  Tube1localRot->rotateY(pi/2);
  G4AffineTransform *Tube1affine = new G4AffineTransform(Tube1localRot,*Tube1localPos);
  *Tube1affine*=*assemAffine;
  G4ThreeVector *Tube1globalPos = new G4ThreeVector(Tube1affine->NetTranslation());
  G4RotationMatrix *Tube1globalRot= new G4RotationMatrix(Tube1affine->NetRotation());
  fTube1Ptr->Place(Tube1globalPos, Tube1globalRot, motherLogical);
  delete Tube1localPos;
  delete Tube1localRot;
  delete Tube1affine;
    
  G4ThreeVector *Tube2localPos = new G4ThreeVector((-114.3-6.096-450.367-424.993/2)*mm-3*eps, 0, 0);
  G4RotationMatrix *Tube2localRot = new G4RotationMatrix();
  Tube2localRot->rotateY(pi/2);
  G4AffineTransform *Tube2affine = new G4AffineTransform(Tube2localRot,*Tube2localPos);
  *Tube2affine*=*assemAffine;
  G4ThreeVector *Tube2globalPos = new G4ThreeVector(Tube2affine->NetTranslation());
  G4RotationMatrix *Tube2globalRot= new G4RotationMatrix(Tube2affine->NetRotation());
  fTube2Ptr->Place(Tube2globalPos, Tube2globalRot, motherLogical);
  delete Tube2localPos;
  delete Tube2localRot;
  delete Tube2affine;
    
  G4ThreeVector *CornerlocalPos = new G4ThreeVector((-114.3-6.096-450.367-424.993-53.671)*mm-4*eps, 0, 0);
  G4RotationMatrix *CornerlocalRot = new G4RotationMatrix();
  G4AffineTransform *Corneraffine = new G4AffineTransform(CornerlocalRot,*CornerlocalPos);
  *Corneraffine*=*assemAffine;
  G4ThreeVector *CornerglobalPos = new G4ThreeVector(Corneraffine->NetTranslation());
  G4RotationMatrix *CornerglobalRot= new G4RotationMatrix(Corneraffine->NetRotation());
  fCornerPtr->Place(CornerglobalPos, CornerglobalRot, motherLogical);
  delete CornerlocalPos;
  delete CornerlocalRot;
  delete Corneraffine;
    
  G4ThreeVector *FlangelocalPos = new G4ThreeVector((-114.3-6.096-450.367-424.993-53.671)*mm-4*eps, 0, 22.352+eps);
  G4RotationMatrix *FlangelocalRot = new G4RotationMatrix();
  G4AffineTransform *Flangeaffine = new G4AffineTransform(FlangelocalRot,*FlangelocalPos);
  *Flangeaffine*=*assemAffine;
  G4ThreeVector *FlangeglobalPos = new G4ThreeVector(Flangeaffine->NetTranslation());
  G4RotationMatrix *FlangeglobalRot= new G4RotationMatrix(Flangeaffine->NetRotation());
  fFlangePtr->Place(FlangeglobalPos, FlangeglobalRot, motherLogical);
  delete FlangelocalPos;
  delete FlangelocalRot;
  delete Flangeaffine;
    
    
  delete assemAffine;
}
