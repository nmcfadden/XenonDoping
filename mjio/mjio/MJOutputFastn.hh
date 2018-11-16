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
 * $Id: MJOutputSolidBlock.hh,v 1.2 2005-02-03 21:38:49 jasondet Exp $
 *      
 * CLASS DECLARATION:  MJOutputFastn.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Output class for fast neutrons generated from muons.  For now, for use with solidBlock geometry.
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *   Until MaGe officially runs with Geant4.9.5, some of the functionality of
 *   this output class is limited.  Specifically, the GetTargetNucleus() function
 *   in G4HadronicProcess is non-existent in previous versions of Geant4.  If you
 *   are running with Geant4.9.5, you can uncomment the sections noted below.  
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: Mary Kidd
 * CONTACT: mkidd@lanl.gov
 * FIRST SUBMISSION: 02-03-2012
 * 
 * REVISION:
 * 
 * 01-26-2012, Created, M. Kidd
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTFASTN_HH
#define _MJOUTPUTFASTN_HH

//---------------------------------------------------------------------------//

#include "TObject.h"

#include "G4ThreeVector.hh"

#include "io/MGOutputRoot.hh"


//---------------------------------------------------------------------------//

class TH1D;
class G4DynamicParticle;
class G4HadronicProcess;

class G4UserTrackingAction;

class MJOutputFastn : public MGOutputRoot
{
public:

  //default constructor
  MJOutputFastn(G4bool isMother);

  //copy constructor
  MJOutputFastn(const MJOutputFastn &);

  //destructor
  ~MJOutputFastn();

  //public interface
  void BeginOfEventAction(const G4Event *event);
  void BeginOfRunAction();
  void DefineSchema();
  void EndOfEventAction(const G4Event *event);
  void EndOfRunAction();
  void RootSteppingAction(const G4Step *step);

  G4int kMaxNeutrons;


  //protected members
protected:

    void Clear();
    void KillTrack(G4Track*);

  //private  members
private:

  G4String theProcess;	           // Creator process of neutron
  G4String theParentName;	   // Parent particle of neutron
  //G4HadronicProcess *activeProcess;
  std::vector<std::string> fTheProcessName; //vector to store the creator process of neutron in a branch
  std::vector<std::string> fTheParentName; //vector to store the parent particle of neutron in a branch
  Int_t    nStep;		//number of steps per event (just used to get info for the first step in each event)
  UInt_t   EventNumber;		//event number
  Int_t    fNNeutrons;		//keeps track of number of neutrons generated per event
  Float_t  PEnergy;		//energy of primary muon
  Float_t  PositionX;		//initial x position of primary muon
  Float_t  PositionY;		//initial y position of primary muon
  Float_t  PositionZ;		//initial z position of primary muon
  Float_t  *NPositionX0;	//initial x position of created neutron
  Float_t  *NPositionY0;	//initial y position of created neutron
  Float_t  *NPositionZ0;	//initial z position of created neutron
  Float_t  *nEnergy;		//neutron energy
  Float_t  *totalDistance;	//total distance of created neutron from primary muon
  Float_t  *orthDistance;	//orthogonal distance of created neutron from primary muon track
//  Int_t *targetNucN;		//N of target nucleus which created neutron  ***needs geant4.9.5***
//  Int_t *targetNucZ;		//Z of target nucleus which created neutron  ***needs geant4.9.5***
  Int_t *theParent;		//the parent particle that generated the neutron
  Int_t *theParentID;		//the parent track ID of the particle which generated the neutron
  Int_t *theNeutronTrackID;	//the neutron track ID  (should be used to prevent double counting of neutrons)
  Float_t *nEdep;		//neutron energy deposited in step

  Float_t  Edep; 		//total energy deposited in each step

};
#endif
