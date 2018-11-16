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
 * $Id: MGEPVDetector.hh,v 1.0 2010-04-06 14:22:00 matthiasl Exp $
 *      
 * CLASS DECLARATION:  MGEPVDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 * Geometry class for the MGEPV detector at LNGS 
 *
 * REVISION:
 *
 * 03-08-2010, Added in MaGe, Luciano
 *
 */
// --------------------------------------------------------------------------//

#ifndef _MGEPVDETECTOR_HH
#define _MGEPVDETECTOR_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class MGEPVDetectorMessenger;
class G4Material;

class MGEPVDetector : public MGGeometryDetector
{
public:

  //default constructor
  MGEPVDetector();

  //destructor
  ~MGEPVDetector();

  //public interface
  void ConstructDetector();
  void SetSampleType(G4String geo){sample_geom=geo;};
  void SetSampleMaterial(G4String mat_name){sample_mat_name = mat_name;};
  G4String GetSampleType(){return sample_geom;};
  void SetSMarinelliHeight(G4double smh) {smarinelli_height=smh;};
  void SetSBoxHeight(G4double sbh) {h_sbox=sbh;};
  void SetSBoxIDiameter(G4double sbid) {di_sbox=sbid;};
  void SetSBoxODiameter(G4double sbod) {do_sbox=sbod;};
  void SetSupportHeight(G4double srh) {h_sring=srh;};
  void SetSupportIDiameter(G4double srid) {di_sring=srid;};
  void SetSupportODiameter(G4double srod) {do_sring=srod;};
  void SetBoxHeight(G4double bh) {box_height=bh;};
  void SetBoxWidth(G4double bw) {box_width=bw;};
  void SetBoxThickness(G4double bt) {box_thickness=bt;};
  void SetTubeInnerRadius(G4double tir) {tube_innrad=tir;};
  void SetTubeOuterRadius(G4double tor) {tube_outrad=tor;};
  void SetTubeLength(G4double tl) {tube_length=tl;};
  void SetTubeAngle(G4double ta) {tube_angle=ta;};
  void SetSphereRadius(G4double sr) {sphere_rad=sr;};
  void SetSamplexpos(G4double xpos) {s_x=xpos;};
  void SetSampleypos(G4double ypos) {s_y=ypos;};
  void SetSamplezpos(G4double zpos) {s_z=zpos;};
  void SetDetzpos(G4double dzpos) {d_z=dzpos;};
  void SetDeadThickness(G4double deadt) {d_dead=deadt;};
  void SetHoleDiameter(G4double hd) {hole_d=hd;};
  void SetHoleLength(G4double hl) {hole_l=hl;};
  void SetDetDiameter(G4double cd) {crystal_d=cd;};
  void SetDetLength(G4double cl) {crystal_l=cl;};

private:
  G4String sample_geom;
  G4String sample_mat_name;
  G4double h_sring;
  G4double di_sring;
  G4double do_sring;
  G4double h_sbox;
  G4double di_sbox;
  G4double do_sbox;
  G4double box_height;
  G4double box_width;
  G4double box_thickness;
  G4double smarinelli_height;
  G4double smarinelli_depth1;
  G4double smarinelli_depth2;
  G4double smarinelli_depth3;
 
  G4double tube_innrad;
  G4double tube_outrad;
  G4double tube_length;
  G4double tube_angle;
  G4double sphere_rad;
  G4Material* sample_mat;
  G4double s_x;
  G4double s_y;
  G4double s_z;
  G4double d_z;
  G4double d_dead;
  G4double hole_d;
  G4double hole_l;
  G4double crystal_d;
  G4double crystal_l;
 
  MGEPVDetectorMessenger* theMessenger;


};
#endif
