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
//                                                          
// $Id: MGOutputASCII.cc,v 1.1 2006-01-24 02:56:48 jasondet Exp $ 
//      
// CLASS IMPLEMENTATION:  MGOutputASCII.cc
//
/**
 * AUTHOR: J. Detwiler
 * CONTACT: 
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 */


#include "io/MGLogger.hh" 
#include "io/MGOutputASCII.hh"

using namespace std;

MGOutputASCII::MGOutputASCII()
{
  SetSchemaDefined(true);
}

void MGOutputASCII::OpenFile()
{
  fFile.open(fFileName.c_str());
  if(!fFile.good()) {
    MGLog(fatal) << "Ccouldn't open file " << fFileName << endlog;
  }
}

void MGOutputASCII::CloseFile()
{
  fFile.close();
}

