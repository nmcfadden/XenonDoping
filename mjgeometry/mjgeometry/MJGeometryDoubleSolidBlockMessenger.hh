// --------------------------------------------------------------------------//

#ifndef _MJGEOMETRYDOUBLESOLIDBLOCKMESSENGER_HH
#define _MJGEOMETRYDOUBLESOLIDBLOCKMESSENGER_HH

//---------------------------------------------------------------------------//

#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;
class G4UIcmdWith3VectorAndUnit;


class MJGeometryDoubleSolidBlock;

class MJGeometryDoubleSolidBlockMessenger : public G4UImessenger
{
public:

  //default constructor
  MJGeometryDoubleSolidBlockMessenger(MJGeometryDoubleSolidBlock *doubleblock);

  //destructor
  ~MJGeometryDoubleSolidBlockMessenger();

  //public interface
  void SetNewValue(G4UIcommand *cmd, G4String cmdstr);

  //protected members
protected:


  //private  members
private:
  G4UIdirectory                *fDoubleSolidBlockDir;
  G4UIcmdWithAString           *fAlphaMaterialSelectCmd;
  G4UIcmdWithADoubleAndUnit    *fAlphaSetEdgeLengthCmd;
  G4UIcmdWithABool             *fAlphaSetActiveCmd;
  G4UIcmdWithADoubleAndUnit    *fAlphaSetDeadLayerCmd;
  G4UIcmdWithAString           *fBetaMaterialSelectCmd;
  G4UIcmdWithADoubleAndUnit    *fBetaSetEdgeLengthCmd;
  G4UIcmdWithABool             *fBetaSetActiveCmd;
  G4UIcmdWithADoubleAndUnit    *fBetaSetDeadLayerCmd;
  G4UIcmdWith3VectorAndUnit    *fAlphaPositionCmd;
  G4UIcmdWith3VectorAndUnit    *fBetaPositionCmd;
  MJGeometryDoubleSolidBlock   *fDoubleBlock;
  G4UIcmdWithAString           *fBoundingMaterialSelectCmd;
};
#endif
