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
 * $Id: MGGeneratorMuonsFromFile.hh,v 1.3 2006-01-10 14:43:44 pandola Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorMuonsFromFile.hh
 *
 *---------------------------------------------------------------------------//
 */
/** 
 * AUTHOR: Markus Knapp
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 * 
 * 07-08-2005, Created, M. Knapp
 * 12-30-2005, Now it is a derived class of G4VPrimaryGenerator, talking with MaGe 
 *             through MGGeneratorCosmicRays(), Luciano
 * 01-10-2006, Added methods and data members, Luciano
 */
// --------------------------------------------------------------------------//

#ifndef _MGGENERATORMUONGUN_HH
#define _MGGENERATORMUONGUN_HH

//---------------------------------------------------------------------------//


#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4VPrimaryGenerator.hh"

class G4Event;
class MGGeneratorMuonsFromFileMessenger;

//---------------------------------------------------------------------------//

class MGGeneratorMuonsFromFile : public G4VPrimaryGenerator
{
public:

  //default constructor
  MGGeneratorMuonsFromFile();

  //copy constructor
  MGGeneratorMuonsFromFile(const MGGeneratorMuonsFromFile &);

  //destructor
  ~MGGeneratorMuonsFromFile();

  //public interface
  void GeneratePrimaryVertex(G4Event *event);
  G4String GetFileName() {return fileName;};
  void SetFileName(G4String stri);

  //protected members
protected:


  //private  members
private:
  //G4int n_particle;
  G4int ev_ID;
  G4double x[1500];            // hardcoded, until now, because of low number of available 
  G4double y[1500];            // dangerous muons
  G4double z[1500];
  G4double px[1500];
  G4double py[1500]; 
  G4double pz[1500];
  G4double energy[1500];
  //G4int muon_number[1500];
  MGGeneratorMuonsFromFileMessenger* theMessenger;
  G4String fileName; 
  //G4int muonnumber;
  G4int max_entries; 

  void OpenDataFile();
  G4bool fFileIsOpen;
};
#endif
