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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 02-06-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 07-12-2006, added teststand "collimator" to be implemented in Galatea, Manuela 
 */

// ---------------------------------------------------------------------------

#include "globals.hh"

#include "geometry/MGGeometryDetector.hh"
//#include "geometry/MGGeometryDetectorConstruction.hh"

#include "munichteststand/GEMunichTestStand.hh"

// --------------------------------------------------------------------------- 

GEMunichTestStand::GEMunichTestStand() : MGGeometryDetector("")
{

  fTestStandDB   = new GEMunichTestStandDB(); 
  fLab           = new GEMunichTestStandLab(); 

}


// --------------------------------------------------------------------------- 

GEMunichTestStand::~GEMunichTestStand()
{
/*
  delete fTestStandDB; 
  delete fLab; 

  if (fTestStandDB -> GetTestStandType() == "simple")
    delete  fTestStandSimple; 

  if (fTestStandDB -> GetTestStandType() == "ln2")
    delete fTestStandLN2; 

  if (fTestStandDB -> GetTestStandType() == "vacuum")
    delete fVacuumChamber; 

  if (fTestStandDB -> GetTestStandType() == "coincidence")
    delete  fTestStandCoincidence; 
  
  if (fTestStandDB -> GetTestStandType() == "gerdalinchen2") 
    delete fTestStandGERDAlinchenII; 

  if (fTestStandDB -> GetTestStandType() == "siegfried") 
    delete fTestStandSiegfried;

  if (fTestStandDB -> GetTestStandType() == "collimator") 
    delete fTestStandCollimator;
*/
}
  
// --------------------------------------------------------------------------- 
 

void GEMunichTestStand::ConstructDetector()
{

  // construct lab

  fLab -> ConstructLab(); 
  
  fTestStandDB -> SetMotherVolume(fLab -> GetLabVolume()); 
 
  // construct database 

  fTestStandDB -> ConstructTestStandDB(); 

  // construct test stand 

  if (fTestStandDB -> GetTestStandType() == "simple")
    {
      fTestStandSimple =  new GEMunichTestStandSimple(fTestStandDB); 
      fTestStandSimple -> ConstructTestStand(); 
    }

  if (fTestStandDB -> GetTestStandType() == "ln2")
    {
      fTestStandLN2 =  new GEMunichTestStandLN2(fTestStandDB); 
      fTestStandLN2 -> ConstructTestStand(); 
    }
  
  if (fTestStandDB -> GetTestStandType() == "vacuum")
    {
      fVacuumChamber =  new GEMunichTestStandVacuumChamber(fTestStandDB); 
      fVacuumChamber -> ConstructVacuumChamber(); 
    }

  if (fTestStandDB -> GetTestStandType() == "coincidence")
    {
      fTestStandCoincidence =  new GEMunichTestStandCoincidence(fTestStandDB); 
      fTestStandCoincidence -> ConstructTestStand(); 
    }

  if (fTestStandDB -> GetTestStandType() == "gerdalinchen2")
    {
      fTestStandGERDAlinchenII = new GEMunichTestStandGERDAlinchenII(fTestStandDB); 
      fTestStandGERDAlinchenII -> ConstructTestStand(); 
    }

  if (fTestStandDB -> GetTestStandType() == "siegfried")
    {
      fTestStandSiegfried = new GEMunichTestStandSiegfried(fTestStandDB); 
      fTestStandSiegfried -> SetCoincidenceFlag(false);
      fTestStandSiegfried -> ConstructTestStand(); 
    }

  if (fTestStandDB -> GetTestStandType() == "siegfriedcoincidence")
    {
      fTestStandSiegfried = new GEMunichTestStandSiegfried(fTestStandDB); 
      fTestStandSiegfried -> SetCoincidenceFlag(true);
      fTestStandSiegfried -> ConstructTestStand(); 
    }

  if (fTestStandDB -> GetTestStandType() == "collimator")
    {
     fTestStandCollimator = new GEMunichTestStandCollimator(fTestStandDB);
     fTestStandCollimator -> ConstructTestStand();
    }
  if (fTestStandDB -> GetTestStandType() == "milchkanne")
    {
      fTestStandMilchkanne =  new GEMunichTestStandMilchkanne(fTestStandDB); 
      fTestStandMilchkanne -> ConstructTestStand(); 
    }
  if (fTestStandDB -> GetTestStandType() == "galatea")
    {
      fTestStandGALATEA =  new GEMunichTestStandGALATEA(fTestStandDB); 
      fTestStandGALATEA -> ConstructTestStand(); 
    }


  // set detector properties 

  SetDetectorLogical(fTestStandDB -> GetMotherVolume()); 

  SetDetectorName("MUNICHTESTSTAND"); 

}

// --------------------------------------------------------------------------- 
 

