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
 * $Id: MGOutputGeneralSurfaceSampler.hh,v 1.3 2008-05-19 20:09:46 rojo Exp $
 *      
 * CLASS DECLARATION:  MGOutputGeneralSurfaceSampler.hh
 *
 *
 * DESCRIPTION: 
 *
 * Class for output root file of random coordinates for general surface sampler
 *
 * AUTHOR: R. Johnson
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 * 11/22/2011 Include a messenger that requires the surface to be ONLY between two of the selected volumes. Bjoern Lehnert
 */

#ifndef _GeneralSurfaceSamplerOUTPUTROOT_HH
#define _GeneralSurfaceSamplerOUTPUTROOT_HH

#include <map>
#include <vector>

#include "G4ThreeVector.hh"

#include "io/MGOutputRoot.hh"

class MGOutputGeneralSurfaceSamplerMessenger;

class MGOutputGeneralSurfaceSampler: public MGOutputRoot
{
  public:
    MGOutputGeneralSurfaceSampler();
    ~MGOutputGeneralSurfaceSampler();

    void BeginOfRunAction();
    void DefineSchema();
    void BeginOfEventAction(const G4Event *event);
    void RootSteppingAction(const G4Step *step);
    void EndOfEventAction(const G4Event *event);
    void EndOfRunAction();

    void AddVolume(G4String aVolName);
    void RemoveVolume(G4String aVolName);
    void SetOnlyBetweenSelectedVolumes(G4bool aBool);
    void SetMaxIntersections(G4int Nmax){fNmax = Nmax;} 
    //void GetPreviousProcesses(vector<const G4VProcess*> &) const; 

  protected:
    void DumpNotFound(G4String aVolName);
    bool MatchesWildcard(G4String wildcard, G4String);

  protected:
    std::map<const G4VPhysicalVolume* , G4bool> fInterestingVolMap;
    std::vector<G4ThreeVector> fCoordVector;
    std::vector<G4VPhysicalVolume*> fPostVolumeVector;
    std::vector<G4VPhysicalVolume*> fPreVolumeVector;
    G4int fNmax;
    G4double fX_cm;
    G4double fY_cm;
    G4double fZ_cm;
    G4bool fOnlyBetweenSelectedVolumes;

    MGOutputGeneralSurfaceSamplerMessenger *fMessenger;
};
  
#endif
