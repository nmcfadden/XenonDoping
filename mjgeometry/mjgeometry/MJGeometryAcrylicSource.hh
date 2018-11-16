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
 * CLASS DECLARATION:  MJGeometryAcrylicSource.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Geometry code for a standard acrylic source (1" diameter kind)
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
 * AUTHOR: Ryan Martin
 * CONTACT: rdmartin@lbl.gov
 * FIRST SUBMISSION: Feb 26, 2013
 * 
 * REVISION:
 * 
 * 02-26-2013, Created, R. Martin
 */
// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYACRYLICSOURCE_HH
#define _MJGEOMETRYACRYLICSOURCE_HH

//---------------------------------------------------------------------------//



using namespace std;
//---------------------------------------------------------------------------//

class MJGeometryAcrylicSource
{
  
public:
  MJGeometryAcrylicSource(G4String argName="AcrylicSource");
  ~MJGeometryAcrylicSource();
	void ConstructMaterials();
  void Place(G4ThreeVector* assemPosition, G4RotationMatrix* assemRotation,G4LogicalVolume *motherlogical);

	G4double GetSourceDiameter(){return fSourceDiameter;};

private:
	G4String fName;
	G4double fSourceDiameter;
	G4double fSourceLength;
	G4double fActivityDiameter;//this volume will actually have the activity in it.
	G4double fActivityLength;
	

};
//
#endif
