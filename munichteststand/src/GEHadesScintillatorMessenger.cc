#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh" 
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "munichteststand/GEHadesScintillatorMessenger.hh"

// --------------------------------------------------------------------------- 

GEHadesScintillatorMessenger::GEHadesScintillatorMessenger(GEHadesScintillator* /*detector*/) //: fHadesScintillator(detector) 
{

  // directories 

  fDetectorDir =  new G4UIdirectory("/MG/geometry/hadesscintillator/"); 
  fDetectorDir -> SetGuidance("Ge Detector / scintillator sandwich at Hades"); 
  
  // define commands 

}

// --------------------------------------------------------------------------- 

GEHadesScintillatorMessenger::~GEHadesScintillatorMessenger()
{

  // delete directory 

  if (!fDetectorDir) 
    delete fDetectorDir;

  // delete commands 
  
}

// --------------------------------------------------------------------------- 

void GEHadesScintillatorMessenger::SetNewValue(G4UIcommand* /*cmd*/, G4String /*newvalue*/)
{

  // set values 
  
}

// --------------------------------------------------------------------------- 
