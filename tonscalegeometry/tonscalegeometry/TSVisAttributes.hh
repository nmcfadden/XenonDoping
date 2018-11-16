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
// CLASS IMPLEMENTATION:  TSVisAttribute.cc
//
//---------------------------------------------------------------------------//
/**
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * controlled way of setting vis attributes.
 */
// End class description
// *
// 
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
//

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//available colors, default G4 color map
/*
     white	  	G4Colour::White   ()
     gray    		G4Colour::Gray    ()
     grey   		G4Colour::Grey    ()
     black   		G4Colour::Black   ()
     red    		G4Colour::Red     ()
     green   		G4Colour::Green   ()
     blue    		G4Colour::Blue    ()
     cyan    		G4Colour::Cyan    ()
     magenta 		G4Colour::Magenta ()
     yellow  		G4Colour::Yellow  ()
*/

class TSVisAttributesSolid : public G4VisAttributes
{
  public:
    TSVisAttributesSolid(G4String color);
    ~TSVisAttributesSolid(){delete fColor;}

  private:
    G4Colour *fColor;
};

class TSVisAttributesWireFrame : public G4VisAttributes
{
  public:
    TSVisAttributesWireFrame(G4String color);
    ~TSVisAttributesWireFrame(){delete fColor;}

  private:
    G4Colour *fColor;
};
