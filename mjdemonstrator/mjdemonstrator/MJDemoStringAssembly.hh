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
 * CLASS DECLARATION:  MJDemoStringAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The assembly consisting of a string of 77mm crystal assemblies.
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
 * FIRST SUBMISSION: Jul 22, 2010
 * 
 * REVISION:
 * 
 * 07-22-2010, Created, M. Green
 * 10-29-2013, Added flexible string functionality
 * 11-14-2013, Updated to reflect new adapter bolts and nuts, J. MacMullin
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOSTRINGASSEMBLY_HH
#define _MJDEMOSTRINGASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoStringAdapterPlate;
class MJDemo1032CapturedBolt;
class MJDemo1032Nut;
class MJDemoSplitNutTieRod;
class MJDemoStringTieRod;
class MJDemoBottomNutTieRod;
//class MJDemoCopperSpacer;
class MJDemoCrystalAssembly;
class MJDemoCableAssembly;
class MJDemoHVCableAssembly;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoStringAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoStringAssembly(G4String, G4String, size_t crystalNum = 5);
  MJDemoStringAssembly(const MJDemoStringAssembly &);
  ~MJDemoStringAssembly();

  size_t GetNumberOfCrystals() {return fCrystalNum;}
  void SetNumberOfCrystals(size_t crystalNum);

  MJDemoStringAdapterPlate* GetAdapterPlate() {return fAdapterPlatePtr;}
  MJDemo1032CapturedBolt* GetAdapterBolt1() {return fAdapterBolt1Ptr;}
  MJDemo1032CapturedBolt* GetAdapterBolt2() {return fAdapterBolt2Ptr;}
  MJDemo1032CapturedBolt* GetAdapterBolt3() {return fAdapterBolt3Ptr;}
  MJDemo1032Nut* GetAdapterNut1() {return fAdapterNut1Ptr;}
  MJDemo1032Nut* GetAdapterNut2() {return fAdapterNut2Ptr;}
  MJDemo1032Nut* GetAdapterNut3() {return fAdapterNut3Ptr;}
//  MJDemoCopperSpacer* GetCopperSpacer1() {return fCopperSpacer1Ptr;}
//  MJDemoCopperSpacer* GetCopperSpacer2() {return fCopperSpacer2Ptr;}
//  MJDemoCopperSpacer* GetCopperSpacer3() {return fCopperSpacer3Ptr;}
  MJDemoSplitNutTieRod* GetSplitNut1()    {return fSplitNut1Ptr;}
  MJDemoSplitNutTieRod* GetSplitNut2()    {return fSplitNut2Ptr;}
  MJDemoSplitNutTieRod* GetSplitNut3()    {return fSplitNut3Ptr;}
  MJDemoStringTieRod* GetTieRod1()      {return fTieRod1Ptr;}
  MJDemoStringTieRod* GetTieRod2()      {return fTieRod2Ptr;}
  MJDemoStringTieRod* GetTieRod3()      {return fTieRod3Ptr;}
  MJDemoBottomNutTieRod* GetBottomNut1()   {return fBottomNut1Ptr;}
  MJDemoBottomNutTieRod* GetBottomNut2()   {return fBottomNut2Ptr;}
  MJDemoBottomNutTieRod* GetBottomNut3()   {return fBottomNut3Ptr;}

  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
private:
  size_t fCrystalNum;
  MJDemoStringAdapterPlate* fAdapterPlatePtr;
  MJDemo1032CapturedBolt* fAdapterBolt1Ptr;
  MJDemo1032CapturedBolt* fAdapterBolt2Ptr;
  MJDemo1032CapturedBolt* fAdapterBolt3Ptr;
  MJDemo1032Nut* fAdapterNut1Ptr;
  MJDemo1032Nut* fAdapterNut2Ptr;
  MJDemo1032Nut* fAdapterNut3Ptr;
  MJDemoSplitNutTieRod* fSplitNut1Ptr;
  MJDemoSplitNutTieRod* fSplitNut2Ptr;
  MJDemoSplitNutTieRod* fSplitNut3Ptr;
//  MJDemoCopperSpacer* fCopperSpacer1Ptr;
//  MJDemoCopperSpacer* fCopperSpacer2Ptr;
//  MJDemoCopperSpacer* fCopperSpacer3Ptr;
  MJDemoStringTieRod* fTieRod1Ptr;
  MJDemoStringTieRod* fTieRod2Ptr;
  MJDemoStringTieRod* fTieRod3Ptr;
  MJDemoBottomNutTieRod* fBottomNut1Ptr;
  MJDemoBottomNutTieRod* fBottomNut2Ptr;
  MJDemoBottomNutTieRod* fBottomNut3Ptr;
  MJDemoCrystalAssembly** fCrystalAssemblyPtr;
};
//
#endif
