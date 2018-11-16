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
 * $Id: GEGeometryStringStuff.cc,v 1.3 2008-07-18 08:00:02 schubert Exp $
 *
 *---------------------------------------------------------------------------*/
/**
 * AUTHOR: Jens Schubert
 * CONTACT:
 * FIRST SUBMISSION: @START_DATE@
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 06-05-2008, first version, Jens
 * 07-09-2008, (jens) added phase1-cables
 */
// --------------------------------------------------------------------------//


#include "gerdageometry/GEGeometryStringStuff.hh"
#include "G4LogicalVolume.hh"

GEGeometryStringStuff::GEGeometryStringStuff():
  fElectronicsSideBarZPosition(-99999),
  fElectronicsSideBarHeight(-99999),
  fElectronicsSideBarXOffset(-99999),
  fElectronicsSideBarLogical(0),
  fPhase1_CableLowerPart_Logical(5),
  fPhase1_CableUpperPart_Logical(5)
{}

GEGeometryStringStuff::~GEGeometryStringStuff()
{
    delete fElectronicsSideBarLogical;

    for(unsigned int i=0;i<fPhase1_CableLowerPart_Logical.size();i++) delete fPhase1_CableLowerPart_Logical.at(i) ; 
    fPhase1_CableLowerPart_Logical.clear(); 

    for(unsigned int i=0;i<fPhase1_CableUpperPart_Logical.size();i++) delete fPhase1_CableUpperPart_Logical.at(i) ; 
    fPhase1_CableUpperPart_Logical.clear(); 
    
}


