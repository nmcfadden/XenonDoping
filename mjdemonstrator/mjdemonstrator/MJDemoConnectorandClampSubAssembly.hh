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
 * CLASS DECLARATION:  MJDemoConnectorandClampSubAssembly.hh
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

#ifndef _MJDEMOCONNECTORANDCLAMPSUBASSEMBLY_HH
#define _MJDEMOCONNECTORANDCLAMPSUBASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoBulkheadTopClampLowMount;
class MJDemo832Screw;
class MJDemoSigConnPairAssembly;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoConnectorandClampSubAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoConnectorandClampSubAssembly(G4String, G4String);
  MJDemoConnectorandClampSubAssembly(const MJDemoConnectorandClampSubAssembly &);
  ~MJDemoConnectorandClampSubAssembly();

  MJDemoBulkheadTopClampLowMount* GetClamp() {return fClampPtr;}
  MJDemo832Screw* GetScrew01() {return fScrew01Ptr;}
  MJDemo832Screw* GetScrew02() {return fScrew02Ptr;}
  MJDemoSigConnPairAssembly* GetConnector01() {return fConnector01Ptr;}
  MJDemoSigConnPairAssembly* GetConnector02() {return fConnector02Ptr;}
  MJDemoSigConnPairAssembly* GetConnector03() {return fConnector03Ptr;}
  MJDemoSigConnPairAssembly* GetConnector04() {return fConnector04Ptr;}
  MJDemoSigConnPairAssembly* GetConnector05() {return fConnector05Ptr;}
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);  
  
private:

  MJDemoBulkheadTopClampLowMount* fClampPtr;
  MJDemo832Screw* fScrew01Ptr;
  MJDemo832Screw* fScrew02Ptr;
  MJDemoSigConnPairAssembly* fConnector01Ptr;
  MJDemoSigConnPairAssembly* fConnector02Ptr;
  MJDemoSigConnPairAssembly* fConnector03Ptr;
  MJDemoSigConnPairAssembly* fConnector04Ptr;
  MJDemoSigConnPairAssembly* fConnector05Ptr;
    
};
//
#endif
