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
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION:  MJDemoRadShieldAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The radiation shield assembly
 *
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Sept 10, 2010
 * 
 * REVISION:
 * 
 * 09-22-2010, Created, M. Green
 * 09-01-2017, Move Poly things to PolyAssembly
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMORADSHIELDASSEMBLY_HH
#define _MJDEMORADSHIELDASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoRadShieldCuInner;
class MJDemoRadShieldCuOuter;
class MJDemoRadShieldPbInner;
class MJDemoRadShieldPbOuter;
class MJDemoRadShieldShadowPb;
class MJDemoRadShieldN2VolumeBlank;
class MJDemoRadShieldN2VolumeEast;
class MJDemoRadShieldN2VolumeWest;
class MJDemoRadShieldRnBox;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoRadShieldAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoRadShieldAssembly(G4String, G4String);
  MJDemoRadShieldAssembly(const MJDemoRadShieldAssembly &);
  ~MJDemoRadShieldAssembly();

  MJDemoRadShieldCuInner* GetCuInner() {return fCuInnerPtr;}
  MJDemoRadShieldCuOuter* GetCuOuter() {return fCuOuterPtr;}
  MJDemoRadShieldPbInner* GetPbInner() {return fPbInnerPtr;}
  MJDemoRadShieldPbOuter* GetPbOuter() {return fPbOuterPtr;}
  MJDemoRadShieldShadowPb* GetShadow1() const {return fShadow1Ptr;}
  MJDemoRadShieldShadowPb* GetShadow2() const {return fShadow2Ptr;}
  MJDemoRadShieldShadowPb* GetShadow1() {return fShadow1Ptr;}
  MJDemoRadShieldShadowPb* GetShadow2() {return fShadow2Ptr;}
  MJDemoRadShieldN2VolumeBlank* GetN2BlankE() {return fN2BlankEPtr;}
  MJDemoRadShieldN2VolumeBlank* GetN2BlankW() {return fN2BlankWPtr;}
  MJDemoRadShieldN2VolumeEast* GetN2E() {return fN2EPtr;}
  MJDemoRadShieldN2VolumeWest* GetN2W() {return fN2WPtr;}
  MJDemoRadShieldRnBox* GetRnBox() {return fRnBoxPtr;}

  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemoRadShieldCuInner* fCuInnerPtr;
  MJDemoRadShieldCuOuter* fCuOuterPtr;
  MJDemoRadShieldPbInner* fPbInnerPtr;
  MJDemoRadShieldPbOuter* fPbOuterPtr;
  MJDemoRadShieldShadowPb* fShadow1Ptr;
  MJDemoRadShieldShadowPb* fShadow2Ptr;
  MJDemoRadShieldN2VolumeBlank* fN2BlankEPtr;
  MJDemoRadShieldN2VolumeBlank* fN2BlankWPtr;
  MJDemoRadShieldN2VolumeEast* fN2EPtr;
  MJDemoRadShieldN2VolumeWest* fN2WPtr;
  MJDemoRadShieldRnBox* fRnBoxPtr;

};
//
#endif
