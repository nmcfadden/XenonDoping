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
//
// CLASS IMPLEMENTATION:  GEGeometryLArInstTest.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES: Simple example implementation. See header for details.
 *
 *
 */
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: N. Barros
 * CONTACT: nuno *dot* barros *at* tu-dresden *dot* de
 * FIRST SUBMISSION: 11-02-2011, N. Barros
 *
 * REVISION:
 *
 */
//---------------------------------------------------------------------------//

#include "gerdageometry/GEGeometryLArInstTest.hh"
#include "gerdageometry/GEGeometryCryostatParts.hh"
#include "io/MGLogger.hh"

#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4NistManager.hh"

using namespace CLHEP;

GEGeometryLArInstTest::GEGeometryLArInstTest(GEGeometryDetectorDB* theGeometryDB) : fFiberLogical(0),
fInitDone(false) {
	// TODO Auto-generated constructor stub
	fDetectorDB = theGeometryDB;
	fMotherLogical = fDetectorDB->GetDetectorMotherVolume();
	return;
}

GEGeometryLArInstTest::~GEGeometryLArInstTest() {
	if (fFiberLogical) delete fFiberLogical;
}

void GEGeometryLArInstTest::ConstructDesign() {
	// TODO: Review this geometry to be more realistic
	// This design is defined pure and simply by a cylinder of fiber optic around the crystals

	// The construction of the design is obtained by combining two different solids
	// 1. a tube which makes the body
	// 2. A circular shape that makes the bottom and the top.
	// TODO: Implement a more reallistic geometry.

	// The shape is basically a cylinder with rounded edges on the top and bottom.

    G4double design_innerRadius = 240.0*mm;
    G4double fiber_Thickness   	= 1.0*mm;
    G4double design_Height = 0.5*m;
    G4double design_bottom_Bend = 40.0*mm; // The height of the bending part at the bottom
    //G4double design_top_Bend = 40.0*mm; // The height of the bending part at the top
    // Use 1*mm steps to build the tables;
    const G4int poly_npts = 82;

    G4double rin[poly_npts];
    G4double rout[poly_npts];
    G4double zpos[poly_npts];

    G4double z_bottom_outer = -1.0 * (design_Height/2.0) - design_bottom_Bend - fiber_Thickness;
    G4double z_cyl_border 	= -1.0 * (design_Height/2.0);
    G4double z_bottom_inner = z_bottom_outer + fiber_Thickness;
    // Build the function to get the radius of the bottom part
    G4double b=0.;
    //G4double a;
    G4int pos = 0;
    for (int i = 0; i <poly_npts; ++ i) {
    	// Build from bottom to top.
    	// First 40 points is the curved shape at the bottom.

    	if (i<40) {
    		zpos[i] = z_bottom_outer + 1.0*mm*(G4double)i;

    		// polyconenomial scale factor for the outer and inner surfaces
    		//a = (design_innerRadius + fiber_Thickness)/((z_cyl_border-z_bottom_outer)*(z_cyl_border-z_bottom_outer));
    		rout[i] = b * (zpos[i] - z_bottom_outer)*(zpos[i] - z_bottom_outer);
    		if (rout[i] < 0.0) rout[i] = 0.0;
    		if (zpos[i] < z_bottom_inner) {
    			rin[i] = 0.0;
    		} else {
    			b = (design_innerRadius)/((z_cyl_border-z_bottom_inner)*(z_cyl_border-z_bottom_inner));
    			rin[i] = b * (zpos[i] - z_bottom_inner)*(zpos[i] - z_bottom_inner);
    		}
    		if (rin[i] < 0.0) rin[i] = 0.0;
    	}
    	if ((i==40) || (i == 41)) {
    		zpos[i] = (i==40)?(-1.0*design_Height/2.0):(design_Height/2.0);
    		rin[i] = design_innerRadius;
    		rout[i] = design_innerRadius + fiber_Thickness;
    	}
    	// upper part. Similar to lower with a few additional touches.
    	if (i > 41) {
    		zpos[i] = zpos[i-1] + 1.0*mm;
    		pos = 39-(i-42);
    		rout[i] = rout[pos];
    		/*
    		a = (design_innerRadius + fiber_Thickness)/(((-1.0)*z_cyl_border-(-1.0)*z_bottom_outer)*((-1.0)*z_cyl_border-(-1.0)*z_bottom_outer));
    		rout[i] = b * (zpos[i] - (-1.0)*z_bottom_outer)*(zpos[i] - (-1.0)*z_bottom_outer);
    		*/
    		if (rout[i] < 0.0) rout[i] = 0.0;

    		if (zpos[i] > (-1.0)*z_bottom_inner) {
    			rin[i] = 0.0;
    		} else {
    			/*
    			b = (design_innerRadius)/(((-1.0)*z_cyl_border-(-1.0)*z_bottom_inner)*((-1.0)*z_cyl_border-(-1.0)*z_bottom_inner));
    			rin[i] = b * (zpos[i] - (-1.0)*z_bottom_inner)*(zpos[i] - (-1.0)*z_bottom_inner);
    		*/
    			rin[i] = rin[pos];
    		}
    		if (rin[i] < 0.0) rin[i] = 0.0;

    	}
    }

    fFiberSolid = new G4Polycone("FiberInstrSolid",
            			0.0, 2*M_PI,
            			poly_npts,
            			zpos,rin,rout);

    //fMaterialFiber = G4Material::GetMaterial("PlasticScintillator");
    G4double density;
 //   G4int ncomponents;
//    G4double fractionmass;
    std::vector<G4int> natoms;
    std::vector<G4double> fractionMass;
    std::vector<G4String> elements;

    G4NistManager *nistMan = G4NistManager::Instance();

    //--------------------------------------------------
    // Polystyrene
    //--------------------------------------------------

    elements.push_back("C");     natoms.push_back(8);
    elements.push_back("H");     natoms.push_back(8);

    density = 1.050*g/cm3;

    // -- Rough approximation of BCF-10
    fMaterialFiber = nistMan->
            ConstructNewMaterial("Polystyrene", elements, natoms, density);

    elements.clear();
    natoms.clear();


    fFiberLogical = new G4LogicalVolume(fFiberSolid,
                		fMaterialFiber,
                		"LArInstrFiberLogical");
    /*

    fFiberSolid = new G4Tubs("FiberInstrSolid",
                             design_innerRadius,
                             design_innerRadius + fiber_Thickness,
                             design_Height/2.0,
                              0,
                              2*M_PI);

    fMaterialFiber = G4Material::GetMaterial("PlasticScintillator");

	fFiberLogical = new G4LogicalVolume(fFiberSolid,
            		fMaterialFiber,
            		"LArInstrFiberLogical");
   */
	// -- The positioning is specific for this design. Need to get the center position of the strings.
	// There is really no smarter way to this right now, considering how the geometry is implemented
	// The fiber system is centered on the 3-string flange. (-110*mm, 0.0*mm)
	// the Z position is a bit more problematic.

    /*
	MGLog(trace) << " Array depth found to be : " << fDetectorDB->GetArrayDepth()/mm << endlog;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 4; ++j) {
			G4ThreeVector pos = fDetectorDB->GetCrystalPosition(j,i);
			MGLog(trace) << "Position [ " << j << "," << i << " ] : " << pos.x() << " , " << pos.y() << " , " << pos.z	() << endlog;
		}
	}

	G4NistManager* manager = G4NistManager::Instance();
	std::vector<G4String> names = manager->GetNistMaterialNames();
	//int nmat = manager->GetNumberOfMaterials();
	const G4MaterialTable *tbl = G4Material::GetMaterialTable();

	MGLog(trace) << " NIST table : " << endlog;
	for (int i = 0; i < names.size(); ++i) {
		MGLog(trace) << " " << i << " : " << names.at(i)  << endlog;
	}
	MGLog(trace) << " Standard table : " << endlog;
	for (int i = 0; i < tbl->size(); ++i) {
		MGLog(trace) << " " << i << " : " << (tbl->at(i))->GetName() << endlog;
	}
*/
	G4ThreeVector center_flange(-110.0*mm,0.0,145.0*mm);
	fFiberPhysical = new G4PVPlacement(0,
										center_flange,//G4ThreeVector(0,0,0),
	                                         fFiberLogical,
	                                         "LArInstrFiberPhysical",
	                                         fMotherLogical,
	                                         false,
	                                         0);

	 G4Colour lgreen  (0.0,  0.4, 0.0) ;
	 fFiberVisAtt = new G4VisAttributes(lgreen);
	 fFiberVisAtt -> SetVisibility(true);
	 fFiberVisAtt -> SetForceSolid(false);
	 fFiberLogical->SetVisAttributes(fFiberVisAtt);



	 // -- End of geometry definition.
	 // Let's start attaching optical properties to the materials



	    // BCF-10 has no WLS. Therefore it is to be implemented simply as light guide.

	/* Properties taken from Saint Gobain catalogue. : BCF-10

	Core refractive index: 	1.60
	Density: 				1.05
	Cladding material	:	Acrylic
	Cladding refractive index: 1.49
	Cladding thickness		: 4% fiber size
	Numerical aperture		: 0.58
	Trapping efficiency		: 4.4%
	No. of H atoms per cc (core)	: 4.82 x 10^22
	No. of C atoms per cc (core)	: 4.85 x 10^22
	No. of electrons per cc (core)	: 3.4 x 10^23
	Radiation length		: 42 cm
	Operational Temperature	: -20 to + 50

	--

	Emission peak 	:	432 nm
	Decay time		: 	2.7 ns
	1/e length		: 	2.2 m
	# photons/ MeV	:	~8000
	 */

	    // Initialize the stuff from VM2000 input files.

	    // Let's assume the refractive index remains unchanged for all
	    // wavelengths
	    const G4int nEntries = 250;
	    static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;

	    //G4double PPSCHighE = LambdaE /(115*nanometer);

	    G4double PhotonEnergy[nEntries];
	    G4double wlength[nEntries];
	    G4double RefractiveIndexFiber[nEntries];
	    //G4double EmissionFiber[nEntries];
	    G4double AbsLengthFiber[nEntries];

	    for (int i = 0; i < nEntries; ++i) {
	    	wlength[i] = (115.0 + G4double(i)*((600.0 - 115.0)/250.0))*nanometer;
	    	PhotonEnergy[i] = LambdaE/(wlength[i]);
	    	// Assume the same refractive index
	    	RefractiveIndexFiber[i] = 1.60;
	    	AbsLengthFiber[i] = 2.2*m;
	    }


	    // Add entries into properties table
	    G4MaterialPropertiesTable* MPTWLSfiber = new G4MaterialPropertiesTable();
	    MPTWLSfiber->AddProperty("RINDEX",PhotonEnergy,RefractiveIndexFiber,nEntries);
	    MPTWLSfiber->AddProperty("ABSLENGTH",PhotonEnergy,AbsLengthFiber,nEntries);
	    fMaterialFiber->SetMaterialPropertiesTable(MPTWLSfiber);


	  // Attach a series of optical properties to the rest of the
	  // detector otherwise the photons will die at the boundary and no registered hit is done



	 /*** Optical properties of Germanium
	  * Needs to be attached to all deadlayer logical surfaces.
	  *
	  * Found a reference for reflectivity in Germanium. Need to confirm.
	  * http://prola.aps.org/abstract/PR/v160/i3/p602_1
	  *
	  * It quotes a reflectivity quite high for UV light (~60-70%).
	  * */

	    G4double RefractionGe[nEntries];
	    G4double ReflectivityGe[nEntries];
	    G4double EfficiencyGe[nEntries];
	    //GermaniumRefractiveIndexGe[ji]
	    for (int ji=0;ji < nEntries; ++ji)
	    {
	    	RefractionGe[ji] =this->CalculateRefractiveIndexGe(wlength[ji]/nanometer);
	    	ReflectivityGe[ji] = 0.40;
	    	EfficiencyGe[ji] = 0.;
	    }

	    G4MaterialPropertiesTable *germOptTable = new G4MaterialPropertiesTable();
	    germOptTable->AddProperty("RINDEX",PhotonEnergy,RefractionGe,nEntries);
  germOptTable->AddProperty("REFLECTIVITY",PhotonEnergy,ReflectivityGe,nEntries);
  germOptTable->AddProperty("EFFICIENCY",PhotonEnergy,EfficiencyGe,nEntries);
  G4OpticalSurface* germOptSurface
    = new G4OpticalSurface("Germ surface",glisur,polished,dielectric_metal);
  germOptSurface->SetMaterialPropertiesTable(germOptTable);


	 G4int ngedet = fDetectorDB->GetCrystalNumber();
	 for (int i = 0; i < ngedet ; ++i) {
		 	G4LogicalVolume *dl_log = fDetectorDB->GetCrystalDeadLayerLogical(i);
		 	std::ostringstream oss;
		 	oss << "germanium_surf_" << i;
		 	new G4LogicalSkinSurface(oss.str(),dl_log,germOptSurface);
		 	  //new G4LogicalBorderSurface("germanium_surf",deadLayerPhys,argonPhys,germOptSurface);
	 }


	 /*
	  // Make an optical surface nonetheless
	  G4OpticalSurface* fiberOptSurface
	    = new G4OpticalSurface("LArInstrFiberSurface",glisur,polished,dielectric_metal);
	  fiberOptSurface->SetMaterialPropertiesTable(fiberOptTable);

	  new G4LogicalSkinSurface("fiber_surf",fFiberLogical,fiberOptSurface);
*/

	  fSDLogicVec.push_back(fFiberLogical);
	  fSDLogicNameVec.push_back("Fiber");
	  fSDLogicInstanceVec.push_back(1);
}

 G4LogicalVolume *GEGeometryLArInstTest::GetLArInstrSDLogical(G4int index) {
	return fSDLogicVec.at(index);
	//return fFiberLogical;
}
G4int GEGeometryLArInstTest::GetNumSDLogicVols() {
	return fSDLogicVec.size();
}
G4String GEGeometryLArInstTest::GetLogicalSDName(G4int index) {
	return fSDLogicNameVec.at(index);
}
G4int GEGeometryLArInstTest::GetNLogicalInstances(G4int index) {
	return fSDLogicInstanceVec.at(index);
}


/// Data from Ge
G4double GEGeometryLArInstTest::CalculateRefractiveIndexGe(G4double lambda) {
	// Taken from http://refractiveindex.info/?group=CRYSTALS&material=Ge
	// Only verified down to 200 nm, but should still be approximately valid down to 100 nm
	G4double x = lambda/micrometer;
	G4double n = sqrt( 9.28156 + 6.72880*pow(x,2)/(pow(x,2)-0.44105) + 0.21307*pow(x,2)/(pow(x,2)-3870.1) );
	return n;
}

/// WLS data imported from LArGe

void GEGeometryLArInstTest::InitializeVM2000Spectrum()
{
  char* path = getenv("MGGENERATORDATA");
  if (!path)
    {
      MGLog(warning)<< "MGGENERATORDATA environment variable not set!" << endlog;
      //path = "/d1/neutrino/luciano/MaGe/generators/data";
    }
  G4String pathString(path);
  G4String pathFile = pathString + "/VM2000_em.dat";
  std::ifstream file(pathFile);
  if (file.is_open())
    {
      G4double aa,bb,cc,dd,ee,ff,gg,hh;
      for (G4int i=0;i<npoints;i++)
        {
          file >> aa >> bb >> cc >> dd >> ee >> ff >> gg >> hh;
          frequencyV[i] = gg*nanometer;
          efficiencyV[i] = hh;
        }
      file.close();
      fInitDone = true;
      MGLog(routine) << "VM2000 data successfully read from file" << endlog;
    }
  else
    {
      MGLog(warning) << "Data file not found!" << endlog;
      fInitDone = false;
    }
}


G4double GEGeometryLArInstTest::VM2000EmissionSpectrum(G4double energy)
{
  G4int j=0;
  if (fInitDone)
    {
      //Here the data are loaded
      static const G4double LambdaE = twopi *1.973269602e-16 * m * GeV;
      G4double targetf = LambdaE/energy;

      if (targetf < frequencyV[0] || targetf > frequencyV[npoints-1])
        return 0.0;

      for(j=0;j<npoints-1;j++)
        {
          if (frequencyV[j]<targetf && targetf <= frequencyV[j+1]) break;
        }
      G4double eff = (targetf-frequencyV[j])
        *(efficiencyV[j+1]-efficiencyV[j])/(frequencyV[j+1]-frequencyV[j]);
      eff += efficiencyV[j];
      return eff;
    }
  else
    {
      return 0.2;
    }
}

G4double GEGeometryLArInstTest::CalculateWLSmfp(G4double yield)
{
  //e(-1mm/ll) = 0.40 --> ll = 1.1 mm
  G4double totalPath = 1*mm; //hardcoded, for now
  if (yield < 0. || yield > 1.)
    {
      MGLog(warning) << "Yield cannot be larger than 100%!" << endlog;
      MGLog(warning) << "Yield scaled to 100%" << endlog;
    }
  if (yield == 0) return 10.*m;
  if (yield == 1) return 0.01*mm;
  G4double help = std::log(1.-yield);
  G4double mfp = -1*totalPath/help;
  //MGLog(debugging) << "WLS mean free path = " << mfp/mm << " mm" << endlog;
  return mfp;
}

