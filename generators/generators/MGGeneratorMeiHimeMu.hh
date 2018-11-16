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
 * $Id: MGGeneratorMeiHimeMu.hh,v 1.6 2007-03-16 22:01:25 alexis3 Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorMeiHimeMu.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: This generator is based on Mei and Hime's paper on muons and
 * muon-induced neutrons.  It has the option of generating either just muons, or
 * just the muon-induced neutons described by equations in section C of the
 * paper.  See Physical Review D 73 053004 (2006) for further details.
 * 
 * Converted from LUXSimGeneratorMASN by the LUX collaboration (thank you guys).
 * 
 *
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef MGGeneratorMeiHimMu_HH
#define MGGeneratorMeiHimMu_HH 

#include "G4VPrimaryGenerator.hh"
#include "G4UImessenger.hh"

class G4Event;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;

class MGGeneratorMeiHimeMu : public G4VPrimaryGenerator, public G4UImessenger
{
  public:
    MGGeneratorMeiHimeMu();
    ~MGGeneratorMeiHimeMu();

    void SetVerticalDepth_kmwe(G4double vertDepth) { fVertDepth = vertDepth; }
    void SetOrigin(const G4ThreeVector& origin) { fOrigin = origin; }
    void SetMaxImpactParameter(G4double b) { fMaxImpactPar = b; }

    void SetNewValue(G4UIcommand* command, G4String newValues);

    void GeneratePrimaryVertex(G4Event *event);

  private:
    enum EMeiHimMuConsts { kN = 1001 };
    G4double SampleObservable(G4double vals[kN], G4double cdf[kN]);
    G4double SampleMuonEnergy() { return SampleObservable(fMuonEnergy, fMuonEnergyCDF); }
    G4double SampleMuonAngle()  { return SampleObservable(fMuonAngle,  fMuonAngleCDF); }

    G4double fMuonAngleCDF[kN];
    G4double fMuonAngle[kN];
    G4double fMuonEnergyCDF[kN];
    G4double fMuonEnergy[kN];

    G4double fVertDepth;
    G4ThreeVector fOrigin;
    G4double fMaxImpactPar;

    G4UIdirectory* fDirectory;
    G4UIcmdWithADouble* fVertDepthCmd;
    G4UIcmdWith3VectorAndUnit* fOriginCmd;
    G4UIcmdWithADoubleAndUnit* fMaxBCmd;
};

#endif
