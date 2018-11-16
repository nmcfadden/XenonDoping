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
// $Id: MJGeometryRDCryostatMessenger.cc,v 1.4 2009-04-23 18:32:24 rhenning Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryRDCryostatMessenger.cc
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Nov 16 16:25:29 PST 2005
 * 
 * REVISION:
 *
 * 11-17-2005, Created, R. Henning
 * 04-21-2009, Added DEMO On Command, R. Henning
 *
 */
//---------------------------------------------------------------------------//
//

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcommand.hh"

#include "mjgeometry/MJGeometryRDCryostat.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryRDCryostatMessenger.hh"

using namespace CLHEP;


//---------------------------------------------------------------------------//

MJGeometryRDCryostatMessenger::MJGeometryRDCryostatMessenger(MJGeometryRDCryostat* cryo):
  fRDCryostat(cryo)
{  

  // /MG/geometry/RDCryostat/
  fDirectory = new G4UIdirectory("/MG/geometry/RDCryostat/");
  
  // /MG/geometry/RDCryostat/DEMO
  fDEMOOnCmd = new G4UIcmdWithABool("/MG/geometry/RDCryostat/DEMO", this);
  fDEMOOnCmd->SetGuidance("Choose defaults for DEMONSTRATOR geometry.");
  fDEMOOnCmd->AvailableForStates(G4State_PreInit);
}

//---------------------------------------------------------------------------//
MJGeometryRDCryostatMessenger::MJGeometryRDCryostatMessenger(const MJGeometryRDCryostatMessenger & other) : G4UImessenger(other)
{
}

//---------------------------------------------------------------------------//

MJGeometryRDCryostatMessenger::~MJGeometryRDCryostatMessenger()
{
  delete fDEMOOnCmd;
  delete fDirectory;
}

//---------------------------------------------------------------------------//

void MJGeometryRDCryostatMessenger::SetNewValue(G4UIcommand *cmd, G4String nV)
{
  if(cmd == fDEMOOnCmd) 
    if(fDEMOOnCmd->GetNewBoolValue(nV))
      fRDCryostat->SetDefaultsDEMONSTRATOR();
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
