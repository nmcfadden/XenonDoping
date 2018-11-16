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
// $Id: MGGeneratorPNNLRadioisotope.hh,v 1.1 2004-12-07 08:45:48 pandola Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// File:        MGGeneratorPNNLRadioisotope.hh
// Description: Single-isotope storage class for PNNL radiactive decay model
// Version:     1.0
// Created:     2004-04-06
// Author:      David Jordan
// mail:        david.jordan@pnl.gov
// phone:       509-376-0115

#ifndef MGGeneratorPNNLRadioisotope_h
#define MGGeneratorPNNLRadioisotope_h 1

#include "generators/MGGeneratorPNNLCascade.hh"
#include "generators/MGGeneratorPNNLCascadeEvent.hh"
#include "generators/MGGeneratorPNNLLookup.hh"
#include "globals.hh"

class MGGeneratorPNNLRadioisotope 
{
  public:
    MGGeneratorPNNLRadioisotope() { };
    MGGeneratorPNNLRadioisotope(G4String cascade_filename);
    MGGeneratorPNNLRadioisotope(const MGGeneratorPNNLRadioisotope& 
                                      RadioIsotope);
    ~MGGeneratorPNNLRadioisotope();
    void operator=(const MGGeneratorPNNLRadioisotope& RadioIsotope);

    void SetCascadeFilename(G4String cascade_filename) {
      fCascade_filename = cascade_filename; };
    G4String GetCascadeFilename() { return fCascade_filename; };

    MGGeneratorPNNLCascadeEvent DoCascadeEvent() const;
    void DoPrintCascadeList() const;

  private:
    G4String fCascade_filename;
    G4double fA;          // nucleus mass number A
    G4double fZ;          // nucleus atomic number Z
    G4int fNum_cascades;  // number of cascades
    MGGeneratorPNNLCascade** fCascade_list;
    G4double* fBranching_fraction_list;
    MGGeneratorPNNLLookup* f_pCascadeSelector;
};

#endif


