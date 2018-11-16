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
// $Id: MGManagerDetectorConstruction.cc,v 1.32 2009-07-23 18:01:49 awpoon Exp $
//
// CLASS IMPLEMENTATION:  MGManagerDetectorConstruction.cc
//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: rhenning
 * CONTACT:
 * FIRST SUBMISSION: Mon Apr 19 14:03:00 PDT 2004
 *
 * REVISION:
 *
 * 04-19-2004, Created, R. Henning
 * 05-03-2004, Kareem
 *             First editing after initial creation
 * 07-02-2004, Kareem
 *		   This class now references the base class MJGeometryDetector.
 *               All detectors (e.g., clover, 800g, MEGA) are derived from the
 *               MJGeometryDetector class. They still need individual headers,
 *               of course, but the bookkeeping from the MJGeometryMessenger
 *               point of view is much simpler.
 * 08-05-2004, Added MJGeometryCloverInNaIBarrel as geometry option, R. Henning
 * 11-02-2004  Modified in order to skip database, if wanted, L. Pandola
 * 11-03-2004  The name of theDetectorPhysical is theDetector->
 *             GetDetectorName() and not
 *             necessarily "theDetector" (which is default), Luciano
 * 11-08-2004  Registered Gerda Geometry, X. Liu
 * 11-24-2004  Registered GerdaCrystalHole, C. Tomei
 * 11-26-2004  Registered GerdaArrayHole, C. Tomei
 * 12-01-2004  Registered idealCoax, R. Henning
 * 12-13-2004  Changed MJGeometryIdealCoaxCrystal to MGGeometryIdealCoaxCrystal
 *                R. Henning
 * 01-07-2005  Registsred GerdaArrayHolePhaseI, Luciano
 * 1-09-2005   Added MJ57Banger, R. Henning
 * 1-12-2005   Added G4VisAttributes for world volume
 *             Added small offset (eps) for detector placement, R. Henning
 * 1-19-2005   Registered new Gerda geometry from Kevin
 *             and removed the old ones
 * 2-09-2005   Registered "Sensitive region" for cut-per-region approach,
 *             Luciano
 * 2-15-2005   Changed MGGeometryIdealCoaxCrystal to
 *             MJGeometryIdealCoaxWithShield, R. Henning
 * 2-24-2005   Removed Registration of the Region (not needed: it is done in the
 *             constructor of G4Region), Luciano & Kevin
 * 4-07-2005   Registered new geometry, Luciano
 * 5-13-2005   Registered new geometry, Luciano
 * 7-07-2005   Enlarged world volume, Luciano
 * 09-28-2005  Added cloverInShield, A. Schubert
 * 11-21-2005  Added MJRDCrystalColumn and MJRDCryostat, R. Henning
 * 01-18-2005  New detector registered (MCNPTest), Luciano
 * 07-25-2006  Added LLNL Detector, R. Henning
 * 10-12-2006  Modified in order to allow the definition of new materials from
 *             files, L. Pandola
 * 12-12-2006  Modified to include the possibility to read a simple geometry from
 *             an external file, L. Pandola
 * 02-21-2007  Added MJMEGA, A. Schubert
 * 02-22-2007  Added Bruno, Dario and Corrado detectors to Munichteststand, Dusan
 * 06-26-2007  Added a sandwhich Ge/scintillator detector setup at Hades (HadesScintillator), Kevin
 * 06-28-2007  Registered GDLStand, Luciano
 * 09-04-2007  Removed the definition of SensitiveRegion, to get rid of the problem reported
 *             by M. Marino if /run/verbose is set to 2. Definition now must be done in the
 *             concrete istances of geometry. Luciano
 * 09-28-2007  Enabled GDML. Jing
 * 10-15-2007  Registered UWCountingFacility, A. Schubert
 * 10-15-2007  Registered UWOrtecGMX, A. Schubert
 * 10-29-2007  assign sensitive detector to ONE volume read from GDML file. Jing
 * 10-30-2007  added GDML configuration setup commands. Jing
 * 01-28-2008  added method to check for overlapping volumes. Luciano
 * 01-30-2008  added boolean flag to control verbosity of volume overapping check. Luciano
 * 04-28-2008  increased dimensions of the world volume, because the GERDA simulation includes
 *             now the full LNGS Hall A. Luciano
 * 04-30-2008  set the dimension of the world volume using the radius of the detector bounding
 *             sphere. fWorldHalfLen is not longer hard-coded (but is is minimum 10 m).
 *             Luciano&Jason
 * 09-16-2008 Added VT-1 lowbackground detector geometry. Reyco
 * 02-02-2008 Added KURF-BEGe detector geometry. Kevin
 * 05-14-2009 Added HdBEGe detector, Luciano
 * 05-22-2009 Added LNGSBEGe detector, Luciano
 * 06-09-2009 Added Giove, Alan Poon
 * 01-25-2010 Added KURF-BEGe in shield, A. Schubert
 * 02-01-2010 Removed MEGA, R. Henning
 * 03-08-2009 Updates on G4GDML (geant>=4.9.2): updated possibility to construct the detector
              from reading external .gdml file. Also see warning in comment
              in MGManagerDetectorConstruction::Construct method concerning creating sensitive volumes.  Alex
 * 04-06-2010 Added GSOR detector, Matthias L.
 * 04-07-2010 Added KURF-BEGe in room, A. Schubert
 * 04-07-2010 Added GEPAOLO, GEMI, GEPV detectors, Matthias L.
 * 07-07-2010 Added GECRIS detector, Matthias L.
 * 08-27-2010 Added messenger argument to MJGeometryKURFBEGeDetector, A. Schubert
 * 08-31-2010 Added MGGeometryBEGeDetector, A. Schubert
 * 09-07-2010 Added MJDemonstrator, M.Green
 * 02-15-2011 Added TSMJDemModInTank, R. Martin
 * 04-21-2011 Removed detectors (Clover, Simple800g)) that requires PSQL Database.
 *            Removed old detectors (17A, LLNLDetector), R. Henning
 * 11-18-2011 Added parallel geometry functionality, A. Schubert
 * 11-29-2011 Added GEHADESBEGe class, B. Lehnert
 * 06-21-2012 Added OPPI2 and COPPI geometries
 * 02-25-2013 Added PopTop - Ryan
 * 03-08-2013 Added PopTopXYscan - Wenqin
 * 06-12-2013 Added STC geometry, J. MacMullin
 * 07-22-2013 Added Characterization Stand geometry, A. Wasserman
 * 03-26-2014 Added Prototype geometry, J. MacMullin
 * 12-15-2016 Added Inverted coaxial detector, Y. Kermaidic
 * 05-25-2017 Added LGND_1T_Baseline and LGND_200_Baseline geometries
 */
//---------------------------------------------------------------------------//
//

#include <vector>
#include <map>
//  GEANT4 headers
#include "globals.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4IStore.hh"
#include "G4VisAttributes.hh"
#include "G4VisExtent.hh"
//#include "G4ScoreTable.hh"
#include "G4GeometryManager.hh"
#include "G4UserLimits.hh"

//  majorana detector headers
#include "mjgeometry/MJGeometry57Banger.hh"

// We don't include if MaGe is not compiled with database support
#include "mjgeometry/MJGeometryRDDetectorWBasicShield.hh"
#include "mjgeometry/MJGeometryRDCryostat.hh"
#include "mjgeometry/MJGeometryRDCrystalColumn.hh"
#include "mjgeometry/MJGeometryIdealCoaxWithShield.hh"
#include "mjgeometry/MJGeometryLArGe.hh"
#include "mjgeometry/MJGeometrySolidBlock.hh"
#include "mjgeometry/MJGeometryDoubleSolidBlock.hh"
#include "mjgeometry/MJGeometryUWLArGe.hh"
#include "mjgeometry/MJGeometrySLACBD.hh"
#include "mjgeometry/MJGeometryCERN_NA55.hh"
#include "mjgeometry/MJGeometryWIPPnDetector.hh"
#include "mjgeometry/MJGeometryPbShield.hh"
#include "mjgeometry/MJGeometryUWCountingFacility.hh"
#include "mjgeometry/MJGeometryUWOrtecGMX.hh"
#include "mjgeometry/MJGeometryVT1Detector.hh"
#include "mjgeometry/MJGeometryMelissaDetector.hh"
#include "mjgeometry/MJGeometryKURFBEGeDetector.hh"
#include "mjgeometry/MJGeometryKURFBEGeDetectorInShield.hh"
#include "mjgeometry/MJGeometryKURFBEGeDetectorInRoom.hh"
#include "mjgeometry/MJGeometryOPPI2.hh"
#include "mjgeometry/MJGeometryCOPPIs.hh"
#include "mjgeometry/MJGeometryPopTopXYscan.hh"
#include "mjgeometry/MJGeometryThermosiphon.hh"
#include "mjgeometry/MJGeometryPopTop.hh"
#include "mjdemonstrator/MJGeometryDemonstrator.hh"
#include "mjdemonstrator/MJGeometrySTC.hh"
#include "mjdemonstrator/MJGeometryCharacterization.hh"
#include "mjdemonstrator/MJGeometryPrototype.hh"
#include "tonscalegeometry/TSMJDemModInTank.hh"
#include "legendgeometry/LGND_200_Baseline.hh"
#include "legendgeometry/LGND_1T_Baseline.hh"

#include "bacongeometry/BACON_Baseline.hh"

//  gerda detector headers
#include "gerdageometry/GEGeometryGerda.hh"
#include "munichteststand/GEMunichTestStand.hh"
#include "munichteststand/GEMPIKLArGe.hh"
#include "munichteststand/GEGSLArGe.hh"
#include "munichteststand/GEGSLArGeOptical.hh"
#include "munichteststand/GEMCNPTest.hh"
#include "munichteststand/GEDarioDetector.hh"
#include "munichteststand/GEBrunoDetector.hh"
#include "munichteststand/GECorradoDetector.hh"
#include "munichteststand/GEHadesScintillator.hh"
#include "munichteststand/GEGDLStand.hh"
#include "munichteststand/GEHdBEGeDetector.hh"
#include "munichteststand/GELNGSBEGeDetector.hh"
#include "munichteststand/GEHADESBEGeTests.hh"
#include "munichteststand/GEGioveDetector.hh"
//
#include "lngsgesetups/MGSORDetector.hh"
#include "lngsgesetups/MGEPVDetector.hh"
#include "lngsgesetups/MGEMIDetector.hh"
#include "lngsgesetups/MGECRISDetector.hh"
//#include "munichteststand/MGEBERDetector.hh"
//#include "munichteststand/MGEDSGDetector.hh"
//#include "munichteststand/MGEPAOLODetector.hh"

// common detector headers
#include "management/MGManagerDetectorMessenger.hh"
#include "management/MGManagerDetectorConstruction.hh"
#include "geometry/MGGeometryDetector.hh"
#include "geometry/MGGeometryFromFile.hh"
#include "geometry/MGGeometryBEGeDetector.hh"
#include "geometry/MGGeometryICOAXDetector.hh"
//#include "materials/MJMaterial.hh"
#include "materials/MGGerdaLocalMaterialTable.hh"
#include "io/MGLogger.hh"

// biasing headers
#include "geometry/MGImportanceAlgorithm.hh"

//Luciano
#include "G4MaterialTable.hh"

//Kris
#include "materials/MGMJLocalMaterialTable.hh"
#include "materials/MGLGNDLocalMaterialTable.hh"


#ifndef MG_NO_G4GDML
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "G4GDMLParser.hh"
#include "G4SDManager.hh"
#include "gerdageometry/GEGeometrySD.hh"
#include "G4LogicalVolumeStore.hh"
#endif

using namespace std;
using namespace CLHEP;

//---------------------------------------------------------------------------//

MGManagerDetectorConstruction::MGManagerDetectorConstruction():
  //fTheScorer(NULL),
  fTheImpSampAlgorithm(NULL),
  fTheBiasedParticle("neutron"),
  fTheGeomSampler(NULL),
  fUseImportanceSampling(false),
  fUseScoring(false),
  fImpSampIsInitialized(false),
  //fScoringIsInitialized(false),
  fTheImportanceStore(NULL),
  fUseParallelWorld(false),
  //fMGMaterials(0),
  //fNumberOfMaterials(0),
  logicWorld(NULL),
  physiWorld(NULL),
  theDetector(NULL),
  theDetectorPhysical(NULL),
  fParallelWorld(NULL)
{
  detectorMessenger = new MGManagerDetectorMessenger(this);
  localTable = new MGGerdaLocalMaterialTable;
  fExternalMaterials = new std::vector<G4String>;
  fWorldHalfLen = 10.0 * m;
  fDetectorOffset.set(0.0, 0.0, 0.0);
  fWorldMaterial = "Air";
  fFileNameForGeometry="geometry.def";
  fCheckOverlapsVerbosity = false;


}

//---------------------------------------------------------------------------//

MGManagerDetectorConstruction::~MGManagerDetectorConstruction()
{
  //if(fTheScorer) delete fTheScorer;
  if(fTheImpSampAlgorithm) delete fTheImpSampAlgorithm;
  //for(int i=0; i<fNumberOfMaterials; i++) delete fMGMaterials[i];
  //delete []fMGMaterials;
  delete detectorMessenger;
  delete theDetector;
  delete localTable;
  delete fExternalMaterials;
}

//---------------------------------------------------------------------------//

G4VPhysicalVolume *MGManagerDetectorConstruction::Construct()
{
#ifndef MG_NO_G4GDML
   if (fGDMLsourceFile != "") {
      G4GDMLParser parser;
      parser.Read(fGDMLsourceFile);

      //set sensitive detector
      G4SDManager* SDman = G4SDManager::GetSDMpointer();

/*
//    The feature (bug?) of G4-GDML is that it adds ID to physical volume copy in "assemble" tag
//      as in goes. I.e. if you define the detector in .gdml file after defining any other 4 volumes,
//      it will have ID of 4 (starting from 0) and MaGe will crash as it supports only up to 4 crystals,
//      This is why always put the sensitive volume on the first place if used in "assemble" !!
//    This commented part helped to trace this "feature"
//
      const G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
      std::vector<G4VPhysicalVolume*>::const_iterator pvciter;
      G4VPhysicalVolume* pvt;
      for( pvciter = pvs->begin(); pvciter != pvs->end(); pvciter++ )
      {
        pvt = (*pvciter);
        G4cout<<"Alex::"<<pvt->GetName()<<G4endl;
        G4cout<<"Alex:: pvt copy no."<<pvt->GetCopyNo()<<endl;
      }
*/

      const G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
      std::vector<G4LogicalVolume*>::const_iterator lvciter;
      for( lvciter = lvs->begin(); lvciter != lvs->end(); lvciter++ )
      {
//        G4cout<<(*lvciter)->GetName()<<G4endl;
        G4GDMLAuxListType auxInfo = parser.GetVolumeAuxiliaryInformation(*lvciter);
        std::vector<G4GDMLAuxStructType>::const_iterator ipair; // Changed from G4GDMLAuxPairType, June 2017
        for( ipair = auxInfo.begin(); ipair != auxInfo.end(); ipair++ )
        {
          G4String str=ipair->type;
          std::stringstream temp;
          temp << ipair->value;
          G4double val;
          temp >> val;
          //G4double val=ipair->value;
          G4cout << " Auxiliary Information is found for Logical Volume :  "
                 << (*lvciter)->GetName() << G4endl;
          G4cout << " Name of Auxiliary type is  =  " << str << G4endl;
          G4cout << " Value of Auxiliary type is  =  " << val << G4endl;

          if (val==0)
          {
            GEGeometrySD* CrystalSD = new GEGeometrySD((*lvciter)->GetName());
            SDman -> AddNewDetector(CrystalSD);
            (*lvciter)->SetSensitiveDetector(CrystalSD);
            MGLog(routine) << " Volume " << (*lvciter)->GetName()
                           << " set to be a sensitive detector:" << endlog;
          }
        }
      }


      physiWorld = parser.GetWorldVolume();
      physiWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);

      return physiWorld;
   }
   else {
#endif
   if (theDetector == NULL) {
     MGLog(error) << "theDetector was NULL. You must set a detector via the "
                  << "command /MG/geometry/detector [detector]"
#ifndef MG_NO_G4GDML
                  << "or use the command /MG/geometry/GDML/sourceFile [a gdml file]"
                  << "to specify a GDML source file."
#endif
                  << endlog;
     return NULL;
   }

   if(theDetector->GetMaterialsList()=="MJD") {
     MGLog(routine) << "Selected geometry requires building MJ materials from PSQL dump." << endlog;
     BuildAllMJDMaterials();
   }
   else if (theDetector->GetMaterialsList()=="LGND") {
     MGLog(routine) << "Selected geometry requires building LGND materials from PSQL dump." << endlog;
     BuildAllLGNDMaterials();
   }
   else if (theDetector->GetMaterialsList()=="BACON") {
     MGLog(routine) << "Selected geometry requires building BACON materials from PSQL dump." << endlog;
     MGLog(routine) << "\t (also the same as LGND materials table since Neil put some much work into it)" << endlog;
     BuildAllLGNDMaterials();
   }
   else {
     MGLog(routine) << "Selected geometry requires building GERDA Materials from table." << endlog;
     localTable->CreateMaterials();
   }

      if (fExternalMaterials->size() > 0) //ok, now build "user-defined" materials
	{
	  for (size_t ii=0;ii<fExternalMaterials->size();ii++)
	    {
	      G4String fileName = (*fExternalMaterials)[ii];
	      localTable->AddNewMaterial(fileName);
	    }
	}

  return ConstructDetector();

#ifndef MG_NO_G4GDML
   }
#endif
}

//---------------------------------------------------------------------------//

G4VPhysicalVolume *MGManagerDetectorConstruction::ConstructDetector()
{

  // RH: Construct Detector here, *after* materials are defined.
  theDetector->ConstructDetector();

  //This is the bounding radius of the detector, to set the dimensions of the world
  //volume
  G4double detectorBoundingRadius =
    theDetector->GetDetectorLogical()->GetSolid()->GetExtent().GetExtentRadius();
  fWorldHalfLen = max(fWorldHalfLen,detectorBoundingRadius);
  MGLog(debugging) << "Size of the world box: " << 2.0*fWorldHalfLen/m << " m" << endlog;

  //  Create the physical world
  solidWorld = new G4Box( "solidWorld",
                          fWorldHalfLen, fWorldHalfLen, fWorldHalfLen );
  logicWorld = new G4LogicalVolume( solidWorld,
		  G4Material::GetMaterial(fWorldMaterial), "World" );
  fWorldVisAtt = new G4VisAttributes(G4Colour(1,1,1,0));
  fWorldVisAtt->SetVisibility(false);
  logicWorld->SetVisAttributes(fWorldVisAtt);

  physiWorld = new G4PVPlacement(0, G4ThreeVector(), "World",
				 logicWorld, 0, false, 0 );

  // Note 1: geometry tests will complain unless the detector is slightly
  // offset in the world volume. Old versions of this code offset it to
  // (0.001cm, 0.001cm, 0.001cm)
  // Note 2: the name of the Detector is "Detector" by default
  theDetectorPhysical = new G4PVPlacement(0, fDetectorOffset,
					  theDetector->GetDetectorName(),
					  theDetector->GetDetectorLogical(),
					  physiWorld, false, 0 );
  theDetector->SetDetectorPhysical(theDetectorPhysical);
  //  Always return the physical world

  G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
  for(size_t i=0; i<volumeStore->size(); i++) {
    G4VPhysicalVolume* volume = (*volumeStore)[i];
    double maxStep = fPhysVolStepLimits[volume->GetName()];
    if(maxStep > 0.0) {
      MGLog(trace) << "Setting step limits in volume " << volume->GetName() << " to " << maxStep << endlog;
      volume->GetLogicalVolume()->SetUserLimits(new G4UserLimits(maxStep));
    }
  }

  return physiWorld;
}

//---------------------------------------------------------------------------//

void MGManagerDetectorConstruction::BuildAllMJDMaterials()
{
  MGMJLocalMaterialTable* table = new MGMJLocalMaterialTable();
  table->BuildAll();
}

//---------------------------------------------------------------------------//

void MGManagerDetectorConstruction::BuildAllLGNDMaterials()
{
  MGLGNDLocalMaterialTable* table = new MGLGNDLocalMaterialTable();
  table->BuildAll();
}

//---------------------------------------------------------------------------//

void MGManagerDetectorConstruction::SetDetector(const G4String& detectorType )
{
  if(!theDetector) {
        if (detectorType == "solidBlock")
          theDetector = new MJGeometrySolidBlock();
        else if (detectorType == "doubleBlock")
          theDetector = new MJGeometryDoubleSolidBlock();
        else if (detectorType == "LArGe")
          theDetector = new MJGeometryLArGe("LArGe");
        else if (detectorType == "idealCoax")
          theDetector = new MJGeometryIdealCoaxWithShield("IdealCoax", false);
        else if (detectorType == "GerdaArray")
          theDetector = new GEGeometryGerda();
        else if (detectorType == "MunichTestStand")
          theDetector = new GEMunichTestStand();
        else if (detectorType == "MJ57Banger")
          theDetector = new MJGeometry57Banger("57Banger");
        else if (detectorType == "MPIK_LArGe")
          theDetector = new GEMPIKLArGe();
        else if (detectorType == "GS_LArGe")
          theDetector = new GEGSLArGe();
        else if (detectorType == "GS_LArGeOptical")
          theDetector = new GEGSLArGeOptical();
        else if (detectorType == "MCNPTest")
          theDetector = new GEMCNPTest();
        else if (detectorType == "HdBEGe")
          theDetector = new GEHdBEGeDetector();
        else if (detectorType == "LNGSBEGe")
          theDetector = new GELNGSBEGeDetector();
        else if (detectorType == "HADESBEGeTests")
          theDetector = new GEHADESBEGeTests();
        else if (detectorType == "UWLArGe")
          theDetector = new MJGeometryUWLArGe();
        else if (detectorType == "SLACBD")
          theDetector = new MJGeometrySLACBD();
        else if (detectorType == "MJRDCrystalColumn")
          theDetector = new MJGeometryRDCrystalColumn("MJRDCrystalColumn",false);
        else if (detectorType == "MJRDCryostat")
          theDetector = new MJGeometryRDCryostat("MJRDCryostat");
        else if (detectorType == "MJRDBasicShield")
          theDetector = new MJGeometryRDDetectorWBasicShield("MJRDDetectorWBasicShield");
        else if (detectorType == "CERN_NA55")
          theDetector = new MJGeometryCERN_NA55();
        else if (detectorType == "PbShield")
          theDetector = new MJGeometryPbShield();
        else if (detectorType == "WIPPn")
          theDetector = new MJGeometryWIPPnDetector("WIPPn");
        else if (detectorType == "GeometryFile")
          theDetector = new MGGeometryFromFile(fFileNameForGeometry);
        else if (detectorType == "Dario")
          theDetector = new GEDarioDetector();
        else if (detectorType == "Bruno")
          theDetector = new GEBrunoDetector();
        else if (detectorType == "Corrado")
          theDetector = new GECorradoDetector();
        else if (detectorType == "HadesScintillator")
          theDetector = new GEHadesScintillator();
        else if (detectorType == "GDLStand")
          theDetector = new GEGDLStand();
        else if (detectorType == "UWCountingFacility")
          theDetector = new MJGeometryUWCountingFacility("MJUWCountingFacility");
        else if (detectorType == "UWOrtecGMX")
          theDetector = new MJGeometryUWOrtecGMX("UWOrtecGMX");
        else if (detectorType == "Melissa-KURF")
          theDetector = new MJGeometryMelissaDetector("Melissa-KURF");
        else if (detectorType == "VT1-KURF")
          theDetector = new MJGeometryVT1Detector("VT1-KURF");
        else if (detectorType == "BEGe-KURF")
          theDetector = new MJGeometryKURFBEGeDetector("BEGe-KURF", false);
        else if (detectorType == "BEGe-KURF-InShield")
    	  theDetector = new MJGeometryKURFBEGeDetectorInShield("BEGe-KURF-InShield", false);
    	else if (detectorType == "Giove")
    	  theDetector = new GEGioveDetector();
	else if (detectorType == "BEGe-KURF-InRoom")
          theDetector = new MJGeometryKURFBEGeDetectorInRoom("BEGe-KURF-InRoom");
        else if (detectorType == "MJDemonstrator")
          theDetector = new MJGeometryDemonstrator("MJDemonstrator");
	else if (detectorType == "MJSTC")
	  theDetector = new MJGeometrySTC("MJSTC");
	else if (detectorType == "MJPrototype")
	  theDetector = new MJGeometryPrototype("MJPrototype");
	else if (detectorType == "MJCharacterization")
	  theDetector = new MJGeometryCharacterization("MJCharacterization");
	else if (detectorType == "TSMJDemModInTank")
	  theDetector = new TSMJDemModInTank("TSMJDemModInTank");
	//else if (detectorType == "MGEDSG")
	//  theDetector = new MGEDSGDetector();
    	else if (detectorType == "MGSOR")
    	  theDetector = new MGSORDetector();
    	else if (detectorType == "MGEPV")
    	  theDetector = new MGEPVDetector();
    	//else if (detectorType == "MGEPAOLO")
    	//  theDetector = new MGEPAOLODetector();
     	else if (detectorType == "MGEMI")
    	  theDetector = new MGEMIDetector();
    	else if (detectorType == "MGECRIS")
	  theDetector = new MGECRISDetector();
    	//else if (detectorType == "MGEBER")
    	// theDetector = new MGEBERDetector();
        else if (detectorType == "COPPIs")
          theDetector = new MJGeometryCOPPIs();
        else if (detectorType == "OPPI2")
          theDetector = new MJGeometryOPPI2();
        else if (detectorType == "PopTop")
	  theDetector = new MJGeometryPopTop("MJGeometryPopTop");
        else if (detectorType == "PopTopXYscan")
          theDetector = new MJGeometryPopTopXYscan("MJGeometryPopTopXYscan");
        else if (detectorType == "BEGe")
          theDetector = new MGGeometryBEGeDetector("ActiveCrystal0CrystalColumn0");
        else if (detectorType == "ICOAX"){
          theDetector = new MGGeometryICOAXDetector("ICOAX",false);
	}
    	else if (detectorType == "Thermosiphon")
          theDetector = new MJGeometryThermosiphon();
      	else if (detectorType == "LGND_200_Baseline")
          theDetector = new LGND_200_Baseline("LGND_200_Baseline");
        else if (detectorType == "LGND_1T_Baseline")
          theDetector = new LGND_1T_Baseline("LGND_1T_Baseline");
        else if (detectorType == "BACON")
          theDetector = new BACON_Baseline("BACON_Baseline");
        else
	    MGLog(error) << "Couldn't find detector of type \"" << detectorType
		       << "\"!" << endlog;
        MGLog(routine) << "Using the " << detectorType << " detector."<<endlog;
#ifndef MG_NO_G4GDML
        MGLog(routine) << "This setting can be overridden by the command:"
                       << "/MG/geometry/GDML/sourceFile [a gdml file]"
                       << endlog;

        fGDMLdefaultOutput=detectorType; // save detectorType for GDML writing
#endif

    CreateParallelWorld();

  } else {
    MGLog(error) << "Detector already defined." << endlog;
  }
}


//---------------------------------------------------------------------------//

void MGManagerDetectorConstruction::SetScorer(const G4String& /*aScorer*/)
{
   //static int firstTime = 0;
   /*
   if(firstTime == 1 && fTheScorer)
      delete fTheScorer;
   //allows rerunning with dif parameters

   if(aScorer == "G4Scorer")
   {
      fTheScorer = new G4Scorer();
      if(!fTheScorer) MGLog(error) << "Scorer not allocated!" << endlog;
   }
   else  MGLog(error) << aScorer << " scorer not registered." << endlog;
   */
   MGLog(warning) << "Scoring no longer implemented." << endlog;
   //firstTime = 1;
}

//---------------------------------------------------------------------------/

void MGManagerDetectorConstruction::SetImportanceSamplingAlgorithm(
  const G4String& theAlg)
{
  if( theAlg == "G4default") fTheImpSampAlgorithm = NULL;  // default value
  else if( theAlg == "MGdefault" && !fTheImpSampAlgorithm)
  {
    fTheImpSampAlgorithm = new MGImportanceAlgorithm();
  }
  else
  {
    MGLog(error) << theAlg <<" algorithm not registered."<< endlog;
  }
}

//---------------------------------------------------------------------------/

void MGManagerDetectorConstruction::InitializeVarianceReduction()
{
  // This initializes the importance sampling.
  static int firstTime = 0;
  // this must be run AFTER run initialization
  if(!physiWorld) MGLog(error) << "Physical World Not Constructed." << endlog;
  else if(fUseImportanceSampling || fUseScoring)
  {
    if(firstTime ==1)
    {
      fTheGeomSampler->ClearSampling(); // this can only be initialized once
    }
    else
    {
      MGLog(trace) << "Initializing Geometry Sampler with biased particle: "
        << fTheBiasedParticle << endlog;

      if (fUseParallelWorld && fParallelWorld) {
        fTheGeomSampler = new G4GeometrySampler(fParallelWorld->GetWorldVolume(), fTheBiasedParticle);
        fTheGeomSampler->SetParallel(true);
      } else {
        fTheGeomSampler = new G4GeometrySampler(physiWorld, fTheBiasedParticle);
        fTheGeomSampler->SetParallel(false);
      }
    }

    if(!fTheGeomSampler)
    {
      MGLog(error) << "Geometry Sampler not allocated." << endlog;
      MGLog(fatal) << endlog;
    }
    // the importance store will stay the same.  no need to construct
    // it again.
    if(fUseImportanceSampling && !fTheImportanceStore)
    {
      if (fUseParallelWorld && fParallelWorld) {
        fTheImportanceStore = G4IStore::GetInstance(fParallelWorld->GetWorldVolume()->GetName());
      } else {
        fTheImportanceStore = G4IStore::GetInstance();
      }

      if(!fTheImportanceStore)
      {
        MGLog(error) << "Importance Store not allocated." << endlog;
        MGLog(fatal) << endlog;
      }

      // call MGGeometryImportance methods from MGGeometryDetector or
      // MGGeometryParallelWorld
      if (fUseParallelWorld && fParallelWorld) {
        fParallelWorld->SetImportanceValueForRegion(fParallelWorld->GetWorldVolume());
        // construct the rest (these MUST be filled by the user in Construction)
        fParallelWorld->ConstructImportanceStore(*fTheImportanceStore);
      } else {
        theDetector->SetImportanceValueForRegion(physiWorld);
        theDetector->SetImportanceValueForRegion(theDetectorPhysical);
        // construct the rest (these MUST be filled by the user in Construction)
        theDetector->ConstructImportanceStore(*fTheImportanceStore);
      }

    }
    if(fUseImportanceSampling)
    {
      fTheGeomSampler->PrepareImportanceSampling(fTheImportanceStore,
        fTheImpSampAlgorithm);
      fImpSampIsInitialized = true;
    }
    if(fUseScoring)
    {
      //if(!fTheScorer) SetScorer("G4Scorer");
      //fTheGeomSampler->PrepareScoring(fTheScorer);
      //fScoringIsInitialized = true;
    }
    fTheGeomSampler->Configure();
    firstTime = 1;
  }

}
//---------------------------------------------------------------------------/


//---------------------------------------------------------------------------/
void MGManagerDetectorConstruction::SetTheBiasedParticle(const G4String& theParticle)
{
   fTheBiasedParticle = theParticle; // no checking!
}

//---------------------------------------------------------------------------/
void MGManagerDetectorConstruction::PrintScoreTable()
{
  // Prints the scoring table.  This is good to output at the end of the run.
  /*
  G4Scorer* theScorer = dynamic_cast<G4Scorer*>( fTheScorer);

  if(theScorer && fUseScoring && fScoringIsInitialized)
  {
    G4ScoreTable aScoreTable(fTheImportanceStore);
    aScoreTable.Print(theScorer->GetMapGeometryCellCellScorer());
  }
  else
  {
    MGLog(warning) << "Scoring not initialized.  No table to output." << endlog;
  }
  */
}

//---------------------------------------------------------------------------/
void MGManagerDetectorConstruction::DeleteMGS()
{
   // bad hack for now, due to G4's memory management
   if(fTheGeomSampler) delete fTheGeomSampler;
   MGLog(routine) << "Temporary Hack: removed MGS. " << endlog;
}

//---------------------------------------------------------------------------/

void MGManagerDetectorConstruction::CreateParallelWorld()
{

  if ( fUseParallelWorld && theDetector ) {

    fParallelWorld = theDetector->GimmeParallelWorld();

    if (!fParallelWorld) {
      MGLog(warning) << "parallel world is not defined for this geometry!!" << endlog;
    } else {
      MGLog(routine) << "Registering parallel world." << endlog;
      RegisterParallelWorld(fParallelWorld);
    }  // end test of fParallelWorld

  }
}

//---------------------------------------------------------------------------/

void MGManagerDetectorConstruction::AddNewMaterial(G4String filename)
{
  fExternalMaterials->push_back(filename);
}

//---------------------------------------------------------------------------/

#ifndef MG_NO_G4GDML
void MGManagerDetectorConstruction::setGDMLsourceFile( G4String sourcefile)
{
   ifstream fin(sourcefile);
   if (fin.is_open()) {
      fin.close();
      fGDMLsourceFile=sourcefile;
      MGLog(routine)<<"GDML source file is set to be "<<fGDMLsourceFile<<endlog;
      if (theDetector)
         MGLog(routine)<<"The command /MG/geometry/detector "<<fGDMLdefaultOutput
                       <<" has been overridden."<<endlog;
   }
   else
      MGLog(error)<<"failed to read the GDML source file. Please check your setting again."<<endlog;
}

//---------------------------------------------------------------------------//

void MGManagerDetectorConstruction::setGDMLschemaPath( G4String schemapath)
{
   fGDMLschemaPath=schemapath;
   MGLog(routine)<<"GDML schema definition file is set to be "<<fGDMLschemaPath<<endlog;
}

//---------------------------------------------------------------------------//

void MGManagerDetectorConstruction::setGDMLoutputName(G4String outputname)
{
   if (outputname != "") {
      fGDMLoutputName=outputname+".gdml";
      MGLog(routine)<<"GDML output name is set to be "<<fGDMLoutputName<<endlog;
   }
   else {
      fGDMLoutputName="";
      MGLog(routine)<<"GDML output name is set to the default value: "<<fGDMLdefaultOutput<<".gdml"<<endlog;
   }
}

//---------------------------------------------------------------------------//

void MGManagerDetectorConstruction::setGDMLoutputFormat( G4String outputformat)
{
   fGDMLoutputFormat=outputformat;
   MGLog(routine)<<"GDML output format is set to be "<<fGDMLoutputFormat<<endlog;
}

//---------------------------------------------------------------------------//

void MGManagerDetectorConstruction::setGDMLmodularizeLevels( G4String levels)
{
   MGLog(routine)<<"Clean the old lists (if there is) of levels chosen to be modularized..."<<endlog;
   fGDMLlevels.clear();
   stringstream levellist(levels);
   G4String aLevel;
   MGLog(routine)<<"The levels chosen to be modularized are:"<<endlog;
   while (!levellist.eof()) {
      levellist>>aLevel;
      MGLog(routine)<<" \t"<<aLevel<<endlog;
      fGDMLlevels.push_back(atoi(aLevel));
   }
}

//---------------------------------------------------------------------------//

void MGManagerDetectorConstruction::GDMLwrite()
{
   if (fGDMLschemaPath=="")
      fGDMLschemaPath="http://service-spi.web.cern.ch/service-spi/app/releases/GDML/GDML_3_0_0/schema/gdml.xsd";

   G4String output;
   if (fGDMLoutputName=="") {
      if (fGDMLdefaultOutput!="")
         output=fGDMLdefaultOutput+".gdml";
      else
         output="MGdetector.gdml";
   }
   else
      output=fGDMLoutputName;

   G4GDMLParser parser;
   bool bGDMLoutputformat = true;
   if (fGDMLoutputFormat=="false") bGDMLoutputformat = false;
   else bGDMLoutputformat = true;
   try {

      if (!fGDMLlevels.empty()) {
//         parser.AddModule(fGDMLlevel);
         for (size_t i = 0; i < fGDMLlevels.size(); i++) {
             parser.AddModule(i);
         }
         parser.Write(output,physiWorld,bGDMLoutputformat,fGDMLschemaPath);
      }
      else {
         parser.Write(output,physiWorld,bGDMLoutputformat,fGDMLschemaPath);
      }
   }
   catch(logic_error &lerr) {
      MGLog(error) << "Caught an exception: " << lerr.what () << endlog;
   }

}
//---------------------------------------------------------------------------//

#endif

void MGManagerDetectorConstruction::CheckOverlaps()
{
  G4PhysicalVolumeStore* thePVStore = G4PhysicalVolumeStore::GetInstance();
  MGLog(trace) << thePVStore->size() << " physical volumes are defined" << endlog;
  G4bool overlapFlag = false;
  G4int res=1000;
  G4double tol=0.; //tolerance
  for (size_t i=0;i<thePVStore->size();i++)
    {
      //MGLog(trace) << "Going to check: " << (*thePVStore)[i]->GetName() << endlog;
      overlapFlag = (*thePVStore)[i]->CheckOverlaps(res,tol,fCheckOverlapsVerbosity) | overlapFlag;
    }
  if (overlapFlag)
    MGLog(warning) << "Check: there are overlapping volumes" << endlog;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
