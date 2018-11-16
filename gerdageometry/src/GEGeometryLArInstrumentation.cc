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
//
// CLASS IMPLEMENTATION:  GEGeometryLArInstrumentation.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * - Base class to build the LAr Instrumentation. This class sets the optical properties of
 * the common parts of the detector such as Ge crystals, LAr, etc.
 *
 * - The optical properties currently defined  (initial revision) have to be reviewed.
 *   Present values were shamelessly imported from MPIKLArGe implementation.
 *
 * - To implement a new LAr Instrumentation design the only change in this class
 *   should be in the switch at the beginning of ConstructLArInstrumentation()
 *
 * - TODO list: 1. Review the material optical properties
 * 		2. Add a messenger to tune up the optical properties.
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Nuno Barros
 * CONTACT: nuno *dot* barros *at* tu-dresden *dot* de
 * FIRST SUBMISSION: 11-04-2011
 *
 * REVISION:
 * 05-23-2012, Nuno  , Adapted to use a single, dedicated hit collection class.
 * 					   Added entry for the fiber instrumentation.
 * 05-29-2012, Nuno	 , Added optical properties measured in HD. The input is from input text files.
 * 					   Corrected light yield and time constant to values measured by M. Heisel.
 * 09-04-2012, Nuno	, Corrected problem with attaching different logical volumes to the collections.
 * 09-24-2012, Nuno , Pre-registered the sensitive volumes in the sensitive detector class to avoid change of part number between simulations.
 * 10-01-2012, Nuno , Added Roughness to the metallic surfaces.
 * 26-10-2012, Nuno , Apply reflectivity to the MS (copper reflectivity).
 * 27-10-2012, Nuno,  Apply a scale to the XUV argon absorption length
 *                    Apply a scale to the reflectivities of metals
 * 04-10-2016, Janina, Added holes for cables in mini-shroud coating.
 * 04-25-2016, Janina, Adapted mini-shroud coating to more-detailed mini-shroud implementation for phase2.
 *		       Attached optical properties to the three logical volumes of the more-detailed mini-shroud implementation.
 *
 * 03-05-2016, Katharina, commented automatic activation of optical processes because of high computing time
 * 12-05-2016, JJ added macro command to turn off photon tracking, fixing the hack from above
 * 28-07-2016, JJ added macro command to set LAr light yield (Bjoern's code)
 * 21-11-2017, KvSturm, add optical properties to glued MS rings
 */
//---------------------------------------------------------------------------//


#include "gerdageometry/GEGeometryLArInstrumentation.hh"
#include "gerdageometry/GEGeometryDetectorDB.hh"
#include "gerdageometry/GEGeometryLArInstTest.hh"
#include "gerdageometry/GEGeometryLArInstPMT.hh"
#include "gerdageometry/GEGeometryLArInstFibers.hh"
#include "gerdageometry/GEGeometryLArInstHybrid.hh"
#include "gerdageometry/GEGeometryLArInstAPD.hh"
#include "io/MGLogger.hh"
#include "management/MGManager.hh"
#include "processes/MGProcessesList.hh"
// We better use the same type of hit collation as PMTs
#include "gerdageometry/GEGeometryLArInstSD.hh"
#include "gerdageometry/GEGeometryDetectorParameters.hh"
#include "gerdageometry/GEGeometryHolderPhase1Parts.hh"
#include "gerdageometry/GEGeometryHolderPhase2Parts.hh"
#include "gerdageometry/GEGeometryCryostatParts.hh"

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
const G4double GEGeometryLArInstrumentation::LambdaE = twopi *1.973269602e-16 * m * GeV;

GEGeometryLArInstrumentation::GEGeometryLArInstrumentation(GEGeometryDetectorDB* theGeometryDB)
: successfulInitialization(false),//fInstrumentation(0),
  fMotherLogical(0),fMotherPhysical(0),
  fCuOptSurface(0), fSiOptSurface(0), fTefOptSurface(0),fVM2000OptSurface(0),fWLSoptSurf(0)
{
	MGLog(trace) << "Creating an instance of the LArInstrumentation factory." << endlog;
	fDetectorDB = theGeometryDB;

	fMotherLogical = fDetectorDB->GetDetectorMotherVolume();
//	// For the rough surfaces we also need the physical volume of the mother
	fMotherPhysical = fDetectorDB->GetCryostatParts()->GetCryoInnerwall_Inside()->GetPhysicalVolume();
	MGLog(debugging) << "Logical and Physical volumes imported. Describing:" << endlog;
	MGLog(debugging) << "LOG : " << fMotherLogical->GetName() << " MAT " << fMotherLogical->GetMaterial()->GetName() << endlog;
	MGLog(debugging) << "PHYS : " << fMotherPhysical->GetName() << " LOG " << fMotherPhysical->GetLogicalVolume()->GetName() << endlog;

	// First thing of all is to activate the optics processes in case they are not yet active.
	// In case they are not yet active

   if (fDetectorDB->GetLArInstrumentationPhotTrack()) {
	// Get the pointer to the run manager (MGManager).
	MGManager *mgMgr = MGManager::GetMGManager();

	MGProcessesList *mgProc = dynamic_cast<MGProcessesList*>(mgMgr->GetMGProcessesList());

	// If the optical processes haven't been activated, activate them now
	// We can't know if they are active. Simply for the reactivation
	// FIXME: Add a better check for optical processes activation.
	MGLog(debugging) << "Forcing activation of optical processes." << endlog;
	mgProc->SetOpticalFlag(true);
	// Feed the process manager the new physics list...again
	mgMgr->SetUserInitialization(mgProc);
        }

	InitializeTPBSpectra();
}

GEGeometryLArInstrumentation::~GEGeometryLArInstrumentation() {
	MGLog(trace) << "Delete the LArInstrumentation factory and propagating for the constructed designs" << endlog;
	for (size_t i = 0; i < fInstrumentation.size(); ++i) {
		delete fInstrumentation[i];
	}
	//if (fInstrumentation) delete fInstrumentation;
}

void GEGeometryLArInstrumentation::ConstructLArInstrumentation() {

	// First build the instrumentation

	switch (fDetectorDB->GetLArInstrumentationDesign()) {
	case LArInstr_test:
		MGLog(trace) << "Building LAr Instrumentation dummy design." << endlog;
		fInstrumentation.push_back( new GEGeometryLArInstTest(fDetectorDB));

		fInstrumentation[fInstrumentation.size() - 1]->ConstructDesign();
		break;
	case LArInstr_PMT:
		MGLog(trace) << "Building LAr Instrumentation simple test PMT design." << endlog;
		fInstrumentation.push_back( new GEGeometryLArInstPMT(fDetectorDB));
		fInstrumentation[fInstrumentation.size() - 1]->ConstructDesign();
		break;
	case LArInstr_fibers:
		MGLog(trace) << "Building LAr Instrumentation fiber design." << endlog;
		fInstrumentation.push_back( new GEGeometryLArInstFibers(fDetectorDB));
		fInstrumentation[fInstrumentation.size() - 1]->ConstructDesign();
		break;
	case LArInstr_hybrid:
		MGLog(trace) << "Building LAr Instrumentation hybrid design." << endlog;
		fInstrumentation.push_back( new GEGeometryLArInstHybrid(fDetectorDB));
		fInstrumentation[fInstrumentation.size() - 1]->ConstructDesign();
		break;
	case LArInstr_APD:
		MGLog(trace) << "Building LAr Instrumentation APD design." << endlog;
		fInstrumentation.push_back( new GEGeometryLArInstAPD(fDetectorDB));
		fInstrumentation[fInstrumentation.size() - 1]->ConstructDesign();
		break;
	case LArInstr_hybridAPD:
		MGLog(trace) << "Building LAr Instrumentation hybrid design with APDs." << endlog;
		fInstrumentation.push_back( new GEGeometryLArInstHybrid(fDetectorDB));
		fInstrumentation[fInstrumentation.size() - 1]->ConstructDesign();
		fInstrumentation.push_back( new GEGeometryLArInstAPD(fDetectorDB));
		fInstrumentation[fInstrumentation.size() - 1]->ConstructDesign();
		break;

	default:
		MGLog(error) << "Failure building concrete instrumentation ( " << fDetectorDB->GetLArInstrumentationDesign()
					<< " )" << endlog;
		break;
	}

	/// Register the sensitive detectors associated with the LAr Instrumentation

	// Here comes a tricky bit. We have to know how many different *logical* volumes we want to use.
	// These represent different types of readouts/logical (sensitive) volumes (fibers and PMTs, for example)
	// we have only one instance of sensitive detector, but this is linked to different logical volumes.

//	for (size_t i = 0; i < fInstrumentation.size(); ++i) {
//		nlogicvols += fInstrumentation[i]->GetNumSDLogicVols();
//	}
	G4int ntot_det = 0; // total number of detectors
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	// Create the sensitive detector class
	GEGeometryLArInstSD *LArInstSD = new GEGeometryLArInstSD("LArInstrumentation","LArInstrumentationHC");
	SDman -> AddNewDetector(LArInstSD);

	// Each logical volume can be then replicated as many times as we want
	// That defines the number of sensitive detector parts
	// Loop over the logical volumes

	for (size_t i = 0; i < fInstrumentation.size(); ++i) {
		G4int nlogicvols = fInstrumentation[i]->GetNumSDLogicVols();
		MGLog(routine) << "Generating " << nlogicvols << " groups of sensitive detectors for LAr Instrumentation entry " << i << "." << endlog;

		for (G4int iv = 0; iv < nlogicvols; ++iv) {
			ntot_det += fInstrumentation[i]->GetNLogicalInstances(iv);
			MGLog(routine) << "Adding LV [ " << fInstrumentation[i]->GetLArInstrSDLogical(iv)->GetName()
			    << " ] to the list of sensitive detectors (" << fInstrumentation[i]->GetNLogicalInstances(iv)
			    << " instances)." << endlog;
		  LArInstSD->RegisterDetectorInstances(fInstrumentation[i]->GetLArInstrSDLogical(iv));
			// Attach the sensitive detector to the logical volumes
			(fInstrumentation[i]->GetLArInstrSDLogical(iv))->SetSensitiveDetector(LArInstSD);

		}
	}
	//MGLog(routine) << "Generating " << nlogicvols << " groups of sensitive detectors for LAr Instrumentation." << endlog;

//	for (G4int iv = 0; iv < nlogicvols; ++iv) {
//		// Register the volumes themselves in the collection.
//		// This avoids the typical trouble of changing part codes
//	}

	// First thing is to register the optical properties of LAr.
	// At this point the rest of the detector is constructed so the LAr
	// already exists in the material table
	RegisterArgonOpticalProperties();


	/// Implementation of the optical surface between the Germanium crystals and the Argon.
	/// All other surfaces not specific to the particular LAr Instrumentation design should be implemented here as well.
	/// eg// 	case LArInstr_test:
// 		MGLog(trace) << "Building LAr Instrumentation dummy design." << endlog;
// 		fInstrumentation = new GEGeometryLArInstTest(fDetectorDB);
// 		fInstrumentation->ConstructDesign();
// 		break;. Radon shroud, minishroud, etc.

	// Define and attach optical properties to the volumes
	// Define the optical surfaces with corresponding reflection and transmissions


	 /*** Optical properties of Germanium
	  * Needs to be attached to all deadlayer logical surfaces.
	  *
	  * Found a reference for reflectivity in Germanium. Need to confirm.
	  * http://prola.aps.org/abstract/PR/v160/i3/p602_1
	  *
	  * It quotes a reflectivity quite high for UV light (~60-70%).
	  */


	 // Load the input data file
	 char* envpath = getenv("MGGENERATORDATA");
	 std::string path = ".";
	 if (!envpath)
	 {
		 MGLog(warning)<< "MGGENERATORDATA environment variable not set! Assuming local input (./)."
                       << "You find the files in $MAGE/generators/data. Set MGGENERATORDATA to point there." << endlog;
	 } else {
		 path = envpath;
	 }
	 G4String pathString(path.c_str());
	 G4String pathFile = pathString + "/Reflectivity_Ge.dat";
	 TGraph *geRefl = new TGraph(pathFile.data(),"%lg,%lg,%*lg");

	 G4double *ReflectivityGe = new G4double[geRefl->GetN()];
	 G4double *Wavelength = new G4double[geRefl->GetN()];
	 G4double *PhotonEnergy = new G4double[geRefl->GetN()];
   G4double metals_refl_scale = fDetectorDB->GetLArInstMetalsReflScale();
    if (metals_refl_scale != 1.0) {
      MGLog(routine) << "Scaling reflectivity of metals (Ge, Si, Cu) by a factor of " << metals_refl_scale << endlog;
    }

	    for (int ji=0;ji < geRefl->GetN(); ++ji)
	    {
	    	Wavelength[ji] = (geRefl->GetX())[ji];
	    	PhotonEnergy[ji] = LambdaE/(Wavelength[ji]*nm);
	    	ReflectivityGe[ji] = (geRefl->GetY())[ji]*metals_refl_scale;
			/* Uncomment for debugging purposes
	    	MGLog(debugging) << "Lambda "
					 << (LambdaE/PhotonEnergy[ji])/nm
					 << " nm  Energy " << PhotonEnergy[ji]/eV << " eV : Refl[Ge] = "
					 << ReflectivityGe[ji]<<endlog;
			 */
	    }

	    G4MaterialPropertiesTable *germOptTable = new G4MaterialPropertiesTable();
	    germOptTable->AddProperty("REFLECTIVITY",PhotonEnergy,ReflectivityGe,geRefl->GetN());

	    G4OpticalSurface* germOptSurface = new G4OpticalSurface("Germ surface");
	    germOptSurface->SetType(dielectric_metal);
	    germOptSurface->SetFinish(groundfrontpainted);
	    //,glisur,polished,dielectric_metal);
	    // Polish of the surface (the smaller the value, the more "lambertian-like" the reflection becomes)
	    germOptSurface->SetPolish(0.5);
	    germOptSurface->SetMaterialPropertiesTable(germOptTable);

	    G4Material *germanium = G4Material::GetMaterial("Germanium/Lithium");
	    germanium->SetMaterialPropertiesTable(germOptTable);
	    germanium = G4Material::GetMaterial("Germanium/Boron");
	    germanium->SetMaterialPropertiesTable(germOptTable);
	    germanium = G4Material::GetMaterial("Germanium/Lithium");
	    germanium->SetMaterialPropertiesTable(germOptTable);
	    germanium = G4Material::GetMaterial("EnrichedGe");
	    germanium->SetMaterialPropertiesTable(germOptTable);

	    G4int ngedet = fDetectorDB->GetCrystalNumber();
	 for (int i = 0; i < ngedet ; ++i) {
		 	G4LogicalVolume *dl_log = fDetectorDB->GetCrystalDeadLayerLogical(i);
		 	std::ostringstream oss;
		 	oss << "germanium_surf_" << i;
		 	new G4LogicalSkinSurface(oss.str(),dl_log,germOptSurface);
	 }

	// Below this point the code was shamelessly copied from MPIKLArGe

	 // Attach properties to the other materials
	 // -- copper
	 G4MaterialPropertiesTable *cuOptTable = new G4MaterialPropertiesTable();
	 pathFile = pathString + "/Reflectivity_Cu.dat";
	 TGraph *cuRefl = new TGraph(pathFile.data(),"%lg,%lg,%*lg");

	 G4double *ReflectivityCu = new G4double[cuRefl->GetN()];
	 Wavelength = new G4double[cuRefl->GetN()];
	 PhotonEnergy = new G4double[cuRefl->GetN()];;
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


	 cuOptTable->AddProperty("REFLECTIVITY",PhotonEnergy,ReflectivityCu,cuRefl->GetN());
	 G4Material *copper = G4Material::GetMaterial("MetalCopper");
	 copper->SetMaterialPropertiesTable(cuOptTable);
	 // Since we don't have a refractive index, this alone is not enough. An optical surface has to be specified
	 // for each part
	 // In this case, what matters is the different parts of the holders. Have to check over all of them
	 fCuOptSurface = new G4OpticalSurface("Cu surface");
	 fCuOptSurface->SetType(dielectric_metal);
	 fCuOptSurface->SetFinish(ground);
	 // Reflection of metallic surfaces has a very small polish
	 fCuOptSurface->SetPolish(0.5);

	 //,glisur,polished,dielectric_metal);
	 fCuOptSurface->SetMaterialPropertiesTable(cuOptTable);





	 // -- Silicon
	 G4MaterialPropertiesTable *siOptTable = new G4MaterialPropertiesTable();
	 pathFile = pathString + "/Reflectivity_Si.dat";
	 TGraph *siRefl = new TGraph(pathFile.data(),"%lg,%lg,%*lg");

	 G4double *ReflectivitySi = new G4double[siRefl->GetN()];
	 Wavelength = new G4double[siRefl->GetN()];
	 PhotonEnergy = new G4double[siRefl->GetN()];;
	 for (int ji=0;ji < siRefl->GetN(); ++ji)
	    {
	    	Wavelength[ji] = (siRefl->GetX())[ji];
	    	PhotonEnergy[ji] = LambdaE/(Wavelength[ji]*nm);
	    	ReflectivitySi[ji] = (siRefl->GetY())[ji]*metals_refl_scale;
			/**  Uncomment for debugging purposes
	    	MGLog(debugging) << "Lambda "
					 << (LambdaE/PhotonEnergy[ji])/nm
					 << " nm  Energy " << PhotonEnergy[ji]/eV << " eV : Refl[Si] = "
					 << ReflectivitySi[ji]<<endlog;
			 */
	    }


	 siOptTable->AddProperty("REFLECTIVITY",PhotonEnergy,ReflectivitySi,siRefl->GetN());
	 G4Material *silicon = G4Material::GetMaterial("MetalSilicon");
	 silicon->SetMaterialPropertiesTable(siOptTable);

	 fSiOptSurface = new G4OpticalSurface("Si surface");//,glisur,polished,dielectric_metal);
   fSiOptSurface->SetType(dielectric_metal);
   fSiOptSurface->SetFinish(ground);
   fSiOptSurface->SetPolish(0.7);
   fSiOptSurface->SetMaterialPropertiesTable(siOptTable);


	 // -- Teflon
	 G4MaterialPropertiesTable *tefOptTable = new G4MaterialPropertiesTable();
	 pathFile = pathString + "/Reflectivity_Teflon.dat";
	 TGraph *tefRefl = new TGraph(pathFile.data(),"%lg,%lg,%*lg");

	 G4double *ReflectivityTef = new G4double[tefRefl->GetN()];
	 Wavelength = new G4double[tefRefl->GetN()];
	 PhotonEnergy = new G4double[tefRefl->GetN()];;
	 for (int ji=0;ji < tefRefl->GetN(); ++ji)
	    {
	    	Wavelength[ji] = (tefRefl->GetX())[ji];
	    	PhotonEnergy[ji] = LambdaE/(Wavelength[ji]*nm);
	    	ReflectivityTef[ji] = (tefRefl->GetY())[ji];
	    	/** Uncomment for debugging purposes
	    	MGLog(debugging) << "Lambda "
					 << (LambdaE/PhotonEnergy[ji])/nm
					 << " nm  Energy " << PhotonEnergy[ji]/eV << " eV : Refl[Teflon] = "
					 << ReflectivityTef[ji]<<endlog;
	    	 */
	    }


	 tefOptTable->AddProperty("REFLECTIVITY",PhotonEnergy,ReflectivityTef,tefRefl->GetN());
	 G4Material *teflon = G4Material::GetMaterial("Teflon");
	 teflon->SetMaterialPropertiesTable(tefOptTable);

   fTefOptSurface = new G4OpticalSurface("Teflon surface");//,glisur,polished,dielectric_metal);
   fTefOptSurface->SetType(dielectric_metal);
   fTefOptSurface->SetFinish(ground);

   fTefOptSurface->SetMaterialPropertiesTable(tefOptTable);



	 // -- VM2000
   	 //TODO APD: add optical properties of VM2000

   // These optical properties are directly copied from GEGeometryLArInstrHybrid.cc.
   // They are needed here for the reflectivity of the VM2000 coated MS in the APD design

   // Build table with photon energies
   const G4int numVM2000 = 500;

   G4double PPSCHighEVM2000 = LambdaE /(115*nanometer);
   G4double PPSCLowEVM2000 = LambdaE /(650*nanometer); //598
   G4double deeVM2000 = ((PPSCHighEVM2000 - PPSCLowEVM2000) / ((G4double)(numVM2000-1)));
   G4double LAr_SCPPVM2000[numVM2000];
   for (G4int ji = 0; ji < numVM2000; ji++)
   {
	 G4double ee=PPSCLowEVM2000+ ((G4double)ji) * deeVM2000;
     LAr_SCPPVM2000[ji]=ee;
   }


   //From Dario's thesis, reflectivity of VM2000 is ~98% above 370 nm
   //and ~15% below it (almost a step-function)

   G4double ReflectivityVM2000[numVM2000];
   //G4double Efficiency[numVM2000];

   for (G4int ji=0;ji < numVM2000; ji++)
   {
     if (LAr_SCPPVM2000[ji] < (LambdaE/(370*nanometer)))
       ReflectivityVM2000[ji] = 0.98; //visible
     else
       ReflectivityVM2000[ji] = 0.15; //UV

     //Efficiency[ji] = 0.0;
   }
   G4MaterialPropertiesTable *vmOpTable = new G4MaterialPropertiesTable();
   vmOpTable->AddProperty("REFLECTIVITY",LAr_SCPPVM2000,ReflectivityVM2000,numVM2000);

   fVM2000OptSurface = new G4OpticalSurface("VM_surface");

   fVM2000OptSurface->SetType(dielectric_dielectric);
   // The WLS is on a separate volume.
   // The reflection should be either spike or Lobe.
   fVM2000OptSurface->SetFinish(polishedfrontpainted);
   fVM2000OptSurface->SetMaterialPropertiesTable(vmOpTable);


	 // -- WLS: TPB (Tetraphenyl butadiene)
   // These optical properties are directly copied from GEGeometryLArInstrHybrid.cc.
   // They are needed here for the reflectivity of the VM2000 coated MS in the APD design

   	 //TODO APD: add optical properties of TPB

   fTPB = G4Material::GetMaterial("TPB", false);
   fTPBOnNylon = G4Material::GetMaterial("TPB", false);

   if (fTPB == 0) {
     G4NistManager* nist = G4NistManager::Instance();

     G4Element* H = nist->FindOrBuildElement("H");
     G4Element* C = nist->FindOrBuildElement("C");

     fTPB= new G4Material("TPB", 1*g/cm3, 2, kStateSolid);
     fTPB->AddElement(H, 22);
     fTPB->AddElement(C, 28);

     fTPBOnNylon= new G4Material("TPBOnNylon", 1*g/cm3, 2, kStateSolid);
     fTPBOnNylon->AddElement(H, 22);
     fTPBOnNylon->AddElement(C, 28);

   }



   // Now attach the optical properties to it.
   // Build table with photon energies
   const G4int numTPB = 500;

   G4double PPSCHighETPB = LambdaE /(115*nanometer);
   G4double PPSCLowETPB = LambdaE /(650*nanometer); //598
   G4double deeTPB = ((PPSCHighETPB - PPSCLowETPB) / ((G4double)(numTPB-1)));
   G4double LAr_SCPPTPB[numTPB];
   for (G4int ji = 0; ji < numTPB; ji++)
   {
	 G4double ee=PPSCLowETPB+ ((G4double)ji) * deeTPB;
     LAr_SCPPTPB[ji]=ee;
   }
   G4double WLS_absorption[numTPB];
   G4double WLS_emission[numTPB];
   G4double Refraction[numTPB];
   G4MaterialPropertiesTable *tpbTable = new G4MaterialPropertiesTable();
   G4MaterialPropertiesTable *tpbOnNylonTable = new G4MaterialPropertiesTable();

   // The refractive index of the TPB should be roughly the same as acrylic
   // The polystirene has very similar properties
   // Use the same value as the fibers
   for (G4int ji=0;ji < numTPB; ji++)
   {
     Refraction[ji] = 1.6; //this is just a guess
     // Should the TPB shift the Cherenkov light?
     // This makes a tail starting at 128 until the visible.
     if (LAr_SCPPTPB[ji] > 3.31*eV)
       // For the moment set it to always absorb photons
       WLS_absorption[ji] = 0.001*nm; //absorbs UV (always)
     else
       WLS_absorption[ji] = 10.5*m; //otherwise transparent

     WLS_emission[ji] = TPBEmissionSpectrum(LAr_SCPPTPB[ji]);
     /*
 		MGLog(debugging) << "TPB:  WL: " << (LambdaE/LAr_SCPP[ji])/nanometer<< " nm Energy :" << LAr_SCPP[ji]/eV << " eV Emission : " <<
 				WLS_emission[ji] << " Absorption : " << WLS_absorption[ji]/m << " m."<< endlog;
      */
   }
   tpbTable->AddProperty("RINDEX",LAr_SCPPTPB,Refraction,numTPB);
   tpbTable->AddProperty("WLSABSLENGTH",LAr_SCPPTPB,WLS_absorption,numTPB);
   tpbTable->AddProperty("WLSCOMPONENT",LAr_SCPPTPB,WLS_emission,numTPB);
   // From WArP
   tpbTable->AddConstProperty("WLSTIMECONSTANT", 0.01*ns);
   G4double WLSyield = 1.2;
   tpbTable->AddConstProperty("WLSMEANNUMBERPHOTONS",WLSyield);
   fTPB->SetMaterialPropertiesTable(tpbTable);

   // For 30% TPB 70% PS the emission spectrum is close enough to pure TPB but yield is reduced by 30 % [PC Alexey]
   tpbOnNylonTable->AddProperty("RINDEX",LAr_SCPPTPB,Refraction,numTPB);
   tpbOnNylonTable->AddProperty("WLSABSLENGTH",LAr_SCPPTPB,WLS_absorption,numTPB);
   tpbOnNylonTable->AddProperty("WLSCOMPONENT",LAr_SCPPTPB,WLS_emission,numTPB);
   // From WArP
   tpbOnNylonTable->AddConstProperty("WLSTIMECONSTANT", 0.01*ns);
   G4double WLSOnNylonyield = 0.84; // Information by Alexey
   tpbOnNylonTable->AddConstProperty("WLSMEANNUMBERPHOTONS",WLSOnNylonyield);
   fTPBOnNylon->SetMaterialPropertiesTable(tpbOnNylonTable);


   // Define a rough optical surface to be used in the interface between WLS and LAr
   // 50% roughness in the surface
   // This surface will be attached between the WLS and the LAr in all instances
   G4double roughness = 0.5;
   fWLSoptSurf = new G4OpticalSurface("WLS_rough_surf",glisur,ground,dielectric_dielectric,roughness);




	 // -- Nylon:

 G4Material *fMSNylon = G4Material::GetMaterial("Nylon", true);


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
 fMSNylon->SetMaterialPropertiesTable(nylonTable);


 // DEFINITION OF OPTICAL SURFACE IS RANDOM. NOT A GUESS
 // This surface will be attached between the WLS and the LAr in all instances
 fNylonOptSurf = new G4OpticalSurface("Nylon_rough_surf",glisur,ground,dielectric_dielectric,0.5);


   // Now attach surfaces to all parts of the holders. This requires some knowledge of each part

   AttachOpticalSurface(fDetectorDB->GetHolderCopperLogical());
   // Loop over each crystal and check the crystal type to decide the holder parts
   AttachOpticalSurface(fDetectorDB->GetHolderTeflonLogical());
   for (int i = 0; i < ngedet ; ++i) {
     MGLog(debugging) << "Attaching optical surfaces to holders of detector " << i << endlog;
     GEGeometryDetectorParameters* detector = fDetectorDB->GetDetectorParameters(i);
     // This is to avoid problems
     if (!detector) {
       continue;
     }
     G4int det_id = detector->GetDetectorID();
     // phase 1 detectors
     if (detector->GetGeometry() == 1 || detector->GetGeometry() == 4) {
       GEGeometryHolderPhase1Parts* holderPhase1 = fDetectorDB->GetHolderPhase1Parts(det_id);
       // Unfortunately there is no way to get a list of volumes so this has to be done manually
       AttachOpticalSurface(holderPhase1->GetLogicalVol_LowerStar());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_UpperStar());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_starArm());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_VerticalBar());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_LowerContactDisc());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_LowerPTFETube());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_LowerPressurePlate());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_LowerGrubScrew());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_UpperSpecialNut());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_UpperCounterNut());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_UpperPTFERing());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_UpperChineseHat());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_UpperSmall_1_Isolator());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_UpperSmall_2_SiliconGuider());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_UpperSmall_3_Isolator());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_UpperSmall_4_SiliconSpring());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_UpperSmall_5_Isolator());
       AttachOpticalSurface(holderPhase1->GetLogicalVol_UpperSmall_6_InnerSpecialNut());
     } else if (detector->GetGeometry() == 3 || detector->GetGeometry() == 5 || detector->GetGeometry() == 11){
       GEGeometryHolderPhase2Parts* holderPhase2 = fDetectorDB->GetHolderPhase2Parts(det_id);
       // Unfortunately there is no way to get a list of volumes so this has to be done manually
       AttachOpticalSurface(holderPhase2->GetLogicalVol_LowerPlate());
       AttachOpticalSurface(holderPhase2->GetLogicalVol_UpperPlate());
       AttachOpticalSurface(holderPhase2->GetLogicalVol_VerticalBar());
     }
   }


   // Now construct mini-shroud if needed
   if(fDetectorDB->GetFlagConstructMiniShroud()== true)
	   ConstructMiniShroud();

	return;
}

void GEGeometryLArInstrumentation::ConstructMiniShroud()
{

   // Attach optical surfaces to the MS. Since the surface is applied to the logical volume
   // This only becomes effective if the volume is actually placed.
     G4String fMiniShroudMaterial = fDetectorDB->GetMiniShroudMaterial();

  std::vector<G4LogicalVolume*> *ms_array;
  std::vector<G4LogicalVolume*> *mstop_array;
  std::vector<G4LogicalVolume*> *msbottom_array;
  std::vector<G4LogicalVolume*> *mstopring_array;
  std::vector<G4LogicalVolume*> *msbottomring_array;
  std::vector<G4ThreeVector> *ms_positions;
  std::vector<G4ThreeVector> *msbottom_positions;
  std::vector<G4ThreeVector> *mstop_positions;
  std::vector<G4ThreeVector> *mstopring_positions;
  std::vector<G4ThreeVector> *msbottomring_positions;
  std::vector<double> *ms_inDet_height;

  if (fMiniShroudMaterial=="Nylon"){
      ms_array = fDetectorDB->GetMiniShroudPhase2TubLogical();
      msbottom_array = fDetectorDB->GetMiniShroudPhase2BottomLogical();
      mstop_array = fDetectorDB->GetMiniShroudPhase2TopLogical();
      mstopring_array = fDetectorDB->GetMiniShroudPhase2TopGluedRingLogical();
      msbottomring_array = fDetectorDB->GetMiniShroudPhase2BottomGluedRingLogical();

      ms_positions = fDetectorDB->GetMiniShroudPhase2TubPosition();
      msbottom_positions = fDetectorDB->GetMiniShroudPhase2BottomPosition();
      mstop_positions = fDetectorDB->GetMiniShroudPhase2TopPosition();
      mstopring_positions = fDetectorDB->GetMiniShroudPhase2TopGluedRingPosition();
      msbottomring_positions = fDetectorDB->GetMiniShroudPhase2BottomGluedRingPosition();

      ms_inDet_height=fDetectorDB->GetMiniInDetHeightVector();
      for (int ims = 0; ims < (int) ms_array->size(); ++ims) {
	   MGLog(debugging) << "Attaching optical surface to MS logical volume " << ims
			   << " " << ms_array->at(ims)->GetName() <<", "<< msbottom_array->at(ims)->GetName() << " and " << mstop_array->at(ims)->GetName() << endlog;
		AttachOpticalSurface(ms_array->at(ims));
		AttachOpticalSurface(msbottom_array->at(ims));
		AttachOpticalSurface(mstop_array->at(ims));
        AttachOpticalSurface(mstopring_array->at(ims));
        AttachOpticalSurface(msbottomring_array->at(ims));
      }
      }
   else {
     ms_array = fDetectorDB->GetMiniShroudLogical();
     ms_positions = fDetectorDB->GetMiniShroudPosition();
     for (int ims = 0; ims < (int) ms_array->size(); ++ims) {
	   MGLog(debugging) << "Attaching optical surface to MS logical volume " << ims
			   << " " << ms_array->at(ims)->GetName() << endlog;
	        AttachOpticalSurface(ms_array->at(ims));
   }

   }

   std::vector<double> *ms_radius = fDetectorDB->GetMiniShroudRadiusVector();
   std::vector<double> *ms_height = fDetectorDB->GetMiniShroudHeightVector();


   // construct boolean sum volume and replace MS logical volume with boolean volume
   std::vector<G4LogicalVolume*> VM2000Logicals;
   std::vector<G4LogicalVolume*> WLSLogicals;

   G4Material*fMaterialVM2000 = G4Material::GetMaterial("VM2000");

   G4Colour lblue	(51./255.,153./255.,255./255.) ;
   G4Colour llblue	(102./255.,  255./255., 255./255.) ;
   G4VisAttributes* fVM2000VisAtt = new G4VisAttributes(lblue);
   fVM2000VisAtt -> SetVisibility(true);
   fVM2000VisAtt -> SetForceSolid(true);
   G4VisAttributes* fWLSVisAtt = new G4VisAttributes(llblue);
   fWLSVisAtt -> SetVisibility(true);
   fWLSVisAtt -> SetForceSolid(true);

   //Parameters for holes for cables in mini-shroud coatings
   GEGeometryDetectorParameters* detectorparams(0);
   G4int type(0);
   G4int HoleCounter = 0;
   G4double holderPhase2lenghtY=0.0;
   G4int nrOfRows=fDetectorDB->GetNumberOfRows();
   //check if holes are required
   for( G4int ii=0; ii<9; ii++ ){
   	for(G4int kk=0; kk<nrOfRows; kk++ ){
	type = fDetectorDB->GetCrystalType(ii,kk);
	detectorparams = fDetectorDB->GetDetectorParameters(type);
	if (type>0){
	   	if (detectorparams->GetGeometry()==5 || detectorparams->GetGeometry()==11){
	   	HoleCounter++;
		GEGeometryHolderPhase2Parts* holderMSCableholes = fDetectorDB->GetHolderPhase2Parts(type);
		holderPhase2lenghtY=holderMSCableholes->GetHolderPhase2Width();
		//all holders for phase 2 are identically, no matching to position required
		   									   }
	  	   }
	 }
	}

   G4double MiniShroudHoleCoatingCabx= 0.5*(fDetectorDB->GetPhase2Cable_WidthToEPlate());
   G4double MiniShroudHoleCoatingCaby= 0.5*(fDetectorDB->GetPhase2Cable_thickness())*2.*nrOfRows;
   G4double MiniShroudHoleCoatingRadius= 0.5*holderPhase2lenghtY+MiniShroudHoleCoatingCaby;
   G4double MiniShroudHoleCoatingHolderBarRadius=fDetectorDB->GetHolderBarRadiusPhase2();

   // decide upon MS coating messenger to deploy optical volumes or not

   if (fDetectorDB->GetLArInstrumentationMSCoating()==1) // VM2000 + TPB coating on MS
     for (int ims = 0; ims < (int) ms_array->size(); ++ims) {

    	   G4double Shroud_TubsOuterRadius	= ms_radius->at(ims); // pahse i: 51.5 * mm
    	   G4double Shroud_SideThickness	= fDetectorDB->GetMiniShroudSideThickness(); //0.06 * mm;
    	   G4double Shroud_TubsInnerRadius	= Shroud_TubsOuterRadius - Shroud_SideThickness;
    	   G4double Shroud_Height			= ms_height->at(ims); //500.0 * mm;
    	   G4double Shroud_BaseThickness	= fDetectorDB->GetMiniShroudBaseThickness(); //0.12 * mm;


     	   MGLog(debugging) << "MS:"<< ims << endlog;
    	   MGLog(debugging) << "MS outer radius [mm]:     "<< Shroud_TubsOuterRadius/mm << endlog;
    	   MGLog(debugging) << "MS height [mm]:           "<< Shroud_Height/mm << endlog;
    	   MGLog(debugging) << "Constructing optical volumes for MSs "<< endlog;
    	   MGLog(debugging) << "MS side thickness [mm]:   "<< Shroud_SideThickness/mm << endlog;
    	   MGLog(debugging) << "MS base thickness [mm]:   "<< Shroud_BaseThickness/mm << endlog;


    	   G4double VM2000_Thickness		= 0.1*mm;
    	   G4double VM2000_TubsOuterRadius 	= Shroud_TubsInnerRadius;
    	   G4double VM2000_TubsInnerRadius 	= VM2000_TubsOuterRadius - VM2000_Thickness;
    	   G4double VM2000_Height 			= Shroud_Height - 2*Shroud_BaseThickness;

    	   G4double WLS_Thickness			= 5*micrometer; // important for interaction lenght of scintillation. same as in they hybrid;
    	   G4double WLS_TubsOuterRadius   	= VM2000_TubsInnerRadius;
    	   G4double WLS_Height 				= VM2000_Height - 2*VM2000_Thickness;

    	   MGLog(debugging) << "VM2000 outer radius [mm]: "<< VM2000_TubsOuterRadius/mm << endlog;
    	   MGLog(debugging) << "VM2000 thickness [mm]:    "<< VM2000_Thickness/mm << endlog;
    	   MGLog(debugging) << "VM2000 height [mm]:       "<< VM2000_Height/mm << endlog;

    	   MGLog(debugging) << "WLS outer radius [mm]:    "<< WLS_TubsOuterRadius/mm << endlog;
    	   MGLog(debugging) << "WLS thickness [mm]:       "<< WLS_Thickness/mm << endlog;
    	   MGLog(debugging) << "WLS height [mm]:          "<< WLS_Height/mm << endlog;

	   G4double fCableHoleCoatingMiniShroud_angle1=fDetectorDB->GetAngleForPhase2CablePosition(ims);
           G4RotationMatrix* HVCableHoleCoatingMiniShroudRotOpt1 = new G4RotationMatrix();
           HVCableHoleCoatingMiniShroudRotOpt1->rotateZ(fCableHoleCoatingMiniShroud_angle1);
           G4RotationMatrix* SignalCableHoleCoatingMiniShroudRotOpt1 = new G4RotationMatrix();
           SignalCableHoleCoatingMiniShroudRotOpt1->rotateZ(180.*deg+fCableHoleCoatingMiniShroud_angle1);

    if (fMiniShroudMaterial=="Nylon"){
               std::vector<double> *HoleRad = fDetectorDB-> GetMiniShroudHoleRadiusVector();
	       char solidname1[100];
	       char solidname2[100];

               G4double VM2000InDet_Height=ms_inDet_height->at(ims);
               G4Tubs* VM2000InDet_TubsOuter = new G4Tubs("VM2000InDet_TubsOuter",
	     					   0.,
	     					   VM2000_TubsOuterRadius,
	     					   VM2000InDet_Height/ 2.,
	     					   0.*deg,
	     					   360.*deg);

	       G4Tubs* VM2000InDet_TubsInner = new G4Tubs("VM2000InDet_TubsInner",
	     					   0,
	     					   VM2000_TubsOuterRadius - VM2000_Thickness,
	     					   (VM2000InDet_Height-2.*VM2000_Thickness)/ 2.,
	     					   0.*deg,
	     					   360.*deg);

	       G4double VM2000InAboveDet_Height=Shroud_Height-ms_inDet_height->at(ims)-2.*Shroud_BaseThickness;
	       G4Tubs* VM2000InAboveDet_TubsOuter = new G4Tubs("VM2000InAboveDet_TubsOuter",
	     					   0.,
	     					   VM2000_TubsOuterRadius,
	     					   0.5*(VM2000InAboveDet_Height),
	     					   0.*deg,
	     					   360.*deg);

	       G4Tubs* VM2000InAboveDet_TubsInner = new G4Tubs("VM2000InAboveDet_TubsInner",
	     					   0,
	     					   VM2000_TubsOuterRadius - VM2000_Thickness,
	     					   0.5*(VM2000InAboveDet_Height-VM2000_Thickness),
	     					   0.*deg,
	     					   360.*deg);

	       G4double WSLInDet_Height=VM2000InDet_Height-2.*VM2000_Thickness;
               G4Tubs* WLSInDet_TubsOuter = new G4Tubs("WLSInDet_TubsOuter",
	     					   0.,
	     					   WLS_TubsOuterRadius,
	     					   WSLInDet_Height/ 2.,
	     					   0.*deg,
	     					   360.*deg);

	       G4Tubs* WLSInDet_TubsInner = new G4Tubs("WLSInDet_TubsInner",
	     					   0,
	     					   WLS_TubsOuterRadius - WLS_Thickness,
	     					   (WSLInDet_Height-2.*WLS_Thickness)/ 2.,
	     					   0.*deg,
	     					   360.*deg);

	       G4double WSLInAboveDet_Height=VM2000InAboveDet_Height-VM2000_Thickness;
	       G4Tubs* WLSInAboveDet_TubsOuter = new G4Tubs("WLSInAboveDet_TubsOuter",
	     					   0.,
	     					   WLS_TubsOuterRadius,
	     					   0.5*(WSLInAboveDet_Height),
	     					   0.*deg,
	     					   360.*deg);

	       G4Tubs* WLSInAboveDet_TubsInner = new G4Tubs("WLSInAboveDet_TubsInner",
	     					   0,
	     					   WLS_TubsOuterRadius - WLS_Thickness,
	     					   0.5*(WSLInAboveDet_Height-WLS_Thickness),
	     					   0.*deg,
	     					   360.*deg);

               //holes: for signal and HV cable, for the 3 holder bars
	       G4Box* MiniShroudHoleCoatingVM2000In_HVCab = new G4Box("MiniShroudHoleCoatingVM2000In_HVCab",
						 		       MiniShroudHoleCoatingCabx,
						 		       MiniShroudHoleCoatingCaby,
								       0.5*VM2000_Thickness);

	       G4Box* MiniShroudHoleCoatingVM2000In_SignalCab = new G4Box("MiniShroudHoleCoatingVM2000In_SignalHVCab",
						 			MiniShroudHoleCoatingCabx,
						 			MiniShroudHoleCoatingCaby,
									0.5*VM2000_Thickness);
	       G4Tubs* MiniShroudHoleVM2000HolderBars = new G4Tubs("MiniShroudHoleVM2000HolderBars",
	     					   0.,
	     					   MiniShroudHoleCoatingHolderBarRadius,
	     					   0.5*VM2000_Thickness,
	     					   0.*deg,
	     					   360.*deg);

	       G4Box* MiniShroudHoleCoatingWLSIn_HVCab = new G4Box("MiniShroudHoleCoatingWLSIn_HVCab",
						 		       MiniShroudHoleCoatingCabx,
						 		       MiniShroudHoleCoatingCaby,
								       0.5*WLS_Thickness);

	       G4Box* MiniShroudHoleCoatingWLSIn_SignalCab = new G4Box("MiniShroudHoleCoatingWLSIn_SignalHVCab",
						 			MiniShroudHoleCoatingCabx,
						 			MiniShroudHoleCoatingCaby,
									0.5*WLS_Thickness);
	       G4Tubs* MiniShroudHoleWLSHolderBars = new G4Tubs("MiniShroudHoleWLSHolderBars",
	     					   0.,
	     					   MiniShroudHoleCoatingHolderBarRadius,
	     					   0.5*WLS_Thickness,
	     					   0.*deg,
	     					   360.*deg);

	       //inner coating, enclosing detectors, VM2000 layer
	       G4SubtractionSolid* VM2000InDet_SubtractionMain1 = new G4SubtractionSolid("VM2000InDet_SubtractionMain",
	        		 				     VM2000InDet_TubsOuter,
	        		 				     VM2000InDet_TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0,0.0,0.0));

               G4SubtractionSolid* VM2000InDet_SubtractionMain2 =
	 					new G4SubtractionSolid( "VM2000InDet_SubtractionMain2",
				   		VM2000InDet_SubtractionMain1,
				   		MiniShroudHoleCoatingVM2000In_HVCab,
				  		HVCableHoleCoatingMiniShroudRotOpt1,
						G4ThreeVector(MiniShroudHoleCoatingRadius*sin(fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(fCableHoleCoatingMiniShroud_angle1), 0.5*VM2000InDet_Height-0.5*VM2000_Thickness));

  	       G4SubtractionSolid* VM2000InDet_SubtractionMain3 =
						new G4SubtractionSolid("VM2000In_SubtractionMain3",
				   		VM2000InDet_SubtractionMain2,
				   		MiniShroudHoleCoatingVM2000In_SignalCab,
				   		SignalCableHoleCoatingMiniShroudRotOpt1,
				   	        G4ThreeVector(MiniShroudHoleCoatingRadius*sin(180.*deg + fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(180.*deg+fCableHoleCoatingMiniShroud_angle1), 0.5*VM2000InDet_Height-0.5*VM2000_Thickness));

		G4SubtractionSolid* current_solid_msInDet1 = VM2000InDet_SubtractionMain3;
		for (int i = 0; i < 3; i++) {
		const G4double VertBarAngle1 = ((G4double) i)* 120.*deg+fCableHoleCoatingMiniShroud_angle1;
                const G4ThreeVector VertBarTranslation1(HoleRad->at(ims)*std::cos(VertBarAngle1),HoleRad->at(ims)*std::sin(VertBarAngle1) , 0.5*VM2000InDet_Height-0.5*VM2000_Thickness);

                sprintf(solidname1, "solid_msInDet1_%dhole", i + 1);
                G4SubtractionSolid* solid_msInDet1_nHoles = new G4SubtractionSolid(solidname1,
							       current_solid_msInDet1,
							       MiniShroudHoleVM2000HolderBars,
							       0,
							       VertBarTranslation1);
                 current_solid_msInDet1 = solid_msInDet1_nHoles;
                 }
                G4SubtractionSolid* VM2000InDet_SubtractionMain = current_solid_msInDet1;

		//inner coating, enclosing detectors, WLS layer
	        G4SubtractionSolid* WLSInDet_SubtractionMain1 = new G4SubtractionSolid("WLSInDet_SubtractionMain",
	        		 				     WLSInDet_TubsOuter,
	        		 				     WLSInDet_TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0,0.0,0.0));

               G4SubtractionSolid* WLSInDet_SubtractionMain2 =
	 					new G4SubtractionSolid( "WLSInDet_SubtractionMain2",
				   		WLSInDet_SubtractionMain1,
				   		MiniShroudHoleCoatingWLSIn_HVCab,
				  		HVCableHoleCoatingMiniShroudRotOpt1,
						G4ThreeVector(MiniShroudHoleCoatingRadius*sin(fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(fCableHoleCoatingMiniShroud_angle1), 0.5*WSLInDet_Height-0.5*WLS_Thickness));

  	       G4SubtractionSolid* WLSInDet_SubtractionMain3 =
						new G4SubtractionSolid("WLSIn_SubtractionMain3",
				   		WLSInDet_SubtractionMain2,
				   		MiniShroudHoleCoatingWLSIn_SignalCab,
				   		SignalCableHoleCoatingMiniShroudRotOpt1,
				   	        G4ThreeVector(MiniShroudHoleCoatingRadius*sin(180.*deg + fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(180.*deg+fCableHoleCoatingMiniShroud_angle1), 0.5*WSLInDet_Height-0.5*WLS_Thickness));

		G4SubtractionSolid* current_solid_msInDet2 = WLSInDet_SubtractionMain3;
		for (int i = 0; i < 3; i++) {
		const G4double VertBarAngle2 = ((G4double) i)* 120.*deg+fCableHoleCoatingMiniShroud_angle1;
                const G4ThreeVector VertBarTranslation2(HoleRad->at(ims)*std::cos(VertBarAngle2),HoleRad->at(ims)*std::sin(VertBarAngle2) , 0.5*WSLInDet_Height-0.5*WLS_Thickness);

                sprintf(solidname2, "solid_msInDet2_%dhole", i + 1);
                G4SubtractionSolid* solid_msInDet2_nHoles = new G4SubtractionSolid(solidname2,
							       current_solid_msInDet2,
							       MiniShroudHoleWLSHolderBars,
							       0,
							       VertBarTranslation2);
                 current_solid_msInDet2 = solid_msInDet2_nHoles;
                 }
                G4SubtractionSolid* WLSInDet_SubtractionMain = current_solid_msInDet2;

	        //inner coating, above detectors, VM2000 layer
		G4SubtractionSolid* VM2000InAboveDet_SubtractionMain1 = new G4SubtractionSolid("VM2000InAboveDet_SubtractionMain",
	        		 				     VM2000InAboveDet_TubsOuter,
	        		 				     VM2000InAboveDet_TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0,0.0,VM2000_Thickness/2.));


		G4SubtractionSolid* VM2000InAboveDet_SubtractionMain2 =
	 					new G4SubtractionSolid( "VM2000InAboveDet_SubtractionMain2",
				   		VM2000InAboveDet_SubtractionMain1,
				   		MiniShroudHoleCoatingVM2000In_HVCab,
				  		HVCableHoleCoatingMiniShroudRotOpt1,
						G4ThreeVector(MiniShroudHoleCoatingRadius*sin(fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(fCableHoleCoatingMiniShroud_angle1), -0.5*VM2000InAboveDet_Height+0.5*VM2000_Thickness));

  	       G4SubtractionSolid* VM2000InAboveDet_SubtractionMain3 =
						new G4SubtractionSolid("VM2000AboveIn_SubtractionMain3",
				   		VM2000InAboveDet_SubtractionMain2,
				   		MiniShroudHoleCoatingVM2000In_SignalCab,
				   		SignalCableHoleCoatingMiniShroudRotOpt1,
				   	        G4ThreeVector(MiniShroudHoleCoatingRadius*sin(180.*deg + fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(180.*deg+fCableHoleCoatingMiniShroud_angle1), -0.5*VM2000InAboveDet_Height+0.5*VM2000_Thickness));


                G4SubtractionSolid* current_solid_msInAboveDet1 = VM2000InAboveDet_SubtractionMain3;
		for (int i = 0; i < 3; i++) {
		const G4double VertBarAngle1 = ((G4double) i)*120.*deg+fCableHoleCoatingMiniShroud_angle1;
                const G4ThreeVector VertBarTranslation1(HoleRad->at(ims)*std::cos(VertBarAngle1),HoleRad->at(ims)*std::sin(VertBarAngle1) , -0.5*VM2000InAboveDet_Height+0.5*VM2000_Thickness);

                sprintf(solidname1, "solid_msInAboveDet1_%dhole", i + 1);
                G4SubtractionSolid* solid_msInAboveDet1_nHoles = new G4SubtractionSolid(solidname1,
							       current_solid_msInAboveDet1,
							       MiniShroudHoleVM2000HolderBars,
							       0,
							       VertBarTranslation1);
                 current_solid_msInAboveDet1 = solid_msInAboveDet1_nHoles;
                 }
                G4SubtractionSolid* VM2000InAboveDet_SubtractionMain = current_solid_msInAboveDet1;

               //inner coating, above detectors, WLS layer
		G4SubtractionSolid* WLSInAboveDet_SubtractionMain1 = new G4SubtractionSolid("WLSInAboveDet_SubtractionMain",
	        		 				     WLSInAboveDet_TubsOuter,
	        		 				     WLSInAboveDet_TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0,0.0,WLS_Thickness/2.));


		G4SubtractionSolid* WLSInAboveDet_SubtractionMain2 =
	 					new G4SubtractionSolid( "WLSInAboveDet_SubtractionMain2",
				   		WLSInAboveDet_SubtractionMain1,
				   		MiniShroudHoleCoatingWLSIn_HVCab,
				  		HVCableHoleCoatingMiniShroudRotOpt1,
						G4ThreeVector(MiniShroudHoleCoatingRadius*sin(fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(fCableHoleCoatingMiniShroud_angle1), -0.5*WSLInAboveDet_Height+0.5*WLS_Thickness));

  	       G4SubtractionSolid* WLSInAboveDet_SubtractionMain3 =
						new G4SubtractionSolid("WLSAboveIn_SubtractionMain3",
				   		WLSInAboveDet_SubtractionMain2,
				   		MiniShroudHoleCoatingWLSIn_SignalCab,
				   		SignalCableHoleCoatingMiniShroudRotOpt1,
				   	        G4ThreeVector(MiniShroudHoleCoatingRadius*sin(180.*deg + fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(180.*deg+fCableHoleCoatingMiniShroud_angle1), -0.5*WSLInAboveDet_Height+0.5*WLS_Thickness));


                G4SubtractionSolid* current_solid_msInAboveDet2 = WLSInAboveDet_SubtractionMain3;
		for (int i = 0; i < 3; i++) {
		const G4double VertBarAngle2 = ((G4double) i)*120.*deg+fCableHoleCoatingMiniShroud_angle1;
                const G4ThreeVector VertBarTranslation2(HoleRad->at(ims)*std::cos(VertBarAngle2),HoleRad->at(ims)*std::sin(VertBarAngle2) , -0.5*WSLInAboveDet_Height+0.5*WLS_Thickness);

                sprintf(solidname2, "solid_msInAboveDet2_%dhole", i + 1);
                G4SubtractionSolid* solid_msInAboveDet2_nHoles = new G4SubtractionSolid(solidname2,
							       current_solid_msInAboveDet2,
							       MiniShroudHoleWLSHolderBars,
							       0,
							       VertBarTranslation2);
                 current_solid_msInAboveDet2 = solid_msInAboveDet2_nHoles;
                 }
                G4SubtractionSolid* WLSInAboveDet_SubtractionMain = current_solid_msInAboveDet2;

               // creating logical volumes

	       G4String VM2000InDetNameLogical = Form("VM2000InDetShroudLogical_%i",ims);
	       G4LogicalVolume *tmpVM2000InDetLogical = new G4LogicalVolume( VM2000InDet_SubtractionMain ,
	    		   fTPBOnNylon,
	    		   VM2000InDetNameLogical);
               tmpVM2000InDetLogical->SetVisAttributes( fVM2000VisAtt );

               G4String VM2000InAboveDetNameLogical = Form("VM2000InAboveDetShroudLogical_%i",ims);
	       G4LogicalVolume *tmpVM2000InAboveDetLogical = new G4LogicalVolume( VM2000InAboveDet_SubtractionMain ,
	    		   fTPBOnNylon,
	    		   VM2000InAboveDetNameLogical);
               tmpVM2000InAboveDetLogical->SetVisAttributes( fVM2000VisAtt );

	       G4String WLSInDetNameLogical = Form("WLSInDetShroudLogical_%i",ims);
	       G4LogicalVolume *tmpWLSInDetLogical = new G4LogicalVolume( WLSInDet_SubtractionMain ,
	    		   fTPBOnNylon,
	    		   WLSInDetNameLogical);
               tmpWLSInDetLogical->SetVisAttributes( fWLSVisAtt );

               G4String WLSInAboveDetNameLogical = Form("WLSInAboveDetShroudLogical_%i",ims);
	       G4LogicalVolume *tmpWLSInAboveDetLogical = new G4LogicalVolume( WLSInAboveDet_SubtractionMain ,
	    		   fTPBOnNylon,
	    		   WLSInAboveDetNameLogical);
               tmpWLSInAboveDetLogical->SetVisAttributes( fWLSVisAtt );


	       // placement
	       G4ThreeVector VM2000InDetPlacement=msbottom_positions->at(ims);
	       VM2000InDetPlacement.setZ(msbottom_positions->at(ims).z()+0.5*Shroud_BaseThickness+0.5*VM2000InDet_Height);

	       G4String VM2000InDetNamePhysical = Form ("VM2000InDetShroudPhysical_%i",ims);
	       new G4PVPlacement(0,
	    		   VM2000InDetPlacement,
	    		   tmpVM2000InDetLogical,
	    		   VM2000InDetNamePhysical,
	    		   fMotherLogical,
	    		   false,
	    		   0);

               G4ThreeVector VM2000InAboveDetPlacement=mstop_positions->at(ims);
	       VM2000InAboveDetPlacement.setZ(mstop_positions->at(ims).z()+0.5*Shroud_BaseThickness+0.5*VM2000InAboveDet_Height);

	       G4String VM2000InAboveDetNamePhysical = Form ("VM2000InAboveDetShroudPhysical_%i",ims);
	       new G4PVPlacement(0,
	    		   VM2000InAboveDetPlacement,
	    		   tmpVM2000InAboveDetLogical,
	    		   VM2000InAboveDetNamePhysical,
	    		   fMotherLogical,
	    		   false,
	    		   0);

               G4ThreeVector WLSInDetPlacement=msbottom_positions->at(ims);
	       WLSInDetPlacement.setZ(msbottom_positions->at(ims).z()+0.5*Shroud_BaseThickness+VM2000_Thickness+0.5*WSLInDet_Height);

	       G4String WLSInDetNamePhysical = Form ("WLSInDetShroudPhysical_%i",ims);
	       G4VPhysicalVolume* WLSInDetPhysical = new G4PVPlacement(0,
	    		   WLSInDetPlacement,
	    		   tmpWLSInDetLogical,
	    		   WLSInDetNamePhysical,
	    		   fMotherLogical,
	    		   false,
	    		   0);

               G4ThreeVector WLSInAboveDetPlacement=mstop_positions->at(ims);
	       WLSInAboveDetPlacement.setZ(mstop_positions->at(ims).z()+0.5*Shroud_BaseThickness+VM2000_Thickness+0.5*WSLInAboveDet_Height);

	       G4String WLSInAboveDetNamePhysical = Form ("WLSInAboveDetShroudPhysical_%i",ims);
	       G4VPhysicalVolume* WLSInAboveDetPhysical = new G4PVPlacement(0,
	    		   WLSInAboveDetPlacement,
	    		   tmpWLSInAboveDetLogical,
	    		   WLSInAboveDetNamePhysical,
	    		   fMotherLogical,
	    		   false,
	    		   0);

               new G4LogicalBorderSurface("LArInstr_MSNylon_WLSInsideAroundDetectors_surf_in",fMotherPhysical,WLSInDetPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSInsideAroundDetectors_surf_out",WLSInDetPhysical,fMotherPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSInsideAboveDetectors_surf_in",fMotherPhysical,WLSInAboveDetPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSInsideAboveDetectors_surf_out",WLSInAboveDetPhysical,fMotherPhysical,fWLSoptSurf);
    }
    else {
	       //creating VM2000 and WLS volumes as cut out boolean solids
	       G4Tubs* VM2000TubsOuter = new G4Tubs("VM2000TubsOuter",
	     					   0.,
	     					   VM2000_TubsOuterRadius,
	     					   VM2000_Height / 2.,
	     					   0.*deg,
	     					   360.*deg);

	       G4Tubs* VM2000TubsInner = new G4Tubs("VM2000TubsInner",
	     					   0,
	     					   VM2000_TubsOuterRadius - VM2000_Thickness,
	     					   (VM2000_Height - 2*VM2000_Thickness) / 2.,
	     					   0.*deg,
	     					   360.*deg);

		//holes for cables of phase2
		G4SubtractionSolid* VM2000SubtractionMain;
		if (HoleCounter>0){
		        G4double MiniShroudHoleCoatingCabVM2000z= 0.5*VM2000_Thickness;
			G4SubtractionSolid* VM2000SubtractionMain1 = new G4SubtractionSolid("VM2000SubtractionMain1",
	        		 				     VM2000TubsOuter,
	        		 				     VM2000TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0, 0.0, 0.0));

		        G4Box* MiniShroudHoleCoatingVM2000HVCab = new G4Box("MiniShroudHoleCoatingVM2000HVCab",
						 		       MiniShroudHoleCoatingCabx,
						 		       MiniShroudHoleCoatingCaby,
								       MiniShroudHoleCoatingCabVM2000z);

	  		G4Box* MiniShroudHoleCoatingVM2000SignalCab = new G4Box("MiniShroudHoleCoatingVM2000SignalHVCab",
						 			MiniShroudHoleCoatingCabx,
						 			MiniShroudHoleCoatingCaby,
									MiniShroudHoleCoatingCabVM2000z);
			G4SubtractionSolid* VM2000SubtractionMain2 =
	 					new G4SubtractionSolid( "VM2000SubtractionMain2",
				   		VM2000SubtractionMain1,
				   		MiniShroudHoleCoatingVM2000HVCab,
				  		HVCableHoleCoatingMiniShroudRotOpt1,
						G4ThreeVector(MiniShroudHoleCoatingRadius*sin(fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(fCableHoleCoatingMiniShroud_angle1), 0.5*VM2000_Height-0.5*VM2000_Thickness));

  	 		VM2000SubtractionMain =
						new G4SubtractionSolid("VM2000SubtractionMain",
				   		VM2000SubtractionMain2,
				   		MiniShroudHoleCoatingVM2000SignalCab,
				   		SignalCableHoleCoatingMiniShroudRotOpt1,
				   	        G4ThreeVector(MiniShroudHoleCoatingRadius*sin(180*deg + fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(180*deg+fCableHoleCoatingMiniShroud_angle1), 0.5*VM2000_Height-0.5*VM2000_Thickness));
				}
		else {VM2000SubtractionMain = new G4SubtractionSolid("VM2000SubtractionMain",
	        		 				     VM2000TubsOuter,
	        		 				     VM2000TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0, 0.0, 0.0));}

	       G4Tubs* WLSTubsOuter = new G4Tubs("WLSTubsOuter",
	     					   0.,
	     					   WLS_TubsOuterRadius,
	     					   WLS_Height  / 2.,
	     					   0.*deg,
	     					   360.*deg);

	       G4Tubs* WLSTubsInner = new G4Tubs("WLSTubsInner",
	     					   0,
	     					   WLS_TubsOuterRadius - WLS_Thickness,
	     					   (WLS_Height - 2*WLS_Thickness) / 2.,
	     					   0.*deg,
	     					   360.*deg);
		//holes for cables of phase2
		G4SubtractionSolid* WLSSubtractionMain;
		if (HoleCounter>0){
		        G4double MiniShroudHoleCoatingCabWLSz= 0.5*WLS_Thickness;
			G4SubtractionSolid* WLSSubtractionMain1 = new G4SubtractionSolid("WLSSubtractionMain1",
	        		 				   WLSTubsOuter,
	        		 				   WLSTubsInner,
	        		 				   0,
								   G4ThreeVector(0.0, 0.0, 0.0));

	        	G4Box* MiniShroudHoleCoatingWLSHVCab = new G4Box("MiniShroudHoleCoatingWLSHVCab",
						 		       MiniShroudHoleCoatingCabx,
						 		       MiniShroudHoleCoatingCaby,
								       MiniShroudHoleCoatingCabWLSz);

	  		G4Box* MiniShroudHoleCoatingWLSSignalCab = new G4Box("MiniShroudHoleCoatingWLSSignalHVCab",
						 			MiniShroudHoleCoatingCabx,
						 			MiniShroudHoleCoatingCaby,
									MiniShroudHoleCoatingCabWLSz);

			G4SubtractionSolid* WLSSubtractionMain2 =
	 					new G4SubtractionSolid( "WLSSubtractionMain2",
				   		WLSSubtractionMain1,
				   		MiniShroudHoleCoatingWLSHVCab,
				  		HVCableHoleCoatingMiniShroudRotOpt1,
						G4ThreeVector(MiniShroudHoleCoatingRadius*sin(fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(fCableHoleCoatingMiniShroud_angle1), 0.5*WLS_Height-0.5*WLS_Thickness));

  	 		WLSSubtractionMain =
						new G4SubtractionSolid("WLSSubtractionMain",
				   		WLSSubtractionMain2,
				   		MiniShroudHoleCoatingWLSSignalCab,
				   		SignalCableHoleCoatingMiniShroudRotOpt1,
				   	        G4ThreeVector(MiniShroudHoleCoatingRadius*sin(180*deg + fCableHoleCoatingMiniShroud_angle1), MiniShroudHoleCoatingRadius*cos(180*deg+fCableHoleCoatingMiniShroud_angle1), 0.5*WLS_Height-0.5*WLS_Thickness));


				 }
		else { WLSSubtractionMain = new G4SubtractionSolid("WLSSubtractionMain",
	        		 				   WLSTubsOuter,
	        		 				   WLSTubsInner,
	        		 				   0,
	        		 				   G4ThreeVector(0.0, 0.0, 0.0));
		     }

	       // creating logical volumes

	       G4String VM2000NameLogical = Form("VM2000ShroudLogical_%i",ims);
	       G4LogicalVolume *tmplog = new G4LogicalVolume( VM2000SubtractionMain,
	   	   	   	   	  fMaterialVM2000,
					  VM2000NameLogical);
	       new G4LogicalSkinSurface("MS_VM2000_surface",tmplog,fVM2000OptSurface);

	       VM2000Logicals.push_back(tmplog);
	       VM2000Logicals.at(ims)->SetVisAttributes( fVM2000VisAtt );


	       G4String WLSNameLogical = Form("WLSShroudLogical_%i",ims);
	       WLSLogicals.push_back( new G4LogicalVolume( WLSSubtractionMain,
	    		   	   	   	   	  fTPB,
	    		   	   	   	   	  WLSNameLogical));
	       WLSLogicals.at(ims)->SetVisAttributes( fWLSVisAtt );


	       // placing VM2000 and WLS volumes
	       G4String VM2000NamePhysical = Form("VM2000ShroudPhysical_%i",ims);
	       new G4PVPlacement(0,
	    		   ms_positions->at(ims),
	    		   VM2000Logicals.at(ims),
	    		   VM2000NamePhysical,
	    		   fMotherLogical,
	    		   false,
	    		   0);


	       G4String WLSNamePhysical = Form ("WLSShroudPhysical_%i",ims);
	       G4VPhysicalVolume* wlsPhysical = new G4PVPlacement(0,
	    		   ms_positions->at(ims),
	    		   WLSLogicals.at(ims),
	    		   WLSNamePhysical,
	    		   fMotherLogical,
	    		   false,
	    		   0);


	       new G4LogicalBorderSurface("LArInstr_MSAPD_WLS_surf_in",fMotherPhysical,wlsPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSAPD_WLS_surf_out",wlsPhysical,fMotherPhysical,fWLSoptSurf);
	       }

       }// end loop
   else if (fDetectorDB->GetLArInstrumentationMSCoating()==2) // Nylon MS with TPB-Nylon-TPB
     for (int ims = 0; ims < (int) ms_array->size(); ++ims) {


       	   G4double Shroud_TubsOuterRadius	= ms_radius->at(ims); // phase i: 51.5 * mm
           G4double Shroud_SideThickness	= fDetectorDB->GetMiniShroudSideThickness(); //0.06 * mm;
           G4double Shroud_TubsInnerRadius	= Shroud_TubsOuterRadius - Shroud_SideThickness;
           G4double Shroud_Height			= fDetectorDB->GetMiniShroudHeightPhaseIINylon();// ms_height->at(ims); //500.0 * mm;
           G4double Shroud_BaseThickness	= fDetectorDB->GetMiniShroudBaseThickness(); //0.12 * mm;

           G4double Shroud_GluedRingThickness = 2 * Shroud_SideThickness; // two additional layers
           G4double Shroud_TopGlueRingHeight = fDetectorDB->GetMiniShroudTopGluedRingHeight();
           G4double Shroud_BottomGlueRingHeight = fDetectorDB->GetMiniShroudBottomGluedRingHeight();

           MGLog(debugging) << "MS:"<< ims << endlog;
           MGLog(debugging) << "MS outer radius [mm]:     "<< Shroud_TubsOuterRadius/mm << endlog;
           MGLog(debugging) << "MS height [mm]:           "<< Shroud_Height/mm << endlog;
           MGLog(debugging) << "Constructing optical volumes for MSs "<< endlog;
           MGLog(debugging) << "MS side thickness [mm]:   "<< Shroud_SideThickness/mm << endlog;
           MGLog(debugging) << "MS base thickness [mm]:   "<< Shroud_BaseThickness/mm << endlog;



    	   // inside WLS volume
    	   G4double WLSIn_Thickness			= 0.001*mm;
    	   G4double WLSIn_TubsOuterRadius 	= Shroud_TubsInnerRadius;
    	   //G4double WLSIn_TubsInnerRadius 	= WLSIn_TubsOuterRadius - WLSIn_Thickness;
    	   G4double WLSIn_Height 			= Shroud_Height - 2.*Shroud_BaseThickness;

    	   // outside WLS volume
    	   G4double WLSOut_Thickness			= 0.001*mm;
       	   G4double WLSOut_TubsInnerRadius   	= Shroud_TubsOuterRadius;
    	   G4double WLSOut_TubsOuterRadius   	= WLSOut_TubsInnerRadius + WLSOut_Thickness;
       	   G4double WLSOut_Height 		= Shroud_Height + 2.*WLSOut_Thickness;

    	   MGLog(debugging) << "Inner WLS outer radius [mm]: "<< WLSIn_TubsOuterRadius/mm << endlog;
    	   MGLog(debugging) << "Inner WLS thickness [mm]:    "<< WLSIn_Thickness/mm << endlog;
    	   MGLog(debugging) << "Inner WLS height [mm]:       "<< WLSIn_Height/mm << endlog;

    	   MGLog(debugging) << "Outer WLS outer radius [mm]:    "<< WLSOut_TubsOuterRadius/mm << endlog;
    	   MGLog(debugging) << "Outer WLS thickness [mm]:       "<< WLSOut_Thickness/mm << endlog;
    	   MGLog(debugging) << "Outer WLS height [mm]:          "<< WLSOut_Height/mm << endlog;

	   G4double fCableHoleCoatingMiniShroud_angle2=fDetectorDB->GetAngleForPhase2CablePosition(ims);
           G4RotationMatrix* HVCableHoleCoatingMiniShroudRotOpt2 = new G4RotationMatrix();
           HVCableHoleCoatingMiniShroudRotOpt2->rotateZ(fCableHoleCoatingMiniShroud_angle2);
           G4RotationMatrix* SignalCableHoleCoatingMiniShroudRotOpt2 = new G4RotationMatrix();
           SignalCableHoleCoatingMiniShroudRotOpt2->rotateZ(180.*deg+fCableHoleCoatingMiniShroud_angle2);

  //more detailed mini-shroud for phase2
  if (fMiniShroudMaterial=="Nylon"){
               std::vector<double> *HoleRad = fDetectorDB-> GetMiniShroudHoleRadiusVector();
	       char solidname[100];

               G4Tubs* WLSOut_TubsOuter = new G4Tubs("WLSOut_TubsOuter",
	     					   0.,
	     					   WLSOut_TubsOuterRadius + Shroud_GluedRingThickness,
	     					   (WLSOut_Height - WLSOut_Thickness)/ 2.,
	     					   0.*deg,
	     					   360.*deg);

	       G4Tubs* WLSOut_TubsInner = new G4Tubs("WLSOut_TubsInner",
	     					   0,
	     					   WLSOut_TubsOuterRadius - WLSOut_Thickness,
	     					   Shroud_Height/ 2.,
	     					   0.*deg,
	     					   360.*deg);

             // parts to be subtracted to account for glued ring
             G4Tubs* WLSOut_GluedRing = new G4Tubs("WLSOut_GluedRing",
                                0,
                                Shroud_TubsOuterRadius + Shroud_GluedRingThickness,
                                Shroud_BottomGlueRingHeight/2.,
                                0.*deg,
	     					   360.*deg);

             G4Tubs* WLSOut_AntiGluedRing = new G4Tubs("WLSOut_AntiGluedRing",
                                WLSOut_TubsOuterRadius,
                                WLSOut_TubsOuterRadius + Shroud_GluedRingThickness,
                                (Shroud_Height - Shroud_BottomGlueRingHeight - WLSOut_Thickness) /2.,
                                0.*deg,
	     					   360.*deg);
             // END

               G4double WSLInDet_Height=ms_inDet_height->at(ims);
               G4Tubs* WLSInDet_TubsOuter = new G4Tubs("WLSInDet_TubsOuter",
	     					   0.,
	     					   WLSIn_TubsOuterRadius,
	     					   WSLInDet_Height/ 2.,
	     					   0.*deg,
	     					   360.*deg);

	       G4Tubs* WLSInDet_TubsInner = new G4Tubs("WLSInDet_TubsInner",
	     					   0,
	     					   WLSIn_TubsOuterRadius - WLSIn_Thickness,
	     					   (WSLInDet_Height-2.*WLSIn_Thickness)/ 2.,
	     					   0.*deg,
	     					   360.*deg);

	       G4double WSLInAboveDet_Height=Shroud_Height-ms_inDet_height->at(ims)-2.*Shroud_BaseThickness;
	       G4Tubs* WLSInAboveDet_TubsOuter = new G4Tubs("WLSInAboveDet_TubsOuter",
	     					   0.,
	     					   WLSIn_TubsOuterRadius,
	     					   0.5*(WSLInAboveDet_Height),
	     					   0.*deg,
	     					   360.*deg);

	       G4Tubs* WLSInAboveDet_TubsInner = new G4Tubs("WLSInAboveDet_TubsInner",
	     					   0,
	     					   WLSIn_TubsOuterRadius - WLSIn_Thickness - Shroud_GluedRingThickness,
	     					   0.5*(WSLInAboveDet_Height-WLSIn_Thickness),
	     					   0.*deg,
	     					   360.*deg);

           // parts to be subtracted to account for glued ring
           G4Tubs* WLSInAbove_GluedRing = new G4Tubs("WLSInAbove_GluedRing",
                               WLSIn_TubsOuterRadius - Shroud_GluedRingThickness,
                               WLSIn_TubsOuterRadius,
                               Shroud_TopGlueRingHeight/2.,
                               0.*deg,
  	     					   360.*deg);

            MGLog(debugging) << WSLInAboveDet_Height / cm << " cm" << endlog;

            G4Tubs* WLSInAbove_AntiGluedRing = 0;

            if( WSLInAboveDet_Height > Shroud_TopGlueRingHeight )
            {
                WLSInAbove_AntiGluedRing = new G4Tubs("WLSInAbove_AntiGluedRing",
                               WLSIn_TubsOuterRadius - WLSIn_Thickness - Shroud_GluedRingThickness,
                               WLSIn_TubsOuterRadius - WLSIn_Thickness,
                               (WSLInAboveDet_Height - WLSIn_Thickness - Shroud_TopGlueRingHeight) /2.,
                               0.*deg,
   	     					   360.*deg);
            }
            // END

               //holes: for signal and HV cable, for the 3 holder bars
	       G4Box* MiniShroudHoleCoatingWLSIn_HVCab = new G4Box("MiniShroudHoleCoatingWLSIn_HVCab",
						 		       MiniShroudHoleCoatingCabx,
						 		       MiniShroudHoleCoatingCaby,
								       0.5*WLSIn_Thickness);

	       G4Box* MiniShroudHoleCoatingWLSIn_SignalCab = new G4Box("MiniShroudHoleCoatingWLSIn_SignalHVCab",
						 			MiniShroudHoleCoatingCabx,
						 			MiniShroudHoleCoatingCaby,
									0.5*WLSIn_Thickness);
	       G4Tubs* MiniShroudHoleHolderBars = new G4Tubs("MiniShroudHoleHolderBars",
	     					   0.,
	     					   MiniShroudHoleCoatingHolderBarRadius,
	     					   0.5*WLSIn_Thickness,
	     					   0.*deg,
	     					   360.*deg);

               //outer coating
	       G4SubtractionSolid* WLSOut_SubtractionMain = new G4SubtractionSolid("WLSOut_SubtractionMain",
	        		 				     WLSOut_TubsOuter,
	        		 				     WLSOut_TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0, 0.0,WLSOut_Thickness/2.));
           G4SubtractionSolid* WLSOut_SubtractRing = new G4SubtractionSolid("WLSOut_SubtractRing",
	        		 				     WLSOut_SubtractionMain,
	        		 				     WLSOut_GluedRing,
	        		 				     0,
	        		 				     G4ThreeVector(0.0, 0.0, Shroud_TopGlueRingHeight/2.+WLSOut_Thickness/2.-Shroud_Height/2.));
           G4SubtractionSolid* WLSOut_SubtractAntiRing = new G4SubtractionSolid("WLSOut_SubtractAntiRing",
  	        		 				     WLSOut_SubtractRing,
  	        		 				     WLSOut_AntiGluedRing,
  	        		 				     0,
  	        		 				     G4ThreeVector(0.0, 0.0, WLSOut_Thickness+Shroud_BottomGlueRingHeight/2.));
           WLSOut_SubtractionMain = WLSOut_SubtractAntiRing;

	       //inner coating, enclosing detectors
	       G4SubtractionSolid* WLSInDet_SubtractionMain1 = new G4SubtractionSolid("WLSInDet_SubtractionMain",
	        		 				     WLSInDet_TubsOuter,
	        		 				     WLSInDet_TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0,0.0,0.0));

               G4SubtractionSolid* WLSInDet_SubtractionMain2 =
	 					new G4SubtractionSolid( "WLSInDet_SubtractionMain2",
				   		WLSInDet_SubtractionMain1,
				   		MiniShroudHoleCoatingWLSIn_HVCab,
				  		HVCableHoleCoatingMiniShroudRotOpt2,
						G4ThreeVector(MiniShroudHoleCoatingRadius*sin(fCableHoleCoatingMiniShroud_angle2), MiniShroudHoleCoatingRadius*cos(fCableHoleCoatingMiniShroud_angle2), 0.5*WSLInDet_Height-0.5*WLSIn_Thickness));

  	       G4SubtractionSolid* WLSInDet_SubtractionMain3 =
						new G4SubtractionSolid("WLSIn_SubtractionMain3",
				   		WLSInDet_SubtractionMain2,
				   		MiniShroudHoleCoatingWLSIn_SignalCab,
				   		SignalCableHoleCoatingMiniShroudRotOpt2,
				   	        G4ThreeVector(MiniShroudHoleCoatingRadius*sin(180.*deg + fCableHoleCoatingMiniShroud_angle2), MiniShroudHoleCoatingRadius*cos(180.*deg+fCableHoleCoatingMiniShroud_angle2), 0.5*WSLInDet_Height-0.5*WLSIn_Thickness));



		G4SubtractionSolid* current_solid_msInDet = WLSInDet_SubtractionMain3;
		for (int i = 0; i < 3; i++) {
		const G4double VertBarAngle = ((G4double) i)* 120.*deg+fCableHoleCoatingMiniShroud_angle2;
                const G4ThreeVector VertBarTranslation(HoleRad->at(ims)*std::cos(VertBarAngle),HoleRad->at(ims)*std::sin(VertBarAngle) , 0.5*WSLInDet_Height-0.5*WLSIn_Thickness);

                sprintf(solidname, "solid_msInDet_%dhole", i + 1);
                G4SubtractionSolid* solid_msInDet_nHoles = new G4SubtractionSolid(solidname,
							       current_solid_msInDet,
							       MiniShroudHoleHolderBars,
							       0,
							       VertBarTranslation);
                 current_solid_msInDet = solid_msInDet_nHoles;
                 }
                G4SubtractionSolid* WLSInDet_SubtractionMain = current_solid_msInDet;

                //inner coating, above detectors
		G4SubtractionSolid* WLSInAboveDet_SubtractionMain1 = new G4SubtractionSolid("WLSInAboveDet_SubtractionMain",
	        		 				     WLSInAboveDet_TubsOuter,
	        		 				     WLSInAboveDet_TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0,0.0,WLSIn_Thickness/2.));
        G4SubtractionSolid* WLSInAboveDet_SubtractRing = new G4SubtractionSolid("WLSInAboveDet_SubtractRing",
	        		 				     WLSInAboveDet_SubtractionMain1,
	        		 				     WLSInAbove_GluedRing,
	        		 				     0,
	        		 				     G4ThreeVector(0.0,0.0, Shroud_TopGlueRingHeight/2.-WSLInAboveDet_Height/2.));

        G4SubtractionSolid* WLSInAboveDet_SubtractAntiRing;

        if( WSLInAboveDet_Height > Shroud_TopGlueRingHeight )
        {
            WLSInAboveDet_SubtractAntiRing = new G4SubtractionSolid("WLSInAboveDet_SubtractAntiRing",
	        		 				     WLSInAboveDet_SubtractRing,
	        		 				     WLSInAbove_AntiGluedRing,
	        		 				     0,
	        		 				     G4ThreeVector(0.0,0.0,Shroud_TopGlueRingHeight/2. + WLSIn_Thickness/2.));

            WLSInAboveDet_SubtractionMain1 = WLSInAboveDet_SubtractAntiRing;
        }
        else
            WLSInAboveDet_SubtractionMain1 = WLSInAboveDet_SubtractRing;


		G4SubtractionSolid* WLSInAboveDet_SubtractionMain2 =
	 					new G4SubtractionSolid( "WLSInAboveDet_SubtractionMain2",
				   		WLSInAboveDet_SubtractionMain1,
				   		MiniShroudHoleCoatingWLSIn_HVCab,
				  		HVCableHoleCoatingMiniShroudRotOpt2,
						G4ThreeVector(MiniShroudHoleCoatingRadius*sin(fCableHoleCoatingMiniShroud_angle2), MiniShroudHoleCoatingRadius*cos(fCableHoleCoatingMiniShroud_angle2), -0.5*WSLInAboveDet_Height+0.5*WLSIn_Thickness));

  	       G4SubtractionSolid* WLSInAboveDet_SubtractionMain3 =
						new G4SubtractionSolid("WLSAboveIn_SubtractionMain3",
				   		WLSInAboveDet_SubtractionMain2,
				   		MiniShroudHoleCoatingWLSIn_SignalCab,
				   		SignalCableHoleCoatingMiniShroudRotOpt2,
				   	        G4ThreeVector(MiniShroudHoleCoatingRadius*sin(180.*deg + fCableHoleCoatingMiniShroud_angle2), MiniShroudHoleCoatingRadius*cos(180.*deg+fCableHoleCoatingMiniShroud_angle2), -0.5*WSLInAboveDet_Height+0.5*WLSIn_Thickness));


                G4SubtractionSolid* current_solid_msInAboveDet = WLSInAboveDet_SubtractionMain3;
		for (int i = 0; i < 3; i++) {
		const G4double VertBarAngle = ((G4double) i)*120.*deg+fCableHoleCoatingMiniShroud_angle2;
                const G4ThreeVector VertBarTranslation1(HoleRad->at(ims)*std::cos(VertBarAngle),HoleRad->at(ims)*std::sin(VertBarAngle) , -0.5*WSLInAboveDet_Height+0.5*WLSIn_Thickness);

                sprintf(solidname, "solid_msInAboveDet_%dhole", i + 1);
                G4SubtractionSolid* solid_msInAboveDet_nHoles = new G4SubtractionSolid(solidname,
							       current_solid_msInAboveDet,
							       MiniShroudHoleHolderBars,
							       0,
							       VertBarTranslation1);
                 current_solid_msInAboveDet = solid_msInAboveDet_nHoles;
                 }
                G4SubtractionSolid* WLSInAboveDet_SubtractionMain = current_solid_msInAboveDet;


               // creating logical volumes
	       G4String WLSOutNameLogical = Form("WLSOutShroudLogical_%i",ims);
	       G4LogicalVolume *WLSOutLogical = new G4LogicalVolume( WLSOut_SubtractionMain,
	    		   fTPBOnNylon,
	    		   WLSOutNameLogical);
	       WLSOutLogical->SetVisAttributes( fWLSVisAtt );

	       G4String WLSInDetNameLogical = Form("WLSInDetShroudLogical_%i",ims);
	       G4LogicalVolume *tmpWLSInDetLogical = new G4LogicalVolume( WLSInDet_SubtractionMain ,
	    		   fTPBOnNylon,
	    		   WLSInDetNameLogical);
               tmpWLSInDetLogical->SetVisAttributes( fWLSVisAtt );

               G4String WLSInAboveDetNameLogical = Form("WLSInAboveDetShroudLogical_%i",ims);
	       G4LogicalVolume *tmpWLSInAboveDetLogical = new G4LogicalVolume( WLSInAboveDet_SubtractionMain ,
	    		   fTPBOnNylon,
	    		   WLSInAboveDetNameLogical);
               tmpWLSInAboveDetLogical->SetVisAttributes( fWLSVisAtt );

	       // placement
	       G4ThreeVector WLSOutPlacement=ms_positions->at(ims);
	       WLSOutPlacement.setZ(ms_positions->at(ims).z()-0.5*WLSOut_Thickness);

	       G4String WLSOutNamePhysical = Form ("WLSOutShroudPhysical_%i",ims);
	       G4VPhysicalVolume* WLSOutPhysical = new G4PVPlacement(0,
	    		   WLSOutPlacement,
	    		   WLSOutLogical,
	    		   WLSOutNamePhysical,
	    		   fMotherLogical,
	    		   false,
	    		   0);

	       G4ThreeVector WLSInDetPlacement=msbottom_positions->at(ims);
	       WLSInDetPlacement.setZ(msbottom_positions->at(ims).z()+0.5*Shroud_BaseThickness+0.5*WSLInDet_Height);

	       G4String WLSInDetNamePhysical = Form ("WLSInDetShroudPhysical_%i",ims);
	       G4VPhysicalVolume* WLSInDetPhysical = new G4PVPlacement(0,
	    		   WLSInDetPlacement,
	    		   tmpWLSInDetLogical,
	    		   WLSInDetNamePhysical,
	    		   fMotherLogical,
	    		   false,
	    		   0);

               G4ThreeVector WLSInAboveDetPlacement=mstop_positions->at(ims);
	       WLSInAboveDetPlacement.setZ(mstop_positions->at(ims).z()+0.5*Shroud_BaseThickness+0.5*WSLInAboveDet_Height);

	       G4String WLSInAboveDetNamePhysical = Form ("WLSInAboveDetShroudPhysical_%i",ims);
	       G4VPhysicalVolume* WLSInAboveDetPhysical = new G4PVPlacement(0,
	    		   WLSInAboveDetPlacement,
	    		   tmpWLSInAboveDetLogical,
	    		   WLSInAboveDetNamePhysical,
	    		   fMotherLogical,
	    		   false,
	    		   0);

	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSInsideAroundDetectors_surf_in",fMotherPhysical,WLSInDetPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSInsideAroundDetectors_surf_out",WLSInDetPhysical,fMotherPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSInsideAboveDetectors_surf_in",fMotherPhysical,WLSInAboveDetPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSInsideAboveDetectors_surf_out",WLSInAboveDetPhysical,fMotherPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSOutside_surf_in",fMotherPhysical,WLSOutPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSOutside_surf_out",WLSOutPhysical,fMotherPhysical,fWLSoptSurf);



    }
    else {
	       //creating WLS volumes as cut out boolean solids
	       G4Tubs* WLSIn_TubsOuter = new G4Tubs("WLSIn_TubsOuter",
	     					   0.,
	     					   WLSIn_TubsOuterRadius,
	     					   WLSIn_Height / 2.,
	     					   0.*deg,
	     					   360.*deg);

	       G4Tubs* WLSIn_TubsInner = new G4Tubs("WLSIn_TubsInner",
	     					   0,
	     					   WLSIn_TubsOuterRadius - WLSIn_Thickness,
	     					   (WLSIn_Height - 2*WLSIn_Thickness) / 2.,
	     					   0.*deg,
	     					   360.*deg);

	//holes for cables of phase2
		G4SubtractionSolid* WLSIn_SubtractionMain;
		if (HoleCounter>0){
		        G4double MiniShroudHoleCoatingCabWLSIn_z= 0.5*WLSIn_Thickness;
			G4SubtractionSolid* WLSIn_SubtractionMain1 = new G4SubtractionSolid("WLSIn_SubtractionMain1",
	        		 				     WLSIn_TubsOuter,
	        		 				     WLSIn_TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0, 0.0, 0.0));

		        G4Box* MiniShroudHoleCoatingWLSIn_HVCab = new G4Box("MiniShroudHoleCoatingWLSIn_HVCab",
						 		       MiniShroudHoleCoatingCabx,
						 		       MiniShroudHoleCoatingCaby,
								       MiniShroudHoleCoatingCabWLSIn_z);

	  		G4Box* MiniShroudHoleCoatingWLSIn_SignalCab = new G4Box("MiniShroudHoleCoatingWLSIn_SignalHVCab",
						 			MiniShroudHoleCoatingCabx,
						 			MiniShroudHoleCoatingCaby,
									MiniShroudHoleCoatingCabWLSIn_z);
			G4SubtractionSolid* WLSIn_SubtractionMain2 =
	 					new G4SubtractionSolid( "WLSIn_SubtractionMain2",
				   		WLSIn_SubtractionMain1,
				   		MiniShroudHoleCoatingWLSIn_HVCab,
				  		HVCableHoleCoatingMiniShroudRotOpt2,
						G4ThreeVector(MiniShroudHoleCoatingRadius*sin(fCableHoleCoatingMiniShroud_angle2), MiniShroudHoleCoatingRadius*cos(fCableHoleCoatingMiniShroud_angle2), 0.5*WLSIn_Height-0.5*WLSIn_Thickness));

  	 		WLSIn_SubtractionMain =
						new G4SubtractionSolid("WLSIn_SubtractionMain",
				   		WLSIn_SubtractionMain2,
				   		MiniShroudHoleCoatingWLSIn_SignalCab,
				   		SignalCableHoleCoatingMiniShroudRotOpt2,
				   	        G4ThreeVector(MiniShroudHoleCoatingRadius*sin(180*deg + fCableHoleCoatingMiniShroud_angle2), MiniShroudHoleCoatingRadius*cos(180*deg+fCableHoleCoatingMiniShroud_angle2), 0.5*WLSIn_Height-0.5*WLSIn_Thickness));
				}
		else {WLSIn_SubtractionMain = new G4SubtractionSolid("WLSIn_SubtractionMain",
	        		 				     WLSIn_TubsOuter,
	        		 				     WLSIn_TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0, 0.0, 0.0));}


	       G4Tubs* WLSOut_TubsOuter = new G4Tubs("WLSOut_TubsOuter",
	     					   0.,
	     					   WLSOut_TubsOuterRadius,
	     					   WLSOut_Height / 2.,
	     					   0.*deg,
	     					   360.*deg);

	       G4Tubs* WLSOut_TubsInner = new G4Tubs("WLSOut_TubsInner",
	     					   0,
	     					   WLSOut_TubsOuterRadius - WLSOut_Thickness,
	     					   (WLSOut_Height - 2.*WLSOut_Thickness) / 2.,
	     					   0.*deg,
	     					   360.*deg);

		//holes for cables of phase2
		G4SubtractionSolid* WLSOut_SubtractionMain;
		if (HoleCounter>0){
		        G4double MiniShroudHoleCoatingCabWLSOut_z= 0.5*WLSOut_Thickness;
			G4SubtractionSolid* WLSOut_SubtractionMain1 = new G4SubtractionSolid("WLSOut_SubtractionMain1",
	        		 				     WLSOut_TubsOuter,
	        		 				     WLSOut_TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0, 0.0, 0.0));

		        G4Box* MiniShroudHoleCoatingWLSOut_HVCab = new G4Box("MiniShroudHoleCoatingWLSOut_HVCab",
						 		       MiniShroudHoleCoatingCabx,
						 		       MiniShroudHoleCoatingCaby,
								       MiniShroudHoleCoatingCabWLSOut_z);

	  		G4Box* MiniShroudHoleCoatingWLSOut_SignalCab = new G4Box("MiniShroudHoleCoatingWLSOut_SignalHVCab",
						 			MiniShroudHoleCoatingCabx,
						 			MiniShroudHoleCoatingCaby,
									MiniShroudHoleCoatingCabWLSOut_z);
			G4SubtractionSolid* WLSOut_SubtractionMain2 =
	 					new G4SubtractionSolid( "WLSOut_SubtractionMain2",
				   		WLSOut_SubtractionMain1,
				   		MiniShroudHoleCoatingWLSOut_HVCab,
				  		HVCableHoleCoatingMiniShroudRotOpt2,
						G4ThreeVector(MiniShroudHoleCoatingRadius*sin(fCableHoleCoatingMiniShroud_angle2), MiniShroudHoleCoatingRadius*cos(fCableHoleCoatingMiniShroud_angle2), 0.5*WLSOut_Height-0.5*WLSOut_Thickness));

  	 		WLSOut_SubtractionMain =
						new G4SubtractionSolid("WLSOut_SubtractionMain",
				   		WLSOut_SubtractionMain2,
				   		MiniShroudHoleCoatingWLSOut_SignalCab,
				   		SignalCableHoleCoatingMiniShroudRotOpt2,
				   	        G4ThreeVector(MiniShroudHoleCoatingRadius*sin(180*deg + fCableHoleCoatingMiniShroud_angle2), MiniShroudHoleCoatingRadius*cos(180*deg+fCableHoleCoatingMiniShroud_angle2), 0.5*WLSOut_Height-0.5*WLSOut_Thickness));
				}
		else {WLSOut_SubtractionMain = new G4SubtractionSolid("WLSOut_SubtractionMain",
	        		 				     WLSOut_TubsOuter,
	        		 				     WLSOut_TubsInner,
	        		 				     0,
	        		 				     G4ThreeVector(0.0, 0.0, 0.0));}

	       // creating logical volumes

	       G4String WLSInNameLogical = Form("WLSInShroudLogical_%i",ims);
	       G4LogicalVolume *tmpWLSInLogical = new G4LogicalVolume( WLSIn_SubtractionMain,
	    		   fTPBOnNylon,
	    		   WLSInNameLogical);

	       tmpWLSInLogical->SetVisAttributes( fWLSVisAtt );


	       G4String WLSOutNameLogical = Form("WLSOutShroudLogical_%i",ims);
	       G4LogicalVolume *WLSOutLogical = new G4LogicalVolume( WLSOut_SubtractionMain,
	    		   fTPBOnNylon,
	    		   WLSOutNameLogical);

	       WLSOutLogical->SetVisAttributes( fWLSVisAtt );

	       // placement
	       G4String WLSInNamePhysical = Form ("WLSInShroudPhysical_%i",ims);
	       G4VPhysicalVolume* WLSInPhysical = new G4PVPlacement(0,
	    		   ms_positions->at(ims),
	    		   tmpWLSInLogical,
	    		   WLSInNamePhysical,
	    		   fMotherLogical,
	    		   false,
	    		   0);

	       G4String WLSOutNamePhysical = Form ("WLSOutShroudPhysical_%i",ims);
	       G4VPhysicalVolume* WLSOutPhysical = new G4PVPlacement(0,
	    		   ms_positions->at(ims),
	    		   WLSOutLogical,
	    		   WLSOutNamePhysical,
	    		   fMotherLogical,
	    		   false,
	    		   0);

	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSInside_surf_in",fMotherPhysical,WLSInPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSInside_surf_out",WLSInPhysical,fMotherPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSOutside_surf_in",fMotherPhysical,WLSOutPhysical,fWLSoptSurf);
	       new G4LogicalBorderSurface("LArInstr_MSNylon_WLSOutside_surf_out",WLSOutPhysical,fMotherPhysical,fWLSoptSurf);

       }
       }

}

/// Methods imported from the MPIKLarGe class
/// Optical properties of LAr in several places
void GEGeometryLArInstrumentation::RegisterArgonOpticalProperties()
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
	  G4double LAr_LY_scale = fDetectorDB->GetLArInstArgonLYScale();
	  photon_yield = 28120. * LAr_LY_scale;

	  tau_s = 5.95*ns;
	  tau_l = 922*ns;

	  G4double LAr_att_scale = fDetectorDB->GetLArInstArgonAbsLScale();
    if (LAr_att_scale != 1.0) {
      MGLog(routine) << "Scaling XUV argon attenuation length by a factor of " << LAr_att_scale << endlog;
    }

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
	  G4double LAr_ABSL_vis = 1000*m;
	  LAr_ABSL_xuv *= LAr_att_scale;

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
    MGLog(routine) << "XUV attenuation length: " << LAr_ABSL_xuv/cm << " cm" << endlog;
    MGLog(routine) << "VIS attenuation length: " << LAr_ABSL_vis/m << " m" << endlog;

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
	  // Value used was provided by Mark
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

	  G4Material::GetMaterial("LiquidArgon")->SetMaterialPropertiesTable(myMPT1);

	  G4Material::GetMaterial("LiquidArgon")->GetIonisation()->SetBirksConstant(5.1748e-4*cm/MeV);

}

G4double GEGeometryLArInstrumentation::LArRefIndex(const G4double lambda)
{
  return ( sqrt(LArEpsilon(lambda)) ); // square root of dielectric constant
}

// Calculates the dielectric constant of LAr from the Bideau-Sellmeier formula.
// See : A. Bideau-Mehu et al., "Measurement of refractive indices of Ne, Ar,
// Kr and Xe ...", J. Quant. Spectrosc. Radiat. Transfer, Vol. 25 (1981), 395

G4double GEGeometryLArInstrumentation::LArEpsilon(const G4double lambda)
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

G4double GEGeometryLArInstrumentation::LArRayLength(const G4double lambda,const
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

G4double GEGeometryLArInstrumentation::ArScintillationSpectrum(const G4double kk)
{
  G4double waveL;
  waveL =exp(-0.5*((kk-128.0)/(2.929))*((kk-128.0)/(2.929)));
  return waveL;
}

void GEGeometryLArInstrumentation::AttachOpticalSurface(G4LogicalVolume *log_vol) {
  // check if the logical volume actually exists. If not return without action
  if (!log_vol) return;

  // check iteratively. Only need to attach surfaces for Teflon, Sylicon and Copper. For the rest we have no values
  if (log_vol->GetMaterial()->GetName() == "MetalCopper") {
    new G4LogicalSkinSurface(log_vol->GetName() + "_surf",log_vol,fCuOptSurface);
  } else if (log_vol->GetMaterial()->GetName() == "Teflon") {
    new G4LogicalSkinSurface(log_vol->GetName() + "_surf",log_vol,fTefOptSurface);
  } else if (log_vol->GetMaterial()->GetName() == "MetalSilicon") {
    new G4LogicalSkinSurface(log_vol->GetName() + "_surf",log_vol,fSiOptSurface);
  } else if (log_vol->GetMaterial()->GetName() == "Nylon") {
    new G4LogicalSkinSurface(log_vol->GetName() + "_surf",log_vol,fNylonOptSurf);
  } else {
	  MGLog(warning) << "No optical surface attached to volume " << log_vol->GetName()
			  << " due to inexistent reflectivity for material \'" << log_vol->GetMaterial()->GetName()
			  << "\'." << endlog;
  }
  // anything else just ignore
}

//copied from GEGSLArGeOptical.cc
void GEGeometryLArInstrumentation::InitializeTPBSpectra()
{
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
    successfulInitialization = true;
    MGLog(routine) << "TPB re-emission spectrum ( " << fTPBspec->GetN()
							      << " points) successfully loaded from file." << endlog;
    /** Uncomment for debugging purposes
		fVM2000spec->Print();
     */
  } else {
    successfulInitialization = false;
    MGLog(warning) << "TPB re-emission spectrum failed to load from [" << pathFile.data() << "]." << endlog;
    MGLog(warning) << "Cross-check that this is intended... otherwise the output won't be reliable." << endlog;
  }
}

//copied from GEGSLArGeOptical.cc
G4double GEGeometryLArInstrumentation::TPBEmissionSpectrum(G4double energy)
{
  if (successfulInitialization)
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
