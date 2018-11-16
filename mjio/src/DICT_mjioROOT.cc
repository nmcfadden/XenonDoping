// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdIDICT_mjioROOTdOcc

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
#include "mjio/MJOutputDetectorEventData.hh"
#include "mjio/MJOutputSegXtal.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_MJOutputSegXtal(void *p = 0);
   static void *newArray_MJOutputSegXtal(Long_t size, void *p);
   static void delete_MJOutputSegXtal(void *p);
   static void deleteArray_MJOutputSegXtal(void *p);
   static void destruct_MJOutputSegXtal(void *p);
   static void streamer_MJOutputSegXtal(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MJOutputSegXtal*)
   {
      ::MJOutputSegXtal *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MJOutputSegXtal >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MJOutputSegXtal", ::MJOutputSegXtal::Class_Version(), "MJOutputSegXtal.hh", 73,
                  typeid(::MJOutputSegXtal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MJOutputSegXtal::Dictionary, isa_proxy, 16,
                  sizeof(::MJOutputSegXtal) );
      instance.SetNew(&new_MJOutputSegXtal);
      instance.SetNewArray(&newArray_MJOutputSegXtal);
      instance.SetDelete(&delete_MJOutputSegXtal);
      instance.SetDeleteArray(&deleteArray_MJOutputSegXtal);
      instance.SetDestructor(&destruct_MJOutputSegXtal);
      instance.SetStreamerFunc(&streamer_MJOutputSegXtal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MJOutputSegXtal*)
   {
      return GenerateInitInstanceLocal((::MJOutputSegXtal*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MJOutputSegXtal*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MJOutputDetectorEventData(void *p = 0);
   static void *newArray_MJOutputDetectorEventData(Long_t size, void *p);
   static void delete_MJOutputDetectorEventData(void *p);
   static void deleteArray_MJOutputDetectorEventData(void *p);
   static void destruct_MJOutputDetectorEventData(void *p);
   static void streamer_MJOutputDetectorEventData(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MJOutputDetectorEventData*)
   {
      ::MJOutputDetectorEventData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MJOutputDetectorEventData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MJOutputDetectorEventData", ::MJOutputDetectorEventData::Class_Version(), "MJOutputDetectorEventData.hh", 69,
                  typeid(::MJOutputDetectorEventData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MJOutputDetectorEventData::Dictionary, isa_proxy, 16,
                  sizeof(::MJOutputDetectorEventData) );
      instance.SetNew(&new_MJOutputDetectorEventData);
      instance.SetNewArray(&newArray_MJOutputDetectorEventData);
      instance.SetDelete(&delete_MJOutputDetectorEventData);
      instance.SetDeleteArray(&deleteArray_MJOutputDetectorEventData);
      instance.SetDestructor(&destruct_MJOutputDetectorEventData);
      instance.SetStreamerFunc(&streamer_MJOutputDetectorEventData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MJOutputDetectorEventData*)
   {
      return GenerateInitInstanceLocal((::MJOutputDetectorEventData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MJOutputDetectorEventData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr MJOutputSegXtal::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MJOutputSegXtal::Class_Name()
{
   return "MJOutputSegXtal";
}

//______________________________________________________________________________
const char *MJOutputSegXtal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MJOutputSegXtal*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MJOutputSegXtal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MJOutputSegXtal*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MJOutputSegXtal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MJOutputSegXtal*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MJOutputSegXtal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MJOutputSegXtal*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MJOutputDetectorEventData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MJOutputDetectorEventData::Class_Name()
{
   return "MJOutputDetectorEventData";
}

//______________________________________________________________________________
const char *MJOutputDetectorEventData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MJOutputDetectorEventData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MJOutputDetectorEventData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MJOutputDetectorEventData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MJOutputDetectorEventData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MJOutputDetectorEventData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MJOutputDetectorEventData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MJOutputDetectorEventData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void MJOutputSegXtal::Streamer(TBuffer &R__b)
{
   // Stream an object of class MJOutputSegXtal.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      { TString R__str; R__str.Streamer(R__b); fXtalName = R__str.Data(); }
      R__b >> fEdep;
      R__b >> fXtalNumber;
      R__b.CheckByteCount(R__s, R__c, MJOutputSegXtal::IsA());
   } else {
      R__c = R__b.WriteVersion(MJOutputSegXtal::IsA(), kTRUE);
      TObject::Streamer(R__b);
      { TString R__str = fXtalName.c_str(); R__str.Streamer(R__b);}
      R__b << fEdep;
      R__b << fXtalNumber;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MJOutputSegXtal(void *p) {
      return  p ? new(p) ::MJOutputSegXtal : new ::MJOutputSegXtal;
   }
   static void *newArray_MJOutputSegXtal(Long_t nElements, void *p) {
      return p ? new(p) ::MJOutputSegXtal[nElements] : new ::MJOutputSegXtal[nElements];
   }
   // Wrapper around operator delete
   static void delete_MJOutputSegXtal(void *p) {
      delete ((::MJOutputSegXtal*)p);
   }
   static void deleteArray_MJOutputSegXtal(void *p) {
      delete [] ((::MJOutputSegXtal*)p);
   }
   static void destruct_MJOutputSegXtal(void *p) {
      typedef ::MJOutputSegXtal current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_MJOutputSegXtal(TBuffer &buf, void *obj) {
      ((::MJOutputSegXtal*)obj)->::MJOutputSegXtal::Streamer(buf);
   }
} // end of namespace ROOT for class ::MJOutputSegXtal

//______________________________________________________________________________
void MJOutputDetectorEventData::Streamer(TBuffer &R__b)
{
   // Stream an object of class MJOutputDetectorEventData.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fDetectorEventNum;
      R__b >> fMCEventNum;
      R__b >> fNumberOfCrystals;
      R__b >> fSeedTime;
      fCrystals->Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, MJOutputDetectorEventData::IsA());
   } else {
      R__c = R__b.WriteVersion(MJOutputDetectorEventData::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << fDetectorEventNum;
      R__b << fMCEventNum;
      R__b << fNumberOfCrystals;
      R__b << fSeedTime;
      fCrystals->Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MJOutputDetectorEventData(void *p) {
      return  p ? new(p) ::MJOutputDetectorEventData : new ::MJOutputDetectorEventData;
   }
   static void *newArray_MJOutputDetectorEventData(Long_t nElements, void *p) {
      return p ? new(p) ::MJOutputDetectorEventData[nElements] : new ::MJOutputDetectorEventData[nElements];
   }
   // Wrapper around operator delete
   static void delete_MJOutputDetectorEventData(void *p) {
      delete ((::MJOutputDetectorEventData*)p);
   }
   static void deleteArray_MJOutputDetectorEventData(void *p) {
      delete [] ((::MJOutputDetectorEventData*)p);
   }
   static void destruct_MJOutputDetectorEventData(void *p) {
      typedef ::MJOutputDetectorEventData current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_MJOutputDetectorEventData(TBuffer &buf, void *obj) {
      ((::MJOutputDetectorEventData*)obj)->::MJOutputDetectorEventData::Streamer(buf);
   }
} // end of namespace ROOT for class ::MJOutputDetectorEventData

namespace {
  void TriggerDictionaryInitialization_libROOT_mjioROOT_Impl() {
    static const char* headers[] = {
"MJOutputDetectorEventData.hh",
"MJOutputSegXtal.hh",
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
#line 1 "libROOT_mjioROOT dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$MJOutputSegXtal.hh")))  __attribute__((annotate("$clingAutoload$mjio/MJOutputDetectorEventData.hh")))  MJOutputSegXtal;
class __attribute__((annotate("$clingAutoload$MJOutputDetectorEventData.hh")))  MJOutputDetectorEventData;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libROOT_mjioROOT dictionary payload"

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
#include "MJOutputDetectorEventData.hh"
#include "MJOutputSegXtal.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"MJOutputDetectorEventData", payloadCode, "@",
"MJOutputSegXtal", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libROOT_mjioROOT",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libROOT_mjioROOT_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libROOT_mjioROOT_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libROOT_mjioROOT() {
  TriggerDictionaryInitialization_libROOT_mjioROOT_Impl();
}
