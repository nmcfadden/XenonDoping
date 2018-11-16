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
// $Id: MGGeneratorPNNLDecayChain.cc,v 1.3 2006-01-04 22:52:20 jasondet Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "generators/MGGeneratorPNNLDecayChain.hh"
#include "Randomize.hh"
#include <iostream>
#include <iomanip>

using namespace std;

const G4int MGGeneratorPNNLDecayChain::kSecond =   1;
const G4int MGGeneratorPNNLDecayChain::kMinute =  60 * kSecond;
const G4int MGGeneratorPNNLDecayChain::kHour   =  60 * kMinute;
const G4int MGGeneratorPNNLDecayChain::kDay    =  24 * kHour;
const G4int MGGeneratorPNNLDecayChain::kYear   = 365 * kDay;

//////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLDecayChain::MGGeneratorPNNLDecayChain()
{
  fDecayChain_filename = "DecayChain.dat";  // default
  fSource_age = 0.0;

  DoInitializeGenerator();
}

//////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLDecayChain::
MGGeneratorPNNLDecayChain(G4String decay_chain_filename, G4double source_age) :
  fDecayChain_filename(decay_chain_filename),
  fSource_age(source_age)
{
  DoInitializeGenerator();
}

//////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLDecayChain::
MGGeneratorPNNLDecayChain(const MGGeneratorPNNLDecayChain& DecayChain)
{
  fDecayChain_filename = DecayChain.fDecayChain_filename;
  fNum_RadioIsotopes   = DecayChain.fNum_RadioIsotopes;
  fSource_age          = DecayChain.fSource_age;
  fLastIsotopeSampled  = DecayChain.fLastIsotopeSampled;

  fActivity_list           = new G4double[fNum_RadioIsotopes];
  fRadioIsotope_label_list = new G4String[fNum_RadioIsotopes];
  f_pT_half_list           = new G4double[fNum_RadioIsotopes];
  f_pLamda_list            = new G4double[fNum_RadioIsotopes];
  f_pT_half_units_list     = new char[fNum_RadioIsotopes];
  fRadioIsotope_list = new MGGeneratorPNNLRadioisotope*[fNum_RadioIsotopes];

  for (G4int i=0; i<fNum_RadioIsotopes; i++) {
    fActivity_list[i]           = DecayChain.fActivity_list[i];
    fRadioIsotope_label_list[i] = DecayChain.fRadioIsotope_label_list[i];
    f_pT_half_list[i]           = DecayChain.f_pT_half_list[i];
    f_pLamda_list[i]            = DecayChain.f_pLamda_list[i];
    f_pT_half_units_list[i]     = DecayChain.f_pT_half_units_list[i];

    fRadioIsotope_list[i] = new MGGeneratorPNNLRadioisotope(
			    *(DecayChain.fRadioIsotope_list[i]));
  }

  f_pRadioIsotopeSelector = new MGGeneratorPNNLLookup(
			    *(DecayChain.f_pRadioIsotopeSelector));

}

//////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLDecayChain::
operator=(const MGGeneratorPNNLDecayChain& DecayChain)
{
  fDecayChain_filename = DecayChain.fDecayChain_filename;
  fNum_RadioIsotopes   = DecayChain.fNum_RadioIsotopes;
  fSource_age          = DecayChain.fSource_age;
  fLastIsotopeSampled  = DecayChain.fLastIsotopeSampled;

  fActivity_list           = new G4double[fNum_RadioIsotopes];
  fRadioIsotope_label_list = new G4String[fNum_RadioIsotopes];
  f_pT_half_list           = new G4double[fNum_RadioIsotopes];
  f_pLamda_list            = new G4double[fNum_RadioIsotopes];
  f_pT_half_units_list     = new char[fNum_RadioIsotopes];
  fRadioIsotope_list = new MGGeneratorPNNLRadioisotope*[fNum_RadioIsotopes];

  for (G4int i=0; i<fNum_RadioIsotopes; i++) {
    fActivity_list[i]           = DecayChain.fActivity_list[i];
    fRadioIsotope_label_list[i] = DecayChain.fRadioIsotope_label_list[i];
    f_pT_half_list[i]           = DecayChain.f_pT_half_list[i];
    f_pLamda_list[i]            = DecayChain.f_pLamda_list[i];
    f_pT_half_units_list[i]     = DecayChain.f_pT_half_units_list[i];

    fRadioIsotope_list[i] = new MGGeneratorPNNLRadioisotope(
			    *(DecayChain.fRadioIsotope_list[i]));
  }

  f_pRadioIsotopeSelector = new MGGeneratorPNNLLookup(
			    *(DecayChain.f_pRadioIsotopeSelector));

}

//////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLDecayChain::
DoInitializeGenerator() 
{

  if (fSource_age < 0.0) {
    cout << "Error in MGGeneratorPNNLDecayChain::DoInitializeGenerator."
	 << endl;
    cout << "Source age is negative, must be positive." << endl;
    cout << "fSource_age = " << fSource_age << endl;
    cout << "Aborting." << endl;
    exit(1);
  }

  // N.B. Source age supplied by user is in years.
  fSource_age *= kYear;

  // Read the decay chain file.  This contains
  // (a) the number of radioisotopes in the decay chain (first line in file);
  // (b) a list of radioisotopes, with each line containing the following info:
  //   1.  integer index # (not used -- for user's reference only)
  //   2.  string isotope label, e.g. Th-232
  //   3.  half-life (double), e.g. 1.41e10
  //   4.  units of half-life (one character), can be y, m, d, s for year,
  //       month, day, second, respectively
  //   5.  cascade file for that radioisotope
  //  Example line:
  //  1  Th-232  1.41e10  y  Th-232_Cascade.dat

  ifstream pDecayChain_file((const char *)fDecayChain_filename); 
  					//ydc temp fix ,ios::nocreate);

  //if (pDecayChain_file.fail()) {	//ydc itemp fix

  if (!pDecayChain_file){		//ydc temp fix
    cout << "Error in MGGeneratorPNNLDecayChain::DoInitializeGenerator." 
	 << endl
	 << "Could not open decay chain file " 
	 << fDecayChain_filename << endl;
    cout << "Aborting." << endl;
    exit(1);
  }

  pDecayChain_file >> fNum_RadioIsotopes;
  cout << "MGGeneratorPNNLDecayChain:" << endl;
  cout << "Initiating parameters from file " << fDecayChain_filename << endl;
  cout << "Number of radioisotopes: " 
       << fNum_RadioIsotopes << endl;

  if (fNum_RadioIsotopes <= 0) {
    cout << "ERROR in MGGeneratorPNNLDecayChain constructor" << endl
	 << "Number of radioisotopes read from file: " 
	 << fNum_RadioIsotopes << endl
	 << "Number of radioisotopes must be > 0 to create radioisotope list."
	 << endl
	 << "Aborting." << endl;
    exit(1);
  }

  fRadioIsotope_list = new MGGeneratorPNNLRadioisotope*[fNum_RadioIsotopes];
  fActivity_list           = new G4double[fNum_RadioIsotopes];
  fRadioIsotope_label_list = new G4String[fNum_RadioIsotopes];
  f_pT_half_list           = new G4double[fNum_RadioIsotopes];
  f_pLamda_list            = new G4double[fNum_RadioIsotopes];
  f_pT_half_units_list     = new char[fNum_RadioIsotopes];

  G4int    idum;
  char     isotope_label[10];
  G4double T_half;
  char     units_ch;
  char     cascade_file[20];

  for (G4int i=0; i<fNum_RadioIsotopes; i++) {
    pDecayChain_file >> idum >> isotope_label >> T_half 
		     >> units_ch >> cascade_file;
    fRadioIsotope_label_list[i] = isotope_label;
    units_ch = toupper(units_ch);

    switch (units_ch) {
    case 'S':
      T_half *= kSecond;
      break;
    case 'M':
      T_half *= kMinute;
      break;
    case 'H':
      T_half *= kHour;
      break;
    case 'D':
      T_half *= kDay;
      break;
    case 'Y':
      T_half *= kYear;
      break;
    default:
      cout << "ERROR in MGGeneratorPNNLDecayChain." << endl;
      cout << "Unrecognized units code for radioisotope #" << i << endl;
      cout << "units_ch: " << units_ch << endl;
      cout << "Valid options for units are S, H, M, D, Y." << endl;
      cout << "Aborting." << endl;
      exit(1);
    }
    
    f_pT_half_list[i]       = T_half;
    f_pT_half_units_list[i] = units_ch;
    f_pLamda_list[i]        = log(2.0)/T_half;   // exponential decay constant
    fRadioIsotope_list[i]   = new MGGeneratorPNNLRadioisotope(
			                G4String(cascade_file));
  }

  pDecayChain_file.close();

  CalculateActivityList();

  if (fNum_RadioIsotopes > 1) 
    f_pRadioIsotopeSelector = new MGGeneratorPNNLLookup(fNum_RadioIsotopes, 
						        fActivity_list);

  fLastIsotopeSampled = -1;

  DoPrintRadioisotopeList();
}

//////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLDecayChain::~MGGeneratorPNNLDecayChain()
{
  for (G4int i=0; i<fNum_RadioIsotopes; i++)
    delete fRadioIsotope_list[i];

  if (fRadioIsotope_list)
    delete [] fRadioIsotope_list;
  if (fActivity_list)
    delete [] fActivity_list;
  if (fRadioIsotope_label_list)
    delete [] fRadioIsotope_label_list;
  if (f_pT_half_list)
    delete [] f_pT_half_list;
  if (f_pLamda_list)
    delete [] f_pLamda_list;
  if (f_pT_half_units_list)
    delete [] f_pT_half_units_list;
  
  if (f_pRadioIsotopeSelector)
    delete f_pRadioIsotopeSelector;
}

//////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLCascadeEvent MGGeneratorPNNLDecayChain::
DoCascadeEvent() {

  G4int indx_radioisotope;
  
  if (fNum_RadioIsotopes > 1) {
    G4double x = G4UniformRand();
    indx_radioisotope = f_pRadioIsotopeSelector->DoLookupDiscrete(x);
    if ((indx_radioisotope < 0) || (indx_radioisotope >= fNum_RadioIsotopes)) {
      cout << "ERROR in MGGeneratorPNNLDecayChain::DoCascadeEvent" << endl
	   << "Anomalous value for sampled radioisotope index" << endl
	   << "indx_radioisotope = " << indx_radioisotope << endl
	   << "Aborting." << endl;
      exit(1);
    }
  }
  else
    indx_radioisotope = 0;

  MGGeneratorPNNLRadioisotope* pRadioIsotope = 
            fRadioIsotope_list[indx_radioisotope];

  fLastIsotopeSampled = indx_radioisotope;
  
  return pRadioIsotope->DoCascadeEvent();
  
}

//////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLDecayChain::DoPrintRadioisotopeList() const {

  G4double T_half;
  char     units_ch;
  G4double Lamda;

  cout << "DecayChain contains " << fNum_RadioIsotopes << " isotopes." 
       << endl;
  cout << "Activities normalized to a single nucleus of isotope at head of decay chain." << endl;
  cout << "Radioisotope    T_half     Decay Const.    Activity     Rel. Activity" << endl;
  cout << "                             (1/sec)         (Bq)"  << endl;

  for (G4int i=0; i<fNum_RadioIsotopes; i++) {

    T_half   = f_pT_half_list[i];
    units_ch = f_pT_half_units_list[i];
    Lamda    = f_pLamda_list[i];

    switch (units_ch) {
    case 'S':
      T_half /= kSecond;
      break;
    case 'M':
      T_half /= kMinute;
      break;
    case 'H':
      T_half /= kHour;
      break;
    case 'D':
      T_half /= kDay;
      break;
    case 'Y':
      T_half /= kYear;
      break;
    default:
      cout << "ERROR in MGGeneratorPNNLDecayChain." << endl;
      cout << "Unrecognized units code for radioisotope #" << i << endl;
      cout << "units_ch: " << units_ch << endl;
      cout << "Valid options for units are S, H, M, D, Y." << endl;
      cout << "Aborting." << endl;
      exit(1);
    }

    cout << setw(10) << fRadioIsotope_label_list[i]
	 << setw(10) << T_half 
	 << setw(2)  << units_ch 
	 << setw(15) << Lamda
	 << setw(15) << fActivity_list[i] 
	 << setw(15) << fActivity_list[i]/fActivity_list[0] 
	 << endl;
  }
}

//////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLDecayChain::CalculateActivityList() {
  
  // Calculate Bateman coefficients & relative activities of each member of
  // the decay chain.  See Krane, "Introductory Nuclear 
  // Physics", New York: Wiley, 1988, section 6.4, "Growth of Daughter
  // Activities", in particular eqn's (6.37) and (6.38).  This function
  // requires that the following member variables have been previously
  // initialized:
  // fNum_RadioIsotopes -- number of radioisotopes in the chain;
  // f_pLamda_list -- array of decay constants for each decay chain member;
  // fSource_age -- age of source, with T=0 corresponding to a source
  //                with an initial concentration of 100% of the first isotope
  //                in the chain.
  //
  // The function fills the array fActivity_list, containing the activities
  // relative to the total number of initial nuclei of the first isotope in
  // the list (i.e. setting N0=1 in Krane's equation 6.37).


  // The 2D array ppCoeff has dimensions 
  // fNum_RadioIsotopes x fNum_RadioIsotopes,
  // and containes Crane's c_m coefficients for the n'th isotope in the chain.

  G4double** ppCoeff = new G4double*[fNum_RadioIsotopes];
  for (G4int i=0; i<fNum_RadioIsotopes; i++){
    ppCoeff[i] = new G4double[fNum_RadioIsotopes];
  }

  for (G4int n=0; n<fNum_RadioIsotopes; n++) {
    G4double prod_numer = 1.0;
    for (G4int i=0; i<=n; i++)
      prod_numer *= f_pLamda_list[i];
    for (G4int im=0; im<=n; im++) {
      G4double prod_denom = 1.0;
      for (G4int i=0; i<=n; i++) {
	if (i != im) 
	  prod_denom *= (f_pLamda_list[i] - f_pLamda_list[im]);
      }
      ppCoeff[n][im] = prod_numer/prod_denom;
    }
  }
      
  // Calculate the activities (relative to head of decay chain), using
  // Krane's eq'n 6.37.

  for (G4int n=0; n<fNum_RadioIsotopes; n++) {
    G4double sum = 0.0;
    for (G4int i=0; i<=n; i++) {
      G4double fac = exp(-f_pLamda_list[i] * fSource_age);
      sum += ppCoeff[n][i] * fac;
    }

    fActivity_list[n] = sum;

    //cout << "Isotope: " << n << " Activity: " << fActivity_list[n] << endl;
  }

  // Cleanup 
  for (G4int i=0; i<fNum_RadioIsotopes; i++)
    delete [] ppCoeff[i];
  
  delete [] ppCoeff;

}
