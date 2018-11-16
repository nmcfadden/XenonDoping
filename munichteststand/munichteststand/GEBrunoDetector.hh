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
 * $Id: GEBrunoDetector.hh,v 1.3 2009-05-18 16:10:36 dusan Exp $
 *      
 * CLASS DECLARATION:  GEBrunoDetector.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 * Geometry class for the Bruno detector in Heidelberg 
 *
 * REVISION:
 *
 * 03-06-2007, Added in MaGe, Luciano
 * 03-06-2007, Changed to avoid problem with materials defined on-the-fly, Luciano
 *
 */
// --------------------------------------------------------------------------//

#ifndef _GEBRUNODETECTOR_HH
#define _GEBRUNODETECTOR_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class GEBrunoDetectorMessenger;
class G4Material;
class GEBrunoDetector : public MGGeometryDetector
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
  G4double sphere_rad;
  G4Material* sample_mat;
  G4double s_x;
  G4double s_y;
  G4double s_z;

  GEBrunoDetectorMessenger* theMessenger;

public:

  //default constructor
  GEBrunoDetector();

  //destructor
  ~GEBrunoDetector();

  //public interface
  void ConstructDetector();
  void SetSampleGeometry(G4String geo){sample_geom=geo;};
  void SetSampleMaterial(G4String mat_name) {sample_mat_name = mat_name;};
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

};
#endif
