// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdIDICT_ioROOTdOcc

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "io/MGOutputG4StepsData.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_MGOutputG4StepsData(void *p = 0);
   static void *newArray_MGOutputG4StepsData(Long_t size, void *p);
   static void delete_MGOutputG4StepsData(void *p);
   static void deleteArray_MGOutputG4StepsData(void *p);
   static void destruct_MGOutputG4StepsData(void *p);
   static void streamer_MGOutputG4StepsData(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MGOutputG4StepsData*)
   {
      ::MGOutputG4StepsData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MGOutputG4StepsData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MGOutputG4StepsData", ::MGOutputG4StepsData::Class_Version(), "MGOutputG4StepsData.hh", 76,
                  typeid(::MGOutputG4StepsData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MGOutputG4StepsData::Dictionary, isa_proxy, 16,
                  sizeof(::MGOutputG4StepsData) );
      instance.SetNew(&new_MGOutputG4StepsData);
      instance.SetNewArray(&newArray_MGOutputG4StepsData);
      instance.SetDelete(&delete_MGOutputG4StepsData);
      instance.SetDeleteArray(&deleteArray_MGOutputG4StepsData);
      instance.SetDestructor(&destruct_MGOutputG4StepsData);
      instance.SetStreamerFunc(&streamer_MGOutputG4StepsData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MGOutputG4StepsData*)
   {
      return GenerateInitInstanceLocal((::MGOutputG4StepsData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MGOutputG4StepsData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr MGOutputG4StepsData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MGOutputG4StepsData::Class_Name()
{
   return "MGOutputG4StepsData";
}

//______________________________________________________________________________
const char *MGOutputG4StepsData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MGOutputG4StepsData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MGOutputG4StepsData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MGOutputG4StepsData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MGOutputG4StepsData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MGOutputG4StepsData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MGOutputG4StepsData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MGOutputG4StepsData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void MGOutputG4StepsData::Streamer(TBuffer &R__b)
{
   // Stream an object of class MGOutputG4StepsData.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fTotalNumberOfSteps;
      R__b >> fTotalSensitiveEdep;
      delete [] fParticleID;
      fParticleID = new Int_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fParticleID,fTotalNumberOfSteps);
      delete [] fPreviousTrackID;
      fPreviousTrackID = new Int_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fPreviousTrackID,fTotalNumberOfSteps);
      delete [] fTrackID;
      fTrackID = new Int_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fTrackID,fTotalNumberOfSteps);
      delete [] fX;
      fX = new Float_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fX,fTotalNumberOfSteps);
      delete [] fY;
      fY = new Float_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fY,fTotalNumberOfSteps);
      delete [] fZ;
      fZ = new Float_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fZ,fTotalNumberOfSteps);
      delete [] fPx;
      fPx = new Float_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fPx,fTotalNumberOfSteps);
      delete [] fPy;
      fPy = new Float_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fPy,fTotalNumberOfSteps);
      delete [] fPz;
      fPz = new Float_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fPz,fTotalNumberOfSteps);
      delete [] fEdep;
      fEdep = new Float_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fEdep,fTotalNumberOfSteps);
      R__b >> fProcessNamePos;
      delete [] fG4Process;
      fG4Process = new Char_t[fProcessNamePos];
      R__b.ReadFastArray(fG4Process,fProcessNamePos);
      R__b >> fVolumeNamePos;
      delete [] fPhysicalVolume;
      fPhysicalVolume = new Char_t[fVolumeNamePos];
      R__b.ReadFastArray(fPhysicalVolume,fVolumeNamePos);
      delete [] fLocalX;
      fLocalX = new Float_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fLocalX,fTotalNumberOfSteps);
      delete [] fLocalY;
      fLocalY = new Float_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fLocalY,fTotalNumberOfSteps);
      delete [] fLocalZ;
      fLocalZ = new Float_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fLocalZ,fTotalNumberOfSteps);
      delete [] fStatus01;
      fStatus01 = new Int_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fStatus01,fTotalNumberOfSteps);
      delete [] fMaterialIndex;
      fMaterialIndex = new Int_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fMaterialIndex,fTotalNumberOfSteps);
      delete [] fGlobalTime;
      fGlobalTime = new Double_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fGlobalTime,fTotalNumberOfSteps);
      delete [] fOffsetTime;
      fOffsetTime = new Double_t[fTotalNumberOfSteps];
      R__b.ReadFastArray(fOffsetTime,fTotalNumberOfSteps);
      R__b.CheckByteCount(R__s, R__c, MGOutputG4StepsData::IsA());
   } else {
      R__c = R__b.WriteVersion(MGOutputG4StepsData::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << fTotalNumberOfSteps;
      R__b << fTotalSensitiveEdep;
      R__b.WriteFastArray(fParticleID,fTotalNumberOfSteps);
      R__b.WriteFastArray(fPreviousTrackID,fTotalNumberOfSteps);
      R__b.WriteFastArray(fTrackID,fTotalNumberOfSteps);
      R__b.WriteFastArray(fX,fTotalNumberOfSteps);
      R__b.WriteFastArray(fY,fTotalNumberOfSteps);
      R__b.WriteFastArray(fZ,fTotalNumberOfSteps);
      R__b.WriteFastArray(fPx,fTotalNumberOfSteps);
      R__b.WriteFastArray(fPy,fTotalNumberOfSteps);
      R__b.WriteFastArray(fPz,fTotalNumberOfSteps);
      R__b.WriteFastArray(fEdep,fTotalNumberOfSteps);
      R__b << fProcessNamePos;
      R__b.WriteFastArray(fG4Process,fProcessNamePos);
      R__b << fVolumeNamePos;
      R__b.WriteFastArray(fPhysicalVolume,fVolumeNamePos);
      R__b.WriteFastArray(fLocalX,fTotalNumberOfSteps);
      R__b.WriteFastArray(fLocalY,fTotalNumberOfSteps);
      R__b.WriteFastArray(fLocalZ,fTotalNumberOfSteps);
      R__b.WriteFastArray(fStatus01,fTotalNumberOfSteps);
      R__b.WriteFastArray(fMaterialIndex,fTotalNumberOfSteps);
      R__b.WriteFastArray(fGlobalTime,fTotalNumberOfSteps);
      R__b.WriteFastArray(fOffsetTime,fTotalNumberOfSteps);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MGOutputG4StepsData(void *p) {
      return  p ? new(p) ::MGOutputG4StepsData : new ::MGOutputG4StepsData;
   }
   static void *newArray_MGOutputG4StepsData(Long_t nElements, void *p) {
      return p ? new(p) ::MGOutputG4StepsData[nElements] : new ::MGOutputG4StepsData[nElements];
   }
   // Wrapper around operator delete
   static void delete_MGOutputG4StepsData(void *p) {
      delete ((::MGOutputG4StepsData*)p);
   }
   static void deleteArray_MGOutputG4StepsData(void *p) {
      delete [] ((::MGOutputG4StepsData*)p);
   }
   static void destruct_MGOutputG4StepsData(void *p) {
      typedef ::MGOutputG4StepsData current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_MGOutputG4StepsData(TBuffer &buf, void *obj) {
      ((::MGOutputG4StepsData*)obj)->::MGOutputG4StepsData::Streamer(buf);
   }
} // end of namespace ROOT for class ::MGOutputG4StepsData

namespace {
  void TriggerDictionaryInitialization_libROOT_ioROOT_Impl() {
    static const char* headers[] = {
"MGOutputG4StepsData.hh",
0
    };
    static const char* includePaths[] = {
"/mnt/mjdDisk1/Majorana/soft/root-6.12.06-build/include",
"/mnt/mjdDisk1/Majorana/soft/MGDO/install/include/mgdo",
"/opt/CLHEP/2.4.0.1/install/include",
"/usr/include",
"/include",
"/usr/include/X11/extensions",
"/usr/include/X11",
"/mnt/mjdDisk1/Majorana/soft/geant4.10.04.p02-build/include/Geant4",
"/usr/local/include/mage",
"/mnt/mjdDisk1/Majorana/soft/root-6.12.06-build/include",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libROOT_ioROOT dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$MGOutputG4StepsData.hh")))  MGOutputG4StepsData;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libROOT_ioROOT dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef MG_NO_G4GDML
  #define MG_NO_G4GDML 1
#endif
#ifndef G4USE_STD11
  #define G4USE_STD11 1
#endif
#ifndef G4OPTIMISE
  #define G4OPTIMISE 1
#endif
#ifndef G4VERBOSE
  #define G4VERBOSE 1
#endif
#ifndef G4_STORE_TRAJECTORY
  #define G4_STORE_TRAJECTORY 1
#endif
#ifndef G4UI_USE_TCSH
  #define G4UI_USE_TCSH 1
#endif
#ifndef G4UI_USE
  #define G4UI_USE 1
#endif
#ifndef G4VIS_USE_ASCIITREE
  #define G4VIS_USE_ASCIITREE 1
#endif
#ifndef G4VIS_USE_DAWNFILE
  #define G4VIS_USE_DAWNFILE 1
#endif
#ifndef G4VIS_USE_GMOCRENFILE
  #define G4VIS_USE_GMOCRENFILE 1
#endif
#ifndef G4VIS_USE_HEPREPFILE
  #define G4VIS_USE_HEPREPFILE 1
#endif
#ifndef G4VIS_USE_RAYTRACER
  #define G4VIS_USE_RAYTRACER 1
#endif
#ifndef G4VIS_USE_VRMLFILE
  #define G4VIS_USE_VRMLFILE 1
#endif
#ifndef G4VIS_USE_OPENGLX
  #define G4VIS_USE_OPENGLX 1
#endif
#ifndef G4VIS_USE_OPENGL
  #define G4VIS_USE_OPENGL 1
#endif
#ifndef G4VIS_USE_RAYTRACERX
  #define G4VIS_USE_RAYTRACERX 1
#endif
#ifndef G4VIS_USE
  #define G4VIS_USE 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "MGOutputG4StepsData.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"MGOutputG4StepsData", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libROOT_ioROOT",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libROOT_ioROOT_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libROOT_ioROOT_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libROOT_ioROOT() {
  TriggerDictionaryInitialization_libROOT_ioROOT_Impl();
}
