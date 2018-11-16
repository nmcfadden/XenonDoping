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
 *Tonne scale geometry module (cryostat with strings in it). This is the 'master
 *class' for building a module. Based on the size of the detectors in the array,
 *the configuration of the string, the number of detectors per string, it will build:
 *a coldplate, a thermal shield and a cryostat and returns the dimensions required
 *by TSMJDemModInTank to build a vacuum clerance around the cryostat.
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
 * 07-30-2012, Added 1HEXRing and 2HEXRing array configurations, K. Nguyen
 *
 */
//---------------------------------------------------------------------------//

#ifndef _TSModule_HH
#define _TSModule_HH

//---------------------------------------------------------------------------//

#include <string>
#include <vector>
//#include "tonscalegeometry/TSModuleMessenger.hh"
#include "tonscalegeometry/TSStringAssembly.hh"
#include "io/MGLogger.hh" 

//---------------------------------------------------------------------------//

class G4LogicalVolume;

class TSModule 
{
  public:
    TSModule(G4String arg = "DetectorArray_000");
    ~TSModule();
  
    enum TSModuleType { kMJDemonstrator, k1Ring, k2Ring, k3Ring, k4Ring, k1String, k2String, 
                        k1HEXRing, k2HEXRing}; // FIXME: Added by K. Nguyen

    void Initialize();
    void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,G4LogicalVolume *cryoclearancelogical);
    
    void SetModuleType(G4String arg){
    	//add more string configurations here
    	if(arg.find("MJDemonstrator") !=arg.npos) fModuleType = kMJDemonstrator;
    	else if(arg.find("1Ring") != arg.npos) fModuleType = k1Ring;
    	else if(arg.find("2Ring") != arg.npos) fModuleType = k2Ring;
    	else if(arg.find("3Ring") != arg.npos) fModuleType = k3Ring;
    	else if(arg.find("4Ring") != arg.npos) fModuleType = k4Ring;	
        else if(arg.find("1String") != arg.npos) fModuleType = k1String;	
    	else if(arg.find("2String") != arg.npos) fModuleType = k2String;		
    	else if(arg.find("1HEXRing") != arg.npos) fModuleType = k1HEXRing;  // FIXME: Added by K. Nguyen
    	else if(arg.find("2HEXRing") != arg.npos) fModuleType = k2HEXRing;  // FIXME: Added by K. Nguyen
    	else fModuleType = kMJDemonstrator;
    }
    void SetCrystalAssemblyType(G4String arg){ fCrystalAssemblyType = arg; }
    void SetnCrystalsPerString(G4int arg){ fnCrystalsPerString = arg; }
    void SetCryostatThickness(G4double arg){ fCryostatThickness = arg; }
    void SetCrystalDiameter(G4double arg){ fCrystalDiameter = arg; }
    void SetCrystalHeight(G4double arg){ fCrystalHeight = arg; }
    //void SetStringRadialClearance(G4double arg){ fStringRadialClearance = arg; } // FIXME: Added by K. Nguyen
    //void SetRadialSpacingBetweenStrings(G4double arg){ fRadialSpacingBetweenStrings = arg; } // FIXME: Added by K. Nguyen
    void SetStringRadialClearance(G4double arg){ 
      fStringRadialClearance = arg; 
      cout << "Set string radial clearance to: " << fStringRadialClearance << endl;
    } // FIXME: Added by K. Nguyen
    void SetRadialSpacingBetweenStrings(G4double arg){ 
      fRadialSpacingBetweenStrings = arg; 
      cout << "Set radial spacing between strings to: " << fRadialSpacingBetweenStrings << endl;
    } // FIXME: Added by K. Nguyen
  
    //G4double GetCryostatOuterRadius(){ return fCryostatInnerRadius+fCryostatThickness; }
    //G4double GetCryostatLengthWithLidAndBottom(){ return fCryostatLength+fCryostatLidThickness+fCryostatBottomThickness; }
    G4double GetCryostatRadialClearance(){ return fCryostatInnerRadius+fCryostatThickness+fCryostatClearanceDistance; }
    G4double GetCryostatLengthClearance(){ return fCryostatLength+fCryostatLidThickness+fCryostatBottomThickness+2*fCryostatClearanceDistance; }
    
  private:
    TSModuleType fModuleType;
    //TSModuleMessenger *fG4Messenger;
    G4String fName;
    
    G4int fnStrings;
    G4int fnCrystalsPerString;
    G4String fCrystalAssemblyType;
    G4double fCrystalDiameter;
    G4double fCrystalHeight;
    
    TSStringAssembly **fStrings;
    G4double fRadialSpacingBetweenStrings;
    G4double fStringArrayClearanceRadius;
    G4double fStringArrayClearanceLength;
    G4double fStringBottomToThermalClearance;
	
    G4double fEpsilon;

    G4double fStringLengthClearance;
    G4double fStringRadialClearance;
	
    G4double fThermalShieldInnerRadius;
    G4double fThermalShieldThickness;
    G4double fThermalShieldLength;
    G4double fThermalToCryostatClearance;

    G4double fThermalShieldBottomThickness;
    G4double fColdPlateThickness;
    G4double fColdPlateVerticalClearance;
    
    G4double fCryostatInnerRadius;
    G4double fCryostatThickness;
    G4double fCryostatLength;
    G4double fCryostatLidThickness;
    G4double fCryostatBottomThickness;
    G4double fCryostatClearanceDistance;
};
#endif

