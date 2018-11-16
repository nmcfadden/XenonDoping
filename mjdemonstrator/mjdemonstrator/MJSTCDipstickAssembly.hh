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
// CLASS IMPLEMENTATION:  MJSTCDipstickAssembly.hh
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie Strain
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday June 12 2013
 * 
 * REVISION:
 * 06-12-2013, Created, J. Strain
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#ifndef _MJSTCDIPSTICKASSEMBLY_HH
#define _MJSTCDIPSTICKASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJSTCDipstickFlange;
class MJSTCDipstickJacket;
class MJSTCDipstickStick;
class MJSTCDipstickEndCap;

using namespace std;
//---------------------------------------------------------------------------//

class MJSTCDipstickAssembly: public MJVDemoAssembly
{
  
public:
  MJSTCDipstickAssembly(G4String, G4String);
  MJSTCDipstickAssembly(const MJSTCDipstickAssembly &);
  ~MJSTCDipstickAssembly();

  MJSTCDipstickFlange* GetFlange() {return fFlangePtr;}
  MJSTCDipstickJacket* GetJacket() {return fJacketPtr;}
  MJSTCDipstickStick* GetStick() {return fStickPtr;}
  MJSTCDipstickEndCap* GetEndCap() { return fEndCapPtr;}  
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);
  
private:
  MJSTCDipstickFlange* fFlangePtr;
  MJSTCDipstickJacket* fJacketPtr;
  MJSTCDipstickStick* fStickPtr;
  MJSTCDipstickEndCap* fEndCapPtr;   

};
//
#endif
