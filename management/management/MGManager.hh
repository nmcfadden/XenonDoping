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
 * $Id: MGManager.hh,v 1.3 2007-02-20 18:02:20 mgmarino Exp $
 *      
 * CLASS DECLARATION:  MGManager.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Manager class for MaGe package.
 *    1. Contains pointers to other management classes (G4, waveform, etc.).
 *    2. Associated messenger controls management settings.
 *
 * Pass this object to object A defined within it if object A needs to access
 * other objects.
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Tue Jul  6 16:15:44 PDT 2004
 * 
 * REVISION:
 * 
 * 07-06-2004, Created, R. Henning
 * 07-19-2004, Converted to singleton with global access, R. Henning
 * 11-08-2004, Added management of TrackingAction, Xiang
 * 12-07-2004, Modified for Gerda Integration, R. Henning
 * 12-15-2004, Added management of Stacking Action and getter for TrackingAction,
 *             L. Pandola
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGMANAGER_HH
#define _MGMANAGER_HH

//---------------------------------------------------------------------------//

class  G4RunManager;
class  G4VisManager;
class  MGGeneratorPrimary;
class  MGManagerDetectorConstruction;
class  MGManagementEventAction;
class  MGManagementRunAction;
class  MGManagementSteppingAction;
class  MGManagementStackingAction;
class  MGManagerMessenger;
class  MGProcessesList;
class  MGWaveformManager;
class  MGManagementTrackingAction;
class  G4VUserPhysicsList;

//---------------------------------------------------------------------------//

class MGManager 
{
public:

  //default constructor
  MGManager();

  //copy constructor
  MGManager(const MGManager &);

  //destructor
  ~MGManager();

  //public interface

  G4RunManager       *GetG4RunManager() { return fG4RunManager; }
  G4VisManager *GetG4VisManager() { return fG4VisManager; }
  //  MGWaveformManager  *GetMGWaveformManager() { return fWaveformManager; }
  MGGeneratorPrimary *GetMGGeneratorPrimary() { return fGeneratorPrimary; }
  MGManagerDetectorConstruction *GetManagerDetectorConstruction()
    { return fManagerDetectorConstruction; }
  MGManagementEventAction        *GetMGEventAction() 
    { return fManagementEventAction;}
  MGManagementRunAction        *GetMGRunAction() 
    { return fManagementRunAction;}
  MGManagementSteppingAction     *GetMGSteppingAction()
    { return fManagementSteppingAction; }
  G4VUserPhysicsList      *GetMGProcessesList()
    { return fProcessesList; }
  MGManagementStackingAction *GetMGStackingAction()
  { return fManagementStackingAction; }
  MGManagementTrackingAction    *GetMGTrackingAction()
  { return fManagementTrackingAction;}

  // Return pointer to singleton MGManager.
  static MGManager *GetMGManager();

  void SetUserInitialization(G4RunManager *g4Manager) 
     { fG4RunManager = g4Manager; }
  void SetUserInitialization(G4VisManager *vis) 
     { fG4VisManager = vis; }
  //  void SetUserInitialization(MGWaveformManager *wvmgr) 
  //   { fWaveformManager = wvmgr; }
  void SetUserInitialization(MGGeneratorPrimary *gene) 
     { fGeneratorPrimary = gene; }
  void SetUserInitialization(MGManagerDetectorConstruction *det) 
     { fManagerDetectorConstruction = det; }
  void SetUserInitialization(MGManagementEventAction *evt) 
     { fManagementEventAction = evt; }
  void SetUserInitialization(MGManagementRunAction *runa) 
     { fManagementRunAction = runa; }
  void SetUserInitialization(MGManagementTrackingAction *tracka) 
     {fManagementTrackingAction = tracka;}
  void SetUserInitialization(MGManagementStackingAction *stacka) 
     {fManagementStackingAction = stacka;}
  void SetUserInitialization(MGManagementSteppingAction *stepa) 
     { fManagementSteppingAction = stepa; }
  void SetUserInitialization(G4VUserPhysicsList *proc) 
     { fProcessesList = proc; }

  //protected members
protected:


  //private  members
private:
  // Pointers to main objects.
  G4RunManager                    *fG4RunManager;
  G4VisManager                    *fG4VisManager;
  MGGeneratorPrimary              *fGeneratorPrimary;
  MGManagerDetectorConstruction  *fManagerDetectorConstruction;
  MGManagementEventAction         *fManagementEventAction;
  MGManagementRunAction           *fManagementRunAction;
  MGManagementSteppingAction      *fManagementSteppingAction;
  G4VUserPhysicsList              *fProcessesList;
  // MGWaveformManager               *fWaveformManager;
  MGManagementTrackingAction    *fManagementTrackingAction;
  MGManagementStackingAction    *fManagementStackingAction;

  MGManagerMessenger              *fG4Messenger; // G4 messenger for UI.

  static MGManager                *fMGManager; // Pointer to MJManager
};
#endif
