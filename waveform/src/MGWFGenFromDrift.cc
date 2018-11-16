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
 *
 *
 * AUTHOR:
 *      Jing Liu
 *
 * FIRST SUBMISSION:
 *      10-10-2008, Jing
 *.
 * REVISION: MM-DD-YYYY
 *
 *      11-24-2008, added Runge-Kutta method for calculation, Jing.
 *      11-10-2009. Removed problems concerning the interpolation in GetWFValueAtTime(),added warning. Sabine
 *      03-24-2010, fixed bug for generating more than 1 hit. Alex.
 *      02-10-2011, 1. Added information about (combined!!)hits, elec/holes end points, their trajectories to the output file
 *                  2. Added save option to choose which variables from above(item 1) are stored
 *                  3. Fixed (bug??) in MGWFGenFromDrift::CalculateWFsForHit:
 *                     loop over iSample now starts from 0 (before from 1).
 *                     !!! Warning!!! Previously generated waveforms would be shifted by one sample comparing with the present ones.
 *                  4. Indentation changed
 *                  5. Added Zero() method inherited from MGWaveformGenerator to clean the variables that are written to the output file
 *                   Alex
 *      05-27-2011, Pass drift calculator in via constructor, and get crystal
 *                  fields and crystal data from it. J. Detwiler
 *      06-06-2011, Classes MGWFlibGenEuler and MGWFlibGenRK4 merged into this class not to duplicate the same functions.
 *                  Added features: calculate occupancy for a set of hits.
 *                  Old classes removed. See updated examples of executables in sandbox/
 *                  Doxygen-friendly comments added.
 *                   Alex
 *
 */

#include "TMath.h"
#include "TTree.h"

#include "io/MGLogger.hh"
#include "waveform/MGWFGenFromDrift.hh"
#include <cstring>

using namespace std;
using namespace CLHEP;

MGWFGenFromDrift::MGWFGenFromDrift(
  const MGVDriftVelocity& drift,
  size_t wfLen,
  double sampFreq,
  double preTriggerTime,
  double ePair,
  MGWaveform::EWFType wfType)
:   MGVWaveformGenerator(
      drift.GetCrystalData(),
      wfLen,
      sampFreq,
      wfType,
      preTriggerTime),
    fDrift(&drift),
    fEPair(ePair),
    fPrecision(2),
    fNcontacts(0),
    fPresentWaveform(NULL),
    fSaveOpt(kSaveWaveforms)
{}

// ----------------------------------------------------------------------------

MGWFGenFromDrift::~MGWFGenFromDrift()
{
  delete [] fPresentWaveform;
}

// ----------------------------------------------------------------------------

void MGWFGenFromDrift::SetPrecision(int precision=1)
{
/**
  Set precision for particle drift:\n
    1: for Euler method \n
    2: for 4th order Runge-Kutta method

*/
  fPrecision=precision;
  if (precision==1)
    MGLog(routine)<<"Precision set to 1. Euler method will be used to calculate drifts"<<endlog;
  else if (precision==2)
    MGLog(routine)<<"Precision set to 2. Ronge-Kutta method will be used to calculate drifts"<<endlog;
  else {
    MGLog(warning)<<"The precision "<<fPrecision<< " is not allowed. Please choose one of:"<<endlog;
    MGLog(warning)<<"1: Euler method (default);"<<endlog;
    MGLog(warning)<<"2: 4th-order Runge-Kutta method."<<endlog;
    MGLog(warning)<<"The default method will be used."<<endlog;
    fPrecision = 1;
  }
}

// ----------------------------------------------------------------------------

void MGWFGenFromDrift::Initialize()
{
/**
  This function is called from the parent class MGVWaveformGenerator. \n
  Initialize number of contacts and reserve memoty for the present waveform

*/

  MGLog(routine)<<"Waveform generator initializing"<<endlog;

  fNcontacts = fCrystalData->GetNContacts();
  fPresentWaveform = new double[fNcontacts*fWFLength];
  MGLog(routine)<<"Waveform generator initializing - DONE"<<endlog;
}

// ----------------------------------------------------------------------------

void MGWFGenFromDrift::Zero()
{
/**
  Clean variables before each event. The function is called from the parent class MGVWaveformGenerator.\n
  Cleaning number of hits is very important, otherwise the number of combined hits
  in each events will increase, and increase, and increase...

*/

  hits_totnum = 0;

/*
  //
  // Commented for now
  // Need to check how much time the following setters take:
  //
  memset(hits_edep, 0, MAX_NHITS*sizeof(float));
  memset(hits_xpos, 0, MAX_NHITS*sizeof(float));
  memset(hits_ypos, 0, MAX_NHITS*sizeof(float));
  memset(hits_zpos, 0, MAX_NHITS*sizeof(float));
  memset(hits_time, 0, MAX_NHITS*sizeof(float));

  memset(endp_elec_xpos, 0, MAX_NHITS*sizeof(float));
  memset(endp_elec_ypos, 0, MAX_NHITS*sizeof(float));
  memset(endp_elec_zpos, 0, MAX_NHITS*sizeof(float));
  memset(endp_elec_xpos, 0, MAX_NHITS*sizeof(float));
  memset(endp_elec_ypos, 0, MAX_NHITS*sizeof(float));
  memset(endp_elec_zpos, 0, MAX_NHITS*sizeof(float));

  memset(trj_elec_x, 0, MAX_NHITS*MAX_NTRJP*sizeof(float));
  memset(trj_elec_y, 0, MAX_NHITS*MAX_NTRJP*sizeof(float));
  memset(trj_elec_z, 0, MAX_NHITS*MAX_NTRJP*sizeof(float));
  memset(trj_hole_x, 0, MAX_NHITS*MAX_NTRJP*sizeof(float));
  memset(trj_hole_y, 0, MAX_NHITS*MAX_NTRJP*sizeof(float));
  memset(trj_hole_z, 0, MAX_NHITS*MAX_NTRJP*sizeof(float));
*/
}
// ----------------------------------------------------------------------------

void MGWFGenFromDrift::SetExtraBranches(MGWaveformWriterROOT* wfwriter)
{
/**
  Define additional branches to be written to the output file :\n
    - hits position;\n
    - end points of electrons/holes drift;\n
    - trajectories of electron/holes

*/

  TTree* tree = wfwriter->GetTree();
  if ( fSaveOpt & (kSaveHits|kSaveTrajectories|kSaveEndPoints) ) { // any of the flags in brackets
    tree->Branch("hits_totnum", &hits_totnum, "hits_totnum/I");
  }

  if (! (fSaveOpt & kSaveWaveforms) ) { // Remove waveform from the tree
    TBranch *b = tree->GetBranch(Form("%sWFs", GetName()));
    tree->GetListOfBranches()->Remove(b);
  }

  if ( fSaveOpt & kSaveHits ) { // Combined hits
    MGLog(routine)<<"Output will include hits information"<<endlog;
    tree->Branch("hits_edep", hits_edep, "hits_edep[hits_totnum]/F");
    tree->Branch("hits_xpos", hits_xpos, "hits_xpos[hits_totnum]/F");
    tree->Branch("hits_ypos", hits_ypos, "hits_ypos[hits_totnum]/F");
    tree->Branch("hits_zpos", hits_zpos, "hits_zpos[hits_totnum]/F");
    tree->Branch("hits_time", hits_time, "hits_time[hits_totnum]/F");
  }
  if ( (fSaveOpt & kSaveEndPointsElectrons) || (fSaveOpt & kSaveEndPoints) ) { // End points of electrones
    MGLog(routine)<<"Output will include end points of electrones information"<<endlog;
//    tree->Branch("endp_elec_xpos", endp_elec_xpos, "endp_elec_xpos[hits_totnum]/F");
//    tree->Branch("endp_elec_ypos", endp_elec_ypos, "endp_elec_ypos[hits_totnum]/F");
    tree->Branch("endp_elec_zpos", endp_elec_zpos, "endp_elec_zpos[hits_totnum]/F");
    tree->Branch("endp_elec_rho",  endp_elec_rho,  "endp_elec_rho[hits_totnum]/F");
    tree->Branch("endp_elec_phi",  endp_elec_phi,  "endp_elec_phi[hits_totnum]/F");
  }
  if ( (fSaveOpt & kSaveEndPointsHoles) || (fSaveOpt & kSaveEndPoints) ) { // End points of holes
    MGLog(routine)<<"Output will include end points of holes information"<<endlog;
//    tree->Branch("endp_hole_xpos", endp_hole_xpos, "endp_hole_xpos[hits_totnum]/F");
//    tree->Branch("endp_hole_ypos", endp_hole_ypos, "endp_hole_ypos[hits_totnum]/F");
    tree->Branch("endp_hole_zpos", endp_hole_zpos, "endp_hole_zpos[hits_totnum]/F");
    tree->Branch("endp_hole_rho",  endp_hole_rho,  "endp_hole_rho[hits_totnum]/F");
    tree->Branch("endp_hole_phi",  endp_hole_phi,  "endp_hole_phi[hits_totnum]/F");
  }
  if ( (fSaveOpt & kSaveTrajectoriesElectrons) || (fSaveOpt & kSaveTrajectories) ) { // Trajectories of electrones drift
    MGLog(warning)<<"Output will include electrons trajectories. This is very space-consuming. Be carefull!!"<<endlog;
    tree->Branch("trj_numpoints", &trj_numpoints, "trj_numpoints/I");
    tree->Branch("trj_elec_x", trj_elec_x, "trj_elec_x[hits_totnum][trj_numpoints]/F");
    tree->Branch("trj_elec_y", trj_elec_y, "trj_elec_y[hits_totnum][trj_numpoints]/F");
    tree->Branch("trj_elec_z", trj_elec_z, "trj_elec_z[hits_totnum][trj_numpoints]/F");
  }
  if ( (fSaveOpt & kSaveTrajectoriesHoles) || (fSaveOpt & kSaveTrajectories)  ) { // Trajectories of holes drift
    MGLog(warning)<<"Output will include holes trajectories. This is very space-consuming. Be carefull!!"<<endlog;
    tree->Branch("trj_numpoints", &trj_numpoints, "trj_numpoints/I");
    tree->Branch("trj_hole_x", trj_hole_x, "trj_hole_x[hits_totnum][trj_numpoints]/F");
    tree->Branch("trj_hole_y", trj_hole_y, "trj_hole_y[hits_totnum][trj_numpoints]/F");
    tree->Branch("trj_hole_z", trj_hole_z, "trj_hole_z[hits_totnum][trj_numpoints]/F");
  }
}

void MGWFGenFromDrift::CalculateWFsForHit(Hep3Vector hit, MGWaveform::EWFType wfType)
{
/**
  Calculates present waveforms for a given hit.\n
  If kSaveHits/kSaveEndPoints/kSaveTrajectories are set, also store information
  about hits/end points/trajectories, respectively. Also possible to save them only
  for electrones or holes.\n
  Filling additional branches migrated from the classes MGWFlibGenRK4 and MGWFlibGenEuler.
  Due to duplication of functions, the classes were deleted

*/

  // clear previous fPresentWaveform
  memset(fPresentWaveform, 0, fNcontacts*fWFLength*sizeof(double));

  Hep3Vector ePoint=hit, hPoint=hit;
  Hep3Vector ve1,ve2,ve3,ve4, vh1,vh2,vh3,vh4;
  Double_t dt = 1./fSamplingFrequency;
  trj_numpoints = fWFLength;
  for (size_t iSample=1; iSample<fWFLength; iSample++) {
    ve1=fDrift->GetVeAt(ePoint);
    vh1=fDrift->GetVhAt(hPoint);

    if ( fSaveOpt & kSaveTrajectories ) { // Calculate trajectories
      // at this point hits_totnum is the current hit number
      trj_elec_x[hits_totnum][iSample] = ePoint.x() / mm;
      trj_elec_y[hits_totnum][iSample] = ePoint.y() / mm;
      trj_elec_z[hits_totnum][iSample] = ePoint.z() / mm;
      trj_hole_x[hits_totnum][iSample] = hPoint.x() / mm;
      trj_hole_y[hits_totnum][iSample] = hPoint.y() / mm;
      trj_hole_z[hits_totnum][iSample] = hPoint.z() / mm;
    }

    if (fPrecision!=2) { // Euler method
      ePoint+=ve1*dt;
      hPoint+=vh1*dt;
    }
    else { // 4th-order Runge-Kutta method
      ve2=fDrift->GetVeAt(ePoint+0.5*ve1*dt);
      ve3=fDrift->GetVeAt(ePoint+0.5*ve2*dt);
      ve4=fDrift->GetVeAt(ePoint+0.5*ve3*dt);
      vh2=fDrift->GetVhAt(hPoint+0.5*vh1*dt);
      vh3=fDrift->GetVhAt(hPoint+0.5*vh2*dt);
      vh4=fDrift->GetVhAt(hPoint+0.5*vh3*dt);

      ePoint+=(ve1+2.*ve2+2.*ve3+ve4)/6.*dt;
      hPoint+=(vh1+2.*vh2+2.*vh3+vh4)/6.*dt;
    }
    for (size_t iContact=0; iContact<fNcontacts; iContact++) {
      if (wfType==MGWaveform::kCurrent)
        fPresentWaveform[iContact*fWFLength+iSample]
          = fDrift->GetCrystalFields().GetWField(hPoint,iContact).dot(fDrift->GetVhAt(hPoint))
          - fDrift->GetCrystalFields().GetWField(ePoint,iContact).dot(fDrift->GetVeAt(ePoint));
      else // as MGWaveform::kCharge
        fPresentWaveform[iContact*fWFLength+iSample]
          = fDrift->GetCrystalFields().GetWPotential(hPoint,iContact)
          - fDrift->GetCrystalFields().GetWPotential(ePoint,iContact);
    }
  }

  if ( fSaveOpt & kSaveEndPoints ) { // End points of electrones/holes
    // at this point hits_totnum is the current hit number
//    endp_elec_xpos[hits_totnum] = ePoint.x() / mm;
//    endp_elec_ypos[hits_totnum] = ePoint.y() / mm;
    endp_elec_zpos[hits_totnum] = ePoint.z()   / mm;
    endp_elec_rho[hits_totnum]  = ePoint.rho() / mm;
    endp_elec_phi[hits_totnum]  = ePoint.phi() / degree;
//    endp_hole_xpos[hits_totnum] = hPoint.x() / mm;
//    endp_hole_ypos[hits_totnum] = hPoint.y() / mm;
    endp_hole_zpos[hits_totnum] = hPoint.z() / mm;
    endp_hole_rho[hits_totnum]  = hPoint.rho() / mm;
    endp_hole_phi[hits_totnum]  = hPoint.phi() / degree;
  }
}

// ----------------------------------------------------------------------------

void MGWFGenFromDrift::GenerateWaveformsForHit(
      double x,
      double y,
      double z,
      double t, // MGCrystalHit->fT - minTime + fPreTriggerTime
      double E,
      MGWaveformCollection* partialOutput)
{
/**
  Generates waveforms for a given hit and adds waveforms to partialOutput
  Function is overloaded from MGVWaveformGenerator and called in
  MGVWaveformGenerator::GenerateWaveformsForHits()

*/

  Hep3Vector hit(x,y,z);
  CalculateWFsForHit(hit,fWFType);

  //fill waveforms
  double Q = E/fEPair*eplus;

  double samplingPeriod = 1.0/fSamplingFrequency;
  if ( t != 0 && t < (0.00001*samplingPeriod) ) {
    MGLog(warning)<<"Timeoffset smaller than (0.00001*samplingPeriod)."
                  <<"Wrong slope might be used in the interpolation to calculate WFValue in GetWFValueAtTime()..."<<endlog;
  }

  if ( fSaveOpt & kSaveWaveforms ) { // Fill waveforms now
    //   be sure that you have partialOutput already cleaned up before generating first waveform (for first hit etc.),
    //   otherwise it might cause problems
    for (size_t iContact=0; iContact<fNcontacts; iContact++) {
      for (size_t iSample=0; iSample<fWFLength; iSample++) {
        double tSample = partialOutput->GetWaveform(0)->GetTLocal(iSample);
        double cSample = Q*GetWFValueAtTime(tSample-t,iContact);
        (*partialOutput->GetWaveform(iContact))[iSample] += cSample;
      }
    }
  }

  // Store hits information
  // !!
  // Warning: at this point, x,y,z are coordinates of combined hits!! These may be different
  // from the ones from the root event tree that are fed to waveforms simulation
  // !!
  if (fSaveOpt & kSaveHits) {
    hits_xpos[hits_totnum] = x / mm;
    hits_ypos[hits_totnum] = y / mm;
    hits_zpos[hits_totnum] = z / mm;
    hits_time[hits_totnum] = t / ns; // note that t is not the hit time anymore! (see the method's argument list)
    hits_edep[hits_totnum] = E / MeV;
  }
  hits_totnum++;
}

// ----------------------------------------------------------------------------

double MGWFGenFromDrift::GetWFValueAtTime(double t, size_t iContact)
{
/**
  Return value of the waveform at any time t for the contact iContact

*/

  if(t <= 0) return 0;

  size_t iOffset = iContact * fWFLength;
  double samplingPeriod = 1.0/fSamplingFrequency;
  double maxTime = samplingPeriod * (fWFLength-1);
  if(t >= maxTime) return fPresentWaveform[iOffset + fWFLength - 1];
  // dLow and if needed to prevent from getting into wrong bin as c++ sometimes does rounding errors
  // when doing typecast from double to int
  double dLow=t*(1./samplingPeriod);
  int iLow = (int)dLow;
  // !!!! if time offset is smaller than 0.00001*samplingPeriod the wrong slope is used for the interpolation
  if(fabs(dLow-(iLow+1)) < (0.00001*samplingPeriod)){
    iLow=iLow+1;
  }
  double tLow = samplingPeriod * iLow;
  double vLow = fPresentWaveform[iOffset + iLow];
  if(tLow == t) return vLow;
  // Perform simple linear interpolation.
  double tHigh = tLow + samplingPeriod;
  double vHigh = fPresentWaveform[iOffset + iLow + 1];
  return vLow + (t - tLow) / (tHigh - tLow) * (vHigh - vLow);
}

// ----------------------------------------------------------------------------

void MGWFGenFromDrift::CalculateOccupancyForHits(MGCrystalHits& hits)
{
/**
  Calculates drift end points of holes for hits.\n
  Mainly repeats MGWFGenFromDrift::CalculateWFsForHit, but does not include waveform part
  and accepts many hits.

*/

  Hep3Vector hPoint, hPointOld, v1,v2,v3,v4;
  MGCrystalHit oneHit;

  Double_t dt = 1./fSamplingFrequency;

  // Set number of hits
  hits_totnum = hits.GetNHits();

  // Loop over hits and calculate hole end points
  for (int iHit=0; iHit<hits_totnum; iHit++) {
    oneHit=hits.GetHit(iHit);
/*
         // debug:
         std::cout<<"hit "<<iHit<<": ("
           <<oneHit->fX/CLHEP::mm<<", "
           <<oneHit->fY/CLHEP::mm<<", "
           <<oneHit->fZ/CLHEP::mm<<")*mm"<<std::endl;
*/
    hPoint.set(oneHit.fX,oneHit.fY,oneHit.fZ);
    if ( hPoint.rho()>fDrift->GetCrystalData().GetOuterRadius() ||
         hPoint.rho()<fDrift->GetCrystalData().GetInnerRadius() ||
         std::fabs(hPoint.z())>fDrift->GetCrystalData().GetCrystalHeight()/2.) {
      MGLog(warning)<<"The hit is located outside the crystal and not simulated!!!"
                    <<"r = "<<hPoint.rho()<<", z = "<<hPoint.z()<<endlog;
      continue;
    }

    // Fill the hit information
    hits_xpos[iHit] = oneHit.fX / mm;
    hits_ypos[iHit] = oneHit.fY / mm;
    hits_zpos[iHit] = oneHit.fZ / mm;
    hits_edep[iHit] = oneHit.fE / MeV;

    // Calculate drift
    for (size_t iWF=1; iWF<fWFLength; iWF++) {
      hPointOld=hPoint;

      v1=fDrift->GetVhAt(hPoint);
      if (fPrecision!=2) { // Euler method
        hPoint+=v1*dt;
      }
      else { // 4th-order Runge-Kutta method
        v2=fDrift->GetVhAt(hPoint+0.5*v1*dt);
        v3=fDrift->GetVhAt(hPoint+0.5*v2*dt);
        v4=fDrift->GetVhAt(hPoint+0.5*v3*dt);

        hPoint+=(v1+2.*v2+2.*v3+v4)/6.*dt;
      }

      // The following line saves time. Break if the trajectory is already at the surface, or further away
      if (fabs((hPoint-hPointOld).rho())<0.00001  || hPoint.rho() > fDrift->GetCrystalData().GetOuterRadius() ) break;
    }
    // Save end point coordinates
    endp_hole_zpos[iHit] = hPoint.z() / mm;
    endp_hole_rho[iHit]  = hPoint.rho() / mm;
    endp_hole_phi[iHit]  = hPoint.phi() / degree;
    if (endp_hole_phi[iHit]<0) endp_hole_phi[iHit] += 360.;
  }
}

void MGWFGenFromDrift::Anisotropy(char* /*output*/)
{
/**
 The function migrated from MGWFlibGenEuler.cc. \warning Currently not used

*/
/*
  std::ofstream fout(output);
  float phi[360]={0}, dphi[360]={0}, vstart[360]={0}, time[360]={0};
  for (size_t i=0; i<360; i++) {
    CLHEP::Hep3Vector ePoint, ve;
    phi[i]=i;
    ePoint.setRhoPhiZ(GetCrystalData().GetOuterRadius(),phi[i]*3.1415926/180.,0);

    for (size_t j=1; j<fWFLength; j++) {
      ve=fDrift->GetVeAt(ePoint);
      if (ve.mag()==0) {
        time[i]= ((float) j)/10.;
        break;
      }
      if (j==1) vstart[i]=ve.rho()/(CLHEP::mm/CLHEP::ns);
      ePoint+=ve*(1./fWFSamplingRate);
    }
    if(phi[i]<=180)
      dphi[i] = ePoint.phi()/CLHEP::degree - phi[i];
    else
      dphi[i] = ePoint.phi()/CLHEP::degree + 360 - phi[i];

    fout<<phi[i]<<" \t "<<dphi[i]<<" \t "<<vstart[i]<<" \t "<<time[i]<<std::endl;
  }
  fout.close();
*/
}

void MGWFGenFromDrift::WriteWFlib(char* /*output*/)
{
/**
 The function migrated from MGWFlibGenEuler.cc. \warning Currently not used

*/
/*
  size_t nhits=fCoordinates->size();
  size_t nsegs=GetCrystalData().GetNSegments();
  size_t nsams=fWFLength;

  std::ofstream file;

  file.open(output);

  MGLog(routine)<<"writing "<<nhits<<" events in to file "
                <<output<<" ..."<<endlog;
  for (size_t i=0; i<nhits; i++){
    if (i%500==0) MGLog(routine)<<"finished "<<i<<" events"<<endlog;
    for (size_t j=0; j<nsegs; j++) {
      for (size_t k=0; k<nsams; k++) {
        file<<fWFCollection.at(i*nsegs+j).At(k)<<" ";
      }
      file<<std::endl;
    }
  }
  MGLog(routine)<<"Finished writing "<<nhits<<" events in to file."<<endlog;

  file.close();
*/
}

