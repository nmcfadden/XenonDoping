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
// $Id: MGGeneratorPNNLRadioisotope.cc,v 1.2 2006-01-04 22:52:20 jasondet Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "generators/MGGeneratorPNNLRadioisotope.hh"
#include "Randomize.hh"
#include <iostream>
#include <fstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLRadioisotope::
MGGeneratorPNNLRadioisotope(G4String cascade_filename) :
  fCascade_filename(cascade_filename)
{
  ifstream pCascade_file((const char *)cascade_filename); 
  		//,ios::nocreate);   //ydc temp fix

  //if (pCascade_file.fail()) {      //ydc temp fix

  if (!pCascade_file){               //ydc temp fix
    cout << "Error in MGGeneratorPNNLRadioisotope constructor." << endl
	 << "Could not open radioisotope cascade file " 
	 << cascade_filename << endl;
    cout << "Aborting." << endl;
    exit(1);
  }

  pCascade_file >> fA >> fZ >> fNum_cascades;
  cout << "MGGeneratorPNNLRadioisotope:" << endl;
  cout << "Initiating parameters from file " << cascade_filename << endl;
  cout << "A: " << fA << " Z: " << fZ << " number of cascades: " 
       << fNum_cascades << endl;

  if (fNum_cascades <= 0) {
    cout << "ERROR in MGGeneratorPNNLRadioisotope constructor" << endl
	 << "Number of cascades read from file: " << fNum_cascades << endl
	 << "Number of cascades must be > 0 to create cascade list." << endl
	 << "Aborting." << endl;
    exit(1);
  }

  fCascade_list = new MGGeneratorPNNLCascade*[fNum_cascades];
  fBranching_fraction_list = new G4double[fNum_cascades];

  G4double E_endpoint;
  G4int iparticle_type;
  G4int delta_L;
  G4int num_gammas;

  for (G4int i=0; i<fNum_cascades; i++) {
    pCascade_file >> fBranching_fraction_list[i] >> E_endpoint
		  >> iparticle_type >> delta_L >> num_gammas;
    G4double* pGamma_E = new G4double[num_gammas];
    for (G4int j=0; j<num_gammas; j++)
       pCascade_file >> pGamma_E[j];

    fCascade_list[i] = new 
      MGGeneratorPNNLCascade(iparticle_type, fA, fZ, E_endpoint, delta_L, 
			     num_gammas, pGamma_E);

    delete [] pGamma_E;
  }

  pCascade_file.close();

  if (fNum_cascades > 1) 
    f_pCascadeSelector = new MGGeneratorPNNLLookup(fNum_cascades, 
						   fBranching_fraction_list);
}

//////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLRadioisotope::
MGGeneratorPNNLRadioisotope(const MGGeneratorPNNLRadioisotope& RadioIsotope)
{
  fCascade_filename = RadioIsotope.fCascade_filename;
  fA = RadioIsotope.fA;
  fZ = RadioIsotope.fZ;
  fNum_cascades = RadioIsotope.fNum_cascades;
  fCascade_list = new MGGeneratorPNNLCascade*[fNum_cascades];
  fBranching_fraction_list = new G4double[fNum_cascades];
  for (G4int i=0; i<fNum_cascades; i++) {
    fBranching_fraction_list[i] = RadioIsotope.fBranching_fraction_list[i];
    fCascade_list[i] = new MGGeneratorPNNLCascade(
		     *(RadioIsotope.fCascade_list[i]));
  }
  f_pCascadeSelector = new MGGeneratorPNNLLookup(
		       *(RadioIsotope.f_pCascadeSelector));
}
//////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLRadioisotope::
operator=(const MGGeneratorPNNLRadioisotope& RadioIsotope)
{
  fCascade_filename = RadioIsotope.fCascade_filename;
  fA = RadioIsotope.fA;
  fZ = RadioIsotope.fZ;
  fNum_cascades = RadioIsotope.fNum_cascades;
  fCascade_list = new MGGeneratorPNNLCascade*[fNum_cascades];
  fBranching_fraction_list = new G4double[fNum_cascades];
  for (G4int i=0; i<fNum_cascades; i++) {
    fBranching_fraction_list[i] = RadioIsotope.fBranching_fraction_list[i];
    fCascade_list[i] = new MGGeneratorPNNLCascade(
		     *(RadioIsotope.fCascade_list[i]));
  }
  f_pCascadeSelector = new MGGeneratorPNNLLookup(
		       *(RadioIsotope.f_pCascadeSelector));
}
//////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLRadioisotope::
~MGGeneratorPNNLRadioisotope()
{
  for (G4int i=0; i<fNum_cascades; i++)
    delete fCascade_list[i];

  delete [] fCascade_list;
  delete [] fBranching_fraction_list;

  if (f_pCascadeSelector)
    delete f_pCascadeSelector;
}

//////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLCascadeEvent MGGeneratorPNNLRadioisotope::
DoCascadeEvent() const {

  G4int indx_cascade;
  
  if (fNum_cascades > 1) {
    G4double x = G4UniformRand();
    indx_cascade = f_pCascadeSelector->DoLookupDiscrete(x);
    if ((indx_cascade < 0) || (indx_cascade >= fNum_cascades)) {
      cout << "ERROR in MGGeneratorPNNLRadioisotope::DoCascadeEvent" << endl
	   << "Anomalous value for sampled cascade index" << endl
	   << "indx_cascade = " << indx_cascade << endl
	   << "Aborting." << endl;
      exit(1);
    }
  }
  else
    indx_cascade = 0;

  MGGeneratorPNNLCascade* pCascade = fCascade_list[indx_cascade];

  G4double E_charged_particle = pCascade->DoSampleChargedParticleEnergy();
  G4int num_gammas = pCascade->GetNumGammas();
  G4double* pGammaList = pCascade->GetGammaList();
  
  MGGeneratorPNNLCascadeEvent CascadeEvent(indx_cascade, E_charged_particle, 
					   num_gammas, pGammaList);

  return CascadeEvent;
  
}

//////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLRadioisotope::
DoPrintCascadeList() const 
{
  for (G4int i=0; i<fNum_cascades; i++) {
    cout << "-----------------------" << endl;
    cout << "Cascade " << i << endl;
    fCascade_list[i]->DoPrintCascade();
  }
}
