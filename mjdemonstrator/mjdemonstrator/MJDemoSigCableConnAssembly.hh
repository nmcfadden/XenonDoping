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
 * CLASS DECLARATION:  MJDemoSigCableConnAssembly.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *The assembly constituting the signal cable connector for mounting on the 
 * cold plate.
 *
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
 * CONTACT: mpgreen@physics.unc.edu
 * FIRST SUBMISSION: Jan 14, 2011
 * 
 * REVISION:
 * 
 * 01-14-2011, Created, M. Green
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOSIGCABLECONNASSEMBLY_HH
#define _MJDEMOSIGCABLECONNASSEMBLY_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoAssembly.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;
class MJDemoSigCableConnSilica;
class MJDemoSigCableConnClamp;


using namespace std;
//---------------------------------------------------------------------------//

class MJDemoSigCableConnAssembly: public MJVDemoAssembly
{
  
public:
  MJDemoSigCableConnAssembly(G4String, G4String);
  MJDemoSigCableConnAssembly(const MJDemoSigCableConnAssembly &);
  ~MJDemoSigCableConnAssembly();

  MJDemoSigCableConnSilica* GetSilica() {return fSilicaPtr;}
  MJDemoSigCableConnClamp*  GetClamp()  {return fClampPtr;}

  
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,
                     G4LogicalVolume* motherLogical);

  
  
private:
  MJDemoSigCableConnSilica* fSilicaPtr;
  MJDemoSigCableConnClamp*  fClampPtr;


};
//
#endif
