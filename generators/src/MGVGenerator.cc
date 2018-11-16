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
// $Id: MGVGenerator.cc,v 1.2 2007-02-23 09:24:09 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGVGenerator.cc
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
 * AUTHOR: rhenning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jul  7 16:33:10 PDT 2004
 * 
 * REVISION:
 *
 * 07-07-2004, Created, 
 */
//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//

#include "generators/MGVGenerator.hh"

//---------------------------------------------------------------------------//

MGVGenerator::MGVGenerator():
  fGeneratorName(""), fG4Messenger(0), fReportingFrequency(1000)
{;}

//---------------------------------------------------------------------------//

MGVGenerator::MGVGenerator(const MGVGenerator&)
{;}

//---------------------------------------------------------------------------//

MGVGenerator::~MGVGenerator()
{;}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

