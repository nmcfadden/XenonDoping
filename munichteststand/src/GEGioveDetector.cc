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
/**
 * 
 * REVISION:
 *
 * June 24, 2009  Alan Poon		First version
 * Feb  26, 2010  Alan Poon	    Added additional shielding configurations
 *
 */
//---------------------------------------------------------------------------//
//

//#include "globals.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"

#include "gerdageometry/GEGeometrySD.hh"
#include "geometry/MGGeometryGlobals.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "munichteststand/GEGioveDetector.hh"
#include "munichteststand/GEGioveDetectorMessenger.hh"

using namespace CLHEP;

//---------------------------------------------------------------------------//

GEGioveDetector::GEGioveDetector() : MGGeometryDetector("")
{
  //set defaults

  shielding_config = 0;
  num_layers = 10;
  box_height=25.*cm;
  box_width=25.*cm;
  layer_thickness=5.*cm;

  theMessenger = new GEGioveDetectorMessenger(this);
}

//---------------------------------------------------------------------------//

GEGioveDetector::~GEGioveDetector()
{
  delete theMessenger;
}

//---------------------------------------------------------------------------//

void GEGioveDetector::ConstructDetector()
{
  // Retrieve materials from the Table
  //G4Material* Vacuum = G4Material::GetMaterial("Vacuum");
  //G4Material* Brass = G4Material::GetMaterial("Brass");
  //G4Material* N2Gas = G4Material::GetMaterial("NitrogenGas");
  G4Material* Ge = G4Material::GetMaterial("NaturalGe");
  //G4Material* GeLi = G4Material::GetMaterial("Germanium/Lithium");
  G4Material* Pb = G4Material::GetMaterial("MetalLead");
  //G4Material* Fe = G4Material::GetMaterial("MetalIron");
  G4Material* Cu = G4Material::GetMaterial("MetalCopper");
  //G4Material* Al = G4Material::GetMaterial("MetalAluminium");
  //G4Material* Vespel = G4Material::GetMaterial("Vespel");
  //G4Material* Acrylic = G4Material::GetMaterial("Acrylic");
  G4Material* PE = G4Material::GetMaterial("PE");
  G4Material* BPE = G4Material::GetMaterial("BoronPolyethylene");
  //G4Material* BPE10 = G4Material::GetMaterial("BoronPolyethylene10");
  G4Material* BPE5 = G4Material::GetMaterial("BoronPolyethylene5");
  G4Material* LiPE = G4Material::GetMaterial("LithiumPolyethylene");
  //G4Material* Teflon = G4Material::GetMaterial("Teflon");
  G4Material* PlasticScintillator = G4Material::GetMaterial("PlasticScintillator");
  G4Material* Air = G4Material::GetMaterial("Air");

 // Visualization attributes
 // G4VisAttributes * red= new G4VisAttributes( G4Colour(255/255. ,0/255. ,12/255. ));
 // G4VisAttributes * Brasscolour = red;

 // G4VisAttributes * grass= new G4VisAttributes( G4Colour(0/255. ,239/255. ,0/255. ));
 // G4VisAttributes * Cucolour = grass;

 // G4VisAttributes * purple= new G4VisAttributes( G4Colour(187/255. ,28/255. ,0/255. ));
 // G4VisAttributes * GeLicolour = purple;

 // G4VisAttributes * redbrown = new G4VisAttributes( G4Colour(123/255. ,45/255. ,65/255. ));
 // G4VisAttributes * Fecolour = redbrown;

 // G4VisAttributes * blue = new G4VisAttributes( G4Colour(0/255. ,0/255. ,255/255. ));
 // G4VisAttributes * N2Gascolour = blue;

 // G4VisAttributes * gray= new G4VisAttributes( G4Colour(210/255. ,213/255. ,210/255. ));
 // G4VisAttributes * Alcolour = gray;

 // G4VisAttributes * yellowgray= new G4VisAttributes( G4Colour(214/255. ,214/255. ,207/255. ));
 // G4VisAttributes * Vespelcolour = yellowgray;

 // G4VisAttributes * indigo= new G4VisAttributes( G4Colour(0/255. ,0/255. ,190/255. ));
 // G4VisAttributes * Acryliccolour = indigo;

	G4VisAttributes * skyblue = new G4VisAttributes( G4Colour(175/255. ,164/255. ,255/255. ));
	G4VisAttributes * Vacuumcolour = skyblue;
	
	G4VisAttributes * darkviolet = new G4VisAttributes( G4Colour(64/255. ,0/255. ,64/255. ));
	G4VisAttributes * Pbcolour = darkviolet;
	
	G4VisAttributes * violet= new G4VisAttributes( G4Colour(224/255. ,0/255. ,255/255. ));
	G4VisAttributes * Gecolour = violet;

  //------------------------------------------------------ volumes
  //------------------------------ experimental hall (world volume) 
  G4double expHall_x = 1*m;
  G4double expHall_y = 1*m;
  G4double expHall_z = 1*m;
  G4Box* experimentalHall_box
    = new G4Box("expHall_box", expHall_x, expHall_y, expHall_z);
  G4LogicalVolume* experimentalHall_log = new 
    G4LogicalVolume(experimentalHall_box, Air, "expHall_log", 0, 0, 0);
//  experimentalHall_log -> SetVisAttributes(G4VisAttributes::Invisible);
  experimentalHall_log -> SetVisAttributes(Vacuumcolour);
	
  G4Box* *layer_box = new G4Box*[num_layers];
  G4LogicalVolume* *layer_log_volume = new G4LogicalVolume*[num_layers];
  G4Material* *layer_material = new G4Material*[num_layers];
	
	if ( (num_layers < 5) || ( (shielding_config>3)  && (num_layers != 10)) ){  
	  // cases where shielding_config >3 have fixed number of layers
		MGLog(error) << "The number of shielding layers is " << shielding_config << 
		  " and is not correct for the chosen shielding configuration" << endlog;
		MGLog(error) << "Please check and run MaGe again!" << endlog;
		G4Exception("GEGioveDetector::ConstructDetector()","err001",FatalException,"Exit MaGe");	
	}
	
	if (shielding_config == 0){
		
		// Air, PE, Cu, PE, Pb, PE, Pb, PE, Pb, PE...
		layer_material[0] = Air;
		layer_material[1] = PE;
		layer_material[2] = Cu;
		layer_material[3] = PE;
		layer_material[4] = Pb;
		for (G4int j=5; j<num_layers; j++){
			layer_material[j]=Pb;
			if ( j%2 ){ layer_material[j] = PE;}
		}
		
	} else if (shielding_config == 1) {
		
		// same with option 0 above, but with borated PE
		// Air, BPE, Cu, BPE, Pb, BPE, Pb, BPE, Pb, BPE...
		layer_material[0] = Air;
		layer_material[1] = BPE;
		layer_material[2] = Cu;
		layer_material[3] = BPE;
		layer_material[4] = Pb;
		for (G4int j=5; j<num_layers; j++){
			layer_material[j]=Pb;
			if ( j%2 ){ layer_material[j] = BPE;}
		}
		
    } else if (shielding_config == 2) {
		
		// similar option 1 above, but with Cu closer to the detector:
		// Air, Cu, BPE, Pb, BPE, Pb, BPE, Pb, BPE, Pb
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = BPE;
		layer_material[3] = Pb;
		layer_material[4] = BPE;
		for (G4int j=5; j<num_layers; j++){
			layer_material[j]=BPE;
			if ( j%2 ){ layer_material[j] = Pb;}
		}
		
    } else if (shielding_config == 3) {
		
		// More copper near the Ge crystal
		// Air, Cu, Cu, BPE, Pb, BPE, Pb, BPE, Pb, BPE
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = Cu;
		layer_material[3] = BPE;
		layer_material[4] = Pb;
		for (G4int j=5; j<num_layers; j++){
			layer_material[j]=Pb;
			if ( j%2 ){ layer_material[j] = BPE;}
		}
		
	} else if (shielding_config == 4) {
		
		// this is one possible reference design.  
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = Pb;
		layer_material[4] = PE;
		layer_material[5] = PE;
		layer_material[6] = Pb;
		layer_material[7] = PE;
		layer_material[8] = Pb;
		layer_material[9] = Air;

	}  else if (shielding_config == 5) {
		
		// same as shielding_config =4 above, but with PE replaced by BPE
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = Pb;
		layer_material[4] = BPE5;
		layer_material[5] = BPE5;
		layer_material[6] = Pb;
		layer_material[7] = BPE5;
		layer_material[8] = Pb;
		layer_material[9] = Air;
		
	}  else if (shielding_config == 6) {
		
		// An alternate design
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = BPE5;
		layer_material[4] = Pb;
		layer_material[5] = BPE5;
		layer_material[6] = BPE5;
		layer_material[7] = Pb;
		layer_material[8] = Pb;
		layer_material[9] = Air;
		
	} else if (shielding_config == 7) {
		
		// similar to sheilding_config=6 above, but with one fewer layer of BPE
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = BPE5;
		layer_material[4] = Pb;
		layer_material[5] = BPE5;
		layer_material[6] = Pb;
		layer_material[7] = Pb;
		layer_material[8] = Air;
		layer_material[9] = Air;
		
	} else if (shielding_config == 8) {
		
		// similar to sheilding_config=5 above, but with BPE instead of BPE5
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = Pb;
		layer_material[4] = BPE;
		layer_material[5] = BPE;
		layer_material[6] = Pb;
		layer_material[7] = BPE;
		layer_material[8] = Pb;
		layer_material[9] = Air;
		
	} else if (shielding_config == 9) {
		
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = Pb;
		layer_material[4] = BPE5;
		layer_material[5] = Pb;
		layer_material[6] = BPE5;
		layer_material[7] = Pb;
		layer_material[8] = Air;
		layer_material[9] = Air;
		
	} else if (shielding_config == 10) {
		
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = BPE5;
		layer_material[4] = Pb;
		layer_material[5] = BPE5;
		layer_material[6] = Pb;
		layer_material[7] = Pb;
		layer_material[8] = Air;
		layer_material[9] = Air;
		
	} else if (shielding_config == 11) {
		
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = Pb;
		layer_material[4] = BPE5;
		layer_material[5] = BPE5;
		layer_material[6] = Pb;
		layer_material[7] = BPE5;
		layer_material[8] = Pb;
		layer_material[9] = Air;
		
	} else if (shielding_config == 12) {
		
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = BPE5;
		layer_material[4] = Pb;
		layer_material[5] = BPE5;
		layer_material[6] = Pb;
		layer_material[7] = BPE5;
		layer_material[8] = Pb;
		layer_material[9] = Air;
		
	} else if (shielding_config == 13) {
		
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = BPE5;
		layer_material[4] = Pb;
		layer_material[5] = PE;
		layer_material[6] = Pb;
		layer_material[7] = PE;
		layer_material[8] = Pb;
		layer_material[9] = Air;
		
	} else if (shielding_config == 14) {
		
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = PE;
		layer_material[4] = Pb;
		layer_material[5] = PE;
		layer_material[6] = Pb;
		layer_material[7] = PE;
		layer_material[8] = Pb;
		layer_material[9] = Air;
		
	} else if (shielding_config == 15) {
		
		layer_material[0] = Air;
		layer_material[1] = Cu;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = LiPE;
		layer_material[4] = Pb;
		layer_material[5] = PE;
		layer_material[6] = Pb;
		layer_material[7] = PE;
		layer_material[8] = Pb;
		layer_material[9] = Air;
		
	} else if (shielding_config == 16) {
		
		layer_material[0] = Air;
		layer_material[1] = Pb;
		layer_material[2] = PlasticScintillator; 
		layer_material[3] = LiPE;
		layer_material[4] = Pb;
		layer_material[5] = PE;
		layer_material[6] = Pb;
		layer_material[7] = PE;
		layer_material[8] = Pb;
		layer_material[9] = Air;

	} else {
		
		MGLog(error) << "The shielding configuration " << shielding_config << " does not exist" << endlog;
		MGLog(error) << "Please check and run MaGe again!" << endlog;
		G4Exception("GEGioveDetector::ConstructDetector()","err002",FatalException,"Exit MaGe");		
	}
	
	
	G4double total_half_width = ((G4double) (num_layers-1)) * layer_thickness + box_width/2. ; // box_width is the width of inner empty region ("0th layer")
	G4double total_half_height = ((G4double) (num_layers-1)) * layer_thickness + box_height/2. ; // box_width is the height of inner empty region
	
	if ( (total_half_width > expHall_x) ||  (total_half_width > expHall_y) || (total_half_height > expHall_z) ){
		MGLog(error) << "World mother volume too small" << endlog;
		MGLog(error) << "Please check and run MaGe again!" << endlog;
		G4Exception("GEGioveDetector::ConstructDetector()","err003",FatalException,"Exit MaGe");
	}	
	
	for (G4int i=num_layers-1; i>=0; i--){	
	
		char c1[64];
		char c2[64];
		sprintf(c1,"shield_layer_%i",i);
		sprintf(c2,"shield_layer_log_%i",i);
        G4String layer_name(c1);
		G4String layer_logname(c2);
		
		G4double hw = ((G4double) i)*layer_thickness + box_width/2. ;
		G4double hh = ((G4double) i)*layer_thickness + box_height/2. ;
 
		layer_box[i] = new G4Box(layer_name, hw, hw, hh);
		layer_log_volume[i] = new G4LogicalVolume(layer_box[i],layer_material[i], layer_logname, 0, 0, 0);
		layer_log_volume[i]->SetVisAttributes(Pbcolour);
		if (i==(num_layers-1)){
			new G4PVPlacement(0, G4ThreeVector(), layer_log_volume[i],layer_name,experimentalHall_log,false,0);
		} else {
			new G4PVPlacement(0, G4ThreeVector(), layer_log_volume[i],layer_name,layer_log_volume[i+1],false,0);
		}
		
	}

  // ---------------------------------------------------------------------
  // --- a Ge crystal
  // ---------------------------------------------------------------------

    
 	
 G4Tubs* crystal_tubs = new G4Tubs("crystal_tubs", 0.,  4.*cm,  4.*cm, 0, 2*M_PI);  // 8 cm diameter x 8 cm long Ge crystal

 G4LogicalVolume* crystal_log 
    = new G4LogicalVolume(crystal_tubs, Ge, "crystal_log", 0, 0, 0);
 crystal_log -> SetVisAttributes(Gecolour);

 new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), crystal_log, "crystal", layer_log_volume[0], false, 0);
	
  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  
  G4SDManager* SDman   =  G4SDManager::GetSDMpointer();
  G4String CrystalSDname      = "/mydet/gerda/gecrystal";
  GEGeometrySD* CrystalSD      = new GEGeometrySD(CrystalSDname);
  SDman->AddNewDetector(CrystalSD);
  crystal_log-> SetSensitiveDetector(CrystalSD);
  
  SetDetectorLogical(experimentalHall_log);
  SetDetectorName("ExperimentalHall");
 
	
	delete [] layer_box;
	delete [] layer_log_volume;
	delete [] layer_material;
	
}

//---------------------------------------------------------------------------//
