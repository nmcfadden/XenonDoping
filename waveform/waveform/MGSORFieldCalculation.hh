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
 *
 * DESCRIPTION:
 * 	Class containing methods to calculate the electric and the weighting fields
 * 	using Successive Over-Relaxation technique  
 *
 * AUTHOR:
 * 	Daniel Lenz
 *
 * FIRST SUBMISSION: 
 * 
 * REVISION: MM-DD-YYYY
 *    08-29-2008 Now proper separation between fXMaxGridpoints
 *               and fXMaxArrayIndex, devided fXmax to the two new
 *               variables, Daniel Lenz
 *
 *    07-23-2009 started to use CLHEP physics constants, Jing Liu
 *    05-25-2011 Changed function names for consistency, Alex.
 *    05-28-2011 Adapt to new const& storage of crystal data in crystal fields, J. Detwiler
 * 
 */

#ifndef _MGSORFIELDCALCULATION_HH
#define _MGSORFIELDCALCULATION_HH

#include "MGVFieldCalculation.hh"
#include "MGSORCrystalFields.hh"
#include <fstream>

class MGSORFieldCalculation : public MGVFieldCalculation
{
public:
  
  MGSORFieldCalculation();
  MGSORFieldCalculation(MGVCrystalFields* crystalFields);
  ~MGSORFieldCalculation();
  
  
  //Set the Convergence limit, i.e. define at what variations of the fields inside the crystal the calculation should stop. default = 10E-5 The value should be smaller than 1, since it defines at what relative change of the fields the updating should stop. 
  inline void SetConvergenceLimit(double convergence = 10E-5){fConvergenceLimit = convergence;};
  inline double GetConvergenceLimit(){return fConvergenceLimit;};

  //Set Successive Over Relaxation Constant. SOR only converges if constant is between 1 and 2 (depending on the problem even values smaller than 2 will not converge) default is 1.4 
  inline void SetSORConst (double SOR = 1.4) {fSORConst = SOR;};
  inline double GetSORConst(){return fSORConst;};

 
  //Set the charge carrier number density per 1/cm^3 but I need it in 1/m^3
//   inline void SetChargeCarrierNumberDensity(double chargeCarrier){fChargeCarrierNumberDensity = chargeCarrier * 1000000.;}
//   inline const double GetChargeCarrierNumberDensity(){return fChargeCarrierNumberDensity/1000000.;}

  //Further member functions
  void Initialize();
  void CalculateFields();

  //Save Convergence and ConvergenceNormConst to file to see if the alorithm converged
  void SaveConvergenceToFile(char* filename){ fConvergenceFilename = filename; SaveConvergence = true;}




  
private:
  // private functions
  void AllocateArrays();
  void DeAllocateArrays();

  void ResetFields();
 
  //initalize, i.e. set boundary conditions, voltage,...
  void InitializeEPotential();
  void InitializeEPotential_WellType();
  void InitializeWeightingPotential(int segment);
  void InitializeWeightingPotential_WellType(int segment);
  // calculate the boundaries of the segments. Needed in Initialize() and also in UpdatePotential() since we want floating boundaries!
  void CalculateSegBoundaries(int segment);
  //these variables are needed to store individual segment boundaries in gridpoints!
  int fPhiSegment;
  int fiphiLowSegBoundary;
  int fiphiUpSegBoundary;
  int fizLowSegBoundary;
  int fizUpSegBoundary;
  

  //
  void UpdateEPotential();
  void UpdateEPotential_WellType();
  void UpdateWeightingPotential(int segment);
  void UpdateWeightingPotential_WellType(int segment);

  //Derive the fields from potential 
  void PotentialToField();
  void PotentialToField_WellType();
  
  //need to get a value to normalize convergence!!
  double ConvergenceNormalizationConst();

  //to save Convergence to file
  std::ofstream fConvergenceStream;
  char* fConvergenceFilename;
  bool SaveConvergence;

  //Array for the potential and fields
//   double***  fPotential;
//   double**** fFieldArray;
//   double***  fChargeDensity;

  //try to make everthing float
  float***  fPotential;
  float**** fFieldArray;
  float***  fChargeDensity;

  // Charge carrier number density 
  //double fChargeCarrierNumberDensity;

  // Successive Over Relaxation Constant  
  double fSORConst;
  

  //Max. number of grid points
  int fRMaxGridpoints;
  int fPhiMaxGridpoints;
  int fZMaxGridpoints;

  int fRMaxArrayIndex;
  int fPhiMaxArrayIndex;
  int fZMaxArrayIndex;

  // The following variables only for a well-type (kClosedEnd) detectors,
  // specify grid indices of the well in r and z
  int fRBound;
  int fZBound;

  // according to crystal dimensions and max number of grid points the stepsize is calcualted and stored inside fdelR, fdelPhi and fdelZ
  double fdelR;
  double fdelPhi;
  double fdelZ;

  //when to stop calculation
  double fConvergenceLimit;
  
};
#endif
