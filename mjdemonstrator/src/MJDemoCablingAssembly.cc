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
// CLASS IMPLEMENTATION:  MJDemoCablingAssembly.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *Assembly origin: Center of coldplate top
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jan 14, 2011
 * 
 * REVISION:
 * 
 * 01-14-2011, Created, M. Green
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
#include "mjdemonstrator/MJDemoCablingAssembly.hh"
#include "mjdemonstrator/MJDemoHVConnector.hh"
#include "mjdemonstrator/MJDemoSigCableConnAssembly.hh"
#include "mjdemonstrator/MJDemoHVCableAssembly.hh"
#include "mjdemonstrator/MJDemoCableAssembly.hh"


using namespace CLHEP;


//---------------------------------------------------------------------------//

MJDemoCablingAssembly::MJDemoCablingAssembly(G4String fName, G4String serialNumber) :
  MJVDemoAssembly(fName, serialNumber, "Cabling")
{
  fHVConnector1Ptr = new MJDemoHVConnector(fName+"_HVConnector_001", "867-5309");  
  fHVConnector2Ptr = new MJDemoHVConnector(fName+"_HVConnector_002", "867-5309");  
  fHVConnector3Ptr = new MJDemoHVConnector(fName+"_HVConnector_003", "867-5309");  
  fHVConnector4Ptr = new MJDemoHVConnector(fName+"_HVConnector_004", "867-5309");  
  fHVConnector5Ptr = new MJDemoHVConnector(fName+"_HVConnector_005", "867-5309");  
  fHVConnector6Ptr = new MJDemoHVConnector(fName+"_HVConnector_006", "867-5309");  
  fHVConnector7Ptr = new MJDemoHVConnector(fName+"_HVConnector_007", "867-5309");  
  fSigCableConn1Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_001", "867-5309");
  fSigCableConn2Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_002", "867-5309");
  fSigCableConn3Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_003", "867-5309");
  fSigCableConn4Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_004", "867-5309");
  fSigCableConn5Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_005", "867-5309");
  fSigCableConn6Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_006", "867-5309");
  fSigCableConn7Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_007", "867-5309");
  fHVCable11Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_011", "867-5309", 1*25.4*mm);
  fHVCable12Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_012", "867-5309", 1*25.4*mm);
  fHVCable13Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_013", "867-5309", 1*25.4*mm);
  fHVCable14Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_014", "867-5309", 1*25.4*mm);
  fHVCable15Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_015", "867-5309", 1*25.4*mm);
  fHVCable21Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_021", "867-5309", 1*25.4*mm);
  fHVCable22Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_022", "867-5309", 1*25.4*mm);
  fHVCable23Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_023", "867-5309", 1*25.4*mm);
  fHVCable24Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_024", "867-5309", 1*25.4*mm);
  fHVCable25Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_025", "867-5309", 1*25.4*mm);
  fHVCable31Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_031", "867-5309", 1*25.4*mm);
  fHVCable32Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_032", "867-5309", 1*25.4*mm);
  fHVCable33Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_033", "867-5309", 1*25.4*mm);
  fHVCable34Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_034", "867-5309", 1*25.4*mm);
  fHVCable35Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_035", "867-5309", 1*25.4*mm);
  fHVCable41Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_041", "867-5309", 1*25.4*mm);
  fHVCable42Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_042", "867-5309", 1*25.4*mm);
  fHVCable43Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_043", "867-5309", 1*25.4*mm);
  fHVCable44Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_044", "867-5309", 1*25.4*mm);
  fHVCable45Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_045", "867-5309", 1*25.4*mm);
  fHVCable51Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_051", "867-5309", 1*25.4*mm);
  fHVCable52Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_052", "867-5309", 1*25.4*mm);
  fHVCable53Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_053", "867-5309", 1*25.4*mm);
  fHVCable54Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_054", "867-5309", 1*25.4*mm);
  fHVCable55Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_055", "867-5309", 1*25.4*mm);
  fHVCable61Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_061", "867-5309", 1*25.4*mm);
  fHVCable62Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_062", "867-5309", 1*25.4*mm);
  fHVCable63Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_063", "867-5309", 1*25.4*mm);
  fHVCable64Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_064", "867-5309", 1*25.4*mm);
  fHVCable65Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_065", "867-5309", 1*25.4*mm);
  fHVCable71Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_071", "867-5309", 1*25.4*mm);
  fHVCable72Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_072", "867-5309", 1*25.4*mm);
  fHVCable73Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_073", "867-5309", 1*25.4*mm);
  fHVCable74Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_074", "867-5309", 1*25.4*mm);
  fHVCable75Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_075", "867-5309", 1*25.4*mm);
  fCable11Ptr = new MJDemoCableAssembly(fName+"_Cable_011", "867-5309", 6.15*25.4*mm);
  fCable12Ptr = new MJDemoCableAssembly(fName+"_Cable_012", "867-5309", 6.15*25.4*mm);
  fCable13Ptr = new MJDemoCableAssembly(fName+"_Cable_013", "867-5309", 6.15*25.4*mm);
  fCable14Ptr = new MJDemoCableAssembly(fName+"_Cable_014", "867-5309", 6.15*25.4*mm);
  fCable15Ptr = new MJDemoCableAssembly(fName+"_Cable_015", "867-5309", 6.15*25.4*mm);
  fCable21Ptr = new MJDemoCableAssembly(fName+"_Cable_021", "867-5309", 2.95*25.4*mm);
  fCable22Ptr = new MJDemoCableAssembly(fName+"_Cable_022", "867-5309", 2.95*25.4*mm);
  fCable23Ptr = new MJDemoCableAssembly(fName+"_Cable_023", "867-5309", 2.95*25.4*mm);
  fCable24Ptr = new MJDemoCableAssembly(fName+"_Cable_024", "867-5309", 2.95*25.4*mm);
  fCable25Ptr = new MJDemoCableAssembly(fName+"_Cable_025", "867-5309", 2.95*25.4*mm);
  fCable31Ptr = new MJDemoCableAssembly(fName+"_Cable_031", "867-5309", 6.50*25.4*mm);
  fCable32Ptr = new MJDemoCableAssembly(fName+"_Cable_032", "867-5309", 6.50*25.4*mm);
  fCable33Ptr = new MJDemoCableAssembly(fName+"_Cable_033", "867-5309", 6.50*25.4*mm);
  fCable34Ptr = new MJDemoCableAssembly(fName+"_Cable_034", "867-5309", 6.50*25.4*mm);
  fCable35Ptr = new MJDemoCableAssembly(fName+"_Cable_035", "867-5309", 6.50*25.4*mm);
  fCable41Ptr = new MJDemoCableAssembly(fName+"_Cable_041", "867-5309", 9.40*25.4*mm);
  fCable42Ptr = new MJDemoCableAssembly(fName+"_Cable_042", "867-5309", 9.40*25.4*mm);
  fCable43Ptr = new MJDemoCableAssembly(fName+"_Cable_043", "867-5309", 9.40*25.4*mm);
  fCable44Ptr = new MJDemoCableAssembly(fName+"_Cable_044", "867-5309", 9.40*25.4*mm);
  fCable45Ptr = new MJDemoCableAssembly(fName+"_Cable_045", "867-5309", 9.40*25.4*mm);
  fCable51Ptr = new MJDemoCableAssembly(fName+"_Cable_051", "867-5309", 9.40*25.4*mm);
  fCable52Ptr = new MJDemoCableAssembly(fName+"_Cable_052", "867-5309", 9.40*25.4*mm);
  fCable53Ptr = new MJDemoCableAssembly(fName+"_Cable_053", "867-5309", 9.40*25.4*mm);
  fCable54Ptr = new MJDemoCableAssembly(fName+"_Cable_054", "867-5309", 9.40*25.4*mm);
  fCable55Ptr = new MJDemoCableAssembly(fName+"_Cable_055", "867-5309", 9.40*25.4*mm);
  fCable61Ptr = new MJDemoCableAssembly(fName+"_Cable_061", "867-5309", 6.50*25.4*mm);
  fCable62Ptr = new MJDemoCableAssembly(fName+"_Cable_062", "867-5309", 6.50*25.4*mm);
  fCable63Ptr = new MJDemoCableAssembly(fName+"_Cable_063", "867-5309", 6.50*25.4*mm);
  fCable64Ptr = new MJDemoCableAssembly(fName+"_Cable_064", "867-5309", 6.50*25.4*mm);
  fCable65Ptr = new MJDemoCableAssembly(fName+"_Cable_065", "867-5309", 6.50*25.4*mm);
  fCable71Ptr = new MJDemoCableAssembly(fName+"_Cable_071", "867-5309", 2.95*25.4*mm);
  fCable72Ptr = new MJDemoCableAssembly(fName+"_Cable_072", "867-5309", 2.95*25.4*mm);
  fCable73Ptr = new MJDemoCableAssembly(fName+"_Cable_073", "867-5309", 2.95*25.4*mm);
  fCable74Ptr = new MJDemoCableAssembly(fName+"_Cable_074", "867-5309", 2.95*25.4*mm);
  fCable75Ptr = new MJDemoCableAssembly(fName+"_Cable_075", "867-5309", 2.95*25.4*mm);
}

MJDemoCablingAssembly::MJDemoCablingAssembly(const MJDemoCablingAssembly & rhs) : 
  MJVDemoAssembly(rhs)
{
  fHVConnector1Ptr = new MJDemoHVConnector(fName+"_HVConnector_001", "867-5309");  
  fHVConnector2Ptr = new MJDemoHVConnector(fName+"_HVConnector_002", "867-5309");  
  fHVConnector3Ptr = new MJDemoHVConnector(fName+"_HVConnector_003", "867-5309");  
  fHVConnector4Ptr = new MJDemoHVConnector(fName+"_HVConnector_004", "867-5309");  
  fHVConnector5Ptr = new MJDemoHVConnector(fName+"_HVConnector_005", "867-5309");  
  fHVConnector6Ptr = new MJDemoHVConnector(fName+"_HVConnector_006", "867-5309");  
  fHVConnector7Ptr = new MJDemoHVConnector(fName+"_HVConnector_007", "867-5309");  
  fSigCableConn1Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_001", "867-5309");
  fSigCableConn2Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_002", "867-5309");
  fSigCableConn3Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_003", "867-5309");
  fSigCableConn4Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_004", "867-5309");
  fSigCableConn5Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_005", "867-5309");
  fSigCableConn6Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_006", "867-5309");
  fSigCableConn7Ptr = new MJDemoSigCableConnAssembly(fName+"_SigCableConnector_007", "867-5309");
  fHVCable11Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_011", "867-5309", 1*25.4*mm);
  fHVCable12Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_012", "867-5309", 1*25.4*mm);
  fHVCable13Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_013", "867-5309", 1*25.4*mm);
  fHVCable14Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_014", "867-5309", 1*25.4*mm);
  fHVCable15Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_015", "867-5309", 1*25.4*mm);
  fHVCable21Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_021", "867-5309", 1*25.4*mm);
  fHVCable22Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_022", "867-5309", 1*25.4*mm);
  fHVCable23Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_023", "867-5309", 1*25.4*mm);
  fHVCable24Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_024", "867-5309", 1*25.4*mm);
  fHVCable25Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_025", "867-5309", 1*25.4*mm);
  fHVCable31Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_031", "867-5309", 1*25.4*mm);
  fHVCable32Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_032", "867-5309", 1*25.4*mm);
  fHVCable33Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_033", "867-5309", 1*25.4*mm);
  fHVCable34Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_034", "867-5309", 1*25.4*mm);
  fHVCable35Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_035", "867-5309", 1*25.4*mm);
  fHVCable41Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_041", "867-5309", 1*25.4*mm);
  fHVCable42Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_042", "867-5309", 1*25.4*mm);
  fHVCable43Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_043", "867-5309", 1*25.4*mm);
  fHVCable44Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_044", "867-5309", 1*25.4*mm);
  fHVCable45Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_045", "867-5309", 1*25.4*mm);
  fHVCable51Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_051", "867-5309", 1*25.4*mm);
  fHVCable52Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_052", "867-5309", 1*25.4*mm);
  fHVCable53Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_053", "867-5309", 1*25.4*mm);
  fHVCable54Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_054", "867-5309", 1*25.4*mm);
  fHVCable55Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_055", "867-5309", 1*25.4*mm);
  fHVCable61Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_061", "867-5309", 1*25.4*mm);
  fHVCable62Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_062", "867-5309", 1*25.4*mm);
  fHVCable63Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_063", "867-5309", 1*25.4*mm);
  fHVCable64Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_064", "867-5309", 1*25.4*mm);
  fHVCable65Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_065", "867-5309", 1*25.4*mm);
  fHVCable71Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_071", "867-5309", 1*25.4*mm);
  fHVCable72Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_072", "867-5309", 1*25.4*mm);
  fHVCable73Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_073", "867-5309", 1*25.4*mm);
  fHVCable74Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_074", "867-5309", 1*25.4*mm);
  fHVCable75Ptr = new MJDemoHVCableAssembly(fName+"_HVCable_075", "867-5309", 1*25.4*mm);
  fCable11Ptr = new MJDemoCableAssembly(fName+"_Cable_011", "867-5309", 6.15*25.4*mm);
  fCable12Ptr = new MJDemoCableAssembly(fName+"_Cable_012", "867-5309", 6.15*25.4*mm);
  fCable13Ptr = new MJDemoCableAssembly(fName+"_Cable_013", "867-5309", 6.15*25.4*mm);
  fCable14Ptr = new MJDemoCableAssembly(fName+"_Cable_014", "867-5309", 6.15*25.4*mm);
  fCable15Ptr = new MJDemoCableAssembly(fName+"_Cable_015", "867-5309", 6.15*25.4*mm);
  fCable21Ptr = new MJDemoCableAssembly(fName+"_Cable_021", "867-5309", 2.95*25.4*mm);
  fCable22Ptr = new MJDemoCableAssembly(fName+"_Cable_022", "867-5309", 2.95*25.4*mm);
  fCable23Ptr = new MJDemoCableAssembly(fName+"_Cable_023", "867-5309", 2.95*25.4*mm);
  fCable24Ptr = new MJDemoCableAssembly(fName+"_Cable_024", "867-5309", 2.95*25.4*mm);
  fCable25Ptr = new MJDemoCableAssembly(fName+"_Cable_025", "867-5309", 2.95*25.4*mm);
  fCable31Ptr = new MJDemoCableAssembly(fName+"_Cable_031", "867-5309", 6.50*25.4*mm);
  fCable32Ptr = new MJDemoCableAssembly(fName+"_Cable_032", "867-5309", 6.50*25.4*mm);
  fCable33Ptr = new MJDemoCableAssembly(fName+"_Cable_033", "867-5309", 6.50*25.4*mm);
  fCable34Ptr = new MJDemoCableAssembly(fName+"_Cable_034", "867-5309", 6.50*25.4*mm);
  fCable35Ptr = new MJDemoCableAssembly(fName+"_Cable_035", "867-5309", 6.50*25.4*mm);
  fCable41Ptr = new MJDemoCableAssembly(fName+"_Cable_041", "867-5309", 9.40*25.4*mm);
  fCable42Ptr = new MJDemoCableAssembly(fName+"_Cable_042", "867-5309", 9.40*25.4*mm);
  fCable43Ptr = new MJDemoCableAssembly(fName+"_Cable_043", "867-5309", 9.40*25.4*mm);
  fCable44Ptr = new MJDemoCableAssembly(fName+"_Cable_044", "867-5309", 9.40*25.4*mm);
  fCable45Ptr = new MJDemoCableAssembly(fName+"_Cable_045", "867-5309", 9.40*25.4*mm);
  fCable51Ptr = new MJDemoCableAssembly(fName+"_Cable_051", "867-5309", 9.40*25.4*mm);
  fCable52Ptr = new MJDemoCableAssembly(fName+"_Cable_052", "867-5309", 9.40*25.4*mm);
  fCable53Ptr = new MJDemoCableAssembly(fName+"_Cable_053", "867-5309", 9.40*25.4*mm);
  fCable54Ptr = new MJDemoCableAssembly(fName+"_Cable_054", "867-5309", 9.40*25.4*mm);
  fCable55Ptr = new MJDemoCableAssembly(fName+"_Cable_055", "867-5309", 9.40*25.4*mm);
  fCable61Ptr = new MJDemoCableAssembly(fName+"_Cable_061", "867-5309", 6.50*25.4*mm);
  fCable62Ptr = new MJDemoCableAssembly(fName+"_Cable_062", "867-5309", 6.50*25.4*mm);
  fCable63Ptr = new MJDemoCableAssembly(fName+"_Cable_063", "867-5309", 6.50*25.4*mm);
  fCable64Ptr = new MJDemoCableAssembly(fName+"_Cable_064", "867-5309", 6.50*25.4*mm);
  fCable65Ptr = new MJDemoCableAssembly(fName+"_Cable_065", "867-5309", 6.50*25.4*mm);
  fCable71Ptr = new MJDemoCableAssembly(fName+"_Cable_071", "867-5309", 2.95*25.4*mm);
  fCable72Ptr = new MJDemoCableAssembly(fName+"_Cable_072", "867-5309", 2.95*25.4*mm);
  fCable73Ptr = new MJDemoCableAssembly(fName+"_Cable_073", "867-5309", 2.95*25.4*mm);
  fCable74Ptr = new MJDemoCableAssembly(fName+"_Cable_074", "867-5309", 2.95*25.4*mm);
  fCable75Ptr = new MJDemoCableAssembly(fName+"_Cable_075", "867-5309", 2.95*25.4*mm);
}

MJDemoCablingAssembly::~MJDemoCablingAssembly()
{
  delete fHVConnector1Ptr;
  delete fHVConnector2Ptr;
  delete fHVConnector3Ptr;
  delete fHVConnector4Ptr;
  delete fHVConnector5Ptr;
  delete fHVConnector6Ptr;
  delete fHVConnector7Ptr;  
  delete fSigCableConn1Ptr;
  delete fSigCableConn2Ptr;
  delete fSigCableConn3Ptr;
  delete fSigCableConn4Ptr;
  delete fSigCableConn5Ptr;
  delete fSigCableConn6Ptr;
  delete fSigCableConn7Ptr;  
  delete fHVCable11Ptr;
  delete fHVCable12Ptr;
  delete fHVCable13Ptr;
  delete fHVCable14Ptr;
  delete fHVCable15Ptr;
  delete fHVCable21Ptr;
  delete fHVCable22Ptr;
  delete fHVCable23Ptr;
  delete fHVCable24Ptr;
  delete fHVCable25Ptr;
  delete fHVCable31Ptr;
  delete fHVCable32Ptr;
  delete fHVCable33Ptr;
  delete fHVCable34Ptr;
  delete fHVCable35Ptr;
  delete fHVCable41Ptr;
  delete fHVCable42Ptr;
  delete fHVCable43Ptr;
  delete fHVCable44Ptr;
  delete fHVCable45Ptr;
  delete fHVCable51Ptr;
  delete fHVCable52Ptr;
  delete fHVCable53Ptr;
  delete fHVCable54Ptr;
  delete fHVCable55Ptr;
  delete fHVCable61Ptr;
  delete fHVCable62Ptr;
  delete fHVCable63Ptr;
  delete fHVCable64Ptr;
  delete fHVCable65Ptr;
  delete fHVCable71Ptr;
  delete fHVCable72Ptr;
  delete fHVCable73Ptr;
  delete fHVCable74Ptr;
  delete fHVCable75Ptr;
  delete fCable11Ptr;
  delete fCable12Ptr;
  delete fCable13Ptr;
  delete fCable14Ptr;
  delete fCable15Ptr;
  delete fCable21Ptr;
  delete fCable22Ptr;
  delete fCable23Ptr;
  delete fCable24Ptr;
  delete fCable25Ptr;
  delete fCable31Ptr;
  delete fCable32Ptr;
  delete fCable33Ptr;
  delete fCable34Ptr;
  delete fCable35Ptr;
  delete fCable41Ptr;
  delete fCable42Ptr;
  delete fCable43Ptr;
  delete fCable44Ptr;
  delete fCable45Ptr;
  delete fCable51Ptr;
  delete fCable52Ptr;
  delete fCable53Ptr;
  delete fCable54Ptr;
  delete fCable55Ptr;
  delete fCable61Ptr;
  delete fCable62Ptr;
  delete fCable63Ptr;
  delete fCable64Ptr;
  delete fCable65Ptr;
  delete fCable71Ptr;
  delete fCable72Ptr;
  delete fCable73Ptr;
  delete fCable74Ptr;
  delete fCable75Ptr;

}

void MJDemoCablingAssembly::Place(G4ThreeVector* assemPosition, 
                G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical)
{
  G4AffineTransform *assemAffine = new G4AffineTransform(assemRotation,*assemPosition);

  G4ThreeVector *HVConnector1localPos = new G4ThreeVector(-0.07*25.4*mm, -1.21*25.4*mm, 0);
  G4RotationMatrix *HVConnector1localRot = new G4RotationMatrix();
  HVConnector1localRot->rotateZ(-20*deg);
  G4AffineTransform *HVConnector1affine = new G4AffineTransform(HVConnector1localRot,*HVConnector1localPos);
  *HVConnector1affine*=*assemAffine;  
  G4ThreeVector *HVConnector1globalPos = new G4ThreeVector(HVConnector1affine->NetTranslation());
  G4RotationMatrix *HVConnector1globalRot= new G4RotationMatrix(HVConnector1affine->NetRotation());
  fHVConnector1Ptr->Place(HVConnector1globalPos, HVConnector1globalRot, motherLogical);
  delete HVConnector1localPos;
  delete HVConnector1localRot;
  delete HVConnector1affine;

  G4ThreeVector *HVConnector2localPos = new G4ThreeVector(4.04*25.4*mm, 2.71*25.4*mm, 0);
  G4RotationMatrix *HVConnector2localRot = new G4RotationMatrix();
  HVConnector2localRot->rotateZ(-140*deg);
  G4AffineTransform *HVConnector2affine = new G4AffineTransform(HVConnector2localRot,*HVConnector2localPos);
  *HVConnector2affine*=*assemAffine;  
  G4ThreeVector *HVConnector2globalPos = new G4ThreeVector(HVConnector2affine->NetTranslation());
  G4RotationMatrix *HVConnector2globalRot= new G4RotationMatrix(HVConnector2affine->NetRotation());
  fHVConnector2Ptr->Place(HVConnector2globalPos, HVConnector2globalRot, motherLogical);
  delete HVConnector2localPos;
  delete HVConnector2localRot;
  delete HVConnector2affine;
  
  G4ThreeVector *HVConnector3localPos = new G4ThreeVector(0.41*25.4*mm, 2.89*25.4*mm, 0);
  G4RotationMatrix *HVConnector3localRot = new G4RotationMatrix();
  HVConnector3localRot->rotateZ(-20*deg);
  G4AffineTransform *HVConnector3affine = new G4AffineTransform(HVConnector3localRot,*HVConnector3localPos);
  *HVConnector3affine*=*assemAffine;  
  G4ThreeVector *HVConnector3globalPos = new G4ThreeVector(HVConnector3affine->NetTranslation());
  G4RotationMatrix *HVConnector3globalRot= new G4RotationMatrix(HVConnector3affine->NetRotation());
  fHVConnector3Ptr->Place(HVConnector3globalPos, HVConnector3globalRot, motherLogical);
  delete HVConnector3localPos;
  delete HVConnector3localRot;
  delete HVConnector3affine;
  
  G4ThreeVector *HVConnector4localPos = new G4ThreeVector(-2.85*25.4*mm, 0.94*25.4*mm, 0);
  G4RotationMatrix *HVConnector4localRot = new G4RotationMatrix();
  HVConnector4localRot->rotateZ(-20*deg);
  G4AffineTransform *HVConnector4affine = new G4AffineTransform(HVConnector4localRot,*HVConnector4localPos);
  *HVConnector4affine*=*assemAffine;  
  G4ThreeVector *HVConnector4globalPos = new G4ThreeVector(HVConnector4affine->NetTranslation());
  G4RotationMatrix *HVConnector4globalRot= new G4RotationMatrix(HVConnector4affine->NetRotation());
  fHVConnector4Ptr->Place(HVConnector4globalPos, HVConnector4globalRot, motherLogical);
  delete HVConnector4localPos;
  delete HVConnector4localRot;
  delete HVConnector4affine;
  
  G4ThreeVector *HVConnector5localPos = new G4ThreeVector(-2.85*25.4*mm, -2.86*25.4*mm, 0);
  G4RotationMatrix *HVConnector5localRot = new G4RotationMatrix();
  HVConnector5localRot->rotateZ(-20*deg);
  G4AffineTransform *HVConnector5affine = new G4AffineTransform(HVConnector5localRot,*HVConnector5localPos);
  *HVConnector5affine*=*assemAffine;  
  G4ThreeVector *HVConnector5globalPos = new G4ThreeVector(HVConnector5affine->NetTranslation());
  G4RotationMatrix *HVConnector5globalRot= new G4RotationMatrix(HVConnector5affine->NetRotation());
  fHVConnector5Ptr->Place(HVConnector5globalPos, HVConnector5globalRot, motherLogical);
  delete HVConnector5localPos;
  delete HVConnector5localRot;
  delete HVConnector5affine;
  
  G4ThreeVector *HVConnector6localPos = new G4ThreeVector(0.39*25.4*mm, -4.62*25.4*mm, 0);
  G4RotationMatrix *HVConnector6localRot = new G4RotationMatrix();
  HVConnector6localRot->rotateZ(-20*deg);
  G4AffineTransform *HVConnector6affine = new G4AffineTransform(HVConnector6localRot,*HVConnector6localPos);
  *HVConnector6affine*=*assemAffine;  
  G4ThreeVector *HVConnector6globalPos = new G4ThreeVector(HVConnector6affine->NetTranslation());
  G4RotationMatrix *HVConnector6globalRot= new G4RotationMatrix(HVConnector6affine->NetRotation());
  fHVConnector6Ptr->Place(HVConnector6globalPos, HVConnector6globalRot, motherLogical);
  delete HVConnector6localPos;
  delete HVConnector6localRot;
  delete HVConnector6affine;
  
  G4ThreeVector *HVConnector7localPos = new G4ThreeVector(3.59*25.4*mm, -2.73*25.4*mm, 0);
  G4RotationMatrix *HVConnector7localRot = new G4RotationMatrix();
  HVConnector7localRot->rotateZ(-20*deg);
  G4AffineTransform *HVConnector7affine = new G4AffineTransform(HVConnector7localRot,*HVConnector7localPos);
  *HVConnector7affine*=*assemAffine;  
  G4ThreeVector *HVConnector7globalPos = new G4ThreeVector(HVConnector7affine->NetTranslation());
  G4RotationMatrix *HVConnector7globalRot= new G4RotationMatrix(HVConnector7affine->NetRotation());
  fHVConnector7Ptr->Place(HVConnector7globalPos, HVConnector7globalRot, motherLogical);
  delete HVConnector7localPos;
  delete HVConnector7localRot;
  delete HVConnector7affine;  
  
  G4ThreeVector *SigCableConn1localPos = new G4ThreeVector(-1.4*25.4*mm, 0.45*25.4*mm, 0);
  G4RotationMatrix *SigCableConn1localRot = new G4RotationMatrix();
  G4AffineTransform *SigCableConn1affine = new G4AffineTransform(SigCableConn1localRot,*SigCableConn1localPos);
  *SigCableConn1affine*=*assemAffine;  
  G4ThreeVector *SigCableConn1globalPos = new G4ThreeVector(SigCableConn1affine->NetTranslation());
  G4RotationMatrix *SigCableConn1globalRot= new G4RotationMatrix(SigCableConn1affine->NetRotation());
  fSigCableConn1Ptr->Place(SigCableConn1globalPos, SigCableConn1globalRot, motherLogical);
  delete SigCableConn1localPos;
  delete SigCableConn1localRot;
  delete SigCableConn1affine; 

  G4ThreeVector *SigCableConn2localPos = new G4ThreeVector(2.01*25.4*mm, 2.58*25.4*mm, 0);
  G4RotationMatrix *SigCableConn2localRot = new G4RotationMatrix();
  G4AffineTransform *SigCableConn2affine = new G4AffineTransform(SigCableConn2localRot,*SigCableConn2localPos);
  *SigCableConn2affine*=*assemAffine;  
  G4ThreeVector *SigCableConn2globalPos = new G4ThreeVector(SigCableConn2affine->NetTranslation());
  G4RotationMatrix *SigCableConn2globalRot= new G4RotationMatrix(SigCableConn2affine->NetRotation());
  fSigCableConn2Ptr->Place(SigCableConn2globalPos, SigCableConn2globalRot, motherLogical);
  delete SigCableConn2localPos;
  delete SigCableConn2localRot;
  delete SigCableConn2affine; 

  G4ThreeVector *SigCableConn3localPos = new G4ThreeVector(-1.50*25.4*mm, 4.42*25.4*mm, 0);
  G4RotationMatrix *SigCableConn3localRot = new G4RotationMatrix();
  G4AffineTransform *SigCableConn3affine = new G4AffineTransform(SigCableConn3localRot,*SigCableConn3localPos);
  *SigCableConn3affine*=*assemAffine;  
  G4ThreeVector *SigCableConn3globalPos = new G4ThreeVector(SigCableConn3affine->NetTranslation());
  G4RotationMatrix *SigCableConn3globalRot= new G4RotationMatrix(SigCableConn3affine->NetRotation());
  fSigCableConn3Ptr->Place(SigCableConn3globalPos, SigCableConn3globalRot, motherLogical);
  delete SigCableConn3localPos;
  delete SigCableConn3localRot;
  delete SigCableConn3affine; 

  G4ThreeVector *SigCableConn4localPos = new G4ThreeVector(-4.55*25.4*mm, 2.55*25.4*mm, 0);
  G4RotationMatrix *SigCableConn4localRot = new G4RotationMatrix();
  G4AffineTransform *SigCableConn4affine = new G4AffineTransform(SigCableConn4localRot,*SigCableConn4localPos);
  *SigCableConn4affine*=*assemAffine;  
  G4ThreeVector *SigCableConn4globalPos = new G4ThreeVector(SigCableConn4affine->NetTranslation());
  G4RotationMatrix *SigCableConn4globalRot= new G4RotationMatrix(SigCableConn4affine->NetRotation());
  fSigCableConn4Ptr->Place(SigCableConn4globalPos, SigCableConn4globalRot, motherLogical);
  delete SigCableConn4localPos;
  delete SigCableConn4localRot;
  delete SigCableConn4affine; 

  G4ThreeVector *SigCableConn5localPos = new G4ThreeVector(-4.62*25.4*mm, -1.16*25.4*mm, 0);
  G4RotationMatrix *SigCableConn5localRot = new G4RotationMatrix();
  G4AffineTransform *SigCableConn5affine = new G4AffineTransform(SigCableConn5localRot,*SigCableConn5localPos);
  *SigCableConn5affine*=*assemAffine;  
  G4ThreeVector *SigCableConn5globalPos = new G4ThreeVector(SigCableConn5affine->NetTranslation());
  G4RotationMatrix *SigCableConn5globalRot= new G4RotationMatrix(SigCableConn5affine->NetRotation());
  fSigCableConn5Ptr->Place(SigCableConn5globalPos, SigCableConn5globalRot, motherLogical);
  delete SigCableConn5localPos;
  delete SigCableConn5localRot;
  delete SigCableConn5affine; 

  G4ThreeVector *SigCableConn6localPos = new G4ThreeVector(-1.31*25.4*mm, -3.20*25.4*mm, 0);
  G4RotationMatrix *SigCableConn6localRot = new G4RotationMatrix();
  G4AffineTransform *SigCableConn6affine = new G4AffineTransform(SigCableConn6localRot,*SigCableConn6localPos);
  *SigCableConn6affine*=*assemAffine;  
  G4ThreeVector *SigCableConn6globalPos = new G4ThreeVector(SigCableConn6affine->NetTranslation());
  G4RotationMatrix *SigCableConn6globalRot= new G4RotationMatrix(SigCableConn6affine->NetRotation());
  fSigCableConn6Ptr->Place(SigCableConn6globalPos, SigCableConn6globalRot, motherLogical);
  delete SigCableConn6localPos;
  delete SigCableConn6localRot;
  delete SigCableConn6affine; 

  G4ThreeVector *SigCableConn7localPos = new G4ThreeVector(1.74*25.4*mm, -1.58*25.4*mm, 0);
  G4RotationMatrix *SigCableConn7localRot = new G4RotationMatrix();
  G4AffineTransform *SigCableConn7affine = new G4AffineTransform(SigCableConn7localRot,*SigCableConn7localPos);
  *SigCableConn7affine*=*assemAffine;  
  G4ThreeVector *SigCableConn7globalPos = new G4ThreeVector(SigCableConn7affine->NetTranslation());
  G4RotationMatrix *SigCableConn7globalRot= new G4RotationMatrix(SigCableConn7affine->NetRotation());
  fSigCableConn7Ptr->Place(SigCableConn7globalPos, SigCableConn7globalRot, motherLogical);
  delete SigCableConn7localPos;
  delete SigCableConn7localRot;
  delete SigCableConn7affine; 

  G4ThreeVector *Cable11localPos = new G4ThreeVector(0*25.4*mm, 0.2*25.4*mm, 2.0*25.4*mm);
  G4RotationMatrix *Cable11localRot = new G4RotationMatrix();
  Cable11localRot->rotateY(pi/2);
  G4AffineTransform *Cable11affine = new G4AffineTransform(Cable11localRot,*Cable11localPos);
  *Cable11affine*=*assemAffine;  
  G4ThreeVector *Cable11globalPos = new G4ThreeVector(Cable11affine->NetTranslation());
  G4RotationMatrix *Cable11globalRot= new G4RotationMatrix(Cable11affine->NetRotation());
  fCable11Ptr->Place(Cable11globalPos, Cable11globalRot, motherLogical);
  delete Cable11localPos;
  delete Cable11localRot;
  delete Cable11affine; 

  G4ThreeVector *Cable12localPos = new G4ThreeVector(0*25.4*mm, 0.1*25.4*mm, 2.0*25.4*mm);
  G4RotationMatrix *Cable12localRot = new G4RotationMatrix();
  Cable12localRot->rotateY(pi/2);
  G4AffineTransform *Cable12affine = new G4AffineTransform(Cable12localRot,*Cable12localPos);
  *Cable12affine*=*assemAffine;  
  G4ThreeVector *Cable12globalPos = new G4ThreeVector(Cable12affine->NetTranslation());
  G4RotationMatrix *Cable12globalRot= new G4RotationMatrix(Cable12affine->NetRotation());
  fCable12Ptr->Place(Cable12globalPos, Cable12globalRot, motherLogical);
  delete Cable12localPos;
  delete Cable12localRot;
  delete Cable12affine; 

  G4ThreeVector *Cable13localPos = new G4ThreeVector(0*25.4*mm, 0*25.4*mm, 2.0*25.4*mm);
  G4RotationMatrix *Cable13localRot = new G4RotationMatrix();
  Cable13localRot->rotateY(pi/2);
  G4AffineTransform *Cable13affine = new G4AffineTransform(Cable13localRot,*Cable13localPos);
  *Cable13affine*=*assemAffine;  
  G4ThreeVector *Cable13globalPos = new G4ThreeVector(Cable13affine->NetTranslation());
  G4RotationMatrix *Cable13globalRot= new G4RotationMatrix(Cable13affine->NetRotation());
  fCable13Ptr->Place(Cable13globalPos, Cable13globalRot, motherLogical);
  delete Cable13localPos;
  delete Cable13localRot;
  delete Cable13affine; 

  G4ThreeVector *Cable14localPos = new G4ThreeVector(0*25.4*mm, -0.1*25.4*mm, 2.0*25.4*mm);
  G4RotationMatrix *Cable14localRot = new G4RotationMatrix();
  Cable14localRot->rotateY(pi/2);
  G4AffineTransform *Cable14affine = new G4AffineTransform(Cable14localRot,*Cable14localPos);
  *Cable14affine*=*assemAffine;  
  G4ThreeVector *Cable14globalPos = new G4ThreeVector(Cable14affine->NetTranslation());
  G4RotationMatrix *Cable14globalRot= new G4RotationMatrix(Cable14affine->NetRotation());
  fCable14Ptr->Place(Cable14globalPos, Cable14globalRot, motherLogical);
  delete Cable14localPos;
  delete Cable14localRot;
  delete Cable14affine; 

  G4ThreeVector *Cable15localPos = new G4ThreeVector(0*25.4*mm, -0.2*25.4*mm, 2.0*25.4*mm);
  G4RotationMatrix *Cable15localRot = new G4RotationMatrix();
  Cable15localRot->rotateY(pi/2);
  G4AffineTransform *Cable15affine = new G4AffineTransform(Cable15localRot,*Cable15localPos);
  *Cable15affine*=*assemAffine;  
  G4ThreeVector *Cable15globalPos = new G4ThreeVector(Cable15affine->NetTranslation());
  G4RotationMatrix *Cable15globalRot= new G4RotationMatrix(Cable15affine->NetRotation());
  fCable15Ptr->Place(Cable15globalPos, Cable15globalRot, motherLogical);
  delete Cable15localPos;
  delete Cable15localRot;
  delete Cable15affine; 

  G4ThreeVector *Cable21localPos = new G4ThreeVector(3.23*25.4*mm, 1.86*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *Cable21localRot = new G4RotationMatrix();
  Cable21localRot->rotateZ(8.51*deg);
  Cable21localRot->rotateY(pi/2);
  G4AffineTransform *Cable21affine = new G4AffineTransform(Cable21localRot,*Cable21localPos);
  *Cable21affine*=*assemAffine;  
  G4ThreeVector *Cable21globalPos = new G4ThreeVector(Cable21affine->NetTranslation());
  G4RotationMatrix *Cable21globalRot= new G4RotationMatrix(Cable21affine->NetRotation());
  fCable21Ptr->Place(Cable21globalPos, Cable21globalRot, motherLogical);
  delete Cable21localPos;
  delete Cable21localRot;
  delete Cable21affine; 

  G4ThreeVector *Cable22localPos = new G4ThreeVector(3.23*25.4*mm, 1.86*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *Cable22localRot = new G4RotationMatrix();
  Cable22localRot->rotateZ(8.51*deg);
  Cable22localRot->rotateY(pi/2);
  G4AffineTransform *Cable22affine = new G4AffineTransform(Cable22localRot,*Cable22localPos);
  *Cable22affine*=*assemAffine;  
  G4ThreeVector *Cable22globalPos = new G4ThreeVector(Cable22affine->NetTranslation());
  G4RotationMatrix *Cable22globalRot= new G4RotationMatrix(Cable22affine->NetRotation());
  fCable22Ptr->Place(Cable22globalPos, Cable22globalRot, motherLogical);
  delete Cable22localPos;
  delete Cable22localRot;
  delete Cable22affine; 

  G4ThreeVector *Cable23localPos = new G4ThreeVector(3.23*25.4*mm, 1.86*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *Cable23localRot = new G4RotationMatrix();
  Cable23localRot->rotateZ(8.51*deg);
  Cable23localRot->rotateY(pi/2);
  G4AffineTransform *Cable23affine = new G4AffineTransform(Cable23localRot,*Cable23localPos);
  *Cable23affine*=*assemAffine;  
  G4ThreeVector *Cable23globalPos = new G4ThreeVector(Cable23affine->NetTranslation());
  G4RotationMatrix *Cable23globalRot= new G4RotationMatrix(Cable23affine->NetRotation());
  fCable23Ptr->Place(Cable23globalPos, Cable23globalRot, motherLogical);
  delete Cable23localPos;
  delete Cable23localRot;
  delete Cable23affine; 

  G4ThreeVector *Cable24localPos = new G4ThreeVector(3.23*25.4*mm, 1.86*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *Cable24localRot = new G4RotationMatrix();
  Cable24localRot->rotateZ(8.51*deg);
  Cable24localRot->rotateY(pi/2);
  G4AffineTransform *Cable24affine = new G4AffineTransform(Cable24localRot,*Cable24localPos);
  *Cable24affine*=*assemAffine;  
  G4ThreeVector *Cable24globalPos = new G4ThreeVector(Cable24affine->NetTranslation());
  G4RotationMatrix *Cable24globalRot= new G4RotationMatrix(Cable24affine->NetRotation());
  fCable24Ptr->Place(Cable24globalPos, Cable24globalRot, motherLogical);
  delete Cable24localPos;
  delete Cable24localRot;
  delete Cable24affine; 

  G4ThreeVector *Cable25localPos = new G4ThreeVector(3.23*25.4*mm, 1.86*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *Cable25localRot = new G4RotationMatrix();
  Cable25localRot->rotateZ(8.51*deg);
  Cable25localRot->rotateY(pi/2);
  G4AffineTransform *Cable25affine = new G4AffineTransform(Cable25localRot,*Cable25localPos);
  *Cable25affine*=*assemAffine;  
  G4ThreeVector *Cable25globalPos = new G4ThreeVector(Cable25affine->NetTranslation());
  G4RotationMatrix *Cable25globalRot= new G4RotationMatrix(Cable25affine->NetRotation());
  fCable25Ptr->Place(Cable25globalPos, Cable25globalRot, motherLogical);
  delete Cable25localPos;
  delete Cable25localRot;
  delete Cable25affine; 

  G4ThreeVector *Cable31localPos = new G4ThreeVector(0*25.4*mm, 3.73*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *Cable31localRot = new G4RotationMatrix();
  Cable31localRot->rotateZ(18.91*deg);
  Cable31localRot->rotateY(pi/2);
  G4AffineTransform *Cable31affine = new G4AffineTransform(Cable31localRot,*Cable31localPos);
  *Cable31affine*=*assemAffine;  
  G4ThreeVector *Cable31globalPos = new G4ThreeVector(Cable31affine->NetTranslation());
  G4RotationMatrix *Cable31globalRot= new G4RotationMatrix(Cable31affine->NetRotation());
  fCable31Ptr->Place(Cable31globalPos, Cable31globalRot, motherLogical);
  delete Cable31localPos;
  delete Cable31localRot;
  delete Cable31affine; 

  G4ThreeVector *Cable32localPos = new G4ThreeVector(0*25.4*mm, 3.73*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *Cable32localRot = new G4RotationMatrix();
  Cable32localRot->rotateZ(18.91*deg);
  Cable32localRot->rotateY(pi/2);
  G4AffineTransform *Cable32affine = new G4AffineTransform(Cable32localRot,*Cable32localPos);
  *Cable32affine*=*assemAffine;  
  G4ThreeVector *Cable32globalPos = new G4ThreeVector(Cable32affine->NetTranslation());
  G4RotationMatrix *Cable32globalRot= new G4RotationMatrix(Cable32affine->NetRotation());
  fCable32Ptr->Place(Cable32globalPos, Cable32globalRot, motherLogical);
  delete Cable32localPos;
  delete Cable32localRot;
  delete Cable32affine; 

  G4ThreeVector *Cable33localPos = new G4ThreeVector(0*25.4*mm, 3.73*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *Cable33localRot = new G4RotationMatrix();
  Cable33localRot->rotateZ(18.91*deg);
  Cable33localRot->rotateY(pi/2);
  G4AffineTransform *Cable33affine = new G4AffineTransform(Cable33localRot,*Cable33localPos);
  *Cable33affine*=*assemAffine;  
  G4ThreeVector *Cable33globalPos = new G4ThreeVector(Cable33affine->NetTranslation());
  G4RotationMatrix *Cable33globalRot= new G4RotationMatrix(Cable33affine->NetRotation());
  fCable33Ptr->Place(Cable33globalPos, Cable33globalRot, motherLogical);
  delete Cable33localPos;
  delete Cable33localRot;
  delete Cable33affine; 

  G4ThreeVector *Cable34localPos = new G4ThreeVector(0*25.4*mm, 3.73*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *Cable34localRot = new G4RotationMatrix();
  Cable34localRot->rotateZ(18.91*deg);
  Cable34localRot->rotateY(pi/2);
  G4AffineTransform *Cable34affine = new G4AffineTransform(Cable34localRot,*Cable34localPos);
  *Cable34affine*=*assemAffine;  
  G4ThreeVector *Cable34globalPos = new G4ThreeVector(Cable34affine->NetTranslation());
  G4RotationMatrix *Cable34globalRot= new G4RotationMatrix(Cable34affine->NetRotation());
  fCable34Ptr->Place(Cable34globalPos, Cable34globalRot, motherLogical);
  delete Cable34localPos;
  delete Cable34localRot;
  delete Cable34affine; 

  G4ThreeVector *Cable35localPos = new G4ThreeVector(0*25.4*mm, 3.73*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *Cable35localRot = new G4RotationMatrix();
  Cable35localRot->rotateZ(18.91*deg);
  Cable35localRot->rotateY(pi/2);
  G4AffineTransform *Cable35affine = new G4AffineTransform(Cable35localRot,*Cable35localPos);
  *Cable35affine*=*assemAffine;  
  G4ThreeVector *Cable35globalPos = new G4ThreeVector(Cable35affine->NetTranslation());
  G4RotationMatrix *Cable35globalRot= new G4RotationMatrix(Cable35affine->NetRotation());
  fCable35Ptr->Place(Cable35globalPos, Cable35globalRot, motherLogical);
  delete Cable35localPos;
  delete Cable35localRot;
  delete Cable35affine; 

  G4ThreeVector *Cable41localPos = new G4ThreeVector(-3.23*25.4*mm, 1.86*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *Cable41localRot = new G4RotationMatrix();
  Cable41localRot->rotateZ(3.83*deg);
  Cable41localRot->rotateY(pi/2);
  G4AffineTransform *Cable41affine = new G4AffineTransform(Cable41localRot,*Cable41localPos);
  *Cable41affine*=*assemAffine;  
  G4ThreeVector *Cable41globalPos = new G4ThreeVector(Cable41affine->NetTranslation());
  G4RotationMatrix *Cable41globalRot= new G4RotationMatrix(Cable41affine->NetRotation());
  fCable41Ptr->Place(Cable41globalPos, Cable41globalRot, motherLogical);
  delete Cable41localPos;
  delete Cable41localRot;
  delete Cable41affine; 

  G4ThreeVector *Cable42localPos = new G4ThreeVector(-3.23*25.4*mm, 1.86*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *Cable42localRot = new G4RotationMatrix();
  Cable42localRot->rotateZ(3.83*deg);
  Cable42localRot->rotateY(pi/2);
  G4AffineTransform *Cable42affine = new G4AffineTransform(Cable42localRot,*Cable42localPos);
  *Cable42affine*=*assemAffine;  
  G4ThreeVector *Cable42globalPos = new G4ThreeVector(Cable42affine->NetTranslation());
  G4RotationMatrix *Cable42globalRot= new G4RotationMatrix(Cable42affine->NetRotation());
  fCable42Ptr->Place(Cable42globalPos, Cable42globalRot, motherLogical);
  delete Cable42localPos;
  delete Cable42localRot;
  delete Cable42affine; 

  G4ThreeVector *Cable43localPos = new G4ThreeVector(-3.23*25.4*mm, 1.86*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *Cable43localRot = new G4RotationMatrix();
  Cable43localRot->rotateZ(3.83*deg);
  Cable43localRot->rotateY(pi/2);
  G4AffineTransform *Cable43affine = new G4AffineTransform(Cable43localRot,*Cable43localPos);
  *Cable43affine*=*assemAffine;  
  G4ThreeVector *Cable43globalPos = new G4ThreeVector(Cable43affine->NetTranslation());
  G4RotationMatrix *Cable43globalRot= new G4RotationMatrix(Cable43affine->NetRotation());
  fCable43Ptr->Place(Cable43globalPos, Cable43globalRot, motherLogical);
  delete Cable43localPos;
  delete Cable43localRot;
  delete Cable43affine; 

  G4ThreeVector *Cable44localPos = new G4ThreeVector(-3.23*25.4*mm, 1.86*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *Cable44localRot = new G4RotationMatrix();
  Cable44localRot->rotateZ(3.83*deg);
  Cable44localRot->rotateY(pi/2);
  G4AffineTransform *Cable44affine = new G4AffineTransform(Cable44localRot,*Cable44localPos);
  *Cable44affine*=*assemAffine;  
  G4ThreeVector *Cable44globalPos = new G4ThreeVector(Cable44affine->NetTranslation());
  G4RotationMatrix *Cable44globalRot= new G4RotationMatrix(Cable44affine->NetRotation());
  fCable44Ptr->Place(Cable44globalPos, Cable44globalRot, motherLogical);
  delete Cable44localPos;
  delete Cable44localRot;
  delete Cable44affine; 

  G4ThreeVector *Cable45localPos = new G4ThreeVector(-3.23*25.4*mm, 1.86*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *Cable45localRot = new G4RotationMatrix();
  Cable45localRot->rotateZ(3.83*deg);
  Cable45localRot->rotateY(pi/2);
  G4AffineTransform *Cable45affine = new G4AffineTransform(Cable45localRot,*Cable45localPos);
  *Cable45affine*=*assemAffine;  
  G4ThreeVector *Cable45globalPos = new G4ThreeVector(Cable45affine->NetTranslation());
  G4RotationMatrix *Cable45globalRot= new G4RotationMatrix(Cable45affine->NetRotation());
  fCable45Ptr->Place(Cable45globalPos, Cable45globalRot, motherLogical);
  delete Cable45localPos;
  delete Cable45localRot;
  delete Cable45affine; 

  G4ThreeVector *Cable51localPos = new G4ThreeVector(-3.23*25.4*mm, -1.86*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *Cable51localRot = new G4RotationMatrix();
  Cable51localRot->rotateZ(-3.83*deg);
  Cable51localRot->rotateY(pi/2);
  G4AffineTransform *Cable51affine = new G4AffineTransform(Cable51localRot,*Cable51localPos);
  *Cable51affine*=*assemAffine;  
  G4ThreeVector *Cable51globalPos = new G4ThreeVector(Cable51affine->NetTranslation());
  G4RotationMatrix *Cable51globalRot= new G4RotationMatrix(Cable51affine->NetRotation());
  fCable51Ptr->Place(Cable51globalPos, Cable51globalRot, motherLogical);
  delete Cable51localPos;
  delete Cable51localRot;
  delete Cable51affine; 

  G4ThreeVector *Cable52localPos = new G4ThreeVector(-3.23*25.4*mm, -1.86*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *Cable52localRot = new G4RotationMatrix();
  Cable52localRot->rotateZ(-3.83*deg);
  Cable52localRot->rotateY(pi/2);
  G4AffineTransform *Cable52affine = new G4AffineTransform(Cable52localRot,*Cable52localPos);
  *Cable52affine*=*assemAffine;  
  G4ThreeVector *Cable52globalPos = new G4ThreeVector(Cable52affine->NetTranslation());
  G4RotationMatrix *Cable52globalRot= new G4RotationMatrix(Cable52affine->NetRotation());
  fCable52Ptr->Place(Cable52globalPos, Cable52globalRot, motherLogical);
  delete Cable52localPos;
  delete Cable52localRot;
  delete Cable52affine; 

  G4ThreeVector *Cable53localPos = new G4ThreeVector(-3.23*25.4*mm, -1.86*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *Cable53localRot = new G4RotationMatrix();
  Cable53localRot->rotateZ(-3.83*deg);
  Cable53localRot->rotateY(pi/2);
  G4AffineTransform *Cable53affine = new G4AffineTransform(Cable53localRot,*Cable53localPos);
  *Cable53affine*=*assemAffine;  
  G4ThreeVector *Cable53globalPos = new G4ThreeVector(Cable53affine->NetTranslation());
  G4RotationMatrix *Cable53globalRot= new G4RotationMatrix(Cable53affine->NetRotation());
  fCable53Ptr->Place(Cable53globalPos, Cable53globalRot, motherLogical);
  delete Cable53localPos;
  delete Cable53localRot;
  delete Cable53affine; 

  G4ThreeVector *Cable54localPos = new G4ThreeVector(-3.23*25.4*mm, -1.86*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *Cable54localRot = new G4RotationMatrix();
  Cable54localRot->rotateZ(-3.83*deg);
  Cable54localRot->rotateY(pi/2);
  G4AffineTransform *Cable54affine = new G4AffineTransform(Cable54localRot,*Cable54localPos);
  *Cable54affine*=*assemAffine;  
  G4ThreeVector *Cable54globalPos = new G4ThreeVector(Cable54affine->NetTranslation());
  G4RotationMatrix *Cable54globalRot= new G4RotationMatrix(Cable54affine->NetRotation());
  fCable54Ptr->Place(Cable54globalPos, Cable54globalRot, motherLogical);
  delete Cable54localPos;
  delete Cable54localRot;
  delete Cable54affine; 

  G4ThreeVector *Cable55localPos = new G4ThreeVector(-3.23*25.4*mm, -1.86*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *Cable55localRot = new G4RotationMatrix();
  Cable55localRot->rotateZ(-3.83*deg);
  Cable55localRot->rotateY(pi/2);  
  G4AffineTransform *Cable55affine = new G4AffineTransform(Cable55localRot,*Cable55localPos);
  *Cable55affine*=*assemAffine;  
  G4ThreeVector *Cable55globalPos = new G4ThreeVector(Cable55affine->NetTranslation());
  G4RotationMatrix *Cable55globalRot= new G4RotationMatrix(Cable55affine->NetRotation());
  fCable55Ptr->Place(Cable55globalPos, Cable55globalRot, motherLogical);
  delete Cable55localPos;
  delete Cable55localRot;
  delete Cable55affine; 

  G4ThreeVector *Cable61localPos = new G4ThreeVector(0*25.4*mm, -3.73*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *Cable61localRot = new G4RotationMatrix();
  Cable61localRot->rotateZ(-18.91*deg);
  Cable61localRot->rotateY(pi/2);  
  G4AffineTransform *Cable61affine = new G4AffineTransform(Cable61localRot,*Cable61localPos);
  *Cable61affine*=*assemAffine;  
  G4ThreeVector *Cable61globalPos = new G4ThreeVector(Cable61affine->NetTranslation());
  G4RotationMatrix *Cable61globalRot= new G4RotationMatrix(Cable61affine->NetRotation());
  fCable61Ptr->Place(Cable61globalPos, Cable61globalRot, motherLogical);
  delete Cable61localPos;
  delete Cable61localRot;
  delete Cable61affine; 

  G4ThreeVector *Cable62localPos = new G4ThreeVector(0*25.4*mm, -3.73*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *Cable62localRot = new G4RotationMatrix();
  Cable62localRot->rotateZ(-18.91*deg);
  Cable62localRot->rotateY(pi/2);  
  G4AffineTransform *Cable62affine = new G4AffineTransform(Cable62localRot,*Cable62localPos);
  *Cable62affine*=*assemAffine;  
  G4ThreeVector *Cable62globalPos = new G4ThreeVector(Cable62affine->NetTranslation());
  G4RotationMatrix *Cable62globalRot= new G4RotationMatrix(Cable62affine->NetRotation());
  fCable62Ptr->Place(Cable62globalPos, Cable62globalRot, motherLogical);
  delete Cable62localPos;
  delete Cable62localRot;
  delete Cable62affine; 

  G4ThreeVector *Cable63localPos = new G4ThreeVector(0*25.4*mm, -3.73*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *Cable63localRot = new G4RotationMatrix();
  Cable63localRot->rotateZ(-18.91*deg);
  Cable63localRot->rotateY(pi/2);  
  G4AffineTransform *Cable63affine = new G4AffineTransform(Cable63localRot,*Cable63localPos);
  *Cable63affine*=*assemAffine;  
  G4ThreeVector *Cable63globalPos = new G4ThreeVector(Cable63affine->NetTranslation());
  G4RotationMatrix *Cable63globalRot= new G4RotationMatrix(Cable63affine->NetRotation());
  fCable63Ptr->Place(Cable63globalPos, Cable63globalRot, motherLogical);
  delete Cable63localPos;
  delete Cable63localRot;
  delete Cable63affine; 

  G4ThreeVector *Cable64localPos = new G4ThreeVector(0*25.4*mm, -3.73*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *Cable64localRot = new G4RotationMatrix();
  Cable64localRot->rotateZ(-18.91*deg);
  Cable64localRot->rotateY(pi/2);  
  G4AffineTransform *Cable64affine = new G4AffineTransform(Cable64localRot,*Cable64localPos);
  *Cable64affine*=*assemAffine;  
  G4ThreeVector *Cable64globalPos = new G4ThreeVector(Cable64affine->NetTranslation());
  G4RotationMatrix *Cable64globalRot= new G4RotationMatrix(Cable64affine->NetRotation());
  fCable64Ptr->Place(Cable64globalPos, Cable64globalRot, motherLogical);
  delete Cable64localPos;
  delete Cable64localRot;
  delete Cable64affine; 

  G4ThreeVector *Cable65localPos = new G4ThreeVector(0*25.4*mm, -3.73*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *Cable65localRot = new G4RotationMatrix();
  Cable65localRot->rotateZ(-18.91*deg);
  Cable65localRot->rotateY(pi/2);  
  G4AffineTransform *Cable65affine = new G4AffineTransform(Cable65localRot,*Cable65localPos);
  *Cable65affine*=*assemAffine;  
  G4ThreeVector *Cable65globalPos = new G4ThreeVector(Cable65affine->NetTranslation());
  G4RotationMatrix *Cable65globalRot= new G4RotationMatrix(Cable65affine->NetRotation());
  fCable65Ptr->Place(Cable65globalPos, Cable65globalRot, motherLogical);
  delete Cable65localPos;
  delete Cable65localRot;
  delete Cable65affine; 

  G4ThreeVector *Cable71localPos = new G4ThreeVector(3.23*25.4*mm, -1.86*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *Cable71localRot = new G4RotationMatrix();
  Cable71localRot->rotateZ(-8.51*deg);
  Cable71localRot->rotateY(pi/2);  
  G4AffineTransform *Cable71affine = new G4AffineTransform(Cable71localRot,*Cable71localPos);
  *Cable71affine*=*assemAffine;  
  G4ThreeVector *Cable71globalPos = new G4ThreeVector(Cable71affine->NetTranslation());
  G4RotationMatrix *Cable71globalRot= new G4RotationMatrix(Cable71affine->NetRotation());
  fCable71Ptr->Place(Cable71globalPos, Cable71globalRot, motherLogical);
  delete Cable71localPos;
  delete Cable71localRot;
  delete Cable71affine; 

  G4ThreeVector *Cable72localPos = new G4ThreeVector(3.23*25.4*mm, -1.86*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *Cable72localRot = new G4RotationMatrix();
  Cable72localRot->rotateZ(-8.51*deg);
  Cable72localRot->rotateY(pi/2);  
  G4AffineTransform *Cable72affine = new G4AffineTransform(Cable72localRot,*Cable72localPos);
  *Cable72affine*=*assemAffine;  
  G4ThreeVector *Cable72globalPos = new G4ThreeVector(Cable72affine->NetTranslation());
  G4RotationMatrix *Cable72globalRot= new G4RotationMatrix(Cable72affine->NetRotation());
  fCable72Ptr->Place(Cable72globalPos, Cable72globalRot, motherLogical);
  delete Cable72localPos;
  delete Cable72localRot;
  delete Cable72affine; 

  G4ThreeVector *Cable73localPos = new G4ThreeVector(3.23*25.4*mm, -1.86*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *Cable73localRot = new G4RotationMatrix();
  Cable73localRot->rotateZ(-8.51*deg);
  Cable73localRot->rotateY(pi/2);  
  G4AffineTransform *Cable73affine = new G4AffineTransform(Cable73localRot,*Cable73localPos);
  *Cable73affine*=*assemAffine;  
  G4ThreeVector *Cable73globalPos = new G4ThreeVector(Cable73affine->NetTranslation());
  G4RotationMatrix *Cable73globalRot= new G4RotationMatrix(Cable73affine->NetRotation());
  fCable73Ptr->Place(Cable73globalPos, Cable73globalRot, motherLogical);
  delete Cable73localPos;
  delete Cable73localRot;
  delete Cable73affine; 

  G4ThreeVector *Cable74localPos = new G4ThreeVector(3.23*25.4*mm, -1.86*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *Cable74localRot = new G4RotationMatrix();
  Cable74localRot->rotateZ(-8.51*deg);
  Cable74localRot->rotateY(pi/2);  
  G4AffineTransform *Cable74affine = new G4AffineTransform(Cable74localRot,*Cable74localPos);
  *Cable74affine*=*assemAffine;  
  G4ThreeVector *Cable74globalPos = new G4ThreeVector(Cable74affine->NetTranslation());
  G4RotationMatrix *Cable74globalRot= new G4RotationMatrix(Cable74affine->NetRotation());
  fCable74Ptr->Place(Cable74globalPos, Cable74globalRot, motherLogical);
  delete Cable74localPos;
  delete Cable74localRot;
  delete Cable74affine; 

  G4ThreeVector *Cable75localPos = new G4ThreeVector(3.23*25.4*mm, -1.86*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *Cable75localRot = new G4RotationMatrix();
  Cable75localRot->rotateZ(-8.51*deg);
  Cable75localRot->rotateY(pi/2);  
  G4AffineTransform *Cable75affine = new G4AffineTransform(Cable75localRot,*Cable75localPos);
  *Cable75affine*=*assemAffine;  
  G4ThreeVector *Cable75globalPos = new G4ThreeVector(Cable75affine->NetTranslation());
  G4RotationMatrix *Cable75globalRot= new G4RotationMatrix(Cable75affine->NetRotation());
  fCable75Ptr->Place(Cable75globalPos, Cable75globalRot, motherLogical);
  delete Cable75localPos;
  delete Cable75localRot;
  delete Cable75affine; 

  G4ThreeVector *HVCable11localPos = new G4ThreeVector(0*25.4*mm, 0.2*25.4*mm, 2.1*25.4*mm);
  G4RotationMatrix *HVCable11localRot = new G4RotationMatrix();
  HVCable11localRot->rotateY(pi/2);
  G4AffineTransform *HVCable11affine = new G4AffineTransform(HVCable11localRot,*HVCable11localPos);
  *HVCable11affine*=*assemAffine;  
  G4ThreeVector *HVCable11globalPos = new G4ThreeVector(HVCable11affine->NetTranslation());
  G4RotationMatrix *HVCable11globalRot= new G4RotationMatrix(HVCable11affine->NetRotation());
  fHVCable11Ptr->Place(HVCable11globalPos, HVCable11globalRot, motherLogical);
  delete HVCable11localPos;
  delete HVCable11localRot;
  delete HVCable11affine; 

  G4ThreeVector *HVCable12localPos = new G4ThreeVector(0*25.4*mm, 0.1*25.4*mm, 2.1*25.4*mm);
  G4RotationMatrix *HVCable12localRot = new G4RotationMatrix();
  HVCable12localRot->rotateY(pi/2);
  G4AffineTransform *HVCable12affine = new G4AffineTransform(HVCable12localRot,*HVCable12localPos);
  *HVCable12affine*=*assemAffine;  
  G4ThreeVector *HVCable12globalPos = new G4ThreeVector(HVCable12affine->NetTranslation());
  G4RotationMatrix *HVCable12globalRot= new G4RotationMatrix(HVCable12affine->NetRotation());
  fHVCable12Ptr->Place(HVCable12globalPos, HVCable12globalRot, motherLogical);
  delete HVCable12localPos;
  delete HVCable12localRot;
  delete HVCable12affine; 

  G4ThreeVector *HVCable13localPos = new G4ThreeVector(0*25.4*mm, 0*25.4*mm, 2.1*25.4*mm);
  G4RotationMatrix *HVCable13localRot = new G4RotationMatrix();
  HVCable13localRot->rotateY(pi/2);
  G4AffineTransform *HVCable13affine = new G4AffineTransform(HVCable13localRot,*HVCable13localPos);
  *HVCable13affine*=*assemAffine;  
  G4ThreeVector *HVCable13globalPos = new G4ThreeVector(HVCable13affine->NetTranslation());
  G4RotationMatrix *HVCable13globalRot= new G4RotationMatrix(HVCable13affine->NetRotation());
  fHVCable13Ptr->Place(HVCable13globalPos, HVCable13globalRot, motherLogical);
  delete HVCable13localPos;
  delete HVCable13localRot;
  delete HVCable13affine; 

  G4ThreeVector *HVCable14localPos = new G4ThreeVector(0*25.4*mm, -0.1*25.4*mm, 2.1*25.4*mm);
  G4RotationMatrix *HVCable14localRot = new G4RotationMatrix();
  HVCable14localRot->rotateY(pi/2);
  G4AffineTransform *HVCable14affine = new G4AffineTransform(HVCable14localRot,*HVCable14localPos);
  *HVCable14affine*=*assemAffine;  
  G4ThreeVector *HVCable14globalPos = new G4ThreeVector(HVCable14affine->NetTranslation());
  G4RotationMatrix *HVCable14globalRot= new G4RotationMatrix(HVCable14affine->NetRotation());
  fHVCable14Ptr->Place(HVCable14globalPos, HVCable14globalRot, motherLogical);
  delete HVCable14localPos;
  delete HVCable14localRot;
  delete HVCable14affine; 

  G4ThreeVector *HVCable15localPos = new G4ThreeVector(0*25.4*mm, -0.2*25.4*mm, 2.1*25.4*mm);
  G4RotationMatrix *HVCable15localRot = new G4RotationMatrix();
  HVCable15localRot->rotateY(pi/2);
  G4AffineTransform *HVCable15affine = new G4AffineTransform(HVCable15localRot,*HVCable15localPos);
  *HVCable15affine*=*assemAffine;  
  G4ThreeVector *HVCable15globalPos = new G4ThreeVector(HVCable15affine->NetTranslation());
  G4RotationMatrix *HVCable15globalRot= new G4RotationMatrix(HVCable15affine->NetRotation());
  fHVCable15Ptr->Place(HVCable15globalPos, HVCable15globalRot, motherLogical);
  delete HVCable15localPos;
  delete HVCable15localRot;
  delete HVCable15affine; 

  G4ThreeVector *HVCable21localPos = new G4ThreeVector(3.23*25.4*mm, 1.96*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *HVCable21localRot = new G4RotationMatrix();
  HVCable21localRot->rotateZ(8.51*deg);
  HVCable21localRot->rotateY(pi/2);
  G4AffineTransform *HVCable21affine = new G4AffineTransform(HVCable21localRot,*HVCable21localPos);
  *HVCable21affine*=*assemAffine;  
  G4ThreeVector *HVCable21globalPos = new G4ThreeVector(HVCable21affine->NetTranslation());
  G4RotationMatrix *HVCable21globalRot= new G4RotationMatrix(HVCable21affine->NetRotation());
  fHVCable21Ptr->Place(HVCable21globalPos, HVCable21globalRot, motherLogical);
  delete HVCable21localPos;
  delete HVCable21localRot;
  delete HVCable21affine; 

  G4ThreeVector *HVCable22localPos = new G4ThreeVector(3.23*25.4*mm, 1.96*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *HVCable22localRot = new G4RotationMatrix();
  HVCable22localRot->rotateZ(8.51*deg);
  HVCable22localRot->rotateY(pi/2);
  G4AffineTransform *HVCable22affine = new G4AffineTransform(HVCable22localRot,*HVCable22localPos);
  *HVCable22affine*=*assemAffine;  
  G4ThreeVector *HVCable22globalPos = new G4ThreeVector(HVCable22affine->NetTranslation());
  G4RotationMatrix *HVCable22globalRot= new G4RotationMatrix(HVCable22affine->NetRotation());
  fHVCable22Ptr->Place(HVCable22globalPos, HVCable22globalRot, motherLogical);
  delete HVCable22localPos;
  delete HVCable22localRot;
  delete HVCable22affine; 

  G4ThreeVector *HVCable23localPos = new G4ThreeVector(3.23*25.4*mm, 1.96*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *HVCable23localRot = new G4RotationMatrix();
  HVCable23localRot->rotateZ(8.51*deg);
  HVCable23localRot->rotateY(pi/2);
  G4AffineTransform *HVCable23affine = new G4AffineTransform(HVCable23localRot,*HVCable23localPos);
  *HVCable23affine*=*assemAffine;  
  G4ThreeVector *HVCable23globalPos = new G4ThreeVector(HVCable23affine->NetTranslation());
  G4RotationMatrix *HVCable23globalRot= new G4RotationMatrix(HVCable23affine->NetRotation());
  fHVCable23Ptr->Place(HVCable23globalPos, HVCable23globalRot, motherLogical);
  delete HVCable23localPos;
  delete HVCable23localRot;
  delete HVCable23affine; 

  G4ThreeVector *HVCable24localPos = new G4ThreeVector(3.23*25.4*mm, 1.96*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *HVCable24localRot = new G4RotationMatrix();
  HVCable24localRot->rotateZ(8.51*deg);
  HVCable24localRot->rotateY(pi/2);
  G4AffineTransform *HVCable24affine = new G4AffineTransform(HVCable24localRot,*HVCable24localPos);
  *HVCable24affine*=*assemAffine;  
  G4ThreeVector *HVCable24globalPos = new G4ThreeVector(HVCable24affine->NetTranslation());
  G4RotationMatrix *HVCable24globalRot= new G4RotationMatrix(HVCable24affine->NetRotation());
  fHVCable24Ptr->Place(HVCable24globalPos, HVCable24globalRot, motherLogical);
  delete HVCable24localPos;
  delete HVCable24localRot;
  delete HVCable24affine; 

  G4ThreeVector *HVCable25localPos = new G4ThreeVector(3.23*25.4*mm, 1.96*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *HVCable25localRot = new G4RotationMatrix();
  HVCable25localRot->rotateZ(8.51*deg);
  HVCable25localRot->rotateY(pi/2);
  G4AffineTransform *HVCable25affine = new G4AffineTransform(HVCable25localRot,*HVCable25localPos);
  *HVCable25affine*=*assemAffine;  
  G4ThreeVector *HVCable25globalPos = new G4ThreeVector(HVCable25affine->NetTranslation());
  G4RotationMatrix *HVCable25globalRot= new G4RotationMatrix(HVCable25affine->NetRotation());
  fHVCable25Ptr->Place(HVCable25globalPos, HVCable25globalRot, motherLogical);
  delete HVCable25localPos;
  delete HVCable25localRot;
  delete HVCable25affine; 

  G4ThreeVector *HVCable31localPos = new G4ThreeVector(0*25.4*mm, 3.83*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *HVCable31localRot = new G4RotationMatrix();
  HVCable31localRot->rotateZ(18.91*deg);
  HVCable31localRot->rotateY(pi/2);
  G4AffineTransform *HVCable31affine = new G4AffineTransform(HVCable31localRot,*HVCable31localPos);
  *HVCable31affine*=*assemAffine;  
  G4ThreeVector *HVCable31globalPos = new G4ThreeVector(HVCable31affine->NetTranslation());
  G4RotationMatrix *HVCable31globalRot= new G4RotationMatrix(HVCable31affine->NetRotation());
  fHVCable31Ptr->Place(HVCable31globalPos, HVCable31globalRot, motherLogical);
  delete HVCable31localPos;
  delete HVCable31localRot;
  delete HVCable31affine; 

  G4ThreeVector *HVCable32localPos = new G4ThreeVector(0*25.4*mm, 3.83*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *HVCable32localRot = new G4RotationMatrix();
  HVCable32localRot->rotateZ(18.91*deg);
  HVCable32localRot->rotateY(pi/2);
  G4AffineTransform *HVCable32affine = new G4AffineTransform(HVCable32localRot,*HVCable32localPos);
  *HVCable32affine*=*assemAffine;  
  G4ThreeVector *HVCable32globalPos = new G4ThreeVector(HVCable32affine->NetTranslation());
  G4RotationMatrix *HVCable32globalRot= new G4RotationMatrix(HVCable32affine->NetRotation());
  fHVCable32Ptr->Place(HVCable32globalPos, HVCable32globalRot, motherLogical);
  delete HVCable32localPos;
  delete HVCable32localRot;
  delete HVCable32affine; 

  G4ThreeVector *HVCable33localPos = new G4ThreeVector(0*25.4*mm, 3.83*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *HVCable33localRot = new G4RotationMatrix();
  HVCable33localRot->rotateZ(18.91*deg);
  HVCable33localRot->rotateY(pi/2);
  G4AffineTransform *HVCable33affine = new G4AffineTransform(HVCable33localRot,*HVCable33localPos);
  *HVCable33affine*=*assemAffine;  
  G4ThreeVector *HVCable33globalPos = new G4ThreeVector(HVCable33affine->NetTranslation());
  G4RotationMatrix *HVCable33globalRot= new G4RotationMatrix(HVCable33affine->NetRotation());
  fHVCable33Ptr->Place(HVCable33globalPos, HVCable33globalRot, motherLogical);
  delete HVCable33localPos;
  delete HVCable33localRot;
  delete HVCable33affine; 

  G4ThreeVector *HVCable34localPos = new G4ThreeVector(0*25.4*mm, 3.83*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *HVCable34localRot = new G4RotationMatrix();
  HVCable34localRot->rotateZ(18.91*deg);
  HVCable34localRot->rotateY(pi/2);
  G4AffineTransform *HVCable34affine = new G4AffineTransform(HVCable34localRot,*HVCable34localPos);
  *HVCable34affine*=*assemAffine;  
  G4ThreeVector *HVCable34globalPos = new G4ThreeVector(HVCable34affine->NetTranslation());
  G4RotationMatrix *HVCable34globalRot= new G4RotationMatrix(HVCable34affine->NetRotation());
  fHVCable34Ptr->Place(HVCable34globalPos, HVCable34globalRot, motherLogical);
  delete HVCable34localPos;
  delete HVCable34localRot;
  delete HVCable34affine; 

  G4ThreeVector *HVCable35localPos = new G4ThreeVector(0*25.4*mm, 3.83*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *HVCable35localRot = new G4RotationMatrix();
  HVCable35localRot->rotateZ(18.91*deg);
  HVCable35localRot->rotateY(pi/2);
  G4AffineTransform *HVCable35affine = new G4AffineTransform(HVCable35localRot,*HVCable35localPos);
  *HVCable35affine*=*assemAffine;  
  G4ThreeVector *HVCable35globalPos = new G4ThreeVector(HVCable35affine->NetTranslation());
  G4RotationMatrix *HVCable35globalRot= new G4RotationMatrix(HVCable35affine->NetRotation());
  fHVCable35Ptr->Place(HVCable35globalPos, HVCable35globalRot, motherLogical);
  delete HVCable35localPos;
  delete HVCable35localRot;
  delete HVCable35affine; 

  G4ThreeVector *HVCable41localPos = new G4ThreeVector(-3.23*25.4*mm, 1.96*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *HVCable41localRot = new G4RotationMatrix();
  HVCable41localRot->rotateZ(3.83*deg);
  HVCable41localRot->rotateY(pi/2);
  G4AffineTransform *HVCable41affine = new G4AffineTransform(HVCable41localRot,*HVCable41localPos);
  *HVCable41affine*=*assemAffine;  
  G4ThreeVector *HVCable41globalPos = new G4ThreeVector(HVCable41affine->NetTranslation());
  G4RotationMatrix *HVCable41globalRot= new G4RotationMatrix(HVCable41affine->NetRotation());
  fHVCable41Ptr->Place(HVCable41globalPos, HVCable41globalRot, motherLogical);
  delete HVCable41localPos;
  delete HVCable41localRot;
  delete HVCable41affine; 

  G4ThreeVector *HVCable42localPos = new G4ThreeVector(-3.23*25.4*mm, 1.96*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *HVCable42localRot = new G4RotationMatrix();
  HVCable42localRot->rotateZ(3.83*deg);
  HVCable42localRot->rotateY(pi/2);
  G4AffineTransform *HVCable42affine = new G4AffineTransform(HVCable42localRot,*HVCable42localPos);
  *HVCable42affine*=*assemAffine;  
  G4ThreeVector *HVCable42globalPos = new G4ThreeVector(HVCable42affine->NetTranslation());
  G4RotationMatrix *HVCable42globalRot= new G4RotationMatrix(HVCable42affine->NetRotation());
  fHVCable42Ptr->Place(HVCable42globalPos, HVCable42globalRot, motherLogical);
  delete HVCable42localPos;
  delete HVCable42localRot;
  delete HVCable42affine; 

  G4ThreeVector *HVCable43localPos = new G4ThreeVector(-3.23*25.4*mm, 1.96*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *HVCable43localRot = new G4RotationMatrix();
  HVCable43localRot->rotateZ(3.83*deg);
  HVCable43localRot->rotateY(pi/2);
  G4AffineTransform *HVCable43affine = new G4AffineTransform(HVCable43localRot,*HVCable43localPos);
  *HVCable43affine*=*assemAffine;  
  G4ThreeVector *HVCable43globalPos = new G4ThreeVector(HVCable43affine->NetTranslation());
  G4RotationMatrix *HVCable43globalRot= new G4RotationMatrix(HVCable43affine->NetRotation());
  fHVCable43Ptr->Place(HVCable43globalPos, HVCable43globalRot, motherLogical);
  delete HVCable43localPos;
  delete HVCable43localRot;
  delete HVCable43affine; 

  G4ThreeVector *HVCable44localPos = new G4ThreeVector(-3.23*25.4*mm, 1.96*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *HVCable44localRot = new G4RotationMatrix();
  HVCable44localRot->rotateZ(3.83*deg);
  HVCable44localRot->rotateY(pi/2);
  G4AffineTransform *HVCable44affine = new G4AffineTransform(HVCable44localRot,*HVCable44localPos);
  *HVCable44affine*=*assemAffine;  
  G4ThreeVector *HVCable44globalPos = new G4ThreeVector(HVCable44affine->NetTranslation());
  G4RotationMatrix *HVCable44globalRot= new G4RotationMatrix(HVCable44affine->NetRotation());
  fHVCable44Ptr->Place(HVCable44globalPos, HVCable44globalRot, motherLogical);
  delete HVCable44localPos;
  delete HVCable44localRot;
  delete HVCable44affine; 

  G4ThreeVector *HVCable45localPos = new G4ThreeVector(-3.23*25.4*mm, 1.96*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *HVCable45localRot = new G4RotationMatrix();
  HVCable45localRot->rotateZ(3.83*deg);
  HVCable45localRot->rotateY(pi/2);
  G4AffineTransform *HVCable45affine = new G4AffineTransform(HVCable45localRot,*HVCable45localPos);
  *HVCable45affine*=*assemAffine;  
  G4ThreeVector *HVCable45globalPos = new G4ThreeVector(HVCable45affine->NetTranslation());
  G4RotationMatrix *HVCable45globalRot= new G4RotationMatrix(HVCable45affine->NetRotation());
  fHVCable45Ptr->Place(HVCable45globalPos, HVCable45globalRot, motherLogical);
  delete HVCable45localPos;
  delete HVCable45localRot;
  delete HVCable45affine; 

  G4ThreeVector *HVCable51localPos = new G4ThreeVector(-3.23*25.4*mm, -1.96*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *HVCable51localRot = new G4RotationMatrix();
  HVCable51localRot->rotateZ(-3.83*deg);
  HVCable51localRot->rotateY(pi/2);
  G4AffineTransform *HVCable51affine = new G4AffineTransform(HVCable51localRot,*HVCable51localPos);
  *HVCable51affine*=*assemAffine;  
  G4ThreeVector *HVCable51globalPos = new G4ThreeVector(HVCable51affine->NetTranslation());
  G4RotationMatrix *HVCable51globalRot= new G4RotationMatrix(HVCable51affine->NetRotation());
  fHVCable51Ptr->Place(HVCable51globalPos, HVCable51globalRot, motherLogical);
  delete HVCable51localPos;
  delete HVCable51localRot;
  delete HVCable51affine; 

  G4ThreeVector *HVCable52localPos = new G4ThreeVector(-3.23*25.4*mm, -1.96*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *HVCable52localRot = new G4RotationMatrix();
  HVCable52localRot->rotateZ(-3.83*deg);
  HVCable52localRot->rotateY(pi/2);
  G4AffineTransform *HVCable52affine = new G4AffineTransform(HVCable52localRot,*HVCable52localPos);
  *HVCable52affine*=*assemAffine;  
  G4ThreeVector *HVCable52globalPos = new G4ThreeVector(HVCable52affine->NetTranslation());
  G4RotationMatrix *HVCable52globalRot= new G4RotationMatrix(HVCable52affine->NetRotation());
  fHVCable52Ptr->Place(HVCable52globalPos, HVCable52globalRot, motherLogical);
  delete HVCable52localPos;
  delete HVCable52localRot;
  delete HVCable52affine; 

  G4ThreeVector *HVCable53localPos = new G4ThreeVector(-3.23*25.4*mm, -1.96*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *HVCable53localRot = new G4RotationMatrix();
  HVCable53localRot->rotateZ(-3.83*deg);
  HVCable53localRot->rotateY(pi/2);
  G4AffineTransform *HVCable53affine = new G4AffineTransform(HVCable53localRot,*HVCable53localPos);
  *HVCable53affine*=*assemAffine;  
  G4ThreeVector *HVCable53globalPos = new G4ThreeVector(HVCable53affine->NetTranslation());
  G4RotationMatrix *HVCable53globalRot= new G4RotationMatrix(HVCable53affine->NetRotation());
  fHVCable53Ptr->Place(HVCable53globalPos, HVCable53globalRot, motherLogical);
  delete HVCable53localPos;
  delete HVCable53localRot;
  delete HVCable53affine; 

  G4ThreeVector *HVCable54localPos = new G4ThreeVector(-3.23*25.4*mm, -1.96*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *HVCable54localRot = new G4RotationMatrix();
  HVCable54localRot->rotateZ(-3.83*deg);
  HVCable54localRot->rotateY(pi/2);
  G4AffineTransform *HVCable54affine = new G4AffineTransform(HVCable54localRot,*HVCable54localPos);
  *HVCable54affine*=*assemAffine;  
  G4ThreeVector *HVCable54globalPos = new G4ThreeVector(HVCable54affine->NetTranslation());
  G4RotationMatrix *HVCable54globalRot= new G4RotationMatrix(HVCable54affine->NetRotation());
  fHVCable54Ptr->Place(HVCable54globalPos, HVCable54globalRot, motherLogical);
  delete HVCable54localPos;
  delete HVCable54localRot;
  delete HVCable54affine; 

  G4ThreeVector *HVCable55localPos = new G4ThreeVector(-3.23*25.4*mm, -1.96*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *HVCable55localRot = new G4RotationMatrix();
  HVCable55localRot->rotateZ(-3.83*deg);
  HVCable55localRot->rotateY(pi/2);  
  G4AffineTransform *HVCable55affine = new G4AffineTransform(HVCable55localRot,*HVCable55localPos);
  *HVCable55affine*=*assemAffine;  
  G4ThreeVector *HVCable55globalPos = new G4ThreeVector(HVCable55affine->NetTranslation());
  G4RotationMatrix *HVCable55globalRot= new G4RotationMatrix(HVCable55affine->NetRotation());
  fHVCable55Ptr->Place(HVCable55globalPos, HVCable55globalRot, motherLogical);
  delete HVCable55localPos;
  delete HVCable55localRot;
  delete HVCable55affine; 

  G4ThreeVector *HVCable61localPos = new G4ThreeVector(0*25.4*mm, -3.83*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *HVCable61localRot = new G4RotationMatrix();
  HVCable61localRot->rotateZ(-18.91*deg);
  HVCable61localRot->rotateY(pi/2);  
  G4AffineTransform *HVCable61affine = new G4AffineTransform(HVCable61localRot,*HVCable61localPos);
  *HVCable61affine*=*assemAffine;  
  G4ThreeVector *HVCable61globalPos = new G4ThreeVector(HVCable61affine->NetTranslation());
  G4RotationMatrix *HVCable61globalRot= new G4RotationMatrix(HVCable61affine->NetRotation());
  fHVCable61Ptr->Place(HVCable61globalPos, HVCable61globalRot, motherLogical);
  delete HVCable61localPos;
  delete HVCable61localRot;
  delete HVCable61affine; 

  G4ThreeVector *HVCable62localPos = new G4ThreeVector(0*25.4*mm, -3.83*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *HVCable62localRot = new G4RotationMatrix();
  HVCable62localRot->rotateZ(-18.91*deg);
  HVCable62localRot->rotateY(pi/2);  
  G4AffineTransform *HVCable62affine = new G4AffineTransform(HVCable62localRot,*HVCable62localPos);
  *HVCable62affine*=*assemAffine;  
  G4ThreeVector *HVCable62globalPos = new G4ThreeVector(HVCable62affine->NetTranslation());
  G4RotationMatrix *HVCable62globalRot= new G4RotationMatrix(HVCable62affine->NetRotation());
  fHVCable62Ptr->Place(HVCable62globalPos, HVCable62globalRot, motherLogical);
  delete HVCable62localPos;
  delete HVCable62localRot;
  delete HVCable62affine; 

  G4ThreeVector *HVCable63localPos = new G4ThreeVector(0*25.4*mm, -3.83*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *HVCable63localRot = new G4RotationMatrix();
  HVCable63localRot->rotateZ(-18.91*deg);
  HVCable63localRot->rotateY(pi/2);  
  G4AffineTransform *HVCable63affine = new G4AffineTransform(HVCable63localRot,*HVCable63localPos);
  *HVCable63affine*=*assemAffine;  
  G4ThreeVector *HVCable63globalPos = new G4ThreeVector(HVCable63affine->NetTranslation());
  G4RotationMatrix *HVCable63globalRot= new G4RotationMatrix(HVCable63affine->NetRotation());
  fHVCable63Ptr->Place(HVCable63globalPos, HVCable63globalRot, motherLogical);
  delete HVCable63localPos;
  delete HVCable63localRot;
  delete HVCable63affine; 

  G4ThreeVector *HVCable64localPos = new G4ThreeVector(0*25.4*mm, -3.83*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *HVCable64localRot = new G4RotationMatrix();
  HVCable64localRot->rotateZ(-18.91*deg);
  HVCable64localRot->rotateY(pi/2);  
  G4AffineTransform *HVCable64affine = new G4AffineTransform(HVCable64localRot,*HVCable64localPos);
  *HVCable64affine*=*assemAffine;  
  G4ThreeVector *HVCable64globalPos = new G4ThreeVector(HVCable64affine->NetTranslation());
  G4RotationMatrix *HVCable64globalRot= new G4RotationMatrix(HVCable64affine->NetRotation());
  fHVCable64Ptr->Place(HVCable64globalPos, HVCable64globalRot, motherLogical);
  delete HVCable64localPos;
  delete HVCable64localRot;
  delete HVCable64affine; 

  G4ThreeVector *HVCable65localPos = new G4ThreeVector(0*25.4*mm, -3.83*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *HVCable65localRot = new G4RotationMatrix();
  HVCable65localRot->rotateZ(-18.91*deg);
  HVCable65localRot->rotateY(pi/2);  
  G4AffineTransform *HVCable65affine = new G4AffineTransform(HVCable65localRot,*HVCable65localPos);
  *HVCable65affine*=*assemAffine;  
  G4ThreeVector *HVCable65globalPos = new G4ThreeVector(HVCable65affine->NetTranslation());
  G4RotationMatrix *HVCable65globalRot= new G4RotationMatrix(HVCable65affine->NetRotation());
  fHVCable65Ptr->Place(HVCable65globalPos, HVCable65globalRot, motherLogical);
  delete HVCable65localPos;
  delete HVCable65localRot;
  delete HVCable65affine; 

  G4ThreeVector *HVCable71localPos = new G4ThreeVector(3.23*25.4*mm, -1.96*25.4*mm, 0.6*25.4*mm);
  G4RotationMatrix *HVCable71localRot = new G4RotationMatrix();
  HVCable71localRot->rotateZ(-8.51*deg);
  HVCable71localRot->rotateY(pi/2);  
  G4AffineTransform *HVCable71affine = new G4AffineTransform(HVCable71localRot,*HVCable71localPos);
  *HVCable71affine*=*assemAffine;  
  G4ThreeVector *HVCable71globalPos = new G4ThreeVector(HVCable71affine->NetTranslation());
  G4RotationMatrix *HVCable71globalRot= new G4RotationMatrix(HVCable71affine->NetRotation());
  fHVCable71Ptr->Place(HVCable71globalPos, HVCable71globalRot, motherLogical);
  delete HVCable71localPos;
  delete HVCable71localRot;
  delete HVCable71affine; 

  G4ThreeVector *HVCable72localPos = new G4ThreeVector(3.23*25.4*mm, -1.96*25.4*mm, 0.7*25.4*mm);
  G4RotationMatrix *HVCable72localRot = new G4RotationMatrix();
  HVCable72localRot->rotateZ(-8.51*deg);
  HVCable72localRot->rotateY(pi/2);  
  G4AffineTransform *HVCable72affine = new G4AffineTransform(HVCable72localRot,*HVCable72localPos);
  *HVCable72affine*=*assemAffine;  
  G4ThreeVector *HVCable72globalPos = new G4ThreeVector(HVCable72affine->NetTranslation());
  G4RotationMatrix *HVCable72globalRot= new G4RotationMatrix(HVCable72affine->NetRotation());
  fHVCable72Ptr->Place(HVCable72globalPos, HVCable72globalRot, motherLogical);
  delete HVCable72localPos;
  delete HVCable72localRot;
  delete HVCable72affine; 

  G4ThreeVector *HVCable73localPos = new G4ThreeVector(3.23*25.4*mm, -1.96*25.4*mm, 0.8*25.4*mm);
  G4RotationMatrix *HVCable73localRot = new G4RotationMatrix();
  HVCable73localRot->rotateZ(-8.51*deg);
  HVCable73localRot->rotateY(pi/2);  
  G4AffineTransform *HVCable73affine = new G4AffineTransform(HVCable73localRot,*HVCable73localPos);
  *HVCable73affine*=*assemAffine;  
  G4ThreeVector *HVCable73globalPos = new G4ThreeVector(HVCable73affine->NetTranslation());
  G4RotationMatrix *HVCable73globalRot= new G4RotationMatrix(HVCable73affine->NetRotation());
  fHVCable73Ptr->Place(HVCable73globalPos, HVCable73globalRot, motherLogical);
  delete HVCable73localPos;
  delete HVCable73localRot;
  delete HVCable73affine; 

  G4ThreeVector *HVCable74localPos = new G4ThreeVector(3.23*25.4*mm, -1.96*25.4*mm, 0.9*25.4*mm);
  G4RotationMatrix *HVCable74localRot = new G4RotationMatrix();
  HVCable74localRot->rotateZ(-8.51*deg);
  HVCable74localRot->rotateY(pi/2);  
  G4AffineTransform *HVCable74affine = new G4AffineTransform(HVCable74localRot,*HVCable74localPos);
  *HVCable74affine*=*assemAffine;  
  G4ThreeVector *HVCable74globalPos = new G4ThreeVector(HVCable74affine->NetTranslation());
  G4RotationMatrix *HVCable74globalRot= new G4RotationMatrix(HVCable74affine->NetRotation());
  fHVCable74Ptr->Place(HVCable74globalPos, HVCable74globalRot, motherLogical);
  delete HVCable74localPos;
  delete HVCable74localRot;
  delete HVCable74affine; 

  G4ThreeVector *HVCable75localPos = new G4ThreeVector(3.23*25.4*mm, -1.96*25.4*mm, 1.0*25.4*mm);
  G4RotationMatrix *HVCable75localRot = new G4RotationMatrix();
  HVCable75localRot->rotateZ(-8.51*deg);
  HVCable75localRot->rotateY(pi/2);  
  G4AffineTransform *HVCable75affine = new G4AffineTransform(HVCable75localRot,*HVCable75localPos);
  *HVCable75affine*=*assemAffine;  
  G4ThreeVector *HVCable75globalPos = new G4ThreeVector(HVCable75affine->NetTranslation());
  G4RotationMatrix *HVCable75globalRot= new G4RotationMatrix(HVCable75affine->NetRotation());
  fHVCable75Ptr->Place(HVCable75globalPos, HVCable75globalRot, motherLogical);
  delete HVCable75localPos;
  delete HVCable75localRot;
  delete HVCable75affine; 



  delete assemAffine;
}
