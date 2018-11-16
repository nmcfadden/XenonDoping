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
 * $Id: MGOutputRoot.hh,v 1.11 2007-07-20 08:42:45 pandola Exp $
 *      
 * CLASS DECLARATION:  MGOutputRoot.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 * Abstract base class for Root output file. Defines basic operations on 
 * root files and trees. This class is inherited by classes that
 * define detector specific outputs.  
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
 * AUTHOR: RHenning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Apr  7 16:12:33 PDT 2004
 * 
 * REVISION:
 * 
 * 04-07-2004 Created, R. Henning
 * 08-08-2004 Added GetTree() method. R. Henning
 * 11-08-2004 Now it inherits from MGVOutputManager. Some generic methods 
 *            (OpenFile, etc.) have been moved to the parent class, Luciano
 * 06-18-2005 Add access to stepping manager, Jason
 * 01-21-2006 Don't need to make a separate Open/CloseRootFile, and don't
 *            need a separate filename. Jason
 * 07-20-2007 Added a virtual method called PrepareFileForClosing(), in order to 
 *            allow multiple runs within a single ROOT file. Luciano
 *
 */

#ifndef _MGOUTPUTROOT_HH
#define _MGOUTPUTROOT_HH 1

//CLHEP
//#include <CLHEP/Units/SystemOfUnits.h>
//#include <CLHEP/config/CLHEP.h>

// Root
#include "TFile.h"
#include "TObject.h"
#include "TTree.h"

// Geant 4
#include "globals.hh"
#include "G4ios.hh"
#include "G4Event.hh"
#include "G4Step.hh"

//base class
#include "io/MGVOutputManager.hh"

class MGOutputRoot : public MGVOutputManager
{
  public:
    MGOutputRoot(G4bool isMother);
    virtual ~MGOutputRoot() {}

    void OpenFile();
    // Opens Rootfile to save event tree and possibly other Root objects.

    void CloseFile();
    // Writes data to tree, saves tree, closes file and deletes file object.

    virtual void WriteFile();
    // Write any data available so far to the file so that it is not lost in
    // the event of a crash.
  
    virtual void PrepareFileForClosing() {;};
    //Contains specific commands to be given by the concrete output before 
    //closing the file

    void FillTree() { fTree->Fill(); }
    // usually done at end of event

    virtual void DefineSchema() = 0;
    // This is where you define the branches of your trees, and allocate any
    // other objects to be saved to file. Usually done at the beginning of
    // a run.


    /** Action to perform at beginning and end of events and runs
     Detector specific.
     Only include actions that directly affect the Root tree.
     Other actions should go into the MGManagementEventAction class.
    */
    virtual void BeginOfEventAction(const G4Event*) {;}
    virtual void BeginOfRunAction() {;}
    virtual void EndOfEventAction(const G4Event*) {;}
    virtual void EndOfRunAction() {;}
    void SteppingAction(const G4Step* step, G4SteppingManager*) 
      { RootSteppingAction(step); }

    /** Modify event tree variables during Geant 4 SteppingAction, ie. accumulate
     energy deposits, charge distributions, etc. 
     Detector specific.
     Pure virtual method. MUST be defined in derived class.
    */
    virtual void RootSteppingAction(const G4Step* step) = 0;

    // Overloaded Conversion Methods: Root --> Geant 4
    G4double  RootToG4(Double_t XX) { return (G4double) XX; }
    G4float   RootToG4(Float_t XX) { return (G4float) XX; }
    G4int     RootToG4(Int_t XX) { return (G4int) XX; }
    G4long    RootToG4(Long_t XX) { return (G4long) XX; }

    // Overloaded Conversion Methods: Geant4 --> Root
    Double_t  G4ToRoot(G4double XX) { return (Double_t) XX; }
    Float_t   G4ToRoot(G4float XX) { return (Float_t) XX; }
    Int_t     G4ToRoot(G4int XX) { return (Int_t) XX; }
    Long_t    G4ToRoot(G4long XX) { return (Long_t) XX; }

    // Overloaded conversion methods: CLHEP --> Root
    /*
    Double_t CLHEPtoROOT(HepDouble XX) { return (Double_t) XX; }
    Float_t  CLHEPtoROOT(HepFloat XX) { return (Float_t) XX; }
    Int_t    CLHEPtoROOT(HepInt XX) { return (Int_t) XX; }
    */

    // Overloaded conversion methods: Root --> CLHEP
    /*
    HepDouble ROOTtoCLHEP(Double_t XX) { return (HepDouble) XX; }
    HepFloat  ROOTtoCLHEP(Float_t XX) { return (HepFloat) XX; }
    HepInt    ROOTtoCLHEP(Int_t XX) { return (HepInt) XX; }
    */

    // Setters and Getters.
    // G4String GetFileName() { return fFileName; }
    TFile *GetRootFile() { return fRootFile; }
    TTree *GetTree() { return fTree; }
    G4bool IsMother() { return fIsMother; }
    void SetRootFileName(G4String name) { SetFileName(name);}
    void SetRootFile(TFile *rfile) { fRootFile = rfile; }
    void SetTree(TTree *XtalTree) { fTree = XtalTree; }

  protected:
    static TTree *fTree; 
    // Tree that is written to file that contains event information.

  private:
    static TFile *fRootFile;
    // File that tree and other information that may be defined in the 
    // inherited class is written to.

    G4bool   fIsMother;  // true if this instation is the mother of all other
                         // MGOutputRoot object.
};

#endif













