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
 * $Id: MGGeneratorPNNL.hh,v 1.2 2004-12-16 23:40:15 dimarcom Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorPNNL.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Generator for PNNL
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jul  7 18:12:47 PDT 2004
 * 
 * REVISION:
 * 
 * 07-08-2004, Created, R. Henning
 * 11-08-2004, Implemented virtual method SetParticlePosition of MGVGenerator, Luciano
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORPNNL_HH
#define _MGGENERATORPNNL_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4ThreeVector.hh"

#include "generators/MGGeneratorPNNLDecayChain.hh"
#include "generators/MGGeneratorPNNLRadioisotope.hh"
#include "generators/MGVGenerator.hh"

class G4Event;
class G4ParticleGun;
class MGPrimaryGeneratorMessenger;

//---------------------------------------------------------------------------//

class MGGeneratorPNNL : public MGVGenerator
{
public:

  //default constructor
  MGGeneratorPNNL();

  //copy constructor
  MGGeneratorPNNL(const MGGeneratorPNNL &);

  //destructor
  ~MGGeneratorPNNL();

  //public interface
  void BeginOfRunAction(const G4Run *run);
  void EndOfRunAction(const G4Run *run);
  void GeneratePrimaryVertex(G4Event *event);
  void SetParticlePosition(G4ThreeVector vec) {SetPosition(vec);}
  //  Histogram1D* GetHist_Egen() { return pHist_EgammaGen; };
  void SetPNNLGenerateBeta(G4bool generate_beta)
                         {fPNNL_generate_beta = generate_beta;}
  void SetPNNLSourceAge(G4double source_age) 
                         {fPNNL_source_age = source_age;}
  void SetPNNLDecayChainFile(G4String DecayChain_file) 
                    {fPNNL_DecayChain_file = DecayChain_file;}
  void SetPosition(G4ThreeVector pos) { fPosition = pos; }

  //protected members
protected:


  //private  members
private:
  void pick_point_in_circle(G4double R, G4double &x, G4double &y);
  
  G4ParticleGun* fParticleGun;
  //G4int num_events;
  G4int igen_flag;
  G4int idirection_flag;
  G4double E_gamma;
  G4double RADIUS_DETECTOR;
  G4double ZPOS_DETECTOR;
  
  MGGeneratorPNNLRadioisotope* f_pRadioIsotope;
  MGGeneratorPNNLDecayChain* f_pPNNLDecayChain;
  
  G4int hist_cascade[3];
  G4int hist_isotope[10];

  G4bool   fFirstCall;
  G4bool   fUsePNNLGen;
  G4String fPNNL_DecayChain_file;
  G4bool fPNNL_generate_beta;
  G4double fPNNL_source_age;

  G4ThreeVector fPosition;
};
#endif
