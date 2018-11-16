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
// $Id: MGGeneratorPNNLLookup.hh,v 1.2 2007-02-23 13:39:50 mgmarino Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// File:        MGGeneratorPNNLLookup.hh
// Description: Utility class for lookup table storage and sampling in PNNL
//              radioactive decay model.
// Version:     1.0
// Created:     2004-04-23
// Author:      David Jordan
// mail:        david.jordan@pnl.gov
// phone:       509-376-0115


#ifndef MGGeneratorPNNLLookup_h
#define MGGeneratorPNNLLookup_h 1

#include "globals.hh"

// Lookup table utility class for PNNL radioactive decay generator.
// Handles calculation of lookup table and sampling from table for two
// lookup table types: an "interpolated" lookup on a table of (x,f(x))
// values, where f(x) is a probability distribution in x, and a "discrete
// index" lookup on a table of f(n), n = 0, 1, 2, ..., nmax values.

class MGGeneratorPNNLLookup {

public:
  MGGeneratorPNNLLookup() { };
  MGGeneratorPNNLLookup(G4int num_entries, G4double* x_arr, G4double* f_arr);
  MGGeneratorPNNLLookup(G4int num_entries, G4double* f_arr);
  MGGeneratorPNNLLookup(const MGGeneratorPNNLLookup& Lookup);
  ~MGGeneratorPNNLLookup();
  void operator=(const MGGeneratorPNNLLookup& Lookup);

  G4int     GetType()        const {return fType;         };
  G4double* GetXTable()      const {return f_pX_table;    };
  G4double* GetDistTable()   const {return f_pDist_table; };
  G4double* GetLookupTable() const {return f_pSum_table;  };

  void DoCreateTable(G4int num_entries, G4double* x_arr, G4double* f_arr);
  void DoCreateTable(G4int num_entries, G4double* f_arr);
  G4double DoLookupInterp(G4double x_rnd);
  G4int    DoLookupDiscrete(G4double x_rnd);

private:
  G4int     fType;          // interpolated- or discrete-index lookup type
  G4int     fNum_entries;   // refers to # of entries in input distribution
  G4double* f_pX_table;     // not used for discrete-index lookup type
  G4double* f_pDist_table;  // input distribution array
  G4double* f_pSum_table;   // lookup table

  G4int Locate(double x);
  static const G4int kTYPE_INTERPOLATED_LOOKUP;
  static const G4int kTYPE_DISCRETE_LOOKUP;

};

#endif
