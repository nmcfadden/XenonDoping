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
// CLASS IMPLEMENTATION:  MJDemoConnectorandClampSubAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin: Coaxial with screw 1, coincident with bottom face of clamp
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
#include "mjdemonstrator/MJDemoConnectorandClampSubAssembly.hh"
#include "mjdemonstrator/MJDemoBulkheadTopClampLowMount.hh"
#include "mjdemonstrator/MJDemo832Screw.hh"
#include "mjdemonstrator/MJDemoSigConnPairAssembly.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoConnectorandClampSubAssembly::MJDemoConnectorandClampSubAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "connectorandclampdwg")
{
  fClampPtr = new MJDemoBulkheadTopClampLowMount(fName+"_ConnectorClamp_001", "867-5309");
  fScrew01Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_001", "867-5309", 6.35);
  fScrew02Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_002", "867-5309", 6.35);
  fConnector01Ptr = new MJDemoSigConnPairAssembly(fName+"_Connector01", "867-5309");
  fConnector02Ptr = new MJDemoSigConnPairAssembly(fName+"_Connector02", "867-5309");
  fConnector03Ptr = new MJDemoSigConnPairAssembly(fName+"_Connector03", "867-5309");
  fConnector04Ptr = new MJDemoSigConnPairAssembly(fName+"_Connector04", "867-5309");
  fConnector05Ptr = new MJDemoSigConnPairAssembly(fName+"_Connector05", "867-5309");
}

MJDemoConnectorandClampSubAssembly::MJDemoConnectorandClampSubAssembly(const MJDemoConnectorandClampSubAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fClampPtr = new MJDemoBulkheadTopClampLowMount(fName+"_ConnectorClamp_001", "867-5309");
  fScrew01Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_001", "867-5309", 6.35);
  fScrew02Ptr = new MJDemo832Screw(fName+"_8_32x0.25Screw_002", "867-5309", 6.35);
  fConnector01Ptr = new MJDemoSigConnPairAssembly(fName+"_Connector01", "867-5309");
  fConnector02Ptr = new MJDemoSigConnPairAssembly(fName+"_Connector02", "867-5309");
  fConnector03Ptr = new MJDemoSigConnPairAssembly(fName+"_Connector03", "867-5309");
  fConnector04Ptr = new MJDemoSigConnPairAssembly(fName+"_Connector04", "867-5309");
  fConnector05Ptr = new MJDemoSigConnPairAssembly(fName+"_Connector05", "867-5309");
}

MJDemoConnectorandClampSubAssembly::~MJDemoConnectorandClampSubAssembly()
{
    delete fClampPtr;
    delete fScrew01Ptr;
    delete fScrew02Ptr;
    delete fConnector01Ptr;
    delete fConnector02Ptr;
    delete fConnector03Ptr;
    delete fConnector04Ptr;
    delete fConnector05Ptr;
}

void MJDemoConnectorandClampSubAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  G4double eps = 0.01*mm;

  G4ThreeVector *ClamplocalPos = new G4ThreeVector(25.4*mm, -3.175*mm, 4.763/2*mm);
  G4RotationMatrix *ClamplocalRot = new G4RotationMatrix();
  ClamplocalRot->rotateX(0);
  ClamplocalRot->rotateY(0);
  G4AffineTransform *Clampaffine = new G4AffineTransform(ClamplocalRot,*ClamplocalPos);
  *Clampaffine*=*assemAffine;  
  G4ThreeVector *ClampglobalPos = new G4ThreeVector(Clampaffine->NetTranslation());
  G4RotationMatrix *ClampglobalRot= new G4RotationMatrix(Clampaffine->NetRotation());
  fClampPtr->Place(ClampglobalPos, ClampglobalRot, motherLogical);
  delete ClamplocalPos;
  delete ClamplocalRot;
  delete Clampaffine;

  G4ThreeVector *Screw01localPos = new G4ThreeVector(0, 0, 4.763*mm+eps);
  G4RotationMatrix *Screw01localRot = new G4RotationMatrix();
  Screw01localRot->rotateZ(0);
  Screw01localRot->rotateX(0);
  G4AffineTransform *Screw01affine = new G4AffineTransform(Screw01localRot,*Screw01localPos);
  *Screw01affine*=*assemAffine;
  G4ThreeVector *Screw01globalPos = new G4ThreeVector(Screw01affine->NetTranslation());
  G4RotationMatrix *Screw01globalRot= new G4RotationMatrix(Screw01affine->NetRotation());
  fScrew01Ptr->Place(Screw01globalPos, Screw01globalRot, motherLogical);
  delete Screw01localPos;
  delete Screw01localRot;
  delete Screw01affine;
    
  G4ThreeVector *Screw02localPos = new G4ThreeVector(50.8*mm, -6.35*mm, 4.763*mm+eps);
  G4RotationMatrix *Screw02localRot = new G4RotationMatrix();
  Screw02localRot->rotateZ(0);
  Screw02localRot->rotateX(0);
  G4AffineTransform *Screw02affine = new G4AffineTransform(Screw02localRot,*Screw02localPos);
  *Screw02affine*=*assemAffine;
  G4ThreeVector *Screw02globalPos = new G4ThreeVector(Screw02affine->NetTranslation());
  G4RotationMatrix *Screw02globalRot= new G4RotationMatrix(Screw02affine->NetRotation());
  fScrew02Ptr->Place(Screw02globalPos, Screw02globalRot, motherLogical);
  delete Screw02localPos;
  delete Screw02localRot;
  delete Screw02affine;
 
  G4ThreeVector *Connector01localPos = new G4ThreeVector(6.37*mm, 7*mm, -1.6*mm);
  G4RotationMatrix *Connector01localRot = new G4RotationMatrix();
  Connector01localRot->rotateZ(0);
  Connector01localRot->rotateX(-pi/2);
  G4AffineTransform *Connector01affine = new G4AffineTransform(Connector01localRot,*Connector01localPos);
  *Connector01affine*=*assemAffine;
  G4ThreeVector *Connector01globalPos = new G4ThreeVector(Connector01affine->NetTranslation());
  G4RotationMatrix *Connector01globalRot= new G4RotationMatrix(Connector01affine->NetRotation());
  fConnector01Ptr->Place(Connector01globalPos, Connector01globalRot, motherLogical);
  delete Connector01localPos;
  delete Connector01localRot;
  delete Connector01affine;

  G4ThreeVector *Connector02localPos = new G4ThreeVector((6.37+9.525)*mm, 7*mm, -1.6*mm);
  G4RotationMatrix *Connector02localRot = new G4RotationMatrix();
  Connector02localRot->rotateZ(0);
  Connector02localRot->rotateX(-pi/2);
  G4AffineTransform *Connector02affine = new G4AffineTransform(Connector02localRot,*Connector02localPos);
  *Connector02affine*=*assemAffine;
  G4ThreeVector *Connector02globalPos = new G4ThreeVector(Connector02affine->NetTranslation());
  G4RotationMatrix *Connector02globalRot= new G4RotationMatrix(Connector02affine->NetRotation());
  fConnector02Ptr->Place(Connector02globalPos, Connector02globalRot, motherLogical);
  delete Connector02localPos;
  delete Connector02localRot;
  delete Connector02affine;

  G4ThreeVector *Connector03localPos = new G4ThreeVector((6.37+2*9.525)*mm, 7*mm, -1.6*mm);
  G4RotationMatrix *Connector03localRot = new G4RotationMatrix();
  Connector03localRot->rotateZ(0);
  Connector03localRot->rotateX(-pi/2);
  G4AffineTransform *Connector03affine = new G4AffineTransform(Connector03localRot,*Connector03localPos);
  *Connector03affine*=*assemAffine;
  G4ThreeVector *Connector03globalPos = new G4ThreeVector(Connector03affine->NetTranslation());
  G4RotationMatrix *Connector03globalRot= new G4RotationMatrix(Connector03affine->NetRotation());
  fConnector03Ptr->Place(Connector03globalPos, Connector03globalRot, motherLogical);
  delete Connector03localPos;
  delete Connector03localRot;
  delete Connector03affine;

  G4ThreeVector *Connector04localPos = new G4ThreeVector((6.37+3*9.525)*mm, 7*mm, -1.6*mm);
  G4RotationMatrix *Connector04localRot = new G4RotationMatrix();
  Connector04localRot->rotateZ(0);
  Connector04localRot->rotateX(-pi/2);
  G4AffineTransform *Connector04affine = new G4AffineTransform(Connector04localRot,*Connector04localPos);
  *Connector04affine*=*assemAffine;
  G4ThreeVector *Connector04globalPos = new G4ThreeVector(Connector04affine->NetTranslation());
  G4RotationMatrix *Connector04globalRot= new G4RotationMatrix(Connector04affine->NetRotation());
  fConnector04Ptr->Place(Connector04globalPos, Connector04globalRot, motherLogical);
  delete Connector04localPos;
  delete Connector04localRot;
  delete Connector04affine;

  G4ThreeVector *Connector05localPos = new G4ThreeVector((6.37+4*9.525)*mm, 7*mm, -1.6*mm);
  G4RotationMatrix *Connector05localRot = new G4RotationMatrix();
  Connector05localRot->rotateZ(0);
  Connector05localRot->rotateX(-pi/2);
  G4AffineTransform *Connector05affine = new G4AffineTransform(Connector05localRot,*Connector05localPos);
  *Connector05affine*=*assemAffine;
  G4ThreeVector *Connector05globalPos = new G4ThreeVector(Connector05affine->NetTranslation());
  G4RotationMatrix *Connector05globalRot= new G4RotationMatrix(Connector05affine->NetRotation());
  fConnector05Ptr->Place(Connector05globalPos, Connector05globalRot, motherLogical);
  delete Connector05localPos;
  delete Connector05localRot;
  delete Connector05affine;

  delete assemAffine;
}
