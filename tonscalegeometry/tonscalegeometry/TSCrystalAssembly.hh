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
 * $Id: MJheaderTemplate.hh,v 1.3 2005-02-15 23:26:55 akbarm Exp $
 *      
 * CLASS DECLARATION:  TSModule.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Tonne scale simplified crystal assembly, includes a crystal, an HV ring, a
 * simple Mercedes and rods for the mount (can be hollow).
 *The assembly can handle variable sized crystals
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ryan Martin
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 * 06-13-2011, Created, R. Martin
 *
 */
//---------------------------------------------------------------------------//
//


#ifndef _TSCrystalAssembly_HH
#define _TSCrystalAssembly_HH
 
#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"


//---------------------------------------------------------------------------//
#include <string>
#include <vector>
#include "io/MGLogger.hh" 

//---------------------------------------------------------------------------//
class G4LogicalVolume;

class TSCrystalAssembly: public MJVDemoAssembly
{
  public:
    TSCrystalAssembly(G4String arg="");
    virtual ~TSCrystalAssembly() {}
  
    void Initialize();
    void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,G4LogicalVolume *motherlogical);
  
    void SetCrystalDiameter(G4double arg){ fCrystalRadius = arg/2.; }
    void SetCrystalHeight(G4double arg){ fCrystalHeight = arg; }
  
    G4double GetRodPlacememtRadius(){ return fRodOuterRadius+fRodToCrystalRadialClearance+fCrystalRadius; }
    G4double GetAssemblyRadius(){ return fCrystalRadius+fRodToCrystalRadialClearance+2.*fRodOuterRadius; }
    G4double GetAssemblyLength(){ return fCrystalHeight+fHVRingThickness+fEpsilon+fPTFEBlockZ+fEpsilon+fMercedesThickness+fEpsilon; }
	
  private:
    G4double fCrystalRadius;
    G4double fCrystalHeight;

    G4double fEpsilon;
    
    G4double fRodOuterRadius;
    G4double fRodInnerRadius;
    G4double fRodHeight;
    G4double fRodToCrystalRadialClearance;

    G4double fHVRingInnerRadius;
    G4double fHVRingOuterRadius;
    G4double fHVRingThickness;

    G4double fPTFEBlockX;
    G4double fPTFEBlockY;	
    G4double fPTFEBlockZ;

    G4double fMercedesThickness;
    G4double fMercedesRadius;
    G4double fMercedesCutoutRadius;
};
#endif

