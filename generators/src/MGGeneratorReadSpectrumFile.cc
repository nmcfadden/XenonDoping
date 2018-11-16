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
// Shoots particle energy according to an energy spectrum (histogram) read from 
// an external file. The ShootEnergy() method is called by an other generator 
// (e.g. G4ParticleGun).
// 
// The file name must be indicated with the path, otherwise it is looked for in the
// current directory (just like it is done for decay0)
//
// The format of the file should be, for each line:
// Energy (keV)     PDF (a.u.)
//
// History:
// --------
// 29 May 2008   L.Pandola    First implementation
// 17 Mar 2011   A. Volynets  1. Optimized search in the cumulative distributions
//                            2. Some variables renamed
//


#include "generators/MGGeneratorReadSpectrumFile.hh"
#include "io/MGLogger.hh"
#include "G4PrimaryParticle.hh"
#include "G4ThreeVector.hh"
#include <fstream>
#include "Randomize.hh"

#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;


MGGeneratorReadSpectrumFile::MGGeneratorReadSpectrumFile() 
{
  fSourceFile = "NULL"; //default
  //fTheMessenger = new MGGeneratorReadSpectrumFileMessenger(this);

  fileInitialized = false;

}

MGGeneratorReadSpectrumFile::~MGGeneratorReadSpectrumFile()
{
  fVectorSpectrumCumulative.clear();
  fVectorEnergy.clear();
}


void MGGeneratorReadSpectrumFile::InitializeSpectrumFromFile()
{
  //1) check if the file has been set and if it exists
  if (fSourceFile == "NULL")
    {
      MGLog(error) << "Please set file name for MGGeneratorReadSpectrumFile" << endlog;
      G4Exception("MGGeneratorReadSpectrumFile::InitializeSpectrumFromFile()", "err001", FatalException, "Abort program");
    }  
  // First look for file in the current directory
  std::ifstream file(fSourceFile);
  if (!(file.is_open()))
    {
      file.close();
      //if not found, look in the MGGENERATORDATA directory
      char* path = getenv("MGGENERATORDATA");
      if (!path)
        {
          MGLog(error) << "You neither specified the sampling distribution file, nor put it into $MGGENERATORDATA" << endlog;
          G4Exception("MGGeneratorReadSpectrumFile::InitializeSpectrumFromFile()", "err002", FatalException, 
                      "MGGENERATORDATA environment variable not set!"); 
        }
      G4String pathString(path);
      G4String pathFile =  pathString + "/" + fSourceFile;
      file.open(pathFile);
      if (!(file.is_open()))
	{
	  MGLog(error) << "Data file " + pathFile + " not found!" << endlog;
	  MGLog(error) << "after looking in the current directory " << endlog;
	  MGLog(error) << "and in MGGENERATORDATA" << endlog;
	  MGLog(fatal);
          G4Exception("MGGeneratorReadSpectrumFile::InitializeSpectrumFromFile()", "err003", FatalException, " ");
	}
    }

  //
  //2) fill the vectors
  //Format is energy (keV) and content (a.u.)
  //
  //energybin is the lower edge of the bin
  G4double tempEn,tempFlux;
  //Integral spectrum
  fVectorSpectrumCumulative.push_back(0.);
  G4double cumul_flux;
  while(file.good())
    {
      file >> tempEn;
      if (file.eof()) break; // This is to avoid reading zeros if there is an empty line in the end
      file >> tempFlux;
      fVectorEnergy.push_back(tempEn*keV);

      // Now get the cumulative distribution:
      cumul_flux = fVectorSpectrumCumulative[ fVectorSpectrumCumulative.size()-1 ] + tempFlux;
      fVectorSpectrumCumulative.push_back(cumul_flux);
    }
  file.close();
  if (fVectorEnergy.size() < 2)
    {
      MGLog(error) << "There are " << fVectorEnergy.size() << " energy bins!" << endlog;
      MGLog(error) << "Cannot generate from this spectrum!" << endlog;
      G4Exception("MGGeneratorReadSpectrumFile::InitializeSpectrumFromFile()", "err004", FatalException, "Abort run");
  }

  G4double finalEnergyBin = fVectorEnergy[ fVectorEnergy.size()-1 ]
                          + fVectorEnergy[ fVectorEnergy.size()-1 ] - fVectorEnergy[ fVectorEnergy.size()-2 ];
  //final energy bin = E_n + (E_n - E_n-1)
  fVectorEnergy.push_back(finalEnergyBin);

  //3) Normalize the spectrum
  G4double normal = fVectorSpectrumCumulative[ fVectorSpectrumCumulative.size()-1 ];
  MGLog(debugging) << "Integral: " << normal << "." << endlog;

  for (size_t k=0;k<fVectorSpectrumCumulative.size();k++)
    {
      fVectorSpectrumCumulative[k] /= normal;
    }
  fileInitialized = true;
}


G4double MGGeneratorReadSpectrumFile::ShootEnergy()
{
  //
  // Here the energy is sampled. A random number is generated.
  // The sampled energy corresponds to the position of this random
  // number in cumulative distribution. The search is done using the
  // binary search which works much faster than the linear one.
  // Then the energy is once more randomized within the bin width.
  //
  // Caution!! You have to consider using the distribution with the same binning,
  // otherwise the sampled energy will have sizeable steps-shape. You should then
  // first convert the distribution to the same binning according to some model
  //
  if (!fileInitialized)
    {
      InitializeSpectrumFromFile();
      if (!fileInitialized)
      { //if it is still un-initialized = problems!
        MGLog(fatal) << "Source spectrum not initialized correctly" << endlog;
        G4Exception("MGGeneratorReadSpectrumFile::ShootEnergy()", "err001", FatalException, " ");
      }
    }
  G4double rand = G4UniformRand();

  // Now apply binary search. Works much faster than the linear search
  G4int lb=0,                                 // lb=>lower bound,
        ub=fVectorSpectrumCumulative.size();  // ub=>upper bound
  G4int mid = 0;                                  // current middle position
  G4double valLeft, valRight;                 // Values on the left/right from the curr. position
  for(;lb<ub;)
    {
      mid=(lb+ub)/2;
      valLeft  = fVectorSpectrumCumulative[mid-1];
      valRight = fVectorSpectrumCumulative[mid];
      if (valLeft <= rand   &&   rand < valRight)
        break;
      else if (valLeft <= rand)
        lb=mid;
      else if (rand < valRight)
        ub=mid;
    }
  if(lb>=ub)
    MGLog(error)<<"Binary search failed"<<endlog;

  // Now randomize within the bin width
  G4double enelow = fVectorEnergy[mid-1];
  G4double eneup = fVectorEnergy[mid];
  valLeft  = fVectorSpectrumCumulative[mid-1];
  valRight = fVectorSpectrumCumulative[mid];
  G4double ene = enelow + (eneup-enelow) * (rand - valLeft)/(valRight - valLeft);

  return ene;
}

void MGGeneratorReadSpectrumFile::ChangeFileName(G4String newname)
{
  if (newname == fSourceFile) return; //nothing happens
  MGLog(debugging) << "Try to register file " << newname << endlog;
  std::ifstream check(newname);
  if (!(check.is_open()))
    {
      check.close();
      //if not found, look in the MGGENERATORDATA directory
      char* path = getenv("MGGENERATORDATA");
      if (!path)
	{
          MGLog(error) << "You neither specified the sampling distribution file, nor put it into $MGGENERATORDATA" << endlog;
          G4Exception("MGGeneratorReadSpectrumFile::ChangeFileName()", "err001", FatalException, 
                      "MGGENERATORDATA environment variable not set!"); 
	}
      G4String pathString(path);
      G4String pathFile =  pathString + "/" + newname;
      check.open(pathFile);
      if (!(check.is_open()))
	{
	  MGLog(warning) << "Data file " + newname + " does not exist!" << endlog;
	  MGLog(warning) << "Nothing happens" << endlog;
	  return;
	}
    }
  check.close();
  MGLog(debugging) << "File found" << endlog;
  fileInitialized = false;
  fVectorEnergy.clear();
  fVectorSpectrumCumulative.clear();
  fSourceFile = newname;
  InitializeSpectrumFromFile();
  if (fileInitialized) 
    MGLog(debugging) << "File initialized" << endlog;
}
