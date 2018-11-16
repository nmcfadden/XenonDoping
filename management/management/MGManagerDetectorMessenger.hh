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
/**                                                            
 * $Id: MGManagerDetectorMessenger.hh,v 1.5 2008-01-30 11:28:34 pandola Exp $
 *      
 * CLASS DECLARATION:  MGManagerDetectorMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *  This class builds the messenger class for the detector geometry. It defines
 *  commands to be used within the macros to define what detector is going to be
 *  built (e.g., clover, MEGA, SEGA).
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *  The default detector to be built is the clover. If there is no explicit call
 *  from within the macro file, then you get the clover.
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: kareem
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jun 30 15:23:21 PDT 2004
 * 
 * REVISION:
 * 
 * 06-30-2004, Kareem
 *			   Initial file creation and submission.
 * Nov 2004, Luciano
 *                         Added command to switch the Database
 * 10-12-2006, Luciano
 *                         Added command to define new materials from file
 * 12-12-2006, Luciano
 *                         Added command to give the filename for geometries
 *                         read from file
 * 28-09-2007, Jing
 *                         Added GDML reading and writing commands
 * 10-30-2007, JIng
 *                         Added GDML configuration setup commands
 * 01-28-2008, Luciano   
 *                         Added command to check for overlapping volumes
 * 01-30-2008, Luciano
 *                         Added command to set verbosity for overlapping volumes
 * 03-08-2009, Alex
 *                         Modified G4GDML support. Alex
 * 18 Nov 2011, Alexis     
 *                         Added command to use parallel geometry
 * 11-23-2011  K. Nguyen 
 *                         Added command to set visibility of physical volumes
 * 07-30-2012  K. Nguyen 
 *                         Added long-overdue command to print list of physical volumes
 * 08-08-2012  K. Nguyen 
 *                         Changed name of GetListOfPhysicalVolumes() to PrintListOfPhysicalVolumes()
 */
// --------------------------------------------------------------------------//

#ifndef _MGMANAGERDETECTORMESSENGER_HH
#define _MGMANAGERDETECTORMESSENGER_HH

//---------------------------------------------------------------------------//

//  GEANT4 headers
#include "globals.hh"
#include "G4UImessenger.hh"

class MGManagerDetectorConstruction;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
//---------------------------------------------------------------------------//

class MGManagerDetectorMessenger : public G4UImessenger 
{
  public:
    MGManagerDetectorMessenger( MGManagerDetectorConstruction* );
    ~MGManagerDetectorMessenger();

    void SetNewValue( G4UIcommand*, G4String );

 private:
  MGManagerDetectorConstruction *theDetector;
  G4UIdirectory *MGGeometryDir;
  G4UIcmdWithAString *MGGeometryDetectorChoiceCommand;
  G4UIcmdWithoutParameter *fDumpG4MaterialsCmd;
  G4UIcmdWithABool* fUseDatabaseCmd;
  G4UIcmdWithADoubleAndUnit *fWorldHalfLenCmd;
  G4UIcmdWith3VectorAndUnit *fDetectorOffsetCmd;
  G4UIcmdWithAString *fWorldMaterialCmd;
  G4UIcmdWithAString *fAddNewMaterialCmd;
  G4UIcmdWithAString *fAddNewGeometryCmd;

    // MGMarino 29 Sept 2005
    G4UIdirectory *fEventBiasingDir;
    G4UIcmdWithABool* fUseImportanceSamplingCmd;
    G4UIcmdWithAString *fSetScorerCmd;
    G4UIcmdWithABool* fUseScoringCmd;
    G4UIcmdWithAString *fSetImpSampAlgorithmCmd;
    G4UIcmdWithoutParameter *fInitializeVarRedCmd;
    G4UIcmdWithAString *fSetBiasedParticleCmd;
    G4UIcmdWithoutParameter *fPrintScoreTableCmd;
    // temp hack since G4 deletes something where it shouldn't
    // be deleted - MGMarino
    G4UIcmdWithoutParameter *fDeleteMGSCmd;

  G4UIcmdWithABool *fUseParallelWorldCmd; // 14 Nov 2011 A. Schubert

  G4UIcmdWithoutParameter* fOverlapCheckCmd;

  G4UIcmdWithABool* fOverlapCheckVerbosityCmd;

  // Added by Khang Nguyen, 21 Nov 2011
  G4UIcmdWithAString *fSetVisForPhysVolsMatchingCmd;
  // Added by Khang Nguyen, 25 Jul 2012
  G4UIcmdWithoutParameter *fPrintListOfPhysVolumesCmd;

   // GDML related commands, Jing 16th. Sept. 2007
   // Modified to G4-GDML. Alex, Nov. 2009
#ifndef MG_NO_G4GDML
   G4UIdirectory *fGDMLdir;
   G4UIcmdWithAString *fSetGDMLsourceFileCmd;
   G4UIcmdWithAString *fSetGDMLschemaPathCmd;
   G4UIcmdWithAString *fSetGDMLoutputNameCmd;
   G4UIcmdWithAString *fSetGDMLoutputFormatCmd;
   G4UIcmdWithAString *fSetGDMLmodularizeLevelsCmd;
   G4UIcmdWithoutParameter *fGDMLwriteCmd;
#endif

};
#endif
