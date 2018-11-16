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
 * $Id: MGGeneratorGeneralSurfaceSamplerMessenger.hh,v 1.1 2006-01-24 02:55:47 jasondet Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorGeneralSurfaceSamplerMessenger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Messenger class for commands /MG/generator/gss
 *
 * AUTHOR: J. Detwiler
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 */

#ifndef _MGGENERATORGENERALSURFACESAMPLERMESSENGER_HH
#define _MGGENERATORGENERALSURFACESAMPLERMESSENGER_HH


#include "G4UImessenger.hh"

class MGGeneratorGeneralSurfaceSampler;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAString;

class MGGeneratorGeneralSurfaceSamplerMessenger : public G4UImessenger
{
  public:
    MGGeneratorGeneralSurfaceSamplerMessenger(MGGeneratorGeneralSurfaceSampler* generator);
    ~MGGeneratorGeneralSurfaceSamplerMessenger();

    void SetNewValue(G4UIcommand* command, G4String newValues);

  protected:
    MGGeneratorGeneralSurfaceSampler* fGeneratorGeneralSurfaceSampler;
  
    G4UIdirectory* fGSSDirectory;
    // /MG/generator/gss
    // directory for the General Surface Sampler
  
    G4UIcmdWithADoubleAndUnit* fGSSBoundingRCmd;
    // /MG/generator/gss/boundingR [radius]
    // Sets the bounding radius of a sphere surrounding the volumes whose
    // surfaces are to be sampled.

    G4UIcmdWith3VectorAndUnit* fGSSOriginCmd;
    // /MG/generator/gss/origin [x y z]
    // Sets the center of the bounding sphere, in case the volumes to be
    // studied are not centered at (0, 0, 0)
    // /MG/generator/position has the same effect, but the name here is
    // better.

    G4UIcmdWithAString* fGSSBoundVolCmd;
    // /MG/generator/gss/boundvol [volume]
    // performs both /MG/generator/gss/boundingR and
    // /MG/generator/gss/origin but uses a the boundingR and global
    // coordinates of an existing volume, which the user supplies by name.
    // If more than one volume of the same name exists in the
    // G4PhysicalVolumeStore, then the first one retrieved will be used.
};
#endif
