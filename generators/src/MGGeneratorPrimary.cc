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
// $Id: MGGeneratorPrimary.cc,v 1.14 2008-06-27 08:33:51 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeneratorPrimary.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * All settings are done via the associated messenger class.
 *
 *
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Mon Apr 19 14:55:56 PDT 2004
 * 
 * REVISION:
 *
 * 04-19-2004 Initiated, R. Henning
 * 11-08-2004 Interfaced with GeneratorPositionSampling(), Luciano
 *            the default is "notconfined"
 * 11-22-2004 Added "copyN", for future improvement of sampler, Luciano
 * 01-31-2005 Added volumelist command for sampling in lists of volumes, Kevin 
 * 03-31-2005 Added possibility to set positions for point-like sources, Luciano
 * 04-08-2005 Bug fix with the point-like sources, Luciano
 * 01-21-2006 Add General Surface Sampler rootfile read-in. Jason
 * 11-30-2007 Add two new codes for confinement in a geometrical surface or volume. Add a new 
 *            fPositionSamplerGeometrical to take care of this. Luciano
 * 04-24-2008 Modify volumelist sampling, to take care of the fact that the volumes in the list
 *            (e.g. the crystals) may have different volumes, so they have not the same probability.
 *            Luciano
 * 06-26-2008 Modified volumelist sampling in order to have an arbitrary list of numbers (volumes 
 *            are not required to be consecutively numbered), Luciano
 * 06-27-2008 Bug fixed, Luciano
 */

#include "G4ThreeVector.hh"
#include "Randomize.hh" 
#include "G4RandomDirection.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "TLeaf.h"

#include "management/MGManager.hh"
#include "generators/MGGeneratorPrimaryMessenger.hh" 
#include "io/MGLogger.hh"

#include "generators/MGGeneratorPrimary.hh" 

using namespace CLHEP;

class G4Event;

MGGeneratorPrimary::MGGeneratorPrimary():
  fMGGenerator(0), fConfinementCode(noconfined), fPositionSamplerGeometrical(0),
  fMassFractionForVolumeList(0),fIDVolumeList(0),
  fMassFractionForVolumeArray(0),fIDVolumeArray(0)
{
  fVolumeName = "NULL";
  fPositionSampler = new MGGeneratorPositionSampling();
  fG4Messenger = new MGGeneratorPrimaryMessenger(this);
  
  G4ThreeVector zero(0.,0.,0.);
  fPosition = zero; //this is the default position

  fVolumeListName = "NULL"; 
  fVolumeListFrom = 0; 
  fVolumeListTo   = 0; 
  fVolumeListInitialized = false;
  
  fVolumeArrayInitialized = false;

  fGSSChain = NULL;
  fGSSOffset = 0.0*mm;
  fGSSEventNumber = 0;
}


MGGeneratorPrimary::~MGGeneratorPrimary()
{
  delete fG4Messenger;
  delete fPositionSampler;
  delete fMGGenerator;
  delete fGSSChain;
  if (fPositionSamplerGeometrical) delete fPositionSamplerGeometrical;
  if (fMassFractionForVolumeList) delete fMassFractionForVolumeList;
  if (fMassFractionForVolumeArray) delete fMassFractionForVolumeArray;
  if (fIDVolumeList) delete fIDVolumeList;
  if (fIDVolumeArray) delete fIDVolumeArray;
}


void MGGeneratorPrimary::GeneratePrimaries(G4Event *event)
{
  G4int copyN = 0;
  if(fMGGenerator) {
    if (fConfinementCode == volume) {
      fPosition = fPositionSampler->SampleUniformlyInVolume(GetVolumeName(),copyN);
      fMGGenerator->SetParticlePosition(fPosition);
    }

    else if (fConfinementCode == surface) {
      fPosition = fPositionSampler->SampleOnSurface(GetVolumeName(),copyN);
      fMGGenerator->SetParticlePosition(fPosition);
    }

    else if (fConfinementCode == volumelist) {

      if (GetVolumeListName() == "NULL")
	{
	  MGLog(error) << "Volume name list is not defined" << endlog;
	  MGLog(error) << "Please set it using the proper macro command" << endlog;
          G4Exception("MGGeneratorPrimary::GeneratePrimaries()", "err001", FatalException, "Aborting execution");
	}

      G4String volumename = ChooseVolumeFromList();
      MGLog(debugging) << "Volume to be sampled from: " << volumename << endlog; 

      fPosition = fPositionSampler->SampleUniformlyInVolume(volumename,copyN);
      fMGGenerator->SetParticlePosition(fPosition);    
    }
    
    else if (fConfinementCode == volumearray) {
      G4String volumename = ChooseVolumeFromArray();
      MGLog(debugging) << "Volume to be sampled from: " << volumename << endlog; 

      fPosition = fPositionSampler->SampleUniformlyInVolume(volumename,copyN);
      fMGGenerator->SetParticlePosition(fPosition);    
    }

    else if (fConfinementCode == surfacelist) {
      G4double x_rand         = G4UniformRand(); 
      G4int    nelements      = GetVolumeListTo() - GetVolumeListFrom() + 1; 
      G4int    elementnumber  = GetVolumeListFrom() + G4int(x_rand * nelements); 
      G4String volumename; 
      char     elnumber[4]; 

      sprintf(elnumber,"%d", elementnumber); 
      volumename =  GetVolumeListName(); 	    
      volumename += "_"; 
      volumename += elnumber; 	 

      fPosition = fPositionSampler->SampleOnSurface(volumename,copyN);
      fMGGenerator->SetParticlePosition(fPosition);
    }
    else if (fConfinementCode == geometricalvolume)
      {
	fPosition = fPositionSamplerGeometrical->SampleInGeometricalVolume();
	fMGGenerator->SetParticlePosition(fPosition);
      }
    else if (fConfinementCode == geometricalsurface)
      {
	fPosition = fPositionSamplerGeometrical->SampleInGeometricalSurface();
	fMGGenerator->SetParticlePosition(fPosition);
    }

    if (fGSSChain != NULL) {
      static Long64_t iEntry = fGSSEventNumber;
      static Long64_t nEntries = fGSSChain->GetEntries();
      if(iEntry >= nEntries) {
        MGLog(warning) << "There are only " << nEntries 
                       << " vertex positions available in the GSS file."
                       << "You have requsted " << iEntry << " >= " << nEntries 
                       << "; you will get the same vertex position from now on!" 
                       << endlog;
      }
      fGSSChain->GetEntry(iEntry);
      double x = fGSSChain->GetLeaf("x")->GetValue();
      double y = fGSSChain->GetLeaf("y")->GetValue();
      double z = fGSSChain->GetLeaf("z")->GetValue();
      fPosition.set(x, y, z);
      if (fGSSOffset !=0){
        G4ThreeVector Offset = G4RandomDirection();
	Offset = Offset * G4RandGauss::shoot(0, fGSSOffset);
        fPosition += Offset;
      }
      fMGGenerator->SetParticlePosition(fPosition);
      iEntry++;
    }

    fMGGenerator->GeneratePrimaryVertex(event);
  }
  else {
    MGLog(error) << "No generator specified!" << endlog;
    MGLog(fatal) << endlog;
  }
}

void MGGeneratorPrimary::SetConfinementCode(EConfinementCode code)
{
  fConfinementCode = code;
  if (fConfinementCode == noconfined) 
    {
      //reset the starting position
      G4ThreeVector zero(0.,0.,0.);
      fPosition = zero;
      fMGGenerator -> SetParticlePosition(fPosition);
      MGLog(trace) << "Default position re-set to " << fPosition << endlog;
    }
  else if (fConfinementCode == geometricalvolume || fConfinementCode == geometricalsurface)
    {
      if (!fPositionSamplerGeometrical)
	{
	  fPositionSamplerGeometrical = new MGGeneratorPositionSamplingGeometrical();
	  MGLog(trace) << "Instantiated MGGeneratorPositionSamplingGeometrical" << G4endl;
	  MGLog(trace) << "Please set the target volume/surface" << G4endl;
	}
    }
}

void MGGeneratorPrimary::AddGSSFile(const char* filename)
{
  if(fGSSChain == NULL) {
    fGSSChain = new TChain("GSSTree", "General Surface Sampler Coordinate Tree");
  }
  fGSSChain->AddFile(filename);
}

void MGGeneratorPrimary::InitializeVolumeListSampling()
{
  MGLog(debugging) << "Start InitializeVolumeListSampling" << endlog;
  if (fMassFractionForVolumeList) delete fMassFractionForVolumeList;

  if (!fIDVolumeList) fIDVolumeList = new std::vector<G4int>;
  fMassFractionForVolumeList = new std::vector<G4double>;

  G4double totalMass = 0.0;
  G4String volumename; 
  char     elnumber[4];
  G4PhysicalVolumeStore* thePhysicalVolumeStore = G4PhysicalVolumeStore::GetInstance();

  //First element is zero
  fMassFractionForVolumeList->push_back(totalMass);

  //This is the old style: if the vector is empty, fill with the full list
  if (!fIDVolumeList->size() && GetVolumeListTo() > GetVolumeListFrom())
    {
      for (G4int i=GetVolumeListFrom();i<=GetVolumeListTo();i++)
	{
	  fIDVolumeList->push_back(i);
	  MGLog(debugging) << "Added detector # " << i << endlog;
	}
    }
  
  if (!fIDVolumeList->size())
    {
      MGLog(error) << "Problem with the volumelist: " << endlog;
      MGLog(error) << "No list of volumes selected (neither one by one nor list)" << endlog;
      MGLog(error) << "Changing confining code to unconfined" << endlog;
      fConfinementCode = noconfined;
    }

  //Get masses of volumes involved
  for (size_t i=0;i<fIDVolumeList->size();i++)
    {
      sprintf(elnumber, "%d", (*fIDVolumeList)[i]); 
      volumename =  GetVolumeListName(); 	    
      volumename += "_"; 
      volumename += elnumber;
      G4VPhysicalVolume* physvolume = thePhysicalVolumeStore->GetVolume(volumename);
      if (!physvolume)
	{
	  MGLog(error) << "The volume called " << volumename << " does not exist" << endlog;
	  return;
	}
      G4double volMass = physvolume->GetLogicalVolume()->GetMass(false,false);
      MGLog(debugging) << "Mass of volume " << volumename << "= " << volMass/kg << " kg" << endlog;
      totalMass += volMass;
      fMassFractionForVolumeList->push_back(totalMass);
    }
  MGLog(routine) << "Number of volumes: " << fMassFractionForVolumeList->size()-1 << endlog;
  MGLog(routine) << "Total mass: " << totalMass/kg << " kg" << endlog;

  for (size_t k=0;k<fMassFractionForVolumeList->size();k++)
    {
      //Normalize to one
      (*fMassFractionForVolumeList)[k] = (*fMassFractionForVolumeList)[k]/totalMass;
      MGLog(debugging) << "Fractional mass #" << k << " = " << (*fMassFractionForVolumeList)[k] << endlog;
    }


  fVolumeListInitialized = true;
}

void MGGeneratorPrimary::InitializeVolumeArraySampling()
{
  MGLog(routine) << "Start InitializeVolumeArraySampling" << endlog;
  if (fMassFractionForVolumeArray) delete fMassFractionForVolumeArray;

  if (!fIDVolumeArray) fIDVolumeArray = new std::vector<G4String>;
  fMassFractionForVolumeArray = new std::vector<G4double>;

  G4double totalMass = 0.0;
  G4String volumename; 
  G4PhysicalVolumeStore* thePhysicalVolumeStore = G4PhysicalVolumeStore::GetInstance();

  //First element is zero
  fMassFractionForVolumeArray->push_back(totalMass);
  
  if (!fIDVolumeArray->size())
    {
      MGLog(error) << "Problem with the volumearray: " << endlog;
      MGLog(error) << "No list of volumes selected (neither one by one nor list)" << endlog;
      MGLog(error) << "Changing confining code to unconfined" << endlog;
      fConfinementCode = noconfined;
    }

  //Get masses of volumes involved
  for (size_t i=0;i<fIDVolumeArray->size();i++)
    {
      G4VPhysicalVolume* physvolume = thePhysicalVolumeStore->GetVolume((*fIDVolumeArray)[i]);
      if (!physvolume)
	{
	  MGLog(error) << "The volume called " << volumename << " does not exist" << endlog;
	  return;
	}
      G4double volMass = physvolume->GetLogicalVolume()->GetMass(false,false);
      MGLog(routine) << "Mass of volume " << volumename << "= " << volMass/kg << " kg" << endlog;
      totalMass += volMass;
      fMassFractionForVolumeArray->push_back(totalMass);
    }
  MGLog(routine) << "Number of volumes: " << fMassFractionForVolumeArray->size()-1 << endlog;
  MGLog(routine) << "Total mass: " << totalMass/kg << " kg" << endlog;

  for (size_t k=0;k<fMassFractionForVolumeArray->size();k++)
    {
      //Normalize to one
      (*fMassFractionForVolumeArray)[k] = (*fMassFractionForVolumeArray)[k]/totalMass;
      MGLog(routine) << "Fractional mass #" << k << " = " << (*fMassFractionForVolumeArray)[k] << endlog;
    }


  fVolumeArrayInitialized = true;
}

G4String MGGeneratorPrimary::ChooseVolumeFromList()
{
  G4String sampledVolume;
  if (!fVolumeListInitialized)
    {
      InitializeVolumeListSampling();
      if (!fVolumeListInitialized) //if it is still false, it means that the initialization failed
	{
	  MGLog(error) << "Not able to initialize the list" << endlog;
	  MGLog(error) << "Abort execution" << endlog;
          G4Exception("MGGeneratorPrimary::ChooseVolumeFromList()", "err001", FatalException, "Aborting execution");
	}
    }

  //Ok, now it is initialized and we can start

  G4double ran = G4UniformRand();
  //If ran is between W(k-1) and W(k), volume (k-1)-th is chosen, since W(0)=0
  G4double foundVolume = false;
  size_t ifound=0;
  for (size_t k=1;k<fMassFractionForVolumeList->size() && !foundVolume;k++)
    {
      if (ran > (*fMassFractionForVolumeList)[k-1] && ran <= (*fMassFractionForVolumeList)[k])
	{
	  foundVolume = true;//it breaks when this happens
	  ifound = k-1;
	}
    }
  MGLog(debugging) << "Found volume #" << ifound << "; random number: " << ran << endlog;
  G4int    elementnumber = -1;

  if (ifound > (fIDVolumeList->size()-1))
    {
      MGLog(error) << "Something strange is happening! " << endlog;
      MGLog(warning) << "index = " << ifound << endlog;
      MGLog(warning) << "Array size = " << fIDVolumeList->size();
      MGLog(warning) << "Sampling from the last volume " << (*fIDVolumeList)[fIDVolumeList->size()-1] 
		     << endlog;
      elementnumber = (*fIDVolumeList)[fIDVolumeList->size()-1];
    }
  else 
    elementnumber  = (*fIDVolumeList)[ifound];

  char     elnumber[4]; 
  sprintf(elnumber, "%d", elementnumber); 
  sampledVolume =  GetVolumeListName(); 	    
  sampledVolume += "_"; 
  sampledVolume += elnumber; 	 

  
  return sampledVolume;
}

G4String MGGeneratorPrimary::ChooseVolumeFromArray()
{
  G4String sampledVolume;
  if (!fVolumeArrayInitialized)
    {
      InitializeVolumeArraySampling();
      if (!fVolumeArrayInitialized) //if it is still false, it means that the initialization failed
	{
	  MGLog(error) << "Not able to initialize the array" << endlog;
	  MGLog(error) << "Abort execution" << endlog;
          G4Exception("MGGeneratorPrimary::ChooseVolumeFromArray()", "err001", FatalException, "Aborting execution");
	}
    }

  //Ok, now it is initialized and we can start
  G4double ran = G4UniformRand();
  //If ran is between W(k-1) and W(k), volume (k-1)-th is chosen, since W(0)=0
  G4double foundVolume = false;
  size_t ifound=0;
  for (size_t k=1;k<fMassFractionForVolumeArray->size() && !foundVolume;k++)
    {
      if (ran > (*fMassFractionForVolumeArray)[k-1] && ran <= (*fMassFractionForVolumeArray)[k])
	{
	  foundVolume = true;//it breaks when this happens
	  ifound = k-1;
	}
    }
  MGLog(debugging) << "Found volume #" << ifound << "; random number: " << ran << endlog;

  if (ifound > (fIDVolumeArray->size()-1))
    {
      MGLog(error) << "Something strange is happening! " << endlog;
      MGLog(warning) << "index = " << ifound << endlog;
      MGLog(warning) << "Array size = " << fIDVolumeArray->size();
      MGLog(warning) << "Sampling from the last volume " << (*fIDVolumeArray)[fIDVolumeArray->size()-1] 
		     << endlog;
      sampledVolume = (*fIDVolumeArray)[fIDVolumeArray->size()-1];
    }
  else 
    sampledVolume = (*fIDVolumeArray)[ifound]; 
  
  return sampledVolume;
}

void MGGeneratorPrimary::AddVolumeNumberToList(G4int det)
{
  if (!fIDVolumeList) fIDVolumeList = new std::vector<G4int>;
  fIDVolumeList->push_back(det);
  MGLog(debugging) << "Added detector # " << det << endlog;
  MGLog(debugging) << "Size of the list: " << fIDVolumeList->size() << endlog;
}

void MGGeneratorPrimary::AddVolumeNameToArray(G4String det)
{
  if (!fIDVolumeArray) fIDVolumeArray = new std::vector<G4String>;
  fIDVolumeArray->push_back(det);
  MGLog(debugging) << "Added detector # " << det << endlog;
  MGLog(debugging) << "Size of the list: " << fIDVolumeArray->size() << endlog;
}

void MGGeneratorPrimary::ClearList()
{
  MGLog(debugging) << "Going to clear volume list" << endlog;
  if (fIDVolumeList)
    {
      delete fIDVolumeList;
      fIDVolumeList = NULL;
      MGLog(debugging) << "Volume list cleared" << endlog;
   }
  fVolumeListInitialized = false;
}
