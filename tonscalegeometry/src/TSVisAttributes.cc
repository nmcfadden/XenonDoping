
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
// $Id: MJcodeTemplate.cc,v 1.3 2005-02-15 23:26:55 akbarm Exp $ 
//      
// CLASS IMPLEMENTATION:  TSVisAttributes
//
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
/**
 * AUTHOR: Ryan Martin
 * CONTACT: 
 * 
 * REVISION:
 *
 * 06-13-2011, Created, R. Martin
 *
 */
//---------------------------------------------------------------------------//

#include "tonscalegeometry/TSVisAttributes.hh"

TSVisAttributesSolid::TSVisAttributesSolid(G4String color) : G4VisAttributes(G4Colour::White())
{
  fColor = new G4Colour(G4Colour::White());
  G4Colour::GetColour(color,*fColor);
  this->SetColor(*fColor);
  this->SetForceSolid(true);
  this->SetForceWireframe(false);
}

// -----------------------------------------------------------------------

TSVisAttributesWireFrame::TSVisAttributesWireFrame(G4String color) : G4VisAttributes(G4Colour::White())
{
  fColor = new G4Colour(G4Colour::White());
  G4Colour::GetColour(color,*fColor);
  this->SetColor(*fColor);
  this->SetForceWireframe(true);
}

// -----------------------------------------------------------------------

