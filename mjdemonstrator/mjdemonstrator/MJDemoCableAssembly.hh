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
 * CLASS DECLARATION:  MJDemoCableAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The variable-length signal cable assembly
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

#ifndef _MJDEMOCABLEASSEMBLY_HH
#define _MJDEMOCABLEASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoCableConductor;
class MJDemoCableInsulation;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoCableAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoCableAssembly(G4String, G4String, G4double);
  MJDemoCableAssembly(const MJDemoCableAssembly &);
  ~MJDemoCableAssembly();

  MJDemoCableConductor* GetWire1() {return fWire1Ptr;}
  MJDemoCableConductor* GetWire2() {return fWire2Ptr;}
  MJDemoCableConductor* GetWire3() {return fWire3Ptr;}
  MJDemoCableConductor* GetWire4() {return fWire4Ptr;}
  MJDemoCableInsulation* GetInsulation()  {return fInsulationPtr;}

  void SetLength(G4double length)		{fCableLength = length;}
  G4double GetLength()					{return fCableLength;}  

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);  
  
private:
  MJDemoCableConductor* fWire1Ptr;
  MJDemoCableConductor* fWire2Ptr;
  MJDemoCableConductor* fWire3Ptr;
  MJDemoCableConductor* fWire4Ptr;
  MJDemoCableInsulation* fInsulationPtr;

  G4double fCableLength;
};
//
#endif
