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
 * $Id: MJOutputSLACBD.hh,v 1.5 2007-01-25 06:17:16 mgmarino Exp $
 *      
 * CLASS DECLARATION:  MJOutputSLACBD.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Root output class for spallation neutron studies in solid block.
 *
 * AUTHOR: M. Marino
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 * 08-2005: Created, M. Marino
 * 08-22-2005: Ported to MaGe, J. Detwiler
 *
 */

#ifndef _SLACBDOUTPUTROOT_HH
#define _SLACBDOUTPUTROOT_HH
#include <map>
#include <vector>
#include <set>
#include "io/MGOutputRoot.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VProcess.hh"
#include "TParameter.h"

using namespace std;
class MJOutputSLACBD: public MGOutputRoot
{
  public:
    MJOutputSLACBD(G4bool isMother);
    virtual ~MJOutputSLACBD();

    //inherited virtual functions.
    void BeginOfRunAction();
    void DefineSchema();
    void BeginOfEventAction(const G4Event *event);
    void RootSteppingAction(const G4Step *step);
    void EndOfEventAction(const G4Event *event);
    void EndOfRunAction();
    void WriteFile();
    void CloseFile();
    virtual void FillCorrectionMap(const G4Material*, G4double); 
    //void GetPreviousProcesses(vector<const G4VProcess*> &) const; 
  protected:
    virtual void Clear();
    virtual void GetData(const G4Step*, const G4VPhysicalVolume*);
    virtual void KillTrack(G4Track*);
    virtual void ReweightTheTrack(G4Track*);

  protected:
    //enum EConsts {kNumProcesses = 4};
    void FillPhysVolMap();
    G4double fX_cm;
    G4double fY_cm;
    G4double fZ_cm;
    G4double fE_MeV;
    G4double fXMomentumDir;
    G4double fYMomentumDir;
    G4double fZMomentumDir;
    G4int fSlabNumber;
    G4double fTime;
    G4double fParticleWeight;
    G4int fEventNumber;   
    G4int fTrackID;
    G4int fParentID;
    map<const G4VPhysicalVolume* , G4int> fPhysVolMap;
    map<const G4VPhysicalVolume* , G4int> :: const_iterator fTheEndOfPhysVolMap; 
    map<const G4VPhysicalVolume* , G4int> :: const_iterator fThePreVolIt; 
    map<const G4VPhysicalVolume* , G4int> :: const_iterator fThePostVolIt; 
    map<const G4Material*, G4double> fMaterialCorrectionMap;
    map<const G4Material* , G4double> :: const_iterator fMaterialCorrectIt; 
    G4bool fFirstTime;
    G4bool fIsForwardMoving;
    G4int fNumSlabs;
    G4int fSlabNumberDataLimit;
    G4double fYBlackHolePos;

    TParameter<long>* fParNumSlabs;
    TParameter<long>* fParEventSuccess;
    // allows us to keep track of previous processes.

};
  
#endif
