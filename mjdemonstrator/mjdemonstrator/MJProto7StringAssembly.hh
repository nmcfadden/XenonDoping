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
 * CLASS DECLARATION:  MJProto7StringAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * This should be the same as MJDemo7StringAssembly. However, there are overlapping 
 * geometries in that file that still need to be addressed. So this is a "temporary" 
 * file until the overlaps are fixed so that the PC geometry can be used.
 *
 *
 *
 *The assembly consisting of 7 detector strings, cold plate.
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
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Wednesday March 26, 2014
 * 
 * REVISION:
 * 03-26-2014, Created, J. MacMullin
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJPROTO7STRINGASSEMBLY_HH
#define _MJPROTO7STRINGASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"
#include "G4LogicalVolume.hh"

using namespace std;
//---------------------------------------------------------------------------//

class MJProto7StringAssembly: public MJVDemoAssembly
{
  
public:
  MJProto7StringAssembly(G4String, G4String);
  MJProto7StringAssembly(const MJProto7StringAssembly &);
  ~MJProto7StringAssembly();


  MJVDemoPart* GetColdPlate() {return fColdPlatePtr;}
  MJVDemoAssembly* GetString1() {return fString1Ptr;}
  MJVDemoAssembly* GetString2() {return fString2Ptr;}
  MJVDemoAssembly* GetString3() {return fString3Ptr;}
  MJVDemoAssembly* GetString4() {return fString4Ptr;}
  MJVDemoAssembly* GetString5() {return fString5Ptr;}
  MJVDemoAssembly* GetString6() {return fString6Ptr;}
  MJVDemoAssembly* GetString7() {return fString7Ptr;}
  /*MJVDemoPart* GetHVTwist1() {return fHVTwist1Ptr;} //Wenqin
  MJVDemoPart* GetHVTwist2() {return fHVTwist2Ptr;} //Wenqin
  MJVDemoPart* GetHVTwist3() {return fHVTwist3Ptr;} //Wenqin
  MJVDemoPart* GetHVTwist4() {return fHVTwist4Ptr;} //Wenqin
  MJVDemoPart* GetHVTwist5() {return fHVTwist5Ptr;} //Wenqin
  MJVDemoPart* GetHVTwist6() {return fHVTwist6Ptr;} //Wenqin
  MJVDemoPart* GetHVTwist7() {return fHVTwist7Ptr;} //Wenqin
  MJVDemoPart* GetSilicaStack1() {return fSilicaStack1Ptr;} //Wenqin
  MJVDemoPart* GetSilicaStack2() {return fSilicaStack2Ptr;} //Wenqin
  MJVDemoPart* GetSilicaStack3() {return fSilicaStack3Ptr;} //Wenqin
  MJVDemoPart* GetSilicaStack4() {return fSilicaStack4Ptr;} //Wenqin
  MJVDemoPart* GetSilicaStack5() {return fSilicaStack5Ptr;} //Wenqin
  MJVDemoPart* GetSilicaStack6() {return fSilicaStack6Ptr;} //Wenqin
  MJVDemoPart* GetSilicaStack7() {return fSilicaStack7Ptr;} //Wenqin
  MJVDemoPart* GetPin1() {return fPin1Ptr;}
  MJVDemoPart* GetPin2() {return fPin2Ptr;}
  MJVDemoPart* GetPin3() {return fPin3Ptr;}*/
  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  size_t fNumStrings;
  MJVDemoPart*      fColdPlatePtr;
  MJVDemoAssembly*  fString1Ptr;
  MJVDemoAssembly*  fString2Ptr;
  MJVDemoAssembly*  fString3Ptr;
  MJVDemoAssembly*  fString4Ptr;
  MJVDemoAssembly*  fString5Ptr;
  MJVDemoAssembly*  fString6Ptr;
  MJVDemoAssembly*  fString7Ptr;
  /*MJVDemoPart*	    fHVTwist1Ptr; //Wenqin
  MJVDemoPart*	    fHVTwist2Ptr; //Wenqin
  MJVDemoPart*	    fHVTwist3Ptr; //Wenqin
  MJVDemoPart*	    fHVTwist4Ptr; //Wenqin
  MJVDemoPart*	    fHVTwist5Ptr; //Wenqin
  MJVDemoPart*	    fHVTwist6Ptr; //Wenqin
  MJVDemoPart*	    fHVTwist7Ptr; //Wenqin
  MJVDemoPart*	    fSilicaStack1Ptr; //Wenqin
  MJVDemoPart*	    fSilicaStack2Ptr; //Wenqin
  MJVDemoPart*	    fSilicaStack3Ptr; //Wenqin
  MJVDemoPart*	    fSilicaStack4Ptr; //Wenqin
  MJVDemoPart*	    fSilicaStack5Ptr; //Wenqin
  MJVDemoPart*	    fSilicaStack6Ptr; //Wenqin
  MJVDemoPart*	    fSilicaStack7Ptr; //Wenqin
  MJVDemoPart*      fPin1Ptr;
  MJVDemoPart*      fPin2Ptr;
  MJVDemoPart*      fPin3Ptr;*/
};
//
#endif
