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
 * $Id: MJOutputSolidBlock.hh,v 1.2 2005-02-03 21:38:49 jasondet Exp $
 *      
 * CLASS DECLARATION:  MJOutputSolidBlock.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Output class for Solid block of materials. Save energy deposit in a 
 * 3 D block.
 * WARNING : Generated large output trees!
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Tue Aug 24 14:25:50 PDT 2004
 * 
 * REVISION:
 * 
 * 08-24-2004, Created, R. Henning
 * 12-07-2004, Modified for Gerda integration, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTSOLIDBLOCK_HH
#define _MJOUTPUTSOLIDBLOCK_HH

//---------------------------------------------------------------------------//

#include "TObject.h"

#include "G4ThreeVector.hh"

#include "io/MGOutputRoot.hh"

//---------------------------------------------------------------------------//

class TH1D;
class G4DynamicParticle;

class MJOutputSolidBlock : public MGOutputRoot
{
public:

  //default constructor
  MJOutputSolidBlock(G4bool isMother);

  //copy constructor
  MJOutputSolidBlock(const MJOutputSolidBlock &);

  //destructor
  ~MJOutputSolidBlock();

  //public interface
  void BeginOfEventAction(const G4Event *event);
  void BeginOfRunAction();
  void DefineSchema();
  void EndOfEventAction(const G4Event *event);
  void EndOfRunAction();
  void RootSteppingAction(const G4Step *step);
  G4bool ConvertCoordinateToGridPoint(G4ThreeVector pos, G4int *gpt);

  //protected members
protected:


  //private  members
private:

  G4int    fMaxNumParticles;
  const G4DynamicParticle    *fPrimarye; // Primary particle (e-)
  G4double fTrackLength;           // Accumulated track length of primary. 
  G4int    feNSteps;               // Number of Step Primary particle makes.
  G4int    fEdepArraySize;         // Length of flattened Edep array
  G4bool   fPrimaryDemiseRecorded; // Has the demise of the primary particle
                                   // being recorded?
  G4double fOldKineticEnergy;      // Kinetic energy in previous step.
  G4ThreeVector   fOrigin;         // Origin of primary electron

  // Root histograms.
  TH1D     *feTrackDistanceHisto; // Distance of each step os primary particle 
  TH1D     *feEdepHisto;          // Energy deposits for other particles
  TH1D     *feVertexDistance;     // Distance of primary e- from vertex at end
  TH1D     *feNStepsHisto;        // Number of steps primary e- takes.
  TH1D     *feEdepStep;           // Energy deposit per step for primary e-
  TH1D     *feEAtEnd;             // Energy of primary e- at end of track.     
  TH1D     *feAccTrackLen;        // Accumulated Track Length
  TH1D     *feG4Distance;         // Track length from G4.
  TH1D     *feKEnergyHisto;       // Kinetic energy at each step.
  TH1D     *fEDistance;           // Energydeposit/r^2 as fn of r.

  // MC Data ---> Begin
  // Note : MC data does not follow TAligent naming convention
  //        to distinguish it from "normal" class fields.

  UInt_t   EventNumber;
  Int_t    NumberOfParticles;
  Int_t    *Particle;
  Float_t  *MomentumX;
  Float_t  *MomentumY;
  Float_t  *MomentumZ;
  Float_t  *PositionX;
  Float_t  *PositionY;
  Float_t  *PositionZ;

  Int_t   nX;  // Number of points in the x direction
  Int_t   nY;  // Number of points in the y direction
  Int_t   nZ;  // Number of points in the z direction

  Float_t  dX;  // Size (cm) of x step
  Float_t  dY;  // Size (cm) of y step
  Float_t  dZ;  // Size (cm) of z step

  Float_t  OriginX; // Origin of block in mother coordinates. (cm)
  Float_t  OriginY;
  Float_t  OriginZ;

  Float_t  *Edep; // 3-D array of energy deposits saved as flat array

  Float_t  EdepOutside; // Energy deposit outside block.
  // MC Data --> End

};
#endif
