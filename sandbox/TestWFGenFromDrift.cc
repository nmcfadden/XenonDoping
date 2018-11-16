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
 * $Id: TestWFGenFromDrift.cc,v 1.2 2008-11-24 08:57:36 jliu Exp $
 *      
 * CLASS DECLARATION:  MJAnalysisWF.cc
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 * 	test MGWFGenFromDrift
 * AUTHOR: 
 * 	Jing Liu
 * REVISION:
 * 	10-10-2008, Created, Jing.
 * 	24-11-2008, minor changes, Jing.
 *
 */
#include <math.h>

#include "io/MGLogger.hh"
#include "MGCrystalHits.hh"

#include "waveform/MGWaveformManager.hh"
#include "waveform/MGBruyneelDriftVelocity.hh"
#include "waveform/MGWFGenFromDrift.hh"
#include "waveform/MGSORCrystalFields.hh"

#include "MGCrystalData.hh"
#include "MGMultiWaveformTransformer.hh"
#include "MGWFAddNoise.hh"
#include "MGWFRCDifferentiation.hh"
#include "MGWFRCIntegration.hh"

using namespace std;
using namespace CLHEP;

int main()
{
   MGLogger::SetSeverity(MGLogger::routine);
   //MGLogger::SetSeverity(MGLogger::debugging);

   cout<<"\n----- Here comes the Waveform part -----\n"<<endl;
   
   cout<<"Setup Waveform Manager"<<endl;
   MGWaveformManager wfM;
   wfM.SetCoincidenceTime(1*ns);
   wfM.SetCoincidenceLength(1*mm);
   cout<<"Setup Waveform Manager - DONE\n"<<endl;

   cout<<"Setup Crystal Data"<<endl;
   int id = 0;
   double ri = 5.0*mm, ro = 37.5*mm, hi = 70.0*mm;
   MGCrystalData* crystal = new MGCrystalData(
	 id, 		// crystal ID
	 MGCrystalData::kNType,
	 MGCrystalData::kCoaxial,
	 ri,  		// inner radius
	 ro, 		// outer radius
	 hi, 		// height
	 hi, 		// well depth
	 0.0, 		// global x
	 0.0, 		// global y
	 0.0, 		// global z
	 3,   		// Nseg in z
	 6);  		// Nseg in phi
   //crystal->SetCrystalAxesAngle(35.0*CLHEP::degree);// for susie
   //crystal->SetSegmentationAngle(25.0*CLHEP::degree);// for susie
   crystal->SetCrystalAxesAngle(0.0*CLHEP::degree);
   crystal->SetSegmentationAngle(0.0*CLHEP::degree);
   cout<<"Setup Crystal Data - DONE\n"<<endl;

   cout<<"Setup Crystal Fields"<<endl;
   MGSORCrystalFields* fields = new MGSORCrystalFields(*crystal);
   fields->SetFileName("/remote/gx336-01/dlenz/MediumRootFile");
   fields->LoadFields();
   cout<<"Setup Crystal Fields - DONE\n"<<endl;

   cout<<"Setup Drift Velocity"<<endl;
   MGBruyneelDriftVelocity drift(*fields);
   cout<<"Setup Drift Velocity - DONE\n"<<endl;

   cout<<"Setup Waveform Generator"<<endl;
   MGWFGenFromDrift* wfGen = new MGWFGenFromDrift(drift,7000,10*CLHEP::GHz,10*ns,2.95*eV, MGWaveform::kCharge);
   wfGen->SetPrecision(2);
   cout<<"Setup Waveform Generator - DONE\n"<<endl;
   
   cout<<"Adding Waveform Generator to Manager"<<endl;
   wfM.AddWaveformGenerator(wfGen);
   cout<<"Adding Waveform Generator to Manager - DONE\n"<<endl;

   cout<<"Adding Transformations to Manager"<<endl;
//   MGWFAddNoise addNoise;
//   addNoise.SetNoiseAmplitude(600); // 5 keV / 2.95 eV (pair production energy)
//   MGWFRCDifferentiation decay;
//   decay.SetTimeConstant(5000*ns);
//   MGWFRCIntegration bandwidth;
//   bandwidth.SetTimeConstant(20*ns);
//   MGMultiWaveformTransformer trans;
//   trans.AddWFTransformer(&decay);
//   trans.AddWFTransformer(&bandwidth);
//   trans.AddWFTransformer(&addNoise);
   //wfM.SetElectronicsModel(&trans);
   cout<<"Adding Transformations to Manager - DONE\n"<<endl;

   cout<<"Setup Crystal Hits"<<endl;
   MGCrystalHits CHits;
   cout<<"Setup Crystal Hits - DONE\n"<<endl;

   cout<<"Generate WFs event by event:"<<endl;

   int Nhits = 37;
   cout<<"Total event: "<<Nhits<<endl;
   float dphi=3.14159265/(Nhits-1);
   //for (int i=7; i<8; i++) {
   for (int i=0; i<Nhits; i++) {
      cout<<"\t Processing event "<<i<<endl; 
      CHits.Reset();
      float phi=dphi*i;
      float x=(ro-0.1*mm)*cos(phi);
      float y=(ro-0.1*mm)*sin(phi);
      cout<<"hit x, y:                    "<<x<<" \t "<<y<<endl;
      CHits.AddCrystalHit(x,y,0*mm,0*ns,122*CLHEP::keV,id);
      //CHits.Print();
      wfM.GenerateWaveforms(&CHits);    
   }

//   CHits.Print();
//   cout<<"Processing event 0"<<endl; 
//   CHits.Reset();
//   CHits.AddCrystalHit(0*cm,0.5*cm,0*cm,0*ns,1000*CLHEP::keV,id);
//   CHits.Print();
//   wfM.GenerateWaveforms(&CHits);    
//   CHits.Print();

//   cout<<"Processing event 1"<<endl; 
//   CHits.Reset();
//   CHits.AddCrystalHit(0*cm,3.75*cm,0*cm,0*ns,1000*CLHEP::keV,id);
//   CHits.Print();
//   wfM.GenerateWaveforms(&CHits);    
//   CHits.Print();
//
//   cout<<"Processing event 2"<<endl; 
//   CHits.Reset();
//   CHits.AddCrystalHit(0*cm,0.5*cm,0*CLHEP::cm,0*ns,1000*CLHEP::keV,id);
//   CHits.AddCrystalHit(0*cm,3.75*cm,0*CLHEP::cm,0*ns,1000*CLHEP::keV,id);
//   CHits.Print();
//   wfM.GenerateWaveforms(&CHits);    
// 
//   cout<<"Processing event 3"<<endl; 
//   CHits.Reset();
//   CHits.AddCrystalHit(0*cm,2*cm,0*CLHEP::cm,0*ns,1000*CLHEP::keV,id);
//   CHits.Print();
//   wfM.GenerateWaveforms(&CHits);    
//
//   cout<<"Processing event 4"<<endl; 
//   CHits.Reset();
//   CHits.AddCrystalHit(0*cm,2*cm,0*CLHEP::cm,0*ns,1000*CLHEP::keV,id);
//   CHits.AddCrystalHit(0*cm,2*cm,0*CLHEP::cm,100*ns,1000*CLHEP::keV,id);
//   CHits.Print();
//   wfM.GenerateWaveforms(&CHits);    
//
//   cout<<"Processing event 5"<<endl; 
//   CHits.Reset();
//   CHits.AddCrystalHit(0*CLHEP::cm,0.5*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,1000*CLHEP::keV,id);
//   CHits.AddCrystalHit(0*CLHEP::cm,3.75*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,5000*CLHEP::keV,id);
//   CHits.Print();
//   wfM.GenerateWaveforms(&CHits);    
//
//   cout<<"Processing event 6"<<endl; 
//   CHits.Reset();
//   CHits.AddCrystalHit(0*CLHEP::cm,1.8*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,1000*CLHEP::keV,id);
//   CHits.AddCrystalHit(0*CLHEP::cm,2.2*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,1000*CLHEP::keV,id);
//   CHits.Print();
//   wfM.GenerateWaveforms(&CHits);    
//
//   cout<<"Processing event 7"<<endl; 
//   CHits.Reset();
//   CHits.AddCrystalHit(0*CLHEP::cm,1.6*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,1000*CLHEP::keV,id);
//   CHits.AddCrystalHit(0*CLHEP::cm,2.4*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,1000*CLHEP::keV,id);
//   CHits.Print();
//   wfM.GenerateWaveforms(&CHits);    
//
//   cout<<"Processing event 8"<<endl; 
//   CHits.Reset();
//   CHits.AddCrystalHit(0*CLHEP::cm,1.4*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,1000*CLHEP::keV,id);
//   CHits.AddCrystalHit(0*CLHEP::cm,2.6*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,1000*CLHEP::keV,id);
//   CHits.Print();
//   wfM.GenerateWaveforms(&CHits);    
//
//   cout<<"Processing event 9"<<endl; 
//   CHits.Reset();
//   CHits.AddCrystalHit(0*CLHEP::cm,1.2*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,1000*CLHEP::keV,id);
//   CHits.AddCrystalHit(0*CLHEP::cm,2.8*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,1000*CLHEP::keV,id);
//   CHits.Print();
//   wfM.GenerateWaveforms(&CHits);    
//
//   cout<<"Processing event 10"<<endl; 
//   CHits.Reset();
//   CHits.AddCrystalHit(0*CLHEP::cm,3.5*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,1000*CLHEP::keV,id);
//   CHits.AddCrystalHit(0*CLHEP::cm,2.5*CLHEP::cm,0*CLHEP::cm,0*CLHEP::ns,1000*CLHEP::keV,id);
//   CHits.Print();
//   wfM.GenerateWaveforms(&CHits);    

   // prepare some random hits
//   CHits.Reset();
//   TF1 *f1 = new TF1("f1","x",ri,ro);
//   TRandom3 p(0), z(0);
//   for (size_t i=0; i<10000; i++) {
//      CHits.AddCrystalHit(
//	    f1->GetRandom()*cos(p.Uniform(-3.1415926*CLHEP::rad,3.1415926*CLHEP::rad)),
//	    f1->GetRandom()*sin(p.Uniform(-180.*CLHEP::deg,180.*aCLHEP::rad)),  
//	    z.Uniform(-hi/6.,hi/6.));
//   }
//   wfM.GenerateWaveforms(&CHits);    

   cout<<"Generate WFs event by event - DONE"<<endl;
   
   wfM.EndOfRunAction();
}

