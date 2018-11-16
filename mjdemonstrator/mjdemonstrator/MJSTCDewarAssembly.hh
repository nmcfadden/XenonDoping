//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//                                                          
//      
// CLASS IMPLEMENTATION:  MJSTCDewarAssembly.hh
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 *
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Jacquie MacMullin
 * CONTACT: jacq3749@gmail.com
 * FIRST SUBMISSION: Thursday October 17 2013
 * 
 * REVISION:
 * 10-17-2013, Created, J. MacMullin
 *
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//
#ifndef _MJSTCDEWARASSEMBLY_HH
#define _MJSTCDEWARASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJSTCDewarIntExt;
class MJSTCDewarInsulation;
class MJSTCDewarLN;
class MJSTCDewarActChar;

using namespace std;
//---------------------------------------------------------------------------//

class MJSTCDewarAssembly: public MJVDemoAssembly
{
  
public:
  MJSTCDewarAssembly(G4String, G4String);
  MJSTCDewarAssembly(const MJSTCDewarAssembly &);
  ~MJSTCDewarAssembly();

  MJSTCDewarIntExt* GetIntExt() {return fIntExtPtr;}
  MJSTCDewarInsulation* GetInsulation() {return fInsulationPtr;}
  MJSTCDewarLN* GetLN() {return fLNPtr;}
  MJSTCDewarActChar* GetActChar() {return fActCharPtr;}
    
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);
  
private:
  MJSTCDewarIntExt* fIntExtPtr;
  MJSTCDewarInsulation* fInsulationPtr;
  MJSTCDewarLN* fLNPtr;
  MJSTCDewarActChar* fActCharPtr;

};
//
#endif
