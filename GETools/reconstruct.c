#include <iostream.h>
#include <fstream.h>
#include <vector.h>

#include <TROOT.h> 
#include <TFile.h> 
#include <TTree.h> 
#include <TString.h> 
#include <TRandom.h> 

// --------------------------------------------------------- 

const int MAX_DETECTORS = 50; 
const int MAX_SEGMENTS = 50; 

const float THRESHOLD = 20 * 1e-3; 

// --------------------------------------------------------- 

void reconstruct(); 
float reconstruct_energy(float e, float m, float b); 

// --------------------------------------------------------- 

// define detector energy container 

typedef float energycontainer[MAX_SEGMENTS]; 

// define detector 

class TDetector
{
 public: 
  int detectorid;
  TString name; 
  int geometry;
  double innerradius;
  double outerradius;
  double height;
  double boredepth;
  double dlinnerthickness;
  double dlouterthickness;
  double dltopthickness;
  double dlbottomthickness;
  double passivationthickness;
  int nphisegments;
  int nzsegments;
  double angle;
  TString bulkmaterial;
  TString dlmaterial;
  TString passivationmaterial; 
  vector <float> resolution_b; 
  vector <float> resolution_m; 
}; 

// --------------------------------------------------------- 

int main() 
{
  reconstruct(); 

  return 0; 

}

// --------------------------------------------------------- 

void reconstruct()
{

  TString inputfilename    = "../PhaseII.root"; 
  TString geometryfilename = "../gerdageometry/geometry.dat"; 
  TString matrixfilename   = "../gerdageometry/matrix_test.dat"; 

   // initialize random number generator 

   gRandom = new TRandom(); 
   gRandom -> SetSeed(0); 

  // detector data base 

  vector<TDetector*> detectorcontainer; 

  // read input ROOT file 

  TString filename; 
  
  filename = inputfilename; 
  
  TFile* inputfile = new TFile(filename, "READ"); 

  // open output ROOT file 

  filename.Insert(filename.Sizeof() - 6, "_output"); 

  TFile* outputfile = new TFile(filename, "RECREATE"); 

  // get input tree 

  TTree* inputtree = (TTree*) inputfile -> Get("fTree"); 

  // input tree variables 

  int           eventnumber;
  float         hits_tote;
  int           seg_totnum;
  int           seg_id[MAX_SEGMENTS];
  float         seg_edep[MAX_SEGMENTS];
  
  // Set branch addresses.

  inputtree -> SetBranchAddress("eventnumber",&eventnumber);
  inputtree -> SetBranchAddress("hits_tote",&hits_tote);
  inputtree -> SetBranchAddress("seg_totnum",&seg_totnum);
  inputtree -> SetBranchAddress("seg_id",seg_id);
  inputtree -> SetBranchAddress("seg_edep",seg_edep);

  // open geometry file 

  filename = geometryfilename; 

  ifstream geometryfile; 

  geometryfile.open(filename, ifstream::in); 

  if (!geometryfile.is_open()) 
    {
      cout << " Could not open geometry file. Abort. " << endl; 
      return; 
    }   

  // open matrix file 

  filename = "../gerdageometry/matrix_test.dat"; 

  ifstream matrixfile; 

  matrixfile.open(filename, ifstream::in); 

  if (!matrixfile.is_open()) 
    {
      cout << " Could not open matrix file. Abort. " << endl; 
      return; 
    } 

  // read geometries 

  int ndetectors = 0; 
  int nsegments  = 0; 

  while (!geometryfile.eof())
    {
      int detectorid;
      char name[255];
      int geometry;
      double innerradius;
      double outerradius;
      double height;
      double boredepth;
      double dlinnerthickness;
      double dlouterthickness;
      double dltopthickness;
      double dlbottomthickness;
      double passivationthickness;
      int    nphisegments;
      int    nzsegments;
      double angle;
      char bulkmaterial[255];
      char dlmaterial[255];
      char passivationmaterial[255];
      vector <float> resolution_b; 
      vector <float> resolution_m; 

      geometryfile >> detectorid;
      geometryfile >> name;
      geometryfile >> geometry;
      geometryfile >> innerradius;
      geometryfile >> outerradius;
      geometryfile >> height;
      geometryfile >> boredepth;
      geometryfile >> dlinnerthickness;
      geometryfile >> dlouterthickness;
      geometryfile >> dltopthickness;
      geometryfile >> dlbottomthickness;
      geometryfile >> passivationthickness;
      geometryfile >> nphisegments;
      geometryfile >> nzsegments;
      geometryfile >> angle;
      geometryfile >> bulkmaterial;
      geometryfile >> dlmaterial;
      geometryfile >> passivationmaterial;

      for (int i = 0; i < nphisegments * nzsegments + 1; i++) 
	{
	  float m, b; 
	  
	  geometryfile >> b; 
	  geometryfile >> m; 
	  
	  resolution_b.push_back(b * 1e-3); 
	  resolution_m.push_back(m * 1e-3); 
	}

      // add to detector database 

      TDetector* detector = new TDetector(); 

      detector -> detectorid = detectorid;
      detector -> name = TString(name);
      detector -> geometry = geometry;
      detector -> innerradius = innerradius;
      detector -> outerradius = outerradius;
      detector -> height = height; 
      detector -> boredepth = boredepth;
      detector -> dlinnerthickness = dlinnerthickness;
      detector -> dlouterthickness = dlouterthickness;
      detector -> dltopthickness = dltopthickness;
      detector -> dlbottomthickness = dlbottomthickness;
      detector -> passivationthickness = passivationthickness;
      detector -> nphisegments = nphisegments;
      detector -> nzsegments = nzsegments;
      detector -> angle = angle;
      detector -> bulkmaterial = TString(bulkmaterial);
      detector -> dlmaterial = TString(dlmaterial); 
      detector -> passivationmaterial = TString(passivationmaterial); 
      detector -> resolution_b = resolution_b; 
      detector -> resolution_m = resolution_m; 

      detectorcontainer.push_back(detector); 

      // increase number of detectors and segments 

      ndetectors++; 
      nsegments += nphisegments * nzsegments; 
      
      if (nphisegments * nzsegments == 0) 
	nsegments++; 

    }

  // read matrix 

  int matrix[19][5]; 

  for (int j = 0; j < 5; j++)
    for (int i = 0; i < 19; i++)
      {
	int type; 

	matrixfile >> type; 

	matrix[i][j] = type; 
      }

  // create output tree 

  TTree* outputtree = new TTree("fTree", "GERDA Tree"); 

  // output tree variables 

  int   event_number; 
  float event_energy; 
  int   event_detectors; 
  int   event_segments; 

  int   detector_multiplicity; 
  int   segment_multiplicity; 
  
  int detector_electrodes[MAX_DETECTORS]; 
  energycontainer detector_energy[MAX_DETECTORS]; 
  energycontainer detector_energy_true[MAX_DETECTORS]; 

  // create branches 

  outputtree -> Branch("event_number", &event_number, "event number/I"); 
  outputtree -> Branch("event_energy", &event_energy, "total energy/F"); 
  outputtree -> Branch("event_detectors", &event_detectors, "number of detectors/I"); 
  outputtree -> Branch("event_segments", &event_segments, "number of segments/I"); 

  outputtree -> Branch("detector_multiplicity", &detector_multiplicity, "detector multiplicity/I"); 
  outputtree -> Branch("segment_multiplicity", &segment_multiplicity, "segment multiplicity/I"); 
  
  for (int i = 0; i < detectorcontainer.size(); i++)
    { 
      TString detectorname = detectorcontainer[i] -> name; 

      TString detectorname_segments = detectorname + "_segments"; 
      TString detectorname_segments2 = detectorname + " number of electrodes/I"; 

      TString detectorname_energy = detectorname + "_energy"; 
      TString detectorname_energy2 = "energy of " + detectorname + "[" + detectorname + " number of electrodes]/F"; 

      TString detectorname_energy_true = detectorname + "_energy_true"; 
      TString detectorname_energy_true2 = "true energy of " + detectorname + "[" + detectorname + " number of electrodes]/F"; 

      int nsegments = detectorcontainer[i] -> nphisegments * 
	detectorcontainer[i] -> nzsegments; 

      detector_electrodes[i] = nsegments + 1; 

      outputtree -> Branch(detectorname_segments.Data(), &detector_electrodes[i], detectorname_segments2.Data());  
      outputtree -> Branch(detectorname_energy.Data(), detector_energy[i], detectorname_energy2.Data());  
      outputtree -> Branch(detectorname_energy_true.Data(), detector_energy_true[i], detectorname_energy_true2.Data());  
    }

  // loop over input file 

  int nevents = inputtree -> GetEntries(); 

  for (int ievent = 0; ievent < nevents; ievent++) 
    {
      inputtree -> GetEntry(ievent); 

      // copy event information 

      event_number = eventnumber; 
      event_energy = hits_tote; 
      event_detectors = ndetectors; 
      event_segments = nsegments; 

      // assign energy to each detector/segment 

      int segmentid = 0; 

      // loop over possible detector positions 

      for (int i = 0; i < 19; i++)
	for (int j = 0; j < 5; j++)
	  {
	    // get detector type 

	    int detectorid = matrix[i][j]; 

	    if (detectorid >=0)
	      {
		int nphisegments = detectorcontainer[detectorid] -> nphisegments; 
		int nzsegments = detectorcontainer[detectorid] -> nzsegments; 

		// set core energy to zero 

		detector_energy_true[detectorid][0] = 0.0; 

		// loop over segments 

		for (int iphi = 0; iphi < TMath::Max(1, nphisegments); iphi++)
		  for (int iz = 0; iz < TMath::Max(1, nzsegments); iz++)
		    {
		      // get energy deposit 

		      int segmentnumber = iphi * nzsegments + iz + 1; 

		      detector_energy_true[detectorid][segmentnumber] = 0.0; 

		      // loop over all energy deposits 

		      for (int isegments = 0; isegments < seg_totnum; isegments++)
			{
			  if (segmentid == seg_id[isegments])
			    detector_energy_true[detectorid][segmentnumber] = seg_edep[isegments]; 
			}
		      
		      // increase segment id counter 
		      
		      segmentid++; 

		      // calculate core energy 

		      detector_energy_true[detectorid][0] += detector_energy_true[detectorid][segmentnumber]; 
		    }
	      }
	  }

      // smear energies and apply threshold cuts 

      for (int idetector = 0; idetector < event_detectors; idetector++)
	for (int ielectrode = 0; ielectrode < detector_electrodes[idetector]; ielectrode++) 
	  {
	    float energy = detector_energy_true[idetector][ielectrode]; 

	    float energy_reconstructed = reconstruct_energy(energy, 
							    detectorcontainer[idetector] -> resolution_m[ielectrode], 
							    detectorcontainer[idetector] -> resolution_b[ielectrode]); 
	    
	    if (energy_reconstructed < THRESHOLD) 
	      energy_reconstructed = 0.0; 

	    detector_energy[idetector][ielectrode] = energy_reconstructed; 
	  }

      // calculate multiplicities 

      detector_multiplicity = 0; 
      segment_multiplicity = 0; 

      for (int idetector = 0; idetector < event_detectors; idetector++)
	{
	  if (detector_energy[idetector][0] > 0.0) 
	    {
	      detector_multiplicity++; 
	      
	      if (detector_electrodes[idetector] == 1) 
		segment_multiplicity++; 
	    }

	  for (int ielectrode = 1; ielectrode < detector_electrodes[idetector]; ielectrode++) 
	    {
	      if (detector_energy[idetector][ielectrode] > 0.0) 
		segment_multiplicity++; 
	    }
	}

      // fill output tree 

      outputtree -> Fill(); 
    }

  // write output file 

  outputfile -> cd(); 
  outputtree -> Write(); 
  
  // close files 
  
  inputfile -> Close(); 
  outputfile -> Close(); 
  
  geometryfile.close(); 
  matrixfile.close(); 
  
}

float reconstruct_energy(float e, float m, float b) 
{

  float sigma = e * m + b; 

  float e_reconstructed = e + gRandom -> Gaus(0.0, sigma); 

  if (e_reconstructed < 0) 
    e_reconstructed = 0.0; 

  return e_reconstructed; 

}
