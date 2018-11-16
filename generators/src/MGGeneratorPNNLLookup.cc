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
// $Id: MGGeneratorPNNLLookup.cc,v 1.2 2006-01-04 22:52:20 jasondet Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "generators/MGGeneratorPNNLLookup.hh"
#include <iostream>
#include <fstream>

using namespace std;

const G4int MGGeneratorPNNLLookup::kTYPE_INTERPOLATED_LOOKUP = 0;
const G4int MGGeneratorPNNLLookup::kTYPE_DISCRETE_LOOKUP     = 1;


/////////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLLookup::
MGGeneratorPNNLLookup(G4int num_entries, G4double* x_arr, G4double* f_arr) 
{

  // This constructor sets up an (x,probability)-type lookup table.  X-values
  // sampled from this lookup are interpolated on the table of X's.  Note:
  // the lookup table is set up to have the same number of entries as the
  // input distribution array.  (This is in contrast to the "discrete" lookup
  // table type.)

  fType = kTYPE_INTERPOLATED_LOOKUP;

  f_pX_table    = NULL;
  f_pDist_table = NULL;
  f_pSum_table  = NULL;

  DoCreateTable(num_entries, x_arr, f_arr);
}

/////////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLLookup::
MGGeneratorPNNLLookup(G4int num_entries, G4double* f_arr) 
{
  // This constructor sets up an integer-indexed, or discrete, lookup table.
  // The values sampled from this table type are integer indices  
  // 0, 1, 2, ..., num_entries-1.  The lookup table has num_entries+1 entries.


  fType = kTYPE_DISCRETE_LOOKUP;

  f_pX_table    = NULL;   // not used for this lookup table type
  f_pDist_table = NULL;
  f_pSum_table  = NULL;

  DoCreateTable(num_entries, f_arr);
}

/////////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLLookup::
~MGGeneratorPNNLLookup() 
{

  if (f_pX_table)
    delete [] f_pX_table;
  if (f_pDist_table)
    delete [] f_pDist_table;
  if (f_pSum_table)
    delete [] f_pSum_table;
}

/////////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLLookup::
MGGeneratorPNNLLookup(const MGGeneratorPNNLLookup &Lookup)
{
  fType         = Lookup.fType;
  fNum_entries  = Lookup.fNum_entries;
  f_pX_table    = NULL;
  f_pDist_table = NULL;
  f_pSum_table  = NULL;

  if (fType == kTYPE_INTERPOLATED_LOOKUP) {
    f_pX_table    = new G4double[fNum_entries];
    f_pDist_table = new G4double[fNum_entries];
    f_pSum_table  = new G4double[fNum_entries];
    
    for (G4int i=0; i<fNum_entries; i++) {
      f_pX_table[i]    = Lookup.f_pX_table[i];
      f_pDist_table[i] = Lookup.f_pDist_table[i];
      f_pSum_table[i]  = Lookup.f_pSum_table[i];
    }
  }
  else {  // kTYPE_DISCRETE_LOOKUP
    f_pDist_table = new G4double[fNum_entries];
    f_pSum_table  = new G4double[fNum_entries+1];
    
    for (G4int i=0; i<fNum_entries; i++) {
      f_pDist_table[i] = Lookup.f_pDist_table[i];
      f_pSum_table[i]  = Lookup.f_pSum_table[i];
    }
    f_pSum_table[fNum_entries] = Lookup.f_pSum_table[fNum_entries];
  }
}

/////////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLLookup::
operator=(const MGGeneratorPNNLLookup& Lookup)
{
  fType         = Lookup.fType;
  fNum_entries  = Lookup.fNum_entries;
  f_pX_table    = NULL;
  f_pDist_table = NULL;
  f_pSum_table  = NULL;

  if (fType == kTYPE_INTERPOLATED_LOOKUP) {
    f_pX_table    = new G4double[fNum_entries];
    f_pDist_table = new G4double[fNum_entries];
    f_pSum_table  = new G4double[fNum_entries];
    
    for (G4int i=0; i<fNum_entries; i++) {
      f_pX_table[i]    = Lookup.f_pX_table[i];
      f_pDist_table[i] = Lookup.f_pDist_table[i];
      f_pSum_table[i]  = Lookup.f_pSum_table[i];
    }
  }
  else {  // kTYPE_DISCRETE_LOOKUP
    f_pDist_table = new G4double[fNum_entries];
    f_pSum_table  = new G4double[fNum_entries+1];
    
    for (G4int i=0; i<fNum_entries; i++) {
      f_pDist_table[i] = Lookup.f_pDist_table[i];
      f_pSum_table[i]  = Lookup.f_pSum_table[i];
    }
    f_pSum_table[fNum_entries] = Lookup.f_pSum_table[fNum_entries];
  }
}

/////////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLLookup::
DoCreateTable(G4int num_entries, G4double* x_arr, G4double* f_arr) 
{

  // This is the version of CreateTable for setting up an interpolated table.

  if (num_entries <= 0) {
    cout << "Error in MGGeneratorPNNLLookup::CreateTable" << endl;
    cout << "CANNOT CREATE TABLE WITH NUM_ENTRIES <=0" << endl;
    cout << "num_entries = " << num_entries << endl;
    cout << "Aborting." << endl;
    exit(1);
  }

  fNum_entries = num_entries;

  if (f_pX_table)
    delete [] f_pX_table;
  if (f_pDist_table)
    delete [] f_pDist_table;
  if (f_pSum_table)
    delete [] f_pSum_table;

  f_pX_table    = new G4double[fNum_entries];
  f_pDist_table = new G4double[fNum_entries];
  f_pSum_table  = new G4double[fNum_entries];

  G4int i, j;

  for (i=0; i<fNum_entries; i++) {
    f_pX_table[i] = x_arr[i];
    f_pDist_table[i] = f_arr[i];
  }

  f_pSum_table[0] = 0.0;

  G4double Sum = 0.0;
  G4double dx, Avg;

  for (i=1; i<fNum_entries; i++) {
    j = i-1;
    Avg = (f_arr[i] + f_arr[j])*0.5;
    dx  = x_arr[i] - x_arr[j];
    Sum += dx * Avg;
    f_pSum_table[i] = Sum;
  }

  for (i=0; i<num_entries; i++)
    f_pSum_table[i] /= Sum;

  /*    
  cout << "MGGeneratorPNNLLookup: Created continuous-x lookup table from input arrays." 
       << endl;
  cout << "Number of table entries: " << fNum_entries << endl;
  cout << "First input (x,f) entry: " << x_arr[0] << " " << f_arr[0] << endl;
  cout << "Last  input (x,f) entry: " << x_arr[fNum_entries-1] << " "
                                      << f_arr[fNum_entries-1] << endl;
  cout << "First table (x,sum) entry: " << f_pX_table[0] << " " 
                                        << f_pSum_table[0] << endl;
  cout << "Last  table (x,sum) entry: " << f_pX_table[num_entries-1] << " " 
                                        << f_pSum_table[num_entries-1] << endl;
  */
}

/////////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLLookup::
DoCreateTable(G4int num_entries, G4double* f_arr) {

  // This version of DoCreateTable is for the discrete lookup table type.

  if (num_entries <= 0) {
    cout << "Error in MGGeneratorPNNLLookup::CreateTable" << endl;
    cout << "CANNOT CREATE TABLE WITH NUM_ENTRIES <=0" << endl;
    cout << "num_entries = " << num_entries << endl;
    cout << "Aborting." << endl;
    exit(1);
  }

  fNum_entries = num_entries;

  if (f_pDist_table)
    delete [] f_pDist_table;
  if (f_pSum_table)
    delete [] f_pSum_table;

  f_pDist_table = new G4double[fNum_entries];
  f_pSum_table  = new G4double[fNum_entries+1];

  G4int i;

  for (i=0; i<fNum_entries; i++) {
    f_pDist_table[i] = f_arr[i];
  }

  f_pSum_table[0] = 0.0;

  G4double Sum = 0.0;

  f_pSum_table[0] = 0.0;

  for (i=0; i<fNum_entries; i++) {
    Sum += f_pDist_table[i];
    f_pSum_table[i+1] = Sum;
  }

  for (i=1; i<fNum_entries+1; i++)
    f_pSum_table[i] /= Sum;
    
  /*
  cout << "MGGeneratorPNNLLookup: Created discrete-index lookup table from input arrays." << endl;
  cout << "Number of table entries: " << fNum_entries << endl;
  cout << "First input distribution table entry: " 
       << f_arr[0] << endl;
  cout << "Last  input distribution table entry: " 
       << f_arr[fNum_entries-1] << endl;
  cout << "First sum table entry: " 
       << f_pSum_table[0] << endl;
  cout << "Last sum table entry: " 
       << f_pSum_table[fNum_entries] << endl;
  */
}

/////////////////////////////////////////////////////////////////////////////
G4double MGGeneratorPNNLLookup::
DoLookupInterp(G4double x_rnd) 
{

  G4int indx = Locate(x_rnd);

  //cout << "DoLookup: x_rnd= " << x_rnd << " indx = " << indx << endl;

  G4double Xtab[2];
  G4double Sum[2];

  Xtab[0] = f_pX_table[indx];
  Xtab[1] = f_pX_table[indx+1];
  Sum[0]  = f_pSum_table[indx];
  Sum[1]  = f_pSum_table[indx+1];

  G4double dX_dSum = (Xtab[1] - Xtab[0])/(Sum[1] - Sum[0]);
  
  G4double x_interp = Xtab[0] + (x_rnd - Sum[0])*dX_dSum;

  return x_interp;

}
/////////////////////////////////////////////////////////////////////////////
G4int MGGeneratorPNNLLookup::
DoLookupDiscrete(G4double x_rnd) 
{

  G4int indx = Locate(x_rnd);

  return indx;

}

/////////////////////////////////////////////////////////////////////////////
G4int MGGeneratorPNNLLookup::
Locate(G4double x) 
{

  // Adapted from Numerical Recipes routine LOCATE for searching of
  // monotonically-ordered table.

  G4int nmax;

  if (fType == kTYPE_INTERPOLATED_LOOKUP)
    nmax = fNum_entries;
  else
    nmax = fNum_entries + 1;

  G4int JL = 0;
  G4int JU = nmax + 1;
  G4int JM;

  G4bool b1 = f_pSum_table[nmax-1] > f_pSum_table[0];
  G4bool b2;

  while ((JU-JL) > 1) {
    JM = (JU + JL)/2;
    b2 = x > f_pSum_table[JM-1];
    if (b1 == b2)
      JL = JM;
    else
      JU = JM;
  }

  return JL-1;
}
