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
 * 	Class containing methods to store and access the electric and weigthing fields calculated by MGSORFieldCalculation
 *
 * AUTHOR:
 * 	Daniel Lenz
 *
 * FIRST SUBMISSION:
 * 
 * REVISION: MM-DD-YYYY
 * 	08-29-2008 Daniel: Separated fXmax to fXMaxGridpoints and fXMaxArrayIndex 
 * 	10-10-2008 Daniel: Changed LoadFieldFromRoot method.
 *      05-28-2011 Jason:  Adapt to new const& storage of crystal data in crystal fields
 */

#ifndef _MGSORCRYSTALFIELDS_HH
#define _MGSORCRYSTALFIELDS_HH

#include "MGVCrystalFields.hh"
#include <fstream>
#include <iostream>
#include "io/MGLogger.hh"
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"

class MGSORCrystalFields : public MGVCrystalFields
{
public:
  MGSORCrystalFields(const MGCrystalData& crystal);
  MGSORCrystalFields(const MGCrystalData& crystal, int gridrmax, int gridphimax, int gridzmax);
  ~MGSORCrystalFields();
  
  //Store and load functions
  void SaveFields();
  void LoadFields();
  void DrawFields();


//   void SaveFieldToTxt(int rmax, int phimax, int zmax, double**** array, const char* filename);
//   void SaveFieldToRoot(int rmax, int phimax, int zmax, double**** array, const char* filename);
  //try to move everything to float
  void SaveFieldToTxt(int rmax, int phimax, int zmax, float**** array, const char* filename);
  void SaveFieldToRoot(int rmax, int phimax, int zmax, float**** array, const char* filename);
  
  void LoadFieldFromTxt(const char* efieldFilename,const char* wfieldsFilename);
  void LoadFieldFromRoot();

  
  //Methods to access the electric and weighting field
  CLHEP::Hep3Vector GetEField(const CLHEP::Hep3Vector& coordinates)const;
  CLHEP::Hep3Vector GetWField(const CLHEP::Hep3Vector& coordinates,size_t segment)const;
  double GetWPotential(const CLHEP::Hep3Vector& coordinates,size_t segment)const;

  //Define the grid
  //default is 100 100 100
  void SetGrid(int rmax = 100, int phimax=100, int zmax=100);
  //return max number of GridPoints!
  inline double GetRMax(){return (double) fRMaxGridpoints;};
  inline double GetPhiMax(){return (double) fPhiMaxGridpoints;};
  inline double GetZMax(){return (double) fZMaxGridpoints;};
  CLHEP::Hep3Vector GetGrid();
  
  //Return pointer to Array
  //  double**** GetArray();
  //try to make everything to float
  float**** GetArray();
  void ResetFields();
  
  //Initialize
  void Initialize();
  
  //Sets name of electric field file, including path to it
  // first function to call after constructor
  //  void SetFileName(char* name);
  inline void SetFileName(std::string name){fFileName.assign(name);};
  inline void AppendFileName(std::string append){fAppendix.assign(append);};
  inline std::string GetFileName(){return fFileName;};


private:

  CLHEP::Hep3Vector Interpolation(CLHEP::Hep3Vector coordinates) const;
  CLHEP::Hep3Vector InterpolationWField(CLHEP::Hep3Vector coordinates, size_t segment) const;
  double InterpolationWPotential(CLHEP::Hep3Vector coordinates, size_t segment) const;
  
  void AllocateArrays();
  void DeAllocateArrays();

  void ReadHeader();
  //  void ReadArray(int rmax, int phimax, int zmax, int segment);
    
  //open and close files
  bool OpenFile(const char* name);
  bool CloseFile();

  //coordinate transformation, such that field values are given from the correct position
  CLHEP::Hep3Vector CoordinateTransform(CLHEP::Hep3Vector coordinates) const;
  
  
//   double**** fEField;
//   double***** fWField;
  //try to make everything to float
  float**** fEField;
  float***** fWField;

  std::fstream fDataStream; 

  int fRMaxGridpoints;
  int fPhiMaxGridpoints;
  int fZMaxGridpoints;

  int fRMaxArrayIndex;
  int fPhiMaxArrayIndex;
  int fZMaxArrayIndex;


  double fdelR;
  double fdelPhi;
  double fdelZ;

  int fSegments;

  //segmentationangle needed to rotate field to correct position
  //double fSegmentationAngle;

  //filename
  std::string fFileName;
  std::string fAppendix;

  //to save into root file
  TFile* fFieldsOutputFile;
  TTree* fFieldsOutputTree;
  


};
#endif
