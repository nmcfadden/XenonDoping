//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
// $Id: MJcodeTemplate.cc,v 1.3 2005-02-15 23:26:55 akbarm Exp $
//
// CLASS IMPLEMENTATION:  MGLGNDOpticalMaterialProperties.cc
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
 * AUTHOR: Neil McFadden
 * CONTACT: nmcfadde@unm.edu
 * FIRST SUBMISSION: 
 *
 * REVISION:
 * 
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#include "materials/MGLGNDOpticalMaterialProperties.hh"
#include "io/MGLogger.hh"

#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"

#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

// G4 headers
#include "G4SDManager.hh"

// ROOT headers
#include "TGraph.h"

using namespace CLHEP;
const G4double MGLGNDOpticalMaterialProperties::LambdaE = twopi *1.973269602e-16 * m * GeV;
const G4int    MGLGNDOpticalMaterialProperties::NUMENTRIES = 69;
const G4int    MGLGNDOpticalMaterialProperties::NUMENTRIES_1 = 5;
const G4int    MGLGNDOpticalMaterialProperties::NUMENTRIES_2 = 500;
const G4int    MGLGNDOpticalMaterialProperties::NUMENTRIES_3 = 29;


MGLGNDOpticalMaterialProperties::MGLGNDOpticalMaterialProperties()
  :fSuccessfulInitialization(false)
{
  
}

MGLGNDOpticalMaterialProperties::~MGLGNDOpticalMaterialProperties()
{}

void MGLGNDOpticalMaterialProperties::ConstructionOpticalProperties()
{
  ph_energies = new G4double[NUMENTRIES_2];
  //Energy Array used for all materials from 115 nm to 650 nm with 500 points (1.07 nm/bin)
  for (int i = 0; i < NUMENTRIES_2; i++) {
    ph_energies[i] = LambdaE/(650*nm) + i*((LambdaE/(115*nm) - (LambdaE/(650*nm)))/(NUMENTRIES_2-1));
  }
  RegisterArgonOpticalProperties();
  Register_TPB_Properties();
  Register_Fiber_Properties();
  Register_Fiber_Cladding_Properties();
  Register_Nylon_Properties();
  Register_Copper_Properties();
  Register_Germanium_Properties();
  Register_Silicon_Properties();
  Register_Teflon_Properties();
  Register_Silica_Properties();
  //Register_VM2000();
  Register_StainlessSteel();
  Register_Acrylic();

  MGLog(routine) <<"Constructed LGND Optical Material Properties"<<endlog;
}

/// Methods imported from the MPIKLarGe class
/// Optical properties of LAr in several places
void MGLGNDOpticalMaterialProperties::RegisterArgonOpticalProperties()
{
	  static const G4int NUMENTRIES = 69;
	  const G4int num = 69;
	  static const G4double temp = 88.5*kelvin;
	  static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;

	  /**
	   * Nominal values for pure argon
	   */
	  G4double scint_yield = 23.6*eV;  // Nominal energy to produce a photon (measured)
	  G4double photon_yield = 1.0*MeV/scint_yield;
	  G4double tau_s = 6.0*ns;
	  G4double tau_l = 1590.0*ns;
	  /*G4double yield_ratio = 0.23; // For gammas and electrons*/


	  // New value based on the triplet lifetime from Mark Heisel
	  // Redefine the values to res-scale according to Mark's calculation
	  // TODO - what is the correct yield value?
    // G4double LAr_LY_scale = fDetectorDB->GetLArInstArgonLYScale();
	  // photon_yield = 28120. * LAr_LY_scale;
  
	  tau_s = 5.95*ns;
	  tau_l = 922*ns;

	  /*
    ** TODO - 
    G4double LAr_att_scale = fDetectorDB->GetLArInstArgonAbsLScale();
    if (LAr_att_scale != 1.0) {
      MGLog(routine) << "Scaling XUV argon attenuation length by a factor of " << LAr_att_scale << endlog;
    }
    */

	  MGLog(routine) << "LAr Optical parameters: " << endlog;
	  MGLog(routine) << "     Scintillation yield : " << photon_yield << " ph/MeV" << endlog;
	  MGLog(routine) << "     Singlet lifetime : " << tau_s/ns << " ns" << endlog;
	  MGLog(routine) << "     Triplet lifetime : " << tau_l/ns << " ns" << endlog;

	  G4int ji;
	  G4double e;
	  G4double ee;

	  G4double PPCKOVHighE = LambdaE / (115*nanometer);
	  G4double PPCKOVLowE = LambdaE / (650*nanometer);
	  G4double de = ((PPCKOVHighE - PPCKOVLowE) / ((G4double)(NUMENTRIES-1)));

	  // liquid argon (LAr)
	  G4double LAr_PPCK[(NUMENTRIES)];
	  G4double LAr_RIND[(NUMENTRIES)];
	  G4double LAr_RAYL[(NUMENTRIES)];
	  G4double LAr_ABSL[(NUMENTRIES)];

	  G4double LAr_ABSL_xuv = 60*cm;
	  //G4double LAr_ABSL_xuv = 110*cm;
	  G4double LAr_ABSL_vis = 1000*m;
	  //TODO
    //LAr_ABSL_xuv *= LAr_att_scale;

	  MGLog(debugging)  << "Rayleigh scattering lenght [m]:" << endlog;
	  for (ji = 0; ji < NUMENTRIES; ji++){
	      e = PPCKOVLowE + ((G4double)ji) * de;
	      LAr_PPCK[ji] = e;
	      LAr_RIND[ji] = LArRefIndex((LambdaE / e));
	      LAr_RAYL[ji] = LArRayLength((LambdaE / e), temp);
	      MGLog(debugging) << (LambdaE/LAr_PPCK[ji])/nm <<", "<< LAr_RAYL[ji] << endlog;
	      /* Uncomment for debugging purposes
	      MGLog(debugging) << " WL: " << (LambdaE/LAr_PPCK[ji])/nm<< " nm Energy: " << LAr_PPCK[ji]/eV << " eV; Refr: " <<
		  LAr_RIND[ji] << " ; Rayleigh l. " << LAr_RAYL[ji]/m << " m" << endlog;
	       */

	      if (((LambdaE / e)/nm) < 200.0) {
	    	  LAr_ABSL[ji] =LAr_ABSL_xuv;
	      } 
        else {
	    	  LAr_ABSL[ji] = LAr_ABSL_vis;
	      }

	  }
    MGLog(debugging) << "XUV attenuation length: " << LAr_ABSL_xuv/cm << " cm" << endlog;
    MGLog(debugging) << "VIS attenuation length: " << LAr_ABSL_vis/m << " m" << endlog;

	  G4double PPSCHighE = LambdaE /(115*nanometer);
	  G4double PPSCLowE = LambdaE /(136*nanometer);
	  G4double dee = ((PPSCHighE - PPSCLowE) / ((G4double)(num-1)));
	  G4double LAr_SCIN[num];
	  G4double LAr_SCPP[num];
	  for (ji = 0; ji < num; ji++){
	      ee=PPSCLowE+ ((G4double)ji) * dee;
	      LAr_SCPP[ji]=ee;
	      LAr_SCIN[ji]=ArScintillationSpectrum((LambdaE/ee)/nanometer);
	      /** Keep for debugging purposes
	      MGLog(debugging) << " WL: " << (LambdaE/LAr_SCPP[ji])/nanometer<< " nm Scint: " << LAr_SCPP[ji]/eV << " eV :: " << LAr_SCIN[ji] << endlog;

	      MGLog(routine) << " WL1: " << (LambdaE/ee)/nanometer << " WL: " << (LambdaE/LAr_PPCK[ji])/nm<< " En: " << LAr_PPCK[ji]/eV << " ;n: " <<
		  LAr_RIND[ji] << " ; Rayleigh  " << LAr_RAYL[ji]/m << " m; Scint " << LAr_SCIN[ji] << endlog;
	       */
	  }

	  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

	  myMPT1->AddProperty("RINDEX",        LAr_PPCK, LAr_RIND, NUMENTRIES);
	  myMPT1->AddProperty("RAYLEIGH",      LAr_PPCK, LAr_RAYL, NUMENTRIES);
	  myMPT1->AddProperty("ABSLENGTH",     LAr_PPCK, LAr_ABSL, NUMENTRIES);

	  // Fast and slow components of the scintillation
	  // They should both be the same
	  if ( (LAr_SCPP[0] >= PPCKOVLowE) &&
	       (LAr_SCPP[(sizeof(LAr_SCPP)/sizeof(G4double) - 1)] <= PPCKOVHighE) )
	    {
	      myMPT1->AddProperty("FASTCOMPONENT",LAr_SCPP,LAr_SCIN,num);
	      myMPT1->AddProperty("SLOWCOMPONENT",LAr_SCPP,LAr_SCIN,num);
	    }
	  myMPT1->AddConstProperty("SCINTILLATIONYIELD",photon_yield);
	  myMPT1->AddConstProperty("FASTTIMECONSTANT", tau_s);
	  myMPT1->AddConstProperty("SLOWTIMECONSTANT",tau_l);
	  // This is the value for electrons and gammas
	  // For example, for nuclear recoils it should be 0.75
	  // nominal value for electrons and gamas: 0.23
	  // Value used was provided by F. Art
	  myMPT1->AddConstProperty("YIELDRATIO",0.3);

	  // Scintillation yield
	  // WArP data:
	  /*
	   * ScintillationYield (mean energy to produce a UV photon)
	   * dependent on the nature of the impinging particles
	   *
	   * for flat top response particles the mean energy to produce a photon:  is 19.5 eV
	   *  Y =  1/19.5 eV
	   *
	   * ScintillationYield dependent also on:
	   *  - Field configuration
	   *  - Quencher impurities
	   *
	   * @ zero E field:
	   *  Y_e = 0.8 Y
	   *  Y_alpha = 0.7 Y
	   *  Y_recoils = 0.2-0.4
	   *
	   *  These scales should be added to the physics list
	   *
	   *
	   * G4double scint_yield=1.0/(19.5*eV);
	   * myMPT1->AddConstProperty("SCINTILLATIONYIELD",scint_yield);
	   */




	  /**
	   * statistical yield fluctuation can be broadened or narrower
	   * (impurities, fano factor):
	   *
	   * LAr Fano factor = 0.11 ( Doke et al, NIM 134 (1976)353 )
	   *
	   *
	   */

	  G4double fano = 0.11;// Doke et al, NIM 134 (1976)353
	  myMPT1->AddConstProperty("RESOLUTIONSCALE",fano);
    fArgonLiquid = G4Material::GetMaterial("Argon-Liq");
	  fArgonLiquid->SetMaterialPropertiesTable(myMPT1);

	  fArgonLiquid->GetIonisation()->SetBirksConstant(5.1748e-4*cm/MeV);

    //What is the difference between gaseous argon and liquid argon?
    //density,triplet,IndexOfRefraction...what else?
    //density,triplet,IndexOfRefraction,Rayleigh,abslength...what else?
    G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
    for (ji = 0; ji < NUMENTRIES; ji++){
      e = PPCKOVLowE + ((G4double)ji) * de;
      //according to 
      //https://refractiveindex.info/?shelf=main&book=Ar&page=Bideau-Mehu
      //Argon gas does not really change index of refraction much 1.0004 @ 140 nm and 1.00028 @ 500 nm
      LAr_RIND[ji] = 1.00034;
      //This is just a guess, scale by the ratio of the densities
      LAr_RAYL[ji] = 781.95*LAr_RAYL[ji];
      if (((LambdaE / e)/nm) < 200.0) {
        LAr_ABSL[ji] = 781.95*LAr_ABSL_xuv;
      }
      else {
        LAr_ABSL[ji] = 781.95*LAr_ABSL_vis;
      }
      G4cout<<"Ray "<<LAr_RAYL[ji]/cm<<", Atten "<<LAr_ABSL[ji]/cm<<", wavelength "<< (LambdaE / e)/nm<<G4endl;
    }
    myMPT2->AddProperty("RINDEX",        LAr_PPCK, LAr_RIND, NUMENTRIES);
    myMPT2->AddProperty("RAYLEIGH",      LAr_PPCK, LAr_RAYL, NUMENTRIES);
    myMPT2->AddProperty("ABSLENGTH",     LAr_PPCK, LAr_ABSL, NUMENTRIES);
    myMPT2->AddConstProperty("FASTTIMECONSTANT", tau_s);
    myMPT2->AddConstProperty("SLOWTIMECONSTANT",2880*ns);
    //What is the photon yeild for GAr? assuming same as LAr
    myMPT2->AddConstProperty("SCINTILLATIONYIELD",photon_yield);
    G4Material::GetMaterial("Argon")->SetMaterialPropertiesTable(myMPT2);
}

G4double MGLGNDOpticalMaterialProperties::LArRefIndex(const G4double lambda)
{
  return ( sqrt(LArEpsilon(lambda)) ); // square root of dielectric constant
}

// Calculates the dielectric constant of LAr from the Bideau-Sellmeier formula.
// See : A. Bideau-Mehu et al., "Measurement of refractive indices of Ne, Ar,
// Kr and Xe ...", J. Quant. Spectrosc. Radiat. Transfer, Vol. 25 (1981), 395

G4double MGLGNDOpticalMaterialProperties::LArEpsilon(const G4double lambda)
{
  G4double epsilon;
  if (lambda < 110*nanometer) return 1.0e4; // lambda MUST be > 110.0 nm
  epsilon = lambda / micrometer; // switch to micrometers
  epsilon = 1.0 / (epsilon * epsilon); // 1 / (lambda)^2
  epsilon = 1.2055e-2 * ( 0.2075 / (91.012 - epsilon) +
                          0.0415 / (87.892 - epsilon) +
                          4.3330 / (214.02 - epsilon) );
  epsilon *= (8./12.); // Bideau-Sellmeier -> Clausius-Mossotti
  G4double LArRho = 1.396*g/cm3;
  G4double GArRho = 1.66e-03*g/cm3;
  epsilon *= (LArRho / GArRho); // density correction (Ar gas -> LAr liquid)
  if (epsilon < 0.0 || epsilon > 0.999999) return 4.0e6;
  epsilon = (1.0 + 2.0 * epsilon) / (1.0 - epsilon); // solve Clausius-Mossotti
  return epsilon;
}

//-------------------------------------------------------------------------><
// Calculates the Rayleigh scattering length using equations given in
// G. M. Seidel at al., "Rayleigh scattering in rare-gas liquids",
// arXiv:hep-ex/0111054 v2 22 Apr 2002

G4double MGLGNDOpticalMaterialProperties::LArRayLength(const G4double lambda,const
				   G4double temp)
{
  G4double dyne = 1.0e-5*newton;
  static const G4double LArKT = 2.18e-10 * cm2/dyne; // LAr isothermal compressibility
  static const G4double k = 1.380658e-23 * joule/kelvin; // the Boltzmann constant
  G4double h;
  h = LArEpsilon(lambda);
  if (h < 1.00000001) h = 1.00000001; // just a precaution
  h = (h - 1.0) * (h + 2.0); // the "dielectric constant" dependance
  h *= h; // take the square
  h *= LArKT * temp * k; // compressibility * temp * Boltzmann constant
  h /= lambda * lambda * lambda * lambda; // (lambda)^4
  h *= 9.18704494231105429; // (2 * Pi / 3)^3
  if ( h < (1.0 / (10.0 * km)) ) h = 1.0 / (10.0 * km); // just a precaution
  if ( h > (1.0 / (0.1 * nanometer)) ) h = 1.0 / (0.1 * nanometer); // just a precaution
  return ( 1.0 / h );}

G4double MGLGNDOpticalMaterialProperties::ArScintillationSpectrum(const G4double kk)
{
  G4double waveL;
  waveL =exp(-0.5*((kk-128.0)/(2.929))*((kk-128.0)/(2.929)));
  return waveL;
}

void MGLGNDOpticalMaterialProperties::Register_TPB_Properties()
{
  //taken from MGGerdaLocalMaterialTable.cc in the gerda-optical branch because they do it so much better than me (N. McFadden)
  //Also, I am guessing most of the comments are from Luigi Pertoldi
  //Adding TPB emission properties for different materials and generic TPB

  G4NistManager*   nist = G4NistManager::Instance();
  G4Element* elH = nist->FindOrBuildElement("H");
  G4Element* elC = nist->FindOrBuildElement("C");
  G4Element* elF = nist->FindOrBuildElement("F");
  
  // TPB
  G4Material* TPB = new G4Material("TPB", 1.08*g/cm3, 2, kStateSolid);
              TPB -> AddElement(elH, 22);
              TPB -> AddElement(elC, 28);

  // TPB on Nylon
  G4Material* TPBOnNylon = new G4Material("TPBOnNylon", 1.08*g/cm3, 2, kStateSolid);
              TPBOnNylon -> AddElement(elH, 22);
              TPBOnNylon -> AddElement(elC, 28);

  // TPB on VM2000
  G4Material* TPBOnVM2000 = new G4Material("TPBOnVM2000", 1.08*g/cm3, 2, kStateSolid);
              TPBOnVM2000 -> AddElement(elH, 22);
              TPBOnVM2000 -> AddElement(elC, 28);

  // TPB on Tetratex
  G4Material* TPBOnTetratex = new G4Material("TPBOnTetratex", 1.08*g/cm3, 2, kStateSolid);
              TPBOnTetratex -> AddElement(elH, 22);
              TPBOnTetratex -> AddElement(elC, 28);
  ////////////
  // VM2000 //
  ////////////

  //VM2000 will NOT be used in LEGEND because Tetratex is more radio pure, it is added here just to have it...
  /** Reflectivity taken from https://www.osti.gov/servlets/purl/1184400
   * "Reflectivity spectra for commonly used reflectors" by M. Janacek
   *
   * Seems to be a well-done measurement, done using an integrating sphere and taking
   * into account the (even little) fluorescence. He uses a 65um thick foil without the
   * glue on the back. The results he shows are already normalized by the reflectivity
   * of the reference PTFE sample (-> absolute reflectivity!)
   *
   * EDIT: I found this paper: arXiv:1304.6117 in which people measure the reflectivity
   *       of VM2000 with TPB evaporated on it, and it's a bit different. The measurement
   *       seems to be done properly as they take into account the effect of TPB's emission
   *       spectrum. The measurement seems to be pretty independent on the TPB layer thickness
   *
   *       The old reflectivity values can be found in 'Reflectivity_VM2000.dat'
   *  -Luigi Pertoldi
   */

    //auto VM2000ReflGraph = std::unique_ptr<TGraph>(ReadSpectrumFromFile("Reflectivity_VM2000.dat"));
  auto VM2000ReflGraph = std::unique_ptr<TGraph>(ReadSpectrumFromFile("Reflectivity_TPBCoatedVM2000.dat"));
  G4double refl_VM2000[NUMENTRIES_2];

  for (int i = 0; i < NUMENTRIES_2; i++) {
    auto r = VM2000ReflGraph->Eval(LambdaE/(ph_energies[i])/nm);
    refl_VM2000[i] = r >= 0 ? r : 0;
  }

  G4MaterialPropertiesTable* VM2000OpTable = new G4MaterialPropertiesTable();
  VM2000OpTable->AddProperty("REFLECTIVITY", ph_energies, refl_VM2000, NUMENTRIES_2);
  G4Material::GetMaterial("VM2000")->SetMaterialPropertiesTable(VM2000OpTable);

  //////////////
  // Tetratex //
  //////////////

  /** Reflectivity taken from https://www.osti.gov/servlets/purl/1184400
   * "Reflectivity spectra for commonly used reflectors" by M. Janacek
   *
   * He measures the reflectivity of 2 and 4 superimposed layers of 160um thick
   * Tetratex. As our layer in GERDA is 254um thick I'm taking here his results
   * for the two superimposed foils (= 320um). So, in reality, the reflectivity
   * of our foil should be (negligibly) smaller.
   *
   * In addition to this, the TPB layer has some effect on the reflectivity,
   * just like VM2000 above.
   * -Luigi Pertoldi
   */

  // Tetratex (PTFE)
  // density from data sheet found in Zurich
  G4Material* Tetratex = new G4Material("Tetratex", 0.35*g/cm3, 2);
              Tetratex -> AddElement(elF, 0.76);
              Tetratex -> AddElement(elC, 0.24);

  auto TetratexReflGraph = std::unique_ptr<TGraph>(ReadSpectrumFromFile("Reflectivity_Tetratex.dat"));
  G4double refl_Tetratex[NUMENTRIES_2];

  for (int i = 0; i < NUMENTRIES_2; i++) {
    auto r = TetratexReflGraph->Eval(LambdaE/(ph_energies[i])/nm);
    refl_Tetratex[i] = r >= 0 ? r : 0;
  }

  G4MaterialPropertiesTable* TetratexOpTable = new G4MaterialPropertiesTable();
  TetratexOpTable->AddProperty("REFLECTIVITY", ph_energies, refl_Tetratex, NUMENTRIES_2);
  G4Material::GetMaterial("Tetratex")->SetMaterialPropertiesTable(TetratexOpTable);

  /////////
  // TPB //
  /////////

  /** Not actually to be used, please prefer TPBOnSomething materials
   *
   * - Emission spectrum taken from arXiv:1104.3259 (128nm excitation wavelength)
   * - Absorption spectrum: very difficult to find good data. Values in TPBAbsorption.dat
   *                        correspond to measurements reported in arXiv:1709.05002 for TPB
   *                        evaporated on utraviolet-transmitting acrylic substrate
   * - Quantum efficiency: literature value of 1.2 suggested by M. Walter
   *                       recommendation of DARWIN/ArDM
   * - Time constant: arbitrary small, same used by WArP people
   * - Refractive index: // http://www.molbase.com/en/overview_1450-63-1-moldata-77892.html
   *
   * -Luigi Pertoldi
   */

  G4double TPB_QuantumEff   = 1.2;
  G4double TPB_TimeConstant = 0.01 *ns;
  G4double TPB_RefrIndex    = 1.635;

  G4double TPB_refraction[NUMENTRIES_2];
  G4double TPB_absorption[NUMENTRIES_2];
  G4double TPB_WLSabsorption[NUMENTRIES_2];
  G4double TPB_emission  [NUMENTRIES_2];

  // read emission spectrum from file
  auto TPBEmissionGraph   = std::unique_ptr<TGraph>(ReadSpectrumFromFile("TPBEmission.dat"));
  // read absorption from file (lengths in nanometers)
  auto TPBAbsorptionGraph = std::unique_ptr<TGraph>(ReadSpectrumFromFile("TPBAbsorption.dat"));
  for (int i = 0; i < NUMENTRIES_2; i++) {
    TPB_refraction[i] = TPB_RefrIndex;

    // use emission spectrum from file
    auto e = TPBEmissionGraph->Eval(LambdaE/(ph_energies[i])/nm);
    TPB_emission[i] = e >= 0 ? e : 0;

    // use absorption length from file
    auto a = TPBAbsorptionGraph->Eval(LambdaE/(ph_energies[i])/nm) *nm;
    TPB_absorption[i] = a >= 0 ? a : 0;
    
    //Note that Absorption for WLS should not over lay the emission spectrum of the TPB
    //else WLS photons can be reemmitted by the WLS
    if(LambdaE/(ph_energies[i])/nm < 350){
      TPB_WLSabsorption[i] = 100*nm;
    }
    else{
      TPB_WLSabsorption[i] = 100*m;
    }
  }

  auto TPBTable = new G4MaterialPropertiesTable();
  TPBTable->AddProperty     ("RINDEX",               ph_energies, TPB_refraction, NUMENTRIES_2);
  TPBTable->AddProperty     ("WLSABSLENGTH",         ph_energies, TPB_WLSabsorption, NUMENTRIES_2);
  TPBTable->AddProperty     ("WLSCOMPONENT",         ph_energies, TPB_emission,   NUMENTRIES_2);
  TPBTable->AddConstProperty("WLSTIMECONSTANT",      TPB_TimeConstant);
  TPBTable->AddConstProperty("WLSMEANNUMBERPHOTONS", TPB_QuantumEff);
  //TPBTable->AddProperty     ("ABSLENGTH"           , ph_energies, TPB_WLSabsorption, NUMENTRIES_2);
  G4Material::GetMaterial("TPB")->SetMaterialPropertiesTable(TPBTable);

  //////////////////
  // TPB On Nylon //
  //////////////////

  /** Emission spectrum
   *
   * Taken from GSTR-15-504
   * The TPB should be embedded in a polystyrene matrix (30% TPB 70% PS) and diluited in
   * toluene (ratio 1:10), then brushed on nylon. The spectrum in TPBOnNylonEmission.dat
   * comes from Fig. 2b, brown curve. The spectrum is similar to the one in arXiv:1304.6117
   * Fig. 14, for TPB in PS matrix on glass, which makes me hope that the spectrum makes sense
   *
   * For 30% TPB 70% PS the WLS light yield is reduced by 30% [Alexey]
   *
   * See material TPB for the other properties
   *  -Luigi Pertoldi
   */

  G4double TPBOnNylon_emission   [NUMENTRIES_2];
  //Need attenuation length and WLS-attenuation because both physical properties exist
  G4double TPBOnNylon_absorption [NUMENTRIES_2];


  // read emission spectrum from file
  auto TPBOnNylonSpecGraph = std::unique_ptr<TGraph>(ReadSpectrumFromFile("TPBOnNylonEmission.dat"));

  // lengths are in m in the file
  auto NylonAbsGraph = std::unique_ptr<TGraph>(ReadSpectrumFromFile("NylonAbsorption.dat"));

  for (int i = 0; i < NUMENTRIES_2; i++) {
    // use emission from file
    auto e = TPBOnNylonSpecGraph->Eval(LambdaE/(ph_energies[i])/nm);
    TPBOnNylon_emission[i] = e >= 0 ? e : 0;

    auto a = NylonAbsGraph->Eval(LambdaE/(ph_energies[i])/nm) *m;
    TPBOnNylon_absorption[i]= a >= 0 ? a : 0;
  }

  G4MaterialPropertiesTable *TPBOnNylonTable = new G4MaterialPropertiesTable();
  TPBOnNylonTable->AddProperty     ("RINDEX",               ph_energies, TPB_refraction,        NUMENTRIES_2);
  TPBOnNylonTable->AddProperty     ("WLSABSLENGTH",         ph_energies, TPB_absorption,        NUMENTRIES_2);
  TPBOnNylonTable->AddProperty     ("ABSLENGTH",            ph_energies, TPBOnNylon_absorption, NUMENTRIES_2);
  TPBOnNylonTable->AddProperty     ("WLSCOMPONENT",         ph_energies, TPBOnNylon_emission,   NUMENTRIES_2);
  TPBOnNylonTable->AddConstProperty("WLSTIMECONSTANT",      TPB_TimeConstant);
  TPBOnNylonTable->AddConstProperty("WLSMEANNUMBERPHOTONS", 0.84);
  //TPBOnNylonTable->AddConstProperty("SCINTILLATIONYIELD",   TPB_ScintYield);
  G4Material::GetMaterial("TPBOnNylon")->SetMaterialPropertiesTable(TPBOnNylonTable);

  ///////////////////
  // TPB on VM2000 //
  ///////////////////
  
  //
  //VM2000 will NOT be used in LEGEND because Tetratex is more radio pure, it is added here just to have it...
  /** Emission spectrum
   *
   * Values taken from arXiv:1304.6117
   * They measure the emission spectrum of TPB (~160 um thick layer) on VM2000
   * at an excitation wavelength of 128nm and at 87K, so exactly in our experimental
   * conditions. The major differences brougth by the LAr temperature are the vibronic
   * structures that modify the shape of the spectrum.
   *
   * The old emission spectrum can be still found in TPBOnVM2000Emission.dat and at
   * https://www.mpi-hd.mpg.de/gerdawiki/img_auth.php/8/8a/MaGe_optical_properties_table.pdf
   * as was measured (under which conditions?) by M. Heisel and A. Wegmann in the past
   *  -Luigi Pertoldi
   * See material TPB for the other properties
   */

  G4double TPBOnVM2000_emission  [NUMENTRIES_2];

  auto TPBOnVM2000SpecGraph = std::unique_ptr<TGraph>(ReadSpectrumFromFile("TPBOnVM2000Emission-87K.dat"));

  for (int i = 0; i < NUMENTRIES_2; i++) {
    // use emission from file
    auto e = TPBOnVM2000SpecGraph->Eval(LambdaE/(ph_energies[i])/nm);
    TPBOnVM2000_emission[i] = e >= 0 ? e : 0;
  }

  G4MaterialPropertiesTable *TPBOnVM2000Table = new G4MaterialPropertiesTable();
  TPBOnVM2000Table->AddProperty     ("RINDEX",               ph_energies, TPB_refraction,       NUMENTRIES_2);
  TPBOnVM2000Table->AddProperty     ("WLSABSLENGTH",         ph_energies, TPB_absorption,       NUMENTRIES_2);
  TPBOnVM2000Table->AddProperty     ("WLSCOMPONENT",         ph_energies, TPBOnVM2000_emission, NUMENTRIES_2);
  TPBOnVM2000Table->AddConstProperty("WLSTIMECONSTANT",      TPB_TimeConstant);
  TPBOnVM2000Table->AddConstProperty("WLSMEANNUMBERPHOTONS", TPB_QuantumEff);
  //TPBOnVM2000Table->AddConstProperty("SCINTILLATIONYIELD",   TPB_ScintYield);
  G4Material::GetMaterial("TPBOnVM2000")->SetMaterialPropertiesTable(TPBOnVM2000Table);

  /////////////////////
  // TPB on Tetratex //
  /////////////////////

  /** Emission spectrum
   *
   * Taken from (our) publication: arXiv:1503.05349
   * Seems that the TPB is dip-coated (0.9 mg/cm2 ~ 8 um thickness) on the Tetratex
   * so here I'm taking the red curve in Fig. 4 (without the peak), which actually is
   * measured for 0.17 mg/cm2. In principle the thickness affects the shape of the
   * emission spectrum, as the efficiency of the reabsorption effect increases with
   * the thickness of the layer. However, I didn't find other measurements around...
   *  -Luigi Pertoldi
   * See material TPB for the other properties
   */

  G4double TPBOnTetratex_emission[NUMENTRIES_2];

  // read emission spectrum from file
  auto TetratexSpecGraph = std::unique_ptr<TGraph>(ReadSpectrumFromFile("TPBOnTetratexEmission.dat"));

  for (int i = 0; i < NUMENTRIES_2; i++) {
    // use emission from file
    auto e = TetratexSpecGraph->Eval(LambdaE/(ph_energies[i])/nm);
    TPBOnTetratex_emission[i] = e >= 0 ? e : 0;
  }

  G4MaterialPropertiesTable *TPBOnTetratexTable = new G4MaterialPropertiesTable();
  TPBOnTetratexTable->AddProperty     ("RINDEX",               ph_energies, TPB_refraction,         NUMENTRIES_2);
  TPBOnTetratexTable->AddProperty     ("WLSABSLENGTH",         ph_energies, TPB_absorption,         NUMENTRIES_2);
  TPBOnTetratexTable->AddProperty     ("WLSCOMPONENT",         ph_energies, TPBOnTetratex_emission, NUMENTRIES_2);
  TPBOnTetratexTable->AddConstProperty("WLSTIMECONSTANT",      TPB_TimeConstant);
  TPBOnTetratexTable->AddConstProperty("WLSMEANNUMBERPHOTONS", TPB_QuantumEff);
  //TPBOnTetratexTable->AddConstProperty("SCINTILLATIONYIELD",   TPB_ScintYield);
  G4Material::GetMaterial("TPBOnTetratex")->SetMaterialPropertiesTable(TPBOnTetratexTable);

}


//copied from GEGSLArGeOptical.cc
G4double MGLGNDOpticalMaterialProperties::TPBEmissionSpectrum(G4double energy)
{
  if (fSuccessfulInitialization)
  {
    //Here the data are loaded
    G4double targetf = LambdaE/energy;

    G4double eff =fTPBspec->Eval(targetf/nm);
    if (eff < 0.2) eff = 0.2;
    //MGLog(routine) << "Eval ("<< targetf/nm<< ")yielded a value of " << eff << endlog;
    return eff;
  }
  else
  {
    return 0.2;
  }
}

void MGLGNDOpticalMaterialProperties::Register_Fiber_Properties()
{
  //Stolen from gerdageometry/src/GEGeometryLArInstHybrid.cc
  //N. McFadden
  //updated with: https://www.crystals.saint-gobain.com/sites/imdf.crystals.com/files/documents/fiber-product-sheet.pdf
  ///
  /// -- Material definition --
  ///

  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  G4NistManager *nistMan = G4NistManager::Instance();

  //--------------------------------------------------
  // Fiber and WLS: Polystyrene
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(8);
  elements.push_back("H");     natoms.push_back(8);

  density = 1.050*g/cm3;

  // -- Rough approximation of BCF-91A             
  fFiber_material = nistMan->ConstructNewMaterial("PolystyreneFiber", elements, natoms, density);
  
  // Fibers Polystyrene //
  ////////////////////////

  /** Absorption spectrum
   *
   * The data sheet from Saint Gobain at
   * https://www.crystals.saint-gobain.com/sites/imdf.crystals.com/files/documents/fiber-brochure.pdf
   * reports the absorption spectrum for BCF-91A. Knowing that the fibers are 1mm thick one can
   * extract the absorption length: starting from the trivial relation:
   *
   * 1 - P(E) = exp(-x/l(E))
   *
   * where P(E) is the probability (thus proportional to the absorption spectrum) for a photon
   * travelling a distance x to be absorbed in the material given the attenuation length l(E), one
   * can extract l(E) from P(E).  By integrating over the thickness of the material L one obtains:
   *
   * (1 - P(E)) * L = l(E) * (1 - exp(-L/l(E)))
   *
   * but the problem now is that l(E) cannot be extracted analytically (inhomogeneus expression).
   * I wrote a Mathematica script that solves it numerically, the result is saved in
   * FiberAbsorption.dat. Remeber that the units are arbitrary because the original absorption
   * spectrum has arbitrary units.
   *
   * Jozsef measured an absorption length of 0.7 mm at 400 nm, the spectrum has been rescaled by
   * that. Reference in Raphael Kneissl's bachelor thesis
   *
   * Emission spectrum: from the Saint-Gobain data sheets
   * WLS time constant: from the Saint-Gobain data sheets
   * -Luigi Pertoldi
   */

  // read absorption spectrum from file
  auto FibersAbsorptionGr = std::unique_ptr<TGraph>(ReadSpectrumFromFile("FibersAbsorption.dat"));
  // read emission spectrum from file
  auto FibersEmissionGr = std::unique_ptr<TGraph>(ReadSpectrumFromFile("FibersEmission.dat"));

  G4double* FiberWLSAbsorption = new G4double[NUMENTRIES_2];
  G4double* FiberWLSEmission   = new G4double[NUMENTRIES_2];
  G4double* FiberRIndex        = new G4double[NUMENTRIES_2];
  G4double* FiberAbsorption    = new G4double[NUMENTRIES_2];

  // stuff that we have no variable information and therefore use a constant value

  // compute scale factor for absorption lengths
  G4double scaleAbs = 0.7*mm / (FibersAbsorptionGr->Eval(400)*m);

  MGLog(debugging) << "Making PolystyreneFiber optical properties" << endlog;
  MGLog(debugging) << "energy[eV]\twavelength[nm]\tWLS-absl[mm]\tWLS-emi" << endlog;

  for (int i = 0; i < NUMENTRIES_2; ++i) {
    FiberRIndex[i] = 1.6;
    FiberAbsorption[i] = 3.8*m;
    // use absorption from file
    auto a = FibersAbsorptionGr->Eval(LambdaE/(ph_energies[i])/nm) *m;
    FiberWLSAbsorption[i] = a >= 0 ? a : 0;
    FiberWLSAbsorption[i] *= scaleAbs;
    
    // use emission from file
    auto e = FibersEmissionGr->Eval(LambdaE/(ph_energies[i])/nm);
    FiberWLSEmission[i] = e >= 0 ? e : 0;
    MGLog(debugging) << ph_energies[i]/eV << "\t" << LambdaE/(ph_energies[i])/nm << "\t\t"
      << FiberWLSAbsorption[i] << "\t\t" << FiberWLSEmission[i] << "\t" << endlog;
  }

  G4MaterialPropertiesTable* fiberTable = new G4MaterialPropertiesTable();
  fiberTable->AddProperty     ("RINDEX",          ph_energies, FiberRIndex,        NUMENTRIES_2);
  fiberTable->AddProperty     ("ABSLENGTH",       ph_energies, FiberAbsorption,    NUMENTRIES_2);
  fiberTable->AddProperty     ("WLSABSLENGTH",    ph_energies, FiberWLSAbsorption, NUMENTRIES_2);
  fiberTable->AddProperty     ("WLSCOMPONENT",    ph_energies, FiberWLSEmission,   NUMENTRIES_2);
  fiberTable->AddConstProperty("WLSTIMECONSTANT", 12 *ns);
  //fiberTable->AddConstProperty("WLSMEANNUMBERPHOTONS", ???);
  G4Material::GetMaterial("PolystyreneFiber")->SetMaterialPropertiesTable(fiberTable);

}

//Copied from GEGeometryLArInstHybrid.cc
void MGLGNDOpticalMaterialProperties::Register_Fiber_Cladding_Properties()
{
  //--------------------------------------------------
  // Fiber cladding double wrapped: PMMA (inner) and Fluor-acrylic (outer)
  // It does not really matter the material, just the refractive index
  // https://www.crystals.saint-gobain.com/sites/imdf.crystals.com/files/documents/fiber-product-sheet.pdf
  //--------------------------------------------------
  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  G4NistManager *nistMan = G4NistManager::Instance();

  elements.push_back("C");  natoms.push_back(5);
  elements.push_back("H");  natoms.push_back(8);
  elements.push_back("O");  natoms.push_back(2);
  density = 1.2*g/cm3;

  // -- Rough approximation of BCF-10
  fFiber_claddingInner_material = nistMan->ConstructNewMaterial("PMMA", elements, natoms, density);

  elements.clear();
  natoms.clear();

  // Attach optical properties to the cladding. Only the RIndex is important
  const G4int npoints_fixed = 2;
  G4double claddingFixEnergies[npoints_fixed] = {LambdaE/(650.0*nanometer),LambdaE/(115.0*nanometer)};
  G4double claddingRIndex[npoints_fixed] = {1.49,1.49};
  //Photons seem to get stuck between TPB and fiber cladding, maybe cladding needs absorption length
  //G4double claddingAbsorption[npoints_fixed] = {1*m,1*m};
  G4MaterialPropertiesTable *claddingTable = new G4MaterialPropertiesTable();

  claddingTable->AddProperty("RINDEX",claddingFixEnergies,claddingRIndex,npoints_fixed);
  //claddingTable->AddProperty("ABSLENGTH",claddingFixEnergies,claddingAbsorption,npoints_fixed);

  fFiber_claddingInner_material->SetMaterialPropertiesTable(claddingTable);

  elements.clear();natoms.clear();
  //could not find Fluor-acrylic properties, maybe trade marked?
  //Polyacrylonitrile is similar and used instead, index of refraction is the only real important value (taken from saint-gobain)
  density = 1.184*g/cm3;
  elements.push_back("C");  natoms.push_back(3);
  elements.push_back("H");  natoms.push_back(3);
  elements.push_back("N");  natoms.push_back(1);

  fFiber_claddingOuter_material = nistMan->ConstructNewMaterial("Fluor-acrylic",elements, natoms, density);
  
  G4double claddingOuterFixEnergies[npoints_fixed] = {LambdaE/(650.0*nanometer),LambdaE/(115.0*nanometer)};
  G4double claddingOuterRIndex[npoints_fixed] = {1.42,1.42};
  //Photons seem to get stuck between TPB and fiber cladding, maybe cladding needs absorption length
  //G4double claddingAbsorption[npoints_fixed] = {1*m,1*m};
  G4MaterialPropertiesTable *claddingOuterTable = new G4MaterialPropertiesTable();

  claddingOuterTable->AddProperty("RINDEX",claddingOuterFixEnergies,claddingOuterRIndex,npoints_fixed);
 // claddingTable->AddProperty("ABSLENGTH",claddingOuterFixEnergies,claddingOuterAbsorption,npoints_fixed);

  fFiber_claddingOuter_material->SetMaterialPropertiesTable(claddingOuterTable);
  MGLog (debugging) << " Constructed Fiber Cladding Properties"<< endlog;
}

//copied from GEGeometryLArInstrumentation.cc
void MGLGNDOpticalMaterialProperties::Register_Nylon_Properties()
{



  ///////////
  // Nylon //
  ///////////

  /**
   * Absorption length from arXiv:1704.02291
   */

  // lengths are in m in the file
  auto NylonAbsGraph = std::unique_ptr<TGraph>(ReadSpectrumFromFile("NylonAbsorption.dat"));

  G4double Nylon_absorption[NUMENTRIES_2];
  G4double Nylon_refraction[NUMENTRIES_2];

  for (int i = 0; i < NUMENTRIES_2; i++) {
    Nylon_refraction[i] = 1.54;

    // use absorption length from file
    auto a = NylonAbsGraph->Eval(LambdaE/(ph_energies[i])/nm) *m;
    Nylon_absorption[i] = a >= 0 ? a : 0;
  }

  auto NylonTable = new G4MaterialPropertiesTable();
  NylonTable->AddProperty("RINDEX",       ph_energies, Nylon_refraction, NUMENTRIES_2);
  //GERDA code had this as WLSABSLEGNTH, should be ABSLENGTH
  NylonTable->AddProperty("ABSLENGTH", ph_energies, Nylon_absorption, NUMENTRIES_2);
  G4Material::GetMaterial("Nylon")->SetMaterialPropertiesTable(NylonTable);
}

//stolen from gerdageometry/src/GEGeometryLArInstrumentation.cc for MetalCopper
//TODO Not sure optical proper electroformed copper but for now assuming the same as metal coppper
void MGLGNDOpticalMaterialProperties::Register_Copper_Properties()
{

  ////////////
  // Copper //
  ////////////

  /* Measurements from Anne Wegmann's thesis:
   * https://www.mpi-hd.mpg.de/gerda/public/2017/phd2017-anneWegmann.pdf
  */

  auto CuReflGr = std::unique_ptr<TGraph>(ReadSpectrumFromFile("Reflectivity_Cu.dat"));

  const G4int n_points_cu = CuReflGr->GetN();
  G4double* ReflectivityCu   = new G4double[n_points_cu];
  G4double* PhotonEnergyCu   = new G4double[n_points_cu];

  // put energies in ascending order
  for (int i = 0; i < n_points_cu; ++i) {
    PhotonEnergyCu[i] = LambdaE/(CuReflGr->GetX()[n_points_cu-1-i]*nm);
    ReflectivityCu[i] = CuReflGr->GetY()[n_points_cu-1-i];
    MGLog(debugging)<<"Copper reflectivity "<<ReflectivityCu[i]<<" "<<PhotonEnergyCu[i]<<endlog;
  }

  auto CuOptTable = new G4MaterialPropertiesTable();
  CuOptTable->AddProperty("REFLECTIVITY", PhotonEnergyCu, ReflectivityCu, n_points_cu);

  G4Material::GetMaterial("Copper-EF")->SetMaterialPropertiesTable(CuOptTable);

}

void MGLGNDOpticalMaterialProperties::Register_Germanium_Properties()
{
  ///////////////
  // Germanium //
  ///////////////

  /*** Optical properties of Germanium
   * Needs to be attached to all deadlayer logical surfaces.
   *
   * Measurements from Anne Wegmann's thesis:
   * https://www.mpi-hd.mpg.de/gerda/public/2017/phd2017-anneWegmann.pdf
   * The Ge measured there is our dead-layer Li-doped germanium, at room temperature
   *
   * Found a reference for reflectivity to VUV light in Germanium:
   * http://prola.aps.org/abstract/PR/v160/i3/p602_1
   * It quotes a reflectivity quite high for UV light (~60-70%), but strongly
   * dependent on the incident angle
  */

  auto GeReflGr = std::unique_ptr<TGraph>(ReadSpectrumFromFile("Reflectivity_Ge.dat"));

  const G4int n_points_ge = GeReflGr->GetN();
  G4double* ReflectivityGe = new G4double[n_points_ge];
  G4double* PhotonEnergyGe = new G4double[n_points_ge];

  // put energies in ascending order
  for (int i = 0; i < n_points_ge; ++i) {
    PhotonEnergyGe[i] = LambdaE/(GeReflGr->GetX()[n_points_ge-1-i]*nm);
    ReflectivityGe[i] = GeReflGr->GetY()[n_points_ge-1-i];
  }

  G4MaterialPropertiesTable *geOptTable = new G4MaterialPropertiesTable();
  geOptTable->AddProperty("REFLECTIVITY", PhotonEnergyGe, ReflectivityGe, n_points_ge);
  
  //LGND naming scheme
  G4Material *germanium = G4Material::GetMaterial("Germanium-Enr");
  germanium->SetMaterialPropertiesTable(geOptTable);
  germanium = G4Material::GetMaterial("Germanium-Nat");
  germanium->SetMaterialPropertiesTable(geOptTable);
}

void MGLGNDOpticalMaterialProperties::Register_Silicon_Properties()
{
  /////////////
  // Silicon //
  /////////////

  /* Measurements from Anne Wegmann's thesis:
   * https://www.mpi-hd.mpg.de/gerda/public/2017/phd2017-anneWegmann.pdf
   */

  auto SiReflGr = std::unique_ptr<TGraph>(ReadSpectrumFromFile("Reflectivity_Si.dat"));

  const G4int n_points_si = SiReflGr->GetN();
  G4double* ReflectivitySi   = new G4double[n_points_si];
  G4double* PhotonEnergySi   = new G4double[n_points_si];

  // put energies in ascending order
  for (int i = 0; i < n_points_si; ++i) {
    PhotonEnergySi[i] = LambdaE/(SiReflGr->GetX()[n_points_si-1-i]*nm);
    ReflectivitySi[i] = SiReflGr->GetY()[n_points_si-1-i];
  }

  auto siOptTable = new G4MaterialPropertiesTable();
  siOptTable->AddProperty("REFLECTIVITY", PhotonEnergySi, ReflectivitySi, n_points_si);

}

void MGLGNDOpticalMaterialProperties::Register_Teflon_Properties()
{

    /////////////
  // Teflon //
  /////////////

  /* Measurements from Anne Wegmann's thesis:
   * https://www.mpi-hd.mpg.de/gerda/public/2017/phd2017-anneWegmann.pdf
   */

  auto TeflonReflGr = std::unique_ptr<TGraph>(ReadSpectrumFromFile("Reflectivity_Teflon.dat"));

  const G4int n_points_tef = TeflonReflGr->GetN();
  G4double* ReflectivityTeflon   = new G4double[n_points_tef];
  G4double* PhotonEnergyTeflon   = new G4double[n_points_tef];
  // put energies in ascending order
  for (int i = 0; i < n_points_tef; ++i) {
    PhotonEnergyTeflon[i] = LambdaE/(TeflonReflGr->GetX()[n_points_tef-1-i]*nm);
    ReflectivityTeflon[i] = TeflonReflGr->GetY()[n_points_tef-1-i];
  }

  auto TeflonOptTable = new G4MaterialPropertiesTable();
  TeflonOptTable->AddProperty("REFLECTIVITY", PhotonEnergyTeflon, ReflectivityTeflon, n_points_tef);

  G4Material::GetMaterial("Teflon")->SetMaterialPropertiesTable(TeflonOptTable);
}

void MGLGNDOpticalMaterialProperties::Register_Silica_Properties()
{
  G4MaterialPropertiesTable *silicaOptTable = new G4MaterialPropertiesTable;

  //from https://refractiveindex.info/?shelf=main&book=SiO2&page=Malitson
  //..........and............
  //Optical constants of silica glass from extreme ultraviolet to far infrared at near room temperature 
  //Rei Kitamura,1 Laurent Pilon,1,* and Miroslaw Jonasz2
  const G4int npoints= 7;
  G4double wavelength[npoints]  = {100.,120.,210.,300.,350.,500.,650.0};
  G4double index[npoints]       = {1.32,2.35,1.54,1.49,1.48,1.46,1.456};
  G4double absorbptionlength[npoints]       = {0.8*m,10*m,10*m,10*m,10*m,10*m,10*m};
  G4double energy[npoints] ;
  for(int i = 0; i < npoints;i++){
    energy[npoints - i - 1] = LambdaE/(wavelength[npoints-1-i]*nm);
    
  }

  silicaOptTable->AddProperty("RINDEX",energy,index,npoints);
  silicaOptTable->AddProperty("ABSLENGTH",energy,absorbptionlength,npoints);
  G4Material::GetMaterial("Silica")->SetMaterialPropertiesTable(silicaOptTable);
}

void MGLGNDOpticalMaterialProperties::Register_VM2000()
{
  G4MaterialPropertiesTable *vm2000OptTable = new G4MaterialPropertiesTable;
  G4int ji = 0;
  G4double ee=0;
  // Build table with photon energies
  const G4int spot = 500;

  G4double PPSCHighE = LambdaE /(115*nanometer);
  G4double PPSCLowE = LambdaE /(650*nanometer); //598
  G4double dee = ((PPSCHighE - PPSCLowE) / ((G4double)(spot-1)));
  G4double LAr_SCPP[spot];
  for (ji = 0; ji < spot; ji++)
  {
    ee=PPSCLowE+ ((G4double)ji) * dee;
    LAr_SCPP[ji]=ee;
  }
  ///Reflector (VM2000)
  //From Dario's thesis, reflectivity of VM2000 is ~98% above 370 nm
  //and ~15% below it (almost a step-function)

  G4double Reflectivity[spot];
  G4double Efficiency[spot];

  for (ji=0;ji < spot; ji++)
  {
    if (LAr_SCPP[ji] < (LambdaE/(370*nanometer)))
      Reflectivity[ji] = 0.98; //visible
    else
      Reflectivity[ji] = 0.15; //UV
    Efficiency[ji] = 0.0;
  }
  vm2000OptTable->AddProperty("REFLECTIVITY",LAr_SCPP,Reflectivity,spot);
  vm2000OptTable->AddProperty("EFFICIENCY",LAr_SCPP,Efficiency,spot);

  G4Material::GetMaterial("VM2000")->SetMaterialPropertiesTable(vm2000OptTable);
}

void MGLGNDOpticalMaterialProperties::Register_StainlessSteel()
{
    // Attach properties to the other materials
   // -- germanium
   //
  char* path = getenv("MGGENERATORDATA");
  if (!path)
  {
    MGLog(warning)<< "MGGENERATORDATA environment variable not set! Assuming local input (./)."
                  << "You find the files in $MAGE/generators/data. Set MGGENERATORDATA to point there." << endlog;
    path = (char*) ".";
  }
  G4String pathString(path);

  G4MaterialPropertiesTable *ssOptTable = new G4MaterialPropertiesTable();
  G4String pathFile = pathString + "/RIndexReal_SS.dat";

  TGraph *ssRIndexReal = new TGraph(pathFile.data(),"%lg,%lg,%*lg");  
  G4double *RIndexRealSi = new G4double[ssRIndexReal->GetN()];
  G4double *waveIndexRealSi = new G4double[ssRIndexReal->GetN()];
  G4double *nrgIndexRealSi = new G4double[ssRIndexReal->GetN()];

  pathFile = pathString + "/RIndexImag_SS.dat";
  TGraph *ssRIndexImag = new TGraph(pathFile.data(),"%lg,%lg,%*lg");
  G4double *RIndexImagSi = new G4double[ssRIndexImag->GetN()];
  G4double *waveIndexImagSi = new G4double[ssRIndexImag->GetN()];
  G4double *nrgIndexImagSi = new G4double[ssRIndexReal->GetN()];

  G4int NpointsReal =  ssRIndexReal->GetN(); 
  G4int NpointsImag =  ssRIndexImag->GetN(); 
  if(NpointsReal != NpointsImag){
    G4cout<<"RIndexReal_Si.dat has different number of points than RIndexImag_Si.dat"<<G4endl;
    MGLog(fatal)<<endlog;
  }
  G4double *RIndexSi = new G4double[NpointsReal];
  G4double *absLengthSi = new G4double[NpointsReal];
  for(int i = 0; i < ssRIndexReal->GetN();i++){

    waveIndexRealSi[i] = (ssRIndexReal->GetX())[i];
    waveIndexImagSi[i] = (ssRIndexImag->GetX())[i];

    nrgIndexRealSi[NpointsReal-i-1] = LambdaE/(waveIndexRealSi[i]*nm);
    nrgIndexImagSi[NpointsImag-i-1] = LambdaE/(waveIndexImagSi[i]*nm);

    RIndexSi[i] = sqrt( (ssRIndexReal->GetY())[i]*(ssRIndexReal->GetY())[i] +(ssRIndexImag->GetY())[i]*(ssRIndexImag->GetY())[i] );
    absLengthSi[i] = waveIndexRealSi[i]/(4*pi*(ssRIndexImag->GetY())[i]);//skin depth ~100 nm
  }


  ssOptTable->AddProperty("RINDEX",nrgIndexRealSi,RIndexSi,NpointsReal);
  ssOptTable->AddProperty("ABSLENGTH",nrgIndexRealSi,absLengthSi,NpointsReal);

  G4Material *ssteel = G4Material::GetMaterial("Steel");
  ssteel->SetMaterialPropertiesTable(ssOptTable);
}
void MGLGNDOpticalMaterialProperties::Register_Acrylic()
{
  G4NistManager*   nist = G4NistManager::Instance();
  G4Element* elH = nist->FindOrBuildElement("H");
  G4Element* elC = nist->FindOrBuildElement("C");
  G4Element* elO = nist->FindOrBuildElement("O");

  G4Material* Acrylic;
  if(G4Material::GetMaterial("Acrylic") != NULL){
    Acrylic = G4Material::GetMaterial("Acrylic");
  }
  else{
    Acrylic  = new G4Material("Acrylic",1.18*g/cm3,3,kStateSolid);
    Acrylic->AddElement(elH,8);
    Acrylic->AddElement(elC,5);
    Acrylic->AddElement(elO,2);
  }
  //Data is in wavelength (nm) and absorption length (mm)
  //Taken from https://wiki.bnl.gov/dayabay/upload/Acrylic_Transmittance_Sep14.pdf
  auto acrylicAbsorptionGraph = std::unique_ptr<TGraph>(ReadSpectrumFromFile("AcrylicAbsorption.dat"));
  G4double acrylic_absorption[NUMENTRIES_2];
  G4double acrylic_index[NUMENTRIES_2];
  for (int i = 0; i < NUMENTRIES_2; i++) {
    auto r = acrylicAbsorptionGraph->Eval(LambdaE/(ph_energies[i])/nm)*mm;
    if(LambdaE/(ph_energies[i])/nm < 200) r = 0;
    acrylic_absorption[i] = r >= 0 ? r : 0;
    acrylic_index[i] = 1.490;
  }
  
  auto acrylicTable = new G4MaterialPropertiesTable();
  acrylicTable->AddProperty("RINDEX"   , ph_energies, acrylic_index, NUMENTRIES_2);
  acrylicTable->AddProperty("ABSLENGTH", ph_energies, acrylic_absorption, NUMENTRIES_2);
  G4Material::GetMaterial("Acrylic")->SetMaterialPropertiesTable(acrylicTable);
  
}

TGraph* MGLGNDOpticalMaterialProperties::ReadSpectrumFromFile(G4String filename) {

  MGLog(debugging) << "Looking for " << filename << " file" << endlog;
  G4String pathFile;
  if (!getenv("MGGENERATORDATA")) {
    MGLog(warning) << "MGGENERATORDATA environment variable not set! Setting it to '.'" << endlog;
    pathFile = ".";
  }
  else pathFile = G4String(getenv("MGGENERATORDATA"));
  pathFile += "/" + filename;
  if(!std::ifstream(pathFile).is_open()) {
    MGLog(fatal) << "Could not find " << pathFile << ". pleas set the MGGENERATORDATA variable." << endlog;
  }

  auto _g = new TGraph(pathFile.data());

  if (!_g->IsZombie() and _g->GetN() > 0 ) {
    MGLog(trace) << "Spectrum (" << _g->GetN()
                   << " points) successfully loaded from " << filename << endlog;
  } else {
    MGLog(fatal) << "could not read data from from " << pathFile.data() << endlog;
  }

  return _g;
}
