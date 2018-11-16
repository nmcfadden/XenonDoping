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
/*
 11-02-2004 First Implementation, Luciano
 11-12-2004 added materials: enriched Ge76,
                 liquid N2, copper, water
 12-3-2004  new materials and naming conventions, C.Tomei
 12-16-2004 added Beryllium and Lithium, Luciano
            (used to by-pass a stupid tracking in bug)
 02-15-2005 added Optical Properties of water, Luciano
 03-16-2005 changed ssteel material composition and density,
            added Cr, Mn and Ni elements, dkollar
				(material name should be changed to StainlessSteel)
 04-07-2005 added Quartz, Luciano
 07-11-2005 added nickel metal and gold metal, Kevin 
 09-14-2005 added polyethylene, Kevin 
 06-09-2006 fixed typo with the density of gaseous nitrogen, Luciano
 10-12-2006 added method to define materials on-the-fly from external files, Luciano
 12-07-2006 added tungsten, Manuela
 02-22-2007 added materials: Brass, ProportionalGas, N2Gas, GeLi,Acrylic, PVC, HD100, Monel,PE, and element Zinc,   Dusan
 03-15-2007 added silver, Manuela
 04-03-2007 added materials: Bronze, GeB, and element Tin,   Hardy
 06-27-2007 added hydrogen material for testing purposes, Luciano
 07-02-2007 added gaseous argon, Luciano
 10-16-2007 added PET, Daniel
 10-18-2007 added Polyimid aka. Kapton, Daniel
 11-28-2007 added Polycarbonate(makrolon), Jens
 12-07-2007 added Torlon, Jens
 04-29-2008 added LNGS Rock, Luciano
 07-02-2008 modified density of enriched Germanium, according to data from the existing detectors at LNGS 
            (density about 5.5 g/cm3, while it is 5.32 g/cm3 for natural metallic Ge).
 07-10-2008 modified again density of enriched Germanium, according to calculations performed by Jens
            (density = 1.04236*density_nat = 5.54 g/cm3)
 17-02-2009 added Tantalum, Francis
 14-05-2009 added Erbium and Krypton elements, Marc
 22-07-2009 added borated polyethylene of 5% and 10% Boron content, Alan Poon
 21-01-2010 added lithiated polyethylene (7.5% by weight), Alan Poon
 08-03-2010 added PPO, DMP, generic epoxyresin, CU5, polypropylene, Matthias Laubenstein
 28-06-2012 added elAm 241, AmO2, Katharina von Sturm
 09-04-2015 added effective gold density of gerda calibration source activity volume
 23-04-2016 changed density of natural and enriched Ge as operated at cryogenic temperatures, Janina
            (as proposed in GSTR-13-008)
*/

#include "materials/MGGerdaLocalMaterialTable.hh"
#include "io/MGLogger.hh"

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"

using namespace CLHEP;

MGGerdaLocalMaterialTable::MGGerdaLocalMaterialTable()
{;}    

MGGerdaLocalMaterialTable::~MGGerdaLocalMaterialTable()
{;}

void MGGerdaLocalMaterialTable::CreateMaterials()
{
  //here one should define the material he/she wants to use 
  //in the geometry, if not read from the DB
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density,fractionmass,abundance;
  G4int iz,in;
  G4String name,symbol;
  G4int ncomponents;
  G4int natoms;
  G4double temperature, pressure;
  G4State  state;

  G4Element* elH = new G4Element
    (name="Hydrogen",symbol="H",z=1.,a=1.00794*g/mole);
  G4Element* elLi = new G4Element
    (name="Lithium",symbol="Li",z=3.,a=6.941*g/mole);
  G4Element* elBe = new G4Element
    (name="Beryllium",symbol="Be",z=4.,a=9.012*g/mole);
  G4Element* elB = new G4Element
    (name="Boron",symbol="B",z=5.,a=10.811*g/mole);
  G4Element* elC = new G4Element
    (name="Carbon",symbol="C",z=6.,a=12.011*g/mole);
  G4Element* elN = new G4Element 
    (name="Nitrogen",symbol="N",z=7.,a=14.01*g/mole);
  G4Element* elO = new G4Element
    (name="Oxygen",symbol="O",z=8.,a=16.00*g/mole);
  G4Element* elF = new G4Element
    (name="Fluorine",symbol="F",z=9.,a=19.00*g/mole);
  /*   G4Element* elNa = */ new G4Element
     (name="Natrium",symbol="Na",z=11.,a=22.990*g/mole);
  G4Element* elAl = new G4Element
    (name="Aluminum" ,symbol="Al",z=13.,a=26.98*g/mole);
  G4Element* elSi = new G4Element
    (name="Silicon",symbol="Si",z=14.,a=28.09*g/mole);
  /*   G4Element* elS = */new G4Element
     (name="Sulfur",symbol="S",z=16.,a=32.066*g/mole);
  G4Element* elCl = new G4Element
     (name="Chlorine",symbol="Cl",z=17.,a=35.45*g/mole);
  G4Element* elAr = new G4Element 
    (name="Argon",symbol="Ar",z=18.,a=39.95*g/mole);
  G4Element* elK = new G4Element
    (name="Potassium",symbol="K",z=19.,a=39.098*g/mole);
  G4Element* elCa = new G4Element
    (name="Calcium",symbol="Ca",z=20.,a=40.078*g/mole);
  G4Element* elTi = new G4Element
    (name="Titanium",symbol="Ti",z=22.,a=47.867*g/mole);
  G4Element* elCr = new G4Element
    (name="Chromium",symbol="Cr",z=24.,a=51.9961*g/mole);
  G4Element* elMn = new G4Element
    (name="Manganese",symbol="Mn",z=25.,a=54.93805*g/mole);
  G4Element* elFe = new G4Element
    (name="Iron",symbol="Fe",z=26.,a=55.845*g/mole);
  G4Element* elIn = new G4Element
    (name="Indium",symbol="In",z=49.,a=114.82*g/mole);
  G4Element* elCo = new G4Element
     (name="Cobalt",symbol="Co",z=27.,a=58.9332*g/mole);
  G4Element* elNi = new G4Element
    (name="Nickel",symbol="Ni",z=28.,a=58.6934*g/mole);
  G4Element* elNb = new G4Element
    (name="Niobium",symbol="Nb",z=41.,a=92.91*g/mole);
  G4Element* elCu = new G4Element
    (name="Copper",symbol="Cu",z=29.,a=63.55*g/mole);
  /*
  G4Element* elKr = new G4Element
    (name="Krypton",symbol="Kr",z=36.,a=83.798*g/mole);
  G4Element* elEr = new G4Element
    (name="Erbium",symbol="Er",z=68.,a=167.26*g/mole);
  */
  G4Element* elPb = new G4Element
    (name="Lead",symbol="Pb",z=82.,a=207.2*g/mole);
  G4Element* elAu = new G4Element
    (name="Gold", symbol="Au", z=79, a=196.97*g/mole); 
  G4Element* elAg = new G4Element
    (name="Silver", symbol="Ag", z=47, a=107.87*g/mole); 
  G4Element* elW = new G4Element
    (name="Tungsten", symbol="W", z=74, a=182.30*g/mole); 
  G4Element* elZn = new G4Element
    (name="Zinc", symbol="Zn", z=30.,a=65.38*g/mole);
  G4Element* elSn = new G4Element
    (name="Tin", symbol="Sn", z=50.,a=118.71*g/mole);
  G4Element* elTa = new G4Element
    (name="Tantalum", symbol="Ta", z= 73, a=180.94*g/mole);
  G4Element* elTe = new G4Element
    (name="Tellurium",symbol="Te",z=52,a=127.60*g/mole);
  G4Element* elAm = new G4Element
    (name="Americium",symbol="Am",z=95,a=241.06*g/mole);
    



  G4Isotope* Ga69 = new G4Isotope(name="Gallium69",iz= 31,in=69,a=69*g/mole);
  G4Isotope* Ga71 = new G4Isotope(name="Gallium71",iz=31,in=71,a=71*g/mole);
  G4Element* elGa = new G4Element
        (name="Gallium",symbol="Ga",ncomponents=2);
  elGa->AddIsotope(Ga69, abundance=0.61);
  elGa->AddIsotope(Ga71, abundance=0.39);

  G4Isotope* Ge70 = new G4Isotope(name="Ge70",  32, 70, 69.92*g/mole);
  G4Isotope* Ge72 = new G4Isotope(name="Ge72",  32, 72, 71.92*g/mole);
  G4Isotope* Ge73 = new G4Isotope(name="Ge73",  32, 73, 73.0*g/mole);
  G4Isotope* Ge74 = new G4Isotope(name="Ge74",  32, 74, 74.0*g/mole);
  G4Isotope* Ge76 = new G4Isotope(name="Ge76",  32, 76, 76.0*g/mole);

  a = 75.71*g/mole;
  G4int nIsotopes=5;
  G4Element* elGeNat=new G4Element(name="naturalGermanium",
					    symbol="GeNat",nIsotopes);
  elGeNat->AddIsotope(Ge70,abundance= 20.9*perCent);
  elGeNat->AddIsotope(Ge72,abundance= 27.5*perCent);
  elGeNat->AddIsotope(Ge73,abundance= 7.7*perCent);
  elGeNat->AddIsotope(Ge74,abundance= 36.3*perCent);
  elGeNat->AddIsotope(Ge76,abundance= 7.6*perCent);

  G4Element* elGeEnr = new G4Element(name="enrichedGermanium", 
				      symbol="GeEnr",nIsotopes);
  elGeEnr->AddIsotope(Ge70,abundance= 0.0*perCent);
  elGeEnr->AddIsotope(Ge72,abundance= 0.1*perCent);
  elGeEnr->AddIsotope(Ge73,abundance= 0.2*perCent);
  elGeEnr->AddIsotope(Ge74,abundance= 13.1*perCent);
  elGeEnr->AddIsotope(Ge76,abundance= 86.6*perCent);

  

  
  
  
  //-- then materials
 
  
  // vacuum
  a= 14.000674*g/mole;
  density = 1.E-10*g/cm3;
  /*G4Material* vacuum = */new G4Material(name="Vacuum",z=7.,a,density);

  // air
  density=1.290*mg/cm3;
  G4Material *air = new G4Material(name="Air",density,ncomponents=2,
                                    kStateGas,300.00*kelvin,1.0*atmosphere);
  air->AddElement(elN,fractionmass=0.8);
  air->AddElement(elO,fractionmass=0.2);

  //concrete
  G4Material* concrete = new G4Material
    (name="Concrete",density=2.3*g/cm3,ncomponents=6);
  concrete->AddElement(elSi,fractionmass= 0.227915);
  concrete->AddElement(elO,fractionmass= 0.60541);
  concrete->AddElement(elH,fractionmass= 0.09972);
  concrete->AddElement(elCa,fractionmass= 0.04986);
  concrete->AddElement(elAl,fractionmass= 0.014245);
  concrete->AddElement(elFe,fractionmass= 0.00285);
 
  //stainless steel
  G4Material* ssteel = new G4Material
    (name="Steel",density=7.9*g/cm3,ncomponents=5);
  ssteel->AddElement(elSi,fractionmass= 0.01);
  ssteel->AddElement(elCr,fractionmass= 0.20);
  ssteel->AddElement(elMn,fractionmass= 0.02);
  ssteel->AddElement(elFe,fractionmass= 0.67);
  ssteel->AddElement(elNi,fractionmass= 0.10);

  //old ship steel
  G4Material* shipsteel = new G4Material
    (name="ShipSteel",density=7.9*g/cm3,ncomponents=3);
  shipsteel->AddElement(elCr,fractionmass= 0.02);
  shipsteel->AddElement(elFe,fractionmass= 0.94);
  shipsteel->AddElement(elNi,fractionmass= 0.04);
  
    //Kovar
  G4Material* kovar = new G4Material
    (name="Kovar",density=8.0*g/cm3,ncomponents=6);
  kovar->AddElement(elC,fractionmass= 0.0001);
  kovar->AddElement(elSi,fractionmass= 0.002);
  kovar->AddElement(elMn,fractionmass= 0.003);
  kovar->AddElement(elCo,fractionmass= 0.17);
  kovar->AddElement(elNi,fractionmass= 0.29);
  kovar->AddElement(elFe,fractionmass= 0.5349);
  
  //  VM2000
  G4Material* VM2000 = new G4Material(name="VM2000", density=0.9*g/cm3, ncomponents=2);
  VM2000->AddElement(elC, 2);
  VM2000->AddElement(elH, 4);
  
  // liquid N2
  density=0.808*g/cm3;
  ncomponents=1;
  natoms=1;
  state=kStateLiquid;
  temperature=77.0*kelvin;
  pressure=1.0*bar;
  name="LiquidNitrogen";
  G4Material* liquidnitrogen = new G4Material(name,density,ncomponents,
					      state,temperature,pressure);
  liquidnitrogen->AddElement(elN,natoms);

  // gaseous N2 (pressure) 
  density=4.614*mg/cm3; 
  ncomponents=1;
  natoms=1;
  state=kStateGas;
  temperature=77.0*kelvin;
  pressure=1.0*bar;
  name="GaseousNitrogen";
  G4Material* gaseousnitrogen = new G4Material(name,density,ncomponents,
					       state,temperature,pressure);
  gaseousnitrogen->AddElement(elN,natoms);

  // liquid Ar
  density=1.390*g/cm3;
  ncomponents=1;
  natoms=1;
  state=kStateLiquid;
  temperature=87.0*kelvin;
  pressure=1.0*bar;
  name="LiquidArgon";
  G4Material* liquidargon = new G4Material(name,density,ncomponents,
					      state,temperature,pressure);
  liquidargon->AddElement(elAr,natoms);

  //gaseous Ar
  density = 5.4*mg/cm3;
  ncomponents = 1;
  natoms = 1;
  state=kStateGas;
  temperature=87.0*kelvin;
  pressure=1.0*bar;
  name="GaseousArgon";
  G4Material* gaseousargon = new G4Material(name,density,ncomponents,
					      state,temperature,pressure);
  gaseousargon->AddElement(elAr,natoms);

  // natural germanium
  density = 5.34*g/cm3;//cryogenic temperature (at room temperature: 5.32*g/cm3)
  G4Material* natGe = new G4Material(name="NaturalGe", density, 1);
  natGe->AddElement(elGeNat,natoms=1);

  // enriched germanium
  density = 5.56*g/cm3;//cryogenic temperature (at room temperature: 5.54*g/cm3)
  G4Material* enrGe = new G4Material(name="EnrichedGe", density, 1);
  enrGe->AddElement(elGeEnr,natoms=1);
  
  // copper
  G4Material* metalCu = new G4Material
    (name="MetalCopper",density=8.960*g/cm3,ncomponents=1);
  metalCu->AddElement(elCu,natoms=1);
  
  // Niobium
  G4Material* metalNb = new G4Material
    (name="MetalNiobium",density=8.57*g/cm3,ncomponents=1);
  metalNb->AddElement(elNb,natoms=1);
  
  //water
  G4Material* water = new G4Material
    (name="Water", density=1.00*g/cm3, ncomponents=2);
  water->AddElement(elH,natoms=2);
  water->AddElement(elO,natoms=1);
  
  // lead
  G4Material* metalPb = new G4Material
    (name="MetalLead", density=11.340*g/cm3, ncomponents=1);
  metalPb->AddElement(elPb,natoms=1);

 // tungsten
  G4Material* metalW = new G4Material
    (name="MetalTungsten", density=19.250*g/cm3, ncomponents=1);
  metalW->AddElement(elW,natoms=1);

  // tantalum
  G4Material* metalTa = new G4Material
    (name="MetalTantalum", density=16.69*g/cm3, ncomponents=1);
  metalTa->AddElement(elTa,natoms=1);

  //aluminium
  G4Material* metalAl = new G4Material
    (name="MetalAluminium", density=2.702*g/cm3, ncomponents=1);
  metalAl->AddElement(elAl,natoms=1);

  //iron
  G4Material* metalFe = new G4Material
    (name="MetalIron", density=7.874*g/cm3, ncomponents=1);
  metalFe->AddElement(elFe,natoms=1);

  //silicon
  G4Material* metalSi = new G4Material
    (name="MetalSilicon", density=2.330*g/cm3, ncomponents=1);
  metalSi->AddElement(elSi,natoms=1);

  // teflon (PTFE)
  G4Material* teflon = new G4Material
    (name="Teflon", density=2.165*g/cm3, ncomponents=2);
  teflon->AddElement(elF,fractionmass= 0.76);
  teflon->AddElement(elC,fractionmass= 0.24);
  
  // tefzel (ETFE)
  G4Material* tefzel = new G4Material
    (name="Tefzel", density=1.70*g/cm3, ncomponents=3);
  tefzel->AddElement(elH,fractionmass= 0.031);
  tefzel->AddElement(elF,fractionmass= 0.594);
  tefzel->AddElement(elC,fractionmass= 0.375);

  // vespel
  G4Material* vespel = new G4Material
    (name="Vespel", density=1.430*g/cm3, ncomponents=2);
  vespel->AddElement(elH,fractionmass= 6.7*perCent);
  vespel->AddElement(elN,fractionmass= 93.3*perCent);

  // nylon
  density = 1.15*g/cm3;  //  http://www.boedeker.com
  G4Material* nylon = new G4Material
    (name="Nylon", density=1.15*g/cm3, ncomponents=4);
  nylon->AddElement(elH,fractionmass= 10.0*perCent);
  nylon->AddElement(elN,fractionmass= 10.0*perCent);
  nylon->AddElement(elO,fractionmass= 15.0*perCent);
  nylon->AddElement(elC,fractionmass= 65.0*perCent);

  // Parylene C (white powder)
  density = 0.590*g/cm3;  
  G4Material* parylenec = new G4Material
    (name="ParyleneC", density=0.590*g/cm3, ncomponents=3);
  parylenec->AddElement(elH,fractionmass= 5.09*perCent);
  parylenec->AddElement(elCl,fractionmass= 25.58*perCent);
  parylenec->AddElement(elC,fractionmass= 69.33*perCent);

//boron-ethanolamine
  G4Material* borEtha = new G4Material
    (name="BoronEthanolamine", density=1.435*g/cm3, ncomponents=5);
  borEtha->AddElement(elH,fractionmass= 12.4*perCent);
  borEtha->AddElement(elC,fractionmass= 31.3*perCent);
  borEtha->AddElement(elB,fractionmass= 11.8*perCent);
  borEtha->AddElement(elN,fractionmass= 18.3*perCent);
  borEtha->AddElement(elO,fractionmass= 26.1*perCent);

  //boron-polyethylen
  G4Material* borPoly = new G4Material
    (name="BoronPolyethylene", density=0.942*g/cm3, ncomponents=3);
  borPoly->AddElement(elH,fractionmass= 11.5*perCent);
  borPoly->AddElement(elC,fractionmass= 73.5*perCent);
  borPoly->AddElement(elB,fractionmass= 15.0*perCent);
	
  //boron-polyethylene 5%-B by mass
  G4Material* borPoly5 = new G4Material
   (name="BoronPolyethylene5", density=0.94*g/cm3, ncomponents=3);
  borPoly5->AddElement(elH,fractionmass= 13.5*perCent);
  borPoly5->AddElement(elC,fractionmass= 81.5*perCent);
  borPoly5->AddElement(elB,fractionmass= 5.0*perCent);	

  //boron-polyethylene 10%-B by mass
  G4Material* borPoly10 = new G4Material
	(name="BoronPolyethylene10", density=0.94*g/cm3, ncomponents=3);
  borPoly10->AddElement(elH,fractionmass= 12.8*perCent);
  borPoly10->AddElement(elC,fractionmass= 77.2*perCent);
  borPoly10->AddElement(elB,fractionmass= 10.0*perCent);	

  //lithium-polyethylene 7.5%-Li by mass
  G4Material* LiPoly = new G4Material
  (name="LithiumPolyethylene", density=1.06*g/cm3, ncomponents=3);
  LiPoly->AddElement(elH,fractionmass= 13.2*perCent);
  LiPoly->AddElement(elC,fractionmass= 79.3*perCent);
  LiPoly->AddElement(elLi,fractionmass= 7.5*perCent);	
	
  // styropor
  G4Material* styropor = new G4Material
    (name="Styropor", density=20*kg/m3, ncomponents=2);
  styropor->AddElement(elH,fractionmass= 12.8*perCent);
  styropor->AddElement(elC,fractionmass= 87.2*perCent);

  //plastic scintillator
  G4Material* plasticScint = new G4Material
    (name="PlasticScintillator", density= 1.032*g/cm3, ncomponents=2);
  plasticScint->AddElement(elC,natoms=9);
  plasticScint->AddElement(elH,natoms=10);

  // polyethylene 
 
  G4Material* polyethylene = new G4Material
    (name = "Polyethylene", density = 0.92*g/cm3, ncomponents = 2); 
  polyethylene -> AddElement(elC, natoms = 2); 
  polyethylene -> AddElement(elH, natoms = 4); 

  // polypropylene 
 
  G4Material* polypropylene = new G4Material
    (name = "Polypropylene", density = 0.90*g/cm3, ncomponents = 2); 
  polypropylene -> AddElement(elC, natoms = 3); 
  polypropylene -> AddElement(elH, natoms = 6); 

  // polyethylen terephthalat  (PET, Mylar)
 
  G4Material* PET = new G4Material
    (name = "PET", density = 1370 * kg/m3, ncomponents = 3); 
  PET -> AddElement(elC, natoms = 10); 
  PET -> AddElement(elH, natoms = 8); 
  PET -> AddElement(elO, natoms = 4); 

  // poly(4,4'-oxydiphenylene-pyromellitimide) (Kapton)
 
  G4Material* kapton = new G4Material
    (name = "kapton", density = 1530 * kg/m3, ncomponents = 4); 
  kapton -> AddElement(elC, natoms = 20); 
  kapton -> AddElement(elH, natoms = 9); 
  kapton -> AddElement(elO, natoms = 2); 
  kapton -> AddElement(elN, natoms = 1);

  // epoxy resin (generic)
 
  G4Material* epoxy = new G4Material
    (name = "Epoxy", density = 1129 * kg/m3, ncomponents = 4); 
  epoxy -> AddElement(elC, natoms = 41); 
  epoxy -> AddElement(elH, natoms = 49); 
  epoxy -> AddElement(elO, natoms = 7); 
  epoxy -> AddElement(elN, natoms = 3);

  // modified epoxy resin (generic)
 
  G4Material* epoxy_mod = new G4Material
    (name = "Epoxy_mod", density = 711 * kg/m3, ncomponents = 4); 
  epoxy_mod -> AddElement(elC, natoms = 41); 
  epoxy_mod -> AddElement(elH, natoms = 49); 
  epoxy_mod -> AddElement(elO, natoms = 7); 
  epoxy_mod -> AddElement(elN, natoms = 3);

  // 2,5 - diphenyl oxazole (PPO)
 
  G4Material* PPO = new G4Material
    (name = "PPO", density = 1421 * kg/m3, ncomponents = 4); 
  PPO -> AddElement(elC, natoms = 15); 
  PPO -> AddElement(elH, natoms = 11); 
  PPO -> AddElement(elO, natoms = 1); 
  PPO -> AddElement(elN, natoms = 1);

  // dimethyl benzene-1,2-dicarboxylate (Dimethyl phtalate, DMP)
 
  G4Material* dmp = new G4Material
    (name = "DMP", density = 1073 * kg/m3, ncomponents = 3); 
  dmp->AddElement(elH, natoms = 10);
  dmp->AddElement(elC, natoms = 10);
  dmp->AddElement(elO, natoms = 4);

  G4Material* makrolon = new G4Material
    (name = "polycarbonate", density = 1200 * kg/m3, ncomponents = 3); 
  makrolon->AddElement(elH,fractionmass= 5.5491*perCent);
  makrolon->AddElement(elC,fractionmass= 75.5751*perCent);
  makrolon->AddElement(elO,fractionmass= 18.8758*perCent);

  // Polyamid-imide
 
  G4Material* torlon = new G4Material
    (name = "torlon", density = 1.420 * g/cm3, ncomponents = 4);
  torlon->AddElement(elH , natoms=4 );
  torlon->AddElement(elC , natoms=8 );
  torlon->AddElement(elN , natoms=2 );
  torlon->AddElement(elO , natoms=3 );

  // Korrodin CU5 bolts (CU5 former CuNi60, Ni (1.0 - 1.6 %), Si (0.4 - 0.7 %), rest Cu; put apparent density) 

  G4Material* CU5 = new G4Material
    (name="CU5",density=3672*kg/m3,ncomponents=3);
  CU5->AddElement(elSi,fractionmass= 0.55*perCent);
  CU5->AddElement(elNi,fractionmass= 1.3*perCent);
  CU5->AddElement(elCu,fractionmass= 98.15*perCent);
  
  // Sapphire disks for GeDSG 

  G4Material* sapphire = new G4Material
    (name="Sapphire",density=3972*kg/m3,ncomponents=2);
  sapphire->AddElement(elAl , natoms=2 );
  sapphire->AddElement(elO , natoms=3 );
  
  //metallic beryllium 

  G4Material* metalBe = new G4Material
    (name="MetalBeryllium",density=1.848*g/cm3,ncomponents=1);
  metalBe->AddElement(elBe,natoms=1);
  
  //carbon fibre 

  G4Material* carbonf = new G4Material
    (name="CarbonFibre",density=1.75*g/cm3,ncomponents=1);
  carbonf->AddElement(elC,natoms=1);
  
  //metallic lithium 
  G4Material* metalLi = new G4Material
    (name="MetalLithium",density=0.535*g/cm3,ncomponents=1);
  metalLi->AddElement(elLi,natoms=1);

  //quartz
  G4Material* quartz = new G4Material
    (name="Quartz",density=2.2*g/cm3,ncomponents=2);
  quartz->AddElement(elSi,1);
  quartz->AddElement(elO,2);

  // titanium
  G4Material* metalTi = new G4Material(name = "MetalTitanium", density = 4.51*g/cm3, ncomponents = 1);
  metalTi -> AddElement(elTi, natoms = 1);

  // nickel
  G4Material* metalNi = new G4Material(name = "MetalNickel", density = 8.800*g/cm3, ncomponents = 1);
  metalNi -> AddElement(elNi, natoms = 1);

  // gold 
  G4Material* metalAu = new G4Material(name = "MetalGold", density = 19.320*g/cm3, ncomponents = 1);
  metalAu -> AddElement(elAu, natoms = 1);

  // modify density in order to have the equivalent of two 2x2cm gold foils, with 20 um thickness
  G4Material* Gold_source_mat = new G4Material( "CapsGold", density=4.825*g/cm3, ncomponents = 1);
  Gold_source_mat->AddElement(elAu,1.0);

  // indium 
  G4Material* metalIn = new G4Material(name = "MetalIndium", density = 7.31*g/cm3, ncomponents = 1);
  metalIn -> AddElement(elIn, natoms = 1);

  // silver 
  G4Material* metalAg = new G4Material(name = "MetalSilver", density = 10.49*g/cm3, ncomponents = 1);
  metalAg -> AddElement(elAg, natoms = 1);

  //   PMT photocathode aluminium
  G4Material* CathodeMetalAluminium = new G4Material
      (name="CathodeMetalAluminium", density=2.7*g/cm3, ncomponents=1);
  CathodeMetalAluminium->AddElement(elAl, 1);

  // Brass
  density = 8.5*g/cm3;
  G4Material* Brass = new G4Material(name="Brass", density, ncomponents=3);
  Brass -> AddElement(elCu, fractionmass=0.61);
  Brass -> AddElement(elZn, fractionmass=0.37);
  Brass -> AddElement(elPb, fractionmass=0.02);

  // Proportional Gas
  density = 0.001563*g/cm3;
  G4Material* ProportionalGas = new G4Material(name="ProportionalGas", density,
					       ncomponents=3);
  ProportionalGas -> AddElement(elAr, fractionmass=0.90);
  ProportionalGas -> AddElement(elC, fractionmass=0.0748694);
  ProportionalGas -> AddElement(elH, fractionmass=0.0251306);
  // composition corrected (original : C-> 0.02  H-> 0.08 )

  // Nitrogen Gas
  density = 0.0011467*g/cm3;
  G4Material* N2Gas = new G4Material(name="NitrogenGas", density, 1);
  N2Gas -> AddElement(elN, natoms=2);

  // Germanium/Lithium mixture of 1 per cent assumed (in dead layer)
  density = 5.323*g/cm3;
  G4Material* GeLi = new G4Material(name="Germanium/Lithium", density, ncomponents=2);
  GeLi -> AddElement(elGeNat, fractionmass=0.99);
  GeLi -> AddElement(elLi, fractionmass=0.01);

  // Acrylic
  G4Material* Acrylic = new G4Material
    (name="Acrylic", density= 1.15*g/cm3, ncomponents=3);
  Acrylic->AddElement(elC,natoms=5);
  Acrylic->AddElement(elH,natoms=8);
  Acrylic->AddElement(elO,natoms=2);

  // PolyVinylChlorid
  G4Material* PVC = new G4Material
    (name="PVC", density= 1.38*g/cm3, ncomponents=3);
  PVC->AddElement(elC,natoms=2);
  PVC->AddElement(elH,natoms=3);
  PVC->AddElement(elCl,natoms=1);

  // HD1000 UHMW Polyethylene
  density = 0.93*g/cm3;
  G4Material* HD1000 = new G4Material(name="HD1000", density, ncomponents=2);
  HD1000 -> AddElement(elH, natoms=4);
  HD1000 -> AddElement(elC, natoms=2);

  // MONEL 400
  G4Material* MONEL = new G4Material
    (name="MONEL",density=8.83*g/cm3,ncomponents=5);
  MONEL->AddElement(elNi,fractionmass= 0.64);
  MONEL->AddElement(elCu,fractionmass= 0.31);
  MONEL->AddElement(elMn,fractionmass= 0.02);
  MONEL->AddElement(elFe,fractionmass= 0.025);
  MONEL->AddElement(elSi,fractionmass= 0.005);

  // Polyethylene
  density = 0.945*g/cm3;
  G4Material* PE = new G4Material(name="PE", density, ncomponents=2);
  PE -> AddElement(elH, natoms=4);
  PE -> AddElement(elC, natoms=2);

  // Polystyrene
  density = 1.05*g/cm3;
  G4Material* PS = new G4Material(name="PS", density, ncomponents=2);
  PS -> AddElement(elH, natoms=8);
  PS -> AddElement(elC, natoms=8);

  /* ***Bronze*** mixture of 60% Cu and 40% Sn is assumed */
  density = 8.8*g/cm3;
  G4Material* Bronze = new G4Material(name="Bronze", density, ncomponents=2);
  Bronze->AddElement(elCu, natoms=14);
  Bronze->AddElement(elSn, natoms=5);

  /* *** Germanium/Boron mixture of 1 per cent assumed (in dead layer *** */
  density = 5.18*g/cm3;
  G4Material* GeB = new G4Material(name="Germanium/Boron", density, ncomponents=2);
  GeB -> AddElement(elGeNat, fractionmass=0.99);
  GeB -> AddElement(elB, fractionmass=0.01);

  // gaseous hydrogen
  density=0.0899*mg/cm3;
  G4Material *hydrogen = new G4Material(name="Hydrogen",density,ncomponents=1,
                                    kStateGas,300.00*kelvin,1.0*atmosphere);
  hydrogen->AddElement(elH,natoms=1);

  //LNGS rock from Wulandari et al. Astrop Phys 22 (2004) 313
  density = 2.71*g/cm3;
  G4Material* lngsRock = new G4Material(name="LNGSRock",density,ncomponents=7);
  lngsRock->AddElement(elC,fractionmass=11.88*perCent);
  lngsRock->AddElement(elO,fractionmass=48.92*perCent);
  lngsRock->AddElement(elMn,fractionmass=5.58*perCent);
  lngsRock->AddElement(elAl,fractionmass=1.03*perCent);
  lngsRock->AddElement(elSi,fractionmass=1.27*perCent);
  lngsRock->AddElement(elK,fractionmass=1.03*perCent);
  lngsRock->AddElement(elCa,fractionmass=30.29*perCent);
  
  
  // Americium Oxide AmO2
  density = 11.68*g/cm3;
  G4Material* AmericiumOxide = new G4Material(name="AmericiumOxide",density,ncomponents=2);
  AmericiumOxide->AddElement(elAm,natoms=1);
  AmericiumOxide->AddElement(elO,natoms=2);
  
  
  //Luciano, debugging only
  density = 6.0*g/cm3;
  G4Material* TeO2 = new G4Material(name="TeO2",density,ncomponents=2);
  TeO2->AddElement(elTe,natoms=1);
  TeO2->AddElement(elO,natoms=2);

  density = 6.0*g/cm3;
  G4Material* O2Te = new G4Material(name="O2Te",density,ncomponents=2);
  O2Te->AddElement(elO,natoms=2);
  O2Te->AddElement(elTe,natoms=1);  

  // ********************************************************************
  // Register optical properties of materials
  // ********************************************************************

  //water
  const G4int water_NUMENTRIES = 5;
  G4double water_Energy[water_NUMENTRIES]    = {1.8*eV, 2.0*eV , 3.06*eV, 6.2*eV, 10.5*eV }; // equal to 708, 590, 405 nm
  G4double water_RIND[water_NUMENTRIES]  = { 1.3, 1.3308 , 1.3435, 1.4249, 1.54  };        //refraction index
  G4double water_ABSL[water_NUMENTRIES]  = { 12*m, 12.*m, 12.*m, 12.*m, 12*m};             //absorption length 
  G4MaterialPropertiesTable* water_mt = new G4MaterialPropertiesTable();
  water_mt->AddProperty("RINDEX",        water_Energy, water_RIND,  water_NUMENTRIES);
  water_mt->AddProperty("ABSLENGTH",     water_Energy, water_ABSL,  water_NUMENTRIES);
  water_mt->AddConstProperty("SCINTILLATIONYIELD",0.); 

  water->SetMaterialPropertiesTable(water_mt); //attach the property table to the material

  // PMT photocathode aluminium
  const G4int CathMetal_NUMENTRIES = 5;
  G4double CathodeMetalAluminium_PP[CathMetal_NUMENTRIES]   = {1.8*eV, 2.0*eV , 3.06*eV, 6.2*eV, 10.5*eV };
  G4double CathodeMetalAluminium_RIND[CathMetal_NUMENTRIES] = { 1.74, 1.3, 0.523, 0.119, 0.040 };     // refraction index
  G4double CathodeMetalAluminium_ABSL[CathMetal_NUMENTRIES] = { 1.e-30*m, 1.e-30*m, 1.e-30*m,  1.e-30*m,  1.e-30*m };// attenuation length
  G4MaterialPropertiesTable *CathodeMetalAluminium_mt = new G4MaterialPropertiesTable();
  CathodeMetalAluminium_mt->AddProperty("RINDEX", CathodeMetalAluminium_PP, CathodeMetalAluminium_RIND,CathMetal_NUMENTRIES);
  CathodeMetalAluminium_mt->AddProperty("ABSLENGTH", CathodeMetalAluminium_PP, CathodeMetalAluminium_ABSL, CathMetal_NUMENTRIES);
  CathodeMetalAluminium->SetMaterialPropertiesTable(CathodeMetalAluminium_mt);

}

void MGGerdaLocalMaterialTable::AddNewMaterial(G4String filename)
{
  G4double density;
  G4String name;
  G4int cptNumber;
  
  std::ifstream inputFile(filename);
 
  if (!(inputFile.is_open())) {//not open correctly
    MGLog(error) << "Invalid file name " << filename << endlog;
    return;
  }
  //read the first line
  if (!inputFile.eof())
    {
      inputFile >> name >> density >> cptNumber;
      if (G4Material::GetMaterial(name))
	{
	  MGLog(warning) << "A material called " << name << " already exists!" << endlog;
	  MGLog(warning) << "Nothing happens" << endlog;
	  return;
	}
      G4Material *mymate = new G4Material(name,density*g/cm3,cptNumber);
      G4String elName,symbol;
      G4double z,a;
      G4double fractionmass;
      //now read the composition
      for (G4int k=0;k<cptNumber;k++)
	{
	  if (!inputFile.eof()) 
	    {
	      inputFile >> elName >> symbol >> z >> a >> fractionmass;
	      //Check if the element already exists
	      if (G4Element::GetElement(elName))
		{
		  mymate->AddElement(G4Element::GetElement(elName),fractionmass);
		}
	      else //the material has to be created
		{
		  mymate->AddElement(new G4Element(elName,symbol,z,a*g/mole),fractionmass);
		}
	    }
	  else
	    {
	      MGLog(error) << "Corrupted file: there are less elements than declared!" 
			   << endlog;
	      MGLog(error) << "Material not defined" << endlog;
	      delete mymate;
	      return;
	    }
	}
    }
  else
    {
      MGLog(error) << "Corrupted file " << filename << endlog;
      MGLog(error) << "Nothing happens!" << endlog;
      return;
    }
  inputFile.close();
  MGLog(trace) << "New material " << name << " correctly defined from file " << filename  << endlog;
}
