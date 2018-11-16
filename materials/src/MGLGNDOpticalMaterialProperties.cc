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

MGLGNDOpticalMaterialProperties::MGLGNDOpticalMaterialProperties()
  :fSuccessfulInitialization(false)
{
  
}

MGLGNDOpticalMaterialProperties::~MGLGNDOpticalMaterialProperties()
{}

void MGLGNDOpticalMaterialProperties::ConstructionOpticalProperties()
{
  InitializeTPBSpectra();
  InitializeFiberSpectra();
  RegisterArgonOpticalProperties();
  RegisterXeDopedArgonOpticalProperties();
  Register_TPB_Properties();
  Register_Fiber_Properties();
  Register_Fiber_Cladding_Properties();
  Register_Nylon_Properties();
  Register_Copper_Properties();
  Register_Germanium_Properties();
  Register_Silicon_Properties();
  Register_Teflon_Properties();
  Register_Silica_Properties();
  Register_VM2000();
  Register_StainlessSteel();

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
	  for (ji = 0; ji < NUMENTRIES; ji++)
	    {
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
	      } else {
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
	  for (ji = 0; ji < num; ji++)
	    {
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
G4double MGLGNDOpticalMaterialProperties::XeDopedArScintillationSpectrum(const G4double kk){
  G4double waveL;
  waveL =exp(-0.5*((kk-175.0)/(8.6))*((kk-175.0)/(8.6)));
  return waveL;
}

//Assume that all Ar 128 nm is converted to 175 nm (100-1000 ppm)
//1.10 m attenuation length
//light yeild is the same
void MGLGNDOpticalMaterialProperties::RegisterXeDopedArgonOpticalProperties()
{
	  static const G4int NUMENTRIES = 65;
	  const G4int num = 65;
	  static const G4double temp = 88.5*kelvin;
	  static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;

	  /**
	   * Nominal values for pure argon
	   */
	  G4double scint_yield = 23.6*eV;  // Nominal energy to produce a photon (measured)
	  G4double photon_yield = 1.0*MeV/scint_yield;
	  G4double tau_s = 6.0*ns;
	  G4double tau_l = 240.*ns;
	  /*G4double yield_ratio = 0.23; // For gammas and electrons*/


	  // New value based on the triplet lifetime from Mark Heisel
	  // Redefine the values to res-scale according to Mark's calculation
	  // TODO - what is the correct yield value?
    // G4double LAr_LY_scale = fDetectorDB->GetLArInstArgonLYScale();
	  // photon_yield = 28120. * LAr_LY_scale;
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
	  G4double PPCKOVLowE = LambdaE / (240*nanometer);
	  G4double de = ((PPCKOVHighE - PPCKOVLowE) / ((G4double)(NUMENTRIES-1)));

	  // liquid argon (LAr)
	  G4double LAr_PPCK[(NUMENTRIES)];
	  G4double LAr_RIND[(NUMENTRIES)];
	  G4double LAr_RAYL[(NUMENTRIES)];
	  G4double LAr_ABSL[(NUMENTRIES)];

	  //G4double LAr_ABSL_xuv = 60*cm;
	  G4double LAr_ABSL_xuv = 110*cm;
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
      } else {
        LAr_ABSL[ji] = LAr_ABSL_vis;
      }
    }
    MGLog(debugging) << "XUV attenuation length: " << LAr_ABSL_xuv/cm << " cm" << endlog;
    MGLog(debugging) << "VIS attenuation length: " << LAr_ABSL_vis/m << " m" << endlog;

	  G4double PPSCHighE = LambdaE /(155*nanometer);
	  G4double PPSCLowE = LambdaE /(190*nanometer);
	  G4double dee = ((PPSCHighE - PPSCLowE) / ((G4double)(num-1)));
	  G4double LAr_SCIN[num];
	  G4double LAr_SCPP[num];
    for (ji = 0; ji < num; ji++){
      ee=PPSCLowE+ ((G4double)ji) * dee;
      LAr_SCPP[ji]=ee;
      LAr_SCIN[ji]=XeDopedArScintillationSpectrum((LambdaE/ee)/nanometer);
    }

	  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

	  myMPT1->AddProperty("RINDEX",        LAr_PPCK, LAr_RIND, NUMENTRIES);
	  myMPT1->AddProperty("RAYLEIGH",      LAr_PPCK, LAr_RAYL, NUMENTRIES);
	  myMPT1->AddProperty("ABSLENGTH",     LAr_PPCK, LAr_ABSL, NUMENTRIES);

	  // Fast and slow components of the scintillation
	  // They should both be the same
	  if ( (LAr_SCPP[0] >= PPCKOVLowE) && (LAr_SCPP[(sizeof(LAr_SCPP)/sizeof(G4double) - 1)] <= PPCKOVHighE) ){
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

	  G4double fano = 0.11;// Doke et al, NIM 134 (1976)353
	  myMPT1->AddConstProperty("RESOLUTIONSCALE",fano);
    /*
    fArgonLiquid = G4Material::GetMaterial("Argon-Liq");
	  fArgonLiquid->SetMaterialPropertiesTable(myMPT1);
    */
    fXenonArgonLiquid = G4Material::GetMaterial("Xenon-Doped-Argon-Liq");
    fXenonArgonLiquid->SetMaterialPropertiesTable(myMPT1);
    fXenonArgonLiquid->GetIonisation()->SetBirksConstant(5.1748e-4*cm/MeV);
    /*
    XenonArgonLiquid->AddElement(G4Element::GetElement("Argon"), 1.-1.e-6);
    new G4Isotope("Xenon124", 54, 124, 123.9058 * g / mole); //0.09
    new G4Isotope("Xenon126", 54, 126, 125.904 * g / mole); //0.09
    new G4Isotope("Xenon128", 54, 128, 127.9035 * g / mole);//1.92
    new G4Isotope("Xenon129", 54, 129, 128.9047 * g / mole); //26.44
    new G4Isotope("Xenon130", 54, 130, 129.9035 * g / mole); //4.08
    new G4Isotope("Xenon131", 54, 131, 130.9050 * g / mole); //21.18
    new G4Isotope("Xenon132", 54, 132, 131.9041 * g / mole); //26.89
    new G4Isotope("Xenon134", 54, 134, 133.9053 * g / mole); //10.44
    new G4Isotope("Xenon136", 54, 136, 135.9072 * g / mole); //8.87
    G4Element* elXenon = new G4Element("Xenon","Xe",9);
    elXenon->AddIsotope(G4Isotope::GetIsotope("Xenon124"),0.090);
    elXenon->AddIsotope(G4Isotope::GetIsotope("Xenon126"),0.090);
    elXenon->AddIsotope(G4Isotope::GetIsotope("Xenon128"),1.920);
    elXenon->AddIsotope(G4Isotope::GetIsotope("Xenon129"),26.44);
    elXenon->AddIsotope(G4Isotope::GetIsotope("Xenon130"),4.080);
    elXenon->AddIsotope(G4Isotope::GetIsotope("Xenon131"),21.18);
    elXenon->AddIsotope(G4Isotope::GetIsotope("Xenon132"),26.89);
    elXenon->AddIsotope(G4Isotope::GetIsotope("Xenon134"),10.44);
    elXenon->AddIsotope(G4Isotope::GetIsotope("Xenon136"),8.870);
    XenonArgonLiquid->AddElement(G4Element,1.e-6);
    */
    //what's the real value?

}

void MGLGNDOpticalMaterialProperties::Register_TPB_Properties()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Element* elementH = nist->FindOrBuildElement("H");
  G4Element* elementC = nist->FindOrBuildElement("C");
  fTPB= new G4Material("TPB", 1*g/cm3, 2, kStateSolid);
  fTPB->AddElement(elementH, 22);
  fTPB->AddElement(elementC, 28);
  // Now attach the optical properties to it.
  // Build table with photon energies
  
   const G4int numTPB = 500;
   //G4double HighETPB = LambdaE /(350*nanometer);
   G4double HighETPB = LambdaE /(115*nanometer);
   G4double LowETPB = LambdaE /(650*nanometer);//(650*nanometer); //598
   G4double deeTPB = ((HighETPB - LowETPB) / ((G4double)(numTPB-1)));
   G4double LAr_SCPPTPB[numTPB];
   for (G4int ji = 0; ji < numTPB; ji++)  LAr_SCPPTPB[ji]=LowETPB+ ((G4double)ji) * deeTPB;
   
   G4double WLS_absorption[numTPB];
   G4double WLS_emission[numTPB];
   G4double WLS_refraction[numTPB];

   for (G4int ji=0;ji < numTPB; ji++) {
     //N. McFadden
     //arXiv:1210.3793v3 says that TPB is imbedded in cladding thus has same refractive index
     WLS_refraction[ji] =1.42;
     // Should the TPB shift the Cherenkov light?
     // This makes a tail starting at 128 until the visible.
     if (LAr_SCPPTPB[ji] >= 3.31*eV){// <= 374.57 nm 
       // For the moment set it to always absorb photons
       WLS_absorption[ji] = 0.001*nm; //absorbs UV (always)
     } else {
       // < 374.57 nm
       WLS_absorption[ji] = 10.5*m; //otherwise transparent
       //WLS_absorption[ji] = 1.5*m; //otherwise transparent
     }
     WLS_emission[ji] = TPBEmissionSpectrum(LAr_SCPPTPB[ji]);
     //G4cout<<" \t\t"<< WLS_emission[ji] << " "<< LAr_SCPPTPB[ji]<<G4endl;
   }

   // make new table 
   G4MaterialPropertiesTable *tpbTable = new G4MaterialPropertiesTable();
   tpbTable->AddProperty("RINDEX",LAr_SCPPTPB,WLS_refraction,numTPB);
   tpbTable->AddProperty("WLSABSLENGTH",LAr_SCPPTPB,WLS_absorption,numTPB);
   tpbTable->AddProperty("WLSCOMPONENT",LAr_SCPPTPB,WLS_emission,numTPB);
   // From WArP
   tpbTable->AddConstProperty("WLSTIMECONSTANT", 0.01*ns);
   G4double WLSyield = 1.2;
   tpbTable->AddConstProperty("WLSMEANNUMBERPHOTONS",WLSyield);
   //apperently TPB scintillates?!?!?
   G4double LightYield    = 10000.; //7000 - 8000 for scintillating fibers, for WLS fiber is less
   tpbTable->AddConstProperty("SCINTILLATIONYIELD",LightYield / MeV); // limit scint. phot. JJ
   if(!fTPB) G4cout<<" TPB is bad material and you should feel bad for using it!"<<G4endl;
   fTPB->SetMaterialPropertiesTable(tpbTable);
   MGLog(routine) << "Constructed TPB Properties"<<endlog;
}
//copied from GEGSLArGeOptical.cc
void MGLGNDOpticalMaterialProperties::InitializeTPBSpectra()
{
  fSuccessfulInitialization = false;
  char* path = getenv("MGGENERATORDATA");
  if (!path)
  {
    MGLog(warning)<< "MGGENERATORDATA environment variable not set! Assuming local input (./)."
                  << "You find the files in $MAGE/generators/data. Set MGGENERATORDATA to point there." << endlog;
    path = (char*) ".";
  }
  G4String pathString(path);
  G4String pathFile = pathString + "/VM2000_em_spec.dat";
  fTPBspec = new TGraph(pathFile.data());
  if (fTPBspec->GetN() > 0 ) {
    fSuccessfulInitialization = true;
    MGLog(routine) << "TPB re-emission spectrum ( " << fTPBspec->GetN()
							      << " points) successfully loaded from file." << endlog;
    /** Uncomment for debugging purposes
		fVM2000spec->Print();
     */
  } else {
    fSuccessfulInitialization = false;
    MGLog(warning) << "TPB re-emission spectrum failed to load from [" << pathFile.data() << "]." << endlog;
    MGLog(warning) << "Cross-check that this is intended... otherwise the output won't be reliable." << endlog;
  }
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

  elements.clear();
  natoms.clear();

  G4int npoints_abs = fFibersAbsorptionSpec->GetN();
  static const  G4int npoints = 250;
  G4double *FiberAbsEnergies = new G4double[npoints];
  G4double *FiberWLSAbsorption = new G4double[npoints];
  G4double FiberTimeConstant = 7.2*ns;
  // stuff that we have no variable information and therefore use a constant value
  const G4int npoints_fixed = 2;
  G4double FiberFixEnergies[npoints_fixed] = {LambdaE/(650.0*nanometer),LambdaE/(115.0*nanometer)};
  G4double FiberRIndex[npoints_fixed] = {1.6,1.6};
  G4double FiberAbsorption[npoints_fixed] = {3.8*m,3.8*m};
  G4int idx = 0;
  // The WLS absorption is a distance.
  // Jozsef measured the value to be 0.7 mm at 400 nm.
  // Use this to scale the spectrum.
  // Have to do a backtrack of the value from the estimate at 400 nm.
  G4double wls_abs_scale = 0.7*mm*fFibersAbsorptionSpec->Eval(400);

  MGLog(debugging) << " Fiber WLS absorption spectrum (distance scale : " << wls_abs_scale << " ) :" << endlog;

  for (int i = 0; i < npoints_abs; ++i) {
    idx = npoints_abs-i-1;
    FiberAbsEnergies[i] = LambdaE/(fFibersAbsorptionSpec->GetX()[idx]*nanometer);

    //assuming the minimum as 0.02*cm at 423 nm
    // value taken from http://www-zeuthen.desy.de/lcdet/Feb_05_WS/talks/rd_lcdet_sim.pdf
    // based on measurements for MINOS
    // TODO: Should be verified with Jozsef
    FiberWLSAbsorption[i] = wls_abs_scale/fFibersAbsorptionSpec->GetY()[idx];
    MGLog(debugging) << "WL : " << fFibersAbsorptionSpec->GetX()[idx]
        << " nm  En : " << FiberAbsEnergies[i]/MeV << " MeV Abs : " << FiberWLSAbsorption[i]/cm << " cm " << endlog;
  }

  G4int npoints_em = fFibersEmissionSpec->GetN();
  G4double *FiberEmEnergies = new G4double[npoints_em];
  G4double *FiberWLSEmission = new G4double[npoints_em];

  MGLog(debugging) << " Fiber WLS emission spectrum :" << endlog;
  for (int i = 0; i < npoints_em; ++i) {
    idx = npoints_em-i-1;
    FiberEmEnergies[i] = LambdaE/(fFibersEmissionSpec->GetX()[idx]*nanometer);
    FiberWLSEmission[i] = fFibersEmissionSpec->GetY()[idx];

    MGLog(debugging) << "WL : " << fFibersAbsorptionSpec->GetX()[idx]
        << " nm En : " << FiberEmEnergies[i]/MeV << " MeV Emission : " << FiberWLSEmission[i] << endlog;
  }

  G4MaterialPropertiesTable *fiberTable = new G4MaterialPropertiesTable();

  fiberTable->AddProperty("RINDEX",FiberFixEnergies,FiberRIndex,npoints_fixed);
  fiberTable->AddProperty("ABSLENGTH",FiberFixEnergies,FiberAbsorption,npoints_fixed);
  fiberTable->AddProperty("WLSABSLENGTH",FiberAbsEnergies,FiberWLSAbsorption,npoints_abs);
  fiberTable->AddProperty("WLSCOMPONENT",FiberEmEnergies,FiberWLSEmission,npoints_em);

  // From WArP
  fiberTable->AddConstProperty("WLSTIMECONSTANT", FiberTimeConstant);

  fFiber_material->SetMaterialPropertiesTable(fiberTable);
  MGLog(debugging) << "Constructed Fiber Properties"<<endlog;
}

//copied from GEGSLArGeOptical.cc
void MGLGNDOpticalMaterialProperties::InitializeFiberSpectra()
{
  fSuccessfulInitialization = false;
  char* path = getenv("MGGENERATORDATA");
  if (!path)
  {
    MGLog(warning)<< "MGGENERATORDATA environment variable not set!" << endlog;
    path = (char*) ".";
  }
  G4String pathString(path);
  G4String pathFile = pathString + "/FibersAbsorption.dat";
  fFibersAbsorptionSpec = new TGraph(pathFile.data(),"%lg,%lg");
  if (fFibersAbsorptionSpec->GetN() > 0 ) {
    MGLog(routine) << "Fibers absorption spectrum ( " << fFibersAbsorptionSpec->GetN()
                    << " points) successfully loaded from file." << endlog;
    /** Uncomment for debugging purposes
    fFibersAborptionspec->Print();
     */
  }
  else {

    MGLog(warning) << "Fiber WLS absorption spectrum failed to load from [" << pathFile.data() << "]." << endlog;
    MGLog(warning) << "Cross-check that this is intended... otherwise the output won't be reliable." << endlog;
    G4Exception("GEGeometryLArInstHybrid::InitializeFiberSpectra",
                "MissingInputData",
                FatalException,
                "Fiber WLS absorption spectrum not found.");
  }

  pathFile = pathString + "/FibersEmission.dat";
  fFibersEmissionSpec = new TGraph(pathFile.data(),"%lg,%lg");
  if (fFibersEmissionSpec->GetN() > 0 ) {
    MGLog(routine) << "Fibers emission spectrum ( " << fFibersEmissionSpec->GetN()
                      << " points) successfully loaded from file." << endlog;
    /** Uncomment for debugging purposes
      fFibersEmissionSpec->Print();
     */

  } else {
    MGLog(warning) << "Fiber re-emission spectrum failed to load from [" << pathFile.data() << "]." << endlog;
    MGLog(warning) << "Cross-check that this is intended... otherwise the output won't be reliable." << endlog;
    G4Exception("GEGeometryLArInstHybrid::InitializeFiberSpectra",
                "MissingInputData",
                FatalException,
                "Fiber WLS re-emission spectrum not found.");
  }
  fSuccessfulInitialization = true;
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
     // -- Nylon:

 fNylon = G4Material::GetMaterial("Nylon", true);

 // Now attach the optical properties to it.
 // Build table with photon energies
 const G4int numMSNylon = 500;

 G4double PPSCHighEMSNylon = LambdaE /(115*nanometer);
 G4double PPSCLowEMSNylon = LambdaE /(650*nanometer); //598
 G4double deeMSNylon = ((PPSCHighEMSNylon - PPSCLowEMSNylon) / ((G4double)(numMSNylon-1)));
 G4double LAr_SCPPMSNylon[numMSNylon];
 for (G4int ji = 0; ji < numMSNylon; ji++)
 {
   G4double ee=PPSCLowEMSNylon+ ((G4double)ji) * deeMSNylon;
   LAr_SCPPMSNylon[ji]=ee;
 }
 G4double MSNylon_absorption[numMSNylon];
// G4double WLS_emission[numMSNylon];
 G4double MSNylonRefraction[numMSNylon];
 G4MaterialPropertiesTable *nylonTable = new G4MaterialPropertiesTable();
 // The refractive index of the TPB should be roughly the same as acrylic
 // The polystirene has very similar properties
 // Use the same value as the fibers
 for (G4int ji=0;ji < numMSNylon; ji++)
 {
   MSNylonRefraction[ji] = 1.54; //
   MSNylon_absorption[ji] = 100*m; //

 }
 nylonTable->AddProperty("RINDEX",LAr_SCPPMSNylon,MSNylonRefraction,numMSNylon);
 nylonTable->AddProperty("ABSLENGTH",LAr_SCPPMSNylon,MSNylon_absorption,numMSNylon);
 fNylon->SetMaterialPropertiesTable(nylonTable);
 
 MGLog(debugging) << "Constructed Nylon Optical Properties"<<endlog;
}

//stolen from gerdageometry/src/GEGeometryLArInstrumentation.cc for MetalCopper
//TODO Not sure optical proper electroformed copper but for now assuming the same as metal coppper
void MGLGNDOpticalMaterialProperties::Register_Copper_Properties()
{
     // Attach properties to the other materials
   // -- copper
   //
  char* path = getenv("MGGENERATORDATA");
  if (!path)
  {
    MGLog(warning)<< "MGGENERATORDATA environment variable not set! Assuming local input (./)."
                  << "You find the files in $MAGE/generators/data. Set MGGENERATORDATA to point there." << endlog;
    path = (char*) ".";
  }
  G4String pathString(path);

  G4MaterialPropertiesTable *cuOptTable = new G4MaterialPropertiesTable();
  G4String pathFile = pathString + "/Reflectivity_Cu.dat";
  TGraph *cuRefl = new TGraph(pathFile.data(),"%lg,%lg,%*lg");

  G4double *ReflectivityCu = new G4double[cuRefl->GetN()];
  G4double *Wavelength = new G4double[cuRefl->GetN()];
  G4double *PhotonEnergy = new G4double[cuRefl->GetN()];

  G4double metals_refl_scale = 1.0;

  for (int ji=0;ji < cuRefl->GetN(); ++ji)
  {
    Wavelength[ji] = (cuRefl->GetX())[ji];
    PhotonEnergy[ji] = LambdaE/(Wavelength[ji]*nm);
    ReflectivityCu[ji] = (cuRefl->GetY())[ji]*metals_refl_scale;
    /** Uncomment for debugging purposes
      MGLog(debugging) << "Lambda "
      << (LambdaE/PhotonEnergy[ji])/nm
      << " nm  Energy " << PhotonEnergy[ji]/eV << " eV : Refl[Cu] = "
      << ReflectivityCu[ji]<<endlog;
      */
  }
  //https://refractiveindex.info/?shelf=main&book=Cu&page=Johnson 
  pathFile = pathString + "/RIndexReal_Cu.dat";
  TGraph *cuRIndexReal = new TGraph(pathFile.data(),"%lg,%lg,%*lg");  
  G4double *RIndexRealCu = new G4double[cuRIndexReal->GetN()];
  G4double *waveIndexRealCu = new G4double[cuRIndexReal->GetN()];
  G4double *nrgIndexRealCu = new G4double[cuRIndexReal->GetN()];
  
  pathFile = pathString + "/RIndexImag_Cu.dat";
  TGraph *cuRIndexImag = new TGraph(pathFile.data(),"%lg,%lg,%*lg");
  G4double *RIndexImagCu = new G4double[cuRIndexImag->GetN()];
  G4double *waveIndexImagCu = new G4double[cuRIndexImag->GetN()];
  G4double *nrgIndexImagCu = new G4double[cuRIndexReal->GetN()];

  G4int NpointsReal =  cuRIndexReal->GetN(); 
  G4int NpointsImag =  cuRIndexImag->GetN(); 
  if(NpointsReal != NpointsImag){
    G4cout<<"RIndexReal_Cu.dat has different number of points than RIndexImag_Cu.dat"<<G4endl;
    MGLog(fatal)<<endlog;
  }
  G4double *RIndexCu = new G4double[NpointsReal];
  G4double *absLengthCu = new G4double[NpointsReal];
  for(int i = 0; i < cuRIndexReal->GetN();i++){
    
    waveIndexRealCu[i] = (cuRIndexReal->GetX())[i];
    waveIndexImagCu[i] = (cuRIndexImag->GetX())[i];
    
    nrgIndexRealCu[NpointsReal-i-1] = LambdaE/(waveIndexRealCu[i]*nm);
    nrgIndexImagCu[NpointsImag-i-1] = LambdaE/(waveIndexImagCu[i]*nm);
    
    RIndexCu[i] = sqrt( (cuRIndexReal->GetY())[i]*(cuRIndexReal->GetY())[i] +(cuRIndexImag->GetY())[i]*(cuRIndexImag->GetY())[i] );
    absLengthCu[i] = waveIndexRealCu[i]/(4*pi*(cuRIndexImag->GetY())[i]);//skin depth 
  }

//  cuOptTable->AddProperty("REFLECTIVITY",PhotonEnergy,ReflectivityCu,cuRefl->GetN());
  cuOptTable->AddProperty("RINDEX",   nrgIndexRealCu,RIndexCu,cuRIndexReal->GetN());
  cuOptTable->AddProperty("ABSLENGTH",nrgIndexRealCu,absLengthCu,cuRIndexReal->GetN());
//  cuOptTable->DumpTable();
  fCopperEF = G4Material::GetMaterial("Copper-EF",true);
  fCopperEF->SetMaterialPropertiesTable(cuOptTable);
  MGLog(debugging) << "Constructed Copper-EF Optical Properties"<<endlog;
}

void MGLGNDOpticalMaterialProperties::Register_Germanium_Properties()
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

  G4MaterialPropertiesTable *geOptTable = new G4MaterialPropertiesTable();
  G4String pathFile = pathString + "/Reflectivity_Ge.dat";
  TGraph *geRefl = new TGraph(pathFile.data(),"%lg,%lg,%*lg");

	 G4double *ReflectivityGe = new G4double[geRefl->GetN()];
	 G4double *Wavelength = new G4double[geRefl->GetN()];
	 G4double *PhotonEnergy = new G4double[geRefl->GetN()];
   G4double metals_refl_scale = 1.0;//fDetectorDB->GetLArInstMetalsReflScale();
    if (metals_refl_scale != 1.0) {
      MGLog(routine) << "Scaling reflectivity of metals (Ge, Si, Cu) by a factor of " << metals_refl_scale << endlog;
    }

	    for (int ji=0;ji < geRefl->GetN(); ++ji)
	    {
	    	Wavelength[ji] = (geRefl->GetX())[ji];
	    	PhotonEnergy[geRefl->GetN() - ji - 1] = LambdaE/(Wavelength[ji]*nm);
	    	ReflectivityGe[ji] = (geRefl->GetY())[ji]*metals_refl_scale;
			/* Uncomment for debugging purposes
	    	MGLog(debugging) << "Lambda "
					 << (LambdaE/PhotonEnergy[ji])/nm
					 << " nm  Energy " << PhotonEnergy[ji]/eV << " eV : Refl[Ge] = "
					 << ReflectivityGe[ji]<<endlog;
			 */
	    }
      //https://refractiveindex.info/?shelf=main&book=Ge&page=Aspnes for 200-650 nm
      //TODO made a "edge-a-ma-cated" guess for n and r at 115 nm
      pathFile = pathString + "/RIndexReal_Ge.dat";
      TGraph *geRIndexReal = new TGraph(pathFile.data(),"%lg,%lg,%*lg");  
      G4double *RIndexRealGe = new G4double[geRIndexReal->GetN()];
      G4double *waveIndexRealGe = new G4double[geRIndexReal->GetN()];
      G4double *nrgIndexRealGe = new G4double[geRIndexReal->GetN()];

      pathFile = pathString + "/RIndexImag_Ge.dat";
      TGraph *geRIndexImag = new TGraph(pathFile.data(),"%lg,%lg,%*lg");
      G4double *RIndexImagGe = new G4double[geRIndexImag->GetN()];
      G4double *waveIndexImagGe = new G4double[geRIndexImag->GetN()];
      G4double *nrgIndexImagGe = new G4double[geRIndexReal->GetN()];

      G4int NpointsReal =  geRIndexReal->GetN(); 
      G4int NpointsImag =  geRIndexImag->GetN(); 
      if(NpointsReal != NpointsImag){
        G4cout<<"RIndexReal_Ge.dat has different number of points than RIndexImag_Ge.dat"<<G4endl;
        MGLog(fatal)<<endlog;
      }
      G4double *RIndexGe = new G4double[NpointsReal];
      G4double *absLengthGe = new G4double[NpointsReal];
      for(int i = 0; i < geRIndexReal->GetN();i++){

        waveIndexRealGe[i] = (geRIndexReal->GetX())[i];
        waveIndexImagGe[i] = (geRIndexImag->GetX())[i];

        nrgIndexRealGe[NpointsReal-i-1] = LambdaE/(waveIndexRealGe[i]*nm);
        nrgIndexImagGe[NpointsImag-i-1] = LambdaE/(waveIndexImagGe[i]*nm);
    
        RIndexGe[i] = sqrt( (geRIndexReal->GetY())[i]*(geRIndexReal->GetY())[i] +(geRIndexImag->GetY())[i]*(geRIndexImag->GetY())[i] );
        absLengthGe[i] = waveIndexRealGe[i]/(4*pi*(geRIndexImag->GetY())[i]);//skin depth
      }
      //  geOptTable->AddProperty("REFLECTIVITY",PhotonEnergy,ReflectivityGe,geRefl->GetN());
      geOptTable->AddProperty("RINDEX",   nrgIndexRealGe,RIndexGe,geRIndexReal->GetN());
      geOptTable->AddProperty("ABSLENGTH",nrgIndexRealGe,absLengthGe,geRIndexReal->GetN());

      //LGND naming scheme
	    G4Material *germanium = G4Material::GetMaterial("Germanium-Enr");
	    germanium->SetMaterialPropertiesTable(geOptTable);
	    germanium = G4Material::GetMaterial("Germanium-Nat");
	    germanium->SetMaterialPropertiesTable(geOptTable);
}

void MGLGNDOpticalMaterialProperties::Register_Silicon_Properties()
{
 // Attach properties to the other materials
   // -- Silicon
   //
  char* path = getenv("MGGENERATORDATA");
  if (!path)
  {
    MGLog(warning)<< "MGGENERATORDATA environment variable not set! Assuming local input (./)."
                  << "You find the files in $MAGE/generators/data. Set MGGENERATORDATA to point there." << endlog;
    path = (char*) ".";
  }
  G4String pathString(path);

  G4MaterialPropertiesTable *siOptTable = new G4MaterialPropertiesTable();
  G4String pathFile = pathString + "/Reflectivity_Si.dat";
	 TGraph *siRefl = new TGraph(pathFile.data(),"%lg,%lg,%*lg");

	 G4double *ReflectivitySi = new G4double[siRefl->GetN()];
	 G4double *Wavelength = new G4double[siRefl->GetN()];
	 G4double *PhotonEnergy = new G4double[siRefl->GetN()];;
	 for (int ji=0;ji < siRefl->GetN(); ++ji)
   {
     Wavelength[ji] = (siRefl->GetX())[ji];
     PhotonEnergy[siRefl->GetN()- ji - 1] = LambdaE/(Wavelength[ji]*nm);
     ReflectivitySi[ji] = (siRefl->GetY())[ji];
     /**  Uncomment for debugging purposes
       MGLog(debugging) << "Lambda "
       << (LambdaE/PhotonEnergy[ji])/nm
       << " nm  Energy " << PhotonEnergy[ji]/eV << " eV : Refl[Si] = "
       << ReflectivitySi[ji]<<endlog;
       */
   }
   //https://refractiveindex.info/?shelf=main&book=Si&page=Pierce for 100-200 nm
   //https://refractiveindex.info/?shelf=main&book=Si&page=Aspnes 200-650 nm
   pathFile = pathString + "/RIndexReal_Si.dat";
   TGraph *siRIndexReal = new TGraph(pathFile.data(),"%lg,%lg,%*lg");  
   G4double *RIndexRealSi = new G4double[siRIndexReal->GetN()];
   G4double *waveIndexRealSi = new G4double[siRIndexReal->GetN()];
   G4double *nrgIndexRealSi = new G4double[siRIndexReal->GetN()];
  
   pathFile = pathString + "/RIndexImag_Si.dat";
   TGraph *siRIndexImag = new TGraph(pathFile.data(),"%lg,%lg,%*lg");
   G4double *RIndexImagSi = new G4double[siRIndexImag->GetN()];
   G4double *waveIndexImagSi = new G4double[siRIndexImag->GetN()];
   G4double *nrgIndexImagSi = new G4double[siRIndexReal->GetN()];

   G4int NpointsReal =  siRIndexReal->GetN(); 
   G4int NpointsImag =  siRIndexImag->GetN(); 
   if(NpointsReal != NpointsImag){
     G4cout<<"RIndexReal_Si.dat has different number of points than RIndexImag_Si.dat"<<G4endl;
     MGLog(fatal)<<endlog;
   }
   G4double *RIndexSi = new G4double[NpointsReal];
   G4double *absLengthSi = new G4double[NpointsReal];
   for(int i = 0; i < siRIndexReal->GetN();i++){
    
     waveIndexRealSi[i] = (siRIndexReal->GetX())[i];
     waveIndexImagSi[i] = (siRIndexImag->GetX())[i];

     nrgIndexRealSi[NpointsReal-i-1] = LambdaE/(waveIndexRealSi[i]*nm);
     nrgIndexImagSi[NpointsImag-i-1] = LambdaE/(waveIndexImagSi[i]*nm);

     RIndexSi[i] = sqrt( (siRIndexReal->GetY())[i]*(siRIndexReal->GetY())[i] +(siRIndexImag->GetY())[i]*(siRIndexImag->GetY())[i] );
     absLengthSi[i] = waveIndexRealSi[i]/(4*pi*(siRIndexImag->GetY())[i]);//skin depth
   }

	 //siOptTable->AddProperty("REFLECTIVITY",PhotonEnergy,ReflectivitySi,siRefl->GetN());
   
   siOptTable->AddProperty("RINDEX",nrgIndexRealSi,RIndexSi,NpointsReal);
   siOptTable->AddProperty("ABSLENGTH",nrgIndexRealSi,absLengthSi,NpointsReal);

	 G4Material *silicon = G4Material::GetMaterial("Silicon");
	 silicon->SetMaterialPropertiesTable(siOptTable);
}

void MGLGNDOpticalMaterialProperties::Register_Teflon_Properties()
{
// Attach properties to the other materials
   // --Teflon 
   //
  char* path = getenv("MGGENERATORDATA");
  if (!path)
  {
    MGLog(warning)<< "MGGENERATORDATA environment variable not set! Assuming local input (./)."
                  << "You find the files in $MAGE/generators/data. Set MGGENERATORDATA to point there." << endlog;
    path = (char*) ".";
  }
  G4String pathString(path);

  G4MaterialPropertiesTable *tefOptTable = new G4MaterialPropertiesTable();
  G4String pathFile = pathString + "/Reflectivity_Teflon.dat";
   TGraph *tefRefl = new TGraph(pathFile.data(),"%lg,%lg,%*lg");

   G4double *ReflectivityTef = new G4double[tefRefl->GetN()];
   G4double *Wavelength = new G4double[tefRefl->GetN()];
   G4double *PhotonEnergy = new G4double[tefRefl->GetN()];;
   for (int ji=0;ji < tefRefl->GetN(); ++ji)
      {
        Wavelength[ji] = (tefRefl->GetX())[ji];
        PhotonEnergy[ji] = LambdaE/(Wavelength[ji]*nm);
        ReflectivityTef[ji] = (tefRefl->GetY())[ji];
      }
   //J. Micro/Nanolith. MEMS MOEMS 7(3), 033010 (Jul-Sep 2008)
   const G4int nIndex = 10;
   G4double IndexRefraction[nIndex] = {1.38,1.42,1.34,1.31,1.30,1.29,1.28,1.28,1.28,1.27};
   G4double IndexWavelength[nIndex] = {100,147,153,173,210,261,336,428,591,826};
   G4double IndexEnergy[nIndex];
   for(int i = 0; i<nIndex;i++) IndexEnergy[nIndex - i - 1] = LambdaE/(IndexWavelength[i]*nm);
   tefOptTable->AddProperty("REFLECTIVITY",PhotonEnergy,ReflectivityTef,tefRefl->GetN());
   tefOptTable->AddProperty("RINDEX",IndexEnergy,IndexRefraction,nIndex);
   G4Material *teflon = G4Material::GetMaterial("Teflon");
   teflon->SetMaterialPropertiesTable(tefOptTable);
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
  G4double *RIndexRealSS = new G4double[ssRIndexReal->GetN()];
  G4double *waveIndexRealSS = new G4double[ssRIndexReal->GetN()];
  G4double *nrgIndexRealSS = new G4double[ssRIndexReal->GetN()];

  pathFile = pathString + "/RIndexImag_SS.dat";
  TGraph *ssRIndexImag = new TGraph(pathFile.data(),"%lg,%lg,%*lg");
  G4double *RIndexImagSS = new G4double[ssRIndexImag->GetN()];
  G4double *waveIndexImagSS = new G4double[ssRIndexImag->GetN()];
  G4double *nrgIndexImagSS = new G4double[ssRIndexReal->GetN()];

  G4int NpointsReal =  ssRIndexReal->GetN(); 
  G4int NpointsImag =  ssRIndexImag->GetN(); 
  if(NpointsReal != NpointsImag){
    G4cout<<"RIndexReal_SS.dat has different number of points than RIndexImag_SS.dat"<<G4endl;
    MGLog(fatal)<<endlog;
  }
  if( NpointsImag == 0 || NpointsReal == 0){
    G4cout<<"RIndexReal_SS.dat or RIndexImag_SS has zero points"<<G4endl;
    G4cout<<pathFile<<G4endl;
    MGLog(fatal)<<endlog;
  }
  G4double *RIndexSS = new G4double[NpointsReal];
  G4double *absLengthSS = new G4double[NpointsReal];
  for(int i = 0; i < ssRIndexReal->GetN();i++){

    waveIndexRealSS[i] = (ssRIndexReal->GetX())[i];
    waveIndexImagSS[i] = (ssRIndexImag->GetX())[i];

    nrgIndexRealSS[NpointsReal-i-1] = LambdaE/(waveIndexRealSS[i]*nm);
    nrgIndexImagSS[NpointsImag-i-1] = LambdaE/(waveIndexImagSS[i]*nm);

    RIndexSS[i] = sqrt( (ssRIndexReal->GetY())[i]*(ssRIndexReal->GetY())[i] +(ssRIndexImag->GetY())[i]*(ssRIndexImag->GetY())[i] );
    absLengthSS[i] = waveIndexRealSS[i]/(4*pi*(ssRIndexImag->GetY())[i]);//skin depth 
  }


  ssOptTable->AddProperty("RINDEX",nrgIndexRealSS,RIndexSS,NpointsReal);
  ssOptTable->AddProperty("ABSLENGTH",nrgIndexRealSS,absLengthSS,NpointsReal);

  G4Material *ssteel = G4Material::GetMaterial("Steel");
  ssteel->SetMaterialPropertiesTable(ssOptTable);
}
