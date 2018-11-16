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
 *
 * DESCRIPTION: 
 *
 *
 * This class describes a Canberra BEGe detector.  The default
 * geometry is Canberra's model BE5030, which is 70 mm in diameter and
 * 30 mm tall.  There is a ditch on the -z face of the detector. The
 * dead layer is not implemented in the model.  Canberra's BEGe
 * detectors include a copper foil wrap and an Aluminum HV contact.
 * These are not included in the model.
 *
 * 
 * AUTHOR: alexis
 * CONTACT: 
 * FIRST SUBMISSION: Mon Oct 26 13:35:21 PDT 2009
 * 
 * REVISION:
 * 
 * 10-26-2009, creation, A. Schubert
 * 03-03-2010, fixed method name; changed GetDitchDepthRadius() to
 *             GetDitchDepth(), A. Schubert
 * 04-28-2010, added boolean flag to decide where to put the ditch 
 *             (upper/lower surface). L. Pandola
 * 04 Feb 2011, changed default value of fDitchBelow from false to true to
 *              comply with definition in MGCrystalData, A. Schubert
 * 12-20-2011, added an option for conical corners to the crystal. B. Lehnert
 * 12 Mar 2012, changed initialization order in constructor to fix compiler
 *              warning; formatted for doxygen. -- A. Schubert
 * 01-24-2012, added DL option to reduce size of BEGe to active volume (off by default) B. Lehnert
 *
 */

#ifndef _MGGEOMETRYBEGEDETECTOR_HH
#define _MGGEOMETRYBEGEDETECTOR_HH


#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

class G4Material;
class G4LogicalVolume;
class G4VisAttributes;

/// BEGe detector geometry
class MGGeometryBEGeDetector : public MGGeometryDetector 
{
  //public interface
public:
  //default constructor
  MGGeometryBEGeDetector( G4String serNum );

  //copy constructor
  MGGeometryBEGeDetector(const MGGeometryBEGeDetector &);

  //destructor
  virtual ~MGGeometryBEGeDetector();

  virtual void ConstructDetector();
  virtual void Dump();
  virtual G4Material* GetCrystalMaterial() { return fCrystalMaterial; }
  virtual void SetG4MaterialName( G4String name ) { fCrystalMaterialName = name; }

  virtual G4double GetRadius(){ return fRadius; } 
  virtual void SetRadius( G4double radius ){ fRadius = radius; }
  virtual G4double GetHeight(){ return fHeight; }
  virtual void SetHeight( G4double height ){ fHeight = height; }
  virtual G4double GetDitchOuterRadius(){ return fDitchOuterRadius; }
  virtual void SetDitchOuterRadius( G4double rad ){ fDitchOuterRadius = rad; }
  virtual G4double GetDitchInnerRadius(){ return fDitchInnerRadius; }
  virtual void SetDitchInnerRadius( G4double rad ){ fDitchInnerRadius = rad; }
  virtual G4double GetDitchDepth(){ return fDitchDepth; }
  virtual void SetDitchDepth( G4double depth ){ fDitchDepth = depth; }
 
  virtual void SetCornerDiameter( G4double val ){ fCornerDiameter = val; }
  virtual G4double GetCornerDiameter(){ return fCornerDiameter; }
  virtual void SetCornerHeight( G4double val ){ fCornerHeight = val; }
  virtual G4double GetCornerHeight(){ return fCornerHeight; }
  virtual void SetCornerOnBottom( G4bool val ){ fCornerOnBottom = val; }
  virtual G4double GetCornerOnBottom(){ return fCornerOnBottom;} 

  virtual void SetDLOuterContact( G4double val ){ fDLOuterContact = val; }
  virtual G4double GetDLOuterContact(){ return fDLOuterContact;}

  virtual void SetDLInnerContact( G4double val ){ fDLInnerContact = val; }
  virtual G4double GetDLInnerContact(){ return fDLInnerContact;}

  virtual void SetDitchBelow(G4bool myb){fDitchBelow = myb;}

  virtual G4VSolid *GetDetectorSolid(){ return fBEGeSolid;}


  //protected members
protected:
  G4double      fHeight;        // detector height
  G4double      fRadius;        // detector radius

  G4double      fDitchOuterRadius;    // OD of ditch
  G4double      fDitchInnerRadius;    // ID of ditch
  G4double      fDitchDepth;          // depth of ditch

  G4double      fCornerDiameter;      // diameter of cornered side
  G4double      fCornerHeight;        // height of corner in z
  G4bool        fCornerOnBottom;      // cornered side on top or bottom

  G4double      fDLOuterContact;      // DL thickness of outer contact
  G4double      fDLInnerContact;      // DL thickness of inner contact


  G4Material*   fCrystalMaterial;
  G4String      fCrystalMaterialName;

  G4bool fDitchBelow;

  G4VSolid * fBEGeSolid;	// solid of logical detector in order to add dead layer outside

  //private  members
private:
};
#endif
