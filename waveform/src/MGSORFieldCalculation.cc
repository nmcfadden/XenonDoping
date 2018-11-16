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
 * 	Class containing simple way to calculate the fields
 * 	The field is calculated using "Successive Over Relaxation" (SOR). 
 *
 * AUTHOR:
 *	Daniel Lenz
 *
 * FIRST SUBMISSION:
 * 
 * REVISION: MM-DD-YYYY
 *   08-19-2008  Daniel: Mapped the grid only on interesting r range. r_outer - r_inner : r = (ir *delR) => r = (ir * delR) + r_inner
 *   08-20-2008  Daniel: Changed segmentation in InitializeWeightingPotential
 *   08-20-2008  Daniel: Fixed bug that didn't take into account VZMinus in the update of WP for segments 
 *   08-29-2008  Daniel: Now proper separation between fXMaxGridpoints and fXMaxArrayIndex 
 *   07-23-2009  Jing:   made the calculation free of units
 *   05-25-2011  Alex:   1. UpdateWPotential did the same for the core and segment, cleaned up
 *                       2. Changed function names for consistency
 *                       3. Total clean up of the code: often-used variables moved out from loops,
 *                          removed never-satisfied conditions
 *                          The code tested to produce the same results as before for true-coax detectors!!
 *                       4. Introduced (partially duplicated) functions for a well-type detector:
 *                            InitializeEPotential_WellType();
 *                            InitializeWeightingPotential_WellType(int segment);
 *                            UpdateEPotential_WellType();
 *                            UpdateWeightingPotential_WellType(int segment);
 *                            PotentialToField_WellType();
 *   05-28-2011  Jason:  Adapt to new const& storage of crystal data in crystal fields.
 *   07-23-2011  Alex:   Bug fix in well-type WPotential calculation;
 *                       Normalization constant modified for a well-type detector
 *   09-14-2011, Alex:   Changed the Z-ranges of the crystal to -height/2 .. height/2
 *                       instead of 0 .. height for impuritues calculation
 *   12-19-2011, Alex:   Bug fixes:
 *                       1. Phi component of the field should be divided by r;
 *                       2. iphi=fPhiMaxArrayIndex is skipped during the SOR calculation
 *                          to avoid double-calculation for iphi=0. In the loop, always [iphi%fPhiMaxArrayIndex]
 *                          is used, so the index [fPhiMaxArrayIndex] is never reached but rather [0] is used twice).
 *
 *
 *
 */

#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

#include "waveform/MGSORFieldCalculation.hh"

using namespace std;
using namespace CLHEP;

MGSORFieldCalculation::MGSORFieldCalculation() : MGVFieldCalculation()
{
  cout.precision(8);
  fPotential = NULL;
  fChargeDensity = NULL;
  fFieldArray = NULL;
} 
//=============================================================================
MGSORFieldCalculation::MGSORFieldCalculation(MGVCrystalFields* crystalFields)
: MGVFieldCalculation(crystalFields)
{
  cout.precision(8);
  fPotential = NULL;
  fChargeDensity = NULL;
  fFieldArray = NULL;
}
//=============================================================================
MGSORFieldCalculation::~MGSORFieldCalculation()
{ //delete all arrays in use
  if(fPotential!=NULL && fChargeDensity!=NULL && fFieldArray!= NULL)
     DeAllocateArrays();
}
//=============================================================================
void MGSORFieldCalculation::Initialize()
{
  MGLog(debugging)<<"initializing SOR field calculator..."<<endlog;

  MGSORCrystalFields* fields = dynamic_cast<MGSORCrystalFields*> (fCrystalFields);

  Hep3Vector grid = fields->GetGrid();

  fRMaxGridpoints = (int)grid.x();
  fPhiMaxGridpoints = (int)grid.y();
  fZMaxGridpoints = (int)grid.z();

  MGLog(debugging)<<"grid size (r, phi, z): ("
                  <<fRMaxGridpoints<<", "
                  <<fPhiMaxGridpoints<<", "
                  <<fZMaxGridpoints<<")"
                  <<endlog;

  fRMaxArrayIndex = fRMaxGridpoints-1;
  fPhiMaxArrayIndex = fPhiMaxGridpoints-1;
  fZMaxArrayIndex = fZMaxGridpoints-1;

  fFieldArray = fields->GetArray();

  const MGCrystalData& crystal = fCrystalFields->GetCrystalData();

  // define step width
  //step width is: Radius/ (maxgridpoints-1) 
  //since: *-*-* :3 gridpoints, but divide whole range only by 2!!
  double Router = crystal.GetOuterRadius();
  double Rinner = crystal.GetInnerRadius();
  double Height = crystal.GetCrystalHeight();

  MGCrystalData::EGeometryType type = crystal.GetGeometryType();
  if   (type==MGCrystalData::kCoaxial)   fdelR   = (Router - Rinner) / (fields->GetRMax()-1);
  else if (type==MGCrystalData::kClosedEnd) fdelR   = Router / (fields->GetRMax()-1);
  else MGLog(error)<<"Not supported geometry type. Exit now..."<<endlog;

  fdelPhi = 2*M_PI / (fields->GetPhiMax()-1);
  fdelZ   = Height / (fields->GetZMax()-1);

  MGLog(debugging)<<"delta R   = "<<fdelR/mm<<"*mm"<<endlog;
  MGLog(debugging)<<"delta Phi = "<<fdelPhi/deg<<"*degree"<<endlog;
  MGLog(debugging)<<"delta Z   = "<<fdelZ/mm<<"*mm"<<endlog;

  //Allocate the necessary arrays
  AllocateArrays();

  // initializeEPotenial, i.e. setup boundary conditions and make initial guess
  // of potential
  InitializeEPotential();

  MGLog(debugging)<<"initializing SOR field calculator (done)\n"<<endlog;
}
//=============================================================================
void MGSORFieldCalculation::CalculateFields()
{

  if (fCrystalFields->GetCrystalData().GetGeometryType()==MGCrystalData::kCoaxial) {
    InitializeEPotential();
    UpdateEPotential();
//    PotentialToField();
  }
  else {
    InitializeEPotential_WellType();
    UpdateEPotential_WellType();
  }

  PotentialToField();
  dynamic_cast<MGSORCrystalFields*> (fCrystalFields)->AppendFileName("_EField.dat");
  fCrystalFields->SaveFields();
  ResetFields();
  dynamic_cast<MGSORCrystalFields*> (fCrystalFields)->ResetFields();

  size_t Nsegs = fCrystalFields->GetCrystalData().GetNSegments();
  for(size_t segment=0;segment<=Nsegs;segment++){
    CalculateSegBoundaries(segment);
    if (fCrystalFields->GetCrystalData().GetGeometryType()==MGCrystalData::kCoaxial) {
      InitializeWeightingPotential(segment);
      UpdateWeightingPotential(segment);
    }
    else {
      InitializeWeightingPotential_WellType(segment);
      UpdateWeightingPotential_WellType(segment);
    }
    PotentialToField();

    // to save to extra file
    ostringstream os;
    os << "_WField_Segment_" << segment << ".dat";
    string segmentName(os.str());
    dynamic_cast<MGSORCrystalFields*> (fCrystalFields)->AppendFileName(segmentName);
    fCrystalFields->SaveFields();
    ResetFields();
    dynamic_cast<MGSORCrystalFields*> (fCrystalFields)->ResetFields();
  }

}
//=============================================================================
//private functions from here on
//=============================================================================
void MGSORFieldCalculation::AllocateArrays()
{
  //Allocate the potential and charge density array 
  //  Always allocate one more than max steps!!!
  fPotential = NULL;
  fChargeDensity = NULL;

//   fPotential     = new double**[fRMaxGridpoints];
//   fChargeDensity = new double**[fRMaxGridpoints];

//   for(int i=0;i<=fRMaxArrayIndex;i++){
//     fPotential[i] = new double*[fPhiMaxGridpoints];
//     fChargeDensity[i] = new double*[fPhiMaxGridpoints];

//     for(int j=0;j<=fPhiMaxArrayIndex;j++){
//       fPotential[i][j] = new double[fZMaxGridpoints];
//       fChargeDensity[i][j] = new double[fZMaxGridpoints];
//     }
//   }

  //try to make everything float
  fPotential     = new float**[fRMaxGridpoints];
  fChargeDensity = new float**[fRMaxGridpoints];

  for(int i=0;i<=fRMaxArrayIndex;i++){
    fPotential[i] = new float*[fPhiMaxGridpoints];
    fChargeDensity[i] = new float*[fPhiMaxGridpoints];

    for(int j=0;j<=fPhiMaxArrayIndex;j++){
      fPotential[i][j] = new float[fZMaxGridpoints];
      fChargeDensity[i][j] = new float[fZMaxGridpoints];
    }
  }
}
//=============================================================================
void MGSORFieldCalculation::DeAllocateArrays()
{

  //when deleting take good care to delete in the correct way
  for(int i=fRMaxArrayIndex; i>=0; i--){
    for(int j=fPhiMaxArrayIndex; j>=0; j--){
      delete[] fPotential[i][j];
      delete[] fChargeDensity[i][j];
    }
    delete[] fPotential[i];
    delete[] fChargeDensity[i];
  }
  delete[] fPotential;
  delete[] fChargeDensity;
}
//=============================================================================
void MGSORFieldCalculation::ResetFields()
{
  //After each save of fields reset them!
  for(int i=0;i<=fRMaxArrayIndex;i++){
    for(int j=0;j<=fPhiMaxArrayIndex;j++){
      for(int k=0;k<=fZMaxArrayIndex;k++){
        fPotential[i][j][k] = 0;
      }
    }
  }
  
}
//=============================================================================
void MGSORFieldCalculation::InitializeEPotential()
{
  MGLog(debugging)<<"initializing electrical potential..."<<endlog;

  const MGCrystalData& crystal = fCrystalFields->GetCrystalData();
  double innerRadius = crystal.GetInnerRadius();
  double voltage = crystal.GetVoltage();
  MGLog(debugging)<<"voltage got from crystal data: "
     <<voltage/volt<<"*volt"<<endlog;

  ///////////////////////////////
  //Impurity density calculation
  ///////////////////////////////
  double r;
  double phi;
  double z;

  //sign needed for the body charge (charge density), 
  //n-type = holes => + , p-type = electrons => -
  int sign = 0;
  // == 0 means kPType 
  if(crystal.GetDiodeType() == 0) { sign = 1;}
  // == 1 means kNType 
  if(crystal.GetDiodeType() == 1) { sign = -1;}

  for( int ir=0; ir<=fRMaxArrayIndex; ir++ ){
    for( int iphi=0; iphi<=fPhiMaxArrayIndex; iphi++ ){
      for( int iz=0; iz<=fZMaxArrayIndex; iz++ ){

        // Get the  impurity density from crystal Data
        r   = ir  *fdelR + innerRadius;
        phi = iphi*fdelPhi;
        // Here z=0 means the center of the detector and only used for impurity calculation
        z   = iz  *fdelZ - crystal.GetCrystalHeight()/2.;
        Hep3Vector pos;
        pos.setRhoPhiZ(r,phi,z);

        float NChargeCarrier = 0;
        NChargeCarrier = crystal.GetImpurityDensity(pos);

        //MGLog(debugging)<<"impurity density at (r="
        //   <<pos.getRho()/mm<<"*mm, phi="
        //   <<pos.getPhi()/deg<<"*degree, z="
        //   <<pos.getZ()/mm<<"*mm) = "
        //   <<NChargeCarrier/(1./cm3)
        //   <<"/cm3"<<endlog;

        fChargeDensity[ir][iphi%fPhiMaxArrayIndex][iz]
                                                   = sign * electron_charge * NChargeCarrier;

        //apply boundary conditions to the fields
        if(ir == 0) //if on inner boundary apply potential
          fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = voltage;
        else if (ir==fRMaxArrayIndex) //on outer boundary apply ground
          fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 0;
        else //inside crystal
          fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 
            fPotential[ir-1][iphi%fPhiMaxArrayIndex][iz] * 
            (fRMaxArrayIndex - ir)/(float)fRMaxArrayIndex; //initial guess
      }
    }
  }
  MGLog(debugging)<<"initializing electrical potential (done)\n"<<endlog;
}
//=============================================================================
void MGSORFieldCalculation::InitializeEPotential_WellType()
{

  MGLog(debugging)<<"initializing electrical potential..."<<endlog;

  const MGCrystalData& crystal = fCrystalFields->GetCrystalData();
  double innerRadius = crystal.GetInnerRadius();
  double height = crystal.GetCrystalHeight();
  double voltage = crystal.GetVoltage();
  MGLog(debugging)<<"voltage got from crystal data: "
     <<voltage/volt<<"*volt"<<endlog;

  ///////////////////////////////
  //Impurity density calculation
  ///////////////////////////////
  double r;
  double phi;
  double z;

  //sign needed for the body charge (charge density),
  //n-type = holes => + , p-type = electrons => -
  int sign = 0;
  // == 0 means kPType
  if(crystal.GetDiodeType() == 0) { sign = 1;}
  // == 1 means kNType
  if(crystal.GetDiodeType() == 1) { sign = -1;}

  double wellDepth = crystal.GetWellDepth();
  fRBound = int( innerRadius / fdelR );
  // Index in z is calculated, considering the well is from the top,
  // also therefore use ceil
  fZBound = int (ceil( ( height - wellDepth ) / fdelZ ));


  for(int ir=0;ir<=fRMaxArrayIndex;ir++){
     for(int iphi=0;iphi<=fPhiMaxArrayIndex;iphi++){
        for(int iz=0;iz<=fZMaxArrayIndex;iz++){

           // Get the  impurity density from crystal Data
           r   = ir  *fdelR;
           phi = iphi*fdelPhi;
           // Here z=0 means the center of the detector and only used for impurity calculation
           z   = iz  *fdelZ - crystal.GetCrystalHeight()/2.;
           Hep3Vector pos;
           pos.setRhoPhiZ(r,phi,z);

           float NChargeCarrier = 0;
           NChargeCarrier = crystal.GetImpurityDensity(pos);

           //MGLog(debugging)<<"impurity density at (r="
           //   <<pos.getRho()/mm<<"*mm, phi="
           //   <<pos.getPhi()/deg<<"*degree, z="
           //   <<pos.getZ()/mm<<"*mm) = "
           //   <<NChargeCarrier/(1./cm3)
           //   <<"/cm3"<<endlog;

           fChargeDensity[ir][iphi%fPhiMaxArrayIndex][iz]
              = sign * electron_charge * NChargeCarrier;

           //apply boundary conditions to the fields
           if(ir == fRBound && iz>=fZBound) { //if on inner boundary apply potential
              fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = voltage;
              continue;
           }
           if(ir <= fRBound && iz==fZBound) { //if on the bottom of the well apply potential
              fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = voltage;
              continue;
           }
           if(ir==fRMaxArrayIndex) {//on outer boundary apply ground
              fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 0;
              continue;
           }
           if(iz==0) {//on the bottom also apply ground
              fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 0;
              continue;
           }
           if ( ir==0 ||                        // at r=0 or ...
               (ir <= fRBound && iz<fZBound) ) {// ... between the groove and the 0 potential apply linear approximation
             fPotential[ir][iphi%fPhiMaxArrayIndex][iz]
                   = voltage * float(iz) / float(fZBound);
             continue;
           }
           // In the crystal:
           fPotential[ir][iphi%fPhiMaxArrayIndex][iz]
                 = fPotential[ir-1][iphi%fPhiMaxArrayIndex][iz]
                 * (fRMaxArrayIndex - ir)/(float)fRMaxArrayIndex; //initial guess

        }
     }
  }
  MGLog(debugging)<<"initializing electrical potential (done)\n"<<endlog;
}
//=============================================================================
void MGSORFieldCalculation::CalculateSegBoundaries(int segment)
{
  //WHAT WILL HAPPEN IF THERE ARE NO SEGMENTS !!! ???
  //Everything should be ok since there will be only a segment 0 = core!

  MGLog(debugging)<<"getting grid indecs on segment "
     <<segment<<" boundaries..."<<endlog;

  //Calculate the segment boundary in grid points
  int NsegPhi=fCrystalFields->GetCrystalData().GetNPhiSegments();
  int NsegZ  =fCrystalFields->GetCrystalData().GetNZSegments();
  //phi: 
  //  double PhiSegmentSpanningAngleInGridPoints = ((float)fPhiMaxGridpoints/(float)fCrystalFields->GetCrystalData().GetNPhiSegments());
  double PhiSegmentSpanningAngleInGridPoints = ((float)fPhiMaxArrayIndex/(float)NsegPhi);
  fPhiSegment = int ((segment - 1) / (float)NsegZ);
  

  fiphiLowSegBoundary = int ((fPhiSegment) * PhiSegmentSpanningAngleInGridPoints);
  //    iphiUpSegBoundary = int (((PhiSegment + 1) * PhiSegmentSpanningAngleInGridPoints)-1);
  fiphiUpSegBoundary = int (((fPhiSegment + 1) * PhiSegmentSpanningAngleInGridPoints));

  MGLog(debugging)<<" fPhiMaxArrayIndex = "<<fPhiMaxArrayIndex<<endlog;
  MGLog(debugging)<<" PhiSegmentSpanningAngleInGridPoints = "
     <<PhiSegmentSpanningAngleInGridPoints<<endlog;
  MGLog(debugging)<<" fPhiSegment = "<<fPhiSegment<<endlog;
  MGLog(debugging)<<" fiphiLowSegBoundary = "<<fiphiLowSegBoundary<<endlog;
  MGLog(debugging)<<" fiphiUpSegBoundary = "<<fiphiUpSegBoundary<<endlog;

  //z:
  double ZSegmentHeightInGridPoints = ((float)fZMaxArrayIndex/(float)NsegZ);
  //double ZSegmentHeightInGridPoints = ((float)fZMaxGridpoints/(float)NsegZ);
  int ZSegment = (segment - fPhiSegment*NsegZ - 1);

  // Segment counting from Top to bottom 
  //  fizUpSegBoundary = int( fZMaxArrayIndex - (ZSegment * ZSegmentHeightInGridPoints));
  fizUpSegBoundary  = int(fZMaxArrayIndex - (ZSegment * ZSegmentHeightInGridPoints));
  fizLowSegBoundary = int(fZMaxArrayIndex - (((ZSegment + 1) * ZSegmentHeightInGridPoints)));

  MGLog(debugging)<<" fZMaxGridpoints = "<<fZMaxGridpoints<<endlog;
  MGLog(debugging)<<" fZMaxArrayIndex = "<<fZMaxArrayIndex<<endlog;
  MGLog(debugging)<<" ZSegmentHeightInGridpoints = "
     <<ZSegmentHeightInGridPoints<<endlog;
  MGLog(debugging)<<" ZSegment = "<<ZSegment<<endlog;
  MGLog(debugging)<<" fizLowSegBoundary = "<<fizLowSegBoundary<<endlog;
  MGLog(debugging)<<" fizUpSegBoundary = "<<fizUpSegBoundary<<endlog;

  MGLog(debugging)<<"getting grid indecs on segment "
     <<segment<<" boundaries (done)"<<endlog;
}
//=============================================================================
void MGSORFieldCalculation::InitializeWeightingPotential(int segment)
{
  if(segment == 0){//core
    for(int ir=0;ir<=fRMaxArrayIndex;ir++){
      for(int iphi=0;iphi<=fPhiMaxArrayIndex;iphi++){
        for(int iz=0;iz<=fZMaxArrayIndex;iz++){
          if( ir ==  0 ) {
            //weighting potential boundary condition
            fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 1;
            continue;
          }
          //inside crystal
          fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 0;
        }
      }
    }
    return;
  }

  else{//all segments
    for( int ir=0; ir<=fRMaxArrayIndex; ir++ ){
      for( int iphi=0; iphi<=fPhiMaxArrayIndex; iphi++ ){
        for( int iz=0; iz<=fZMaxArrayIndex; iz++ ){
          if( ir== fRMaxArrayIndex && iphi >= fiphiLowSegBoundary && iphi <= fiphiUpSegBoundary && iz >= fizLowSegBoundary && iz<=fizUpSegBoundary){//only segments are set to 1 volt
            if(fPhiSegment == 0 && iphi != fPhiMaxArrayIndex) //to prevent from overwriting first entry again!!!
              fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 1; //weighting potential boundary condition

            if(fPhiSegment != 0)//to prevent from overwriting first entry again!!!
              fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 1; //weighting potential boundary condition

            //!!! DEBUG
            //      cout<<"Phi Segment is = "<<fPhiSegment<<endl;
            if((unsigned)fPhiSegment ==  fCrystalFields->GetCrystalData().GetNPhiSegments()-1){//to prevent from overwriting first entry again!!!
              // DEBUG!!!
              //        cout<<"We are at r:phi:z "<<ir<<iphi<<iz<<endl;
              if(iphi != fPhiMaxArrayIndex)
                fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 1; //weighting potential boundary condition
              else{;}//if it is max arrayIndex, don't do anything!!
            }

            //keep this as last step
            if( fCrystalFields->GetCrystalData().GetNSegments() > 1 && (iphi == fiphiLowSegBoundary || iphi == fiphiUpSegBoundary || iz == fizLowSegBoundary || iz == fizUpSegBoundary)){//only boundary points are set to 0.5
              fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 0.5; //weighting potential on boundary condition
//        cout<<"Indeed set to 0.5"<<endl;
//        cout<<"r:phi:z"<<ir<<":"<<iphi<<":"<<iz<<" fPot = "<<fPotential[ir][iphi%fPhiMaxArrayIndex][iz]<<endl;
            }
          }
          else{
            if(fPhiSegment == 0 && iphi != fPhiMaxArrayIndex) //to prevent from overwriting first entry again!!!
              fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 0;
            if(fPhiSegment != 0)//to prevent from overwriting first entry again!!!
              fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 0;
          }
        }
      }
    }
  }
}
//=============================================================================
void MGSORFieldCalculation::InitializeWeightingPotential_WellType(int segment)
{

  // for a well-type:
  if(segment == 0){//core
    for( int ir=0; ir<=fRMaxArrayIndex; ir++ ){
      for( int iphi=0; iphi<=fPhiMaxArrayIndex; iphi++ ){
        for( int iz=0; iz<=fZMaxArrayIndex; iz++ ){
          if(ir == fRBound && iz>=fZBound) {
            //if on inner boundary apply potential
            fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 1;
            continue;
          }
          if(ir <= fRBound && iz==fZBound) {
            //if on the bottom of the well apply potential
            fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 1;
            continue;
          }
          // inside the crystal
          fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 0;
        }
      }
    }
  }
  else { //the only "segment"
    for( int ir=0; ir<=fRMaxArrayIndex; ir++ ){
      for( int iphi=0; iphi<=fPhiMaxArrayIndex; iphi++ ){
        for( int iz=0; iz<=fZMaxArrayIndex; iz++ ){
          if( ( ir   == fRMaxArrayIndex || iz==0) && //only segment is set to 1 volt
              ( iphi != fPhiMaxArrayIndex)           ){ //to prevent from overwriting first entry again!!!
              fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 1; //weighting potential boundary condition
              continue;
          }
          else{
            if(iphi != fPhiMaxArrayIndex) //to prevent from overwriting first entry again!!!
              fPotential[ir][iphi%fPhiMaxArrayIndex][iz] = 0;
          }
        }
      }
    }
  }
} 
//=============================================================================
void MGSORFieldCalculation::UpdateEPotential()
{
  //If save convergence to file
  if(SaveConvergence){
    char tmpFilename[1000];
    sprintf(tmpFilename,"%s_EField.dat",fConvergenceFilename);
    fConvergenceStream.open(tmpFilename);
    fConvergenceStream.precision(8);
  }

  double innerRadius = fCrystalFields->GetCrystalData().GetInnerRadius();
  cout<<endl<<"Updating Potential "<<endl;

  double convergence = 0;
  double convergenceNormConst = 0;
  //   double oldPotential;
  //   double updatedPotential;

  //try to make everything float
  float oldPotential;
  float updatedPotential;

  double epsilonGe = 16;

  float VRPlus    = 0;
  float VRMinus   = 0;
  float VPhiPlus  = 0;
  float VPhiMinus = 0;
  float VZPlus    = 0;
  float VZMinus   = 0;
  float r = 0;
  float c = 0;

  int loops = 0;
  while(convergence >= fConvergenceLimit || loops < 300){
    loops++;

    convergence = 0.;
    convergenceNormConst = ConvergenceNormalizationConst();
    if(loops%50 == 0){cout<<"*"<<flush;}

    // ******************************
    // Note loop on r = 1..fRMaxArrayIndex-1
    // The boundary values should not change
    // ******************************
    for( int ir=1; ir<=fRMaxArrayIndex-1; ir++ ){
      for( int iphi=0; iphi<fPhiMaxArrayIndex; iphi++ ){
        for( int iz=0; iz<=fZMaxArrayIndex; iz++ ){
          r = (ir * fdelR) + innerRadius;

          // if along r inside the crystal and not on the boundary
          //shouldn't it be fRmax + 1 ? no since fRmax = max array index ie.
          //100 but this value should not be updated but kept unchanged!

          VRPlus    = 0;
          VRMinus   = 0;
          VPhiPlus  = 0;
          VPhiMinus = 0;
          VZPlus    = 0;
          VZMinus   = 0;
          c = -2./(fdelR * fdelR)
              -2./(r * r * fdelPhi * fdelPhi)
              -2./(fdelZ * fdelZ);

          VRMinus = fPotential[ir-1][iphi%fPhiMaxArrayIndex][iz];
          VRPlus  = fPotential[ir+1][iphi%fPhiMaxArrayIndex][iz];


          if(iphi==0) //phi == 0 apply periodic boundary conditions
               VPhiMinus = fPotential[ir][fPhiMaxArrayIndex-1][iz]; //-1 important, since fPhiMaxArrayIndex is == to [0]!
          else VPhiMinus = fPotential[ir][iphi-1][iz];

          VPhiPlus = fPotential[ir][(iphi+1)%fPhiMaxArrayIndex][iz];


          if(iz==0) //iz == 0 use same potential value; no periodic boundary
               VZMinus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
          else VZMinus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz-1];

          if(iz==fZMaxArrayIndex) //iz == izmax use same potential value; no peridic boundary
               VZPlus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
          else VZPlus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz+1];

          //this is the real updating step;
          //save old value
          oldPotential = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];

          // update according to algorithm
          updatedPotential = (1/c) * (
                (-fChargeDensity[ir][iphi%fPhiMaxArrayIndex][iz]/(epsilon0 * epsilonGe))
              - (VZPlus + VZMinus)/(fdelZ * fdelZ)
              - (VPhiMinus + VPhiPlus)/(r*r*fdelPhi*fdelPhi)
              - (2.*r-fdelR)*VRMinus/(2.*r*fdelR*fdelR)
              - (2.*r+fdelR)*VRPlus/(2.*r*fdelR*fdelR) );

          // Successive over relaxation: Difference between potential *
          // OverRelaxationConst + old potential
          fPotential[ir][iphi%fPhiMaxArrayIndex][iz] =
              fSORConst * (updatedPotential - oldPotential) + oldPotential;

          //check for convergence
          convergence +=
              fabs(oldPotential - fPotential[ir][iphi%fPhiMaxArrayIndex][iz]) / convergenceNormConst;
        }
      }
    }
    if(SaveConvergence)
      fConvergenceStream<<convergence<<"\t"<<convergenceNormConst<<endl;
  }
  cout<<endl<<"FINISHED Updating Potential at convergence "<<convergence<<endl;
  if(SaveConvergence) fConvergenceStream.close();
}
//=============================================================================
void MGSORFieldCalculation::UpdateEPotential_WellType()
{
  //If save convergence to file
  if(SaveConvergence){
    char tmpFilename[1000];
    sprintf(tmpFilename,"%s_EField.dat",fConvergenceFilename);
    fConvergenceStream.open(tmpFilename);
    fConvergenceStream.precision(8);
  }

  cout<<endl<<"Updating Potential "<<endl;

  double convergence = 0;
  double convergenceNormConst = 0;
  //   double oldPotential;
  //   double updatedPotential;

  //try to make everything float
  float oldPotential;
  float updatedPotential;

  double epsilonGe = 16;

  float VRPlus    = 0;
  float VRMinus   = 0;
  float VPhiPlus  = 0;
  float VPhiMinus = 0;
  float VZPlus    = 0;
  float VZMinus   = 0;
  float r = 0;
  float c = 0;

  int loops = 0;
  while(convergence >= fConvergenceLimit || loops < 300){
    loops++;

    convergence = 0.;
    convergenceNormConst = ConvergenceNormalizationConst();
    if(loops%50 == 0){cout<<"*"<<flush;}

    // ******************************
    // Note loop on r = 1..fRMaxArrayIndex-1
    // and z from 1 !!
    // ******************************
    for( int ir=1; ir<=fRMaxArrayIndex-1; ir++ ){
      for( int iphi=0; iphi<fPhiMaxArrayIndex; iphi++ ){
        for( int iz=1; iz<=fZMaxArrayIndex; iz++ ){

          if(ir <= fRBound && iz >= fZBound)
          {
            // skip the area in the well - don't care about that
            continue;
          }

          r = ir * fdelR;

          VRPlus    = 0;
          VRMinus   = 0;
          VPhiPlus  = 0;
          VPhiMinus = 0;
          VZPlus    = 0;
          VZMinus   = 0;
          c = -2./(fdelR * fdelR)
              -2./(r * r * fdelPhi * fdelPhi)
              -2./(fdelZ * fdelZ);

          if(ir==1)
               VRMinus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
          else VRMinus = fPotential[ir-1][iphi%fPhiMaxArrayIndex][iz];

          VRPlus = fPotential[ir+1][iphi%fPhiMaxArrayIndex][iz];


          if(iphi==0) //phi == 0 apply periodic boundary conditions
               VPhiMinus = fPotential[ir][fPhiMaxArrayIndex-1][iz]; //-1 important, since fPhiMaxArrayIndex is == to [0]!
          else VPhiMinus = fPotential[ir][iphi-1][iz];

          VPhiPlus = fPotential[ir][(iphi+1)%fPhiMaxArrayIndex][iz];


          VZMinus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz-1];

          if(iz==fZMaxArrayIndex)
               VZPlus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
          else VZPlus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz+1];


          //this is the real updating step;
          //save old value
          oldPotential = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];

          // update according to algorithm
          updatedPotential = (1/c) * (
                (-fChargeDensity[ir][iphi%fPhiMaxArrayIndex][iz]/(epsilon0 * epsilonGe))
              - (VZPlus + VZMinus)/(fdelZ * fdelZ)
              - (VPhiMinus + VPhiPlus)/(r*r*fdelPhi*fdelPhi)
              - (2.*r-fdelR)*VRMinus/(2.*r*fdelR*fdelR)
              - (2.*r+fdelR)*VRPlus/(2.*r*fdelR*fdelR) );

          // Successive over relaxation: Difference between potential *
          // OverRelaxationConst + old potential
          fPotential[ir][iphi%fPhiMaxArrayIndex][iz] =
              fSORConst * (updatedPotential - oldPotential) + oldPotential;

          //check for convergence
          convergence +=
              fabs(oldPotential-fPotential[ir][iphi%fPhiMaxArrayIndex][iz]) / convergenceNormConst;
        }
      }
    }
    if(SaveConvergence)
      fConvergenceStream<<convergence<<"\t"<<convergenceNormConst<<endl;
  }

  // Now have to fill the values at r=0:
  // just the same values, since the electrical field is vertical here
  // because of the phi-symmetry, therefore potential does not change in r
  for( int iphi=0; iphi<fPhiMaxArrayIndex; iphi++ )
    for( int iz=1; iz<=fZBound-1; iz++ )
      fPotential[0][iphi%fPhiMaxArrayIndex][iz] = fPotential[1][iphi%fPhiMaxArrayIndex][iz];

  cout<<endl<<"FINISHED Updating Potential at convergence "<<convergence<<endl;
  if(SaveConvergence) fConvergenceStream.close();

}
//=============================================================================
void MGSORFieldCalculation::UpdateWeightingPotential(int segment)
{
  //If save convergence to file
  if(SaveConvergence){
    char tmpFilename[1000];
    sprintf(tmpFilename,"%s_Segment_%i.dat",fConvergenceFilename,segment);
    fConvergenceStream.open(tmpFilename);
  }

  double innerRadius = fCrystalFields->GetCrystalData().GetInnerRadius();

  cout<<endl<<"Updating Weighting Potential for Segment "<<segment<<endl;
  double convergence = 0.;
  double convergenceNormConst = 0;
  float oldPotential;
  float updatedPotential;
  int loops = 0;

  float VRPlus    = 0;
  float VRMinus   = 0;
  float VPhiPlus  = 0;
  float VPhiMinus = 0;
  float VZPlus    = 0;
  float VZMinus   = 0;
  float r = 0;
  float c = 0;

  while(convergence >= fConvergenceLimit || loops < 300){
    loops++;
    if(loops%50 == 0){cout<<"*"<<flush;}
    convergence = 0.;
    convergenceNormConst = ConvergenceNormalizationConst();

    // ******************************
    // Note loop on r = 1..fRMaxArrayIndex-1
    // The boundary values should not change
    // ******************************
    for( int ir=1; ir<=fRMaxArrayIndex-1; ir++ ){
      for( int iphi=0; iphi<fPhiMaxArrayIndex; iphi++ ){
        for( int iz=0; iz<=fZMaxArrayIndex; iz++ ){
          r = (ir * fdelR) + innerRadius;

          VRPlus    = 0;
          VRMinus   = 0;
          VPhiPlus  = 0;
          VPhiMinus = 0;
          VZPlus    = 0;
          VZMinus   = 0;
          c = -2./(fdelR * fdelR)
              -2./(r * r * fdelPhi * fdelPhi)
              -2./(fdelZ * fdelZ);

          VRMinus = fPotential[ir-1][iphi%fPhiMaxArrayIndex][iz];
          VRPlus  = fPotential[ir+1][iphi%fPhiMaxArrayIndex][iz];

          if(iphi==0) //phi == 0 apply periodic boundary conditions
               VPhiMinus = fPotential[ir][fPhiMaxArrayIndex-1][iz]; //-1 important, since fPhiMaxArrayIndex is == to [0]!
          else VPhiMinus = fPotential[ir][iphi-1][iz];

          VPhiPlus = fPotential[ir][(iphi+1)%fPhiMaxArrayIndex][iz];

          if(iz==0) //iz == 0 use same potential value; no periodic boundary
               VZMinus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
          else VZMinus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz-1];

          if(iz==fZMaxArrayIndex) //iz == izmax use same potential value; no peridic boundary
               VZPlus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
          else VZPlus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz+1];

          //this is the real updating step;
          //save old value
          oldPotential = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];

          // update according to algorithm
          // almost as for the E potential, except no charge density
          updatedPotential = (1/c) * (
              - (VZPlus + VZMinus)/(fdelZ * fdelZ)
              - (VPhiMinus + VPhiPlus)/(r*r*fdelPhi*fdelPhi)
              - (2.*r-fdelR)*VRMinus/(2.*r*fdelR*fdelR)
              - (2.*r+fdelR)*VRPlus/(2.*r*fdelR*fdelR) );

          // Successive over relaxation: Difference between potential *
          // OverRelaxationConst + old potential
          fPotential[ir][iphi%fPhiMaxArrayIndex][iz] =
              fSORConst * (updatedPotential - oldPotential) + oldPotential;

          //check for convergence
          convergence +=
              fabs(oldPotential - fPotential[ir][iphi%fPhiMaxArrayIndex][iz]) / convergenceNormConst;
          //	      cout<<"convergence : convergenceNormConst"<<convergence<<":"<<convergenceNormConst<<endl;
        }
      }
    }
    if(SaveConvergence)
      fConvergenceStream<<convergence<<"\t"<<convergenceNormConst<<endl;
  }
  cout<<endl<<"FINISHED Updating Weighting Potential for Segment "<<segment
      <<" at convergence "<< convergence <<endl;
  if(SaveConvergence) fConvergenceStream.close();
}
//=============================================================================
void MGSORFieldCalculation::UpdateWeightingPotential_WellType(int segment)
{
  //If save convergence to file
  if(SaveConvergence){
    char tmpFilename[1000];
    sprintf(tmpFilename,"%s_Segment_%i.dat",fConvergenceFilename,segment);
    fConvergenceStream.open(tmpFilename);
  }

  cout<<endl<<"Updating Weighting Potential for Segment "<<segment<<endl;

  double convergence = 0.;
  double convergenceNormConst = 0;
  float oldPotential;
  float updatedPotential;
  int loops = 0;

  float VRPlus    = 0;
  float VRMinus   = 0;
  float VPhiPlus  = 0;
  float VPhiMinus = 0;
  float VZPlus    = 0;
  float VZMinus   = 0;
  float r = 0;
  float c = 0;

  while(convergence >= fConvergenceLimit || loops < 300){
    loops++;
    if(loops%50 == 0){cout<<"*"<<flush;}
    convergence = 0.;
    convergenceNormConst = ConvergenceNormalizationConst();

    // ******************************
    // Note loop on r = 1..fRMaxArrayIndex-1
    // and z from 1 !!
    // ******************************
    for( int ir=1; ir<=fRMaxArrayIndex-1; ir++ ){
      for( int iphi=0; iphi<fPhiMaxArrayIndex; iphi++ ){
        for( int iz=1; iz<=fZMaxArrayIndex; iz++ ){

          if(ir <= fRBound && iz >= fZBound)
          {
            // skip the area in the well - don't care about that
            continue;
          }

          r = ir * fdelR;

          VRPlus    = 0;
          VRMinus   = 0;
          VPhiPlus  = 0;
          VPhiMinus = 0;
          VZPlus    = 0;
          VZMinus   = 0;
          c = -2./(fdelR * fdelR)
              -2./(r * r * fdelPhi * fdelPhi)
              -2./(fdelZ * fdelZ);

          if(ir==1)
               // at 0 - ignore for now
               VRMinus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
          else VRMinus = fPotential[ir-1][iphi%fPhiMaxArrayIndex][iz];

          VRPlus = fPotential[ir+1][iphi%fPhiMaxArrayIndex][iz];


          if(iphi==0) //phi == 0 apply periodic boundary conditions
               VPhiMinus = fPotential[ir][fPhiMaxArrayIndex-1][iz]; //-1 important, since fPhiMaxArrayIndex is == to [0]!
          else VPhiMinus = fPotential[ir][iphi-1][iz];

          VPhiPlus = fPotential[ir][(iphi+1)%fPhiMaxArrayIndex][iz];


          VZMinus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz-1];

          if(iz==fZMaxArrayIndex)
               VZPlus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
          else VZPlus = fPotential[ir][iphi%fPhiMaxArrayIndex][iz+1];

          //this is the real updating step;
          //save old value
          oldPotential = fPotential[ir][iphi%fPhiMaxArrayIndex][iz];

          // update according to algorithm
          // almost as for the E potential, except no charge density
          updatedPotential = (1/c) * (
              - (VZPlus + VZMinus)/(fdelZ * fdelZ)
              - (VPhiMinus + VPhiPlus)/(r*r*fdelPhi*fdelPhi)
              - (2.*r-fdelR)*VRMinus/(2.*r*fdelR*fdelR)
              - (2.*r+fdelR)*VRPlus/(2.*r*fdelR*fdelR) );

          // Successive over relaxation: Difference between potential *
          // OverRelaxationConst + old potential
          fPotential[ir][iphi%fPhiMaxArrayIndex][iz] =
              fSORConst * (updatedPotential - oldPotential) + oldPotential;

          //check for convergence
          convergence +=
              fabs(oldPotential - fPotential[ir][iphi%fPhiMaxArrayIndex][iz]) / convergenceNormConst;
          //        cout<<"convergence : convergenceNormConst"<<convergence<<":"<<convergenceNormConst<<endl;
        }
      }
    }
    if(SaveConvergence)
      fConvergenceStream<<convergence<<"\t"<<convergenceNormConst<<endl;
  }

  // Now have to fill the values at r=0:
  // just the same values, since the electrical field is vertical here
  // because of the phi-symmetry, therefore potential does not change in r
  for( int iphi=0; iphi<fPhiMaxArrayIndex; iphi++ )
    for( int iz=1; iz<=fZBound-1; iz++ )
      fPotential[0][iphi%fPhiMaxArrayIndex][iz] = fPotential[1][iphi%fPhiMaxArrayIndex][iz];


  cout<<endl<<"FINISHED Updating Weighting Potential for Segment "<<segment
      <<" at convergence "<< convergence <<endl;
  if(SaveConvergence) fConvergenceStream.close();

}
//=============================================================================
void MGSORFieldCalculation::PotentialToField()
{
  double rho;
  double innerRadius = fCrystalFields->GetCrystalData().GetInnerRadius();

  for( int ir=0; ir<=fRMaxArrayIndex; ir++ ){
    for( int iphi=0; iphi<=fPhiMaxArrayIndex; iphi++ ){
      for( int iz=0; iz<=fZMaxArrayIndex; iz++ ){

        if( (ir != 0 ) && (ir != fRMaxArrayIndex )){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][0] = -0.5*(1./fdelR)*
              (fPotential[ir+1][iphi%fPhiMaxArrayIndex][iz] -
               fPotential[ir-1][iphi%fPhiMaxArrayIndex][iz]);
        }
        if(ir == 0 ){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][0] = - 1*(1./fdelR)*
              (fPotential[ir+1][iphi%fPhiMaxArrayIndex][iz] -
               fPotential[ir][iphi%fPhiMaxArrayIndex][iz]);
        }
        if(ir==fRMaxArrayIndex){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][0] = - 1*(1./fdelR)*
              (fPotential[ir][iphi%fPhiMaxArrayIndex][iz] -
               fPotential[ir-1][iphi%fPhiMaxArrayIndex][iz]);
        }

        //
        // Phi component is divided by rho!!
        //
        rho = ir * fdelR + innerRadius;
        if(iphi!=0 && iphi!=fPhiMaxArrayIndex){
          fFieldArray[ir][iphi][iz][1] = - 0.5 * (1./fdelPhi) *(1./rho) *
              (fPotential[ir][(iphi+1)%fPhiMaxArrayIndex][iz] -
               fPotential[ir][(iphi-1)%fPhiMaxArrayIndex][iz]);
        }
        if(iphi == 0){
          fFieldArray[ir][iphi][iz][1] = - 0.5 * (1./fdelPhi) * (1./rho) *
              (fPotential[ir][1][iz] -
               fPotential[ir][fPhiMaxArrayIndex-1][iz]);
        }
        if(iphi == fPhiMaxArrayIndex){
          fFieldArray[ir][iphi][iz][1] = - 0.5 * (1./fdelPhi) * (1./rho) *
              (fPotential[ir][1][iz] -
               fPotential[ir][fPhiMaxArrayIndex-1][iz]);
        }

        if(iz!=0 && iz!=fZMaxArrayIndex ){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][2] = -0.5*(1./fdelZ)*
              (fPotential[ir][iphi%fPhiMaxArrayIndex][iz+1] -
               fPotential[ir][iphi%fPhiMaxArrayIndex][iz-1]);
        }
        if(iz == 0){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][2] = - 1*(1./fdelZ)*
              (fPotential[ir][iphi%fPhiMaxArrayIndex][iz+1] -
               fPotential[ir][iphi%fPhiMaxArrayIndex][iz]);
        }
        if(iz == fZMaxArrayIndex){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][2] = - 1*(1./fdelZ)*
              (fPotential[ir][iphi%fPhiMaxArrayIndex][iz] -
               fPotential[ir][iphi%fPhiMaxArrayIndex][iz-1]);
        }

        fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][3] =
            fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
      }
    }
  }
}
//=============================================================================
void MGSORFieldCalculation::PotentialToField_WellType()
{
  double rho = 0;

  for(int ir=0;ir<=fRMaxArrayIndex;ir++){
    for(int iphi=0;iphi<=fPhiMaxArrayIndex;iphi++){
      for(int iz=0;iz<=fZMaxArrayIndex;iz++){

        if(ir < fRBound && iz > fZBound)
        {
          // the field in the well is set to 0
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][0] = 0;
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][1] = 0;
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][2] = 0;
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][3] = 0;
          continue;
        }

        if( ir == fRBound && iz >= fZBound){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][0] = -1*(1./fdelR)*
              (fPotential[fRBound+1][iphi%fPhiMaxArrayIndex][iz] -
               fPotential[fRBound]  [iphi%fPhiMaxArrayIndex][iz]);
        }
        else if( ir != 0  && ir != fRMaxArrayIndex ){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][0] = -0.5*(1./fdelR)*
              (fPotential[ir+1][iphi%fPhiMaxArrayIndex][iz] -
               fPotential[ir-1][iphi%fPhiMaxArrayIndex][iz]);
        }
        else if(ir == 0 ){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][0] = - 1*(1./fdelR)*
              (fPotential[1][iphi%fPhiMaxArrayIndex][iz] -
               fPotential[0][iphi%fPhiMaxArrayIndex][iz]);
        }
        else if(ir==fRMaxArrayIndex){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][0] = - 1*(1./fdelR)*
              (fPotential[fRMaxArrayIndex]  [iphi%fPhiMaxArrayIndex][iz] -
               fPotential[fRMaxArrayIndex-1][iphi%fPhiMaxArrayIndex][iz]);
        }

        //
        // Phi component is divided by r!!
        //
        rho = ir * fdelR;
        if(iphi!=0 && iphi!=fPhiMaxArrayIndex){
          fFieldArray[ir][iphi][iz][1] = - 0.5 * (1./fdelPhi) * (1./rho) *
              (fPotential[ir][(iphi+1)%fPhiMaxArrayIndex][iz] -
               fPotential[ir][(iphi-1)%fPhiMaxArrayIndex][iz]);
        }
        else if(iphi == 0){
          fFieldArray[ir][iphi][iz][1] = - 0.5 * (1./fdelPhi) * (1./rho) *
              (fPotential[ir][1]                  [iz] -
               fPotential[ir][fPhiMaxArrayIndex-1][iz]);
        }
        else if(iphi == fPhiMaxArrayIndex){
          fFieldArray[ir][iphi][iz][1] = - 0.5 * (1./fdelPhi) * (1./rho) *
              (fPotential[ir][1]                  [iz] -
               fPotential[ir][fPhiMaxArrayIndex-1][iz]);
        }

        if( (iz == fZMaxArrayIndex && ir >= fRBound) ||
            (iz == fZBound && ir <= fRBound)            ) {
          // We are close to upper boundaries, either top of the crystal of top of the well,
          // take current and lower values
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][2] = - 1*(1./fdelZ)*
              (fPotential[ir][iphi%fPhiMaxArrayIndex][iz] -
               fPotential[ir][iphi%fPhiMaxArrayIndex][iz-1]);
        }
        else if(iz!=0 && iz!=fZMaxArrayIndex ){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][2] = -0.5*(1./fdelZ)*
              (fPotential[ir][iphi%fPhiMaxArrayIndex][iz+1] -
               fPotential[ir][iphi%fPhiMaxArrayIndex][iz-1]);
        }
        else if(iz == 0){
          fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][2] = - 1*(1./fdelZ)*
              (fPotential[ir][iphi%fPhiMaxArrayIndex][1] -
               fPotential[ir][iphi%fPhiMaxArrayIndex][0]);
        }

        fFieldArray[ir][iphi%fPhiMaxArrayIndex][iz][3] =
            fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
      }
    }
  }
}

//=============================================================================

double MGSORFieldCalculation::ConvergenceNormalizationConst()
{
  double NormConst=0;

  // ******************************
  // Note loop on r = 1..fRMaxArrayIndex-1
  // The boundary values should not change
  // ******************************

  const MGCrystalData& crystal = fCrystalFields->GetCrystalData();
  MGCrystalData::EGeometryType type = crystal.GetGeometryType();

  if   (type==MGCrystalData::kCoaxial) {
    for(int ir=1;ir<=fRMaxArrayIndex-1;ir++)
      for(int iphi=0;iphi<fPhiMaxArrayIndex;iphi++)
        for(int iz=0;iz<=fZMaxArrayIndex;iz++)
            NormConst += fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
  }
  else if (type==MGCrystalData::kClosedEnd) {
    for(int ir=1;ir<=fRMaxArrayIndex-1;ir++)
      for(int iphi=0;iphi<fPhiMaxArrayIndex;iphi++)
        for(int iz=0;iz<fZBound;iz++)
            NormConst += fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
    for(int ir=fRBound+1;ir<=fRMaxArrayIndex-1;ir++)
      for(int iphi=0;iphi<fPhiMaxArrayIndex;iphi++)
        for(int iz=fZBound;iz<=fZMaxArrayIndex;iz++)
            NormConst += fPotential[ir][iphi%fPhiMaxArrayIndex][iz];
  }

  return fabs(NormConst);
}

