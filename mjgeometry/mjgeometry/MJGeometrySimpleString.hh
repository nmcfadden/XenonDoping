/*
27 Oct 2009 Changed from MGGeometryIdealCoaxCrystal to
            MGGeometryBEGeDetector, four detectors instead of 3

*/
#ifndef _SIMPLESTRING_HH
#define _SIMPLESTRING_HH

//---------------------------------------------------------------------------//

#include <string>
#include <vector>
#include "globals.hh"
#include "geometry/MGGeometryDetector.hh"

//---------------------------------------------------------------------------//

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;

class MGGeometryBEGeDetector;

class MJGeometrySimpleString : public MGGeometryDetector
{
public:

  //default constructor
  MJGeometrySimpleString(G4String serNum);

  //copy constructor
  MJGeometrySimpleString(const MJGeometrySimpleString &);

  //destructor
  ~MJGeometrySimpleString();

  //public interface
  void ConstructDetector();

  MGGeometryBEGeDetector*  GetTemplateCrystal()
    { return fTemplateCrystal; }

  bool UsesMJPSQLDatabase() { return true; }  

//private  members
private:
  
  MGGeometryBEGeDetector  *fTemplateCrystal;

  // Physical volumes:
  G4VPhysicalVolume* fCu_phys;
  G4LogicalVolume* fCu_log;
  G4VPhysicalVolume* fDetZero_phys;
  G4VPhysicalVolume* fDetOne_phys;
  G4VPhysicalVolume* fDetTwo_phys;
  G4VPhysicalVolume* fDetThree_phys;

};
#endif
