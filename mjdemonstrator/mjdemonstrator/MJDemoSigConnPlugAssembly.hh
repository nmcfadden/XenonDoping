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
 * The assembly consisting of a single signal plug.
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
 * FIRST SUBMISSION: Jun 4, 2014
 * 
 * REVISION:
 * 
 * 06-04-2014, Created, I. Guinn
 * 08-20-2014, Pins and Solder blobs are now represented by a single part
 *             rather than 8, I. Guinn
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOSIGCONNPLUGASSEMBLY_HH
#define _MJDEMOSIGCONNPLUGASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "G4GeometryTolerance.hh"

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"
#include "mjdemonstrator/MJDemoSigConnHousing.hh"
#include "mjdemonstrator/MJDemoSigConnPins.hh"
#include "mjdemonstrator/MJDemoSigConnSolder.hh"
#include "mjdemonstrator/MJDemoSigConnStrainRelief.hh"
#include "geometry/MGGeometryGlobals.hh"

class G4LogicalVolume;
class MJDemoSigConnHousing;
class MJDemoSigConnPins;
class MJDemoSigConnSolder;
class MJDemoSigConnStrainRelief;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoSigConnPlugAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoSigConnPlugAssembly(G4String, G4String);
  MJDemoSigConnPlugAssembly(const MJDemoSigConnPlugAssembly &);
  ~MJDemoSigConnPlugAssembly();

  inline MJDemoSigConnHousing* GetHousing() const {return fHousingPtr;}
  inline MJDemoSigConnPins* GetPins() const {return fPinsPtr;}
  inline MJDemoSigConnSolder* GetSolder() const {return fSolderPtr;}
  inline MJDemoSigConnStrainRelief* GetStrainRelief() const {return fStrRelPtr;}
  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemoSigConnHousing* fHousingPtr;
  MJDemoSigConnPins* fPinsPtr;
  MJDemoSigConnSolder* fSolderPtr;
  MJDemoSigConnStrainRelief* fStrRelPtr;

};
//
#endif
