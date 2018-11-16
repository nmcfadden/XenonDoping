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
// $Id: MGOutputG4StepsData.cc,v 1.8 2007-05-03 18:14:42 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MGOutputG4Steps.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Thu Dec 16 17:03:08 PST 2004
 * 
 * REVISION:
 *
 * 12-16-2004, Created, R. Henning
 * 12-22-2005, Set all pointers to 0 during instantiation, R. Henning
 * 01-02-2005, Added and set fPhysicalVolume to 0, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include "io/MGLogger.hh"
#include "io/MGOutputG4StepsData.hh"
#include "MGCrystalHits.hh"

using namespace CLHEP;

ClassImp(MGOutputG4StepsData)

MGOutputG4StepsData::MGOutputG4StepsData() :
  fParticleID(0),
  fPreviousTrackID(0),
  fTrackID(0),
  fX(0),
  fY(0),
  fZ(0),
  fPx(0),
  fPy(0),
  fPz(0),
  fEdep(0),
  fG4Process(0),
  fPhysicalVolume(0),
  fLocalX(0),
  fLocalY(0),
  fLocalZ(0),
  fStatus01(0),        
  fMaterialIndex(0),   
  fGlobalTime(0),
  fOffsetTime(0)
{;}
        
//---------------------------------------------------------------------------//
 
MGOutputG4StepsData::~MGOutputG4StepsData() 
{
  if(ArraysAreAllocated()) DeallocateArrays();
}

//---------------------------------------------------------------------------//

#include <iostream>
using namespace std;
Int_t MGOutputG4StepsData::GetCrystalID(size_t iStep) const
{
  // MGWaveform classes need an integer "ID" associated with each crystal.
  // Really, there should be an fCrystalID member of MGOutputG4StepsData,
  // a value which is filled from a MGGeometryDetector function
  // GetCrystalID(G4PhysicalVolume* volume) (or similar).
  // For now, create the ID from the PhysicalVolumeName. We have to assume
  // some particular geometry, so it will be the MJ reference design
  // geometry. This routine is taken from Reyco's
  // MGProcessorMCSegmentation::ProcessEventAction().
  // Assume the name is ActiveCrystalLCrystalColumnN, where L is 0-2, N is
  // 0-18

  const Int_t kCrystalsInLayer = 19;

  size_t volNamePos = 0;
  for(size_t i=0; i<iStep; i++) {
    volNamePos += strlen(fPhysicalVolume + volNamePos) + 1;
  }

  Int_t crystalID = (Int_t) (fPhysicalVolume[volNamePos + 13] - '0') * kCrystalsInLayer;

  if(fPhysicalVolume[volNamePos + 28] == '\0') {
    crystalID += (Int_t) (fPhysicalVolume[volNamePos + 27] - '0');
  } else {
    crystalID += 10 * (Int_t) (fPhysicalVolume[volNamePos + 27] - '0');
    crystalID += (Int_t) (fPhysicalVolume[volNamePos + 28] - '0');
  }

  return crystalID;
}

size_t MGOutputG4StepsData::AllocateArrays(size_t maxArrayLength, size_t avgLengthProcessName)
{
  if(maxArrayLength < 1) {
    MGLog(error) << "Cannot allocate arrays with < 1 elements." << endlog;
    return 0;
  } 
  size_t maxProcArrayLength = maxArrayLength * avgLengthProcessName;
  fParticleID = new Int_t[maxArrayLength];
  fPreviousTrackID = new Int_t[maxArrayLength];
  fTrackID = new Int_t[maxArrayLength];
  fX = new Float_t[maxArrayLength];
  fY = new Float_t[maxArrayLength];
  fZ = new Float_t[maxArrayLength];
  fPx = new Float_t[maxArrayLength];
  fPy = new Float_t[maxArrayLength];
  fPz = new Float_t[maxArrayLength];
  fEdep = new Float_t[maxArrayLength];
  fG4Process = new Char_t[maxProcArrayLength];
  fPhysicalVolume = new Char_t[maxProcArrayLength];
  fStatus01 = new Int_t[maxArrayLength];
  fMaterialIndex = new Int_t[maxArrayLength];
  fGlobalTime = new Double_t[maxArrayLength];
  fOffsetTime = new Double_t[maxArrayLength];
  fLocalX = new Float_t[maxArrayLength];
  fLocalY = new Float_t[maxArrayLength];
  fLocalZ = new Float_t[maxArrayLength];
  return maxProcArrayLength;
}

void MGOutputG4StepsData::DeallocateArrays()
{
  delete [] fParticleID;
  delete [] fPreviousTrackID;
  delete [] fTrackID;
  delete [] fX;
  delete [] fY;
  delete [] fZ;
  delete [] fPx;
  delete [] fPy;
  delete [] fPz;
  delete [] fEdep;
  delete [] fG4Process;
  delete [] fPhysicalVolume;
  delete [] fStatus01;
  delete [] fMaterialIndex;
  delete [] fGlobalTime;
  delete [] fOffsetTime;
  delete [] fLocalX;
  delete [] fLocalY;
  delete [] fLocalZ;

  // Set all pointers to NULL, as a nice programmer would.
  fParticleID = fPreviousTrackID =  fTrackID = fStatus01 = 0;
  fMaterialIndex = 0;
  fG4Process = fPhysicalVolume = 0;
  fX = fY = fZ = fPx = fPy = fPz = fEdep = 0;
  fGlobalTime = fOffsetTime = 0;
  fLocalX = fLocalY = fLocalZ = 0;
}

void MGOutputG4StepsData::FillCrystalHits(MGCrystalHits* xtalHits)
{
  xtalHits->Reset();
  for(int i=0; i<fTotalNumberOfSteps; i++) {
    if(fStatus01[i] == 1) {
      xtalHits->AddCrystalHit(fLocalX[i]*cm, fLocalY[i]*cm, fLocalZ[i]*cm, 
                              fGlobalTime[i]*ns, fEdep[i]*keV, GetCrystalID(i));
    }
  }
}
