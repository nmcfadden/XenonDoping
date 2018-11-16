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
// CLASS IMPLEMENTATION:  MJDemoPolyAssembly.cc
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
 * AUTHOR: Ralph Massarczyk
 * CONTACT: massarczyk@lanl.gov
 * FIRST SUBMISSION: Sept 01, 2017
 * 
 * REVISION:
 * 
 * 09-01-2017, Created, Ralph
 */
//---------------------------------------------------------------------------//
//

#ifndef _MJDEMOPOLYASSEMBLY_HH
#define _MJDEMOPOLYASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoPoly_Panel0;
class MJDemoPoly_Panel1;
class MJDemoPoly_Panel2;
class MJDemoPoly_Panel3;
class MJDemoPoly_Panel4;
class MJDemoPoly_Panel5;
class MJDemoPoly_Panel6;
class MJDemoPoly_Panel7;
class MJDemoPoly_Panel8;
class MJDemoPoly_Panel9;
class MJDemoPoly_Panel10;
class MJDemoPoly_Panel11;
class MJDemoPoly_Panel12;
class MJDemoPoly_Panel13;
class MJDemoPoly_Panel14;
class MJDemoPoly_Panel15;
class MJDemoPoly_Panel16;
class MJDemoPoly_Panel17;
class MJDemoPoly_Panel18;
class MJDemoPoly_Panel19;
class MJDemoPoly_Panel20;
class MJDemoPoly_Panel21;
class MJDemoPoly_Panel22;
class MJDemoPoly_Panel23;
//-----------------------------------------------

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoPolyAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoPolyAssembly(G4String, G4String);
  MJDemoPolyAssembly(const MJDemoPolyAssembly &);
  ~MJDemoPolyAssembly();

  MJDemoPoly_Panel0* 			GetPoly_Panel0_001() 				{return fPoly_Panel0_001;}
  MJDemoPoly_Panel0* 			GetPoly_Panel0_002() 				{return fPoly_Panel0_002;}
  MJDemoPoly_Panel0* 			GetPoly_Panel0_003() 				{return fPoly_Panel0_003;}
  MJDemoPoly_Panel0* 			GetPoly_Panel0_004() 				{return fPoly_Panel0_004;}
  MJDemoPoly_Panel0* 			GetPoly_Panel0_005() 				{return fPoly_Panel0_005;}
  MJDemoPoly_Panel0* 			GetPoly_Panel0_006() 				{return fPoly_Panel0_006;}
  MJDemoPoly_Panel0* 			GetPoly_Panel0_007() 				{return fPoly_Panel0_007;}
  MJDemoPoly_Panel0* 			GetPoly_Panel0_008() 				{return fPoly_Panel0_008;}
  MJDemoPoly_Panel0* 			GetPoly_Panel0_009() 				{return fPoly_Panel0_009;}
  MJDemoPoly_Panel0* 			GetPoly_Panel0_010() 				{return fPoly_Panel0_010;}
  MJDemoPoly_Panel0* 			GetPoly_Panel0_011() 				{return fPoly_Panel0_011;}
  MJDemoPoly_Panel0* 			GetPoly_Panel0_012() 				{return fPoly_Panel0_012;}

  MJDemoPoly_Panel1* 			GetPoly_Panel1_001() 				{return fPoly_Panel1_001;}
  MJDemoPoly_Panel1* 			GetPoly_Panel1_002() 				{return fPoly_Panel1_002;}
  MJDemoPoly_Panel1* 			GetPoly_Panel1_003() 				{return fPoly_Panel1_003;}
  MJDemoPoly_Panel1* 			GetPoly_Panel1_004() 				{return fPoly_Panel1_004;}
  MJDemoPoly_Panel1* 			GetPoly_Panel1_005() 				{return fPoly_Panel1_005;}
  MJDemoPoly_Panel1* 			GetPoly_Panel1_006() 				{return fPoly_Panel1_006;}

  MJDemoPoly_Panel2* 			GetPoly_Panel2_001() 				{return fPoly_Panel2_001;}
  MJDemoPoly_Panel2* 			GetPoly_Panel2_002() 				{return fPoly_Panel2_002;}
  MJDemoPoly_Panel2* 			GetPoly_Panel2_003() 				{return fPoly_Panel2_003;}

  MJDemoPoly_Panel3* 			GetPoly_Panel3_001() 				{return fPoly_Panel3_001;}
  MJDemoPoly_Panel3* 			GetPoly_Panel3_002() 				{return fPoly_Panel3_002;}
  MJDemoPoly_Panel3* 			GetPoly_Panel3_003() 				{return fPoly_Panel3_003;}

  MJDemoPoly_Panel4* 			GetPoly_Panel4_001() 				{return fPoly_Panel4_001;}
  MJDemoPoly_Panel4* 			GetPoly_Panel4_002() 				{return fPoly_Panel4_002;}
  MJDemoPoly_Panel4* 			GetPoly_Panel4_003() 				{return fPoly_Panel4_003;}
  MJDemoPoly_Panel4* 			GetPoly_Panel4_004() 				{return fPoly_Panel4_004;}
  MJDemoPoly_Panel4* 			GetPoly_Panel4_005() 				{return fPoly_Panel4_005;}
  MJDemoPoly_Panel4* 			GetPoly_Panel4_006() 				{return fPoly_Panel4_006;}

  MJDemoPoly_Panel5* 			GetPoly_Panel5_001() 				{return fPoly_Panel5_001;}
  MJDemoPoly_Panel5* 			GetPoly_Panel5_002() 				{return fPoly_Panel5_002;}
  MJDemoPoly_Panel5* 			GetPoly_Panel5_003() 				{return fPoly_Panel5_003;}
  MJDemoPoly_Panel5* 			GetPoly_Panel5_004() 				{return fPoly_Panel5_004;}
  MJDemoPoly_Panel5* 			GetPoly_Panel5_005() 				{return fPoly_Panel5_005;}
  MJDemoPoly_Panel5* 			GetPoly_Panel5_006() 				{return fPoly_Panel5_006;}
  
  MJDemoPoly_Panel6* 			GetPoly_Panel6_001() 				{return fPoly_Panel6_001;}
  MJDemoPoly_Panel6* 			GetPoly_Panel6_002() 				{return fPoly_Panel6_002;}
  MJDemoPoly_Panel6* 			GetPoly_Panel6_003() 				{return fPoly_Panel6_003;}
  
  MJDemoPoly_Panel7* 			GetPoly_Panel7_001() 				{return fPoly_Panel7_001;}
  MJDemoPoly_Panel7* 			GetPoly_Panel7_002() 				{return fPoly_Panel7_002;}
  MJDemoPoly_Panel7* 			GetPoly_Panel7_003() 				{return fPoly_Panel7_003;}
  
  MJDemoPoly_Panel8* 			GetPoly_Panel8_001() 				{return fPoly_Panel8_001;}
  MJDemoPoly_Panel8* 			GetPoly_Panel8_002() 				{return fPoly_Panel8_002;}
  MJDemoPoly_Panel8* 			GetPoly_Panel8_003() 				{return fPoly_Panel8_003;}
  
  MJDemoPoly_Panel9* 			GetPoly_Panel9_001() 				{return fPoly_Panel9_001;}
  MJDemoPoly_Panel9* 			GetPoly_Panel9_002() 				{return fPoly_Panel9_002;}
  MJDemoPoly_Panel9* 			GetPoly_Panel9_003() 				{return fPoly_Panel9_003;}
  
  MJDemoPoly_Panel10* 			GetPoly_Panel10_001() 				{return fPoly_Panel10_001;}
  MJDemoPoly_Panel10* 			GetPoly_Panel10_002() 				{return fPoly_Panel10_002;}
  MJDemoPoly_Panel10* 			GetPoly_Panel10_003() 				{return fPoly_Panel10_003;}

  MJDemoPoly_Panel11* 			GetPoly_Panel11_001() 				{return fPoly_Panel11_001;}
  MJDemoPoly_Panel11* 			GetPoly_Panel11_002() 				{return fPoly_Panel11_002;}
  MJDemoPoly_Panel11* 			GetPoly_Panel11_003() 				{return fPoly_Panel11_003;}

  MJDemoPoly_Panel12* 			GetPoly_Panel12_001() 				{return fPoly_Panel12_001;}
  MJDemoPoly_Panel12* 			GetPoly_Panel12_002() 				{return fPoly_Panel12_002;}
  MJDemoPoly_Panel12* 			GetPoly_Panel12_003() 				{return fPoly_Panel12_003;}

  MJDemoPoly_Panel13* 			GetPoly_Panel13_001() 				{return fPoly_Panel13_001;}
  MJDemoPoly_Panel13* 			GetPoly_Panel13_002() 				{return fPoly_Panel13_002;}
  MJDemoPoly_Panel13* 			GetPoly_Panel13_003() 				{return fPoly_Panel13_003;}

  MJDemoPoly_Panel14* 			GetPoly_Panel14_001() 				{return fPoly_Panel14_001;}
  MJDemoPoly_Panel14* 			GetPoly_Panel14_002() 				{return fPoly_Panel14_002;}
  MJDemoPoly_Panel14* 			GetPoly_Panel14_003() 				{return fPoly_Panel14_003;}

  MJDemoPoly_Panel15* 			GetPoly_Panel15_001() 				{return fPoly_Panel15_001;}
  MJDemoPoly_Panel15* 			GetPoly_Panel15_002() 				{return fPoly_Panel15_002;}
  MJDemoPoly_Panel15* 			GetPoly_Panel15_003() 				{return fPoly_Panel15_003;}
  
  MJDemoPoly_Panel16* 			GetPoly_Panel16_001() 				{return fPoly_Panel16_001;}
  MJDemoPoly_Panel16* 			GetPoly_Panel16_002() 				{return fPoly_Panel16_002;}
  MJDemoPoly_Panel16* 			GetPoly_Panel16_003() 				{return fPoly_Panel16_003;}
  
  MJDemoPoly_Panel17* 			GetPoly_Panel17_001() 				{return fPoly_Panel17_001;}
  MJDemoPoly_Panel17* 			GetPoly_Panel17_002() 				{return fPoly_Panel17_002;}
  MJDemoPoly_Panel17* 			GetPoly_Panel17_003() 				{return fPoly_Panel17_003;}
  
  MJDemoPoly_Panel18* 			GetPoly_Panel18_001() 				{return fPoly_Panel18_001;}
  MJDemoPoly_Panel18* 			GetPoly_Panel18_002() 				{return fPoly_Panel18_002;}
  MJDemoPoly_Panel18* 			GetPoly_Panel18_003() 				{return fPoly_Panel18_003;}
  
  MJDemoPoly_Panel19* 			GetPoly_Panel19_001() 				{return fPoly_Panel19_001;}
  MJDemoPoly_Panel19* 			GetPoly_Panel19_002() 				{return fPoly_Panel19_002;}
  MJDemoPoly_Panel19* 			GetPoly_Panel19_003() 				{return fPoly_Panel19_003;}
  
  MJDemoPoly_Panel20* 			GetPoly_Panel20_001() 				{return fPoly_Panel20_001;}
  MJDemoPoly_Panel20* 			GetPoly_Panel20_002() 				{return fPoly_Panel20_002;}
  MJDemoPoly_Panel20* 			GetPoly_Panel20_003() 				{return fPoly_Panel20_003;}
  
  MJDemoPoly_Panel21* 			GetPoly_Panel21_001() 				{return fPoly_Panel21_001;}
  MJDemoPoly_Panel21* 			GetPoly_Panel21_002() 				{return fPoly_Panel21_002;}
  MJDemoPoly_Panel21* 			GetPoly_Panel21_003() 				{return fPoly_Panel21_003;}
  
  MJDemoPoly_Panel22* 			GetPoly_Panel22_001() 				{return fPoly_Panel22_001;}
  MJDemoPoly_Panel22* 			GetPoly_Panel22_002() 				{return fPoly_Panel22_002;}
  MJDemoPoly_Panel22* 			GetPoly_Panel22_003() 				{return fPoly_Panel22_003;}

  MJDemoPoly_Panel23* 			GetPoly_Panel23_001() 				{return fPoly_Panel23_001;}
  MJDemoPoly_Panel23* 			GetPoly_Panel23_002() 				{return fPoly_Panel23_002;}
  MJDemoPoly_Panel23* 			GetPoly_Panel23_003() 				{return fPoly_Panel23_003;}

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
	G4String PanelDesign;
	G4int PanelDesign_number;

  MJDemoPoly_Panel0*				fPoly_Panel0_001;
  MJDemoPoly_Panel0*				fPoly_Panel0_002;
  MJDemoPoly_Panel0*				fPoly_Panel0_003;
  MJDemoPoly_Panel0*				fPoly_Panel0_004;
  MJDemoPoly_Panel0*				fPoly_Panel0_005;
  MJDemoPoly_Panel0*				fPoly_Panel0_006;
  MJDemoPoly_Panel0*				fPoly_Panel0_007;
  MJDemoPoly_Panel0*				fPoly_Panel0_008;
  MJDemoPoly_Panel0*				fPoly_Panel0_009;
  MJDemoPoly_Panel0*				fPoly_Panel0_010;
  MJDemoPoly_Panel0*				fPoly_Panel0_011;
  MJDemoPoly_Panel0*				fPoly_Panel0_012;

  MJDemoPoly_Panel1*				fPoly_Panel1_001;
  MJDemoPoly_Panel1*				fPoly_Panel1_002;
  MJDemoPoly_Panel1*				fPoly_Panel1_003;
  MJDemoPoly_Panel1*				fPoly_Panel1_004;
  MJDemoPoly_Panel1*				fPoly_Panel1_005;
  MJDemoPoly_Panel1*				fPoly_Panel1_006;

  MJDemoPoly_Panel2*				fPoly_Panel2_001;
  MJDemoPoly_Panel2*				fPoly_Panel2_002;
  MJDemoPoly_Panel2*				fPoly_Panel2_003;

  MJDemoPoly_Panel3*				fPoly_Panel3_001;
  MJDemoPoly_Panel3*				fPoly_Panel3_002;
  MJDemoPoly_Panel3*				fPoly_Panel3_003;

  MJDemoPoly_Panel4*				fPoly_Panel4_001;
  MJDemoPoly_Panel4*				fPoly_Panel4_002;
  MJDemoPoly_Panel4*				fPoly_Panel4_003;
  MJDemoPoly_Panel4*				fPoly_Panel4_004;
  MJDemoPoly_Panel4*				fPoly_Panel4_005;
  MJDemoPoly_Panel4*				fPoly_Panel4_006;

  MJDemoPoly_Panel5*				fPoly_Panel5_001;
  MJDemoPoly_Panel5*				fPoly_Panel5_002;
  MJDemoPoly_Panel5*				fPoly_Panel5_003;
  MJDemoPoly_Panel5*				fPoly_Panel5_004;
  MJDemoPoly_Panel5*				fPoly_Panel5_005;
  MJDemoPoly_Panel5*				fPoly_Panel5_006;

  MJDemoPoly_Panel6*				fPoly_Panel6_001;
  MJDemoPoly_Panel6*				fPoly_Panel6_002;
  MJDemoPoly_Panel6*				fPoly_Panel6_003;

  MJDemoPoly_Panel7*				fPoly_Panel7_001;
  MJDemoPoly_Panel7*				fPoly_Panel7_002;
  MJDemoPoly_Panel7*				fPoly_Panel7_003;

  MJDemoPoly_Panel8*				fPoly_Panel8_001;
  MJDemoPoly_Panel8*				fPoly_Panel8_002;
  MJDemoPoly_Panel8*				fPoly_Panel8_003;

  MJDemoPoly_Panel9*				fPoly_Panel9_001;
  MJDemoPoly_Panel9*				fPoly_Panel9_002;
  MJDemoPoly_Panel9*				fPoly_Panel9_003;

  MJDemoPoly_Panel10*				fPoly_Panel10_001;
  MJDemoPoly_Panel10*				fPoly_Panel10_002;
  MJDemoPoly_Panel10*				fPoly_Panel10_003;

  MJDemoPoly_Panel11*				fPoly_Panel11_001;
  MJDemoPoly_Panel11*				fPoly_Panel11_002;
  MJDemoPoly_Panel11*				fPoly_Panel11_003;

  MJDemoPoly_Panel12*				fPoly_Panel12_001;
  MJDemoPoly_Panel12*				fPoly_Panel12_002;
  MJDemoPoly_Panel12*				fPoly_Panel12_003;

  MJDemoPoly_Panel13*				fPoly_Panel13_001;
  MJDemoPoly_Panel13*				fPoly_Panel13_002;
  MJDemoPoly_Panel13*				fPoly_Panel13_003;

  MJDemoPoly_Panel14*				fPoly_Panel14_001;
  MJDemoPoly_Panel14*				fPoly_Panel14_002;
  MJDemoPoly_Panel14*				fPoly_Panel14_003;

  MJDemoPoly_Panel15*				fPoly_Panel15_001;
  MJDemoPoly_Panel15*				fPoly_Panel15_002;
  MJDemoPoly_Panel15*				fPoly_Panel15_003;

  MJDemoPoly_Panel16*				fPoly_Panel16_001;
  MJDemoPoly_Panel16*				fPoly_Panel16_002;
  MJDemoPoly_Panel16*				fPoly_Panel16_003;

  MJDemoPoly_Panel17*				fPoly_Panel17_001;
  MJDemoPoly_Panel17*				fPoly_Panel17_002;
  MJDemoPoly_Panel17*				fPoly_Panel17_003;

  MJDemoPoly_Panel18*				fPoly_Panel18_001;
  MJDemoPoly_Panel18*				fPoly_Panel18_002;
  MJDemoPoly_Panel18*				fPoly_Panel18_003;

  MJDemoPoly_Panel19*				fPoly_Panel19_001;
  MJDemoPoly_Panel19*				fPoly_Panel19_002;
  MJDemoPoly_Panel19*				fPoly_Panel19_003;

  MJDemoPoly_Panel20*				fPoly_Panel20_001;
  MJDemoPoly_Panel20*				fPoly_Panel20_002;
  MJDemoPoly_Panel20*				fPoly_Panel20_003;

  MJDemoPoly_Panel21*				fPoly_Panel21_001;
  MJDemoPoly_Panel21*				fPoly_Panel21_002;
  MJDemoPoly_Panel21*				fPoly_Panel21_003;

  MJDemoPoly_Panel22*				fPoly_Panel22_001;
  MJDemoPoly_Panel22*				fPoly_Panel22_002;
  MJDemoPoly_Panel22*				fPoly_Panel22_003;

  MJDemoPoly_Panel23*				fPoly_Panel23_001;
  MJDemoPoly_Panel23*				fPoly_Panel23_002;
  MJDemoPoly_Panel23*				fPoly_Panel23_003;


};
//
#endif
