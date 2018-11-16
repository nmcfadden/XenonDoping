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
 * CLASS DECLARATION:  MJDemoThermosyphonSupportThickAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The copper thermosyphon shunt assembly
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

#ifndef _MJDEMOTHERMOSYPHONCABLETHERMALSHUNTASSEMBLY_HH
#define _MJDEMOTHERMOSYPHONCABLETHERMALSHUNTASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoThermosyphonCableThermalShuntHalf;
class MJDemoThermosyphonCableThermalShuntClip;
class MJDemo832Screw;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoThermosyphonCableThermalShuntAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoThermosyphonCableThermalShuntAssembly(G4String, G4String);
  MJDemoThermosyphonCableThermalShuntAssembly(const MJDemoThermosyphonCableThermalShuntAssembly &);
  ~MJDemoThermosyphonCableThermalShuntAssembly();

  MJDemoThermosyphonCableThermalShuntHalf* GetFemaleHalf() {return fFemaleHalfPtr;}
  MJDemoThermosyphonCableThermalShuntHalf* GetMaleHalf() {return fMaleHalfPtr;}
  MJDemoThermosyphonCableThermalShuntClip* GetClip01() {return fClip01Ptr;}
  MJDemoThermosyphonCableThermalShuntClip* GetClip02() {return fClip02Ptr;}
  MJDemo832Screw* GetScrew01() {return fScrew01Ptr;}
  MJDemo832Screw* GetScrew02() {return fScrew02Ptr;}
  MJDemo832Screw* GetScrew03() {return fScrew03Ptr;}
  MJDemo832Screw* GetScrew04() {return fScrew04Ptr;}
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);  
  
private:
  MJDemoThermosyphonCableThermalShuntHalf* fFemaleHalfPtr;
  MJDemoThermosyphonCableThermalShuntHalf* fMaleHalfPtr;
  MJDemoThermosyphonCableThermalShuntClip* fClip01Ptr;
  MJDemoThermosyphonCableThermalShuntClip* fClip02Ptr;
  MJDemo832Screw* fScrew01Ptr;
  MJDemo832Screw* fScrew02Ptr;
  MJDemo832Screw* fScrew03Ptr;
  MJDemo832Screw* fScrew04Ptr;
};
//
#endif
