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
 * $Id: GEDarioDetector.hh,v 1.2 2007-03-07 08:48:20 pandola Exp $
 *      
 * CLASS DECLARATION:  GEDarioDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 * Geometry class for the Dario detector in Heidelberg 
 *
 * REVISION:
 *
 * 03-06-2007, Added in MaGe, Luciano
 *
 */
// --------------------------------------------------------------------------//

#ifndef _GEDARIODETECTOR_HH
#define _GEDARIODETECTOR_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class GEDarioDetectorMessenger;
class G4Material;
class GEDarioDetector : public MGGeometryDetector
{

private:
  G4String sample_geom;
  G4String sample_mat_name;
  G4double box_height;
  G4double box_width;
  G4double box_thickness;
  /*
  G4double marinellibox_height;
  G4double marinellibox_width;
  G4double marinellibox_thickness;
  G4double marinellitube_length;
  G4double marinellitube_outrad;
  */
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
  G4double crystal_d;
  G4double crystal_l;
  //G4double f_size;

  GEDarioDetectorMessenger* theMessenger;

public:

  //default constructor
  GEDarioDetector();

  //destructor
  ~GEDarioDetector();

  //public interface
  void ConstructDetector();
  void SetSampleGeometry(G4String geo){sample_geom=geo;};
  void SetSampleMaterial(G4String mat_name){sample_mat_name = mat_name;};
  G4String GetSampleGeometry(){return sample_geom;};
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
  void SetDetDiameter(G4double cd) {crystal_d=cd;};
  void SetDetLenght(G4double cl) {crystal_l=cl;};

};
#endif
