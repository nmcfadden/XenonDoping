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
//                                                          
// $Id: MGImportanceAlgorithm.cc,v 1.0
//      
// CLASS IMPLEMENTATION:  MGImportanceAlgorithm.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: M. Marino
 * CONTACT: mgmarino@u.washington.edu
 * FIRST SUBMISSION: 4 Nov 2005
 *   This is a class primarily based upon G4's Importance Algorithm with a few 
 *   warning changes in order to provide more accurate warning info to the user. 
 *  
 * REVISION:
 *
 */
//---------------------------------------------------------------------------//
//


#include "G4Types.hh"
#include <sstream>
#include "Randomize.hh"
#include "io/MGLogger.hh"
#include "mjio/MJOutputSLACBD.hh"
#include "io/MGOutputRoot.hh"
#include "io/MGVOutputManager.hh"
#include "management/MGManagementEventAction.hh"
#include "G4UserEventAction.hh"
#include "geometry/MGImportanceAlgorithm.hh"

using namespace std;

MGImportanceAlgorithm::MGImportanceAlgorithm(): 
  fLowerBound(1e-3),
  fWarnedLesser(false), fWarnedGreater(false)
  
{}

MGImportanceAlgorithm::~MGImportanceAlgorithm()
{
  if(fWarnedGreater) {
    Warning("~MGImportanceAlgorithm: ipre_over_ipost > 4 seen");
  }
  if(fWarnedLesser) {
    ostringstream os;
    os << "~MGImportanceAlgorithm: ipre_over_ipost < "
      << fLowerBound << "seen" << '\0';
    Warning(os.str());
  }
}

G4Nsplit_Weight
MGImportanceAlgorithm::Calculate(G4double ipre,
				 G4double ipost,
                                 G4double init_w) const
{
  G4Nsplit_Weight nw = {0,0};
  if (ipost>0.){
    if (!(ipre>0.)){
      Error("MGImportanceAlgorithm::Calculate() - ipre==0.");
    }
    G4double ipre_over_ipost = ipre/ipost;
    if (ipre_over_ipost> 4 && !fWarnedGreater) {
      ostringstream os;
      os << "Calculate: ipre_over_ipost > 4: ipre_over_ipost = "
	 << ipre_over_ipost << '\0' << G4endl;
      Warning(os.str());
      fWarnedGreater = true;
      if (ipre_over_ipost<=0) {
	Error("MGImportanceAlgorithm::Calculate() - ipre_over_ipost<=0");
      }
    } else if (ipre_over_ipost < fLowerBound) {
      // this is to check if too many particles are being made at a boundary 
      ostringstream os;
      os << "Calculate: ipre_over_ipost < "<< fLowerBound 
	 << " : ipre_over_ipost = " << ipre_over_ipost 
	 << ".  Potential Memory Problem!"  << '\0';
      Warning(os.str());
      fWarnedLesser = true;
    }
    if (init_w<=0.) {
      Error("MGImportanceAlgorithm::Calculate() - iniitweight<= 0. found");
    }

    // default geometrical splitting 
    // in integer mode 
    // for ipre_over_ipost <= 1
    G4double inv = ipost / ipre;
    nw.fN = static_cast<G4int>(inv);
    nw.fW = init_w * ipre_over_ipost;
    
    // geometrical splitting for double mode
    if (ipre_over_ipost<1) {
      if ( static_cast<G4double>(nw.fN) != inv) {
	// double mode
	// probability p for splitting into n+1 tracks
	G4double p = inv - nw.fN;
	// get a random number out of [0,1)
	G4double r = G4UniformRand();
	if (r<p) {
	  nw.fN++;
	} 
      }  
    }
    // ipre_over_ipost > 1
    //  russian roulett
    else if (ipre_over_ipost>1) {
      // probabiity for killing track
      G4double p = 1-inv;
      // get a random number out of [0,1)
      G4double r = G4UniformRand();
      if (r<p) {
	// kill track
	nw.fN = 0;
	nw.fW = 0;
      }
      else {
	nw.fN = 1;     
      }
    }
  }
  return nw;
}

void MGImportanceAlgorithm::Error(const G4String &m) const
{
  MGLog(error) << "ERROR - MGImportanceAlgorithm: " << m << endlog;
 
}

void MGImportanceAlgorithm::Warning(const G4String &m) const
{
  MGLog(warning) << "MGImportanceAlgorithm::Warning()" <<  m << endlog;
}
