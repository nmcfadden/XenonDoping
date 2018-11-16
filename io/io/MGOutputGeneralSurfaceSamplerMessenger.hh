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
 * $Id: MGOutputGeneralSurfaceSamplerMessenger.hh,v 1.1 2006-01-24 02:55:47 jasondet Exp $
 *      
 * CLASS DECLARATION:  MGOutputGeneralSurfaceSamplerMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Messenger class for interacting with General Surface Sampling class.  Sets
 * the names of the volumes of interest, as well as the maximum number of
 * intersections a random ray through the geometry could achieve.
 *
 * AUTHOR: R. Johnson
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 * 11/22/2011 Include a messenger that requires the surface to be ONLY between two of the selected volumes. Bjoern Lehnert
 * 
 */

#ifndef _MGOUTPUTGENERICSURFACEMESSENGER_HH
#define _MGOUTPUTGENERICSURFACEMESSENGER_HH

#include "G4UImessenger.hh"

class MGOutputGeneralSurfaceSampler;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class MGOutputGeneralSurfaceSamplerMessenger : public G4UImessenger
{
  public:
    MGOutputGeneralSurfaceSamplerMessenger(MGOutputGeneralSurfaceSampler* outputGSS);
    ~MGOutputGeneralSurfaceSamplerMessenger();

    void SetNewValue(G4UIcommand* command, G4String newValue);

  private:
    G4UIdirectory* fOutputGeneralSurfaceSamplerDirectory;
    MGOutputGeneralSurfaceSampler* fOutputGeneralSurfaceSampler; 
    G4UIcmdWithAString* fAddVolumeCmd;
    G4UIcmdWithAString *fRemoveVolumeCmd;
    G4UIcmdWithAnInteger* fSetMaxIntersectionsCmd;
    G4UIcmdWithABool* fSetOnlyBetweenSelectedVolumesCmd;
};
#endif
