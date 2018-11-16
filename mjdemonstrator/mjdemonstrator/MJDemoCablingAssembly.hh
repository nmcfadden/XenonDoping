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
 * CLASS DECLARATION:  MJDemoCablingAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The Cabling assembly
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
 * FIRST SUBMISSION: Oct 21, 2010
 * 
 * REVISION:
 * 
 * 1-14-2011, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOCABLINGASSEMBLY_HH
#define _MJDEMOCABLINGASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoHVConnector;
class MJDemoSigCableConnAssembly;
class MJDemoHVCableAssembly;
class MJDemoCableAssembly;


using namespace std;
//---------------------------------------------------------------------------//

class MJDemoCablingAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoCablingAssembly(G4String, G4String);
  MJDemoCablingAssembly(const MJDemoCablingAssembly &);
  ~MJDemoCablingAssembly();

  MJDemoHVConnector* GetHVConnector1() {return fHVConnector1Ptr;}
  MJDemoHVConnector* GetHVConnector2() {return fHVConnector2Ptr;}
  MJDemoHVConnector* GetHVConnector3() {return fHVConnector3Ptr;}
  MJDemoHVConnector* GetHVConnector4() {return fHVConnector4Ptr;}
  MJDemoHVConnector* GetHVConnector5() {return fHVConnector5Ptr;}
  MJDemoHVConnector* GetHVConnector6() {return fHVConnector6Ptr;}
  MJDemoHVConnector* GetHVConnector7() {return fHVConnector7Ptr;}
  MJDemoSigCableConnAssembly* GetSigConn1()    {return fSigCableConn1Ptr;}
  MJDemoSigCableConnAssembly* GetSigConn2()    {return fSigCableConn2Ptr;}
  MJDemoSigCableConnAssembly* GetSigConn3()    {return fSigCableConn3Ptr;}
  MJDemoSigCableConnAssembly* GetSigConn4()    {return fSigCableConn4Ptr;}
  MJDemoSigCableConnAssembly* GetSigConn5()    {return fSigCableConn5Ptr;}
  MJDemoSigCableConnAssembly* GetSigConn6()    {return fSigCableConn6Ptr;}
  MJDemoSigCableConnAssembly* GetSigConn7()    {return fSigCableConn7Ptr;}
  MJDemoHVCableAssembly* GetHVCable11()		{return fHVCable11Ptr;}
  MJDemoHVCableAssembly* GetHVCable12()		{return fHVCable12Ptr;}
  MJDemoHVCableAssembly* GetHVCable13()		{return fHVCable13Ptr;}
  MJDemoHVCableAssembly* GetHVCable14()		{return fHVCable14Ptr;}
  MJDemoHVCableAssembly* GetHVCable15()		{return fHVCable15Ptr;}
  MJDemoHVCableAssembly* GetHVCable21()		{return fHVCable21Ptr;}
  MJDemoHVCableAssembly* GetHVCable22()		{return fHVCable22Ptr;}
  MJDemoHVCableAssembly* GetHVCable23()		{return fHVCable23Ptr;}
  MJDemoHVCableAssembly* GetHVCable24()		{return fHVCable24Ptr;}
  MJDemoHVCableAssembly* GetHVCable25()		{return fHVCable25Ptr;}
  MJDemoHVCableAssembly* GetHVCable31()		{return fHVCable31Ptr;}
  MJDemoHVCableAssembly* GetHVCable32()		{return fHVCable32Ptr;}
  MJDemoHVCableAssembly* GetHVCable33()		{return fHVCable33Ptr;}
  MJDemoHVCableAssembly* GetHVCable34()		{return fHVCable34Ptr;}
  MJDemoHVCableAssembly* GetHVCable35()		{return fHVCable35Ptr;}
  MJDemoHVCableAssembly* GetHVCable41()		{return fHVCable41Ptr;}
  MJDemoHVCableAssembly* GetHVCable42()		{return fHVCable42Ptr;}
  MJDemoHVCableAssembly* GetHVCable43()		{return fHVCable43Ptr;}
  MJDemoHVCableAssembly* GetHVCable44()		{return fHVCable44Ptr;}
  MJDemoHVCableAssembly* GetHVCable45()		{return fHVCable45Ptr;}
  MJDemoHVCableAssembly* GetHVCable51()		{return fHVCable51Ptr;}
  MJDemoHVCableAssembly* GetHVCable52()		{return fHVCable52Ptr;}
  MJDemoHVCableAssembly* GetHVCable53()		{return fHVCable53Ptr;}
  MJDemoHVCableAssembly* GetHVCable54()		{return fHVCable54Ptr;}
  MJDemoHVCableAssembly* GetHVCable55()		{return fHVCable55Ptr;}
  MJDemoHVCableAssembly* GetHVCable61()		{return fHVCable61Ptr;}
  MJDemoHVCableAssembly* GetHVCable62()		{return fHVCable62Ptr;}
  MJDemoHVCableAssembly* GetHVCable63()		{return fHVCable63Ptr;}
  MJDemoHVCableAssembly* GetHVCable64()		{return fHVCable64Ptr;}
  MJDemoHVCableAssembly* GetHVCable65()		{return fHVCable65Ptr;}
  MJDemoHVCableAssembly* GetHVCable71()		{return fHVCable71Ptr;}
  MJDemoHVCableAssembly* GetHVCable72()		{return fHVCable72Ptr;}
  MJDemoHVCableAssembly* GetHVCable73()		{return fHVCable73Ptr;}
  MJDemoHVCableAssembly* GetHVCable74()		{return fHVCable74Ptr;}
  MJDemoHVCableAssembly* GetHVCable75()		{return fHVCable75Ptr;}
  MJDemoCableAssembly* GetCable11()		{return fCable11Ptr;}
  MJDemoCableAssembly* GetCable12()		{return fCable12Ptr;}
  MJDemoCableAssembly* GetCable13()		{return fCable13Ptr;}
  MJDemoCableAssembly* GetCable14()		{return fCable14Ptr;}
  MJDemoCableAssembly* GetCable15()		{return fCable15Ptr;}
  MJDemoCableAssembly* GetCable21()		{return fCable21Ptr;}
  MJDemoCableAssembly* GetCable22()		{return fCable22Ptr;}
  MJDemoCableAssembly* GetCable23()		{return fCable23Ptr;}
  MJDemoCableAssembly* GetCable24()		{return fCable24Ptr;}
  MJDemoCableAssembly* GetCable25()		{return fCable25Ptr;}
  MJDemoCableAssembly* GetCable31()		{return fCable31Ptr;}
  MJDemoCableAssembly* GetCable32()		{return fCable32Ptr;}
  MJDemoCableAssembly* GetCable33()		{return fCable33Ptr;}
  MJDemoCableAssembly* GetCable34()		{return fCable34Ptr;}
  MJDemoCableAssembly* GetCable35()		{return fCable35Ptr;}
  MJDemoCableAssembly* GetCable41()		{return fCable41Ptr;}
  MJDemoCableAssembly* GetCable42()		{return fCable42Ptr;}
  MJDemoCableAssembly* GetCable43()		{return fCable43Ptr;}
  MJDemoCableAssembly* GetCable44()		{return fCable44Ptr;}
  MJDemoCableAssembly* GetCable45()		{return fCable45Ptr;}
  MJDemoCableAssembly* GetCable51()		{return fCable51Ptr;}
  MJDemoCableAssembly* GetCable52()		{return fCable52Ptr;}
  MJDemoCableAssembly* GetCable53()		{return fCable53Ptr;}
  MJDemoCableAssembly* GetCable54()		{return fCable54Ptr;}
  MJDemoCableAssembly* GetCable55()		{return fCable55Ptr;}
  MJDemoCableAssembly* GetCable61()		{return fCable61Ptr;}
  MJDemoCableAssembly* GetCable62()		{return fCable62Ptr;}
  MJDemoCableAssembly* GetCable63()		{return fCable63Ptr;}
  MJDemoCableAssembly* GetCable64()		{return fCable64Ptr;}
  MJDemoCableAssembly* GetCable65()		{return fCable65Ptr;}
  MJDemoCableAssembly* GetCable71()		{return fCable71Ptr;}
  MJDemoCableAssembly* GetCable72()		{return fCable72Ptr;}
  MJDemoCableAssembly* GetCable73()		{return fCable73Ptr;}
  MJDemoCableAssembly* GetCable74()		{return fCable74Ptr;}
  MJDemoCableAssembly* GetCable75()		{return fCable75Ptr;}

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemoHVConnector* 				fHVConnector1Ptr;
  MJDemoHVConnector* 				fHVConnector2Ptr;
  MJDemoHVConnector* 				fHVConnector3Ptr;
  MJDemoHVConnector* 				fHVConnector4Ptr;
  MJDemoHVConnector* 				fHVConnector5Ptr;
  MJDemoHVConnector* 				fHVConnector6Ptr;
  MJDemoHVConnector* 				fHVConnector7Ptr;
  MJDemoSigCableConnAssembly*		fSigCableConn1Ptr;
  MJDemoSigCableConnAssembly*		fSigCableConn2Ptr;
  MJDemoSigCableConnAssembly*		fSigCableConn3Ptr;
  MJDemoSigCableConnAssembly*		fSigCableConn4Ptr;
  MJDemoSigCableConnAssembly*		fSigCableConn5Ptr;
  MJDemoSigCableConnAssembly*		fSigCableConn6Ptr;
  MJDemoSigCableConnAssembly*		fSigCableConn7Ptr;
  MJDemoHVCableAssembly* 		fHVCable11Ptr;
  MJDemoHVCableAssembly* 		fHVCable12Ptr;
  MJDemoHVCableAssembly* 		fHVCable13Ptr;
  MJDemoHVCableAssembly* 		fHVCable14Ptr;
  MJDemoHVCableAssembly* 		fHVCable15Ptr;
  MJDemoHVCableAssembly* 		fHVCable21Ptr;
  MJDemoHVCableAssembly* 		fHVCable22Ptr;
  MJDemoHVCableAssembly* 		fHVCable23Ptr;
  MJDemoHVCableAssembly* 		fHVCable24Ptr;
  MJDemoHVCableAssembly* 		fHVCable25Ptr;
  MJDemoHVCableAssembly* 		fHVCable31Ptr;
  MJDemoHVCableAssembly* 		fHVCable32Ptr;
  MJDemoHVCableAssembly* 		fHVCable33Ptr;
  MJDemoHVCableAssembly* 		fHVCable34Ptr;
  MJDemoHVCableAssembly* 		fHVCable35Ptr;
  MJDemoHVCableAssembly* 		fHVCable41Ptr;
  MJDemoHVCableAssembly* 		fHVCable42Ptr;
  MJDemoHVCableAssembly* 		fHVCable43Ptr;
  MJDemoHVCableAssembly* 		fHVCable44Ptr;
  MJDemoHVCableAssembly* 		fHVCable45Ptr;
  MJDemoHVCableAssembly* 		fHVCable51Ptr;
  MJDemoHVCableAssembly* 		fHVCable52Ptr;
  MJDemoHVCableAssembly* 		fHVCable53Ptr;
  MJDemoHVCableAssembly* 		fHVCable54Ptr;
  MJDemoHVCableAssembly* 		fHVCable55Ptr;
  MJDemoHVCableAssembly* 		fHVCable61Ptr;
  MJDemoHVCableAssembly* 		fHVCable62Ptr;
  MJDemoHVCableAssembly* 		fHVCable63Ptr;
  MJDemoHVCableAssembly* 		fHVCable64Ptr;
  MJDemoHVCableAssembly* 		fHVCable65Ptr;
  MJDemoHVCableAssembly* 		fHVCable71Ptr;
  MJDemoHVCableAssembly* 		fHVCable72Ptr;
  MJDemoHVCableAssembly* 		fHVCable73Ptr;
  MJDemoHVCableAssembly* 		fHVCable74Ptr;
  MJDemoHVCableAssembly* 		fHVCable75Ptr;
  MJDemoCableAssembly* 		fCable11Ptr;
  MJDemoCableAssembly* 		fCable12Ptr;
  MJDemoCableAssembly* 		fCable13Ptr;
  MJDemoCableAssembly* 		fCable14Ptr;
  MJDemoCableAssembly* 		fCable15Ptr;
  MJDemoCableAssembly* 		fCable21Ptr;
  MJDemoCableAssembly* 		fCable22Ptr;
  MJDemoCableAssembly* 		fCable23Ptr;
  MJDemoCableAssembly* 		fCable24Ptr;
  MJDemoCableAssembly* 		fCable25Ptr;
  MJDemoCableAssembly* 		fCable31Ptr;
  MJDemoCableAssembly* 		fCable32Ptr;
  MJDemoCableAssembly* 		fCable33Ptr;
  MJDemoCableAssembly* 		fCable34Ptr;
  MJDemoCableAssembly* 		fCable35Ptr;
  MJDemoCableAssembly* 		fCable41Ptr;
  MJDemoCableAssembly* 		fCable42Ptr;
  MJDemoCableAssembly* 		fCable43Ptr;
  MJDemoCableAssembly* 		fCable44Ptr;
  MJDemoCableAssembly* 		fCable45Ptr;
  MJDemoCableAssembly* 		fCable51Ptr;
  MJDemoCableAssembly* 		fCable52Ptr;
  MJDemoCableAssembly* 		fCable53Ptr;
  MJDemoCableAssembly* 		fCable54Ptr;
  MJDemoCableAssembly* 		fCable55Ptr;
  MJDemoCableAssembly* 		fCable61Ptr;
  MJDemoCableAssembly* 		fCable62Ptr;
  MJDemoCableAssembly* 		fCable63Ptr;
  MJDemoCableAssembly* 		fCable64Ptr;
  MJDemoCableAssembly* 		fCable65Ptr;
  MJDemoCableAssembly* 		fCable71Ptr;
  MJDemoCableAssembly* 		fCable72Ptr;
  MJDemoCableAssembly* 		fCable73Ptr;
  MJDemoCableAssembly* 		fCable74Ptr;
  MJDemoCableAssembly* 		fCable75Ptr;
  
};
//
#endif
