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
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *
 * CLASS DECLARATION:  LGND_JSONDet.cc
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION:
 *
 */
// Begin description of class here
/**
 *
 *Creates a detector from parameters read in MGCrystalData
 *
 *
 */
// End class description
//
/**
 * SPECIAL NOTES:
 *
 */
//
// --------------------------------------------------------------------------//
/**
 * AUTHOR: Kris Vorren
 * CONTACT: krisvorren@unc.edu
 * FIRST SUBMISSION: Oct 29, 2013
 *
 * REVISION:
 * 5-26-2017, Adapted for legend geometry, M. Green
 * 10-29-2013, Created, K. Vorren
 */
// --------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4UIcommand.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4ThreeVector.hh"
#include "G4Polycone.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "legendio/LGND_JSONReader.hh"
#include "legendgeometry/LGND_JSONDet.hh"
#include "legendgeometry/LGND_Part.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


LGND_JSONDet::LGND_JSONDet(G4String partName, G4String serialNumber, G4String jsonFile) :
  LGND_Detector(partName, serialNumber, "GenericDetector", "Germanium-Enr")
{
  ReadJSONFile(jsonFile);
}

LGND_JSONDet::LGND_JSONDet(const LGND_JSONDet & rhs) :
  LGND_Detector(rhs), fDetParameters(rhs.fDetParameters)
{;}

LGND_JSONDet::~LGND_JSONDet()
{;}

void LGND_JSONDet::ReadJSONFile(G4String jsonFile)
{
  if(fDetParameters.size())
    fDetParameters.clear();
  LGND_JSONReader* reader = new LGND_JSONReader(jsonFile);
  vector<double> r_i = reader->ReadDetectorRis(fSerialNumber);
  vector<double> r_o = reader->ReadDetectorRos(fSerialNumber);
  vector<double> z = reader->ReadDetectorZs(fSerialNumber);
  delete reader;

  if(r_i.size()&&r_o.size()&&z.size()&&r_i.size()==r_o.size()&&r_i.size()==z.size())
    {
      G4int numPlanes = r_o.size();
      G4double maxZ = 0.0;
      G4double minZ = 0.0;
      G4double maxR = 0.0;
      for(G4int i = 0; i < numPlanes; i++)
	{
	  G4double tempZ = z.at(i);
	  G4double tempR = r_o.at(i);
	  maxR = (tempR > maxR) ? tempR : maxR;
	  maxZ = (tempZ > maxZ) ? tempZ : maxZ; //return the max of maxZ and temp
	  minZ = (tempZ < minZ) ? tempZ : minZ; //return the min of minZ and temp
	  G4ThreeVector tempVec = G4ThreeVector(r_i.at(i), r_o.at(i), z.at(i));
	  fDetParameters.push_back(tempVec);
	}
      fCrystalDiameter = 2*maxR;

      if((maxZ - minZ) > 0)
	{
	  fCrystalHeight = maxZ - minZ;
	  MGLog(debugging) << "Crystal height set to " << fCrystalHeight << "." << endlog;
	}
      else
	MGLog(error) << "Error! Non-positive crystal height!" << endlog;
    }
  else
    {
      fDetParameters.clear();
      fDetParameters.push_back(G4ThreeVector(0, 77.0/2*mm, 33.0/2*mm));
      fDetParameters.push_back(G4ThreeVector(0, 77.0/2*mm, -33.0/2*mm));
      fCrystalHeight = 33*mm;
    }
}

G4LogicalVolume* LGND_JSONDet::BuildLogicalVolume()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fDrawingNumber + "_" + fPartMaterial;

  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4int numPlanes = fDetParameters.size();
    G4double *r_i = new G4double[numPlanes];
    G4double *r_o = new G4double[numPlanes];
    G4double *z = new G4double[numPlanes];

    for(G4int i = 0; i<numPlanes; i++){
      r_i[i] = fDetParameters.at(i).getX();
      r_o[i] = fDetParameters.at(i).getY();
      z[i] = -fDetParameters.at(i).getZ();
      }

    G4Polycone *crystal = new G4Polycone("crystal", 0, 2.*pi, numPlanes, z, r_i, r_o);

    G4VisAttributes* geVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0,1.0)); //blue color
    geVisAtt->SetForceWireframe( false );
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    pVol = new G4LogicalVolume(crystal, material, logicalName);
    pVol->SetVisAttributes(geVisAtt);
    MGLog(debugging) << "Created JSON Detector Logical" << endlog;

    delete[] r_i;
    delete[] r_o;
    delete[] z;
  }
  else  MGLog(debugging) << "Using pre-existing JSON Detector Logical" << endlog;
  return pVol;
}
