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
 * CLASS DECLARATION:  GEGeometryCryostatMassiveShape.hh
 *
 * DESCRIPTION:
 *
 *   A class to hold basic shape of one layer of the cryostat
 *   Instances of these layers are: inner steel wall, outer steel wall ,  ...
 *
 * AUTHOR: Jens Schubert
 *
 * REVISION: MM-DD-YYYY
 *
 *   12-05-2007, first version, Jens
 */


#ifndef _GEGEOMETRYCRYOSTATMASSIVESHAPE_HH
#define _GEGEOMETRYCRYOSTATMASSIVESHAPE_HH

//---------------------------------------------------------------------------//
//      Geant4 headers
#include "globals.hh"

// MG headers
#include "GEGeometryDetectorDB.hh"

//---------------------------------------------------------------------------//

class G4Polycone;
class G4UnionSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;


class GEGeometryCryostatMassiveShape {

  public:
    GEGeometryCryostatMassiveShape(const char* cryo_part_name);
    ~GEGeometryCryostatMassiveShape();


    void                 SetHneck         (G4double height) { hneck       = height;}
    void                 SetHtopcylbot    (G4double height) { htopcylbot  = height;}
    void                 SetHlittlecyl    (G4double height) { hlittlecyl  = height;}
    void                 SetSlopetoplid   (G4double dzdr)   { slopetoplid = dzdr; }
    void                 SetSlopebotlid   (G4double dzdr)   { slopebotlid = dzdr; }

    void                 SetRneck         (G4double r)      { rneck      = r; }
    void                 SetRcyl          (G4double r)      { rcyl       = r; }
    void                 SetRlittlecyl    (G4double r)      { rlittlecyl = r; }

    void                 SetCryoPartName  (const char* name) { theCryoPartName = name; }

    void                 SetMaterial         (G4Material* material) { theMaterial = material; }
    G4Polycone*          CreatePolycone      ();
    G4UnionSolid*        CreateUnionSolid    ();
    G4LogicalVolume*     CreateLogicalVolume (const char* name);
    G4VPhysicalVolume*   CreatePhysicalVolume(const char* aName,G4LogicalVolume* aMother,
                                              const G4ThreeVector& positionShift=G4ThreeVector(0,0,0));


    G4double             GetHneck         () const  { return hneck;      }
    G4double             GetHtopcylbot    () const  { return htopcylbot; }
    G4double             GetHlittlecyl    () const  { return hlittlecyl; }
    G4double             GetSlopetoplid   () const  { return slopetoplid;}
    G4double             GetSlopebotlid   () const  { return slopebotlid;}

    G4double             GetHtoplid       () const  { return slopetoplid*(rcyl-rneck); }
    G4double             GetHbotlid       () const  { return slopebotlid*(rcyl-rlittlecyl); }
    G4double             GetHcyl          () const  { return htopcylbot-GetHtoplid()-GetHbotlid(); }

    G4double             GetRneck         () const  { return rneck;      }
    G4double             GetRcyl          () const  { return rcyl;       }
    G4double             GetRlittlecyl    () const  { return rlittlecyl; }

    const char*          GetCryoPartName  () const  { return theCryoPartName; }

    G4VSolid*            GetSolid         () const  { return theSolid;         }
    G4LogicalVolume*     GetLogicalVolume () const  { return theLogicalVolume; }
    G4Material*          GetMaterial      () const  { return theMaterial;      }
    G4VPhysicalVolume*   GetPhysicalVolume() const  { return thePhysicalVolume;}


  public:
    static G4double GeneralPolyconeZShift;

    //private:
  public:
    G4double hneck;
    G4double htopcylbot;
    G4double hlittlecyl;
    G4double slopetoplid;
    G4double slopebotlid;

    G4double rneck;
    G4double rcyl;
    G4double rlittlecyl;

    G4String theCryoPartName;

    G4VSolid*           theSolid;
    G4LogicalVolume*    theLogicalVolume;
    G4Material*         theMaterial;
    G4VPhysicalVolume*  thePhysicalVolume;

};

#endif

