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
// $Id: MGGeneratorPNNLCascadeEvent.cc,v 1.2 2006-01-04 22:52:20 jasondet Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "generators/MGGeneratorPNNLCascadeEvent.hh"
#include <iostream>
#include <fstream>

using namespace std;

///////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLCascadeEvent::
MGGeneratorPNNLCascadeEvent(G4int index_cascade, 
			    G4double E_charged_particle, G4int num_gammas,
                            G4double* pGammaList) :
  fIndex_cascade(index_cascade),
  fE_charged_particle(E_charged_particle),
  fNum_gammas(num_gammas),
  f_pGammaList(pGammaList)
{

}

///////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLCascadeEvent::
MGGeneratorPNNLCascadeEvent()
{
  fIndex_cascade      = 0;
  fE_charged_particle = 0.0;
  fNum_gammas         = 0;
  f_pGammaList        = NULL;
}

///////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLCascadeEvent::
MGGeneratorPNNLCascadeEvent(const MGGeneratorPNNLCascadeEvent& CascadeEvent)
{
  // N.B. The CascadeEvent class isn't responsible for dynamically allocating
  // memory (even the GammaList pointer just points to memory previously
  // allocated by the Cascade class), so the copy constructor and the 
  // assignment operator are trivial.

  fIndex_cascade      = CascadeEvent.fIndex_cascade;
  fE_charged_particle = CascadeEvent.fE_charged_particle;
  fNum_gammas         = CascadeEvent.fNum_gammas;
  f_pGammaList        = CascadeEvent.f_pGammaList;

}

///////////////////////////////////////////////////////////////////////////
MGGeneratorPNNLCascadeEvent::
~MGGeneratorPNNLCascadeEvent()
{

}

///////////////////////////////////////////////////////////////////////////
void MGGeneratorPNNLCascadeEvent::
operator=(const MGGeneratorPNNLCascadeEvent& CascadeEvent)
{
  // N.B. The CascadeEvent class isn't responsible for dynamically allocating
  // memory (even the GammaList pointer just points to memory previously
  // allocated by the Cascade class), so the assignment operator is trivial.

  fIndex_cascade      = CascadeEvent.fIndex_cascade;
  fE_charged_particle = CascadeEvent.fE_charged_particle;
  fNum_gammas         = CascadeEvent.fNum_gammas;
  f_pGammaList        = CascadeEvent.f_pGammaList;
}
