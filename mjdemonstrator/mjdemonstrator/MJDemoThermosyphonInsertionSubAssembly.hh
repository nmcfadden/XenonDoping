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
 * CLASS DECLARATION:  MJDemoThermosyphonInsertionSubAssembly.hh
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

#ifndef _MJDEMOTHERMOSYPHONINSERTIONSUBASSEMBLY_HH
#define _MJDEMOTHERMOSYPHONINSERTIONSUBASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoThermosyphonWeldmentAssembly;
class MJDemoThermosyphonSupportThickAssembly;
class MJDemoThermosyphonSupportThinAssembly;
class MJDemoThermosyphonCableThermalShuntAssembly;
class MJDemoClampPlateSignalConnector;
class MJDemoBulkheadCableClamp;
class MJDemo832Screw;
class MJDemoConnectorandClampSubAssembly;
class MJDemoTSShieldBlock;
class MJDemoCubeShieldPlates;


using namespace std;
//---------------------------------------------------------------------------//

class MJDemoThermosyphonInsertionSubAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoThermosyphonInsertionSubAssembly(G4String, G4String);
  MJDemoThermosyphonInsertionSubAssembly(const MJDemoThermosyphonInsertionSubAssembly &);
  ~MJDemoThermosyphonInsertionSubAssembly();

  MJDemoThermosyphonWeldmentAssembly* GetWeldment() {return fWeldmentPtr;}
  MJDemoThermosyphonSupportThickAssembly* GetThickSupport() {return fThickSupportPtr;}
  MJDemoThermosyphonSupportThinAssembly* GetThinSupport() {return fThinSupportPtr;}
  MJDemoThermosyphonCableThermalShuntAssembly* GetShunt01() {return fShunt01Ptr;}
  MJDemoThermosyphonCableThermalShuntAssembly* GetShunt02() {return fShunt02Ptr;}
  MJDemoClampPlateSignalConnector* GetPlate() {return fPlatePtr;}
  MJDemoBulkheadCableClamp* GetCableClamp01() {return fCableClamp01Ptr;}
  MJDemoBulkheadCableClamp* GetCableClamp02() {return fCableClamp02Ptr;}
  MJDemo832Screw* GetScrew01() {return fScrew01Ptr;}
  MJDemo832Screw* GetScrew02() {return fScrew02Ptr;}
  MJDemo832Screw* GetScrew03() {return fScrew03Ptr;}
  MJDemo832Screw* GetScrew04() {return fScrew04Ptr;}
  MJDemoConnectorandClampSubAssembly* GetConnectors01() {return fConnectors01Ptr;}
  MJDemoConnectorandClampSubAssembly* GetConnectors02() {return fConnectors02Ptr;}
  MJDemoConnectorandClampSubAssembly* GetConnectors03() {return fConnectors03Ptr;}
  MJDemoConnectorandClampSubAssembly* GetConnectors04() {return fConnectors04Ptr;}
  MJDemoConnectorandClampSubAssembly* GetConnectors05() {return fConnectors05Ptr;}
  MJDemoConnectorandClampSubAssembly* GetConnectors06() {return fConnectors06Ptr;}
  MJDemoConnectorandClampSubAssembly* GetConnectors07() {return fConnectors07Ptr;}
  MJDemoConnectorandClampSubAssembly* GetConnectors08() {return fConnectors08Ptr;}
  MJDemoTSShieldBlock* GetShieldBlock1() {return fShieldBlock01Ptr;} 
  MJDemoTSShieldBlock* GetShieldBlock2() {return fShieldBlock02Ptr;}
  MJDemoTSShieldBlock* GetShieldBlock3() {return fShieldBlock03Ptr;}
  MJDemoTSShieldBlock* GetShieldBlock4() {return fShieldBlock04Ptr;}  
  MJDemoCubeShieldPlates* GetCubeShieldPlates() {return fCubeShieldPlatesPtr;}
     
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);  
  
private:
  MJDemoThermosyphonWeldmentAssembly* fWeldmentPtr;
  MJDemoThermosyphonSupportThickAssembly* fThickSupportPtr;
  MJDemoThermosyphonSupportThinAssembly* fThinSupportPtr;
  MJDemoThermosyphonCableThermalShuntAssembly* fShunt01Ptr;
  MJDemoThermosyphonCableThermalShuntAssembly* fShunt02Ptr;
  MJDemoClampPlateSignalConnector* fPlatePtr;
  MJDemoBulkheadCableClamp* fCableClamp01Ptr;
  MJDemoBulkheadCableClamp* fCableClamp02Ptr;
  MJDemo832Screw* fScrew01Ptr;
  MJDemo832Screw* fScrew02Ptr;
  MJDemo832Screw* fScrew03Ptr;
  MJDemo832Screw* fScrew04Ptr;
  MJDemoConnectorandClampSubAssembly* fConnectors01Ptr;
  MJDemoConnectorandClampSubAssembly* fConnectors02Ptr;
  MJDemoConnectorandClampSubAssembly* fConnectors03Ptr;
  MJDemoConnectorandClampSubAssembly* fConnectors04Ptr;
  MJDemoConnectorandClampSubAssembly* fConnectors05Ptr;
  MJDemoConnectorandClampSubAssembly* fConnectors06Ptr;
  MJDemoConnectorandClampSubAssembly* fConnectors07Ptr;
  MJDemoConnectorandClampSubAssembly* fConnectors08Ptr;
  MJDemoTSShieldBlock* fShieldBlock01Ptr;
  MJDemoTSShieldBlock* fShieldBlock02Ptr;
  MJDemoTSShieldBlock* fShieldBlock03Ptr;
  MJDemoTSShieldBlock* fShieldBlock04Ptr;
  MJDemoCubeShieldPlates* fCubeShieldPlatesPtr;
  
};
//
#endif
