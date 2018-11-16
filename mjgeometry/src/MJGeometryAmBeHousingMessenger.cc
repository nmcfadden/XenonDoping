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
// $Id: MJGeometryAmBeHousingMessenger.cc,v 1.2 2007-02-23 13:42:01 alexis3 Exp $ 
//      
// CLASS IMPLEMENTATION:  MJGeometryAmBeHousingMessenger.cc
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
 * AUTHOR: alexis3
 * CONTACT: 
 * FIRST SUBMISSION: Fri Feb 17 13:09:53 PST 2006
 * 
 * REVISION:
 *
 * 02-23-2007, Initial submission, A. Schubert
 */
//---------------------------------------------------------------------------//
//

#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "mjgeometry/MJGeometryAmBeHousing.hh"
#include "io/MGLogger.hh"

//---------------------------------------------------------------------------//

#include "mjgeometry/MJGeometryAmBeHousingMessenger.hh"      //Present MJ Class Headers 

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJGeometryAmBeHousingMessenger::MJGeometryAmBeHousingMessenger(MJGeometryAmBeHousing* amBeHousing):
fAmBeHousing(amBeHousing)
{
  fAmBeHousingDirectory = new G4UIdirectory("/MG/geometry/AmBeHousing/");
  fPositionCmd = 
    new G4UIcmdWith3VectorAndUnit("/MG/geometry/AmBeHousing/position",this);
  fPositionCmd->SetGuidance("Set location of center of housing, which should match source position.");
  fPositionCmd->SetParameterName("X","Y","Z",true,true);
  fPositionCmd->SetUnitCategory("Length");

}

MJGeometryAmBeHousingMessenger::~MJGeometryAmBeHousingMessenger()
{
  delete fAmBeHousingDirectory;
  delete fPositionCmd;
}

void MJGeometryAmBeHousingMessenger::SetNewValue(G4UIcommand *command,
					      G4String newValues)
{
  if (command == fPositionCmd) 
  {
    fAmBeHousing->SetHousingPosition(
      fPositionCmd->GetNew3VectorValue(newValues));
    MGLog(trace) << "AmBe Housing position set to " 
      << fPositionCmd->GetNew3VectorValue(newValues) << endlog;
  }

}

