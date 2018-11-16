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
// $Id: MGGeneratorPNNLCascade.hh,v 1.4 2007-02-23 13:39:50 mgmarino Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// File:        MGGeneratorPNNLCascade.hh
// Description: Single-cascade storage & beta-distribution sampling class for 
//              PNNL radiactive decay model
// Version:     1.0
// Created:     2004-04-06
// Author:      David Jordan
// mail:        david.jordan@pnl.gov
// phone:       509-376-0115

#ifndef MGGeneratorPNNLCascade_h
#define MGGeneratorPNNLCascade_h 1


#include "globals.hh"
#include "generators/MGGeneratorPNNLLookup.hh"

class MGGeneratorPNNLCascade
{
  public:
    MGGeneratorPNNLCascade(){ };
    MGGeneratorPNNLCascade(G4int particle_type,
                           G4double A, G4double Z,
                           G4double E_endpoint, G4int delta_L, 
                           G4int num_gammas, G4double* gamma_list);
    MGGeneratorPNNLCascade(const MGGeneratorPNNLCascade& Cascade);
    ~MGGeneratorPNNLCascade();
    void operator=(const MGGeneratorPNNLCascade& Cascade);

    void SetChargedParticleType(G4int itype) {fType = itype;};
    void SetA(G4double A) {fA = A;};
    void SetZ(G4double Z) {fZ = Z;};
    void SetEndpointE(G4double E_endpoint) {fE_endpoint = E_endpoint;};   
    void SetDeltaL(G4int delta_L) {fDelta_L = delta_L;};
    void SetGammaList(G4int num_gammas, G4double* gamma_list);

    G4int     GetChargedParticleType() const {return fType;               }; 
    G4double  GetA()                   const {return fA;                  };
    G4double  GetZ()                   const {return fZ;                  };
    G4double  GetEndpointE()           const {return fE_endpoint;         };
    G4int     GetDeltaL()              const {return fDelta_L;            };
    G4int     GetNumGammas()           const {return fNum_gammas;         };
    G4double* GetGammaList()           const {return fGamma_list;         };
    G4double* GetBetaETable()          const 
                               {return f_pBetaEnergyGen->GetXTable();     };
    G4double* GetBetaDistTable()       const 
                               {return f_pBetaEnergyGen->GetDistTable();  }; 
    G4double* GetBetaLookupTable()     const 
                               {return f_pBetaEnergyGen->GetLookupTable();}; 

    G4double DoSampleChargedParticleEnergy() const;
    void      DoPrintCascade()   const;

  private:
    G4int fType;                  // charged particle type
    G4double fA;                  // parent nucleus mass number A
    G4double fZ;                  // parent nucleus atomic number Z
    G4double fE_endpoint;         // beta endpoint energy
    G4int fDelta_L;               // momentum change in decay
    G4int fNum_gammas;            // number of gammas in cascade
    G4double* fGamma_list;        // list of gamma energies
    MGGeneratorPNNLLookup* f_pBetaEnergyGen;
    G4double fE_alpha;            // mono-energetic alpha energy

    static const G4int kNUM_BETA_TABLE_ELEMENTS;
    static const G4int kPARTICLE_INDEX_ALPHA;
    static const G4int kPARTICLE_INDEX_BETA;
    static const G4int kPARTICLE_INDEX_POSITRON;

    void Setup_beta_shape(G4double Qval, G4double Z, G4double A,
			  G4int IDL, G4int num_entries, 
			  G4double* E_arr, G4double* Beta_arr);
    G4double  DoSampleBetaDist() const;
};
#endif


