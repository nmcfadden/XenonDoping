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
 * $Id: MGGeneratorAmBe.hh,v 1.6 2007-03-16 22:01:25 alexis3 Exp $
 *      
 * CLASS DECLARATION:  MGGeneratorAmBe.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 * Defining class for the neutron AmBe generator.
 *
 *---------------------------------------------------------------------------//
 *
 * SPECIAL NOTES:
 *
 * The AmBe neutron spectrum used comes from Figure 5 of Marsh,
 * Thomas, and Burke, "High resolution measurements of neutron
 * energy spectra from Am-Be and Am-B neutron sources", Nucl. Inst.
 * and Meth. A 366 (1995) 340-348. Gamma energies are taken
 * from http://www.nndc.bnl.gov/nudat2/getdataset.jsp?nucleus=12C
 * Choice of gammas depending on neutron energy is taken from Figure 3 of 
 * Geiger et al, Nucl. Inst. and Meth. 131 (1975) p315.
 *
 * The spectrum in the paper is normalized, so it is a PDF. The PDF was
 * digitized, and a CDF created from those numbers. It is the CDF that is
 * defined in the arrays in this file.
 *
 *---------------------------------------------------------------------------//
 *
 * AUTHOR: kareem
 * CONTACT: 
 * FIRST SUBMISSION: Tue Sep  6 07:48:24 PDT 2005
 * 
 * REVISION:
 * 
 * 09-05-2005 Initial submission, Kareem
 * 11-24-2005 Using CDF values from the article referenced in the SPECIAL
 *            NOTES above, Kareem
 * 01-16-2006 Use Fig 5 instead of Fig 4 of the paper to make fit to
 *            clover data better
 * 03-27-2006 Added correlations between neutron energy and gamma 
 *            release, A. Schubert
 * 01-29-2007 Fix angular distribution of vertices, gamma energies, add 9.6
 *            MeV cascade, and general code clean-up. J. Detwiler
 * 03-16-2007 Add gamma smearing, A. Schubert
 */

#ifndef _MGGENERATORAMBE_HH
#define _MGGENERATORAMBE_HH

#include "generators/MGVGenerator.hh"
#include "G4ParticleGun.hh"
#include "TRandom3.h"

class MGGeneratorAmBe : public MGVGenerator
{
  public:
    MGGeneratorAmBe();
    ~MGGeneratorAmBe();

    void GeneratePrimaryVertex(G4Event *event);
    void SetParticlePosition(G4ThreeVector pos) { fPosition = pos; }

  protected:
    G4ParticleGun* fParticleGun;
    G4ThreeVector fPosition;
    G4int fEventNumber;
    TRandom* fRandEngine;
  
    //  The neutron energy CDF 
    enum EAmBeConstants { kCDFSize = 1319 };
    G4double fSpectrumCDFValue[kCDFSize], fSpectrumCDFEnergy_MeV[kCDFSize];
};
#endif

