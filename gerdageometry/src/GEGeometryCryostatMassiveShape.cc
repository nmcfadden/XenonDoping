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
 * $Id: GEGeometryCryostatMassiveShape.cc,v 1.2 2007-12-10 10:40:05 schubert Exp $
 *
 *
 *---------------------------------------------------------------------------//
 */
/**
 * SPECIAL NOTES:
 *
 */
//
// --------------------------------------------------------------------------//
/**
 * AUTHOR: Jens Schubert
 * CONTACT:
 * FIRST SUBMISSION: @START_DATE@
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 12-05-2007, first version, Jens
 * 12-10-2007, debugged destructor, Jens
 */
// --------------------------------------------------------------------------//


#include "gerdageometry/GEGeometryCryostatMassiveShape.hh"
#include "io/MGLogger.hh"

#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

using namespace CLHEP;


G4double GEGeometryCryostatMassiveShape::GeneralPolyconeZShift(0);

GEGeometryCryostatMassiveShape::GEGeometryCryostatMassiveShape(const char* cryo_part_name):
  hneck(0),
  htopcylbot(0),
  hlittlecyl(0),
  slopetoplid(0),
  slopebotlid(0),
  
  rneck(0),
  rcyl(0),
  rlittlecyl(0),

  theCryoPartName(cryo_part_name),

  theSolid(0),
  theLogicalVolume(0),
  theMaterial(0),
  thePhysicalVolume(0)

{
}


GEGeometryCryostatMassiveShape::~GEGeometryCryostatMassiveShape()
{
    delete theLogicalVolume;
}




G4UnionSolid* 
GEGeometryCryostatMassiveShape::CreateUnionSolid()
{
    static G4int iCpy(0);
    iCpy++;
    const G4String nameNeck     ("CryShapeNeck"     );
    const G4String nameToplid   ("CryShapeToplid"   );
    const G4String nameCyl      ("CryShapeCyl"      );
    const G4String nameBotlid   ("CryShapeBotlid"   );
    const G4String nameLittlecyl("CryShapeLittlecyl");

    G4double ConeRupper;
    G4double ConeRlower;
    G4double HalfHeight;
    G4double TubeR;
    G4double Rmin(0);
    G4double PhiStart(0.0*deg);
    G4double PhiDelta(360.0*deg);;

    char str_iCpy[10];
    sprintf(str_iCpy,"_%i",iCpy);


    // neck
    TubeR      = rneck ;
    HalfHeight = 0.5 * hneck ;
    G4Tubs* solNeck = new G4Tubs(nameNeck+str_iCpy,
                                 Rmin,TubeR,
                                 HalfHeight,
                                 PhiStart,PhiDelta);
    // top lid
    ConeRupper   =  rneck;
    ConeRlower   =  rcyl;
    G4double toplidheight = slopetoplid*fabs(ConeRupper-ConeRlower);
    HalfHeight =  0.5 * toplidheight;
    G4Cons* solToplid = new G4Cons(nameToplid+str_iCpy,
                                   Rmin,ConeRlower,
                                   Rmin,ConeRupper,
                                   HalfHeight,
                                   PhiStart,PhiDelta);
    // bot lid
    ConeRupper   =  rcyl;
    ConeRlower   =  rlittlecyl;
    G4double botlidheight = slopebotlid*fabs(ConeRupper-ConeRlower);;
    HalfHeight =  0.5 * botlidheight;
    G4Cons* solBotlid = new G4Cons(nameBotlid+str_iCpy,
                                   Rmin,ConeRlower,
                                   Rmin,ConeRupper,
                                   HalfHeight,
                                   PhiStart,PhiDelta);
    // cylinder
    TubeR      = rcyl ;
    G4double cylheight = htopcylbot - toplidheight - botlidheight;
    HalfHeight = 0.5 * cylheight;
    G4Tubs* solCyl = new G4Tubs(nameCyl+str_iCpy,
                                Rmin,TubeR,
                                HalfHeight,
                                PhiStart,PhiDelta);
    // little cylinder
    TubeR      = rlittlecyl ;
    HalfHeight = 0.5 * hlittlecyl;
    G4Tubs* solLittlecyl = new G4Tubs(nameLittlecyl+str_iCpy,
                                      Rmin,TubeR,
                                      HalfHeight,
                                      PhiStart,PhiDelta);
    
    G4double z_us1;
    z_us1 = - 0.5*(hneck+toplidheight);
    G4UnionSolid* solU1 = new G4UnionSolid(G4String("CryoShapeUS1_")+str_iCpy,
                                           solNeck,
                                           solToplid,
                                           0,
                                           G4ThreeVector(0., 0., z_us1)); 

    G4double z_us2;
    z_us2 = -0.5*hneck - toplidheight -0.5*cylheight;
    G4UnionSolid* solU2 = new G4UnionSolid(G4String("CryoShapeUS2_")+str_iCpy,
                                           solU1,
                                           solCyl,
                                           0,
                                           G4ThreeVector(0., 0., z_us2)); 
    G4double z_us3;
    z_us3 = -0.5*hneck - toplidheight - cylheight - 0.5*botlidheight;
    G4UnionSolid* solU3 = new G4UnionSolid(G4String("CryoShapeUS3_")+str_iCpy,
                                           solU2,
                                           solBotlid,
                                           0,
                                           G4ThreeVector(0., 0., z_us3)); 
    G4double z_us4;
    z_us4 = -0.5*hneck - htopcylbot - 0.5*hlittlecyl;
    G4UnionSolid* solU4 = new G4UnionSolid(theCryoPartName,
                                           solU3,
                                           solLittlecyl,
                                           0,
                                           G4ThreeVector(0., 0., z_us4)); 
    theSolid = solU4;
    return solU4;
}




G4Polycone* 
GEGeometryCryostatMassiveShape::CreatePolycone()
{
    if(theSolid!=0) {
        MGLog(warning)<< "\"theSolid\" has already been created." << endlog;
        MGLog(warning)<< "... the old \"theSolid\" is used!!!" << endlog;
        abort();
        return 0;
    }


    G4double phistart(0.0 *deg);
    G4double phitot(360.0 *deg);
    G4int numzplanes(6);
    G4double coord_z[6];
    G4double coord_rInner[6] = {0,0,0,0,0,0};
    G4double coord_rOuter[6] = {rneck,rneck,rcyl,rcyl,rlittlecyl,rlittlecyl};
    //double coord_rOuter[6] = {rlittlecyl,rlittlecyl,rcyl,rcyl,rneck,rneck};

    coord_z[0] = 0;
    coord_z[1] = - hneck;
    coord_z[2] = - hneck - slopetoplid*(rcyl-rneck); 
    coord_z[3] = - hneck - htopcylbot + slopebotlid*(rcyl-rlittlecyl); 
    coord_z[4] = - hneck - htopcylbot;
    coord_z[5] = - hneck - htopcylbot - hlittlecyl;

    //  //std::cout <<"i\t"<<"z\t"<<"rInner\t"<<"rOuter\t"<<std::endl;
    //  for(int i=0;i<numzplanes;i++) {
    //      std::cout <<i<<"\t"<<coord_z[i]<<"\t"<<coord_rInner[i]<<"\t"<<coord_rOuter[i]<<"\t"<<std::endl;
    //  }

    //std::cout <<"===>>>Add z-offset"<<std::endl;
    for(int i=0;i<numzplanes;i++) {
        coord_z[i] += GeneralPolyconeZShift;
    }

    //  std::cout <<"i\t"<<"z\t"<<"rInner\t"<<"rOuter\t"<<std::endl;
    //  for(int i=0;i<numzplanes;i++) {
    //      std::cout <<i<<"\t"<<coord_z[i]<<"\t"<<coord_rInner[i]<<"\t"<<coord_rOuter[i]<<"\t"<<std::endl;
    //  }
    //  std::cout <<"============================================================"<<std::endl<<std::endl;


    G4Polycone* thePolycone;
    if(hneck==0) {
        thePolycone = new G4Polycone(theCryoPartName,
                                     phistart,phitot,
                                     numzplanes-1,
                                     &(coord_z[1]),&(coord_rInner[1]),&(coord_rOuter[1]));
    }
    else {
        thePolycone = new G4Polycone(theCryoPartName,
                                     phistart,phitot,
                                     numzplanes,
                                     coord_z,coord_rInner,coord_rOuter);
    }


    theSolid = thePolycone;
    return thePolycone;
}

G4LogicalVolume*
GEGeometryCryostatMassiveShape::CreateLogicalVolume(const char* name)
{
    theLogicalVolume = new G4LogicalVolume(theSolid,
                                           theMaterial, 
					   name); 
    return theLogicalVolume;
}


G4VPhysicalVolume*
GEGeometryCryostatMassiveShape::CreatePhysicalVolume(const char* aName,G4LogicalVolume* aMother,
                                                     const G4ThreeVector& positionShift)
{
  thePhysicalVolume = new G4PVPlacement(0, 
                                        positionShift,
                                        theLogicalVolume,
                                        aName,
                                        aMother,
                                        false, 
                                        0); 
    return thePhysicalVolume;
}
