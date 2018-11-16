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
 * $Id: MGGeneratorUtil.hh,v 1.3 2005-03-19 15:28:43 jasondet Exp $
 *
 * CLASS DECLARATION: MGGeneratorUtil.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 * The structure contains some utility functions to sample points within some 
 * basic geometrical volumes
 */
/**
 * AUTHOR: David Jordan
 * CONTACT: david.jordan@pnl.gov
 * FIRST SUBMISSION: 10-20-2004
 *
 * REVISION:
 *
 * 11-15-2004, First implementation of the algorithms in the MJ framework
 * 12-06-2004, Re-naming, Luciano
 */
// --------------------------------------------------------------------------//

#ifndef MGGeneratorUtil_hh
#define MGGeneratorUtil_hh 1

#include "G4ThreeVector.hh"
#include "Randomize.hh"

class MGGeneratorUtil
{
public:
  MGGeneratorUtil();
  //MGGeneratorUtil(const MGGeneratorUtil &);
  ~MGGeneratorUtil();

public: 
  G4ThreeVector  pick_isotropic();

  G4ThreeVector  pick_point_in_box(G4double x_lo, G4double x_hi,
                                 G4double y_lo, G4double y_hi,
                                 G4double z_lo, G4double z_hi);
  G4ThreeVector  pick_point_on_box(G4double x_lo, G4double x_hi,
                                 G4double y_lo, G4double y_hi,
                                 G4double z_lo, G4double z_hi);

  G4ThreeVector  pick_point_in_annulus(G4double r1, G4double r2, G4double h);
  G4ThreeVector  pick_point_in_annulus(G4double r1, G4double r2, G4double h,
				       G4double theta0, G4double dtheta);
  G4ThreeVector  pick_point_in_cylinder(G4double Radius, G4double L);
  G4ThreeVector  pick_point_on_cylinder(G4double Radius, G4double L);
  
  G4ThreeVector  pick_point_in_shell(G4double r1, G4double r2);
  G4ThreeVector  pick_point_in_sphere(G4double r);
  G4ThreeVector  pick_point_on_sphere(G4double r);
  G4ThreeVector  pick_point_on_wholetube(G4double r_in,
					 G4double r_out,
					 G4double height); 
  void pick_point_in_circle(G4double Radius, G4double &x, G4double &y);
};

#endif
