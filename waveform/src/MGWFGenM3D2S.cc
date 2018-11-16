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
 * AUTHOR:
 * 	Y. Chan
 *
 * FIRST SUBMISSION:
 * 
 * REVISION: MM-DD-YYYY
 *
 */

#include "TMath.h"
#include "io/MGLogger.hh"
#include "waveform/MGWFGenM3D2S.hh"
#include <cstring>

using namespace std;
using namespace CLHEP;

MGWFGenM3D2S::MGWFGenM3D2S(const MGCrystalData& crystalData, size_t wfLen, double sampFreq, double preTriggerTime, double ePair) :
MGVWaveformGenerator(crystalData, wfLen, sampFreq, MGWaveform::kCharge, preTriggerTime)
{
  fEPair = ePair;

  fWaveformArray = NULL;
  fLookUpTableFileName = "";
  fWFTWaveformLength = 50; // GRETINA default
  fWFTSamplingFreq = 1.0*CLHEP::GHz; // GRETINA default
  fPresentWaveform = NULL;
  fSegmentOffset = 0;
  fInput = NULL;
  memset(fTrilinearIndex, 0, 8*sizeof(int));
  fPresentGridIndex = 0;
  fPreviousGridIndex = 0;

  if(!fCrystalData->IsCentralContact(0)) {
    MGLog(warning) << "Bogus iCentralContact" << endlog;
  }
}

MGWFGenM3D2S::~MGWFGenM3D2S()
{
  delete [] fPresentWaveform;
  delete [] fWaveformArray;
}

void MGWFGenM3D2S::Initialize()
{
  if(fIsInitialized) return; // this class shouldn't be re-initialized

  fInput=fopen(fLookUpTableFileName.c_str(),"rb");
  if(fInput) {
    MGLog(trace) << "===================MGWFGenM3D2S Log:====================" << endlog;
    MGLog(trace) << "MGWFGenM3D2S file: " << fLookUpTableFileName << " Found. Please wait while reading ..." << endlog;
  } else {
    MGLog(routine) << "Cannot locate MGWFGenM3D2S WFT file: "<< fLookUpTableFileName << endlog;
    MGLog(fatal) << "Application aborted. Please try again. "<< endlog;
  }

  int totalSize=fXSize*fYSize*fZSize;

  int nwave=totalSize*fCrystalData->GetNSegments();

  MGLog(trace) <<"No. of grid points (x y z) = " <<fXSize<<" "<<fYSize<<" "<<fZSize<<endlog;
  MGLog(trace) <<"Total seg = "<<fCrystalData->GetNSegments()<<" Total No. of Ref. Waveforms = "<<nwave<<endlog;
  MGLog(debugging) << fGridxMin <<" "<< fGridyMin <<" "<<fGridzMin <<endlog;
  MGLog(debugging) << fGridxMax <<" "<< fGridyMax <<" "<<fGridzMax <<endlog;
  MGLog(debugging) << fGridxSpacing <<" "<< fGridySpacing <<" "<<fGridzSpacing <<endlog;
  MGLog(debugging) << "Malloc for internal array (float) = " << nwave*fWFTWaveformLength << endlog;
  MGLog(debugging) <<"        ----------------------------------              "<<endlog;

  //allocate memory for required internal arrays

  //buffer array for one event 
  fPresentWaveform = new double[fCrystalData->GetNSegments()*fWFTWaveformLength];

  //full grid array
  fWaveformArray = new float[fWFTWaveformLength*nwave];

  // read in WaveformTable
  if(fWaveformArray == NULL) {
    MGLog(fatal) << "Memory allocation failed. Applciation abort."<< endlog;
  }

  MGLog(routine) <<"Memory request ["<<fWFTWaveformLength*nwave<<" (floats)] for main array done."<<endlog;
  MGLog(routine) << nwave << " Reading waveform entries from file..."<< endlog;

  fread (fWaveformArray,sizeof(float),nwave*fWFTWaveformLength,fInput);

  MGLog(routine) <<"====================================================== "<<fCrystalData->GetNSegments()<< endlog;
  MGLog(routine) <<"Waveform Library successfully loaded into memory."<< endlog;

  // define segment look up indices
  //for(size_t i=0; i<fCrystalData->GetNSegments(); i++){
    // not yet implemented
  //}
}

void MGWFGenM3D2S::SetGrid(int aGridxMin, int aGridxMax, int aGridxSpacing,
                           int aGridyMin, int aGridyMax, int aGridySpacing,
                           int aGridzMin, int aGridzMax, int aGridzSpacing)
{
  fGridxMin = aGridxMin; fGridxMax = aGridxMax; fGridxSpacing = aGridxSpacing;
  fGridyMin = aGridyMin; fGridyMax = aGridyMax; fGridySpacing = aGridySpacing;
  fGridzMin = aGridzMin; fGridzMax = aGridzMax; fGridzSpacing = aGridzSpacing;

  int r_mm_asInt = (int) (fCrystalData->GetOuterRadius()/mm);
  int h_mm_asInt = (int) (fCrystalData->GetCrystalHeight()/mm);
  if(fGridxMax !=  r_mm_asInt ||
     fGridxMin != -r_mm_asInt ||
     fGridyMax !=  r_mm_asInt ||
     fGridyMin != -r_mm_asInt ||
     fGridzMax != h_mm_asInt ||
     fGridzMin != 0) {
    MGLog(warning) << "MGWFGenM3D2S must use 1 mm grid spacing. " << endl
                   << " Xmax = " << fGridxMax << " but rMax is " 
                   << fCrystalData->GetOuterRadius()/mm << " mm" << endl
                   << " Zmax = " << fGridzMax << " but crystal height is "
                   << fCrystalData->GetCrystalHeight()/mm << " mm" << endlog;
  }

  fXSize=fGridxMax-fGridxMin+1;
  fYSize=fGridyMax-fGridyMin+1;
  fZSize=fGridzMax-fGridzMin+1;

  fGridyGridzOffset = fYSize*fZSize;
  fGridzOffset = fZSize;
}

void MGWFGenM3D2S::DumpWFTable()
{
  MGLog(routine) << "Dumping waveform table..." << endlog;
  for(int ii=0; ii<fXSize; ii++){
    for(int jj=0; jj<fYSize; jj++){
      for(int kk=0; kk<fZSize; kk++){
        for(size_t pp=0; pp<fCrystalData->GetNSegments(); pp++){
          int jindex=(ii*fYSize*fZSize+jj*fZSize+kk)*fCrystalData->GetNSegments();
          for(size_t imm=0; imm<fWFTWaveformLength; imm++){
               cout << "wft " << ii << ' ' << jj << ' ' << kk << ' ' << pp << ' ' << jindex << ' ' << imm << ' ' 
                    << fWaveformArray[jindex*fWFTWaveformLength +pp*fWFTWaveformLength+imm] << endl;
          }
        }
      }
    }
  }
}

void MGWFGenM3D2S::GenerateWaveformsForHit(double x, double y, double z, double t, double E, MGWaveformCollection* partialOutput)
{
  double chargeDep = E/fEPair*eplus;

  // Important - match up with m3d2s coordindate (convention)
  // where the crystal front points towards z=0. So needs to flip the z coordinate
  z = -z;  //flipping the z coordiante
  z += fCrystalData->GetCrystalHeight()/2.0; //matching the z=0 point

  // for test phase, only consider first quadrand - not implemented

  // symmetry handling (by the fSegmentOffset key - not implemented
  fSegmentOffset = 0;

  size_t iCentralContact = fCrystalData->GetICentralContact();

  //MGLog(routine) <<"n_seg = "<< fCrystalData->GetNSegments()<< endlog;;

  // load waveforms for all segments  from the wave table 
  // (note: waveforms are stored in the fPresentWaveform array and accessed via GetWFValueAtTime(t, iSeg))
  InterpolateTrilinearWaveform(x/mm, y/mm, z/mm);

  //fill waveforms
  for(size_t iSeg =0; iSeg< fCrystalData->GetNSegments(); iSeg++){
    for(size_t iSample = 0; iSample < fWFLength; iSample++) {
      double tSample = partialOutput->GetWaveform(iCentralContact)->GetTLocal(iSample);
      double cSample = chargeDep * GetWFValueAtTime(tSample - t, iSeg);

      //for segments
      (*partialOutput->GetWaveform(fCrystalData->GetIContact(iSeg)))[iSample] += cSample;

      //for central contact (note: in m3d2s, the central contact waveform is a sum of all the segments)
      (*partialOutput->GetWaveform(iCentralContact))[iSample] -= cSample;
    }
  }
}

void MGWFGenM3D2S::InterpolateTrilinearWaveform(double x0_mm, double y0_mm, double z0_mm)
{
  //NOTE: make sure that the coordinate system is correct and working in mm
  //Earlier cvs code has problem in units

  // clear previous fPresentWaveform
  memset(fPresentWaveform, 0, fCrystalData->GetNSegments()*fWFTWaveformLength*sizeof(double));

  // segment offset here
  int kOffSet=0;

  // snap to grid
  int ix0 = (int) x0_mm;
  int iy0 = (int) y0_mm;
  int iz0 = (int) z0_mm;

  // get interpolation vector
  double dx = x0_mm - ix0;
  double dy = y0_mm - iy0;
  double dz = z0_mm - iz0;

  // get grid point indices
  ix0 = (int) (x0_mm-fGridxMin);
  iy0 = (int) (y0_mm-fGridyMin);
  iz0 = (int) (z0_mm-fGridzMin);

  //printf("point index = %d %d %d %f %f %f %f %f %f\n", ix0, iy0, iz0, x0, y0, z0, dx, dy, dz);

  // genearte waveform grid indices for interpolation cube
  int index000 = (ix0    )*fGridyGridzOffset + (iy0   )*fGridzOffset + (iz0    ) + kOffSet;
  int index001 = (ix0    )*fGridyGridzOffset + (iy0   )*fGridzOffset + (iz0+1  ) + kOffSet;
  int index101 = (ix0+1  )*fGridyGridzOffset + (iy0   )*fGridzOffset + (iz0+1  ) + kOffSet;
  int index100 = (ix0+1  )*fGridyGridzOffset + (iy0   )*fGridzOffset + (iz0    ) + kOffSet;
  int index010 = (ix0    )*fGridyGridzOffset + (iy0+1 )*fGridzOffset + (iz0    ) + kOffSet;
  int index011 = (ix0    )*fGridyGridzOffset + (iy0+1 )*fGridzOffset + (iz0+1  ) + kOffSet;
  int index111 = (ix0+1  )*fGridyGridzOffset + (iy0+1 )*fGridzOffset + (iz0+1  ) + kOffSet;
  int index110 = (ix0+1  )*fGridyGridzOffset + (iy0+1 )*fGridzOffset + (iz0    ) + kOffSet;


  // array indices
  // note: fTrilinearIndex[] is in the "grid" space, the array (waveform) space should be
  // fTrilinearIndex[]*fCrystalData->GetNSegments()*fWFTWaveformLength
  fTrilinearIndex[0]=index000*fCrystalData->GetNSegments()*fWFTWaveformLength;
  fTrilinearIndex[1]=index001*fCrystalData->GetNSegments()*fWFTWaveformLength;
  fTrilinearIndex[2]=index101*fCrystalData->GetNSegments()*fWFTWaveformLength;
  fTrilinearIndex[3]=index100*fCrystalData->GetNSegments()*fWFTWaveformLength;
  fTrilinearIndex[4]=index010*fCrystalData->GetNSegments()*fWFTWaveformLength;
  fTrilinearIndex[5]=index011*fCrystalData->GetNSegments()*fWFTWaveformLength;
  fTrilinearIndex[6]=index111*fCrystalData->GetNSegments()*fWFTWaveformLength;
  fTrilinearIndex[7]=index110*fCrystalData->GetNSegments()*fWFTWaveformLength;

  double coef8[8];
  coef8[0] = (1-dx)*(1-dy)*(1-dz);
  coef8[1] = (dx)*(1-dy)*(1-dz);
  coef8[2] = (1-dx)*(dy)*(1-dz);
  coef8[3] = (1-dx)*(1-dy)*(dz);
  coef8[4] = (dx)*(1-dy)*(dz);
  coef8[5] = (1-dx)*(dy)*(dz);
  coef8[6] = (dx)*(dy)*(1-dz);
  coef8[7] = (dx)*(dy)*(dz);
  //printf("coef8 = %f %f %f %f %f %f %f %f\n",coef8[0], coef8[1],coef8[2],coef8[3],coef8[4],coef8[5],coef8[6],coef8[7]);

  // loop over segments
  for(size_t j_seg=0; j_seg<fCrystalData->GetNSegments(); j_seg++){
    // copy all contagious segment waveforms from main array, with interpolation
    for(size_t k=0; k<fWFTWaveformLength; k++){
      for (int im=0; im<8; im++){
        int itemp = fTrilinearIndex[im] + j_seg*fWFTWaveformLength;
        fPresentWaveform[j_seg*fWFTWaveformLength+k] += coef8[im]*fWaveformArray[itemp+k];
        //printf("check %d %ld %e\n",k,itemp,fPresentWaveform[j_seg*fWFTWaveformLength+k]);
      }
    }
  }
}


double MGWFGenM3D2S::GetWFValueAtTime(double t, size_t iSeg)
{
  if(t <= 0) return 0;

  size_t iOffset = iSeg * fWFTWaveformLength;
  double samplingPeriod = 1.0/fWFTSamplingFreq;
  double maxTime = samplingPeriod * (fWFTWaveformLength-1);
  if(t >= maxTime) return fPresentWaveform[iOffset + fWFTWaveformLength - 1];

  size_t iLow = (size_t) (t/samplingPeriod);
  double tLow = samplingPeriod * iLow;
  double vLow = fPresentWaveform[iOffset + iLow];
  if(tLow == t) return vLow;

  // Perform simple linear interpolation.
  double tHigh = tLow + samplingPeriod;
  double vHigh = fPresentWaveform[iOffset + iLow + 1];
  return vLow + (t - tLow) / (tHigh - tLow) * (vHigh - vLow);
}
