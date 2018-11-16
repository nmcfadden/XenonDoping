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
// $Id: 
//      
// CLASS IMPLEMENTATION:  MJGeometryCERN_NA55Messenger.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
    Messenger for CERN NA55 beam dump simulation.
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: mgmarino 
 * CONTACT: 
 * FIRST SUBMISSION:
 * 
 * REVISION:
 *
 *  */
//---------------------------------------------------------------------------//
//

//#include <systemG4Stuff.hh>             //additional system Headers
//#include "globalsStuff.hh"              //additional Global Headers
//#include "G4Stuff.hh"		              //additional G4 Class Headers
//#include "MJStuff.hh"                   //additional MJ Headers

#include "globals.hh"
#include "G4ios.hh"
#include "G4ApplicationState.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

#include "mjgeometry/MJGeometryCERN_NA55.hh"
#include "io/MGLogger.hh"


//---------------------------------------------------------------------------//

//Present MJ Class Headers: 
#include "mjgeometry/MJGeometryCERN_NA55Messenger.hh"  

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryCERN_NA55Messenger::MJGeometryCERN_NA55Messenger(MJGeometryCERN_NA55 *theDet) : fCERN_NA55(theDet)
{
  // /MG/geometry/CERN_NA55/
  fCERN_NA55Directory = new G4UIdirectory("/MG/geometry/CERN_NA55/");

  // /MG/geometry/CERN_NA55/setBeamDumpType
  fSetBeamDumpTypeCmd = new G4UIcmdWithAString(
     "/MG/geometry/CERN_NA55/setBeamDumpType", this);
  fSetBeamDumpTypeCmd->SetGuidance("Set type of beam dump.");
  fSetBeamDumpTypeCmd->AvailableForStates(G4State_PreInit);
  fSetBeamDumpTypeCmd->SetCandidates("Graphite Copper Lead");
}

//---------------------------------------------------------------------------//

MJGeometryCERN_NA55Messenger::~MJGeometryCERN_NA55Messenger()
{
  delete fSetBeamDumpTypeCmd;
  delete fCERN_NA55Directory;
}

//---------------------------------------------------------------------------//

void MJGeometryCERN_NA55Messenger::SetNewValue(G4UIcommand* cmd, G4String nV)
{
  if(cmd == fSetBeamDumpTypeCmd) {
    if(nV=="Graphite") fCERN_NA55->SetBeamDumpType(kGraphite);
    else if(nV=="Copper") fCERN_NA55->SetBeamDumpType(kCopper);
    else if(nV=="Lead") fCERN_NA55->SetBeamDumpType(kLead);
  }
}
