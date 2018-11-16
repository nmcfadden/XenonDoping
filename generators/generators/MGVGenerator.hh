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
 * $Id: MGVGenerator.hh,v 1.2 2007-02-23 09:24:09 mgmarino Exp $
 *      
 * CLASS DECLARATION:  MGVGenerator.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Pure virtual base class for MG generators. 
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
 * FIRST SUBMISSION: Wed Jul  7 16:33:10 PDT 2004
 * 
 * REVISION:
 * 
 * 07-07-2004, Created, R. Henning
 * 11-08-2004, Added virtual method to set position (necessary to use the 
 *             position sampling algorithm for all the concrete generators), L. Pandola
 */
// --------------------------------------------------------------------------//

#ifndef _MGVGENERATOR_HH
#define _MGVGENERATOR_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4ThreeVector.hh"

#include "io/MGLogger.hh"


//---------------------------------------------------------------------------//

class G4Event;
class G4UImessenger;
class G4Run;

//---------------------------------------------------------------------------//

class MGVGenerator 
{
public:

  //default constructor
  MGVGenerator();

  //copy constructor
  MGVGenerator(const MGVGenerator &);

  //destructor
  virtual ~MGVGenerator();

  //public interface
  virtual void BeginOfRunAction(const G4Run*) {;} 
  // Called at beginning of Run
  virtual void EndOfRunAction(const G4Run*) {;} // Called at End of run.
  virtual void GeneratePrimaryVertex(G4Event *event) = 0;
  virtual void SetParticlePosition(G4ThreeVector vec) = 0;
  G4String GetGeneratorName() { return fGeneratorName; }
  void SetReportingFrequency(G4int freq) { fReportingFrequency = freq; }

  //protected members
protected:
  G4String        fGeneratorName;  // Name of Generator.
  G4UImessenger   *fG4Messenger;   // G4Messenger for setting up generator.
  G4int           fReportingFrequency; // Generate report every 
                                       // fReportingFrequency events;  
  //private  members
private:
};
#endif
