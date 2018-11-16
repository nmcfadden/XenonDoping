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
 * 	Class containing methods to store and access the electric and weigthing fields calculated by MGSORFieldCalculation
 *
 * AUTHOR:
 * 	Daniel Lenz
 *
 * FIRST SUBMISSION:
 * 
 * REVISION: MM-DD-YYYY
 *   08-19-2008  Daniel: Mapped the grid only on interesting r range. r_outer - r_inner : r = (ir *delR) => r = (ir * delR) + r_inner
 *   08-29-2008  Daniel: Now proper separation between fXMaxGridpoints and fXMaxArrayIndex, instead of one var. fXMax
 *                       GetWPotential changed such that it returns the same values if inside inner bore holes as if on boundary. Same for outside. And same for the segments. 
 *   09-21-2008  Daniel: Introduced peridic boundary conditions using % now. 
 *                       Some additional "fixes", e.g. precision is now 8 instead of 12.   
 *   10-10-2008  Daniel: Added SaveToRootFile and LoadFromRootFile. This is default behaviour now. Still need to dump whole CrystalData into root file 
 *                       Changes in the interpolation, mainly small corrections for rare cases.
 *   07-23-2009  Jing:   fixed some minor units problems.
 *   05-28-2011  Jason:  Adapt to new const& storage of crystal data in crystal fields
 *   11-29-2011  Alex:   Fixed a bug concerning WPotential calculation introduced in r4287:
 *                       inside the bore hole the core WPotential should equal 1, not 0
 */

#include "waveform/MGSORCrystalFields.hh"
#include <sstream>
#include <cstdlib>
#include <math.h>

//include root headers needed to DrawFields
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TEllipse.h"
#include "TLine.h"

using namespace CLHEP;
using namespace std;

//=============================================================================
MGSORCrystalFields::MGSORCrystalFields(const MGCrystalData& crystal) : MGVCrystalFields(crystal)
{
  std::cout.precision(8);
  fEField = NULL;
  fWField = NULL;
  fFieldsOutputFile = NULL;
  fFieldsOutputTree = NULL;
  //  MGLogger::SetSeverity(MGLogger::routine);

}
//=============================================================================
MGSORCrystalFields::MGSORCrystalFields(const MGCrystalData& crystal,int gridrmax, int gridphimax, int gridzmax) : MGVCrystalFields(crystal)
{
  fRMaxGridpoints = gridrmax;
  fRMaxArrayIndex = gridrmax-1;

  fPhiMaxGridpoints = gridphimax;
  fPhiMaxArrayIndex = gridphimax-1;

  fZMaxGridpoints = gridzmax;
  fZMaxArrayIndex = gridzmax-1;

  std::cout.precision(8);
  fEField = NULL;
  fWField = NULL;
  fFieldsOutputFile = NULL;
  fFieldsOutputTree = NULL;

  AllocateArrays();
  //  MGLogger::SetSeverity(MGLogger::routine);

}
//=============================================================================
MGSORCrystalFields::~MGSORCrystalFields()
{
  if(fEField!=NULL && fWField!=NULL)DeAllocateArrays();
  if(fFieldsOutputFile != NULL){
    fFieldsOutputFile->Close();
    delete fFieldsOutputFile;
  }


}
//=============================================================================
void MGSORCrystalFields::SaveFields()
{
  //   std::string FileName(fFileName);
  //   FileName.append(fAppendix);
  //   SaveFieldToTxt(fRMaxArrayIndex,fPhiMaxArrayIndex,fZMaxArrayIndex, fEField, FileName.c_str());
  //   std::string Name(fFileName);
  //   SaveFieldToRoot(fRMaxArrayIndex,fPhiMaxArrayIndex,fZMaxArrayIndex, fEField, Name.c_str());
  std::string FileName(fFileName);
  SaveFieldToRoot(fRMaxArrayIndex,fPhiMaxArrayIndex,fZMaxArrayIndex, fEField, FileName.c_str());

}
//=============================================================================
void MGSORCrystalFields::LoadFields()
{
  //LoadFieldFromTxt(GetFileName().c_str(),GetFileName().c_str());
  LoadFieldFromRoot();
}
//=============================================================================
void MGSORCrystalFields::DrawFields()
{ 

  int NSegs = fCrystalData->GetNSegments();
  float rOut = fCrystalData->GetOuterRadius()/mm;
  float rIn  = fCrystalData->GetInnerRadius()/mm;


  //Calculate number of bins in FULL Grids, not only crystal region!
  int  binsXYfullgrid = (int)((fRMaxGridpoints/(rOut - rIn)) * rOut);  
  int xbins = 20 * binsXYfullgrid;
  int ybins = 20 * binsXYfullgrid;
  //   int zbins = fZMaxGridpoints;
  float nfdelR = (rOut*2)/(float)xbins;

  //HELPS TO DRAW A PROPER DETECTOR
  const int M = 361;
  double cox[M],coy[M],cix[M],ciy[M];
  for(int i=0;i<M;i++){
    cox[i] = rOut*cos(2*M_PI*i/(M-1));
    coy[i] = rOut*sin(2*M_PI*i/(M-1));
    cix[i] = rIn*cos(2*M_PI*i/(M-1));
    ciy[i] = rIn*sin(2*M_PI*i/(M-1));
  }
  TGraph* g1 = new TGraph(M,cox,coy);
  TGraph* g2 = new TGraph(M,cix,ciy);  


  //WEIGHTING POTENTIAL
  //ARRAY FOR SUM WP
  float** sumWP = new float*[xbins+1]; 
  for(int i=0;i<xbins+1;i++){
    sumWP[i] = new float[ybins+1];
  }

  float lowerB = -rOut - (nfdelR/2.);
  float upperB = rOut + (nfdelR/2.);
  TH2F* sumWP2d = new TH2F("sumWP","sumWP",xbins-1,lowerB,upperB,ybins-1,lowerB,upperB);
  for(int x = 0 ; x<=xbins; x++){
    for(int y =0; y<=ybins; y++){
      sumWP[x][y]=0;
    }
  }

  //FILL THE HISTOGRAMS
  for(int Seg=0;Seg<=NSegs;Seg++){
    TH2F* tmpWP2d = new TH2F("test2","test2",xbins-1,-rOut,rOut,ybins-1,-rOut,rOut);

    tmpWP2d->SetStats(kFALSE);
    for(int x = 0 ; x<=xbins; x++){
      for(int y =0; y<=ybins; y++){
        float xpos = (x * nfdelR)-rOut;
        float ypos = (y * nfdelR)-rOut;
        float zpos = 0;
        Hep3Vector coord;
        coord.set(xpos*mm,ypos*mm,zpos*mm);
        if( rOut >= sqrt(xpos*xpos + ypos*ypos)  &&
            rIn <= sqrt(xpos*xpos + ypos*ypos) ){
          float WP = this->GetWPotential(coord,Seg);
          tmpWP2d->SetBinContent(x,y,WP);
          if(Seg!=0){sumWP[x][y]+=WP;}
        }
      }
    }

    //PREPARE THE CANVAS
    TCanvas* cv = new TCanvas("","",500,500);
    cv->SetRightMargin(cv->GetRightMargin()*1.5);
    cv->SetLogz(1);

    //PLOT THE "DETECTOR" 
    g1->SetLineWidth(3);
    g1->Draw("al");
    g1->SetTitle("");
    g1->GetXaxis()->SetTitle("x [mm]");
    g1->GetXaxis()->CenterTitle();
    g1->GetYaxis()->SetTitle("y [mm]");
    g1->GetYaxis()->CenterTitle();

    //DRAW THE HISTOGRAM
    tmpWP2d->SetTitle("");
    tmpWP2d->SetContour(100);
    tmpWP2d->GetZaxis()->SetRangeUser(0.0001,1.001);
    tmpWP2d->Draw("COLZSAME");

    //PLOT THE "DETECTOR" PART 2
    g2->SetLineWidth(3);
    g2->Draw("l");
    g1->Draw("l");

    //TO HAVE NICE PLOTS 
    gStyle->SetPalette(1);
    gStyle->SetStripDecimals(kFALSE);
    gStyle->SetColorModelPS(1);

    //DO THE OUTPUT
    std::stringstream histOutNameStream;
    histOutNameStream<<this->GetFileName() <<"_WP_Seg"<<Seg;
    std::string histOutNamePNG(histOutNameStream.str());
    histOutNamePNG.append(".png");
    cv->Print(histOutNamePNG.c_str());
    std::string histOutNameEPS(histOutNameStream.str());
    histOutNameEPS.append(".eps");
    cv->Print(histOutNameEPS.c_str());
    tmpWP2d->Reset();
    delete tmpWP2d;  
    delete cv;
  }

  //TO FILL SUMWP PLOT 
  for(int x = 0 ; x<=xbins; x++){
    for(int y =0; y<=ybins; y++){
      sumWP2d->SetBinContent(x,y,sumWP[x][y]);
    }
  }

  TCanvas* cv1 = new TCanvas("","",500,500);
  cv1->SetRightMargin(cv1->GetRightMargin()*1.5);
  //  cv1->SetLogz(1);
  //PLOT THE "DETECTOR" 
  g1->SetLineWidth(3);
  g1->Draw("al");
  g1->SetTitle("");
  g1->GetXaxis()->SetTitle("x [mm]");
  g1->GetXaxis()->CenterTitle();
  g1->GetYaxis()->SetTitle("y [mm]");
  g1->GetYaxis()->CenterTitle();
  //DRAW THE HISTOGRAM
  sumWP2d->SetTitle("");
  sumWP2d->SetContour(100);
  //   sumWP2d->GetZaxis()->SetRangeUser(0.0001,1);
  sumWP2d->Draw("COLZSAME");

  //PLOT THE "DETECTOR" PART 2
  g2->SetLineWidth(3);
  g2->Draw("l");
  g1->Draw("l");

  //TO HAVE NICE PLOTS 
  gStyle->SetPalette(1);
  gStyle->SetStripDecimals(kFALSE);
  gStyle->SetColorModelPS(1);

  //DO THE OUTPUT
  std::stringstream histOutNameStream;
  histOutNameStream<<this->GetFileName() <<"_WP_SumSeg";
  std::string histOutNamePNG(histOutNameStream.str());
  histOutNamePNG.append(".png");
  cv1->Print(histOutNamePNG.c_str());
  std::string histOutNameEPS(histOutNameStream.str());
  histOutNameEPS.append(".eps");
  cv1->Print(histOutNameEPS.c_str());
  delete cv1;




  //NOW THE ELECTRIC FIELD
  TH2F* tmpER2d = new TH2F("ER","ER",xbins,-rOut,rOut,ybins,-rOut,rOut);
  TH1F* tmpER1d = new TH1F("ER1d","ER1d",xbins,-rOut,rOut);
  tmpER2d->SetStats(kFALSE);
  for(int x = 0 ; x<=xbins; x++){
    for(int y =0; y<=ybins; y++){
      float xpos = (x * nfdelR)-rOut;
      float ypos = (y * nfdelR)-rOut;
      float zpos = 0;
      Hep3Vector coord;
      coord.set(xpos*mm,ypos*mm,zpos*mm);
      Hep3Vector EField = this->GetEField(coord);
      if(floor(ybins/2.)==y){
        tmpER1d->SetBinContent(x,EField.getRho()/(volt/mm));
      }
      tmpER2d->SetBinContent(x,y,EField.getRho()/(volt/mm));
    }
  }

  TCanvas* cv2 = new TCanvas("","",500,500);
  cv2->SetRightMargin(cv2->GetRightMargin()*1.5);
  //  cv2->SetLogz(1);
  //PLOT THE "DETECTOR" 
  g1->SetLineWidth(3);
  g1->Draw("al");
  g1->SetTitle("");
  g1->GetXaxis()->SetTitle("x [mm]");
  g1->GetXaxis()->CenterTitle();
  g1->GetYaxis()->SetTitle("y [mm]");
  g1->GetYaxis()->CenterTitle();
  //DRAW THE HISTOGRAM
  tmpER2d->SetTitle("");
  tmpER2d->SetContour(100);
  //   tmpER2d->GetZaxis()->SetRangeUser(0.0001,1);
  tmpER2d->GetZaxis()->SetTitle("E(r) [V/mm]");
  //  tmpER2d->GetZaxis()->SetRangeUser(0,);
  tmpER2d->Draw("COLZSAME");

  //PLOT THE "DETECTOR" PART 2
  g2->SetLineWidth(3);
  g2->Draw("l");
  g1->Draw("l");

  //TO HAVE NICE PLOTS 
  gStyle->SetPalette(1);
  gStyle->SetStripDecimals(kFALSE);
  gStyle->SetColorModelPS(1);

  //DO THE OUTPUT
  std::stringstream histEOutNameStream;
  histEOutNameStream<<this->GetFileName() <<"_ER";
  std::string histEOutNamePNG(histEOutNameStream.str());
  histEOutNamePNG.append(".png");
  cv2->Print(histEOutNamePNG.c_str());
  std::string histEOutNameEPS(histEOutNameStream.str());
  histEOutNameEPS.append(".eps");
  cv2->Print(histEOutNameEPS.c_str());
  delete cv2;
  delete tmpER2d;

  TCanvas* cv3 = new TCanvas();
  tmpER1d->SetTitle("");
  tmpER1d->SetStats(kFALSE);
  tmpER1d->Draw();
  //DO THE OUTPUT
  std::stringstream histE1dOutNameStream;
  histE1dOutNameStream<<this->GetFileName() <<"_ER_1d";
  std::string histE1dOutNamePNG(histE1dOutNameStream.str());
  histE1dOutNamePNG.append(".png");
  cv3->Print(histE1dOutNamePNG.c_str());
  std::string histE1dOutNameEPS(histE1dOutNameStream.str());
  histE1dOutNameEPS.append(".eps");
  cv3->Print(histE1dOutNameEPS.c_str());


  delete cv3;
  delete tmpER1d;



  delete g1;
  delete g2;

  //  delete the sumWP again!
  for(int i=ybins;i>=0; i--){
    delete[] sumWP[i];
  }
  delete[] sumWP;

}
//=============================================================================
void MGSORCrystalFields::SaveFieldToTxt(int rmax, int phimax, int zmax, float**** array, const char* filename)
{

  //fXMaxArrayIndex should be given to that function


  bool isOpen = false;
  isOpen = OpenFile(filename); 

  if(isOpen){
    // write field to file 
    //header with all information needed
    fDataStream << "#Crystal Type    : "<< fCrystalData->GetDiodeType() <<"\t 0: kPType \t 1:kNType " <<std::endl;
    fDataStream << "#Crystal Geometry: "<< fCrystalData->GetGeometryType()<<"\t 0: kCoaxial \t 1:kClosedEnd \t 2:kPointContact "<<std::endl;
    fDataStream << "#Impurity Density [1/cm^3] = "<<fCrystalData->GetImpurityDensityTop() / (1./cm3)<<std::endl;
    fDataStream << "#Inner Radius [m]    = "<<fCrystalData->GetInnerRadius() / m<<std::endl;
    fDataStream << "#Outer Radius [m]    = "<<fCrystalData->GetOuterRadius() / m <<std::endl;
    fDataStream << "#Crystal Height [m]  = "<<fCrystalData->GetCrystalHeight() / m<<std::endl;
    fDataStream << "#Applied Voltage [V] = "<<fCrystalData->GetVoltage() / volt<<std::endl;
    fDataStream << "#Z Max Steps   = "<<fZMaxGridpoints << std::endl;
    fDataStream << "#R Max Steps   = "<<fRMaxGridpoints<<std::endl;
    fDataStream << "#Phi Max Steps = "<<fPhiMaxGridpoints<<std::endl;
    fDataStream << "#delZ  [m]   = rand "<<std::endl; 
    fDataStream << "#delR  [m]   = rand "<<std::endl;
    fDataStream << "#delPhi[rad] = rand "<<std::endl;
    fDataStream << "#Format of Saving: Er\t Ephi\t Ez\t Potential"<<std::endl;
    fDataStream << "#Electric Field BEGINS"<<std::endl;

    for(int ir=0;ir<=rmax;ir++){
      for(int iphi=0;iphi<=phimax;iphi++){
        for(int iz=0;iz<=zmax;iz++){
          fDataStream <<array[ir][iphi%phimax][iz][0] <<"\t"<< array[ir][iphi%phimax][iz][1]<<"\t"<< array[ir][iphi%phimax][iz][2] <<"\t"<< array[ir][iphi%phimax][iz][3]<<"\n ";

        }
        //	fDataStream<<"\n";
      } 
      //      fDataStream<<"\n";
    }

    bool isClosed = false;
    isClosed = CloseFile();
    if(!isClosed){std::cerr<<"MGSORCrystalFields::SaveFieldToTxt, could not close file "<<filename<<std::endl;
    }

  }
  else std::cerr<<"MGSORCrystalFields::SaveFieldToTxt, could not open file "<<filename<<std::endl;

}
//=============================================================================
void MGSORCrystalFields::SaveFieldToRoot(int rmax, int phimax, int zmax, float**** array, const char* filename)
{
  std::string realfilename(filename);
  realfilename.append(".root");  

  //try to find out which fields are supposed to be written
  size_t pos1,pos2;
  pos1 = fAppendix.find("_");
  pos2 = fAppendix.find(".dat");
  std::string tmpTreeName = fAppendix.substr(pos1+1,pos2-pos1-1); 

  bool bEField = false;
  if(fAppendix.find("_EField.dat") != std::string::npos){bEField = true;}


  // write all important information to file
  int RMaxGridpoints   = fRMaxGridpoints;
  int PhiMaxGridpoints = fPhiMaxGridpoints;
  int ZMaxGridpoints   = fZMaxGridpoints;

  //dump complete Crystal Data!!!
  //first dump some basic infos:
  double InnerRadius = fCrystalData->GetInnerRadius();
  double OuterRadius = fCrystalData->GetOuterRadius();
  double CrystalHeight = fCrystalData->GetCrystalHeight();

  //if no file open, open it!
  if( fFieldsOutputFile == NULL ){
    fFieldsOutputFile = new TFile(realfilename.c_str(), "RECREATE", "Fields Storage File",9); //set maximum compression, since expecting big files!!!
  }
  //create a new tree
  fFieldsOutputTree = new TTree(tmpTreeName.c_str(),tmpTreeName.c_str());

  //and write some variable into it 
  fFieldsOutputTree -> Branch("RMaxGridpoints", &RMaxGridpoints, "RMaxGridpoints/I");
  fFieldsOutputTree -> Branch("PhiMaxGridpoints", &PhiMaxGridpoints, "PhiMaxGridpoints/I");
  fFieldsOutputTree -> Branch("ZMaxGridpoints", &ZMaxGridpoints, "ZMaxGridpoints/I");
  fFieldsOutputTree -> Branch("InnerRadius", &InnerRadius, "InnerRadius/D");
  fFieldsOutputTree -> Branch("OuterRadius", &OuterRadius, "OuterRadius/D");
  fFieldsOutputTree -> Branch("CrystalHeight", &CrystalHeight, "CrystalHeight/D");

  if(bEField){//    if it is the Electric field

    //fill the arrays now, to write into tree
    int array_size = RMaxGridpoints * PhiMaxGridpoints * ZMaxGridpoints;    

    float* E_R;
    float* E_Phi;
    float* E_Z;
    float* Potential;

    E_R = new float[array_size];
    E_Phi = new float[array_size];
    E_Z = new float[array_size];
    Potential = new float[array_size];

    fFieldsOutputTree -> Branch("ArraySize",&array_size,"ArraySize/I");
    fFieldsOutputTree -> Branch("E_R",E_R,"E_R[ArraySize]/F");
    fFieldsOutputTree -> Branch("E_Phi",E_Phi,"E_Phi[ArraySize]/F");  
    fFieldsOutputTree -> Branch("E_Z",E_Z,"E_Z[ArraySize]/F");
    fFieldsOutputTree -> Branch("Potential",Potential,"Potential[ArraySize]/F");


    fFieldsOutputFile->cd();

    int index = 0;

    for(int ir=0;ir<=rmax;ir++){
      for(int iphi=0;iphi<=phimax;iphi++){
        for(int iz=0;iz<=zmax;iz++){
          E_R[index] = array[ir][iphi%phimax][iz][0];
          E_Phi[index] = array[ir][iphi%phimax][iz][1];
          E_Z[index] = array[ir][iphi%phimax][iz][2];
          Potential[index] = array[ir][iphi%phimax][iz][3];

          index++;
        }
      } 
    }

    //write everything to tree
    fFieldsOutputTree->Fill();

    delete[] E_R;
    delete[] E_Phi;
    delete[] E_Z;
    delete[] Potential;
  }

  else{//for weighting fields

    int array_size = RMaxGridpoints * PhiMaxGridpoints * ZMaxGridpoints;    
    // Weighting Field and Wpotential
    float* WF_R;
    float* WF_Phi;
    float* WF_Z;
    float* WPotential;

    WF_R = new float[array_size];
    WF_Phi = new float[array_size];
    WF_Z = new float[array_size];
    WPotential = new float[array_size];

    fFieldsOutputTree -> Branch("ArraySize",&array_size,"ArraySize/I");
    fFieldsOutputTree -> Branch("WF_R",WF_R,"WF_R[ArraySize]/F");
    fFieldsOutputTree -> Branch("WF_Phi",WF_Phi,"WF_Phi[ArraySize]/F");  
    fFieldsOutputTree -> Branch("WF_Z",WF_Z,"WF_Z[ArraySize]/F");
    fFieldsOutputTree -> Branch("WPotential",WPotential,"WPotential[ArraySize]/F");



    fFieldsOutputFile->cd();

    int wfIndex = 0;

    for(int ir=0;ir<=fRMaxArrayIndex;ir++){
      for(int iphi=0;iphi<=fPhiMaxArrayIndex;iphi++){
        for(int iz=0;iz<=fZMaxArrayIndex;iz++){
          WF_R[wfIndex] = array[ir][iphi%phimax][iz][0];
          WF_Phi[wfIndex] = array[ir][iphi%phimax][iz][1];
          WF_Z[wfIndex] = array[ir][iphi%phimax][iz][2];
          WPotential[wfIndex] = array[ir][iphi%phimax][iz][3];  
          wfIndex++;
        }
      }
    }

    //write everything to tree
    fFieldsOutputTree->Fill();

    delete[] WF_R;
    delete[] WF_Phi;
    delete[] WF_Z;
    delete[] WPotential;

  }
  //  write the Tree and delete it
  fFieldsOutputTree->Write();
  delete fFieldsOutputTree;

}
//=============================================================================
void MGSORCrystalFields::LoadFieldFromTxt(const char* efieldFilename, const char* wfieldsFilename)
{

  char filename[2000];
  sprintf(filename,"%s_EField.dat",efieldFilename);

  //first load efield
  bool isOpen = false;
  isOpen = OpenFile(filename);  
  if(isOpen){
    //ReadHeader and extract important information concerning size of array then AllocateArrays
    ReadHeader();
    if(fEField==NULL && fWField==NULL)AllocateArrays();
    else ResetFields();

    //loop over file to read in EField
    for(int ir=0;ir<=fRMaxArrayIndex;ir++){
      for(int iphi=0;iphi<=fPhiMaxArrayIndex;iphi++){
        for(int iz=0;iz<=fZMaxArrayIndex;iz++){
          fDataStream >> fEField[ir][iphi][iz][0] >> fEField[ir][iphi][iz][1] >> fEField[ir][iphi][iz][2] >> fEField[ir][iphi][iz][3];
        }
      } 
    }

    bool isClosed = false;
    isClosed = CloseFile();
    if(!isClosed){std::cerr<<"MGSORCrystalFields::LoadFieldFromTxt, could not close file "<<filename<<std::endl;}    
  }
  else std::cerr<<"MGSORCrystalFields::LoadFieldFromTxt, could not open file "<<filename<<std::endl;

  //now load all segments weigthing fields
  for(size_t seg=0;seg<=fCrystalData->GetNSegments();seg++){
    std::ostringstream os;
    os << wfieldsFilename << "_WField_Segment_" << seg << ".dat";
    isOpen = false;
    isOpen = OpenFile(os.str().c_str()); 
    if(isOpen){
      //need to read header for each file, to cut away the header
      ReadHeader();

      //loop over file to read in EField
      for(int ir=0;ir<=fRMaxArrayIndex;ir++){
        for(int iphi=0;iphi<=fPhiMaxArrayIndex;iphi++){
          for(int iz=0;iz<=fZMaxArrayIndex;iz++){
            fDataStream >> fWField[seg][ir][iphi][iz][0] >> fWField[seg][ir][iphi][iz][1] >> fWField[seg][ir][iphi][iz][2] >> fWField[seg][ir][iphi][iz][3];
          }
        }
      }

      bool isClosed = false;
      isClosed = CloseFile();
      if(!isClosed){ std::cerr << "MGSORCrystalFields::LoadFieldFromTxt, could not close file " << os.str() << std::endl;}    
    }
    else std::cerr << "MGSORCrystalFields::LoadFieldFromTxt, could not open file "<< os.str() << std::endl;

  }


}
//=============================================================================
void MGSORCrystalFields::LoadFieldFromRoot()
{
  //construct Filename and open 
  std::string realfilename( this->GetFileName() );
  realfilename.append(".root");  

  std::cout<<"MGSORCrystalFields::LoadFieldFromRoot, loading fields from file "<<realfilename<<std::endl;

  //if no file is open, open them
  if(fFieldsOutputFile != NULL){
    if(!fFieldsOutputFile->IsOpen()){
      fFieldsOutputFile = new TFile(realfilename.c_str(), "READ"); 
    }
  }
  else{
    fFieldsOutputFile = new TFile(realfilename.c_str(), "READ"); 
  }

  //now file is open. Start reading in all possible values 

  // define some int, that are needed throughout reading
  int RMaxGridpoints;
  int PhiMaxGridpoints;
  int ZMaxGridpoints;
  int array_size;

  double InnerRadius;
  double OuterRadius;
  double CrystalHeight;


  // define some tmp arrays
  //   double* E_R;
  //   double* E_Phi;
  //   double* E_Z;
  //   double* Potential;

  //try to make everthing float
  float* E_R;
  float* E_Phi;
  float* E_Z;
  float* Potential;

  //first read in the EField
  fFieldsOutputTree = (TTree*)fFieldsOutputFile->Get("EField"); 

  fFieldsOutputTree -> SetBranchAddress("RMaxGridpoints", &RMaxGridpoints);
  fFieldsOutputTree -> SetBranchAddress("PhiMaxGridpoints", &PhiMaxGridpoints);
  fFieldsOutputTree -> SetBranchAddress("ZMaxGridpoints", &ZMaxGridpoints);
  fFieldsOutputTree -> SetBranchAddress("ArraySize", &array_size);
  fFieldsOutputTree -> SetBranchAddress("InnerRadius", &InnerRadius);
  fFieldsOutputTree -> SetBranchAddress("OuterRadius", &OuterRadius);
  fFieldsOutputTree -> SetBranchAddress("CrystalHeight", &CrystalHeight);

  fFieldsOutputTree ->GetEntry(0);

  //   E_R = new double[array_size];
  //   E_Phi = new double[array_size];
  //   E_Z = new double[array_size];
  //   Potential = new double[array_size];

  //try to make all float 
  E_R = new float[array_size];
  E_Phi = new float[array_size];
  E_Z = new float[array_size];
  Potential = new float[array_size];


  fFieldsOutputTree -> SetBranchAddress("E_R", E_R);
  fFieldsOutputTree -> SetBranchAddress("E_Phi", E_Phi);
  fFieldsOutputTree -> SetBranchAddress("E_Z", E_Z);
  fFieldsOutputTree -> SetBranchAddress("Potential", Potential);
  fFieldsOutputTree -> GetEntry(0);

  fRMaxGridpoints   = RMaxGridpoints; 
  fPhiMaxGridpoints = PhiMaxGridpoints ;
  fZMaxGridpoints   = ZMaxGridpoints;

  fRMaxArrayIndex   = fRMaxGridpoints - 1;
  fPhiMaxArrayIndex = fPhiMaxGridpoints - 1;
  fZMaxArrayIndex   = fZMaxGridpoints - 1;

  //are the right Arrays allocated already? if not do it now else reset them
  if(fEField==NULL && fWField==NULL)AllocateArrays();
  else ResetFields();  

  int index = 0;
  //loop over file to read in EField
  for(int ir=0;ir<=fRMaxArrayIndex;ir++){
    for(int iphi=0;iphi<=fPhiMaxArrayIndex;iphi++){
      for(int iz=0;iz<=fZMaxArrayIndex;iz++){
        fEField[ir][iphi][iz][0] = E_R[index];
        fEField[ir][iphi][iz][1] = E_Phi[index];
        fEField[ir][iphi][iz][2] = E_Z[index];
        fEField[ir][iphi][iz][3] = Potential[index];

        index++;
      }
    } 
  }

  //need to reset all branch addresses before deleting any array
  fFieldsOutputTree->ResetBranchAddresses();


  //delete the tmp arrays again
  delete[] E_R;
  delete[] E_Phi;
  delete[] E_Z;
  delete[] Potential;


  //now fill the WP Trees

  for(size_t seg=0;seg<=fCrystalData->GetNSegments();seg++){
    std::ostringstream os;
    os << "WField_Segment_" << seg;
    // define some tmp arrays
    //     double* WF_R;
    //     double* WF_Phi;
    //     double* WF_Z;
    //     double* WPotential;

    //try to make all float
    float* WF_R;
    float* WF_Phi;
    float* WF_Z;
    float* WPotential;


    fFieldsOutputTree = (TTree*)fFieldsOutputFile->Get(os.str().c_str()); 
    fFieldsOutputTree -> SetBranchAddress("RMaxGridpoints", &RMaxGridpoints);
    fFieldsOutputTree -> SetBranchAddress("PhiMaxGridpoints", &PhiMaxGridpoints);
    fFieldsOutputTree -> SetBranchAddress("ZMaxGridpoints", &ZMaxGridpoints);
    fFieldsOutputTree -> SetBranchAddress("ArraySize", &array_size);
    fFieldsOutputTree ->GetEntry(0);

    //     WF_R = new double[array_size];
    //     WF_Phi = new double[array_size];
    //     WF_Z = new double[array_size];
    //     WPotential = new double[array_size];

    //try to make everthing float
    WF_R = new float[array_size];
    WF_Phi = new float[array_size];
    WF_Z = new float[array_size];
    WPotential = new float[array_size];

    fFieldsOutputTree -> SetBranchAddress("WF_R", WF_R);
    fFieldsOutputTree -> SetBranchAddress("WF_Phi", WF_Phi);
    fFieldsOutputTree -> SetBranchAddress("WF_Z", WF_Z);
    fFieldsOutputTree -> SetBranchAddress("WPotential", WPotential);
    fFieldsOutputTree ->GetEntry(0);

    //loop over file to read in EField
    int wfIndex=0;

    for(int ir=0;ir<=fRMaxArrayIndex;ir++){
      for(int iphi=0;iphi<=fPhiMaxArrayIndex;iphi++){
        for(int iz=0;iz<=fZMaxArrayIndex;iz++){
          fWField[seg][ir][iphi][iz][0] = WF_R[wfIndex];
          fWField[seg][ir][iphi][iz][1] = WF_Phi[wfIndex];
          fWField[seg][ir][iphi][iz][2] = WF_Z[wfIndex];
          fWField[seg][ir][iphi][iz][3] = WPotential[wfIndex];

          wfIndex++;
        }
      } 
    }

    //need to reset all branch addresses before deleting any array
    fFieldsOutputTree->ResetBranchAddresses();
    delete[] WF_R;
    delete[] WF_Phi;
    delete[] WF_Z;
    delete[] WPotential;
  }

  //finally 
  MGCrystalData::EGeometryType type = fCrystalData->GetGeometryType();
  if      (type==MGCrystalData::kCoaxial)   fdelR   = (OuterRadius - InnerRadius) /(double)(fRMaxGridpoints - 1);
  else if (type==MGCrystalData::kClosedEnd) fdelR   = OuterRadius /(double)(fRMaxGridpoints - 1);
  fdelPhi =  2 * M_PI/(double)(fPhiMaxGridpoints - 1);  
  fdelZ = CrystalHeight/(double) (fZMaxGridpoints - 1);

  std::cout<<"MGSORCrystalFields::LoadFieldFromRoot, fields loaded "<<std::endl;

}
//=============================================================================
Hep3Vector MGSORCrystalFields::GetEField(const Hep3Vector& coordinates)const
{
  Hep3Vector EVec(0.,0.,0.);

  // return a value only if inside the crystal, else 0
  if (fCrystalData->GetGeometryType()==MGCrystalData::kCoaxial)
  {
    if( coordinates.getRho() <= fCrystalData->GetOuterRadius() && coordinates.getRho() >= fCrystalData->GetInnerRadius() && fabs(coordinates.getZ())<= fCrystalData->GetCrystalHeight()/ 2.){
      EVec = Interpolation(CoordinateTransform(coordinates));
    }
  }
  else
    if( coordinates.getRho() <= fCrystalData->GetOuterRadius() && fabs(coordinates.getZ())<= fCrystalData->GetCrystalHeight()/ 2. &&
        // here is the well-type condition (the well is from the top)
        (coordinates.getZ() <= fCrystalData->GetCrystalHeight()/ 2.-fCrystalData->GetWellDepth() || coordinates.getRho() >= fCrystalData->GetInnerRadius() ) ){
      EVec = Interpolation(CoordinateTransform(coordinates));
    }

  return EVec;
}
//=============================================================================
Hep3Vector MGSORCrystalFields::GetWField(const Hep3Vector& coordinates, size_t segment)const
{

  Hep3Vector WField(0.,0.,0.);


  // return a value only if inside the crystal, else 0
  if (fCrystalData->GetGeometryType()==MGCrystalData::kCoaxial)
  {
    if( coordinates.getRho() <= fCrystalData->GetOuterRadius() && coordinates.getRho() >= fCrystalData->GetInnerRadius() && fabs(coordinates.getZ())<= fCrystalData->GetCrystalHeight()/ 2.){
      WField = InterpolationWField(CoordinateTransform(coordinates),segment)  ;
    }
  }
  else
    if( coordinates.getRho() <= fCrystalData->GetOuterRadius() && fabs(coordinates.getZ())<= fCrystalData->GetCrystalHeight()/ 2. &&
        // here is the well-type condition (the well is from the top)
        (coordinates.getZ() <= fCrystalData->GetCrystalHeight()/ 2.-fCrystalData->GetWellDepth() || coordinates.getRho() >= fCrystalData->GetInnerRadius() ) ){
      WField = InterpolationWField(CoordinateTransform(coordinates),segment)  ;
    }
  // return a value only if inside the crystal, else 0
  if( (coordinates.getRho() <= (fCrystalData->GetOuterRadius()) && coordinates.getRho() >= (fCrystalData->GetInnerRadius()) ) && (coordinates.getZ()<= (fCrystalData->GetCrystalHeight()/ 2.) && coordinates.getZ()>= - (fCrystalData->GetCrystalHeight()/ 2.) ) ){
  }

  return WField;
}
//=============================================================================
double MGSORCrystalFields::GetWPotential(const Hep3Vector& coordinates, size_t segment)const
{


  //Calculate the segment boundary in grid points
  //phi: 
  double PhiSegmentSpanningAngleInGridPoints = ((float)fPhiMaxGridpoints/(float)fCrystalData->GetNPhiSegments());
  int PhiSegment = int ((segment - 1) / (float)fCrystalData->GetNZSegments());

  int iphiLowSegBoundary;
  int iphiUpSegBoundary;

  iphiLowSegBoundary = int ((PhiSegment) * PhiSegmentSpanningAngleInGridPoints);
  //  iphiUpSegBoundary = int (((PhiSegment + 1) * PhiSegmentSpanningAngleInGridPoints)-1);
  iphiUpSegBoundary = int (((PhiSegment + 1) * PhiSegmentSpanningAngleInGridPoints));

  //   //     //   // // DEBUG !!!
  //   std::cout<<std::endl;
  //   std::cout<<" Segment = "<< segment<<std::endl;
  //   std::cout<<" PhiSegment = "<< PhiSegment<<std::endl;
  //   std::cout<<" PhiSegmentSpanningAngleInGridPoints = "<<PhiSegmentSpanningAngleInGridPoints<<std::endl;
  //   std::cout<<" iphiLowSegBoundary = "<< iphiLowSegBoundary<<std::endl;
  //   std::cout<<" iphiUpSegBoundary = "<< iphiUpSegBoundary<<std::endl;
  //   std::cout<<" fPhiMaxArrayIndex = "<< fPhiMaxArrayIndex<<std::endl;

  //z:
  double ZSegmentHeightInGridPoints = ((float)fZMaxGridpoints/(float)fCrystalData->GetNZSegments());
  int ZSegment = (segment - ( PhiSegment * fCrystalData->GetNZSegments()) - 1);
  int izLowSegBoundary;
  int izUpSegBoundary;
  // Segment counting from Top to bottom 
  izUpSegBoundary = int( fZMaxArrayIndex - (ZSegment * ZSegmentHeightInGridPoints) +1);
  izLowSegBoundary = int (fZMaxArrayIndex - (((ZSegment + 1) * ZSegmentHeightInGridPoints)-1));

  // //   // //DEBUG!!!
  //   std::cout<<" Segment = "<< segment<<std::endl;
  //   std::cout<<" ZSegment = "<<ZSegment<<std::endl;
  //   std::cout<<" ZSegmentHeightInGridpoints = "<<ZSegmentHeightInGridPoints<<std::endl;
  //   std::cout<<" izLowSegBoundary = "<< izLowSegBoundary<<std::endl;
  //   std::cout<<" izUpSegBoundary = "<< izUpSegBoundary<<std::endl;
  //   std::cout<<" izMax = "<< fZMaxArrayIndex<<std::endl;


  //convert into height and rad
  double zLowSegBoundary = izLowSegBoundary * fdelZ;
  double zUpSegBoundary = izUpSegBoundary * fdelZ;  
  double phiLowSegBoundary = iphiLowSegBoundary * fdelPhi;
  double phiUpSegBoundary = iphiUpSegBoundary * fdelPhi;
  //   std::cout<<"fdelPhi = "<<fdelPhi<<std::endl;
  //   std::cout<<"fdelZ = "<<fdelZ<<std::endl;
  //   std::cout<<" phiLowSegBoundary = "<< phiLowSegBoundary<<std::endl;
  //   std::cout<<" phiUpSegBoundary = "<< phiUpSegBoundary<<std::endl;
  //   std::cout<<" zLowSegBoundary = "<< zLowSegBoundary<<std::endl;
  //   std::cout<<" zUpSegBoundary = "<< zUpSegBoundary<<std::endl;

  double WPotential = 0;

  Hep3Vector NewCoordinates = CoordinateTransform(coordinates);

  double changePhi = NewCoordinates.getPhi()/CLHEP::rad;
  if(changePhi<0){changePhi=changePhi + 2* M_PI;}


  //  std::cout<<"R: "<< NewCoordinates.getRho()<< std::endl;
  //   //work around to make calculation of waveforms easier  
  if(segment == 0){
    if(NewCoordinates.getRho() < (fCrystalData->GetInnerRadius())){ //core and inside the bore hole
      return 1;
    }
    else if(NewCoordinates.getRho() > (fCrystalData->GetOuterRadius())){//core and outside the crystal
      return 0;
    }
    else{
      if( NewCoordinates.getRho() <= fCrystalData->GetOuterRadius() && NewCoordinates.getZ()<= fCrystalData->GetCrystalHeight() && NewCoordinates.getZ() >= 0 ){
        WPotential = InterpolationWPotential(NewCoordinates,segment);
        return WPotential;
      }
      else return 0;
    }
  }
  else{
    if(NewCoordinates.getRho() <= (fCrystalData->GetInnerRadius())){//segments and inside bore hole
      return 0;
    }
    if(NewCoordinates.getRho() >= (fCrystalData->GetOuterRadius()) && changePhi >= phiLowSegBoundary && changePhi <= phiUpSegBoundary && NewCoordinates.getZ() >= zLowSegBoundary &&  NewCoordinates.getZ() <= zUpSegBoundary  ){//segments and outside crystal
      return 1;
    }
    else{
      if( NewCoordinates.getRho() < fCrystalData->GetOuterRadius() && NewCoordinates.getZ()<= fCrystalData->GetCrystalHeight() && NewCoordinates.getZ() >= 0 ){
        WPotential = InterpolationWPotential(NewCoordinates,segment);
        return WPotential;

      }
      else {
        //if segment 1 has WP 1, at 360 degree WP should be equal 1 again
        if(changePhi == 2* M_PI && segment==1 && NewCoordinates.getRho() >= fCrystalData->GetOuterRadius()){ return 1;}
        return 0;
      }
    }
  }
}
//=============================================================================
Hep3Vector MGSORCrystalFields::GetGrid()
{ //returns the fXMaxGridpoints
  Hep3Vector gridsize(fRMaxGridpoints,fPhiMaxGridpoints, fZMaxGridpoints);
  return gridsize;

}
//=============================================================================
//double**** MGSORCrystalFields::GetArray()
//try to make everthing float
float**** MGSORCrystalFields::GetArray()
{
  if(fEField==NULL && fWField==NULL) AllocateArrays();
  return fEField;

}
//=============================================================================
void MGSORCrystalFields::ResetFields()
{
  if(fEField==NULL && fWField==NULL) AllocateArrays();
  else{
    //After each save of fields reset them!
    for(int i=0;i<=fRMaxArrayIndex;i++){
      for(int j=0;j<=fPhiMaxArrayIndex;j++){
        for(int k=0;k<=fZMaxArrayIndex;k++){
          for(int l=0;l<4;l++){//number of dimensions of array
            fEField[i][j][k][l]=0;
          }
        }
      }
    }    

    for(size_t seg=0;seg<fCrystalData->GetNSegments()+1;seg++){
      for(int i=0;i<=fRMaxArrayIndex;i++){
        for(int j=0;j<=fPhiMaxArrayIndex;j++){
          for(int k=0;k<=fZMaxArrayIndex;k++){
            for(int l=0;l<4;l++){
              fWField[seg][i][j][k][l]=0;
            }
          }
        }
      }    
    }
  }

}
//=============================================================================
void MGSORCrystalFields::Initialize()
{
  //  ReadHeader();
}
//=============================================================================
void MGSORCrystalFields::SetGrid(int rmax, int phimax, int zmax)
{
  fRMaxGridpoints = rmax; fRMaxArrayIndex = rmax - 1;
  fPhiMaxGridpoints = phimax; fPhiMaxArrayIndex = phimax - 1;
  fZMaxGridpoints = zmax; fZMaxArrayIndex = zmax - 1;
}

//=============================================================================
// void MGSORCrystalFields::SetFileName(std::string name)
// {
//   fFileName.assign(name);
// }
//=============================================================================
// From now on private function
//=============================================================================
Hep3Vector MGSORCrystalFields::Interpolation(Hep3Vector coordinates) const
{
  //
  //In here everything has to be done in meter and rad
  //

  double eR=0;  
  double ePhi=0;
  double eZ=0;

  MGCrystalData::EGeometryType type = fCrystalData->GetGeometryType();

  //
  //Treatment of r variables 
  //

  int irUp;
  int irLow;
  if (type==MGCrystalData::kCoaxial){
    irUp  = int((coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR) + 1;
    irLow = int((coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR);
    //  since fdelR is a double you will most probably never reach exactly fRMaxArrayIndex with irUp (numerical problems)! correct for it!
    if( (coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR - irLow < 1 &&((coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR) - irLow > 0.9999 ) {
      if(fRMaxGridpoints >= 1000){std::cout<< "Interpolation in r-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
      irUp = irUp + 1;
      irLow = irLow + 1;
    }
  }
  else{
    irUp  = int(coordinates.getRho()/fdelR) + 1;
    irLow = int(coordinates.getRho()/fdelR);
    //  since fdelR is a double you will most probably never reach exactly fRMaxArrayIndex with irUp (numerical problems)! correct for it!
    if( coordinates.getRho()/fdelR - irLow < 1 && coordinates.getRho()/fdelR - irLow > 0.9999 ) {
      if(fRMaxGridpoints >= 1000){std::cout<< "Interpolation in r-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
      irUp = irUp + 1;
      irLow = irLow + 1;
    }
  }

  //
  //Treatment of Phi variables 
  //


  //CLHEP Phi from -Pi to +Pi but need Phi from 0 - 2Pi
  double changePhi = coordinates.getPhi()/CLHEP::rad;
  if(changePhi<0){changePhi=changePhi + 2* M_PI;}

  int iphiUp  = int(changePhi/fdelPhi)+1;
  int iphiLow = int(changePhi/fdelPhi);

  if( (changePhi/fdelPhi) - iphiLow < 1 && (changePhi/fdelPhi) - iphiLow > 0.9999 ) {
    if(fPhiMaxGridpoints >= 1000){std::cout<< "Interpolation in Phi-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
    iphiUp = iphiUp + 1;
    iphiLow = iphiLow + 1;
  }

  //periodic boundaries for phi
  if(iphiLow == fPhiMaxArrayIndex){iphiLow=0;iphiUp = 1;changePhi=0;}

  //
  //Treatment of Z variables 
  //

  int izUp  = int(coordinates.getZ()/fdelZ)+1;
  int izLow = int(coordinates.getZ()/fdelZ);


  if( coordinates.getZ()/fdelZ - izLow < 1 && coordinates.getZ()/fdelZ - izLow > 0.9999 ) {
    if(fZMaxGridpoints >= 1000){std::cout<< "Interpolation in Z-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
    izUp = izUp + 1;
    izLow = izLow + 1;
  }


  //
  //calculate the postion of iXUp and IXLow
  //

  double rLow = irLow * fdelR;
  double rUp  = irUp * fdelR;

  // For a coaxial detector add inner radius as internal area is not considered
  if (type==MGCrystalData::kCoaxial){
    rLow += fCrystalData->GetInnerRadius();
    rUp  += fCrystalData->GetInnerRadius();
  }

  double phiLow = iphiLow * fdelPhi;
  double phiUp  = iphiUp * fdelPhi;

  double zLow   = izLow * fdelZ;
  double zUp    = izUp * fdelZ;


  //   std::cout<<"OuterRadius = "<<fCrystalData->GetOuterRadius() <<std::endl;
  //   std::cout<<"Coordinates = "<<coordinates.getRho() <<std::endl;
  //   std::cout<<"irUp = "<<irUp <<std::endl;
  //   std::cout<<"irLow = "<<irLow <<std::endl;
  //   std::cout<<"fRMaxArrayIndex = "<<fRMaxArrayIndex <<std::endl;
  //   std::cout<<"rUp = "<<rUp <<std::endl;
  //   std::cout<<"rLow = "<<rLow <<std::endl;


  //
  //"Exception handling"
  //

  //if exactly on outer boundary (to avoid crashing because of array size) set iXUp equal to iXLow 
  //do this only after calculation of rLow and rUp to avoid division by zero below
  if(fCrystalData->GetOuterRadius() == coordinates.getRho() ){
    irUp = irLow;
  }
  if(irUp > fRMaxArrayIndex){
    irUp = fRMaxArrayIndex;
    irLow = irUp - 1;
    rLow  = irLow * fdelR;
    rUp   = irUp * fdelR;
    if (type==MGCrystalData::kCoaxial){
      rLow +=fCrystalData->GetInnerRadius();
      rUp  +=fCrystalData->GetInnerRadius();
    }
  }

  //    std::cout<<"CrystalHeight = "<<fCrystalData->GetCrystalHeight() <<std::endl;
  //    std::cout<<"Coordinates = "<<coordinates.getZ() <<std::endl;
  if(fCrystalData->GetCrystalHeight() == coordinates.getZ()){
    izUp = izLow;
  }
  if(izUp > fZMaxArrayIndex){
    izUp = fZMaxArrayIndex;
    izLow = izUp - 1;
    zLow   = izLow * fdelZ;
    zUp    = izUp * fdelZ;
  }

  //
  // Do the interpolation
  //

  for(int i = 0;i<4;i++){
    //for E
    //some tmp var
    double e1;
    double e2;
    double e3;
    double e4;

    // y = mx + n = (yb-ya)/(xb-xa) * (x-xa) + ya     
    e1 = (( fEField[irUp][iphiLow%fPhiMaxArrayIndex][izLow][i] - fEField[irLow][iphiLow%fPhiMaxArrayIndex][izLow][i] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fEField[irLow][iphiLow%fPhiMaxArrayIndex][izLow][i];
    //  std::cout<<"1"<<std::endl;

    e2 = ((fEField[irUp][iphiLow%fPhiMaxArrayIndex][izUp][i] - fEField[irLow][iphiLow%fPhiMaxArrayIndex][izUp][i] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fEField[irLow][iphiLow%fPhiMaxArrayIndex][izUp][i];
    //    std::cout<<"2"<<std::endl;

    e3 = ((fEField[irUp][iphiUp%fPhiMaxArrayIndex][izLow][i] - fEField[irLow][iphiUp%fPhiMaxArrayIndex][izLow][i] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fEField[irLow][iphiUp%fPhiMaxArrayIndex][izLow][i] ;
    //    std::cout<<"3"<<std::endl;

    e4 = ((fEField[irUp][iphiUp%fPhiMaxArrayIndex][izUp][i] - fEField[irLow][iphiUp%fPhiMaxArrayIndex][izUp][i] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fEField[irLow][iphiUp%fPhiMaxArrayIndex][izUp][i];
    //    std::cout<<"4"<<std::endl;


    //more tmp vars:
    double e5;
    double e6;

    //    std::cout<<"5"<<std::endl;

    e5 = ((e3 - e1)/(phiUp - phiLow)) * (changePhi - phiLow) +  e1;
    e6 = ((e4 - e2)/(phiUp - phiLow)) * (changePhi - phiLow) +  e2;

    //    std::cout<<"6"<<std::endl;

    //last step 
    double efieldvalue;
    efieldvalue = ((e6 - e5) / (zUp-zLow)) * (coordinates.getZ() - zLow) + e5;
    //    std::cout<<"7"<<std::endl;


    if(i==0){eR = efieldvalue; }
    if(i==1){ePhi = efieldvalue; }
    if(i==2){eZ = efieldvalue; }
    //    std::cout<<"8"<<std::endl;
  }
  //  std::cout<<"9"<<std::endl;

  // Now convert cylindrical coordinates to cortesian by rotation around z:
  Hep3Vector EVec(eR,ePhi,eZ);
  EVec.rotateZ(changePhi + (fCrystalData->GetSegmentationAngle()/CLHEP::rad));
  //  std::cout<<"10"<<std::endl;
  return EVec;
}
//=============================================================================
Hep3Vector MGSORCrystalFields::InterpolationWField(Hep3Vector coordinates, size_t segment) const
{

  //
  //In here everything has to be done in meter and rad
  //

  double eR=0;  
  double ePhi=0;
  double eZ=0;

  //
  //Treatment of r variables 
  //

  MGCrystalData::EGeometryType type = fCrystalData->GetGeometryType();

  int irUp;
  int irLow;
  if (type==MGCrystalData::kCoaxial){
    irUp  = int((coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR) + 1;
    irLow = int((coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR);
    //  since fdelR is a double you will most probably never reach exactly fRMaxArrayIndex with irUp (numerical problems)! correct for it!
    if( (coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR - irLow < 1 &&((coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR) - irLow > 0.9999 ) {
      if(fRMaxGridpoints >= 1000){std::cout<< "Interpolation in r-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
      irUp = irUp + 1;
      irLow = irLow + 1;
    }
  }
  else{
    irUp  = int(coordinates.getRho()/fdelR) + 1;
    irLow = int(coordinates.getRho()/fdelR);
    //  since fdelR is a double you will most probably never reach exactly fRMaxArrayIndex with irUp (numerical problems)! correct for it!
    if( coordinates.getRho()/fdelR - irLow < 1 && coordinates.getRho()/fdelR - irLow > 0.9999 ) {
      if(fRMaxGridpoints >= 1000){std::cout<< "Interpolation in r-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
      irUp = irUp + 1;
      irLow = irLow + 1;
    }
  }


  //
  //Treatment of Phi variables 
  //

  //CLHEP Phi from -Pi to +Pi but need Phi from 0 - 2Pi
  double changePhi = coordinates.getPhi()/CLHEP::rad;
  if(changePhi<0){changePhi=changePhi + 2* M_PI;}

  int iphiUp  = int(changePhi/fdelPhi)+1;
  int iphiLow = int(changePhi/fdelPhi);

  if( (changePhi/fdelPhi) - iphiLow < 1 && (changePhi/fdelPhi) - iphiLow > 0.9999 ) {
    if(fPhiMaxGridpoints >= 1000){std::cout<< "Interpolation in Phi-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
    iphiUp = iphiUp + 1;
    iphiLow = iphiLow + 1;
  }

  //periodic boundaries for phi
  if(iphiLow == fPhiMaxArrayIndex){iphiLow=0;iphiUp = 1;changePhi=0;}

  //
  //Treatment of Z variables 
  //

  int izUp  = int(coordinates.getZ()/fdelZ)+1;
  int izLow = int(coordinates.getZ()/fdelZ);

  if( coordinates.getZ()/fdelZ - izLow < 1 && coordinates.getZ()/fdelZ - izLow > 0.9999 ) {
    if(fZMaxGridpoints >= 1000){std::cout<< "Interpolation in Z-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
    izUp = izUp + 1;
    izLow = izLow + 1;
  }


  //
  //calculate the postion of iXUp and IXLow
  //

  double rLow = irLow * fdelR;
  double rUp  = irUp * fdelR;

  // For a coaxial detector add inner radius as internal area is not considered
  if (type==MGCrystalData::kCoaxial){
    rLow += fCrystalData->GetInnerRadius();
    rUp  += fCrystalData->GetInnerRadius();
  }

  double phiLow = iphiLow * fdelPhi;
  double phiUp  = iphiUp * fdelPhi;

  double zLow   = izLow * fdelZ;
  double zUp    = izUp * fdelZ;


  //
  //"Exception handling"
  //

  //if exactly on outer boundary (to avoid crashing because of array size) set iXUp equal to iXLow 
  //do this only after calculation of rLow and rUp to avoid division by zero below
  if(fCrystalData->GetOuterRadius() == coordinates.getRho()){
    irUp = irLow;
  }
  if(irUp > fRMaxArrayIndex){
    irUp = fRMaxArrayIndex;
    irLow = irUp - 1;
    rLow  = irLow * fdelR;
    rUp   = irUp * fdelR;
    if (type==MGCrystalData::kCoaxial){
      rLow +=fCrystalData->GetInnerRadius();
      rUp  +=fCrystalData->GetInnerRadius();
    }
  }

  if(fCrystalData->GetCrystalHeight() == coordinates.getZ()){
    izUp = izLow;
  }
  if(izUp > fZMaxArrayIndex){
    izUp = fZMaxArrayIndex;
    izLow = izUp - 1;
    zLow   = izLow * fdelZ;
    zUp    = izUp * fdelZ;
  }


  //
  //Do the interpolation:
  //

  for(int i = 0;i<3;i++){
    //for E
    //some tmp var
    double e1;
    double e2;
    double e3;
    double e4;
    // y = mx + n = (yb-ya)/(xb-xa) * (x-xa) + ya 

    e1 = (( fWField[segment][irUp][iphiLow%fPhiMaxArrayIndex][izLow][i] - fWField[segment][irLow][iphiLow%fPhiMaxArrayIndex][izLow][i] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fWField[segment][irLow][iphiLow%fPhiMaxArrayIndex][izLow][i];


    e2 = ((fWField[segment][irUp][iphiLow%fPhiMaxArrayIndex][izUp][i] - fWField[segment][irLow][iphiLow%fPhiMaxArrayIndex][izUp][i] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fWField[segment][irLow][iphiLow%fPhiMaxArrayIndex][izUp][i];


    e3 = ((fWField[segment][irUp][iphiUp%fPhiMaxArrayIndex][izLow][i] - fWField[segment][irLow][iphiUp%fPhiMaxArrayIndex][izLow][i] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fWField[segment][irLow][iphiUp%fPhiMaxArrayIndex][izLow][i] ;


    e4 = ((fWField[segment][irUp][iphiUp%fPhiMaxArrayIndex][izUp][i] - fWField[segment][irLow][iphiUp%fPhiMaxArrayIndex][izUp][i] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fWField[segment][irLow][iphiUp%fPhiMaxArrayIndex][izUp][i];


    //more tmp vars:
    double e5;
    double e6;

    e5 = ((e3 - e1)/(phiUp - phiLow)) * (changePhi - phiLow) +  e1;
    e6 = ((e4 - e2)/(phiUp - phiLow)) * (changePhi - phiLow) +  e2;

    //last step 
    double efieldvalue;
    efieldvalue = ((e6 - e5) / (zUp-zLow)) * ((coordinates.getZ()/CLHEP::m) - zLow) + e5;

    if(i==0){eR = efieldvalue; }
    if(i==1){ePhi = efieldvalue; }
    if(i==2){eZ = efieldvalue; }
  }

  Hep3Vector EVec(eR,ePhi,eZ);
  EVec.rotateZ(changePhi + (fCrystalData->GetSegmentationAngle()/CLHEP::rad));

  return EVec;
}
//=============================================================================
double MGSORCrystalFields::InterpolationWPotential(CLHEP::Hep3Vector coordinates, size_t segment) const
{  
  //
  //In here everything has to be done in meter and rad
  //


  //
  //Treatment of r variables 
  //

  MGCrystalData::EGeometryType type = fCrystalData->GetGeometryType();
  int irUp;
  int irLow;
  if (type==MGCrystalData::kCoaxial){
    irUp  = int((coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR) + 1;
    irLow = int((coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR);
    //  since fdelR is a double you will most probably never reach exactly fRMaxArrayIndex with irUp (numerical problems)! correct for it!
    if( (coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR - irLow < 1 &&((coordinates.getRho() - fCrystalData->GetInnerRadius())/fdelR) - irLow > 0.9999 ) {
      if(fRMaxGridpoints >= 1000){std::cout<< "Interpolation in r-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
      irUp = irUp + 1;
      irLow = irLow + 1;
    }
  }
  else{
    irUp  = int(coordinates.getRho()/fdelR) + 1;
    irLow = int(coordinates.getRho()/fdelR);
    //  since fdelR is a double you will most probably never reach exactly fRMaxArrayIndex with irUp (numerical problems)! correct for it!
    if( coordinates.getRho()/fdelR - irLow < 1 && coordinates.getRho()/fdelR - irLow > 0.9999 ) {
      if(fRMaxGridpoints >= 1000){std::cout<< "Interpolation in r-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
      irUp = irUp + 1;
      irLow = irLow + 1;
    }
  }

  //
  //Treatment of Phi variables 
  //

  //CLHEP Phi from -Pi to +Pi but need Phi from 0 - 2Pi
  double changePhi = coordinates.getPhi()/CLHEP::rad;
  if(changePhi<0){changePhi=changePhi + 2* M_PI;}

  int iphiUp  = int(changePhi/fdelPhi)+1;
  int iphiLow = int(changePhi/fdelPhi);

  if( (changePhi/fdelPhi) - iphiLow < 1 && (changePhi/fdelPhi) - iphiLow > 0.9999 ) {
    if(fPhiMaxGridpoints >= 1000){std::cout<< "Interpolation in Phi-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
    iphiUp = iphiUp + 1;
    iphiLow = iphiLow + 1;
  }

  //periodic boundaries for phi
  if(iphiLow == fPhiMaxArrayIndex){iphiLow=0;iphiUp = 1;changePhi=0;}


  //
  //Treatment of Z variables 
  //

  int izUp  = int(coordinates.getZ()/fdelZ)+1;
  int izLow = int(coordinates.getZ()/fdelZ);

  if( coordinates.getZ()/fdelZ - izLow < 1 && coordinates.getZ()/fdelZ - izLow > 0.9999 ) {
    if(fZMaxGridpoints >= 1000){std::cout<< "Interpolation in Z-direction might be wrong, since correction criteria does not apply anymore"<<std::endl; }
    izUp = izUp + 1;
    izLow = izLow + 1;
  }


  //
  //calculate the postion of iXUp and IXLow
  //

  double rLow = irLow * fdelR;
  double rUp  = irUp * fdelR;

  // For a coaxial detector add inner radius as internal area is not considered
  if (type==MGCrystalData::kCoaxial){
    rLow += fCrystalData->GetInnerRadius();
    rUp  += fCrystalData->GetInnerRadius();
  }

  double phiLow = iphiLow * fdelPhi;
  double phiUp  = iphiUp * fdelPhi;

  double zLow   = izLow * fdelZ;
  double zUp    = izUp * fdelZ;

  //
  //"Exception handling"
  //

  //if exactly on outer boundary (to avoid crashing because of array size) set iXUp equal to iXLow 
  //do this only after calculation of rLow and rUp to avoid division by zero below
  if(fCrystalData->GetOuterRadius() == coordinates.getRho()){
    irUp = irLow;
  }
  if(irUp > fRMaxArrayIndex){
    irUp = fRMaxArrayIndex;
    irLow = irUp - 1;
    rLow  = irLow * fdelR;
    rUp   = irUp * fdelR;
    if (type==MGCrystalData::kCoaxial){
      rLow +=fCrystalData->GetInnerRadius();
      rUp  +=fCrystalData->GetInnerRadius();
    }
  }


  if(fCrystalData->GetCrystalHeight() == coordinates.getZ()){
    izUp = izLow;
  }
  if(izUp > fZMaxArrayIndex){
    izUp = fZMaxArrayIndex;
    izLow = izUp - 1;
    zLow   = izLow * fdelZ;
    zUp    = izUp * fdelZ;
  }


  //
  //Do the interpolation:
  //

  //for E
  //some tmp var
  double e1;
  double e2;
  double e3;
  double e4;
  // y = mx + n = (yb-ya)/(xb-xa) * (x-xa) + ya 

  e1 = (( fWField[segment][irUp][iphiLow%fPhiMaxArrayIndex][izLow][3] - fWField[segment][irLow][iphiLow%fPhiMaxArrayIndex][izLow][3] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fWField[segment][irLow][iphiLow%fPhiMaxArrayIndex][izLow][3];


  e2 = ((fWField[segment][irUp][iphiLow%fPhiMaxArrayIndex][izUp][3] - fWField[segment][irLow][iphiLow%fPhiMaxArrayIndex][izUp][3] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fWField[segment][irLow][iphiLow%fPhiMaxArrayIndex][izUp][3];


  e3 = ((fWField[segment][irUp][iphiUp%fPhiMaxArrayIndex][izLow][3] - fWField[segment][irLow][iphiUp%fPhiMaxArrayIndex][izLow][3] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fWField[segment][irLow][iphiUp%fPhiMaxArrayIndex][izLow][3] ;


  e4 = ((fWField[segment][irUp][iphiUp%fPhiMaxArrayIndex][izUp][3] - fWField[segment][irLow][iphiUp%fPhiMaxArrayIndex][izUp][3] ) / (double) (rUp-rLow)) * (coordinates.getRho() - rLow) + fWField[segment][irLow][iphiUp%fPhiMaxArrayIndex][izUp][3];



  //more tmp vars:
  double e5;
  double e6;

  e5 = ((e3 - e1)/(phiUp - phiLow)) * (changePhi - phiLow) +  e1;
  e6 = ((e4 - e2)/(phiUp - phiLow)) * (changePhi - phiLow) +  e2;

  //last step 
  double wpotential;
  wpotential = ((e6 - e5) / (zUp-zLow)) * (coordinates.getZ() - zLow) + e5;


  return wpotential;
}
//=============================================================================
void MGSORCrystalFields::AllocateArrays()
{
  //   //allocate the field array
  //   fEField = NULL;

  //   fEField = new double***[fRMaxGridpoints];
  //   for(int i=0;i<=fRMaxArrayIndex;i++){

  //     fEField[i] = new double**[fPhiMaxGridpoints];

  //     for(int j=0;j<=fPhiMaxArrayIndex;j++){

  //       fEField[i][j] = new double*[fZMaxGridpoints];

  //       for(int k=0;k<=fZMaxArrayIndex;k++){

  // 	fEField[i][j][k] = new double[4];

  //       }
  //     }
  //   }

  //   //allocate the weighting field array

  //   fWField = NULL;
  //   fWField = new double****[fCrystalData->GetNSegments()+1];

  //   for(size_t seg=0; seg<fCrystalData->GetNSegments()+1;seg++){

  //     fWField[seg] = new double***[fRMaxGridpoints];
  //     for(int i=0;i<=fRMaxArrayIndex;i++){

  //       fWField[seg][i] = new double**[fPhiMaxGridpoints];

  //       for(int j=0;j<=fPhiMaxArrayIndex;j++){

  // 	fWField[seg][i][j] = new double*[fZMaxGridpoints];

  // 	for(int k=0;k<=fZMaxArrayIndex;k++){

  // 	  fWField[seg][i][j][k] = new double[4];

  // 	}
  //       }
  //     }
  //   }


  //try to make everthing float
  //allocate the field array
  fEField = NULL;

  fEField = new float***[fRMaxGridpoints];
  for(int i=0;i<=fRMaxArrayIndex;i++){

    fEField[i] = new float**[fPhiMaxGridpoints];

    for(int j=0;j<=fPhiMaxArrayIndex;j++){

      fEField[i][j] = new float*[fZMaxGridpoints];

      for(int k=0;k<=fZMaxArrayIndex;k++){

        fEField[i][j][k] = new float[4];

      }
    }
  }

  //allocate the weighting field array

  fWField = NULL; 
  fWField = new float****[fCrystalData->GetNSegments()+1];

  for(size_t seg=0; seg<fCrystalData->GetNSegments()+1;seg++){

    fWField[seg] = new float***[fRMaxGridpoints];
    for(int i=0;i<=fRMaxArrayIndex;i++){

      fWField[seg][i] = new float**[fPhiMaxGridpoints];

      for(int j=0;j<=fPhiMaxArrayIndex;j++){

        fWField[seg][i][j] = new float*[fZMaxGridpoints];

        for(int k=0;k<=fZMaxArrayIndex;k++){

          fWField[seg][i][j][k] = new float[4];

        }
      }
    }
  }

  //set all fields values to 0 for a start
  ResetFields(); 

  //Save them into member variable, such that we can use it in DeAllocate => avoid crash if delete in the wrong order, i.e. first MGCrystalData instance and then instance of MGCrystalFields!
  fSegments = fCrystalData->GetNSegments();
}
//=============================================================================
void MGSORCrystalFields::DeAllocateArrays()
{
  //deallocate fEField array
  for(int i=fRMaxArrayIndex;i>0;i--){
    for(int j=fPhiMaxArrayIndex;j>0; j--){
      for(int k=fZMaxArrayIndex;k>0; k--){
        delete[] fEField[i-1][j-1][k-1];
      }
      delete[] fEField[i-1][j-1];
    }
    delete[] fEField[i-1];
  }
  delete[] fEField;


  //Use member variable for segments, such that we can use it in DeAllocate => avoid crash if delete in the wrong order, i.e. first MGCrystalData instance and then instance of MGCrystalFields!

  //deallocate fWField array
  for(int seg=fSegments+1;seg>0;seg--){
    for(int i=fRMaxArrayIndex;i>0;i--){
      for(int j=fPhiMaxArrayIndex;j>0; j--){
        for(int k=fZMaxArrayIndex;k>0; k--){
          delete[] fWField[seg-1][i-1][j-1][k-1];
        }
        delete[] fWField[seg-1][i-1][j-1];
      }
      delete[] fWField[seg-1][i-1];
    }
    delete[] fWField[seg-1];
  }
  delete[] fWField;

}
//=============================================================================
void MGSORCrystalFields::ReadHeader()
{
  //to read in important information from header of file
  //do not change these values!!!

  char c[200];

  float rmin;
  float rmax;
  float zmax;

  for(int i=0;i<2;i++){


    fDataStream.getline(c,sizeof(c),'\n');
  }
  fDataStream.getline(c,sizeof(c),'\n');
  fDataStream.getline(c,sizeof(c),'\n');

  //read in rmin  
  std::string headerline(c);
  std::string parameters = headerline.substr(headerline.rfind('=')+1);
  //  cout<<"TEST: rmin = "<< parameters<<endl;
  rmin = atof(parameters.c_str());

  //read in rmax  
  headerline.clear();
  parameters.clear();

  fDataStream.getline(c,sizeof(c),'\n');
  headerline.assign(c);
  parameters = headerline.substr(headerline.rfind('=')+1);
  rmax = atof(parameters.c_str());
  //  cout<<"TEST: rmax = "<< rmax<<endl;

  //read in zmax  
  headerline.clear();
  parameters.clear();

  fDataStream.getline(c,sizeof(c),'\n');
  headerline.assign(c);
  parameters = headerline.substr(headerline.rfind('=')+1);
  zmax = atof(parameters.c_str());
  //  cout<<"TEST: zmax = "<< zmax<<endl;

  //cut away applied voltage
  fDataStream.getline(c,sizeof(c),'\n');

  //read in izmax  
  headerline.clear();
  parameters.clear();

  fDataStream.getline(c,sizeof(c),'\n');
  headerline.assign(c);
  parameters = headerline.substr(headerline.rfind('=')+1);
  fZMaxGridpoints = atoi(parameters.c_str());
  fZMaxArrayIndex = fZMaxGridpoints-1;
  //  cout<<"TEST: izmax = "<< izmax<<endl;

  //read in irmax  
  headerline.clear();
  parameters.clear();

  fDataStream.getline(c,sizeof(c),'\n');
  headerline.assign(c);
  parameters = headerline.substr(headerline.rfind('=')+1);
  fRMaxGridpoints = atoi(parameters.c_str());
  fRMaxArrayIndex = fRMaxGridpoints-1;
  //   cout<<"TEST: irmax = "<< fRMaxArrayIndex<<endl;

  //read in iphimax  
  headerline.clear();
  parameters.clear();

  fDataStream.getline(c,sizeof(c),'\n');
  headerline.assign(c);
  parameters = headerline.substr(headerline.rfind('=')+1);
  fPhiMaxGridpoints = atoi(parameters.c_str());
  fPhiMaxArrayIndex = fPhiMaxGridpoints-1;
  //  cout<<"TEST: iphimax = "<< iphimax<<endl;

  //read in delZ  
  headerline.clear();
  parameters.clear();

  fDataStream.getline(c,sizeof(c),'\n');
  headerline.assign(c);
  parameters = headerline.substr(headerline.rfind('=')+1);
  // better calculate it, than just reading in
  //  it is fZMaxGridpoints -1 since this: *-*-*
  // have 3 gridpoints, but devide the whole range only by 2
  fdelZ = zmax / (double) (fZMaxGridpoints-1); 
  //  cout<<"TEST: delZ = "<< fdelZ<<endl;  

  //read in delR
  headerline.clear();
  parameters.clear();

  fDataStream.getline(c,sizeof(c),'\n');
  headerline.assign(c);
  parameters = headerline.substr(headerline.rfind('=')+1);
  //  fdelR = atof(parameters.c_str());
  // better calculate it, than just reading in
  fdelR = (rmax-rmin)/(double) (fRMaxGridpoints-1);
  //   cout<<"TEST: delR = "<< fdelR<<endl;

  //read in delPhi
  headerline.clear();
  parameters.clear();

  fDataStream.getline(c,sizeof(c),'\n');
  headerline.assign(c);
  parameters = headerline.substr(headerline.rfind('=')+1);
  //  fdelPhi = atof(parameters.c_str());
  //better do it like this!!!
  fdelPhi =  2 * M_PI/(double)(fPhiMaxGridpoints-1);
  //  cout<<"TEST: delPhi = "<< fdelPhi<<endl;

  //cut away last 2 lines of header
  fDataStream.getline(c,sizeof(c),'\n');
  fDataStream.getline(c,sizeof(c),'\n');

  //   std::cout<<"fdelR:fdelPhi:fdelZ"<<fdelR<<":"<<fdelPhi<<":"<<fdelZ<<std::endl;
}
//=============================================================================
bool MGSORCrystalFields::OpenFile( const char* name)
{
  if( fDataStream.is_open()){
    cerr<<"MGSORCrystalFields::OpenFile: File "<<name<<" is already opened!"<<std::endl;
    return false;
  }
  else{
    fDataStream.open(name,std::fstream::in | std::fstream::out);
    if(fDataStream.is_open()){
      cout<<"MGSORCrystalFields::OpenFile: File "<<name<<" opened"<<std::endl;
      fDataStream.precision(8);  
    }
    else{
      fDataStream.open(name,std::fstream::out);
      if(fDataStream.is_open()){
        cout<<"MGSORCrystalFields::OpenFile: File "<<name<<" opened"<<std::endl;
        fDataStream.precision(8);
      }
      else{
        cerr<<"MGSORCrystalFields::OpenFile: File "<<name<<" could not be opened!"<<std::endl;
        return false;
      }
    }
  }
  return true;
}
//=============================================================================
bool MGSORCrystalFields::CloseFile()
{
  if(!fDataStream.is_open()){
    cerr<<"MGSORCrystalFields::CloseFile: File is not opened!"<<std::endl;
    return false;
  }
  else{
    fDataStream.close();
    if(!fDataStream.is_open()){cout<<"MGSORCrystalFields::CloseFile: File closed"<<std::endl;}
    else{
      cerr <<"MGSORCrystalFields::CloseFile: File could not be closed"<<std::endl;
      return false;
    }
  }

  return true;
}
//=============================================================================
CLHEP::Hep3Vector MGSORCrystalFields::CoordinateTransform(CLHEP::Hep3Vector coordinates) const
{
  ///////////////////////////////////////////////////////////////////
  //Coordinate Transformation
  // phi and z
  //
  ///////////////////////////////////////////////////////////////////
  //because phi in CLHEP is from -Pi to Pi, we need from 0 to 2* Pi 
  double changePhi = coordinates.getPhi() /CLHEP::rad;
  if(changePhi<0){
    changePhi = changePhi+(2* M_PI);
  }
  //to include a segmentation angle, need to "rotate" back!! 
  changePhi = changePhi - (fCrystalData->GetSegmentationAngle()/CLHEP::rad);

  //if due to rotation of segmentation angle, might end up in neg. phi values again!
  if(changePhi<0){
    changePhi = changePhi+(2* M_PI);
  }

  // because z runs in Crystal coordinate system from -z/2 to +z/2. I simulation it runs from z=0 to z=z
  double changeZ = coordinates.z() + (fCrystalData->GetCrystalHeight() /2.) ;

  ///////////////////////////////////////////////////////////////////
  //Coordinate Transformation
  // phi and z
  //END
  ///////////////////////////////////////////////////////////////////

  CLHEP::Hep3Vector newCoordinates;
  newCoordinates.setRhoPhiZ(coordinates.getRho(),changePhi,changeZ);
  return newCoordinates;

}
