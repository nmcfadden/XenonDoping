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
 * $Id: GECorradoDetector.hh,v 1.4 2009-05-18 16:10:36 dusan Exp $
 *      
 * CLASS DECLARATION:  GECorradoDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 * Geometry class for the Corrado detector in Heidelberg 
 *
 * REVISION:
 *
 * 03-06-2007, Added in MaGe, Luciano
 * 06-01-2007, Implemented parameters from Hardy, Dusan
 * 14-05-2009, Added hole-size parameters, Dusan
 *
 */
// --------------------------------------------------------------------------//

#ifndef _GECORRADODETECTOR_HH
#define _GECORRADODETECTOR_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class GECorradoDetectorMessenger;
class G4Material;
class GECorradoDetector : public MGGeometryDetector
{

private:
  G4String sample_geom;
  G4String sample_mat_name;
  G4double box_height;
  G4double box_width;
  G4double box_thickness;
  G4double tube_innrad;
  G4double tube_outrad;
  G4double tube_length;
  G4double tube_angle;
  G4Material* sample_mat;
  G4double s_x;
  G4double s_y;
  G4double s_z;
  G4double d_side;
  G4double d_front;
  G4double coll_x;
  G4double coll_y;
  G4double coll_z;
  G4double hole_radius;
  G4double hole_length;


  GECorradoDetectorMessenger* theMessenger;

public:

  //default constructor
  GECorradoDetector();

  //destructor
  ~GECorradoDetector();

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
  void SetSamplexpos(G4double xpos) {s_x=xpos;};
  void SetSampleypos(G4double ypos) {s_y=ypos;};
  void SetSamplezpos(G4double zpos) {s_z=zpos;};
  void SetFrontDeadThickness(G4double deadfront) {d_front=deadfront;};
  void SetSideDeadThickness(G4double deadside) {d_side=deadside;};
  void SetCollimatorxpos(G4double collxpos) {coll_x=collxpos;};
  void SetCollimatorypos(G4double collypos) {coll_y=collypos;};
  void SetCollimatorzpos(G4double collzpos) {coll_z=collzpos;};
  void SetHoleRadius(G4double hr) {hole_radius=hr;};
  void SetHoleLength(G4double hl) {hole_length=hl;};


};
#endif
