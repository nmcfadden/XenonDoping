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
 * $Id: MGGeneratorDoubleBeta.hh,v 1.2 2007-02-23 09:24:09 mgmarino Exp $
 *
 * CLASS DECLARATION:  MGGeneratorDoubleBeta.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 * SPECIAL NOTES:
 *  This generator is an implementation of BB-decay event generation via the
 *  theoretical phase space factors and related single electron spectral
 *  calculations of J. Kotila and F. Iachello. The datafiles it uses are
 *  provided directly by Kotila & Iachello and can be downloaded here:
 *  http://nucleartheory.yale.edu/double-beta-decay-phase-space-factors
 *  Their work is published in the article "Phase-space Factors for Double-B
 *  Decay": J. Kotila and F. Iachello, Physical Review C 85, 034316 (2012) and 
 *  nucleartheory.yale.edu
*/
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: massarczyk@lanl.gov
 * CONTACT:
 * FIRST SUBMISSION: 2016
 *
 * REVISION:
 *
 * 03-08-2016, Created, R. Massarczyk
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORDOUBLEBETA_HH
#define _MGGENERATORDOUBLEBETA_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4ThreeVector.hh"

#include "generators/MGVGenerator.hh"

#include "TH1D.h"
#include "TH2D.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
//---------------------------------------------------------------------------//

class G4Event;
class G4Messenger;
class G4ParticleGun;
class G4Run;

class MGGeneratorDoubleBeta : public MGVGenerator
{
public:

  //default constructor
  MGGeneratorDoubleBeta(G4int);

  //copy constructor
  MGGeneratorDoubleBeta(const MGGeneratorDoubleBeta &);

  //destructor
  ~MGGeneratorDoubleBeta();

  //public interface
  void GeneratePrimaryVertex(G4Event *event);
  void LoadFiles();
  void SetParticlePosition(G4ThreeVector pos) { fPosition = pos; }

  void SetNeutrino(G4int number) { NeutrinoNumber = number; LoadFiles();}

  //protected members
protected:

  //private  members
private:
  G4ParticleGun      *fParticleGun;
  G4int				 NeutrinoNumber;
  G4ThreeVector      fPosition;

  std::ifstream in;
  std::stringstream filenamestream;
  std::string filename;
  G4int index, index2;
  G4double energy, energy2;
  G4double quantity;

  G4double e1e2_int;
  G4double e1e2_angle;

  G4double e1_int;
  G4int e1_Ekin;
  G4double e1_px;
  G4double e1_py;
  G4double e1_pz;
  G4double e1_ptot;
  G4double e1_cosTheta;
  G4double e1_sinTheta;
  G4double e1_phi;

  G4double e2_int;
  G4int e2_Ekin;
  G4double e2_px;
  G4double e2_py;
  G4double e2_pz;
  G4double e2_ptot;
  G4double e2_cosTheta;
  G4double e2_sinTheta;
  G4double e2_phi;

  G4double eSe_ptot;
  G4double eSe_Ekin;

  G4double help_c, help_R, help_alpha;


  TH1D* singleE = new TH1D("single electron energy probalility","single electron energy probalility",2040,0,2040);
  TH1D* singleE_int = new TH1D("single electron energy probalility Integral","single electron energy probalility Integral",2040,0,2040);

  TH2D* prob_angle = new TH2D("probability for an angle at E","probability for an angle at E",2040,0,2040,200,-1,1);
  TH2D* prob_angle_int = new TH2D("probability for an angle at E Integral","probability for an angle at E Integral",2040,0,2040,200,-1,1);

  TH2D* bothE = new TH2D("bothE","bothE",2040,0,2040,2040,0,2040);
  TH2D* bothE_int = new TH2D("both integral for 2nd","bothE2vbb for 2nd",2040,0,2040,2040,0,2040);


};
#endif
