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
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION:  MJDemoThermosyphonAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *This assembly consists of the Thermosyphon insertion assembly [Thermosyphon weldment,
 *vespel supports, copper cable thermal shunts, signal connectors, top-of-thermosyphon 
 *clamping hardware signal cable take-ups and 5/16" thermosyphon clamping bolts, and 
 *the glass electrical break at the cube end.
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
 * AUTHOR: Matthew Green
 * CONTACT: mpgreen@gmail.com
 * FIRST SUBMISSION: July 30, 2014
 * 
 * REVISION:
 * 
 * 07-30-2014, Created, M. Green
 * 10-01-2015, Added electrical break - M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOTHERMOSYPHONASSEMBLY_HH
#define _MJDEMOTHERMOSYPHONASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoThermosyphonInsertionSubAssembly;
class MJDemo51624Bolt;
class MJDemoSignalCableTakeupSubAssembly;
class MJDemoThermosyphonEBreakAssembly;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoThermosyphonAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoThermosyphonAssembly(G4String, G4String);
  MJDemoThermosyphonAssembly(const MJDemoThermosyphonAssembly &);
  ~MJDemoThermosyphonAssembly();

  MJDemoThermosyphonInsertionSubAssembly* GetInsertionAssembly() const {return fInsertPtr;}
  MJDemoThermosyphonInsertionSubAssembly* GetInsertionAssembly() {return fInsertPtr;}
  MJDemo51624Bolt* GetBolt01() {return fBolt01Ptr;}
  MJDemo51624Bolt* GetBolt02() {return fBolt02Ptr;}
  MJDemo51624Bolt* GetBolt03() {return fBolt03Ptr;}
  MJDemo51624Bolt* GetBolt04() {return fBolt04Ptr;}
  MJDemo51624Bolt* GetBolt05() {return fBolt05Ptr;}
  MJDemo51624Bolt* GetBolt06() {return fBolt06Ptr;}
  MJDemoSignalCableTakeupSubAssembly* GetTakeup01() {return fTakeup01Ptr;}
  MJDemoSignalCableTakeupSubAssembly* GetTakeup02() {return fTakeup02Ptr;}
  MJDemoSignalCableTakeupSubAssembly* GetTakeup03() {return fTakeup03Ptr;}
  MJDemoSignalCableTakeupSubAssembly* GetTakeup04() {return fTakeup04Ptr;}
  MJDemoSignalCableTakeupSubAssembly* GetTakeup05() {return fTakeup05Ptr;}
  MJDemoSignalCableTakeupSubAssembly* GetTakeup06() {return fTakeup06Ptr;}
  MJDemoSignalCableTakeupSubAssembly* GetTakeup07() {return fTakeup07Ptr;}
  MJDemoSignalCableTakeupSubAssembly* GetTakeup08() {return fTakeup08Ptr;}
  MJDemoThermosyphonEBreakAssembly* GetEBreak() {return fEBreakPtr;}
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);  
  
private:
  MJDemoThermosyphonInsertionSubAssembly* fInsertPtr;
  MJDemo51624Bolt* fBolt01Ptr;
  MJDemo51624Bolt* fBolt02Ptr;
  MJDemo51624Bolt* fBolt03Ptr;
  MJDemo51624Bolt* fBolt04Ptr;
  MJDemo51624Bolt* fBolt05Ptr;
  MJDemo51624Bolt* fBolt06Ptr;
  MJDemoSignalCableTakeupSubAssembly* fTakeup01Ptr;
  MJDemoSignalCableTakeupSubAssembly* fTakeup02Ptr;
  MJDemoSignalCableTakeupSubAssembly* fTakeup03Ptr;
  MJDemoSignalCableTakeupSubAssembly* fTakeup04Ptr;
  MJDemoSignalCableTakeupSubAssembly* fTakeup05Ptr;
  MJDemoSignalCableTakeupSubAssembly* fTakeup06Ptr;
  MJDemoSignalCableTakeupSubAssembly* fTakeup07Ptr;
  MJDemoSignalCableTakeupSubAssembly* fTakeup08Ptr;
  MJDemoThermosyphonEBreakAssembly* fEBreakPtr;

};
//
#endif
