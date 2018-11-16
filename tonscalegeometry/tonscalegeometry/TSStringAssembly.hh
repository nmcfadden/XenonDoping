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
 * CLASS DECLARATION:  TSStringAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *This is a generalized "string assembly", inspired by the MJDemonstrator classes.
 *One can instantiate and place a string with a variable number of detector assemblies
 *which can either be from the MJDemonstrator geometry or TSCrystalAssembly
 *The crystal assemblies contain the crystal + mount.
 *The string assembly contains everything up to the cold plate (including adapter plate)
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
 * AUTHOR: Ryan Martin
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 * 
 * 06-13-2011, Created, R. Martin
 */
// --------------------------------------------------------------------------//

#ifndef _TSStringASSEMBLY_HH
#define _TSStringASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

#include "tonscalegeometry/TSCrystalAssembly.hh"

class G4LogicalVolume;
class MJDemoStringAdapterPlate;
class MJDemoCrystalAssembly;
class TSCrystalAssembly;

//---------------------------------------------------------------------------//

class TSStringAssembly : public MJVDemoAssembly
{
  public:
    TSStringAssembly(G4String, G4String);
    TSStringAssembly(const TSStringAssembly &); // copy constructor
    ~TSStringAssembly();
  
    enum DetectorAssemblyType { kMJDemonstrator, kTS };
  
    void Defaults();
    void Initialize();
    void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation, G4LogicalVolume* motherLogical);
  	
    void SetnCrystals(G4int arg) { fnCrystals = arg; }
    void SetCrystalAssemblyType(G4String arg){
      if(arg.find("MJDemonstrator") != arg.npos) fCrystalAssemblyType = kMJDemonstrator;
      else if(arg.find("TS") != arg.npos) fCrystalAssemblyType = kTS;
    }
    void SetCrystalDiameter(G4double arg){ fCrystalDiameter = arg;}
    void SetCrystalHeight(G4double arg){ fCrystalHeight = arg;}
  	
    G4double GetStringLength(){
      if(fCrystalAssemblyType == kMJDemonstrator) return (1.8792 + fnCrystals * 2.057 + 0.5289)*25.4*CLHEP::mm;
      else if(fCrystalAssemblyType == kTS){
        return fnCrystals*(fTSCrystalAssemblies[0]->GetAssemblyLength()+fSpaceBetweenTSCrystalAssemblies)+fColdPlateStandoffLength+2*fEpsilon;
      }
      else return 0.;
    }
    G4double GetStringRadius(){		
      if(fCrystalAssemblyType == kMJDemonstrator) return 3.2*25.4*CLHEP::mm;
      else if(fCrystalAssemblyType == kTS) return fTSCrystalAssemblies[0]->GetAssemblyRadius();
      else return 0.;
    }
  
    TSCrystalAssembly* GetTSCrystalAssembly(G4int arg){ return fTSCrystalAssemblies[arg]; }

  private:
    G4int fnCrystals;	
    DetectorAssemblyType fCrystalAssemblyType;
    G4double fCrystalDiameter;
    G4double fCrystalHeight;	
  	
    MJDemoStringAdapterPlate* fMJAdapterPlatePtr;
    MJDemoCrystalAssembly** fMJCrystalAssemblies;	
  
    G4double fSpaceBetweenTSCrystalAssemblies;
    TSCrystalAssembly** fTSCrystalAssemblies;
  
    G4double fColdPlateStandoffLength; //little rods to stand the string away from cold plate (instead of adapter plate)
    G4double fColdPlateStandoffRadius;
    G4double fEpsilon;
};

#endif
