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
* $Id: MGGeneratorBACoNSurfaceMuonsMessenger.hh,v 1.0 2009/5/17
*      
* CLASS DECLARATION:  MGGeneratorBACoNSurfaceMuonsMessenger.hh
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

#ifndef _MGGENERATORBACONSURFACEMUONMESSENGER_HH
#define _MGGENERATORBACONSURFACEMUONMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADouble.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWithAString;
class MGGeneratorBACoNSurfaceMuons;

class MGGeneratorBACoNSurfaceMuonsMessenger : public G4UImessenger
{
public:
	MGGeneratorBACoNSurfaceMuonsMessenger(MGGeneratorBACoNSurfaceMuons *generator);
	MGGeneratorBACoNSurfaceMuonsMessenger(const MGGeneratorBACoNSurfaceMuonsMessenger &);
	~MGGeneratorBACoNSurfaceMuonsMessenger();

	void SetNewValue(G4UIcommand *cmd, G4String newValue);

protected:

private:
	MGGeneratorBACoNSurfaceMuons	*fLiquidMuonGenerator;
	G4UIdirectory	*fLiquidMuonDirectory;
  G4UIcmdWithADoubleAndUnit *fLiquidMuonSetRadius;
  G4UIcmdWithADoubleAndUnit *fLiquidMuonSetRadiusMin;
  G4UIcmdWithADoubleAndUnit *fLiquidMuonSetHeight;
  G4UIcmdWith3VectorAndUnit *fLiquidMuonSetCenterVector;
  G4UIcmdWithAString *fLiquidMuonSetParticle;
  G4UIcmdWithADoubleAndUnit *fLiquidMuonSetEnergy;
  G4UIcmdWithADoubleAndUnit *fLiquidMuonSetBinWidth;
  G4UIcmdWithADouble        *fLiquidMuonSetNParticles;
	//G4UIcmdWithAString	*fSetSurfaceVolumeCmd;

};
#endif

