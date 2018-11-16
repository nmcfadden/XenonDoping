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
 * CLASS DECLARATION:  MJDemoThermosyphonWeldmentAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The thick plastic thermosyphon support assembly
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
 * CONTACT: mpgreen@gmail.com
 * FIRST SUBMISSION: July 30, 2014
 * 
 * REVISION:
 * 
 * 07-30-2014, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOTHERMOSYPHONWELDMENTASSEMBLY_HH
#define _MJDEMOTHERMOSYPHONWELDMENTASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoThermosyphonColdplateInterface;
class MJDemoThermosyphonTubeAdapter;
class MJDemoThermosyphonTubeSection1;
class MJDemoThermosyphonTubeSection2;
class MJDemoThermosyphonCornerPiece;
class MJDemoThermosyphonFlange;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoThermosyphonWeldmentAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoThermosyphonWeldmentAssembly(G4String, G4String);
  MJDemoThermosyphonWeldmentAssembly(const MJDemoThermosyphonWeldmentAssembly &);
  ~MJDemoThermosyphonWeldmentAssembly();

  MJDemoThermosyphonColdplateInterface* GetCPInterface() {return fCPInterfacePtr;}
  MJDemoThermosyphonTubeAdapter* GetAdapter() {return fAdapterPtr;}
  MJDemoThermosyphonTubeSection1* GetTube1() {return fTube1Ptr;}
  MJDemoThermosyphonTubeSection2* GetTube2() {return fTube2Ptr;}
  MJDemoThermosyphonCornerPiece* GetCorner() {return fCornerPtr;}
  MJDemoThermosyphonFlange* GetFlange() {return fFlangePtr;}
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);  
  
private:
  MJDemoThermosyphonColdplateInterface* fCPInterfacePtr;
  MJDemoThermosyphonTubeAdapter* fAdapterPtr;
  MJDemoThermosyphonTubeSection1* fTube1Ptr;
  MJDemoThermosyphonTubeSection2* fTube2Ptr;
  MJDemoThermosyphonCornerPiece* fCornerPtr;
  MJDemoThermosyphonFlange* fFlangePtr;
    
};
//
#endif
