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
 * $Id: MGGeneratorPrimary.hh,v 1.9 2008-06-26 15:47:22 pandola Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorPrimary.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Class to manage and select generators.
 *
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Apr 19 14:55:56 PDT 2004
 * 
 * REVISION:
 * 
 * 04-19-2004 Created, R. Henning
 * 07-07-2004 
 * 11-08-2004 Added private variables and methods for the management of 
 *            confined source via MGGeneratorPositionSampling, Luciano
 * 01-31-2005 Added volumelist command for sampling in lists of volumes, Kevin 
 * 03-31-2005 Added method and member for setting point-like positions for 
 *            the source. Luciano
 * 01-21-2006 Add General Surface Sampler rootfile read-in. Jason
 * 11-29-2007 Add two new codes for confinement in a geometrical surface or volume. GPS does that, 
 *            but I need that to use other MaGe tools (e.g. 
 *            it is not possible to generate with GSP points over a surface with directions pointing 
 *            towards a fixed point). Luciano
 * 04-24-2008 Modify volumelist sampling, to take care of the fact that the volumes in the list
 *            (e.g. the crystals) may have different volumes, so they have not the same probability.
 *            Luciano
 * 06-26-2008 Modified volumelist sampling in order to have an arbitrary list of numbers (volumes 
 *            are not required to be consecutively numbered), Luciano
 */

#ifndef _MGGENERATORPRIMARY_HH
#define _MGGENERATORPRIMARY_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"

#include "TChain.h"

#include "generators/MGVGenerator.hh"
#include "generators/MGGeneratorPositionSampling.hh"
#include "generators/MGGeneratorPositionSamplingGeometrical.hh"

class G4Event;
class MGGeneratorPrimaryMessenger;

class MGGeneratorPrimary : public G4VUserPrimaryGeneratorAction
{
  public:
  enum EConfinementCode { noconfined, volume, volumelist, volumearray, surface, surfacelist, 
			  geometricalvolume, geometricalsurface};
  MGGeneratorPrimary();
  ~MGGeneratorPrimary();

  //public interface
  void GeneratePrimaries(G4Event *event);
  MGVGenerator *GetMGGenerator() { return fMGGenerator; }
  void SetMGGenerator(MGVGenerator *gene) { fMGGenerator = gene; }
  
  void SetVolumeName(G4String name) {fVolumeName = name;}
  G4String GetVolumeName() {return fVolumeName;}
  
  void SetConfinementCode(EConfinementCode code);
  EConfinementCode GetConfinementCode() {return fConfinementCode;}
  
  void  SetVolumeListName(G4String name) { fVolumeListName = name; }; 
  void  SetVolumeListFrom(G4int n)       { fVolumeListFrom = n;    };
  void  SetVolumeListTo  (G4int n)       { fVolumeListTo   = n;    };
  G4String GetVolumeListName() { return fVolumeListName; };
  G4int    GetVolumeListFrom() { return fVolumeListFrom; };
  G4int    GetVolumeListTo()   { return fVolumeListTo;   };
  void SetVolumeListInitialized(G4bool val){fVolumeListInitialized = val;};
  void SetVolumeArrayInitialized(G4bool val){fVolumeArrayInitialized = val;};
  G4bool GetVolumeListInitialized(){return fVolumeListInitialized;};
  G4bool GetVolumeArrayInitialized(){return fVolumeArrayInitialized;};
  void AddVolumeNumberToList(G4int nnn);
  void AddVolumeNameToArray(G4String pname);
  void ClearList();

  void SetParticlePosition(G4ThreeVector vec) {fPosition = vec;}
  G4ThreeVector GetParticlePosition() {return fPosition;}
  
  void AddGSSFile(const char* filename);
  void SetGSSOffset(G4double GSSOffset) {fGSSOffset = GSSOffset;};
  void SetGSSEventNumber(Long64_t GSSEventNumber) {fGSSEventNumber = GSSEventNumber;};
  
 private:
  MGVGenerator* fMGGenerator;
  MGGeneratorPrimaryMessenger* fG4Messenger;
  G4String fVolumeName;
  EConfinementCode fConfinementCode;
  G4ThreeVector fPosition;

  MGGeneratorPositionSampling* fPositionSampler;
  MGGeneratorPositionSamplingGeometrical* fPositionSamplerGeometrical;

  G4String fVolumeListName;
  G4int    fVolumeListFrom;
  G4int    fVolumeListTo;
  G4bool fVolumeListInitialized;
  G4bool fVolumeArrayInitialized;
  void InitializeVolumeListSampling();
  void InitializeVolumeArraySampling();
  G4String ChooseVolumeFromList();
  G4String ChooseVolumeFromArray();
  std::vector<G4double> *fMassFractionForVolumeList;
  std::vector<G4int> *fIDVolumeList;
  std::vector<G4double> *fMassFractionForVolumeArray;
  std::vector<G4String> *fIDVolumeArray;
  //G4bool fInsertEachElementOfList;

  G4double fGSSOffset;
  Long64_t fGSSEventNumber;
  
  TChain* fGSSChain;
};
#endif
