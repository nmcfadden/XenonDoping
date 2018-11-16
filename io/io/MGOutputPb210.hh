//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
/**                                                            
*      
* CLASS DECLARATION:  MGOutputPb210.hh
*
*---------------------------------------------------------------------------//
*
* DESCRIPTION: 
*
* Root output class for gammas emitted from a lead shield.  Makes and saves
* histograms of the energy distribution and angular distribution of photons
* leaving the shield.  Angular distribution is dependent on energy, so a
* two dimensional histogram is constructed, as well as one dimensional
* slices at two energy values, E1 and E2.
*
* AUTHOR: austinst
* CONTACT: paddy@physics.unc.edu
* FIRST SUBMISSION: 5/17/09 
* 
* REVISION:
*
* 
*
*/
// ---------------------------------------------------------------------------//

#ifndef _MGOUTPUTPB210_HH
#define _MGOUTPUTPB210_HH

#include "TObject.h"
#include "G4ThreeVector.hh"

#include "globals.hh"
#include "io/MGOutputRoot.hh"
#include "generators/MGGeneratorPb210.hh"
#include "G4Navigator.hh"

#include "TH2D.h"
#include "TH1D.h"

// ---------------------------------------------------------------------------//


class MGOutputPb210: public MGOutputRoot
{
	public:
	//constructor
	MGOutputPb210(G4bool isMother);

	//copy constructor
	//MGOutputPb210(const MGOutputPb210 &);

	//destructor
	~MGOutputPb210();

	//inherited virtual functions
	void DefineSchema();
	void BeginOfRunAction();
	void BeginOfEventAction(const G4Event *event);
	void EndOfRunAction();
	void WriteFile();

	void TakeAngleHistoSlice();	
	void CalculateEnergy(const G4Event* event);
	void CalculateZenithAngle(const G4Event* event);
	G4ThreeVector FindZenithAxis();

	//unused, but must override pure virtual function
	void RootSteppingAction(const G4Step*) {;}

	private:	

	//histograms 
	TH1D* fEnergyHisto;
	TH2D* fEnergyAngleHisto;
	TH1D* fAngleHistoAtE1;
	TH1D* fAngleHistoAtE2;

	//histogram parameters
	static const Double_t fEnergyLow;
	static const Double_t fEnergyHigh;
	static const Int_t fEnergyBins = 1121;
	static const Int_t fAngleBins = 1000;

	//energies where Angular distribution is recorded (keV)
	static const Double_t fE1;
	static const Double_t fE2;

	// Energy width in bins of slice taken by TakeAngleHistoSlice 
	static const Int_t fSliceWidth = 4;

	//values to be put in histograms
	Double_t	fEnergy;
	Double_t	fZenithAngle; 

	//particle momentum & position
	G4ThreeVector fMomentum;
	G4ThreeVector fInitialPosition; 

	//pointer to generator
	MGGeneratorPb210* fTheGenerator;

	//navigator 
	G4Navigator* gNavigator;

	protected:

};


#endif

