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
 /*
 * DESCRIPTION: 
 * Geometry class for the Giove detector in Heidelberg 
 *
 * REVISION:
 *
 * 09-06-2009, Added in MaGe, Luciano
 *
 */
// --------------------------------------------------------------------------//

#ifndef _GEGIOVEDETECTOR_HH
#define _GEGIOVEDETECTOR_HH

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class GEGioveDetectorMessenger;
class G4Material;
class GEGioveDetector : public MGGeometryDetector
{

private:

  G4int shielding_config;
  G4int num_layers;
  G4double box_height;
  G4double box_width;
  G4double layer_thickness;
  GEGioveDetectorMessenger* theMessenger;

public:

  //default constructor
  GEGioveDetector();

  //destructor
  ~GEGioveDetector();

  //public interface
  void ConstructDetector();
  void SetShieldingConfig(G4int sc) { shielding_config=sc;}
  void SetNumberOfLayers(G4int nl) {num_layers=nl;}
  void SetBoxHeight(G4double bh) {box_height=bh;};
  void SetBoxWidth(G4double bw) {box_width=bw;};
  void SetLayerThickness(G4double bt) {layer_thickness=bt;};

};
#endif
