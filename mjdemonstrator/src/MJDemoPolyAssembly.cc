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
 * 09-01-2017, Created, Ralph Massarczyk
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
#include "mjdemonstrator/MJDemoPolyAssembly.hh"
#include "mjdemonstrator/MJDemoPoly_Panel0.hh"
#include "mjdemonstrator/MJDemoPoly_Panel1.hh"
#include "mjdemonstrator/MJDemoPoly_Panel2.hh"
#include "mjdemonstrator/MJDemoPoly_Panel3.hh"
#include "mjdemonstrator/MJDemoPoly_Panel4.hh"
#include "mjdemonstrator/MJDemoPoly_Panel5.hh"
#include "mjdemonstrator/MJDemoPoly_Panel6.hh"
#include "mjdemonstrator/MJDemoPoly_Panel7.hh"
#include "mjdemonstrator/MJDemoPoly_Panel8.hh"
#include "mjdemonstrator/MJDemoPoly_Panel9.hh"
#include "mjdemonstrator/MJDemoPoly_Panel10.hh"
#include "mjdemonstrator/MJDemoPoly_Panel11.hh"
#include "mjdemonstrator/MJDemoPoly_Panel12.hh"
#include "mjdemonstrator/MJDemoPoly_Panel13.hh"
#include "mjdemonstrator/MJDemoPoly_Panel14.hh"
#include "mjdemonstrator/MJDemoPoly_Panel15.hh"
#include "mjdemonstrator/MJDemoPoly_Panel16.hh"
#include "mjdemonstrator/MJDemoPoly_Panel17.hh"
#include "mjdemonstrator/MJDemoPoly_Panel18.hh"
#include "mjdemonstrator/MJDemoPoly_Panel19.hh"
#include "mjdemonstrator/MJDemoPoly_Panel20.hh"
#include "mjdemonstrator/MJDemoPoly_Panel21.hh"
#include "mjdemonstrator/MJDemoPoly_Panel22.hh"
#include "mjdemonstrator/MJDemoPoly_Panel23.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoPolyAssembly::MJDemoPolyAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "PolyShield")
{
  PanelDesign = serialNumber;

	if (PanelDesign.contains("DS0")) PanelDesign_number = 1;  //1
	else if (PanelDesign.contains("DS1")) PanelDesign_number = 51; //110011
	else if (PanelDesign.contains("DS2")) PanelDesign_number = 1151; //110011
	else if (PanelDesign.contains("DS3")) PanelDesign_number = 2772223;	
	else if (PanelDesign.contains("DS4")) PanelDesign_number = 2772233;
	else if (PanelDesign.contains("DS5a")) PanelDesign_number = 6999167;  
	else if (PanelDesign.contains("DS5b")) PanelDesign_number = 16777215;
	else if (PanelDesign.contains("DS6")) PanelDesign_number = 16777215; 
	else {
		PanelDesign_number = atoi(PanelDesign.data());
		PanelDesign = "individual";
	}

 	if (PanelDesign_number%2/1){
	  fPoly_Panel0_001 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_001", "Polyethylene");
	  fPoly_Panel0_002 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_002", "Polyethylene");
	  fPoly_Panel0_003 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_003", "Polyethylene");
	  fPoly_Panel0_004 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_004", "Polyethylene");
	  fPoly_Panel0_005 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_005", "Polyethylene");
	  fPoly_Panel0_006 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_006", "Polyethylene");
	 
	  fPoly_Panel0_007 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_007", "Polyethylene");
	  fPoly_Panel0_008 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_008", "Polyethylene");
	  fPoly_Panel0_009 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_009", "Polyethylene");
	  fPoly_Panel0_010 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_010", "Polyethylene");
	  fPoly_Panel0_011 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_011", "Polyethylene");
	  fPoly_Panel0_012 = new MJDemoPoly_Panel0(fName+"_OverFloorPanel_012", "Polyethylene");
 	}
 	if (PanelDesign_number%4/2){
	  fPoly_Panel1_001 = new MJDemoPoly_Panel1(fName+"_MainRoof_001", "Borated-Poly");
	  fPoly_Panel1_002 = new MJDemoPoly_Panel1(fName+"_MainRoof_002", "Polyethylene");
	  fPoly_Panel1_003 = new MJDemoPoly_Panel1(fName+"_MainRoof_003", "Polyethylene");
	  fPoly_Panel1_004 = new MJDemoPoly_Panel1(fName+"_MainRoof_004", "Polyethylene");
	  fPoly_Panel1_005 = new MJDemoPoly_Panel1(fName+"_MainRoof_005", "Polyethylene");
	  fPoly_Panel1_006 = new MJDemoPoly_Panel1(fName+"_MainRoof_006", "Polyethylene");
  }
 	if (PanelDesign_number%8/4){
	  fPoly_Panel2_001 = new MJDemoPoly_Panel2(fName+"_M1Roof_001", "Borated-Poly");
	  fPoly_Panel2_002 = new MJDemoPoly_Panel2(fName+"_M1Roof_002", "Polyethylene");
	  fPoly_Panel2_003 = new MJDemoPoly_Panel2(fName+"_M1Roof_003", "Polyethylene");
  }
 	if (PanelDesign_number%16/8){
	  fPoly_Panel3_001 = new MJDemoPoly_Panel3(fName+"_M2Roof_001", "Borated-Poly");
	  fPoly_Panel3_002 = new MJDemoPoly_Panel3(fName+"_M2Roof_002", "Polyethylene");
	  fPoly_Panel3_003 = new MJDemoPoly_Panel3(fName+"_M2Roof_003", "Polyethylene");
  }
 	if (PanelDesign_number%32/16){
	  fPoly_Panel4_001 = new MJDemoPoly_Panel4(fName+"_WestWallBig_001", "Borated-Poly");
	  fPoly_Panel4_002 = new MJDemoPoly_Panel4(fName+"_WestWallBig_002", "Polyethylene");
	  fPoly_Panel4_003 = new MJDemoPoly_Panel4(fName+"_WestWallBig_003", "Polyethylene");
	  fPoly_Panel4_004 = new MJDemoPoly_Panel4(fName+"_WestWallBig_004", "Polyethylene");
	  fPoly_Panel4_005 = new MJDemoPoly_Panel4(fName+"_WestWallBig_005", "Polyethylene");
	  fPoly_Panel4_006 = new MJDemoPoly_Panel4(fName+"_WestWallBig_006", "Polyethylene");
  }
 	if (PanelDesign_number%64/32){
	  fPoly_Panel5_001 = new MJDemoPoly_Panel5(fName+"_NorthWallBig_001", "Borated-Poly");
	  fPoly_Panel5_002 = new MJDemoPoly_Panel5(fName+"_NorthWallBig_002", "Polyethylene");
	  fPoly_Panel5_003 = new MJDemoPoly_Panel5(fName+"_NorthWallBig_003", "Polyethylene");
	  fPoly_Panel5_004 = new MJDemoPoly_Panel5(fName+"_NorthWallBig_004", "Polyethylene");
	  fPoly_Panel5_005 = new MJDemoPoly_Panel5(fName+"_NorthWallBig_005", "Polyethylene");
	  fPoly_Panel5_006 = new MJDemoPoly_Panel5(fName+"_NorthWallBig_006", "Polyethylene");
  }
 	if (PanelDesign_number%128/64){
	  fPoly_Panel6_001 = new MJDemoPoly_Panel6(fName+"_NorthWallSmallInnerLower_001", "Borated-Poly");
	  fPoly_Panel6_002 = new MJDemoPoly_Panel6(fName+"_NorthWallSmallInnerLower_002", "Polyethylene");
	  fPoly_Panel6_003 = new MJDemoPoly_Panel6(fName+"_NorthWallSmallInnerLower_003", "Polyethylene");
  }
 	if (PanelDesign_number%256/128){
	  fPoly_Panel7_001 = new MJDemoPoly_Panel7(fName+"_NorthWallSmallInnerUpper_001", "Borated-Poly");
	  fPoly_Panel7_002 = new MJDemoPoly_Panel7(fName+"_NorthWallSmallInnerUpper_002", "Polyethylene");
	  fPoly_Panel7_003 = new MJDemoPoly_Panel7(fName+"_NorthWallSmallInnerUpper_003", "Polyethylene");
  }
 	if (PanelDesign_number%512/256){
	  fPoly_Panel8_001 = new MJDemoPoly_Panel8(fName+"_NorthWallSmallOuterLower_001", "Polyethylene");
	  fPoly_Panel8_002 = new MJDemoPoly_Panel8(fName+"_NorthWallSmallOuterLower_002", "Polyethylene");
	  fPoly_Panel8_003 = new MJDemoPoly_Panel8(fName+"_NorthWallSmallOuterLower_003", "Polyethylene");
  }
 	if (PanelDesign_number%1024/512){
	  fPoly_Panel9_001 = new MJDemoPoly_Panel9(fName+"_NorthWallSmallOuterUpper_001", "Polyethylene");
	  fPoly_Panel9_002 = new MJDemoPoly_Panel9(fName+"_NorthWallSmallOuterUpper_002", "Polyethylene");
	  fPoly_Panel9_003 = new MJDemoPoly_Panel9(fName+"_NorthWallSmallOuterUpper_003", "Polyethylene");
  }
 	if (PanelDesign_number%2048/1024){
	  fPoly_Panel10_001 = new MJDemoPoly_Panel10(fName+"_WestWallSmallInnerLower_001", "Borated-Poly");
	  fPoly_Panel10_002 = new MJDemoPoly_Panel10(fName+"_WestWallSmallInnerLower_002", "Polyethylene");
	  fPoly_Panel10_003 = new MJDemoPoly_Panel10(fName+"_WestWallSmallInnerLower_003", "Polyethylene");
 	}
 	if (PanelDesign_number%4096/2048){
	  fPoly_Panel11_001 = new MJDemoPoly_Panel11(fName+"_WestWallSmallInnerUpper_001", "Borated-Poly");
	  fPoly_Panel11_002 = new MJDemoPoly_Panel11(fName+"_WestWallSmallInnerUpper_002", "Polyethylene");
	  fPoly_Panel11_003 = new MJDemoPoly_Panel11(fName+"_WestWallSmallInnerUpper_003", "Polyethylene");
 	}
 	if (PanelDesign_number%8192/4096){
	  fPoly_Panel12_001 = new MJDemoPoly_Panel12(fName+"_WestWallSmallOuterLower_001", "Polyethylene");
	  fPoly_Panel12_002 = new MJDemoPoly_Panel12(fName+"_WestWallSmallOuterLower_002", "Polyethylene");
	  fPoly_Panel12_003 = new MJDemoPoly_Panel12(fName+"_WestWallSmallOuterLower_003", "Polyethylene");
 	}
 	if (PanelDesign_number%16384/8192){
	  fPoly_Panel13_001 = new MJDemoPoly_Panel13(fName+"_WestWallSmallOuterUpper_001", "Polyethylene");
	  fPoly_Panel13_002 = new MJDemoPoly_Panel13(fName+"_WestWallSmallOuterUpper_002", "Polyethylene");
	  fPoly_Panel13_003 = new MJDemoPoly_Panel13(fName+"_WestWallSmallOuterUpper_003", "Polyethylene");
 	}
 	if (PanelDesign_number%32768/16384){
	  fPoly_Panel14_001 = new MJDemoPoly_Panel14(fName+"_SouthWallModuleInnerLower_001", "Borated-Poly");
	  fPoly_Panel14_002 = new MJDemoPoly_Panel14(fName+"_SouthWallModuleInnerLower_002", "Polyethylene");
	  fPoly_Panel14_003 = new MJDemoPoly_Panel14(fName+"_SouthWallModuleInnerLower_003", "Polyethylene");
 	}
 	if (PanelDesign_number%65536/32768){
	  fPoly_Panel15_001 = new MJDemoPoly_Panel15(fName+"_SouthWallModuleInnerUpper_001", "Borated-Poly");
	  fPoly_Panel15_002 = new MJDemoPoly_Panel15(fName+"_SouthWallModuleInnerUpper_002", "Polyethylene");
	  fPoly_Panel15_003 = new MJDemoPoly_Panel15(fName+"_SouthWallModuleInnerUpper_003", "Polyethylene");
 	}
 	if (PanelDesign_number%131072/65536){
	  fPoly_Panel16_001 = new MJDemoPoly_Panel16(fName+"_SouthWallModuleOuter_001", "Polyethylene");
	  fPoly_Panel16_002 = new MJDemoPoly_Panel16(fName+"_SouthWallModuleOuter_002", "Polyethylene");
	  fPoly_Panel16_003 = new MJDemoPoly_Panel16(fName+"_SouthWallModuleOuter_003", "Polyethylene");
 	}
 	if (PanelDesign_number%262144/131072){
	  fPoly_Panel17_001 = new MJDemoPoly_Panel17(fName+"_EastWallChillerInner_001", "Borated-Poly");
	  fPoly_Panel17_002 = new MJDemoPoly_Panel17(fName+"_EastWallChillerInner_002", "Polyethylene");
	  fPoly_Panel17_003 = new MJDemoPoly_Panel17(fName+"_EastWallChillerInner_003", "Polyethylene");
 	}
 	if (PanelDesign_number%524288/262144){
	  fPoly_Panel18_001 = new MJDemoPoly_Panel18(fName+"_EastWallChillerOuter_001", "Polyethylene");
	  fPoly_Panel18_002 = new MJDemoPoly_Panel18(fName+"_EastWallChillerOuter_002", "Polyethylene");
	  fPoly_Panel18_003 = new MJDemoPoly_Panel18(fName+"_EastWallChillerOuter_003", "Polyethylene");
 	}
 	if (PanelDesign_number%1048576/524288){
	  fPoly_Panel19_001 = new MJDemoPoly_Panel19(fName+"_SouthWallChillerInner_001", "Borated-Poly");
	  fPoly_Panel19_002 = new MJDemoPoly_Panel19(fName+"_SouthWallChillerInner_002", "Polyethylene");
	  fPoly_Panel19_003 = new MJDemoPoly_Panel19(fName+"_SouthWallChillerInner_003", "Polyethylene");
 	}
 	if (PanelDesign_number%2097152/1048576){
	  fPoly_Panel20_001 = new MJDemoPoly_Panel20(fName+"_SouthWallChillerOuter_001", "Polyethylene");
	  fPoly_Panel20_002 = new MJDemoPoly_Panel20(fName+"_SouthWallChillerOuter_002", "Polyethylene");
	  fPoly_Panel20_003 = new MJDemoPoly_Panel20(fName+"_SouthWallChillerOuter_003", "Polyethylene");
 	}
 	if (PanelDesign_number%4194304/2097152){
	  fPoly_Panel21_001 = new MJDemoPoly_Panel21(fName+"_EastWallModuleInnerLower_001", "Borated-Poly");
	  fPoly_Panel21_002 = new MJDemoPoly_Panel21(fName+"_EastWallModuleInnerLower_002", "Polyethylene");
	  fPoly_Panel21_003 = new MJDemoPoly_Panel21(fName+"_EastWallModuleInnerLower_003", "Polyethylene");
 	}
 	if (PanelDesign_number%8388608/4194304){
	  fPoly_Panel22_001 = new MJDemoPoly_Panel22(fName+"_EastWallModuleInnerUpper_001", "Borated-Poly");
	  fPoly_Panel22_002 = new MJDemoPoly_Panel22(fName+"_EastWallModuleInnerUpper_002", "Polyethylene");
	  fPoly_Panel22_003 = new MJDemoPoly_Panel22(fName+"_EastWallModuleInnerUpper_003", "Polyethylene");
 	}
 	if (PanelDesign_number%16777216/8388608){
	  fPoly_Panel23_001 = new MJDemoPoly_Panel23(fName+"_EastWallModuleOuter_001", "Polyethylene");
	  fPoly_Panel23_002 = new MJDemoPoly_Panel23(fName+"_EastWallModuleOuter_002", "Polyethylene");
	  fPoly_Panel23_003 = new MJDemoPoly_Panel23(fName+"_EastWallModuleOuter_003", "Polyethylene");
 	}
}

MJDemoPolyAssembly::MJDemoPolyAssembly(const MJDemoPolyAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
 
}


MJDemoPolyAssembly::~MJDemoPolyAssembly()
{
	delete fPoly_Panel0_001;
  delete fPoly_Panel0_002;
  delete fPoly_Panel0_003;
  delete fPoly_Panel0_004;
  delete fPoly_Panel0_005;
  delete fPoly_Panel0_006;
  delete fPoly_Panel0_007;
  delete fPoly_Panel0_008;
  delete fPoly_Panel0_009;
  delete fPoly_Panel0_010;
  delete fPoly_Panel0_011;
  delete fPoly_Panel0_012;

  delete fPoly_Panel1_001;
  delete fPoly_Panel1_002;
  delete fPoly_Panel1_003;
  delete fPoly_Panel1_004;
  delete fPoly_Panel1_005;
  delete fPoly_Panel1_006;

  delete fPoly_Panel2_001;
  delete fPoly_Panel2_002;
  delete fPoly_Panel2_003;

  delete fPoly_Panel3_001;
  delete fPoly_Panel3_002;
  delete fPoly_Panel3_003;

  delete fPoly_Panel4_001;
  delete fPoly_Panel4_002;
  delete fPoly_Panel4_003;
  delete fPoly_Panel4_004;
  delete fPoly_Panel4_005;
  delete fPoly_Panel4_006;

  delete fPoly_Panel5_001;
  delete fPoly_Panel5_002;
  delete fPoly_Panel5_003;
  delete fPoly_Panel5_004;
  delete fPoly_Panel5_005;
  delete fPoly_Panel5_006;

  delete fPoly_Panel6_001;
  delete fPoly_Panel6_002;
  delete fPoly_Panel6_003;

  delete fPoly_Panel7_001;
  delete fPoly_Panel7_002;
  delete fPoly_Panel7_003;

  delete fPoly_Panel8_001;
  delete fPoly_Panel8_002;
  delete fPoly_Panel8_003;

  delete fPoly_Panel9_001;
  delete fPoly_Panel9_002;
  delete fPoly_Panel9_003;

	delete fPoly_Panel10_001;
  delete fPoly_Panel10_002;
  delete fPoly_Panel10_003;

  delete fPoly_Panel11_001;
  delete fPoly_Panel11_002;
  delete fPoly_Panel11_003;

  delete fPoly_Panel12_001;
  delete fPoly_Panel12_002;
  delete fPoly_Panel12_003;

  delete fPoly_Panel13_001;
  delete fPoly_Panel13_002;
  delete fPoly_Panel13_003;

  delete fPoly_Panel14_001;
  delete fPoly_Panel14_002;
  delete fPoly_Panel14_003;

  delete fPoly_Panel15_001;
  delete fPoly_Panel15_002;
  delete fPoly_Panel15_003;

  delete fPoly_Panel16_001;
  delete fPoly_Panel16_002;
  delete fPoly_Panel16_003;

  delete fPoly_Panel17_001;
  delete fPoly_Panel17_002;
  delete fPoly_Panel17_003;

  delete fPoly_Panel18_001;
  delete fPoly_Panel18_002;
  delete fPoly_Panel18_003;

  delete fPoly_Panel19_001;
  delete fPoly_Panel19_002;
  delete fPoly_Panel19_003;

	delete fPoly_Panel20_001;
  delete fPoly_Panel20_002;
  delete fPoly_Panel20_003;

  delete fPoly_Panel21_001;
  delete fPoly_Panel21_002;
  delete fPoly_Panel21_003;

  delete fPoly_Panel22_001;
  delete fPoly_Panel22_002;
  delete fPoly_Panel22_003;

  delete fPoly_Panel23_001;
  delete fPoly_Panel23_002;
  delete fPoly_Panel23_003;

}



void MJDemoPolyAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);
  
	MGLog(routine) << "============  Place the poly shield  ============" << endlog;
	MGLog(routine) << "= " << PanelDesign << endlog;
	MGLog(routine) << "= " << PanelDesign_number << endlog;
	MGLog(debugging) << "= Overfloor                : " << PanelDesign_number%2/1 << endlog;
	MGLog(debugging) << "= MainRoof                 : " << PanelDesign_number%4/2 << endlog;
	MGLog(debugging) << "= Roof above M1 VacSystem  : " << PanelDesign_number%8/4 << endlog;
	MGLog(debugging) << "= Roof above M2 VacSystem  : " << PanelDesign_number%16/8 << endlog;
	MGLog(debugging) << "= West Wall Big            : " << PanelDesign_number%32/16 << endlog;
	MGLog(debugging) << "= North Wall Big           : " << PanelDesign_number%64/32 << endlog;
	MGLog(debugging) << "= North Wall Small In Low  : " << PanelDesign_number%128/64 << endlog;
	MGLog(debugging) << "= North Wall Small In Up   : " << PanelDesign_number%256/128 << endlog;
	MGLog(debugging) << "= North Wall Small Out Low : " << PanelDesign_number%512/256 << endlog;
	MGLog(debugging) << "= North Wall Small Out Up  : " << PanelDesign_number%1024/512 << endlog;
	MGLog(debugging) << "= West Wall Small In Low   : " << PanelDesign_number%2048/1024 << endlog;
	MGLog(debugging) << "= West Wall Small In Up    : " << PanelDesign_number%4096/2048 << endlog;
	MGLog(debugging) << "= West Wall Small Out Low  : " << PanelDesign_number%8192/4096 << endlog;
	MGLog(debugging) << "= West Wall Small Out Up   : " << PanelDesign_number%16384/8192 << endlog;
	MGLog(debugging) << "= South Wall Module In Low : " << PanelDesign_number%32768/16384 << endlog;
	MGLog(debugging) << "= South Wall Module In Up  : " << PanelDesign_number%65536/32768 << endlog;
	MGLog(debugging) << "= South Wall Module Out    : " << PanelDesign_number%131072/65536 << endlog;
	MGLog(debugging) << "= East Wall Chiller In     : " << PanelDesign_number%262144/131072 << endlog;
	MGLog(debugging) << "= East Wall Chiller Out    : " << PanelDesign_number%524288/262144 << endlog;
	MGLog(debugging) << "= South Wall Chiller In    : " << PanelDesign_number%1048576/524288 << endlog;
	MGLog(debugging) << "= South Wall Chiller Out   : " << PanelDesign_number%2097152/1048576 << endlog;
	MGLog(debugging) << "= East Wall Module In Low  : " << PanelDesign_number%4194304/2097152 << endlog;
	MGLog(debugging) << "= East Wall Module In Up   : " << PanelDesign_number%8388608/4194304 << endlog;
	MGLog(debugging) << "= East Wall Module Out     : " << PanelDesign_number%16777216/8388608 << endlog;
	MGLog(routine) << "==================================================" << endlog;

  ///////////////////////////////////////////////////////////////////
  //Overfloor panels 1-6 upper 7-12 lower
	// move in X (88.9), Y (-70), z(50.8) relative to Mary's original design.
	// because West wall is placement more accurate and old design would cause overlaps
  if (PanelDesign_number%2/1){
		for (int i = 1; i<=12; i++){
	 	 	G4ThreeVector *polyPanel0_localPos = new G4ThreeVector(0, 0, 0);
	 	 	if (i<7){
				polyPanel0_localPos->setX((-2*i+7)*181.61*mm + 88.9*mm);	
				polyPanel0_localPos->setY(-70*mm);
				polyPanel0_localPos->setZ(-1054.084*mm + 50.8*mm);
	 	 	}
	  	else{
				polyPanel0_localPos->setX(88.9*mm);
				polyPanel0_localPos->setY((-2*i+19)*181.61*mm - 70*mm);
				polyPanel0_localPos->setZ(-1257.284*mm + 50.8*mm);
	  	}
	  
	  	G4RotationMatrix *polyPanel0_localRot = new G4RotationMatrix();
	 	 	if (i<7) polyPanel0_localRot->rotateZ(90.0*deg);
	  	G4AffineTransform *polyPanel0_affine = new G4AffineTransform(polyPanel0_localRot, *polyPanel0_localPos);
	  	*polyPanel0_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel0_globalPos = new G4ThreeVector(polyPanel0_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel0_globalRot= new G4RotationMatrix(polyPanel0_affine->NetRotation());
	  	if (i==1)  fPoly_Panel0_001->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel0_002->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel0_003->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
		  else if (i==4)  fPoly_Panel0_004->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
		  else if (i==5)  fPoly_Panel0_005->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
		  else if (i==6)  fPoly_Panel0_006->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
		  else if (i==7)  fPoly_Panel0_007->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
		  else if (i==8)  fPoly_Panel0_008->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
		  else if (i==9)  fPoly_Panel0_009->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
		  else if (i==10)  fPoly_Panel0_010->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
		  else if (i==11)  fPoly_Panel0_011->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
		  else if (i==12)  fPoly_Panel0_012->Place(polyPanel0_globalPos, polyPanel0_globalRot, motherLogical);
	  
		  delete polyPanel0_localPos;
		  delete polyPanel0_localRot;
		  delete polyPanel0_affine;
		}
  }
	///////////////////////////////////////////////////////////////////
	// all location from Matthew B dimensions spreadsheet. all in m, everything with xx/2 inch are the panel widths
	// add 3inch for all z values to Matthews z=0
	// small changes in Matthews sheet to avoid overlaps:
	// use 947, 406, and 1169 as fixed 
	// 2302 => 2304.6
	// 937 => 939.2
	// 1871 => 1872.65
	// 531 => 532.95
  // 1021 => 1021.9
  // 1227 => 1226.4
  // 1338 => 1339 (floor level)
	G4double poly_zoffset = 3*25.4*mm;
	///////////////////////////////////////////////////////////////////
  //MainRoof
  if (PanelDesign_number%4/2){
		for (int i = 1; i<=6; i++){
	 	 	G4ThreeVector *polyPanel1_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel1_localPos->setX(-115.85*mm); //0.203 + 1.021 -105.5/2 inch
			polyPanel1_localPos->setY(321.275*mm); //1.169 + 12*1inch - 90.75/2 inch
			polyPanel1_localPos->setZ(947*mm + poly_zoffset + (i-0.5)*(2*25.4*mm));  //0.947 + half panel + offset
	  
	  	G4RotationMatrix *polyPanel1_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel1_affine = new G4AffineTransform(polyPanel1_localRot, *polyPanel1_localPos);
	  	*polyPanel1_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel1_globalPos = new G4ThreeVector(polyPanel1_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel1_globalRot= new G4RotationMatrix(polyPanel1_affine->NetRotation());
			if (i==1)  fPoly_Panel1_001->Place(polyPanel1_globalPos, polyPanel1_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel1_002->Place(polyPanel1_globalPos, polyPanel1_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel1_003->Place(polyPanel1_globalPos, polyPanel1_globalRot, motherLogical);
		  else if (i==4)  fPoly_Panel1_004->Place(polyPanel1_globalPos, polyPanel1_globalRot, motherLogical);
		  else if (i==5)  fPoly_Panel1_005->Place(polyPanel1_globalPos, polyPanel1_globalRot, motherLogical);
		  else if (i==6)  fPoly_Panel1_006->Place(polyPanel1_globalPos, polyPanel1_globalRot, motherLogical);
	  
		  delete polyPanel1_localPos;
		  delete polyPanel1_localRot;
		  delete polyPanel1_affine;
		}
  }
	///////////////////////////////////////////////////////////////////
  //Roof above M1 Vac system 
  if (PanelDesign_number%8/4){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel2_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel2_localPos->setX(37.5*mm-0.06*mm);  //-0.203 - 0.947 + 93.5/2 inch
			polyPanel2_localPos->setY(-1351.95*mm );	//-1872.65 + 41/2 inch
			polyPanel2_localPos->setZ(1226.4*mm + poly_zoffset + (i-0.5)*(2*25.4*mm));  //1.227 + half panel + offset
	  	G4RotationMatrix *polyPanel2_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel2_affine = new G4AffineTransform(polyPanel2_localRot, *polyPanel2_localPos);
	  	*polyPanel2_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel2_globalPos = new G4ThreeVector(polyPanel2_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel2_globalRot= new G4RotationMatrix(polyPanel2_affine->NetRotation());
			if (i==1)  fPoly_Panel2_001->Place(polyPanel2_globalPos, polyPanel2_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel2_002->Place(polyPanel2_globalPos, polyPanel2_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel2_003->Place(polyPanel2_globalPos, polyPanel2_globalRot, motherLogical);

		  delete polyPanel2_localPos;
		  delete polyPanel2_localRot;
		  delete polyPanel2_affine;
		}
  }
  ///////////////////////////////////////////////////////////////////
  //Roof above M2 Vac system 
  if (PanelDesign_number%16/8){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel3_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel3_localPos->setX(1866.25*mm); //1.021.9 + 0.203 + 50.5/2 inch
			polyPanel3_localPos->setY(114.9*mm);  //1.169 - 83/2 inch
			polyPanel3_localPos->setZ(1226.4*mm + poly_zoffset + (i-0.5)*(2*25.4*mm));  //1.227 + half panel + offset
	  	G4RotationMatrix *polyPanel3_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel3_affine = new G4AffineTransform(polyPanel3_localRot, *polyPanel3_localPos);
	  	*polyPanel3_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel3_globalPos = new G4ThreeVector(polyPanel3_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel3_globalRot= new G4RotationMatrix(polyPanel3_affine->NetRotation());
			if (i==1)  fPoly_Panel3_001->Place(polyPanel3_globalPos, polyPanel3_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel3_002->Place(polyPanel3_globalPos, polyPanel3_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel3_003->Place(polyPanel3_globalPos, polyPanel3_globalRot, motherLogical);
	  
		  delete polyPanel3_localPos;
		  delete polyPanel3_localRot;
		  delete polyPanel3_affine;
		}
  }
  ///////////////////////////////////////////////////////////////////
  //Big West Wall
  if (PanelDesign_number%32/16){
		for (int i = 1; i<=6; i++){
	 	 	G4ThreeVector *polyPanel4_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel4_localPos->setX(-1150*mm - (i-0.5)*(2*25.4*mm)); //0.203 + 0.947 + half a panel
			polyPanel4_localPos->setY(501.869*mm+0.762); //1.169 + 12/1inch -76.53/2
			polyPanel4_localPos->setZ(-196.4*mm + poly_zoffset);  //offset + 0.947 - 90/2 inch
	  
	  	G4RotationMatrix *polyPanel4_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel4_affine = new G4AffineTransform(polyPanel4_localRot, *polyPanel4_localPos);
	  	*polyPanel4_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel4_globalPos = new G4ThreeVector(polyPanel4_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel4_globalRot= new G4RotationMatrix(polyPanel4_affine->NetRotation());
			if (i==1)  fPoly_Panel4_001->Place(polyPanel4_globalPos, polyPanel4_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel4_002->Place(polyPanel4_globalPos, polyPanel4_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel4_003->Place(polyPanel4_globalPos, polyPanel4_globalRot, motherLogical);
		  else if (i==4)  fPoly_Panel4_004->Place(polyPanel4_globalPos, polyPanel4_globalRot, motherLogical);
		  else if (i==5)  fPoly_Panel4_005->Place(polyPanel4_globalPos, polyPanel4_globalRot, motherLogical);
		  else if (i==6)  fPoly_Panel4_006->Place(polyPanel4_globalPos, polyPanel4_globalRot, motherLogical);
	  
		  delete polyPanel4_localPos;
		  delete polyPanel4_localRot;
		  delete polyPanel4_affine;
		}
  }
  ///////////////////////////////////////////////////////////////////
  //Big North Wall
  if (PanelDesign_number%64/32){
		for (int i = 1; i<=6; i++){
	 	 	G4ThreeVector *polyPanel5_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel5_localPos->setX(-64.15*mm ); //0.947 + 0.203 + 85.5/2
			polyPanel5_localPos->setY(1169*mm + (i-0.5)*(2*25.4*mm)); //1.169 + half a panel
			polyPanel5_localPos->setZ(19.9*mm + poly_zoffset);  //offset + 0.947 - 73/2 inch
	  
	  	G4RotationMatrix *polyPanel5_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel5_affine = new G4AffineTransform(polyPanel5_localRot, *polyPanel5_localPos);
	  	*polyPanel5_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel5_globalPos = new G4ThreeVector(polyPanel5_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel5_globalRot= new G4RotationMatrix(polyPanel5_affine->NetRotation());
			if (i==1)  fPoly_Panel5_001->Place(polyPanel5_globalPos, polyPanel5_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel5_002->Place(polyPanel5_globalPos, polyPanel5_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel5_003->Place(polyPanel5_globalPos, polyPanel5_globalRot, motherLogical);
		  else if (i==4)  fPoly_Panel5_004->Place(polyPanel5_globalPos, polyPanel5_globalRot, motherLogical);
		  else if (i==5)  fPoly_Panel5_005->Place(polyPanel5_globalPos, polyPanel5_globalRot, motherLogical);
		  else if (i==6)  fPoly_Panel5_006->Place(polyPanel5_globalPos, polyPanel5_globalRot, motherLogical);
	  
		  delete polyPanel5_localPos;
		  delete polyPanel5_localRot;
		  delete polyPanel5_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //Small North Wall Inside Lower
  if (PanelDesign_number%128/64){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel6_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel6_localPos->setX(1764.65*mm ); //2304.6 + 0.203 - 58.5/2
			polyPanel6_localPos->setY(1169*mm + (i-0.5)*(2*25.4*mm)); //1.169 + half a panel
			polyPanel6_localPos->setZ(-145.2*mm + poly_zoffset);  //offset + 947 - 73*25.4 + 60/2 //same level as big wall
	  
	  	G4RotationMatrix *polyPanel6_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel6_affine = new G4AffineTransform(polyPanel6_localRot, *polyPanel6_localPos);
	  	*polyPanel6_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel6_globalPos = new G4ThreeVector(polyPanel6_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel6_globalRot= new G4RotationMatrix(polyPanel6_affine->NetRotation());
			if (i==1)  fPoly_Panel6_001->Place(polyPanel6_globalPos, polyPanel6_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel6_002->Place(polyPanel6_globalPos, polyPanel6_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel6_003->Place(polyPanel6_globalPos, polyPanel6_globalRot, motherLogical);
		  delete polyPanel6_localPos;
		  delete polyPanel6_localRot;
		  delete polyPanel6_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //Small North Wall Inside Upper
  if (PanelDesign_number%256/128){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel7_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel7_localPos->setX(1764.65*mm ); //2304.6 + 0.203 - 58.5/2
			polyPanel7_localPos->setY(1169*mm + (i-0.5)*(2*25.4*mm)); //1.169 + half a panel
			polyPanel7_localPos->setZ(997.8*mm + poly_zoffset);  //offset + 947 - 73*25.4 + 60(lower) + (5*6)/2
	  
	  	G4RotationMatrix *polyPanel7_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel7_affine = new G4AffineTransform(polyPanel7_localRot, *polyPanel7_localPos);
	  	*polyPanel7_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel7_globalPos = new G4ThreeVector(polyPanel7_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel7_globalRot= new G4RotationMatrix(polyPanel7_affine->NetRotation());
			if (i==1)  fPoly_Panel7_001->Place(polyPanel7_globalPos, polyPanel7_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel7_002->Place(polyPanel7_globalPos, polyPanel7_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel7_003->Place(polyPanel7_globalPos, polyPanel7_globalRot, motherLogical);
		  delete polyPanel7_localPos;
		  delete polyPanel7_localRot;
		  delete polyPanel7_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //Small North Wall Outside Lower
  if (PanelDesign_number%512/256){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel8_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel8_localPos->setX(1840.85*mm + 25.146*mm); //2304.6 + 0.203 + 6inch inner - 64.5/2 + gap
			polyPanel8_localPos->setY(1169*mm + (i-0.5+3)*(2*25.4*mm) + 26.162*mm); //1.169 + half a panel + inner panels + gap
			polyPanel8_localPos->setZ(-145.2*mm + poly_zoffset);  //offset + 947 - 73*25.4 + 60/2 //same level as big wall
	  
	  	G4RotationMatrix *polyPanel8_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel8_affine = new G4AffineTransform(polyPanel8_localRot, *polyPanel8_localPos);
	  	*polyPanel8_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel8_globalPos = new G4ThreeVector(polyPanel8_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel8_globalRot= new G4RotationMatrix(polyPanel8_affine->NetRotation());
			if (i==1)  fPoly_Panel8_001->Place(polyPanel8_globalPos, polyPanel8_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel8_002->Place(polyPanel8_globalPos, polyPanel8_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel8_003->Place(polyPanel8_globalPos, polyPanel8_globalRot, motherLogical);
		  delete polyPanel8_localPos;
		  delete polyPanel8_localRot;
		  delete polyPanel8_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //Small North Wall Outside Upper
  if (PanelDesign_number%1024/512){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel9_localPos = new G4ThreeVector(0, 0, 0); 
			polyPanel9_localPos->setX(1840.85*mm + 25.146*mm); //2304.6 + 0.203 + 6inch inner - 64.5/2 + gap
			polyPanel9_localPos->setY(1169*mm + (i-0.5+3)*(2*25.4*mm) + 26.162*mm); //1.169 + half a panel + inner panels + gap
			polyPanel9_localPos->setZ(997.8*mm + poly_zoffset);  //offset + 947 - 73*25.4 + 60(lower) + (5*6)/2
	  
	  	G4RotationMatrix *polyPanel9_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel9_affine = new G4AffineTransform(polyPanel9_localRot, *polyPanel9_localPos);
	  	*polyPanel9_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel9_globalPos = new G4ThreeVector(polyPanel9_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel9_globalRot= new G4RotationMatrix(polyPanel9_affine->NetRotation());
			if (i==1)  fPoly_Panel9_001->Place(polyPanel9_globalPos, polyPanel9_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel9_002->Place(polyPanel9_globalPos, polyPanel9_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel9_003->Place(polyPanel9_globalPos, polyPanel9_globalRot, motherLogical);
		  delete polyPanel9_localPos;
		  delete polyPanel9_localRot;
		  delete polyPanel9_affine;
		}
  }

	///////////////////////////////////////////////////////////////////
  //Small West Wall Inside Lower
  if (PanelDesign_number%2048/1024){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel10_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel10_localPos->setX(-1150*mm - (i-0.5)*(2*25.4*mm)-0.06*mm); //-0.947 - 0.203 + half a panel
			polyPanel10_localPos->setY(-1170.975*mm ); //-1872.65 + 55.25/2
			polyPanel10_localPos->setZ(-551.6*mm + poly_zoffset);  //offset -1339 + 62/2 //same level as big wall
	  
	  	G4RotationMatrix *polyPanel10_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel10_affine = new G4AffineTransform(polyPanel10_localRot, *polyPanel10_localPos);
	  	*polyPanel10_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel10_globalPos = new G4ThreeVector(polyPanel10_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel10_globalRot= new G4RotationMatrix(polyPanel10_affine->NetRotation());
			if (i==1)  fPoly_Panel10_001->Place(polyPanel10_globalPos, polyPanel10_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel10_002->Place(polyPanel10_globalPos, polyPanel10_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel10_003->Place(polyPanel10_globalPos, polyPanel10_globalRot, motherLogical);
		  delete polyPanel10_localPos;
		  delete polyPanel10_localRot;
		  delete polyPanel10_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //Small West Wall Inside Upper
  if (PanelDesign_number%4096/2048){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel11_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel11_localPos->setX(-1150*mm - (i-0.5)*(2*25.4*mm)-1*mm); //-0.947 - 0.203 + half a panel
			polyPanel11_localPos->setY(-1170.975*mm ); //-1872.65 + 55.25/2
			polyPanel11_localPos->setZ(807.3*mm + poly_zoffset);  //offset -1339 + 62(lower) + (5*9)/2
	  
	  	G4RotationMatrix *polyPanel11_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel11_affine = new G4AffineTransform(polyPanel11_localRot, *polyPanel11_localPos);
	  	*polyPanel11_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel11_globalPos = new G4ThreeVector(polyPanel11_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel11_globalRot= new G4RotationMatrix(polyPanel11_affine->NetRotation());
			if (i==1)  fPoly_Panel11_001->Place(polyPanel11_globalPos, polyPanel11_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel11_002->Place(polyPanel11_globalPos, polyPanel11_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel11_003->Place(polyPanel11_globalPos, polyPanel11_globalRot, motherLogical);
		  delete polyPanel11_localPos;
		  delete polyPanel11_localRot;
		  delete polyPanel11_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //Small West Wall Outside Lower
  if (PanelDesign_number%8192/4096){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel12_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel12_localPos->setX(-1150*mm - (i-0.5)*(2*25.4*mm) - 179.57*mm); //-0.947 - 0.203 + half a panel and gap so it lines up with Panel 16
			polyPanel12_localPos->setY(-1247.175*mm - 26.162*mm); //-1872.65 + 61.25/2 - 6inch inner + gap
			polyPanel12_localPos->setZ(-551.6*mm + poly_zoffset);  //offset -1339 + 62/2 //same level as big wall
	  
	  	G4RotationMatrix *polyPanel12_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel12_affine = new G4AffineTransform(polyPanel12_localRot, *polyPanel12_localPos);
	  	*polyPanel12_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel12_globalPos = new G4ThreeVector(polyPanel12_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel12_globalRot= new G4RotationMatrix(polyPanel12_affine->NetRotation());
			if (i==1)  fPoly_Panel12_001->Place(polyPanel12_globalPos, polyPanel12_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel12_002->Place(polyPanel12_globalPos, polyPanel12_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel12_003->Place(polyPanel12_globalPos, polyPanel12_globalRot, motherLogical);
		  delete polyPanel12_localPos;
		  delete polyPanel12_localRot;
		  delete polyPanel12_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //Small West Wall Outside Upper
  if (PanelDesign_number%16384/8192){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel13_localPos = new G4ThreeVector(0, 0, 0); 
			polyPanel13_localPos->setX(-1150*mm - (i-0.5)*(2*25.4*mm) - 179.57*mm); //-0.947 - 0.203 + half a panel and gap so it lines up with Panel 16
			polyPanel13_localPos->setY(-1247.175*mm - 26.162*mm); //-1872.65 + 61.25/2 - 6inch inner + gap
			polyPanel13_localPos->setZ(807.3*mm + poly_zoffset);  //offset -1339 + 62(lower) + (5*9)/2
	  
	  	G4RotationMatrix *polyPanel13_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel13_affine = new G4AffineTransform(polyPanel13_localRot, *polyPanel13_localPos);
	  	*polyPanel13_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel13_globalPos = new G4ThreeVector(polyPanel13_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel13_globalRot= new G4RotationMatrix(polyPanel13_affine->NetRotation());
			if (i==1)  fPoly_Panel13_001->Place(polyPanel13_globalPos, polyPanel13_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel13_002->Place(polyPanel13_globalPos, polyPanel13_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel13_003->Place(polyPanel13_globalPos, polyPanel13_globalRot, motherLogical);
		  delete polyPanel13_localPos;
		  delete polyPanel13_localRot;
		  delete polyPanel13_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //South Wall behind the Module Inside Lower
  if (PanelDesign_number%32768/16384){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel14_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel14_localPos->setX(-207.025*mm ); //-0.947 - 0.203 + -6inch + 86.25/2
			polyPanel14_localPos->setY(-1872.65*mm - (i-0.5)*(2*25.4*mm) ); //-1872.65 
			polyPanel14_localPos->setZ(-551.6*mm + poly_zoffset);  //offset -1339 +62/2
	  
	  	G4RotationMatrix *polyPanel14_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel14_affine = new G4AffineTransform(polyPanel14_localRot, *polyPanel14_localPos);
	  	*polyPanel14_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel14_globalPos = new G4ThreeVector(polyPanel14_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel14_globalRot= new G4RotationMatrix(polyPanel14_affine->NetRotation());
			if (i==1)  fPoly_Panel14_001->Place(polyPanel14_globalPos, polyPanel14_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel14_002->Place(polyPanel14_globalPos, polyPanel14_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel14_003->Place(polyPanel14_globalPos, polyPanel14_globalRot, motherLogical);
		  delete polyPanel14_localPos;
		  delete polyPanel14_localRot;
		  delete polyPanel14_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //South Wall behind the Module Inside Upper
  if (PanelDesign_number%65536/32768){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel15_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel15_localPos->setX(-207.025*mm ); //-0.947 - 0.203 + -6inch + 86.25/2
			polyPanel15_localPos->setY(-1872.65*mm - (i-0.5)*(2*25.4*mm) ); //-1872.65 
			polyPanel15_localPos->setZ(807.3*mm + poly_zoffset);  //offset -1339 +62 + 45/2
	  
	  	G4RotationMatrix *polyPanel15_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel15_affine = new G4AffineTransform(polyPanel15_localRot, *polyPanel15_localPos);
	  	*polyPanel15_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel15_globalPos = new G4ThreeVector(polyPanel15_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel15_globalRot= new G4RotationMatrix(polyPanel15_affine->NetRotation());
			if (i==1)  fPoly_Panel15_001->Place(polyPanel15_globalPos, polyPanel15_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel15_002->Place(polyPanel15_globalPos, polyPanel15_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel15_003->Place(polyPanel15_globalPos, polyPanel15_globalRot, motherLogical);
		  delete polyPanel15_localPos;
		  delete polyPanel15_localRot;
		  delete polyPanel15_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //South Wall behind the Module Outside
  if (PanelDesign_number%131072/65536){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel16_localPos = new G4ThreeVector(0, 0, 0); 
			polyPanel16_localPos->setX(-207.025*mm - 1.016*mm); //align holes
			polyPanel16_localPos->setY(-2025.05*mm - (i-0.5)*(2*25.4*mm) -26.162*mm); //-1872.65 - 6inch inner + gap
			polyPanel16_localPos->setZ(19.9*mm + poly_zoffset);  //offset -1339 + 107/2
	  
	  	G4RotationMatrix *polyPanel16_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel16_affine = new G4AffineTransform(polyPanel16_localRot, *polyPanel16_localPos);
	  	*polyPanel16_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel16_globalPos = new G4ThreeVector(polyPanel16_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel16_globalRot= new G4RotationMatrix(polyPanel16_affine->NetRotation());
			if (i==1)  fPoly_Panel16_001->Place(polyPanel16_globalPos, polyPanel16_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel16_002->Place(polyPanel16_globalPos, polyPanel16_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel16_003->Place(polyPanel16_globalPos, polyPanel16_globalRot, motherLogical);
		  delete polyPanel16_localPos;
		  delete polyPanel16_localRot;
		  delete polyPanel16_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //East Wall Chiller Part Inside 
  if (PanelDesign_number%262144/131072){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel17_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel17_localPos->setX(735.95*mm + (i-0.5)*(2*25.4*mm)); //0.53295 + 0.203 
			polyPanel17_localPos->setY(-1405.925*mm); //-1872.65 +36.75/2
			polyPanel17_localPos->setZ(235.8*mm + poly_zoffset);  //offset +1226.4 + 6inch -90/2
	  
	  	G4RotationMatrix *polyPanel17_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel17_affine = new G4AffineTransform(polyPanel17_localRot, *polyPanel17_localPos);
	  	*polyPanel17_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel17_globalPos = new G4ThreeVector(polyPanel17_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel17_globalRot= new G4RotationMatrix(polyPanel17_affine->NetRotation());
			if (i==1)  fPoly_Panel17_001->Place(polyPanel17_globalPos, polyPanel17_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel17_002->Place(polyPanel17_globalPos, polyPanel17_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel17_003->Place(polyPanel17_globalPos, polyPanel17_globalRot, motherLogical);
		  delete polyPanel17_localPos;
		  delete polyPanel17_localRot;
		  delete polyPanel17_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //East Wall Chiller Part Outside
  if (PanelDesign_number%524288/262144){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel18_localPos = new G4ThreeVector(0, 0, 0); 
			polyPanel18_localPos->setX(888.35*mm + (i-0.5)*(2*25.4*mm) + 25.146*mm); //0.53295 + 0.203 +6inner +gap
			polyPanel18_localPos->setY(-1558.325*mm -26.162*mm); //-1872.65 - 6inch inner -36.75/2 + gap to line up holes
			polyPanel18_localPos->setZ(235.8*mm + poly_zoffset);  //offset +1226.4 + 6inch -90/2
	  
	  	G4RotationMatrix *polyPanel18_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel18_affine = new G4AffineTransform(polyPanel18_localRot, *polyPanel18_localPos);
	  	*polyPanel18_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel18_globalPos = new G4ThreeVector(polyPanel18_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel18_globalRot= new G4RotationMatrix(polyPanel18_affine->NetRotation());
			if (i==1)  fPoly_Panel18_001->Place(polyPanel18_globalPos, polyPanel18_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel18_002->Place(polyPanel18_globalPos, polyPanel18_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel18_003->Place(polyPanel18_globalPos, polyPanel18_globalRot, motherLogical);
		  delete polyPanel18_localPos;
		  delete polyPanel18_localRot;
		  delete polyPanel18_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //South Wall Chiller Part Inside 
  if (PanelDesign_number%1048576/524288){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel19_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel19_localPos->setX(1697.975*mm); //0.53295 + 0.203 + 6inch + 63.75/2
			polyPanel19_localPos->setY(-939.2*mm - (i-0.5)*(2*25.4*mm)); //-0.9392
			polyPanel19_localPos->setZ(235.8*mm + poly_zoffset);  //offset +1226.4 + 6inch -90/2
	  
	  	G4RotationMatrix *polyPanel19_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel19_affine = new G4AffineTransform(polyPanel19_localRot, *polyPanel19_localPos);
	  	*polyPanel19_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel19_globalPos = new G4ThreeVector(polyPanel19_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel19_globalRot= new G4RotationMatrix(polyPanel19_affine->NetRotation());
			if (i==1)  fPoly_Panel19_001->Place(polyPanel19_globalPos, polyPanel19_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel19_002->Place(polyPanel19_globalPos, polyPanel19_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel19_003->Place(polyPanel19_globalPos, polyPanel19_globalRot, motherLogical);
		  delete polyPanel19_localPos;
		  delete polyPanel19_localRot;
		  delete polyPanel19_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //South Wall Chiller Part Outside
  if (PanelDesign_number%2097152/1048576){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel20_localPos = new G4ThreeVector(0, 0, 0); 
			polyPanel20_localPos->setX(1850.375*mm + 25.146*mm); //0.53295 + 0.203 +12inch +gap +63.75/2
			polyPanel20_localPos->setY(-1091.6*mm - (i-0.5)*(2*25.4*mm) - 26.162*mm); //-0.9392 + 6inner +gap
			polyPanel20_localPos->setZ(235.8*mm + poly_zoffset);  //offset +1226.4 + 6inch -90/2
	  
	  	G4RotationMatrix *polyPanel20_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel20_affine = new G4AffineTransform(polyPanel20_localRot, *polyPanel20_localPos);
	  	*polyPanel20_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel20_globalPos = new G4ThreeVector(polyPanel20_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel20_globalRot= new G4RotationMatrix(polyPanel20_affine->NetRotation());
			if (i==1)  fPoly_Panel20_001->Place(polyPanel20_globalPos, polyPanel20_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel20_002->Place(polyPanel20_globalPos, polyPanel20_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel20_003->Place(polyPanel20_globalPos, polyPanel20_globalRot, motherLogical);
		  delete polyPanel20_localPos;
		  delete polyPanel20_localRot;
		  delete polyPanel20_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //East Wall behind the Module Inside Lower
  if (PanelDesign_number%4194304/2097152){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel21_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel21_localPos->setX(2507.6 + (i-0.5)*(2*25.4*mm)); //2304.6 + 203
			polyPanel21_localPos->setY(114.9*mm); //1.169+ 6inner- 95/2 
			polyPanel21_localPos->setZ(-551.6*mm + poly_zoffset);  //offset +1226.4 - 45 -62/2
	  
	  	G4RotationMatrix *polyPanel21_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel21_affine = new G4AffineTransform(polyPanel21_localRot, *polyPanel21_localPos);
	  	*polyPanel21_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel21_globalPos = new G4ThreeVector(polyPanel21_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel21_globalRot= new G4RotationMatrix(polyPanel21_affine->NetRotation());
			if (i==1)  fPoly_Panel21_001->Place(polyPanel21_globalPos, polyPanel21_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel21_002->Place(polyPanel21_globalPos, polyPanel21_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel21_003->Place(polyPanel21_globalPos, polyPanel21_globalRot, motherLogical);
		  delete polyPanel21_localPos;
		  delete polyPanel21_localRot;
		  delete polyPanel21_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //East Wall behind the Module Inside Upper
  if (PanelDesign_number%8388608/4194304){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel22_localPos = new G4ThreeVector(0, 0, 0);
			polyPanel22_localPos->setX(2507.6 + (i-0.5)*(2*25.4*mm)); //2304.6 + 203
			polyPanel22_localPos->setY(114.9*mm); //
			polyPanel22_localPos->setZ(807.3*mm + poly_zoffset);  //offset -1339 +62 + 45/2
	  
	  	G4RotationMatrix *polyPanel22_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel22_affine = new G4AffineTransform(polyPanel22_localRot, *polyPanel22_localPos);
	  	*polyPanel22_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel22_globalPos = new G4ThreeVector(polyPanel22_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel22_globalRot= new G4RotationMatrix(polyPanel22_affine->NetRotation());
			if (i==1)  fPoly_Panel22_001->Place(polyPanel22_globalPos, polyPanel22_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel22_002->Place(polyPanel22_globalPos, polyPanel22_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel22_003->Place(polyPanel22_globalPos, polyPanel22_globalRot, motherLogical);
		  delete polyPanel22_localPos;
		  delete polyPanel22_localRot;
		  delete polyPanel22_affine;
		}
  }

  ///////////////////////////////////////////////////////////////////
  //East Wall behind the Module Outside
  if (PanelDesign_number%16777216/8388608){
		for (int i = 1; i<=3; i++){
	 	 	G4ThreeVector *polyPanel23_localPos = new G4ThreeVector(0, 0, 0); 
			polyPanel23_localPos->setX(2507.6 + (i-0.5+3)*(2*25.4*mm) + 25.146); //2304.6 + 203 + 6inner +gap
			polyPanel23_localPos->setY(114.9*mm); //same as inner
			polyPanel23_localPos->setZ(19.9*mm + poly_zoffset);  //offset -1339 + 107/2
	  
	  	G4RotationMatrix *polyPanel23_localRot = new G4RotationMatrix();
	  	G4AffineTransform *polyPanel23_affine = new G4AffineTransform(polyPanel23_localRot, *polyPanel23_localPos);
	  	*polyPanel23_affine*=*assemAffine;
	  	G4ThreeVector *polyPanel23_globalPos = new G4ThreeVector(polyPanel23_affine->NetTranslation());
	  	G4RotationMatrix *polyPanel23_globalRot= new G4RotationMatrix(polyPanel23_affine->NetRotation());
			if (i==1)  fPoly_Panel23_001->Place(polyPanel23_globalPos, polyPanel23_globalRot, motherLogical);
	  	else if (i==2)  fPoly_Panel23_002->Place(polyPanel23_globalPos, polyPanel23_globalRot, motherLogical);
		  else if (i==3)  fPoly_Panel23_003->Place(polyPanel23_globalPos, polyPanel23_globalRot, motherLogical);
		  delete polyPanel23_localPos;
		  delete polyPanel23_localRot;
		  delete polyPanel23_affine;
		}
  }





  delete assemAffine;


} 
