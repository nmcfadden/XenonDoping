#ifndef _MGLGNDOpticalMaterialProperties_HH
#define _MGLGNDOpticalMaterialProperties_HH

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "TGraph.h"
#include "TFile.h"

class MGLGNDOpticalMaterialProperties{
  public:
    MGLGNDOpticalMaterialProperties();
    virtual ~MGLGNDOpticalMaterialProperties();
   
    void ConstructionOpticalProperties();
    void RegisterArgonOpticalProperties();
    G4double LArRefIndex(const G4double lambda);
    G4double LArEpsilon(const G4double lambda);
    G4double LArRayLength(const G4double lambda,const
           G4double temp);
    G4double ArScintillationSpectrum(const G4double kk);
    inline G4Material* Get_ArgonLiquid(){return fArgonLiquid;}
    
    void Register_TPB_Properties();
    void InitializeTPBSpectra();
    G4double TPBEmissionSpectrum(G4double energy);
    
    void Register_Fiber_Properties();
    void InitializeFiberSpectra();
    inline G4Material* Get_Fiber(){return fFiber_material;}

    void Register_Fiber_Cladding_Properties();
    inline G4Material* Get_Fibber_CladdingInner(){return fFiber_claddingInner_material;}
    inline G4Material* Get_Fibber_CladdingOuter(){return fFiber_claddingOuter_material;}

    void Register_Nylon_Properties();
    inline G4Material* Get_Nylon(){return fNylon;}

    void Register_Copper_Properties();
    inline G4Material* Get_CopperEF(){return fCopperEF;}

    void Register_Germanium_Properties();
    void Register_Silicon_Properties();
    void Register_Teflon_Properties();
    void Register_Silica_Properties();
    void Register_VM2000();
    void Register_StainlessSteel();
    void Register_Acrylic();

    TGraph* ReadSpectrumFromFile(G4String filename);

  private:
    static const G4double LambdaE;
    G4Material *fArgonLiquid;
    G4Material *fTPBFiber;
    G4Material *fTPBNylon;
    G4Material *fFiber_material;
    G4Material *fFiber_claddingInner_material;
    G4Material *fFiber_claddingOuter_material;
    G4Material *fNylon;
    G4Material *fCopperEF;
    TGraph *fTPBspec;
    TGraph *fFibersAbsorptionSpec;
    TGraph *fFibersEmissionSpec;
    bool fSuccessfulInitialization;

    // define useful stuff
    static const G4int NUMENTRIES;// = 69;
    static const G4int NUMENTRIES_1;// = 5;
    static const G4int NUMENTRIES_2;// = 500;
    static const G4int NUMENTRIES_3;// = 29;
    G4double* ph_energies;//[NUMENTRIES_2];

};
#endif
