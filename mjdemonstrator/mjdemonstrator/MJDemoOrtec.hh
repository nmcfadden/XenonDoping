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
 * CLASS DECLARATION:  MJDemoOrtec.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for a generic BeGe
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
 * AUTHOR: Wenqin Xu
 * CONTACT: wxu@lanl.gov
 * FIRST SUBMISSION: Jan 22, 2013
 * 
 * REVISION:
 * 
 * 01-22-2013, Created, W. Xu
 */
// --------------------------------------------------------------------------//

#ifndef _MJDEMOPUCKBEGE_HH
#define _MJDEMOPUCKBEGE_HH

//---------------------------------------------------------------------------//

#include "mjdemonstrator/MJVDemoPart.hh"

class G4LogicalVolume;

using namespace std;
//---------------------------------------------------------------------------//

class MJDemoOrtec: public MJVDemoPart
{
  
public:
  MJDemoOrtec(
	G4String partName, 
	G4String serialNumber,
	G4double diameter, 
	G4double height, 
	G4double topdiameter,
	G4double topheight,
	G4double bottomdiameter1, 
	G4double bottomheight1,
	G4double bottomdiameter2, 
	G4double bottomheight2,
	G4double bottomdiameter3, 
	G4double bottomheight3
  );
  MJDemoOrtec(const MJDemoOrtec &);
  ~MJDemoOrtec();

  G4LogicalVolume* ConstructPart();

  void SetDiameter(G4double diameter)	{fCrystalDiameter = diameter*CLHEP::mm;}
  void SetHeight(G4double height)		{fCrystalHeight = height*CLHEP::mm;}
  void SetTopDiameter(G4double diameter)	{fCrystalTopDiameter = diameter*CLHEP::mm;}
  void SetTopHeight(G4double height)		{fCrystalTopHeight = height*CLHEP::mm;}
  void SetBottomDiameter1(G4double diameter)	{fCrystalBottomDiameter1 = diameter*CLHEP::mm;}
  void SetBottomHeight1(G4double height)	{fCrystalBottomHeight1 = height*CLHEP::mm;}
  void SetBottomDiameter2(G4double diameter)	{fCrystalBottomDiameter2 = diameter*CLHEP::mm;}
  void SetBottomHeight2(G4double height)	{fCrystalBottomHeight2 = height*CLHEP::mm;}
  void SetBottomDiameter3(G4double diameter)	{fCrystalBottomDiameter3 = diameter*CLHEP::mm;}
  void SetBottomHeight3(G4double height)	{fCrystalBottomHeight3 = height*CLHEP::mm;}
  G4double GetDiameter() 				{return fCrystalDiameter;}
  G4double GetHeight()					{return fCrystalHeight;}  
  G4double GetTopDiameter() 				{return fCrystalTopDiameter;}
  G4double GetTopHeight()				{return fCrystalTopHeight;}  
  G4double GetBottomDiameter1() 			{return fCrystalBottomDiameter1;}
  G4double GetBottomHeight1()				{return fCrystalBottomHeight1;}  
  G4double GetBottomDiameter2() 			{return fCrystalBottomDiameter2;}
  G4double GetBottomHeight2()				{return fCrystalBottomHeight2;}  
  G4double GetBottomDiameter3() 			{return fCrystalBottomDiameter3;}
  G4double GetBottomHeight3()				{return fCrystalBottomHeight3;}  

private:
  G4double fCrystalDiameter;
  G4double fCrystalHeight;
  G4double fCrystalTopDiameter;
  G4double fCrystalTopHeight;
  G4double fCrystalBottomDiameter1;
  G4double fCrystalBottomHeight1;
  G4double fCrystalBottomDiameter2;
  G4double fCrystalBottomHeight2;
  G4double fCrystalBottomDiameter3;
  G4double fCrystalBottomHeight3;
};
//
#endif
