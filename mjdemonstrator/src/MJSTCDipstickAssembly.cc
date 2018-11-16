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
//      
// CLASS IMPLEMENTATION:  MJSTCDipstickAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 * The placement of the dipstick needs to be checked. See note below.
 * Geometry made from UW SolidWorks file.
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday June 12 2013
 * 
 * REVISION:
 * 06-12-2013, Created, J. MacMullin
 * 03-25-2014, Modified spacing between "touching" surfaces, J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4AffineTransform.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjdemonstrator/MJSTCDipstickAssembly.hh"
#include "mjdemonstrator/MJSTCDipstickFlange.hh"
#include "mjdemonstrator/MJSTCDipstickJacket.hh"
#include "mjdemonstrator/MJSTCDipstickStick.hh"
#include "mjdemonstrator/MJSTCDipstickEndCap.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJSTCDipstickAssembly::MJSTCDipstickAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "Dipstick")
{
  fFlangePtr = new MJSTCDipstickFlange(fName+"_Flange_001", "867-5309");
  fJacketPtr = new MJSTCDipstickJacket(fName+"_Jacket_001", "867-5309");
  fStickPtr = new MJSTCDipstickStick(fName+"_Stick_001", "867-5309");
  fEndCapPtr = new MJSTCDipstickEndCap(fName+"_EndCap_001", "867-5309");
}

MJSTCDipstickAssembly::MJSTCDipstickAssembly(const MJSTCDipstickAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fFlangePtr = new MJSTCDipstickFlange(fName+"_Flange_001", "867-5309");
  fJacketPtr = new MJSTCDipstickJacket(fName+"_Jacket_001", "867-5309");
  fStickPtr = new MJSTCDipstickStick(fName+"_Stick_001", "867-5309");
  fEndCapPtr = new MJSTCDipstickEndCap(fName+"_EndCap_001", "867-5309");
}

MJSTCDipstickAssembly::~MJSTCDipstickAssembly()
{
  delete fFlangePtr;
  delete fJacketPtr;
  delete fStickPtr;
  delete fEndCapPtr;
}

void MJSTCDipstickAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4double eps = 0.001 * mm;
  
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *flangelocalPos = new G4ThreeVector(0, 0, 0);
  G4RotationMatrix *flangelocalRot = new G4RotationMatrix();
  G4AffineTransform *flangeaffine = new G4AffineTransform(flangelocalRot,*flangelocalPos);
  *flangeaffine*=*assemAffine;  
  G4ThreeVector *flangeglobalPos = new G4ThreeVector(flangeaffine->NetTranslation());
  G4RotationMatrix *flangeglobalRot= new G4RotationMatrix(flangeaffine->NetRotation());
  fFlangePtr->Place(flangeglobalPos, flangeglobalRot, motherLogical);
  delete flangelocalPos;
  delete flangelocalRot;
  delete flangeaffine;
 
 // The dipstick is not actually touching the IR shield, which is incorrect according to 
 // the Solid Works file from UW. There is an error somewhere in the Solid Works and I 
 // think it is in the height of the dipstick/jacket wrt the dipstick flange
  G4ThreeVector *jacketlocalPos = new G4ThreeVector(0, 0, -122.631*mm);
  G4RotationMatrix *jacketlocalRot = new G4RotationMatrix();
  G4AffineTransform *jacketaffine = new G4AffineTransform(jacketlocalRot,*jacketlocalPos);
  *jacketaffine*=*assemAffine;  
  G4ThreeVector *jacketglobalPos = new G4ThreeVector(jacketaffine->NetTranslation());
  G4RotationMatrix *jacketglobalRot= new G4RotationMatrix(jacketaffine->NetRotation());
  fJacketPtr->Place(jacketglobalPos, jacketglobalRot, motherLogical);
  delete jacketlocalPos;
  delete jacketlocalRot;
  delete jacketaffine;
 
  G4ThreeVector *sticklocalPos = new G4ThreeVector(0, 0, (-122.631-121.717+2.286+127.4445)*mm);
  G4RotationMatrix *sticklocalRot = new G4RotationMatrix();
  G4AffineTransform *stickaffine = new G4AffineTransform(sticklocalRot,*sticklocalPos);
  *stickaffine*=*assemAffine;  
  G4ThreeVector *stickglobalPos = new G4ThreeVector(stickaffine->NetTranslation());
  G4RotationMatrix *stickglobalRot= new G4RotationMatrix(stickaffine->NetRotation());
  fStickPtr->Place(stickglobalPos, stickglobalRot, motherLogical);
  delete sticklocalPos;
  delete sticklocalRot;
  delete stickaffine;
 
  G4ThreeVector *endcaplocalPos = new G4ThreeVector(0, 0, (-122.631-121.717+2.286)*mm-eps);
  G4RotationMatrix *endcaplocalRot = new G4RotationMatrix();
  G4AffineTransform *endcapaffine = new G4AffineTransform(endcaplocalRot,*endcaplocalPos);
  *endcapaffine*=*assemAffine;  
  G4ThreeVector *endcapglobalPos = new G4ThreeVector(endcapaffine->NetTranslation());
  G4RotationMatrix *endcapglobalRot= new G4RotationMatrix(endcapaffine->NetRotation());
  fEndCapPtr->Place(endcapglobalPos, endcapglobalRot, motherLogical);
  delete endcaplocalPos;
  delete endcaplocalRot;
  delete endcapaffine;    

  delete assemAffine;  
}
