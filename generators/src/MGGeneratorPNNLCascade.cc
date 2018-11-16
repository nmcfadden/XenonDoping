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
// $Id: MGGeneratorPNNLCascade.cc,v 1.3 2006-01-04 22:52:20 jasondet Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "generators/MGGeneratorPNNLCascade.hh"
#include "Randomize.hh"
#include <iostream>
#include <fstream>

#include "CLHEP/Units/SystemOfUnits.h"

using namespace std;
using namespace CLHEP;

const G4int MGGeneratorPNNLCascade::kNUM_BETA_TABLE_ELEMENTS = 100;
const G4int MGGeneratorPNNLCascade::kPARTICLE_INDEX_ALPHA    = 0;
const G4int MGGeneratorPNNLCascade::kPARTICLE_INDEX_BETA     = 1;
const G4int MGGeneratorPNNLCascade::kPARTICLE_INDEX_POSITRON = 2;

MGGeneratorPNNLCascade::
MGGeneratorPNNLCascade(G4int particle_type, 
                       G4double A, G4double Z,
                       G4double E_endpoint, G4int delta_L, 
                       G4int num_gammas, G4double* gamma_list) :
  fType(particle_type),
  fA(A), fZ(Z), 
  fE_endpoint(E_endpoint), fDelta_L(delta_L),
  fNum_gammas(num_gammas)

{
  fGamma_list      = NULL;
  f_pBetaEnergyGen = NULL;
  fE_alpha = 0.0;

  if (fNum_gammas > 0) {
    fGamma_list = new G4double[fNum_gammas];
    for (G4int i=0; i<fNum_gammas; i++)
      fGamma_list[i] = gamma_list[i];
  }

  if ((fType == kPARTICLE_INDEX_BETA) || (fType == kPARTICLE_INDEX_POSITRON)) {

    // Calculate beta(+/-) energy distribution.  Note that arrays of energies,
    // distributions, and integrated distributions are stored in the lookup
    // table object, so they're not saved as member variables in the Cascade
    // object.

    G4double* Beta_E_arr    = new G4double[kNUM_BETA_TABLE_ELEMENTS];
    G4double* Beta_dist_arr = new G4double[kNUM_BETA_TABLE_ELEMENTS];
    G4double Z_setup = fabs(fZ);  // make sure default is Z_setup > 0

    // N.B. Function Setup_beta_shape expects Z>0 for beta-, Z<0 for beta+
    if (fType == kPARTICLE_INDEX_POSITRON)
      Z_setup *= -1.0;

    Setup_beta_shape(fE_endpoint, Z_setup, fA, fDelta_L, 
		     kNUM_BETA_TABLE_ELEMENTS, Beta_E_arr, Beta_dist_arr);

    f_pBetaEnergyGen = new MGGeneratorPNNLLookup(kNUM_BETA_TABLE_ELEMENTS,
						 Beta_E_arr, Beta_dist_arr);
    delete [] Beta_E_arr;
    delete [] Beta_dist_arr;
  }
  else if (fType == kPARTICLE_INDEX_ALPHA) {
    fE_alpha = fE_endpoint;
  }
  else {
    cout << "ERROR in MGGeneratorPNNLCascade constructor: " << endl
	 << "Unrecognized charged particle type: " << fType << endl
	 << "Valid options are" << endl
	 << "(0) alpha" << endl
	 << "(1) beta-" << endl
	 << "(2) beta+" << endl
	 << "Aborting." << endl;
    exit(1);
  }
}
  
//////////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLCascade::
MGGeneratorPNNLCascade(const MGGeneratorPNNLCascade& Cascade)
{
  fType       = Cascade.fType;
  fA          = Cascade.fA;
  fZ          = Cascade.fZ;
  fDelta_L    = Cascade.fDelta_L;
  fNum_gammas = Cascade.fNum_gammas;
  
  fE_endpoint = 0.0;
  fE_alpha    = 0.0;

  fGamma_list      = NULL;
  f_pBetaEnergyGen = NULL;

  if (fNum_gammas > 0) {
    fGamma_list = new G4double[fNum_gammas];
    for (G4int i=0; i<fNum_gammas; i++)
      fGamma_list[i] = Cascade.fGamma_list[i];
  }

  if ((fType == kPARTICLE_INDEX_BETA) || (fType == kPARTICLE_INDEX_POSITRON)) {
    fE_endpoint = Cascade.fE_endpoint;
    f_pBetaEnergyGen = new MGGeneratorPNNLLookup(*(Cascade.f_pBetaEnergyGen));
  }
  else {
    fE_alpha = Cascade.fE_alpha;
  }
}

//////////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLCascade::
operator=(const MGGeneratorPNNLCascade& Cascade)
{
  fType       = Cascade.fType;
  fA          = Cascade.fA;
  fZ          = Cascade.fZ;
  fDelta_L    = Cascade.fDelta_L;
  fNum_gammas = Cascade.fNum_gammas;
  
  fE_endpoint = 0.0;
  fE_alpha    = 0.0;

  fGamma_list      = NULL;
  f_pBetaEnergyGen = NULL;

  if (fNum_gammas > 0) {
    fGamma_list = new G4double[fNum_gammas];
    for (G4int i=0; i<fNum_gammas; i++)
      fGamma_list[i] = Cascade.fGamma_list[i];
  }

  if ((fType == kPARTICLE_INDEX_BETA) || (fType == kPARTICLE_INDEX_POSITRON)) {
    fE_endpoint = Cascade.fE_endpoint;
    f_pBetaEnergyGen = new MGGeneratorPNNLLookup(*(Cascade.f_pBetaEnergyGen));
  }
  else {
    fE_alpha = Cascade.fE_alpha;
  }
}

//////////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLCascade::
~MGGeneratorPNNLCascade()
{
  if (fGamma_list)
    delete [] fGamma_list;
}

//////////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLCascade::
SetGammaList(G4int num_gammas, G4double* gamma_list) 
{

  fNum_gammas = num_gammas;

  if (fGamma_list)
    delete [] fGamma_list;

  fGamma_list = new G4double[num_gammas];
  for (G4int i=0; i<num_gammas; i++)
    fGamma_list[i] = gamma_list[i];
}

//////////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLCascade::
DoPrintCascade() const 
{

  cout << "Particle Type:       " << fType           << endl
       << "Parent (A,Z):        " << fA << " " << fZ << endl;

  if (fType == kPARTICLE_INDEX_BETA)
    cout << "Beta- endpoint (MeV): " << fE_endpoint << endl
	 << "Delta(L):            " << fDelta_L    << endl;
  else if (fType == kPARTICLE_INDEX_POSITRON)
    cout << "Beta+ endpoint (MeV): " << fE_endpoint << endl
	 << "Delta(L):            " << fDelta_L    << endl;
  else
    cout << "Alpha energy (MeV):  " << fE_endpoint << endl;

  cout << "Number of gammas:    " << fNum_gammas << endl;
  if (fNum_gammas > 0) {
    cout << "Gamma energy list:   ";

    for (G4int i=0; i<fNum_gammas; i++) {
      if (i == 0)
        cout << fGamma_list[i] << endl;
      else
        cout << "                     " << fGamma_list[i] << endl;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
G4double MGGeneratorPNNLCascade::
DoSampleChargedParticleEnergy() const 
{

  if (fType == kPARTICLE_INDEX_ALPHA)
    return fE_alpha;
  else
    return DoSampleBetaDist();   // beta- or beta+
}

//////////////////////////////////////////////////////////////////////////////
G4double MGGeneratorPNNLCascade::
DoSampleBetaDist() const 
{

  G4double x = G4UniformRand();
  G4double E = f_pBetaEnergyGen->DoLookupInterp(x) * MeV;

  return E;
}

//////////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLCascade::
Setup_beta_shape(G4double Qval, G4double Z, 
                 G4double /*A*/, G4int IDL, G4int num_entries, 
		 G4double* E_arr, G4double* Beta_arr) 
{

  // Translated from H.S. Miley's Fortran function BETA_SHAPE, DJ 4-Oct-02
  // Given Qval, A, and Z, returns an allowed beta shape, using a 
  // non-relativistic Fermi function.  The table of (E_arr, Beta_arr) will
  // have num_entries entries.  
  //
  // N.B. Convention for beta- vs. beta+ decay is Z>0 for beta-, Z<0 for beta+.
  //      (This follows Evans, "The Atomic Nucleus", eq. (3.14).)
  //
  // IDL >=0 gives single beta decay, IDL<0 gives double beta decay.  2beta
  // formula (Primakoff-Rosen approx.) taken from CEA's modification of 
  // crazy.mortran, Rev. 1.33.

  const G4double PI = 3.1415927;
  const G4double M_e = 0.511;

  G4double ZD;
  G4double T0;
  G4double Estep;
  G4double T, UF;
  G4double Y, FERFAC;
  G4double tmp1, tmp2;

  ZD    = (Z+1.0)/137.0;    // Z>0 ==> beta- decay, Z<0 ==> beta+ decay
  T0    = Qval/M_e;  
  Estep = Qval/(num_entries-1);

  for (G4int i=0; i<num_entries; i++) {
    T = i * Estep/M_e;
    E_arr[i] = T * M_e;

    if (IDL >= 0) {   // single beta decay
      switch (IDL) {
        case 0:
          UF = 1.0;
          break;
        case 1:
          UF=T*(T+2.0) + pow(T0-T,2.0);
          break;
        case 2:
          UF= pow((T*(T+2)),2.0) + pow((T0-T),4.0) + 
              (10.0/3.0)*T*(T+2)*pow((T0-T),2.0);
          break;
        case 3:
          UF = pow((T*(T+2.0)),3.0) + pow((T0-T),6.0);
          UF += 7.0*T*(T+2)*pow((T0-T),2.0)*(T*(T+2.0)+pow((T0-T),2.0));
          break;
        default:
          cout << "Beta_shape: Unsupported value of IDL = " << IDL << endl;
          cout << "Only supported values at present are IDL=0,1,2,3." << endl;
          cout << "Aborting." << endl;
          exit(1);
      }

      if (T > 0.0) {
        Beta_arr[i] = (T+1.0)*sqrt(T*(T+2.0))*pow((T0-T),2.0);
        Y           = ZD*(T+1.0)/sqrt(T*(T+2.0));
        FERFAC      = 2.0*PI*Y/(1.0-exp(-2.0*PI*Y));
      }
      else {
	if (Z > 0.0) {  // beta- shape is finite, >0 as E->0
          Beta_arr[i] = (T+1.0)*pow((T0-T),2.0);
          Y           = ZD*(T+1.0);
          FERFAC      = 2.0*PI*Y;
	}
	else {         // but beta+ shape ->0 as E->0
	  Beta_arr[i] = 0.0;
	  FERFAC = 0.0;
	}
      }

      Beta_arr[i] *= FERFAC*UF;
    }

    else {   // double beta-decay; Primakoff-Rosen approximation
      tmp1 = double(i)/double(num_entries-1);
      tmp2 = 1.0/Qval;
      Beta_arr[i] = pow(Qval,2.0) * pow((tmp1 + tmp2),2.0) * 
                                    pow((1.0 - tmp1 + tmp2),2.0);
    } 
     
  } 
}

