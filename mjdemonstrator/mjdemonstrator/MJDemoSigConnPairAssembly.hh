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
 * CLASS DECLARATION:  MJDemoSigConnPlugAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * The assembly consisting of a pair of signal plugs.
 * Male and Female plugs are modelled the same.
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
 * AUTHOR: Ian Guinn
 * CONTACT: iguinn@uw.edu
 * FIRST SUBMISSION: Jun 20, 2014
 * 
 * REVISION:
 * 
 * 06-20-2014, Created, I. Guinn
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOSIGCONNPAIRASSEMBLY_HH
#define _MJDEMOSIGCONNPAIRASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "G4GeometryTolerance.hh"

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"
#include "mjdemonstrator/MJDemoSigConnPlugAssembly.hh"

class G4LogicalVolume;
class MJDemoSigConnPlugAssembly;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoSigConnPairAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoSigConnPairAssembly(G4String, G4String);
  MJDemoSigConnPairAssembly(const MJDemoSigConnPairAssembly &);
  ~MJDemoSigConnPairAssembly();

  inline MJDemoSigConnPlugAssembly* GetPlug1() const {return fPlug1;}
  inline MJDemoSigConnPlugAssembly* GetPlug2() const {return fPlug2;}
  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemoSigConnPlugAssembly* fPlug1;
  MJDemoSigConnPlugAssembly* fPlug2;
};
//
#endif
