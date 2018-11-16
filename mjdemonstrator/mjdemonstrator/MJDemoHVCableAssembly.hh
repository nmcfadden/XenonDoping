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
 * CLASS DECLARATION:  MJDemoHVCableAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The variable-length HV Cable assembly
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
 * 10-21-2010, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOHVCABLEASSEMBLY_HH
#define _MJDEMOHVCABLEASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoHVCableConductor;
class MJDemoHVCableInsulation;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoHVCableAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoHVCableAssembly(G4String, G4String, G4double);
  MJDemoHVCableAssembly(const MJDemoHVCableAssembly &);
  ~MJDemoHVCableAssembly();

  MJDemoHVCableConductor* GetWire() {return fWirePtr;}
  MJDemoHVCableInsulation* GetInsulation()  {return fInsulationPtr;}

  void SetLength(G4double length)		{fCableLength = length;}
  G4double GetLength()					{return fCableLength;}  

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);  
  
private:
  MJDemoHVCableConductor* fWirePtr;
  MJDemoHVCableInsulation* fInsulationPtr;

  G4double fCableLength;
};
//
#endif
