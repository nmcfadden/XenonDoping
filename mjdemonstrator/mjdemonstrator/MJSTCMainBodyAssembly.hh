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
// CLASS IMPLEMENTATION:  MJSTCMainBodyAssembly.hh
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
#ifndef _MJSTCMAINBODYASSEMBLY_HH
#define _MJSTCMAINBODYASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJSTCMainBodyCan;
class MJSTCMainBody8Flange6Bore;
class MJSTCMainBodyHoop;
class MJSTCMainBodyBlankFlange;

using namespace std;
//---------------------------------------------------------------------------//

class MJSTCMainBodyAssembly: public MJVDemoAssembly
{
  
public:
  MJSTCMainBodyAssembly(G4String, G4String);
  MJSTCMainBodyAssembly(const MJSTCMainBodyAssembly &);
  ~MJSTCMainBodyAssembly();

  MJSTCMainBodyCan* GetCan() {return fCanPtr;}
  MJSTCMainBody8Flange6Bore* GetBottomFlange() {return fBottomFlangePtr;}
  MJSTCMainBodyHoop* GetHoop() { return fHoopPtr;}  
  MJSTCMainBody8Flange6Bore* GetTopFlange() {return fTopFlangePtr;}
  MJSTCMainBodyBlankFlange* GetBlankFlange() {return fBlankFlangePtr;}
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);
  
private:
  MJSTCMainBodyCan* fCanPtr;
  MJSTCMainBody8Flange6Bore* fBottomFlangePtr;
  MJSTCMainBodyHoop* fHoopPtr;
  MJSTCMainBody8Flange6Bore* fTopFlangePtr;
  MJSTCMainBodyBlankFlange* fBlankFlangePtr;

};
//
#endif
