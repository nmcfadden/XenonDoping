//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
/**                                                            
* $Id: MGGeneratorLGNDLiquidArgonMessenger.hh,v 1.0 2009/5/17
*      
* CLASS DECLARATION:  MGGeneratorLGNDLiquidArgonMessenger.hh
*
*---------------------------------------------------------------------------//
*/
// 
// --------------------------------------------------------------------------//
/** 
* AUTHOR: Neil McFadden
* CONTACT: nmcfadde@unm.edu
* FIRST SUBMISSION:
* 
* REVISION:
* 
* 
*/
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORLGNDLIQUIDARGONMESSENGER_HH
#define _MGGENERATORLGNDLIQUIDARGONMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADouble.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWithAString;
class MGGeneratorLGNDLiquidArgon;

class MGGeneratorLGNDLiquidArgonMessenger : public G4UImessenger
{
public:
	MGGeneratorLGNDLiquidArgonMessenger(MGGeneratorLGNDLiquidArgon *generator);
	MGGeneratorLGNDLiquidArgonMessenger(const MGGeneratorLGNDLiquidArgonMessenger &);
	~MGGeneratorLGNDLiquidArgonMessenger();

	void SetNewValue(G4UIcommand *cmd, G4String newValue);

protected:

private:
	MGGeneratorLGNDLiquidArgon	*fLiquidArgonGenerator;
	G4UIdirectory	*fLiquidArgonDirectory;
  G4UIcmdWithADoubleAndUnit *fLiquidArgonSetRadius;
  G4UIcmdWithADoubleAndUnit *fLiquidArgonSetRadiusMin;
  G4UIcmdWithADoubleAndUnit *fLiquidArgonSetHeight;
  G4UIcmdWith3VectorAndUnit *fLiquidArgonSetCenterVector;
  G4UIcmdWithAString *fLiquidArgonSetParticle;
  G4UIcmdWithADoubleAndUnit *fLiquidArgonSetEnergy;
  G4UIcmdWithADoubleAndUnit *fLiquidArgonSetBinWidth;
  G4UIcmdWithADouble        *fLiquidArgonSetNParticles;
	//G4UIcmdWithAString	*fSetSurfaceVolumeCmd;

};
#endif

