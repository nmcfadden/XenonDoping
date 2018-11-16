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
 * $Id: MGOutputRootPreWF.hh,v 1.1 2007-06-08 13:03:13 jliu Exp $
 *      
 * CLASS DECLARATION:  MGOutputRootPreWF.hh
 *
 *---------------------------------------------------------------------------//
 */ 

/**
 * DESCRIPTION: 
 *
 * output the crystal data and hits information to a root file
 *
 */

/**  
 * SPECIAL NOTES:
 *
 */
 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: Jing Liu
 * CONTACT: jingliu at mppmu.mpg.de
 * FIRST SUBMISSION: 06-08-2007
 * 
 * REVISION:
 * 
 * 06-08-2007, Created, Jing Liu
 */
// --------------------------------------------------------------------------//

#ifndef _MGOutputRootPreWF_HH
#define _MGOutputRootPreWF_HH

//---------------------------------------------------------------------------//

// MJ header
#include "MGOutputRoot.hh"

// MGDO header
#include "MGTCrystalDataContainer.hh"

// ROOT header
//#include "TFile.h"

//---------------------------------------------------------------------------//

class MGOutputRootPreWF : public MGOutputRoot
{
  //public members
public:
  //default constructor
  MGOutputRootPreWF(G4bool isMother);

  //copy constructor
  MGOutputRootPreWF(const MGOutputRootPreWF &);

  //destructor
  ~MGOutputRootPreWF();

//    void SetPreWFFileName(G4String name){fFileName = name;}
//    G4String GetPreWFFileName() {return fFileName;}

//    void OpenPreWFFile();
//    void ClosePreWFFile();

   void DefineSchema();
   
   void BeginOfRunAction();     // open root file 
   void BeginOfEventAction(const G4Event *event);
   void EndOfEventAction(const G4Event *event); // hits from sensitive D
   void EndOfRunAction();                       // close root file

   void RootSteppingAction(const G4Step*);

  //protected members
protected:


  //private  members
private:
//    G4String fFileName;
//    TFile* fRootFile;

   MGTCrystalDataContainer* container;
};
//
#endif
