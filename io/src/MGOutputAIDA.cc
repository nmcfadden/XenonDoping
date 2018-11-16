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
 * SPECIAL NOTES:
 * 
 *
 */
// 
// 11-08-2004 Created file, but to be written!, Luciano
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//

#include "io/MGLogger.hh" 

//---------------------------------------------------------------------------//

#include "io/MGOutputAIDA.hh"

//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//

MGOutputAIDA::MGOutputAIDA()
{;}

//---------------------------------------------------------------------------//

MGOutputAIDA::MGOutputAIDA(const MGOutputAIDA& other) : MGVOutputManager(other) 
{;}

//---------------------------------------------------------------------------//

MGOutputAIDA::~MGOutputAIDA()
{;}

//---------------------------------------------------------------------------//

//concrete AIDA/HBOOK implementation of virtual methods
void MGOutputAIDA::OpenFile()
{
  // use fFileName for filename
  MGLog(warning) << "This does not actually open the file: "
                 << "do in the derived class!" << endlog;
}

void MGOutputAIDA::CloseFile()
{
  MGLog(warning) << "This does not actually close the file: "
                 << "do in the derived class!" << endlog;
}


