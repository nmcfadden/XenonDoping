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
// $Id: MGGeneratorPNNLDecayChain.hh,v 1.2 2007-02-23 13:39:50 mgmarino Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// File:        MGGeneratorPNNLDecayChain.hh
// Description: Determines relative freqency of radioisotope decays and
//              stores radioisotope objects in PNNL radiactive decay model
// Version:     1.0
// Created:     2004-05-08
// Author:      David Jordan
// mail:        david.jordan@pnl.gov
// phone:       509-376-0115

#ifndef MGGeneratorPNNLDecayChain_h
#define MGGeneratorPNNLDecayChain_h 1

#include "generators/MGGeneratorPNNLRadioisotope.hh"
#include "generators/MGGeneratorPNNLLookup.hh"
#include "globals.hh"

class MGGeneratorPNNLDecayChain 
{
  public:
    MGGeneratorPNNLDecayChain();
    MGGeneratorPNNLDecayChain(G4String decay_chain_filename, 
			      G4double source_age);
    MGGeneratorPNNLDecayChain(const MGGeneratorPNNLDecayChain& DecayChain); 
    ~MGGeneratorPNNLDecayChain();
    void operator=(const MGGeneratorPNNLDecayChain& DecayChain); 

    void SetDecayChainFilename(G4String decay_chain_filename) {
                        fDecayChain_filename = decay_chain_filename; };
    void SetSourceAge(G4double source_age) {fSource_age = source_age;};

    G4String GetDecayChainFilename() const {return fDecayChain_filename; };
    G4double GetSourceAge()          const {return fSource_age/kYear; };
    G4int    GetIndexOfLastIsotopeSampled() const 
                                     {return fLastIsotopeSampled;};

    MGGeneratorPNNLCascadeEvent DoCascadeEvent();
    void DoInitializeGenerator();
    void DoPrintRadioisotopeList() const;

  private:
    G4String   fDecayChain_filename;
    G4int      fNum_RadioIsotopes;  
    G4double*  fActivity_list;
    G4double*  f_pT_half_list;
    char*      f_pT_half_units_list;
    G4double*  f_pLamda_list;
    G4String*  fRadioIsotope_label_list;
    G4double   fSource_age;
    G4int      fLastIsotopeSampled;
  
    MGGeneratorPNNLRadioisotope** fRadioIsotope_list;
    MGGeneratorPNNLLookup* f_pRadioIsotopeSelector;

    void CalculateActivityList();

    static const G4int kSecond;
    static const G4int kMinute;
    static const G4int kHour;
    static const G4int kDay;
    static const G4int kYear;
};

#endif


